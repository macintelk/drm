//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#include "kern_nblue.hpp"
#include "kern_gen11.hpp"
#include "DYLDPatches.hpp"
#include "HDMI.hpp"
#include "Firmware.hpp"
#include "kern_patcherplus.hpp"
#include <Headers/kern_api.hpp>
#include <Headers/kern_devinfo.hpp>
#include <IOKit/IOCatalogue.h>

static const char *pathIOAcceleratorFamily2= "/System/Library/Extensions/IOAcceleratorFamily2.kext/Contents/MacOS/IOAcceleratorFamily2";
static const char *pathAGDP = "/System/Library/Extensions/AppleGraphicsControl.kext/Contents/PlugIns/"
							  "AppleGraphicsDevicePolicy.kext/Contents/MacOS/AppleGraphicsDevicePolicy";
static const char *pathBacklight = "/System/Library/Extensions/AppleBacklight.kext/Contents/MacOS/AppleBacklight";
static const char *pathMCCSControl = "/System/Library/Extensions/AppleMCCSControl.kext/Contents/MacOS/AppleMCCSControl";
static const char *pathIOGraphics= "/System/Library/Extensions/IOGraphicsFamily.kext/IOGraphicsFamily";

static KernelPatcher::KextInfo kextAGDP {"com.apple.driver.AppleGraphicsDevicePolicy", &pathAGDP, 1, {true}, {},
	KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextBacklight {"com.apple.driver.AppleBacklight", &pathBacklight, 1, {true}, {},
	KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextMCCSControl {"com.apple.driver.AppleMCCSControl", &pathMCCSControl, 1, {true}, {},
	KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextIOGraphics { "com.apple.iokit.IOGraphicsFamily", &pathIOGraphics, 1, {true}, {},
	KernelPatcher::KextInfo::Unloaded };
static KernelPatcher::KextInfo kextIOAcceleratorFamily2 { "com.apple.iokit.IOAcceleratorFamily2", &pathIOAcceleratorFamily2, 1, {true}, {},
	KernelPatcher::KextInfo::Unloaded };

NBlue *NBlue::callback = nullptr;

static Gen11 gen11;
static DYLDPatches dyldpatches;
static HDMI agfxhda;


void NBlue::init() {
    callback = this;
	
	lilu.onKextLoadForce(&kextAGDP);
	lilu.onKextLoadForce(&kextBacklight);
	lilu.onKextLoadForce(&kextMCCSControl);
	lilu.onKextLoadForce(&kextIOGraphics);
	lilu.onKextLoadForce(&kextIOAcceleratorFamily2);
	
	gen11.init();
	dyldpatches.init();
	
    lilu.onPatcherLoadForce(
        [](void *user, KernelPatcher &patcher) { static_cast<NBlue *>(user)->processPatcher(patcher); }, this);
    lilu.onKextLoadForce(
        nullptr, 0,
        [](void *user, KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
            static_cast<NBlue *>(user)->processKext(patcher, index, address, size);
        },this);
	
}


void NBlue::processPatcher(KernelPatcher &patcher) {
    auto *devInfo = DeviceInfo::create();
    if (devInfo) {
        devInfo->processSwitchOff();
		
        this->iGPU = OSDynamicCast(IOPCIDevice, devInfo->videoBuiltin);
        PANIC_COND(!this->iGPU, "nblue", "videoBuiltin is not IOPCIDevice");
		
		WIOKit::renameDevice(this->iGPU, "IGPU");
		WIOKit::awaitPublishing(this->iGPU);
		
		//this->iGPU->enablePCIPowerManagement(kPCIPMCSPowerStateD0);
		this->iGPU->setBusMasterEnable(true);
		this->iGPU->setMemoryEnable(true);
		//this->iGPU->setIOEnable(true);
		
        static uint8_t builtin[] = {0x00};
		
		/*static uint8_t builtin2[] = {0x00, 0x00, 0x52, 0x8A};
		static uint8_t builtin3[] = {0x52, 0x8A, 0x00, 0x00};
		int ok=0;
		//tgl
		if (ok)
		{
			builtin2[2]=0x49;
			builtin2[3]=0x9A;
			builtin3[0]=0x49;
			builtin3[1]=0x9A;
		}
		
		this->iGPU->setProperty("AAPL,ig-platform-id", builtin2, arrsize(builtin2));
		this->iGPU->setProperty("device-id", builtin3, arrsize(builtin3));
		*/
		this->iGPU->setProperty("built-in", builtin, arrsize(builtin));
		this->iGPU->setProperty("AAPL,slot-name", const_cast<char *>("built-in"), 9);
		this->iGPU->setProperty("hda-gfx", const_cast<char *>("onboard-1"), 10);
		this->iGPU->setProperty("model", const_cast<char *>("Intel Iris Xe Graphics"), 23);
			
		i915b=nullptr;
		nlock=IOSimpleLockAlloc();
        this->deviceId = WIOKit::readPCIConfigValue(this->iGPU, WIOKit::kIOPCIConfigDeviceID);
        this->pciRevision = WIOKit::readPCIConfigValue(NBlue::callback->iGPU, WIOKit::kIOPCIConfigRevisionID);
		
		uint32_t device;
		WIOKit::getOSDataValue(this->iGPU, "device-id", device);
		this->tglid=device==0x9a49;
		this->iclid=device==0x8a52;

		KernelPatcher::routeVirtual(this->iGPU, WIOKit::PCIConfigOffset::ConfigRead16, configRead16, &orgConfigRead16);
		KernelPatcher::routeVirtual(this->iGPU, WIOKit::PCIConfigOffset::ConfigRead32, configRead32, &orgConfigRead32);

        DeviceInfo::deleter(devInfo);
		
    } else {
        SYSLOG("nblue", "Failed to create DeviceInfo");
    }
	
	
	dyldpatches.processPatcher(patcher);
	
	KernelPatcher::RouteRequest request{"__ZN11IOCatalogue10addDriversEP7OSArrayb", wrapAddDrivers,
										this->orgAddDrivers};
	PANIC_COND(!patcher.routeMultipleLong(KernelPatcher::KernelID, &request, 1), "DriverInjector",
			   "Failed to route addDrivers");
	
	
	
}

bool NBlue::wrapAddDrivers(void* const self, OSArray* const array, const bool doNubMatching)
{
	
	OSArray *tdrivers= array;
	bool doNub=doNubMatching;
	int ok=0;
	vnode_t vnode = NULLVP;
	vfs_context_t ctxt = vfs_context_create(nullptr);
	errno_t err = vnode_lookup("/usr/bin/sudo", 0, &vnode, ctxt);
	if (!err) vnode_put(vnode);
	vfs_context_rele(ctxt);
	if (!err) ok=1;
	
	
	if (ok && (NBlue::callback->tglid || NBlue::callback->iclid)) { //not running mac os installer
		
		
		int ok2=0;
			ok=0;
			vnode = NULLVP;
			ctxt = vfs_context_create(nullptr);
			err = vnode_lookup("/Library/Extensions/AppleIntelTGLGraphicsFramebuffer.kext/Contents/MacOS/AppleIntelTGLGraphicsFramebuffer", 0, &vnode, ctxt);
			if (!err) vnode_put(vnode);
			vfs_context_rele(ctxt);
			if (!err) ok=1;
			if (!NBlue::callback->tglid ) ok=0;
			if (ok) ok2=1;
		
	
		int tcap=0;
		if (NBlue::callback->tglid || NBlue::callback->iclid) {

				const auto driversXML = getFWByName(ok ? "Driverf2.xml":"Driverf1.xml");
				auto *dataNull = new char[driversXML.size + 1];
				memcpy(dataNull, driversXML.data, driversXML.size);
				dataNull[driversXML.size] = 0;
				OSString *errStr = nullptr;
				auto *dataUnserialized = OSUnserializeXML(dataNull, driversXML.size + 1, &errStr);
				delete[] dataNull;
				PANIC_COND(!dataUnserialized, "NRed", "Failed to unserialize Drivers.xml: %s",
						   errStr ? errStr->getCStringNoCopy() : "Unspecified");
				auto *drivers = OSDynamicCast(OSDictionary, dataUnserialized);
				PANIC_COND(!drivers, "NRed", "Failed to cast Drivers.xml data");
				auto* Match = OSDynamicCast(OSString, drivers->getObject("IOPCIPrimaryMatch"));
				if (Match->getLength()>0){
					tcap++;
					tdrivers->ensureCapacity(tcap);
					tdrivers->setObject(tcap-1,drivers);
					doNub=true;
				} else ok=0;
				OSSafeReleaseNULL(dataUnserialized);
				IOFree(driversXML.data, driversXML.size);
		}
		if (NBlue::callback->tglid || NBlue::callback->iclid) {
				ok=0;
				int sle=-1;
				vnode = NULLVP;
				ctxt = vfs_context_create(nullptr);
				err = vnode_lookup("/System/Library/Extensions/AppleIntelTGLGraphics.kext/Contents/MacOS/AppleIntelTGLGraphics", 0, &vnode, ctxt);
				if (!err) vnode_put(vnode);
				vfs_context_rele(ctxt);
				if (!err) ok=1;
				if (!err) sle=1;
			
					if (!ok) {
						vnode = NULLVP;
						ctxt = vfs_context_create(nullptr);
						err = vnode_lookup("/Library/Extensions/AppleIntelTGLGraphics.kext/Contents/MacOS/AppleIntelTGLGraphics", 0, &vnode, ctxt);
					   if (!err) vnode_put(vnode);
					   vfs_context_rele(ctxt);
					   if (!err) ok=1;
						if (!err) sle=0;
					}
			if (!NBlue::callback->tglid ) ok=0;
			
			if (ok && sle>=0){
				const auto driversXML = getFWByName(sle ? "Drivera2.xml": "Drivera3.xml");
				auto *dataNull = new char[driversXML.size + 1];
				memcpy(dataNull, driversXML.data, driversXML.size);
				dataNull[driversXML.size] = 0;
				OSString *errStr = nullptr;
				auto *dataUnserialized = OSUnserializeXML(dataNull, driversXML.size + 1, &errStr);
				delete[] dataNull;
				PANIC_COND(!dataUnserialized, "NRed", "Failed to unserialize Drivers.xml: %s",
						   errStr ? errStr->getCStringNoCopy() : "Unspecified");
				auto *drivers = OSDynamicCast(OSDictionary, dataUnserialized);
				PANIC_COND(!drivers, "NRed", "Failed to cast Drivers.xml data");
				auto* Match = OSDynamicCast(OSString, drivers->getObject("IOPCIPrimaryMatch"));
				if (Match->getLength()>0){
					tcap++;
					tdrivers->ensureCapacity(tcap);
					tdrivers->setObject(tcap-1,drivers);
					doNub=true;
				}
				OSSafeReleaseNULL(dataUnserialized);
				IOFree(driversXML.data, driversXML.size);
			}
			else if (!ok2 && NBlue::callback->iclid)
			{
				const auto driversXML = getFWByName("Drivera1.xml");
				auto *dataNull = new char[driversXML.size + 1];
				memcpy(dataNull, driversXML.data, driversXML.size);
				dataNull[driversXML.size] = 0;
				OSString *errStr = nullptr;
				auto *dataUnserialized = OSUnserializeXML(dataNull, driversXML.size + 1, &errStr);
				delete[] dataNull;
				PANIC_COND(!dataUnserialized, "NRed", "Failed to unserialize Drivers.xml: %s",
						   errStr ? errStr->getCStringNoCopy() : "Unspecified");
				auto *drivers = OSDynamicCast(OSDictionary, dataUnserialized);
				PANIC_COND(!drivers, "NRed", "Failed to cast Drivers.xml data");
				auto* Match = OSDynamicCast(OSString, drivers->getObject("IOPCIPrimaryMatch"));
				if (Match->getLength()>0){
					tcap++;
					tdrivers->ensureCapacity(tcap);
					tdrivers->setObject(tcap-1,drivers);
					doNub=true;
				}
				OSSafeReleaseNULL(dataUnserialized);
				IOFree(driversXML.data, driversXML.size);
			}
		}
		
	}

	return FunctionCast(wrapAddDrivers, callback->orgAddDrivers)(self, array, doNub);
}





bool NBlue::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
	
	if (kextIOAcceleratorFamily2.loadIndex == index) {
		/*KernelPatcher::RouteRequest requests[] = {
			{"__ZN14IOAccelSurface11set_id_modeEjj", lset_id_mode, olset_id_mode},
			{"__ZN20IOAccelLegacySurface11set_id_modeEjj", lset_id_mode, olset_id_mode},
		};*/
			
			//SYSLOG_COND(!patcher.routeMultipleLong(index, requests, address, size), "IOAcceleratorFamily2", "Failed to apply patch");
			//patcher.clearError();
		return true;
	}  else if (kextIOGraphics.loadIndex == index) {

		return true;
	}  else if (kextAGDP.loadIndex == index) {
		
		const LookupPatchPlus boardIdPatch {&kextAGDP, kAGDPBoardIDKeyOriginal, kAGDPBoardIDKeyPatched, 1};
		SYSLOG_COND(!boardIdPatch.apply(patcher, address, size), "AGDP", "Failed to apply AGDP board-id patch");

		if (getKernelVersion() == KernelVersion::Ventura) {
			const LookupPatchPlus patch {&kextAGDP, kAGDPFBCountCheckOriginal13, kAGDPFBCountCheckPatched13, 1};
			SYSLOG_COND(!patch.apply(patcher, address, size), "AGDP", "Failed to apply AGDP FB count check patch");
		} else {
			const LookupPatchPlus patch {&kextAGDP, kAGDPFBCountCheckOriginal, kAGDPFBCountCheckPatched, 1};
			SYSLOG_COND(!patch.apply(patcher, address, size), "AGDP", "Failed to apply AGDP FB count check patch");
		}
		
		
		return true;
	}  else if (kextBacklight.loadIndex == index) {
		KernelPatcher::RouteRequest request {"__ZN15AppleIntelPanel10setDisplayEP9IODisplay", wrapApplePanelSetDisplay,
	  orgApplePanelSetDisplay};
			if (patcher.routeMultipleLong(kextBacklight.loadIndex, &request, 1, address, size)) {
				const UInt8 find[] = {"F%uT%04x"};
				const UInt8 replace[] = {"F%uTxxxx"};
				const LookupPatchPlus patch {&kextBacklight, find, replace, 1};
				SYSLOG_COND(!patch.apply(patcher, address, size), "NBlue", "Failed to apply backlight patch");
			}
		return true;
} else if (kextMCCSControl.loadIndex == index) {
		KernelPatcher::RouteRequest requests[] = {
				{"__ZN25AppleMCCSControlGibraltar5probeEP9IOServicePi", wrapFunctionReturnZero},
				{"__ZN21AppleMCCSControlCello5probeEP9IOServicePi", wrapFunctionReturnZero},
			};
			patcher.routeMultipleLong(index, requests, address, size);
			patcher.clearError();
	return true;
} else if (gen11.processKext(patcher, index, address, size)) {
        DBGLOG("nblue", "Processed Generation 11 configuration");
	return true;
    } /*else if (agfxhda.processKext(patcher, index, address, size)) {
		DBGLOG("nblue", "Processed AppleGFXHDA");
	}*/
	
	
    return false;
}



uint16_t NBlue::configRead16(IORegistryEntry *service, uint32_t space, uint8_t offset) {
	if (callback && callback->orgConfigRead16) {
		auto result = callback->orgConfigRead16(service, space, offset);
		if (offset == WIOKit::kIOPCIConfigDeviceID && service != nullptr) {
			auto name = service->getName();
			if (name && name[0] == 'I' && name[1] == 'G' && name[2] == 'P' && name[3] == 'U') {
				uint32_t device;
				if (WIOKit::getOSDataValue(service, "device-id", device) && device != result) {
					return device;
				}
			}
		}

		return result;
	}

	return 0;
}

uint32_t NBlue::configRead32(IORegistryEntry *service, uint32_t space, uint8_t offset) {
	if (callback && callback->orgConfigRead32) {
		auto result = callback->orgConfigRead32(service, space, offset);
		// According to lvs unaligned reads may happen
		if ((offset == WIOKit::kIOPCIConfigDeviceID || offset == WIOKit::kIOPCIConfigVendorID) && service != nullptr) {
			auto name = service->getName();
			if (name && name[0] == 'I' && name[1] == 'G' && name[2] == 'P' && name[3] == 'U') {
				uint32_t device;
				if (WIOKit::getOSDataValue(service, "device-id", device) && device != (result & 0xFFFF)) {
					device = (result & 0xFFFF) | (device << 16);
					return device;
				}
			}
		}

		return result;
	}

	return 0;
}

size_t NBlue::wrapFunctionReturnZero() { return 0; }

struct ApplePanelData {
	const char *deviceName;
	UInt8 deviceData[36];
};


static ApplePanelData appleBacklightData[] = {
	{"F14Txxxx", {0x00, 0x11, 0x00, 0x00, 0x00, 0x34, 0x00, 0x52, 0x00, 0x73, 0x00, 0x94, 0x00, 0xBE, 0x00, 0xFA, 0x01,
					 0x36, 0x01, 0x72, 0x01, 0xC5, 0x02, 0x2F, 0x02, 0xB9, 0x03, 0x60, 0x04, 0x1A, 0x05, 0x0A, 0x06,
					 0x0E, 0x07, 0x10}},
	{"F15Txxxx", {0x00, 0x11, 0x00, 0x00, 0x00, 0x36, 0x00, 0x54, 0x00, 0x7D, 0x00, 0xB2, 0x00, 0xF5, 0x01, 0x49, 0x01,
					 0xB1, 0x02, 0x2B, 0x02, 0xB8, 0x03, 0x59, 0x04, 0x13, 0x04, 0xEC, 0x05, 0xF3, 0x07, 0x34, 0x08,
					 0xAF, 0x0A, 0xD9}},
	{"F16Txxxx", {0x00, 0x11, 0x00, 0x00, 0x00, 0x18, 0x00, 0x27, 0x00, 0x3A, 0x00, 0x52, 0x00, 0x71, 0x00, 0x96, 0x00,
					 0xC4, 0x00, 0xFC, 0x01, 0x40, 0x01, 0x93, 0x01, 0xF6, 0x02, 0x6E, 0x02, 0xFE, 0x03, 0xAA, 0x04,
					 0x78, 0x05, 0x6C}},
	{"F17Txxxx", {0x00, 0x11, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x34, 0x00, 0x4F, 0x00, 0x71, 0x00, 0x9B, 0x00, 0xCF, 0x01,
					 0x0E, 0x01, 0x5D, 0x01, 0xBB, 0x02, 0x2F, 0x02, 0xB9, 0x03, 0x60, 0x04, 0x29, 0x05, 0x1E, 0x06,
					 0x44, 0x07, 0xA1}},
	{"F18Txxxx", {0x00, 0x11, 0x00, 0x00, 0x00, 0x53, 0x00, 0x8C, 0x00, 0xD5, 0x01, 0x31, 0x01, 0xA2, 0x02, 0x2E, 0x02,
					 0xD8, 0x03, 0xAE, 0x04, 0xAC, 0x05, 0xE5, 0x07, 0x59, 0x09, 0x1C, 0x0B, 0x3B, 0x0D, 0xD0, 0x10,
					 0xEA, 0x14, 0x99}},
	{"F19Txxxx", {0x00, 0x11, 0x00, 0x00, 0x02, 0x8F, 0x03, 0x53, 0x04, 0x5A, 0x05, 0xA1, 0x07, 0xAE, 0x0A, 0x3D, 0x0E,
					 0x14, 0x13, 0x74, 0x1A, 0x5E, 0x24, 0x18, 0x31, 0xA9, 0x44, 0x59, 0x5E, 0x76, 0x83, 0x11, 0xB6,
					 0xC7, 0xFF, 0x7B}},
	{"F24Txxxx", {0x00, 0x11, 0x00, 0x01, 0x00, 0x34, 0x00, 0x52, 0x00, 0x73, 0x00, 0x94, 0x00, 0xBE, 0x00, 0xFA, 0x01,
					 0x36, 0x01, 0x72, 0x01, 0xC5, 0x02, 0x2F, 0x02, 0xB9, 0x03, 0x60, 0x04, 0x1A, 0x05, 0x0A, 0x06,
					 0x0E, 0x07, 0x10}},
};

bool NBlue::wrapApplePanelSetDisplay(IOService *that, IODisplay *display) {
	static bool once = false;
	if (!once) {
		once = true;
		auto *panels = OSDynamicCast(OSDictionary, that->getProperty("ApplePanels"));
		if (panels) {
			auto *rawPanels = panels->copyCollection();
			panels = OSDynamicCast(OSDictionary, rawPanels);

			if (panels) {
				for (auto &entry : appleBacklightData) {
					auto pd = OSData::withBytes(entry.deviceData, sizeof(entry.deviceData));
					if (pd) {
						panels->setObject(entry.deviceName, pd);
						//! No release required by current AppleBacklight implementation.
					} else {
					}
				}
				that->setProperty("ApplePanels", panels);
			}

			OSSafeReleaseNULL(rawPanels);
		} else {
		}
	}

	bool ret = FunctionCast(wrapApplePanelSetDisplay, callback->orgApplePanelSetDisplay)(that, display);
	return ret;
}

uint64_t NBlue::lset_id_mode(void *that,uint param_1,uint param_2)
{
	#define APPLE_LEGACY_VALID_MASK 0x007f8c3f
	param_2 &= APPLE_LEGACY_VALID_MASK;
	auto ret = FunctionCast(lset_id_mode, callback->olset_id_mode)(that, param_1,param_2);
	return ret;
}

uint64_t NBlue::lset_id_mode2(void *that,uint param_1,uint param_2)
{
	#define APPLE_LEGACY_VALID_MASK 0x007f8c3f
	param_2 &= APPLE_LEGACY_VALID_MASK;
	auto ret = FunctionCast(lset_id_mode2, callback->olset_id_mode2)(that, param_1,param_2);
	return ret;
}

u32 _get_blocksize(const u8 *block_base)
{
	if (*block_base == BDB_MIPI_SEQUENCE && *(block_base + 3) >= 3)
		return *((const u32 *)(block_base + 4));
	else
		return *((const u16 *)(block_base + 1));
}

static const void *
find_raw_section(const void *_bdb, enum bdb_block_id section_id)
{
	const struct bdb_header *bdb = (const struct bdb_header *)_bdb;
	const u8 *base = (const u8 *)_bdb;
	int index = 0;
	u32 total, current_size;
	enum bdb_block_id current_id;

	/* skip to first section */
	index += bdb->header_size;
	total = bdb->bdb_size;

	/* walk the sections looking for section_id */
	while (index + 3 < total) {
		current_id = static_cast<enum bdb_block_id>( *(base + index));
		current_size = _get_blocksize(base + index);
		index += 3;

		if (index + current_size > total)
			return NULL;

		if (current_id == section_id)
			return base + index;

		index += current_size;
	}

	return NULL;
}

static const void *
bdb_find_section(struct intel_display *display,
		 enum bdb_block_id section_id)
{
	struct bdb_block_entry *entry;

	list_for_each_entry(entry, &display->vbt.bdb_blocks, node) {
		if (entry->section_id == section_id)
			return entry->data + 3;
	}

	return NULL;
}

static size_t lfp_data_min_size(struct intel_display *display)
{
	const struct bdb_lfp_data_ptrs *ptrs;
	size_t size;

	ptrs = (const struct bdb_lfp_data_ptrs *)bdb_find_section(display, BDB_LFP_DATA_PTRS);
	if (!ptrs)
		return 0;

	size = sizeof(struct bdb_lfp_data);
	if (ptrs->panel_name.table_size)
		size = max(size, ptrs->panel_name.offset +
			   sizeof(struct bdb_lfp_data_tail));

	return size;
}

int get_child_device_expected_size(u16 vbt_version)
{
	if (vbt_version > 264) return -1;
	else if (vbt_version >= 263) return 44;
	else if (vbt_version >= 256) return 40;
	else if (vbt_version >= 216) return 39;
	else if (vbt_version >= 196) return 38;
	else if (vbt_version >= 195) return 37;
	else if (vbt_version >= 111) return 33;
	else if (vbt_version >= 106) return 27;
	else return 22;
}
const char *aux_ch_name(struct intel_display *display,
				   char *buf, int size, enum aux_ch aux_ch)
{
	if (DISPLAY_VER(display) >= 13 && aux_ch >= AUX_CH_D_XELPD)
		snprintf(buf, size, "AUX_CH_%c", 'A' + aux_ch - AUX_CH_D_XELPD + AUX_CH_D);
	else if (DISPLAY_VER(display) >= 12 && aux_ch >= AUX_CH_USBC1)
		snprintf(buf, size, "AUX_CH_USBC%c", '1' + aux_ch - AUX_CH_USBC1);
	else
		snprintf(buf, size, "AUX_CH_%c", 'A' + aux_ch);

	return buf;
}


const char *intel_ddi_encoder_name(struct intel_display *display,
					  enum port port, char *s, int size)
{
	if (DISPLAY_VER(display) >= 13 && port >= PORT_D_XELPD) {
		snprintf(s, size, "DDI_%c",
				   port_name(port - PORT_D_XELPD + PORT_D));
	} else if (DISPLAY_VER(display) >= 12) {

		snprintf(s, size,  "DDI_%c",
				 port_name(port));
	} else if (DISPLAY_VER(display) >= 11) {

		snprintf(s, size,  "DDI_%c",
				   port_name(port));
	} else {
		snprintf(s, size,  "DDI_%c", port_name(port));
	}

	return (s);
}

enum aux_ch map_aux_ch(struct intel_display *display, u8 aux_channel)
{
	const u8 *aux_ch_map;
	int i, n_entries;

	if (DISPLAY_VER(display) >= 13) {
		aux_ch_map = adlp_aux_ch_map;
		n_entries = ARRAY_SIZE(adlp_aux_ch_map);
	} else if (display->platform.alderlake_s) {
		aux_ch_map = adls_aux_ch_map;
		n_entries = ARRAY_SIZE(adls_aux_ch_map);
	} else if (display->platform.dg1 || display->platform.rocketlake) {
		aux_ch_map = rkl_aux_ch_map;
		n_entries = ARRAY_SIZE(rkl_aux_ch_map);
	} else {
		aux_ch_map = direct_aux_ch_map;
		n_entries = ARRAY_SIZE(direct_aux_ch_map);
	}

	for (i = 0; i < n_entries; i++) {
		if (aux_ch_map[i] == aux_channel)
			return static_cast<enum aux_ch>(i);
	}

	return AUX_CH_NONE;
}
const char* phy_to_string(enum phy port)
{
	switch (port) {
		case PHY_A: return "PHY_A";
		case PHY_B: return "PHY_B";
		case PHY_C: return "PHY_C";
		case PHY_D: return "PHY_D";
		case PHY_E: return "PHY_E";
		case PHY_F: return "PHY_F";
		case PHY_G: return "PHY_G";
		case PHY_H: return "PHY_H";
		default: return "Unknown";
	}
}

enum phy intel_port_to_phy(struct intel_display *display, enum port port)
{
	if (DISPLAY_VER(display) >= 13 && port >= PORT_D_XELPD)
			return static_cast<enum phy>(PHY_D + port - PORT_D_XELPD);
		else if (DISPLAY_VER(display) >= 13 && port >= PORT_TC1)
			return static_cast<enum phy>(PHY_F + port - PORT_TC1);
		else if (display->platform.alderlake_s && port >= PORT_TC1)
			return static_cast<enum phy>(PHY_B + port - PORT_TC1);
		else if ((display->platform.dg1 || display->platform.rocketlake) && port >= PORT_TC1)
			return static_cast<enum phy>(PHY_C + port - PORT_TC1);
		/*else if ((display->platform.jasperlake || display->platform.elkhartlake) &&
			 port == PORT_D)
			return PHY_A;*/

		return static_cast<enum phy>(PHY_A + port - PORT_A);
}

enum port __dvo_port_to_port(int n_ports, int n_dvo,
					const int port_mapping[][3], u8 dvo_port)
{
	int i, j;

	for (i = 0; i < n_ports; i++) {
		for (j = 0; j < n_dvo; j++) {
			if (port_mapping[i][j] == -1)
				break;

			if (dvo_port == port_mapping[i][j])
				return static_cast<enum port>(i);
		}
	}

	return PORT_NONE;
}

enum port dvo_port_to_port(struct intel_display *display, u8 dvo_port)
{

	const int port_mapping[][3] = {
		[PORT_A] = { DVO_PORT_HDMIA, DVO_PORT_DPA, -1 },
		[PORT_B] = { DVO_PORT_HDMIB, DVO_PORT_DPB, -1 },
		[PORT_C] = { DVO_PORT_HDMIC, DVO_PORT_DPC, -1 },
		[PORT_D] = { DVO_PORT_HDMID, DVO_PORT_DPD, -1 },
		[PORT_E] = { DVO_PORT_HDMIE, DVO_PORT_DPE, DVO_PORT_CRT },
		[PORT_F] = { DVO_PORT_HDMIF, DVO_PORT_DPF, -1 },
		[PORT_G] = { DVO_PORT_HDMIG, DVO_PORT_DPG, -1 },
		[PORT_H] = { DVO_PORT_HDMIH, DVO_PORT_DPH, -1 },
		[PORT_I] = { DVO_PORT_HDMII, DVO_PORT_DPI, -1 },
	};

	const int rkl_port_mapping[][3] = {
		[PORT_A] = { DVO_PORT_HDMIA, DVO_PORT_DPA, -1 },
		[PORT_B] = { DVO_PORT_HDMIB, DVO_PORT_DPB, -1 },
		[PORT_C] = { -1 },
		[PORT_TC1] = { DVO_PORT_HDMIC, DVO_PORT_DPC, -1 },
		[PORT_TC2] = { DVO_PORT_HDMID, DVO_PORT_DPD, -1 },
	};

	const int adls_port_mapping[][3] = {
		[PORT_A] = { DVO_PORT_HDMIA, DVO_PORT_DPA, -1 },
		[PORT_B] = { -1 },
		[PORT_C] = { -1 },
		[PORT_TC1] = { DVO_PORT_HDMIB, DVO_PORT_DPB, -1 },
		[PORT_TC2] = { DVO_PORT_HDMIC, DVO_PORT_DPC, -1 },
		[PORT_TC3] = { DVO_PORT_HDMID, DVO_PORT_DPD, -1 },
		[PORT_TC4] = { DVO_PORT_HDMIE, DVO_PORT_DPE, -1 },
	};
	const int xelpd_port_mapping[][3] = {
		[PORT_A] = { DVO_PORT_HDMIA, DVO_PORT_DPA, -1 },
		[PORT_B] = { DVO_PORT_HDMIB, DVO_PORT_DPB, -1 },
		[PORT_C] = { DVO_PORT_HDMIC, DVO_PORT_DPC, -1 },
		[PORT_D_XELPD] = { DVO_PORT_HDMID, DVO_PORT_DPD, -1 },
		[PORT_E_XELPD] = { DVO_PORT_HDMIE, DVO_PORT_DPE, -1 },
		[PORT_TC1] = { DVO_PORT_HDMIF, DVO_PORT_DPF, -1 },
		[PORT_TC2] = { DVO_PORT_HDMIG, DVO_PORT_DPG, -1 },
		[PORT_TC3] = { DVO_PORT_HDMIH, DVO_PORT_DPH, -1 },
		[PORT_TC4] = { DVO_PORT_HDMII, DVO_PORT_DPI, -1 },
	};


	if (DISPLAY_VER(display) >= 13)
		return __dvo_port_to_port(sizeof(xelpd_port_mapping) / sizeof(xelpd_port_mapping[0]),
					  3, // array size
					  xelpd_port_mapping,
					  dvo_port);
	else if (display->platform.alderlake_s)
		return __dvo_port_to_port(sizeof(adls_port_mapping) / sizeof(adls_port_mapping[0]),
					  3,
					  adls_port_mapping,
					  dvo_port);
	else if (display->platform.dg1 || display->platform.rocketlake)
		return __dvo_port_to_port(sizeof(rkl_port_mapping) / sizeof(rkl_port_mapping[0]),
					  3,
					  rkl_port_mapping,
					  dvo_port);
	else
		return __dvo_port_to_port(sizeof(port_mapping) / sizeof(port_mapping[0]),
					  3,
					  port_mapping,
					  dvo_port);
}

const char* port_to_string(enum port port)
{
	switch (port) {
		case PORT_A: return "PORT_A";
		case PORT_B: return "PORT_B";
		case PORT_C: return "PORT_C";
		case PORT_D: return "PORT_D";
		case PORT_E: return "PORT_E";
		case PORT_F: return "PORT_F";
		case PORT_G: return "PORT_G";
		case PORT_H: return "PORT_H";
		case PORT_I: return "PORT_I";
		case PORT_TC1: return "PORT_TC1";
		case PORT_TC2: return "PORT_TC2";
		case PORT_TC3: return "PORT_TC3";
		case PORT_TC4: return "PORT_TC4";
		default: return "Unknown";
	}
}






u32 bxt_hz_to_pwm(u32 pwm_freq_hz)
{
	return DIV_ROUND_CLOSEST(KHz(19200), pwm_freq_hz);
}

u32 cnp_hz_to_pwm(u32 rawclk_freq, u32 pwm_freq_hz)
{

	return DIV_ROUND_CLOSEST(KHz(rawclk_freq),pwm_freq_hz);
}

uint32_t scale(uint32_t source_val,
					  uint32_t source_min, uint32_t source_max,
					  uint32_t target_min, uint32_t target_max)
{
	if (source_val < source_min) {
		source_val = source_min;
	} else if (source_val > source_max) {
		source_val = source_max;
	}
	uint64_t source_range = static_cast<uint64_t>(source_max) - source_min;
	uint64_t target_range = static_cast<uint64_t>(target_max) - target_min;

	uint64_t target_val = (static_cast<uint64_t>(source_val) - source_min) * target_range;

	target_val = (target_val + (source_range / 2)) / source_range;

	target_val += target_min;

	return static_cast<uint32_t>(target_val);
}



bool cnp_backlight_controller_is_valid( int controller)
{
	if (controller < 0 || controller >= 2)
		return false;

	if (controller == 1)
		return NBlue::callback->readReg32( SOUTH_CHICKEN1) & ICP_SECOND_PPS_IO_SELECT;

	return true;
}


void bxt_set_backlight(struct intel_panel *panel, u32 level)
{
	NBlue::callback->writeReg32( BXT_BLC_PWM_DUTY(panel->backlight.controller), level);
}

void cnp_enable_backlight(struct intel_panel *panel, u32 level)
{
	u32 pwm_ctl;

	pwm_ctl = NBlue::callback->readReg32( BXT_BLC_PWM_CTL(panel->backlight.controller));
	if (pwm_ctl & BXT_BLC_PWM_ENABLE) {
		pwm_ctl &= ~BXT_BLC_PWM_ENABLE;
		NBlue::callback->writeReg32( BXT_BLC_PWM_CTL(panel->backlight.controller),
				   pwm_ctl);
	}

	NBlue::callback->writeReg32( BXT_BLC_PWM_FREQ(panel->backlight.controller),
			   panel->backlight.pwm_level_max);

	bxt_set_backlight(panel, level);

	pwm_ctl = 0;
	if (panel->backlight.active_low_pwm)
		pwm_ctl |= BXT_BLC_PWM_POLARITY;
	
	NBlue::callback->writeReg32( BXT_BLC_PWM_CTL(panel->backlight.controller), pwm_ctl);
	NBlue::callback->readReg32( BXT_BLC_PWM_CTL(panel->backlight.controller));
	NBlue::callback->writeReg32( BXT_BLC_PWM_CTL(panel->backlight.controller),
			   pwm_ctl | BXT_BLC_PWM_ENABLE);
}

unsigned int panel_bits(unsigned int value, int panel_type, int num_bits)
{
	return (value >> (panel_type * num_bits)) & (BIT(num_bits) - 1);
}

void vbt_edp_to_pps_delays(struct intel_pps_delays *pps,
				  const struct edp_power_seq *edp_pps)
{
	pps->power_up = edp_pps->t1_t3;
	pps->backlight_on = edp_pps->t8;
	pps->backlight_off = edp_pps->t9;
	pps->power_down = edp_pps->t10;
	pps->power_cycle = edp_pps->t11_t12;
}

static bool panel_bool(unsigned int value, int panel_type)
{
	return panel_bits(value, panel_type, 1);
}

static void
parse_edp(struct intel_display *display,
	  struct intel_panel *panel)
{
	const struct bdb_edp *edp;
	const struct edp_fast_link_params *edp_link_params;
	int panel_type = panel->vbt.panel_type;

	edp = (const struct bdb_edp *)bdb_find_section(display, BDB_EDP);
	if (!edp)
		return;

	switch (panel_bits(edp->color_depth, panel_type, 2)) {
	default:
	/*case EDP_18BPP:
		panel->vbt.edp.bpp = 18;
		break;*/
	case EDP_24BPP:
		panel->vbt.edp.bpp = 24;
		break;
	case EDP_30BPP:
		panel->vbt.edp.bpp = 30;
		break;
	}

	
	/* Get the eDP sequencing and link info */
	edp_link_params = &edp->fast_link_params[panel_type];

	vbt_edp_to_pps_delays(&panel->vbt.edp.pps,
				  &edp->power_seqs[panel_type]);

	if (display->vbt.version >= 224) {
		panel->vbt.edp.rate =
			edp->edp_fast_link_training_rate[panel_type] * 20;
	} else {
		switch (edp_link_params->rate) {
		case EDP_RATE_1_62:
			panel->vbt.edp.rate = 162000;
			break;
		case EDP_RATE_2_7:
			panel->vbt.edp.rate = 270000;
			break;
		case EDP_RATE_5_4:
			panel->vbt.edp.rate = 540000;
			break;
		default:
			break;
		}
	}

	switch (edp_link_params->lanes) {
	case EDP_LANE_1:
		panel->vbt.edp.lanes = 1;
		break;
	case EDP_LANE_2:
		panel->vbt.edp.lanes = 2;
		break;
	case EDP_LANE_4:
		panel->vbt.edp.lanes = 4;
		break;
	default:
		break;
	}

	switch (edp_link_params->preemphasis) {
	case EDP_PREEMPHASIS_NONE:
		panel->vbt.edp.preemphasis = DP_TRAIN_PRE_EMPH_LEVEL_0;
		break;
	case EDP_PREEMPHASIS_3_5dB:
		panel->vbt.edp.preemphasis = DP_TRAIN_PRE_EMPH_LEVEL_1;
		break;
	case EDP_PREEMPHASIS_6dB:
		panel->vbt.edp.preemphasis = DP_TRAIN_PRE_EMPH_LEVEL_2;
		break;
	case EDP_PREEMPHASIS_9_5dB:
		panel->vbt.edp.preemphasis = DP_TRAIN_PRE_EMPH_LEVEL_3;
		break;
	default:
		break;
	}

	switch (edp_link_params->vswing) {
	case EDP_VSWING_0_4V:
		panel->vbt.edp.vswing = DP_TRAIN_VOLTAGE_SWING_LEVEL_0;
		break;
	case EDP_VSWING_0_6V:
		panel->vbt.edp.vswing = DP_TRAIN_VOLTAGE_SWING_LEVEL_1;
		break;
	case EDP_VSWING_0_8V:
		panel->vbt.edp.vswing = DP_TRAIN_VOLTAGE_SWING_LEVEL_2;
		break;
	case EDP_VSWING_1_2V:
		panel->vbt.edp.vswing = DP_TRAIN_VOLTAGE_SWING_LEVEL_3;
		break;
	default:
		break;
	}

	if (display->vbt.version >= 173) {
		u8 vswing;

		/* Don't read from VBT if module parameter has valid value*/
		/*if (display->params.edp_vswing) {
			panel->vbt.edp.low_vswing =
				display->params.edp_vswing == 1;
		} else {*/
			vswing = (edp->edp_vswing_preemph >> (panel_type * 4)) & 0xF;
			panel->vbt.edp.low_vswing = vswing == 0;
		//}
	}

	panel->vbt.edp.drrs_msa_timing_delay =
		panel_bits(edp->sdrrs_msa_timing_delay, panel_type, 2);

	if (display->vbt.version >= 244)
		panel->vbt.edp.max_link_rate =
			edp->edp_max_port_link_rate[panel_type] * 20;

	if (display->vbt.version >= 251)
		panel->vbt.edp.dsc_disable =
			panel_bool(edp->edp_dsc_disable, panel_type);

	if (display->vbt.version >= 261)
		panel->vbt.edp.pipe_joiner_enable =
			panel_bool(edp->pipe_joiner_enable, panel_type);
}

int pps_units_to_msecs(int val)
{
	return DIV_ROUND_UP(val, 10);
}
int msecs_to_pps_units(int msecs)
{
	/* PPS uses 100us units */
	return msecs * 10;
}




static void
parse_lfp_backlight()
{
	struct drm_i915_private *i915=NBlue::callback->i915b;
	if (i915->initok) return;
	i915->initok=true;
	struct intel_display *display=i915->display;
	
	struct intel_panel *panel=&display->panel;
	const struct bdb_lfp_backlight *backlight_data;
	const struct lfp_backlight_data_entry *entry;
	int panel_type = panel->vbt.panel_type;
	u16 level;
	
	if ((NBlue::callback->readReg32( DP_A) & EDP_PLL_FREQ_MASK) == EDP_PLL_FREQ_162MHZ)
		display->port_clock = 162000;
	else
		display->port_clock = 270000;
	
	int port_clock= display->port_clock;
	struct intel_dp *intel_dp=&display->intel_dp0;
	struct intel_crtc_state *crtc_state=&display->crtc_state0;
	
	intel_dp->link_rate = port_clock;
	crtc_state->port_clock=port_clock;

	backlight_data = (const struct bdb_lfp_backlight *)bdb_find_section(display, BDB_LFP_BACKLIGHT);
	if (!backlight_data)
		return;

	if (backlight_data->entry_size != sizeof(backlight_data->data[0])) {
		return;
	}

	entry = &backlight_data->data[panel_type];

	panel->vbt.backlight.present = entry->type == BDB_BACKLIGHT_TYPE_PWM;
	if (!panel->vbt.backlight.present) {
		return;
	}

	panel->vbt.backlight.type = INTEL_BACKLIGHT_DISPLAY_DDI;
	panel->vbt.backlight.controller = 0;
	if (display->vbt.version >= 191) {
		const struct lfp_backlight_control_method *method;

		method = &backlight_data->backlight_control[panel_type];
		
		panel->vbt.backlight.type = static_cast<enum intel_backlight_type>(method->type);
		panel->vbt.backlight.controller = method->controller;
	}
	
	panel->vbt.backlight.pwm_freq_hz = entry->pwm_freq_hz;
	panel->vbt.backlight.active_low_pwm = entry->active_low_pwm;

	if (display->vbt.version >= 234) {
		u16 min_level;
		bool scale;

		level = backlight_data->brightness_level[panel_type].level;
		min_level = backlight_data->brightness_min_level[panel_type].level;

		if (display->vbt.version >= 236)
			scale = backlight_data->brightness_precision_bits[panel_type] == 16;
		else
			scale = level > 255;

		if (scale)
			min_level = min_level / 255;

		if (min_level > 255) {
			level = 255;
		}
		panel->vbt.backlight.min_brightness = min_level;

		panel->vbt.backlight.brightness_precision_bits =
			backlight_data->brightness_precision_bits[panel_type];
	} else {
		level = backlight_data->level[panel_type];
		panel->vbt.backlight.min_brightness = entry->min_brightness;
	}

	if (display->vbt.version >= 239)
		panel->vbt.backlight.hdr_dpcd_refresh_timeout =
			DIV_ROUND_UP(backlight_data->hdr_dpcd_refresh_timeout[panel_type], 100);
	else
		panel->vbt.backlight.hdr_dpcd_refresh_timeout = 30;
	
	
	panel->backlight.controller = panel->vbt.backlight.controller;
	if (!cnp_backlight_controller_is_valid( panel->backlight.controller)) {
		panel->backlight.controller = 0;
	}
	panel->pps.pps_idx=panel->vbt.backlight.controller;
	
	if (panel->backlight.controller == 1) {
		u32 val = NBlue::callback->readReg32( UTIL_PIN_CTL);
		panel->backlight.util_pin_active_low =val & UTIL_PIN_POLARITY;
	}
	
	
	//panel->rawclk_freq= cnp_rawclk();
	
	u32 pwm_ctl = NBlue::callback->readReg32(BXT_BLC_PWM_CTL(panel->backlight.controller));
	panel->backlight.active_low_pwm = pwm_ctl & BXT_BLC_PWM_POLARITY;
	panel->backlight.pwm_level_max = NBlue::callback->readReg32( BXT_BLC_PWM_FREQ(panel->backlight.controller));

	if (!panel->backlight.pwm_level_max)
		panel->backlight.pwm_level_max = cnp_hz_to_pwm(panel->rawclk_freq,panel->vbt.backlight.pwm_freq_hz);
	
	int val = static_cast<int>(panel->vbt.backlight.min_brightness);
	int vmin = (val < 0) ? 0 : (val > 64 ? 64 : val);
	
	panel->backlight.pwm_level_min = scale(vmin, 0, 255, 0, panel->backlight.pwm_level_max);
	
	panel->backlight.level=panel->backlight.pwm_level_max-(5*panel->vbt.backlight.min_brightness)*0x100;
	
	panel->backlight.pwm_enabled = pwm_ctl & BXT_BLC_PWM_ENABLE;
	
	//cnp_enable_backlight()
	//bxt_set_backlight()
	
	
	u32 pp_on, pp_off, pp_ctl, power_cycle_delay,pp_div,pp_sta;
	
	panel->regs.pp_ctrl = PP_CONTROL(panel, panel->pps.pps_idx);
	panel->regs.pp_stat = PP_STATUS(panel, panel->pps.pps_idx);
	panel->regs.pp_on = PP_ON_DELAYS(panel, panel->pps.pps_idx);
	panel->regs.pp_off = PP_OFF_DELAYS(panel, panel->pps.pps_idx);
	panel->regs.pp_div = PP_DIVISOR(panel, panel->pps.pps_idx);
	
	
	pp_ctl = NBlue::callback->readReg32(panel->regs.pp_ctrl);
	pp_on = NBlue::callback->readReg32( panel->regs.pp_on);
	pp_off = NBlue::callback->readReg32( panel->regs.pp_off);
	pp_div = NBlue::callback->readReg32( panel->regs.pp_div);
	pp_sta = NBlue::callback->readReg32( panel->regs.pp_stat);
	
	panel->pps.bios_pps_delays.power_up = REG_FIELD_GET(PANEL_POWER_UP_DELAY_MASK, pp_on);
	panel->pps.bios_pps_delays.backlight_on = REG_FIELD_GET(PANEL_LIGHT_ON_DELAY_MASK, pp_on);
	panel->pps.bios_pps_delays.backlight_off = REG_FIELD_GET(PANEL_LIGHT_OFF_DELAY_MASK, pp_off);
	panel->pps.bios_pps_delays.power_down = REG_FIELD_GET(PANEL_POWER_DOWN_DELAY_MASK, pp_off);

	if (NBlue::callback->readReg32(panel->regs.pp_div)!=-1) {
		u32 pp_div;
		pp_div = NBlue::callback->readReg32( panel->regs.pp_div);
		power_cycle_delay = REG_FIELD_GET(PANEL_POWER_CYCLE_DELAY_MASK, pp_div);
	} else {
		power_cycle_delay = REG_FIELD_GET(BXT_POWER_CYCLE_DELAY_MASK, pp_ctl);
	}
	
	panel->pps.bios_pps_delays.power_cycle = power_cycle_delay ? (power_cycle_delay - 1) * 1000 : 0;
	
	struct intel_pps_delays bios, vbt, spec;
	
	spec.power_up = msecs_to_pps_units(10 + 200);
	spec.backlight_on = msecs_to_pps_units(50);
	spec.backlight_off = msecs_to_pps_units(50);
	spec.power_down = msecs_to_pps_units(500);
	spec.power_cycle = msecs_to_pps_units(10+500);
	
	//parse_edp(display,panel);
	vbt = panel->vbt.edp.pps;
	bios = panel->pps.bios_pps_delays;
	
	struct intel_pps_delays *finalb = &panel->pps.pps_delays;
	
#define assign_final(field)	finalb->field = (max(bios.field, vbt.field) == 0 ? \
					   spec.field : \
					   max(bios.field, vbt.field))
	assign_final(power_up);
	assign_final(backlight_on);
	assign_final(backlight_off);
	assign_final(power_down);
	assign_final(power_cycle);
#undef assign_final
	
	
	panel->pps.panel_power_up_delay = pps_units_to_msecs(finalb->power_up);
	panel->pps.backlight_on_delay = pps_units_to_msecs(finalb->backlight_on);
	panel->pps.backlight_off_delay = pps_units_to_msecs(finalb->backlight_off);
	panel->pps.panel_power_down_delay = pps_units_to_msecs(finalb->power_down);
	panel->pps.panel_power_cycle_delay = pps_units_to_msecs(finalb->power_cycle);
	
	finalb->power_cycle = roundup(finalb->power_cycle, msecs_to_pps_units(100));
	
	//[drm:intel_edp_fixup_vbt_bpp [i915]] pipe has 24 bpp for eDP panel, overriding BIOS-provided max 18 bpp
	//if (panel->vbt.edp.bpp==18) panel->vbt.edp.bpp=24;
	
	OSArray *connectorArray = OSArray::withCapacity(1);
	OSDictionary *connectorDict = OSDictionary::withCapacity(40);
	
	connectorDict->setObject("step_name", OSString::withCString( DISPLAY_RUNTIME_INFO(display)->step_name ));
	connectorDict->setObject("display_ver", OSNumber::withNumber(DISPLAY_VER(display), 32));
	connectorDict->setObject("devid", OSNumber::withNumber(NBlue::callback->deviceId, 32));
	connectorDict->setObject("revid", OSNumber::withNumber(NBlue::callback->pciRevision, 32));
	
	connectorDict->setObject("panel_type", OSNumber::withNumber(panel_type, 32));
	connectorDict->setObject("pp_ctl", OSNumber::withNumber(pp_ctl, 32));
	connectorDict->setObject("pp_status", OSNumber::withNumber(pp_sta, 32));
	connectorDict->setObject("pp_on", OSNumber::withNumber(pp_on, 32));
	connectorDict->setObject("pp_off", OSNumber::withNumber(pp_off, 32));
	connectorDict->setObject("pp_div", OSNumber::withNumber(pp_div, 32));
	connectorDict->setObject("panel->rawclk_freq", OSNumber::withNumber(panel->rawclk_freq, 32));
	connectorDict->setObject("levelX", OSNumber::withNumber(panel->backlight.level, 32));
	connectorDict->setObject("vbt.pwm_freq_hz", OSNumber::withNumber(panel->vbt.backlight.pwm_freq_hz, 32));
	connectorDict->setObject("vbt.controller", OSNumber::withNumber(panel->vbt.backlight.controller, 32));
	connectorDict->setObject("vbt.min_brightness", OSNumber::withNumber(panel->vbt.backlight.min_brightness, 32));
	connectorDict->setObject("vbt.edp.bpp", OSNumber::withNumber(panel->vbt.edp.bpp, 32));
	connectorDict->setObject("vbt.edp.rate", OSNumber::withNumber(panel->vbt.edp.rate, 32));
	connectorDict->setObject("vbt.edp.lanes", OSNumber::withNumber(panel->vbt.edp.lanes, 32));
	
	connectorDict->setObject("pwm_enabled", OSNumber::withNumber(panel->backlight.pwm_enabled, 32));
	connectorDict->setObject("pwm_level_min", OSNumber::withNumber(panel->backlight.pwm_level_min, 32));
	connectorDict->setObject("pwm_level_max", OSNumber::withNumber(panel->backlight.pwm_level_max, 32));
	connectorDict->setObject("active_low_pwm", OSNumber::withNumber(panel->backlight.active_low_pwm , 32));
	
	connectorDict->setObject("pps->power_up", OSNumber::withNumber(panel->pps.pps_delays.power_up, 32));
	connectorDict->setObject("pps->power_down", OSNumber::withNumber(panel->pps.pps_delays.power_down, 32));
	connectorDict->setObject("pps->power_cycle", OSNumber::withNumber(panel->pps.pps_delays.power_cycle, 32));
	connectorDict->setObject("pps->backlight_on", OSNumber::withNumber(panel->pps.pps_delays.backlight_on, 32));
	connectorDict->setObject("pps->backlight_off", OSNumber::withNumber(panel->pps.pps_delays.backlight_off, 32));
	
	connectorDict->setObject("pps->round", OSNumber::withNumber(DIV_ROUND_UP(panel->pps.pps_delays.power_cycle, 1000) + 1, 32));
	
	
	
	connectorArray->setObject(connectorDict);
	connectorDict->release();

	NBlue::callback->iGPU->setProperty("PANEL", connectorArray);
	connectorArray->release();
	
	
	
}


static enum hpd_pin tgl_hpd_pin(enum port port)
{
	if (port >= PORT_TC1)
		return  static_cast<enum hpd_pin>( HPD_PORT_TC1 + port - PORT_TC1);
	else
		return static_cast<enum hpd_pin>( HPD_PORT_A + port - PORT_A);
}

static enum port
dsi_dvo_port_to_port(struct intel_display *display, u8 dvo_port)
{
	switch (dvo_port) {
	case DVO_PORT_MIPIA:
		return PORT_A;
	case DVO_PORT_MIPIC:
		if (DISPLAY_VER(display) >= 11)
			return PORT_B;
		else
			return PORT_C;
	default:
		return PORT_NONE;
	}
}






static u32 get_blocksize(const void *block_data)
{
	return _get_blocksize((u8*)block_data - 3);
}

static int make_lfp_data_ptr(struct lfp_data_ptr_table *table,
				 int table_size, int total_size)
{
	if (total_size < table_size)
		return total_size;

	table->table_size = table_size;
	table->offset = total_size - table_size;

	return total_size - table_size;
}
static void next_lfp_data_ptr(struct lfp_data_ptr_table *next,
				  const struct lfp_data_ptr_table *prev,
				  int size)
{
	next->table_size = prev->table_size;
	next->offset = prev->offset + size;
}
static void *generate_lfp_data_ptrs(struct intel_display *display,
					const void *bdb)
{
	int i, size, table_size, block_size, offset, fp_timing_size;
	struct bdb_lfp_data_ptrs *ptrs;
	const void *block;
	void *ptrs_block;

	/*
	 * The hardcoded fp_timing_size is only valid for
	 * modernish VBTs. All older VBTs definitely should
	 * include block 41 and thus we don't need to
	 * generate one.
	 */
	if (display->vbt.version < 155)
		return NULL;

	fp_timing_size = 38;

	block = find_raw_section(bdb, BDB_LFP_DATA);
	if (!block)
		return NULL;


	block_size = get_blocksize(block);

	size = fp_timing_size + sizeof(struct bdb_edid_dtd) +
		sizeof(struct bdb_edid_pnp_id);
	if (size * 16 > block_size)
		return NULL;
	ptrs_block =IOMalloc(sizeof(struct bdb_lfp_data_ptrs) + 3);
	//ptrs_block = kzalloc(sizeof(*ptrs) + 3, GFP_KERNEL);
	if (!ptrs_block)
		return NULL;

	*(u8 *)((u8 *)ptrs_block + 0) = BDB_LFP_DATA_PTRS;
	*(u16 *)((u8 *)ptrs_block + 1) = sizeof(*ptrs);
	ptrs =(struct bdb_lfp_data_ptrs *)( (u8 *)ptrs_block + 3);

	table_size = sizeof(struct bdb_edid_pnp_id);
	size = make_lfp_data_ptr(&ptrs->ptr[0].panel_pnp_id, table_size, size);

	table_size = sizeof(struct bdb_edid_dtd);
	size = make_lfp_data_ptr(&ptrs->ptr[0].dvo_timing, table_size, size);

	table_size = fp_timing_size;
	size = make_lfp_data_ptr(&ptrs->ptr[0].fp_timing, table_size, size);

	if (ptrs->ptr[0].fp_timing.table_size)
		ptrs->num_entries++;
	if (ptrs->ptr[0].dvo_timing.table_size)
		ptrs->num_entries++;
	if (ptrs->ptr[0].panel_pnp_id.table_size)
		ptrs->num_entries++;

	if (size != 0 || ptrs->num_entries != 3) {
		//kfree(ptrs_block);
		IOFree(ptrs_block, sizeof(*ptrs) + 3);
		return NULL;
	}

	size = fp_timing_size + sizeof(struct bdb_edid_dtd) +
		sizeof(struct bdb_edid_pnp_id);
	for (i = 1; i < 16; i++) {
		next_lfp_data_ptr(&ptrs->ptr[i].fp_timing, &ptrs->ptr[i-1].fp_timing, size);
		next_lfp_data_ptr(&ptrs->ptr[i].dvo_timing, &ptrs->ptr[i-1].dvo_timing, size);
		next_lfp_data_ptr(&ptrs->ptr[i].panel_pnp_id, &ptrs->ptr[i-1].panel_pnp_id, size);
	}

	table_size = sizeof(struct bdb_edid_product_name);

	if (16 * (size + table_size) <= block_size) {
		ptrs->panel_name.table_size = table_size;
		ptrs->panel_name.offset = size * 16;
	}

	offset = (u8 *)block - (u8 *)bdb;

	for (i = 0; i < 16; i++) {
		ptrs->ptr[i].fp_timing.offset += offset;
		ptrs->ptr[i].dvo_timing.offset += offset;
		ptrs->ptr[i].panel_pnp_id.offset += offset;
	}

	if (ptrs->panel_name.table_size)
		ptrs->panel_name.offset += offset;

	return ptrs_block;
}
static u32 raw_block_offset(const void *bdb, enum bdb_block_id section_id)
{
	const void *block;

	block = find_raw_section(bdb, section_id);
	if (!block)
		return 0;

	return (u8 *)block - (u8 *)bdb;
}
static bool validate_lfp_data_ptrs(const void *bdb,
				   const struct bdb_lfp_data_ptrs *ptrs)
{
	int fp_timing_size, dvo_timing_size, panel_pnp_id_size, panel_name_size;
	int data_block_size, lfp_data_size;
	const void *data_block;
	int i;

	data_block = find_raw_section(bdb, BDB_LFP_DATA);
	if (!data_block)
		return false;

	data_block_size = get_blocksize(data_block);
	if (data_block_size == 0)
		return false;

	/* always 3 indicating the presence of fp_timing+dvo_timing+panel_pnp_id */
	if (ptrs->num_entries != 3)
		return false;

	fp_timing_size = ptrs->ptr[0].fp_timing.table_size;
	dvo_timing_size = ptrs->ptr[0].dvo_timing.table_size;
	panel_pnp_id_size = ptrs->ptr[0].panel_pnp_id.table_size;
	panel_name_size = ptrs->panel_name.table_size;

	/* fp_timing has variable size */
	if (fp_timing_size < 32 ||
		dvo_timing_size != sizeof(struct bdb_edid_dtd) ||
		panel_pnp_id_size != sizeof(struct bdb_edid_pnp_id))
		return false;

	/* panel_name is not present in old VBTs */
	if (panel_name_size != 0 &&
		panel_name_size != sizeof(struct bdb_edid_product_name))
		return false;

	lfp_data_size = ptrs->ptr[1].fp_timing.offset - ptrs->ptr[0].fp_timing.offset;
	if (16 * lfp_data_size > data_block_size)
		return false;

	/* make sure the table entries have uniform size */
	for (i = 1; i < 16; i++) {
		if (ptrs->ptr[i].fp_timing.table_size != fp_timing_size ||
			ptrs->ptr[i].dvo_timing.table_size != dvo_timing_size ||
			ptrs->ptr[i].panel_pnp_id.table_size != panel_pnp_id_size)
			return false;

		if (ptrs->ptr[i].fp_timing.offset - ptrs->ptr[i-1].fp_timing.offset != lfp_data_size ||
			ptrs->ptr[i].dvo_timing.offset - ptrs->ptr[i-1].dvo_timing.offset != lfp_data_size ||
			ptrs->ptr[i].panel_pnp_id.offset - ptrs->ptr[i-1].panel_pnp_id.offset != lfp_data_size)
			return false;
	}

	/*
	 * Except for vlv/chv machines all real VBTs seem to have 6
	 * unaccounted bytes in the fp_timing table. And it doesn't
	 * appear to be a really intentional hole as the fp_timing
	 * 0xffff terminator is always within those 6 missing bytes.
	 */
	if (fp_timing_size + 6 + dvo_timing_size + panel_pnp_id_size == lfp_data_size)
		fp_timing_size += 6;

	if (fp_timing_size + dvo_timing_size + panel_pnp_id_size != lfp_data_size)
		return false;

	if (ptrs->ptr[0].fp_timing.offset + fp_timing_size != ptrs->ptr[0].dvo_timing.offset ||
		ptrs->ptr[0].dvo_timing.offset + dvo_timing_size != ptrs->ptr[0].panel_pnp_id.offset ||
		ptrs->ptr[0].panel_pnp_id.offset + panel_pnp_id_size != lfp_data_size)
		return false;

	/* make sure the tables fit inside the data block */
	for (i = 0; i < 16; i++) {
		if (ptrs->ptr[i].fp_timing.offset + fp_timing_size > data_block_size ||
			ptrs->ptr[i].dvo_timing.offset + dvo_timing_size > data_block_size ||
			ptrs->ptr[i].panel_pnp_id.offset + panel_pnp_id_size > data_block_size)
			return false;
	}

	if (ptrs->panel_name.offset + 16 * panel_name_size > data_block_size)
		return false;

	/* make sure fp_timing terminators are present at expected locations */
	for (i = 0; i < 16; i++) {
		 u16 *t = (u16 *)((u8 *)data_block + ptrs->ptr[i].fp_timing.offset +
		fp_timing_size - 2);

		if (*t != 0xffff)
			return false;
	}

	return true;
}
static bool fixup_lfp_data_ptrs(const void *bdb, void *ptrs_block)
{
	struct bdb_lfp_data_ptrs *ptrs = (struct bdb_lfp_data_ptrs *)ptrs_block;
	u32 offset;
	int i;

	offset = raw_block_offset(bdb, BDB_LFP_DATA);

	for (i = 0; i < 16; i++) {
		if (ptrs->ptr[i].fp_timing.offset < offset ||
			ptrs->ptr[i].dvo_timing.offset < offset ||
			ptrs->ptr[i].panel_pnp_id.offset < offset)
			return false;

		ptrs->ptr[i].fp_timing.offset -= offset;
		ptrs->ptr[i].dvo_timing.offset -= offset;
		ptrs->ptr[i].panel_pnp_id.offset -= offset;
	}

	if (ptrs->panel_name.table_size) {
		if (ptrs->panel_name.offset < offset)
			return false;

		ptrs->panel_name.offset -= offset;
	}

	return validate_lfp_data_ptrs(bdb, ptrs);
}

static void
init_bdb_block(struct intel_display *display,
		   const void *bdb, enum bdb_block_id section_id,
		   size_t min_size)
{
	struct bdb_block_entry *entry;
	struct bdb_lfp_data_ptrs *ptrs;
	void *temp_block = NULL;
	const void *block;
	size_t block_size;

	block = find_raw_section(bdb, section_id);

	if (!block && section_id == BDB_LFP_DATA_PTRS) {
		temp_block = generate_lfp_data_ptrs(display, bdb);
		if (temp_block)
			block = (u8 *)temp_block + 3;
	}
	if (!block)
		return;


	block_size = get_blocksize(block);


	if (section_id == BDB_MIPI_SEQUENCE && *(const u8 *)block >= 3)
		block_size += 5;
	
	
	entry = (struct bdb_block_entry*)IOMalloc(sizeof(struct bdb_block_entry) + max(min_size, block_size) + 3);
	/*entry = kzalloc_flex(*entry, data, max(min_size, block_size) + 3);
	if (!entry) {
		kfree(temp_block);
		return;
	}*/

	entry->section_id = section_id;
	memcpy(entry->data, (u8 *)block - 3, block_size + 3);

	//kfree(temp_block);
	IOFree(temp_block,sizeof(*ptrs) + 3);

	if (section_id == BDB_LFP_DATA_PTRS &&
		!fixup_lfp_data_ptrs(bdb, entry->data + 3)) {
		//kfree(entry);
		IOFree(entry->data,max(min_size, block_size) + 3);
		return;
	}

	list_add_tail(&entry->node, &display->vbt.bdb_blocks);
}





static void init_bdb_blocks(struct intel_display *display,
				const void *bdb)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(bdb_blocks); i++) {
		enum bdb_block_id section_id = bdb_blocks[i].section_id;
		size_t min_size = bdb_blocks[i].min_size;

		if (section_id == BDB_LFP_DATA)
			min_size = lfp_data_min_size(display);

		init_bdb_block(display, bdb, section_id, min_size);
	}
}

static const struct platform_desc *find_platform_desc()
{
	u32 i;

	for (i = 0; i < ARRAY_SIZE(intel_display_ids); i++) {
		if (intel_display_ids[i].devid == NBlue::callback->deviceId)
			return intel_display_ids[i].desc;
	}

	return NULL;
}

static const struct subplatform_desc *
find_subplatform_desc( const struct platform_desc *desc)
{
	const struct subplatform_desc *sp;
	const u16 *id;

	for (sp = desc->subplatforms; sp && sp->pciidlist; sp++)
		for (id = sp->pciidlist; *id; id++)
			if (*id == NBlue::callback->deviceId)
				return sp;

	return NULL;
}


#define __STEP_NAME(name) [STEP_##name] = #name,

static void initialize_step(struct intel_display *display, enum intel_step step)
{
	static const char step_names[][3] = {
		STEP_NAME_LIST(__STEP_NAME)
	};

	DISPLAY_RUNTIME_INFO(display)->step = step;

	if (step >= 0 && step < ARRAY_SIZE(step_names))
		strcpy(DISPLAY_RUNTIME_INFO(display)->step_name, step_names[step],sizeof(DISPLAY_RUNTIME_INFO(display)->step_name));
}

#undef __STEP_NAME

static enum intel_step get_pre_gmdid_step(
					  const struct stepping_desc *main,
					  const struct stepping_desc *sub)
{
	const enum intel_step *map = main->map;
	int size = (int)main->size;
	int revision = NBlue::callback->pciRevision;
	enum intel_step step;

	if (sub && sub->map && sub->size) {
		map = sub->map;
		size = (int)sub->size;
	}

	if (!map || !size)
		return STEP_NONE;

	if (revision < size && map[revision] != STEP_NONE) {
		step = map[revision];
	} else {

		while (revision < size && map[revision] == STEP_NONE)
			revision++;

		if (revision < size) {
			step = map[revision];
		} else {
			step = STEP_FUTURE;
		}
	}

	return step;
}



void __bitmap_or(unsigned long *dst, const unsigned long *bitmap1,
		 const unsigned long *bitmap2, int bits)
{
	int k;
	int nr = BITS_TO_LONGS(bits);

	for (k = 0; k < nr; k++)
		dst[k] = bitmap1[k] | bitmap2[k];
}

#define small_const_nbits(nbits) \
	(__builtin_constant_p(nbits) && (nbits) <= BITS_PER_LONG && (nbits) > 0)
static void bitmap_or(unsigned long *dst, const unsigned long *src1,
		   const unsigned long *src2, unsigned int nbits)
{
	if (small_const_nbits(nbits))
		*dst = *src1 | *src2;
	else
		__bitmap_or(dst, src1, src2, nbits);
}
static unsigned int display_platforms_num_bits(void)
{
	return sizeof(((struct intel_display_platforms *)0)->bitmap) * BITS_PER_BYTE;
}
static void display_platforms_or(struct intel_display_platforms *dst,
				 const struct intel_display_platforms *src)
{
	bitmap_or(dst->bitmap, dst->bitmap, src->bitmap, display_platforms_num_bits());
}

struct intel_display *intel_display_device_probe(struct intel_display *display)
{
	const struct intel_display_device_info *info;
	const struct platform_desc *desc;
	const struct subplatform_desc *subdesc;
	enum intel_step step;


	desc = find_platform_desc();

	info = desc->info;

	DISPLAY_INFO(display) = info;

	memcpy(DISPLAY_RUNTIME_INFO(display),
		   &DISPLAY_INFO(display)->__runtime_defaults,
		   sizeof(*DISPLAY_RUNTIME_INFO(display)));


	display->platform = desc->platforms;

	subdesc = find_subplatform_desc( desc);
	if (subdesc) {

		display_platforms_or(&display->platform, &subdesc->platforms);
	}

	step = get_pre_gmdid_step( &desc->step_info,
					  subdesc ? &subdesc->step_info : NULL);
	

	initialize_step(display, step);


	return display;

}

static enum intel_pch
intel_pch_type(unsigned short id)
{
	switch (id) {
	case INTEL_PCH_IBX_DEVICE_ID_TYPE:
		return PCH_IBX;
	case INTEL_PCH_CPT_DEVICE_ID_TYPE:
		return PCH_CPT;
	case INTEL_PCH_PPT_DEVICE_ID_TYPE:
		return PCH_CPT;
	case INTEL_PCH_LPT_DEVICE_ID_TYPE:
		return PCH_LPT_H;
	case INTEL_PCH_LPT_LP_DEVICE_ID_TYPE:
		return PCH_LPT_LP;
	case INTEL_PCH_WPT_DEVICE_ID_TYPE:
		return PCH_LPT_H;
	case INTEL_PCH_WPT_LP_DEVICE_ID_TYPE:
		return PCH_LPT_LP;
	case INTEL_PCH_SPT_DEVICE_ID_TYPE:
		return PCH_SPT;
	case INTEL_PCH_SPT_LP_DEVICE_ID_TYPE:
		return PCH_SPT;
	case INTEL_PCH_KBP_DEVICE_ID_TYPE:
		return PCH_SPT;
	case INTEL_PCH_CNP_DEVICE_ID_TYPE:
		return PCH_CNP;
	case INTEL_PCH_CNP_LP_DEVICE_ID_TYPE:
		return PCH_CNP;
	case INTEL_PCH_CMP_DEVICE_ID_TYPE:
	case INTEL_PCH_CMP2_DEVICE_ID_TYPE:
		return PCH_CNP;
	case INTEL_PCH_CMP_V_DEVICE_ID_TYPE:
		return PCH_SPT;
	case INTEL_PCH_ICP_DEVICE_ID_TYPE:
	case INTEL_PCH_ICP2_DEVICE_ID_TYPE:
		return PCH_ICP;
	case INTEL_PCH_MCC_DEVICE_ID_TYPE:
		return PCH_TGP;
	case INTEL_PCH_TGP_DEVICE_ID_TYPE:
	case INTEL_PCH_TGP2_DEVICE_ID_TYPE:
		return PCH_TGP;
	case INTEL_PCH_JSP_DEVICE_ID_TYPE:
		return PCH_ICP;
	case INTEL_PCH_ADP_DEVICE_ID_TYPE:
	case INTEL_PCH_ADP2_DEVICE_ID_TYPE:
	case INTEL_PCH_ADP3_DEVICE_ID_TYPE:
	case INTEL_PCH_ADP4_DEVICE_ID_TYPE:
		return PCH_ADP;
	default:
		return PCH_NONE;
	}
}

static u32 get_allowed_dc_mask(struct intel_display *display, int enable_dc)
{
	u32 mask;
	int requested_dc;
	int max_dc;


	if (DISPLAY_VER(display) >= 20)
		max_dc = 2;
	else if (display->platform.dg2)
		max_dc = 1;
	else if (display->platform.dg1)
		max_dc = 3;
	else if (DISPLAY_VER(display) >= 12)
		max_dc = 4;
	else if (display->platform.geminilake || display->platform.broxton)
		max_dc = 1;
	else if (DISPLAY_VER(display) >= 9)
		max_dc = 2;
	else
		max_dc = 0;


	mask = display->platform.geminilake || display->platform.broxton ||
		DISPLAY_VER(display) >= 11 ? DC_STATE_EN_DC9 : 0;

	//if (!display->params.disable_power_well)
	//	max_dc = 0;

	if (enable_dc >= 0 && enable_dc <= max_dc) {
		requested_dc = enable_dc;
	} else if (enable_dc == -1) {
		requested_dc = max_dc;
	} else if (enable_dc > max_dc && enable_dc <= 4) {
		requested_dc = max_dc;
	} else {
		requested_dc = max_dc;
	}

	switch (requested_dc) {
	case 4:
		mask |= DC_STATE_EN_DC3CO | DC_STATE_EN_UPTO_DC6;
		break;
	case 3:
		mask |= DC_STATE_EN_DC3CO | DC_STATE_EN_UPTO_DC5;
		break;
	case 2:
		mask |= DC_STATE_EN_UPTO_DC6;
		break;
	case 1:
		mask |= DC_STATE_EN_UPTO_DC5;
		break;
	}


	return mask;
}

static int intel_num_pps(struct intel_display *display)
{
	if (display->platform.valleyview || display->platform.cherryview)
		return 2;

	if (display->platform.geminilake || display->platform.broxton)
		return 2;

	if (INTEL_PCH_TYPE(display) >= PCH_MTL)
		return 2;

	if (INTEL_PCH_TYPE(display) >= PCH_DG1)
		return 1;

	if (INTEL_PCH_TYPE(display) >= PCH_ICP)
		return 2;

	return 1;
}
void intel_pps_unlock_regs_wa(struct intel_display *display)
{
	int pps_num;
	int pps_idx;

	
	pps_num = intel_num_pps(display);

	for (pps_idx = 0; pps_idx < pps_num; pps_idx++)
		NBlue::callback->intel_de_rmw(display, PP_CONTROL(display, pps_idx),
				 PANEL_UNLOCK_MASK, PANEL_UNLOCK_REGS);
}



static int intel_bios_ssc_frequency(struct intel_display *display,
					bool alternate)
{
	switch (DISPLAY_VER(display)) {
	case 2:
		return alternate ? 66667 : 48000;
	case 3:
	case 4:
		return alternate ? 100000 : 96000;
	default:
		return alternate ? 100000 : 120000;
	}
}

static void
parse_general_features(struct intel_display *display)
{
	const struct bdb_general_features *general;

	general = (const struct bdb_general_features *)bdb_find_section(display, BDB_GENERAL_FEATURES);
	if (!general)
		return;

	display->vbt.int_tv_support = general->int_tv_support;
	/* int_crt_support can't be trusted on earlier platforms */
	if (display->vbt.version >= 155 &&
		(HAS_DDI(display) || display->platform.valleyview))
		display->vbt.int_crt_support = general->int_crt_support;
	display->vbt.lvds_use_ssc = general->enable_ssc;
	display->vbt.lvds_ssc_freq =
		intel_bios_ssc_frequency(display, general->ssc_freq);
	display->vbt.display_clock_mode = general->display_clock_mode;
	display->vbt.fdi_rx_polarity_inverted = general->fdi_rx_polarity_inverted;
	if (display->vbt.version >= 181) {
		display->vbt.orientation = general->rotate_180 ?
			DRM_MODE_PANEL_ORIENTATION_BOTTOM_UP :
			DRM_MODE_PANEL_ORIENTATION_NORMAL;
	} else {
		display->vbt.orientation = DRM_MODE_PANEL_ORIENTATION_UNKNOWN;
	}

	if (display->vbt.version >= 249 && general->afc_startup_config) {
		display->vbt.override_afc_startup = true;
		display->vbt.override_afc_startup_val = general->afc_startup_config == 1 ? 0 : 7;
	}


}



static const struct gmbus_pin *get_gmbus_pin(struct intel_display *display,
						 unsigned int pin)
{
	const struct gmbus_pin *pins;
	size_t size;

	/*if (INTEL_PCH_TYPE(display) >= PCH_MTL) {
		pins = gmbus_pins_mtp;
		size = ARRAY_SIZE(gmbus_pins_mtp);
	} else if (INTEL_PCH_TYPE(display) >= PCH_DG2) {
		pins = gmbus_pins_dg2;
		size = ARRAY_SIZE(gmbus_pins_dg2);
	} else if (INTEL_PCH_TYPE(display) >= PCH_DG1) {
		pins = gmbus_pins_dg1;
		size = ARRAY_SIZE(gmbus_pins_dg1);
	} else */if (INTEL_PCH_TYPE(display) >= PCH_ICP) {
		pins = gmbus_pins_icp;
		size = ARRAY_SIZE(gmbus_pins_icp);
	}/* else if (HAS_PCH_CNP(display)) {
		pins = gmbus_pins_cnp;
		size = ARRAY_SIZE(gmbus_pins_cnp);
	} else if (display->platform.geminilake || display->platform.broxton) {
		pins = gmbus_pins_bxt;
		size = ARRAY_SIZE(gmbus_pins_bxt);
	} else if (DISPLAY_VER(display) == 9) {
		pins = gmbus_pins_skl;
		size = ARRAY_SIZE(gmbus_pins_skl);
	} else if (display->platform.broadwell) {
		pins = gmbus_pins_bdw;
		size = ARRAY_SIZE(gmbus_pins_bdw);
	} else {
		pins = gmbus_pins;
		size = ARRAY_SIZE(gmbus_pins);
	}*/

	if (pin >= size || !pins[pin].name)
		return NULL;

	return &pins[pin];
}
bool intel_gmbus_is_valid_pin(struct intel_display *display, unsigned int pin)
{
	return get_gmbus_pin(display, pin);
}

static int child_device_expected_size(u16 version)
{

	if (version > 264)
		return -ENOENT;
	else if (version >= 263)
		return 44;
	else if (version >= 256)
		return 40;
	else if (version >= 216)
		return 39;
	else if (version >= 196)
		return 38;
	else if (version >= 195)
		return 37;
	else if (version >= 111)
		return LEGACY_CHILD_DEVICE_CONFIG_SIZE;
	else if (version >= 106)
		return 27;
	else
		return 22;
}

static bool child_device_size_valid(struct intel_display *display, int size)
{
	int expected_size;

	expected_size = child_device_expected_size(display->vbt.version);
	if (expected_size < 0) {
		expected_size = sizeof(struct child_device_config);
	}


	if (size < LEGACY_CHILD_DEVICE_CONFIG_SIZE) {

		return false;
	}

	return true;
}

static const struct child_device_config *
child_device_ptr(const struct bdb_general_definitions *defs, int i)
{
	return (const struct child_device_config *) &defs->devices[i * defs->child_dev_size];
}

static void
parse_general_definitions(struct intel_display *display)
{
	const struct bdb_general_definitions *defs;
	struct intel_bios_encoder_data *devdata;
	const struct child_device_config *child;
	int i, child_device_num;
	u16 block_size;
	int bus_pin;

	defs = (const struct bdb_general_definitions *)bdb_find_section(display, BDB_GENERAL_DEFINITIONS);
	if (!defs) {
		return;
	}

	block_size = get_blocksize(defs);
	if (block_size < sizeof(*defs)) {
		return;
	}

	bus_pin = defs->crt_ddc_gmbus_pin;

	if (intel_gmbus_is_valid_pin(display, bus_pin))
		display->vbt.crt_ddc_pin = bus_pin;

	if (!child_device_size_valid(display, defs->child_dev_size))
		return;

	/* get the number of child device */
	child_device_num = (block_size - sizeof(*defs)) / defs->child_dev_size;

	for (i = 0; i < child_device_num; i++) {
		child = child_device_ptr(defs, i);
		if (!child->device_type)
			continue;

	//	drm_dbg_kms(display->drm,				"Found VBT child device with type 0x%x\n",				child->device_type);

		devdata = (struct intel_bios_encoder_data *)IOMalloc(sizeof(struct intel_bios_encoder_data));
		
		if (!devdata)
			break;

		devdata->display = display;

		memcpy(&devdata->child, child,
			   /*min_t(sizeof(size_t), defs->child_dev_size,*/ sizeof(*child));

		list_add_tail(&devdata->node, &display->vbt.display_devices);
	}

	/*if (list_empty(&display->vbt.display_devices))
		drm_dbg_kms(display->drm,
				"no child dev is parsed from VBT\n");*/
}

static void
parse_driver_features(struct intel_display *display)
{
	const struct bdb_driver_features *driver;

	driver = (const struct bdb_driver_features *)bdb_find_section(display, BDB_DRIVER_FEATURES);
	if (!driver)
		return;

	if (DISPLAY_VER(display) >= 5) {

		if (driver->lvds_config != BDB_DRIVER_FEATURE_INT_LVDS)
			display->vbt.int_lvds_support = 0;
	} else {

		if (display->vbt.version >= 134 &&
			driver->lvds_config != BDB_DRIVER_FEATURE_INT_LVDS &&
			driver->lvds_config != BDB_DRIVER_FEATURE_INT_SDVO_LVDS)
			display->vbt.int_lvds_support = 0;
	}
}

static void
parse_compression_parameters(struct intel_display *display)
{
	const struct bdb_compression_parameters *params;
	struct intel_bios_encoder_data *devdata;
	u16 block_size;
	int index;

	if (display->vbt.version < 198)
		return;

	params = (const struct bdb_compression_parameters *)bdb_find_section(display, BDB_COMPRESSION_PARAMETERS);
	if (params) {

		if (params->entry_size != sizeof(params->data[0])) {
			return;
		}

		block_size = get_blocksize(params);
		if (block_size < sizeof(*params)) {
			return;
		}
	}

	list_for_each_entry(devdata, &display->vbt.display_devices, node) {
		const struct child_device_config *child = &devdata->child;

		if (!child->compression_enable)
			continue;

		if (!params) {
			continue;
		}

		if (child->compression_method_cps) {
			continue;
		}

		index = child->compression_structure_index;

		devdata->dsc = (struct dsc_compression_parameters_entry *)IOMallocZero(sizeof(struct dsc_compression_parameters_entry));
		if (devdata->dsc) {
			memcpy(devdata->dsc, &params->data[index], sizeof(*devdata->dsc));
		}
		//devdata->dsc = kmemdup(&params->data[index],sizeof(*devdata->dsc), GFP_KERNEL);
	}
}

static bool is_port_valid(struct intel_display *display, enum port port)
{

	if (port == PORT_F && display->platform.icelake)
		return display->platform.icelake_port_f;

	return true;
}
enum port intel_bios_encoder_port(const struct intel_bios_encoder_data *devdata)
{
	struct intel_display *display = devdata->display;
	const struct child_device_config *child = &devdata->child;
	enum port port;

	port = dvo_port_to_port(display, child->dvo_port);
	if (port == PORT_NONE && DISPLAY_VER(display) >= 11)
		port = dsi_dvo_port_to_port(display, child->dvo_port);

	return port;
}
static void parse_ddi_port(struct intel_bios_encoder_data *devdata)
{
	struct intel_display *display = devdata->display;
	enum port port;

	port = intel_bios_encoder_port(devdata);
	if (port == PORT_NONE)
		return;

	if (!is_port_valid(display, port)) {
		return;
	}

	/*sanitize_dedicated_external(devdata, port);
	sanitize_device_type(devdata, port);
	sanitize_hdmi_level_shift(devdata, port);*/
}


static bool
intel_bios_encoder_supports_crt(const struct intel_bios_encoder_data *devdata)
{
	return devdata->child.device_type & DEVICE_TYPE_ANALOG_OUTPUT;
}

bool
intel_bios_encoder_supports_dvi(const struct intel_bios_encoder_data *devdata)
{
	return devdata->child.device_type & DEVICE_TYPE_TMDS_DVI_SIGNALING;
}

bool
intel_bios_encoder_supports_hdmi(const struct intel_bios_encoder_data *devdata)
{
	return intel_bios_encoder_supports_dvi(devdata) &&
		(devdata->child.device_type & DEVICE_TYPE_NOT_HDMI_OUTPUT) == 0;
}

bool
intel_bios_encoder_supports_dp(const struct intel_bios_encoder_data *devdata)
{
	return devdata->child.device_type & DEVICE_TYPE_DISPLAYPORT_OUTPUT;
}

bool
intel_bios_encoder_supports_edp(const struct intel_bios_encoder_data *devdata)
{
	return intel_bios_encoder_supports_dp(devdata) &&
		devdata->child.device_type & DEVICE_TYPE_INTERNAL_CONNECTOR;
}

bool
intel_bios_encoder_supports_dsi(const struct intel_bios_encoder_data *devdata)
{
	return devdata->child.device_type & DEVICE_TYPE_MIPI_OUTPUT;
}

bool
intel_bios_encoder_is_lspcon(const struct intel_bios_encoder_data *devdata)
{
	return devdata && HAS_LSPCON(devdata->display) && devdata->child.lspcon;
}
static void print_ddi_port(const struct intel_bios_encoder_data *devdata)
{
	struct intel_display *display = devdata->display;
	const struct child_device_config *child = &devdata->child;
	bool is_dvi, is_hdmi, is_dp, is_edp, is_dsi, is_crt, supports_typec_usb, supports_tbt;
	int dp_boost_level, dp_max_link_rate, hdmi_boost_level, hdmi_level_shift, max_tmds_clock;
	enum port port;

	port = intel_bios_encoder_port(devdata);
	if (port == PORT_NONE)
		return;

	is_dvi = intel_bios_encoder_supports_dvi(devdata);
	is_dp = intel_bios_encoder_supports_dp(devdata);
	is_crt = intel_bios_encoder_supports_crt(devdata);
	is_hdmi = intel_bios_encoder_supports_hdmi(devdata);
	is_edp = intel_bios_encoder_supports_edp(devdata);
	is_dsi = intel_bios_encoder_supports_dsi(devdata);

	//supports_typec_usb = intel_bios_encoder_supports_typec_usb(devdata);
	//supports_tbt = intel_bios_encoder_supports_tbt(devdata);
	
	enum phy phy = intel_port_to_phy(display, port);

	if (port == PORT_A) {
		display->hotplug.hpd_pin=tgl_hpd_pin(port);
		display->child0=child;
		display->port0=port;
		display->phy0=phy;
		display->pipe0=PIPE_A;
	}
	
	int ii=-1;
	for (int i = 0; i < 6; i++) {
		if (display->bconnectors[i].type==ConnectorDummy) {
			ii=i;
			break;
		}
	}
	
	if (ii!=-1) {
		
		ConnectorType type=ConnectorDummy;
		if (is_dp) type=ConnectorDP;
		if (is_hdmi) type=ConnectorHDMI;
		
		u32 flags=CNAlterAppertureRequirements;
		if (is_dp) flags+=CNFlagDP;
		if (is_edp) flags+=CNFlagInternalOverride;
		if (is_edp) flags+=CNUseMiscIoPowerWell;
		if (is_edp) flags+=CNFlagNoHPD;
		if (is_hdmi) flags+=CNFlagHDMI;
		
		display->bconnectors[ii].busId=child->ddc_pin;
		display->bconnectors[ii].pipe=port;
		display->bconnectors[ii].type=type;
		display->bconnectors[ii].flags=flags;
	}

	/*drm_dbg_kms(display->drm,
			"Port %c VBT info: CRT:%d DVI:%d HDMI:%d DP:%d eDP:%d DSI:%d DP++:%d LSPCON:%d USB-Type-C:%d TBT:%d DSC:%d\n",
			port_name(port), is_crt, is_dvi, is_hdmi, is_dp, is_edp, is_dsi,
			intel_bios_encoder_supports_dp_dual_mode(devdata),
			intel_bios_encoder_is_lspcon(devdata),
			supports_typec_usb, supports_tbt,
			devdata->dsc != NULL);*/

	/*if (intel_bios_encoder_is_dedicated_external(devdata))
		drm_dbg_kms(display->drm,
				"Port %c is dedicated external\n",
				port_name(port));

	if (intel_bios_encoder_supports_dyn_port_over_tc(devdata))
		drm_dbg_kms(display->drm,
				"Port %c supports dynamic DDI allocation in TCSS\n",
				port_name(port));

	hdmi_level_shift = intel_bios_hdmi_level_shift(devdata);
	if (hdmi_level_shift >= 0) {
		drm_dbg_kms(display->drm,
				"Port %c VBT HDMI level shift: %d\n",
				port_name(port), hdmi_level_shift);
	}

	max_tmds_clock = intel_bios_hdmi_max_tmds_clock(devdata);
	if (max_tmds_clock)
		drm_dbg_kms(display->drm,
				"Port %c VBT HDMI max TMDS clock: %d kHz\n",
				port_name(port), max_tmds_clock);


	 dp_boost_level = intel_bios_dp_boost_level(devdata);
	if (dp_boost_level)
		drm_dbg_kms(display->drm,
				"Port %c VBT (e)DP boost level: %d\n",
				port_name(port), dp_boost_level);

	hdmi_boost_level = intel_bios_hdmi_boost_level(devdata);
	if (hdmi_boost_level)
		drm_dbg_kms(display->drm,
				"Port %c VBT HDMI boost level: %d\n",
				port_name(port), hdmi_boost_level);

	dp_max_link_rate = intel_bios_dp_max_link_rate(devdata);
	if (dp_max_link_rate)
		drm_dbg_kms(display->drm,
				"Port %c VBT DP max link rate: %d\n",
				port_name(port), dp_max_link_rate);

	drm_WARN(display->drm, child->use_vbt_vswing,
		 "Port %c asks to use VBT vswing/preemph tables\n",
		 port_name(port));*/
}

static bool has_ddi_port_info(struct intel_display *display)
{
	return DISPLAY_VER(display) >= 5 || display->platform.g4x;
}

static void parse_ddi_ports(struct intel_display *display)
{
	struct intel_bios_encoder_data *devdata;

	if (!has_ddi_port_info(display))
		return;

	list_for_each_entry(devdata, &display->vbt.display_devices, node)
		parse_ddi_port(devdata);

	list_for_each_entry(devdata, &display->vbt.display_devices, node)
		print_ddi_port(devdata);
}

static void
parse_panel_options(struct intel_display *display,
			struct intel_panel *panel)
{
	const struct bdb_lfp_options *lfp_options;
	int panel_type = panel->vbt.panel_type;
	int drrs_mode;

	lfp_options = (const struct bdb_lfp_options *)bdb_find_section(display, BDB_LFP_OPTIONS);
	if (!lfp_options)
		return;

	panel->vbt.lvds_dither = lfp_options->pixel_dither;

	panel->vbt.panel_type=lfp_options->panel_type;
	
	if (get_blocksize(lfp_options) < 16)
		return;

	drrs_mode = panel_bits(lfp_options->dps_panel_type_bits,
				   panel_type, 2);

	switch (drrs_mode) {
	case 0:
		panel->vbt.drrs_type = DRRS_TYPE_STATIC;
		break;
	case 2:
		panel->vbt.drrs_type = DRRS_TYPE_SEAMLESS;
		break;
	default:
		panel->vbt.drrs_type = DRRS_TYPE_NONE;
		break;
	}
}

static void
parse_panel_driver_features(struct intel_display *display,
				struct intel_panel *panel)
{
	const struct bdb_driver_features *driver;

	driver = (const struct bdb_driver_features *)bdb_find_section(display, BDB_DRIVER_FEATURES);
	if (!driver)
		return;

	if (display->vbt.version < 228) {

		if (!driver->drrs_enabled && panel->vbt.drrs_type != DRRS_TYPE_NONE) {

			if (driver->dmrrs_enabled)
				panel->vbt.drrs_type = DRRS_TYPE_STATIC;
			else
				panel->vbt.drrs_type = DRRS_TYPE_NONE;
		}

		panel->vbt.psr.enable = driver->psr_enabled;
	}
}


static void
parse_power_conservation_features(struct intel_display *display,
				  struct intel_panel *panel)
{
	const struct bdb_lfp_power *power;
	u8 panel_type = panel->vbt.panel_type;

	panel->vbt.vrr = true; /* matches Windows behaviour */

	if (display->vbt.version < 228)
		return;

	power = (const struct bdb_lfp_power *)bdb_find_section(display, BDB_LFP_POWER);
	if (!power)
		return;

	panel->vbt.psr.enable = panel_bool(power->psr, panel_type);


	if (!panel_bool(power->drrs, panel_type) && panel->vbt.drrs_type != DRRS_TYPE_NONE) {

		if (panel_bool(power->dmrrs, panel_type))
			panel->vbt.drrs_type = DRRS_TYPE_STATIC;
		else
			panel->vbt.drrs_type = DRRS_TYPE_NONE;
	}

	if (display->vbt.version >= 232)
		panel->vbt.edp.hobl = panel_bool(power->hobl, panel_type);

	if (display->vbt.version >= 233)
		panel->vbt.vrr = panel_bool(power->vrr_feature_enabled,
						panel_type);
}

static void
parse_psr(struct intel_display *display,
	  struct intel_panel *panel)
{
	const struct bdb_psr *psr;
	const struct psr_table *psr_table;
	int panel_type = panel->vbt.panel_type;

	psr = (const struct bdb_psr *)bdb_find_section(display, BDB_PSR);
	if (!psr) {
		return;
	}

	psr_table = &psr->psr_table[panel_type];

	panel->vbt.psr.full_link = psr_table->full_link;
	panel->vbt.psr.require_aux_wakeup = psr_table->require_aux_to_wakeup;
	panel->vbt.psr.idle_frames = psr_table->idle_frames;


	if (display->vbt.version >= 205 &&
		(DISPLAY_VER(display) >= 9 && !display->platform.broxton)) {
		switch (psr_table->tp1_wakeup_time) {
		case 0:
			panel->vbt.psr.tp1_wakeup_time_us = 500;
			break;
		case 1:
			panel->vbt.psr.tp1_wakeup_time_us = 100;
			break;
		case 3:
			panel->vbt.psr.tp1_wakeup_time_us = 0;
			break;
		default:
		case 2:
			panel->vbt.psr.tp1_wakeup_time_us = 2500;
			break;
		}

		switch (psr_table->tp2_tp3_wakeup_time) {
		case 0:
			panel->vbt.psr.tp2_tp3_wakeup_time_us = 500;
			break;
		case 1:
			panel->vbt.psr.tp2_tp3_wakeup_time_us = 100;
			break;
		case 3:
			panel->vbt.psr.tp2_tp3_wakeup_time_us = 0;
			break;
		default:
		case 2:
			panel->vbt.psr.tp2_tp3_wakeup_time_us = 2500;
		break;
		}
	} else {
		panel->vbt.psr.tp1_wakeup_time_us = psr_table->tp1_wakeup_time * 100;
		panel->vbt.psr.tp2_tp3_wakeup_time_us = psr_table->tp2_tp3_wakeup_time * 100;
	}

	if (display->vbt.version >= 226) {
		u32 wakeup_time = psr->psr2_tp2_tp3_wakeup_time;

		wakeup_time = panel_bits(wakeup_time, panel_type, 2);
		switch (wakeup_time) {
		case 0:
			wakeup_time = 500;
			break;
		case 1:
			wakeup_time = 100;
			break;
		case 3:
			wakeup_time = 50;
			break;
		default:
		case 2:
			wakeup_time = 2500;
			break;
		}
		panel->vbt.psr.psr2_tp2_tp3_wakeup_time_us = wakeup_time;
	} else {
		panel->vbt.psr.psr2_tp2_tp3_wakeup_time_us = panel->vbt.psr.tp2_tp3_wakeup_time_us;
	}
}

void drm_mode_set_name(struct drm_display_mode *mode)
{
	bool interlaced = !!(mode->flags & DRM_MODE_FLAG_INTERLACE);

	snprintf(mode->name, DRM_DISPLAY_MODE_LEN, "%dx%d%s",
		 mode->hdisplay, mode->vdisplay,
		 interlaced ? "i" : "");
}

static void
parse_generic_dtd(struct intel_display *display,
		  struct intel_panel *panel)
{
	const struct bdb_generic_dtd *generic_dtd;
	const struct generic_dtd_entry *dtd;
	struct drm_display_mode *panel_fixed_mode;
	int num_dtd;

	if (display->vbt.version < 229)
		return;

	generic_dtd = (const struct bdb_generic_dtd *)bdb_find_section(display, BDB_GENERIC_DTD);
	if (!generic_dtd)
		return;

	if (generic_dtd->gdtd_size < sizeof(struct generic_dtd_entry)) {
		return;
	} else if (generic_dtd->gdtd_size !=
		   sizeof(struct generic_dtd_entry)) {
	}

	num_dtd = (get_blocksize(generic_dtd) -
		   sizeof(struct bdb_generic_dtd)) / generic_dtd->gdtd_size;
	if (panel->vbt.panel_type >= num_dtd) {
		return;
	}

	dtd = &generic_dtd->dtd[panel->vbt.panel_type];

	panel_fixed_mode = (struct drm_display_mode *)IOMalloc(sizeof(struct drm_display_mode));
	if (!panel_fixed_mode)
		return;

	panel_fixed_mode->hdisplay = dtd->hactive;
	panel_fixed_mode->hsync_start =
		panel_fixed_mode->hdisplay + dtd->hfront_porch;
	panel_fixed_mode->hsync_end =
		panel_fixed_mode->hsync_start + dtd->hsync;
	panel_fixed_mode->htotal =
		panel_fixed_mode->hdisplay + dtd->hblank;

	panel_fixed_mode->vdisplay = dtd->vactive;
	panel_fixed_mode->vsync_start =
		panel_fixed_mode->vdisplay + dtd->vfront_porch;
	panel_fixed_mode->vsync_end =
		panel_fixed_mode->vsync_start + dtd->vsync;
	panel_fixed_mode->vtotal =
		panel_fixed_mode->vdisplay + dtd->vblank;

	panel_fixed_mode->clock = dtd->pixel_clock;
	panel_fixed_mode->width_mm = dtd->width_mm;
	panel_fixed_mode->height_mm = dtd->height_mm;

	panel_fixed_mode->type = DRM_MODE_TYPE_PREFERRED;
	drm_mode_set_name(panel_fixed_mode);

	if (dtd->hsync_positive_polarity)
		panel_fixed_mode->flags |= DRM_MODE_FLAG_PHSYNC;
	else
		panel_fixed_mode->flags |= DRM_MODE_FLAG_NHSYNC;

	if (dtd->vsync_positive_polarity)
		panel_fixed_mode->flags |= DRM_MODE_FLAG_PVSYNC;
	else
		panel_fixed_mode->flags |= DRM_MODE_FLAG_NVSYNC;


	panel->vbt.lfp_vbt_mode = panel_fixed_mode;
}

static const struct bdb_edid_dtd *
get_lfp_dvo_timing(const struct bdb_lfp_data *data,
		   const struct bdb_lfp_data_ptrs *ptrs,
		   int index)
{
	return (const struct bdb_edid_dtd *)((u8*)data + ptrs->ptr[index].dvo_timing.offset);
}

static void
fill_detail_timing_data(struct intel_display *display,
			struct drm_display_mode *panel_fixed_mode,
			const struct bdb_edid_dtd *dvo_timing)
{
	panel_fixed_mode->hdisplay = (dvo_timing->hactive_hi << 8) |
		dvo_timing->hactive_lo;
	panel_fixed_mode->hsync_start = panel_fixed_mode->hdisplay +
		((dvo_timing->hsync_off_hi << 8) | dvo_timing->hsync_off_lo);
	panel_fixed_mode->hsync_end = panel_fixed_mode->hsync_start +
		((dvo_timing->hsync_pulse_width_hi << 8) |
			dvo_timing->hsync_pulse_width_lo);
	panel_fixed_mode->htotal = panel_fixed_mode->hdisplay +
		((dvo_timing->hblank_hi << 8) | dvo_timing->hblank_lo);

	panel_fixed_mode->vdisplay = (dvo_timing->vactive_hi << 8) |
		dvo_timing->vactive_lo;
	panel_fixed_mode->vsync_start = panel_fixed_mode->vdisplay +
		((dvo_timing->vsync_off_hi << 4) | dvo_timing->vsync_off_lo);
	panel_fixed_mode->vsync_end = panel_fixed_mode->vsync_start +
		((dvo_timing->vsync_pulse_width_hi << 4) |
			dvo_timing->vsync_pulse_width_lo);
	panel_fixed_mode->vtotal = panel_fixed_mode->vdisplay +
		((dvo_timing->vblank_hi << 8) | dvo_timing->vblank_lo);
	panel_fixed_mode->clock = dvo_timing->clock * 10;
	panel_fixed_mode->type = DRM_MODE_TYPE_PREFERRED;

	if (dvo_timing->hsync_positive)
		panel_fixed_mode->flags |= DRM_MODE_FLAG_PHSYNC;
	else
		panel_fixed_mode->flags |= DRM_MODE_FLAG_NHSYNC;

	if (dvo_timing->vsync_positive)
		panel_fixed_mode->flags |= DRM_MODE_FLAG_PVSYNC;
	else
		panel_fixed_mode->flags |= DRM_MODE_FLAG_NVSYNC;

	panel_fixed_mode->width_mm = (dvo_timing->himage_hi << 8) |
		dvo_timing->himage_lo;
	panel_fixed_mode->height_mm = (dvo_timing->vimage_hi << 8) |
		dvo_timing->vimage_lo;

	/* Some VBTs have bogus h/vsync_end values */
	if (panel_fixed_mode->hsync_end > panel_fixed_mode->htotal) {
		panel_fixed_mode->hsync_end = panel_fixed_mode->htotal;
	}
	if (panel_fixed_mode->vsync_end > panel_fixed_mode->vtotal) {
		panel_fixed_mode->vsync_end = panel_fixed_mode->vtotal;
	}

	drm_mode_set_name(panel_fixed_mode);
}

static const struct fp_timing *
get_lfp_fp_timing(const struct bdb_lfp_data *data,
		  const struct bdb_lfp_data_ptrs *ptrs,
		  int index)
{
	return (const struct fp_timing *)((u8*)data + ptrs->ptr[index].fp_timing.offset);
}
static void
parse_lfp_panel_dtd(struct intel_display *display,
			struct intel_panel *panel,
			const struct bdb_lfp_data *lfp_data,
			const struct bdb_lfp_data_ptrs *lfp_data_ptrs)
{
	const struct bdb_edid_dtd *panel_dvo_timing;
	const struct fp_timing *fp_timing;
	struct drm_display_mode *panel_fixed_mode;
	int panel_type = panel->vbt.panel_type;

	panel_dvo_timing = get_lfp_dvo_timing(lfp_data,
						  lfp_data_ptrs,
						  panel_type);

	panel_fixed_mode = (struct drm_display_mode *)IOMalloc(sizeof(struct drm_display_mode));
	if (!panel_fixed_mode)
		return;

	fill_detail_timing_data(display, panel_fixed_mode, panel_dvo_timing);

	panel->vbt.lfp_vbt_mode = panel_fixed_mode;

	fp_timing = get_lfp_fp_timing(lfp_data,
					  lfp_data_ptrs,
					  panel_type);

	/* check the resolution, just to be sure */
	if (fp_timing->x_res == panel_fixed_mode->hdisplay &&
		fp_timing->y_res == panel_fixed_mode->vdisplay) {
		panel->vbt.bios_lvds_val = fp_timing->lvds_reg_val;

	}
}

static const struct bdb_lfp_data_tail *
get_lfp_data_tail(const struct bdb_lfp_data *data,
		  const struct bdb_lfp_data_ptrs *ptrs)
{
	if (ptrs->panel_name.table_size)
		return (const struct bdb_lfp_data_tail *) ((u8*)data + ptrs->panel_name.offset);
	else
		return NULL;
}

static void
parse_lfp_data(struct intel_display *display,
		   struct intel_panel *panel)
{
	const struct bdb_lfp_data *data;
	const struct bdb_lfp_data_tail *tail;
	const struct bdb_lfp_data_ptrs *ptrs;
	int panel_type = panel->vbt.panel_type;

	ptrs = (const struct bdb_lfp_data_ptrs *)bdb_find_section(display, BDB_LFP_DATA_PTRS);
	if (!ptrs)
		return;

	data = (const struct bdb_lfp_data *)bdb_find_section(display, BDB_LFP_DATA);
	if (!data)
		return;

	if (!panel->vbt.lfp_vbt_mode)
		parse_lfp_panel_dtd(display, panel, data, ptrs);
	
	tail = get_lfp_data_tail(data, ptrs);
	if (!tail)
		return;

	if (display->vbt.version >= 188) {
		panel->vbt.seamless_drrs_min_refresh_rate =
			tail->seamless_drrs_min_refresh_rate[panel_type];

	}
}

static bool find_devid(u16 id, const u16 *p, unsigned int num)
{
	for (; num; num--, p++) {
		if (*p == id)
			return true;
	}

	return false;
}

static void intel_device_info_subplatform_init(struct drm_i915_private *i915)
{
	const struct intel_device_info *info = INTEL_INFO(i915);
	const struct intel_runtime_info *rinfo = RUNTIME_INFO(i915);
	const unsigned int pi = __platform_mask_index(rinfo, info->platform);
	const unsigned int pb = __platform_mask_bit(rinfo, info->platform);
	u16 devid = INTEL_DEVID(i915);
	u32 mask = 0;

	RUNTIME_INFO(i915)->platform_mask[pi] = BIT(pb);

	if (find_devid(devid, subplatform_uy_ids,
			   ARRAY_SIZE(subplatform_uy_ids))) {
		mask = BIT(INTEL_SUBPLATFORM_UY);
	} else if (find_devid(devid, subplatform_n_ids,
				ARRAY_SIZE(subplatform_n_ids))) {
		mask = BIT(INTEL_SUBPLATFORM_N);
	} else if (find_devid(devid, subplatform_rpl_ids,
				  ARRAY_SIZE(subplatform_rpl_ids))) {
		mask = BIT(INTEL_SUBPLATFORM_RPL);
		if (find_devid(devid, subplatform_rplu_ids,
				   ARRAY_SIZE(subplatform_rplu_ids)))
			mask |= BIT(INTEL_SUBPLATFORM_RPLU);
	}

	RUNTIME_INFO(i915)->platform_mask[pi] |= mask;
}

void intel_device_info_driver_create(struct drm_i915_private *i915,
					 u16 device_id,
					 const struct intel_device_info *match_info)
{
	struct intel_runtime_info *runtime;

	i915->__info = match_info;

	runtime = RUNTIME_INFO(i915);
	memcpy(runtime, &INTEL_INFO(i915)->__runtime, sizeof(*runtime));

	runtime->device_id = device_id;
}

int NBlue::intel_opregion_setup()
{
	if (i915b!=nullptr) return 0;
	
	i915b = (struct drm_i915_private*)IOMalloc(sizeof(struct drm_i915_private));
	i915b->display= (struct intel_display*)IOMalloc(sizeof(struct intel_display));
	i915b->gt[0]= (struct intel_gt*)IOMalloc(sizeof(struct intel_gt));
	i915b->display->dmc.dmc=(struct intel_dmc*)IOMalloc(sizeof(struct intel_dmc));
	
	struct drm_i915_private *i915=i915b;
	struct intel_gt *gt=to_gt(i915);
	struct intel_guc *guc = gt_to_guc(gt);
	struct intel_display *display=i915->display;
	struct intel_guc_ct *ct=&guc->ct;
	
	guc->ads_vma=(struct i915_vma*)IOMalloc(sizeof(struct i915_vma));
	ct->vma=(struct i915_vma*)IOMalloc(sizeof(struct i915_vma));
	guc->log.vma=(struct i915_vma*)IOMalloc(sizeof(struct i915_vma));
	guc->stage_desc_pool=(struct i915_vma*)IOMalloc(sizeof(struct i915_vma));
	guc->lrc_desc_pool_v69=(struct i915_vma*)IOMalloc(sizeof(struct i915_vma));
	
	guc->gt=gt;
	i915b->initok=false;
	
	int i;
	const struct intel_device_info *desc;
	for (i = 0; i < ARRAY_SIZE(intel_platform_ids); i++) {
		if (intel_platform_ids[i].devid == deviceId)
		{
			desc= intel_platform_ids[i].desc;
			break;
		}
	}
	if (!desc) panic ("intel_platform_ids");
		
	intel_device_info_driver_create(i915,deviceId,desc);
	intel_device_info_subplatform_init(i915);
	
	RUNTIME_INFO(i915)->media.ip = RUNTIME_INFO(i915)->graphics.ip;
	
	gt->i915 = i915;
	gt->name = "Primary GT";
	gt->info.engine_mask = INTEL_INFO(gt->i915)->platform_engine_mask;
	gt->wopcm.size = GEN11_WOPCM_SIZE;
	gt->type = GT_PRIMARY;
	
	struct intel_opregion *opregion = &display->opregion;
	display->dmc.dmc->display=display;
	
	unsigned short id = deviceId & INTEL_PCH_DEVICE_ID_MASK;
	display->pch_type = intel_pch_type( id);
	
	display->power.domains.allowed_dc_mask = get_allowed_dc_mask(display, true);
	display->power.domains.lock= IOSimpleLockAlloc();
	
	u32 asls, mboxes;
	char buf[sizeof(OPREGION_SIGNATURE)];
	int err = 0;
	const void *vbt = nullptr;
	u32 vbt_size = 0;
	
	asls = iGPU->configRead32(0xFC);
	
	if (asls == 0) {
		return -ENOMEM;
	}

	IOPhysicalAddress physAddr = asls;
	IOMemoryDescriptor *memDesc = IOMemoryDescriptor::withPhysicalAddress(
		physAddr, OPREGION_SIZE, kIODirectionInOut);
	
	IOMemoryMap *memMap = memDesc->map();
	mach_vm_address_t virtAddr = memMap->getVirtualAddress();
	
	memcpy(buf, (void *)virtAddr, sizeof(buf));
	if (memcmp(buf, OPREGION_SIGNATURE, 16)) {
		err = -EINVAL;
		goto err_out;
	}

	opregion->header = reinterpret_cast<struct opregion_header *>(virtAddr);
	mboxes = opregion->header->mboxes;
	
	if (mboxes & MBOX_ACPI) {
			opregion->acpi = (struct opregion_acpi *)((uint8_t *)virtAddr + OPREGION_ACPI_OFFSET);
			opregion->acpi->chpd = 1;
		}
	
		if (mboxes & MBOX_ASLE) {
			opregion->asle = (struct opregion_asle *)((uint8_t *)virtAddr + OPREGION_ASLE_OFFSET);
			opregion->asle->ardy = ASLE_ARDY_NOT_READY;
		}

		if (mboxes & MBOX_ASLE_EXT) {
			opregion->asle_ext = (struct opregion_asle_ext *)((uint8_t *)virtAddr + OPREGION_ASLE_EXT_OFFSET);
		}

		if (mboxes & MBOX_BACKLIGHT) {
		}

	if (opregion->header->over.major >= 2 && opregion->asle &&
		opregion->asle->rvda && opregion->asle->rvds) {
		resource_size_t rvda = opregion->asle->rvda;

		if (opregion->header->over.major > 2 ||
			opregion->header->over.minor >= 1) {
			rvda += asls;
		}

		IOMemoryDescriptor *rvdaDesc = IOMemoryDescriptor::withPhysicalAddress(
			rvda, opregion->asle->rvds, kIODirectionInOut);
		
		if (rvdaDesc) {
			IOMemoryMap *rvdaMap = rvdaDesc->map();
			if (rvdaMap) {
				vbt = reinterpret_cast<const void *>(rvdaMap->getVirtualAddress());
				vbt_size = opregion->asle->rvds;
				
				opregion->vbt = vbt;
				opregion->vbt_size = vbt_size;
				opregion->rvda = rvdaMap;
			}
			rvdaDesc->release();
		}
	}

	if (!opregion->vbt) {
		vbt = (void *)((uint8_t *)virtAddr + OPREGION_VBT_OFFSET);
		vbt_size = (mboxes & MBOX_ASLE_EXT) ?
			OPREGION_ASLE_EXT_OFFSET : OPREGION_SIZE;
		vbt_size -= OPREGION_VBT_OFFSET;
		
		opregion->vbt = vbt;
		opregion->vbt_size = vbt_size;
	}
	
	if (vbt) {
			const struct vbt_header *vbth = reinterpret_cast<struct vbt_header *>(const_cast<void *>(vbt));
			const char *base = static_cast<const char *>(vbt);
			const struct bdb_header *bdb = reinterpret_cast<const struct bdb_header *>(base + vbth->bdb_offset);

			if (bdb) {
				
				for (int i = 0; i < 6; i++) {
					display->bconnectors[i].index=i;
					display->bconnectors[i].busId=0;
					display->bconnectors[i].pipe=0;
					display->bconnectors[i].pad=0;
					display->bconnectors[i].type=ConnectorDummy;
					display->bconnectors[i].flags=0;
				}

				display->pps.mmio_base = PCH_PPS_BASE;
				display->pps.mutex=IOSimpleLockAlloc();
				display->panel.pps.mmio_base = PCH_PPS_BASE;
				display->vbt.version = bdb->version;
				display->bdb=bdb;
				display->vbt.crt_ddc_pin = GMBUS_PIN_VGADDC;
				
				struct intel_dp *intel_dp=&display->intel_dp0;
				struct intel_crtc_state *crtc_state=&display->crtc_state0;
				struct intel_panel *panel=&display->panel;
				INIT_LIST_HEAD(&display->vbt.display_devices);
				INIT_LIST_HEAD(&display->vbt.bdb_blocks);
				
				intel_display_device_probe(display);
				//intel_pps_unlock_regs_wa(display);
				
				init_bdb_blocks(display, bdb);
				
				parse_general_features(display);
				parse_general_definitions(display);
				parse_driver_features(display);
				parse_compression_parameters(display);
				parse_ddi_ports(display);
				
				intel_dp->output_reg = DDI_BUF_CTL(display->port0);
				
				if (HAS_TRANSCODER(display, TRANSCODER_EDP) && display->port0 == PORT_A)
					crtc_state->cpu_transcoder = TRANSCODER_EDP;
				else
					crtc_state->cpu_transcoder = (enum transcoder) display->pipe0;
				
				//crtc_state->master_transcoder = INVALID_TRANSCODER;
				
				parse_panel_options(display, panel);
				parse_generic_dtd(display, panel);
				parse_lfp_data(display, panel);
				parse_panel_driver_features(display, panel);
				parse_power_conservation_features(display, panel);
				parse_edp(display, panel);
				parse_psr(display, panel);
				
				//intel_panel_add_edid_fixed_modes(connector, true);
				
				intel_dp->lane_count = display->panel.vbt.edp.lanes;
				crtc_state->lane_count = display->panel.vbt.edp.lanes;
				crtc_state->dither=display->panel.vbt.lvds_dither;
				
				
				//intel_dp_init_connector
				
			}
		}
	

err_out:
	if (memMap) memMap->release();
	if (memDesc) memDesc->release();
	
	return err;
}

void NBlue::parse_backlight()
{
	parse_lfp_backlight();
}

UInt32 NBlue::readReg32(unsigned long reg) {
	
	//IOSimpleLockLock(nlock);
	
	if (Gen11::callback) {
		//IOSimpleLockUnlock(nlock);
		if (rmmioPtr==nullptr) {
			rmmioPtr= Gen11::callback->rmmioPtr;
			rmmioLen= Gen11::callback->rmmioLen;
		}
	}
	
	if (reg < rmmioLen-4) {
		auto ret= *(uint *)((long)rmmioPtr + reg);
		//IOSimpleLockUnlock(nlock);
		return ret;
	} else {
		auto ret=  0;
		//IOSimpleLockUnlock(nlock);
		return ret;
	}
}

void NBlue::writeReg32(unsigned long reg, UInt32 val) {
	
	//IOSimpleLockLock(nlock);
	
	if (Gen11::callback) {
		//IOSimpleLockUnlock(nlock);
		if (rmmioPtr==nullptr) {
			rmmioPtr= Gen11::callback->rmmioPtr;
			rmmioLen= Gen11::callback->rmmioLen;
		}
	}
	
	if (reg < rmmioLen-4) {
		*(uint *)((long)rmmioPtr + reg) = val;
	}
	//IOSimpleLockUnlock(nlock);
}

uint32_t NBlue::intel_de_rmw(struct intel_display *display, uint32_t reg, uint32_t clear, uint32_t set)
{
	uint32_t old, val;
	old = readReg32( reg);
	val = (old & ~clear) | set;
	writeReg32( reg, val);
	return old;
}

uint32_t NBlue::intel_de_read(struct intel_display *display, uint32_t reg)
{
	return readReg32(reg);
}
void NBlue::intel_de_write(struct intel_display *display, uint32_t reg, uint32_t val)
{
	writeReg32( reg, val);
}
void NBlue::intel_de_posting_read(struct intel_display *display, uint32_t reg)
{
	readReg32(reg);
}
