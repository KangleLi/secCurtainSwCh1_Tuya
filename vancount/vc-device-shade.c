/*
 * vc-switch.c
 *
 *  Created on: Jun 15, 2018
 *      Author: tao
 */
#include "app/framework/include/af.h"
#include "./vancount-z3-net.h"
#include "./vancount-z3.h"
#include "./list.h"
#include "./vc-onoff.h"
#include "./vc-scenes.h"
#include "vc-device-shade.h"
#include "vcHalLight.h"
#include "./../attribute-id.h"

#define VC_SHADE_NUMBER   (1)
typedef struct VcShadeControl{
	uint8_t state;
	uint8_t setLevel;
	uint8_t shadeOpen;
	uint8_t shadeClose;
	uint32_t position;
	uint32_t curPosition;
	uint8_t level;
}VcShadeControl_t;

enum VcShadeOperate{
	SHADE_CLOSING = 0,
	SHADE_OPENING,
	SHADE_STOP,
};

enum VcShadeSetLevel {
	SET_NONE_LEVEL = 0,
	SET_BUTTON_LEVEL = 1,
	SET_STOP_LEVEL = 2,
};
#define SHADE_CLOSED_LIMIT_MAX  (180000) // 180s

static uint32_t  shadeDurationMs[VC_SHADE_NUMBER] = {0};
static VcShadeControl_t shadeControlArray[VC_SHADE_NUMBER] = {
		{SHADE_STOP, 0, VC_LIGHT_0, VC_LIGHT_1, 0},
};
EmberEventControl customShade0EventControl;
EmberEventControl * shadeEventArray[VC_SHADE_NUMBER] = {
		&customShade0EventControl
};
uint8_t LevelOnOffAction = 1;
extern void vcLightControl(uint8_t light, uint8_t cmd);

void vcShadeInit(void)
{
	uint8_t i;
	uint8_t ep;
	for (i = 0; i < VC_SHADE_NUMBER; i++) {
		shadeControlArray[i].state = SHADE_STOP;
		shadeControlArray[i].position = 0;
	}
	vcShadeAttributeInit();
}

void vcShadeAttributeInit(void)
{
	uint8_t buf[VC_NODE_CONFIG_SIZE];
	uint8_t ep;
	uint8_t level;
	uint8_t i;
	for (i = 0; i < VC_SHADE_NUMBER; i++) {
		ep = emberAfEndpointFromIndex(i);
		emberAfReadServerAttribute(ep,
		ZCL_LEVEL_CONTROL_CLUSTER_ID,
		ZCL_CURRENT_LEVEL_ATTRIBUTE_ID, (uint8_t *) &level, sizeof(level));
		shadeControlArray[i].position = vcGetShadePositionFromLevel(i, level);
		shadeControlArray[i].level = level;
		emberAfCorePrintln("shadeControlArray[%d] position:%d", i, shadeControlArray[i].position);
	}

	vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
	vcWriteMfgClusterServerAttribute(ZCL_VC_MFG_EXTENSION_CLUSTER_ID, ZCL_ATTRIBUTE_MFG_INDICATOR_STATE_ATTRIBUTE_ID, &buf[VC_CONFIG_LED_STATE], ZCL_INT8U_ATTRIBUTE_TYPE);
	vcWriteMfgClusterServerAttribute(ZCL_VC_MFG_EXTENSION_CLUSTER_ID, ZCL_ATTRIBUTE_MFG_TIME_SCALE_ATTRIBUTE_ID, &buf[VC_CONFIG_TIME_SCALE], ZCL_INT8U_ATTRIBUTE_TYPE);
}
extern void vcReportAnAttributeEx(uint8_t srcEp, EmberAfClusterId clusterId, EmberAfAttributeId attributeId, uint8_t isMfg);
extern EmberAfStatus vcWriteMfgClusterServerAttribute( EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t dataType);
static void shadeOpen(uint8_t index, uint32_t pos)
{
	uint8_t data;
	uint8_t ep;
	if (shadeControlArray[index].state != SHADE_STOP){
		vcLightControl(shadeControlArray[index].shadeClose, 0);
	}
	data = 0;
	ep = emberAfEndpointFromIndex(index);
	vcWriteMfgClusterServerAttributeEx(ep, ZCL_VC_MFG_EXTENSION_CLUSTER_ID, ZCL_ATTRIBUTE_MFG_MOVE_DIRECTION_ATTRIBUTE_ID, &data, ZCL_INT8U_ATTRIBUTE_TYPE);
	vcReportAnAttributeEx(ep, ZCL_VC_MFG_EXTENSION_CLUSTER_ID, ZCL_ATTRIBUTE_MFG_MOVE_DIRECTION_ATTRIBUTE_ID, 1);
	vcLightControl(shadeControlArray[index].shadeOpen, 1);
	emberEventControlSetDelayMS(*(shadeEventArray[index]),
							(uint16_t )pos);
	shadeDurationMs[index] = halCommonGetInt32uMillisecondTick();
	shadeControlArray[index].state = SHADE_OPENING;
}

static void shadeClose(uint8_t index, uint32_t pos)
{
	uint8_t data;
	uint8_t ep;
	if (shadeControlArray[index].state != SHADE_STOP){
		vcLightControl(shadeControlArray[index].shadeOpen, 0);
	}
	data = 1;
	ep = emberAfEndpointFromIndex(index);
	vcWriteMfgClusterServerAttributeEx(ep, ZCL_VC_MFG_EXTENSION_CLUSTER_ID, ZCL_ATTRIBUTE_MFG_MOVE_DIRECTION_ATTRIBUTE_ID, &data, ZCL_INT8U_ATTRIBUTE_TYPE);
	vcReportAnAttributeEx(ep, ZCL_VC_MFG_EXTENSION_CLUSTER_ID, ZCL_ATTRIBUTE_MFG_MOVE_DIRECTION_ATTRIBUTE_ID, 1);
	vcLightControl(shadeControlArray[index].shadeClose, 1);
	emberEventControlSetDelayMS(*(shadeEventArray[index]),
							(uint16_t )pos);
	shadeDurationMs[index] = halCommonGetInt32uMillisecondTick();
	shadeControlArray[index].state = SHADE_CLOSING;
}

extern uint8_t vcStoreClosedLimit(uint8_t shadeIndex, uint32_t position);

static uint32_t shadePositionRecomputer(uint8_t ep, uint8_t index, uint32_t lastPosition)
{
	uint16_t closedLimit;
	uint32_t movingPosition;

	emberAfReadServerAttribute(ep,
	ZCL_SHADE_CONFIG_CLUSTER_ID,
	ZCL_SHADE_CONFIG_CLOSED_LIMIT_ATTRIBUTE_ID, (uint8_t *) &closedLimit,
			sizeof(closedLimit));
	emberAfCorePrintln("shadeDurationMs[%d]:%d", index, shadeDurationMs[index]);
	// When the shadeDurationMs % 10 > 5, movingPosition + 1
	movingPosition = (shadeDurationMs[index] + 5) / 10;
	lastPosition /= 10;

	if (movingPosition > closedLimit) {
		movingPosition = closedLimit;
	}

	if (shadeControlArray[index].state == SHADE_CLOSING) {
		lastPosition += movingPosition;
		if (lastPosition > closedLimit) {
			lastPosition = closedLimit;
		}
	} else {
		if (lastPosition > movingPosition) {
			lastPosition -= movingPosition;
		} else {
			lastPosition = 0;
		}
	}
	lastPosition *= 10;
	emberAfCorePrintln("shade[%d] cur pos:%d", index, lastPosition);

	return lastPosition;
}

static void shadeStop(uint8_t index, uint8_t reporting)
{
	uint8_t ep;
	uint8_t mode;
	uint8_t level;
	uint16_t closedLimit;
	uint32_t movingPosition;

	emberEventControlSetInactive(*(shadeEventArray[index]));

	if (shadeControlArray[index].state == SHADE_CLOSING) {
		vcLightControl(shadeControlArray[index].shadeClose, 0);
	}
	if (shadeControlArray[index].state == SHADE_OPENING) {
		vcLightControl(shadeControlArray[index].shadeOpen, 0);
	}
	shadeDurationMs[index] = elapsedTimeInt32u(shadeDurationMs[index],
			halCommonGetInt32uMillisecondTick());
	if (shadeDurationMs[index] > 400) {
		shadeDurationMs[index] -= 400;
	}
	ep = emberAfEndpointFromIndex(index);
	emberAfReadServerAttribute(ep,
	ZCL_SHADE_CONFIG_CLUSTER_ID,
	ZCL_SHADE_CONFIG_MODE_ATTRIBUTE_ID, (uint8_t *) &mode, sizeof(mode));

	if (mode == 1) {
		vcStoreClosedLimit(index, shadeDurationMs[index]);
	}
	emberAfCorePrintln("setLevel:%0d", shadeControlArray[index].setLevel);
	if (shadeControlArray[index].setLevel == SET_BUTTON_LEVEL ||
		shadeControlArray[index].setLevel == SET_STOP_LEVEL) {
		shadeControlArray[index].position =
				shadePositionRecomputer(ep, index, shadeControlArray[index].curPosition);

		level = vcSetShadePositionToLevel(index,
				shadeControlArray[index].position);
		emberAfWriteServerAttribute(ep,
		ZCL_LEVEL_CONTROL_CLUSTER_ID,
		ZCL_CURRENT_LEVEL_ATTRIBUTE_ID, (uint8_t *) &level,
				ZCL_INT8U_ATTRIBUTE_TYPE);
	}

	if (shadeControlArray[index].setLevel == SET_NONE_LEVEL){
		emberAfReadServerAttribute(ep,
		ZCL_LEVEL_CONTROL_CLUSTER_ID,
		ZCL_CURRENT_LEVEL_ATTRIBUTE_ID, (uint8_t *) &level, sizeof(level));

	}
	emberAfCorePrintln("stop: shade:%d, pos:%d level:%d", index,
			shadeControlArray[index].position, level);
	if (reporting) {
		vcReportAnAttribute(0x00, ep, 0x01, ZCL_LEVEL_CONTROL_CLUSTER_ID,
				ZCL_CURRENT_LEVEL_ATTRIBUTE_ID, 1);
		vcResetReportingInterval();
	}

	shadeControlArray[index].level = level;
	shadeControlArray[index].curPosition = shadeControlArray[index].position;
	shadeControlArray[index].state = SHADE_STOP;
	shadeControlArray[index].setLevel = SET_NONE_LEVEL;
}

void customShade0EventHandler(void)
{
	shadeEventHandler(0);
}

void shadeEventHandler(uint8_t index)
{
	vcShadeStartStop(index);
}

void vcShadeStartStop(uint8_t shadeIndex)
{
	shadeStop(shadeIndex, 1);
}

void vcShadeOperateByPosition(uint8_t shadeIndex, uint32_t position)
{
	uint32_t movingPos;
	uint32_t newPosition;

//	switch (shadeControlArray[shadeIndex].state) {
//	case SHADE_CLOSING:
//		shadeStop(shadeIndex, 0);
//		break;
//	case SHADE_OPENING:
//		shadeStop(shadeIndex, 0);
//		break;
//	case SHADE_STOP:
//		break;
//	}
	if (shadeControlArray[shadeIndex].state == SHADE_CLOSING ||
			shadeControlArray[shadeIndex].state == SHADE_OPENING){
		uint8_t ep;
		uint16_t closedLimit;

		shadeDurationMs[shadeIndex] = elapsedTimeInt32u(shadeDurationMs[shadeIndex],
				halCommonGetInt32uMillisecondTick());
		if (shadeDurationMs[shadeIndex] > 400) {
			shadeDurationMs[shadeIndex] -= 400;
		}
		ep = emberAfEndpointFromIndex(shadeIndex);
		emberAfReadServerAttribute(ep,
		ZCL_SHADE_CONFIG_CLUSTER_ID,
		ZCL_SHADE_CONFIG_CLOSED_LIMIT_ATTRIBUTE_ID, (uint8_t *) &closedLimit,
				sizeof(closedLimit));
		emberAfCorePrintln("re shadeDurationMs[%d]:%d", shadeIndex, shadeDurationMs[shadeIndex]);
		// When the shadeDurationMs % 10 > 5, movingPosition + 1
		movingPos = (shadeDurationMs[shadeIndex] + 5) / 10;
		newPosition = shadeControlArray[shadeIndex].curPosition / 10;

		if (movingPos > closedLimit) {
			movingPos = closedLimit;
		}

		if (shadeControlArray[shadeIndex].state == SHADE_CLOSING) {
			newPosition += movingPos;
			if (newPosition > closedLimit) {
				newPosition = closedLimit;
			}
		} else {
			if (newPosition > movingPos) {
				newPosition -= movingPos;
			} else {
				newPosition = 0;
			}
		}
		newPosition *= 10;
		shadeControlArray[shadeIndex].position = newPosition;
		emberAfCorePrintln("shade[%d] repos:%d", shadeIndex, shadeControlArray[shadeIndex].position);
	}

	if (position > shadeControlArray[shadeIndex].position) {
		movingPos = position - shadeControlArray[shadeIndex].position + 400;
		emberAfCorePrintln("closing pos:%d", movingPos);
		shadeClose(shadeIndex, movingPos);
		shadeControlArray[shadeIndex].position = position;
		return;
	}

	if (position < shadeControlArray[shadeIndex].position) {
		movingPos = shadeControlArray[shadeIndex].position - position + 400;
		emberAfCorePrintln("opening pos:%d", movingPos);
		shadeOpen(shadeIndex, movingPos);
		shadeControlArray[shadeIndex].position = position;
		return;
	}
}

void vcShadeOperateFromLevelAbort(uint8_t endpoint)
{
	uint8_t index;

	index = emberAfIndexFromEndpoint(endpoint);
	if (shadeControlArray[index].state == SHADE_STOP) {
		return;
	}
	shadeControlArray[index].setLevel = SET_STOP_LEVEL;
	emberAfCorePrintln("Stop the Level");
	vcShadeStartStop(index);
}

void vcShadeOperateFromLevel(uint8_t endpoint)
{
	uint8_t level;
	uint16_t position;
	uint8_t index;
	
	index = emberAfIndexFromEndpoint(endpoint);
	// Return When the button is operating
	if (shadeControlArray[index].setLevel != SET_NONE_LEVEL) {
		return;
	}

	switch (shadeControlArray[index].state) {
	case SHADE_CLOSING:
	case SHADE_OPENING:
		shadeControlArray[index].setLevel = SET_STOP_LEVEL;
		shadeStop(index, 0);
		break;
	}
	shadeDurationMs[index] = halCommonGetInt32uMillisecondTick();
	shadeControlArray[index].setLevel = SET_NONE_LEVEL;
	emberAfReadServerAttribute(endpoint,
	ZCL_LEVEL_CONTROL_CLUSTER_ID,
	ZCL_CURRENT_LEVEL_ATTRIBUTE_ID, (uint8_t *) &level, sizeof(level));
	emberAfCorePrintln("curLevel:%d newLevel:%d", shadeControlArray[index].level, level);
	if (shadeControlArray[index].level == level) {
		return;
	}
	position = vcGetShadePositionFromLevel((index), (level));
	emberAfCorePrintln("index:%d, position:%d, level:%d", index, position,
			level);
	vcShadeOperateByPosition(index, position);
}

uint32_t vcGetShadePositionFromLevel(uint8_t shadeIndex, uint8_t level)
{
	uint16_t closedLimit;
	uint8_t endpoint = emberAfEndpointFromIndex(shadeIndex);
	if (emberAfReadServerAttribute(endpoint,
	ZCL_SHADE_CONFIG_CLUSTER_ID,
	ZCL_SHADE_CONFIG_CLOSED_LIMIT_ATTRIBUTE_ID, (uint8_t *) &closedLimit,
			sizeof(closedLimit)) == EMBER_ZCL_STATUS_SUCCESS) {
		return (closedLimit * (255 - level) / 255) * 10;
	}
	return 0;
}

uint8_t vcSetShadePositionToLevel(uint8_t shadeIndex, uint32_t position)
{
	uint16_t closedLimit;
	uint8_t endpoint = emberAfEndpointFromIndex(shadeIndex);
	uint8_t level;
	
	emberAfReadServerAttribute(endpoint,
		ZCL_SHADE_CONFIG_CLUSTER_ID,
		ZCL_SHADE_CONFIG_CLOSED_LIMIT_ATTRIBUTE_ID, (uint8_t *) &closedLimit,
				sizeof(closedLimit));
	if (closedLimit == 0) {
		closedLimit = 255;
	}
	position /= 10;
	if (position > closedLimit) {
		position = closedLimit;
	}
	level = 255 - (position * 255) / closedLimit;
	if (level > 254) {
		level = 254;
	}
	if (level < 1) {
		level = 0;
	}
	return level;
}

uint8_t vcStoreClosedLimit(uint8_t shadeIndex, uint32_t position)
{
	uint16_t closedLimit;
	uint8_t endpoint = emberAfEndpointFromIndex(shadeIndex);
	uint8_t level;

	closedLimit = position / 10;
	if (closedLimit == 0) {
		closedLimit = 255;
	}
	emberAfWriteServerAttribute(endpoint,
	ZCL_SHADE_CONFIG_CLUSTER_ID,
	ZCL_SHADE_CONFIG_CLOSED_LIMIT_ATTRIBUTE_ID, (uint8_t *) &closedLimit,
			ZCL_INT8U_ATTRIBUTE_TYPE);
}

//===================================================================================
static uint8_t vcGetShadeIndexFromButton(uint8_t buttonIndex)
{
	uint8_t i;
	for (i = 0; i < VC_SHADE_NUMBER; i++) {
		if (shadeControlArray[i].shadeOpen == buttonIndex) {
			return i;
		}
		if (shadeControlArray[i].shadeClose == buttonIndex) {
			return i;
		}
	}
	return 0xff;
}

static void vcShadeHandleShortPressed(uint8_t shadeIndex, uint8_t buttonIndex)
{
	switch (shadeControlArray[shadeIndex].state) {
	case SHADE_CLOSING:
		shadeStop(shadeIndex, 1);
		break;
	case SHADE_OPENING:
		shadeStop(shadeIndex, 1);
		break;
	case SHADE_STOP:
		if (shadeControlArray[shadeIndex].shadeClose == buttonIndex) {
			shadeClose(shadeIndex, SHADE_CLOSED_LIMIT_MAX);
		}
		if (shadeControlArray[shadeIndex].shadeOpen == buttonIndex) {
			shadeOpen(shadeIndex, SHADE_CLOSED_LIMIT_MAX);
		}
		break;
	}
}

static void vcShadeHandleLongPressed(uint8_t shadeIndex, uint8_t buttonIndex)
{
	switch (shadeControlArray[shadeIndex].state) {
	case SHADE_CLOSING:
		break;
	case SHADE_OPENING:
		break;
	case SHADE_STOP:
		if (shadeControlArray[shadeIndex].shadeClose == buttonIndex) {
			shadeClose(shadeIndex, SHADE_CLOSED_LIMIT_MAX);
		}
		if (shadeControlArray[shadeIndex].shadeOpen == buttonIndex) {
			shadeOpen(shadeIndex, SHADE_CLOSED_LIMIT_MAX);
		}
		break;
	}
}

static void vcShadeHandleLongReleased(uint8_t shadeIndex, uint8_t buttonIndex)
{
	switch (shadeControlArray[shadeIndex].state) {
	case SHADE_CLOSING:
		shadeStop(shadeIndex, 1);
		break;
	case SHADE_OPENING:
		shadeStop(shadeIndex, 1);
		break;
	case SHADE_STOP:
		break;
	}
}

void vcShadeOperateByButton(uint8_t buttonIndex, uint8_t state)
{
	uint8_t shadeIndex = vcGetShadeIndexFromButton(buttonIndex);
	
	shadeControlArray[shadeIndex].setLevel = SET_BUTTON_LEVEL;
	switch (state) {
	case 0: //Short pressed and release
		vcShadeHandleShortPressed(shadeIndex, buttonIndex);
		break;
	case 1: //Long pressed
		vcShadeHandleLongPressed(shadeIndex, buttonIndex);
		break;
	case 2: //Long release
		vcShadeHandleLongReleased(shadeIndex, buttonIndex);
		break;
	}
}
//================================================================
void vcShadeResetInit(void)
{
	uint8_t buf[VC_NODE_CONFIG_SIZE];
	bool onOff;
	uint8_t endpoint;
	uint8_t i;

	vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
	buf[VC_CONFIG_LED_STATE] = 0; // VC_LED_INDICATOR_STATE
	buf[VC_CONFIG_TIME_SCALE] = 0;
	vcSetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
	vcUpdateLedIndicator(buf[VC_CONFIG_LED_STATE]);
}


extern void vcUpdateLedIndicator(uint8_t state);
void vcShadeSetNodeConfig(uint8_t ep, uint16_t clusterId, uint16_t attributeId, uint8_t *data)
{
	uint8_t buf[VC_NODE_CONFIG_SIZE];

	if (clusterId == ZCL_VC_MFG_SYS_CONFIG_CLUSTER_ID &&
			attributeId == ZCL_ATTRIBUTE_MFG_SYSTEM_MODE_ATTRIBUTE_ID) {
		vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
		buf[VC_CONFIG_NODE_MODE] = data[0];
		vcSetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
		return;
	}

	if (clusterId == ZCL_VC_MFG_EXTENSION_CLUSTER_ID &&
			attributeId == ZCL_ATTRIBUTE_MFG_INDICATOR_STATE_ATTRIBUTE_ID) {
		vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
		buf[VC_CONFIG_LED_STATE] = data[0];
		vcSetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
		vcUpdateLedIndicator(data[0]);
		return;
	}

	if (clusterId == ZCL_VC_MFG_EXTENSION_CLUSTER_ID &&
				attributeId == ZCL_ATTRIBUTE_MFG_TIME_SCALE_ATTRIBUTE_ID) {
		vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
		buf[VC_CONFIG_TIME_SCALE] = data[0];
		vcSetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
		vcUpdateLedIndicator(data[0]);
		return;
	}

	if (vcGetNodeModeState(ep) == VC_DOUBLE_CONTROL_SWITCH &&
			(clusterId == ZCL_VC_MFG_EXTENSION_CLUSTER_ID ||
			attributeId == ZCL_ATTRIBUTE_MFG_CLIENT_ONOFF_STATE_ATTRIBUTE_ID)) {
		uint8_t light;
		vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
		light = emberAfIndexFromEndpoint(ep);
		vcLightIndicatorControl(light, data[0]);
		gVcNextState[light] = data[0]? 0: 1;
		return;
	}
}


void vcMfSetKeyStateCallback(uint8_t state)
{
	uint8_t buf[VC_NODE_CONFIG_SIZE];

	vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
    vcReadMfgClusterServerAttribute(ZCL_VC_MFG_EXTENSION_CLUSTER_ID,
    		ZCL_ATTRIBUTE_MFG_KEY_STATE_ATTRIBUTE_ID, (uint8_t *) &buf[VC_CONFIG_KEY_STATE],
			sizeof(uint8_t));
    vcSetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
}

void vcMfSetPowerMeterCalibrationCallback(void)
{

}

void vcMfSetManufactureStateCallback(uint8_t type)
{
	uint8_t buf[VC_NODE_CONFIG_SIZE];
	vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
	buf[VC_CONFIG_MANUFACTURE_STATE] = type;
	vcSetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
}

uint8_t vcMfGetManufactureStateCallback(void)
{
	uint8_t buf[VC_NODE_CONFIG_SIZE];
	vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
	return buf[VC_CONFIG_MANUFACTURE_STATE];
}

void vcMfSetLEDStateCallback(void)
{
	uint8_t buf[VC_NODE_CONFIG_SIZE];
	vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
	buf[VC_CONFIG_LED_STATE] = vcGetSwLedState();
	vcSetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
}

uint8_t vcMfGetTimeScaleCallback(void)
{
	uint8_t buf[VC_NODE_CONFIG_SIZE];
	vcGetNodeConfig(buf, VC_NODE_CONFIG_SIZE);
	return buf[VC_CONFIG_TIME_SCALE];
}
