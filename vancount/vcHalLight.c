#include PLATFORM_HEADER
#include "app/framework/include/af.h"
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "serial/serial.h"

#include "em_cmu.h"
#include "em_gpio.h"

#include "vcHalLight.h"
#if 0
#define VC_HAL_595_SHCP_PORT       (gpioPortA)
#define VC_HAL_595_SHCP_PIN        (1U)

#define VC_HAL_595_STCP_PORT       (gpioPortA)
#define VC_HAL_595_STCP_PIN        (2U)

#define VC_HAL_595_SDA_PORT        (gpioPortA)
#define VC_HAL_595_SDA_PIN         (3U)

static void vcHal595Write(uint8_t da)
{
	uint8_t count;
	uint32_t delay;

	for (count = 0; count < 8; count++){
        if (da & 0x80) {
        	GPIO_PinOutSet(VC_HAL_595_SDA_PORT, VC_HAL_595_SDA_PIN);
        }else {
        	GPIO_PinOutClear(VC_HAL_595_SDA_PORT, VC_HAL_595_SDA_PIN);
        }
        GPIO_PinOutClear(VC_HAL_595_SHCP_PORT, VC_HAL_595_SHCP_PIN);
        delay = 1600;
        while(delay--);
        GPIO_PinOutSet(VC_HAL_595_SHCP_PORT, VC_HAL_595_SHCP_PIN);
        //delay = 48;
        //while(delay--);
        da <<= 1;
	}

    //GPIO_PinOutClear(VC_HAL_595_STCP_PORT, VC_HAL_595_STCP_PIN);
    //delay = 100;
    //while(delay--);
    GPIO_PinOutSet(VC_HAL_595_STCP_PORT, VC_HAL_595_STCP_PIN);
    delay = 1600;
    while(delay--);
    GPIO_PinOutClear(VC_HAL_595_STCP_PORT, VC_HAL_595_STCP_PIN);
}

static void vcHal595Init(void)
{
	GPIO_PinModeSet(VC_HAL_595_SHCP_PORT,
	VC_HAL_595_SHCP_PIN,
	gpioModePushPull,
	0);
	GPIO_PinModeSet(VC_HAL_595_STCP_PORT,
	VC_HAL_595_STCP_PIN,
	gpioModePushPull,
	0);
	GPIO_PinModeSet(VC_HAL_595_SDA_PORT,
	VC_HAL_595_SDA_PIN,
	gpioModePushPull,
	0);

	GPIO_PinOutClear(VC_HAL_595_SHCP_PORT, VC_HAL_595_SHCP_PIN);
	GPIO_PinOutClear(VC_HAL_595_STCP_PORT, VC_HAL_595_STCP_PIN);
	GPIO_PinOutClear(VC_HAL_595_SDA_PORT, VC_HAL_595_SDA_PIN);
}


void vcHalLightSet(uint8_t da)
{
	uint8_t data = 0;
	data |= da;
	vcHal595Write(data);
}

void vcHalLightClear(uint8_t da)
{
	uint8_t data = 0;
	vcHal595Write(data);
}

void vcHalToggleLight(uint8_t da)
{
	uint8_t data = 0;
   if (data | da){
	   vcHalLightClear(da);
   } else {
	   vcHalLightSet(da);
   }
}
#endif
// Because the switch on/off is controled by a pluse,
// clear the output after 10ms.
extern void vcHalLightEndHandleTime(void);
#if 0
void vcHalLightOnOffEnd(void)
{
	vcHal595Write(0);
}

void vcHalLightOn(uint8_t on, uint8_t off)
{
	uint8_t data = 0x0;
	data &= ~on;
	data |= off;
	vcHal595Write(data);
	vcHalLightEndHandleTime();
}

void vcHalLightOff(uint8_t on, uint8_t off)
{
	uint8_t data = 0;
	data |= on;
	data &= ~off;
	vcHal595Write(data);
	vcHalLightEndHandleTime();
}

void vcHalLightInit(void)
{
	uint8_t data = 0;
	vcHal595Init();
	vcHal595Write(data);
}
#endif
//---------------------------------------------------------------
typedef struct {
	  GPIO_Port_TypeDef   port;
	  unsigned int        pin;
} VcUcLoadArray_t;
static VcUcLoadArray_t vcUcLoadArray[HAL_UC_LOAD_COUNT] = BSP_UC_LOAD_INIT;


void vcHalLightOnOffEnd(void)
{
	uint8_t i;
	for (i = 0; i < HAL_UC_LOAD_COUNT; i++) {
		GPIO_PinOutClear(vcUcLoadArray[i].port, vcUcLoadArray[i].pin);
	}
}

void vcHalLightOn(uint8_t on, uint8_t off)
{
	GPIO_PinOutSet(vcUcLoadArray[on].port, vcUcLoadArray[on].pin);
	GPIO_PinOutClear(vcUcLoadArray[off].port, vcUcLoadArray[off].pin);
	vcHalLightEndHandleTime();
}

void vcHalLightOff(uint8_t on, uint8_t off)
{
	GPIO_PinOutClear(vcUcLoadArray[on].port, vcUcLoadArray[on].pin);
	GPIO_PinOutSet(vcUcLoadArray[off].port, vcUcLoadArray[off].pin);
	vcHalLightEndHandleTime();
}

void vcHalLightInit(void)
{
	uint8_t i = 0;
	uint8_t enableUcLoads[HAL_UC_LOAD_COUNT] = HAL_UC_LOAD_ENABLE;
	for (i = 0; i < HAL_UC_LOAD_COUNT; i++) {
		GPIO_PinModeSet(vcUcLoadArray[enableUcLoads[i]].port,
				vcUcLoadArray[enableUcLoads[i]].pin, gpioModePushPull, 0);
	}
}
//---------------------------------------------------------------
#include "list.h"
LIST(vcLightList);
typedef struct vcLightItem{
	struct vcLightItem *next;
	uint8_t on;
	uint8_t off;
	uint8_t led;
    uint8_t cmd;
}VcLightItem_t;
EmberEventControl customLightEventControl;
static VcLightItem_t     vcLightItem[VC_LIGHT_ITME_NUM] = {
		{NULL, BSP_UC_LOAD0, BSP_UC_LOAD1, 0, 0},
		{NULL, BSP_UC_LOAD2, BSP_UC_LOAD3, 1, 0}};


extern void vcLEDOff(uint8_t led);
extern void vcLEDOn(uint8_t led);
extern uint8_t vcGetSwLedState(void);
void vcLightIndicatorControl(uint8_t led, uint8_t cmd)
{
	switch(vcGetSwLedState()) {
	case VC_LED_INDICATOR_LOC:
    	if (cmd == 1) {
    		vcLEDOff(led);
    	} else{
    		vcLEDOn(led);
    	}
		break;
	case VC_LED_INDICATOR_STATE:
    	if (cmd == 1) {
    		vcLEDOn(led);
    	} else{
    		vcLEDOff(led);
    	}
		break;
	case VC_LED_NO_INDICATOR:
		vcLEDOff(led);
		break;
	}
}

void customLightEventHandler(void)
{
	VcLightItem_t *item;
	if (list_head(vcLightList) == NULL){
		emberEventControlSetInactive(customLightEventControl);
		return;
	}
	//vcTestPrintf(11);
    item = list_pop(vcLightList);
	//vcTestPrintf(item->cmd);
	if (item->cmd == 1) {
		vcHalLightOn(item->on, item->off);
	} else {
		vcHalLightOff(item->on, item->off);
	}
	vcLightIndicatorControl(item->led, item->cmd);
	emberEventControlSetDelayMS(customLightEventControl, 200);
}

void vcLightInit(void)
{
	vcHalLightInit();
	list_init(vcLightList);
}

void vcLightControl(uint8_t light, uint8_t cmd)
{
	vcLightItem[light].cmd = cmd;
	list_add(vcLightList, vcLightItem + light);
	if (emberEventControlGetActive(customLightEventControl) == 0) {
		emberEventControlSetDelayMS(customLightEventControl, 200);
	}
}

// Array of endpoints that are supported
//#define FIXED_ENDPOINT_ARRAY { 51, 52, 53, 54, 242 }
extern void vcTestPrintf(uint8_t step);
void vcSwControl(uint8_t ep, uint8_t cmd)
{
	uint8_t light;

	light = emberAfIndexFromEndpoint(ep);
	//vcTestPrintf(ep);
	vcLightItem[light].cmd = cmd;
	list_add(vcLightList, vcLightItem + light);
	emberAfCorePrintln("%p, %d", __func__, __LINE__);
	if (emberEventControlGetActive(customLightEventControl) == 0) {
		emberEventControlSetDelayMS(customLightEventControl, 200);
	}
}

