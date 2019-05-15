/*
 * vancount-customer-extend-types.h
 *
 *  Created on: Dec 9, 2018
 *      Author: tao
 */

#ifndef VANCOUNT_VANCOUNT_CUSTOMER_EXTEND_TYPES_H_
#define VANCOUNT_VANCOUNT_CUSTOMER_EXTEND_TYPES_H_

typedef struct VceTimerEvent {
	struct VceTimerEvent *next;
	uint8_t id;
	uint8_t  period;
	uint16_t cluster;
	uint32_t intervalTime;
	uint32_t remainTime;
	uint16_t attribute;
	union {
		uint8_t onOff;
		uint8_t level;
	} op;
	uint8_t ep;
	uint8_t isInvalid;
	void (*callbak)(struct VceTimerEvent *e);
}VceTimerEvent_t;

#endif /* VANCOUNT_VANCOUNT_CUSTOMER_EXTEND_TYPES_H_ */
