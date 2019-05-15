/*
 * vancount-mf-test.h
 *
 *  Created on: May 16, 2018
 *      Author: tao
 */

#ifndef VANCOUNT_VANCOUNT_MF_TEST_H_
#define VANCOUNT_VANCOUNT_MF_TEST_H_

#define VC_SWITCH_1_KEY ((uint16_t)0x0101)
#define VC_SWITCH_2_KEY ((uint16_t)0x0102)
#define VC_SWITCH_3_KEY ((uint16_t)0x0103)
#define VC_SWITCH_4_KEY ((uint16_t)0x0104)

#define VC_OUTLET_3 ((uint16_t)0x0203)
#define VC_OUTLET_5 ((uint16_t)0x0205)

#define VC_DEVICE_T_TYPE         EMBER_AF_PLUGIN_OTA_CLIENT_POLICY_IMAGE_TYPE_ID


extern void vcMfInit(void);
extern void vcMfSend(void);
extern void vcMfRecv(void);

#endif /* VANCOUNT_VANCOUNT_MF_TEST_H_ */
