/*
 * vancount-hal-serial.c
 *
 *  Created on: May 16, 2018
 *      Author: tao
 */
#if 0
#include "vancount-hal-serial.h"

Ecode_t vcHalSerialInit(void)
{
	Ecode_t status = ECODE_OK;
	COM_Init_t initDataUsart1 = (COM_Init_t ) COM_USART1_DEFAULT;
	status = COM_Init(comPortUsart1, &initDataUsart1);
	if (status != ECODE_OK) {
		return status;
	}
	return 0;
}

extern uint16_t COM_ReadAvailable(COM_Port_t port);
uint16_t vcHalSerialReadAvalible(void)
{
	return COM_ReadAvailable(comPortUsart1);
}

extern Ecode_t COM_WriteData(COM_Port_t port, uint8_t *data, uint8_t length);
uint16_t vcHalSerialWrite(uint8_t *data, uint8_t dataSize)
{
	return COM_WriteData(comPortUsart1, data, dataSize);
}

extern Ecode_t COM_ReadDataTimeout(COM_Port_t port,
                            uint8_t *data,
                            uint16_t length,
                            uint16_t *bytesRead,
                            uint16_t firstByteTimeout,
                            uint16_t subsequentByteTimeout);

uint16_t vcHalSerialRead(uint8_t *data, uint8_t *dataSize)
{
	uint16_t len;
	uint8_t status;
	status = COM_ReadDataTimeout(comPortUsart1, data, *dataSize, &len, 1, 2);
	*dataSize = len;
	return status;
}
#endif
