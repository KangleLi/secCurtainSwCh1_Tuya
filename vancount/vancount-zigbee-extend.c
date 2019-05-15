/*
 * vancount-mf-test.c
 *
 *  Created on: May 16, 2018
 *      Author: tao
 */
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "app/framework/include/af.h"

#define VC_Z3_EXTEND_BUFF_SIZE  ((uint8_t)24)
uint8_t sVcZ3ExtendBuff[VC_Z3_EXTEND_BUFF_SIZE];
uint8_t sVcZ3ExtendBuffLen = 0;
static uint8_t *sPayload = sVcZ3ExtendBuff + 4;
//=============================================================
extern void VcMfgExtensionClusterTransparentSendResponse(uint8_t *data, uint8_t size);
static void vcZ3ExtendSend(void)
{
	VcMfgExtensionClusterTransparentSendResponse(sVcZ3ExtendBuff, sVcZ3ExtendBuffLen);
}
//=============================================================
static void vcZ3ExtendMfTestPacketAndSend(uint16_t cmd, uint16_t subCmd)
{
	sVcZ3ExtendBuff[0] = (uint8_t)(cmd);
	sVcZ3ExtendBuff[1] = (uint8_t)(cmd >> 8);
	sVcZ3ExtendBuff[2] = (uint8_t)(subCmd);
	sVcZ3ExtendBuff[3] = (uint8_t)(subCmd >> 8);
	sVcZ3ExtendBuffLen += 4;
	VcMfgExtensionClusterTransparentSendResponse(sVcZ3ExtendBuff, sVcZ3ExtendBuffLen);

	//vcZ3ExtendSend();
}
//=============================================================
// Reverse the bits in a byte
static uint8_t vcReverse(uint8_t b)
{
#if defined(EZSP_HOST) || defined(BOARD_SIMULATION)
  return ((b * 0x0802UL & 0x22110UL) | (b * 0x8020UL & 0x88440UL)) * 0x10101UL >> 16;
#else
  return (__RBIT((uint32_t)b) >> 24); // Cortex function __RBIT uses uint32_t
#endif // EZSP_HOST
}

uint16_t getInstallCodeCrc(uint8_t *installCode, uint8_t length)
{
    uint16_t crc = 0xFFFF;
    uint8_t index;
    for (index = 0; index < length; index++)
    {
        crc = halCommonCrc16(vcReverse(installCode[index]), crc);
        //crc = halCommonCrc16(installCode[index], crc);
    }
    crc = ~HIGH_LOW_TO_INT(vcReverse(LOW_BYTE(crc)), vcReverse(HIGH_BYTE(crc)));
    return crc;
}
//=============================================================
#define VC_MF_READ_CMD                      ((uint16_t)0x0005)
#define VC_MF_WRITE_CMD                     ((uint16_t)0x0006)
#define VC_MF_CTL_CMD                       ((uint16_t)0x0007)
//--------
// read sub cmd
#define VC_MF_READ_CALIBRATION        ((uint16_t)0x0003)
#define VC_MF_READ_MEASUREMENT        ((uint16_t)0x0005)
#define VC_MF_READ_FIRMWARE_VERSION   ((uint16_t)0x00FB)
#define VC_MF_READ_MANUFACTURE_STATE  ((uint16_t)0x00FC)
#define VC_MF_READ_INSTALL_CODE       ((uint16_t)0x00FD)
//--------
// write sub cmd
#define VC_MF_WRITE_CALIBRATION        ((uint16_t)0x0003)
#define VC_MF_WRITE_MANUFACTURE_STATE  ((uint16_t)0x00FC)
#define VC_MF_WRITE_INSTALL_CODE       ((uint16_t)0x00FD)
//--------
// ctl sub cmd
#define VC_MF_CTL_CALIBRATION_START           ((uint16_t)0x0000)
#define VC_MF_CTL_CALIBRATION_END             ((uint16_t)0x0001)
#define VC_MF_CTL_OVER_POWER_TEST_START       ((uint16_t)0x0004)
#define VC_MF_CTL_OVER_POWER_TEST_END         ((uint16_t)0x0005)
//===========================================================
static void vzeTestResponseError(uint16_t cmd, uint16_t subCmd, uint8_t error)
{
	sPayload[0] = error;
	sVcZ3ExtendBuffLen = 1;
	vcZ3ExtendMfTestPacketAndSend(cmd, subCmd);
}
//===========================================================
//--------
static void vzeTestReadCalibration(uint8_t *data)
{
	sPayload[0] = 0x86;
	sVcZ3ExtendBuffLen = 1;
	vcZ3ExtendMfTestPacketAndSend(VC_MF_READ_CMD, VC_MF_READ_CALIBRATION);
}
//--------
static void vzeTestReadMeasurement(uint8_t *data)
{
	sPayload[0] = 0x86;
	sVcZ3ExtendBuffLen = 1;
	vcZ3ExtendMfTestPacketAndSend(VC_MF_READ_CMD, VC_MF_READ_MEASUREMENT);
}
//--------
static void vzeTestReadFirmwareVersion(uint8_t *data)
{
	uint32_t ver = EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_FIRMWARE_VERSION;
	sPayload[0] = 0x0;
	sPayload[1] = (uint8_t)(ver >> 24);
	sPayload[2] = (uint8_t)(ver >> 16);
	sPayload[3] = (uint8_t)(ver >> 8);
	sPayload[4] = (uint8_t)(ver);
	sVcZ3ExtendBuffLen = 5;
	vcZ3ExtendMfTestPacketAndSend(VC_MF_READ_CMD, VC_MF_READ_FIRMWARE_VERSION);
}
//--------
static void vzeTestReadManufactureState(uint8_t *data)
{
	sPayload[0] = 0x0;
	sPayload[1] = vcMfGetManufactureStateCallback();
	sVcZ3ExtendBuffLen = 2;
	vcZ3ExtendMfTestPacketAndSend(VC_MF_READ_CMD, VC_MF_READ_MANUFACTURE_STATE);
}
//--------
static void vzeTestReadInstallCode(uint8_t *data)
{
	uint8_t i;
	uint8_t len;
	uint16_t crc;

	halCommonGetToken(sPayload, TOKEN_MFG_INSTALLATION_CODE);
	switch (sPayload[0]) {
	case 0:
		len = 6;
		break;
	case 2:
		len = 8;
		break;
	case 4:
		len = 12;
		break;
	case 6:
		len = 16;
		break;
	default:
			len = 16;
			sPayload[0] = 6;
			break;
	}
	len += 4;

	for (i = 2; i < len; i++) {
		sPayload[i - 1] = sPayload[i];
	}
	sVcZ3ExtendBuffLen = i - 1;

	vcZ3ExtendMfTestPacketAndSend(VC_MF_READ_CMD, VC_MF_READ_INSTALL_CODE);
}

static uint8_t vcZ3ExtendForMfTestRead(uint8_t *data)
{
	uint16_t subCmd;
	subCmd = (uint16_t) (data[0]) + (uint16_t) (data[1] << 8);
	data += 2;

	switch (subCmd) {
	case VC_MF_READ_CALIBRATION:
		vzeTestReadCalibration(data);
		break;
	case VC_MF_READ_MEASUREMENT:
		vzeTestReadMeasurement(data);
		break;
	case VC_MF_READ_FIRMWARE_VERSION:
		vzeTestReadFirmwareVersion(data);
		break;
	case VC_MF_READ_MANUFACTURE_STATE:
		vzeTestReadManufactureState(data);
		break;
	case VC_MF_READ_INSTALL_CODE:
		vzeTestReadInstallCode(data);
		break;
	}
}
//-----------------------------------------------------------
extern void vcMfSetPowerMeterCalibrationCallback(void);
extern void vcMfSetKeyStateCallback(void);
extern void vcMfSetLEDStateCallback(void);
extern void vcMfSetManufactureStateCallback(uint8_t type);
//--------
static void vzeTestWriteCalibration(uint8_t *data)
{
	sPayload[0] = 0x86;
	sVcZ3ExtendBuffLen = 1;
	vcZ3ExtendMfTestPacketAndSend(VC_MF_WRITE_CMD, VC_MF_WRITE_CALIBRATION);
}
//--------
static void vzeTestWriteManufactureState(uint8_t *data)
{
	if (data[0] != 1) {
		vzeTestResponseError(VC_MF_WRITE_CMD, VC_MF_WRITE_MANUFACTURE_STATE, 0x81);
		return;
	}
	data++;
	if (data[0] == 1) {
		vcMfSetManufactureStateCallback(data[0]);
	}
	sPayload[0] = 0x00;
	sPayload[1] = vcMfGetManufactureStateCallback();
	sVcZ3ExtendBuffLen = 2;
	vcZ3ExtendMfTestPacketAndSend(VC_MF_WRITE_CMD, VC_MF_WRITE_MANUFACTURE_STATE);
}
//--------
static void vzeTestWriteInstallCode(uint8_t *data)
{
	uint8_t i;
	uint8_t len;
	uint16_t crc;
	switch(data[1]) {
	case 0:
		len = 6;
		break;
	case 2:
		len = 8;
		break;
	case 4:
		len = 12;
		break;
	case 6:
		len = 16;
		break;
	}
	len += 4;

	if (data[0] != (len - 1)) {
		vzeTestResponseError(VC_MF_WRITE_CMD, VC_MF_WRITE_INSTALL_CODE, 0x81);
		return;
	}

	if (data[1] == 6) {
		data[0] = data[1];
		data[1] = 0;
		crc = getInstallCodeCrc(data + 2, len - 4);
		if ((uint8_t)(crc) != data[len - 2] ||
			(uint8_t)(crc >> 8) != data[len - 1]) {
			vzeTestResponseError(VC_MF_WRITE_CMD, VC_MF_WRITE_INSTALL_CODE, 0x82);
			return;
		}
		// TODO: Check CRC
		volatile uint16_t buf[10];
		memcpy((uint8_t *)buf, data, len);
		halCommonSetToken(TOKEN_MFG_INSTALLATION_CODE, (uint8_t *)buf);
	}
	halCommonGetToken(sPayload, TOKEN_MFG_INSTALLATION_CODE);

	for (i = 2; i < len; i++) {
		sPayload[i - 1] = sPayload[i];
	}
	sVcZ3ExtendBuffLen = i - 1;

	vcZ3ExtendMfTestPacketAndSend(VC_MF_WRITE_CMD, VC_MF_WRITE_INSTALL_CODE);
}

static uint8_t vcZ3ExtendForMfTestWrite(uint8_t *data)
{
	uint16_t subCmd;
	subCmd = (uint16_t) (data[0]) + (uint16_t) (data[1] << 8);
	data += 2;

	switch (subCmd) {
	case VC_MF_WRITE_CALIBRATION:
		vzeTestWriteCalibration(data);
		break;
	case VC_MF_WRITE_MANUFACTURE_STATE:
		vzeTestWriteManufactureState(data);
		break;
	case VC_MF_WRITE_INSTALL_CODE:
		vzeTestWriteInstallCode(data);
		break;
	}
}
//-----------------------------------------------------------
//--------
static void vzeTestCtlCalibrationStart(uint8_t *data)
{
#if 1
	sPayload[0] = 0x86;
	sVcZ3ExtendBuffLen = 1;
	vcZ3ExtendMfTestPacketAndSend(VC_MF_CTL_CMD, VC_MF_CTL_CALIBRATION_START);
#endif
}
//--------
static void vzeTestCtlCalibrationEnd(uint8_t *data)
{
#if 1
	sPayload[0] = 0x86;
	sVcZ3ExtendBuffLen = 1;
	vcZ3ExtendMfTestPacketAndSend(VC_MF_CTL_CMD, VC_MF_CTL_CALIBRATION_END);
#endif
}
//--------
static void vzeTestCtlOverPowerStart(uint8_t *data)
{
#if 1
	sPayload[0] = 0x86;
	sVcZ3ExtendBuffLen = 1;
	vcZ3ExtendMfTestPacketAndSend(VC_MF_CTL_CMD, VC_MF_CTL_OVER_POWER_TEST_START);
#endif
}
//--------
static void vzeTestCtlOverPowerEnd(uint8_t *data)
{
#if 1
	sPayload[0] = 0x86;
	sVcZ3ExtendBuffLen = 1;
	vcZ3ExtendMfTestPacketAndSend(VC_MF_CTL_CMD, VC_MF_CTL_OVER_POWER_TEST_END);
#endif
}

static uint8_t vcZ3ExtendForMfTestCtl(uint8_t *data)
{
	uint16_t subCmd;
	subCmd = (uint16_t) (data[0]) + (uint16_t) (data[1]  << 8);
	data += 2;

	switch (subCmd) {
	case VC_MF_CTL_CALIBRATION_START:
		vzeTestCtlCalibrationStart(data);
		break;
	case VC_MF_CTL_CALIBRATION_END:
		vzeTestCtlCalibrationEnd(data);
		break;
	case VC_MF_CTL_OVER_POWER_TEST_START:
		vzeTestCtlOverPowerStart(data);
		break;
	case VC_MF_CTL_OVER_POWER_TEST_END:
		vzeTestCtlOverPowerEnd(data);
		break;
	}
}
//-----------------------------------------------------------
static uint8_t vcZ3ExtendForMfTest(uint8_t *data)
{
  uint16_t cmd;
  cmd = (uint16_t)(data[0]) + (uint16_t)(data[1]  << 8);
  data += 2;

  switch(cmd){
  case VC_MF_READ_CMD:
	  vcZ3ExtendForMfTestRead(data);
	  break;
  case  VC_MF_WRITE_CMD:
	  vcZ3ExtendForMfTestWrite(data);
	  break;
  case	VC_MF_CTL_CMD:
	  vcZ3ExtendForMfTestCtl(data);
	  break;
  }
}
//-----------------------------------------------------------
void vcZ3ExtendParse(uint8_t *data, uint8_t size)
{
	if (size == 0) {
		vcZ3ExtendForMfTest(data);
	}
}
