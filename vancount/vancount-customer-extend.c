/*
 * vancount-customer-extend.c
 *
 *  Created on: Dec 3, 2018
 *      Author: tao
 */
#include "af.h"
#include "string.h"
#include "stdint.h"
#include "vancount-customer-extend.h"


extern uint8_t sVcZ3ExtendBuff[];
extern uint8_t sVcZ3ExtendBuffLen;
static uint8_t *sPayload = sVcZ3ExtendBuff + 5;
extern void VcMfgExtensionClusterZigbeeExtendRsp(uint8_t *data, uint8_t size);
static void vcVceCmdReadParse(uint8_t *data);
static void vcVceCmdWriteParse(uint8_t *data);
//static void vcVceCmdCtlParse(uint8_t *data);
static void vcVceCmdParse(uint8_t *data);
static void vcVceCmdCtlParse(uint8_t *data);

static void vcVceReadSubTimerEventParse(uint8_t *data);
static void vcVceWriteSubTimerEventParse(uint8_t *data);

static void vcVcePacketAndSend(uint8_t cmd, uint16_t subCmd)
{
	sVcZ3ExtendBuff[1] = VC_VCE_FRAME_CTL_DEVICE_TO_GATEWAY;
	sVcZ3ExtendBuff[2] = cmd;
	sVcZ3ExtendBuff[3] = (uint8_t)(subCmd);
	sVcZ3ExtendBuff[4] = (uint8_t)(subCmd >> 8);
	sVcZ3ExtendBuffLen += 4;

	sVcZ3ExtendBuff[0] = sVcZ3ExtendBuffLen;
	VcMfgExtensionClusterZigbeeExtendRsp(sVcZ3ExtendBuff, sVcZ3ExtendBuffLen + 1);
}

static void vcVceErrorReply(uint8_t cmd, uint16_t subCmd, uint8_t code)
{
	sPayload[0] = (uint8_t)subCmd;
	sPayload[1] = (uint8_t)(subCmd >> 8);
	sPayload[2] = code;
	sVcZ3ExtendBuffLen = 3;
	vcVcePacketAndSend(cmd, VC_VCE_FRAME_SUB_ERROR);
}

void vcZ3CustomerExtend(uint8_t *data)
{
	// First is Data Length
	data++;
	emberAfCorePrint("Zigbee Extend:");
	switch(data[VC_VCE_FRAME_CTL_POS]){
	case VC_VCE_FRAME_CTL_GATEWAY_TO_DEVICE:
		vcVceCmdParse(data);
		break;
	}
}

static void vcVceCmdParse(uint8_t *data)
{
	emberAfCorePrint("cmd:%0x", data[VC_VCE_FRAME_CMD_POS]);
	switch(data[VC_VCE_FRAME_CMD_POS]){
	case VC_VCE_FRAME_CMD_READ:
		vcVceCmdReadParse(data);
		break;
	case VC_VCE_FRAME_CMD_WRITE:
		vcVceCmdWriteParse(data);
		break;
	case VC_VCE_FRAME_CMD_CTL:
		vcVceCmdCtlParse(data);
		break;
	}
}

static void vcVceCmdReadParse(uint8_t *data)
{
	uint16_t sub;
	sub = data[VC_VCE_FRAME_SUB_POS + 1];
	sub = data[VC_VCE_FRAME_SUB_POS] | (sub << 8);
	emberAfCorePrint("sub:%0x\r\n", sub);
	switch(sub){
	case VC_VCE_FRAME_SUB_TIME_EVENT:
		vcVceReadSubTimerEventParse(data + VC_VCE_FRAME_PAYLOAD_POS);
		break;
	}
}

static void vcVceCmdWriteParse(uint8_t *data)
{
	uint16_t sub;
	sub = data[VC_VCE_FRAME_SUB_POS + 1];
	sub = data[VC_VCE_FRAME_SUB_POS] | (sub << 8);
	emberAfCorePrint("sub:%02x\r\n", sub);
	switch(sub){
	case VC_VCE_FRAME_SUB_TIME_EVENT:
		vcVceWriteSubTimerEventParse(data + VC_VCE_FRAME_PAYLOAD_POS);
		break;
	}
}
//========================================================================
static void vcVceCtlSubTimerEventParse(uint8_t *data);
static void vcVceCmdCtlParse(uint8_t *data)
{
	uint16_t sub;
	sub = data[VC_VCE_FRAME_SUB_POS + 1];
	sub = data[VC_VCE_FRAME_SUB_POS] | (sub << 8);
	emberAfCorePrint("sub:%02x\r\n", sub);
	switch(sub){
	case VC_VCE_FRAME_SUB_TIME_EVENT:
		vcVceCtlSubTimerEventParse(data + VC_VCE_FRAME_PAYLOAD_POS);
		break;
	}
}
//========================================================================
static void vcVceReadSubTimerEventParse(uint8_t *data)
{
	VceTimerEvent_t *e = vcVceGetTimerEventByID(data[0]);

	if (e == NULL) {
		emberAfCorePrint("error no existing");
		vcVceErrorReply(VC_VCE_FRAME_CMD_READ, VC_VCE_FRAME_SUB_TIME_EVENT, VC_VCE_ERROR_CODE_NO_EXISTING);
	} else {
		sPayload[0] = e->id = data[0];
		sPayload[1] = (uint8_t)e->remainTime;
		sPayload[2] = (uint8_t)(e->remainTime >> 8);
		sPayload[3] = (uint8_t)(e->remainTime >> 16);
		sPayload[4] = (uint8_t)(e->remainTime >> 24);
		sPayload[5] = (uint8_t)e->cluster;
		sPayload[6] = (uint8_t)(e->cluster >> 8);
		sPayload[7] = (uint8_t)e->attribute;
		sPayload[8] = (uint8_t)(e->attribute >> 8);
		sPayload[9] = e->op.level;
		sVcZ3ExtendBuffLen = 10;
		vcVcePacketAndSend(VC_VCE_FRAME_CMD_READ, VC_VCE_FRAME_SUB_TIME_EVENT);
	}
}
//===========================================================================
VceTimerEvent_t *vcVceGetTimerEventByID(uint8_t id)
{
	uint8_t i;
	for (i = 0; i < VC_VCE_TIME_EVENT_MAX_SIZE; i++) {
		if (vcVceTimerEventTable[i].id == id &&
			vcVceTimerEventTable[i].isInvalid == 1) {
			return &vcVceTimerEventTable[i];
		}
	}
	return NULL;
}

VceTimerEvent_t *vcVceGetInvalidTimerEvent(void)
{
	uint8_t i;
	for (i = 0; i < VC_VCE_TIME_EVENT_MAX_SIZE; i++) {
		if (vcVceTimerEventTable[i].isInvalid == 0) {
			return &vcVceTimerEventTable[i];
		}
	}
	return NULL;
}

extern void vcVceWriteSubTimerEventCallback(VceTimerEvent_t *e);
VceTimerEvent_t vcVceTimerEventTable[VC_VCE_TIME_EVENT_MAX_SIZE];
static void vcVceWriteSubTimerEventParse(uint8_t *data)
{
	VceTimerEvent_t *e = vcVceGetTimerEventByID(data[0]);
	if (e == NULL) {
		e = vcVceGetInvalidTimerEvent();
	}
	if (e == NULL) {
		vcVceErrorReply(VC_VCE_FRAME_CMD_WRITE, VC_VCE_FRAME_SUB_TIME_EVENT, VC_VCE_ERROR_CODE_TIME_EVENT_FULL);
	} else {
		e->ep = emberAfCurrentEndpoint();
		e->id = data[0];
		e->remainTime = data[4];
		e->remainTime = data[3] | (e->remainTime << 8);
		e->remainTime = data[2] | (e->remainTime << 8);
		e->remainTime = data[1] | (e->remainTime << 8);
		e->isInvalid = 1;
		e->cluster = data[6];
		e->cluster = data[5] | (e->cluster << 8);
		e->attribute = data[8];
		e->attribute = data[7] | (e->attribute << 8);
		e->op.level = data[9];
		sPayload[0] = e->id;
		sVcZ3ExtendBuffLen = 1;
		vcVceWriteSubTimerEventCallback(e);
		vcVcePacketAndSend(VC_VCE_FRAME_CMD_WRITE, VC_VCE_FRAME_SUB_TIME_EVENT);
	}
}
//===========================================================================
extern void vcVceCtlCancelSubTimerEventCallback(VceTimerEvent_t *e);
static void vcVceCtlSubTimerEventParse(uint8_t *data)
{
	VceTimerEvent_t *e = vcVceGetTimerEventByID(data[1]);
	if (data[0] == 1) {
		if (e == NULL) {
			vcVceErrorReply(VC_VCE_FRAME_CMD_CTL, VC_VCE_FRAME_SUB_TIME_EVENT, VC_VCE_ERROR_CODE_NO_EXISTING);
			return;
		}
		vcVceCtlCancelSubTimerEventCallback(e);
		sPayload[0] = 1;
		sPayload[1] = e->id;
		sVcZ3ExtendBuffLen = 2;
		vcVcePacketAndSend(VC_VCE_FRAME_CMD_CTL, VC_VCE_FRAME_SUB_TIME_EVENT);
	}
}
