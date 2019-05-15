/*
 * vancount-mf-test.c
 *
 *  Created on: May 16, 2018
 *      Author: tao
 */
#if 0
#include "app/framework/include/af.h"
#include "cluster-id.h"
//#include "attribute-storage.h"
#include "vancount-hal-serial.h"
#include "vancount-mf-test.h"

extern EmberAfStatus vcReadMfgClusterServerAttribute( EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t readLength);
extern EmberAfStatus vcWriteMfgClusterServerAttribute( EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t dataType);
static void vcMfCtl(uint8_t *data, uint8_t dataSize);
static void vcMfGet(uint8_t *data, uint8_t dataSize);
static void vcMfSet(uint8_t *data, uint8_t dataSize);
static uint8_t vcMfParse(uint8_t *data, uint8_t dataSize);
//static void vcMfACK(void);
static void vcMfNAK(void);
extern void vcMfReportNodeInfo(void);
extern void vcMfReportNetworkState(void);
extern uint8_t vcGetActiveEpByIndex(uint8_t index);
//==============================================================
// Static Function

//==============================================================
EmberEventControl customMfTestEventControl;

#define VC_MF_TEST_BUF_SIZE    ((uint8_t)64)
uint8_t sVcMfTestBuf[VC_MF_TEST_BUF_SIZE];
uint8_t sVcMfTestBufSend[VC_MF_TEST_BUF_SIZE];
uint8_t sVcMfTestBufSendSize;
static uint8_t sMfInit = 0;
static uint8_t sMfRet = 0;
void customMfTestEventHandler(void)
{
	vcHalSerialWrite(sVcMfTestBuf, sMfRet);
	emberEventControlSetInactive(customMfTestEventControl);
	//vcMfSend();
}

void vcMfInit(void)
{
	vcHalSerialInit();
	//emberEventControlSetActive(customMfTestEventControl);
	sMfInit = 1;
	sVcMfTestBufSendSize = 0;
	vcMfReportNodeInfo();
}

void vcMfSend(void)
{
	if (sMfInit == 0){
		return;
	}
	vcHalSerialWrite(sVcMfTestBufSend, sVcMfTestBufSendSize);
	sVcMfTestBufSendSize = 0;
}

void vcMfRecv(void)
{
	uint8_t ret;
	if (sMfInit == 0){
		return;
	}
	ret = vcHalSerialReadAvalible();
	if (ret < 10) {
		return;
	}

	ret = VC_MF_TEST_BUF_SIZE;
	memset(sVcMfTestBuf, 0, ret);
	vcHalSerialRead(sVcMfTestBuf , &ret);
	sMfRet = ret ;
	vcMfParse(sVcMfTestBuf, sMfRet);
	//vcMfSend(sVcMfTestBuf, ret);
	//vcHalSerialWrite(sVcMfTestBuf, 8);
	//emberEventControlSetActive(customMfTestEventControl);
}

void vcMfRecvHandle(uint8_t *data, uint8_t dataSize)
{


}
//======================================================================
#define VC_DEVICE_APP_VERSION   ((uint16_t)0x0001)
//======================================================================
#define VC_MF_FRAME_START          ((uint8_t)0x68)
#define VC_MF_FRAME_END             ((uint8_t)0x16)
#define VC_MF_VER                  ((uint16_t)0x01)

#define VC_MF_FRAME_START_INDEX    ((uint8_t)0)
#define VC_MF_FRAME_VER_INDEX    ((uint8_t)1)
#define VC_MF_FRAME_L_INDEX    ((uint8_t)3)
#define VC_MF_FRAME_C_INDEX    ((uint8_t)4)
#define VC_MF_FRAME_T_INDEX    ((uint8_t)5)
#define VC_MF_FRAME_FN_INDEX    ((uint8_t)7)
#define VC_MF_FRAME_PAYLOAD_INDEX    ((uint8_t)8)

#define VC_MF_FRAME_FIXED_LEN   ((uint8_t)10)
//======================================================================
#define VC_MF_FRAME_C_REQ       ((uint8_t)0x00)
#define VC_MF_FRAME_C_RES       ((uint8_t)0x80)
//======================================================================
#define VC_MF_FRAME_FN_00H      ((uint8_t)0x00)
//======================================================================
#define VC_MF_FRAME_FN_GET      ((uint8_t)0x05)
#define VC_MF_FRAME_SUB_READ_ON_OFF_STATE  ((uint8_t)0)
#define VC_MF_FRAME_SUB_READ_LED_STATE  ((uint8_t)1)
#define VC_MF_FRAME_SUB_READ_KIDLOCK_STATE  ((uint8_t)2)
#define VC_MF_FRAME_SUB_READ_POWER_METER_CAL  ((uint8_t)3)
#define VC_MF_FRAME_SUB_READ_KEY_STATE  ((uint8_t)4)
#define VC_MF_FRAME_SUB_READ_POWER_METER_COUNT  ((uint8_t)5)
#define VC_MF_FRAME_SUB_READ_NODE_INFO         ((uint8_t)0xFE)
//======================================================================
#define VC_MF_FRAME_FN_SET      ((uint8_t)0x06)
#define VC_MF_FRAME_SUB_WRITE_ON_OFF_STATE  ((uint8_t)0)
#define VC_MF_FRAME_SUB_WRITE_LED_STATE  ((uint8_t)1)
#define VC_MF_FRAME_SUB_WRITE_KIDLOCK_STATE  ((uint8_t)2)
#define VC_MF_FRAME_SUB_WRITE_POWER_METER_CAL  ((uint8_t)3)
#define VC_MF_FRAME_SUB_WRITE_KEY_STATE  ((uint8_t)4)
//======================================================================
#define VC_MF_FRAME_FN_CONTROL  ((uint8_t)0x07)
#define VC_MF_FRAME_SUB_POWER_METER_CAL_START  ((uint8_t)0x00)
#define VC_MF_FRAME_SUB_POWER_METER_CAL_END    ((uint8_t)0x01)
#define VC_MF_FRAME_SUB_NETWORK_JOIN           ((uint8_t)0x02)
#define VC_MF_FRAME_SUB_NETWORK_LEAVE          ((uint8_t)0x03)
//======================================================================
#define VC_MF_FRAME_FN_REPORT   ((uint8_t)0x08)
#define VC_MF_FRAME_SUB_NETWORK_STATE  ((uint8_t)0x00)
#define VC_MF_FRAME_SUB_NODE_INFO      ((uint8_t)0x01)
//======================================================================
static inline uint8_t vcMfGetCs(uint8_t *data, uint8_t dataSize)
{
	uint8_t i;
	uint8_t cs = 0;
	for (i = 0; i < dataSize; i++) {
		cs += data[i];
	}
	return cs;
}

static uint8_t vcMfParse(uint8_t *data, uint8_t dataSize)
{
	uint8_t cs;
	uint16_t type;

	if (dataSize < VC_MF_FRAME_FIXED_LEN) {
		return 0;
	}
	while (*data != VC_MF_FRAME_START) {
		data++;
		dataSize--;
		if (dataSize < VC_MF_FRAME_FIXED_LEN) {
			return 0;
		}
	}
	if (*data != VC_MF_FRAME_START) {
		return 0;
	}
	if (data[VC_MF_FRAME_L_INDEX] > dataSize) {
		return 0;
	}

	if (data[dataSize - 1] != VC_MF_FRAME_END) {
		return 0;
	}
	// CS is not include start v cs end
	cs = vcMfGetCs(data + VC_MF_FRAME_L_INDEX, dataSize - 3 - 2);
	if (data[dataSize - 2] != cs) {
		return 0;
	}

	if (data[VC_MF_FRAME_C_INDEX] != 0x00) {
		return 0;
	}

	type = data[VC_MF_FRAME_T_INDEX];
	type = (type << 8) | data[VC_MF_FRAME_T_INDEX + 1];
	if (type != VC_DEVICE_T_TYPE) {
		vcMfNAK();
		return 0;
	}

	switch (data[VC_MF_FRAME_FN_INDEX]) {
	case VC_MF_FRAME_FN_GET:
		vcMfGet(data + VC_MF_FRAME_PAYLOAD_INDEX,
				dataSize - VC_MF_FRAME_FIXED_LEN);
		break;
	case VC_MF_FRAME_FN_SET:
		vcMfSet(data + VC_MF_FRAME_PAYLOAD_INDEX,
				dataSize - VC_MF_FRAME_FIXED_LEN);
		break;
	case VC_MF_FRAME_FN_CONTROL:
		vcMfCtl(data + VC_MF_FRAME_PAYLOAD_INDEX,
						dataSize - VC_MF_FRAME_FIXED_LEN);
		break;
	default:
		vcMfNAK();
		break;
	}
	return 1;
}

static uint8_t vcMfPacket(uint8_t c, uint8_t fn, uint8_t payloadSize)
{
	uint8_t *p = sVcMfTestBufSend;
	uint8_t len;

	len = payloadSize + VC_MF_FRAME_FIXED_LEN;
	p[VC_MF_FRAME_START_INDEX] = VC_MF_FRAME_START;
	p[VC_MF_FRAME_VER_INDEX] = (uint8_t)(VC_MF_VER >> 8);
	p[VC_MF_FRAME_VER_INDEX + 1] = (uint8_t)(VC_MF_VER);
	p[VC_MF_FRAME_L_INDEX] = len;
	p[VC_MF_FRAME_C_INDEX] = c;
	p[VC_MF_FRAME_FN_INDEX] = fn;

	p[VC_MF_FRAME_T_INDEX] =(uint8_t)(VC_DEVICE_T_TYPE >> 8);
	p[VC_MF_FRAME_T_INDEX + 1] =(uint8_t)(VC_DEVICE_T_TYPE);

	p[len - 2] = vcMfGetCs(p + VC_MF_FRAME_L_INDEX, len - 3 -2);
	p[len - 1] = VC_MF_FRAME_END;
	sVcMfTestBufSendSize = len;
	vcMfSend();

	return 0;
}
//======================================================================
// ACK/NACK
#if 0
static void vcMfACK(void)
 {
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;
	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = 0;
	len = 1;
	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_00H, len);
}
#endif
static void vcMfNAK(void)
 {
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;
	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = 1;
	len = 1;
	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_00H, len);
}
//======================================================================
// Get Cmd Handle
static void vcMfGetOnOffState(uint8_t *data, uint8_t dataSize);
static void vcMfGetLedState(uint8_t *data, uint8_t dataSize);
static void vcMfGetKidLockState(uint8_t *data, uint8_t dataSize);
static void vcMfGetKeyState(uint8_t *data, uint8_t dataSize);
static void vcMfGetNodeInfo(uint8_t *data, uint8_t dataSize);
static void vcMfGet(uint8_t *data, uint8_t dataSize)
 {
	uint8_t sub;
	sub = data[0];
	data++;
	dataSize--;
	switch (sub) {
	case VC_MF_FRAME_SUB_READ_ON_OFF_STATE:
		vcMfGetOnOffState(data, dataSize);
		break;
	case VC_MF_FRAME_SUB_READ_LED_STATE:
		vcMfGetLedState(data, dataSize);
		break;
	case VC_MF_FRAME_SUB_READ_KIDLOCK_STATE:
		//vcMfGetKidLockState(data, dataSize);
		break;
	case VC_MF_FRAME_SUB_READ_POWER_METER_CAL:
		//vcMfGetPowerMeterCalibration(data, dataSize);
		break;
	case VC_MF_FRAME_SUB_READ_KEY_STATE:
		vcMfGetKeyState(data, dataSize);
		break;
	case VC_MF_FRAME_SUB_READ_POWER_METER_COUNT:
		//vcMfGetPowerMeterCount(data, dataSize);
		break;
	case VC_MF_FRAME_SUB_READ_NODE_INFO:
		vcMfGetNodeInfo(data, dataSize);
		break;
	default:
		vcMfNAK();
		break;
	}
}

static void vcMfGetOnOffState(uint8_t *data, uint8_t dataSize)
 {
	bool onOff;
	uint8_t endpoint;
	uint8_t i;
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;

	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_READ_ON_OFF_STATE;
	p++;
	*p = FIXED_ENDPOINT_COUNT;
	p++;
	len = 2;
	for (i = 0; i < FIXED_ENDPOINT_COUNT; i++) {
		endpoint = emberAfEndpointFromIndex(i);
		if (emberAfEndpointIsEnabled(endpoint) == 0xff ||
			emberAfEndpointIsEnabled(endpoint) == 0xf2) {
			continue;
		}
		*p = endpoint;
		p++;
		emberAfReadServerAttribute(endpoint, ZCL_ON_OFF_CLUSTER_ID,
				ZCL_ON_OFF_ATTRIBUTE_ID, (uint8_t *) &onOff, sizeof(onOff));
		*p = onOff;
		p++;
		len += 2;
	}

	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_GET, len);
}

extern uint8_t vcGetSwLedState(void);
static void vcMfGetLedState(uint8_t *data, uint8_t dataSize)
 {
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;

	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_READ_LED_STATE;
	p++;
	*p = vcGetSwLedState();
	len = 2;

	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_GET, len);
}

static void vcMfGetKidLockState(uint8_t *data, uint8_t dataSize)
 {

	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;

	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_READ_KIDLOCK_STATE;
	p++;

    vcReadMfgClusterServerAttribute(ZCL_VC_MFG_EXTENSION_CLUSTER_ID,
	ZCL_ATTRIBUTE_MFG_KIDSLOCK_STATE_ATTRIBUTE_ID, (uint8_t *) &p,
			sizeof(uint8_t));
	len = 2;

	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_GET, len);
}

static void vcMfGetKeyState(uint8_t *data, uint8_t dataSize)
{
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;

	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_READ_KIDLOCK_STATE;
	p++;

    vcReadMfgClusterServerAttribute(ZCL_VC_MFG_EXTENSION_CLUSTER_ID,
    		ZCL_ATTRIBUTE_MFG_KEY_STATE_ATTRIBUTE_ID, (uint8_t *) p,
			sizeof(uint8_t));
	len = 2;

	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_GET, len);
}

static void vcMfGetNodeInfo(uint8_t *data, uint8_t dataSize)
 {
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;

	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_READ_NODE_INFO;
	p++;
	p[0] = (uint8_t) (VC_DEVICE_T_TYPE >> 8);
	p[1] = (uint8_t) (VC_DEVICE_T_TYPE);

	p[2] = (uint8_t) (VC_DEVICE_APP_VERSION >> 8);
	p[3] = (uint8_t) (VC_DEVICE_APP_VERSION);
	p += 4;
	emberAfGetEui64(p);
	p[12] = 4;
	p += 13;
	memcpy(p, "0102", 4);
	len = 18;

	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_GET, len);
}
//======================================================================
// Set Cmd Handle

static void vcMfSetOnOffState(uint8_t *data, uint8_t dataSize);
static void vcMfSetLEDState(uint8_t *data, uint8_t dataSize);
static void vcMfSetKeyState(uint8_t *data, uint8_t dataSize);

static void vcMfSet(uint8_t *data, uint8_t dataSize)
{
  uint8_t sub = data[0];
  data++;
  dataSize--;
  switch(sub){
  case VC_MF_FRAME_SUB_WRITE_ON_OFF_STATE:
	  vcMfSetOnOffState(data, dataSize);
	  break;
  case VC_MF_FRAME_SUB_WRITE_LED_STATE:
	  vcMfSetLEDState(data, dataSize);
	  break;
  case VC_MF_FRAME_SUB_WRITE_KIDLOCK_STATE:
	  //vcMfSetKidlockState(data, dataSize);
	  break;
  case VC_MF_FRAME_SUB_WRITE_POWER_METER_CAL:
	  //vcMfSetPowerMeterCalibration(data, dataSize);
	  break;
  case VC_MF_FRAME_SUB_WRITE_KEY_STATE:
	  vcMfSetKeyState(data, dataSize);
	  break;
  }
}

static void vcMfSetOnOffState(uint8_t *data, uint8_t dataSize)
 {
	uint8_t num;
	uint8_t endpoint;
	uint8_t i;
	uint8_t len;
	uint8_t *p = data;

	num = p[0];
	p++;
	for (i = 0; i < num; i++) {
		endpoint = emberAfEndpointFromIndex( *p++);
		if (endpoint == 0xFF) {
			continue;
		}
		emberAfWriteAttribute(endpoint,
		ZCL_ON_OFF_CLUSTER_ID,
		ZCL_ON_OFF_ATTRIBUTE_ID,
		CLUSTER_MASK_SERVER, (uint8_t *) p ,
				ZCL_BOOLEAN_ATTRIBUTE_TYPE);
		p++;
	}

	p = sVcMfTestBufSend;
	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_WRITE_ON_OFF_STATE;
	p++;
	*p = num;
	p++;
	len = 2;
	data++;
	for (i = 0; i < num; i++) {
		*p = data[0];
		data += 2;
		p++;
		endpoint = emberAfEndpointFromIndex(data[0]);
		if (endpoint == 0xFF) {
			*p = 0xFF;
		} else {
			emberAfReadServerAttribute(endpoint, ZCL_ON_OFF_CLUSTER_ID,
					ZCL_ON_OFF_ATTRIBUTE_ID, (uint8_t *) p, sizeof(uint8_t));
		}

		p++;
		len += 2;
	}

	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_SET, len);
}
extern void vcMfSetLEDStateCallback(void);
static void vcMfSetLEDState(uint8_t *data, uint8_t dataSize)
{
	uint8_t len;
	uint8_t *p = data;

	emberAfWriteManufacturerSpecificServerAttribute(51,
	ZCL_VC_MFG_EXTENSION_CLUSTER_ID,
	ZCL_ATTRIBUTE_MFG_INDICATOR_STATE_ATTRIBUTE_ID, EMBER_AF_MANUFACTURER_CODE, (uint8_t *) data,
			sizeof(uint8_t));
	vcMfSetLEDStateCallback();
	p = sVcMfTestBufSend;
	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_WRITE_LED_STATE;
	p++;
	*p = vcGetSwLedState();
	len = 2;

	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_SET, len);
}

extern void vcMfSetKeyStateCallback(void);
static void vcMfSetKeyState(uint8_t *data, uint8_t dataSize)
 {
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;

	emberAfWriteManufacturerSpecificServerAttribute(51,
	ZCL_VC_MFG_EXTENSION_CLUSTER_ID,
	ZCL_ATTRIBUTE_MFG_KEY_STATE_ATTRIBUTE_ID, EMBER_AF_MANUFACTURER_CODE, (uint8_t *) data,
			sizeof(uint8_t));
	vcMfSetKeyStateCallback();
	p = sVcMfTestBufSend;
	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_WRITE_KIDLOCK_STATE;
	p++;
    vcReadMfgClusterServerAttribute(ZCL_VC_MFG_EXTENSION_CLUSTER_ID,
    		ZCL_ATTRIBUTE_MFG_KEY_STATE_ATTRIBUTE_ID, (uint8_t *) &p,
			sizeof(uint8_t));
	len = 2;

	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_SET, len);
}
//======================================================================
// Control Cmd Handle
extern void vcBL0937CalEndCallback(void);
extern void vcBL0937CalStartCallback(void);
extern void vcZ3CommissionActive(uint32_t delayMS);
extern void vcHalBL0937CalibrationStore(void);

static void vcMfCtlPowerMeterCalStart(uint8_t *data, uint8_t dataSize);
static void vcMfCtlPowerMeterCalEnd(uint8_t *data, uint8_t dataSize);
static void vcMfCtlNetworkJoin(uint8_t *data, uint8_t dataSize);
static void vcMfCtlNetworkLeave(uint8_t *data, uint8_t dataSize);

static void vcMfCtl(uint8_t *data, uint8_t dataSize) {
	uint8_t sub = data[0];
	data++;
	dataSize--;
	switch (sub) {
	case VC_MF_FRAME_SUB_POWER_METER_CAL_START:
		//vcMfCtlPowerMeterCalStart(data, dataSize);
		break;
	case VC_MF_FRAME_SUB_POWER_METER_CAL_END:
		//vcMfCtlPowerMeterCalEnd(data, dataSize);
		break;
	case VC_MF_FRAME_SUB_NETWORK_JOIN:
		vcMfCtlNetworkJoin(data, dataSize);
		break;
	case VC_MF_FRAME_SUB_NETWORK_LEAVE:
		vcMfCtlNetworkLeave(data, dataSize);
		break;
	}
}

static void vcMfCtlNetworkJoin(uint8_t *data, uint8_t dataSize)
 {
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;

	p = sVcMfTestBufSend;
	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_NETWORK_JOIN;
	p++;
	len = 1;

	vcZ3CommissionActive(0);

	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_CONTROL, len);
}

static void vcMfCtlNetworkLeave(uint8_t *data, uint8_t dataSize)
{
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;

	p = sVcMfTestBufSend;
	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_NETWORK_LEAVE;
	len = 1;
	emberLeaveNetwork();

	vcMfPacket(VC_MF_FRAME_C_RES, VC_MF_FRAME_FN_CONTROL, len);
}
//======================================================================
// Report Cmd Handle
uint8_t vcMfRssi;
 void vcMfReportNetworkState(void)
 {
	uint16_t temp;
	int8_t lastHopRssi;
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;

	p = sVcMfTestBufSend;
	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_NETWORK_STATE;
	len = 1;
	p++;
	p[0] = emberAfNetworkState() == EMBER_JOINED_NETWORK? 1: 0;
	temp = emberAfGetPanId();
	p[1] = (uint8_t) (temp >> 8);
	p[2] = (uint8_t) (temp);
	temp = emberAfGetNodeId();
	p[3] = (uint8_t) (temp >> 8);
	p[4] = (uint8_t) (temp);
	p[5] = emberAfGetRadioChannel();
	p[6] = vcMfRssi;

	len += 7;
	vcMfPacket(VC_MF_FRAME_C_REQ, VC_MF_FRAME_FN_REPORT, len);
}

void vcMfReportNodeInfo(void) {
	uint8_t len;
	uint8_t *p = sVcMfTestBufSend;

	p = sVcMfTestBufSend;
	p += VC_MF_FRAME_PAYLOAD_INDEX;
	*p = VC_MF_FRAME_SUB_NODE_INFO;
	p++;
	len = 1;
	p[0] = (uint8_t) (VC_DEVICE_T_TYPE >> 8);
	p[1] = (uint8_t) (VC_DEVICE_T_TYPE);

	p[2] = (uint8_t) (VC_DEVICE_APP_VERSION >> 8);
	p[3] = (uint8_t) (VC_DEVICE_APP_VERSION);
	p += 4;
	emberAfGetEui64(p);
	p += 8;
	p[0] = 4;
	p += 1;
	memcpy(p, "0102", 4);
	len += 17;
	vcMfPacket(VC_MF_FRAME_C_REQ, VC_MF_FRAME_FN_REPORT, len);
}
#endif
