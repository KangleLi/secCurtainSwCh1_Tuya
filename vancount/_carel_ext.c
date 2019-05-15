/*
 * _carel_ext.c
 *
 *  Created on: 2019ƒÍ4‘¬10»’
 *      Author: Carel
 */

#include "app/framework/include/af.h"
#include "./vancount-z3-net.h"
#include "./vancount-z3.h"
#include "./list.h"
#include "./vc-onoff.h"
#include "./vc-scenes.h"
#include "./vc-device-shade.h"
#include "vancount-customer-extend.h"

#define DIM_CH2_ENABLE 			0
enum {
	OEM_NO_SIMON = 0,		// simon - 0
	OEM_NO_TUYA,			// tuya  - 1
};
#if DIM_CH2_ENABLE == 0
#define _MODELID_SIMON			"S2100-E818-2001"
#define _MODELID_TUYA			"SM0301"
#define _MANUNAME_SIMON			"SEC"
//#define _MANUNAME_TUYA			"_TYZB01_k4yr34vv"
#define _MANUNAME_TUYA			"_TZ2000_k4yr34vv"
#else
#define _MODELID_SIMON			"S2100-E818-2002"
#define _MODELID_TUYA			"SM0302"
#define _MANUNAME_SIMON			"SEC"
//#define _MANUNAME_TUYA			"_TYZB01_fq1cwsbu"
#define _MANUNAME_TUYA			"_TZ2000_fq1cwsbu"
#endif
#define _AUTHCODE_TUYA_ATTR1	{0x28,\
								0xdb,0xfb,0xa6,0x31,0xd1,0x8c,0x30,0xa4,0x54,0x26,\
								0x9a,0x22,0xfd,0x42,0x86,0x3f,0xe8,0xce,0x4f,0x26,\
								0xe1,0x6d,0x96,0x7f,0x47,0xf9,0x1d,0x2f,0x39,0x17,\
								0x38,0x0d,0x4a,0x14,0xd4,0xf2,0x8b,0xfc,0xbd,0x74}

extern void vancountClearNetworkTables(void);
void emberAfPluginBasicResetToFactoryDefaultsCallback(uint8_t endpoint){
	vancountClearNetworkTables();
	_OEM_Handler();
	extern uint8_t oem_select;
	emberAfWriteServerAttribute(1, \
							 0x0000, \
							 0xFC00, \
							 &oem_select,
							 0x20);
	//setZllState(0, (EMBER_ZLL_STATE_FACTORY_NEW | EMBER_ZLL_STATE_PROFILE_INTEROP));
	emberLeaveNetwork();
}


boolean emberAfBasicClusterTuya_ext_resetCallback(void){
	vancountClearNetworkTables();
	_OEM_Handler();
	extern uint8_t oem_select;
	emberAfWriteServerAttribute(1, \
							 0x0000, \
							 0xFC00, \
							 &oem_select,
							 0x20);
	//setZllState(0, (EMBER_ZLL_STATE_FACTORY_NEW | EMBER_ZLL_STATE_PROFILE_INTEROP));
	// carel
	vcReportAnAttribute(0x00, 1, 0x01, 0x0000, 0xFFFE, 1);
	// 20190411
	return true;
}


uint8_t tuya_auth_attr2[41];
uint8_t oem_select;
// ≈–∂œOEM
void _OEM_Handler(void){
	uint8_t tuya_auth_attr1[] = _AUTHCODE_TUYA_ATTR1;
	uint8_t *datPtr;

	switch(oem_select){
	case OEM_NO_SIMON:
		// –¥modelId
		datPtr = malloc(strlen(_MODELID_SIMON)+1);
		datPtr[0] = strlen(_MODELID_SIMON);
		memcpy(datPtr+1, _MODELID_SIMON, strlen(_MODELID_SIMON));
		emberAfWriteServerAttribute(1, \
								 0x0000, \
								 0x0005, \
								 datPtr,
								 ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
		free(datPtr);

		datPtr = malloc(strlen(_MANUNAME_SIMON)+1);
		datPtr[0] = strlen(_MANUNAME_SIMON);
		memcpy(datPtr+1, _MANUNAME_SIMON, strlen(_MANUNAME_SIMON));
		// –¥manufacture name
		emberAfWriteServerAttribute(1, \
								 0x0000, \
								 0x0004, \
								 datPtr,
								 ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
		free(datPtr);
		break;
	case OEM_NO_TUYA:
		// –¥modelId
		datPtr = malloc(strlen(_MODELID_TUYA)+1);
		datPtr[0] = strlen(_MODELID_TUYA);
		memcpy(datPtr+1, _MODELID_TUYA, strlen(_MODELID_TUYA));
		emberAfWriteServerAttribute(1, \
								 0x0000, \
								 0x0005, \
								 datPtr,
								 ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
		free(datPtr);

		// –¥manufacture name
		datPtr = malloc(strlen(_MANUNAME_TUYA)+1);
		datPtr[0] = strlen(_MANUNAME_TUYA);
		memcpy(datPtr+1, _MANUNAME_TUYA, strlen(_MANUNAME_TUYA));
		emberAfWriteServerAttribute(1, \
								 0x0000, \
								 0x0004, \
								 datPtr,
								 ZCL_CHAR_STRING_ATTRIBUTE_TYPE);
		free(datPtr);

		// –¥ ⁄»® Ù–‘1
		emberAfWriteServerAttribute(1, \
								 0x0000, \
								 0xffe0, \
								 tuya_auth_attr1,
								 ZCL_ARRAY_ATTRIBUTE_TYPE);
		// –¥ ⁄»® Ù–‘2
		if(tuya_auth_attr2[0] == 0x28){
			emberAfWriteServerAttribute(1, \
								 0x0000, \
								 0xffe1, \
								 tuya_auth_attr2,
								 ZCL_ARRAY_ATTRIBUTE_TYPE);
		}

		break;
	default:
		break;
	}

}

void OEM_Start(void){
	emberAfReadServerAttribute(1,
							0x0000,
							0xFC00,
							&oem_select,
							1);

	emberAfReadServerAttribute(1, \
							 0x0000, \
							 0xFFE1, \
							 tuya_auth_attr2,
							 41);
}

// Basic Attr Changed Calback
void emberAfBasicClusterServerAttributeChangedCallback(int8u endpoint,
                                                       EmberAfAttributeId attributeId){
	uint8_t buf_auth[41];
	if(attributeId == 0xFC00){
		OEM_Start();
		_OEM_Handler();
	}
	else if(attributeId == 0xFC01){
		if(emberAfReadServerAttribute(1, \
								 0x0000, \
								 0xFC01, \
								 buf_auth,
								 41) == 0x00){
			emberAfWriteServerAttribute(1, \
								 0x0000, \
								 0xffe1, \
								 buf_auth,
								 ZCL_ARRAY_ATTRIBUTE_TYPE);
			OEM_Start();
		}
	}
}
