//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#pragma once

typedef unsigned int  IOSelect;
//#include "kern_netdbg.hpp"
#include <Headers/kern_patcher.hpp>
#include <Headers/kern_iokit.hpp>
#include <Headers/kern_util.hpp>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/graphics/IOFramebuffer.h>
#include <IOKit/acpi/IOACPIPlatformExpert.h>
#include "intel_vbt_defs.h"


class AppleACPIPlatformExpert : IOACPIPlatformExpert {
	friend class NBlue;
};


/*
class EXPORT PRODUCT_NAME : public IOService {
	OSDeclareDefaultStructors(PRODUCT_NAME);

	public:
	IOService *probe(IOService *provider, SInt32 *score) override;
	bool start(IOService *provider) override;
};*/



class NBlue {
    friend class Gen11;
	friend class DYLDPatches;

    public:
	
	struct drm_i915_private *i915b;
    static NBlue *callback;
	
	IOPCIDevice *iGPU {nullptr};
	
	uint32_t deviceId {0};
	uint16_t revision {0};
	uint32_t pciRevision {0};
	bool tglid;
	bool iclid;
	u8 edid[0x80];
	bool edidok;
	
	volatile UInt32 *rmmioPtr {nullptr};
	unsigned long rmmioLen;
	
    void init();
    void processPatcher(KernelPatcher &patcher);
    bool processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);
	
	IOSimpleLock *nlock;
	
	void parse_backlight();
	int intel_opregion_setup();
	UInt32 readReg32(unsigned long reg);
	void writeReg32(unsigned long reg, UInt32 val);
	uint32_t intel_de_rmw(struct intel_display *display, uint32_t reg, uint32_t clear, uint32_t set);
	uint32_t intel_de_read(struct intel_display *display, uint32_t reg);
	void intel_de_write(struct intel_display *display, uint32_t reg, uint32_t val);
	void intel_de_posting_read(struct intel_display *display, uint32_t reg);
	
	static uint16_t configRead16(IORegistryEntry *service, uint32_t space, uint8_t offset);
	static uint32_t configRead32(IORegistryEntry *service, uint32_t space, uint8_t offset);
	WIOKit::t_PCIConfigRead16 orgConfigRead16 {nullptr};
	WIOKit::t_PCIConfigRead32 orgConfigRead32 {nullptr};
	
	
	static size_t wrapFunctionReturnZero();
	
	mach_vm_address_t orgApplePanelSetDisplay {0};
	static bool wrapApplePanelSetDisplay(IOService *that, IODisplay *display);
	
	static uint64_t lset_id_mode(void *that,uint param_1,uint param_2);
	mach_vm_address_t olset_id_mode {0};
	
	static uint64_t lset_id_mode2(void *that,uint param_1,uint param_2);
	mach_vm_address_t olset_id_mode2 {0};
	
//private:
	
	static bool wrapAddDrivers(void* const self, OSArray* const array, const bool doNubMatching);
	mach_vm_address_t orgAddDrivers{0};
	
    
	
};


// Change frame-buffer count >= 2 check to >= 1.
static const UInt8 kAGDPFBCountCheckOriginal[] = {0x02, 0x00, 0x00, 0x83, 0xF8, 0x02};
static const UInt8 kAGDPFBCountCheckPatched[] = {0x02, 0x00, 0x00, 0x83, 0xF8, 0x01};

// Ditto
static const UInt8 kAGDPFBCountCheckOriginal13[] = {0x41, 0x83, 0xBE, 0x14, 0x02, 0x00, 0x00, 0x02};
static const UInt8 kAGDPFBCountCheckPatched13[] = {0x41, 0x83, 0xBE, 0x14, 0x02, 0x00, 0x00, 0x01};

// Neutralise access to AGDP configuration by board identifier.
static const UInt8 kAGDPBoardIDKeyOriginal[] = "board-id";
static const UInt8 kAGDPBoardIDKeyPatched[] = "applehax";

