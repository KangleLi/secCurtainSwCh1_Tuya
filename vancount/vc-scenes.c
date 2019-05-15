/*
 * vc-scenes.c
 *
 *  Created on: Jun 13, 2018
 *      Author: tao
 */
#include "app/framework/include/af.h"
#include "scenes.h"
#include "./vancount-z3-net.h"
#include "./vancount-z3.h"
#include "./list.h"


void vcClientSceneControl(uint8_t ep)
{
	// find the groutpId and sceneId according to the ep;
	EmberAfSceneTableEntry entry;
	uint8_t i;

	for (i = 0; i < EMBER_AF_PLUGIN_SCENES_TABLE_SIZE; i++) {
		emberAfPluginScenesServerRetrieveSceneEntry(entry, i);
		if (entry.endpoint == ep) {
			break;
		}
	}
	if (i == EMBER_AF_PLUGIN_SCENES_TABLE_SIZE) {
		vcSetLedHandleState(LED_HANDLE_OFF_END);
		vcLEDBlink(emberAfIndexFromEndpoint(ep), 1, 8);
		return;
	}
	vcSetLedHandleState(LED_HANDLE_OFF_END);
	emberAfCorePrintln("%p, ep:%d", __func__, emberAfIndexFromEndpoint(ep));
	halMultiLedBlinkLedOn(2, emberAfIndexFromEndpoint(ep));
	//halMultiLedBlinkLedOff(0, emberAfIndexFromEndpoint(ep));
	emberAfSetCommandEndpoints(ep, 0);
	emberAfFillCommandScenesClusterRecallScene(entry.groupId, entry.sceneId);
	emberAfSendCommandMulticast(entry.groupId);
	//emberAfSendCommandMulticastToBindings();
}

uint8_t  vcGetScenesInfoByEp(uint8_t ep)
{
	EmberAfSceneTableEntry entry;
	uint8_t i;

	for (i = 0; i < EMBER_AF_PLUGIN_SCENES_TABLE_SIZE; i++) {
		emberAfPluginScenesServerRetrieveSceneEntry(entry, i);
		if (entry.endpoint == ep) {
			return i;
		}
	}
	return 0xFF;
}
