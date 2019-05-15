/*
 * vancount-customer-extend.h
 *
 *  Created on: Dec 3, 2018
 *      Author: tao
 */
#include "vancount-customer-extend-types.h"
#ifndef VANCOUNT_VANCOUNT_CUSTOMER_EXTEND_H_
#define VANCOUNT_VANCOUNT_CUSTOMER_EXTEND_H_

#define VC_VCE_FRAME_CTL_GATEWAY_TO_DEVICE   0x01
#define VC_VCE_FRAME_CTL_DEVICE_TO_GATEWAY   0x02

#define VC_VCE_FRAME_CMD_READ                0x01
#define VC_VCE_FRAME_CMD_WRITE               0x02
#define VC_VCE_FRAME_CMD_CTL                 0x03

#define VC_VCE_FRAME_SUB_ERROR               0x0000
#define VC_VCE_FRAME_SUB_TIME_EVENT          0x0101


//=============================================================
#define VC_VCE_ERROR_CODE_SUCCESS            0x00
#define VC_VCE_ERROR_CODE_SUPPORT            0x01
#define VC_VCE_ERROR_CODE_NO_EXISTING        0x02
#define VC_VCE_ERROR_CODE_TIME_EVENT_FULL    0x03
//=============================================================
#define VC_VCE_FRAME_CTL_POS                 0
#define VC_VCE_FRAME_CMD_POS                 1
#define VC_VCE_FRAME_SUB_POS                 2
#define VC_VCE_FRAME_PAYLOAD_POS             4
//=============================================================
// time event
#define VC_VCE_TIME_EVENT_MAX_SIZE           2
extern VceTimerEvent_t vcVceTimerEventTable[];
//=============================================================
extern void vcZ3CustomerExtend(uint8_t *data);
extern VceTimerEvent_t *vcVceGetTimerEventByID(uint8_t id);
extern VceTimerEvent_t *vcVceGetInvalidTimerEvent(void);

#endif /* VANCOUNT_VANCOUNT_CUSTOMER_EXTEND_H_ */
