// *****************************************************************************
// * button-joining.c
// *
// * Routines for forming/joining using the hardware buttons.
// *
// *   button 0: if not joined: FORM if the device is capable of forming
// *             (a.k.a. a coordinator).  Otherwise form a network.
// *             if joined: broadcast ZDO MGMT Permit Join in network.
// *             Hold for 5 seconds and release: leave network
// *   button 1: Unused (Callback executed)
// *
// * Copyright 2011 by Ember Corporation. All rights reserved.              *80*
// *****************************************************************************

#include "app/framework/include/af.h"
#include "app/framework/util/af-main.h"
#include "app/framework/plugin/ezmode-commissioning/ez-mode.h"
#include "vancount/vancount-z3.h"

//------------------------------------------------------------------------------
// Forward Declaration

EmberEventControl emberAfPluginButtonJoiningButton0EventControl;
EmberEventControl emberAfPluginButtonJoiningButton1EventControl;
EmberEventControl customButtonTimeoutEventControl;
#define BUTTON0_IRQ   ((uint8_t)0x01)
#define BUTTON1_IRQ   ((uint8_t)0x02)
#define BUTTON2_IRQ   ((uint8_t)0x04)
#define BUTTON3_IRQ   ((uint8_t)0x08)

#define BUTTON_FOR_JOIN_NET  (2)
static bool buttonPress(uint8_t button, uint8_t state);
static uint32_t  buttonPressDurationMs[4] = {0};
static uint8_t   buttonPressFlag = 0;
static uint8_t   buttonReleaseFlag = 0;
static uint8_t   buttonPressCount = 0;
static uint8_t keyAbortCount = 0;

#define vcSetKeyState(k)  			vcKeyState |= (1 << (k))
#define vcResetKeyState(k)  		vcKeyState &= ~(1 << (k))
#define vcGetKeyState(k)  			(vcKeyState & (1 << (k)))
static uint8_t vcKeyState;

#define BUTTON_HOLD_DURATION_MS 5000
#define BUTTON_JOIN_MS          (uint32_t)3000
#define BUTTON_LEAVE_MS         (uint32_t)10000
#define BUTTON_DISABLE_MS       (uint32_t)60000
#define BUTTON_SHORT_PRESSED_MS (uint32_t)500
//------------------------------------------------------------------------------
// Globals

#define buttonEvent0 emberAfPluginButtonJoiningButton0EventControl
#define buttonEvent1 emberAfPluginButtonJoiningButton1EventControl


#define PERMIT_JOIN_TIMEOUT EMBER_AF_PLUGIN_BUTTON_JOINING_PERMIT_JOIN_TIMEOUT

//------------------------------------------------------------------------------

void emberAfPluginButtonJoiningButton0EventHandler(void)
{
	EmberAfStatus status;
	uint8_t lock;

	emberEventControlSetInactive(buttonEvent0);
	if (buttonPressDurationMs[BUTTON_FOR_JOIN_NET] < BUTTON_JOIN_MS) {
		return;
	}

	if (buttonPressDurationMs[BUTTON_FOR_JOIN_NET] >= BUTTON_JOIN_MS
			&& buttonPressDurationMs[BUTTON_FOR_JOIN_NET] < BUTTON_LEAVE_MS) {
		if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
#ifdef EMBER_AF_PLUGIN_NETWORK_CREATOR_SECURITY
			// The Network Creator Security plugin Open process is more comprehensive
			// and also takes care of broadcasting pjoin
			//vcLEDBlink(0, 1, 8);
			//emberAfPluginNetworkCreatorSecurityOpenNetwork();
#else
			//emberAfBroadcastPermitJoin(PERMIT_JOIN_TIMEOUT);
#endif // EMBER_AF_PLUGIN_NETWORK_CREATOR_SECURITY
		} else if (emberAfNetworkState() == EMBER_NO_NETWORK) {
#ifdef EMBER_AF_HAS_COORDINATOR_NETWORK
			emberAfCorePrintln("%p: nwk down: do form", "button0");
			// Use Z3.0 network formation if we have the plugin for it; else, fall back
			// to legacy / custom formation methods
#ifdef EMBER_AF_PLUGIN_NETWORK_CREATOR
			emberAfPluginNetworkCreatorStart(true);
#else // EMBER_AF_PLUGIN_NETWORK_CREATOR
			emberAfFindUnusedPanIdAndForm();
#endif // EMBER_AF_PLUGIN_NETWORK_CREATOR
#else // EMBER_AF_HAS_COORDINATOR_NETWORK
			// Use Z3.0 network steering if we have the plugin for it; else, fall back
			// to legacy / custom joining method
			emberAfCorePrintln("%p: nwk down: do join", "button0");
#ifdef EMBER_AF_PLUGIN_NETWORK_STEERING
			//vcSetLedHandleState(LED_HANDLE_CONTINUE);
			//vcLEDBlink(0, 2, 480);
			//emberAfPluginNetworkSteeringStart();
			vcZ3CommissionActive(0);
#else // EMBER_AF_PLUGIN_NETWORK_STEERING
			emberAfStartSearchForJoinableNetwork();
#endif // EMBER_AF_PLUGIN_NETWORK_STEERING
#endif // EMBER_AF_HAS_COORDINATOR_NETWORK
		} else {
			// MISRA requires ..else if.. to have terminating else.
		}
		return;
	}

	if (buttonPressDurationMs[BUTTON_FOR_JOIN_NET] >= BUTTON_LEAVE_MS
			&& buttonPressDurationMs[BUTTON_FOR_JOIN_NET] < BUTTON_DISABLE_MS) {
		emberAfCorePrintln("Leaving network due to button press.");
		emberLeaveNetwork();
		// carel_add ( 10s离网后自动配网 )
		vcZ3CommissionActive(5000);
		// 20190411
		return;
	}
}
extern void vcAfPluginButtonJoiningButtonEventCallback(uint8_t buttonNumber,
                                                   uint8_t buttonState);
void emberAfPluginButtonJoiningButton1EventHandler(void)
{
	//TODO: 根据KidsLock的Attribute属性值，判断是否需要执行相关的按键操作。
	//EmberAfStatus status;
	//uint8_t lock;
	uint8_t state;
	emberEventControlSetInactive(buttonEvent1);

	if (buttonPressFlag & BUTTON0_IRQ) {
		buttonPressDurationMs[0] = elapsedTimeInt32u(
								buttonPressDurationMs[0],
								halCommonGetInt32uMillisecondTick());
		if (buttonPressDurationMs[0] < BUTTON_SHORT_PRESSED_MS){
			buttonPressFlag &= ~BUTTON0_IRQ;
			buttonReleaseFlag &= ~BUTTON0_IRQ;
			state = 0;
		}
		if (buttonPressDurationMs[0] >= BUTTON_SHORT_PRESSED_MS){
			if (buttonReleaseFlag & BUTTON0_IRQ) {
				state = 2;
				buttonPressFlag &= ~BUTTON0_IRQ;
				buttonReleaseFlag &= ~BUTTON0_IRQ;
			} else {
				state = 1;
			}
		}
		vcAfPluginButtonJoiningButtonEventCallback(0, // button 0 is pressed
				state);
	} else {
		buttonReleaseFlag &= ~BUTTON0_IRQ;
	}

	if (buttonPressFlag & BUTTON1_IRQ) {
		buttonPressDurationMs[1] = elapsedTimeInt32u(
								buttonPressDurationMs[1],
								halCommonGetInt32uMillisecondTick());
		if (buttonPressDurationMs[1] < BUTTON_SHORT_PRESSED_MS){
			buttonPressFlag &= ~BUTTON1_IRQ;
			buttonReleaseFlag &= ~BUTTON1_IRQ;
			state = 0;
		}
		if (buttonPressDurationMs[1] >= BUTTON_SHORT_PRESSED_MS){
			if (buttonReleaseFlag & BUTTON1_IRQ) {
				state = 2;
				buttonPressFlag &= ~BUTTON1_IRQ;
				buttonReleaseFlag &= ~BUTTON1_IRQ;
			} else {
				state = 1;
			}
		}
		vcAfPluginButtonJoiningButtonEventCallback(1, // button 1 is pressed
				state);
	} else {
		buttonReleaseFlag &= ~BUTTON1_IRQ;
	}

	if (buttonPressFlag & BUTTON2_IRQ) {
		emberAfPluginButtonJoiningButtonEventCallback(2, // button 2 is pressed
				buttonPressDurationMs[2]);
		buttonPressFlag &= ~BUTTON2_IRQ;
	}

	if (buttonPressFlag & BUTTON3_IRQ) {
		emberAfPluginButtonJoiningButtonEventCallback(3, // button 3 is pressed
				buttonPressDurationMs[3]);
		buttonPressFlag &= ~BUTTON3_IRQ;
	}
}
#if HAL_BUTTON_COUNT == 1
static const vcButtonPin[HAL_BUTTON_COUNT] = {BUTTON0};
#elif HAL_BUTTON_COUNT == 2
static const vcButtonPin[HAL_BUTTON_COUNT] = {BUTTON0, BUTTON1};
#elif HAL_BUTTON_COUNT == 3
static const vcButtonPin[HAL_BUTTON_COUNT] = {BUTTON0, BUTTON1, BUTTON2};
#elif HAL_BUTTON_COUNT == 4
static const vcButtonPin[HAL_BUTTON_COUNT] = {BUTTON0, BUTTON1, BUTTON2, BUTTON3};
#endif
void customButtonTimeoutEventHandler(void)
{
	EmberAfStatus status;

	uint8_t i;
	uint8_t state = vcKeyState;

	buttonPressCount = 0;
	for (i = 0; i < HAL_BUTTON_COUNT && state != 0; i++) {
		if ((state & 0x01) &&
			(BUTTON_RELEASED == halButtonPinState(vcButtonPin[i]))) {
			emberAfCorePrintln("%p, %d", __func__, __LINE__);
			vcResetKeyState(i);
		}
		state >>= 1;
	}

	keyAbortCount++;
	if ((keyAbortCount == 3 || keyAbortCount == 10) &&
			(buttonPressFlag & BUTTON2_IRQ)) {
		buttonPressDurationMs[BUTTON_FOR_JOIN_NET] = elapsedTimeInt32u(buttonPressDurationMs[BUTTON_FOR_JOIN_NET],
				halCommonGetInt32uMillisecondTick());
		if (keyAbortCount == 10) {
			buttonPressDurationMs[BUTTON_FOR_JOIN_NET] += 5100;
		}
		emberEventControlSetDelayMS(buttonEvent0, 10);
	}
	if ((keyAbortCount == 4) &&
		(buttonPressFlag & BUTTON2_IRQ)) {
		buttonPressDurationMs[BUTTON_FOR_JOIN_NET] = halCommonGetInt32uMillisecondTick();
	}

	if (vcKeyState == 0){
		emberAfCorePrintln("%p, %d", __func__, __LINE__);
		emberEventControlSetInactive(customButtonTimeoutEventControl);
		keyAbortCount = 0;
		return;
	}

	if (keyAbortCount <= 59){
		emberEventControlSetDelayMS(customButtonTimeoutEventControl, 1000);
		return;
	}
	emberAfCorePrintln("%p, %d", __func__, __LINE__);
#if 0
	//emberEventControlSetDelayMS(customButtonTimeoutEventControl, 2000);
	emberEventControlSetInactive(customButtonTimeoutEventControl);
	status = vcWriteMfgClusterServerAttribute(ZCL_VC_MFG_EXTENSION_CLUSTER_ID,
			ZCL_ATTRIBUTE_MFG_KEY_STATE_ATTRIBUTE_ID, (uint8_t *) &vcKeyState,
			ZCL_INT8U_ATTRIBUTE_TYPE);
#endif
	keyAbortCount = 0;
	return;
}

void emberAfPluginButtonJoiningPressButton(uint8_t button)
{
  // We don't bother to check the button press both times
  // since the only reason it fails is invalid button.
  // No point in doing that twice.
  bool result = buttonPress(button, BUTTON_PRESSED);
  buttonPress(button, BUTTON_RELEASED);

  if (!result) {
    emberAfCorePrintln("Invalid button %d", button);
  }
}

// ISR context functions!!!

// WARNING: these functions are in ISR so we must do minimal
// processing and not make any blocking calls (like printf)
// or calls that take a long time.

static bool buttonPress(uint8_t button, uint8_t state)
{
  // ISR CONTEXT!!!
  static uint32_t timeMs;
  EmberEventControl* event;

  if (button == BUTTON0) {
    event = &buttonEvent0;
  } else if (button == BUTTON1) {
    event = &buttonEvent1;
  } else {
    return false;
  }
  if (state == BUTTON_PRESSED) {
    buttonPressDurationMs[0] = 0;
    timeMs = halCommonGetInt32uMillisecondTick();
  } else {
    buttonPressDurationMs[0] = elapsedTimeInt32u(timeMs, halCommonGetInt32uMillisecondTick());
    emberEventControlSetActive(*event);
  }

  return true;
}
//#define BUTTON_DELAY

extern uint8_t vcButtonActivity(void);

void emberAfHalButtonIsrCallback(uint8_t button, uint8_t state)
{
	// ISR CONTEXT!!!
	//buttonPress(button, state);
	//emberAfCorePrintln("bt: %d", button);
	if (!vcButtonActivity()){
		return;
	}

	if (state == BUTTON_PRESSED) {
		if (buttonPressCount == 0) {
			emberEventControlSetDelayMS(customButtonTimeoutEventControl, 1000);
		}
		buttonPressCount++;
	}
	switch (button) {
	case BUTTON0:
		if (state == BUTTON_PRESSED) {
			buttonPressDurationMs[0] = halCommonGetInt32uMillisecondTick();
			buttonPressFlag |= BUTTON0_IRQ;
			vcSetKeyState(0);
			emberEventControlSetDelayMS(buttonEvent1, 500);
		} else {
			buttonReleaseFlag |= BUTTON0_IRQ;
			emberEventControlSetActive(buttonEvent1);
		}
		break;
	case BUTTON1:
		if (state == BUTTON_PRESSED) {
			buttonPressDurationMs[1] = halCommonGetInt32uMillisecondTick();
			buttonPressFlag |= BUTTON1_IRQ;
			vcSetKeyState(1);
			emberEventControlSetDelayMS(buttonEvent1, 500);
		} else {
			buttonReleaseFlag |= BUTTON1_IRQ;
			emberEventControlSetActive(buttonEvent1);
		}
		break;
	case BUTTON2:
		if (state == BUTTON_PRESSED) {
			buttonPressDurationMs[BUTTON_FOR_JOIN_NET] = halCommonGetInt32uMillisecondTick();
			buttonPressFlag |= BUTTON2_IRQ;
			vcSetKeyState(2);
//			emberEventControlSetActive(buttonEvent1);
		} else {
			buttonPressFlag &= ~BUTTON2_IRQ;
//			buttonReleaseFlag |= BUTTON2_IRQ;
//				buttonPressDurationMs[BUTTON_FOR_JOIN_NET] = elapsedTimeInt32u(
//						buttonPressDurationMs[BUTTON_FOR_JOIN_NET],
//						halCommonGetInt32uMillisecondTick());
//				emberEventControlSetActive(buttonEvent0);
		}
		break;
#if HAL_BUTTON_COUNT >= 4
	case BUTTON3:
		if (state == BUTTON_PRESSED) {
			buttonPressFlag |= BUTTON3_IRQ;
			vcSetKeyState(3);
#if !defined(BUTTON_DELAY)
			emberEventControlSetActive(buttonEvent1);
#else
			emberEventControlSetDelayMS(buttonEvent1, 10);
#endif
		}
		break;
#endif
	default:
		break;
	}
}
