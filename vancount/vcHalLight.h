/*
 * vcHalLight.h
 *
 *  Created on: Apr 9, 2018
 *      Author: tao
 */
#ifndef VC_HAL_LIGHT
#define VC_HAL_LIGHT

#ifdef __cplusplus
extern "C" {
#endif

#define BSP_UC_LOAD_PRESENT                              (1)

#define BSP_UC_LOAD0                                     0
#define BSP_UC_LOAD0_PIN                                 (11U)
#define BSP_UC_LOAD0_PORT                                (gpioPortC)

#define BSP_UC_LOAD1                                     1
#define BSP_UC_LOAD1_PIN                                 (10U)
#define BSP_UC_LOAD1_PORT                                (gpioPortC)

#define BSP_UC_LOAD2                                     2
#define BSP_UC_LOAD2_PIN                                 (9U)
#define BSP_UC_LOAD2_PORT                                (gpioPortC)

#define BSP_UC_LOAD3                                     3
#define BSP_UC_LOAD3_PIN                                 (8U)
#define BSP_UC_LOAD3_PORT                                (gpioPortC)

#define HAL_UC_LOAD_ENABLE                               { 0, 1, 2, 3 }
#define HAL_UC_LOAD_COUNT                                (4U)
#define BSP_UC_LOAD_COUNT                                (4U)
#define BSP_UC_LOAD_INIT                                 { { BSP_UC_LOAD0_PORT, BSP_UC_LOAD0_PIN }, { BSP_UC_LOAD1_PORT, BSP_UC_LOAD1_PIN }, \
{ BSP_UC_LOAD2_PORT, BSP_UC_LOAD2_PIN }, { BSP_UC_LOAD3_PORT, BSP_UC_LOAD3_PIN } }

#define VC_HAL_LIGHT_1    ((uint8_t)0x01)
#define VC_HAL_LIGHT_2    ((uint8_t)0x02)
#define VC_HAL_LIGHT_3    ((uint8_t)0x04)
#define VC_HAL_LIGHT_4    ((uint8_t)0x08)
#define VC_HAL_LIGHT_5    ((uint8_t)0x10)
#define VC_HAL_LIGHT_6    ((uint8_t)0x20)
#define VC_HAL_LIGHT_7    ((uint8_t)0x40)
#define VC_HAL_LIGHT_8    ((uint8_t)0x80)

#define VC_LIGHT_ITME_NUM ((uint8_t)2)
#define VC_LIGHT_0    ((uint8_t)0)
#define VC_LIGHT_1    ((uint8_t)1)
#define VC_LIGHT_2    ((uint8_t)2)
#define VC_LIGHT_3    ((uint8_t)3)

#define VC_LED_INDICATOR_LOC    ((uint8_t)1)
#define VC_LED_INDICATOR_STATE  ((uint8_t)0)
#define VC_LED_NO_INDICATOR     ((uint8_t)2)

extern void vcHalLightClear(uint8_t light);
extern void vcHalLightSet(uint8_t light);
extern void vcHalToggleLight(uint8_t da);
extern void vcHalLightInit(void);
extern void vcLightControl(uint8_t light, uint8_t cmd);
#define vcRelayControl(index, cmd)   vcLightControl((index), (cmd))

extern void vcLightInit(void);
extern void vcSwControl(uint8_t ep, uint8_t cmd);
extern void vcHalLightOnOffEnd(void);
#ifdef __cplusplus
}
#endif
#endif
