/*
 * vancount-hal-serial.h
 *
 *  Created on: May 16, 2018
 *      Author: tao
 */

#ifndef VANCOUNT_VANCOUNT_HAL_SERIAL_H_
#define VANCOUNT_VANCOUNT_HAL_SERIAL_H_
#include PLATFORM_HEADER
#include "hal/hal.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "ecode.h"
#include "plugin/serial/com.h"
#include "plugin/serial/com_config.h"

extern Ecode_t vcHalSerialInit(void);
extern uint16_t vcHalSerialReadAvalible(void);
extern uint16_t vcHalSerialWrite(uint8_t *data, uint8_t dataSize);
extern uint16_t vcHalSerialRead(uint8_t *data, uint8_t *dataSize);

#endif /* VANCOUNT_VANCOUNT_HAL_SERIAL_H_ */
