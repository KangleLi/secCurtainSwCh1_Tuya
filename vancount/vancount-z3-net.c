/*
 * =====================================================================================
 *
 *       Filename:  vancount-z3-net.c
 *
 *    Description:  .
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
#include "./vancount-z3.h"

#include EMBER_AF_API_NETWORK_STEERING
#include EMBER_AF_API_FIND_AND_BIND_TARGET

#define ENDPOINT_SWITCH_1 (1)
#define ENDPOINT_SWITCH_2 (2)
#define ENDPOINT_SWITCH_3 (3)
#define ENDPOINT_SWITCH_4 (4)

#define EP_FIXED_COUNT   ((uint8_t)2)
static uint8_t sEpTable[EP_FIXED_COUNT] = {ENDPOINT_SWITCH_1, ENDPOINT_SWITCH_2};
static uint8_t sEpTableIndex;

EmberEventControl commissioningLedEventControl;
EmberEventControl findingAndBindingEventControl;
EmberEventControl vancountNetEventControl;

void vancountNetEventHandler(void)
{
	emberAfCorePrintln("NetEventHandler");
//	emberLeaveNetwork();
	emberEventControlSetInactive(vancountNetEventControl);
}

void commissioningLedEventHandler(void)
{
  emberEventControlSetInactive(commissioningLedEventControl);

  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    uint16_t identifyTime;
    emberAfReadServerAttribute(sEpTable[0],
                               ZCL_IDENTIFY_CLUSTER_ID,
                               ZCL_IDENTIFY_TIME_ATTRIBUTE_ID,
                               (uint8_t *)&identifyTime,
                               sizeof(identifyTime));
    if (identifyTime > 0) {
      //emberAfPluginSilabsDeviceUiLedIdentifyBlink();
    }
  } else {
    //emberAfPluginConnectionManagerStartNetworkSearchCallback();
	vcSetLedHandleState(LED_HANDLE_CONTINUE);
	vcLEDBlink(VC_IDENTIFY_LED_INDEX, 2, 480);
    EmberStatus status = emberAfPluginNetworkSteeringStart();
    emberAfCorePrintln("%p network %p: 0x%X", "Join", "start", status);
  }
}

void findingAndBindingEventHandler(void)
{
  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
    if (sEpTableIndex >= EP_FIXED_COUNT) {
      emberEventControlSetInactive(findingAndBindingEventControl);
      return;
    }
    emberAfCorePrintln("Find and bind target start: 0x%X",
                       emberAfPluginFindAndBindTargetStart(sEpTable[sEpTableIndex++]));
  }
}

static void setZllState(uint16_t clear, uint16_t set)
{
  EmberTokTypeStackZllData token;
  emberZllGetTokenStackZllData(&token);
  token.bitmask &= ~clear;
  token.bitmask |= set;
  emberZllSetTokenStackZllData(&token);
}
/** @brief Stack Status
 *
 * This function is called by the application framework from the stack status
 * handler.  This callbacks provides applications an opportunity to be notified
 * of changes to the stack status and take appropriate action.  The return code
 * from this callback is ignored by the framework.  The framework will always
 * process the stack status after the callback returns.
 *
 * @param status   Ver.: always
 */
extern void vcShadeResetInit(void);
void vancountClearNetworkTables(void)
{
  uint8_t endpointIndex;
  uint8_t endpoint;

  emberClearBindingTable();
  emberAfClearReportTableCallback();
  for (endpointIndex = 0; endpointIndex < emberAfEndpointCount();
       endpointIndex++) {
    endpoint = emberAfEndpointFromIndex(endpointIndex);
    emberAfResetAttributes(endpoint);
    emberAfGroupsClusterClearGroupTableCallback(endpoint);
    emberAfScenesClusterClearSceneTableCallback(endpoint);
  }
  vcShadeResetInit();
  vc3In1ClusterAttributeInit();
}

// carel_add ( 清网络参数 )
static void vancountClearNetworkTables_noAttr(void)
{
  uint8_t endpointIndex;
  uint8_t endpoint;

  emberClearBindingTable();
  emberAfClearReportTableCallback();
  for (endpointIndex = 0; endpointIndex < emberAfEndpointCount();
       endpointIndex++) {
    endpoint = emberAfEndpointFromIndex(endpointIndex);
    //emberAfResetAttributes(endpoint);
    emberAfGroupsClusterClearGroupTableCallback(endpoint);
    emberAfScenesClusterClearSceneTableCallback(endpoint);
  }
  //vcShadeResetInit();
  //vc3In1ClusterAttributeInit();
}
// 20190411

extern  void vcZ3CommissionActive(uint32_t ms);
extern  void vcLedBlinkFinishedCallback(uint8_t led, uint8_t pattern);
bool emberAfStackStatusCallback(EmberStatus status)
{
  // Make sure to change the ZLL factory new state based on whether or not
  // we are on a network.
 // emberAfPluginSilabsDeviceUiStackStatusCallback();
  if (status == EMBER_NETWORK_DOWN
      && emberAfNetworkState() == EMBER_NO_NETWORK) {
    //halClearLed(COMMISSIONING_STATUS_LED);
    // Reset to factory new. Clear all the resource about network.
    /*emberAfPluginConnectionManagerLeaveNetworkCallback();*/

	  // carel_del ( 去除网络掉线后复位和自动配网 )
	  // vancountClearNetworkTables();
	  vancountClearNetworkTables_noAttr();
	  // vcZ3CommissionActive(5000);
	  //setZllState(0, (EMBER_ZLL_STATE_FACTORY_NEW | EMBER_ZLL_STATE_PROFILE_INTEROP));
	  // 20190411

    vcSetLedHandleState(LED_HANDLE_RECOEVERY_END);
    vcLEDBlink(VC_IDENTIFY_LED_INDEX, 1, 8);
  }

  if (status == EMBER_NETWORK_DOWN
      && emberAfNetworkState() == EMBER_JOINED_NETWORK_NO_PARENT) {
    /*emberAfPluginConnectionManagerLeaveNetworkCallback();*/

	  // carel_del
	  //vancountClearNetworkTables();
	  vancountClearNetworkTables_noAttr();
	  //setZllState(0, (EMBER_ZLL_STATE_FACTORY_NEW | EMBER_ZLL_STATE_PROFILE_INTEROP));
	  // 20190411

    //vcZ3CommissionActive(30 * 60 * 1000);
    vcSetLedHandleState(LED_HANDLE_RECOEVERY_END);
    vcLEDBlink(VC_IDENTIFY_LED_INDEX, 1, 8);
  }

	if (status == EMBER_NETWORK_UP) {
		//halSetLed(COMMISSIONING_STATUS_LED);
		setZllState(EMBER_ZLL_STATE_FACTORY_NEW,
				EMBER_ZLL_STATE_PROFILE_INTEROP);
		emberAfCorePrintln("%p network %p: 0x%X", "Open", "for joining", status);
		emberEventControlSetActive(findingAndBindingEventControl);

		// carel
			extern uint8_t  sStartupState;
			if (sStartupState){
				vcSetLedHandleState(LED_HANDLE_RECOEVERY_END);
				vcLEDBlink(VC_IDENTIFY_LED_INDEX, 1, 8);
			}

		// carel
		extern EmberEventControl _carel_report_EC;
		emberEventControlSetDelayMS(_carel_report_EC, 3000);
		// 20190402

		// carel
		vcReportAnAttribute(0x00, 1, 0x01, 0x0000, 0xFFFE, 1);
		// 20190411

		vcReportingIntervalStart();
	}

  // This value is ignored by the framework.
  return false;
}

// carel
EmberEventControl _carel_report_EC;
void _carel_report_EF(void){
	emberEventControlSetInactive(_carel_report_EC);

	vcReportAnAttribute(0x00, 1, 0x01, ZCL_LEVEL_CONTROL_CLUSTER_ID,
							ZCL_CURRENT_LEVEL_ATTRIBUTE_ID, 1);
}

// 20190403

/** @brief Complete
 *
 * This callback is fired when the Network Steering plugin is complete.
 *
 * @param status On success this will be set to EMBER_SUCCESS to indicate a
 * network was joined successfully. On failure this will be the status code of
 * the last join or scan attempt. Ver.: always
 * @param totalBeacons The total number of 802.15.4 beacons that were heard,
 * including beacons from different devices with the same PAN ID. Ver.: always
 * @param joinAttempts The number of join attempts that were made to get onto
 * an open Zigbee network. Ver.: always
 * @param finalState The finishing state of the network steering process. From
 * this, one is able to tell on which channel mask and with which key the
 * process was complete. Ver.: always
 */
static uint8_t count;
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  emberAfCorePrintln("%p network %p: 0x%X", "Join", "complete", status);
  if (status != EMBER_SUCCESS &&
      count < 1) {
    count++;
//    vcZ3CommissionActive((count * 4) * 10);
    vcZ3CommissionActive(40);
    return;
  } else {

	  // carel
	  if(status != EMBER_SUCCESS &&
		count >= 1){
		vcSetLedHandleState(LED_HANDLE_RECOEVERY_END);
		vcLEDBlink(VC_IDENTIFY_LED_INDEX, 0, 20);
	  }
	  // 20190515

    //emberAfPluginConnectionManagerFinishedCallback();
    count = 0;
    emberEventControlSetDelayMS(vancountNetEventControl, 10000);
  }
#if 0
  if (status != EMBER_SUCCESS) {
    // Initialize our ZLL security now so that we are ready to be a touchlink
    // target at any point.
    status = emberAfZllSetInitialSecurityState();
    if (status != EMBER_SUCCESS) {
      //emberAfCorePrintln("Error: cannot initialize ZLL security: 0x%X", status);
    }

    status = emberAfPluginNetworkCreatorStart(false); // distributed
    emberAfCorePrintln("%p network %p: 0x%X", "Form", "start", status);
  }
#endif
}

/** @brief Complete
 *
 * This callback notifies the user that the network creation process has
 * completed successfully.
 *
 * @param network The network that the network creator plugin successfully
 * formed. Ver.: always
 * @param usedSecondaryChannels Whether or not the network creator wants to
 * form a network on the secondary channels Ver.: always
 */
void emberAfPluginNetworkCreatorCompleteCallback(const EmberNetworkParameters *network,
                                                 bool usedSecondaryChannels)
{
  emberAfCorePrintln("%p network %p: 0x%X",
                     "Form distributed",
                     "complete",
                     EMBER_SUCCESS);
}

/** @brief Server Init
 *
 * On/off cluster, Server Init
 *
 * @param endpoint Endpoint that is being initialized  Ver.: always
 */
#if 0
void emberAfOnOffClusterServerInitCallback(uint8_t endpoint)
{
  // At startup, trigger a read of the attribute and possibly a toggle of the
  // LED to make sure they are always in sync.
  emberAfOnOffClusterServerAttributeChangedCallback(endpoint,
                                                    ZCL_ON_OFF_ATTRIBUTE_ID);
}
#endif




void vcZ3CommissionActive(uint32_t delayMS)
{
  /*emberEventControlSetActive(commissioningLedEventControl);*/
  emberEventControlSetDelayMS(commissioningLedEventControl, delayMS);
  sEpTableIndex = 0;
}
