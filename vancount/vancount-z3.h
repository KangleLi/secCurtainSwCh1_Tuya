/* *
 *
 *
 *
 *
 *
 *
 *
 *
 */
#ifndef VANCOUNT_Z3_H
#define VANCOUNT_Z3_H
#include <stdint.h>

#define ACTIVE_ENDPOINT_NUMBER  1
#define VC_IDENTIFY_LED_INDEX      (1)
#define VC_NORMAL_SWITCH                ((uint8_t)0x00)
#define VC_DOUBLE_CONTROL_SWITCH        ((uint8_t)0x01)
#define VC_SCENES_SWITCH                ((uint8_t)0x02)

#define LED_HANDLE_NULL_END             ((uint8_t)0x00)
#define LED_HANDLE_RECOEVERY_END        ((uint8_t)0x01)
#define LED_HANDLE_CONTINUE             ((uint8_t)0x02)
#define LED_HANDLE_OFF_END              ((uint8_t)0x03)
#define vcSetLedHandleState(x)        (ledHandleState = (x))
#define vcGetLedHandleState()         (ledHandleState)

extern uint8_t ledHandleState;
extern uint8_t gVcNextState[];

extern void vc3In1Init(void);
extern void vancountSetRxOffWhenIdle(uint8_t cmd);

extern void halMultiLedBlinkPatternInit(uint8_t  length,
                             uint16_t *pattern,
                             uint8_t  led);

extern void halMultiLedBlinkPatternActivity(uint8_t  led,
                             uint8_t  pattern,
							 uint8_t count);
extern void vcLEDInit(void);
extern uint8_t vcButtonActivity(void);
extern EmberAfStatus vcReadMfgClusterServerAttribute( EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t readLength);
extern EmberAfStatus vcWriteMfgClusterServerAttribute( EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t dataType);
extern uint8_t vcGetNodeModeState(uint8_t ep);

extern EmberAfStatus vcReadMfgClusterServerAttributeEx(uint8_t ep, EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t readLength);
extern EmberAfStatus vcWriteMfgClusterServerAttributeEx(uint8_t ep, EmberAfClusterId cluster,
		EmberAfAttributeId attributeID,
		uint8_t* dataPtr,
		uint8_t dataType);
// extern function
//extern EmberStatus vcSetManufactoryName(char *name);
//extern EmberStatus vcGetManufactoryName(char *name);
//extern EmberStatus vcSetModelId(char *name);
//extern EmberStatus vcGetModelId(char *name);
//extern EmberStatus vcSetDimmableLight(char *data, uint8_t dataSize);
//extern EmberStatus vcGetDimmableLight(char *data, uint8_t *dataSize);
//extern EmberStatus vcSetLightSensor(char *data, uint8_t dataSize);
//extern EmberStatus vcGetLightSensor(char *data, uint8_t *dataSize);
//extern EmberStatus vcSetOccupancySensor(char *data, uint8_t dataSize);
//extern EmberStatus vcGetOccupancySensor(char *data, uint8_t *dataSize);
//extern EmberStatus vcSetAdcCalibration(uint32_t *cali);
//extern EmberStatus vcGetAdcCalibration(uint32_t *cali);
//extern EmberStatus vcSetIllumThreshold(uint16_t *illum);
//extern EmberStatus vcGetIllumThreashold(uint16_t *illum);
//extern EmberStatus vcSetPowerThreshold(uint8_t *power);
//extern EmberStatus vcGetPowerThreshold(uint8_t *power);
//extern EmberStatus vcSetPowerSource(uint8_t *source);
//extern EmberStatus vcGetPowerSource(uint8_t *source);
//extern EmberStatus vcSetModelType(uint8_t *modelType);
//extern EmberStatus vcGetModelType(uint8_t *modelType);
//extern EmberStatus vcSetTxPower(uint8_t *txPower);
//extern EmberStatus vcGetTxPower(uint8_t *txPower);
//extern EmberStatus vcSetPowerCycyle(uint8_t *counter);
//extern EmberStatus vcGetPowerCycle(uint8_t *counter);

#endif
