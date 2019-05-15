/*
 * vc-onoff.h
 *
 *  Created on: Jun 13, 2018
 *      Author: tao
 */

#ifndef VANCOUNT_VC_ONOFF_H_
#define VANCOUNT_VC_ONOFF_H_

extern void vcClientControl(uint8_t ep, uint8_t cmd);
extern uint8_t  vcGetBindingInfoByEp(uint8_t ep);
#endif /* VANCOUNT_VC_ONOFF_H_ */
