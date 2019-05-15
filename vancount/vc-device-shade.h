/*
 * vc-swtich.h
 *
 *  Created on: Jun 15, 2018
 *      Author: tao
 */

#ifndef VANCOUNT_VC_DEVICE_SHADE_H_
#define VANCOUNT_VC_DEVICE_SHADE_H_

#define VC_NODE_MODE                (uint8_t)((EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_IMAGE_TYPE_ID & 0x00F0) >> 4)
#define NODE_MODE_CONFIG   0
extern uint8_t LevelOnOffAction;
#define vc_level_no_action_when_onoff()        do{LevelOnOffAction = 0;} while(0)
#define vc_level_action_when_onoff()           do{LevelOnOffAction = 1;} while(0)
#define vc_is_level_action_when_onoff()        (LevelOnOffAction == 1)

enum{
	VC_CONFIG_NODE_MODE = 0,
	VC_CONFIG_LED_STATE = 1,
	VC_CONFIG_KEY_STATE = 2,
	VC_CONFIG_MANUFACTURE_STATE = 3,
	VC_CONFIG_TIME_SCALE = 4,
};

extern void vcShadeInit(void);
extern void vcShadeAttributeInit(void);
extern uint32_t vcGetShadePositionFromLevel(uint8_t shadeIndex, uint8_t level);
extern uint8_t vcSetShadePositionToLevel(uint8_t shadeIndex, uint32_t position);
extern void vcShadeOperateByPosition(uint8_t shadeIndex, uint32_t position);
extern void vcShadeOperateByButton(uint8_t buttonIndex, uint8_t state);
extern void vcShadeOperateFromLevel(uint8_t endpoint);

extern void vcMfSetLEDStateCallback(void);
extern uint8_t vcMfGetManufactureStateCallback(void);
extern void vcMfSetManufactureStateCallback(uint8_t type);
extern void vcMfSetPowerMeterCalibrationCallback(void);
extern void vcMfSetKeyStateCallback(uint8_t state);
extern void vcShadeSetNodeConfig(uint8_t ep, uint16_t clusterId, uint16_t attributeId, uint8_t *data);
extern void vcShadeResetInit(void);
#endif /* VANCOUNT_VC_DEVICE_SWITCH_H_ */
