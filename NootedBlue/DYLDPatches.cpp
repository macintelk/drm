//! Copyright © 2022-2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.5.
//! See LICENSE for details.

#include "DYLDPatches.hpp"
#include "kern_nblue.hpp"
#include <Headers/kern_api.hpp>
#include <Headers/kern_devinfo.hpp>
#include <IOKit/IODeviceTreeSupport.h>

DYLDPatches *DYLDPatches::callback = nullptr;

void DYLDPatches::init() {
	callback = this;
}

void DYLDPatches::processPatcher(KernelPatcher &patcher) {

	//return;
	
	auto *entry = IORegistryEntry::fromPath("/", gIODTPlane);
	if (entry) {
		DBGLOG("DYLD", "Setting hwgva-id to iMacPro1,1");
		entry->setProperty("hwgva-id", const_cast<char *>(kHwGvaId), arrsize(kHwGvaId));
		OSSafeReleaseNULL(entry);
	}

	KernelPatcher::RouteRequest request {"_cs_validate_page", wrapCsValidatePage, this->orgCsValidatePage};

	SYSLOG_COND(!patcher.routeMultipleLong(KernelPatcher::KernelID, &request, 1), "DYLD",
		"Failed to route kernel symbols");
}


void DYLDPatches::wrapCsValidatePage(vnode *vp, memory_object_t pager, memory_object_offset_t page_offset,
	const void *data, int *validated_p, int *tainted_p, int *nx_p) {
	FunctionCast(wrapCsValidatePage, callback->orgCsValidatePage)(vp, pager, page_offset, data, validated_p, tainted_p,
		nx_p);

	char path[PATH_MAX];
	int pathlen = PATH_MAX;
	if (vn_getpath(vp, path, &pathlen) != 0) { return; }
	

	
	if (!UserPatcher::matchSharedCachePath(path)) {
		
		if ((!strncmp(path, binarydisplaypolicyd, arrsize(binarydisplaypolicyd))) )
		{
			const DYLDPatch patches[] = {
				{displaypolicydf3, displaypolicydr3, "displaypolicyd edid read"},
			};
			DYLDPatch::applyAll(patches, const_cast<void *>(data), PAGE_SIZE);
			return;
		}
		
	    if ((!strncmp(path, kCoreLSKDMSEPath, arrsize(kCoreLSKDMSEPath))) ||
			(!strncmp(path, kCoreLSKDPath, arrsize(kCoreLSKDPath)))) {
			const DYLDPatch patch = {kCoreLSKDOriginal, kCoreLSKDPatched, "CoreLSKD streaming CPUID to Haswell"};
			patch.apply(const_cast<void *>(data), PAGE_SIZE);
			return;
		}
		

			int ok=0;
			int sle=0;
			vnode_t vnode = NULLVP;
			vfs_context_t ctxt = vfs_context_create(nullptr);
			errno_t err = vnode_lookup(TGLGraphicsMTLDriver2, 0, &vnode, ctxt);
			if (!err) vnode_put(vnode);
			vfs_context_rele(ctxt);
			if (!err) ok=1;
			
			vnode = NULLVP;
			ctxt = vfs_context_create(nullptr);
			err = vnode_lookup(TGLGraphicsMTLDriver, 0, &vnode, ctxt);
			if (!err) vnode_put(vnode);
			vfs_context_rele(ctxt);
			if (!err) { ok=1; sle=1;}
			
			
			if (!ok && NBlue::callback->iclid) {
				if ((!strncmp(path, libMTLIGCCompilerPluginPath, arrsize(libMTLIGCCompilerPluginPath)) )) {
					const DYLDPatch patches[] = {
						{klibMTLIGCCompilerPluginOriginal, klibMTLIGCCompilerPluginPatched, "klibMTLIGCCompilerPluginOriginal"},
						{klibMTLIGCCompilerPluginOriginal4, klibMTLIGCCompilerPluginPatched4, "klibMTLIGCCompilerPluginOriginal4"},
						{klibMTLIGCCompilerPluginOriginal5, klibMTLIGCCompilerPluginPatched5, "klibMTLIGCCompilerPluginOriginal5"},
						{klibMTLIGCCompilerPluginOriginal6b, klibMTLIGCCompilerPluginPatched6b, "klibMTLIGCCompilerPluginOriginal6b"},
						{klibMTLIGCCompilerPluginOriginal7, klibMTLIGCCompilerPluginPatched7, "klibMTLIGCCompilerPluginOriginal7"},
					};
					DYLDPatch::applyAll(patches, const_cast<void *>(data), PAGE_SIZE);
					return;
				}
			}
			
			if (ok && NBlue::callback->tglid) {
				if ((!strncmp(path, libMTLIGCCompilerPluginPath, arrsize(libMTLIGCCompilerPluginPath)) )) {
					const DYLDPatch patches[] = {
						{klibMTLIGCCompilerPluginOriginal, klibMTLIGCCompilerPluginPatched, "klibMTLIGCCompilerPluginOriginal"},
						{klibMTLIGCCompilerPluginOriginal2, sle ? klibMTLIGCCompilerPluginPatched2:klibMTLIGCCompilerPluginPatched3, "klibMTLIGCCompilerPluginOriginal2"},
						{klibMTLIGCCompilerPluginOriginal4, klibMTLIGCCompilerPluginPatched4, "klibMTLIGCCompilerPluginOriginal4"},
						{klibMTLIGCCompilerPluginOriginal5, klibMTLIGCCompilerPluginPatched5, "klibMTLIGCCompilerPluginOriginal5"},
						{klibMTLIGCCompilerPluginOriginal6, klibMTLIGCCompilerPluginPatched6, "klibMTLIGCCompilerPluginOriginal6"},
						{klibMTLIGCCompilerPluginOriginal7, klibMTLIGCCompilerPluginPatched7, "klibMTLIGCCompilerPluginOriginal7"},
					};
					DYLDPatch::applyAll(patches, const_cast<void *>(data), PAGE_SIZE);
					return;
				}
			}
		
		
		return;
	}

	

		if (UNLIKELY(KernelPatcher::findAndReplace(const_cast<void *>(data), PAGE_SIZE, kVideoToolboxDRMModelOriginal,
												   arrsize(kVideoToolboxDRMModelOriginal), BaseDeviceInfo::get().modelIdentifier, 20))) {
			DBGLOG("DYLD", "Applied 'VideoToolbox DRM model check' patch");
		}
		
		const DYLDPatch patches[] = {
			{kAGVABoardIdOriginal, kAGVABoardIdPatched, "iMacPro1,1 spoof (AppleGVA)"},
			{kHEVCEncBoardIdOriginal, kHEVCEncBoardIdPatched, "iMacPro1,1 spoof (AppleGVAHEVCEncoder)"},
		};
		DYLDPatch::applyAll(patches, const_cast<void *>(data), PAGE_SIZE);
		
	
	
}
