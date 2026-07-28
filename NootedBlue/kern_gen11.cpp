
#include "kern_gen11.hpp"
#include <Headers/kern_api.hpp>
#include "Firmware.hpp"

static const char *pathG11FB = "/System/Library/Extensions/AppleIntelICLLPGraphicsFramebuffer.kext/Contents/MacOS/"
                               "AppleIntelICLLPGraphicsFramebuffer";
static const char *pathG11FBT = "/Library/Extensions/AppleIntelTGLGraphicsFramebuffer.kext/Contents/MacOS/"
							   "AppleIntelTGLGraphicsFramebuffer";

static const char *pathG11HW =
    "/System/Library/Extensions/AppleIntelICLGraphics.kext/Contents/MacOS/AppleIntelICLGraphics";
static const char *pathG11HWT =
	"/System/Library/Extensions/AppleIntelTGLGraphics.kext/Contents/MacOS/AppleIntelTGLGraphics";
static const char *pathG11HWTe =
	"/Library/Extensions/AppleIntelTGLGraphics.kext/Contents/MacOS/AppleIntelTGLGraphics";

static KernelPatcher::KextInfo kextG11FB {"com.apple.driver.AppleIntelICLLPGraphicsFramebuffer", &pathG11FB, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextG11FBT {"com.xxxxx.driver.AppleIntelTGLGraphicsFramebuffer", &pathG11FBT, 1, {}, {},
	KernelPatcher::KextInfo::Unloaded};

static KernelPatcher::KextInfo kextG11HW {"com.apple.driver.AppleIntelICLGraphics", &pathG11HW, 1, {}, {},
    KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextG11HWT {"com.apple.driver.AppleIntelTGLGraphics", &pathG11HWT, 1, {}, {},
	KernelPatcher::KextInfo::Unloaded};
static KernelPatcher::KextInfo kextG11HWTe {"com.xxxxx.driver.AppleIntelTGLGraphics", &pathG11HWTe, 1, {}, {},
	KernelPatcher::KextInfo::Unloaded};


//globals
void *ccont;
void *ccont2;
int bk=2;
bool kexticl=false;
bool kexttgld=false;
bool kexttglp=false;
IOFramebuffer *frame0;
int hwu=4;
int setpc=0;
void *linkp;
bool dpcdconf=false;;
int Report=-1;
bool seng=false;
bool host2=false;

Gen11 *Gen11::callback = nullptr;

void Gen11::init() {
	callback = this;
	
    lilu.onKextLoadForce(&kextG11FB);
	lilu.onKextLoadForce(&kextG11FBT);
	
    lilu.onKextLoadForce(&kextG11HW);
	lilu.onKextLoadForce(&kextG11HWT);
	lilu.onKextLoadForce(&kextG11HWTe);
}



bool Gen11::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
	
	if (kextG11FB.loadIndex == index) {
		kexticl=true;
		
		SolveRequestPlus solveRequests[] = {
			{"_gPlatformInformationList", this->gPlatformInformationList},
			{"__ZN31AppleIntelFramebufferController14disableCDClockEv", this->orgDisableCDClock},
			{"__ZN31AppleIntelFramebufferController19setCDClockFrequencyEy", this->orgSetCDClockFrequency},
		};
		PANIC_COND(!SolveRequestPlus::solveAll(patcher, index, solveRequests, address, size), "nblue",	"Failed to resolve symbols");
		
		RouteRequestPlus requests[] = {
			
			{"__ZN31AppleIntelFramebufferController23initPlatformWorkaroundsEv",initPlatformWorkarounds, this->oinitPlatformWorkarounds},
			{"__ZN31AppleIntelFramebufferController16getOSInformationEv",getOSInformation2, this->ogetOSInformation2},
			{"__ZN31AppleIntelFramebufferController19getTranscoderOffsetEP14AppleIntelPortj",getTranscoderOffset, this->ogetTranscoderOffset},
			//{"__ZN31AppleIntelFramebufferController14ReadRegister32Em",raReadRegister32, this->oraReadRegister32},
			//{"__ZN31AppleIntelFramebufferController15WriteRegister32Emj",raWriteRegister32, this->oraWriteRegister32},
			{"__ZN21AppleIntelFramebuffer25setAttributeForConnectionEijm",wrapSetAttributeForConnection, this->owrapSetAttributeForConnection},
			{"__ZN21AppleIntelFramebuffer25getAttributeForConnectionEijPm",fgetAttributeForConnection, this->ofgetAttributeForConnection},
			{"__ZN31AppleIntelFramebufferController13FBMemMgr_InitEv", FBMemMgr_Init,this->oFBMemMgr_Init},
			{"__ZN31AppleIntelFramebufferController9hwGetCRTCEP21AppleIntelFramebufferP21AppleIntelDisplayPath",hwGetCRTC, this->ohwGetCRTC},
			{"__ZN31AppleIntelFramebufferController21hwSetPanelPowerConfigEj", hwSetPanelPowerConfig,this->ohwSetPanelPowerConfig},
			{"__ZN31AppleIntelFramebufferController17updateSliceConfigEj",updateSliceConfig, this->oupdateSliceConfig},
			{"__ZN31AppleIntelFramebufferController18setAsyncSliceCountE13IGSliceConfig",setAsyncSliceCount, this->osetAsyncSliceCount},
			{"__ZN21AppleIntelFramebuffer4initEP31AppleIntelFramebufferControllerj",AppleIntelFramebufferinit, this->oAppleIntelFramebufferinit},
			{"__ZN31AppleIntelFramebufferController21probeCDClockFrequencyEv",wrapProbeCDClockFrequency,	this->orgProbeCDClockFrequency},
			{"__ZN31AppleIntelFramebufferController18hwInitializeCStateEv",hwInitializeCState, this->ohwInitializeCState},
			{"__ZN31AppleIntelFramebufferController20hwConfigureCustomAUXEb",hwConfigureCustomAUX, this->ohwConfigureCustomAUX},
			{"__ZN17AppleIntelPortHAL4initEP10PortConfig",AppleIntelPortHALinit, this->oAppleIntelPortHALinit},
			{"__ZN31AppleIntelFramebufferController11SetupParamsEP21AppleIntelFramebufferP21AppleIntelDisplayPathP10CRTCParamsPK29IODetailedTimingInformationV2",SetupParams,	this->oSetupParams},
			{"__ZN31AppleIntelFramebufferController19setupPipeWatermarksEP21AppleIntelFramebufferP21AppleIntelDisplayPathP10CRTCParams",setupPipeWatermarks, this->osetupPipeWatermarks},
			{"__ZN15AppleIntelPlane10setupPlaneEP21AppleIntelDisplayPathi",setupPlane, this->osetupPlane},
			{"__ZN14AppleIntelPort8writeAUXEjPvj",writeAUX, this->owriteAUX},
			{"__ZN14AppleIntelPort7readAUXEjPvj",readAUX, this->oreadAUX},
			{"__ZN31AppleIntelFramebufferController15enableVDDForAuxEP14AppleIntelPort",enableVDDForAux2, this->oenableVDDForAux2},
			{"__ZN31AppleIntelFramebufferController16disableVDDForAuxEP14AppleIntelPort",disableVDDForAux2, this->odisableVDDForAux2},
			{"__ZN31AppleIntelFramebufferController15hwSetPanelPowerEj",hwSetPanelPower, this->ohwSetPanelPower},
			{"__ZN14AppleIntelPort12linkTrainingEP18AGDCDPPortConfig_t",linkTraining, this->olinkTraining},
			//{"__ZN21AppleIntelFramebuffer19getPixelInformationEiiiP18IOPixelInformation",fgetPixelInformation, this->ofgetPixelInformation},
			{"__ZN21AppleIntelDisplayPath8initHDCPEv", dozero},
			//{"__ZN15AppleIntelPlane17configurePlaneCUSEP19FlipTransactionArgs10IGColorCtl",dovoid},
			//{"__ZN15AppleIntelPlane18configurePlaneiCSCEP19FlipTransactionArgs10IGColorCtl",dovoid},
			{"__ZN17AppleIntelPortHAL13getLinkConfigEP16IOFBDPLinkConfig",getLinkConfig, this->ogetLinkConfig},
			{"__ZN21AppleIntelFramebuffer12getAttributeEjPm",fgetAttribute, this->ofgetAttribute},
			{"__ZN21AppleIntelFramebuffer12setAttributeEjm",fsetAttribute, this->ofsetAttribute},
			
			//{"__ZN31AppleIntelFramebufferController16hwRegsNeedUpdateEP21AppleIntelFramebufferP21AppleIntelDisplayPathP10CRTCParamsPK29IODetailedTimingInformationV2PN16AppleIntelScaler12SCALERPARAMSE",hwRegsNeedUpdate, this->ohwRegsNeedUpdate},
			/*{"__ZN21AppleIntelFramebuffer31frameBufferNotificationcallbackEP8OSObjectPvP13IOFramebufferiS2_",aframeBufferNotificationcallback, this->oaframeBufferNotificationcallback},
			{"__ZN31AppleIntelFramebufferController9hwSetModeEP21AppleIntelFramebufferP21AppleIntelDisplayPathiPK29IODetailedTimingInformationV2",hwSetMode, this->ohwSetMode},*/
			
			
			{"__ZN21AppleIntelFramebuffer17prepareToExitWakeEv",dovoid},
			/*{"__ZN21AppleIntelFramebuffer18prepareToExitSleepEv",dovoid},
			{"__ZN21AppleIntelFramebuffer19prepareToEnterSleepEv",dovoid},
			{"__ZN21AppleIntelFramebuffer18prepareToEnterWakeEv",dovoid},*/
			
			
		};
		PANIC_COND(!RouteRequestPlus::routeAll(patcher, index, requests, address, size), "nblue","Failed to route symbols");
		
		//cache + boot
		static const uint8_t f7[]= {0x83, 0x78, 0x08, 0x00, 0x0f, 0x84, 0x32, 0x01, 0x00, 0x00};
		static const uint8_t r7[]= {0x83, 0x78, 0x08, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
		
		static const uint8_t f7a[]= {0x80, 0x7d, 0xd7, 0x00, 0x75, 0x23};
		static const uint8_t r7a[]= {0x80, 0x7d, 0xd7, 0x00, 0xeb, 0x23};
		
		//builtin
		static const uint8_t f9[]= {0x48, 0x8b, 0xb8, 0x40, 0x04, 0x00, 0x00, 0xf6, 0x47, 0x14, 0x08, 0x75, 0x0a};
		static const uint8_t r9[]= {0x48, 0x8b, 0xb8, 0x40, 0x04, 0x00, 0x00, 0xf6, 0x47, 0x14, 0x08, 0xeb, 0x0a};
		//edp panel
		static const uint8_t f9b[]= {0x8b, 0x40, 0x08, 0x85, 0xc0, 0x74, 0x51};
		static const uint8_t r9b[]= {0x8b, 0x40, 0x08, 0x85, 0xc0, 0xeb, 0x51};
		//sleep
		static const uint8_t f9c[]= {0x74, 0x1f, 0x85, 0xc0, 0x0f, 0x85, 0x81, 0x00, 0x00, 0x00};
		static const uint8_t r9c[]= {0x90, 0x90, 0x85, 0xc0, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
		
		
		//register adresses
		static const uint8_t f24b[]= {0xbe, 0x40, 0xf8, 0x06, 0x00};
		static const uint8_t r24b[]= {0xbe, 0x40, 0x08, 0x06, 0x00};
				
		static const uint8_t f24c[]= {0x48, 0x81, 0xfb, 0x00, 0xf8, 0x06, 0x00};
		static const uint8_t r24c[]= {0x48, 0x81, 0xfb, 0x00, 0x08, 0x06, 0x00};
				
		static const uint8_t f24d[]= {0xbe, 0x60, 0xf8, 0x06, 0x00};
		static const uint8_t r24d[]= {0xbe, 0x60, 0x08, 0x06, 0x00};
				
		static const uint8_t f24e[]= {0xbe, 0x00, 0xf8, 0x06, 0x00};
		static const uint8_t r24e[]= {0xbe, 0x00, 0x08, 0x06, 0x00};
		
		static const uint8_t f24f[]= {0xbe, 0x08, 0xf0, 0x07, 0x00};
		static const uint8_t r24f[]= {0xbe, 0x08, 0x00, 0x07, 0x00};
		
		//linktrainig 2 lines
		static const uint8_t f25[]= {0x77, 0x77, 0x00, 0x00};
		static const uint8_t r25[]= {0x77, 0x00, 0x00, 0x00};

		
		LookupPatchPlus const patches[] = {
			{&kextG11FB, f7, r7, arrsize(f7),    1},
			{&kextG11FB, f7a, r7a, arrsize(f7a),    1},
			{&kextG11FB, f9, r9, arrsize(f9),    1},
			//{&kextG11FB, f9b, r9b, arrsize(f9b),    1},
			//{&kextG11FB, f9c, r9c, arrsize(f9c),    1},
			{&kextG11FB, f24b, r24b, arrsize(f24b),    12},
			{&kextG11FB, f24c, r24c, arrsize(f24c),    1},
			{&kextG11FB, f24d, r24d, arrsize(f24d),    10},
			{&kextG11FB, f24e, r24e, arrsize(f24e),    28},
			{&kextG11FB, f24f, r24f, arrsize(f24f),    5},
			{&kextG11FB, f25, r25, arrsize(f25),    7},
			
		};
		PANIC_COND(!LookupPatchPlus::applyAll(patcher, patches , address, size), "nblue", "kextG11FB Failed to apply patches!");

		DBGLOG("nblue", "Loaded AppleIntelICLLPGraphicsFramebuffer!");
		return true;
		
		
	}	else if (kextG11FBT.loadIndex == index) {

		SYSLOG("nblue", "AppleIntelTGLGraphicsFramebuffer");
		
		bool isprod=false;
		auto prod=patcher.solveSymbol(index, "__ZN24AppleIntelBaseController5startEP9IOService", address, size);
		if (!prod) isprod=true;
		kexttgld=!isprod;
		kexttglp=isprod;
		
		if (isprod) {
			SolveRequestPlus solveRequests[] = {
				{"_gPlatformInformationList", this->gPlatformInformationList},
				{"__ZN31AppleIntelFramebufferController14disableCDClockEv", this->orgDisableCDClock},
				{"__ZN31AppleIntelFramebufferController19setCDClockFrequencyEy", this->orgSetCDClockFrequency},
			};
			PANIC_COND(!SolveRequestPlus::solveAll(patcher, index, solveRequests, address, size), "nblue",	"Failed to resolve symbols");
		}
		else
		{
			SolveRequestPlus solveRequests[] = {
				{"_gPlatformInformationList", this->gPlatformInformationList},
				{"__ZN24AppleIntelBaseController14disableCDClockEv", this->orgDisableCDClock},
				{"__ZN24AppleIntelBaseController19setCDClockFrequencyEy", this->orgSetCDClockFrequency},
			};
			PANIC_COND(!SolveRequestPlus::solveAll(patcher, index, solveRequests, address, size), "nblue",	"Failed to resolve symbols");
		}

		
		RouteRequestPlus requests[] = {
			{"__ZN16AppleIntelScaler4initE10IGScalerID", AppleIntelScalerinit,this->oAppleIntelScalerinit},
			{"__ZN15AppleIntelPlane4initE9IGPlaneID", AppleIntelPlaneinit,this->oAppleIntelPlaneinit},
			//{"__ZN31AppleIntelRegisterAccessManager14ReadRegister32Em",raReadRegister32, this->oraReadRegister32},
			//{"__ZN31AppleIntelRegisterAccessManager15WriteRegister32Emj",raWriteRegister32, this->oraWriteRegister32},
			{"__ZN21AppleIntelFramebuffer25setAttributeForConnectionEijm",wrapSetAttributeForConnection, this->owrapSetAttributeForConnection},
			{"__ZN21AppleIntelFramebuffer25getAttributeForConnectionEijPm",fgetAttributeForConnection, this->ofgetAttributeForConnection},
			{"__ZN26AppleIntelDSBAccessManager13isDSBRegisterEj", dozero},
			{"__ZN31AppleIntelRegisterAccessManager18isConflictRegisterEj", isConflictRegister},
			{"__ZN15AppleIntelPlane10setupPlaneEP21AppleIntelDisplayPath",setupPlane2, this->osetupPlane2},
			{"__ZN14AppleIntelPort12linkTrainingEP18AGDCDPPortConfig_t",linkTraining, this->olinkTraining},
			{"__ZN14AppleIntelPort8writeAUXEjPvj",writeAUX, this->owriteAUX},
			{"__ZN14AppleIntelPort7readAUXEjPvj",readAUX, this->oreadAUX},
			{"__ZN21AppleIntelFramebuffer12getAttributeEjPm",fgetAttribute, this->ofgetAttribute},
			{"__ZN21AppleIntelFramebuffer12setAttributeEjm",fsetAttribute, this->ofsetAttribute},
			//{"__ZN21AppleIntelFramebuffer19getPixelInformationEiiiP18IOPixelInformation",fgetPixelInformation, this->ofgetPixelInformation},
			//{"__ZN15AppleIntelPlane18configurePlaneiCSCEP19FlipTransactionArgs10IGColorCtl",dovoid},
			//{"__ZN15AppleIntelPlane17configurePlaneCUSEP19FlipTransactionArgs10IGColorCtl",dovoid},
			{"__ZN21AppleIntelDisplayPath8initHDCPEv", dozero},
			{"__ZN17AppleIntelPortHAL4initEP10PortConfig",AppleIntelPortHALinit, this->oAppleIntelPortHALinit},
			{"__ZN21AppleIntelDisplayPath13getLinkConfigEP16IOFBDPLinkConfig",getLinkConfig, this->ogetLinkConfig},
			{"__ZN15AppleIntelPlane11updatePlaneEb",updatePlane, this->oupdatePlane},
			{"__ZN21AppleIntelFramebuffer19RestoreTransactionsEb",RestoreTransactions, this->oRestoreTransactions},
			{"__ZN19AppleIntelPowerWell19enableDisplayEngineEv",enableDisplayEngine, this->oenableDisplayEngine},
			{"__ZN19AppleIntelPowerWell23overridePowerWellsStateEb",overridePowerWellsState, this->ooverridePowerWellsState},
			{"__ZN19AppleIntelPowerWell19disablePowerWellAuxEj",disablePowerWellAux, this->odisablePowerWellAux},
			{"__ZN19AppleIntelPowerWell19disablePowerWellDDIEj",disablePowerWellDDI, this->odisablePowerWellDDI},
			{"__ZN19AppleIntelPowerWell18disablePowerWellPGEj",disablePowerWellPG, this->odisablePowerWellPG},
			
			
			
			
			/*{"__ZN21AppleIntelFramebuffer17prepareToExitWakeEv",dovoid},
			{"__ZN21AppleIntelFramebuffer18prepareToExitSleepEv",dovoid},
			{"__ZN21AppleIntelFramebuffer19prepareToEnterSleepEv",dovoid},
			{"__ZN21AppleIntelFramebuffer18prepareToEnterWakeEv",dovoid},
			*/
			
			
		};
		PANIC_COND(!RouteRequestPlus::routeAll(patcher, index, requests, address, size), "nblue","Failed to route dp symbols");
		
		if (isprod) {
			RouteRequestPlus requests[] = {
				{"__ZN31AppleIntelFramebufferController13probeBootPipeEPbPN17AppleIntelPortHAL3DDIE",probeBootPipe, this->oprobeBootPipe},
				{"__ZN31AppleIntelFramebufferController11initCDClockEv",initCDClock, this->oinitCDClock},
				//{"__ZN31AppleIntelFramebufferController15configureReportEP19IOReportChannelListjPvS2_",configureReport, this->oconfigureReport},
				{"__ZN19AppleIntelPowerWell4initEP31AppleIntelFramebufferController",AppleIntelPowerWellinit, this->oAppleIntelPowerWellinit},
				//{"__ZN31AppleIntelFramebufferController16hwRegsNeedUpdateEP21AppleIntelFramebufferP21AppleIntelDisplayPathP10CRTCParamsPK29IODetailedTimingInformationV2PN16AppleIntelScaler12SCALERPARAMSE",hwRegsNeedUpdate, this->ohwRegsNeedUpdate},
				{"__ZN31AppleIntelFramebufferController15hwSetPanelPowerEj",hwSetPanelPower, this->ohwSetPanelPower},
				{"__ZN31AppleIntelFramebufferController11SetupParamsEP21AppleIntelFramebufferP21AppleIntelDisplayPathP10CRTCParamsPK29IODetailedTimingInformationV2",SetupParams,	this->oSetupParams},
				{"__ZN31AppleIntelFramebufferController19setupPipeWatermarksEP21AppleIntelFramebufferP21AppleIntelDisplayPathP10CRTCParams",setupPipeWatermarks, this->osetupPipeWatermarks},
				{"__ZN31AppleIntelFramebufferController21probeCDClockFrequencyEv",wrapProbeCDClockFrequency,	this->orgProbeCDClockFrequency},
				{"__ZN31AppleIntelFramebufferController17updateSliceConfigEj",updateSliceConfig, this->oupdateSliceConfig},
				{"__ZN31AppleIntelFramebufferController18setAsyncSliceCountE13IGSliceConfig",setAsyncSliceCount, this->osetAsyncSliceCount},
				{"__ZN31AppleIntelFramebufferController9hwGetCRTCEP21AppleIntelFramebufferP21AppleIntelDisplayPath",hwGetCRTC, this->ohwGetCRTC},
				{"__ZN31AppleIntelFramebufferController21hwSetPanelPowerConfigEj", hwSetPanelPowerConfig,this->ohwSetPanelPowerConfig},
				{"__ZN31AppleIntelFramebufferController15enableVDDForAuxEP14AppleIntelPort",enableVDDForAux, this->oenableVDDForAux},
				{"__ZN31AppleIntelFramebufferController16disableVDDForAuxEv",disableVDDForAux, this->odisableVDDForAux},
				{"__ZN21AppleIntelFramebuffer4initEP31AppleIntelFramebufferControllerj",AppleIntelFramebufferinit, this->oAppleIntelFramebufferinit},
				{"__ZN31AppleIntelFramebufferController13FBMemMgr_InitEv", FBMemMgr_Init,this->oFBMemMgr_Init},
				{"__ZN31AppleIntelFramebufferController23initPlatformWorkaroundsEv",initPlatformWorkarounds, this->oinitPlatformWorkarounds},
				{"__ZN31AppleIntelFramebufferController16getOSInformationEv",getOSInformation, this->ogetOSInformation},
				{"__ZN31AppleIntelFramebufferController18hwInitializeCStateEv",hwInitializeCState, this->ohwInitializeCState},
				{"__ZN31AppleIntelFramebufferController20hwConfigureCustomAUXEb",hwConfigureCustomAUX, this->ohwConfigureCustomAUX},
				
			};
			PANIC_COND(!RouteRequestPlus::routeAll(patcher, index, requests, address, size), "nblue","Failed to route p symbols");
			
		} else //debug version
		{
			RouteRequestPlus requests[] = {
				{"__ZN24AppleIntelBaseController13probeBootPipeEPbPN17AppleIntelPortHAL3DDIE",probeBootPipe, this->oprobeBootPipe},
				{"__ZN24AppleIntelBaseController11initCDClockEv",initCDClock, this->oinitCDClock},
				//{"__ZN24AppleIntelBaseController15configureReportEP19IOReportChannelListjPvS2_",configureReport, this->oconfigureReport},
				{"__ZN19AppleIntelPowerWell4initEP24AppleIntelBaseController",AppleIntelPowerWellinit, this->oAppleIntelPowerWellinit},
				//{"__ZN24AppleIntelBaseController16hwRegsNeedUpdateEP21AppleIntelFramebufferP21AppleIntelDisplayPathP10CRTCParamsPK29IODetailedTimingInformationV2PN16AppleIntelScaler12SCALERPARAMSE",hwRegsNeedUpdate, this->ohwRegsNeedUpdate},
				{"__ZN24AppleIntelBaseController15hwSetPanelPowerEj",hwSetPanelPower, this->ohwSetPanelPower},
				{"__ZN24AppleIntelBaseController11SetupParamsEP21AppleIntelFramebufferP21AppleIntelDisplayPathP10CRTCParamsPK29IODetailedTimingInformationV2",SetupParams,	this->oSetupParams},
				{"__ZN24AppleIntelBaseController19setupPipeWatermarksEP21AppleIntelFramebufferP21AppleIntelDisplayPathP10CRTCParams",setupPipeWatermarks, this->osetupPipeWatermarks},
				{"__ZN24AppleIntelBaseController21probeCDClockFrequencyEv",wrapProbeCDClockFrequency,	this->orgProbeCDClockFrequency},
				{"__ZN24AppleIntelBaseController17updateSliceConfigEj",updateSliceConfig, this->oupdateSliceConfig},
				{"__ZN24AppleIntelBaseController18setAsyncSliceCountE13IGSliceConfig",setAsyncSliceCount, this->osetAsyncSliceCount},
				{"__ZN24AppleIntelBaseController9hwGetCRTCEP21AppleIntelFramebufferP21AppleIntelDisplayPath",hwGetCRTC, this->ohwGetCRTC},
				{"__ZN24AppleIntelBaseController21hwSetPanelPowerConfigEj", hwSetPanelPowerConfig,this->ohwSetPanelPowerConfig},
				{"__ZN24AppleIntelBaseController15enableVDDForAuxEP14AppleIntelPort",enableVDDForAux, this->oenableVDDForAux},
				{"__ZN24AppleIntelBaseController16disableVDDForAuxEv",disableVDDForAux, this->odisableVDDForAux},
				{"__ZN21AppleIntelFramebuffer4initEP24AppleIntelBaseControllerj",AppleIntelFramebufferinit, this->oAppleIntelFramebufferinit},
				{"__ZN24AppleIntelBaseController13FBMemMgr_InitEv", FBMemMgr_Init,this->oFBMemMgr_Init},
				{"__ZN24AppleIntelBaseController23initPlatformWorkaroundsEv",initPlatformWorkarounds, this->oinitPlatformWorkarounds},
				{"__ZN24AppleIntelBaseController16getOSInformationEv",getOSInformation, this->ogetOSInformation},
				{"__ZN24AppleIntelBaseController18hwInitializeCStateEv",hwInitializeCState, this->ohwInitializeCState},
				{"__ZN24AppleIntelBaseController20hwConfigureCustomAUXEb",hwConfigureCustomAUX, this->ohwConfigureCustomAUX},
				
			};
			PANIC_COND(!RouteRequestPlus::routeAll(patcher, index, requests, address, size), "nblue","Failed to route d symbols");
			
		}
		
		
		//dbuff
		static const uint8_t f5[]= {0x74, 0x5a, 0xc7, 0x83, 0x48, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		static const uint8_t r5[]= {0x90, 0x90, 0xc7, 0x83, 0x48, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		
		
		//ReadRegister64
		static const uint8_t f7[]= {0x83, 0xc0, 0xfc, 0x48, 0x39, 0xf0, 0x76, 0x11, 0x48, 0x8b, 0x47, 0x50, 0x48, 0xff, 0x05, 0xca, 0xf5, 0x0c, 0x00};
		static const uint8_t r7[]= {0x83, 0xc0, 0xf8, 0x48, 0x39, 0xf0, 0x76, 0x11, 0x48, 0x8b, 0x47, 0x50, 0x48, 0xff, 0x05, 0xca, 0xf5, 0x0c, 0x00};
		
		static const uint8_t f7p[]= {0x83, 0xc0, 0xfc, 0x48, 0x39, 0xf0, 0x76, 0x11, 0x48, 0x8b, 0x47, 0x50, 0x48, 0xff, 0x05, 0x84, 0x40, 0x08, 0x00};
		static const uint8_t r7p[]= {0x83, 0xc0, 0xf8, 0x48, 0x39, 0xf0, 0x76, 0x11, 0x48, 0x8b, 0x47, 0x50, 0x48, 0xff, 0x05, 0x84, 0x40, 0x08, 0x00};
		
		//builtin
		static const uint8_t f9[]= {0xf6, 0x40, 0x14, 0x08, 0x75, 0x0d};
		static const uint8_t r9[]= {0xf6, 0x40, 0x14, 0x08, 0xeb, 0x0d};
		
		static const uint8_t f9p[]= {0xf6, 0x40, 0x14, 0x08, 0x75, 0x0a};
		static const uint8_t r9p[]= {0xf6, 0x40, 0x14, 0x08, 0xeb, 0x0a};
		
		
		//probeportmode hookcase fix
		static const uint8_t f13[]= {0xff, 0x91, 0x90, 0x01, 0x00, 0x00, 0x83, 0xf8, 0x02, 0x0f, 0x84, 0xec, 0x00, 0x00, 0x00};
		static const uint8_t r13[]= {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
		
		static const uint8_t f13b[]= {0xff, 0x90, 0x90, 0x01, 0x00, 0x00, 0x49, 0x8b, 0x0e, 0x4c, 0x89, 0xf7, 0x89, 0xc6, 0xff, 0x91, 0x38, 0x01, 0x00, 0x00};
		static const uint8_t r13b[]= {0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x49, 0x8b, 0x0e, 0x4c, 0x89, 0xf7, 0x89, 0xc6, 0xff, 0x91, 0x38, 0x01, 0x00, 0x00};

		//probeportmode hookcase fix
		static const uint8_t f13p[]= {0xff, 0x91, 0x78, 0x01, 0x00, 0x00, 0x83, 0xf8, 0x02, 0x74, 0x64};
		static const uint8_t r13p[]= {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90};
		
		static const uint8_t f13pb[]= {0xff, 0x90, 0x78, 0x01, 0x00, 0x00, 0x49, 0x8b, 0x0e, 0x4c, 0x89, 0xf7, 0x89, 0xc6, 0xff, 0x91, 0x38, 0x01, 0x00, 0x00};
		static const uint8_t r13pb[]= {0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00, 0x49, 0x8b, 0x0e, 0x4c, 0x89, 0xf7, 0x89, 0xc6, 0xff, 0x91, 0x38, 0x01, 0x00, 0x00};

		//getPathByPipe disable logs
		static const uint8_t f15[]= {0x74, 0x36, 0x48, 0xff, 0x05, 0x7e, 0x51, 0x08, 0x00, 0x44, 0x89, 0x3c, 0x24, 0x48, 0x8d, 0x15, 0x4d, 0x88, 0x03, 0x00, 0x4c, 0x8d, 0x05, 0x28, 0x8a, 0x03, 0x00};
		static const uint8_t r15[]= {0xeb, 0x36, 0x48, 0xff, 0x05, 0x7e, 0x51, 0x08, 0x00, 0x44, 0x89, 0x3c, 0x24, 0x48, 0x8d, 0x15, 0x4d, 0x88, 0x03, 0x00, 0x4c, 0x8d, 0x05, 0x28, 0x8a, 0x03, 0x00};
		
		//getHPDState register
		static const uint8_t f19[]= {0xbe, 0x70, 0x44, 0x04, 0x00};
		static const uint8_t r19[]= {0xbe, 0xa0, 0x38, 0x16, 0x00};
		
		
		//savenvram
		static const uint8_t f20[]= {0xff, 0x90, 0xf8, 0x09, 0x00, 0x00, 0x41, 0x89, 0xc6, 0x48, 0x85, 0xdb, 0x74, 0x17};
		static const uint8_t r20[]= {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x48, 0x85, 0xdb, 0x74, 0x17};
		
		static const uint8_t f20p[]= {0xff, 0x90, 0xf8, 0x09, 0x00, 0x00, 0x41, 0x89, 0xc6, 0x48, 0x85, 0xdb, 0x74, 0x17};
		static const uint8_t r20p[]= {0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x48, 0x85, 0xdb, 0x74, 0x17};

		//production ver fix register adresses if pipe=0
		static const uint8_t f24bp[]= {0x83, 0x78, 0x08, 0x00, 0x75, 0x0c};
		static const uint8_t r24bp[]= {0x83, 0x78, 0x08, 0x00, 0xeb, 0x0c};
				
		static const uint8_t f24cp[]= {0x00, 0x4c, 0x89, 0xea, 0x75, 0x12};
		static const uint8_t r24cp[]= {0x00, 0x4c, 0x89, 0xea, 0xeb, 0x12};
				
		static const uint8_t f24dp[]= {0x83, 0x78, 0x08, 0x00, 0x75, 0x0d};
		static const uint8_t r24dp[]= {0x83, 0x78, 0x08, 0x00, 0xeb, 0x0d};
				
		//debug ver fix register adresses if pipe=0
		static const uint8_t f24b[]= {0x83, 0x78, 0x08, 0x00, 0x75, 0x0c};
		static const uint8_t r24b[]= {0x83, 0x78, 0x08, 0x00, 0xeb, 0x0c};
				
		static const uint8_t f24c[]= {0x48, 0x8b, 0x55, 0xd0, 0x75, 0x13};
		static const uint8_t r24c[]= {0x48, 0x8b, 0x55, 0xd0, 0xeb, 0x13};
				
		static const uint8_t f24d[]= {0x83, 0x78, 0x08, 0x00, 0x75, 0x0d};
		static const uint8_t r24d[]= {0x83, 0x78, 0x08, 0x00, 0xeb, 0x0d};
		
		//linktrainig 2 lines
		static const uint8_t f25[]= {0x77, 0x77, 0x00, 0x00};
		static const uint8_t r25[]= {0x33, 0x00, 0x00, 0x00};



		
		if (isprod){
			LookupPatchPlus const patchesp[] = {// tgl production kext
				{&kextG11FBT, f5, r5, arrsize(f5),	1},
				{&kextG11FBT, f7p, r7p, arrsize(f7p),	1},
				{&kextG11FBT, f9p, r9p, arrsize(f9p),	1},
				{&kextG11FBT, f13p, r13p, arrsize(f13p),	1},
				{&kextG11FBT, f13pb, r13pb, arrsize(f13pb),	1},
				{&kextG11FBT, f19, r19, arrsize(f19),	1},
				{&kextG11FBT, f20p, r20p, arrsize(f20p),	1},
				{&kextG11FBT, f24bp, r24bp, arrsize(f24bp),	14},
				{&kextG11FBT, f24cp, r24cp, arrsize(f24cp),	1},
				{&kextG11FBT, f24dp, r24dp, arrsize(f24dp),	4},
				{&kextG11FBT, f25, r25, arrsize(f25),	6},

			};
			PANIC_COND(!LookupPatchPlus::applyAll(patcher, patchesp , address, size), "nblue", "kextG11FBT Failed to apply production patches!");
		}
		else {
			LookupPatchPlus const patches[] = {// tgl debug kext
				{&kextG11FBT, f5, r5, arrsize(f5),	1},
				{&kextG11FBT, f7, r7, arrsize(f7),	1},
				{&kextG11FBT, f9, r9, arrsize(f9),	1},
				{&kextG11FBT, f13, r13, arrsize(f13),	1},
				{&kextG11FBT, f13b, r13b, arrsize(f13b),	1},
				{&kextG11FBT, f15, r15, arrsize(f15),	1},
				{&kextG11FBT, f19, r19, arrsize(f19),	1},
				{&kextG11FBT, f20, r20, arrsize(f20),	1},
				{&kextG11FBT, f24b, r24b, arrsize(f24b),	11},
				{&kextG11FBT, f24c, r24c, arrsize(f24c),	1},
				{&kextG11FBT, f24d, r24d, arrsize(f24d),	6},
				{&kextG11FBT, f25, r25, arrsize(f25),	6},

			};
			PANIC_COND(!LookupPatchPlus::applyAll(patcher, patches , address, size), "nblue", "kextG11FBT Failed to apply dbg patches!");
		}
		return true;
		
	}     else if (kextG11HW.loadIndex == index) {
		 // icl
		 RouteRequestPlus requests[] = {
			 
			 {"__ZN16IntelAccelerator19PAVPCommandCallbackE22PAVPSessionCommandID_tjPjb", wrapPavpSessionCallback, this->orgPavpSessionCallback},
			 //{"__ZL27ContextStatusBufferValidateRK15IGHwCsExecList5PK28SGfxContextStatusBufferEntry.cold.2", dovoid},
			 {"__ZN16IntelAccelerator18setAsyncSliceCountE13IGSliceConfig",setAsyncSliceCount2, this->osetAsyncSliceCount2},
			 {"__ZN16IntelAccelerator14setSliceConfigE13IGSliceConfig",setSliceConfig, this->osetSliceConfig},
			 
			// {"__ZN20IGHardwareRingBuffer11waitTimeoutEU13block_pointerFbvE.cold.1", dovoid},
			 {"__ZN20IGHardwareRingBuffer12waitForSpaceEj",waitForSpace, this->owaitForSpace},
			 {"__ZN16IntelAccelerator19startGraphicsEngineEv",startGraphicsEngine, this->ostartGraphicsEngine},
			 {"__ZN16IntelAccelerator31initHardwareStatusPageRegistersEv",initHardwareStatusPageRegisters, this->oinitHardwareStatusPageRegisters},
			 
		 };
		PANIC_COND(!RouteRequestPlus::routeAll(patcher, index, requests, address, size), "nblue","Failed to route symbols");
		
		//sku = 8
		static const uint8_t f2[] = {0x41, 0xc1, 0xef, 0x1c, 0x44, 0x89, 0xbb, 0x50, 0x11, 0x00, 0x00};
		static const uint8_t r2[] = {0xc7, 0x83, 0x50, 0x11, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x90};
		//8 subslices
		static const uint8_t f2a[] = {0x44, 0x89, 0xf8, 0xf7, 0xd0, 0xf3, 0x0f, 0xb8, 0xf0, 0x89, 0xb3, 0x88, 0x11, 0x00, 0x00};
		static const uint8_t r2a[] = {0xc7, 0x83, 0x88, 0x11, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90, 0x90, 0x90};
		//ringmask
		static const uint8_t f2b[] = {0x80, 0x8b, 0x40, 0x13, 0x00, 0x00, 0x1f};
		static const uint8_t r2b[] = {0x80, 0x8b, 0x40, 0x13, 0x00, 0x00, 0x3f};
		
		//hwcaps tgl kext
		static const uint8_t f3[] = {0x48, 0xb8, 0xc0, 0x00, 0x00, 0x00, 0x6c, 0x01, 0x00, 0x00, 0x48, 0x89, 0x83, 0x5c, 0x11, 0x00, 0x00, 0x48, 0xb8, 0xe0, 0x00, 0x00, 0x00, 0x6c, 0x01, 0x00, 0x00, 0x48, 0x89, 0x83, 0x64, 0x11, 0x00, 0x00, 0xc7, 0x83, 0x6c, 0x11, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00};
		static const uint8_t r3[] = {0x48, 0xb8, 0xc0, 0x00, 0x00, 0x00, 0x22, 0x02, 0x00, 0x00, 0x48, 0x89, 0x83, 0x5c, 0x11, 0x00, 0x00, 0x48, 0xb8, 0x50, 0x01, 0x00, 0x00, 0x22, 0x02, 0x00, 0x00, 0x48, 0x89, 0x83, 0x64, 0x11, 0x00, 0x00, 0xc7, 0x83, 0x6c, 0x11, 0x00, 0x00, 0x50, 0x01, 0x00, 0x00};
		
		static const uint8_t f3a[] = {0xc7, 0x83, 0x94, 0x11, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00};
		static const uint8_t r3a[] = {0xc7, 0x83, 0x94, 0x11, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00};
		
		static const uint8_t f3b[] = {0xb9, 0x02, 0x00, 0x00, 0x00, 0xba, 0x04, 0x00, 0x00, 0x00, 0xbe, 0x80, 0x00, 0x00, 0x00};
		static const uint8_t r3b[] = {0xb9, 0x02, 0x00, 0x00, 0x00, 0xba, 0x06, 0x00, 0x00, 0x00, 0xbe, 0x80, 0x00, 0x00, 0x00};
		
		LookupPatchPlus const patches[] = {
			{&kextG11HW, f2, r2, arrsize(f2),	1},
			{&kextG11HW, f2a, r2a, arrsize(f2a),	1},
			{&kextG11HW, f2b, r2b, arrsize(f2b),	1},
			{&kextG11HW, f3, r3, arrsize(f3),	1},
			{&kextG11HW, f3a, r3a, arrsize(f3a),	1},
			{&kextG11HW, f3b, r3b, arrsize(f3b),	1},
		};
		PANIC_COND(!LookupPatchPlus::applyAll(patcher, patches , address, size), "nblue", "kextG11HW Failed to apply patches!");

		return true;
		
		
    } else if (kextG11HWT.loadIndex == index || kextG11HWTe.loadIndex == index) {
		//tgl
		auto kext=kextG11HWT.loadIndex == index ? kextG11HWT: kextG11HWTe;
		
		SolveRequestPlus solveRequests[] = {
			//apple rings control variable -> 11 globals
			{"__ZL11kIGHwCsDesc", this->kIGHwCsDesc},
			
		};
		//PANIC_COND(!SolveRequestPlus::solveAll(patcher, index, solveRequests, address, size), "nblue",	"Failed to resolve symbols");
		 
		
		 RouteRequestPlus requests[] = {
			 
			 {"__ZN16IntelAccelerator19PAVPCommandCallbackE22PAVPSessionCommandID_tjPjb", wrapPavpSessionCallback, this->orgPavpSessionCallback},
			 {"__ZN16IntelAccelerator18setAsyncSliceCountE13IGSliceConfig",setAsyncSliceCount2, this->osetAsyncSliceCount2},
			 {"__ZN16IntelAccelerator14setSliceConfigE13IGSliceConfig",setSliceConfig, this->osetSliceConfig},
			 {"__ZN20IGHardwareRingBuffer12waitForSpaceEj",waitForSpace, this->owaitForSpace},
			 {"__ZN16IntelAccelerator19startGraphicsEngineEv",startGraphicsEngine, this->ostartGraphicsEngine},
			 {"__ZN16IntelAccelerator31initHardwareStatusPageRegistersEv",initHardwareStatusPageRegisters, this->oinitHardwareStatusPageRegisters},
			 {"__ZN13IGHardwareGuC15allocDoorbellIdE25UK_GEN11_CONTEXT_PRIORITY",allocDoorbellId, this->oallocDoorbellId},
			 {"__ZN13IGHardwareGuC15stealDoorbellIdEv",stealDoorbellId, this->ostealDoorbellId},
			 {"__ZN13IGHardwareGuC18setDoorbellPinningEtb",setDoorbellPinning, this->osetDoorbellPinning},
			 {"__ZN13IGHardwareGuC15hostToGuCActionEPKjjiPj",hostToGuCAction, this->ohostToGuCAction},
			 {"__ZN13IGHardwareGuC17releaseDoorbellIdEt",releaseDoorbellId, this->oreleaseDoorbellId},
			 {"__ZN13IGHardwareGuC15acquireDoorbellEP35UK_GEN11_GUC_CONTEXT_DESCRIPTOR_RECb",acquireDoorbell, this->oacquireDoorbell},
			 {"__ZN13IGHardwareGuC15releaseDoorbellEP35UK_GEN11_GUC_CONTEXT_DESCRIPTOR_REC",releaseDoorbell, this->oreleaseDoorbell},
			 {"__ZN21IGHardwareGuCCTBuffer15hostToGuCActionEPKjjiPjb",hostToGuCAction2, this->ohostToGuCAction2},
			 {"__ZN13IGHardwareGuC16initSchedControlEv",initSchedControl, this->oinitSchedControl},
			 {"__ZN20IGSharedMappedBuffer11withOptionsEP11IGAccelTaskmjj",IGSharedMappedBufferwithOptions, this->oIGSharedMappedBufferwithOptions},
			 {"__ZNK20IGSharedMappedBuffer17getVirtualAddressEv",fgetVirtualAddress, this->ofgetVirtualAddress},
			 {"__ZNK14IGMappedBuffer20getGPUVirtualAddressEv",fgetGPUVirtualAddress, this->ofgetGPUVirtualAddress},
			 //{"__ZN5IGGuC18checkWOPCMSettingsEmR14IOVirtualRange",checkWOPCMSettings, this->ocheckWOPCMSettings},
			 {"__ZN16IntelAccelerator13SafeForceWakeEbj",SafeForceWake, this->oSafeForceWake},
			 {"__ZN20IGSharedMappedBuffer4freeEv",IGSharedMappedBufferfree, this->oIGSharedMappedBufferfree},
			 //{"__ZN16IntelAccelerator15configureDeviceEP11IOPCIDevice",fconfigureDevice, this->ofconfigureDevice},
			 //{"__ZN15IGMemoryManager16initDeviceMemoryEv",finitDeviceMemory, this->ofinitDeviceMemory},
			 {"__ZN13IGHardwareGuC13loadGuCBinaryEv",loadGuCBinary, this->oloadGuCBinary},
			 {"__ZN13IGHardwareGuC26setupAdditionalDataStructsEv",setupAdditionalDataStructs, this->osetupAdditionalDataStructs},
			 
		 };
		PANIC_COND(!RouteRequestPlus::routeAll(patcher, index, requests, address, size), "nblue","Failed to route symbols");
		
		// 8 subslices
		static const uint8_t f2[] = {0x8b, 0x5d, 0xd0, 0xf3, 0x0f, 0xb8, 0xf3, 0x01, 0xf6, 0x41, 0x89, 0xb7, 0x58, 0x11, 0x00, 0x00};
		static const uint8_t r2[] = {0x8b, 0x5d, 0xd0, 0x41, 0xc7, 0x87, 0x58, 0x11, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x90, 0x90};
		
		// 8 MaxEUPerSubSlice
		static const uint8_t f2a[] = {0xbe, 0x08, 0x00, 0x00, 0x00, 0x29, 0xde, 0x41, 0x89, 0xb7, 0x6c, 0x11, 0x00, 0x00};
		static const uint8_t r2a[] = {0xbe, 0x08, 0x00, 0x00, 0x00, 0x90, 0x90, 0x41, 0x89, 0xb7, 0x6c, 0x11, 0x00, 0x00};
		
		//sku =2  getGPUInfo
		static const uint8_t f3[] = {0x8b, 0x3e, 0x81, 0xff, 0xee, 0xbe, 0xaf, 0xde, 0x7f, 0x15, 0x81, 0xff, 0x86, 0x80, 0x40, 0x9a, 0x74, 0x2d};
		static const uint8_t r3[] = {0x8b, 0x3e, 0x81, 0xff, 0xee, 0xbe, 0xaf, 0xde, 0x90, 0x90, 0x81, 0xff, 0x86, 0x80, 0x40, 0x9a, 0xeb, 0x2d};
		
		// 12 to 8 subslices + L3BankCount = 8
		static const uint8_t f3a[] = {0x83, 0xfe, 0x01, 0x75, 0x59, 0x83, 0xfa, 0x0c};
		static const uint8_t r3a[] = {0x83, 0xfe, 0x01, 0x75, 0x59, 0x83, 0xfa, 0x08};
		
		//ringmask
		static const uint8_t f4[] = {0x41, 0x80, 0x8d, 0x00, 0x13, 0x00, 0x00, 0x3f};
		static const uint8_t r4[] = {0x41, 0x80, 0x8d, 0x00, 0x13, 0x00, 0x00, 0x3f};
		
		
		//blit3d mem align patch
		static const uint8_t f5[] = {0x40, 0xd2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		static const uint8_t r5[] = {0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		
		
			LookupPatchPlus const patches[] = {
				{&kext, f2, r2, arrsize(f2),	1},
				{&kext, f2a, r2a, arrsize(f2a),	1},
				{&kext, f3, r3, arrsize(f3),	1},
				{&kext, f3a, r3a, arrsize(f3a),	1},
				//{&kext, f4, r4, arrsize(f4),	1},
				{&kext, f5, r5, arrsize(f5),	1},

				
			};
			PANIC_COND(!LookupPatchPlus::applyAll(patcher, patches , address, size), "nblue", "kextG11HWT Failed to apply patches!");

		
		DBGLOG("nblue", "Loaded AppleIntelTGLGraphics!");

		return true;
	}

    return false;
}

uint32_t intel_de_rmw(struct intel_display *display, uint32_t reg, uint32_t clear, uint32_t set)
{
	uint32_t old, val;
	old = NBlue::callback->readReg32( reg);
	val = (old & ~clear) | set;
	NBlue::callback->writeReg32( reg, val);
	return old;
}

uint32_t intel_de_read(struct intel_display *display, uint32_t reg)
{
	return NBlue::callback->readReg32(reg);
}
void intel_de_write(struct intel_display *display, uint32_t reg, uint32_t val)
{
	NBlue::callback->writeReg32( reg, val);
}
void intel_de_posting_read(struct intel_display *display, uint32_t reg)
{
	NBlue::callback->readReg32(reg);
}

static inline int intel_uncore_write_and_verify(struct intel_display *display,
												u32 reg, u32 val,
						u32 mask, u32 expected_val)
{
	u32 reg_val;

	intel_de_write(display, reg, val);
	reg_val = intel_de_read(display, reg);
	
	return (reg_val & mask) != expected_val ? -EINVAL : 0;
}

void  Gen11::initPlatformWorkarounds(void *that)
{
	if (kexticl)
	{
		//PlatformWorkarounds
		getMember<volatile uint32_t>(that, 0xc1c)=
	 /*FB_FLAG_ENABLE_SLICE_FEATURES|FB_FLAG_ENABLE_BACKLIGHT_REG_CONTROL|*/FB_FLAG_LIMIT_4K_SOURCE_SIZE|
		/*FB_FLAG_DISABLE_FEATURE_IPS|FB_FLAG_ALTERNATE_PWM_INCREMENT2|
		FB_FLAG_ALTERNATE_PWM_INCREMENT1|FB_FLAG_DISABLE_HIGH_BITRATE_MODE2|*/
		/*FB_FLAG_FORCE_POWER_ALWAYS_CONNECTED|*/FB_FLAG_AVOID_FAST_LINK_TRAINING;
		
		//ig boot flags
		getMember<volatile uint32_t>(that, 0xc10)=/*FB_FLAG_DISABLE_HIGH_BITRATE_MODE2|*/FB_FLAG_BOOST_PIXEL_FREQUENCY_LIMIT;
		
	}
	else
	{
		//PlatformWorkarounds
		getMember<volatile uint32_t>(that, 0xc5c)=
		/*FB_FLAG_ALTERNATE_PWM_INCREMENT1|FB_FLAG_ENABLE_SLICE_FEATURES|*/
		/*FB_FLAG_FORCE_POWER_ALWAYS_CONNECTED|*/FB_FLAG_AVOID_FAST_LINK_TRAINING/*| FB_FLAG_ENABLE_BACKLIGHT_REG_CONTROL*/|FB_FLAG_LIMIT_4K_SOURCE_SIZE;
		
		//ig boot flags
		getMember<volatile uint32_t>(that, 0xc58)=/*FB_FLAG_ENABLE_BACKLIGHT_REG_CONTROL|*/FB_FLAG_BOOST_PIXEL_FREQUENCY_LIMIT;
	}
}

uint64_t  Gen11::getOSInformation2(void *that)
{//icl
	if (NBlue::callback->intel_opregion_setup()!=0) panic("BAD BIOS");

	
	struct FramebufferICLLP *pinfo =static_cast<FramebufferICLLP *>(callback->gPlatformInformationList);
	int p=0x6;
	
	pinfo[p].flags=
	FB_FLAG_DISABLE_PIPE_SCRAMBLE|FB_FLAG_FRAMEBUFFER_COMPRESSION|/*FB_FLAG_ALLOW_CONNECTOR_RECOVER|FB_FLAG_ENABLE_DITHERING|
	FB_FLAG_LIMIT_4K_SOURCE_SIZE|FB_FLAG_BOOST_PIXEL_FREQUENCY_LIMIT|*/
	/*FB_FLAG_ENABLE_BACKLIGHT_REG_CONTROL|*/FB_FLAG_AVOID_FAST_LINK_TRAINING;
	
	
		pinfo[p].camelliaVersion=0;
	
		pinfo[p].fMobile=1;
		pinfo[p].fPipeCount=3;
		pinfo[p].fPortCount=3;
		pinfo[p].fInfoFramebufferCount=2;

		pinfo[p].fSliceCount=1;
		pinfo[p].fEuCount=8;
		pinfo[p].fSubSliceCount=8;
	
	//pinfo[p].fVideoTurboFreq=270000000;
	
	
	for (int i = 0; i < 6; i++) {
		pinfo[p].connectors[i].index=NBlue::callback->i915b->display->bconnectors[i].index;
		pinfo[p].connectors[i].busId=NBlue::callback->i915b->display->bconnectors[i].busId;
		pinfo[p].connectors[i].pipe=NBlue::callback->i915b->display->bconnectors[i].pipe;
		pinfo[p].connectors[i].pad=NBlue::callback->i915b->display->bconnectors[i].pad;
		pinfo[p].connectors[i].type=NBlue::callback->i915b->display->bconnectors[i].type;
		pinfo[p].connectors[i].flags=NBlue::callback->i915b->display->bconnectors[i].flags;
	}
	
	pinfo[p].connectors[1].type=ConnectorDummy;
	
	//pinfo[p].connectors[0].flags-=CNConnectorAlwaysConnected;
	pinfo[p].connectors[0].pipe=1;

	
	OSArray *connectorArray = OSArray::withCapacity(6);
	for (int i = 0; i < 6; i++) {
		OSDictionary *connectorDict = OSDictionary::withCapacity(10);
		connectorDict->setObject("Index", OSNumber::withNumber(pinfo[p].connectors[i].index, 32));
		connectorDict->setObject("busId", OSNumber::withNumber(pinfo[p].connectors[i].busId, 32));
		connectorDict->setObject("pipe", OSNumber::withNumber(pinfo[p].connectors[i].pipe, 32));
		connectorDict->setObject("pad", OSNumber::withNumber(pinfo[p].connectors[i].pad, 32));
		connectorDict->setObject("type", OSNumber::withNumber(pinfo[p].connectors[i].type, 32));
		connectorDict->setObject("flags", OSNumber::withNumber(pinfo[p].connectors[i].flags, 32));
		connectorArray->setObject(connectorDict);
		connectorDict->release();
	}
	NBlue::callback->iGPU->setProperty("Driver_Connectors", connectorArray);
	connectorArray->release();
	
	auto ret=FunctionCast(getOSInformation2, callback->ogetOSInformation2)(that );
	return ret;
}

uint64_t  Gen11::getOSInformation(void *that)
{//tgl
	
	if (NBlue::callback->intel_opregion_setup()!=0) panic("BAD BIOS");

	
	struct PlatformInfo *pinfo =static_cast<PlatformInfo *>(callback->gPlatformInformationList);
	
	int p=1;
	pinfo[p].fInfoFlags=
	FB_FLAG_DISABLE_PIPE_SCRAMBLE|FB_FLAG_FRAMEBUFFER_COMPRESSION/*|FB_FLAG_ENABLE_BACKLIGHT_REG_CONTROL*/
	/*|FB_FLAG_FORCE_POWER_ALWAYS_CONNECTED*/|FB_FLAG_AVOID_FAST_LINK_TRAINING/*|FB_FLAG_ALLOW_CONNECTOR_RECOVER
	|FB_FLAG_USE_VIDEO_TURBO|FB_FLAG_ALTERNATE_PWM_INCREMENT2*/;
	
	
		pinfo[p].cameliav=0;
		//CamelliaTcon2=2 BanksiaTcon=3
	
		pinfo[p].fMobile=1;
		pinfo[p].fPipeCount=3;
		pinfo[p].fInfoPortCount=3;
		pinfo[p].fInfoFramebufferCount=2;

		pinfo[p].fSliceCount=1;
		pinfo[p].fmaxEuCount=8;
		pinfo[p].fsubslices=8;
	
	//pinfo[p].fInfoFBCompressionMemorySize=	0xB6D000;
	//pinfo[p].fVideoTurboFreq=270000000;
	
	
	for (int i = 0; i < 6; i++) {
		pinfo[p].connectors[i].index=NBlue::callback->i915b->display->bconnectors[i].index;
		pinfo[p].connectors[i].busId=NBlue::callback->i915b->display->bconnectors[i].busId;
		pinfo[p].connectors[i].pipe=NBlue::callback->i915b->display->bconnectors[i].pipe;
		pinfo[p].connectors[i].pad=NBlue::callback->i915b->display->bconnectors[i].pad;
		pinfo[p].connectors[i].type=NBlue::callback->i915b->display->bconnectors[i].type;
		pinfo[p].connectors[i].flags=NBlue::callback->i915b->display->bconnectors[i].flags;
	}
	
	pinfo[p].connectors[1].type=ConnectorDummy;
	//pinfo[p].connectors[0].pipe=1;
		
	OSArray *connectorArray = OSArray::withCapacity(6);
	for (int i = 0; i < 6; i++) {
		OSDictionary *connectorDict = OSDictionary::withCapacity(10);
		connectorDict->setObject("Index", OSNumber::withNumber(pinfo[p].connectors[i].index, 32));
		connectorDict->setObject("busId", OSNumber::withNumber(pinfo[p].connectors[i].busId, 32));
		connectorDict->setObject("pipe", OSNumber::withNumber(pinfo[p].connectors[i].pipe, 32));
		connectorDict->setObject("pad", OSNumber::withNumber(pinfo[p].connectors[i].pad, 32));
		connectorDict->setObject("type", OSNumber::withNumber(pinfo[p].connectors[i].type, 32));
		connectorDict->setObject("flags", OSNumber::withNumber(pinfo[p].connectors[i].flags, 32));
		connectorArray->setObject(connectorDict);
		connectorDict->release();
	}
	NBlue::callback->iGPU->setProperty("Driver_Connectors", connectorArray);
	connectorArray->release();
	
	auto ret=FunctionCast(getOSInformation, callback->ogetOSInformation)(that );
	return ret;
}



IOReturn Gen11::wrapPavpSessionCallback( void *intelAccelerator, int32_t sessionCommand, uint32_t sessionAppId, uint32_t *a4, bool flag) {
	
	if (sessionCommand == 4) {
		return kIOReturnTimeout;
	}

	return FunctionCast(wrapPavpSessionCallback, callback->orgPavpSessionCallback)(intelAccelerator, sessionCommand, sessionAppId, a4, flag);
}

int Gen11::dozero()
{
	return 0;
}

void Gen11::dovoid()
{
}

bool Gen11::dotrue()
{
	return true;
}

bool Gen11::dofalse()
{
	return false;
}



void Gen11::FBMemMgr_Init(void *that)
{
	
	FunctionCast(FBMemMgr_Init, callback->oFBMemMgr_Init)(that);
	ccont2=that;
	callback->rmmioPtr=(getMember<volatile UInt32 *>(that, kexticl ? 0x9b8 : 0x9b0));
	callback->rmmioLen = getMember<uint32_t>(that, 0xC78);
	if (kexticl) ccont=that;
	else
	ccont = getMember<void *>(that, 0xc40);
	
}

uint32_t Gen11::AppleIntelFramebufferinit(void *frame,void *cont,uint param_2)
{
	if (!kexticl) {
		getMember<void *>(frame, 0x4a40) = ccont;
		getMember<void *>(frame, 0xc40) = ccont;
	}
	auto ret=FunctionCast(AppleIntelFramebufferinit, callback->oAppleIntelFramebufferinit)(frame,cont,param_2 );
	if (param_2==0) frame0=(IOFramebuffer *)frame;
	return ret;
}

uint64_t  Gen11::AppleIntelPlaneinit(void *that,uint param_1)
{
	auto ret= FunctionCast(AppleIntelPlaneinit, callback->oAppleIntelPlaneinit)(that,param_1 );
	getMember<void *>(that, 0x90) = ccont;
	return ret;
}

unsigned long Gen11::AppleIntelScalerinit(void *that,uint param_1)
{
	auto ret=  FunctionCast(AppleIntelScalerinit, callback->oAppleIntelScalerinit)(that,param_1 );
	getMember<void *>(that, 0x28) = ccont;
	return ret;
}






void Gen11::hwGetCRTC(void *that,void *param_1,void *param_2)
{
	FunctionCast(hwGetCRTC, callback->ohwGetCRTC)(that,param_1,param_2 );
	
	if (bk==2){
		NBlue::callback->parse_backlight();
		bk=1;
	}
	
	if (bk==1){
		
		//bklfrequency
		getMember<uint32_t>(that, kexticl ? 0xe54 : kexttgld ? 0xe6c : 0x60)=NBlue::callback->i915b->display->panel.backlight.pwm_level_max;
		
		//bkllvl
		if (getMember<uint32_t>(that, kexticl ? 0xe4c : kexttgld ? 0xe64 : 0xe58)<NBlue::callback->i915b->display->panel.backlight.level)
		getMember<uint32_t>(that, kexticl ? 0xe4c : kexttgld ? 0xe64 : 0xe58)=NBlue::callback->i915b->display->panel.backlight.level;
		
		if (getMember<uint32_t>(that, kexticl ? 0xe50 : kexttgld ? 0xe68 : 0xe5c)>NBlue::callback->i915b->display->panel.backlight.level)
			getMember<uint32_t>(that, kexticl ? 0xe4c : kexttgld ? 0xe64 : 0xe58)=getMember<uint32_t>(that, kexticl ? 0xe50 : kexttgld ? 0xe68 : 0xe5c);
		
		//bkllvl_saved
		if (getMember<uint32_t>(that, kexticl ? 0xe50 : kexttgld ? 0xe68 : 0xe5c)<NBlue::callback->i915b->display->panel.backlight.level)
		getMember<uint32_t>(that, kexticl ? 0xe50 : kexttgld ? 0xe68 : 0xe5c)=NBlue::callback->i915b->display->panel.backlight.level;
		
		
		u32 pwm_ctl;

		pwm_ctl = NBlue::callback->readReg32( BXT_BLC_PWM_CTL(NBlue::callback->i915b->display->panel.backlight.controller));
		if (pwm_ctl & BXT_BLC_PWM_ENABLE) {
			pwm_ctl &= ~BXT_BLC_PWM_ENABLE;
			NBlue::callback->writeReg32( BXT_BLC_PWM_CTL(NBlue::callback->i915b->display->panel.backlight.controller),
					   pwm_ctl);
		}

		NBlue::callback->writeReg32( BXT_BLC_PWM_FREQ(NBlue::callback->i915b->display->panel.backlight.controller),
									NBlue::callback->i915b->display->panel.backlight.pwm_level_max);

		NBlue::callback->writeReg32( BXT_BLC_PWM_DUTY(NBlue::callback->i915b->display->panel.backlight.controller), NBlue::callback->i915b->display->panel.backlight.level);

		pwm_ctl = 0;
		if (NBlue::callback->i915b->display->panel.backlight.active_low_pwm)
			pwm_ctl |= BXT_BLC_PWM_POLARITY;

		NBlue::callback->writeReg32( BXT_BLC_PWM_CTL(NBlue::callback->i915b->display->panel.backlight.controller), pwm_ctl);
		NBlue::callback->readReg32( BXT_BLC_PWM_CTL(NBlue::callback->i915b->display->panel.backlight.controller));
		NBlue::callback->writeReg32( BXT_BLC_PWM_CTL(NBlue::callback->i915b->display->panel.backlight.controller),
				   pwm_ctl | BXT_BLC_PWM_ENABLE);

	}

}


void Gen11::hwSetPanelPowerConfig(void *that, uint param_1)
{
	struct intel_display *display=NBlue::callback->i915b->display;
	if (bk==2){
		NBlue::callback->parse_backlight();
		bk=1;
	}
	
	if (kexticl) getMember<uint32_t>(that, 0xd00)= param_1;
	else getMember<uint32_t>(that, 0xd48)= param_1;
	
	struct intel_pps_delays p = NBlue::callback->i915b->display->panel.pps.pps_delays;


	if (kexttgld)
	{
		getMember<uint32_t>(that, 0x1550)= p.backlight_on;
		getMember<uint32_t>(that, 0x1554)= p.backlight_off;
		getMember<uint32_t>(that, 0x1558) = p.power_up;
		getMember<uint32_t>(that, 0x155c)= p.power_down;
		getMember<uint32_t>(that, 0x1560)= p.power_cycle;
	}
	else
	{
		getMember<uint32_t>(that, 0x1548)= p.backlight_on;
		getMember<uint32_t>(that, 0x1550)= p.backlight_off;
		getMember<uint32_t>(that, 0x154c) = p.power_up;
		getMember<uint32_t>(that, 0x1554)= p.power_down;
		getMember<uint32_t>(that, 0x1558)= p.power_cycle;
	}
	
	struct pps_registers regs=NBlue::callback->i915b->display->panel.regs;
	
	
	uint32_t PCH_PP_ON_DELAYS = REG_FIELD_PREP(PANEL_POWER_UP_DELAY_MASK, p.power_up) |
		REG_FIELD_PREP(PANEL_LIGHT_ON_DELAY_MASK, p.backlight_on);
	uint32_t PCH_PP_OFF_DELAYS = REG_FIELD_PREP(PANEL_LIGHT_OFF_DELAY_MASK, p.backlight_off) |
		REG_FIELD_PREP(PANEL_POWER_DOWN_DELAY_MASK, p.power_down);

	NBlue::callback->writeReg32(regs.pp_on, PCH_PP_ON_DELAYS);
	NBlue::callback->writeReg32(regs.pp_off, PCH_PP_OFF_DELAYS);

	int div = NBlue::callback->i915b->display->panel.rawclk_freq / 1000;
	
	if (NBlue::callback->readReg32(regs.pp_div)!=-1)
		NBlue::callback->writeReg32( regs.pp_div,
				   REG_FIELD_PREP(PP_REFERENCE_DIVIDER_MASK,
						  (100 * div) / 2 - 1) |
				   REG_FIELD_PREP(PANEL_POWER_CYCLE_DELAY_MASK,
						  DIV_ROUND_UP(p.power_cycle, 1000) + 1));
	else
		intel_de_rmw(display, regs.pp_ctrl, BXT_POWER_CYCLE_DELAY_MASK,
				 REG_FIELD_PREP(BXT_POWER_CYCLE_DELAY_MASK,
						DIV_ROUND_UP(p.power_cycle, 1000) + 1));
	

	if (kexticl)
	{
		getMember<uint32_t>(that, 0xd30)=PCH_PP_ON_DELAYS;
		getMember<uint32_t>(that, 0xd34)=PCH_PP_OFF_DELAYS;
		getMember<uint32_t>(that, 0xd38) = NBlue::callback->readReg32(regs.pp_ctrl);
	}
	else
	{
		getMember<uint32_t>(that, 0xd78)=PCH_PP_ON_DELAYS;
		getMember<uint32_t>(that, 0xd7c)=PCH_PP_OFF_DELAYS;
		getMember<uint32_t>(that, 0xd80) = NBlue::callback->readReg32(regs.pp_ctrl);
	}

}


uint32_t Gen11::raReadRegister32(void *that,unsigned long param_1)
{
	if (that==nullptr) that=ccont;
	auto ret=FunctionCast(raReadRegister32, callback->oraReadRegister32)(that,param_1);
	return ret;
};
void Gen11::raWriteRegister32(void *that,unsigned long param_1, UInt32 param_2)
{
	if (that==nullptr) that=ccont;
	FunctionCast(raWriteRegister32, callback->oraWriteRegister32)( that,param_1,param_2);
	
};

uint32_t Gen11::enableVDDForAux(void *that,void *param_1)
{
	auto ret= FunctionCast(enableVDDForAux, callback->oenableVDDForAux)( that,param_1);

	if (ret==0xe00002eb) {
		if (getMember<bool>(that, 0xe62 ) == true) {
			IOFramebuffer *r= (IOFramebuffer *)getMember<void *>(that, 0xd60);
			r->setProperty("AAPL,LCD-PowerState-ON", true);
		}
	}
	return ret;

};

uint64_t Gen11::enableVDDForAux2(void *that,void *param_1)
{//icl
	auto ret= FunctionCast(enableVDDForAux2, callback->oenableVDDForAux2)( that,param_1);

	if (ret==0xe00002eb) {
		if (getMember<bool>(that, 0xe48) == true) {
			IOFramebuffer *r= (IOFramebuffer *)getMember<void *>(that, 0xd18);
			r->setProperty("AAPL,LCD-PowerState-ON", true);
		}
	}
	return ret;

};

uint64_t Gen11::disableVDDForAux2(void *that,void *param_1)
{//icl
	auto ret= FunctionCast(disableVDDForAux2, callback->odisableVDDForAux2)( that,param_1);

	if (ret==0xe00002eb) {
		if (getMember<bool>(that, 0xe48) == true) {
			IOFramebuffer *r= (IOFramebuffer *)getMember<void *>(that, 0xd18);
			r->setProperty("AAPL,LCD-PowerState-ON", false);
		}
	}
	return ret;

};

uint64_t Gen11::disableVDDForAux(void *that)
{
	auto ret= FunctionCast(disableVDDForAux, callback->odisableVDDForAux)( that);

	if (ret==0xe00002eb) {
		if (getMember<bool>(that,  0xe62 ) == true) {
			IOFramebuffer *r= (IOFramebuffer *)getMember<void *>(that, 0xd60);
			r->setProperty("AAPL,LCD-PowerState-ON", false);
		}
	}
	return ret;

};

IOReturn Gen11::fgetAttributeForConnection(void* framebuffer, int32_t connectIndex, uint32_t attribute,
										  unsigned long *value)
{
	const auto ret = FunctionCast(fgetAttributeForConnection, callback->ofgetAttributeForConnection)(
																										   framebuffer, connectIndex, attribute, value);
	
	
		
		
	if (attribute != 'bklt') { return ret; }
	
	u32 v=NBlue::callback->i915b->display->panel.backlight.level;
	if (getMember<uint32_t>(ccont2, kexticl ? 0xe4c: kexttgld ? 0xe64 : 0xe58)<v)  getMember<uint32_t>(ccont2, kexticl ? 0xe4c: kexttgld ? 0xe64 : 0xe58)=v;
	
	*value=getMember<uint32_t>(ccont2, kexticl ? 0xe4c: kexttgld ? 0xe64 : 0xe58);
	value[1] = 0;
	value[2] = 0xffff;
	
	return kIOReturnSuccess;
};



static uint32_t oldWSAAState = 0;
uint32_t Gen11::fsetAttribute(void *that, uint param_1, unsigned long param_2)
{
	if (0)
	if (param_1 == 'wsrv')
	{
		uint32_t fbNum = getMember<uint32_t>(that, 0x1dc);
		
		if (fbNum == 1)
		{
			getMember<uint32_t>(that, kexticl ? 0x8a20 : 0x44e0)=0x7fffffff;//IgnoreConnection
			return 0xe00002c7; // kIOReturnNoDevice
		}
		oldWSAAState=getMember<uint32_t>(that, kexticl ? 0x85dc : 0x420c);
		oldWSAAState = oldWSAAState & 0xffffffef;
		FunctionCast(fsetAttribute, callback->ofsetAttribute)(that, param_1, param_2);
		IOSleep(1);
		
		if (0)
		if (!kexticl)
		if (param_2==0x11)
		{
			
			getMember<uint8_t>(ccont2, kexticl ? 0xe45 : 0xe5f)=0;//wservp1
			
			void* dpath=getMember<void*>(that, kexticl ? 0xe45 : 0x4a08);
			void *pla=getMember<void*>(dpath, kexticl ? 0xe45 : 0x32c8);
			
			/*getMember<uint32_t>(that, 0x49e0)=1;//sleepwake
			getMember<uint32_t>(that, 0x4284)=1;//sleepmode
			FunctionCast(fsetAttribute, callback->ofsetAttribute)(frame0, 'powr',2);
			IOSleep(1);
			*/
			
			getMember<int32_t>(that, kexticl ? 0xe45 : 0x4004)=-2;//fTransactionState
			//getMember<int32_t>(that, kexticl ? 0xe45 : 0x4008)=-1;//fTransactionState2
			
			getMember<uint32_t>(that, kexticl ? 0xe45 : 0x4210)=1;//fWSAAState2
			getMember<uint8_t>(pla, kexticl ? 0xe45 : 0x84)=1;//planeconfigured
			getMember<uint32_t>(that, kexticl ? 0xe45 : 0x4214)=1;//fWSAAState3
			updatePlane(pla,true);
			getMember<uint32_t>(that, kexticl ? 0xe45 : 0x4214)=0;//fWSAAState3
			getMember<uint32_t>(that, kexticl ? 0xe45 : 0x49e0)=4;//sleepwake
			IOSleep(1);
			
			
			getMember<uint32_t>(that, kexticl ? 0xe45 : 0x4214)=1;//fWSAAState3
			RestoreTransactions(that,true);
			getMember<uint32_t>(that, kexticl ? 0xe45 : 0x4214)=0;//fWSAAState3
			IOSleep(1);
			
			getMember<uint32_t>(that, kexticl ? 0xe45 : 0x420c)=0x4;//fWSAAState
			
			//getMember<uint8_t>(that, 0x44de)=1;//IOFBNeedsRefresh
			//getMember<uint8_t>(that, 0x4b8c)=0;//unplugged
			//getMember<uint8_t>(that, 0x1e0)=1;//fOnline
			

			
			
		}

		return 0;
		
	}

	return FunctionCast(fsetAttribute, callback->ofsetAttribute)(that, param_1, param_2);
}

uint32_t Gen11::configureReport	(void *that,void *param_1,uint param_2,void *param_3,void *param_4)
{
	
	auto ret=FunctionCast(configureReport, callback->oconfigureReport)(that ,param_1,param_2,param_3,param_4);
	
	if (Report==-1)
	{
		Report=0;
		

		/*getMember<uint32_t>(frame0, 0x420c)=0x11;
		
		getMember<uint32_t>(frame0, 0x49e0)=1;//sleepwake
		getMember<uint32_t>(frame0, 0x4284)=1;//sleepmode
		FunctionCast(fsetAttribute, callback->ofsetAttribute)(frame0, 'powr',2);
		IOSleep(1);
		//fsetAttribute(frame0, 'wsrv',4);
		//IOSleep(1);
		*/
		/*getMember<uint32_t>(frame0, 0x4284)=2;//sleepmode
		
		getMember<uint8_t>(frame0, 0x1e0)=1;//fOnline
		getMember<uint8_t>(frame0, 0x1e1)=0;//newOnlineState
		getMember<uint8_t>(ccont2, 0xe61)=1;//lidisclosed
		wrapSetAttributeForConnection(frame0, 0, 'prob', 1);
		IOSleep(1);
		getMember<uint8_t>(frame0, 0x1e0)=0;//fOnline
		getMember<uint8_t>(frame0, 0x1e1)=1;//newOnlineState
		getMember<uint8_t>(ccont2, 0xe61)=0;//lidisclosed
		wrapSetAttributeForConnection(frame0, 0, 'prob', 1);
		IOSleep(1);*/
		
		//IODelay(1000);
		//getMember<uint32_t>(that, 0x4284)=1;//sleepmode
		//fsetAttribute(that, 'powr',2);

	}
	
	
	return ret;
}

uint32_t Gen11::fgetAttribute(void *that, uint param_1, unsigned long *param_2)
{

	return FunctionCast(fgetAttribute, callback->ofgetAttribute)(that, param_1, param_2);
}

unsigned long  Gen11::fcallPlatformFunction(void *that,void *param_1,bool param_2,void *param_3,void *param_4,void *param_5,void *param_6)
{
	
	return FunctionCast(fcallPlatformFunction, callback->ofcallPlatformFunction)(that, param_1, param_2, param_3, param_4, param_5, param_6);
}

IOReturn Gen11::wrapSetAttributeForConnection(void* framebuffer, int32_t connectIndex, uint32_t attribute,
											  unsigned long value)
{
	const auto ret = FunctionCast(wrapSetAttributeForConnection, callback->owrapSetAttributeForConnection)(
																										   framebuffer, connectIndex, attribute, value);
	
	if (attribute != 'bklt') { return ret; }
	
	
	if (value<NBlue::callback->i915b->display->panel.backlight.level)  value=NBlue::callback->i915b->display->panel.backlight.level;
	
	NBlue::callback->writeReg32( BXT_BLC_PWM_DUTY(NBlue::callback->i915b->display->panel.backlight.controller), value);
	getMember<uint32_t>(ccont2, kexticl ? 0xe4c: kexttgld ? 0xe64 : 0xe58)=value;
	return kIOReturnSuccess;
};

void Gen11::updatePlane(void *that,bool param_1)
{
	return FunctionCast(updatePlane, callback->oupdatePlane)(that, param_1);
}

void Gen11::RestoreTransactions(void *that,bool param_1)
{
	return FunctionCast(RestoreTransactions, callback->oRestoreTransactions)(that, param_1);
}


void  Gen11::overridePowerWellsState(void *that,bool param_1)
{
	return FunctionCast(overridePowerWellsState, callback->ooverridePowerWellsState)(that, param_1);
}

int Gen11::probeBootPipe(void *that,bool *param_1,void *param_2)
{
	return FunctionCast(probeBootPipe, callback->oprobeBootPipe)(that, param_1,param_2);
}

void  Gen11::disablePowerWellAux(void *that,uint param_1)
{
	return FunctionCast(disablePowerWellAux, callback->odisablePowerWellAux)(that, param_1);
}

void  Gen11::disablePowerWellDDI(void *that,uint param_1)
{
	return FunctionCast(disablePowerWellDDI, callback->odisablePowerWellDDI)(that, param_1);
}

void  Gen11::disablePowerWellPG(void *that,uint param_1)
{
	return FunctionCast(disablePowerWellPG, callback->odisablePowerWellPG)(that, param_1);
}



void Gen11::updateSliceConfig(void *that, uint32_t val)
{
	IGSliceConfig requestedConfig;
	requestedConfig.raw = val;
	if (requestedConfig.getSubSliceCount()>4) requestedConfig.setSubSliceCount(4);
	
	if (requestedConfig.getEUCount()>8) requestedConfig.setEUCount(8);
	if (requestedConfig.getSliceCount()!=1) requestedConfig.setSliceCount(1);
	
	FunctionCast(updateSliceConfig, callback->oupdateSliceConfig)( that,requestedConfig.raw);
}

void Gen11::setAsyncSliceCount(void *that, uint32_t val)
{
	IGSliceConfig requestedConfig;
	requestedConfig.raw = val;
	if (requestedConfig.getSubSliceCount()>4) requestedConfig.setSubSliceCount(4);
	
	if (requestedConfig.getEUCount()>8) requestedConfig.setEUCount(8);
	if (requestedConfig.getSliceCount()!=1) requestedConfig.setSliceCount(1);
	
	FunctionCast(setAsyncSliceCount, callback->osetAsyncSliceCount)( that,requestedConfig.raw);
}

unsigned long  Gen11::setSliceConfig(void *that, uint32_t val)
{
	IGSliceConfig requestedConfig;
	requestedConfig.raw = val;
	if (requestedConfig.getSubSliceCount()>4) requestedConfig.setSubSliceCount(4);
	
	if (requestedConfig.getEUCount()>8) requestedConfig.setEUCount(8);
	if (requestedConfig.getSliceCount()!=1) requestedConfig.setSliceCount(1);
	
	getMember<uint32_t>(that, kexticl ? 0x12cc : 0xdd8)=requestedConfig.getSliceCount();//REQSlices
	getMember<uint32_t>(that, kexticl ? 0xdd8 : 0xde0)=requestedConfig.getEUCount();//MAXeus
	getMember<uint32_t>(that, kexticl ? 0xddc : 0xde4)=requestedConfig.getEUCount();//MINeus
	getMember<uint32_t>(that, kexticl ? 0x12d0 : 0xddc)=requestedConfig.getSubSliceCount();//REQNumSubSlices

	
	return FunctionCast(setSliceConfig, callback->osetSliceConfig)( that,requestedConfig.raw);
}

void Gen11::setAsyncSliceCount2(void *that, uint32_t val)
{
	IGSliceConfig requestedConfig;
	requestedConfig.raw = val;
	if (requestedConfig.getSubSliceCount()>4) requestedConfig.setSubSliceCount(4);
	
	if (requestedConfig.getEUCount()>8) requestedConfig.setEUCount(8);
	if (requestedConfig.getSliceCount()!=1) requestedConfig.setSliceCount(1);
	
	FunctionCast(setAsyncSliceCount2, callback->osetAsyncSliceCount2)( that,requestedConfig.raw);
}




bool Gen11::fconfigureDevice(void *param_1)
{
	auto ret= FunctionCast(fconfigureDevice, callback->ofconfigureDevice)( param_1);
	return ret;
}

uint64_t Gen11::finitDeviceMemory(void *that)
{
	auto ret= FunctionCast(finitDeviceMemory, callback->ofinitDeviceMemory)( that);

	return ret;
}

static void _wa_add(struct i915_wa_list *wal, const struct i915_wa *wa)
{
	unsigned int addr = i915_mmio_reg_offset(wa->reg);
	struct drm_i915_private *i915 = wal->gt->i915;
	unsigned int start = 0, end = wal->count;
	const unsigned int grow = WA_LIST_CHUNK;
	struct i915_wa *wa_;

	if (IS_ALIGNED(wal->count, grow)) { /* Either uninitialized or full. */
		struct i915_wa *list;
		size_t new_size = ALIGN(wal->count + 1, grow) * sizeof(struct i915_wa);
		size_t old_size = wal->count * sizeof(struct i915_wa);

		list = (struct i915_wa *)IOMalloc(new_size);
		if (!list) {
			return;
		}

		if (wal->list) {
			memcpy(list, wal->list, old_size);
			IOFree(wal->list, old_size);
		}

		wal->list = list;
	}

	while (start < end) {
		unsigned int mid = start + (end - start) / 2;

		if (i915_mmio_reg_offset(wal->list[mid].reg) < addr) {
			start = mid + 1;
		} else if (i915_mmio_reg_offset(wal->list[mid].reg) > addr) {
			end = mid;
		} else {
			wa_ = &wal->list[mid];

			if ((wa->clr | wa_->clr) && !(wa->clr & ~wa_->clr)) {
				wa_->set &= ~wa->clr;
			}

			wal->wa_count++;
			wa_->set |= wa->set;
			wa_->clr |= wa->clr;
			wa_->read |= wa->read;
			return;
		}
	}

	wal->wa_count++;
	wa_ = &wal->list[wal->count++];
	*wa_ = *wa;

	while (wa_-- > wal->list) {
		if (i915_mmio_reg_offset(wa_[1].reg) >
			i915_mmio_reg_offset(wa_[0].reg))
			break;

		swap(wa_[1], wa_[0]);
	}
}

static void wa_add(struct i915_wa_list *wal, u32 reg,
		   u32 clear, u32 set, u32 read_mask, bool masked_reg)
{
	struct i915_wa wa = {
		.reg  = reg,
		.clr  = clear,
		.set  = set,
		.read = read_mask,
		.masked_reg = masked_reg,
	};

	_wa_add(wal, &wa);
}

static void wa_mcr_add(struct i915_wa_list *wal, u32 reg,
			   u32 clear, u32 set, u32 read_mask, bool masked_reg)
{
	struct i915_wa wa = {
		.mcr_reg = reg,
		.clr  = clear,
		.set  = set,
		.read = read_mask,
		.masked_reg = masked_reg,
		.is_mcr = 1,
	};

	_wa_add(wal, &wa);
}

static void
wa_write_clr_set(struct i915_wa_list *wal, u32 reg, u32 clear, u32 set)
{
	wa_add(wal, reg, clear, set, clear | set, false);
}

static void
wa_mcr_write_clr_set(struct i915_wa_list *wal, u32 reg, u32 clear, u32 set)
{
	wa_mcr_add(wal, reg, clear, set, clear | set, false);
}

static void
wa_write(struct i915_wa_list *wal, u32 reg, u32 set)
{
	wa_write_clr_set(wal, reg, ~0, set);
}

static void
wa_write_or(struct i915_wa_list *wal, u32 reg, u32 set)
{
	wa_write_clr_set(wal, reg, set, set);
}

static void
wa_mcr_write_or(struct i915_wa_list *wal, u32 reg, u32 set)
{
	wa_mcr_write_clr_set(wal, reg, set, set);
}

static void
wa_write_clr(struct i915_wa_list *wal, u32 reg, u32 clr)
{
	wa_write_clr_set(wal, reg, clr, 0);
}

static void
wa_mcr_write_clr(struct i915_wa_list *wal, u32 reg, u32 clr)
{
	wa_mcr_write_clr_set(wal, reg, clr, 0);
}


static void
wa_masked_en(struct i915_wa_list *wal, u32 reg, u32 val)
{
	wa_add(wal, reg, 0, REG_MASKED_FIELD_ENABLE(val), val, true);
}

static void
wa_mcr_masked_en(struct i915_wa_list *wal, u32 reg, u32 val)
{
	wa_mcr_add(wal, reg, 0, REG_MASKED_FIELD_ENABLE(val), val, true);
}

static void
wa_masked_dis(struct i915_wa_list *wal, u32 reg, u32 val)
{
	wa_add(wal, reg, 0, REG_MASKED_FIELD_DISABLE(val), val, true);
}

static void
wa_mcr_masked_dis(struct i915_wa_list *wal, u32 reg, u32 val)
{
	wa_mcr_add(wal, reg, 0, REG_MASKED_FIELD_DISABLE(val), val, true);
}

static void
wa_masked_field_set(struct i915_wa_list *wal, u32 reg,
			u32 mask, u32 val)
{
	wa_add(wal, reg, 0, REG_MASKED_FIELD(mask, val), mask, true);
}

static void
wa_mcr_masked_field_set(struct i915_wa_list *wal, u32 reg,
			u32 mask, u32 val)
{
	wa_mcr_add(wal, reg, 0, REG_MASKED_FIELD(mask, val), mask, true);
}
static void wa_list_apply(const struct i915_wa_list *wal)
{
	struct intel_gt *gt = wal->gt;
	struct drm_i915_private *i915=gt->i915;
	struct intel_display *display = i915->display;
	
	unsigned long flags;
	struct i915_wa *wa;
	unsigned int i;

	if (!wal->count)
		return;
/*
	fw = wal_get_fw_for_rmw(uncore, wal);

	intel_gt_mcr_lock(gt, &flags);
	spin_lock(&uncore->lock);
	intel_uncore_forcewake_get__locked(uncore, fw);
*/
	
	for (i = 0, wa = wal->list; i < wal->count; i++, wa++) {
		u32 val, old = 0;

		/* open-coded rmw due to steering */
		if (wa->clr)
			old = wa->is_mcr ?
			intel_de_read(display, wa->mcr_reg) :
			intel_de_read(display, wa->reg);
		val = (old & ~wa->clr) | wa->set;
		if (val != old || !wa->clr) {
			if (wa->is_mcr)
				intel_de_write(display, wa->mcr_reg, val);
			else
				intel_de_write(display, wa->reg, val);
		}

		/*if (IS_ENABLED(CONFIG_DRM_I915_DEBUG_GEM)) {
			u32 val = wa->is_mcr ?
				intel_gt_mcr_read_any_fw(gt, wa->mcr_reg) :
			intel_de_read(uncore, wa->reg);

			wa_verify(gt, wa, val, wal->name, "application");
		}*/
	}

	/*intel_uncore_forcewake_put__locked(uncore, fw);
	spin_unlock(&uncore->lock);
	intel_gt_mcr_unlock(gt, flags);*/
}


static void
wa_14011060649(struct intel_gt *gt, struct i915_wa_list *wal)
{
	struct intel_engine_cs *engine;
	int id;

	for_each_engine(engine, gt, id) {
		if (engine->classb != VIDEO_DECODE_CLASS ||
			(engine->instance % 2))
			continue;

		wa_write_or(wal, VDBOX_CGCTL3F10(engine->mmio_base),
				IECPUNIT_CLKGATE_DIS);
	}
}


static void __set_mcr_steering(struct i915_wa_list *wal,
				   u32 steering_reg,
				   unsigned int slice, unsigned int subslice)
{
	u32 mcr, mcr_mask;

	mcr = GEN11_MCR_SLICE(slice) | GEN11_MCR_SUBSLICE(subslice);
	mcr_mask = GEN11_MCR_SLICE_MASK | GEN11_MCR_SUBSLICE_MASK;

	wa_write_clr_set(wal, steering_reg, mcr_mask, mcr);
}

unsigned int
intel_sseu_get_hsw_subslices(const struct sseu_dev_info *sseu, u8 slice)
{
	//WARN_ON(sseu->has_xehp_dss);
	if ((slice >= sseu->max_slices))
		return 0;

	return sseu->subslice_mask.hsw[slice];
}

static void __add_mcr_wa(struct intel_gt *gt, struct i915_wa_list *wal,
			 unsigned int slice, unsigned int subslice)
{
	__set_mcr_steering(wal, GEN8_MCR_SELECTOR, slice, subslice);

	gt->default_steering.groupid = slice;
	gt->default_steering.instanceid = subslice;

	//debug_dump_steering(gt);
}

static void
icl_wa_init_mcr(struct intel_gt *gt, struct i915_wa_list *wal)
{
	const struct sseu_dev_info *sseu = &gt->info.sseu;
	unsigned int subslice;

	//GEM_BUG_ON(GRAPHICS_VER(gt->i915) < 11);
	//GEM_BUG_ON(hweight8(sseu->slice_mask) > 1);

	subslice = __ffs(intel_sseu_get_hsw_subslices(sseu, 0));

	if (gt->info.l3bank_mask & BIT(subslice))
		gt->steering_table[L3BANK] = NULL;

	__add_mcr_wa(gt, wal, 0, subslice);
}

static void
gen12_gt_workarounds_init(struct intel_gt *gt, struct i915_wa_list *wal)
{
	icl_wa_init_mcr(gt, wal);

	wa_14011060649(gt, wal);

	wa_mcr_write_or(wal, GEN10_DFR_RATIO_EN_AND_CHICKEN, DFR_DISABLE);
	
	wa_add(wal, GEN7_MISCCPCTL, GEN12_DOP_CLOCK_GATE_RENDER_ENABLE,
		   0, 0, false);
	
}


static u32 guc_ctl_debug_flags(struct intel_guc *guc)
{
	u32 flags = 0;

	//if (!GUC_LOG_LEVEL_IS_VERBOSE(level))
		flags |= GUC_LOG_DISABLED;
	/*else
		flags |= GUC_LOG_LEVEL_TO_VERBOSITY(level) <<
			 GUC_LOG_VERBOSITY_SHIFT;
*/
	return flags;
}

static u32 guc_ctl_feature_flags(struct intel_guc *guc)
{
	u32 flags = 0;

	/*
	 * Enable PXP GuC autoteardown flow.
	 * NB: MTL does things differently.
	 */
	//if (HAS_PXP(gt->i915) && !IS_METEORLAKE(gt->i915))
	//	flags |= GUC_CTL_ENABLE_GUC_PXP_CTL;

	//if (!intel_guc_submission_is_used(guc))
	//	flags |= GUC_CTL_DISABLE_SCHEDULER;

	//if (intel_guc_slpc_is_used(guc))
	//	flags |= GUC_CTL_ENABLE_SLPC;

	return flags;
}

static void _guc_log_init_sizes(struct intel_guc_log *log)
{
	static const struct guc_log_section sections[GUC_LOG_SECTIONS_LIMIT] = {
		{
			GUC_LOG_CRASH_MASK >> GUC_LOG_CRASH_SHIFT,
			GUC_LOG_LOG_ALLOC_UNITS,
			GUC_LOG_DEFAULT_CRASH_BUFFER_SIZE,
			"crash dump"
		},
		{
			GUC_LOG_DEBUG_MASK >> GUC_LOG_DEBUG_SHIFT,
			GUC_LOG_LOG_ALLOC_UNITS,
			GUC_LOG_DEFAULT_DEBUG_BUFFER_SIZE,
			"debug",
		},
		{
			GUC_LOG_CAPTURE_MASK >> GUC_LOG_CAPTURE_SHIFT,
			GUC_LOG_CAPTURE_ALLOC_UNITS,
			GUC_LOG_DEFAULT_CAPTURE_BUFFER_SIZE,
			"capture",
		}
	};
	int i;

	for (i = 0; i < GUC_LOG_SECTIONS_LIMIT; i++)
		log->sizes[i].bytes = sections[i].default_val;

	if (log->sizes[GUC_LOG_SECTIONS_DEBUG].bytes >= SZ_1M &&
		GUC_LOG_DEFAULT_CRASH_BUFFER_SIZE < SZ_1M)
		log->sizes[GUC_LOG_SECTIONS_CRASH].bytes = SZ_1M;

	for (i = 0; i < GUC_LOG_SECTIONS_LIMIT; i++) {
		if ((log->sizes[i].bytes % SZ_1M) == 0) {
			log->sizes[i].units = SZ_1M;
			log->sizes[i].flag = sections[i].flag;
		} else {
			log->sizes[i].units = SZ_4K;
			log->sizes[i].flag = 0;
		}

		if (!IS_ALIGNED(log->sizes[i].bytes, log->sizes[i].units))
		log->sizes[i].count = log->sizes[i].bytes / log->sizes[i].units;

		if (!log->sizes[i].count) {
		} else {
			log->sizes[i].count--;
		}

		if (log->sizes[i].count > sections[i].max) {
			log->sizes[i].count = sections[i].max;
		}
	}

	if (log->sizes[GUC_LOG_SECTIONS_CRASH].units != log->sizes[GUC_LOG_SECTIONS_DEBUG].units) {
		log->sizes[GUC_LOG_SECTIONS_CRASH].units = log->sizes[GUC_LOG_SECTIONS_DEBUG].units;
		log->sizes[GUC_LOG_SECTIONS_CRASH].count = 0;
	}

	log->sizes_initialised = true;
}

static inline u64 __i915_vma_offset(void *vma)
{
	return (u64) vma;//Gen11::callback->fgetGPUVirtualAddress(vma);
	//vma->node.start + vma->guard;
}
static inline u64 i915_vma_offset(void *vma)
{
	//GEM_BUG_ON(!drm_mm_node_allocated(&vma->node));
	return __i915_vma_offset(vma);
}

static inline u32 i915_ggtt_offset(void *vma)
{
	/*GEM_BUG_ON(!i915_vma_is_ggtt(vma));
	GEM_BUG_ON(!drm_mm_node_allocated(&vma->node));
	GEM_BUG_ON(upper_32_bits(i915_vma_offset(vma)));
	GEM_BUG_ON(upper_32_bits(i915_vma_offset(vma) +
				 i915_vma_size(vma) - 1));*/
	return lower_32_bits(i915_vma_offset(vma));
}

static inline u32 intel_guc_ggtt_offset(struct intel_guc *guc,
					void *vma)
{
	u32 offset = i915_ggtt_offset(vma);

	//GEM_BUG_ON(offset < i915_ggtt_pin_bias(vma));
	//GEM_BUG_ON(range_overflows_t(u64, offset, vma->size, GUC_GGTT_TOP));

	return offset;
}

static u32 guc_ctl_log_params_flags(struct intel_guc *guc)
{
	u32 offset, flags=0;
	struct intel_guc_log *log = &guc->log;
	//GEM_BUG_ON(!log->sizes_initialised);

	offset = intel_guc_ggtt_offset(guc, log->vma) >> PAGE_SHIFT;

	flags = GUC_LOG_VALID |
		GUC_LOG_NOTIFY_ON_HALF_FULL |
		log->sizes[GUC_LOG_SECTIONS_DEBUG].flag |
		log->sizes[GUC_LOG_SECTIONS_CAPTURE].flag |
		(log->sizes[GUC_LOG_SECTIONS_CRASH].count << GUC_LOG_CRASH_SHIFT) |
		(log->sizes[GUC_LOG_SECTIONS_DEBUG].count << GUC_LOG_DEBUG_SHIFT) |
		(log->sizes[GUC_LOG_SECTIONS_CAPTURE].count << GUC_LOG_CAPTURE_SHIFT) |
		(offset << GUC_LOG_BUF_ADDR_SHIFT);

	return flags;
}

static u32 guc_ctl_ads_flags(struct intel_guc *guc)
{
	u32 ads = intel_guc_ggtt_offset(guc, guc->ads_vma) >> PAGE_SHIFT;
	u32 flags = ads << GUC_ADS_ADDR_SHIFT;

	return flags;

}


static u32 guc_ctl_wa_flags(struct intel_guc *guc)
{
	u32 flags = 0;
	struct intel_gt *gt = guc->gt;
	
	/* Wa_22012773006:gen11,gen12 < XeHP */
	if (GRAPHICS_VER(gt->i915) >= 11 &&
		GRAPHICS_VER_FULL(gt->i915) < IP_VER(12, 55))
		flags |= GUC_WA_POLLCS;

	/* Wa_14014475959 */
	/*if (IS_GFX_GT_IP_STEP(gt, IP_VER(12, 70), STEP_A0, STEP_B0) ||
		IS_DG2(gt->i915))
		flags |= GUC_WA_HOLD_CCS_SWITCHOUT;*/

	/* Wa_16019325821 */
	/* Wa_14019159160 */
	//if (IS_GFX_GT_IP_RANGE(gt, IP_VER(12, 70), IP_VER(12, 74)))
	//	flags |= GUC_WA_RCS_CCS_SWITCHOUT;

	/*
	 * Wa_14012197797
	 * Wa_22011391025
	 *
	 * The same WA bit is used for both and 22011391025 is applicable to
	 * all DG2.
	 *
	 * Platforms post DG2 prevent this issue in hardware by stalling
	 * submissions. With this flag GuC will schedule as to avoid such
	 * stalls.
	 */
	/*if (IS_DG2(gt->i915) ||
		(CCS_MASK(gt) && GRAPHICS_VER_FULL(gt->i915) >= IP_VER(12, 70)))
		flags |= GUC_WA_DUAL_QUEUE;
*/
	/* Wa_22011802037: graphics version 11/12 */
	//if (intel_engine_reset_needs_wa_22011802037(gt))
		flags |= GUC_WA_PRE_PARSER;

	/*
	 * Wa_22012727170
	 * Wa_22012727685
	 */
//	if (IS_DG2_G11(gt->i915))
	//	flags |= GUC_WA_CONTEXT_ISOLATION;

	/*
	 * Wa_14018913170: Applicable to all platforms supported by i915 so
	 * don't bother testing for all X/Y/Z platforms explicitly.
	 */
	//if (GUC_FIRMWARE_VER(guc) >= MAKE_GUC_VER(70, 7, 0))
	//	flags |= GUC_WA_ENABLE_TSC_CHECK_ON_RC6;

	return flags;
}

static u32 guc_ctl_devid(struct intel_guc *guc)
{
	
	return (NBlue::callback->deviceId << 16) | NBlue::callback->pciRevision;
}


static void guc_init_params(struct intel_guc *guc)
{
	int i;
	u32 *params = guc->params;
	//BUILD_BUG_ON(sizeof(guc->params) != GUC_CTL_MAX_DWORDS * sizeof(u32));

	params[GUC_CTL_LOG_PARAMS] = guc_ctl_log_params_flags(guc);
	params[GUC_CTL_FEATURE] = guc_ctl_feature_flags(guc);
	params[GUC_CTL_DEBUG] = guc_ctl_debug_flags(guc);
	//params[GUC_CTL_ADS] = guc_ctl_ads_flags(guc);
	params[GUC_CTL_WA] = guc_ctl_wa_flags(guc);
	params[GUC_CTL_DEVID] = guc_ctl_devid(guc);


}

void intel_guc_write_params(struct intel_guc *guc)
{
	int i;
	struct intel_display *display = NBlue::callback->i915b->display;

	
	//intel_uncore_forcewake_get(uncore, FORCEWAKE_GT);

	intel_de_write(display,  SOFT_SCRATCH(0), 0);

	for (i = 0; i < GUC_CTL_MAX_DWORDS; i++)
		intel_de_write(display,  SOFT_SCRATCH(1 + i), guc->params[i]);

	//intel_uncore_forcewake_put(uncore, FORCEWAKE_GT);
}


static inline bool guc_load_done(u32 *status, bool *success)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	u32 val = intel_de_read(display, GUC_STATUS);
	u32 uk_val = REG_FIELD_GET(GS_UKERNEL_MASK, val);
	u32 br_val = REG_FIELD_GET(GS_BOOTROM_MASK, val);

	*status = val;
	switch (uk_val) {
	case INTEL_GUC_LOAD_STATUS_READY:
		*success = true;
		return true;

	case INTEL_GUC_LOAD_STATUS_ERROR_DEVID_BUILD_MISMATCH:
	case INTEL_GUC_LOAD_STATUS_GUC_PREPROD_BUILD_MISMATCH:
	case INTEL_GUC_LOAD_STATUS_ERROR_DEVID_INVALID_GUCTYPE:
	case INTEL_GUC_LOAD_STATUS_HWCONFIG_ERROR:
	case INTEL_GUC_LOAD_STATUS_DPC_ERROR:
	case INTEL_GUC_LOAD_STATUS_EXCEPTION:
	case INTEL_GUC_LOAD_STATUS_INIT_DATA_INVALID:
	case INTEL_GUC_LOAD_STATUS_MPU_DATA_INVALID:
	case INTEL_GUC_LOAD_STATUS_INIT_MMIO_SAVE_RESTORE_INVALID:
	case INTEL_GUC_LOAD_STATUS_KLV_WORKAROUND_INIT_ERROR:
		*success = false;
		return true;
	}

	switch (br_val) {
	case INTEL_BOOTROM_STATUS_NO_KEY_FOUND:
	case INTEL_BOOTROM_STATUS_RSA_FAILED:
	case INTEL_BOOTROM_STATUS_PAVPC_FAILED:
	case INTEL_BOOTROM_STATUS_WOPCM_FAILED:
	case INTEL_BOOTROM_STATUS_LOADLOC_FAILED:
	case INTEL_BOOTROM_STATUS_JUMP_FAILED:
	case INTEL_BOOTROM_STATUS_RC6CTXCONFIG_FAILED:
	case INTEL_BOOTROM_STATUS_MPUMAP_INCORRECT:
	case INTEL_BOOTROM_STATUS_EXCEPTION:
	case INTEL_BOOTROM_STATUS_PROD_KEY_CHECK_FAILURE:
		*success = false;
		return true;
	}

	return false;
}

static void wa_init_start(struct i915_wa_list *wal, struct intel_gt *gt,
			  const char *name, const char *engine_name)
{
	wal->gt = gt;
	wal->name = name;
	wal->engine_name = engine_name;
}


static void wa_init_finish(struct i915_wa_list *wal)
{
	if (!IS_ALIGNED(wal->count, WA_LIST_CHUNK)) {
		
		size_t new_size = wal->count * sizeof(struct i915_wa);
		struct i915_wa *list = (struct i915_wa *)IOMalloc(new_size);

		if (list) {
			memcpy(list, wal->list, new_size);
			
			size_t old_size = ((wal->count + WA_LIST_CHUNK - 1) / WA_LIST_CHUNK) * WA_LIST_CHUNK * sizeof(struct i915_wa);
			
			IOFree(wal->list, old_size);
			
			wal->list = list;
		}
	}

	if (!wal->count)
		return;
}

void intel_gt_init_workarounds(struct intel_gt *gt)
{
	struct i915_wa_list *wal = &gt->wa_list;

	wa_init_start(wal, gt, "GT", "global");
	gen12_gt_workarounds_init(gt, wal);
	wa_init_finish(wal);
}


unsigned long Gen11::loadGuCBinary(void *that)
{
	struct drm_i915_private *i915=NBlue::callback->i915b;
	struct intel_gt *gt=to_gt(i915);
	struct intel_display *display = i915->display;
	struct intel_guc *guc = gt_to_guc(gt);
	
	void *m_accelerator = getMember<void *>(that, 0x38);
	
	if (!display || !m_accelerator) return 0;
	
	int err;
	struct Firmware fw = {};
	struct uc_css_header *header = nullptr;
	uint32_t ucode_size = 0;
	uint32_t rsa_size = 0;
	size_t min_expected_size = 0;
	size_t dma_buffer_size = 0;
	u32 reg_base;
	u32 reg_size;
	void *igtask = nullptr;
	void* fwBuffer = nullptr;
	void* vaddr = nullptr;
	u32 dma_flags;
	uint64_t gpuAddr = 0;
	u32 shim_flags = 0;
	u32 ctx_rsvd;
	u32 guc_wopcm_base;
	u32 wopcm_size = 0;
	u32 guc_wopcm_size;
	u32 mask = 0;
	u32 rsa_offset = 0;
	u32 i = 0;
	int dmaRetry = 0;
	u32 huc_agent =0;
	uint32_t auth;
	bool success = false;
	bool done = false;
	int retryCount = 0;
	int count = 0;
	int innerTimeout = 0;
	uint32_t status = 0;
	uint32_t bootrom = 0;
	uint32_t ukernel = 0;
	uint8_t* rsa_bytes;
	uint32_t* rsa_words;
	uint32_t rsa_val;
	
	
	fw = getFWByName("tgl_guc_70.1.1.bin");
	if (!fw.data || fw.size == 0) return 0;
	if (fw.size < sizeof(uc_css_header)) return 0;
	
	header = (struct uc_css_header *)fw.data;
	
	if (header->size_dw > header->header_size_dw) {
		ucode_size = (header->size_dw - header->header_size_dw) * 4;
	} else {
		return 0;
	}
	if (ucode_size == 0) return 0;
	
	guc->fw.private_data_size=header->private_data_size;
	
	rsa_size = header->key_size_dw * 4;
	if (rsa_size > 256) rsa_size = 256;
	
	min_expected_size = sizeof(uc_css_header) + ucode_size + rsa_size;
	if (fw.size < min_expected_size) return 0;
	
	
	dma_buffer_size = ((sizeof(uc_css_header) + ucode_size) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
	
	igtask = getMember<void*>(m_accelerator, 0x150);
	fwBuffer = IGSharedMappedBufferwithOptions(igtask, dma_buffer_size, 2, 0);
	if (!fwBuffer) return 0;
	
	vaddr = (void*)fgetVirtualAddress(fwBuffer);
	if (!vaddr) {
		IGSharedMappedBufferfree(fwBuffer);
		return 0;
	}
	
	memcpy(vaddr, fw.data , sizeof(uc_css_header) + ucode_size);
	
	gpuAddr = fgetGPUVirtualAddress(fwBuffer);
	if (gpuAddr == 0) {
		IGSharedMappedBufferfree(fwBuffer);
		return 0;
	}
	
	if (!initSchedControl(that)) return 0;
	
	SafeForceWake(m_accelerator, true, 7);
	
	intel_gt_init_workarounds(gt);
	wa_list_apply(&gt->wa_list);
	
	
	_guc_log_init_sizes(&guc->log);
	guc->log.vma=getMember<void*>(that, 0x60);
	guc->log.vma=(void*)fgetGPUVirtualAddress(guc->log.vma);
	guc_init_params(guc);
	guc->params[GUC_CTL_ADS] =((u32)(fgetGPUVirtualAddress(getMember<void *>(that, 0x9e8)) >> PAGE_SHIFT) << GUC_ADS_ADDR_SHIFT);
	//getMember<void*>(that, 0x8c)=params;
	intel_guc_write_params(guc);
	

	shim_flags = GUC_ENABLE_READ_CACHE_LOGIC |
			 GUC_ENABLE_READ_CACHE_FOR_SRAM_DATA |
			 GUC_ENABLE_READ_CACHE_FOR_WOPCM_DATA |
			 GUC_ENABLE_MIA_CLOCK_GATING;

	if (GRAPHICS_VER_FULL(i915) < IP_VER(12, 55))
		shim_flags |= GUC_DISABLE_SRAM_INIT_TO_ZEROES |
				  GUC_ENABLE_MIA_CACHING;
	
	intel_de_write(display, GUC_SHIM_CONTROL, shim_flags);
	intel_de_write(display, GEN9_GT_PM_CONFIG, GT_DOORBELL_ENABLE);
	intel_de_rmw(display, GEN6_PMINTRMSK, ARAT_EXPIRED_INTRMSK, 0);
	
	rsa_offset = sizeof(struct uc_css_header) + ucode_size;
	rsa_bytes = reinterpret_cast<uint8_t*>(fw.data) + rsa_offset;
	rsa_words = reinterpret_cast<uint32_t*>(rsa_bytes);

	for ( i = 0; i < UOS_RSA_SCRATCH_COUNT; i++) {
		rsa_val = rsa_words[i];
		intel_de_write(display, UOS_RSA_SCRATCH(i), rsa_val);
	}
	
	wopcm_size = gt->wopcm.size;
	ctx_rsvd=ICL_WOPCM_HW_CTX_RESERVED;
	
	reg_base = intel_de_read(display, DMA_GUC_WOPCM_OFFSET);
	reg_size = intel_de_read(display, GUC_WOPCM_SIZE);
	huc_agent=0;
	
	if (!(reg_size & GUC_WOPCM_SIZE_LOCKED) ||
		!(reg_base & GUC_WOPCM_OFFSET_VALID))
	{
		guc_wopcm_base = WOPCM_RESERVED_SIZE;
		guc_wopcm_base = ALIGN(guc_wopcm_base, GUC_WOPCM_OFFSET_ALIGNMENT);
		guc_wopcm_base = min(guc_wopcm_base, wopcm_size - ctx_rsvd);
		guc_wopcm_size = wopcm_size - ctx_rsvd - guc_wopcm_base;
		guc_wopcm_size &= GUC_WOPCM_SIZE_MASK;
	}
	else
	{
		guc_wopcm_base = reg_base & GUC_WOPCM_OFFSET_MASK;
		guc_wopcm_size = reg_size & GUC_WOPCM_SIZE_MASK;
	}
	
	mask = GUC_WOPCM_SIZE_MASK | GUC_WOPCM_SIZE_LOCKED;
	err = intel_uncore_write_and_verify(display, GUC_WOPCM_SIZE, guc_wopcm_size, mask,
										guc_wopcm_size | GUC_WOPCM_SIZE_LOCKED);
	if (err) goto fail;
	
	mask = GUC_WOPCM_OFFSET_MASK | GUC_WOPCM_OFFSET_VALID | huc_agent;
	 err = intel_uncore_write_and_verify(display, DMA_GUC_WOPCM_OFFSET,
										 guc_wopcm_base | huc_agent, mask,
										 guc_wopcm_base | huc_agent | GUC_WOPCM_OFFSET_VALID);
	if (err) goto fail;
		
	
	intel_de_write(display, GEN12_GUC_TLB_INV_CR, GEN12_GUC_TLB_INV_CR_INVALIDATE);
	while ((intel_de_read(display, GEN12_GUC_TLB_INV_CR) & GEN12_GUC_TLB_INV_CR_INVALIDATE) != 0)
	{
		// Spinwait
	}
	
	dma_flags=UOS_MOVE;
	intel_de_write(display, DMA_ADDR_0_LOW, lower_32_bits(gpuAddr));
	intel_de_write(display, DMA_ADDR_0_HIGH, upper_32_bits(gpuAddr) | DMA_ADDRESS_SPACE_GTT);
	intel_de_write(display, DMA_ADDR_1_LOW, 0x2000);
	intel_de_write(display, DMA_ADDR_1_HIGH, DMA_ADDRESS_SPACE_WOPCM);
	intel_de_write(display, DMA_COPY_SIZE, sizeof(struct uc_css_header) + ucode_size);
	
	intel_de_write(display, DMA_CTRL, REG_MASKED_FIELD_ENABLE(dma_flags | START_DMA));
	
	dmaRetry = 1000;
	while (intel_de_read(display, DMA_CTRL) & START_DMA) {
		IODelay(100);
		if (--dmaRetry <= 0) goto fail;
	}
	
	intel_de_write(display, DMA_CTRL, REG_MASKED_FIELD_DISABLE(dma_flags));
	
	retryCount = 3;
	for (count = 0; count < retryCount; count++) {
		success = false;
		innerTimeout = 1000;
		done = false;
		
		while (innerTimeout > 0) {

			guc_load_done(&status,&success);
			bootrom = REG_FIELD_GET(GS_BOOTROM_MASK, status);
			ukernel = REG_FIELD_GET(GS_UKERNEL_MASK, status);
			
			if (success) {
					done = true;
					break;
			}
			
			IODelay(1000);
			innerTimeout--;
		}
		
		if (done) {
			break;
		}
	}
	
	
	SafeForceWake(m_accelerator, false, 7);
	IGSharedMappedBufferfree(fwBuffer);
	
	auth = status & GS_AUTH_STATUS_MASK;
	
	if (!success)
	panic("auth %x bootrom %x ukernel %x guc_wopcm_base %x guc_wopcm_size %x",auth,bootrom,ukernel,guc_wopcm_base,guc_wopcm_size);
	
	return success ? 1 : 0;

fail:
	SafeForceWake(m_accelerator, false, 7);
	IGSharedMappedBufferfree(fwBuffer);
	return 0;
}
		
	

unsigned int Gen11::allocDoorbellId(void *param_1)
{
	return FunctionCast(allocDoorbellId, callback->oallocDoorbellId)( param_1);
}
unsigned int Gen11::stealDoorbellId(void *that)
{
	return FunctionCast(stealDoorbellId, callback->ostealDoorbellId)( that);
}
void Gen11::setDoorbellPinning(void *that,unsigned short param_1,bool param_2)
{
	FunctionCast(setDoorbellPinning, callback->osetDoorbellPinning)( that,param_1,param_2);
}


bool Gen11::hostToGuCAction2(void *that,uint *param_1,uint param_2,int param_3,uint *param_4,bool param_5)
{
	host2= FunctionCast(hostToGuCAction2, callback->ohostToGuCAction2)( that,param_1,param_2,param_3,param_4,param_5);
	return host2;
}

void Gen11::hostToGuCAction(void *that,unsigned int *param_1,unsigned int param_2,int param_3,unsigned int *param_4)
{
	FunctionCast(hostToGuCAction, callback->ohostToGuCAction)( that,param_1,param_2,param_3,param_4);
}
void  Gen11::releaseDoorbellId(void *that,unsigned short param_1)
{
	FunctionCast(releaseDoorbellId, callback->oreleaseDoorbellId)( that,param_1);
}

bool  Gen11::initSchedControl(void *that)
{
	return FunctionCast(initSchedControl, callback->oinitSchedControl)( that);
}
void* Gen11::IGSharedMappedBufferwithOptions(void *param_1,unsigned long param_2,uint param_3,uint param_4)
{
	return FunctionCast(IGSharedMappedBufferwithOptions, callback->oIGSharedMappedBufferwithOptions)( param_1,param_2,param_3,param_4);
}
long  Gen11::fgetVirtualAddress(void *that)
{
	//return *((uint64_t *)that + 7);
	return FunctionCast(fgetVirtualAddress, callback->ofgetVirtualAddress)( that);
}
uint64_t Gen11::fgetGPUVirtualAddress(void *that)
{
	return FunctionCast(fgetGPUVirtualAddress, callback->ofgetGPUVirtualAddress)( that);
}
void  Gen11::checkWOPCMSettings(void *that,unsigned long param_1,void *param_2)
{
	FunctionCast(checkWOPCMSettings, callback->ocheckWOPCMSettings)( that,param_1,param_2);
}
void Gen11::SafeForceWake(void *that,bool param_1,uint param_2)
{
	FunctionCast(SafeForceWake, callback->oSafeForceWake)( that,param_1,param_2);
	//IOSleep(1);
}

void Gen11::IGSharedMappedBufferfree(void *param_1)
{
	FunctionCast(IGSharedMappedBufferfree, callback->oIGSharedMappedBufferfree)( param_1);
}


unsigned short Gen11::acquireDoorbell(void *that, void *param_1, bool param_2)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	
	uint64_t array_base_addr = getMember<uint64_t>(that, 0x50);
	uintptr_t array_base = static_cast<uintptr_t>(array_base_addr);
	
	uint32_t context_id = getMember<uint32_t>(param_1, 0x8);
	uint64_t ctx_offset = static_cast<uint64_t>(context_id) * 0x20;
	uint32_t* ctx_desc = *reinterpret_cast<uint32_t**>(array_base + 0x10 + ctx_offset);
	
	if (ctx_desc[0] == 0) {
		uint16_t db_id = allocDoorbellId(that);
				
		if (db_id == 0x100) {
			db_id = stealDoorbellId(that);
			
			void* db_entry_val = getMember<void*>(that, 0x1e0 + static_cast<uint64_t>(db_id) * 8);
			
			if (!db_entry_val) {
				uint16_t db_per_client = getMember<uint16_t>(that, 0x9e0);
				uint16_t local_db_idx = db_id % db_per_client;
				uint32_t client_idx = db_id / db_per_client;
				uint32_t word_off = (local_db_idx >> 5) * 4;
				uint32_t client_off = client_idx * 0x20;
				
				uint32_t& bitmap_val = getMember<uint32_t>(that, 0xdc + word_off + client_off);
				bitmap_val &= ~(1 << (local_db_idx & 0x1f));
				
				return 0x100;
			}
			
			releaseDoorbell(that, db_entry_val);

			uint16_t db_per_client = getMember<uint16_t>(that, 0x9e0);
			uint16_t local_db_idx = db_id % db_per_client;
			uint32_t client_idx = db_id / db_per_client;
			
			uint32_t word_off = (local_db_idx >> 5) * 4;
			uint32_t client_off = client_idx * 0x20;
			
			uint32_t& bitmap_val = getMember<uint32_t>(that, 0xdc + word_off + client_off);
			bitmap_val |= (1 << (local_db_idx & 0x1f));
			
			if (db_id == 0x100) return 0x100;
		}

		setDoorbellPinning(that, db_id, param_2);
		getMember<uint32_t>(param_1, 0x24) = db_id;
		
		ctx_desc[0] = 1;
		ctx_desc[1] = 0;
		
		intel_de_write(display, 0xcee8, 1);
		while ((intel_de_read(display, 0xcee8) & 1) != 0) {
			// Spinwait
		}
		
		// acel->capabilities & 0x20)
		// intel_de_read(display, 0x2030);
		
		uint32_t payload[2] = { 0x10, context_id };
		uint32_t response = 0;
		
		hostToGuCAction(that, payload, 2, 0xf, &response);
		
		if (!host2) {
			ctx_desc[0] = 0;
		}
		
		if ((response >> 22 & 1) == 0) {
			ctx_desc[0] = 0;
		} else {
			getMember<void*>(that, 0x1e0 + static_cast<uint64_t>(db_id) * 8) = param_1;
			
			uint32_t wq_offset = (response >> 10) & 0xFC0;
			
			uint64_t& ctx_ptr_val = *reinterpret_cast<uint64_t*>(reinterpret_cast<uintptr_t>(array_base) + 0x10 + ctx_offset);
																 
			ctx_ptr_val += wq_offset;
			uint64_t new_ctx_ptr = ctx_ptr_val;
			
			getMember<uint32_t>(param_1, 0x18) += wq_offset;
			getMember<uint64_t>(param_1, 0x1c) += wq_offset;
			
			uint32_t* desc_ptr = *reinterpret_cast<uint32_t**>(array_base + 0x18 + ctx_offset);
			
			desc_ptr[0] = context_id;
			*reinterpret_cast<uint64_t*>(&desc_ptr[1]) = new_ctx_ptr;
			desc_ptr[10] = getMember<uint32_t>(param_1, 0x5a70);
		}
		return db_id;
	}
	
	return getMember<uint16_t>(param_1, 0x24);
}

void Gen11::releaseDoorbell(void *that, void *param_1)
{
	
	uint64_t array_base_addr = getMember<uint64_t>(that, 0x50);
	uintptr_t array_base = static_cast<uintptr_t>(array_base_addr);
	
	uint32_t db_id = getMember<uint32_t>(param_1, 0x24);
	uint32_t context_id = getMember<uint32_t>(param_1, 0x8);
	uint16_t num_doorbells = getMember<uint16_t>(that, 0x9e0);
	
	uint64_t ctx_offset = static_cast<uint64_t>(context_id) * 0x20;
	uint32_t* desc_ptr = *reinterpret_cast<uint32_t**>(array_base + 0x10 + ctx_offset);
	*desc_ptr = 0;
	
	uint32_t db_index = (db_id & 0xFFFF) % num_doorbells;
	uint32_t db_instance = ((db_id & 0xFF) / num_doorbells) & 7;
	uint32_t instance_select = db_instance << 24;
	
	#define GEN11_DOORBELL_INSTANCE_SELECT 0xfd4
	volatile uint32_t *mmio_base=NBlue::callback->rmmioPtr;
	uint32_t db_reg_offset = GEN8_DRBREGL(db_index);
	uint32_t db_reg_hi_offset = GEN8_DRBREGU(db_index);
	volatile uint32_t* fd4_reg = reinterpret_cast<volatile uint32_t*>(mmio_base + GEN11_DOORBELL_INSTANCE_SELECT);
	volatile uint32_t* db_reg = reinterpret_cast<volatile uint32_t*>(mmio_base + db_reg_offset);
	volatile uint32_t* db_reg_hi = reinterpret_cast<volatile uint32_t*>(mmio_base + db_reg_hi_offset);
		
	uint32_t saved_select = *fd4_reg;
		
	*fd4_reg = instance_select;
	uint32_t db_val = *db_reg;
	*fd4_reg = saved_select;
		
	*db_reg = db_val & 0xFFFFFFFE;
	saved_select = *fd4_reg;
		
	*fd4_reg = instance_select;
	*db_reg_hi = 0;
	*fd4_reg = saved_select;
		
	saved_select = *fd4_reg;
	*fd4_reg = instance_select;
	*db_reg = 0;
	*fd4_reg = saved_select;
	
	uint32_t payload[2] = { 0x20, context_id };
	hostToGuCAction(that, payload, 2, 0xF, nullptr);
	
	releaseDoorbellId(that, static_cast<uint16_t>(db_id));
	
	getMember<uint64_t>(that, 0x1E0 + (db_id & 0xFFFF) * 8) = 0;
	
	getMember<uint32_t>(param_1, 0x24) = 0x100; // GUC_INVALID_DOORBELL_ID
}




static void whitelist_reg_ext(struct i915_wa_list *wal, u32 reg, u32 flags)
{
	struct i915_wa wa = {
		.reg = reg
	};
	wa.reg |= flags;
	_wa_add(wal, &wa);
}

static void whitelist_reg(struct i915_wa_list *wal, u32 reg)
{
	whitelist_reg_ext(wal, reg, RING_FORCE_TO_NONPRIV_ACCESS_RW);
}


static void gen12_ctx_workarounds_init(struct intel_engine_cs *engine,
									   struct i915_wa_list *wal)
{

	wa_masked_en(wal, GEN11_COMMON_SLICE_CHICKEN3,
				 GEN12_DISABLE_CPS_AWARE_COLOR_PIPE);

	wa_masked_field_set(wal, GEN8_CS_CHICKEN1,
						GEN9_PREEMPT_GPGPU_LEVEL_MASK,
						GEN9_PREEMPT_GPGPU_THREAD_GROUP_LEVEL);


	wa_add(wal,
		   GEN12_FF_MODE2,
		   ~0,
		   FF_MODE2_TDS_TIMER_128 | FF_MODE2_GS_TIMER_224,
		   0, false);

	//if (!IS_DG1(i915)) {
		wa_masked_en(wal, HIZ_CHICKEN, HZ_DEPTH_TEST_LE_GE_OPT_DISABLE);

		wa_masked_en(wal, COMMON_SLICE_CHICKEN4, DISABLE_TDC_LOAD_BALANCING_CALC);
	//}

	wa_mcr_write_or(wal, GEN8_WM_CHICKEN2, WAIT_ON_DEPTH_STALL_DONE_DISABLE);
}

static void allow_read_ctx_timestamp(struct intel_engine_cs *engine)
{
	struct i915_wa_list *w = &engine->whitelist;

	if (engine->classb != RENDER_CLASS)
		whitelist_reg_ext(w,
						  RING_CTX_TIMESTAMP(engine->mmio_base),
						  RING_FORCE_TO_NONPRIV_ACCESS_RD);
}

static void tgl_whitelist_build(struct intel_engine_cs *engine)
{
	struct i915_wa_list *w = &engine->whitelist;

	allow_read_ctx_timestamp(engine);

	switch (engine->classb) {
	case RENDER_CLASS:

		whitelist_reg_ext(w, PS_INVOCATION_COUNT,
						  RING_FORCE_TO_NONPRIV_ACCESS_RD |
						  RING_FORCE_TO_NONPRIV_RANGE_4);


		whitelist_reg(w, GEN7_COMMON_SLICE_CHICKEN1);

		whitelist_reg(w, HIZ_CHICKEN);

		whitelist_reg(w, GEN11_COMMON_SLICE_CHICKEN3);

		break;
	default:
		break;
	}
}











static void
add_render_compute_tuning_settings(struct intel_gt *gt, struct i915_wa_list *wal)
{
	struct drm_i915_private *i915 = gt->i915;

	/*if (IS_GFX_GT_IP_RANGE(gt, IP_VER(12, 70), IP_VER(12, 74)) || IS_DG2(i915))
		wa_mcr_write_clr_set(wal, RT_CTRL, STACKID_CTRL, STACKID_CTRL_512);


	if (INTEL_INFO(i915)->tuning_thread_rr_after_dep)
		wa_mcr_masked_field_set(wal, GEN9_ROW_CHICKEN4, THREAD_EX_ARB_MODE,
					THREAD_EX_ARB_MODE_RR_AFTER_DEP);
*/
	if (GRAPHICS_VER(i915) == 12 && GRAPHICS_VER_FULL(i915) < IP_VER(12, 55))
		wa_write_clr(wal, GEN8_GARBCNTL, GEN12_BUS_HASH_CTL_BIT_EXC);
}
static void
general_render_compute_wa_init(struct intel_engine_cs *engine, struct i915_wa_list *wal)
{
	struct drm_i915_private *i915 = engine->i915;
	struct intel_gt *gt = engine->gt;

	add_render_compute_tuning_settings( gt, wal);



	if (GRAPHICS_VER(i915) >= 11) {
		wa_mcr_masked_en(wal,
				 GEN10_SAMPLER_MODE,
				 GEN11_INDIRECT_STATE_BASE_ADDR_OVERRIDE);
	}

	/*if (IS_GFX_GT_IP_STEP(gt, IP_VER(12, 70), STEP_B0, STEP_FOREVER) ||
		IS_GFX_GT_IP_STEP(gt, IP_VER(12, 71), STEP_B0, STEP_FOREVER) ||
		IS_GFX_GT_IP_RANGE(gt, IP_VER(12, 74), IP_VER(12, 74))) {

		wa_mcr_masked_en(wal, GEN9_ROW_CHICKEN3, MTL_DISABLE_FIX_FOR_EOT_FLUSH);


		wa_mcr_masked_en(wal, GEN8_ROW_CHICKEN2, XELPG_DISABLE_TDL_SVHS_GATING);
	}

	if (IS_GFX_GT_IP_STEP(gt, IP_VER(12, 70), STEP_A0, STEP_B0) ||
		IS_GFX_GT_IP_STEP(gt, IP_VER(12, 71), STEP_A0, STEP_B0))

		wa_mcr_masked_en(wal, GEN10_SAMPLER_MODE,
				 MTL_DISABLE_SAMPLER_SC_OOO);

	if (IS_GFX_GT_IP_STEP(gt, IP_VER(12, 71), STEP_A0, STEP_B0))

		wa_mcr_masked_en(wal, GEN10_CACHE_MODE_SS,
				 DISABLE_PREFETCH_INTO_IC);

	if (IS_GFX_GT_IP_STEP(gt, IP_VER(12, 70), STEP_A0, STEP_B0) ||
		IS_GFX_GT_IP_STEP(gt, IP_VER(12, 71), STEP_A0, STEP_B0) ||
		IS_DG2(i915)) {

		wa_mcr_write_or(wal, LSC_CHICKEN_BIT_0_UDW,
				DISABLE_128B_EVICTION_COMMAND_UDW);


		wa_masked_en(wal, VFG_PREEMPTION_CHICKEN, POLYGON_TRIFAN_LINELOOP_DISABLE);
	}

	if (IS_GFX_GT_IP_STEP(gt, IP_VER(12, 70), STEP_A0, STEP_B0) ||
		IS_GFX_GT_IP_STEP(gt, IP_VER(12, 71), STEP_A0, STEP_B0) ||
		IS_DG2(i915)) {

		wa_mcr_write_or(wal, LSC_CHICKEN_BIT_0, DISABLE_D8_D16_COASLESCE);
	}

	if (IS_DG2(i915)) {
		wa_mcr_masked_en(wal, GEN9_ROW_CHICKEN4, XEHP_DIS_BBL_SYSPIPE);

		wa_mcr_write_or(wal, LSC_CHICKEN_BIT_0_UDW, DIS_CHAIN_2XSIMD8);

		wa_mcr_write_or(wal, LSC_CHICKEN_BIT_0_UDW, UGM_FRAGMENT_THRESHOLD_TO_3);
	}

	if (IS_DG2_G11(i915)) {

		wa_mcr_write_clr_set(wal, LSC_CHICKEN_BIT_0_UDW,
					 MAXREQS_PER_BANK,
					 REG_FIELD_PREP(MAXREQS_PER_BANK, 2));

		wa_mcr_write_or(wal, LSC_CHICKEN_BIT_0,
				FORCE_1_SUB_MESSAGE_PER_FRAGMENT);

		wa_mcr_add(wal, GEN10_CACHE_MODE_SS, 0,
			   REG_MASKED_FIELD_ENABLE(ENABLE_PREFETCH_INTO_IC),
			   0 ,
			   true);
	}*/
}


static void
rcs_engine_wa_init(struct intel_engine_cs *engine, struct i915_wa_list *wal)
{
	struct drm_i915_private *i915 = engine->i915;
	struct intel_gt *gt = engine->gt;

	/*if (IS_GFX_GT_IP_STEP(gt, IP_VER(12, 70), STEP_A0, STEP_B0) ||
		IS_GFX_GT_IP_STEP(gt, IP_VER(12, 71), STEP_A0, STEP_B0)) {

		wa_mcr_masked_en(wal, GEN10_CACHE_MODE_SS,
				 ENABLE_EU_COUNT_FOR_TDL_FLUSH);
	}

	if (IS_GFX_GT_IP_STEP(gt, IP_VER(12, 70), STEP_A0, STEP_B0) ||
		IS_GFX_GT_IP_STEP(gt, IP_VER(12, 71), STEP_A0, STEP_B0) ||
		IS_DG2(i915)) {

		wa_mcr_masked_en(wal, GEN10_SAMPLER_MODE,
				 SC_DISABLE_POWER_OPTIMIZATION_EBB);
	}

	if (IS_GFX_GT_IP_STEP(gt, IP_VER(12, 70), STEP_A0, STEP_B0) ||
		IS_DG2(i915)) {

		wa_mcr_masked_en(wal, GEN8_ROW_CHICKEN2,
				 GEN12_DISABLE_READ_SUPPRESSION);
	}

	if (IS_DG2(i915)) {

		wa_mcr_masked_dis(wal, XEHP_HDC_CHICKEN0,
				  LSC_L1_FLUSH_CTL_3D_DATAPORT_FLUSH_EVENTS_MASK);
	}

	if (IS_GFX_GT_IP_RANGE(gt, IP_VER(12, 70), IP_VER(12, 71)) ||
		IS_DG2(i915)) {

		wa_mcr_add(wal, XEHP_HDC_CHICKEN0, 0,
			   REG_MASKED_FIELD_ENABLE(DIS_ATOMIC_CHAINING_TYPED_WRITES),
			   0, true);
	}*/

	

	
	if (IS_DG2(i915) || IS_ALDERLAKE_P(i915) || IS_ALDERLAKE_S(i915) ||
		IS_DG1(i915) || IS_ROCKETLAKE(i915) || IS_TIGERLAKE(i915)) {

		wa_masked_en(wal,
				 GEN9_CS_DEBUG_MODE1,
				 FF_DOP_CLOCK_GATE_DISABLE);
	}

	if (IS_ALDERLAKE_P(i915) || IS_ALDERLAKE_S(i915) || IS_DG1(i915) ||
		IS_ROCKETLAKE(i915) || IS_TIGERLAKE(i915)) {

		wa_mcr_masked_en(wal, GEN8_ROW_CHICKEN2, GEN12_DISABLE_EARLY_READ);


		wa_write_or(wal, GEN7_FF_THREAD_MODE,
				GEN12_FF_TESSELATION_DOP_GATE_DISABLE);


		wa_mcr_masked_en(wal,
				 GEN10_SAMPLER_MODE,
				 ENABLE_SMALLPL);
	}

	if (IS_ALDERLAKE_P(i915) || IS_ALDERLAKE_S(i915) ||
		IS_ROCKETLAKE(i915) || IS_TIGERLAKE(i915)) {

		wa_mcr_masked_en(wal, GEN8_ROW_CHICKEN2,
				 GEN12_PUSH_CONST_DEREF_HOLD_DIS);


		wa_mcr_masked_en(wal, GEN9_ROW_CHICKEN4, GEN12_DISABLE_TDL_PUSH);
	}

	if (IS_ROCKETLAKE(i915) || IS_TIGERLAKE(i915) || IS_ALDERLAKE_P(i915)) {

		wa_masked_en(wal,
				 RING_PSMI_CTL(RENDER_RING_BASE),
				 GEN12_WAIT_FOR_EVENT_POWER_DOWN_DISABLE |
				 GEN8_RC_SEMA_IDLE_MSG_DISABLE);
	}


	
	if (GRAPHICS_VER(i915) >= 9)
		wa_masked_en(wal,
				 GEN7_FF_SLICE_CS_CHICKEN1,
				 GEN9_FFSC_PERCTX_PREEMPT_CTRL);



}

static void ccs_engine_wa_mode(struct intel_engine_cs *engine, struct i915_wa_list *wal)
{
	struct intel_gt *gt = engine->gt;
	u32 mode;

	if (!IS_DG2(gt->i915))
		return;


//	wa_masked_en(wal, GEN12_RCU_MODE, XEHP_RCU_MODE_FIXED_SLICE_CCS_MODE);


	//mode = intel_gt_apply_ccs_mode(gt);
	//wa_masked_en(wal, XEHP_CCS_MODE, mode);
}


static void
engine_fake_wa_init(struct intel_engine_cs *engine, struct i915_wa_list *wal)
{
	u8 mocs_w, mocs_r;


	if (GRAPHICS_VER(engine->i915) >= 12) {
		mocs_r = engine->gt->mocs.uc_index;
		mocs_w = engine->gt->mocs.uc_index;

		if (HAS_L3_CCS_READ(engine->i915) &&
			engine->classb == COMPUTE_CLASS) {
			mocs_r = engine->gt->mocs.wb_index;

		}

		
		wa_masked_field_set(wal,
					RING_CMD_CCTL(engine->mmio_base),
					CMD_CCTL_MOCS_MASK,
					CMD_CCTL_MOCS_OVERRIDE(mocs_w, mocs_r));
	}
}

static void
engine_init_workarounds(struct intel_engine_cs *engine, struct i915_wa_list *wal)
{

	if (GRAPHICS_VER(engine->i915) < 4)
		return;

	engine_fake_wa_init(engine, wal);


	if (engine->flags & I915_ENGINE_FIRST_RENDER_COMPUTE) {
		general_render_compute_wa_init(engine, wal);
		ccs_engine_wa_mode(engine, wal);
	}

	/*if (engine->classb == COMPUTE_CLASS)
		ccs_engine_wa_init(engine, wal);
	else*/ if (engine->classb == RENDER_CLASS)
		rcs_engine_wa_init(engine, wal);
	//else
		//xcs_engine_wa_init(engine, wal);
	
}

void intel_engine_init_ctx_wa(struct intel_engine_cs *engine)
{
	struct i915_wa_list *wal = &engine->ctx_wa_list;

	wa_init_start(wal, engine->gt, "context", engine->name);
	gen12_ctx_workarounds_init(engine, wal);
	wa_init_finish(wal);
}

static void engine_init_whitelist(struct intel_engine_cs *engine, struct i915_wa_list *wal)
{
	tgl_whitelist_build(engine);
}

void intel_engine_init_workarounds(struct intel_engine_cs *engine)
{
	struct i915_wa_list *wal = &engine->wa_list;

	wa_init_start(wal, engine->gt, "engine", engine->name);
	engine_init_workarounds(engine, wal);
	wa_init_finish(wal);
}



void intel_engine_init_whitelist(struct intel_engine_cs *engine)
{
	struct i915_wa_list *wal = &engine->whitelist;

	wa_init_start(wal, engine->gt, "whitelist", engine->name);
	engine_init_whitelist(engine, wal);
	wa_init_finish(wal);
}

void intel_engine_apply_workarounds(struct intel_engine_cs *engine)
{
	wa_list_apply(&engine->wa_list);
}

void intel_engine_apply_whitelist(struct intel_engine_cs *engine)
{
	struct drm_i915_private *i915 = engine->gt->i915;
	struct intel_display *display=i915->display;
	
	struct i915_wa_list *wal = &engine->whitelist;
	const u32 base = engine->mmio_base;
	struct i915_wa *wa;
	unsigned int i;


	if (!wal->count)
		return;

	for (i = 0, wa = wal->list; i < wal->count; i++, wa++)
		intel_de_write(display,
				   RING_FORCE_TO_NONPRIV(base, i),
				   i915_mmio_reg_offset(wa->reg));

	for (; i < RING_MAX_NONPRIV_SLOTS; i++)
		intel_de_write(display,
				   RING_FORCE_TO_NONPRIV(base, i),
				   i915_mmio_reg_offset(RING_NOPID(base)));
}



static
bool gen11_vdbox_has_sfc(struct intel_gt *gt,
			 unsigned int physical_vdbox,
			 unsigned int logical_vdbox, u16 vdbox_mask)
{
	struct drm_i915_private *i915 = gt->i915;

	if ((gt->info.sfc_mask & BIT(physical_vdbox / 2)) == 0)
		return false;
	else if (MEDIA_VER(i915) >= 12)
		return (physical_vdbox % 2 == 0) ||
			!(BIT(physical_vdbox - 1) & vdbox_mask);
	else if (MEDIA_VER(i915) == 11)
		return logical_vdbox % 2 == 0;

	return false;
}

static void engine_mask_apply_media_fuses(struct intel_gt *gt)
{
	struct drm_i915_private *i915 = gt->i915;
	struct intel_display *display=i915->display;
	unsigned int logical_vdbox = 0;
	unsigned int i;
	u32 media_fuse, fuse1;
	u16 vdbox_mask;
	u16 vebox_mask;

	if (MEDIA_VER(gt->i915) < 11)
		return;


	media_fuse = intel_de_read(display, GEN11_GT_VEBOX_VDBOX_DISABLE);
	if (MEDIA_VER_FULL(i915) < IP_VER(12, 55))
		media_fuse = ~media_fuse;

	vdbox_mask = REG_FIELD_GET(GEN11_GT_VDBOX_DISABLE_MASK, media_fuse);
	vebox_mask = REG_FIELD_GET(GEN11_GT_VEBOX_DISABLE_MASK, media_fuse);

	if (MEDIA_VER_FULL(i915) >= IP_VER(12, 55)) {
		fuse1 =intel_de_read(display, HSW_PAVP_FUSE1);
		gt->info.sfc_mask = REG_FIELD_GET(XEHP_SFC_ENABLE_MASK, fuse1);
	} else {
		gt->info.sfc_mask = ~0;
	}

	for (i = 0; i < I915_MAX_VCS; i++) {
		if (!HAS_ENGINE(gt, _VCS(i))) {
			vdbox_mask &= ~BIT(i);
			continue;
		}

		if (!(BIT(i) & vdbox_mask)) {
			gt->info.engine_mask &= ~BIT(_VCS(i));
			continue;
		}

		if (gen11_vdbox_has_sfc(gt, i, logical_vdbox, vdbox_mask))
			gt->info.vdbox_sfc_access |= BIT(i);
		logical_vdbox++;
	}

	//GEM_BUG_ON(vdbox_mask != VDBOX_MASK(gt));

	for (i = 0; i < I915_MAX_VECS; i++) {
		if (!HAS_ENGINE(gt, _VECS(i))) {
			vebox_mask &= ~BIT(i);
			continue;
		}

		if (!(BIT(i) & vebox_mask)) {
			gt->info.engine_mask &= ~BIT(_VECS(i));
		}
	}

	//GEM_BUG_ON(vebox_mask != VEBOX_MASK(gt));
}



uint16_t intel_slicemask_from_xehp_dssmask(intel_sseu_ss_mask_t dss_mask, int dss_per_slice)
{
	uint16_t slice_mask = 0;
	int i = 0;
	uint64_t bits = (uint64_t)dss_mask.xehp;
	uint64_t per_slice_mask = (dss_per_slice >= 64) ? ~0ULL : ((1ULL << dss_per_slice) - 1);

	while (bits != 0) {
		if ((bits & per_slice_mask) != 0) {
			slice_mask |= static_cast<uint16_t>(1 << i);
		}
		bits >>= dss_per_slice;
		i++;
	}

	//assert(i <= 16 * 8);

	return slice_mask;
}
static void engine_mask_apply_compute_fuses(struct intel_gt *gt)
{
	struct drm_i915_private *i915 = gt->i915;
	struct intel_gt_info *info = &gt->info;
	int ss_per_ccs = info->sseu.max_subslices / I915_MAX_CCS;
	unsigned long ccs_mask0;
	unsigned int i;

	if (GRAPHICS_VER(i915) < 11)
		return;

	if (hweight32(CCS_MASK(gt)) <= 1)
		return;

	ccs_mask0 = intel_slicemask_from_xehp_dssmask(info->sseu.compute_subslice_mask,
							 ss_per_ccs);

	for_each_clear_bit(i, &ccs_mask0, I915_MAX_CCS) {
		info->engine_mask &= ~BIT(_CCS(i));
	}
}

static intel_engine_mask_t init_engine_mask(struct intel_gt *gt)
{
	struct intel_gt_info *info = &gt->info;

	//GEM_BUG_ON(!info->engine_mask);

	engine_mask_apply_media_fuses(gt);
	engine_mask_apply_compute_fuses(gt);


	if (__HAS_ENGINE(info->engine_mask, GSC0) /*&& !intel_uc_wants_gsc_uc(&gt->uc)*/) {
		info->engine_mask &= ~BIT(GSC0);
	}

	/*if (IS_DG2(gt->i915)) {
		u8 first_ccs = __ffs(CCS_MASK(gt));

		gt->ccs.cslices = CCS_MASK(gt);

		info->engine_mask &= ~GENMASK(CCS3, CCS0);
		info->engine_mask |= BIT(_CCS(first_ccs));
	}*/

	return info->engine_mask;
}


static void populate_logical_ids(struct intel_gt *gt, u8 *logical_ids,
				 u8 classb, const u8 *map, u8 num_instances)
{
	int i, j;
	u8 current_logical_id = 0;

	for (j = 0; j < num_instances; ++j) {
		for (i = 0; i < ARRAY_SIZE(intel_engines); ++i) {
			if (!HAS_ENGINE(gt, i) ||
				intel_engines[i].classb != classb)
				continue;

			if (intel_engines[i].instance == map[j]) {
				logical_ids[intel_engines[i].instance] =
					current_logical_id++;
				break;
			}
		}
	}
}

static void setup_logical_ids(struct intel_gt *gt, u8 *logical_ids, u8 classb)
{

	if (MEDIA_VER(gt->i915) >= 11 && classb == VIDEO_DECODE_CLASS) {
		const u8 map[] = { 0, 2, 4, 6, 1, 3, 5, 7 };

		populate_logical_ids(gt, logical_ids, classb,
					 map, ARRAY_SIZE(map));
	} else {
		int i;
		u8 map[MAX_ENGINE_INSTANCE + 1];

		for (i = 0; i < MAX_ENGINE_INSTANCE + 1; ++i)
			map[i] = i;
		populate_logical_ids(gt, logical_ids, classb,
					 map, ARRAY_SIZE(map));
	}
}

static u8 guc_class_engine_class_map[] = {
	[GUC_RENDER_CLASS]       = RENDER_CLASS,
	[GUC_BLITTER_CLASS]      = COPY_ENGINE_CLASS,
	[GUC_VIDEO_CLASS]        = VIDEO_DECODE_CLASS,
	[GUC_VIDEOENHANCE_CLASS] = VIDEO_ENHANCEMENT_CLASS,
	[GUC_COMPUTE_CLASS]      = COMPUTE_CLASS,
	[GUC_GSC_OTHER_CLASS]    = OTHER_CLASS,
};
static u8 engine_class_guc_class_map[] = {
	[RENDER_CLASS]            = GUC_RENDER_CLASS,
	[COPY_ENGINE_CLASS]       = GUC_BLITTER_CLASS,
	[VIDEO_DECODE_CLASS]      = GUC_VIDEO_CLASS,
	[VIDEO_ENHANCEMENT_CLASS] = GUC_VIDEOENHANCE_CLASS,
	[OTHER_CLASS]             = GUC_GSC_OTHER_CLASS,
	[COMPUTE_CLASS]           = GUC_COMPUTE_CLASS,
};

static inline u8 engine_class_to_guc_class(u8 classb)
{
	//BUILD_BUG_ON(ARRAY_SIZE(engine_class_guc_class_map) != MAX_ENGINE_CLASS + 1);
	//GEM_BUG_ON(class > MAX_ENGINE_CLASS);

	return engine_class_guc_class_map[classb];
}

static u32 __engine_mmio_base(struct drm_i915_private *i915,
				  const struct engine_mmio_base *bases)
{
	int i;

	for (i = 0; i < MAX_MMIO_BASES; i++)
		if (GRAPHICS_VER(i915) >= bases[i].graphics_ver)
			break;

	//GEM_BUG_ON(i == MAX_MMIO_BASES);
	//GEM_BUG_ON(!bases[i].base);

	return bases[i].base;
}

const char *intel_engine_class_repr(u8 classb)
{
	static const char * uabi_names[] = {
		[RENDER_CLASS] = "rcs",
		[COPY_ENGINE_CLASS] = "bcs",
		[VIDEO_DECODE_CLASS] = "vcs",
		[VIDEO_ENHANCEMENT_CLASS] = "vecs",
		[OTHER_CLASS] = "other",
		[COMPUTE_CLASS] = "ccs",
	};

	if (classb >= ARRAY_SIZE(uabi_names) || !uabi_names[classb])
		return "xxx";

	return uabi_names[classb];
}

static void __sprint_engine_name(struct intel_engine_cs *engine)
{

	(snprintf(engine->name, sizeof(engine->name), "%s'%u",
				 intel_engine_class_repr(engine->classb),
				 engine->instance) >= sizeof(engine->name));
}




static u32 get_reset_domain(u8 ver, enum intel_engine_id id)
{
	u32 reset_domain;

	if (ver >= 11) {
		static const u32 engine_reset_domains[] = {
			[RCS0]  = GEN11_GRDOM_RENDER,
			[BCS0]  = GEN11_GRDOM_BLT,
			[BCS1]  = XEHPC_GRDOM_BLT1,
			[BCS2]  = XEHPC_GRDOM_BLT2,
			[BCS3]  = XEHPC_GRDOM_BLT3,
			[BCS4]  = XEHPC_GRDOM_BLT4,
			[BCS5]  = XEHPC_GRDOM_BLT5,
			[BCS6]  = XEHPC_GRDOM_BLT6,
			[BCS7]  = XEHPC_GRDOM_BLT7,
			[BCS8]  = XEHPC_GRDOM_BLT8,
			[VCS0]  = GEN11_GRDOM_MEDIA,
			[VCS1]  = GEN11_GRDOM_MEDIA2,
			[VCS2]  = GEN11_GRDOM_MEDIA3,
			[VCS3]  = GEN11_GRDOM_MEDIA4,
			[VCS4]  = GEN11_GRDOM_MEDIA5,
			[VCS5]  = GEN11_GRDOM_MEDIA6,
			[VCS6]  = GEN11_GRDOM_MEDIA7,
			[VCS7]  = GEN11_GRDOM_MEDIA8,
			[VECS0] = GEN11_GRDOM_VECS,
			[VECS1] = GEN11_GRDOM_VECS2,
			[VECS2] = GEN11_GRDOM_VECS3,
			[VECS3] = GEN11_GRDOM_VECS4,
			[CCS0]  = GEN11_GRDOM_RENDER,
			[CCS1]  = GEN11_GRDOM_RENDER,
			[CCS2]  = GEN11_GRDOM_RENDER,
			[CCS3]  = GEN11_GRDOM_RENDER,
			[GSC0]  = GEN12_GRDOM_GSC,
		};
		//GEM_BUG_ON(id >= ARRAY_SIZE(engine_reset_domains) ||
			 //  !engine_reset_domains[id]);
		reset_domain = engine_reset_domains[id];
	} else {
		static const u32 engine_reset_domains[] = {
			[RCS0]  = GEN6_GRDOM_RENDER,
			[BCS0]  = GEN6_GRDOM_BLT,
			[VCS0]  = GEN6_GRDOM_MEDIA,
			[VCS1]  = GEN8_GRDOM_MEDIA2,
			[VECS0] = GEN6_GRDOM_VECS,
		};
		//GEM_BUG_ON(id >= ARRAY_SIZE(engine_reset_domains) ||
			//   !engine_reset_domains[id]);
		reset_domain = engine_reset_domains[id];
	}

	return reset_domain;
}

static u32 intel_engine_context_size(struct intel_gt *gt, u8 classb)
{
	u32 cxt_size;
	struct drm_i915_private *i915 = gt->i915;
	struct intel_display *display =i915->display;
	
	//BUILD_BUG_ON(I915_GTT_PAGE_SIZE != PAGE_SIZE);
	
	switch (classb) {
		case COMPUTE_CLASS:
		case RENDER_CLASS:
			switch (GRAPHICS_VER(gt->i915)) {
				default:
					return DEFAULT_LR_CONTEXT_RENDER_SIZE;
				case 12:
				case 11:
					return GEN11_LR_CONTEXT_RENDER_SIZE;
				case 9:
					return GEN9_LR_CONTEXT_RENDER_SIZE;
				case 8:
					return GEN8_LR_CONTEXT_RENDER_SIZE;
				case 7:
					if (IS_HASWELL(gt->i915))
						return HSW_CXT_TOTAL_SIZE;
					
					cxt_size = intel_de_read(display, GEN7_CXT_SIZE);
					return round_up(GEN7_CXT_TOTAL_SIZE(cxt_size) * 64,
									PAGE_SIZE);
				case 6:
					cxt_size = intel_de_read(display, CXT_SIZE);
					return round_up(GEN6_CXT_TOTAL_SIZE(cxt_size) * 64,
									PAGE_SIZE);
				case 5:
				case 4:
					cxt_size = intel_de_read(display, CXT_SIZE) + 1;
					return round_up(cxt_size * 64, PAGE_SIZE);
				case 3:
				case 2:

				case 1:
					return 0;
			}
			break;
			
		default:
		case VIDEO_DECODE_CLASS:
		case VIDEO_ENHANCEMENT_CLASS:
		case COPY_ENGINE_CLASS:
		case OTHER_CLASS:
			if (GRAPHICS_VER(gt->i915) < 8)
				return 0;
			return GEN8_LR_CONTEXT_OTHER_SIZE;
	}
	
}

void intel_engine_set_hwsp_writemask(struct intel_engine_cs *engine, u32 mask)
{

	if (GRAPHICS_VER(engine->i915) < 6 && engine->classb != RENDER_CLASS)
		return;

	struct drm_i915_private *i915 = engine->gt->i915;
	struct intel_display *display =i915->display;
	
	intel_de_write(display,RING_HWSTAM(engine->mmio_base), mask);
	
	//if (GRAPHICS_VER(engine->i915) >= 3)
		//ENGINE_WRITE(engine, RING_HWSTAM, mask);
	//else
		//ENGINE_WRITE16(engine, RING_HWSTAM, mask);
}
static void intel_engine_sanitize_mmio(struct intel_engine_cs *engine)
{
	intel_engine_set_hwsp_writemask(engine, ~0u);
}

static int intel_engine_setup(struct intel_gt *gt, enum intel_engine_id id2,
				  u8 logical_instance)
{
	const struct engine_info *info = &intel_engines[id2];
	struct drm_i915_private *i915 = gt->i915;
	struct intel_engine_cs *engine;
	u8 guc_class;

/*	BUILD_BUG_ON(MAX_ENGINE_CLASS >= BIT(GEN11_ENGINE_CLASS_WIDTH));
	BUILD_BUG_ON(MAX_ENGINE_INSTANCE >= BIT(GEN11_ENGINE_INSTANCE_WIDTH));
	BUILD_BUG_ON(I915_MAX_VCS > (MAX_ENGINE_INSTANCE + 1));
	BUILD_BUG_ON(I915_MAX_VECS > (MAX_ENGINE_INSTANCE + 1));
*/
	if ((id2 >= ARRAY_SIZE(gt->engine)))
		return -EINVAL;

	if ((info->classb > MAX_ENGINE_CLASS))
		return -EINVAL;

	if ((info->instance > MAX_ENGINE_INSTANCE))
		return -EINVAL;

	if ((gt->engine_class[info->classb][info->instance]))
		return -EINVAL;

	engine =  (struct intel_engine_cs *)IOMalloc(sizeof(*engine));
	if (!engine)
		return -ENOMEM;

	//BUILD_BUG_ON(BITS_PER_TYPE(engine->mask) < I915_NUM_ENGINES);

	//INIT_LIST_HEAD(&engine->pinned_contexts_list);
	engine->id2 = id2;
	engine->legacy_idx = INVALID_ENGINE;
	engine->mask = BIT(id2);
	engine->reset_domain = get_reset_domain(GRAPHICS_VER(gt->i915), id2);
	engine->i915 = i915;
	engine->gt = gt;

	guc_class = engine_class_to_guc_class(info->classb);
	engine->guc_id = MAKE_GUC_ID(guc_class, info->instance);
	engine->mmio_base = __engine_mmio_base(i915, info->mmio_bases);

	//engine->irq_handler = nop_irq_handler;

	engine->classb = info->classb;
	engine->instance = info->instance;
	engine->logical_mask = BIT(logical_instance);
	__sprint_engine_name(engine);


	if ((engine->classb == COMPUTE_CLASS || engine->classb == RENDER_CLASS) &&
		__ffs(CCS_MASK(engine->gt) | RCS_MASK(engine->gt)) == engine->instance)
		engine->flags |= I915_ENGINE_FIRST_RENDER_COMPUTE;

	/* features common between engines sharing EUs */
	if (engine->classb == RENDER_CLASS || engine->classb == COMPUTE_CLASS) {
		engine->flags |= I915_ENGINE_HAS_RCS_REG_STATE;
		engine->flags |= I915_ENGINE_HAS_EU_PRIORITY;
	}

	engine->props.heartbeat_interval_ms =
		CONFIG_DRM_I915_HEARTBEAT_INTERVAL;
	engine->props.max_busywait_duration_ns =
		CONFIG_DRM_I915_MAX_REQUEST_BUSYWAIT;
	engine->props.preempt_timeout_ms =
		CONFIG_DRM_I915_PREEMPT_TIMEOUT;
	engine->props.stop_timeout_ms =
		CONFIG_DRM_I915_STOP_TIMEOUT;
	engine->props.timeslice_duration_ms =
		CONFIG_DRM_I915_TIMESLICE_DURATION;


	if (GRAPHICS_VER(i915) == 12 && (engine->flags & I915_ENGINE_HAS_RCS_REG_STATE))
		engine->props.preempt_timeout_ms = CONFIG_DRM_I915_PREEMPT_TIMEOUT_COMPUTE;

/*
#define CLAMP_PROP(field) \
	do { \
		u64 clamp = intel_clamp_##field(engine, engine->props.field); \
		if (clamp != engine->props.field) { \
			drm_notice(&engine->i915->drm, \
				   "Warning, clamping %s to %lld to prevent overflow\n", \
				   #field, clamp); \
			engine->props.field = clamp; \
		} \
	} while (0)

	CLAMP_PROP(heartbeat_interval_ms);
	CLAMP_PROP(max_busywait_duration_ns);
	CLAMP_PROP(preempt_timeout_ms);
	CLAMP_PROP(stop_timeout_ms);
	CLAMP_PROP(timeslice_duration_ms);

#undef CLAMP_PROP
*/
	engine->defaults = engine->props;

	engine->context_size = intel_engine_context_size(gt, engine->classb);
	if ((engine->context_size > BIT(20)))
		engine->context_size = 0;
	if (engine->context_size)
		DRIVER_CAPS(i915)->has_logical_contexts = true;

	//ewma__engine_latency_init(&engine->latency);

	//ATOMIC_INIT_NOTIFIER_HEAD(&engine->context_status_notifier);

	intel_engine_sanitize_mmio(engine);

	gt->engine_class[info->classb][info->instance] = engine;
	gt->engine[id2] = engine;

	return 0;
}

static void __setup_engine_capabilities(struct intel_engine_cs *engine)
{
	struct drm_i915_private *i915 = engine->i915;

	
	if (engine->classb == VIDEO_DECODE_CLASS) {

		if (GRAPHICS_VER(i915) >= 11 ||
			(GRAPHICS_VER(i915) >= 9 && engine->instance == 0))
			engine->uabi_capabilities |=
				I915_VIDEO_CLASS_CAPABILITY_HEVC;


		if ((GRAPHICS_VER(i915) >= 11 &&
			 (engine->gt->info.vdbox_sfc_access &
			  BIT(engine->instance))) ||
			(GRAPHICS_VER(i915) >= 9 && engine->instance == 0))
			engine->uabi_capabilities |=
				I915_VIDEO_AND_ENHANCE_CLASS_CAPABILITY_SFC;
	} else if (engine->classb == VIDEO_ENHANCEMENT_CLASS) {
		if (GRAPHICS_VER(i915) >= 9 &&
			engine->gt->info.sfc_mask & BIT(engine->instance))
			engine->uabi_capabilities |=
				I915_VIDEO_AND_ENHANCE_CLASS_CAPABILITY_SFC;
	}
}

static void intel_setup_engine_capabilities(struct intel_gt *gt)
{
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	for_each_engine(engine, gt, id)
		__setup_engine_capabilities(engine);
}

int intel_engines_init_mmio(struct intel_gt *gt)
{
	struct drm_i915_private *i915 = gt->i915;
	const unsigned int engine_mask = init_engine_mask(gt);
	unsigned int mask = 0;
	unsigned int i, classb;
	u8 logical_ids[MAX_ENGINE_INSTANCE + 1];
	int err;

	/*drm_WARN_ON(&i915->drm, engine_mask == 0);
	drm_WARN_ON(&i915->drm, engine_mask &
			GENMASK(BITS_PER_TYPE(mask) - 1, I915_NUM_ENGINES));
*/
	for (classb = 0; classb < MAX_ENGINE_CLASS + 1; ++classb) {
		setup_logical_ids(gt, logical_ids, classb);

		for (i = 0; i < ARRAY_SIZE(intel_engines); ++i) {
			u8 instance = intel_engines[i].instance;

			if (intel_engines[i].classb != classb ||
				!HAS_ENGINE(gt, i))
				continue;

			err = intel_engine_setup(gt, static_cast<enum intel_engine_id >(i)  ,
						 logical_ids[instance]);
			if (err)
				goto cleanup;

			mask |= BIT(i);
		}
	}


	if (( mask != engine_mask))
		gt->info.engine_mask = mask;

	gt->info.num_engines = hweight32(mask);

	//intel_gt_check_and_clear_faults(gt);

	intel_setup_engine_capabilities(gt);

	//intel_uncore_prune_engine_fw_domains(gt->uncore, gt);

	return 0;

cleanup:
	//intel_engines_free(gt);
	return err;
}

static inline struct intel_sseu
intel_sseu_from_device_info(const struct sseu_dev_info *sseu)
{
	struct intel_sseu value = {
		.slice_mask = sseu->slice_mask,
		.subslice_mask = sseu->subslice_mask.hsw[0],
		.min_eus_per_subslice = sseu->max_eus_per_subslice,
		.max_eus_per_subslice = sseu->max_eus_per_subslice,
	};

	return value;
}

void intel_sseu_set_info(struct sseu_dev_info *sseu, u8 max_slices,
			 u8 max_subslices, u8 max_eus_per_subslice)
{
	sseu->max_slices = max_slices;
	sseu->max_subslices = max_subslices;
	sseu->max_eus_per_subslice = max_eus_per_subslice;
}



static inline bool
intel_sseu_has_subslice(const struct sseu_dev_info *sseu, int slice,
			int subslice)
{
	if (slice >= sseu->max_slices ||
		subslice >= sseu->max_subslices)
		return false;

	if (sseu->has_xehp_dss)
		return test_bit(subslice, sseu->subslice_mask.xehp);
	else
		return sseu->subslice_mask.hsw[slice] & BIT(subslice);
}

static void sseu_set_eus(struct sseu_dev_info *sseu, int slice, int subslice,
			 u16 eu_mask)
{
	//GEM_WARN_ON(eu_mask && __fls(eu_mask) >= sseu->max_eus_per_subslice);
	if (sseu->has_xehp_dss) {
		//GEM_WARN_ON(slice > 0);
		sseu->eu_mask.xehp[subslice] = eu_mask;
	} else {
		sseu->eu_mask.hsw[slice][subslice] = eu_mask;
	}
}

static u16 compute_eu_total(const struct sseu_dev_info *sseu)
{
	int s, ss, total = 0;

	for (s = 0; s < sseu->max_slices; s++)
		for (ss = 0; ss < sseu->max_subslices; ss++)
			if (sseu->has_xehp_dss)
				total += hweight16(sseu->eu_mask.xehp[ss]);
			else
				total +=hweight16(sseu->eu_mask.hsw[s][ss]);

	return total;
}

static void gen11_compute_sseu_info(struct sseu_dev_info *sseu,
					u32 ss_en, u16 eu_en)
{
	u32 valid_ss_mask = GENMASK(sseu->max_subslices - 1, 0);
	int ss;

	sseu->slice_mask |= BIT(0);
	sseu->subslice_mask.hsw[0] = ss_en & valid_ss_mask;

	for (ss = 0; ss < sseu->max_subslices; ss++)
		if (intel_sseu_has_subslice(sseu, 0, ss))
			sseu_set_eus(sseu, 0, ss, eu_en);

	sseu->eu_per_subslice = hweight16(eu_en);
	sseu->eu_total = compute_eu_total(sseu);
}

static void gen12_sseu_info_init(struct intel_gt *gt)
{
	struct drm_i915_private *i915 = gt->i915;
	struct intel_display *display=i915->display;
	
	struct sseu_dev_info *sseu = &gt->info.sseu;
	u32 g_dss_en;
	u16 eu_en = 0;
	u8 eu_en_fuse;
	u8 s_en;
	int eu;

	intel_sseu_set_info(sseu, 1, 6, 16);

	s_en = REG_FIELD_GET(GEN11_GT_S_ENA_MASK,
						 intel_de_read (display, GEN11_GT_SLICE_ENABLE));
	//drm_WARN_ON(&gt->i915->drm, s_en != 0x1);

	g_dss_en = intel_de_read (display,GEN12_GT_GEOMETRY_DSS_ENABLE);

	eu_en_fuse = ~REG_FIELD_GET(GEN11_EU_DIS_MASK,
								intel_de_read (display,GEN11_EU_DISABLE));

	for (eu = 0; eu < sseu->max_eus_per_subslice / 2; eu++)
		if (eu_en_fuse & BIT(eu))
			eu_en |= BIT(eu * 2) | BIT(eu * 2 + 1);

	gen11_compute_sseu_info(sseu, g_dss_en, eu_en);

	sseu->has_slice_pg = 1;
}

static const struct intel_mmio_range icl_l3bank_steering_table[] = {
	{ 0x00B100, 0x00B3FF },
	{},
};

static void gen11_sseu_device_status(struct intel_gt *gt,
					 struct sseu_dev_info *sseu)
{
#define SS_MAX 8
	struct drm_i915_private *i915 = gt->i915;
	struct intel_display *display=i915->display;
	const struct intel_gt_info *info = &gt->info;
	u32 s_reg[SS_MAX], eu_reg[2 * SS_MAX], eu_mask[2];
	int s, ss;
	
	for (s = 0; s < info->sseu.max_slices; s++) {

		s_reg[s] = intel_de_read (display, GEN10_SLICE_PGCTL_ACK(s)) &
			GEN10_PGCTL_VALID_SS_MASK(s);
		eu_reg[2 * s] = intel_de_read (display,
						  GEN10_SS01_EU_PGCTL_ACK(s));
		eu_reg[2 * s + 1] = intel_de_read (display,
							  GEN10_SS23_EU_PGCTL_ACK(s));
	}

	eu_mask[0] = GEN9_PGCTL_SSA_EU08_ACK |
			 GEN9_PGCTL_SSA_EU19_ACK |
			 GEN9_PGCTL_SSA_EU210_ACK |
			 GEN9_PGCTL_SSA_EU311_ACK;
	eu_mask[1] = GEN9_PGCTL_SSB_EU08_ACK |
			 GEN9_PGCTL_SSB_EU19_ACK |
			 GEN9_PGCTL_SSB_EU210_ACK |
			 GEN9_PGCTL_SSB_EU311_ACK;

	for (s = 0; s < info->sseu.max_slices; s++) {
		if ((s_reg[s] & GEN9_PGCTL_SLICE_ACK) == 0)
			continue;

		sseu->slice_mask |= BIT(s);
		sseu->subslice_mask.hsw[s] = info->sseu.subslice_mask.hsw[s];

		for (ss = 0; ss < info->sseu.max_subslices; ss++) {
			unsigned int eu_cnt;

			if (info->sseu.has_subslice_pg &&
				!(s_reg[s] & (GEN9_PGCTL_SS_ACK(ss))))
				continue;

			eu_cnt = 2 * hweight32(eu_reg[2 * s + ss / 2] &
						   eu_mask[ss % 2]);
			sseu->eu_total += eu_cnt;
			sseu->eu_per_subslice = max_t(unsigned int,
							  sseu->eu_per_subslice,
							  eu_cnt);
		}
	}
#undef SS_MAX
}


void Gen11::engines()
{
	struct drm_i915_private *i915 = NBlue::callback->i915b;
	struct intel_gt *gt=to_gt(i915);
	struct intel_display *display=i915->display;
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	gt->submission_method = INTEL_SUBMISSION_GUC;

	gen12_sseu_info_init(gt);
	gen11_sseu_device_status(gt, &gt->info.sseu);
	
	if (GRAPHICS_VER(i915) >= 11 &&
		GRAPHICS_VER_FULL(i915) < IP_VER(12, 55)) {
		gt->steering_table[L3BANK] = icl_l3bank_steering_table;
		gt->info.l3bank_mask =
		~intel_de_read (display, GEN10_MIRROR_FUSE3) &
		GEN10_L3BANK_MASK;
	}
	intel_engines_init_mmio(gt);
	
	for_each_engine(engine, gt, id) {
		
		engine->sseu =	intel_sseu_from_device_info(&engine->gt->info.sseu);
		
		engine->wa_list.count = 0;
		engine->whitelist.count = 0;
		engine->ctx_wa_list.count = 0;
		
		engine->flags |= I915_ENGINE_HAS_RELATIVE_MMIO;
		
		intel_engine_init_workarounds(engine);
		intel_engine_init_whitelist(engine);
		intel_engine_init_ctx_wa(engine);
		
	}
	
	for_each_engine(engine, gt, id) {
		
		intel_engine_apply_workarounds(engine);
		intel_engine_apply_whitelist(engine);
		
		//intel_engine_emit_ctx_wa
		wa_list_apply(&engine->ctx_wa_list); //???
	}
}


unsigned long  Gen11::startGraphicsEngine(void *that)
{
	seng=true;
	auto ret= FunctionCast(startGraphicsEngine, callback->ostartGraphicsEngine)( that);
	
	//intel_gt_resume
	//intel_uc_init_late(&gt->uc);
	
	return ret;
}

void  Gen11::initHardwareStatusPageRegisters(void *that)
{
	FunctionCast(initHardwareStatusPageRegisters, callback->oinitHardwareStatusPageRegisters)( that);
	if (seng)
	{
		seng=false;
		engines();
	}
}

void Gen11::sanitizeCDClockFrequency(void *that) {

	//auto referenceFrequency = callback->wrapReadRegister32(that, SKL_DSSM) & ICL_DSSM_CDCLK_PLL_REFCLK_MASK;
	auto referenceFrequency =NBlue::callback->readReg32(ICL_REG_DSSM)>> 29;
	//auto referenceFrequency = callback->wrapReadRegister32(that, ICL_REG_DSSM) >> 29;
	uint32_t newCdclkFrequency = 0;
	uint32_t newPLLFrequency = 0;
	switch (referenceFrequency) {
		case ICL_REF_CLOCK_FREQ_19_2:
			newCdclkFrequency = ICL_CDCLK_FREQ_652_8;
			newPLLFrequency = ICL_CDCLK_PLL_FREQ_REF_19_2;
			break;
			
		case ICL_REF_CLOCK_FREQ_24_0:
			newCdclkFrequency = ICL_CDCLK_FREQ_648_0;
			newPLLFrequency = ICL_CDCLK_PLL_FREQ_REF_24_0;
			break;
			
		case ICL_REF_CLOCK_FREQ_38_4:
			newCdclkFrequency = ICL_CDCLK_FREQ_652_8;
			newPLLFrequency = ICL_CDCLK_PLL_FREQ_REF_38_4;
			break;
			
		default:
			return;
	}
	
	callback->orgDisableCDClock(that);
	
	callback->orgSetCDClockFrequency(that, newPLLFrequency);
	NBlue::callback->readReg32( ICL_REG_CDCLK_CTL) ;
}

uint32_t Gen11::wrapProbeCDClockFrequency(void *that) {

	//auto cdclk = NBlue::callback->readReg32(ICL_REG_CDCLK_CTL) & BXT_CDCLK_CD2X_DIV_SEL_MASK;
	auto cdclk =NBlue::callback->readReg32(ICL_REG_CDCLK_CTL) & CDCLK_FREQ_DECIMAL_MASK;
	
	if (cdclk < ICL_CDCLK_DEC_FREQ_THRESHOLD) {
		sanitizeCDClockFrequency(that);
	}
	
	auto retVal = callback->orgProbeCDClockFrequency(that);
	return retVal;
}




void Gen11::hwConfigureCustomAUX(void *that,bool param_1)
{
	//FunctionCast(hwConfigureCustomAUX, callback->ohwConfigureCustomAUX)(that,param_1 );
}

unsigned long Gen11::hwRegsNeedUpdate
		  (void *that,void *param_1,
		   void *param_2,void *param_3,void *param_4,
		   void *param_5)
{
	
	auto ret=FunctionCast(hwRegsNeedUpdate, callback->ohwRegsNeedUpdate)(that,param_1,param_2,param_3,param_4,param_5 );

	return 0;
	
	if (hwu==1){
		hwu=0;
		return 1;
	}
	return ret;
}

int Gen11::getTranscoderOffset(void *that,void *param_1,uint param_2)
{
	auto ret=FunctionCast(getTranscoderOffset, callback->ogetTranscoderOffset)(that,param_1,param_2 );
	
	return 0;
	return ret;
}

unsigned long  Gen11::AppleIntelPortHALinit(void *that,void *param_1)
{
	auto ret=FunctionCast(AppleIntelPortHALinit, callback->oAppleIntelPortHALinit)(that,param_1 );
	if (kexticl) {
		getMember<uint32_t>(that, 0x584)=0x60540;
		getMember<uint32_t>(that, 0x588)=0x60544;
	}
	if (linkp==nullptr) linkp=that;
	return ret;
}

unsigned long Gen11::hwSetMode(void *that,void *param_1,void *param_2,int param_3,void *param_4)
{
	auto ret=FunctionCast(hwSetMode, callback->ohwSetMode)(that,param_1,param_2,param_3,param_4 );
	
	if (hwu==2){
		hwu=1;
	}
	
	return ret;
}




uint64_t Gen11::aframeBufferNotificationcallback(void *param_1,void *param_2,void *param_3,int param_4,void *param_5)
{
	auto ret=FunctionCast(aframeBufferNotificationcallback, callback->oaframeBufferNotificationcallback)(param_1,param_2,param_3,param_4,param_5 );

	if (param_4 == 2)
	{
		if (hwu==3) hwu=2;
	}
	return ret;
}


static bool is_valid_dmc_id(enum intel_dmc_id dmc_id)
{
	return dmc_id >= DMC_FW_MAIN && dmc_id < DMC_FW_MAX;
}
static bool fw_info_matches_stepping(const struct intel_fw_info *fw_info,
					 const struct stepping_info *si)
{
	if ((fw_info->substepping == '*' && si->stepping == fw_info->stepping) ||
		(si->stepping == fw_info->stepping && si->substepping == fw_info->substepping) ||

		(si->stepping == '*' && si->substepping == fw_info->substepping) ||
		(fw_info->stepping == '*' && fw_info->substepping == '*'))
		return true;

	return false;
}
static void dmc_set_fw_offset(struct intel_dmc *dmc,
				  const struct intel_fw_info *fw_info,
				  unsigned int num_entries,
				  const struct stepping_info *si,
				  u8 package_ver)
{
	enum intel_dmc_id dmc_id;
	unsigned int i;

	for (i = 0; i < num_entries; i++) {
		dmc_id = static_cast<intel_dmc_id>(package_ver <= 1 ? DMC_FW_MAIN : fw_info[i].dmc_id);

		if (!is_valid_dmc_id(dmc_id)) {
			continue;
		}


		if (dmc->dmc_info[dmc_id].present)
			continue;

		
		if (fw_info_matches_stepping(&fw_info[i], si)) {
			dmc->dmc_info[dmc_id].present = true;
			dmc->dmc_info[dmc_id].dmc_offset = fw_info[i].offset;
		}
	}
}

static u32
parse_dmc_fw_package(struct intel_dmc *dmc,
			 const struct intel_package_header *package_header,
			 const struct stepping_info *si,
			 size_t rem_size)
{
	u32 package_size = sizeof(struct intel_package_header);
	u32 num_entries, max_entries;
	const struct intel_fw_info *fw_info;

	if (rem_size < package_size)
		goto error_truncated;

	if (package_header->header_ver == 1) {
		max_entries = PACKAGE_MAX_FW_INFO_ENTRIES;
	} else if (package_header->header_ver == 2) {
		max_entries = PACKAGE_V2_MAX_FW_INFO_ENTRIES;
	} else {
		return 0;
	}


	package_size += max_entries * sizeof(struct intel_fw_info);
	if (rem_size < package_size)
		goto error_truncated;

	if (package_header->header_len * 4 != package_size) {
		return 0;
	}

	num_entries = package_header->num_entries;
	if ((num_entries > max_entries))
		num_entries = max_entries;
	

	fw_info = (const struct intel_fw_info *)
		((u8 *)package_header + sizeof(*package_header));
	dmc_set_fw_offset(dmc, fw_info, num_entries, si,
			  package_header->header_ver);

	return package_size;

error_truncated:
	return 0;
}

static bool dmc_mmio_addr_sanity_check(struct intel_dmc *dmc,
					   const u32 *mmioaddr, u32 mmio_count,
					   int header_ver, enum intel_dmc_id dmc_id)
{
	struct intel_display *display = dmc->display;
	u32 start_range, end_range;
	u32 i;

	if (header_ver == 1) {
		start_range = DMC_MMIO_START_RANGE;
		end_range = DMC_MMIO_END_RANGE;
	} else if (dmc_id == DMC_FW_MAIN) {
		start_range = TGL_MAIN_MMIO_START;
		end_range = TGL_MAIN_MMIO_END;
	} else if (DISPLAY_VER(display) >= 13) {
		start_range = ADLP_PIPE_MMIO_START;
		end_range = ADLP_PIPE_MMIO_END;
	} else if (DISPLAY_VER(display) >= 12) {
		start_range = TGL_PIPE_MMIO_START(dmc_id);
		end_range = TGL_PIPE_MMIO_END(dmc_id);
	} else {
		return false;
	}

	for (i = 0; i < mmio_count; i++) {
		if (mmioaddr[i] < start_range || mmioaddr[i] > end_range)
			return false;
	}

	return true;
}


static bool is_dmc_evt_ctl_reg(struct intel_display *display,
				   enum intel_dmc_id dmc_id, u32 reg)
{
	u32 offset = (reg);
	u32 start = (DMC_EVT_CTL(display, dmc_id, 0));
	u32 end = (DMC_EVT_CTL(display, dmc_id, DMC_EVENT_HANDLER_COUNT_GEN12));

	return offset >= start && offset < end;
}

static bool is_dmc_evt_htp_reg(struct intel_display *display,
				   enum intel_dmc_id dmc_id, u32 reg)
{
	u32 offset = (reg);
	u32 start = (DMC_EVT_HTP(display, dmc_id, 0));
	u32 end = (DMC_EVT_HTP(display, dmc_id, DMC_EVENT_HANDLER_COUNT_GEN12));

	return offset >= start && offset < end;
}

static bool is_event_handler(struct intel_display *display,
				 enum intel_dmc_id dmc_id,
				 unsigned int event_id,
							 u32 reg, u32 data)
{
	return is_dmc_evt_ctl_reg(display, dmc_id, reg) &&
		REG_FIELD_GET(DMC_EVT_CTL_EVENT_ID_MASK, data) == event_id;
}

static bool fixup_dmc_evt(struct intel_display *display,
			  enum intel_dmc_id dmc_id,
						  u32 reg_ctl, u32 *data_ctl,
						  u32 reg_htp, u32 *data_htp)
{
	if (!is_dmc_evt_ctl_reg(display, dmc_id, reg_ctl))
		return false;

	if (!is_dmc_evt_htp_reg(display, dmc_id, reg_htp))
		return false;

	if ((reg_ctl) - (DMC_EVT_CTL(display, dmc_id, 0)) !=
		(reg_htp) - (DMC_EVT_HTP(display, dmc_id, 0)))
		return false;


	if (display->platform.alderlake_s && dmc_id == DMC_FW_MAIN &&
		is_event_handler(display, dmc_id, 0x32, reg_ctl, *data_ctl)) {
		*data_ctl = 0;
		*data_htp = 0;
		return true;
	}


	if ((display->platform.tigerlake || display->platform.alderlake_s) &&
		is_event_handler(display, dmc_id, 0x32, reg_ctl, *data_ctl)) {
		*data_ctl &= ~DMC_EVT_CTL_EVENT_ID_MASK;
		*data_ctl |=  REG_FIELD_PREP(DMC_EVT_CTL_EVENT_ID_MASK,
									 0x2e);
		return true;
	}

	return false;
}


static u32 parse_dmc_fw_header(struct intel_dmc *dmc,
				   const struct intel_dmc_header_base *dmc_header,
				   size_t rem_size, enum intel_dmc_id dmc_id)
{
	struct intel_display *display = dmc->display;
	struct dmc_fw_info *dmc_info = &dmc->dmc_info[dmc_id];
	unsigned int header_len_bytes, dmc_header_size, payload_size, i;
	const u32 *mmioaddr, *mmiodata;
	u32 mmio_count, mmio_count_max, start_mmioaddr;
	u8 *payload;


	if (rem_size < sizeof(struct intel_dmc_header_base))
		goto error_truncated;

	if (dmc_header->header_ver == 3) {
		const struct intel_dmc_header_v3 *v3 =
			(const struct intel_dmc_header_v3 *)dmc_header;

		if (rem_size < sizeof(struct intel_dmc_header_v3))
			goto error_truncated;

		mmioaddr = v3->mmioaddr;
		mmiodata = v3->mmiodata;
		mmio_count = v3->mmio_count;
		mmio_count_max = DMC_V3_MAX_MMIO_COUNT;

		header_len_bytes = dmc_header->header_len * 4;
		start_mmioaddr = v3->start_mmioaddr;
		dmc_header_size = sizeof(*v3);
	} else if (dmc_header->header_ver == 1) {
		const struct intel_dmc_header_v1 *v1 =
			(const struct intel_dmc_header_v1 *)dmc_header;

		if (rem_size < sizeof(struct intel_dmc_header_v1))
			goto error_truncated;

		mmioaddr = v1->mmioaddr;
		mmiodata = v1->mmiodata;
		mmio_count = v1->mmio_count;
		mmio_count_max = DMC_V1_MAX_MMIO_COUNT;
		header_len_bytes = dmc_header->header_len;
		start_mmioaddr = DMC_V1_MMIO_START_RANGE;
		dmc_header_size = sizeof(*v1);
	} else {
		return 0;
	}

	if (header_len_bytes != dmc_header_size) {
		return 0;
	}

	if (mmio_count > mmio_count_max) {
		return 0;
	}

	if (!dmc_mmio_addr_sanity_check(dmc, mmioaddr, mmio_count,
					dmc_header->header_ver, dmc_id)) {
		return 0;
	}

	for (i = 0; i < mmio_count; i++) {
		dmc_info->mmioaddr[i] = _MMIO(mmioaddr[i]);
		dmc_info->mmiodata[i] = mmiodata[i];
	}

	for (i = 0; i < mmio_count - 1; i++) {

		if (!fixup_dmc_evt(display, dmc_id,
				   dmc_info->mmioaddr[i], &dmc_info->mmiodata[i],
				   dmc_info->mmioaddr[i+1], &dmc_info->mmiodata[i+1]))
			continue;

	}

	dmc_info->mmio_count = mmio_count;
	dmc_info->start_mmioaddr = start_mmioaddr;

	rem_size -= header_len_bytes;

	payload_size = dmc_header->fw_size * 4;
	if (rem_size < payload_size)
		goto error_truncated;

	if (payload_size > dmc->max_fw_size) {
		return 0;
	}
	dmc_info->dmc_fw_size = dmc_header->fw_size;

	dmc_info->payload = static_cast<UInt32 *>(IOMallocZero(payload_size));
	if (!dmc_info->payload)
		return 0;
	

	payload = (u8 *)(dmc_header) + header_len_bytes;
	memcpy(dmc_info->payload, payload, payload_size);

	return header_len_bytes + payload_size;

error_truncated:
	return 0;
}



static void initialize_stepping_info(struct intel_display *display, struct stepping_info *si)
{
	const char *step_name = DISPLAY_RUNTIME_INFO(display)->step_name;

	si->stepping = step_name[0] ?: '*';
	si->substepping = step_name[1] ?: '*';
}

static u32 parse_dmc_fw_css(struct intel_dmc *dmc,
				struct intel_css_header *css_header,
				size_t rem_size)
{

	if (rem_size < sizeof(struct intel_css_header)) {
		return 0;
	}

	if (sizeof(struct intel_css_header) !=
		(css_header->header_len * 4)) {
		return 0;
	}

	dmc->version = css_header->version;

	return sizeof(struct intel_css_header);
}
static struct intel_dmc *display_to_dmc(struct intel_display *display)
{
	return display->dmc.dmc;
}
static void disable_event_handler(struct intel_display *display,
				  u32 ctl_reg, u32 htp_reg)
{
	intel_de_write(display,  ctl_reg,
			   REG_FIELD_PREP(DMC_EVT_CTL_TYPE_MASK,
					  DMC_EVT_CTL_TYPE_EDGE_0_1) |
			   REG_FIELD_PREP(DMC_EVT_CTL_EVENT_ID_MASK,
					  DMC_EVENT_FALSE));
	intel_de_write(display, htp_reg, 0);
}
static bool has_dmc_id_fw(struct intel_display *display, enum intel_dmc_id dmc_id)
{
	struct intel_dmc *dmc = display_to_dmc(display);

	return dmc && dmc->dmc_info[dmc_id].payload;
}

bool intel_dmc_has_payload(struct intel_display *display)
{
	return has_dmc_id_fw(display, DMC_FW_MAIN);
}
static void disable_all_event_handlers(struct intel_display *display,
					   enum intel_dmc_id dmc_id)
{
	int handler;

	if (DISPLAY_VER(display) < 12)
		return;

	if (!has_dmc_id_fw(display, dmc_id))
		return;

	for (handler = 0; handler < DMC_EVENT_HANDLER_COUNT_GEN12; handler++)
		disable_event_handler(display,
					  DMC_EVT_CTL(display, dmc_id, handler),
					  DMC_EVT_HTP(display, dmc_id, handler));
}
static u32 dmc_evt_ctl_disable(u32 dmc_evt_ctl)
{

	return (dmc_evt_ctl & DMC_EVT_CTL_ENABLE) |
		REG_FIELD_PREP(DMC_EVT_CTL_TYPE_MASK,
				   DMC_EVT_CTL_TYPE_EDGE_0_1) |
		REG_FIELD_PREP(DMC_EVT_CTL_EVENT_ID_MASK,
				   DMC_EVENT_FALSE);
}
static bool disable_dmc_evt(struct intel_display *display,
				enum intel_dmc_id dmc_id,
							u32 reg, u32 data)
{
	if (!is_dmc_evt_ctl_reg(display, dmc_id, reg))
		return false;

	if (dmc_id != DMC_FW_MAIN)
		return true;

	if (display->platform.tigerlake &&
		is_event_handler(display, dmc_id, 0xbf, reg, data))
		return true;

	if ((display->platform.tigerlake || display->platform.alderlake_s) &&
		is_event_handler(display, dmc_id, 0x2e, reg, data))
		return true;

	return false;
}
static u32 dmc_mmiodata(struct intel_display *display,
			struct intel_dmc *dmc,
			enum intel_dmc_id dmc_id, int i)
{
	if (disable_dmc_evt(display, dmc_id,
				dmc->dmc_info[dmc_id].mmioaddr[i],
				dmc->dmc_info[dmc_id].mmiodata[i]))
		return dmc_evt_ctl_disable(dmc->dmc_info[dmc_id].mmiodata[i]);
	else
		return dmc->dmc_info[dmc_id].mmiodata[i];
}

static void dmc_load_mmio(struct intel_display *display, enum intel_dmc_id dmc_id)
{
	struct intel_dmc *dmc = display_to_dmc(display);
	u32 i;

	for (i = 0; i < dmc->dmc_info[dmc_id].mmio_count; i++) {
		intel_de_write(display, dmc->dmc_info[dmc_id].mmioaddr[i],
				   dmc_mmiodata(display, dmc, dmc_id, i));
	}
}
static void dmc_load_program(struct intel_display *display, enum intel_dmc_id dmc_id)
{
	struct intel_dmc *dmc = display_to_dmc(display);
	u32 i;

	disable_all_event_handlers(display, dmc_id);

	IOSimpleLock *myLock;
	myLock = IOSimpleLockAlloc();
	IOSimpleLockLock(myLock);

	//preempt_disable();

	for (i = 0; i < dmc->dmc_info[dmc_id].dmc_fw_size; i++) {
		intel_de_write(display,
				  DMC_PROGRAM(dmc->dmc_info[dmc_id].start_mmioaddr, i),
				  dmc->dmc_info[dmc_id].payload[i]);
	}

	//preempt_enable();
	
	IOSimpleLockUnlock(myLock);
	IOSimpleLockFree(myLock);
	
	dmc_load_mmio(display, dmc_id);
}


static u32 gen9_dc_mask(struct intel_display *display)
{
	u32 mask;

	mask = DC_STATE_EN_UPTO_DC5;

	if (DISPLAY_VER(display) >= 12)
		mask |= DC_STATE_EN_DC3CO | DC_STATE_EN_UPTO_DC6
					  | DC_STATE_EN_DC9;
	else if (DISPLAY_VER(display) == 11)
		mask |= DC_STATE_EN_UPTO_DC6 | DC_STATE_EN_DC9;
	else if (display->platform.geminilake || display->platform.broxton)
		mask |= DC_STATE_EN_DC9;
	else
		mask |= DC_STATE_EN_UPTO_DC6;

	return mask;
}
void intel_dmc_update_dc6_allowed_count(struct intel_display *display,
					bool start_tracking)
{
	struct intel_dmc *dmc = display_to_dmc(display);
	u32 dc5_cur_count;

	if (DISPLAY_VER(dmc->display) < 14)
		return;

	dc5_cur_count = intel_de_read(display,  DG1_DMC_DEBUG_DC5_COUNT);

	if (!start_tracking)
		dmc->dc6_allowed.count += dc5_cur_count - dmc->dc6_allowed.dc5_start;

	dmc->dc6_allowed.dc5_start = dc5_cur_count;
}

static void gen9_write_dc_state(struct intel_display *display,
				u32 state)
{
	int rewrites = 0;
	int rereads = 0;
	u32 v;

	intel_de_write(display, DC_STATE_EN, state);

	do  {
		v = intel_de_read(display,  DC_STATE_EN);

		if (v != state) {
			intel_de_write(display, DC_STATE_EN, state);
			rewrites++;
			rereads = 0;
		} else if (rereads++ > 5) {
			break;
		}

	} while (rewrites < 100);

	/*if (v != state)
		drm_err(display->drm,
			"Writing dc state to 0x%x failed, now 0x%x\n",
			state, v);*/

	/* Most of the times we need one retry, avoid spam */
	/*if (rewrites > 1)
		drm_dbg_kms(display->drm,
				"Rewrote dc state to 0x%x %d times\n",
				state, rewrites);*/
}

void gen9_set_dc_state(struct intel_display *display, u32 state)
{
	struct i915_power_domains *power_domains = &display->power.domains;
	bool dc6_was_enabled, enable_dc6;
	u32 mask;
	u32 val;


	if ((state & ~power_domains->allowed_dc_mask))
		state &= power_domains->allowed_dc_mask;

	//if (!power_domains->initializing)
	//	intel_psr_notify_dc5_dc6(display);

	val = intel_de_read(display,  DC_STATE_EN);
	mask = gen9_dc_mask(display);
	//drm_dbg_kms(display->drm, "Setting DC state from %02x to %02x\n",
		//	val & mask, state);

	/* Check if DMC is ignoring our DC state requests */
	//if ((val & mask) != power_domains->dc_state)
	//	drm_err(display->drm, "DC state mismatch (0x%x -> 0x%x)\n",
		//	power_domains->dc_state, val & mask);

	enable_dc6 = state & DC_STATE_EN_UPTO_DC6;
	dc6_was_enabled = power_domains->dc_state & DC_STATE_EN_UPTO_DC6;
	if (!dc6_was_enabled && enable_dc6)
		intel_dmc_update_dc6_allowed_count(display, true);

	val &= ~mask;
	val |= state;

	gen9_write_dc_state(display, val);

	if (!enable_dc6 && dc6_was_enabled)
		intel_dmc_update_dc6_allowed_count(display, false);

	power_domains->dc_state = val & mask;
}

static int gen7_check_mailbox_status(u32 mbox)
{
	switch (mbox & GEN6_PCODE_ERROR_MASK) {
	case GEN6_PCODE_SUCCESS:
		return 0;
	case GEN6_PCODE_ILLEGAL_CMD:
		return -ENXIO;
	case GEN7_PCODE_TIMEOUT:
		return -ETIMEDOUT;
	case GEN7_PCODE_ILLEGAL_DATA:
		return -EINVAL;
	case GEN11_PCODE_ILLEGAL_SUBCOMMAND:
		return -ENXIO;
	case GEN11_PCODE_LOCKED:
		return -EBUSY;
	case GEN11_PCODE_REJECTED:
		return -EACCES;
	case GEN7_PCODE_MIN_FREQ_TABLE_GT_RATIO_OUT_OF_RANGE:
		return -EOVERFLOW;
	default:
		return 0;
	}
}


static int snb_pcode_rw( u32 mbox,
			  u32 *val, u32 *val1,
			  int fast_timeout_us, int slow_timeout_ms,
			  bool is_read)
{
	struct intel_display *display=NBlue::callback->i915b->display;

	
	if (intel_de_read(display, GEN6_PCODE_MAILBOX) & GEN6_PCODE_READY)
		return -EAGAIN;

	intel_de_write(display, GEN6_PCODE_DATA, *val);
	intel_de_write(display, GEN6_PCODE_DATA1, val1 ? *val1 : 0);
	intel_de_write(display, GEN6_PCODE_MAILBOX, GEN6_PCODE_READY | mbox);

	u32 iVar2 = -20;
	u32 iVar4=0;
	do {
		if (iVar2 == 0) return -ETIMEDOUT;
	  IODelay(1);
		iVar4 = intel_de_read(display, GEN6_PCODE_MAILBOX);
	  iVar2 = iVar2 + 1;
	} while (iVar4 < 0);

	if (is_read)
		*val = intel_de_read(display, GEN6_PCODE_DATA);
	if (is_read && val1)
		*val1 = intel_de_read(display, GEN6_PCODE_DATA1);

		return gen7_check_mailbox_status(mbox);

}

int snb_pcode_read( u32 mbox, u32 *val, u32 *val1)
{
	int err;

	IOSimpleLock *myLock;
	myLock = IOSimpleLockAlloc();
		IOSimpleLockLock(myLock);
		err = snb_pcode_rw( mbox, val, val1, 500, 20, true);
		IOSimpleLockUnlock(myLock);

	IOSimpleLockFree(myLock);

	return err;
}

static void
tgl_tc_cold_request(struct intel_display *display, bool block)
{
	u8 tries = 0;
	int ret;
	
	while (1) {
		u32 low_val;
		u32 high_val = 0;
		
		if (block)
			low_val = TGL_PCODE_EXIT_TCCOLD_DATA_L_BLOCK_REQ;
		else
			low_val = TGL_PCODE_EXIT_TCCOLD_DATA_L_UNBLOCK_REQ;
		
		ret=snb_pcode_read( TGL_PCODE_TCCOLD, &low_val, &high_val);
		
		if (ret == 0) {
			if (block &&
				(low_val & TGL_PCODE_EXIT_TCCOLD_DATA_L_EXIT_FAILED))
				ret = -EIO;
			else
				break;
		}
		
		if (++tries == 3)
			break;
		
		IOSleep(1);
	}
	
}



static void intel_pch_reset_handshake(struct intel_display *display,
					  bool enable)
{
	u32 reg;
	u32 reset_bits;

	if (DISPLAY_VER(display) >= 35)
		return;


		reg = HSW_NDE_RSTWRN_OPT;
		reset_bits = RESET_PCH_HANDSHAKE_ENABLE;
	

	//if (DISPLAY_VER(display) >= 14)
	//	reset_bits |= MTL_RESET_PICA_HANDSHAKE_EN;

	intel_de_rmw(display, reg, reset_bits, enable ? reset_bits : 0);
}



static bool check_phy_reg(struct intel_display *display,
			  enum phy phy, u32 reg, u32 mask,
			  u32 expected_val)
{
	u32 val = intel_de_read(display, reg);

	if ((val & mask) != expected_val) {
		return false;
	}

	return true;
}
static const struct icl_procmon *
icl_get_procmon_ref_values(struct intel_display *display, enum phy phy)
{
	u32 val;

	val = intel_de_read(display, ICL_PORT_COMP_DW3(phy));
	switch (val & (PROCESS_INFO_MASK | VOLTAGE_INFO_MASK)) {
	default:
	case VOLTAGE_INFO_0_85V | PROCESS_INFO_DOT_0:
		return &icl_procmon_values[PROCMON_0_85V_DOT_0];
	case VOLTAGE_INFO_0_95V | PROCESS_INFO_DOT_0:
		return &icl_procmon_values[PROCMON_0_95V_DOT_0];
	case VOLTAGE_INFO_0_95V | PROCESS_INFO_DOT_1:
		return &icl_procmon_values[PROCMON_0_95V_DOT_1];
	case VOLTAGE_INFO_1_05V | PROCESS_INFO_DOT_0:
		return &icl_procmon_values[PROCMON_1_05V_DOT_0];
	case VOLTAGE_INFO_1_05V | PROCESS_INFO_DOT_1:
		return &icl_procmon_values[PROCMON_1_05V_DOT_1];
	}
}

static bool icl_verify_procmon_ref_values(struct intel_display *display,
					  enum phy phy)
{
	const struct icl_procmon *procmon;
	bool ret;

	procmon = icl_get_procmon_ref_values(display, phy);

	ret = check_phy_reg(display, phy, ICL_PORT_COMP_DW1(phy),
				(0xff << 16) | 0xff, procmon->dw1);
	ret &= check_phy_reg(display, phy, ICL_PORT_COMP_DW9(phy),
				 -1U, procmon->dw9);
	ret &= check_phy_reg(display, phy, ICL_PORT_COMP_DW10(phy),
				 -1U, procmon->dw10);

	return ret;
}
static bool has_phy_misc(struct intel_display *display, enum phy phy)
{

	if (display->platform.alderlake_s)
		return phy == PHY_A;
	else if ((display->platform.jasperlake || display->platform.elkhartlake) ||
		 display->platform.rocketlake ||
		 display->platform.dg1)
		return phy < PHY_C;

	return true;
}
static bool icl_combo_phy_enabled(struct intel_display *display,
				  enum phy phy)
{
	if (!has_phy_misc(display, phy))
		return intel_de_read(display, ICL_PORT_COMP_DW0(phy)) & COMP_INIT;
	else
		return !(intel_de_read(display, ICL_PHY_MISC(phy)) &
			 ICL_PHY_MISC_DE_IO_COMP_PWR_DOWN) &&
			(intel_de_read(display, ICL_PORT_COMP_DW0(phy)) & COMP_INIT);
}
static bool phy_is_master(struct intel_display *display, enum phy phy)
{

	if (phy == PHY_A)
		return true;
	else if (display->platform.alderlake_s)
		return phy == PHY_D;
	else if (display->platform.dg1 || display->platform.rocketlake)
		return phy == PHY_C;

	return false;
}
static bool icl_combo_phy_verify_state(struct intel_display *display,
					   enum phy phy)
{
	bool ret = true;
	u32 expected_val = 0;

	if (!icl_combo_phy_enabled(display, phy))
		return false;

	if (DISPLAY_VER(display) >= 12) {
		ret &= check_phy_reg(display, phy, ICL_PORT_TX_DW8_LN(0, phy),
					 ICL_PORT_TX_DW8_ODCC_CLK_SEL |
					 ICL_PORT_TX_DW8_ODCC_CLK_DIV_SEL_MASK,
					 ICL_PORT_TX_DW8_ODCC_CLK_SEL |
					 ICL_PORT_TX_DW8_ODCC_CLK_DIV_SEL_DIV2);

		ret &= check_phy_reg(display, phy, ICL_PORT_PCS_DW1_LN(0, phy),
					 DCC_MODE_SELECT_MASK, RUN_DCC_ONCE);
	}

	ret &= icl_verify_procmon_ref_values(display, phy);

	if (phy_is_master(display, phy)) {
		ret &= check_phy_reg(display, phy, ICL_PORT_COMP_DW8(phy),
					 IREFGEN, IREFGEN);

	}

	ret &= check_phy_reg(display, phy, ICL_PORT_CL_DW5(phy),
				 CL_POWER_DOWN_ENABLE, CL_POWER_DOWN_ENABLE);

	return ret;
}
static void icl_set_procmon_ref_values(struct intel_display *display,
					   enum phy phy)
{
	const struct icl_procmon *procmon;

	procmon = icl_get_procmon_ref_values(display, phy);

	intel_de_rmw(display, ICL_PORT_COMP_DW1(phy),
			 (0xff << 16) | 0xff, procmon->dw1);

	intel_de_write(display, ICL_PORT_COMP_DW9(phy), procmon->dw9);
	intel_de_write(display, ICL_PORT_COMP_DW10(phy), procmon->dw10);
}
static void icl_combo_phys_init(struct intel_display *display)
{
	enum phy phy;

	for_each_combo_phy(display, phy) {
		const struct icl_procmon *procmon;
		u32 val;

		if (icl_combo_phy_verify_state(display, phy))
			continue;

		procmon = icl_get_procmon_ref_values(display, phy);

		if (!has_phy_misc(display, phy))
			goto skip_phy_misc;

		val = intel_de_read(display, ICL_PHY_MISC(phy));

		val &= ~ICL_PHY_MISC_DE_IO_COMP_PWR_DOWN;
		intel_de_write(display, ICL_PHY_MISC(phy), val);

skip_phy_misc:
		if (DISPLAY_VER(display) >= 12) {
			val = intel_de_read(display, ICL_PORT_TX_DW8_LN(0, phy));
			val &= ~ICL_PORT_TX_DW8_ODCC_CLK_DIV_SEL_MASK;
			val |= ICL_PORT_TX_DW8_ODCC_CLK_SEL;
			val |= ICL_PORT_TX_DW8_ODCC_CLK_DIV_SEL_DIV2;
			intel_de_write(display, ICL_PORT_TX_DW8_GRP(phy), val);

			val = NBlue::callback->intel_de_read(display, ICL_PORT_PCS_DW1_LN(0, phy));
			val &= ~DCC_MODE_SELECT_MASK;
			val |= RUN_DCC_ONCE;
			intel_de_write(display, ICL_PORT_PCS_DW1_GRP(phy), val);
		}

		icl_set_procmon_ref_values(display, phy);

		if (phy_is_master(display, phy))
			intel_de_rmw(display, ICL_PORT_COMP_DW8(phy),
					 0, IREFGEN);

		intel_de_rmw(display, ICL_PORT_COMP_DW0(phy), 0, COMP_INIT);
		intel_de_rmw(display, ICL_PORT_CL_DW5(phy),
				 0, CL_POWER_DOWN_ENABLE);
	}
}

static IOReturn __intel_de_wait_for_register(struct intel_display *display,
											 uint32_t reg,
											  uint32_t mask,
											  uint32_t value,
											  uint32_t timeout_us,
											  uint32_t *out_val,
											  bool is_atomic)
{
	AbsoluteTime deadline, now;
	uint64_t timeout_ns = (uint64_t)timeout_us * NSEC_PER_USEC;
	nanoseconds_to_absolutetime(timeout_ns, &deadline);
	
	uint32_t wait_us = 10;
	const uint32_t wait_max_us = 1000;
	uint32_t reg_value;
	IOReturn ret = kIOReturnError;
	
	if (timeout_us <= 10) {
		is_atomic = true;
		wait_us = 1;
	}
	
	for (;;) {
		now = mach_absolute_time();
		
		
		reg_value = intel_de_read(display, reg);
		
		if ((reg_value & mask) == value) {
			ret = kIOReturnSuccess;
			break;
		}
		
		if (CMP_ABSOLUTETIME(&now, &deadline)) {
			ret = kIOReturnTimeout;
			break;
		}
		
		if (!is_atomic && wait_us >= 1000) {
			IOSleep(wait_us / 1000);
		} else {
			IODelay(wait_us);
		}
		
		if (wait_us < wait_max_us) {
			wait_us <<= 1;
		}
	}
	
	if (out_val != NULL) {
		*out_val = reg_value;
	}
	
	return ret;
}


static IOReturn intel_de_wait_for_register(struct intel_display *display,
										   uint32_t reg,
											uint32_t mask,
											uint32_t value,
											uint32_t fast_timeout_us,
											uint32_t slow_timeout_us,
											uint32_t *out_value,
											bool is_atomic)
{
	IOReturn ret = kIOReturnError;
	if (fast_timeout_us != 0) {
		ret = __intel_de_wait_for_register(display, reg, mask, value,
										   fast_timeout_us,
										   out_value, is_atomic);
	}
	if (ret != kIOReturnSuccess && slow_timeout_us != 0) {
		ret = __intel_de_wait_for_register(display, reg, mask, value,
										   slow_timeout_us,
										   out_value, is_atomic);
	}
	return ret;
}


IOReturn intel_de_wait_ms(struct intel_display *display,
						  uint32_t reg,
						  uint32_t mask,
						  uint32_t value,
						  unsigned int timeout_ms,
						  uint32_t *out_value)
{
	IOReturn ret;
	ret = intel_de_wait_for_register(display, reg, mask, value,
									 2,
									 timeout_ms * 1000,
									 out_value,
									 false);
	
	return ret;
}



int intel_de_wait_for_set_ms(struct intel_display *display, u32 reg,
							 u32 mask, unsigned int timeout_ms)
{
	return intel_de_wait_ms(display, reg, mask, mask, timeout_ms, NULL);
}

int intel_de_wait_for_clear_ms(struct intel_display *display, u32 reg,
							   u32 mask, unsigned int timeout_ms)
{
	return intel_de_wait_ms(display, reg, mask, 0, timeout_ms, NULL);
}

static void gen12_dbuf_slices_config(struct intel_display *display)
{
	enum dbuf_slice slice;

	for_each_dbuf_slice(display, slice)
	intel_de_rmw(display, DBUF_CTL_S(slice),
				 DBUF_TRACKER_STATE_SERVICE_MASK,
				 DBUF_TRACKER_STATE_SERVICE(8));
}

static void gen9_dbuf_slice_set(struct intel_display *display, enum dbuf_slice slice, bool enable)
{
	u32 reg = DBUF_CTL_S(slice);
	bool state;

	intel_de_rmw(display, reg, DBUF_POWER_REQUEST,
			 enable ? DBUF_POWER_REQUEST : 0);
	intel_de_posting_read(display, reg);
	//udelay(10);
	IODelay(10);
	state = intel_de_read(display, reg) & DBUF_POWER_STATE;
}

void gen9_dbuf_slices_update(struct intel_display *display,
				 u8 req_slices)
{
	struct i915_power_domains *power_domains = &display->power.domains;
	u8 slice_mask = DISPLAY_INFO(display)->dbuf.slice_mask;
	enum dbuf_slice slice;

	//IOSimpleLockLock(power_domains->lock);
	for_each_dbuf_slice(display, slice)
		gen9_dbuf_slice_set(display, slice, req_slices & BIT(slice));

	display->dbuf.enabled_slices = req_slices;
	//IOSimpleLockUnlock(power_domains->lock);
}


u8 intel_enabled_dbuf_slices_mask(struct intel_display *display)
{
	u8 enabled_slices = 0;
	enum dbuf_slice slice;

	for_each_dbuf_slice(display, slice) {
		if (intel_de_read(display, DBUF_CTL_S(slice)) & DBUF_POWER_STATE)
			enabled_slices |= BIT(slice);
	}

	return enabled_slices;
}
static void gen9_dbuf_enable(struct intel_display *display)
{
	u8 slices_mask;

	display->dbuf.enabled_slices = intel_enabled_dbuf_slices_mask(display);

	slices_mask = BIT(DBUF_S1) | display->dbuf.enabled_slices;

	gen9_dbuf_slices_update(display, slices_mask);
}

static void icl_mbus_init(struct intel_display *display)
{
	unsigned long abox_regs = DISPLAY_INFO(display)->abox_mask;
	u32 mask, val, i;

	if (display->platform.alderlake_p || DISPLAY_VER(display) >= 14)
		return;

	mask = MBUS_ABOX_BT_CREDIT_POOL1_MASK |
		MBUS_ABOX_BT_CREDIT_POOL2_MASK |
		MBUS_ABOX_B_CREDIT_MASK |
		MBUS_ABOX_BW_CREDIT_MASK;
	val = MBUS_ABOX_BT_CREDIT_POOL1(16) |
		MBUS_ABOX_BT_CREDIT_POOL2(16) |
		MBUS_ABOX_B_CREDIT(1) |
		MBUS_ABOX_BW_CREDIT(1);


	if (DISPLAY_VER(display) == 12)
		abox_regs |= BIT(0);

	for_each_set_bit(i, &abox_regs, BITS_PER_TYPE(abox_regs))
	intel_de_rmw(display, MBUS_ABOX_CTL(i), mask, val);
}
bool intel_dp_is_edp()
{
	struct intel_display *display = NBlue::callback->i915b->display;

	return (display->child0->device_type & DEVICE_TYPE_INTERNAL_CONNECTOR);
}

static int icl_pcode_read_mem_global_info(struct intel_display *display,
					  struct dram_info *dram_info)
{
	u32 val = 0;
	
	//ret = intel_parent_pcode_read(display, ICL_PCODE_MEM_SUBSYSYSTEM_INFO |
	//				  ICL_PCODE_MEM_SS_READ_GLOBAL_INFO, &val, NULL);
	
	snb_pcode_read(ICL_PCODE_MEM_SUBSYSYSTEM_INFO | ICL_PCODE_MEM_SS_READ_GLOBAL_INFO, &val, NULL);
	//if (ret)
		//return ret;

	if (DISPLAY_VER(display) >= 12) {
		switch (val & 0xf) {
		case 0:
			dram_info->type = INTEL_DRAM_DDR4;
			break;
		case 1:
			dram_info->type = INTEL_DRAM_DDR5;
			break;
		case 2:
			dram_info->type = INTEL_DRAM_LPDDR5;
			break;
		case 3:
			dram_info->type = INTEL_DRAM_LPDDR4;
			break;
		case 4:
			dram_info->type = INTEL_DRAM_DDR3;
			break;
		case 5:
			dram_info->type = INTEL_DRAM_LPDDR3;
			break;
		default:
			return -EINVAL;
		}
	} else {
		switch (val & 0xf) {
		case 0:
			dram_info->type = INTEL_DRAM_DDR4;
			break;
		case 1:
			dram_info->type = INTEL_DRAM_DDR3;
			break;
		case 2:
			dram_info->type = INTEL_DRAM_LPDDR3;
			break;
		case 3:
			dram_info->type = INTEL_DRAM_LPDDR4;
			break;
		default:
			return -EINVAL;
		}
	}

	dram_info->num_channels = (val & 0xf0) >> 4;
	dram_info->num_qgv_points = (val & 0xf00) >> 8;
	dram_info->num_psf_gv_points = (val & 0x3000) >> 12;

	return 0;
}
static int gen12_get_dram_info(struct intel_display *display, struct dram_info *dram_info)
{
	return icl_pcode_read_mem_global_info(display, dram_info);
}

static void tgl_bw_buddy_init(struct intel_display *display)
{
	struct dram_info dram_info0;// = intel_dram_info(display);
	struct dram_info *dram_info=&dram_info0;
	gen12_get_dram_info(display, dram_info);
	const struct buddy_page_mask *table;
	unsigned long abox_mask = DISPLAY_INFO(display)->abox_mask;
	int config, i;

	/* BW_BUDDY registers are not used on dgpu's beyond DG1 */
	if (display->platform.dgfx && !display->platform.dg1)
		return;

	if (intel_display_wa(display, INTEL_DISPLAY_WA_1409767108))
		/* Wa_1409767108 */
		table = wa_1409767108_buddy_page_masks;
	else
		table = tgl_buddy_page_masks;

	for (config = 0; table[config].page_mask != 0; config++)
		if (table[config].num_channels == dram_info->num_channels &&
			table[config].type == dram_info->type)
			break;

	if (table[config].page_mask == 0) {
		for_each_set_bit(i, &abox_mask, BITS_PER_TYPE(abox_mask))
		intel_de_write(display, BW_BUDDY_CTL(i),
					   BW_BUDDY_DISABLE);
	} else {
		for_each_set_bit(i, &abox_mask, BITS_PER_TYPE(abox_mask)) {
			intel_de_write(display, BW_BUDDY_PAGE_MASK(i),
					   table[config].page_mask);

			/* Wa_22010178259:tgl,dg1,rkl,adl-s */
			if (intel_display_wa(display, INTEL_DISPLAY_WA_22010178259))
				intel_de_rmw(display, BW_BUDDY_CTL(i),
						 BW_BUDDY_TLB_REQ_TIMER_MASK,
						 BW_BUDDY_TLB_REQ_TIMER(0x8));
		}
	}
}

static void icl_set_pipe_chicken()
{
	struct intel_display *display =NBlue::callback->i915b->display;
	enum pipe pipe = display->pipe0;
	u32 tmp;

	tmp = intel_de_read(display, PIPE_CHICKEN(pipe));

	tmp |= PER_PIXEL_ALPHA_BYPASS_EN;

	tmp |= PIXEL_ROUNDING_TRUNC_FB_PASSTHRU;

	if (display->platform.dg2)
		tmp &= ~UNDERRUN_RECOVERY_ENABLE_DG2;
	else if ((DISPLAY_VER(display) >= 13) && (DISPLAY_VER(display) < 30))
		tmp |= UNDERRUN_RECOVERY_DISABLE_ADLP;

	if (intel_display_wa(display, INTEL_DISPLAY_WA_14010547955))
		tmp |= DG2_RENDER_CCSTAG_4_3_EN;

	intel_de_write(display, PIPE_CHICKEN(pipe), tmp);
}

static int parse_dmc_fw(struct intel_display * display)
{
	struct intel_dmc *dmc=display->dmc.dmc;
	
	struct intel_css_header *css_header;
	struct intel_package_header *package_header;
	struct intel_dmc_header_base *dmc_header;
	struct stepping_info si = {};
	u32 readcount = 0;
	u32 r, offset;
	
	struct Firmware fw;
	
	
if (display->platform.alderlake_p) {
	fw = getFWByName("adlp_dmc_ver2_16.bin");
	display->dmc.dmc->max_fw_size= DISPLAY_VER13_DMC_MAX_FW_SIZE;
} else if (display->platform.alderlake_s) {
	fw = getFWByName("adls_dmc_ver2_01.bin");
	display->dmc.dmc->max_fw_size=DISPLAY_VER12_DMC_MAX_FW_SIZE;
} else if (display->platform.rocketlake) {
	fw = getFWByName("rkl_dmc_ver2_03.bin");
	display->dmc.dmc->max_fw_size= DISPLAY_VER12_DMC_MAX_FW_SIZE;
} else if (display->platform.tigerlake) {
	fw = getFWByName("tgl_dmc_ver2_12.bin");
	display->dmc.dmc->max_fw_size= DISPLAY_VER12_DMC_MAX_FW_SIZE;
}
	
	if (!fw.data || fw.size == 0) {
		return -1;
	}
	
	initialize_stepping_info(display, &si);
	
	css_header = (struct intel_css_header *)fw.data;
	r = parse_dmc_fw_css(dmc, css_header, fw.size);
	if (!r)
		return -1;

	readcount += r;

	package_header = (struct intel_package_header *)&fw.data[readcount];
	r = parse_dmc_fw_package(dmc, package_header, &si, fw.size - readcount);
	if (!r)
		return -1;

	readcount += r;

	for (uint8_t i = 0; i < DMC_FW_MAX; i++) {
		if (!dmc->dmc_info[i].present)
			continue;
		offset = readcount + dmc->dmc_info[i].dmc_offset * 4;
		if (offset > fw.size) {
			continue;
		}

		dmc_header = (struct intel_dmc_header_base *)&fw.data[offset];
		parse_dmc_fw_header(dmc, dmc_header, fw.size - offset, static_cast<intel_dmc_id>(i));
	}

	if (!intel_dmc_has_payload(display)) {
		return -1;
	}
	
	return 0;
}

static void adlp_pipedmc_clock_gating_wa(struct intel_display *display, bool enable)
{
	enum pipe p;

	if (enable) {
		for (p = static_cast<pipe>(PIPE_A); p <= PIPE_D;
			 p = static_cast<pipe>(static_cast<int>(p) + 1)) {
			intel_de_rmw(display, CLKGATE_DIS_PSL_EXT(static_cast<int>(p)),
					 0, PIPEDMC_GATING_DIS);
		}
	} else {
		for (p = static_cast<pipe>(PIPE_C); p <= PIPE_D;
			 p = static_cast<pipe>(static_cast<int>(p) + 1)) {
			intel_de_rmw(display, CLKGATE_DIS_PSL_EXT(static_cast<int>(p)),
					 PIPEDMC_GATING_DIS, 0);
		}
	}
}
static void pipedmc_clock_gating_wa(struct intel_display *display, bool enable)
{
	/*if (display->platform.meteorlake && enable)
		mtl_pipedmc_clock_gating_wa(display);
	else*/ if (DISPLAY_VER(display) == 13)
		adlp_pipedmc_clock_gating_wa(display, enable);
}

static void assert_dmc_loaded(struct intel_display *display,
				  enum intel_dmc_id dmc_id)
{
	struct intel_dmc *dmc = display_to_dmc(display);
	u32 expected, found;
	int i;

	if (!is_valid_dmc_id(dmc_id) || !has_dmc_id_fw(display, dmc_id))
		return;

	found = intel_de_read(display, DMC_PROGRAM(dmc->dmc_info[dmc_id].start_mmioaddr, 0));
	expected = dmc->dmc_info[dmc_id].payload[0];


	for (i = 0; i < dmc->dmc_info[dmc_id].mmio_count; i++) {
		u32 reg = dmc->dmc_info[dmc_id].mmioaddr[i];

		found = intel_de_read(display, reg);
		expected = dmc_mmiodata(display, dmc, dmc_id, i);

	}
}

static void gen9_set_dc_state_debugmask(struct intel_display *display)
{
	intel_de_rmw(display, DC_STATE_DEBUG, 0,
			 DC_STATE_DEBUG_MASK_CORES | DC_STATE_DEBUG_MASK_MEMORY_UP);
	intel_de_posting_read(display, DC_STATE_DEBUG);
}


void intel_dmc_load_program(struct intel_display *display)
{
	struct i915_power_domains *power_domains = &display->power.domains;
	enum intel_dmc_id dmc_id;

	if (!intel_dmc_has_payload(display))
		return;

	//assert_display_rpm_held(display);

	pipedmc_clock_gating_wa(display, true);

	for_each_dmc_id(dmc_id) {
		dmc_load_program(display, dmc_id);
		assert_dmc_loaded(display, dmc_id);
	}

	/*if (DISPLAY_VER(display) >= 20)
		intel_de_write(display, DMC_FQ_W2_PTS_CFG_SEL,
				   PIPE_D_DMC_W2_PTS_CONFIG_SELECT(PIPE_D) |
				   PIPE_C_DMC_W2_PTS_CONFIG_SELECT(PIPE_C) |
				   PIPE_B_DMC_W2_PTS_CONFIG_SELECT(PIPE_B) |
				   PIPE_A_DMC_W2_PTS_CONFIG_SELECT(PIPE_A));
*/
	power_domains->dc_state = 0;

	gen9_set_dc_state_debugmask(display);

	pipedmc_clock_gating_wa(display, false);
}

void Gen11::hwInitializeCState(void *that)
{
	struct drm_i915_private *i915=NBlue::callback->i915b;
	if (i915->initok) return;
	struct intel_display *display=i915->display;
	
	
	if (getMember<int>(that, kexticl ? 0xb38 : 0xb48) != 1) return;
	
	if (parse_dmc_fw(display)!=0) return;
	
	//intel_early_display_was
	if (intel_display_wa(display, INTEL_DISPLAY_WA_14010480278))
			intel_de_rmw(display, GEN9_CLKGATE_DIS_0, 0, DARBF_GATING_DIS);
	
	
	intel_dmc_load_program(display);

	/* Wa_14011508470:tgl,dg1,rkl,adl-s,adl-p,dg2 */
	if (intel_display_wa(display, INTEL_DISPLAY_WA_14011508470))
		intel_de_rmw(display, GEN11_CHICKEN_DCPR_2, 0,
				 DCPR_CLEAR_MEMSTAT_DIS | DCPR_SEND_RESP_IMM |
				 DCPR_MASK_LPMODE | DCPR_MASK_MAXLATENCY_MEMUP_CLR);

	/* Wa_14011503030:xelpd */
	if (intel_display_wa(display, INTEL_DISPLAY_WA_14011503030))
		intel_de_write(display, XELPD_DISPLAY_ERR_FATAL_MASK, ~0);

	/* Wa_15013987218 */
	if (intel_display_wa(display, INTEL_DISPLAY_WA_15013987218)) {
		intel_de_rmw(display, SOUTH_DSPCLK_GATE_D,
				 0, PCH_GMBUSUNIT_CLOCK_GATE_DISABLE);
		intel_de_rmw(display, SOUTH_DSPCLK_GATE_D,
				 PCH_GMBUSUNIT_CLOCK_GATE_DISABLE, 0);
	}
	
	
	//hsw_crtc_enable
	intel_de_rmw(display, PIPEDMC_CONTROL(PIPE_A), 0, PIPEDMC_ENABLE);
	
	hwConfigureCustomAUX(that, true);
	//intel_flipq_enable
}

void Gen11::setupPlane(void *that,void *param_1,int param_2)
{//icl
	FunctionCast(setupPlane, callback->osetupPlane)(that ,param_1,param_2);
	//skl_get_initial_plane_config
	//PLANE_CTL_1_A (0x00070180): 0x84000400
	//PLANE_STRIDE_1_A (0x00070188): 0x0000000d
	
	//getMember<uint32_t>(that, 0x100)=0x84000400;
	//getMember<uint32_t>(that, 0x118)=0xd;
}

void Gen11::setupPlane2(void *that,void *param_1)
{ //tgl
	FunctionCast(setupPlane2, callback->osetupPlane2)(that ,param_1);
	
	//PLANE_CTL_1_A (0x00070180): 0x84000400
	//PLANE_STRIDE_1_A (0x00070188): 0x0000000d
	
	//getMember<uint32_t>(that, 0x100)=0x84000400;
	//getMember<uint32_t>(that, 0x118)=0xd;
}

void Gen11::SetupParams (void *that,void *param_1,void *param_2,CRTCParams *param_3,void *param_4)
{
	if (getMember<uint32_t>(param_1, 0x1dc) == 0) setpc=1;
	if (!setpc) return FunctionCast(SetupParams, callback->oSetupParams)(that ,param_1,param_2,param_3,param_4);
	SetupParams2(param_2, param_3);
	setupPipeWatermarks(that ,param_1,param_2,param_3);
}

void Gen11::setupPipeWatermarks (void *that,void *param_1,void *param_2,CRTCParams *param_3)
{
	FunctionCast(setupPipeWatermarks, callback->osetupPipeWatermarks)(that ,param_1,param_2,param_3);
}

static u32 intel_ddi_transcoder_func_reg_val_get()
{
	struct intel_display *display=NBlue::callback->i915b->display;
	enum pipe pipe = display->pipe0;
	struct intel_crtc_state *crtc_state=&display->crtc_state0;
	enum transcoder cpu_transcoder = crtc_state->cpu_transcoder;
	enum port port = display->port0;
	u32 temp;
	
	temp = TRANS_DDI_FUNC_ENABLE;
	if (DISPLAY_VER(display) >= 12)
		temp |= TGL_TRANS_DDI_SELECT_PORT(port);
	else
		temp |= TRANS_DDI_SELECT_PORT(port);

	switch (crtc_state->pipe_bpp) {
	default:
	/*case 18:
		temp |= TRANS_DDI_BPC_6;
		break;*/
	case 24:
		temp |= TRANS_DDI_BPC_8;
		break;
	case 30:
		temp |= TRANS_DDI_BPC_10;
		break;
	case 36:
		temp |= TRANS_DDI_BPC_12;
		break;
	}

	if (crtc_state->hw.adjusted_mode.flags & DRM_MODE_FLAG_PVSYNC)
		temp |= TRANS_DDI_PVSYNC;
	if (crtc_state->hw.adjusted_mode.flags & DRM_MODE_FLAG_PHSYNC)
		temp |= TRANS_DDI_PHSYNC;

	if (cpu_transcoder == TRANSCODER_EDP) {
		switch (pipe) {
		default:
		case PIPE_A:
			if (crtc_state->pch_pfit.force_thru)
				temp |= TRANS_DDI_EDP_INPUT_A_ONOFF;
			else
				temp |= TRANS_DDI_EDP_INPUT_A_ON;
			break;
		case PIPE_B:
			temp |= TRANS_DDI_EDP_INPUT_B_ONOFF;
			break;
		case PIPE_C:
			temp |= TRANS_DDI_EDP_INPUT_C_ONOFF;
			break;
		}
	}
	
	temp |= TRANS_DDI_MODE_SELECT_DP_SST;
	temp |= DDI_PORT_WIDTH(crtc_state->lane_count);
	

	return temp;
}

static inline bool
intel_crtc_has_dp_encoder(const struct intel_crtc_state *crtc_state)
{
	return crtc_state->output_types &
		(BIT(INTEL_OUTPUT_DP) |
		 BIT(INTEL_OUTPUT_DP_MST) |
		 BIT(INTEL_OUTPUT_EDP));
}

static u32 intel_ddi_set_dp_msa(struct intel_display *display, bool wr)
{
	struct intel_crtc_state *crtc_state=&display->crtc_state0;
	enum transcoder cpu_transcoder = crtc_state->cpu_transcoder;
	u32 temp;

	//if (!intel_crtc_has_dp_encoder(crtc_state))
	//	return 0;
	
	temp = DP_MSA_MISC_SYNC_CLOCK;

	switch (display->panel.vbt.edp.bpp) {
	//case 18:
	//	temp |= DP_MSA_MISC_6_BPC;
	//	break;
	default:
	case 24:
		temp |= DP_MSA_MISC_8_BPC;
		break;
	case 30:
		temp |= DP_MSA_MISC_10_BPC;
		break;
	case 36:
		temp |= DP_MSA_MISC_12_BPC;
		break;

	}


	if (crtc_state->limited_color_range)
		temp |= DP_MSA_MISC_COLOR_CEA_RGB;


	if (crtc_state->output_format == INTEL_OUTPUT_FORMAT_YCBCR444)
		temp |= DP_MSA_MISC_COLOR_YCBCR_444_BT709;


	//if (intel_dp_needs_vsc_sdp())
	//	temp |= DP_MSA_MISC_COLOR_VSC_SDP;
	
	if (wr)
	intel_de_write(display, TRANS_MSA_MISC(display, cpu_transcoder),temp);
	return temp;
}

static u32 bdw_set_pipe_misc()
{
	struct intel_display *display = NBlue::callback->i915b->display;
	struct intel_crtc_state *crtc_state=&display->crtc_state0;
	
	u32 val = 0;

	switch (crtc_state->pipe_bpp) {
		default:
	/*case 18:
		val |= PIPE_MISC_BPC_6;
		break;*/
	case 24:
		val |= PIPE_MISC_BPC_8;
		break;
	case 30:
		val |= PIPE_MISC_BPC_10;
		break;
	case 36:
		if (DISPLAY_VER(display) >= 13)
			val |= PIPE_MISC_BPC_12_ADLP;
		break;
	//default:
		//break;
	}

	if (crtc_state->dither)
		val |= PIPE_MISC_DITHER_ENABLE | PIPE_MISC_DITHER_TYPE_SP;

	if (crtc_state->output_format == INTEL_OUTPUT_FORMAT_YCBCR420 ||
		crtc_state->output_format == INTEL_OUTPUT_FORMAT_YCBCR444)
		val |= PIPE_MISC_OUTPUT_COLORSPACE_YUV;

	if (crtc_state->output_format == INTEL_OUTPUT_FORMAT_YCBCR420)
		val |= DISPLAY_VER(display) >= 30 ? PIPE_MISC_YUV420_ENABLE :
			PIPE_MISC_YUV420_ENABLE | PIPE_MISC_YUV420_MODE_FULL_BLEND;

	//if (DISPLAY_VER(display) >= 11 && is_hdr_mode(crtc_state))
	//	val |= PIPE_MISC_HDR_MODE_PRECISION;

	if (DISPLAY_VER(display) >= 12)
		val |= PIPE_MISC_PIXEL_ROUNDING_TRUNC;


	//intel_de_write_dsb(display, dsb, PIPE_MISC(crtc->pipe), val);

	
	return val;
}



static const struct intel_ddi_buf_trans *
intel_get_buf_trans(const struct intel_ddi_buf_trans *trans, int *num_entries)
{
	*num_entries = trans->num_entries;
	return trans;
}

static inline bool
intel_crtc_has_type(const struct intel_crtc_state *crtc_state,
			enum intel_output_type type)
{
	return crtc_state->output_types & BIT(type);
}

static bool use_edp_low_vswing()
{
	struct intel_display *display = NBlue::callback->i915b->display;
	return display->panel.vbt.edp.low_vswing;
}

static bool use_edp_hobl()
{
	struct intel_display *display = NBlue::callback->i915b->display;
	struct intel_dp *intel_dp = &display->intel_dp0;

	return display->panel.vbt.edp.hobl && !intel_dp->hobl_failed;
}

static const struct intel_ddi_buf_trans *
tgl_get_combo_buf_trans_dp(const struct intel_crtc_state *crtc_state,
			   int *n_entries)
{
	struct intel_display *display = NBlue::callback->i915b->display;

	if (crtc_state->port_clock > 270000) {
		if (display->platform.tigerlake_uy) {
			return intel_get_buf_trans(&tgl_uy_combo_phy_trans_dp_hbr2,
						   n_entries);
		} else {
			return intel_get_buf_trans(&tgl_combo_phy_trans_dp_hbr2,
						   n_entries);
		}
	} else {
		return intel_get_buf_trans(&tgl_combo_phy_trans_dp_hbr,
					   n_entries);
	}
}

static const struct intel_ddi_buf_trans *
tgl_get_combo_buf_trans_edp(const struct intel_crtc_state *crtc_state,
				int *n_entries)
{
	if (crtc_state->port_clock > 540000) {
		return intel_get_buf_trans(&icl_combo_phy_trans_dp_hbr2_edp_hbr3,
					   n_entries);
	} else if (use_edp_hobl()) {
		return intel_get_buf_trans(&tgl_combo_phy_trans_edp_hbr2_hobl,
					   n_entries);
	} else if (use_edp_low_vswing()) {
		return intel_get_buf_trans(&icl_combo_phy_trans_edp_hbr2,
					   n_entries);
	}

	return tgl_get_combo_buf_trans_dp( crtc_state, n_entries);
}



static const struct intel_ddi_buf_trans *
tgl_get_combo_buf_trans(const struct intel_crtc_state *crtc_state,
			int *n_entries)
{
	if (intel_crtc_has_type(crtc_state, INTEL_OUTPUT_HDMI))
		return intel_get_buf_trans(&icl_combo_phy_trans_hdmi, n_entries);
	else if (intel_crtc_has_type(crtc_state, INTEL_OUTPUT_EDP))
		return tgl_get_combo_buf_trans_edp( crtc_state, n_entries);
	else
		return tgl_get_combo_buf_trans_dp( crtc_state, n_entries);
}

static u32 icl_combo_phy_loadgen_select(int lane)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	if (display->port_clock > 600000)
		return 0;

	if (NBlue::callback->i915b->display->panel.vbt.edp.lanes == 4)
		return lane >= 1 ? LOADGEN_SELECT : 0;
	else
		return lane == 1 || lane == 2 ? LOADGEN_SELECT : 0;
}


static int translate_signal_level(u8 signal_levels)
{
	const u8 *signal_array;
	size_t array_size;
	int i;

	signal_array = index_to_dp_signal_levels;
	array_size = ARRAY_SIZE(index_to_dp_signal_levels);

	for (i = 0; i < array_size; i++) {
		if (signal_array[i] == signal_levels)
			return i;
	}


	return 0;
}

static inline bool drm_dp_is_uhbr_rate(int link_rate)
{
	return link_rate >= 1000000;
}
bool intel_dp_is_uhbr(const struct intel_crtc_state *crtc_state)
{
	return drm_dp_is_uhbr_rate(crtc_state->port_clock);
}

static int intel_ddi_dp_level(struct intel_display *display,
				  int lane)
{
	u8 train_set = display->intel_dp0.train_set[lane];
	
	if (intel_dp_is_uhbr(&display->crtc_state0)) {
		return train_set & DP_TX_FFE_PRESET_VALUE_MASK;
	} else {
		u8 signal_levels = train_set & (DP_TRAIN_VOLTAGE_SWING_MASK |
						DP_TRAIN_PRE_EMPHASIS_MASK);

		return translate_signal_level( signal_levels);
	}
}



int intel_ddi_level(struct intel_display *display,
			int lane)
{
	const struct intel_ddi_buf_trans *trans;
	int level, n_entries;
	struct intel_crtc_state *crtc_state=&display->crtc_state0;

	trans=tgl_get_combo_buf_trans(crtc_state,&n_entries);

	/*if (intel_crtc_has_type(crtc_state, INTEL_OUTPUT_HDMI))
		level = intel_ddi_hdmi_level(encoder, trans);
	else*/
	level = intel_ddi_dp_level(display, lane);

	if (( level >= n_entries))
		level = n_entries - 1;

	return level;
}

bool is_hobl_buf_trans(const struct intel_ddi_buf_trans *table)
{
	return table == &tgl_combo_phy_trans_edp_hbr2_hobl;
}

static void icl_ddi_combo_vswing_program(struct intel_display *display)
{
	const struct intel_ddi_buf_trans *trans;
	enum phy phy = display->phy0;
	int n_entries, ln;
	u32 val;

	struct intel_crtc_state *crtc_state=&display->crtc_state0;

	trans=tgl_get_combo_buf_trans(crtc_state,&n_entries);
	
	if (intel_crtc_has_type(crtc_state, INTEL_OUTPUT_EDP)) {
		struct intel_dp *intel_dp = &display->intel_dp0;

		val = EDP4K2K_MODE_OVRD_EN | EDP4K2K_MODE_OVRD_OPTIMIZED;
		intel_dp->hobl_active = is_hobl_buf_trans(trans);
		intel_de_rmw(display, ICL_PORT_CL_DW10(phy), val,
				 intel_dp->hobl_active ? val : 0);
	}

	
	val = intel_de_read(display,  ICL_PORT_TX_DW5_LN(0, phy));
	val &= ~(SCALING_MODE_SEL_MASK | RTERM_SELECT_MASK |
		 COEFF_POLARITY | CURSOR_PROGRAM |
		 TAP2_DISABLE | TAP3_DISABLE);
	val |= SCALING_MODE_SEL(0x2);
	val |= RTERM_SELECT(0x6);
	val |= TAP3_DISABLE;
	intel_de_write(display, ICL_PORT_TX_DW5_GRP(phy), val);

	for (ln = 0; ln < 4; ln++) {
		int level = intel_ddi_level(display, ln);

		intel_de_rmw(display, ICL_PORT_TX_DW2_LN(ln, phy),
				 SWING_SEL_UPPER_MASK | SWING_SEL_LOWER_MASK | RCOMP_SCALAR_MASK,
				 SWING_SEL_UPPER(trans->entries[level].icl.dw2_swing_sel) |
				 SWING_SEL_LOWER(trans->entries[level].icl.dw2_swing_sel) |
				 RCOMP_SCALAR(0x98));
	}


	for (ln = 0; ln < 4; ln++) {
		int level = intel_ddi_level(display, ln);

		intel_de_rmw(display, ICL_PORT_TX_DW4_LN(ln, phy),
				 POST_CURSOR_1_MASK | POST_CURSOR_2_MASK | CURSOR_COEFF_MASK,
				 POST_CURSOR_1(trans->entries[level].icl.dw4_post_cursor_1) |
				 POST_CURSOR_2(trans->entries[level].icl.dw4_post_cursor_2) |
				 CURSOR_COEFF(trans->entries[level].icl.dw4_cursor_coeff));
	}

	for (ln = 0; ln < 4; ln++) {
		int level = intel_ddi_level(display, ln);

		intel_de_rmw(display, ICL_PORT_TX_DW7_LN(ln, phy),
				 N_SCALAR_MASK,
				 N_SCALAR(trans->entries[level].icl.dw7_n_scalar));
	}
}


static void icl_combo_phy_set_signal_levels(struct intel_display *display)
{
	enum phy phy = display->phy0;
	u32 val;
	int ln;
	struct intel_crtc_state *crtc_state=&display->crtc_state0;

	val = intel_de_read(display, ICL_PORT_PCS_DW1_LN(0, phy));
	if (intel_crtc_has_type(crtc_state, INTEL_OUTPUT_HDMI))
		val &= ~COMMON_KEEPER_EN;
	else
		val |= COMMON_KEEPER_EN;
	intel_de_write(display, ICL_PORT_PCS_DW1_GRP(phy), val);

	
	for (ln = 0; ln < 4; ln++) {
		intel_de_rmw(display, ICL_PORT_TX_DW4_LN(ln, phy),
				 LOADGEN_SELECT,
				 icl_combo_phy_loadgen_select(ln));
	}

	intel_de_rmw(display, ICL_PORT_CL_DW5(phy),
			 0, SUS_CLOCK_CONFIG);

	val = intel_de_read(display, ICL_PORT_TX_DW5_LN(0, phy));
	val &= ~TX_TRAINING_EN;
	intel_de_write(display, ICL_PORT_TX_DW5_GRP(phy), val);

	icl_ddi_combo_vswing_program(display);

	val = intel_de_read(display, ICL_PORT_TX_DW5_LN(0, phy));
	val |= TX_TRAINING_EN;
	intel_de_write(display, ICL_PORT_TX_DW5_GRP(phy), val);
}


static int intel_dp_training_pattern_set_reg(struct intel_dp *intel_dp,enum drm_dp_phy dp_phy)
{
	return dp_phy == DP_PHY_DPRX ?
		DP_TRAINING_PATTERN_SET :
		DP_TRAINING_PATTERN_SET_PHY_REPEATER(dp_phy);
}

static inline u8 intel_dp_training_pattern_symbol(u8 pattern)
{
	return pattern & ~DP_LINK_SCRAMBLING_DISABLE;
}

u32 dp_tp_ctl_reg()
{
	struct intel_display *display = NBlue::callback->i915b->display;
	struct intel_crtc_state *crtc_state = &display->crtc_state0;
	return TGL_DP_TP_CTL(display, crtc_state->cpu_transcoder);
}
static void intel_ddi_set_link_train(struct intel_dp *intel_dp,u8 dp_train_pat)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	u32 temp;

	temp = intel_de_read(display, dp_tp_ctl_reg());
	
	temp &= ~DP_TP_CTL_LINK_TRAIN_MASK;
	switch (intel_dp_training_pattern_symbol(dp_train_pat)) {
	case DP_TRAINING_PATTERN_DISABLE:
		temp |= DP_TP_CTL_LINK_TRAIN_NORMAL;
		break;
	case DP_TRAINING_PATTERN_1:
		temp |= DP_TP_CTL_LINK_TRAIN_PAT1;
		break;
	case DP_TRAINING_PATTERN_2:
		temp |= DP_TP_CTL_LINK_TRAIN_PAT2;
		break;
	case DP_TRAINING_PATTERN_3:
		temp |= DP_TP_CTL_LINK_TRAIN_PAT3;
		break;
	case DP_TRAINING_PATTERN_4:
		temp |= DP_TP_CTL_LINK_TRAIN_PAT4;
		break;
	}

	intel_de_write(display, dp_tp_ctl_reg(), temp);
}

void
intel_dp_program_link_training_pattern(struct intel_dp *intel_dp,enum drm_dp_phy dp_phy,
					   u8 dp_train_pat)
{
	u8 train_pat = intel_dp_training_pattern_symbol(dp_train_pat);
	intel_ddi_set_link_train( intel_dp,dp_train_pat);
}

static bool
intel_dp_set_link_train(struct intel_dp *intel_dp,enum drm_dp_phy dp_phy,
			u8 dp_train_pat)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	int reg = intel_dp_training_pattern_set_reg(intel_dp, dp_phy);
	u8 buf[sizeof(intel_dp->train_set) + 1];
	int len;

	intel_dp_program_link_training_pattern(intel_dp,
						   dp_phy, dp_train_pat);

	buf[0] = dp_train_pat;

	memcpy(buf + 1, intel_dp->train_set, display->panel.vbt.edp.lanes);
	len = display->panel.vbt.edp.lanes+ 1;

	return Gen11::callback->writeAUX(linkp,reg,buf, len)>=0;
}

static bool
intel_dp_reset_link_train(struct intel_dp *intel_dp,enum drm_dp_phy dp_phy,
			  u8 dp_train_pat)
{
	memset(intel_dp->train_set, 0, sizeof(intel_dp->train_set));
	icl_combo_phy_set_signal_levels(NBlue::callback->i915b->display);
	return intel_dp_set_link_train( intel_dp,dp_phy, dp_train_pat);
}

static u8 dp_link_status(const u8 link_status[DP_LINK_STATUS_SIZE], int r)
{
	return link_status[r - DP_LANE0_1_STATUS];
}

static u8 dp_get_lane_status(const u8 link_status[DP_LINK_STATUS_SIZE],
				 int lane)
{
	int i = DP_LANE0_1_STATUS + (lane >> 1);
	int s = (lane & 1) * 4;
	u8 l = dp_link_status(link_status, i);

	return (l >> s) & 0xf;
}

bool drm_dp_clock_recovery_ok(const u8 link_status[DP_LINK_STATUS_SIZE],
				  int lane_count)
{
	int lane;
	u8 lane_status;

	for (lane = 0; lane < lane_count; lane++) {
		lane_status = dp_get_lane_status(link_status, lane);
		if ((lane_status & DP_LANE_CR_DONE) == 0)
			return false;
	}
	return true;
}


static u8 drm_dp_get_adjust_request_voltage(const u8 link_status[DP_LINK_STATUS_SIZE],
					 int lane)
{
	int i = DP_ADJUST_REQUEST_LANE0_1 + (lane >> 1);
	int s = ((lane & 1) ?
		 DP_ADJUST_VOLTAGE_SWING_LANE1_SHIFT :
		 DP_ADJUST_VOLTAGE_SWING_LANE0_SHIFT);
	u8 l = dp_link_status(link_status, i);

	return ((l >> s) & 0x3) << DP_TRAIN_VOLTAGE_SWING_SHIFT;
}

static u8 drm_dp_get_adjust_request_pre_emphasis(const u8 link_status[DP_LINK_STATUS_SIZE],
					  int lane)
{
	int i = DP_ADJUST_REQUEST_LANE0_1 + (lane >> 1);
	int s = ((lane & 1) ?
		 DP_ADJUST_PRE_EMPHASIS_LANE1_SHIFT :
		 DP_ADJUST_PRE_EMPHASIS_LANE0_SHIFT);
	u8 l = dp_link_status(link_status, i);

	return ((l >> s) & 0x3) << DP_TRAIN_PRE_EMPHASIS_SHIFT;
}



static u8 intel_dp_phy_preemph_max(struct intel_dp *intel_dp,
				   enum drm_dp_phy dp_phy)
{
	u8 preemph_max;

	preemph_max = DP_TRAIN_PRE_EMPH_LEVEL_3;

	return preemph_max;
}

static u8 dp_voltage_max(u8 preemph)
{
	switch (preemph & DP_TRAIN_PRE_EMPHASIS_MASK) {
	case DP_TRAIN_PRE_EMPH_LEVEL_0:
		return DP_TRAIN_VOLTAGE_SWING_LEVEL_3;
	case DP_TRAIN_PRE_EMPH_LEVEL_1:
		return DP_TRAIN_VOLTAGE_SWING_LEVEL_2;
	case DP_TRAIN_PRE_EMPH_LEVEL_2:
		return DP_TRAIN_VOLTAGE_SWING_LEVEL_1;
	case DP_TRAIN_PRE_EMPH_LEVEL_3:
	default:
		return DP_TRAIN_VOLTAGE_SWING_LEVEL_0;
	}
}

static u8 intel_ddi_dp_voltage_max(struct intel_display *display)
{
	
	int n_entries;
	const struct intel_ddi_buf_trans *trans;
	struct intel_crtc_state *crtc_state=&display->crtc_state0;
	trans=tgl_get_combo_buf_trans(crtc_state,&n_entries);

	return index_to_dp_signal_levels[n_entries - 1] &
		DP_TRAIN_VOLTAGE_SWING_MASK;
}

static u8 intel_dp_phy_voltage_max(struct intel_dp *intel_dp,
				   enum drm_dp_phy dp_phy)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	u8 voltage_max;


	//if (intel_dp_phy_is_downstream_of_source(intel_dp, dp_phy))
		voltage_max = intel_ddi_dp_voltage_max(display);
	//else
	//	voltage_max = intel_dp_lttpr_voltage_max(intel_dp, dp_phy + 1);


	return voltage_max;
}

static u8 intel_dp_get_lane_adjust_vswing_preemph(struct intel_dp *intel_dp, enum drm_dp_phy dp_phy,
						  const u8 link_status[DP_LINK_STATUS_SIZE],
						  int lane)
{
	u8 v = 0;
	u8 p = 0;
	u8 voltage_max;
	u8 preemph_max;

	if (1/*has_per_lane_signal_levels(intel_dp, dp_phy)*/) {
		lane = min(lane, NBlue::callback->i915b->display->panel.vbt.edp.lanes - 1);

		v = drm_dp_get_adjust_request_voltage(link_status, lane);
		p = drm_dp_get_adjust_request_pre_emphasis(link_status, lane);
	} /*else {
		for (lane = 0; lane < crtc_state->lane_count; lane++) {
			v = max(v, drm_dp_get_adjust_request_voltage(link_status, lane));
			p = max(p, drm_dp_get_adjust_request_pre_emphasis(link_status, lane));
		}
	}*/

	preemph_max = intel_dp_phy_preemph_max(intel_dp, dp_phy);
	if (p >= preemph_max)
		p = preemph_max | DP_TRAIN_MAX_PRE_EMPHASIS_REACHED;

	v = min(v, dp_voltage_max(p));

	voltage_max = intel_dp_phy_voltage_max(intel_dp, dp_phy);
	if (v >= voltage_max)
		v = voltage_max | DP_TRAIN_MAX_SWING_REACHED;

	return v | p;
}

static u8 intel_dp_get_lane_adjust_train(struct intel_dp *intel_dp, enum drm_dp_phy dp_phy,
					 const u8 link_status[DP_LINK_STATUS_SIZE],
					 int lane)
{
	/*if (intel_dp_is_uhbr(crtc_state))
		return intel_dp_get_lane_adjust_tx_ffe_preset(intel_dp, crtc_state,
								  dp_phy, link_status, lane);
	else*/
		return intel_dp_get_lane_adjust_vswing_preemph( intel_dp, dp_phy, link_status, lane);
}

bool
intel_dp_get_adjust_train(struct intel_dp *intel_dp,enum drm_dp_phy dp_phy,
			  const u8 link_status[DP_LINK_STATUS_SIZE])
{
	bool changed = false;
	int lane;


	for (lane = 0; lane < 4; lane++) {
		u8 new2 = intel_dp_get_lane_adjust_train(intel_dp, dp_phy, link_status, lane);
		if (intel_dp->train_set[lane] == new2)
			continue;

		intel_dp->train_set[lane] = new2;
		changed = true;
	}

	return changed;
}

static bool intel_dp_adjust_request_changed(struct intel_dp *intel_dp, const u8 old_link_status[DP_LINK_STATUS_SIZE],
						const u8 new_link_status[DP_LINK_STATUS_SIZE])
{
	int lane;

	for (lane = 0; lane < NBlue::callback->i915b->display->panel.vbt.edp.lanes; lane++) {
		u8 old, new2;
		u8 n1, n2;

		/*if (intel_dp_is_uhbr(crtc_state)) {
			old = drm_dp_get_adjust_tx_ffe_preset(old_link_status, lane);
			new = drm_dp_get_adjust_tx_ffe_preset(new_link_status, lane);
		} else {*/
			old = drm_dp_get_adjust_request_voltage(old_link_status, lane) |
				drm_dp_get_adjust_request_pre_emphasis(old_link_status, lane);
		
		n1 = drm_dp_get_adjust_request_voltage(new_link_status, lane);
		n2=	drm_dp_get_adjust_request_pre_emphasis(new_link_status, lane);
		
			new2 = n1 | n2;
		//}
		
		if (intel_dp->para != nullptr) {
			intel_dp->para->voltageSwing = n1;
			intel_dp->para->preEmphasis = n2;
		}
		
		intel_dp->para0.  voltage=n1;
		intel_dp->para0.  preEmphasis=n2;

		if (old != new2)
			return true;
	}

	return false;
}

static bool intel_dp_lane_max_vswing_reached(u8 train_set_lane)
{
	u8 v = (train_set_lane & DP_TRAIN_VOLTAGE_SWING_MASK) >>
		DP_TRAIN_VOLTAGE_SWING_SHIFT;
	u8 p = (train_set_lane & DP_TRAIN_PRE_EMPHASIS_MASK) >>
		DP_TRAIN_PRE_EMPHASIS_SHIFT;

	if ((train_set_lane & DP_TRAIN_MAX_SWING_REACHED) == 0)
		return false;

	if (v + p != 3)
		return false;

	return true;
}

static bool intel_dp_link_max_vswing_reached(struct intel_dp *intel_dp)
{
	int lane;

	for (lane = 0; lane < NBlue::callback->i915b->display->panel.vbt.edp.lanes; lane++) {
		u8 train_set_lane = intel_dp->train_set[lane];

		/*if (intel_dp_is_uhbr(crtc_state)) {
			if (!intel_dp_lane_max_tx_ffe_reached(train_set_lane))
				return false;
		} else {*/
			if (!intel_dp_lane_max_vswing_reached(train_set_lane))
				return false;
		//}
	}

	return true;
}



static bool
intel_dp_update_link_train(struct intel_dp *intel_dp,
			   enum drm_dp_phy dp_phy)
{
	int reg = dp_phy == DP_PHY_DPRX ?
				DP_TRAINING_LANE0_SET :
				DP_TRAINING_LANE0_SET_PHY_REPEATER(dp_phy);
	int ret;

	icl_combo_phy_set_signal_levels(NBlue::callback->i915b->display);
	
	ret=Gen11::callback->writeAUX(linkp,reg,intel_dp->train_set, NBlue::callback->i915b->display->panel.vbt.edp.lanes);
	

	return ret >= 0;
}

static inline void fsleep(unsigned long usecs)
{
	if (usecs <= 10)
		IODelay(usecs);
	else if (usecs < USLEEP_RANGE_UPPER_BOUND)
		IOPause((uint64_t)usecs * NSEC_PER_USEC);
	else
		IOSleep((usecs + 999) / 1000);
}



static int __8b10b_clock_recovery_delay_us(u8 rd_interval)
{
	if (rd_interval == 0)
		return 100;
	return rd_interval * 4 * USEC_PER_MSEC;
}
static int __8b10b_channel_eq_delay_us(u8 rd_interval)
{
	if (rd_interval == 0)
		return 400;
	return rd_interval * 4 * USEC_PER_MSEC;
}
static int __read_delay(const u8 dpcd[DP_RECEIVER_CAP_SIZE],
			enum drm_dp_phy dp_phy, bool uhbr, bool cr)
{
	unsigned int offset;
	u8 rd_interval, mask;
	
	if (dp_phy == DP_PHY_DPRX) {
		if (0) {
		} else {
			if (cr && dpcd[DP_DPCD_REV] >= DP_DPCD_REV_14)
				return 100;

			offset = DP_TRAINING_AUX_RD_INTERVAL;
			mask = DP_TRAINING_AUX_RD_MASK;

		}
	}

	if (offset < DP_RECEIVER_CAP_SIZE) {
		rd_interval = dpcd[offset];
	} else {
		
		if (Gen11::callback->readAUX(linkp,offset,&rd_interval,1) < 0) {
			return 400;
		}
	}
	
	if (cr)
		return __8b10b_clock_recovery_delay_us( rd_interval & mask);
	else
		return __8b10b_channel_eq_delay_us( rd_interval & mask);

}
int drm_dp_read_clock_recovery_delay(struct drm_dp_aux *aux, const u8 dpcd[DP_RECEIVER_CAP_SIZE],
					 enum drm_dp_phy dp_phy, bool uhbr)
{
	return __read_delay( dpcd, dp_phy, uhbr, true);
}

int drm_dp_read_channel_eq_delay(struct drm_dp_aux *aux, const u8 dpcd[DP_RECEIVER_CAP_SIZE],
				 enum drm_dp_phy dp_phy, bool uhbr)
{
	return __read_delay( dpcd, dp_phy, uhbr, false);
}

static bool
intel_dp_link_training_clock_recovery(struct intel_dp *intel_dp,enum drm_dp_phy dp_phy)
{
	u8 old_link_status[DP_LINK_STATUS_SIZE] = {};
	int voltage_tries, cr_tries, max_cr_tries;
	u8 link_status[DP_LINK_STATUS_SIZE];
	bool max_vswing_reached = false;
	int delay_us;
	struct intel_display *display = NBlue::callback->i915b->display;
	
	delay_us = drm_dp_read_clock_recovery_delay(nullptr, intel_dp->dpcd, DP_PHY_DPRX,false);

	if (intel_dp->para != nullptr) {
		intel_dp->para->CR = (u16)delay_us;
	}
	
	intel_dp->para0. t1Time=(u16)delay_us;
	
	if (!intel_dp_reset_link_train( intel_dp,dp_phy,
					   DP_TRAINING_PATTERN_1 |
					   DP_LINK_SCRAMBLING_DISABLE)) {
		return false;
	}

	if (intel_dp->dpcd[DP_DPCD_REV] >= DP_DPCD_REV_14)
		max_cr_tries = 10;
	else
		max_cr_tries = 80;

	voltage_tries = 1;
	for (cr_tries = 0; cr_tries < max_cr_tries; ++cr_tries) {
		fsleep(delay_us);
		
		int r=Gen11::callback->readAUX(linkp,DP_LANE0_1_STATUS,link_status,DP_LINK_STATUS_SIZE);
		if (r < 0) {
			return false;
		}

		if (drm_dp_clock_recovery_ok(link_status, display->panel.vbt.edp.lanes)) {
			return true;
		}

		if (voltage_tries == 5) {
			//intel_dp_dump_link_status(intel_dp, dp_phy, link_status);
			return false;
		}

		if (max_vswing_reached) {
			//intel_dp_dump_link_status(intel_dp, dp_phy, link_status);
			return false;
		}

		intel_dp_get_adjust_train(intel_dp, dp_phy,link_status);
		if (!intel_dp_update_link_train(intel_dp, dp_phy)) {
			return false;
		}

		if (!intel_dp_adjust_request_changed(intel_dp,  old_link_status, link_status))
			++voltage_tries;
		else
			voltage_tries = 1;

		memcpy(old_link_status, link_status, sizeof(link_status));

		if (intel_dp_link_max_vswing_reached(intel_dp))
			max_vswing_reached = true;
	}

	//intel_dp_dump_link_status(intel_dp, dp_phy, link_status);


	return false;
}


static bool drm_dp_channel_eq_ok(const u8 link_status[DP_LINK_STATUS_SIZE],
			  int lane_count)
{
	u8 lane_align;
	u8 lane_status;
	int lane;

	lane_align = dp_link_status(link_status,
					DP_LANE_ALIGN_STATUS_UPDATED);
	if ((lane_align & DP_INTERLANE_ALIGN_DONE) == 0)
		return false;
	for (lane = 0; lane < lane_count; lane++) {
		lane_status = dp_get_lane_status(link_status, lane);
		if ((lane_status & DP_CHANNEL_EQ_BITS) != DP_CHANNEL_EQ_BITS)
			return false;
	}
	return true;
}

bool intel_dp_source_supports_tps4(struct intel_display *display)
{
	return DISPLAY_VER(display) >= 10;
}
static inline bool
drm_dp_tps4_supported(const u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	return dpcd[DP_DPCD_REV] >= 0x14 &&
		dpcd[DP_MAX_DOWNSPREAD] & DP_TPS4_SUPPORTED;
}
bool intel_dp_source_supports_tps3(struct intel_display *display)
{
	return DISPLAY_VER(display) >= 9 ||
		display->platform.broadwell || display->platform.haswell;
}
static inline bool
drm_dp_tps3_supported(const u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	return dpcd[DP_DPCD_REV] >= 0x12 &&
		dpcd[DP_MAX_LANE_COUNT] & DP_TPS3_SUPPORTED;
}
static u32 intel_dp_training_pattern(struct intel_dp *intel_dp,
					 const struct intel_crtc_state *crtc_state,
					 enum drm_dp_phy dp_phy)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	bool source_tps3, sink_tps3, source_tps4, sink_tps4;

	if (intel_dp_is_uhbr(crtc_state))
		return DP_TRAINING_PATTERN_2;


	source_tps4 = intel_dp_source_supports_tps4(display);
	sink_tps4 = dp_phy != DP_PHY_DPRX ||
			drm_dp_tps4_supported(intel_dp->dpcd);
	if (source_tps4 && sink_tps4) {
		return DP_TRAINING_PATTERN_4;
	} else if (crtc_state->port_clock == 810000) {

	}


	source_tps3 = intel_dp_source_supports_tps3(display);
	sink_tps3 = dp_phy != DP_PHY_DPRX ||
			drm_dp_tps3_supported(intel_dp->dpcd);
	if (source_tps3 && sink_tps3) {
		return  DP_TRAINING_PATTERN_3;
	} else if (crtc_state->port_clock >= 540000) {

	}

	return DP_TRAINING_PATTERN_2;
}

static bool
intel_dp_link_training_channel_equalization(struct intel_dp *intel_dp,enum drm_dp_phy dp_phy)
{
	int tries;
	u32 training_pattern;
	u8 link_status[DP_LINK_STATUS_SIZE];
	bool channel_eq = false;
	int delay_us;
	struct intel_display *display = NBlue::callback->i915b->display;
	const struct intel_crtc_state *crtc_state=&display->crtc_state0;
	
	delay_us = drm_dp_read_channel_eq_delay(nullptr, intel_dp->dpcd, DP_PHY_DPRX,false);;
	
	if (intel_dp->para != nullptr) {
		intel_dp->para->EQ = (u16)delay_us;
	}
	intel_dp->para0. t2Time=(u16)delay_us;
	
	training_pattern = intel_dp_training_pattern(intel_dp, crtc_state, dp_phy);

	if (training_pattern != DP_TRAINING_PATTERN_4)
		training_pattern |= DP_LINK_SCRAMBLING_DISABLE;


	if (!intel_dp_set_link_train(intel_dp,  dp_phy,
					 training_pattern)) {
		return false;
	}
	

	for (tries = 0; tries < 5; tries++) {
		fsleep(delay_us);

		int r=Gen11::callback->readAUX(linkp,DP_LANE0_1_STATUS,link_status,DP_LINK_STATUS_SIZE);
		
		if (r < 0) {
			break;
		}


		if (!drm_dp_clock_recovery_ok(link_status,
									  display->panel.vbt.edp.lanes)) {
			//intel_dp_dump_link_status(intel_dp, dp_phy, link_status);
			break;
		}

		if (drm_dp_channel_eq_ok(link_status,
								 display->panel.vbt.edp.lanes)) {
			channel_eq = true;
			break;
		}


		intel_dp_get_adjust_train(intel_dp,  dp_phy,link_status);
		if (!intel_dp_update_link_train(intel_dp,  dp_phy)) {
			break;
		}
	}


	if (tries == 5) {
		//intel_dp_dump_link_status(intel_dp, dp_phy, link_status);
	}

	return channel_eq;
}

static bool
intel_dp_link_train_phy(struct intel_dp *intel_dp,enum drm_dp_phy dp_phy)
{
	bool ret = false;

	if (!intel_dp_link_training_clock_recovery( intel_dp,dp_phy))
	{
		if (intel_dp->para != nullptr) 	intel_dp->para->status = 3;
		goto out;
	}
	if (!intel_dp_link_training_channel_equalization( intel_dp,dp_phy))
	{
		if (intel_dp->para != nullptr) 	intel_dp->para->status = 5;
		goto out;
		
	}

	ret = true;

out:
	
	
	return ret;
}

static bool intel_dp_disable_dpcd_training_pattern(struct intel_dp *intel_dp,
						   enum drm_dp_phy dp_phy)
{
	int reg = intel_dp_training_pattern_set_reg(intel_dp, dp_phy);
	u8 val = DP_TRAINING_PATTERN_DISABLE;

	auto ret=Gen11::callback->writeAUX(linkp,reg,&val, 1);
	
	return ret >= 0;
}

u8 drm_dp_link_rate_to_bw_code(int link_rate)
{
	switch (link_rate) {
	case 1000000:
		return DP_LINK_BW_10;
	case 1350000:
		return DP_LINK_BW_13_5;
	case 2000000:
		return DP_LINK_BW_20;
	default:
		/* Spec says link_bw = link_rate / 0.27Gbps */
		return link_rate / 27000;
	}
}

static void intel_ddi_init_dp_buf_reg(struct intel_display *display, struct intel_dp *intel_dp)
{
	

	intel_dp->DP = DDI_PORT_WIDTH(display->panel.vbt.edp.lanes) |
		DDI_BUF_TRANS_SELECT(0);

	if (display->child0->lane_reversal)
		intel_dp->DP |= DDI_BUF_PORT_REVERSAL;
	
/*	if (dig_port->ddi_a_4_lanes)
		intel_dp->DP |= DDI_A_4_LANES;

	if (DISPLAY_VER(display) >= 14) {
		if (intel_dp_is_uhbr(crtc_state))
			intel_dp->DP |= DDI_BUF_PORT_DATA_40BIT;
		else
			intel_dp->DP |= DDI_BUF_PORT_DATA_10BIT;
	}


	if (IS_DISPLAY_VER(display, 11, 13) && intel_encoder_is_tc(encoder)) {
		int delay = dp_phy_lane_stagger_delay(crtc_state->port_clock);

		intel_dp->DP |= DDI_BUF_LANE_STAGGER_DELAY(delay);
	}*/
	
}




static u32 intel_ddi_buf_status_reg(struct intel_display *display, enum port port)
{
	if (DISPLAY_VER(display) >= 14)
		return 0;//XELPDP_PORT_BUF_CTL1(display, port);
	else
		return DDI_BUF_CTL(port);
}
static void intel_wait_ddi_buf_active()
{
	struct intel_display *display = NBlue::callback->i915b->display;
	enum port port = display->port0;


	intel_de_wait_for_clear_ms(display, intel_ddi_buf_status_reg(display, port),
								  DDI_BUF_IS_IDLE, 10);
}

static void intel_ddi_buf_enable( u32 buf_ctl)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	enum port port = display->port0;

		intel_de_write(display, DDI_BUF_CTL(port), buf_ctl | DDI_BUF_CTL_ENABLE);
	intel_de_posting_read(display, DDI_BUF_CTL(port));

	intel_wait_ddi_buf_active();
}



static void intel_ddi_prepare_link_retrain(struct intel_display *display, struct intel_dp *intel_dp)
{
	struct intel_crtc_state *crtc_state = &display->crtc_state0;
	u32 dp_tp_ctl;

	
	//dp_tp_ctl = intel_de_read(display, dp_tp_ctl_reg( ));
	
	dp_tp_ctl = DP_TP_CTL_ENABLE | DP_TP_CTL_LINK_TRAIN_PAT1;
	
	if (intel_crtc_has_type(crtc_state, INTEL_OUTPUT_DP_MST) ||
		intel_dp_is_uhbr(crtc_state)) {
		dp_tp_ctl |= DP_TP_CTL_MODE_MST;
	} else {
		dp_tp_ctl |= DP_TP_CTL_MODE_SST;
		if (crtc_state->enhanced_framing)
			dp_tp_ctl |= DP_TP_CTL_ENHANCED_FRAME_ENABLE;
	}

	intel_de_write(display, dp_tp_ctl_reg(), dp_tp_ctl);
	intel_de_posting_read(display, dp_tp_ctl_reg());

	/*if (display->platform.alderlake_p &&
		(intel_tc_port_in_dp_alt_mode(dig_port) || intel_tc_port_in_legacy_mode(dig_port)))
		adlp_tbt_to_dp_alt_switch_wa(encoder);*/

	intel_ddi_buf_enable( intel_dp->DP);
	intel_dp->DP |= DDI_BUF_CTL_ENABLE;
}

static void _icl_ddi_enable_clock(struct intel_display *display, u32 reg,
				  u32 clk_sel_mask, u32 clk_sel, u32 clk_off)
{
	
	IOSimpleLock *myLock;
	myLock = IOSimpleLockAlloc();
	IOSimpleLockLock(myLock);

	intel_de_rmw(display, reg, clk_sel_mask, clk_sel);
	intel_de_rmw(display, reg, clk_off, 0);

	IOSimpleLockUnlock(myLock);
	IOSimpleLockFree(myLock);
}

void intel_combo_phy_power_up_lanes(struct intel_display *display,
					enum phy phy, bool is_dsi,
					int lane_count, bool lane_reversal)
{
	u8 lane_mask;

	if (is_dsi) {

		switch (lane_count) {
		/*case 1:
			lane_mask = PWR_DOWN_LN_3_1_0;
			break;
		case 2:
			lane_mask = PWR_DOWN_LN_3_1;
			break;
		case 3:
			lane_mask = PWR_DOWN_LN_3;
			break;*/
		default:
		case 4:
			lane_mask = PWR_UP_ALL_LANES;
			break;
		}
	} else {
		switch (lane_count) {
		/*case 1:
			lane_mask = lane_reversal ? PWR_DOWN_LN_2_1_0 :
							PWR_DOWN_LN_3_2_1;
			break;*/
		case 2:
			lane_mask = lane_reversal ? PWR_DOWN_LN_1_0 :
							PWR_DOWN_LN_3_2;
			break;
		default:
		case 4:
			lane_mask = PWR_UP_ALL_LANES;
			break;
		}
	}

	intel_de_rmw(display, ICL_PORT_CL_DW10(phy),
			 PWR_DOWN_LN_MASK, lane_mask);
}

int drm_dp_pcon_convert_rgb_to_ycbcr(u8 color_spc)
{
	int ret;
	u8 buf;

	ret = Gen11::callback->readAUX(linkp,DP_PROTOCOL_CONVERTER_CONTROL_2,&buf,1);
	if (ret < 0)
		return ret;

	if (color_spc & DP_CONVERSION_RGB_YCBCR_MASK)
		buf |= (color_spc & DP_CONVERSION_RGB_YCBCR_MASK);
	else
		buf &= ~DP_CONVERSION_RGB_YCBCR_MASK;

	return Gen11::callback->writeAUX(linkp,DP_PROTOCOL_CONVERTER_CONTROL_2, &buf,1);
}

void intel_dp_configure_protocol_converter(struct intel_display *display, struct intel_dp *intel_dp)
{
	struct intel_crtc_state *crtc_state=&display->crtc_state0;
	
	bool ycbcr444_to_420 = false;
	bool rgb_to_ycbcr = false;
	u8 tmp;
	
	if (intel_dp->dpcd[DP_DPCD_REV] < 0x13)
		return;

	//tmp = intel_dp_has_hdmi_sink(intel_dp) ? DP_HDMI_DVI_OUTPUT_CONFIG : 0;
	tmp = 0;
	
	Gen11::callback->writeAUX(linkp,DP_PROTOCOL_CONVERTER_CONTROL_0, &tmp,1);


	if (crtc_state->sink_format == INTEL_OUTPUT_FORMAT_YCBCR420) {
		switch (crtc_state->output_format) {
		case INTEL_OUTPUT_FORMAT_YCBCR420:
			break;
		case INTEL_OUTPUT_FORMAT_YCBCR444:
			ycbcr444_to_420 = true;
			break;
		case INTEL_OUTPUT_FORMAT_RGB:
			rgb_to_ycbcr = true;
			ycbcr444_to_420 = true;
			break;
		default:
			break;
		}
	} else if (crtc_state->sink_format == INTEL_OUTPUT_FORMAT_YCBCR444) {
		switch (crtc_state->output_format) {
		case INTEL_OUTPUT_FORMAT_YCBCR444:
			break;
		case INTEL_OUTPUT_FORMAT_RGB:
			rgb_to_ycbcr = true;
			break;
		default:
			break;
		}
	}

	tmp = ycbcr444_to_420 ? DP_CONVERSION_TO_YCBCR420_ENABLE : 0;
	Gen11::callback->writeAUX(linkp,DP_PROTOCOL_CONVERTER_CONTROL_1, &tmp,1);


	tmp = rgb_to_ycbcr ? DP_CONVERSION_BT709_RGB_YCBCR_ENABLE : 0;
	
	drm_dp_pcon_convert_rgb_to_ycbcr(tmp);


}



static int
write_dsc_decompression_flag( u8 flag, bool set)
{
	int err;
	u8 val;

	err = Gen11::callback->readAUX(linkp,DP_DSC_ENABLE,&val,1);
	if (err < 0)
		return err;

	if (set)
		val |= flag;
	else
		val &= ~flag;
	
	return Gen11::callback->writeAUX(linkp,DP_DSC_ENABLE,&val,1);
}



static u32 dp_tp_status_reg()
{
	struct intel_display *display = NBlue::callback->i915b->display;
	struct intel_crtc_state *crtc_state = &display->crtc_state0;
	
	if (DISPLAY_VER(display) >= 12)
		return TGL_DP_TP_STATUS(display,
								crtc_state->cpu_transcoder);
	else
		return DP_TP_STATUS(display->port0);
}


static void intel_ddi_set_idle_link_train(struct intel_dp *intel_dp)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	enum port port = display->port0;

	intel_de_rmw(display, dp_tp_ctl_reg(),
			 DP_TP_CTL_LINK_TRAIN_MASK, DP_TP_CTL_LINK_TRAIN_IDLE);

	if (port == PORT_A && DISPLAY_VER(display) < 12)
		return;

	intel_de_wait_for_set_ms(display,
					 dp_tp_status_reg( ),
							 DP_TP_STATUS_IDLE_DONE, 2);

}

static bool intel_dp_link_train_all_phys(struct intel_dp *intel_dp)
{
	bool ret;

	ret = intel_dp_link_train_phy(intel_dp, DP_PHY_DPRX);

	intel_dp_disable_dpcd_training_pattern(intel_dp, DP_PHY_DPRX);
	intel_ddi_set_idle_link_train(intel_dp);

	return ret;
}

int intel_dp_rate_index(int *rates, int len, int rate)
{
	int i;

	for (i = 0; i < len; i++)
		if (rate == rates[i])
			return i;

	return -1;
}
int intel_dp_rate_select(struct intel_dp *intel_dp, int rate)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	int i = intel_dp_rate_index(intel_dp->sink_rates,
					intel_dp->num_sink_rates, rate);

	if (i < 0)
		i = 0;

	return i;
}
void intel_dp_compute_rate(struct intel_dp *intel_dp, int port_clock,
			   u8 *link_bw, u8 *rate_select)
{
	struct intel_display *display = NBlue::callback->i915b->display;

	/* eDP 1.4 rate select method. */
	if (intel_dp->use_rate_select) {
		*link_bw = 0;
		*rate_select =
			intel_dp_rate_select(intel_dp, port_clock);
	} else {
		*link_bw = drm_dp_link_rate_to_bw_code(port_clock);
		*rate_select = 0;
	}
}

void intel_dp_link_training_set_mode(struct intel_dp *intel_dp, int link_rate, bool is_vrr)
{
	u8 link_config[2];

	link_config[0] = is_vrr ? DP_MSA_TIMING_PAR_IGNORE_EN : 0;
	link_config[1] = drm_dp_is_uhbr_rate(link_rate) ?
			 DP_SET_ANSI_128B132B : DP_SET_ANSI_8B10B;

	Gen11::callback->writeAUX(linkp,DP_DOWNSPREAD_CTRL,link_config, 2);
}

static void intel_dp_update_downspread_ctrl(struct intel_dp *intel_dp)
{
	struct intel_display *display=NBlue::callback->i915b->display;
	struct intel_crtc_state *crtc_state=&display->crtc_state0;
	intel_dp_link_training_set_mode(intel_dp,
									display->port_clock, crtc_state->vrr.in_range);
}

void intel_dp_link_training_set_bw(struct intel_dp *intel_dp,
				   int link_bw, int rate_select, int lane_count,
				   bool enhanced_framing)
{
	if (enhanced_framing)
		lane_count |= DP_LANE_COUNT_ENHANCED_FRAME_EN;

	if (link_bw) {
		u8 link_config[2];
		link_config[0]=link_bw;
		link_config[1]=lane_count;
		
		Gen11::callback->writeAUX(linkp,DP_LINK_BW_SET,link_config, ARRAY_SIZE(link_config));

	} else {
		Gen11::callback->writeAUX(linkp,DP_LANE_COUNT_SET,&lane_count, 1);
		Gen11::callback->writeAUX(linkp,DP_LINK_RATE_SET,&rate_select, 1);
	}
}

static void intel_dp_update_link_bw_set(struct intel_dp *intel_dp,
					u8 link_bw, u8 rate_select)
{
	struct intel_display *display=NBlue::callback->i915b->display;
	struct intel_crtc_state *crtc_state=&display->crtc_state0;
	
	intel_dp_link_training_set_bw(intel_dp, link_bw, rate_select, display->panel.vbt.edp.lanes,
								  crtc_state->enhanced_framing);
}

static bool
intel_dp_prepare_link_train(struct intel_display *display, struct intel_dp *intel_dp)
{
	u8 link_bw, rate_select;
	
	intel_ddi_prepare_link_retrain(display, intel_dp);
	intel_dp_compute_rate(intel_dp, display->port_clock,&link_bw, &rate_select);

	if (!link_bw) {
		u16 sink_rates[DP_MAX_SUPPORTED_RATES];
		Gen11::callback->readAUX(linkp,DP_SUPPORTED_LINK_RATES,sink_rates, sizeof(sink_rates));
	}

	intel_dp_update_downspread_ctrl(intel_dp);
	intel_dp_update_link_bw_set(intel_dp, link_bw,
					rate_select);

	return true;
}

void intel_dp_stop_link_train(struct intel_display *display, struct intel_dp *intel_dp)
{
	int ret;

	intel_dp->link.active = true;

	intel_dp_program_link_training_pattern(intel_dp, DP_PHY_DPRX,
						   DP_TRAINING_PATTERN_DISABLE);


	//intel_hpd_unblock(encoder);

	/*if (!display->hotplug.ignore_long_hpd &&
		intel_dp->link.seq_train_failures < MAX_SEQ_TRAIN_FAILURES) {
		int delay_ms = intel_dp->link.seq_train_failures ? 0 : 2000;

		intel_encoder_link_check_queue_work(encoder, delay_ms);
	}*/
}

static inline bool
drm_dp_is_branch(const u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	return dpcd[DP_DOWNSTREAMPORT_PRESENT] & DP_DWN_STRM_PORT_PRESENT;
}

static void
intel_dp_init_source_oui(struct intel_dp *intel_dp)
{
	u8 oui[] = { 0x00, 0xaa, 0x01 };
	u8 buf[3] = {};

	if (intel_dp->oui_valid) return;
	
	int r=Gen11::callback->readAUX(linkp,DP_SOURCE_OUI,buf, sizeof(buf));
	if (r<0) return;
	intel_dp->oui_valid=true;
	if (memcmp(oui, buf, sizeof(oui)) == 0) {
		return;
	}

	Gen11::callback->writeAUX(linkp,DP_SOURCE_OUI,oui,sizeof(oui));
}

static bool downstream_hpd_needs_d0(struct intel_dp *intel_dp)
{

	return intel_dp->dpcd[DP_DPCD_REV] == 0x11 &&
		drm_dp_is_branch(intel_dp->dpcd) &&
		intel_dp->downstream_ports[0] & DP_DS_PORT_HPD;
}

void intel_dp_set_power(struct intel_dp *intel_dp, u8 mode)
{
	int ret, i;

	if (intel_dp->dpcd[DP_DPCD_REV] < 0x11)
		return;
	
	if (mode != DP_SET_POWER_D0) {
		if (downstream_hpd_needs_d0(intel_dp))
			return;

		Gen11::callback->writeAUX(linkp,DP_SET_POWER,&mode,1);

	} else {

		intel_dp_init_source_oui(intel_dp);
		
		for (i = 0; i < 3; i++) {
			ret=Gen11::callback->writeAUX(linkp,DP_SET_POWER,&mode,1);
			if (ret == 1)
				break;
			IOSleep(1);
		}

	}

}



void
intel_ddi_config_transcoder_func(struct intel_crtc_state *crtc_state)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	enum transcoder cpu_transcoder = crtc_state->cpu_transcoder;
	u32 ctl;

	//intel_ddi_config_transcoder_dp2(crtc_state, true);

	ctl = intel_ddi_transcoder_func_reg_val_get();
	ctl &= ~TRANS_DDI_FUNC_ENABLE;
	intel_de_write(display, TRANS_DDI_FUNC_CTL(display, cpu_transcoder),
			   ctl);
}


static void intel_ddi_mso_configure(struct intel_display *display, struct intel_crtc_state *crtc_state)
{
	enum pipe pipe = display->pipe0;
	struct intel_dp *intel_dp=&display->intel_dp0;
	
	u32 dss1 = 0;

	if (!HAS_MSO(display))
		return;
	
	
	if (crtc_state->splitter.enable) {
		dss1 |= SPLITTER_ENABLE;
		dss1 |= OVERLAP_PIXELS(crtc_state->splitter.pixel_overlap);
		if (crtc_state->splitter.link_count == 2)
			dss1 |= SPLITTER_CONFIGURATION_2_SEGMENT;
		else
			dss1 |= SPLITTER_CONFIGURATION_4_SEGMENT;
	}

	intel_de_rmw(display, ICL_PIPE_DSS_CTL1(pipe),
			 SPLITTER_ENABLE | SPLITTER_CONFIGURATION_MASK |
			 OVERLAP_PIXELS_MASK, dss1);
}


void intel_dp_sink_enable_decompression(struct intel_display *display)
{

	//if (!new_crtc_state->dsc.compression_enable)
		return;

	/*if (drm_WARN_ON(display->drm,
			!connector->dp.dsc_decompression_aux ||
			connector->dp.dsc_decompression_enabled))
		return;

	if (!intel_dp_dsc_aux_get_ref(state, connector))
		return;*/

	u32 dss_ctl2;
	dss_ctl2 = intel_de_read(display, ICL_PIPE_DSS_CTL2(display->pipe0));

	if (dss_ctl2 & VDSC0_ENABLE) {
		write_dsc_decompression_flag(DP_DSC_PASSTHROUGH_EN, true);
		write_dsc_decompression_flag( DP_DECOMPRESSION_EN, true);
	}
}

static int drm_dp_read_extended_dpcd_caps(struct intel_dp *intel_dp,
					  u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	u8 dpcd_ext[DP_RECEIVER_CAP_SIZE];
	int ret;


	if (!(dpcd[DP_TRAINING_AUX_RD_INTERVAL] &
		  DP_EXTENDED_RECEIVER_CAP_FIELD_PRESENT))
		return 0;

	ret = Gen11::callback->readAUX(linkp, DP_DP13_DPCD_REV, &dpcd_ext,
					sizeof(dpcd_ext));
	if (ret < 0)
		return ret;

	if (dpcd[DP_DPCD_REV] > dpcd_ext[DP_DPCD_REV]) {
		return 0;
	}

	if (!memcmp(dpcd, dpcd_ext, sizeof(dpcd_ext)))
		return 0;


	memcpy(dpcd, dpcd_ext, sizeof(dpcd_ext));

	return 0;
}

int drm_dp_read_dpcd_caps(struct intel_dp *intel_dp,
			  u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	int ret;

	ret = Gen11::callback->readAUX(linkp, DP_DPCD_REV, dpcd, DP_RECEIVER_CAP_SIZE);
	if (ret < 0)
		return ret;
	if (dpcd[DP_DPCD_REV] == 0)
		return -EIO;

	
	ret = drm_dp_read_extended_dpcd_caps(intel_dp, dpcd);
	if (ret < 0)
		return ret;

	
	
	return ret;
}
static u8 drm_dp_downstream_port_count(const u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	u8 port_count = dpcd[DP_DOWN_STREAM_PORT_COUNT] & DP_PORT_COUNT_MASK;

	if (dpcd[DP_DOWNSTREAMPORT_PRESENT] & DP_DETAILED_CAP_INFO_AVAILABLE && port_count > 4)
		port_count = 4;

	return port_count;
}

enum drm_dp_mst_mode drm_dp_read_mst_cap(struct intel_dp *intel_dp,
					 const u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	u8 mstm_cap;

	if (dpcd[DP_DPCD_REV] < DP_DPCD_REV_12)
		return DRM_DP_SST;

	if (Gen11::callback->readAUX(linkp, DP_MSTM_CAP, &mstm_cap,1) < 0)
		return DRM_DP_SST;

	if (mstm_cap & DP_MST_CAP)
		return DRM_DP_MST;

	if (mstm_cap & DP_SINGLE_STREAM_SIDEBAND_MSG)
		return DRM_DP_SST_SIDEBAND_MSG;

	return DRM_DP_SST;
}

bool intel_dp_mst_source_support(struct intel_dp *intel_dp)
{
	return false;//intel_dp->mst.mgr.cbs;
}

static enum drm_dp_mst_mode intel_dp_mst_mode_choose(struct intel_display *display, struct intel_dp *intel_dp,
			 enum drm_dp_mst_mode sink_mst_mode)
{

	//if (!display->params.enable_dp_mst)
	//	return DRM_DP_SST;

	if (!intel_dp_mst_source_support(intel_dp))
		return DRM_DP_SST;

	if (sink_mst_mode == DRM_DP_SST_SIDEBAND_MSG &&
		!(intel_dp->dpcd[DP_MAIN_LINK_CHANNEL_CODING] & DP_CAP_ANSI_128B132B))
		return DRM_DP_SST;

	return sink_mst_mode;
}

static enum drm_dp_mst_mode
intel_dp_mst_detect(struct intel_display *display, struct intel_dp *intel_dp)
{
	enum drm_dp_mst_mode sink_mst_mode;
	enum drm_dp_mst_mode mst_detect;

	sink_mst_mode = drm_dp_read_mst_cap(intel_dp, intel_dp->dpcd);

	mst_detect = intel_dp_mst_mode_choose(display, intel_dp, sink_mst_mode);

	return mst_detect;
}
int drm_dp_read_downstream_info(struct intel_dp *intel_dp,
				const u8 dpcd[DP_RECEIVER_CAP_SIZE],
				u8 downstream_ports[DP_MAX_DOWNSTREAM_PORTS])
{
	int ret;
	u8 len;

	memset(downstream_ports, 0, DP_MAX_DOWNSTREAM_PORTS);

	if (!drm_dp_is_branch(dpcd) || dpcd[DP_DPCD_REV] == DP_DPCD_REV_10)
		return 0;


	len = drm_dp_downstream_port_count(dpcd);
	if (!len)
		return 0;
	
	intel_dp->para0.downspread=1;

	if (dpcd[DP_DOWNSTREAMPORT_PRESENT] & DP_DETAILED_CAP_INFO_AVAILABLE)
		len *= 4;

	ret = Gen11::callback->readAUX(linkp, DP_DOWNSTREAM_PORT_0, downstream_ports, len);
	if (ret < 0)
		return ret;


	return 0;
}

bool intel_dp_start_link_train(struct intel_display *display, struct intel_dp *intel_dp)
{
	//struct intel_digital_port *dig_port = dp_to_dig_port(intel_dp);
	//struct intel_encoder *encoder = &dig_port->base;
	bool passed;

	int lttpr_count;

	//intel_hpd_block(encoder);
/*
	lttpr_count = intel_dp_init_lttpr_and_dprx_caps(intel_dp);

	if (lttpr_count < 0)
		lttpr_count = 0;
*/
	intel_dp_prepare_link_train(display, intel_dp);

	/*if (intel_dp_is_uhbr(crtc_state))
		passed = intel_dp_128b132b_link_train(intel_dp, crtc_state, lttpr_count);
	else
		*/passed = intel_dp_link_train_all_phys(intel_dp);

	if (intel_dp->link.force_train_failure) {
		intel_dp->link.force_train_failure--;
	} else if (passed) {
		intel_dp->link.seq_train_failures = 0;
		return passed;
	}

	intel_dp->link.seq_train_failures++;

	if (display->hotplug.ignore_long_hpd) {
		return passed;
	}

	if (intel_dp->link.seq_train_failures < 2)
		return passed;
	
	if (intel_dp->hobl_active) {
		intel_dp->hobl_failed = true;
	}


	intel_dp->link.retrain_disabled = true;
	return passed;
}

void intel_dp_set_link_params(struct intel_dp *intel_dp,
				  int link_rate, int lane_count)
{
	memset(intel_dp->train_set, 0, sizeof(intel_dp->train_set));
	intel_dp->link.active = false;
	intel_dp->needs_modeset_retry = false;
	intel_dp->link_rate = link_rate;
	intel_dp->lane_count = lane_count;
}


void intel_ddi_enable_transcoder_clock(struct intel_crtc_state *crtc_state)
{
	struct intel_display *display =NBlue::callback->i915b->display;
	enum transcoder cpu_transcoder = crtc_state->cpu_transcoder;
	enum phy phy = display->phy0;
	u32 val;

	if (cpu_transcoder == TRANSCODER_EDP)
		return;

	if (DISPLAY_VER(display) >= 13)
		val = TGL_TRANS_CLK_SEL_PORT(phy);
	else if (DISPLAY_VER(display) >= 12)
		val = TGL_TRANS_CLK_SEL_PORT(display->port0);
	else
		val = TRANS_CLK_SEL_PORT(display->port0);

	intel_de_write(display, TRANS_CLK_SEL(cpu_transcoder), val);
}

static void intel_dp_enable_port(struct intel_dp *intel_dp,
				 const struct intel_crtc_state *crtc_state)
{
	struct intel_display *display =NBlue::callback->i915b->display;

	intel_dp_program_link_training_pattern(intel_dp,
						   DP_PHY_DPRX, DP_TRAINING_PATTERN_1);

	intel_dp->DP |= DP_PORT_EN;

	intel_de_write(display, intel_dp->output_reg, intel_dp->DP);
	intel_de_posting_read(display, intel_dp->output_reg);
}

bool tgl_ddi_pre_enable_dp(struct intel_display *display, struct intel_crtc_state *crtc_state)
{
	struct intel_dp *intel_dp = &display->intel_dp0;
	bool is_mst = intel_crtc_has_type(crtc_state, INTEL_OUTPUT_DP_MST);
	int ret;

	intel_dp_set_link_params(intel_dp,
				 crtc_state->port_clock,
				 crtc_state->lane_count);


	intel_ddi_init_dp_buf_reg( display, intel_dp);
	
	//intel_dp_enable_port(intel_dp,crtc_state);

	//if (!kexticl) Gen11::callback->enableVDDForAux(ccont2,linkp);
	//else Gen11::callback->enableVDDForAux2(ccont2,linkp);

	if (intel_dp_is_edp())
	Gen11::callback->hwSetPanelPower(ccont2,2);

	//if (!kexticl) Gen11::callback->disableVDDForAux(ccont2);
	//else Gen11::callback->disableVDDForAux2(ccont2,linkp);
	
	
/*
	_icl_ddi_enable_clock(display, ICL_DPCLKA_CFGCR0,
							  ICL_DPCLKA_CFGCR0_DDI_CLK_SEL_MASK(display->phy0),
							  ICL_DPCLKA_CFGCR0_DDI_CLK_SEL(DPLL_ID_ICL_DPLL0, display->phy0),
							  ICL_DPCLKA_CFGCR0_DDI_CLK_OFF(display->phy0));
	*/
	/*if (!intel_tc_port_in_tbt_alt_mode(dig_port)) {
		drm_WARN_ON(display->drm, dig_port->ddi_io_wakeref);
		dig_port->ddi_io_wakeref = intel_display_power_get(display,
								   dig_port->ddi_io_power_domain);
	}*/
	
	//intel_ddi_enable_transcoder_clock(crtc_state);
	
	//intel_ddi_config_transcoder_func(crtc_state);

	icl_combo_phy_set_signal_levels(display);
	
	intel_combo_phy_power_up_lanes(display, display->phy0, false,
					   crtc_state->lane_count,
								   display->child0->lane_reversal);
	

	intel_ddi_mso_configure(display, crtc_state);

	if (!is_mst)
		intel_dp_set_power(intel_dp, DP_SET_POWER_D0);

	intel_dp_configure_protocol_converter(display,intel_dp);
	if (!is_mst)
		intel_dp_sink_enable_decompression(display);

	//intel_dp_sink_set_fec_ready(intel_dp, crtc_state, true);

	//intel_dp_check_frl_training(intel_dp);
	//intel_dp_pcon_dsc_configure(intel_dp, crtc_state);


	ret=intel_dp_start_link_train( display,intel_dp);

	//if (!is_trans_port_sync_mode(crtc_state))
		intel_dp_stop_link_train(display,intel_dp);

	//intel_ddi_enable_fec(encoder, crtc_state);
	return ret;
}

static int tgl_ddi_min_voltage_level(const struct intel_crtc_state *crtc_state)
{
	if (crtc_state->port_clock > 594000)
		return 2;
	else
		return 0;
}

static inline bool
drm_dp_enhanced_frame_cap(const u8 dpcd[DP_RECEIVER_CAP_SIZE])
{
	return dpcd[DP_DPCD_REV] >= 0x11 &&
		(dpcd[9] & DP_ENHANCED_FRAME_CAP);
}

bool intel_dsc_source_support(struct intel_display *display, const struct intel_crtc_state *crtc_state)
{
	enum transcoder cpu_transcoder = crtc_state->cpu_transcoder;

	if (!HAS_DSC(display))
		return false;

	if (DISPLAY_VER(display) == 11 && cpu_transcoder == TRANSCODER_A)
		return false;

	return true;
}
static inline int fxp_q4_from_int(int val_int)
{
	return val_int << 4;
}
int intel_dp_output_format_link_bpp_x16(enum intel_output_format output_format, int pipe_bpp)
{

	if (output_format == INTEL_OUTPUT_FORMAT_YCBCR420)
		pipe_bpp /= 2;

	return fxp_q4_from_int(pipe_bpp);
}
int
intel_dp_compute_config(struct intel_display *display, struct intel_crtc_state *pipe_config)
{
	struct intel_dp *intel_dp=&display->intel_dp0;
	int ret = 0, link_bpp_x16;
	struct drm_display_mode *adjusted_mode = &pipe_config->hw.adjusted_mode;
	struct intel_crtc_state *crtc_state=&display->crtc_state0;
	
	/*fixed_mode = intel_panel_fixed_mode(connector, adjusted_mode);
	if (intel_dp_is_edp(intel_dp) && fixed_mode) {
		ret = intel_panel_compute_config(connector, adjusted_mode);
		if (ret)
			return ret;
	}

	if (adjusted_mode->flags & DRM_MODE_FLAG_DBLSCAN)
		return -EINVAL;

	if (!connector->base.interlace_allowed &&
		adjusted_mode->flags & DRM_MODE_FLAG_INTERLACE)
		return -EINVAL;

	if (adjusted_mode->flags & DRM_MODE_FLAG_DBLCLK)
		return -EINVAL;

	if (intel_dp_hdisplay_bad(display, adjusted_mode->crtc_hdisplay))
		return -EINVAL;
	*/
	
	pipe_config->sink_format =INTEL_OUTPUT_FORMAT_RGB;
	pipe_config->output_format = INTEL_OUTPUT_FORMAT_RGB;
	
	
	/*ret = intel_dp_compute_output_format(encoder, pipe_config, conn_state, true);
	if (ret)
		ret = intel_dp_compute_output_format(encoder, pipe_config, conn_state, false);
	if (ret)
		return ret;
	 
	 if ((intel_dp_is_edp() && fixed_mode) ||
		 pipe_config->output_format == INTEL_OUTPUT_FORMAT_YCBCR420) {
		 ret = intel_pfit_compute_config(pipe_config, conn_state);
		 if (ret)
			 return ret;
	 }*/


	pipe_config->limited_color_range = false;
		//intel_dp_limited_color_range(pipe_config, conn_state);


		pipe_config->enhanced_framing =
			drm_dp_enhanced_frame_cap(intel_dp->dpcd);
	

	pipe_config->dsc.compression_enable=intel_dsc_source_support(display, crtc_state);
	
	if (pipe_config->dsc.compression_enable)
		link_bpp_x16 = pipe_config->dsc.compressed_bpp_x16;
	else
		link_bpp_x16 = intel_dp_output_format_link_bpp_x16(pipe_config->output_format,
								   pipe_config->pipe_bpp);

	/*if (intel_dp->mso_link_count) {
		int n = intel_dp->mso_link_count;
		int overlap = intel_dp->mso_pixel_overlap;

		pipe_config->splitter.enable = true;
		pipe_config->splitter.link_count = n;
		pipe_config->splitter.pixel_overlap = overlap;

		adjusted_mode->crtc_hdisplay = adjusted_mode->crtc_hdisplay / n + overlap;
		adjusted_mode->crtc_hblank_start = adjusted_mode->crtc_hblank_start / n + overlap;
		adjusted_mode->crtc_hblank_end = adjusted_mode->crtc_hblank_end / n + overlap;
		adjusted_mode->crtc_hsync_start = adjusted_mode->crtc_hsync_start / n + overlap;
		adjusted_mode->crtc_hsync_end = adjusted_mode->crtc_hsync_end / n + overlap;
		adjusted_mode->crtc_htotal = adjusted_mode->crtc_htotal / n + overlap;
		adjusted_mode->crtc_clock /= n;
	}*/

	//intel_dp_audio_compute_config(encoder, pipe_config, conn_state);

	/*if (!intel_dp_is_uhbr(pipe_config)) {
		intel_link_compute_m_n(link_bpp_x16,
					   pipe_config->lane_count,
					   adjusted_mode->crtc_clock,
					   pipe_config->port_clock,
					   intel_dp_bw_fec_overhead(pipe_config->fec_enable),
					   &pipe_config->dp_m_n);
	}

	ret = intel_dp_compute_min_hblank(pipe_config, conn_state);
	if (ret)
		return ret;

	if (pipe_config->splitter.enable)
		pipe_config->dp_m_n.data_m *= pipe_config->splitter.link_count;*/
	
	crtc_state->vrr.in_range =false;

	//intel_vrr_compute_config(pipe_config, conn_state);
	//intel_dp_compute_as_sdp(intel_dp, pipe_config);
	
	//intel_psr_compute_config(intel_dp, pipe_config, conn_state);
	pipe_config->panel_replay_dsc_support = INTEL_DP_PANEL_REPLAY_DSC_NOT_SUPPORTED;
	pipe_config->has_panel_replay = false;
	pipe_config->has_psr = pipe_config->has_panel_replay ? true : false;
	
	//intel_alpm_lobf_compute_config(intel_dp, pipe_config, conn_state);
	//intel_dp_drrs_compute_config(connector, pipe_config, link_bpp_x16);
	//intel_dp_compute_vsc_sdp(intel_dp, pipe_config, conn_state);
	//intel_dp_compute_hdr_metadata_infoframe_sdp(intel_dp, pipe_config, conn_state);

	return 0;
}

static u8 intel_ddi_splitter_pipe_mask(struct intel_display *display)
{
	if (DISPLAY_VER(display) > 20)
		return ~0;
	else if (display->platform.alderlake_p)
		return BIT(PIPE_A) | BIT(PIPE_B);
	else
		return BIT(PIPE_A);
}

static void intel_ddi_mso_get_config( struct intel_display *display, struct intel_crtc_state *pipe_config)
{
	enum pipe pipe = display->pipe0;
	u32 dss1;

	if (!HAS_MSO(display))
		return;

	dss1 = intel_de_read(display, ICL_PIPE_DSS_CTL1(pipe));

	pipe_config->splitter.enable = dss1 & SPLITTER_ENABLE;
	if (!pipe_config->splitter.enable)
		return;

	if (( !(intel_ddi_splitter_pipe_mask(display) & BIT(pipe)))) {
		pipe_config->splitter.enable = false;
		return;
	}

	switch (dss1 & SPLITTER_CONFIGURATION_MASK) {
	default:
	case SPLITTER_CONFIGURATION_2_SEGMENT:
		pipe_config->splitter.link_count = 2;
		break;
	case SPLITTER_CONFIGURATION_4_SEGMENT:
		pipe_config->splitter.link_count = 4;
		break;
	}

	pipe_config->splitter.pixel_overlap = REG_FIELD_GET(OVERLAP_PIXELS_MASK, dss1);
}

bool
intel_bios_encoder_supports_dp(struct intel_display *display)
{
	return display->child0->device_type & DEVICE_TYPE_DISPLAYPORT_OUTPUT;
}

bool
intel_bios_encoder_supports_edp(struct intel_display *display)
{
	return intel_bios_encoder_supports_dp(display) &&
	display->child0->device_type & DEVICE_TYPE_INTERNAL_CONNECTOR;
}

static void intel_ddi_read_func_ctl_dp_sst(struct intel_display *display, struct intel_crtc_state *crtc_state,
					   u32 ddi_func_ctl)
{
	enum transcoder cpu_transcoder = crtc_state->cpu_transcoder;

	//if (encoder->type == INTEL_OUTPUT_EDP)
	
	if (!crtc_state->output_types)
	{
		if (intel_bios_encoder_supports_edp(display))
			crtc_state->output_types |= BIT(INTEL_OUTPUT_EDP);
		else
			crtc_state->output_types |= BIT(INTEL_OUTPUT_DP);
	}
	//crtc_state->lane_count =
	//	((ddi_func_ctl & DDI_PORT_WIDTH_MASK) >> DDI_PORT_WIDTH_SHIFT) + 1;

/*	if (DISPLAY_VER(display) >= 12 &&
		(ddi_func_ctl & TRANS_DDI_MODE_SELECT_MASK) == TRANS_DDI_MODE_SELECT_FDI_OR_128B132B)
		crtc_state->mst_master_transcoder = static_cast<enum transcoder>(
			REG_FIELD_GET(TRANS_DDI_MST_TRANSPORT_SELECT_MASK, ddi_func_ctl));
*/
	//intel_cpu_transcoder_get_m1_n1(crtc, cpu_transcoder, &crtc_state->dp_m_n);
	//intel_cpu_transcoder_get_m2_n2(crtc, cpu_transcoder, &crtc_state->dp_m2_n2);

	crtc_state->enhanced_framing =	intel_de_read(display, dp_tp_ctl_reg()) &		DP_TP_CTL_ENHANCED_FRAME_ENABLE;

	if (DISPLAY_VER(display) >= 11)
		crtc_state->fec_enable = intel_de_read(display,dp_tp_ctl_reg()) & DP_TP_CTL_FEC_ENABLE;

	/*if (intel_lspcon_active(dig_port) && intel_dp_has_hdmi_sink(&dig_port->dp))
		crtc_state->infoframes.enable |=
			intel_lspcon_infoframes_enabled(encoder, crtc_state);
	else
		crtc_state->infoframes.enable |=
			intel_hdmi_infoframes_enabled(encoder, crtc_state);*/
}

static void intel_ddi_read_func_ctl(struct intel_display *display, struct intel_crtc_state *pipe_config)
{
	enum transcoder cpu_transcoder = pipe_config->cpu_transcoder;
	u32 ddi_func_ctl, ddi_mode, flags = 0;

	ddi_func_ctl = intel_de_read(display, TRANS_DDI_FUNC_CTL(display, cpu_transcoder));
	if (ddi_func_ctl & TRANS_DDI_PHSYNC)
		flags |= DRM_MODE_FLAG_PHSYNC;
	else
		flags |= DRM_MODE_FLAG_NHSYNC;
	if (ddi_func_ctl & TRANS_DDI_PVSYNC)
		flags |= DRM_MODE_FLAG_PVSYNC;
	else
		flags |= DRM_MODE_FLAG_NVSYNC;

	pipe_config->hw.adjusted_mode.flags |= flags;

	switch (ddi_func_ctl & TRANS_DDI_BPC_MASK) {
		default:
	/*case TRANS_DDI_BPC_6:
		pipe_config->pipe_bpp = 18;
		break;*/
	case TRANS_DDI_BPC_8:
		pipe_config->pipe_bpp = 24;
		break;
	case TRANS_DDI_BPC_10:
		pipe_config->pipe_bpp = 30;
		break;
	case TRANS_DDI_BPC_12:
		pipe_config->pipe_bpp = 36;
		break;

	}
	

	ddi_mode = ddi_func_ctl & TRANS_DDI_MODE_SELECT_MASK;

	/*if (ddi_mode == TRANS_DDI_MODE_SELECT_HDMI) {
		intel_ddi_read_func_ctl_hdmi(encoder, pipe_config, ddi_func_ctl);
	} else if (ddi_mode == TRANS_DDI_MODE_SELECT_DVI) {
		intel_ddi_read_func_ctl_dvi(encoder, pipe_config, ddi_func_ctl);
	} else if (ddi_mode == TRANS_DDI_MODE_SELECT_FDI_OR_128B132B && !HAS_DP20(display)) {
		intel_ddi_read_func_ctl_fdi(encoder, pipe_config, ddi_func_ctl);
	} else*/ if (ddi_mode == TRANS_DDI_MODE_SELECT_DP_SST)
		intel_ddi_read_func_ctl_dp_sst( display, pipe_config, ddi_func_ctl);
	/*} else if (ddi_mode == TRANS_DDI_MODE_SELECT_DP_MST) {
		intel_ddi_read_func_ctl_dp_mst(encoder, pipe_config, ddi_func_ctl);
	} else if (ddi_mode == TRANS_DDI_MODE_SELECT_FDI_OR_128B132B && HAS_DP20(display)) {
		struct intel_dp *intel_dp = enc_to_intel_dp(encoder);*/
		

		
		

		/*if (intel_dp_mst_active_streams(intel_dp))
			intel_ddi_read_func_ctl_dp_mst(encoder, pipe_config, ddi_func_ctl);
		else*/
			intel_ddi_read_func_ctl_dp_sst( display, pipe_config, ddi_func_ctl);
}

static enum transcoder bdw_transcoder_master_readout(struct intel_display *display,
							 enum transcoder cpu_transcoder)
{
	u32 master_select;

	if (DISPLAY_VER(display) >= 11) {
		u32 ctl2 = intel_de_read(display,
					 TRANS_DDI_FUNC_CTL2(display, cpu_transcoder));

		if ((ctl2 & PORT_SYNC_MODE_ENABLE) == 0)
			return INVALID_TRANSCODER;

		master_select = REG_FIELD_GET(PORT_SYNC_MODE_MASTER_SELECT_MASK, ctl2);
	} else {
		u32 ctl = intel_de_read(display,
					TRANS_DDI_FUNC_CTL(display, cpu_transcoder));

		if ((ctl & TRANS_DDI_PORT_SYNC_ENABLE) == 0)
			return INVALID_TRANSCODER;

		master_select = REG_FIELD_GET(TRANS_DDI_PORT_SYNC_MASTER_SELECT_MASK, ctl);
	}

	if (master_select == 0)
		return TRANSCODER_EDP;
	else
		return static_cast<enum transcoder>(master_select - 1);
}

static void bdw_get_trans_port_sync_config(struct intel_display *display, struct intel_crtc_state *crtc_state)
{
	u32 transcoders = BIT(TRANSCODER_A) | BIT(TRANSCODER_B) |
		BIT(TRANSCODER_C) | BIT(TRANSCODER_D);
	enum transcoder cpu_transcoder;

	crtc_state->master_transcoder =
		bdw_transcoder_master_readout(display, crtc_state->cpu_transcoder);

	for_each_cpu_transcoder_masked(display, cpu_transcoder, transcoders) {
		/*enum intel_display_power_domain power_domain;
		struct ref_tracker *trans_wakeref;

		power_domain = POWER_DOMAIN_TRANSCODER(cpu_transcoder);
		trans_wakeref = intel_display_power_get_if_enabled(display,
								   power_domain);

		if (!trans_wakeref)
			continue;*/

		if (bdw_transcoder_master_readout(display, cpu_transcoder) ==
			crtc_state->cpu_transcoder)
			crtc_state->sync_mode_slaves_mask |= BIT(cpu_transcoder);

		//intel_display_power_put(display, power_domain, trans_wakeref);
	}

}



static void intel_ddi_get_config(struct intel_display *display, struct intel_crtc_state *pipe_config)
{
	enum transcoder cpu_transcoder = pipe_config->cpu_transcoder;


	intel_ddi_read_func_ctl( display, pipe_config);

	intel_ddi_mso_get_config( display, pipe_config);

	pipe_config->has_audio = false;
		//intel_ddi_is_audio_enabled(display, cpu_transcoder);

	//if (encoder->type == INTEL_OUTPUT_EDP)
	//	intel_edp_fixup_vbt_bpp(encoder, pipe_config->pipe_bpp);
	
	//[drm:intel_edp_fixup_vbt_bpp [i915]] pipe has 24 bpp for eDP panel, overriding BIOS-provided max 18 bpp
	
	
	//ddi_dotclock_get(pipe_config);


	pipe_config->min_voltage_level = tgl_ddi_min_voltage_level(pipe_config);
	
	//intel_hdmi_read_gcp_infoframe(encoder, pipe_config);

	/*intel_read_infoframe(encoder, pipe_config,
				 HDMI_INFOFRAME_TYPE_AVI,
				 &pipe_config->infoframes.avi);
	intel_read_infoframe(encoder, pipe_config,
				 HDMI_INFOFRAME_TYPE_SPD,
				 &pipe_config->infoframes.spd);
	intel_read_infoframe(encoder, pipe_config,
				 HDMI_INFOFRAME_TYPE_VENDOR,
				 &pipe_config->infoframes.hdmi);
	intel_read_infoframe(encoder, pipe_config,
				 HDMI_INFOFRAME_TYPE_DRM,
				 &pipe_config->infoframes.drm);*/

	if (DISPLAY_VER(display) >= 8)
		bdw_get_trans_port_sync_config(display, pipe_config);

	//intel_psr_get_config(encoder, pipe_config);

	/*intel_read_dp_sdp(encoder, pipe_config, HDMI_PACKET_TYPE_GAMUT_METADATA);
	intel_read_dp_sdp(encoder, pipe_config, DP_SDP_VSC);
	intel_read_dp_sdp(encoder, pipe_config, DP_SDP_ADAPTIVE_SYNC);

	intel_audio_codec_get_config(encoder, pipe_config);*/
}
static u32 bdw_trans_port_sync_master_select(enum transcoder master_transcoder)
{
	if (master_transcoder == TRANSCODER_EDP)
		return 0;
	else
		return master_transcoder + 1;
}
void intel_ddi_enable_transcoder_func(struct intel_display *display , struct intel_crtc_state *crtc_state)
{
	enum transcoder cpu_transcoder = crtc_state->cpu_transcoder;

	if (DISPLAY_VER(display) >= 11) {
		enum transcoder master_transcoder = crtc_state->master_transcoder;
		u32 ctl2 = 0;

		if (master_transcoder != INVALID_TRANSCODER) {
			u8 master_select =
				bdw_trans_port_sync_master_select(master_transcoder);

			ctl2 |= PORT_SYNC_MODE_ENABLE |
				PORT_SYNC_MODE_MASTER_SELECT(master_select);
		}

		intel_de_write(display,
				   TRANS_DDI_FUNC_CTL2(display, cpu_transcoder),
				   ctl2);
	}

	intel_de_write(display, TRANS_DDI_FUNC_CTL(display, cpu_transcoder),
			   intel_ddi_transcoder_func_reg_val_get());
}

static bool pipe_scanline_is_moving(struct intel_display *display, enum pipe pipe)
{
	u32 reg = PIPEDSL(display, pipe);
	u32 line1, line2;

	line1 = intel_de_read(display, reg) & PIPEDSL_LINE_MASK;
	//msleep(5);
	IOSleep(5);
	line2 = intel_de_read(display, reg) & PIPEDSL_LINE_MASK;

	return line1 != line2;
}

int wait_for_pipe_scanline_moving(struct intel_display *display)
{

			AbsoluteTime deadline, now;
			uint32_t wait_us = 2;
			bool is_moving=false;
			
			uint64_t timeout_ns = (uint64_t)100 * 1000 * NSEC_PER_USEC;
			nanoseconds_to_absolutetime(timeout_ns, &deadline);

			for (;;) {
				now = mach_absolute_time();
				
				is_moving = pipe_scanline_is_moving(display, display->pipe0);
				
				if (is_moving ) {
					return 0;
				}

				if (CMP_ABSOLUTETIME(&now, &deadline)) {
					return 1;
				}

				if (wait_us < 1000) {
					IODelay(wait_us);
				} else {
					IOSleep(wait_us / 1000);
				}

				if (wait_us < 10000) {
					wait_us <<= 1;
				}
			}
}


void intel_enable_transcoder(struct intel_display *display, struct intel_crtc_state *new_crtc_state)
{
	enum transcoder cpu_transcoder = new_crtc_state->cpu_transcoder;
	enum pipe pipe = display->pipe0;
	u32 val;


	//assert_planes_disabled(crtc);


	/*if (HAS_GMCH(display)) {
		if (intel_crtc_has_type(new_crtc_state, INTEL_OUTPUT_DSI))
			assert_dsi_pll_enabled(display);
		else
			assert_pll_enabled(display, pipe);
	} else {
		if (new_crtc_state->has_pch_encoder) {

			assert_fdi_rx_pll_enabled(display,
						  intel_crtc_pch_transcoder(crtc));
			assert_fdi_tx_pll_enabled(display,
						  (enum pipe) cpu_transcoder);
		}
	}*/

	/* Wa_22012358565:adl-p */
	if (intel_display_wa(display, INTEL_DISPLAY_WA_22012358565))
		intel_de_rmw(display, PIPE_ARB_CTL(display, pipe),
				 0, PIPE_ARB_USE_PROG_SLOTS);

	/*if (DISPLAY_VER(display) >= 14) {
		u32 clear = DP_DSC_INSERT_SF_AT_EOL_WA;
		u32 set = 0;

		if (DISPLAY_VER(display) == 14)
			set |= DP_FEC_BS_JITTER_WA;

		intel_de_rmw(display, CHICKEN_TRANS(display, cpu_transcoder),
				 clear, set);
	}*/

	/*val = intel_de_read(display, TRANSCONF(display, cpu_transcoder));
	if (val & TRANSCONF_ENABLE) {
		return;
	}*/

	/* Wa_1409098942:adlp+ */
	/*if (DISPLAY_VER(display) >= 13 &&
		new_crtc_state->dsc.compression_enable) {
		val &= ~TRANSCONF_PIXEL_COUNT_SCALING_MASK;
		val |= REG_FIELD_PREP(TRANSCONF_PIXEL_COUNT_SCALING_MASK,
					  TRANSCONF_PIXEL_COUNT_SCALING_X4);
	}*/

	/*intel_de_write(display, TRANSCONF(display, cpu_transcoder),
			   val | TRANSCONF_ENABLE);
	intel_de_posting_read(display, TRANSCONF(display, cpu_transcoder));
*/

	//if (intel_crtc_max_vblank_count(new_crtc_state) == 0)
	wait_for_pipe_scanline_moving(display);
}



static void intel_ddi_enable(struct intel_display *display, struct intel_crtc_state *crtc_state)
{
	enum transcoder cpu_transcoder = crtc_state->cpu_transcoder;
	bool is_hdmi = false;
	int i;



	intel_ddi_enable_transcoder_func( display,crtc_state);

	//intel_vrr_transcoder_enable(crtc_state);

	intel_enable_transcoder(display, crtc_state);

	//intel_ddi_wait_for_fec_status(encoder, crtc_state, true);

	/*for_each_pipe_crtc_modeset_enable(display, pipe_crtc, crtc_state, i) {
		const struct intel_crtc_state *pipe_crtc_state =
			intel_atomic_get_new_crtc_state(state, pipe_crtc);

		intel_crtc_vblank_on(pipe_crtc_state);
	}
	 

	if (is_hdmi)
		intel_ddi_enable_hdmi(state, encoder, crtc_state, conn_state);
	else*/
	//	intel_ddi_enable_dp(  crtc_state);

	//intel_hdcp_enable(  crtc_state);

}

static void icl_ddi_combo_get_config(struct intel_display *display, struct intel_crtc_state *crtc_state)
{
	//intel_ddi_get_clock( crtc_state, icl_ddi_combo_get_pll(encoder));
	intel_ddi_get_config( display, crtc_state);
}

static bool intel_pipe_is_interlaced(struct intel_display *display, const struct intel_crtc_state *crtc_state)
{
	enum transcoder cpu_transcoder = crtc_state->cpu_transcoder;

	if (DISPLAY_VER(display) == 2 || DISPLAY_VER(display) >= 35)
		return false;

	if (DISPLAY_VER(display) >= 9 ||
		display->platform.broadwell || display->platform.haswell)
		return intel_de_read(display,
					 TRANSCONF(display, cpu_transcoder)) & TRANSCONF_INTERLACE_MASK_HSW;
	else
		return intel_de_read(display,
					 TRANSCONF(display, cpu_transcoder)) & TRANSCONF_INTERLACE_MASK;
}
static inline bool transcoder_is_dsi(enum transcoder transcoder)
{
	return transcoder == TRANSCODER_DSI_A || transcoder == TRANSCODER_DSI_C;
}

static u8 hsw_panel_transcoders(struct intel_display *display)
{
	u8 panel_transcoder_mask = BIT(TRANSCODER_EDP);

	if (DISPLAY_VER(display) >= 11)
		panel_transcoder_mask |= BIT(TRANSCODER_DSI_0) | BIT(TRANSCODER_DSI_1);

	return panel_transcoder_mask;
}

static void intel_get_transcoder_timings(struct intel_display *display, struct intel_crtc_state *pipe_config)
{
	enum transcoder cpu_transcoder = pipe_config->cpu_transcoder;
	struct drm_display_mode *adjusted_mode = &pipe_config->hw.adjusted_mode;
	u32 tmp;

	tmp = intel_de_read(display, TRANS_HTOTAL(display, cpu_transcoder));
	adjusted_mode->crtc_hdisplay = REG_FIELD_GET(HACTIVE_MASK, tmp) + 1;
	adjusted_mode->crtc_htotal = REG_FIELD_GET(HTOTAL_MASK, tmp) + 1;

	if (!transcoder_is_dsi(cpu_transcoder)) {
		tmp = intel_de_read(display,
					TRANS_HBLANK(display, cpu_transcoder));
		adjusted_mode->crtc_hblank_start = REG_FIELD_GET(HBLANK_START_MASK, tmp) + 1;
		adjusted_mode->crtc_hblank_end = REG_FIELD_GET(HBLANK_END_MASK, tmp) + 1;
	}

	tmp = intel_de_read(display, TRANS_HSYNC(display, cpu_transcoder));
	adjusted_mode->crtc_hsync_start = REG_FIELD_GET(HSYNC_START_MASK, tmp) + 1;
	adjusted_mode->crtc_hsync_end = REG_FIELD_GET(HSYNC_END_MASK, tmp) + 1;

	tmp = intel_de_read(display, TRANS_VTOTAL(display, cpu_transcoder));
	adjusted_mode->crtc_vdisplay = REG_FIELD_GET(VACTIVE_MASK, tmp) + 1;
	adjusted_mode->crtc_vtotal = REG_FIELD_GET(VTOTAL_MASK, tmp) + 1;

	if (!transcoder_is_dsi(cpu_transcoder)) {
		tmp = intel_de_read(display,
					TRANS_VBLANK(display, cpu_transcoder));
		adjusted_mode->crtc_vblank_start = REG_FIELD_GET(VBLANK_START_MASK, tmp) + 1;
		adjusted_mode->crtc_vblank_end = REG_FIELD_GET(VBLANK_END_MASK, tmp) + 1;
	}
	tmp = intel_de_read(display, TRANS_VSYNC(display, cpu_transcoder));
	adjusted_mode->crtc_vsync_start = REG_FIELD_GET(VSYNC_START_MASK, tmp) + 1;
	adjusted_mode->crtc_vsync_end = REG_FIELD_GET(VSYNC_END_MASK, tmp) + 1;

	if (intel_pipe_is_interlaced(display, pipe_config)) {
		adjusted_mode->flags |= DRM_MODE_FLAG_INTERLACE;
		adjusted_mode->crtc_vtotal += 1;
		adjusted_mode->crtc_vblank_end += 1;
	}

	if (DISPLAY_VER(display) >= 13 && !transcoder_is_dsi(cpu_transcoder)) {
		pipe_config->set_context_latency =
			intel_de_read(display,
					  TRANS_SET_CONTEXT_LATENCY(display, cpu_transcoder));
		adjusted_mode->crtc_vblank_start =
			adjusted_mode->crtc_vdisplay +
			pipe_config->set_context_latency;
	} else if (DISPLAY_VER(display) == 12) {

		pipe_config->set_context_latency =
			adjusted_mode->crtc_vblank_start - adjusted_mode->crtc_vdisplay;
	}

	if (DISPLAY_VER(display) >= 30)
		pipe_config->min_hblank = intel_de_read(display,
							DP_MIN_HBLANK_CTL(cpu_transcoder));
}

static int intel_ddi_compute_config_late(struct intel_crtc_state *crtc_state)
{
	u8 port_sync_transcoders = 0;
	int ret = 0;

/*	if (intel_crtc_has_dp_encoder(crtc_state))
		ret = intel_dp_compute_config_late(encoder, crtc_state, conn_state);

	if (ret)
		return ret;


	if (connector->has_tile)
		port_sync_transcoders = intel_ddi_port_sync_transcoders(crtc_state,
									connector->tile_group->id);
*/
	

	if (port_sync_transcoders & BIT(TRANSCODER_EDP))
		crtc_state->master_transcoder = TRANSCODER_EDP;
	else
		crtc_state->master_transcoder = static_cast<enum transcoder >( ffs(port_sync_transcoders) - 1);

	if (crtc_state->master_transcoder == crtc_state->cpu_transcoder) {
		crtc_state->master_transcoder = INVALID_TRANSCODER;
		//crtc_state->sync_mode_slaves_mask =
		//	port_sync_transcoders & ~BIT(crtc_state->cpu_transcoder);
	}

	return 0;
}

uint64_t  Gen11::linkTraining(void *that,void *param_1)
{
	//return FunctionCast(linkTraining, callback->olinkTraining)(that,param_1);
	
	struct intel_display *display = NBlue::callback->i915b->display;
	struct intel_dp *intel_dp=&display->intel_dp0;
	struct intel_crtc_state *crtc_state=&display->crtc_state0;
	int lane_count=display->panel.vbt.edp.lanes;

	intel_dp->para=(struct AGDCDPPortConfig_t *)param_1;
	if (intel_dp->para != nullptr) {
		intel_dp->para->portindex=display->bconnectors[0].index+1;
		intel_dp->para->status = 0;
		intel_dp->para->field1 = 0x200;
		intel_dp->para->field2 = 0;
		intel_dp->para->enableMST = (u8)getMember<u8>(that, kexticl ? 0xf : 0x117);
		intel_dp->para->BitRate = (u8)getMember<u8>(that, kexticl ? 0x24 : 0x128);
		intel_dp->para->NumberOfLanes = (u8)lane_count;
		intel_dp->para->EnhancedFraming = (u8)getMember<u8>(that, kexticl ? 0x12 : 0x11a);
		intel_dp->para->ASR = 0;//(u8)getMember<u8>(that, 0x118);  //DP_EDP_CONFIGURATION_SET
		intel_dp->para->Downspread = 0;//(u8)getMember<u8>(that, kexticl ? 0x11 : 0x119);
		intel_dp->para->BitRate2 = intel_dp->para->BitRate;
		intel_dp->para->NumberOfLanes2 = (u8)lane_count;
		intel_dp->para->CR =0;
		intel_dp->para->EQ =0;
		intel_dp->para->voltageSwing = 0;
		intel_dp->para->preEmphasis = 0;
	}
	
	auto ret=tgl_ddi_pre_enable_dp(display, crtc_state);
	
	intel_dp->para0. version=intel_dp->dpcd[DP_DPCD_REV];		 // 8 bit high (major); 8 bit low (minor)
	intel_dp->para0.  bitRate=(u8)getMember<u8>(that, kexticl ? 0x24 : 0x128);
	intel_dp->para0.  laneCount=(u8)lane_count;
	intel_dp->para0.  maxBitRate=intel_dp->para0. bitRate;	 // same encoding as the bitRate field
	intel_dp->para0.  maxLaneCount=intel_dp->para0.  laneCount;
	
	
	if (!intel_crtc_has_type(crtc_state, INTEL_OUTPUT_DP_MST))
		intel_ddi_set_dp_msa(display, true);
	
	intel_ddi_enable(display, crtc_state);
	
	return 0; // hack
	
	if (ret) return 0;
	panic("x");
	return -1;
}

uint64_t Gen11::getLinkConfig(void *that,IOFBDPLinkConfig *param_1)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	struct intel_dp *intel_dp=&display->intel_dp0;
	
	auto ret=FunctionCast(getLinkConfig, callback->ogetLinkConfig)(that,param_1 );

	param_1->  downspread=0;
	param_1->  scrambler=0;
	param_1->  maxDownspread=0;
	
	if (intel_dp->para0.t1Time) {
		param_1->  downspread=intel_dp->para0.downspread;
		param_1->  maxDownspread=param_1->  downspread;
		param_1-> version=intel_dp->para0.version;		 // 8 bit high (major); 8 bit low (minor)
		param_1->  bitRate=intel_dp->para0.bitRate;			 // same encoding as the spec
		param_1-> t1Time=intel_dp->para0.t1Time;		 // minimum duration of the t1 pattern (microseconds)
		param_1-> t2Time=intel_dp->para0.t2Time;		 // minimum duration of the t2 pattern
		param_1-> t3Time=0;	 // minimum duration of the t3 pattern
		param_1->  idlePatterns=5;   // minimum number of idle patterns
		param_1->  laneCount=intel_dp->para0.laneCount ;		 // number of lanes in the link
		param_1->  voltage=intel_dp->para0.voltage;
		param_1->  preEmphasis=intel_dp->para0.preEmphasis;
		param_1->  maxBitRate=param_1->  bitRate;	 // same encoding as the bitRate field
		param_1->  maxLaneCount=param_1->  laneCount;	 // an integer
	}
	return ret;
}



void Gen11::SetupParams2 (void *param_2, CRTCParams *param_3)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	if (setpc){
		setpc=0;
		
		void *port=getMember<void *>(param_2, kexticl ? 0x4d20 : 0x3648);
		getMember<uint8_t>(port, kexticl ? 0x10 : 0x118)=0;//asr
		getMember<uint8_t>(port, kexticl ? 0x11 : 0x119)=0;//Downspread
		
		
		if (!dpcdconf)
		{
			dpcdconf=true;
			struct intel_dp *intel_dp=&display->intel_dp0;
			struct intel_crtc_state *crtc_state=&display->crtc_state0;
			
			//dpcd_access_needs_probe
			drm_dp_read_dpcd_caps(intel_dp,intel_dp->dpcd);
			
			readAUX(linkp,DP_EDP_DPCD_REV,&intel_dp->edp_dpcd, sizeof(intel_dp->edp_dpcd));
			
			drm_dp_read_downstream_info(intel_dp, intel_dp->dpcd,intel_dp->downstream_ports);
			
			intel_dp->mst_detect = intel_dp_mst_detect(display, intel_dp);
			intel_dp->is_mst = intel_dp->mst_detect != DRM_DP_SST;
			if (!intel_dp->is_mst) intel_dp->mst_detect = DRM_DP_SST;

			memset(intel_dp->lttpr_common_caps, 0, sizeof(intel_dp->lttpr_common_caps));
			intel_dp->use_max_params = intel_dp->edp_dpcd[0] < DP_EDP_14;
			
			intel_dp_init_source_oui(intel_dp);
			
			readAUX(linkp, DP_RECEIVER_ALPM_CAP,&intel_dp->alpm_dpcd,1);
			
			/*intel_psr_init_dpcd(intel_dp);
			intel_edp_set_sink_rates(intel_dp);
			intel_dp_set_max_sink_lane_count(intel_dp);
			intel_dp_detect_dsc_caps(intel_dp, connector);
			*/
			//hsw_get_pipe_config
			//intel_ddi_init
			intel_get_transcoder_timings(display,crtc_state);
			icl_ddi_combo_get_config(display, crtc_state);
			intel_ddi_mso_get_config(display, crtc_state);
			intel_dp_compute_config(display, crtc_state);
			intel_ddi_compute_config_late(crtc_state);
			
			if (hsw_panel_transcoders(display) & BIT(crtc_state->cpu_transcoder)) {
				u32 tmp = intel_de_read(display,
							TRANS_DDI_FUNC_CTL(display, crtc_state->cpu_transcoder));

				if ((tmp & TRANS_DDI_EDP_INPUT_MASK) == TRANS_DDI_EDP_INPUT_A_ONOFF)
					crtc_state->pch_pfit.force_thru = true;
			}

			
		}
		
		param_3->TRANS_CLK_SEL=TGL_TRANS_CLK_SEL_PORT(display->port0);
		param_3->TRANS_MSA_MISC =intel_ddi_set_dp_msa(display, false);
		param_3->TRANS_DDI_FUNC_CTL= intel_ddi_transcoder_func_reg_val_get();
		param_3->PIPE_MISC=bdw_set_pipe_misc();
		param_3->TRANSCONF= 0xc0000024;
		
		int fScanoutHeight=getMember<int>(param_2, kexticl ? 0x2fc : 0xfc);
		int fLinkScanoutWidth=getMember<int>(param_2, kexticl ? 0x2f8 : 0xf8);
		param_3->PIPESRC =
		((fScanoutHeight - 1) & 0x1fff) |
		(param_3->PIPESRC & 0xe000e000) |
		((fLinkScanoutWidth * 0x10000 + 0x1fff0000) & 0x1fff0000);
		
	}
	
}

int Gen11::writeAUX(void *that,uint param_1,void *param_2,uint param_3)
{
	auto ret=FunctionCast(writeAUX, callback->owriteAUX)(that ,param_1,param_2,param_3);
	return ret;
}

uint64_t Gen11::hwSetPanelPower(void *that,uint param_1)
{
	struct intel_display *display=NBlue::callback->i915b->display;
	if (IS_DISPLAY_VER(NBlue::callback->i915b->display, 13, 14))
		intel_de_rmw(display, SOUTH_DSPCLK_GATE_D,
				 0, PCH_DPLSUNIT_CLOCK_GATE_DISABLE);
	
	auto ret= FunctionCast(hwSetPanelPower, callback->ohwSetPanelPower)(that,param_1);
	
	if (IS_DISPLAY_VER(NBlue::callback->i915b->display, 13, 14))
		intel_de_rmw(display, SOUTH_DSPCLK_GATE_D,
				 PCH_DPLSUNIT_CLOCK_GATE_DISABLE, 0);
	
	return ret;
};

void Gen11::initCDClock(void *that)
{
	FunctionCast(initCDClock, callback->oinitCDClock)(that);
}
int Gen11::readAUX(void *that,uint param_1,void *param_2,uint param_3)
{
	auto ret=FunctionCast(readAUX, callback->oreadAUX)(that ,param_1,param_2,param_3);
	return ret;
}

int Gen11::isConflictRegister(void *that,uint param_1)
{
	return -1;
}

int Gen11::fgetPixelInformation(void *that,int param_1,int param_2,int param_3,void *param_4)
{
	auto ret=FunctionCast(fgetPixelInformation, callback->ofgetPixelInformation)(that ,param_1,param_2,param_3,param_4);
	if (ret!=0) ret=0;
	return ret;
}








static constexpr size_t RING_MMIO_PTR			= 0x18;
static constexpr size_t RING_HEAD_REG			= 0x10;
static constexpr uint32_t RING_HEAD_MASK	= 0x1ffffc;
static constexpr size_t RING_HEAD				= 0x60;
static constexpr size_t RING_EMIT				= 0x64;
static constexpr size_t RING_SPACE				= 0x88;
static constexpr size_t RING_SIZE				= 0x8c;
static constexpr size_t RING_EFFECTIVE_SIZE	= 0x90;


bool Gen11::linux_wait_for_space(void *ring, uint32_t total_bytes, uint64_t timeout_ns)
{
	uint32_t size = getMember<uint32_t>(ring, RING_SIZE);
	uint32_t emit = getMember<uint32_t>(ring, RING_EMIT);
	
	uint64_t deadline_ticks;
	nanoseconds_to_absolutetime(timeout_ns, &deadline_ticks);
	uint64_t end_time = mach_absolute_time() + deadline_ticks;

	do {
		uintptr_t mmio = getMember<uintptr_t>(ring, RING_MMIO_PTR);
		uint32_t head = (*reinterpret_cast<volatile uint32_t *>(mmio + RING_HEAD_REG)) & RING_HEAD_MASK;
		
		if (head < size) {
			head &= (size - 1);
			getMember<uint32_t>(ring, RING_HEAD) = head;

			int32_t space = (int32_t)(head - emit) - 8;
			if (space < 0) space += (int32_t)size;
			getMember<int32_t>(ring, RING_SPACE) = space;

			if ((uint32_t)space >= total_bytes) {
				return true;
			}
		}

		IODelay(10);

	} while (mach_absolute_time() < end_time);

	return false;
}


uint64_t Gen11::waitForSpace(void *ring, unsigned int num_dwords)
{
	
	const uint32_t effective_size = getMember<uint32_t>(ring, RING_EFFECTIVE_SIZE);
	const uint32_t emit = getMember<uint32_t>(ring, RING_EMIT);
	const uint32_t size = getMember<uint32_t>(ring, RING_SIZE);
	const uint32_t remain_usable = effective_size - emit;
	
	const uint32_t bytes = num_dwords * sizeof(uint32_t);
	uint32_t need_wrap = 0;
	uint32_t total_bytes = bytes;

	if (size - 8 < bytes) {
		return 0;
	}

	if (bytes > remain_usable) {
		const uint32_t remain_actual = size - emit;
		total_bytes = bytes + remain_actual;
		need_wrap = remain_actual;
	}

	if (total_bytes > getMember<uint32_t>(ring, RING_SPACE)) {
			if (!linux_wait_for_space(ring, total_bytes, 5000000000ULL)) {
				return 0;
			}
		}

	if (need_wrap) {
		uint32_t wrap_bytes = need_wrap & ~3U;
		
		void *pvaddr = getMember<void*>(ring, 0x80);
		uint8_t *vaddr = getMember<uint8_t *>(pvaddr, 0x38);
		
		uint32_t cur_space = getMember<uint32_t>(ring, RING_SPACE);

		if (wrap_bytes > cur_space) return 0;

		memset(vaddr + emit, 0, wrap_bytes);

		getMember<uint32_t>(ring, RING_SPACE) = cur_space - wrap_bytes;
		getMember<uint32_t>(ring, RING_EMIT) = 0;
	}


	uint32_t cur_emit = getMember<uint32_t>(ring, RING_EMIT);
	uint32_t cur_space = getMember<uint32_t>(ring, RING_SPACE);

	if (cur_space < bytes || cur_emit > size - bytes) return 0;

	void *pvaddr = getMember<void*>(ring, 0x80);
	uint8_t *vaddr = getMember<uint8_t *>(pvaddr, 0x38);

	memset(vaddr + cur_emit, 0, bytes);

	getMember<uint32_t>(ring, RING_EMIT) = cur_emit + bytes;
	getMember<uint32_t>(ring, RING_SPACE) = cur_space - bytes;


	return (uint64_t)num_dwords << 8 | 1ULL;
}







static u32 pipe_mbus_dbox_ctl(struct intel_display *display, enum pipe pipe,
				  const struct intel_dbuf_state *dbuf_state)
{
	u32 val = 0;

	if (DISPLAY_VER(display) >= 14)
		val |= MBUS_DBOX_I_CREDIT(2);

	if (DISPLAY_VER(display) >= 12) {
		val |= MBUS_DBOX_B2B_TRANSACTIONS_MAX(16);
		val |= MBUS_DBOX_B2B_TRANSACTIONS_DELAY(1);
		val |= MBUS_DBOX_REGULATE_B2B_TRANSACTIONS_EN;
	}

	if (DISPLAY_VER(display) >= 14)
		val |= dbuf_state->joined_mbus ?
			MBUS_DBOX_A_CREDIT(12) : MBUS_DBOX_A_CREDIT(8);
	else if (intel_display_wa(display, INTEL_DISPLAY_WA_22010947358))
		/* Wa_22010947358:adl-p */
		val |= dbuf_state->joined_mbus ?
			MBUS_DBOX_A_CREDIT(6) : MBUS_DBOX_A_CREDIT(4);
	else
		val |= MBUS_DBOX_A_CREDIT(2);

	if (DISPLAY_VER(display) >= 14) {
		val |= MBUS_DBOX_B_CREDIT(0xA);
	} else if (display->platform.alderlake_p) {
		val |= MBUS_DBOX_BW_CREDIT(2);
		val |= MBUS_DBOX_B_CREDIT(8);
	} else if (DISPLAY_VER(display) >= 12) {
		val |= MBUS_DBOX_BW_CREDIT(2);
		val |= MBUS_DBOX_B_CREDIT(12);
	} else {
		val |= MBUS_DBOX_BW_CREDIT(1);
		val |= MBUS_DBOX_B_CREDIT(8);
	}

	/*if (DISPLAY_VERx100(display) == 1400) {
		if (xelpdp_is_only_pipe_per_dbuf_bank(pipe, dbuf_state->active_pipes))
			val |= MBUS_DBOX_BW_8CREDITS_MTL;
		else
			val |= MBUS_DBOX_BW_4CREDITS_MTL;
	}*/

	return val;
}

static void pipe_mbus_dbox_ctl_update(struct intel_display *display,
					  const struct intel_dbuf_state *dbuf_state)
{
	enum pipe pipe;

	for_each_pipe(display, pipe) {
		if (dbuf_state->active_pipes & BIT(pipe))
			intel_de_write(display, PIPE_MBUS_DBOX_CTL(pipe),
					   pipe_mbus_dbox_ctl(display, pipe, dbuf_state));
	}
}

static void gen9_wait_for_power_well_fuses(struct intel_display *display,
					   enum skl_power_gate pg)
{

			intel_de_wait_for_set_ms(display, SKL_FUSE_STATUS,
						 SKL_FUSE_PG_DIST_STATUS(pg), 1);
}

int intel_de_wait_us(struct intel_display *display, u32 reg,
			 u32 mask, u32 value, unsigned int timeout_us,
			 u32 *out_value)
{
	int ret;

	//intel_dmc_wl_get(display, reg);

	ret = intel_de_wait_for_register(display, reg, mask, value,
					 timeout_us, 0,
					 out_value, false);

//	intel_dmc_wl_put(display, reg);

	return ret;
}

int intel_de_wait_for_set_us(struct intel_display *display, u32 reg,
				 u32 mask, unsigned int timeout_us)
{
	return intel_de_wait_us(display, reg, mask, mask, timeout_us, NULL);
}

static int cnp_rawclk(struct intel_display *display)
{
	int divider, fraction;
	u32 rawclk;

	if (intel_de_read(display, SFUSE_STRAP) & SFUSE_STRAP_RAW_FREQUENCY) {
		/* 24 MHz */
		divider = 24000;
		fraction = 0;
	} else {
		/* 19.2 MHz */
		divider = 19000;
		fraction = 200;
	}

	rawclk = CNP_RAWCLK_DIV(divider / 1000);
	if (fraction) {
		int numerator = 1;

		rawclk |= CNP_RAWCLK_DEN(DIV_ROUND_CLOSEST(numerator * 1000,
							   fraction) - 1);
		if (INTEL_PCH_TYPE(display) >= PCH_ICP)
			rawclk |= ICP_RAWCLK_NUM(numerator);
	}

	intel_de_write(display, PCH_RAWCLK_FREQ, rawclk);
	return divider + fraction;
}

static void mbus_ctl_join_update(struct intel_display *display,
				 const struct intel_dbuf_state *dbuf_state,
				 enum pipe pipe)
{
	u32 mbus_ctl;

	if (dbuf_state->joined_mbus)
		mbus_ctl = MBUS_HASHING_MODE_1x4 | MBUS_JOIN;
	else
		mbus_ctl = MBUS_HASHING_MODE_2x2;

	if (pipe != INVALID_PIPE)
		mbus_ctl |= MBUS_JOIN_PIPE_SELECT(pipe);
	else
		mbus_ctl |= MBUS_JOIN_PIPE_SELECT_NONE;

	intel_de_rmw(display, MBUS_CTL,
			 MBUS_HASHING_MODE_MASK | MBUS_JOIN |
			 MBUS_JOIN_PIPE_SELECT_MASK, mbus_ctl);
}

void intel_dbuf_mdclk_cdclk_ratio_update(struct intel_display *display,
					 int ratio, bool joined_mbus)
{
	enum dbuf_slice slice;

	if (!HAS_MBUS_JOINING(display))
		return;

	/*if (DISPLAY_VER(display) >= 35)
		intel_de_rmw(display, MBUS_CTL, XE3P_MBUS_TRANSLATION_THROTTLE_MIN_MASK,
				 XE3P_MBUS_TRANSLATION_THROTTLE_MIN(ratio - 1));
	else if (DISPLAY_VER(display) >= 20)
		intel_de_rmw(display, MBUS_CTL, MBUS_TRANSLATION_THROTTLE_MIN_MASK,
				 MBUS_TRANSLATION_THROTTLE_MIN(ratio - 1));
*/
	if (joined_mbus)
		ratio *= 2;

	for_each_dbuf_slice(display, slice)
		/*if (DISPLAY_VER(display) >= 35)
			intel_de_rmw(display, DBUF_CTL_S(slice),
					 XE3P_DBUF_MIN_TRACKER_STATE_SERVICE_MASK,
					 XE3P_DBUF_MIN_TRACKER_STATE_SERVICE(ratio - 1));
		else*/
			intel_de_rmw(display, DBUF_CTL_S(slice),
					 DBUF_MIN_TRACKER_STATE_SERVICE_MASK,
					 DBUF_MIN_TRACKER_STATE_SERVICE(ratio - 1));
}


void  Gen11::enableDisplayEngine(void *that0)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	u32 pg_state, pll_state, dbuf_state0, ddi_state;
	AppleIntelPowerWell0 *that=(AppleIntelPowerWell0*)that0;
	
	pg_state = intel_de_read(display, HSW_PWR_WELL_CTL2); // 0x45404
	pll_state = intel_de_read(display, BXT_DE_PLL_ENABLE); // 0x46070
	dbuf_state0 = intel_de_read(display, DBUF_CTL_S(DBUF_S1)); // 0x45008 = DBUF_S0 rename bug

	bool de_enabled = ((pg_state & (HSW_PWR_WELL_CTL_REQ(0) | HSW_PWR_WELL_CTL_STATE(0))) == (HSW_PWR_WELL_CTL_REQ(0) | HSW_PWR_WELL_CTL_STATE(0))) &&
					  ((pll_state & BXT_DE_PLL_LOCK) != 0) &&
					  ((dbuf_state0 & (DBUF_POWER_REQUEST | DBUF_POWER_STATE)) != 0);

	
	if (!de_enabled) {
		ddi_state = intel_de_read(display, ICL_PWR_WELL_CTL_DDI2); // 0x45454
		
		bool any_ddi_requested = false;
		for (int i = 0; i < 9; i++) {
			if (ddi_state & HSW_PWR_WELL_CTL_REQ(i)) {
				any_ddi_requested = true;
				break;
			}
		}
		
		if (!any_ddi_requested) {
			overridePowerWellsState(that0, false);
		} else {
			intel_de_rmw(display, GEN8_CHICKEN_DCPR_1, 0, MASK_WAKEMEM);
			initCDClock(that->contr);
			return;
		}
	}

	
	//icl_set_pipe_chicken();
	
	if (DISPLAY_VER(display) == 12)
		intel_de_rmw(display, CLKREQ_POLICY, CLKREQ_POLICY_MEM_UP_OVRD, 0);
	
	gen9_set_dc_state(display, DC_STATE_DISABLE);

	if (intel_display_wa(display, INTEL_DISPLAY_WA_14011294188))
		intel_de_rmw(display, SOUTH_DSPCLK_GATE_D, 0, PCH_DPMGUNIT_CLOCK_GATE_DISABLE);
	
	
	cnp_rawclk(display);

	intel_pch_reset_handshake(display, !HAS_PCH_NOP(display));
	

	icl_combo_phys_init(display);


	if (!(pg_state & HSW_PWR_WELL_CTL_STATE(0))) {
		
		if (display->platform.alderlake_p)
			intel_de_rmw(display, GEN8_CHICKEN_DCPR_1, 0, DISABLE_FLR_SRC);
			
		gen9_wait_for_power_well_fuses(display, SKL_PG0);
		
		intel_de_rmw(display, HSW_PWR_WELL_CTL2, 0, HSW_PWR_WELL_CTL_REQ(0));
		
		if (intel_de_wait_for_set_ms(display, HSW_PWR_WELL_CTL2, HSW_PWR_WELL_CTL_STATE(0), 1) != 0) {

		}
		
		gen9_wait_for_power_well_fuses(display, SKL_PG1);
	}


	intel_de_rmw(display, HSW_PWR_WELL_CTL2, 0, HSW_PWR_WELL_CTL_REQ(1));
	if (intel_de_wait_for_set_ms(display, HSW_PWR_WELL_CTL2, HSW_PWR_WELL_CTL_STATE(1), 1) != 0) {

	}
	
	gen9_wait_for_power_well_fuses(display, SKL_PG2);
	
	callback->orgSetCDClockFrequency(that->contr, getMember<u64>(that->contr, 0xea8)/*that->contr->fPendingCDClockFrequency*/);
	
	if (DISPLAY_VER(display) == 12 || display->platform.dg2)
		gen12_dbuf_slices_config(display);

	gen9_dbuf_enable(display);
	
	icl_mbus_init(display);

	if (DISPLAY_VER(display) >= 12)
		tgl_bw_buddy_init(display);
	
	/* Wa_14011508470:tgl,dg1,rkl,adl-s,adl-p,dg2 */
	if (intel_display_wa(display, INTEL_DISPLAY_WA_14011508470))
		intel_de_rmw(display, GEN11_CHICKEN_DCPR_2, 0,
				 DCPR_CLEAR_MEMSTAT_DIS | DCPR_SEND_RESP_IMM |
				 DCPR_MASK_LPMODE | DCPR_MASK_MAXLATENCY_MEMUP_CLR);

	/* Wa_14011503030:xelpd */
	if (intel_display_wa(display, INTEL_DISPLAY_WA_14011503030))
		intel_de_write(display, XELPD_DISPLAY_ERR_FATAL_MASK, ~0);

	/* Wa_15013987218 */
	if (intel_display_wa(display, INTEL_DISPLAY_WA_15013987218)) {
		intel_de_rmw(display, SOUTH_DSPCLK_GATE_D,
				 0, PCH_GMBUSUNIT_CLOCK_GATE_DISABLE);
		intel_de_rmw(display, SOUTH_DSPCLK_GATE_D,
				 PCH_GMBUSUNIT_CLOCK_GATE_DISABLE, 0);
	}
	
	

	struct intel_dbuf_state dbuf_state;
	
	dbuf_state.joined_mbus = false;
	
	if (HAS_MBUS_JOINING(display))
		dbuf_state.joined_mbus = intel_de_read(display, MBUS_CTL) & MBUS_JOIN;
	
	//tgl_allowed_dbufs
	dbuf_state.active_pipes = BIT(PIPE_A);
	
	mbus_ctl_join_update(display, &dbuf_state, PIPE_A);
	pipe_mbus_dbox_ctl_update(display, &dbuf_state);
	intel_dbuf_mdclk_cdclk_ratio_update(display, 1,dbuf_state.joined_mbus);

}


int Gen11::probePortMode(struct intel_display *display, int port)
{
	u32 val;
	int tc_port;
	int local_tc_port;
	u32 reg;

	if (port < 3) return 1;

	tc_port = port - 3;

	// FIA0 (0x1638A0) handles TC1-TC4 (global 0-3)
	// FIA1 (0x1648A0) handles TC5-TC6 (global 4-5)
	if (tc_port < 4) {
		reg = 0x1638A0;
		local_tc_port = tc_port;
	} else {
		reg = 0x1648A0;
		local_tc_port = tc_port - 4;
	}

	val = intel_de_read(display, reg);

	if (val == 0xffffffff) return 0;

	if (val & BIT(local_tc_port * 8 + 6)) return 2; // TBT mode

	if (val & BIT(local_tc_port * 8 + 5)) return 3; // DP_ALT mode
	
	return 1;
}

void  Gen11::AppleIntelPowerWellinit(void *that0, void *param_1)
{
	struct intel_display *display = NBlue::callback->i915b->display;
	u32 pg_state, ddi_state, aux_state, tbt_state;
	int bootPipe;
	u32 active_ddi = 9;
	int i;
	AppleIntelPowerWell0 *that=(AppleIntelPowerWell0*)that0;
	
	that->contr = param_1;
	//that->mmio = param_1->mmio;
	that->mmio = getMember<void *>(param_1, 0xc40);
	that->powerwellalwaysON = 0;
	
	memset(&that->fRefCountPG, 0, 0x14);
	memset(&that->DDIA, 0, 0x24);
	memset(&that->AUXA, 0, 0x24);

	
	//if ((that->contr->flags_ig & FB_FLAG_BOOST_PIXEL_FREQUENCY_LIMIT) != 0) {
	if ((getMember<u32>(param_1, 0xc58) & FB_FLAG_BOOST_PIXEL_FREQUENCY_LIMIT) != 0) {
		that->powerwellalwaysON = 1;
	}


	pg_state  = intel_de_read(display, HSW_PWR_WELL_CTL2);       // 0x45404 (PG1-PG5)
	ddi_state = intel_de_read(display, ICL_PWR_WELL_CTL_DDI2);   // 0x45454 (DDI A/B/C/TC1-6)
	aux_state = intel_de_read(display, ICL_PWR_WELL_CTL_AUX2);   // 0x45444 (AUX A/B/C/USBC1-6)
	tbt_state = intel_de_read(display, TGL_PWR_WELL_CTL_AUX4);   // 0x45470 (AUX TBT1-6)


	that->fRefCountPG = 1;

	that->PG1 = (pg_state & (HSW_PWR_WELL_CTL_REQ(0) | HSW_PWR_WELL_CTL_STATE(0))) == (HSW_PWR_WELL_CTL_REQ(0) | HSW_PWR_WELL_CTL_STATE(0));
	that->PG2 = (pg_state & (HSW_PWR_WELL_CTL_REQ(1) | HSW_PWR_WELL_CTL_STATE(1))) == (HSW_PWR_WELL_CTL_REQ(1) | HSW_PWR_WELL_CTL_STATE(1));
	that->PG3 = (pg_state & (HSW_PWR_WELL_CTL_REQ(2) | HSW_PWR_WELL_CTL_STATE(2))) == (HSW_PWR_WELL_CTL_REQ(2) | HSW_PWR_WELL_CTL_STATE(2));
	that->PG4 = (pg_state & (HSW_PWR_WELL_CTL_REQ(3) | HSW_PWR_WELL_CTL_STATE(3))) == (HSW_PWR_WELL_CTL_REQ(3) | HSW_PWR_WELL_CTL_STATE(3));

	bool pg5_enabled = (pg_state & (HSW_PWR_WELL_CTL_REQ(4) | HSW_PWR_WELL_CTL_STATE(4))) == (HSW_PWR_WELL_CTL_REQ(4) | HSW_PWR_WELL_CTL_STATE(4));

	if (that->PG2 && getMember<int>(param_1, 0xb48) == 1) {
		getMember<u32>(param_1, 0xc24) = 0xffffffff;
	}
	/*if (that->PG2 && that->contr->dc6config == 1) {
		that->contr->fDC6EnabledRefCount = 0xffffffff;
	}*/

	that->DDIA = (ddi_state & (HSW_PWR_WELL_CTL_REQ(0) | HSW_PWR_WELL_CTL_STATE(0))) == (HSW_PWR_WELL_CTL_REQ(0) | HSW_PWR_WELL_CTL_STATE(0));
	that->DDIB = (ddi_state & (HSW_PWR_WELL_CTL_REQ(1) | HSW_PWR_WELL_CTL_STATE(1))) == (HSW_PWR_WELL_CTL_REQ(1) | HSW_PWR_WELL_CTL_STATE(1));
	that->DDIC = (ddi_state & (HSW_PWR_WELL_CTL_REQ(2) | HSW_PWR_WELL_CTL_STATE(2))) == (HSW_PWR_WELL_CTL_REQ(2) | HSW_PWR_WELL_CTL_STATE(2));
	that->TC1  = (ddi_state & (HSW_PWR_WELL_CTL_REQ(3) | HSW_PWR_WELL_CTL_STATE(3))) == (HSW_PWR_WELL_CTL_REQ(3) | HSW_PWR_WELL_CTL_STATE(3));
	that->TC2  = (ddi_state & (HSW_PWR_WELL_CTL_REQ(4) | HSW_PWR_WELL_CTL_STATE(4))) == (HSW_PWR_WELL_CTL_REQ(4) | HSW_PWR_WELL_CTL_STATE(4));
	that->TC3  = (ddi_state & (HSW_PWR_WELL_CTL_REQ(5) | HSW_PWR_WELL_CTL_STATE(5))) == (HSW_PWR_WELL_CTL_REQ(5) | HSW_PWR_WELL_CTL_STATE(5));
	that->TC4  = (ddi_state & (HSW_PWR_WELL_CTL_REQ(6) | HSW_PWR_WELL_CTL_STATE(6))) == (HSW_PWR_WELL_CTL_REQ(6) | HSW_PWR_WELL_CTL_STATE(6));
	that->TC5  = (ddi_state & (HSW_PWR_WELL_CTL_REQ(7) | HSW_PWR_WELL_CTL_STATE(7))) == (HSW_PWR_WELL_CTL_REQ(7) | HSW_PWR_WELL_CTL_STATE(7));
	that->TC6  = (ddi_state & (HSW_PWR_WELL_CTL_REQ(8) | HSW_PWR_WELL_CTL_STATE(8))) == (HSW_PWR_WELL_CTL_REQ(8) | HSW_PWR_WELL_CTL_STATE(8));

	that->AUXA   = (aux_state & (HSW_PWR_WELL_CTL_REQ(0) | HSW_PWR_WELL_CTL_STATE(0))) == (HSW_PWR_WELL_CTL_REQ(0) | HSW_PWR_WELL_CTL_STATE(0));
	that->AUXB   = (aux_state & (HSW_PWR_WELL_CTL_REQ(1) | HSW_PWR_WELL_CTL_STATE(1))) == (HSW_PWR_WELL_CTL_REQ(1) | HSW_PWR_WELL_CTL_STATE(1));
	that->AUXC   = (aux_state & (HSW_PWR_WELL_CTL_REQ(2) | HSW_PWR_WELL_CTL_STATE(2))) == (HSW_PWR_WELL_CTL_REQ(2) | HSW_PWR_WELL_CTL_STATE(2));
	that->auxTC1 = (aux_state & (HSW_PWR_WELL_CTL_REQ(3) | HSW_PWR_WELL_CTL_STATE(3))) == (HSW_PWR_WELL_CTL_REQ(3) | HSW_PWR_WELL_CTL_STATE(3));
	that->auxTC2 = (aux_state & (HSW_PWR_WELL_CTL_REQ(4) | HSW_PWR_WELL_CTL_STATE(4))) == (HSW_PWR_WELL_CTL_REQ(4) | HSW_PWR_WELL_CTL_STATE(4));
	that->AUXTC3 = (aux_state & (HSW_PWR_WELL_CTL_REQ(5) | HSW_PWR_WELL_CTL_STATE(5))) == (HSW_PWR_WELL_CTL_REQ(5) | HSW_PWR_WELL_CTL_STATE(5));
	that->AUXTC4 = (aux_state & (HSW_PWR_WELL_CTL_REQ(6) | HSW_PWR_WELL_CTL_STATE(6))) == (HSW_PWR_WELL_CTL_REQ(6) | HSW_PWR_WELL_CTL_STATE(6));
	that->AUXTC5 = (aux_state & (HSW_PWR_WELL_CTL_REQ(7) | HSW_PWR_WELL_CTL_STATE(7))) == (HSW_PWR_WELL_CTL_REQ(7) | HSW_PWR_WELL_CTL_STATE(7));
	that->AUXTC6 = (aux_state & (HSW_PWR_WELL_CTL_REQ(8) | HSW_PWR_WELL_CTL_STATE(8))) == (HSW_PWR_WELL_CTL_REQ(8) | HSW_PWR_WELL_CTL_STATE(8));

	if ((tbt_state & (HSW_PWR_WELL_CTL_REQ(0) | HSW_PWR_WELL_CTL_STATE(0))) == (HSW_PWR_WELL_CTL_REQ(0) | HSW_PWR_WELL_CTL_STATE(0))) {
		that->auxTC1 = 1;
	}
	if ((tbt_state & (HSW_PWR_WELL_CTL_REQ(1) | HSW_PWR_WELL_CTL_STATE(1))) == (HSW_PWR_WELL_CTL_REQ(1) | HSW_PWR_WELL_CTL_STATE(1))) {
		that->auxTC2 = 1;
	}
	if ((tbt_state & (HSW_PWR_WELL_CTL_REQ(2) | HSW_PWR_WELL_CTL_STATE(2))) == (HSW_PWR_WELL_CTL_REQ(2) | HSW_PWR_WELL_CTL_STATE(2))) {
		that->AUXTC3 = 1;
	}
	if ((tbt_state & (HSW_PWR_WELL_CTL_REQ(3) | HSW_PWR_WELL_CTL_STATE(3))) == (HSW_PWR_WELL_CTL_REQ(3) | HSW_PWR_WELL_CTL_STATE(3))) {
		that->AUXTC4 = 1;
	}
	if ((tbt_state & (HSW_PWR_WELL_CTL_REQ(4) | HSW_PWR_WELL_CTL_STATE(4))) == (HSW_PWR_WELL_CTL_REQ(4) | HSW_PWR_WELL_CTL_STATE(4))) that->AUXTC5 = 1;
	if ((tbt_state & (HSW_PWR_WELL_CTL_REQ(5) | HSW_PWR_WELL_CTL_STATE(5))) == (HSW_PWR_WELL_CTL_REQ(5) | HSW_PWR_WELL_CTL_STATE(5))) that->AUXTC6 = 1;


	bootPipe = probeBootPipe(that->contr, (bool *)0x0, &active_ddi);
	
	
	//if (bootPipe != 0xffff && that->PG1 == 0 && getMember<u32>(param_1, 0xd5c) /*that->contr->NumFrameBuffers*/ != 0) {
		enableDisplayEngine(that0);
	//}

	
	for (i = 0; i < 9; i++) {
		if (i == active_ddi) {
			if ((&that->AUXA)[i] != 0) {
				// Combo PHYs (DDI A, B, C) use AUX A, B, C which are safe to leave enabled.
				if (i >= 3) {
					int port_type = probePortMode(display, i);
					if (port_type != 2) {
						//drm_err(display->drm, "EFI should not enable AUX%d power well - overriding\n", i);
						disablePowerWellAux(that0, i);
						(&that->AUXA)[i] = 0; // Clear local state
					}
				}
			}
		} else {
			if ((&that->DDIA)[i] != 0) {
				//drm_err(display->drm, "EFI should not enable DDI%d power well - overriding\n", i);
				disablePowerWellDDI(that0, i);
				(&that->DDIA)[i] = 0; // Clear local state
			}
			if ((&that->AUXA)[i] != 0) {
				//drm_err(display->drm, "EFI should not enable AUX%d power well - overriding\n", i);
				disablePowerWellAux(that0, i);
				(&that->AUXA)[i] = 0; // Clear local state
			}
		}
	}


	if (bootPipe == 0) {
		if (pg5_enabled) { that->PG1 = 2; disablePowerWellPG(that0, 5); pg5_enabled = 0; }
		if (that->PG4) { that->PG1 = 2; disablePowerWellPG(that0, 4); that->PG4 = 0; }
		if (that->PG3) { that->PG1 = 2; disablePowerWellPG(that0, 3); that->PG3 = 0; }
		if (that->PG2) { that->PG1 = 2; disablePowerWellPG(that0, 2); that->PG2 = 0; }
	}
	else if (bootPipe == 1) {
		if (pg5_enabled) { that->PG1 = 2; that->PG2 = 2; that->PG3 = 2; that->PG4 = 2; disablePowerWellPG(that0, 5); pg5_enabled = 0; }
		if (that->PG4) { that->PG1 = 2; that->PG2 = 2; that->PG3 = 2; disablePowerWellPG(that0, 4); that->PG4 = 0; }
	}
	else if (bootPipe == 0xffff) {
		if (pg5_enabled) { disablePowerWellPG(that0, 5); pg5_enabled = 0; }
		if (that->PG4) { disablePowerWellPG(that0, 4); that->PG4 = 0; }
		if (that->PG3) { disablePowerWellPG(that0, 3); that->PG3 = 0; }
		if (that->PG2) { disablePowerWellPG(that0, 2); that->PG2 = 0; }
		if (that->PG1) { disablePowerWellPG(that0, 1); that->PG1 = 0; }
	}


	if (that->TC1 || that->TC2 || that->TC3 || that->TC4 || that->TC5 || that->TC6 ||
		that->auxTC1 || that->auxTC2 || that->AUXTC3 || that->AUXTC4 || that->AUXTC5 || that->AUXTC6) {
		tgl_tc_cold_request(display, true);
	} else {
		tgl_tc_cold_request(display, false);
	}

	if (that->powerwellalwaysON != '\0') {
		overridePowerWellsState(that0, true);
	}
}







static int
guc_mmio_reg_cmp(const void *a, const void *b)
{
	const struct guc_mmio_reg *ra = (const struct guc_mmio_reg *)a;
	const struct guc_mmio_reg *rb = (const struct guc_mmio_reg *)b;

	if (ra->offset > rb->offset)
		return 1;
	else if (ra->offset < rb->offset)
		return -1;
	
	return 0;
}

static struct guc_mmio_reg *
__mmio_reg_add(struct temp_regset *regset, struct guc_mmio_reg *reg)
{
	u32 pos = regset->storage_used;
	struct guc_mmio_reg *slot;

	if (pos >= regset->storage_max) {
		size_t new_size  = ALIGN((pos + 1) * sizeof(*slot), PAGE_SIZE);
		size_t old_bytes = regset->storage_max * sizeof(*slot);

		struct guc_mmio_reg *r =
			(struct guc_mmio_reg *)IOMalloc(new_size);
		if (!r)
			return (struct guc_mmio_reg *)ERR_PTR(-ENOMEM);

		if (regset->storage && regset->storage_used)
			memcpy(r, regset->storage,
				   regset->storage_used * sizeof(*slot));

		if (!regset->registers)
			regset->registers = r;
		else
			regset->registers = r + (regset->registers - regset->storage);

		if (regset->storage)
			IOFree(regset->storage, old_bytes);

		regset->storage     = r;
		regset->storage_max = new_size / sizeof(*slot);
	}

	slot = &regset->storage[pos];
	regset->storage_used++;
	*slot = *reg;

	return slot;
}

static void *
bsearch(const void *key, const void *base, size_t num, size_t size)
{
	const char *pivot;
	int result;

	while (num > 0) {
		pivot = (const char *)base + (num >> 1) * size;
		result = guc_mmio_reg_cmp(key, (const void *)pivot);

		if (result == 0)
			return (void *)pivot;

		if (result > 0) {
			base = pivot + size;
			num--;
		}
		num >>= 1;
	}

	return NULL;
}



static long guc_mmio_reg_add(struct intel_gt *gt,
					  struct temp_regset *regset,
					  u32 offset, u32 flags)
{
	u32 count = regset->storage_used - (regset->registers - regset->storage);
	struct guc_mmio_reg entry = {
		.offset = offset,
		.flags = flags,
	};
	struct guc_mmio_reg *slot;


	if (bsearch(&entry, regset->registers, count, sizeof(entry)))
		return 0;

	slot = __mmio_reg_add(regset, &entry);
	if (IS_ERR(slot))
		return PTR_ERR(slot);

	while (slot-- > regset->registers) {
		//GEM_BUG_ON(slot[0].offset == slot[1].offset);
		if (slot[1].offset > slot[0].offset)
			break;

		swap(slot[1], slot[0]);
	}

	return 0;
}

#define GUC_MMIO_REG_ADD(gt, regset, reg, masked) \
	guc_mmio_reg_add(gt, \
			 regset, \
			 i915_mmio_reg_offset(reg), \
			 (masked) ? GUC_REGSET_MASKED : 0)

#define GUC_REGSET_STEERING(group, instance) ( \
	FIELD_PREP(GUC_REGSET_STEERING_GROUP, (group)) | \
	FIELD_PREP(GUC_REGSET_STEERING_INSTANCE, (instance)) | \
	GUC_REGSET_NEEDS_STEERING \
)

static bool reg_needs_read_steering(struct intel_gt *gt,
									u32 reg,
					enum intel_steering_type type)
{
	u32 offset = i915_mmio_reg_offset(reg);
	const struct intel_mmio_range *entry;

	if ((!gt->steering_table[type]))
		return false;

	//if (IS_GSI_REG(offset))
	//	offset += gt->uncore->gsi_offset;

	for (entry = gt->steering_table[type]; entry->end; entry++) {
		if (offset >= entry->start && offset <= entry->end)
			return true;
	}

	return false;
}

static inline unsigned int
intel_sseu_find_first_xehp_dss(const struct sseu_dev_info *sseu, int groupsize,
				   int groupnum)
{
	return find_next_bit(sseu->subslice_mask.xehp,
				 XEHP_BITMAP_BITS(sseu->subslice_mask),
				 groupnum * groupsize);
}

static void get_nonterminated_steering(struct intel_gt *gt,
					   enum intel_steering_type type,
					   u8 *group, u8 *instance)
{
	u32 dss;

	switch (type) {
	case L3BANK:
		*group = 0;		/* unused */
		*instance = __ffs(gt->info.l3bank_mask);
		break;
	case MSLICE:
		//GEM_WARN_ON(!HAS_MSLICE_STEERING(gt->i915));
		*group = __ffs(gt->info.mslice_mask);
		*instance = 0;	/* unused */
		break;
	case LNCF:

		//GEM_WARN_ON(!HAS_MSLICE_STEERING(gt->i915));
		*group = __ffs(gt->info.mslice_mask) << 1;
		*instance = 0;	/* unused */
		break;
	case GAM:
		*group = IS_DG2(gt->i915) ? 1 : 0;
		*instance = 0;
		break;
	case DSS:
		dss = intel_sseu_find_first_xehp_dss(&gt->info.sseu, 0, 0);
		*group = dss / GEN_DSS_PER_GSLICE;
		*instance = dss % GEN_DSS_PER_GSLICE;
		break;
	case INSTANCE0:
		/*
		 * There are a lot of MCR types for which instance (0, 0)
		 * will always provide a non-terminated value.
		 */
		*group = 0;
		*instance = 0;
		break;
	case OADDRM:
		if ((VDBOX_MASK(gt) | VEBOX_MASK(gt) | gt->info.sfc_mask) & BIT(0))
			*group = 0;
		else
			*group = 1;
		*instance = 0;
		break;
	default:
		*group = 0;
		*instance = 0;
	}
}

void intel_gt_mcr_get_nonterminated_steering(struct intel_gt *gt,
						 u32 reg,
						 u8 *group, u8 *instance)
{
	int type;

	
	for (type = 0; type < static_cast<int>(NUM_STEERING_TYPES); type++) {
		if (reg_needs_read_steering(gt, reg, static_cast<enum intel_steering_type>(type))) {
			get_nonterminated_steering(gt, static_cast<enum intel_steering_type>(type), group, instance);
			return;
		}
	}

	*group = gt->default_steering.groupid;
	*instance = gt->default_steering.instanceid;
}

static long guc_mcr_reg_add(struct intel_gt *gt,
				struct temp_regset *regset,
							u32 reg, u32 flags)
{
	u8 group, inst;

	intel_gt_mcr_get_nonterminated_steering(gt, reg, &group, &inst);
	flags |= GUC_REGSET_STEERING(group, inst);

	return guc_mmio_reg_add(gt, regset, i915_mmio_reg_offset(reg), flags);
}

#define GUC_MCR_REG_ADD(gt, regset, reg, masked) \
	guc_mcr_reg_add(gt, \
			 regset, \
			 (reg), \
			 (masked) ? GUC_REGSET_MASKED : 0)

static int guc_mmio_regset_init(struct temp_regset *regset,
				struct intel_engine_cs *engine)
{
	struct intel_gt *gt = engine->gt;
	const u32 base = engine->mmio_base;
	struct i915_wa_list *wal = &engine->wa_list;
	struct i915_wa *wa;
	unsigned int i;
	int ret = 0;

	regset->registers = regset->storage + regset->storage_used;

	ret |= GUC_MMIO_REG_ADD(gt, regset, RING_MODE_GEN7(base), true);
	ret |= GUC_MMIO_REG_ADD(gt, regset, RING_HWS_PGA(base), false);
	ret |= GUC_MMIO_REG_ADD(gt, regset, RING_IMR(base), false);

	if ((engine->flags & I915_ENGINE_FIRST_RENDER_COMPUTE) &&
		CCS_MASK(engine->gt))
		ret |= GUC_MMIO_REG_ADD(gt, regset, GEN12_RCU_MODE, true);


	for (i = 0, wa = wal->list; i < wal->count; i++, wa++)
		ret |= GUC_MCR_REG_ADD(gt, regset, wa->mcr_reg, wa->masked_reg);

	for (i = 0; i < RING_MAX_NONPRIV_SLOTS; i++)
		ret |= GUC_MMIO_REG_ADD(gt, regset,
					RING_FORCE_TO_NONPRIV(base, i),
					false);

	for (i = 0; i < LNCFCMOCS_REG_COUNT; i++)
		if (GRAPHICS_VER_FULL(engine->i915) >= IP_VER(12, 55))
			ret |= GUC_MCR_REG_ADD(gt, regset, XEHP_LNCFCMOCS(i), false);
		else
			ret |= GUC_MMIO_REG_ADD(gt, regset, GEN9_LNCFCMOCS(i), false);


	
	if (GRAPHICS_VER(engine->i915) >= 12) {
		ret |= GUC_MCR_REG_ADD(gt, regset, MCR_REG(i915_mmio_reg_offset(EU_PERF_CNTL0)), false);
		ret |= GUC_MCR_REG_ADD(gt, regset, MCR_REG(i915_mmio_reg_offset(EU_PERF_CNTL1)), false);
		ret |= GUC_MCR_REG_ADD(gt, regset, MCR_REG(i915_mmio_reg_offset(EU_PERF_CNTL2)), false);
		ret |= GUC_MCR_REG_ADD(gt, regset, MCR_REG(i915_mmio_reg_offset(EU_PERF_CNTL3)), false);
		ret |= GUC_MCR_REG_ADD(gt, regset, MCR_REG(i915_mmio_reg_offset(EU_PERF_CNTL4)), false);
		ret |= GUC_MCR_REG_ADD(gt, regset, MCR_REG(i915_mmio_reg_offset(EU_PERF_CNTL5)), false);
		ret |= GUC_MCR_REG_ADD(gt, regset, MCR_REG(i915_mmio_reg_offset(EU_PERF_CNTL6)), false);
	}

	return ret ? -1 : 0;
}

static long guc_mmio_reg_state_create(struct intel_guc *guc)
{
	struct intel_gt *gt = guc->gt;
	struct intel_engine_cs *engine;
	enum intel_engine_id id;
	struct temp_regset temp_set = {};
	long total = 0;
	long ret;

	for_each_engine(engine, gt, id) {
		u32 used = temp_set.storage_used;

		ret = guc_mmio_regset_init(&temp_set, engine);
		if (ret < 0)
			goto fail_regset_init;

		guc->ads_regset_count[id] = temp_set.storage_used - used;
		total += guc->ads_regset_count[id];
	}

	guc->ads_regset = temp_set.storage;

	return total * sizeof(struct guc_mmio_reg);

fail_regset_init:
	//kfree(temp_set.storage);
	if (temp_set.storage)
			IOFree(temp_set.storage, temp_set.storage_max * sizeof(struct guc_mmio_reg));
	
	return ret;
}




static inline void iosys_map_incr(struct iosys_map *map, size_t incr)
{
	if (map->is_iomem)
		map->vaddr_iomem = static_cast<decltype(map->vaddr_iomem)>(static_cast<uint8_t*>(map->vaddr_iomem) + incr);
	else
		map->vaddr = static_cast<decltype(map->vaddr)>(static_cast<uint8_t*>(map->vaddr) + incr);
}

#define IOSYS_MAP_INIT_OFFSET(map_, offset_) ({				\
	struct iosys_map copy_ = *map_;					\
	iosys_map_incr(&copy_, offset_);				\
	copy_;								\
})

static inline bool iosys_map_is_null(const struct iosys_map *map)
{
	if (map->is_iomem)
		return !map->vaddr_iomem;
	return !map->vaddr;
}
static inline void iosys_map_set_vaddr(struct iosys_map *map, void *vaddr)
{
	map->vaddr = vaddr;
	map->is_iomem = false;
}

static u32 guc_ads_regset_size(struct intel_guc *guc)
{
	//GEM_BUG_ON(!guc->ads_regset_size);
	return guc->ads_regset_size;
}

static u32 guc_ads_regset_offset(struct intel_guc *guc)
{
	return offsetof(struct __guc_ads_blob, regset);
}

static u32 guc_ads_golden_ctxt_offset(struct intel_guc *guc)
{
	u32 offset;

	offset = guc_ads_regset_offset(guc) +
		 guc_ads_regset_size(guc);

	return PAGE_ALIGN(offset);
}







static void fill_engine_enable_masks(struct intel_gt *gt,
					 struct iosys_map *info_map)
{
	info_map_write(info_map, engine_enabled_masks[GUC_RENDER_CLASS], RCS_MASK(gt));
	info_map_write(info_map, engine_enabled_masks[GUC_COMPUTE_CLASS], CCS_MASK(gt));
	info_map_write(info_map, engine_enabled_masks[GUC_BLITTER_CLASS], BCS_MASK(gt));
	info_map_write(info_map, engine_enabled_masks[GUC_VIDEO_CLASS], VDBOX_MASK(gt));
	info_map_write(info_map, engine_enabled_masks[GUC_VIDEOENHANCE_CLASS], VEBOX_MASK(gt));

	if (gt->engine[GSC0])
		info_map_write(info_map, engine_enabled_masks[GUC_GSC_OTHER_CLASS],
				   BIT(gt->engine[GSC0]->instance));
}

static int guc_prep_golden_context(struct intel_guc *guc)
{
	struct intel_gt *gt = guc->gt;
	u32 addr_ggtt, offset;
	u32 total_size = 0, alloc_size, real_size;
	u8 engine_class, guc_class;
	struct guc_gt_system_info local_info;
	struct iosys_map info_map;


	if (!iosys_map_is_null(&guc->ads_map)) {
		offset = guc_ads_golden_ctxt_offset(guc);
		addr_ggtt = intel_guc_ggtt_offset(guc, guc->ads_vma) + offset;
		info_map = IOSYS_MAP_INIT_OFFSET(&guc->ads_map,
						 offsetof(struct __guc_ads_blob, system_info));
	} else {
		memset(&local_info, 0, sizeof(local_info));
		iosys_map_set_vaddr(&info_map, &local_info);
		fill_engine_enable_masks(gt, &info_map);
	}

	for (engine_class = 0; engine_class <= MAX_ENGINE_CLASS; ++engine_class) {
		guc_class = engine_class_to_guc_class(engine_class);

		if (!info_map_read(&info_map, engine_enabled_masks[guc_class]))
			continue;

		real_size = intel_engine_context_size(gt, engine_class);
		alloc_size = PAGE_ALIGN(real_size);
		total_size += alloc_size;

		if (iosys_map_is_null(&guc->ads_map))
			continue;


		ads_blob_write(guc, ads.eng_state_size[guc_class],
				   real_size - LRC_SKIP_SIZE(gt->i915));
		ads_blob_write(guc, ads.golden_context_lrca[guc_class],
				   addr_ggtt);

		addr_ggtt += alloc_size;
	}

//	if (guc->ads_golden_ctxt_size)
	//	GEM_BUG_ON(guc->ads_golden_ctxt_size != total_size);

	return total_size;
}

static u32 guc_ads_golden_ctxt_size(struct intel_guc *guc)
{
	return PAGE_ALIGN(guc->ads_golden_ctxt_size);
}

static u32 guc_ads_waklv_size(struct intel_guc *guc)
{
	return PAGE_ALIGN(guc->ads_waklv_size);
}

static u32 guc_ads_capture_size(struct intel_guc *guc)
{
	return PAGE_ALIGN(guc->ads_capture_size);
}

static u32 guc_ads_private_data_size(struct intel_guc *guc)
{
	return PAGE_ALIGN(guc->fw.private_data_size);
}


static u32 guc_ads_waklv_offset(struct intel_guc *guc)
{
	u32 offset;

	offset = guc_ads_golden_ctxt_offset(guc) +
		 guc_ads_golden_ctxt_size(guc);

	return PAGE_ALIGN(offset);
}
static u32 guc_ads_capture_offset(struct intel_guc *guc)
{
	u32 offset;

	offset = guc_ads_waklv_offset(guc) +
		 guc_ads_waklv_size(guc);

	return PAGE_ALIGN(offset);
}


int
intel_guc_capture_getnullheader(struct intel_guc *guc,
				void **outptr, size_t *size)
{
	struct intel_guc_state_capture *gc = guc->capture;
	int tmp = sizeof(u32) * 4;
	void *null_header;

	if (gc->ads_null_cache) {
		*outptr = gc->ads_null_cache;
		*size = tmp;
		return 0;
	}

	null_header = IOMalloc(tmp);//kzalloc(tmp, GFP_KERNEL);
	if (!null_header) {
		return -ENOMEM;
	}

	gc->ads_null_cache = null_header;
	*outptr = null_header;
	*size = tmp;

	return 0;
}

static inline void iosys_map_memcpy_to(struct iosys_map *dst, size_t dst_offset,
					   const void *src, size_t len)
{
	//if (dst->is_iomem)
	//	memcpy_toio(dst->vaddr_iomem + dst_offset, src, len);
	//else
		memcpy((u8*)dst->vaddr + dst_offset, src, len);
}

static u32 guc_get_capture_engine_mask(struct iosys_map *info_map, u32 capture_class)
{
	u32 mask;

	switch (capture_class) {
	case GUC_CAPTURE_LIST_CLASS_RENDER_COMPUTE:
		mask = info_map_read(info_map, engine_enabled_masks[GUC_RENDER_CLASS]);
		mask |= info_map_read(info_map, engine_enabled_masks[GUC_COMPUTE_CLASS]);
		break;

	case GUC_CAPTURE_LIST_CLASS_VIDEO:
		mask = info_map_read(info_map, engine_enabled_masks[GUC_VIDEO_CLASS]);
		break;

	case GUC_CAPTURE_LIST_CLASS_VIDEOENHANCE:
		mask = info_map_read(info_map, engine_enabled_masks[GUC_VIDEOENHANCE_CLASS]);
		break;

	case GUC_CAPTURE_LIST_CLASS_BLITTER:
		mask = info_map_read(info_map, engine_enabled_masks[GUC_BLITTER_CLASS]);
		break;

	case GUC_CAPTURE_LIST_CLASS_GSC_OTHER:
		mask = info_map_read(info_map, engine_enabled_masks[GUC_GSC_OTHER_CLASS]);
		break;

	default:
		mask = 0;
	}

	return mask;
}

static const struct __guc_mmio_reg_descr_group *
guc_capture_get_one_list(const struct __guc_mmio_reg_descr_group *reglists,
			 u32 owner, u32 type, u32 id)
{
	int i;

	if (!reglists)
		return NULL;

	for (i = 0; reglists[i].list; ++i) {
		if (reglists[i].owner == owner && reglists[i].type == type &&
			(reglists[i].engine == id || reglists[i].type == GUC_CAPTURE_LIST_TYPE_GLOBAL))
			return &reglists[i];
	}

	return NULL;
}

static struct __guc_mmio_reg_descr_group *
guc_capture_get_one_ext_list(struct __guc_mmio_reg_descr_group *reglists,
				 u32 owner, u32 type, u32 id)
{
	int i;

	if (!reglists)
		return NULL;

	for (i = 0; reglists[i].extlist; ++i) {
		if (reglists[i].owner == owner && reglists[i].type == type &&
			(reglists[i].engine == id || reglists[i].type == GUC_CAPTURE_LIST_TYPE_GLOBAL))
			return &reglists[i];
	}

	return NULL;
}

static int
guc_cap_list_num_regs(struct intel_guc_state_capture *gc, u32 owner, u32 type, u32 classid)
{
	const struct __guc_mmio_reg_descr_group *match;
	struct __guc_mmio_reg_descr_group *matchext;
	int num_regs;

	match = guc_capture_get_one_list(gc->reglists, owner, type, classid);
	if (!match)
		return 0;

	num_regs = match->num_regs;

	matchext = guc_capture_get_one_ext_list(gc->extlists, owner, type, classid);
	if (matchext)
		num_regs += matchext->num_regs;

	return num_regs;
}

static int
guc_capture_getlistsize(struct intel_guc *guc, u32 owner, u32 type, u32 classid,
			size_t *size, bool is_purpose_est)
{
	struct intel_guc_state_capture *gc = guc->capture;
	struct __guc_capture_ads_cache *cache = &gc->ads_cache[owner][type][classid];
	int num_regs;

	if (!gc->reglists) {
		return -ENODEV;
	}

	if (cache->is_valid) {
		*size = cache->size;
		return cache->status;
	}

	if (!is_purpose_est && owner == GUC_CAPTURE_LIST_INDEX_PF &&
		!guc_capture_get_one_list(gc->reglists, owner, type, classid)) {

		return -ENODATA;
	}

	num_regs = guc_cap_list_num_regs(gc, owner, type, classid);

	if (!num_regs)
		return -ENODATA;

	if (size)
		*size = PAGE_ALIGN((sizeof(struct guc_debug_capture_list)) +
				   (num_regs * sizeof(struct guc_mmio_reg)));

	return 0;
}

int
intel_guc_capture_getlistsize(struct intel_guc *guc, u32 owner, u32 type, u32 classid,
				  size_t *size)
{
	return guc_capture_getlistsize(guc, owner, type, classid, size, false);
}

static int
guc_capture_list_init(struct intel_guc *guc, u32 owner, u32 type, u32 classid,
			  struct guc_mmio_reg *ptr, u16 num_entries)
{
	u32 i = 0, j = 0;
	const struct __guc_mmio_reg_descr_group *reglists = guc->capture->reglists;
	struct __guc_mmio_reg_descr_group *extlists = guc->capture->extlists;
	const struct __guc_mmio_reg_descr_group *match;
	struct __guc_mmio_reg_descr_group *matchext;

	if (!reglists)
		return -ENODEV;

	match = guc_capture_get_one_list(reglists, owner, type, classid);
	if (!match)
		return -ENODATA;

	for (i = 0; i < num_entries && i < match->num_regs; ++i) {
		ptr[i].offset = match->list[i].reg;
		ptr[i].value = 0xDEADF00D;
		ptr[i].flags = match->list[i].flags;
		ptr[i].mask = match->list[i].mask;
	}

	matchext = guc_capture_get_one_ext_list(extlists, owner, type, classid);
	if (matchext) {
		for (i = match->num_regs, j = 0; i < num_entries &&
			 i < (match->num_regs + matchext->num_regs) &&
			j < matchext->num_regs; ++i, ++j) {
			ptr[i].offset = matchext->extlist[j].reg;
			ptr[i].value = 0xDEADF00D;
			ptr[i].flags = matchext->extlist[j].flags;
			ptr[i].mask = matchext->extlist[j].mask;
		}
	}


	return 0;
}

static void
guc_capture_init_node(struct intel_guc *guc, struct __guc_capture_parsed_output *node)
{
	struct guc_mmio_reg *tmp[GUC_CAPTURE_LIST_TYPE_MAX];
	int i;

	for (i = 0; i < GUC_CAPTURE_LIST_TYPE_MAX; ++i) {
		tmp[i] = node->reginfo[i].regs;
		memset(tmp[i], 0, sizeof(struct guc_mmio_reg) *
			   guc->capture->max_mmio_per_node);
	}
	memset(node, 0, sizeof(*node));
	for (i = 0; i < GUC_CAPTURE_LIST_TYPE_MAX; ++i)
		node->reginfo[i].regs = tmp[i];

	INIT_LIST_HEAD(&node->link);
}

static struct __guc_capture_parsed_output *
guc_capture_alloc_one_node(struct intel_guc *guc)
{
	struct __guc_capture_parsed_output *new2;
	int i;
	size_t regs_size = sizeof(struct guc_mmio_reg) * guc->capture->max_mmio_per_node;

	new2 = (struct __guc_capture_parsed_output *)IOMallocZero(sizeof(*new2));
	if (!new2)
		return NULL;

	for (i = 0; i < GUC_CAPTURE_LIST_TYPE_MAX; ++i) {
		new2->reginfo[i].regs = (struct guc_mmio_reg *)IOMallocZero(regs_size);
		if (!new2->reginfo[i].regs) {
			while (i) {
				IOFree(new2->reginfo[--i].regs, regs_size);
			}
			IOFree(new2, sizeof(*new2));
			return NULL;
		}
	}
	guc_capture_init_node(guc, new2);

	return new2;
}

static void
guc_capture_add_node_to_list(struct __guc_capture_parsed_output *node,
				 struct list_head *list)
{
	list_add_tail(&node->link, list);
}

static void
guc_capture_add_node_to_cachelist(struct intel_guc_state_capture *gc,
				  struct __guc_capture_parsed_output *node)
{
	guc_capture_add_node_to_list(node, &gc->cachelist);
}

static void
__guc_capture_create_prealloc_nodes(struct intel_guc *guc)
{
	struct __guc_capture_parsed_output *node = NULL;
	int i;

	for (i = 0; i < PREALLOC_NODES_MAX_COUNT; ++i) {
		node = guc_capture_alloc_one_node(guc);
		if (!node) {
			/* dont free the priors, use what we got and cleanup at shutdown */
			return;
		}
		guc_capture_add_node_to_cachelist(guc->capture, node);
	}
}

static int
guc_get_max_reglist_count(struct intel_guc *guc)
{
	int i, j, k, tmp, maxregcount = 0;

	for (i = 0; i < GUC_CAPTURE_LIST_INDEX_MAX; ++i) {
		for (j = 0; j < GUC_CAPTURE_LIST_TYPE_MAX; ++j) {
			for (k = 0; k < GUC_MAX_ENGINE_CLASSES; ++k) {
				if (j == GUC_CAPTURE_LIST_TYPE_GLOBAL && k > 0)
					continue;

				tmp = guc_cap_list_num_regs(guc->capture, i, j, k);
				if (tmp > maxregcount)
					maxregcount = tmp;
			}
		}
	}
	if (!maxregcount)
		maxregcount = PREALLOC_NODES_DEFAULT_NUMREGS;

	return maxregcount;
}

static void
guc_capture_create_prealloc_nodes(struct intel_guc *guc)
{
	if (guc->capture->max_mmio_per_node)
		return;

	guc->capture->max_mmio_per_node = guc_get_max_reglist_count(guc);
	__guc_capture_create_prealloc_nodes(guc);
}

int
intel_guc_capture_getlist(struct intel_guc *guc, u32 owner, u32 type, u32 classid,
			  void **outptr)
{
	struct intel_guc_state_capture *gc = guc->capture;
	struct __guc_capture_ads_cache *cache = &gc->ads_cache[owner][type][classid];
	struct guc_debug_capture_list *listnode;
	int ret, num_regs;
	u8 *caplist, *tmp;
	size_t size = 0;

	if (!gc->reglists)
		return -ENODEV;

	if (cache->is_valid) {
		*outptr = cache->ptr;
		return cache->status;
	}

	guc_capture_create_prealloc_nodes(guc);

	ret = intel_guc_capture_getlistsize(guc, owner, type, classid, &size);
	if (ret) {
		cache->is_valid = true;
		cache->ptr = NULL;
		cache->size = 0;
		cache->status = ret;
		return ret;
	}

	caplist = (u8*)IOMalloc(size);
	if (!caplist) {
		return -ENOMEM;
	}

	/* populate capture list header */
	tmp = caplist;
	num_regs = guc_cap_list_num_regs(guc->capture, owner, type, classid);
	listnode = (struct guc_debug_capture_list *)tmp;
	listnode->header.info = FIELD_PREP(GUC_CAPTURELISTHDR_NUMDESCR, (u32)num_regs);

	/* populate list of register descriptor */
	tmp += sizeof(struct guc_debug_capture_list);
	guc_capture_list_init(guc, owner, type, classid, (struct guc_mmio_reg *)tmp, num_regs);

	/* cache this list */
	cache->is_valid = true;
	cache->ptr = caplist;
	cache->size = size;
	cache->status = 0;

	*outptr = caplist;

	return 0;
}

static int
guc_capture_prep_lists(struct intel_guc *guc)
{
	struct intel_gt *gt = guc->gt;
	u32 ads_ggtt, capture_offset, null_ggtt, total_size = 0;
	struct guc_gt_system_info local_info;
	struct iosys_map info_map;
	bool ads_is_mapped;
	size_t size = 0;
	void *ptr;
	int i, j;

	ads_is_mapped = !iosys_map_is_null(&guc->ads_map);
	if (ads_is_mapped) {
		capture_offset = guc_ads_capture_offset(guc);
		ads_ggtt = intel_guc_ggtt_offset(guc, guc->ads_vma);
		info_map = IOSYS_MAP_INIT_OFFSET(&guc->ads_map,
						 offsetof(struct __guc_ads_blob, system_info));
	} else {
		memset(&local_info, 0, sizeof(local_info));
		iosys_map_set_vaddr(&info_map, &local_info);
		fill_engine_enable_masks(gt, &info_map);
	}

	total_size = PAGE_SIZE;
	if (ads_is_mapped) {
		if (!intel_guc_capture_getnullheader(guc, &ptr, &size))
			iosys_map_memcpy_to(&guc->ads_map, capture_offset, ptr, size);
		null_ggtt = ads_ggtt + capture_offset;
		capture_offset += PAGE_SIZE;
	}

	for (i = 0; i < GUC_CAPTURE_LIST_INDEX_MAX; i++) {
		for (j = 0; j < GUC_MAX_ENGINE_CLASSES; j++) {
			u32 engine_mask = guc_get_capture_engine_mask(&info_map, j);

			if (!engine_mask) {
				if (ads_is_mapped) {
					ads_blob_write(guc, ads.capture_class[i][j], null_ggtt);
					ads_blob_write(guc, ads.capture_instance[i][j], null_ggtt);
				}
				continue;
			}
			if (intel_guc_capture_getlistsize(guc, i,
							  GUC_CAPTURE_LIST_TYPE_ENGINE_CLASS,
							  j, &size)) {
				if (ads_is_mapped)
					ads_blob_write(guc, ads.capture_class[i][j], null_ggtt);
				goto engine_instance_list;
			}
			total_size += size;
			if (ads_is_mapped) {
				if (total_size > guc->ads_capture_size ||
					intel_guc_capture_getlist(guc, i,
								  GUC_CAPTURE_LIST_TYPE_ENGINE_CLASS,
								  j, &ptr)) {
					ads_blob_write(guc, ads.capture_class[i][j], null_ggtt);
					continue;
				}
				ads_blob_write(guc, ads.capture_class[i][j], ads_ggtt +
						   capture_offset);
				iosys_map_memcpy_to(&guc->ads_map, capture_offset, ptr, size);
				capture_offset += size;
			}
engine_instance_list:
			if (intel_guc_capture_getlistsize(guc, i,
							  GUC_CAPTURE_LIST_TYPE_ENGINE_INSTANCE,
							  j, &size)) {
				if (ads_is_mapped)
					ads_blob_write(guc, ads.capture_instance[i][j], null_ggtt);
				continue;
			}
			total_size += size;
			if (ads_is_mapped) {
				if (total_size > guc->ads_capture_size ||
					intel_guc_capture_getlist(guc, i,
								  GUC_CAPTURE_LIST_TYPE_ENGINE_INSTANCE,
								  j, &ptr)) {
					ads_blob_write(guc, ads.capture_instance[i][j], null_ggtt);
					continue;
				}
				ads_blob_write(guc, ads.capture_instance[i][j], ads_ggtt +
						   capture_offset);
				iosys_map_memcpy_to(&guc->ads_map, capture_offset, ptr, size);
				capture_offset += size;
			}
		}
		if (intel_guc_capture_getlistsize(guc, i, GUC_CAPTURE_LIST_TYPE_GLOBAL, 0, &size)) {
			if (ads_is_mapped)
				ads_blob_write(guc, ads.capture_global[i], null_ggtt);
			continue;
		}
		total_size += size;
		if (ads_is_mapped) {
			if (total_size > guc->ads_capture_size ||
				intel_guc_capture_getlist(guc, i, GUC_CAPTURE_LIST_TYPE_GLOBAL, 0,
							  &ptr)) {
				ads_blob_write(guc, ads.capture_global[i], null_ggtt);
				continue;
			}
			ads_blob_write(guc, ads.capture_global[i], ads_ggtt + capture_offset);
			iosys_map_memcpy_to(&guc->ads_map, capture_offset, ptr, size);
			capture_offset += size;
		}
	}


	return PAGE_ALIGN(total_size);
}

static int guc_prep_waklv(struct intel_guc *guc)
{
	return PAGE_SIZE;
}

static u32 guc_ads_private_data_offset(struct intel_guc *guc)
{
	u32 offset;

	offset = guc_ads_capture_offset(guc) +
		 guc_ads_capture_size(guc);

	return PAGE_ALIGN(offset);
}

static u32 guc_ads_blob_size(struct intel_guc *guc)
{
	return guc_ads_private_data_offset(guc) +
		   guc_ads_private_data_size(guc);
}

static void guc_policies_init(struct intel_guc *guc)
{
	struct intel_gt *gt = guc->gt;
	struct drm_i915_private *i915 = gt->i915;
	u32 global_flags = 0;

	ads_blob_write(guc, policies.dpc_promote_time,
			   GLOBAL_POLICY_DEFAULT_DPC_PROMOTE_TIME_US);
	ads_blob_write(guc, policies.max_num_work_items,
			   GLOBAL_POLICY_MAX_NUM_WI);

	//if (i915->params.reset < 2)
	//	global_flags |= GLOBAL_POLICY_DISABLE_ENGINE_RESET;

	ads_blob_write(guc, policies.global_flags, global_flags);
	ads_blob_write(guc, policies.is_valid, 1);
}


static void guc_mapping_table_init(struct intel_gt *gt,
				   struct iosys_map *info_map)
{
	unsigned int i, j;
	struct intel_engine_cs *engine;
	enum intel_engine_id id;

	/* Table must be set to invalid values for entries not used */
	for (i = 0; i < GUC_MAX_ENGINE_CLASSES; ++i)
		for (j = 0; j < GUC_MAX_INSTANCES_PER_CLASS; ++j)
			info_map_write(info_map, mapping_table[i][j],
					   GUC_MAX_INSTANCES_PER_CLASS);

	for_each_engine(engine, gt, id) {
		u8 guc_class = engine_class_to_guc_class(engine->classb);

		info_map_write(info_map, mapping_table[guc_class][ilog2(engine->logical_mask)],
				   engine->instance);
	}
}

static void guc_mmio_reg_state_init(struct intel_guc *guc)
{
	struct intel_gt *gt = guc->gt;
	struct intel_engine_cs *engine;
	enum intel_engine_id id;
	u32 addr_ggtt, offset;

	offset = guc_ads_regset_offset(guc);
	addr_ggtt = intel_guc_ggtt_offset(guc, guc->ads_vma) + offset;

	iosys_map_memcpy_to(&guc->ads_map, offset, guc->ads_regset,
				guc->ads_regset_size);

	for_each_engine(engine, gt, id) {
		u32 count = guc->ads_regset_count[id];
		u8 guc_class;

		//GEM_BUG_ON(engine->instance >= GUC_MAX_INSTANCES_PER_CLASS);

		guc_class = engine_class_to_guc_class(engine->classb);

		if (!count) {
			ads_blob_write(guc,
					   ads.reg_state_list[guc_class][engine->instance].address,
					   0);
			ads_blob_write(guc,
					   ads.reg_state_list[guc_class][engine->instance].count,
					   0);
			continue;
		}

		ads_blob_write(guc,
				   ads.reg_state_list[guc_class][engine->instance].address,
				   addr_ggtt);
		ads_blob_write(guc,
				   ads.reg_state_list[guc_class][engine->instance].count,
				   count);

		addr_ggtt += count * sizeof(struct guc_mmio_reg);
	}
}

static void __guc_ads_init(struct intel_guc *guc)
{
	struct intel_gt *gt = guc->gt;
	struct drm_i915_private *i915 = gt->i915;
	struct intel_display *display=i915->display;
	struct iosys_map info_map = IOSYS_MAP_INIT_OFFSET(&guc->ads_map,
			offsetof(struct __guc_ads_blob, system_info));
	u32 base;

	guc_policies_init(guc);

	fill_engine_enable_masks(gt, &info_map);

	ads_blob_write(guc, system_info.generic_gt_sysinfo[GUC_GENERIC_GT_SYSINFO_SLICE_ENABLED],
			   hweight8(gt->info.sseu.slice_mask));
	ads_blob_write(guc, system_info.generic_gt_sysinfo[GUC_GENERIC_GT_SYSINFO_VDBOX_SFC_SUPPORT_MASK],
			   gt->info.vdbox_sfc_access);

	if (GRAPHICS_VER(i915) >= 12 && !IS_DGFX(i915)) {
		u32 distdbreg = intel_de_read(display,
						  GEN12_DIST_DBS_POPULATED);
		ads_blob_write(guc,
				   system_info.generic_gt_sysinfo[GUC_GENERIC_GT_SYSINFO_DOORBELL_COUNT_PER_SQIDI],
				   ((distdbreg >> GEN12_DOORBELLS_PER_SQIDI_SHIFT)
				& GEN12_DOORBELLS_PER_SQIDI) + 1);
	}

	guc_prep_golden_context(guc);

	guc_mapping_table_init(guc->gt, &info_map);

	base = intel_guc_ggtt_offset(guc, guc->ads_vma);

	guc_capture_prep_lists(guc);

	ads_blob_write(guc, ads.scheduler_policies, base +
			   offsetof(struct __guc_ads_blob, policies));
	ads_blob_write(guc, ads.gt_system_info, base +
			   offsetof(struct __guc_ads_blob, system_info));

	guc_mmio_reg_state_init(guc);

	//guc_waklv_init(guc);

	ads_blob_write(guc, ads.private_data, base +
			   guc_ads_private_data_offset(guc));

	//i915_gem_object_flush_map(guc->ads_vma->obj);
}

static void intel_gt_mcr_get_ss_steering(struct intel_gt *gt, unsigned int dss,
				   unsigned int *group, unsigned int *instance)
{
	if (GRAPHICS_VER_FULL(gt->i915) >= IP_VER(12, 55)) {
		*group = dss / GEN_DSS_PER_GSLICE;
		*instance = dss % GEN_DSS_PER_GSLICE;
	} else {
		*group = dss / GEN_MAX_SS_PER_HSW_SLICE;
		*instance = dss % GEN_MAX_SS_PER_HSW_SLICE;
		return;
	}
}


#define _HAS_SS(ss_, gt_, group_, instance_) ( \
	GRAPHICS_VER_FULL(gt_->i915) >= IP_VER(12, 55) ? \
		intel_sseu_has_subslice(&(gt_)->info.sseu, 0, ss_) : \
		intel_sseu_has_subslice(&(gt_)->info.sseu, group_, instance_))

#define for_each_ss_steering(ss_, gt_, group_, instance_) \
	for (ss_ = 0, intel_gt_mcr_get_ss_steering(gt_, 0, &group_, &instance_); \
		 ss_ < I915_MAX_SS_FUSE_BITS; \
		 ss_++, intel_gt_mcr_get_ss_steering(gt_, ss_, &group_, &instance_)) \
		for_each_if(_HAS_SS(ss_, gt_, group_, instance_))


static int
__alloc_ext_regs(struct __guc_mmio_reg_descr_group *newlist,
		 const struct __guc_mmio_reg_descr_group *rootlist, int num_regs)
{
	struct __guc_mmio_reg_descr *list;

	//list = kzalloc_objs(struct __guc_mmio_reg_descr, num_regs);
	list = (struct __guc_mmio_reg_descr *)IOMalloc(num_regs*sizeof(struct __guc_mmio_reg_descr));
	
	if (!list)
		return -ENOMEM;

	newlist->extlist = list;
	newlist->num_regs = num_regs;
	newlist->owner = rootlist->owner;
	newlist->engine = rootlist->engine;
	newlist->type = rootlist->type;

	return 0;
}

static void __fill_ext_reg(struct __guc_mmio_reg_descr *ext,
			   const struct __ext_steer_reg *extlist,
			   int slice_id, int subslice_id)
{
	ext->reg = _MMIO(i915_mmio_reg_offset(extlist->reg));
	ext->flags = FIELD_PREP(GUC_REGSET_STEERING_GROUP, slice_id);
	ext->flags |= FIELD_PREP(GUC_REGSET_STEERING_INSTANCE, subslice_id);
	ext->regname = extlist->name;
}

static void
guc_capture_alloc_steered_lists(struct intel_guc *guc,
				const struct __guc_mmio_reg_descr_group *lists)
{
	struct intel_gt *gt = guc->gt;
	int  iter, i, num_steer_regs, num_tot_regs = 0;
	const struct __guc_mmio_reg_descr_group *list;
	struct __guc_mmio_reg_descr_group *extlists;
	struct __guc_mmio_reg_descr *extarray;
	bool has_xehpg_extregs;
	unsigned int slice,subslice;
	
	list = guc_capture_get_one_list(lists, GUC_CAPTURE_LIST_INDEX_PF,
					GUC_CAPTURE_LIST_TYPE_ENGINE_CLASS,
					GUC_CAPTURE_LIST_CLASS_RENDER_COMPUTE);

	if (!list || guc->capture->extlists)
		return;

	has_xehpg_extregs = GRAPHICS_VER_FULL(gt->i915) >= IP_VER(12, 55);

	num_steer_regs = ARRAY_SIZE(gen8_extregs);
	if (has_xehpg_extregs)
		num_steer_regs += ARRAY_SIZE(xehpg_extregs);

	for_each_ss_steering(iter, gt, slice, subslice)
		num_tot_regs += num_steer_regs;

	if (!num_tot_regs)
		return;

	//extlists = kzalloc_objs(struct __guc_mmio_reg_descr_group, 2);
	extlists = (struct __guc_mmio_reg_descr_group *)IOMalloc(2*sizeof(struct __guc_mmio_reg_descr_group));
	if (!extlists)
		return;

	if (__alloc_ext_regs(&extlists[0], list, num_tot_regs)) {
		//kfree(extlists);
		IOFree(extlists, 2*sizeof(struct __guc_mmio_reg_descr_group));
		return;
	}

	extarray = extlists[0].extlist;
	for_each_ss_steering(iter, gt, slice, subslice) {
		for (i = 0; i < ARRAY_SIZE(gen8_extregs); ++i) {
			__fill_ext_reg(extarray, &gen8_extregs[i], slice, subslice);
			++extarray;
		}

		if (has_xehpg_extregs) {
			for (i = 0; i < ARRAY_SIZE(xehpg_extregs); ++i) {
				__fill_ext_reg(extarray, &xehpg_extregs[i], slice, subslice);
				++extarray;
			}
		}
	}

	guc->capture->extlists = extlists;
}

static const struct __guc_mmio_reg_descr_group *
guc_capture_get_device_reglist(struct intel_guc *guc)
{
	struct drm_i915_private *i915 = guc->gt->i915;
	const struct __guc_mmio_reg_descr_group *lists;

	//if (GRAPHICS_VER(i915) >= 12)
		lists = xe_lp_lists;

	guc_capture_alloc_steered_lists(guc, lists);

	return lists;
}

int intel_guc_capture_init(struct intel_guc *guc)
{
	guc->capture = (struct intel_guc_state_capture*)IOMalloc(sizeof(*guc->capture));
	if (!guc->capture)
		return -ENOMEM;

	guc->capture->reglists = guc_capture_get_device_reglist(guc);

	INIT_LIST_HEAD(&guc->capture->outlist);
	INIT_LIST_HEAD(&guc->capture->cachelist);

	//check_guc_capture_size(guc);

	return 0;
}

uint64_t Gen11::setupAdditionalDataStructs(void *that0) {
	
	struct drm_i915_private *i915= NBlue::callback->i915b;
	struct intel_display *display = i915->display;
	struct intel_gt *gt=to_gt(i915);
	struct intel_guc *guc = gt_to_guc(gt);
	int ret;
	u32 size;
	
	intel_guc_capture_init(guc);
	
	ret = guc_mmio_reg_state_create(guc);
	if (ret < 0)
		return ret;
	guc->ads_regset_size = ret;

	ret = guc_prep_golden_context(guc);
	if (ret < 0)
		return ret;
	guc->ads_golden_ctxt_size = ret;

	ret = guc_capture_prep_lists(guc);
	if (ret < 0)
		return ret;
	guc->ads_capture_size = ret;

	ret = guc_prep_waklv(guc);
	if (ret < 0)
		return ret;
	guc->ads_waklv_size = ret;

	size = guc_ads_blob_size(guc);

	//ret = intel_guc_allocate_and_map_vma(guc, size, &guc->ads_vma, &ads_blob);
	//iosys_map_set_vaddr(&guc->ads_map, ads_blob);
	
	void *acel=getMember<void *>(that0, 0x38);
	void *field_0x150=getMember<void *>(acel, 0x150);
	guc->ads_vma = IGSharedMappedBufferwithOptions(field_0x150, size, 2, 0);
	if (!guc->ads_vma) return 1;
	getMember<void *>(that0, 0x9e8)= guc->ads_vma;
	guc->ads_vma= (void *)fgetVirtualAddress(guc->ads_vma);
	guc->ads_map.vaddr = guc->ads_vma;
	guc->ads_map.is_iomem = false;
	
	
	__guc_ads_init(guc);

	
	
	u32 ads_param_val = ((u32)(fgetGPUVirtualAddress(getMember<void *>(that0, 0x9e8)) >> PAGE_SHIFT) << GUC_ADS_ADDR_SHIFT);
	u32 a0=getMember<u32>(that0, 0xa0);
	getMember<u32>(that0, 0xa0) = (a0 & 0xFFC00001) | ads_param_val;
	
	//intel_guc_ct_init
	
	//IGSharedMappedBufferfree(guc->ads_vma);
	
	//if ((that->acel->capabilities & 0x20) != 0) {
	//	IntelAccelerator::transferOwnership(that->acel, (int)that->field_0x9e8);
	//}
	return 0;
}
