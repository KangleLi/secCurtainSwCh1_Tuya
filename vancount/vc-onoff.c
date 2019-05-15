/*
 * vc-onoff.c
 *
 *  Created on: Jun 13, 2018
 *      Author: tao
 */
#include "app/framework/include/af.h"
#include "./vancount-z3-net.h"
#include "./vancount-z3.h"
#include "./list.h"


void vcClientControl(uint8_t ep, uint8_t cmd)
{
#if 1
	emberAfSetCommandEndpoints(ep, 0);
	if (cmd) {
		emberAfFillCommandOnOffClusterOn();
	} else {
		emberAfFillCommandOnOffClusterOff();
	}
	emberAfSendCommandUnicastToBindings();
#else
	char s[16] = {0};
	if (cmd) {
		sprintf(s, "%p %p %p", "zcl", "on-off", "on");
	} else {
		sprintf(s, "%p %p %p", "zcl", "on-off", "off")
	}
	emberProcessCommandString(s, strlen(s));
	memset(s, 0, 16);
	sprintf(s, "%p %d", "bsend", ep);
	emberProcessCommandString(s, strlen(s));
#endif
}

void vcServerControl(uint8_t cmd)
{
	if (cmd) {
		emberAfFillCommandOnOffClusterOn();
	} else {
		emberAfFillCommandOnOffClusterOff();
	}
	emberAfSendCommandUnicastToBindings();
}

uint8_t  vcGetBindingInfoByEp(uint8_t ep)
{
	EmberStatus status = EMBER_INVALID_BINDING_INDEX;
	uint8_t i;

	for (i = 0; i < EMBER_BINDING_TABLE_SIZE; i++) {
	  EmberBindingTableEntry binding;
	  status = emberGetBinding(i, &binding);
	  if (status != EMBER_SUCCESS) {
	    return 0xFF;
	  }
	  if (binding.type == EMBER_UNICAST_BINDING
	      && binding.local == ep
	      && binding.clusterId == ZCL_ON_OFF_CLUSTER_ID) {
		  return i;
	  }
	}
	return 0xFF;
}
