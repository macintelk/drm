//  Copyright © 2023 ChefKiss Inc. Licensed under the Thou Shalt Not Profit License version 1.0. See LICENSE for
//  details.

#ifndef kern_gen11_hpp
#define kern_gen11_hpp
#include "kern_nblue.hpp"
#include "kern_patcherplus.hpp"
#include <Headers/kern_util.hpp>



static constexpr uint32_t ICL_REG_CDCLK_CTL = 0x46000;

static constexpr uint32_t ICL_REG_DSSM = 0x51004;


enum ICLReferenceClockFrequency {
	
	// 24 MHz
	ICL_REF_CLOCK_FREQ_24_0 = 0x0,
	
	// 19.2 MHz
	ICL_REF_CLOCK_FREQ_19_2 = 0x1,
	
	// 38.4 MHz
	ICL_REF_CLOCK_FREQ_38_4 = 0x2
};


enum ICLCoreDisplayClockDecimalFrequency {
	
	// 172.8 MHz
	ICL_CDCLK_FREQ_172_8 = 0x158,
	
	// 180 MHz
	ICL_CDCLK_FREQ_180_0 = 0x166,
	
	// 192 MHz
	ICL_CDCLK_FREQ_192_0 = 0x17E,
	
	// 307.2 MHz
	ICL_CDCLK_FREQ_307_2 = 0x264,
	
	// 312 MHz
	ICL_CDCLK_FREQ_312_0 = 0x26E,
	
	// 552 MHz
	ICL_CDCLK_FREQ_552_0 = 0x44E,
	
	// 556.8 MHz
	ICL_CDCLK_FREQ_556_8 = 0x458,
	
	// 648 MHz
	ICL_CDCLK_FREQ_648_0 = 0x50E,
	
	// 652.8 MHz
	ICL_CDCLK_FREQ_652_8 = 0x518
};

static constexpr uint32_t ICL_CDCLK_DEC_FREQ_THRESHOLD = ICL_CDCLK_FREQ_648_0;


static constexpr uint32_t ICL_CDCLK_PLL_FREQ_REF_24_0 = 24000000 * 54;

static constexpr uint32_t ICL_CDCLK_PLL_FREQ_REF_19_2 = 19200000 * 68;

static constexpr uint32_t ICL_CDCLK_PLL_FREQ_REF_38_4 = 38400000 * 34;
#define  CDCLK_FREQ_DECIMAL_MASK	(0x7ff)

struct PACKED VoltageConfig {
	uint64_t pad;
	uint8_t* voltageData;
};

struct AGDCFBOnline_t {
	int32_t                 id;
	uint32_t                state;
};

typedef enum portconfig_cflags {
	CNAlterAppertureRequirements=1,
	CNFlagSecondary=2,
	CNFlagForceEDID=4,
	CNConnectorAlwaysConnected=8,
	CNSupport32BPP=16,
	CNFlagLegacy=32,
	CNDisableBlitTranslationTable=64,
	CNUseMiscIoPowerWell=128,
	CNUsePowerWell2=256,
	CNFlagHDMI=512,
	CNFlagDP=1024,
	CNIncreaseLaneCount=2048,
	CNFlagDualLink=4096,
	CNFlagInternalOverride=8192,
	CNFlagEncoderSpecific=16384,
	CNFlagNoHPD=32768
} portconfig_cflags;

union ConnectorFlags {
	struct PACKED ConnectorFlagBits {
		/* 0x1 */
		/* Lets aperture memory to be not required AppleIntelFramebuffer::isApertureMemoryRequired */
		uint8_t CNAlterAppertureRequirements :1;  /* 0x1 */
		
		/* 0x2 - Fixed Name */
		/* Commonly indicates a secondary port or auxiliary connector. */
		uint8_t CNFlagSecondary             :1;  /* 0x2 */
		
		/* 0x4 - Fixed Name */
		/* Forces the driver to assume an EDID is present or validate strictly. */
		uint8_t CNFlagForceEDID             :1;  /* 0x4 */
		
		/* 0x8 */
		/* Normally set for LVDS displays (i.e. built-in displays) */
		uint8_t CNConnectorAlwaysConnected   :1;  /* 0x8 */
		
		/* 0x10 - Fixed Name */
		/* AppleIntelFramebuffer::maxSupportedDepths checks this and returns 2 IODisplayModeInformation::maxDepthIndex.
		 * This enables 32-bit color depth support (High Color). */
		uint8_t CNSupport32BPP              :1;  /* 0x10 */
		
		/* 0x20 - Fixed Name */
		/* Often indicates legacy port or specific audio enablement requirements on older generations. */
		uint8_t CNFlagLegacy                :1;  /* 0x20 */
		
		/* 0x40 */
		/* Disable blit translation table? AppleIntelFramebufferController::ConfigureBufferTranslation */
		uint8_t CNDisableBlitTranslationTable:1;  /* 0x40 */
		
		/* 0x80 */
		/* Used in AppleIntelFramebufferController::setPowerWellState */
		/* Activates MISC IO power well (SKL_DISP_PW_MISC_IO) */
		uint8_t CNUseMiscIoPowerWell        :1;  /* 0x80 */
		
		/* 0x100 */
		/* Used in AppleIntelFramebufferController::setPowerWellState */
		/* Activates Power Well 2 usage (SKL_PW_CTL_IDX_PW_2) */
		uint8_t CNUsePowerWell2             :1;  /* 0x100 */
		
		/* 0x200 - Fixed Name */
		/* Detected in patchConnectorType logic. Indicates HDMI connector type. */
		uint8_t CNFlagHDMI                  :1;  /* 0x200 */
		
		/* 0x400 - Fixed Name */
		/* Detected in patchConnectorType logic. Indicates DisplayPort connector type. */
		uint8_t CNFlagDP                    :1;  /* 0x400 */
		
		/* 0x800 */
		/* Sets fAvailableLaneCount to 30 instead of 20 when specified. Required for 4K resolutions. */
		uint8_t CNIncreaseLaneCount         :1;  /* 0x800 */
		
		/* 0x1000 - Fixed Name */
		/* Commonly associated with Dual-Link DVI or high-bandwidth modes. */
		uint8_t CNFlagDualLink              :1;  /* 0x1000 */
		
		/* 0x2000 - Fixed Name */
		/* Often set for ports that require specific hot-plug handling or are defined as "Internal" on newer platforms. */
		uint8_t CNFlagInternalOverride      :1;  /* 0x2000 */
		
		/* 0x4000 - Fixed Name */
		/* Sometimes associated with specific encoder types (e.g., FDI or specific Transcoder links). */
		uint8_t CNFlagEncoderSpecific       :1;  /* 0x4000 */
		
		/* 0x8000 - Fixed Name */
		/* Indicates that Hot Plug Detect (HPD) is not connected or should be ignored. */
		uint8_t CNFlagNoHPD                 :1;  /* 0x8000 */
		
		uint16_t CNUnknownZeroFlags;
	} bits;
	uint32_t value;
};


struct PACKED ConnectorEntry {
	uint32_t index;
	uint32_t busId;
	uint32_t pipe;
	uint32_t pad;
	uint32_t type;
	uint32_t flags;
};

struct PACKED PlatformInfo {

		uint32_t fInfoPlatformID;
		uint32_t fPchType;
		uint64_t* fModelNameAddr;
		uint8_t fMobile;
		uint8_t fPipeCount;
		uint8_t fInfoPortCount;
		uint8_t fInfoFramebufferCount;
		uint32_t fInfoFramebufferMemorySize;
		uint32_t fInfoFBCompressionMemorySize;
		uint32_t fUnifiedMemorySize;
		struct ConnectorEntry connectors[0x9];
		uint32_t fInfoFlags;
		uint32_t pad0;
		struct VoltageConfig voltages[0x9];
		uint32_t cameliav;
		uint32_t MCLK;
		uint32_t VCLK;
		uint32_t TCONfbindex;
		uint32_t BeaconLoc;
		uint32_t MinFrmTim;
		uint32_t MaxFrmTim;
		uint32_t fInfoDynamicFBCThreshold;
		uint32_t fVideoTurboFreq;
		uint32_t fSliceCount;
		uint32_t fmaxEuCount;
		uint32_t fsubslices;

};

union FramebufferFlags {
	struct FramebufferFlagBits {
		/* Discovered in AppleIntelFBController::LinkTraining. Disables the use of FastLinkTraining.
		 * According to joevt with zero SKL link training happens at 450 MHz else at 540 MHz */
		uint8_t FBAvoidFastLinkTraining      :1;  /* 0x1 */
		uint8_t FBUnknownFlag_2              :1;  /* 0x2 */
		/* Discovered in AppleIntelFramebufferController::getFeatureControl.
		 * This is equivalent to setting FBC=1 in the plist FeatureControl section. */
		uint8_t FBFramebufferCompression     :1;  /* 0x4 */
		/* Discovered in AppleIntelFramebufferController::getFeatureControl.
		 * This is equivalent to setting SliceSDEnable=1, EUSDEnable=1, DynamicSliceSwitch=1 in the plist FeatureControl section.
		 */
		uint8_t FBEnableSliceFeatures        :1;  /* 0x8 */
		/* Discovered in AppleIntelFramebufferController::getFeatureControl.
		 * This is equivalent to setting DynamicFBCEnable=1 in the plist FeatureControl section.
		 */
		uint8_t FBDynamicFBCEnable           :1;  /* 0x10 */
		/* This sets fUseVideoTurbo=1 and loads GPU turbo frequency from the specific field.
		 * Defaults to 14, can be overridden by VideoTurboFreq in the plist FeatureControl section.
		 */
		uint8_t FBUseVideoTurbo              :1;  /* 0x20 */
		/* Discovered in AppleIntelFramebuffer::getDisplayStatus.
		 * Enforces display power reset even on always connected displays (see connector flags CNConnectorAlwaysConnected).
		 */
		uint8_t FBForcePowerAlwaysConnected  :1;  /* 0x40 */
		/* According to joevt this enforces High Bitrate mode 1, which limits DP bitrate to 8.64 Gbit/s instead of normal 17.28 Gbit/s (HBR2).
		 * I do not think this is used on Skylake any longer.
		 */
		uint8_t FBDisableHighBitrateMode2    :1;  /* 0x80 */
		/* This bit is not used on Broadwell and newer but set when fPortCount > 0, i.e. for all online framebuffers.
		 * On Haswell it is used by AppleIntelFramebuffer::GetOnlineInfo and is set only on 0x0D260007 (MacBookPro11,3) and 0x0D26000E, which are top models.
		 * This appears to boost pixel frequency limit (aka pixel clock) to 540000000 Hz (from the usual 216000000, 320000000, 360000000, 450000000).
		 */
		uint8_t FBBoostPixelFrequencyLimit   :1;  /* 0x100 */
		/* Discovered in AppleIntelFramebuffer::ValidateSourceSize.
		 * Limits source size to 4096x4096.
		 */
		uint8_t FBLimit4KSourceSize          :1;  /* 0x200 */
		/* Discovered in AppleIntelFramebufferController::start.
		 * These bits appear to be entirely equivalent and could be used interchangeably. Result in setting:
		 * - PCH_LP_PARTITION_LEVEL_DISABLE (1 << 12) bit in SOUTH_DSPCLK_GATE_D (0xc2020)
		 * - LPT_PWM_GRANULARITY (1 << 5) bit in SOUTH_CHICKEN2 (0xc2004)
		 * See Linux driver sources (lpt_init_clock_gating, lpt_enable_backlight).
		 * Since these bits are setting backlight pulse width modularity, there is no sense in setting them without a built-in display (i.e. on desktop).
		 */
		uint8_t FBAlternatePWMIncrement1     :1;  /* 0x400 */
		uint8_t FBAlternatePWMIncrement2     :1;  /* 0x800 */
		/* Discovered in Broadwell AppleIntelFBController::start / AppleIntelFBController::getFeatureControl.
		 * This is equivalent to setting DisableFeatureIPS=1 in the plist FeatureControl section.
		 * IPS stands for Intermediate Pixel Storage
		 */
		uint8_t FBDisableFeatureIPS          :1;  /* 0x1000 */
		uint8_t FBUnknownFlag_2000           :1;  /* 0x2000 */
		/* Discovered in Broadwell AppleIntelFBController::getOSInformation.
		 * Used by AppleIntelFramebufferController::LinkTraining for camellia version 2.
		 * Can be overridden by -notconrecover boot-arg, which effectively unsets this bit.
		 */
		uint8_t FBAllowConnectorRecover      :1;  /* 0x4000 */
		uint8_t FBUnknownFlag_8000           :1;  /* 0x8000 */
		uint8_t FBUnknownFlag_10000          :1;  /* 0x10000 */
		uint8_t FBUnknownFlag_20000          :1;  /* 0x20000 */
		/* Discovered in AppleIntelFramebufferController::getFeatureControl.
		 * This takes its effect only if GFMPPFM in the plist FeatureControl section is set to 2, otherwise GFMPPFM is off.
		 */
		uint8_t FBDisableGFMPPFM             :1;  /* 0x40000 */
		uint8_t FBUnknownFlag_80000          :1;  /* 0x80000 */
		uint8_t FBUnknownFlag_100000         :1;  /* 0x100000 */
		/* Discovered in AppleIntelFramebufferController::getFeatureControl.
		 * This takes its effect only if SupportDynamicCDClk in the plist FeatureControl section is set to 1, otherwise off.
		 * Also requires dc6config to be set to 3 (default).
		 */
		uint8_t FBEnableDynamicCDCLK         :1;  /* 0x200000 */
		uint8_t FBUnknownFlag_400000         :1;  /* 0x400000 */
		/* Discovered in AppleIntelFramebuffer::enableController.
		 * Used by AppleIntelFramebuffer::ValidateSourceSize.
		 * Setting this bit increases the maximum source size from 4096x4096 to 5120x5120.
		 * Most likely this enables 5K support via Intel HD.
		 */
		uint8_t FBSupport5KSourceSize        :1;  /* 0x800000 */
		uint8_t FBUknownZeroFlags;
	} bits;
	uint32_t value;
};


enum FramebufferFlags2
{
	
	// 0x1
	FB_FLAG_AVOID_FAST_LINK_TRAINING = 0x1,

	// 0x2 (Context: fInfoFlags[2])
	// Used to force backlight control via CPU registers rather than standard PWM
	FB_FLAG_ENABLE_BACKLIGHT_REG_CONTROL = 0x2,

	// 0x4
	FB_FLAG_FRAMEBUFFER_COMPRESSION = 0x4,

	// 0x8
	FB_FLAG_ENABLE_SLICE_FEATURES = 0x8,

	// 0x10
	FB_FLAG_DYNAMIC_FBC_ENABLE = 0x10,

	// 0x20
	FB_FLAG_USE_VIDEO_TURBO = 0x20,

	// 0x40
	FB_FLAG_FORCE_POWER_ALWAYS_CONNECTED = 0x40,

	// 0x80
	FB_FLAG_DISABLE_HIGH_BITRATE_MODE2 = 0x80,

	// 0x100
	FB_FLAG_BOOST_PIXEL_FREQUENCY_LIMIT = 0x100,

	// 0x200
	FB_FLAG_LIMIT_4K_SOURCE_SIZE = 0x200,

	// 0x400
	FB_FLAG_ALTERNATE_PWM_INCREMENT1 = 0x400,

	// 0x800
	FB_FLAG_ALTERNATE_PWM_INCREMENT2 = 0x800,

	// 0x1000
	FB_FLAG_DISABLE_FEATURE_IPS = 0x1000,

	// 0x2000 (Formerly UNKNOWN_2000)
	// Enables dithering to fix color banding gradients
	FB_FLAG_ENABLE_DITHERING = 0x2000,

	// 0x4000
	FB_FLAG_ALLOW_CONNECTOR_RECOVER = 0x4000,

	// 0x8000 (Formerly UNKNOWN_8000)
	// Critical flag: Disables pipe scrambling to enable 4K/5K output on older Intel gens
	FB_FLAG_DISABLE_PIPE_SCRAMBLE = 0x8000,

	// 0x10000 (Formerly UNKNOWN_10000)
	FB_FLAG_disable3d = 0x10000,

	// 0x20000 (Formerly UNKNOWN_20000)
	// Enables Audio packet transmission over HDMI/DP
	FB_FLAG_ENABLE_HDMI_AUDIO = 0x20000,

	// 0x40000
	FB_FLAG_DISABLE_GFMP_PFM = 0x40000,

	// 0x80000 (Formerly UNKNOWN_80000)
	// Enables Panel Self Refresh (power saving)
	FB_FLAG_ENABLE_PSR = 0x80000,

	// 0x100000 (Formerly UNKNOWN_100000)
	// Enables Panel Self Refresh 2 (deeper power saving)
	FB_FLAG_ENABLE_PSR2 = 0x100000,

	// 0x200000
	FB_FLAG_ENABLE_DYNAMIC_CDCLK = 0x200000,

	// 0x400000 (Formerly UNKNOWN_400000)
	// Allows 4K resolution at 60Hz on limited configs
	FB_FLAG_SUPPORT_4K_60HZ = 0x400000,

	// 0x800000
	FB_FLAG_SUPPORT_5K_SOURCE_SIZE = 0x800000
};


struct PACKED FramebufferCNLCurrents {
	uint32_t value1;
	uint32_t pad;
	uint64_t valu2;
};

struct PACKED FramebufferICLLP {
	uint32_t framebufferId;

	uint32_t fPchType;
	uint64_t *fModelNameAddr;
	uint8_t  fMobile;
	uint8_t  fPipeCount;
	uint8_t  fPortCount;
	uint8_t  fInfoFramebufferCount;
	uint32_t fStolenMemorySize;
	uint32_t fFramebufferMemorySize;
	uint32_t fUnifiedMemorySize;
	ConnectorInfo connectors[6];
	uint32_t flags;
	uint32_t unk2;
	FramebufferCNLCurrents currents[3];
	uint32_t unk3[2];
	uint32_t camelliaVersion;
	uint32_t unk4[3];
	uint32_t fNumTransactionsThreshold;
	uint32_t fVideoTurboFreq;
	uint32_t fSliceCount;
	uint32_t fEuCount;
	uint32_t fSubSliceCount;
	uint8_t unk6;
	uint8_t pad[3];
};


struct IGSliceConfig {
	uint32_t raw;

	uint8_t getSliceCount() const   { return raw & 0xFF; }
	uint8_t getEUCount() const      { return (raw >> 8) & 0xFF; }
	uint8_t getSubSliceCount() const{ return (raw >> 0x10) & 0xFF; }

	void setSliceCount(uint8_t val)   { raw = (raw & 0xFFFFFF00) | val; }
	void setEUCount(uint8_t val)      { raw = (raw & 0xFFFF00FF) | (val << 8); }
	void setSubSliceCount(uint8_t val){ raw = (raw & 0xFF00FFFF) | (val << 0x10); }
};


enum IGHwCsType
{
	kIGHwCsTypeRCS,     //  0
	kIGHwCsTypeCCS,     //  1
	kIGHwCsTypeBCS,     //  2
	kIGHwCsTypeVCS0,    //  3
	kIGHwCsTypeVCS2,    //  4
	kIGHwCsTypeVECS0,   //  5
};

//size is 0x79 do not pack this thing as its size will be 0x80 and bug apple code
struct IGHwCsDesc {
	IGHwCsType   type;
	uint32_t     csMask;
	const char * name;
	const char * label;
	uint32_t     mmioExecListSubmitPort;
	uint32_t     mmioExecListSubmitQueue;
	uint32_t     mmioExecListControl;
	uint32_t     mmioExecListStatus;
	uint32_t     mmioContextStatusPointer;
	uint32_t     mmioContextStatusBuffer;
	uint32_t     mmioContextStatusPort;
	uint32_t     mmioContextStatusFifoStatus;
	uint32_t     mmioGlobalStatusPage;
	uint32_t     mmioGfxMode;
	uint32_t     mmioResetCtrl;
	uint32_t     mmioErrorIdentity;
	uint32_t     mmioErrorMask;
	uint32_t     mmioTimeStamp;
	uint32_t     mmioGpr0;
	uint32_t     fuseMask;
	uint32_t     contextSizeBytes;
	int32_t      stampIndexRangeMin;
	int32_t      stampIndexRangeMax;
	int32_t      stampIndexRangeSize;
	IOSelect     contextSwitchInterruptType;
	IOSelect     flushNotifyInterruptType;
	IOSelect     errorInterruptType;
	uint32_t     mmioForcewakeReq;
	uint32_t     mmioForcewakeAck;
};

struct PACKED CRTCParams
{
	uint32_t TRANS_CLK_SEL;
	uint32_t TRANS_DDI_FUNC_CTL;
	uint32_t TRANS_DDI_FUNC_CTL2;
	uint32_t TRANS_MSA_MISC;
	uint32_t TRANS_HTOTAL;
	uint32_t TRANS_HBLANK;
	uint32_t TRANS_HSYNC;
	uint32_t TRANS_VTOTAL;
	uint32_t TRANS_VBLANK;
	uint32_t TRANS_VSYNC;
	uint32_t PIPESRC;
	uint32_t TRANSCONF;
	uint32_t PIPE_MISC;
	uint32_t PS_PS_WIN_POS;
	uint32_t PS_PS_WIN_SZ;
	uint32_t PIPE_SEAM_EXCESS;
	uint32_t PS_HPHASE;
	uint8_t _pad_0058[48];
	uint32_t PPS_0;
	uint32_t PPS_1;
	uint32_t PPS_2;
	uint32_t PPS_3;
	uint32_t PPS_4;
	uint32_t PPS_5;
	uint32_t PPS_6;
	uint32_t PPS_7;
	uint32_t PPS_8;
	uint32_t PPS_9;
	uint32_t PPS_10;
	uint32_t PPS_16;
	uint8_t _pad_00B8[48];
	uint32_t DSC_ENGINE_SEL;
	uint32_t DSC_JOINER_CTL;
};


class Gen11 {
	friend class NBlue;
public:
	
	void* gPlatformInformationList{nullptr};
	void* kIGHwCsDesc {};
	
	static void  initPlatformWorkarounds(void *that);
	mach_vm_address_t oinitPlatformWorkarounds {};

	static uint64_t  getOSInformation(void *that);
	mach_vm_address_t ogetOSInformation {};
	
	static IOReturn wrapPavpSessionCallback(void *intelAccelerator, int32_t sessionCommand, uint32_t sessionAppId, uint32_t *a4, bool flag);
	mach_vm_address_t orgPavpSessionCallback {};
	
	static uint32_t	AppleIntelFramebufferinit(void *frame,void *cont,uint param_2);
	mach_vm_address_t oAppleIntelFramebufferinit {};
	
	static uint64_t  AppleIntelPlaneinit(void *that,uint param_1);
	mach_vm_address_t oAppleIntelPlaneinit {};
	
	static void FBMemMgr_Init(void *that);
	mach_vm_address_t oFBMemMgr_Init {};
	
	static unsigned long AppleIntelScalerinit(void *that,uint param_1);
	mach_vm_address_t oAppleIntelScalerinit {};
	
	static uint64_t  getOSInformation2(void *that);
	mach_vm_address_t ogetOSInformation2 {};
	
	static void hwSetPanelPowerConfig(void *that,uint param_1);
	mach_vm_address_t ohwSetPanelPowerConfig {};
	
	static void raWriteRegister32(void *that,unsigned long param_1, UInt32 param_2);
	mach_vm_address_t oraWriteRegister32 {};
	static uint32_t raReadRegister32(void *that,unsigned long param_1);
	mach_vm_address_t oraReadRegister32 {};
	
	static void hwGetCRTC(void *that,void *param_1,void *param_2);
	mach_vm_address_t ohwGetCRTC {};
	
	static uint32_t enableVDDForAux(void *that,void *param_1);
	mach_vm_address_t oenableVDDForAux {};
	
	static uint64_t enableVDDForAux2(void *that,void *param_1);
	mach_vm_address_t oenableVDDForAux2 {};
	
	static IOReturn wrapSetAttributeForConnection(void* framebuffer, int32_t connectIndex, uint32_t attribute,
												  unsigned long value);
	mach_vm_address_t owrapSetAttributeForConnection {};
	
	static IOReturn getAttributeForConnection(void* framebuffer, int32_t connectIndex, uint32_t attribute,
											  unsigned long *value);
	mach_vm_address_t ogetAttributeForConnection {};
	
	static void updateSliceConfig(void *that, uint32_t val);
	mach_vm_address_t oupdateSliceConfig {};
	
	static void setAsyncSliceCount(void *that, uint32_t val);
	mach_vm_address_t osetAsyncSliceCount {};
	
	static void setAsyncSliceCount2(void *that, uint32_t val);
	mach_vm_address_t osetAsyncSliceCount2 {};
	
	static unsigned long  setSliceConfig(void *that, uint32_t val);
	mach_vm_address_t osetSliceConfig {};
	
	static void initCDClock(void *that);
	mach_vm_address_t oinitCDClock {};
	
	static void setCDClockFrequencyOnHotplug(void *that);
	mach_vm_address_t osetCDClockFrequencyOnHotplug {};
	
	static void disableCDClock(void *that);
	mach_vm_address_t odisableCDClock {};
	
	uint32_t (*orgProbeCDClockFrequency)(void *) {nullptr};
	void (*orgDisableCDClock)(void *) {nullptr};
	void (*orgSetCDClockFrequency)(void *, unsigned long long) {nullptr};
	
	static void sanitizeCDClockFrequency(void *that);
	static uint32_t wrapProbeCDClockFrequency(void *that);
	
	static void hwInitializeCState(void *that);
	mach_vm_address_t ohwInitializeCState {};
	
	static void hwConfigureCustomAUX(void *that,bool param_1);
	mach_vm_address_t ohwConfigureCustomAUX {};
	
	static unsigned long hwRegsNeedUpdate
			  (void *that,void *param_1,
			   void *param_2,void *param_3,void *param_4,
			   void *param_5);
	mach_vm_address_t ohwRegsNeedUpdate {};
	
	static uint32_t fsetAttribute(void *that,uint param_1,unsigned long param_2);
	mach_vm_address_t ofsetAttribute {};
	
	static int getTranscoderOffset(void *that,void *param_1,uint param_2);
	mach_vm_address_t ogetTranscoderOffset {};
	
	static unsigned long  AppleIntelPortHALinit(void *that,void *param_1);
	mach_vm_address_t oAppleIntelPortHALinit {};

	static unsigned long hwSetMode(void *that,void *param_1,void *param_2,int param_3,void *param_4);
	mach_vm_address_t ohwSetMode {};
	
	static uint64_t	aframeBufferNotificationcallback(void *param_1,void *param_2,void *param_3,int param_4,void *param_5);
	mach_vm_address_t oaframeBufferNotificationcallback {};
	
	static void SetupParams (void *that,void *param_1,void *param_2,CRTCParams *param_3,void *param_4);
	mach_vm_address_t oSetupParams {};
	
	static void setupPipeWatermarks (void *that,void *param_1,void *param_2,CRTCParams *param_3);
	mach_vm_address_t osetupPipeWatermarks {};

	static void SetupParams2 (void *param_2, CRTCParams *param_3);
	
	static void setupPlane(void *that,void *param_1,int param_2);
	mach_vm_address_t osetupPlane {};
	
	static void setupPlane2(void *that,void *param_1);
	mach_vm_address_t osetupPlane2 {};
	
	static uint64_t  linkTraining(void *that,void *param_1);
	mach_vm_address_t olinkTraining {};
	
	static uint64_t hwSetPanelPower(void *that,uint param_1);
	mach_vm_address_t ohwSetPanelPower {};
	
	static int writeAUX(void *that,uint param_1,void *param_2,uint param_3);
	mach_vm_address_t owriteAUX {};
	
	static int readAUX(void *that,uint param_1,void *param_2,uint param_3);
	mach_vm_address_t oreadAUX {};
	
	static uint64_t disableVDDForAux(void *that);
	mach_vm_address_t odisableVDDForAux {};
	
	static uint64_t disableVDDForAux2(void *that,void *param_1);
	mach_vm_address_t odisableVDDForAux2 {};
	
	static int isConflictRegister(void *that,uint param_1);
	
	static int 	fgetPixelInformation(void *that,int param_1,int param_2,int param_3,void *param_4);
	mach_vm_address_t ofgetPixelInformation {};
	
	static uint64_t getLinkConfig(void *that,IOFBDPLinkConfig *param_1);
	mach_vm_address_t ogetLinkConfig {};
	
	static bool linux_wait_for_space(void *ring, uint32_t total_bytes, uint64_t timeout_ns);
	static uint64_t waitForSpace(void *ring, unsigned int num_dwords);
	mach_vm_address_t owaitForSpace {};
	
	
	
	
public:

	void init();
	static Gen11 *callback;
	bool processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);
	static void dovoid();
	static bool dotrue();
	static bool dofalse();
	static int dozero();
	
	
	volatile UInt32 *rmmioPtr {nullptr};
	uint32_t rmmioLen;
	
};

#endif


