/*
 * vc-reporting.c
 *
 *  Created on: Jun 12, 2018
 *      Author: tao
 */
#include "app/framework/include/af.h"
#include EMBER_AF_API_NETWORK_STEERING
#include "./vancount-z3.h"
extern bool emberAfEndpointIsEnabled(uint8_t endpoint);

static void vcReportingHandler(uint8_t endpoint,
		EmberAfClusterId clusterId,
        uint8_t *buffer,
        uint16_t bufLen);
/** @brief Report Attributes
 *
 * This function is called by the application framework when a Report Attributes
 * command is received from an external device.  The application should return
 * true if the message was processed or false if it was not.
 *
 * @param clusterId The cluster identifier of this command.  Ver.: always
 * @param buffer Buffer containing the list of attribute report records.  Ver.:
 * always
 * @param bufLen The length in bytes of the list.  Ver.: always
 */
bool emberAfReportAttributesCallback(EmberAfClusterId clusterId,
                                     uint8_t *buffer,
                                     uint16_t bufLen)
{
	uint8_t i;
	uint8_t ep = emberAfCurrentEndpoint();

	vcReportingHandler(ep, clusterId, buffer, bufLen);
	//emberAfCorePrintln("EP: %04x,Reporting Recved: %04x", emberAfCurrentEndpoint(), clusterId);
	//for (i = 0; i < bufLen; i++) {
	//	emberAfCorePrintln("%x ", buffer[i]);
	//}
	return false;
}

extern uint8_t gVcNextState[4];
static void vcReportingHandler(uint8_t endpoint, EmberAfClusterId clusterId, uint8_t *buffer, uint16_t bufLen)
{
	EmberAfStatus status;
	if (emberAfEndpointIsEnabled(endpoint) == false) {
		return;
	}

	switch(vcGetNodeModeState(endpoint)){
	case VC_NORMAL_SWITCH:
		//if (clusterId == ZCL_ON_OFF_CLUSTER_ID) {
		//	emberAfWriteAttribute(endpoint,
		//	ZCL_ON_OFF_CLUSTER_ID,
		//	ZCL_ON_OFF_ATTRIBUTE_ID,
		//	CLUSTER_MASK_SERVER, (uint8_t *) &buffer[3],
		//			ZCL_BOOLEAN_ATTRIBUTE_TYPE);
		//}
		break;
	case VC_DOUBLE_CONTROL_SWITCH:
		if (clusterId == ZCL_ON_OFF_CLUSTER_ID) {
			uint8_t light;
			light = emberAfIndexFromEndpoint(endpoint);
			vcLightIndicatorControl(light, buffer[3]);
			gVcNextState[light] = buffer[3]? 0: 1;
		}
		break;
	case VC_SCENES_SWITCH:
		break;
	}
}

extern uint8_t appZclBuffer[];
extern uint16_t appZclBufferLen;
extern EmberApsFrame globalApsFrame;
extern uint8_t disableDefaultResponse;
extern uint16_t mfgSpecificId;
extern bool zclCmdIsBuilt;
// vcReportAnAttribute(0x00, 0x01, 0x01, 0x0006, 0x00, 1);
void vcSetMfgCode(void)
{
	mfgSpecificId = EMBER_AF_MANUFACTURER_CODE;
}

void vcReportAnAttribute(uint16_t destination, uint8_t srcEp, uint8_t dstEp, EmberAfClusterId clusterId, EmberAfAttributeId attributeId, uint8_t mask)
{
	  EmberApsFrame *apsFrame;
	  EmberAfStatus status;
	  EmberAfAttributeType type;
	  uint8_t size;
	  uint8_t data[ATTRIBUTE_LARGEST];

	  if (emberAfNetworkState() != EMBER_JOINED_NETWORK) {
		  return;
	  }
	  status = emberAfReadAttribute((uint8_t)srcEp, // endpoint
	                                clusterId,
	                                attributeId,
	                                (mask == 0
	                                 ? CLUSTER_MASK_CLIENT
	                                 : CLUSTER_MASK_SERVER),
	                                data,
	                                sizeof(data),
	                                &type);
	  if (status != EMBER_ZCL_STATUS_SUCCESS) {
	    emberAfReportingPrintln("ERR: reading attribute %x", status);
	    return;
	  }

	  zclBufferSetup(ZCL_GLOBAL_COMMAND
	                 | (mask == 0
	                    ? ZCL_FRAME_CONTROL_CLIENT_TO_SERVER
	                    : ZCL_FRAME_CONTROL_SERVER_TO_CLIENT),
	                 clusterId,
	                 ZCL_REPORT_ATTRIBUTES_COMMAND_ID);
	  zclBufferAddWord(attributeId);
	  zclBufferAddByte(type);

	  size = (emberAfIsThisDataTypeAStringType(type)
	          ? emberAfStringLength(data) + 1
	          : emberAfGetDataSize(type));
	  MEMMOVE(appZclBuffer + appZclBufferLen, data, size);
	  appZclBufferLen += size;

	  vcSendCommand(destination, srcEp, dstEp);
}


void vcReportAnAttributeEx(uint8_t srcEp, EmberAfClusterId clusterId, EmberAfAttributeId attributeId, uint8_t isMfg)
{
	EmberApsFrame *apsFrame;
	EmberAfStatus status;
	EmberAfAttributeType type;
	uint16_t destination = 0;
	uint8_t dstEp = 1;
	uint8_t mask = 1;
	uint8_t size;
	uint8_t data[ATTRIBUTE_LARGEST];

	if (emberAfNetworkState() != EMBER_JOINED_NETWORK) {
		return;
	}

	if (isMfg) {
		vcSetMfgCode();
		status = emAfReadAttribute(srcEp, clusterId, attributeId, CLUSTER_MASK_SERVER,
				EMBER_AF_MANUFACTURER_CODE, data, sizeof(data), &type);
	} else {
		status = emberAfReadAttribute(
				(uint8_t) srcEp, // endpoint
				clusterId, attributeId,
				(mask == 0 ? CLUSTER_MASK_CLIENT : CLUSTER_MASK_SERVER), data,
				sizeof(data), &type);
	}

	if (status != EMBER_ZCL_STATUS_SUCCESS) {
		emberAfReportingPrintln("ERR: reading attribute %x", status);
		return;
	}

	zclBufferSetup(
			ZCL_GLOBAL_COMMAND
					| (mask == 0 ?
							ZCL_FRAME_CONTROL_CLIENT_TO_SERVER :
							ZCL_FRAME_CONTROL_SERVER_TO_CLIENT), clusterId,
			ZCL_REPORT_ATTRIBUTES_COMMAND_ID);
	zclBufferAddWord(attributeId);
	zclBufferAddByte(type);

	size = (emberAfIsThisDataTypeAStringType(type) ?
			emberAfStringLength(data) + 1 : emberAfGetDataSize(type));
	MEMMOVE(appZclBuffer + appZclBufferLen, data, size);
	appZclBufferLen += size;

	vcSendCommand(destination, srcEp, dstEp);
}

void vcSendCommand(uint16_t destination, uint8_t srcEndpoint, uint8_t dstEndpoint)
{
  emAfApsFrameEndpointSetup(srcEndpoint, dstEndpoint);
  emberAfSendUnicast(EMBER_OUTGOING_DIRECT,
                                destination,
                                &globalApsFrame,
                                appZclBufferLen,
                                appZclBuffer);

  zclCmdIsBuilt = false;
  mfgSpecificId = EMBER_AF_NULL_MANUFACTURER_CODE;
  disableDefaultResponse = 0;
}

EmberEventControl customReportingEventControl;
void customReportingEventHandler(void)
{
	static uint8_t indexEp = 1;

	vcReportAnAttribute(0x00, indexEp++, 0x01, ZCL_LEVEL_CONTROL_CLUSTER_ID, ZCL_CURRENT_LEVEL_ATTRIBUTE_ID, 1);
	if (indexEp > ACTIVE_ENDPOINT_NUMBER) {
		indexEp = 1;
	}
	vcResetReportingInterval();
}

void vcResetReportingInterval(void)
{
	emberEventControlSetDelayMS(customReportingEventControl, 300000);
}

void vcReportingIntervalStart(void)
{
	emberEventControlSetDelayMS(customReportingEventControl, 1000);
}

