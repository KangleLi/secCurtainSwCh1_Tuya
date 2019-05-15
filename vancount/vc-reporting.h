/*
 * vc-reporting.h
 *
 *  Created on: Jun 12, 2018
 *      Author: tao
 */

#ifndef VANCOUNT_VC_REPORTING_H_
#define VANCOUNT_VC_REPORTING_H_
extern void vcReportingIntervalStart(void);
extern void vcResetReportingInterval(void);
extern void vcReportAnAttribute(uint16_t destination, uint8_t srcEp, uint8_t dstEp, EmberAfClusterId clusterId, EmberAfAttributeId attributeId, uint8_t mask);
extern void vcReportAnAttributeEx(uint8_t srcEp, EmberAfClusterId clusterId, EmberAfAttributeId attributeId, uint8_t isMfg);
#endif /* VANCOUNT_VC_REPORTING_H_ */
