/*
 * =====================================================================================
 *
 *       Filename:  vancount-z3.c
 *
 *    Description:  read and write tokens.
 *
 *        Version:  1.0
 *        Created:  2016/4/28 14:10:40
 *       Revision:  none
 *       Compiler:
 *
 *         Author:  tie tao (tt), catch.tao@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include "app/framework/include/af.h"
#include "./vancount-z3-net.h"
#include "./vancount-z3.h"
#include "./list.h"
#include "./vc-onoff.h"
#include "./vc-scenes.h"
#include "./vc-device-shade.h"
#include "vancount-customer-extend.h"

EmberEventControl customButtonControlEventControl;
EmberEventControl customButtonEventControl;
EmberEventControl customOnOffEventControl;

static uint8_t cmd = 0;
EmberEventControl meteringTestEventControl;
static uint8_t sButtonCount = 0;
static uint8_t sLightIndicatorCount;
void vcEmberAfOnOffOutputCallback(uint8_t newValue);
void (*eventCallback)(void);

extern void vc3In1TokensInit(void);
extern uint8_t vc3In1GetNetType(void);
extern void vc3In1EndpointInit(uint8_t modelType);

static uint8_t  sModelType;
static int8_t   sTxPower;
uint8_t  sStartupState = 0;

extern void vc3In1Init(void);
extern void vc3In1TokensInit(void);
extern void vc3In1ClusterAttributeInit(void);
extern uint8_t vc3In1GetModeType(void);
uint8_t vcDeiceIsReady(void)
{
return sStartupState;
}

void meteringTestEventHandler(void)
{

}


/** @brief Main Init
 *
 * This function is called from the application's main function. It gives the
 * application a chance to do any initialization required at system startup.
 * Any code that you would normally put into the top of the application's
 * main() routine should be put into this function.
        Note: No callback
 * in the Application Framework is associated with resource cleanup. If you
 * are implementing your application on a Unix host where resource cleanup is
 * a consideration, we expect that you will use the standard Posix system
 * calls, including the use of atexit() and handlers for signals such as
 * SIGTERM, SIGINT, SIGCHLD, SIGPIPE and so on. If you use the signal()
 * function to register your signal handler, please mind the returned value
 * which may be an Application Framework function. If the return value is
 * non-null, please make sure that you call the returned function from your
 * handler to avoid negating the resource cleanup of the Application Framework
 * itself.
 *
 */
extern void vcNetworkParametersInit(uint8_t isSleep);

boolean emberAfMainStartCallback(int* returnCode,
                              int argc,
                              char** argv)
{
  /*vcGetModelType(&sModelType);*/
  /*sModelType = MODEL_TYPE_LIGHT_SENSOR_AND_OCCUPANCY_SENSOR;*/
  sModelType = 0;
  emberAfCorePrintln("MainStart sModelType: %d", sModelType);
  return FALSE;
}


extern void vcHalButtonSetISR(uint8_t pin);
void emberAfMainInitCallback(void)
{
  //halLedBlinkLedOff(0);
  sStartupState = 0;
  vcHalButtonSetISR(0);
  vcLEDInit();
  vcLightInit();
  vcReportingIntervalStart();
  //vcZ3CommissionActive(1000);
  emberEventControlSetDelayMS(customButtonEventControl, 1100);
  if (vcMfGetManufactureStateCallback() == 0) {
	  vcZ3CommissionActive(2000);
  }
}

void emberAfMainTickCallback(void)
{
  static uint8_t init = 0;
  /*static uint32_t lastTime;*/
  /*uint32_t currentTime;*/

  if (init == 0) {
    //vc3In1ClusterAttributeInit();
    /*vc3In1NetInit(vc3In1GetNetType());*/
    init = 1;

    // carel_add
	extern void _OEM_Handler(void);
	extern void OEM_Start(void);
	OEM_Start();				// 读OEM_SELECT、授权属性2
	_OEM_Handler();				// 写ModelId、MenuName、授权属性1、授权属性2

	//
  }
}

void vc3In1TokensInit(void)
{

}

void vc3In1ClusterAttributeInit(void)
{
	vcShadeAttributeInit();
}

void vc3In1EndpointInit(uint8_t modelType)
{


}

uint8_t vc3In1GetNetType(void)
{
   return 0;
}

uint8_t vc3In1GetModeType(void)
{
  return 0;
}

/** @brief Start Feedback
 *
 * This function is called by the Identify plugin when identification begins.
 * It informs the Identify Feedback plugin that it should begin providing its
 * implemented feedback functionality (e.g. LED blinking, buzzer sounding,
 * etc.) until the Identify plugin tells it to stop. The identify time is
 * purely a matter of informational convenience; this plugin does not need to
 * know how long it will identify (the Identify plugin will perform the
 * necessary timekeeping.)
 *
 * @param endpoint The identifying endpoint Ver.: always
 * @param identifyTime The identify time Ver.: always
 */
void emberAfPluginIdentifyStartFeedbackCallback(uint8_t endpoint,
                                                uint16_t identifyTime)
{
	if (sStartupState == 0) {
		return;
	}
	emberAfCorePrintln("%p, %d", __func__, __LINE__);
	//vcSetLedHandleState(LED_HANDLE_RECOEVERY_END);
	//vcLEDBlink(VC_IDENTIFY_LED_INDEX, 1, 20);
}

/** @brief Stop Feedback
 *
 * This function is called by the Identify plugin when identification is
 * finished. It tells the Identify Feedback plugin to stop providing its
 * implemented feedback functionality.
 *
 * @param endpoint The identifying endpoint Ver.: always
 */
void emberAfPluginIdentifyStopFeedbackCallback(uint8_t endpoint)
{
	EmberAfStatus status;
	uint8_t onOff = 0;

	if (sStartupState == 0) {
		return;
	}

//	if (vcGetNodeModeState(endpoint) == VC_NORMAL_SWITCH
//			|| vcGetNodeModeState(endpoint) == VC_DOUBLE_CONTROL_SWITCH) {
//		status = emberAfReadAttribute(emberAfEndpointFromIndex(0),
//		ZCL_ON_OFF_CLUSTER_ID,
//		ZCL_ON_OFF_ATTRIBUTE_ID,
//		CLUSTER_MASK_SERVER, (uint8_t *) &onOff, sizeof(onOff),
//		NULL);
//		emberAfCorePrintln("%p, %d", __func__, __LINE__);
//		vcLightIndicatorControl(VC_IDENTIFY_LED_INDEX, onOff);
//	}
}
/** @brief Server Attribute Changed
 *
 * On/off cluster, Server Attribute Changed
 *
 * @param endpoint Endpoint that is being initialized  Ver.: always
 * @param attributeId Attribute that changed  Ver.: always
 */
extern void vcShadeOperateFromLevel(uint8_t endpoint);
void emberAfOnOffClusterServerAttributeChangedCallback(uint8_t endpoint,
                                                       EmberAfAttributeId attributeId)
{
  // When the on/off attribute changes, set the LED appropriately.  If an error
  // occurs, ignore it because there's really nothing we can do.
  if (attributeId == ZCL_ON_OFF_ATTRIBUTE_ID && sStartupState) {
    bool onOff;
    uint16_t level;
    if (emberAfReadServerAttribute(endpoint,
                                   ZCL_ON_OFF_CLUSTER_ID,
                                   ZCL_ON_OFF_ATTRIBUTE_ID,
                                   (uint8_t *)&onOff,
                                   sizeof(onOff))
        == EMBER_ZCL_STATUS_SUCCESS) {
    	emberAfCorePrintln("onOffCallback%d", onOff);
            //vcSwControl(endpoint, onOff);
            //vcReportAnAttribute(0x00, endpoint, 0x01, ZCL_ON_OFF_CLUSTER_ID, ZCL_ON_OFF_ATTRIBUTE_ID, 1);
            //vcResetReportingInterval();
    	if (onOff) {
    		level = 0xFE;
    	} else {
    		level = 1;
    	}
    	if (vc_is_level_action_when_onoff()) {
    		emberAfWriteServerAttribute(endpoint,
    		ZCL_LEVEL_CONTROL_CLUSTER_ID,
    		ZCL_CURRENT_LEVEL_ATTRIBUTE_ID,
    				(uint8_t *) &level,
    				ZCL_INT8U_ATTRIBUTE_TYPE);
    		vcShadeOperateFromLevel(endpoint);
    	}
    	vc_level_action_when_onoff();
    }
  }
}

void vcAfLevelControlClusterServerAttributeChangedCallback(uint8_t endpoint,
                                                       EmberAfAttributeId attributeId)
{
  // When the level attribute changes, set the LED appropriately.  If an error
  // occurs, ignore it because there's really nothing we can do.
//	emberAfCorePrintln("%p, %d", __func__, __LINE__);
//  if (attributeId == ZCL_CURRENT_LEVEL_ATTRIBUTE_ID && sStartupState) {
//      emberAfCorePrintln("ep:%d, level changed", endpoint);
//	  vcShadeOperateFromLevel(endpoint);
//  }
}

void emberAfPluginOnOffClusterServerPostInitCallback(uint8_t endpoint)
{
  // At startup, trigger a read of the attribute and possibly a toggle of the
  // LED to make sure they are always in sync.
  emberAfOnOffClusterServerAttributeChangedCallback(endpoint,
                                                    ZCL_ON_OFF_ATTRIBUTE_ID);
}
/** @brief Allow Network Write Attribute
 *
 * This function is called by the application framework before it writes an
 * attribute in response to a write attribute request from an external device.
 * The value passed into this callback is the value to which the attribute is to
 * be set by the framework.
        Example:	In mirroring simple metering data
 * on an Energy Services Interface (ESI) (formerly called Energy Service Portal
 * (ESP) in SE 1.0).), a mirrored simple meter needs to write read-only
 * attributes on its mirror. The-meter-mirror sample application, located in
 * app/framework/sample-apps, uses this callback to allow the mirrored device to
 * write simple metering attributes on the mirror regardless of the fact that
 * most simple metering attributes are defined as read-only by the ZigBee
 * specification.
        Note:	The ZCL specification does not (as of this
 * writing) specify any permission-level security for writing writeable
 * attributes. As far as the ZCL specification is concerned, if an attribute is
 * writeable, any device that has a link key for the device should be able to
 * write that attribute. Furthermore if an attribute is read only, it should not
 * be written over the air. Thus, if you implement permissions for writing
 * attributes as a feature, you MAY be operating outside the specification. This
 * is unlikely to be a problem for writing read-only attributes, but it may be a
 * problem for attributes that are writeable according to the specification but
 * restricted by the application implementing this callback.
 *
 * @param endpoint   Ver.: always
 * @param clusterId   Ver.: always
 * @param attributeId   Ver.: always
 * @param mask   Ver.: always
 * @param manufacturerCode   Ver.: always
 * @param value   Ver.: always
 * @param type   Ver.: always
 */

EmberAfAttributeWritePermission emberAfAllowNetworkWriteAttributeCallback(uint8_t endpoint,
                                                                          EmberAfClusterId clusterId,
                                                                          EmberAfAttributeId attributeId,
                                                                          uint8_t mask,
                                                                          uint16_t manufacturerCode,
                                                                          uint8_t* value,
                                                                          uint8_t type)
{
	vcShadeSetNodeConfig(endpoint, clusterId, attributeId, value);

	// carel_add
	if(clusterId == 0x0000){
		// 厂商配置
		if(attributeId == 0xFC00 || attributeId == 0xFC01){
			// !0 - 不允许写
			if(vcMfGetManufactureStateCallback()){
				return EMBER_ZCL_ATTRIBUTE_WRITE_PERMISSION_DENY_WRITE;
			}
			// 0 - 厂测允许写
			else{
				return EMBER_ZCL_ATTRIBUTE_WRITE_PERMISSION_ALLOW_WRITE_NORMAL;
			}
		}
	}
	//


  return EMBER_ZCL_ATTRIBUTE_WRITE_PERMISSION_ALLOW_WRITE_NORMAL; // Default
}

void VcMfgExtensionClusterTransparentSendResponse(uint8_t *data, uint8_t size)
{
	emberAfFillCommandVcMfgExtensionClusterTransparentResponse(data, size);
    emberAfSendResponse();
}

#include "vancount-zigbee-extend.h"
bool emberAfVcMfgExtensionClusterTransparentRequestCallback(uint8_t *data)
{
	uint8_t i;
	emberAfCorePrint("Test the Data:");
	for (i = 0; i < 10; i++) {
		emberAfCorePrint("%0x ", data[i]);
	}
	vcZ3ExtendParse(data, 0);
	return true;
}

void VcMfgExtensionClusterZigbeeExtendRsp(uint8_t *data, uint8_t size)
{
	emberAfFillCommandVcMfgExtensionClusterZigbeeExtendRsp(data, size);
    emberAfSendResponse();
}

bool emberAfVcMfgExtensionClusterZigbeeExtendReqCallback(uint8_t *data)
{
	uint8_t i;
	emberAfCorePrint("Zigbee Extend:");
	for (i = 0; i < data[0] + 1; i++) {
		emberAfCorePrint("%0x ", data[i]);
	}
	vcZ3CustomerExtend(data);
	return true;
}
//--------------------------------------------------------------------------------------------
void vcLEDInit(void)
{
  uint16_t pattern[4] = {500, 250, 125, 60};
  uint8_t i;
  //halMultiLedBlinkPatternInit(4, pattern, 0);
  //halMultiLedBlinkPatternInit(4, pattern, 1);
  //halMultiLedBlinkPatternInit(4, pattern, 2);
  //halMultiLedBlinkPatternInit(4, pattern, 3);

  // PORTF need to set
  //GPIO_PinModeSet(BSP_LED1_PORT, BSP_LED1_PIN, gpioModePushPull, 0);
  for (i = 0; i < BSP_LED_COUNT; i++) {
	  halMultiLedBlinkPatternInit(4, pattern, i);
	  halMultiLedBlinkLedOn(1, i);
  }
  //halMultiLedBlinkLedOn(1, 0);
  //halMultiLedBlinkLedOn(1, 1);
  //halMultiLedBlinkLedOn(1, 2);
  //halMultiLedBlinkLedOn(1, 3);
}

/** @brief Button Event
 *
 * This allows another module to get notification when a button is pressed and
 * released but the button joining plugin did not handle it. This callback is
 * NOT called in ISR context so there are no restrictions on what code can
 * execute.
 *
 * @param buttonNumber The button number that was pressed. Ver.: always
 * @param buttonPressDurationMs The length of time button was held down before
 * it was released. Ver.: always
 */
#define vcRelayControl(index, cmd)   vcLightControl((index), (cmd))

uint8_t vcGetNodeModeState(uint8_t ep)
{
	EmberStatus status;
	uint8_t state;
	status = vcReadMfgClusterServerAttribute(ZCL_VC_MFG_SYS_CONFIG_CLUSTER_ID,
			ZCL_ATTRIBUTE_MFG_SYSTEM_MODE_ATTRIBUTE_ID, (uint8_t *) &state,
			sizeof(state));
	return state;
}

uint8_t gVcNextState[4] = {1, 1, 1, 1};
void emberAfPluginButtonJoiningButtonEventCallback(uint8_t buttonNumber,
                                                   uint32_t buttonPressDurationMs)
{
#if 0
	uint8_t ep;
	//emberAfCorePrintln("buttonNumber: %d", buttonNumber);
	if (buttonNumber >= BSP_BUTTON_COUNT || sStartupState == 0) {
		return;
	}
	emberAfCorePrintln("%p, %d", __func__, __LINE__);
	emberAfCorePrintln("button: %d", buttonNumber);
	vcOnoffToggle(buttonNumber);
#endif
}

void vcAfPluginButtonJoiningButtonEventCallback(uint8_t buttonNumber,
                                                   uint8_t buttonState)
{
	uint8_t ep;
	emberAfCorePrintln("buttonNumber: %d: state:%d", buttonNumber, buttonState);
	if (buttonNumber >= BSP_BUTTON_COUNT || sStartupState == 0) {
		return;
	}
	emberAfCorePrintln("%p, %d", __func__, __LINE__);
	vcShadeOperateByButton(buttonNumber, buttonState);
}

void vcLEDBlink(uint8_t led, uint8_t pattern, uint16_t count)
{
	halMultiLedBlinkPatternActivity(led, pattern, count);
}

void vcLEDOn(uint8_t led)
{
	halMultiLedBlinkLedOn(0, led);
}

void vcLEDOff(uint8_t led)
{
	emberAfCorePrintln("%p, %d", __func__, __LINE__);
	halMultiLedBlinkLedOff(0, led);
}
#include "vcHalLight.h"

void vcUpdateLedIndicator(uint8_t state)
 {
	uint8_t ep;
	uint8_t cmd;
	uint8_t i;
	for (i = 0; i < ACTIVE_ENDPOINT_NUMBER; i++) {
		ep = emberAfEndpointFromIndex(i);
		emberAfReadAttribute(ep,
		ZCL_ON_OFF_CLUSTER_ID,
		ZCL_ON_OFF_ATTRIBUTE_ID,
		CLUSTER_MASK_SERVER, (uint8_t *) &cmd, sizeof(cmd),
		NULL);
		switch (state) {
		case VC_LED_INDICATOR_LOC:
			if (cmd == 1) {
				vcLEDOff(i);
			} else {
				vcLEDOn(i);
			}
			break;
		case VC_LED_INDICATOR_STATE:
			if (cmd == 1) {
				vcLEDOn(i);
			} else {
				vcLEDOff(i);
			}
			break;
		case VC_LED_NO_INDICATOR:
			vcLEDOff(i);
			break;
		}
	}
}
void customButtonControlEventHandler(void)
{

}

static uint8_t buttonActivity = 0;

void vcHalLightEndHandleTime(void)
{
	emberEventControlSetDelayMS(customButtonEventControl, 25);
}

void customButtonEventHandler(void)
{
	emberEventControlSetInactive(customButtonEventControl);

	if (buttonActivity == 0) {
		buttonActivity = 1;
		sStartupState = 1;
		vcShadeInit();
		vcShadeStartupInit();
	} else {
		vcHalLightOnOffEnd();
	}

}

uint8_t vcButtonActivity(void)
{
	return buttonActivity;
}

extern void vcLightIndicatorControl(uint8_t led, uint8_t cmd);
uint8_t ledHandleState;
void vcLedBlinkFinishedCallback(uint8_t led, uint8_t pattern)
{
	if (!sStartupState){
		return;
	}

	if (led == 0){
#if 0
		EmberAfStatus status;
		bool updatedOnOff = 0;
		emberAfCorePrintln("ep: %d", emberAfEndpointFromIndex(0));
		status = emberAfReadAttribute(emberAfEndpointFromIndex(0),
		ZCL_ON_OFF_CLUSTER_ID,
		ZCL_ON_OFF_ATTRIBUTE_ID,
		CLUSTER_MASK_SERVER, (uint8_t *) &updatedOnOff, sizeof(updatedOnOff),
		NULL);

		if (status == EMBER_ZCL_STATUS_SUCCESS) {
			if (pattern != 2) {
				vcLightIndicatorControl(0, updatedOnOff);
			} else{
				vcLEDBlink(0, 0, 20);
			}
		}
#endif
		switch(ledHandleState){
		case LED_HANDLE_OFF_END:
			vcLEDOff(0);
			ledHandleState =LED_HANDLE_NULL_END;
			break;
		case LED_HANDLE_RECOEVERY_END: {
			bool updatedOnOff = 0;
			emberAfReadAttribute(emberAfEndpointFromIndex(0),
			ZCL_ON_OFF_CLUSTER_ID,
			ZCL_ON_OFF_ATTRIBUTE_ID,
			CLUSTER_MASK_SERVER, (uint8_t *) &updatedOnOff,
					sizeof(updatedOnOff),
					NULL);
			vcLightIndicatorControl(0, updatedOnOff);
			ledHandleState =LED_HANDLE_NULL_END;
		}
			break;
		case LED_HANDLE_CONTINUE:
			vcLEDBlink(0, 0, 20);
			ledHandleState =LED_HANDLE_RECOEVERY_END;
			break;
		}
	}
}
LIST(vcTimerEventList);

enum {
	VC_ON_OFF_TIMED_ON = 0,
	VC_ON_OFF_ON,
	VC_ON_OFF_DELAYED_OFF,
	VC_ON_OFF_OFF,
};

void vcShadeStartupInit(void)
{
	list_init(vcTimerEventList);
#if 0
	EmberAfStatus status;
	bool updatedOnOff = 0;
	uint8_t i;
	uint8_t ep;

	ep = emberAfEndpointFromIndex(0);
	status = emberAfReadAttribute(ep,
	ZCL_ON_OFF_CLUSTER_ID,
	ZCL_ON_OFF_ATTRIBUTE_ID,
	CLUSTER_MASK_SERVER, (uint8_t *) &updatedOnOff, sizeof(updatedOnOff),
	NULL);
	if (status == EMBER_ZCL_STATUS_SUCCESS) {
		//vcSwControl(ep, updatedOnOff);
		vcOnOffTimeTable[0].state = updatedOnOff == ZCL_OFF_COMMAND_ID? VC_ON_OFF_OFF: VC_ON_OFF_ON;
	}
#endif
}



void vcTestPrintf(uint8_t step)
{
	emberAfCorePrintln("sw: %d", step);
}

#define customTimerEventControl customOnOffEventControl
void customOnOffEventHandler(void)
{
	VceTimerEvent_t *e = (VceTimerEvent_t *)list_head(vcTimerEventList);
	if (e == NULL) {
		emberEventControlSetInactive(customTimerEventControl);
		return;
	}
	emberAfCorePrint("timerEvent\r\n");
	for (; e != NULL; e = e->next){
		e->remainTime--;
		if (e->remainTime == 0) {
			e->callbak(e);
			break;
		}
	}
	emberEventControlSetDelayMS(customTimerEventControl, 1000);
}

void vcVceLevelTimerEventHandler(VceTimerEvent_t *e)
{
	EmberStatus status;
	emberAfCorePrint("timerEventHandled\r\n");
	status = emberAfWriteAttribute(e->ep,
			e->cluster,
			e->attribute,
		CLUSTER_MASK_SERVER, (uint8_t *) &e->op.level, ZCL_INT8U_ATTRIBUTE_TYPE);
	if (status != EMBER_SUCCESS){
		emberAfCorePrint("Fail to set the level\r\n");
	}
	list_remove(vcTimerEventList, e);
	e->isInvalid = 0;
}

void vcVceWriteSubTimerEventCallback(VceTimerEvent_t *e)
{
	emberAfCorePrint("e->ep:%0x\r\n", e->ep);
	emberAfCorePrint("e->cluster:%0x\r\n", e->cluster);
	emberAfCorePrint("e->attribute:%0x\r\n", e->attribute);
	emberAfCorePrint("e->delay:%0d\r\n", e->remainTime);
	emberAfCorePrint("e->op:%0d\r\n", e->op);
	e->callbak = vcVceLevelTimerEventHandler;
	list_add(vcTimerEventList, e);
	emberEventControlSetDelayMS(customTimerEventControl, 1000);
}

void vcVceCtlCancelSubTimerEventCallback(VceTimerEvent_t *e)
{
	list_remove(vcTimerEventList, e);
	e->isInvalid = 0;
}
/** @brief On/off Cluster On With Timed Off
 *
 *
 *
 * @param onOffControl   Ver.: always
 * @param onTime   Ver.: always
 * @param offWaitTime   Ver.: always
 */
#if 0
bool emberAfOnOffClusterOnWithTimedOffCallback(uint8_t onOffControl,
                                                  uint16_t onTime,
                                                  uint16_t offWaitTime)
{
	uint8_t ep = emberAfCurrentEndpoint();
	uint8_t pos;
	EmberAfStatus status;
	VcOnOffTimer_t *p;
	bool updatedOnOff = 0;

	pos = emberAfIndexFromEndpoint(ep);
	p = &vcOnOffTimeTable[pos];
	status = emberAfReadAttribute(ep,
	ZCL_ON_OFF_CLUSTER_ID,
	ZCL_ON_OFF_ATTRIBUTE_ID,
	CLUSTER_MASK_SERVER, (uint8_t *) &updatedOnOff, sizeof(updatedOnOff),
	NULL);
	if (onOffControl && updatedOnOff == ZCL_OFF_COMMAND_ID) {
		return false;
	}

	p->ep = ep;
	status = emberAfWriteAttribute(ep,
	ZCL_ON_OFF_CLUSTER_ID,
	ZCL_ON_TIME_ATTRIBUTE_ID,
	CLUSTER_MASK_SERVER, (uint16_t *) &onTime, ZCL_INT16U_ATTRIBUTE_TYPE);

	status = emberAfWriteAttribute(ep,
	ZCL_ON_OFF_CLUSTER_ID,
	ZCL_OFF_WAIT_TIME_ATTRIBUTE_ID,
	CLUSTER_MASK_SERVER, (uint16_t *) &offWaitTime, ZCL_INT16U_ATTRIBUTE_TYPE);

	p->onOffControl = onOffControl;
	if (0 < onTime && onTime < 10) {
		p->onTime = 1;
	} else {
		p->onTime = onTime / 10;
	}

	if (0 < offWaitTime && offWaitTime < 10) {
		p->offWaitTime = 1;
	} else {
		p->offWaitTime = offWaitTime / 10;
	}

	if (p->state == VC_ON_OFF_ON) {
		p->state = VC_ON_OFF_TIMED_ON;
		list_add(vcOnOffList, p);
		emberEventControlSetDelayMS(customOnOffEventControl, 1000);
	}

	if ( p->state == VC_ON_OFF_OFF) {
		p->state = VC_ON_OFF_TIMED_ON;
		updatedOnOff =ZCL_ON_COMMAND_ID;
		status = emberAfWriteAttribute(p->ep,
		ZCL_ON_OFF_CLUSTER_ID,
		ZCL_ON_OFF_ATTRIBUTE_ID,
		CLUSTER_MASK_SERVER, (uint8_t *) &updatedOnOff,
				ZCL_BOOLEAN_ATTRIBUTE_TYPE);
		list_add(vcOnOffList, p);
		emberEventControlSetDelayMS(customOnOffEventControl, 1000);
	}
	return false;
}
#endif
uint8_t vcGetSwLedState(void)
{
	EmberAfStatus status;
	uint8_t state;

	status = emberAfReadManufacturerSpecificServerAttribute(emberAfEndpointFromIndex(0),
	ZCL_VC_MFG_EXTENSION_CLUSTER_ID,
	ZCL_ATTRIBUTE_MFG_INDICATOR_STATE_ATTRIBUTE_ID, EMBER_AF_MANUFACTURER_CODE, (uint8_t *) &state,
			sizeof(state));

	if (status == EMBER_ZCL_STATUS_SUCCESS) {
		return state;
	}
	emberAfCorePrintln("%p: %d", __func__, __LINE__);
	return 1;
}

EmberAfStatus vcReadMfgClusterServerAttribute( EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t readLength)
{
	EmberAfStatus status;
	status = emberAfReadManufacturerSpecificServerAttribute(emberAfEndpointFromIndex(0),
			cluster,
			attributeID,
			EMBER_AF_MANUFACTURER_CODE,
			dataPtr,
			readLength);

    return status;
}

EmberAfStatus vcWriteMfgClusterServerAttribute( EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t dataType)
{
	EmberAfStatus status;
    status = emberAfWriteManufacturerSpecificServerAttribute(emberAfEndpointFromIndex(0),
    		cluster,
			attributeID,
			EMBER_AF_MANUFACTURER_CODE,
			dataPtr,
			dataType);
    return status;
}

EmberAfStatus vcReadMfgClusterServerAttributeEx(uint8_t ep, EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t readLength)
{
	EmberAfStatus status;
	status = emberAfReadManufacturerSpecificServerAttribute(ep,
			cluster,
			attributeID,
			EMBER_AF_MANUFACTURER_CODE,
			dataPtr,
			readLength);

    return status;
}

EmberAfStatus vcWriteMfgClusterServerAttributeEx(uint8_t ep, EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t dataType)
{
	EmberAfStatus status;
    status = emberAfWriteManufacturerSpecificServerAttribute(ep,
    		cluster,
			attributeID,
			EMBER_AF_MANUFACTURER_CODE,
			dataPtr,
			dataType);
    return status;
}
