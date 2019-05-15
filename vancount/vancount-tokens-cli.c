/*
 * =====================================================================================
 *
 *       Filename:  vancount-tokens.c
 *
 *    Description:  read and write tokens.
 *
 *        Version:  1.0
 *        Created:  2016/4/28 14:10:40
 *       Revision:  none
 *       Compiler:
 *
 *         Author:  tie tao (tt), catch.tao@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */

#include "app/framework/include/af.h"
#include "vancount-tokens.h"

#define VC_SUCCESS                          EMBER_SUCCESS
#define VC_ERROR                            EMBER_ERR_FATAL

EmberStatus vcSetManufactoryName(char *name)
{
  VcBasic_t v;
  uint8_t len;

  halCommonGetToken(&v, TOKEN_VC_BASIC_INFO);
  memset(v.manufactureName, 0, VC_MANUFACTURE_NAME_SIZE);
  len = strlen(name);
  if (VC_MANUFACTURE_NAME_SIZE - 1 < len) {
    len = VC_MANUFACTURE_NAME_SIZE - 1;
  }
  memcpy(v.manufactureName, name, strlen(name));
  halCommonSetToken(TOKEN_VC_BASIC_INFO, &v);

  return EMBER_SUCCESS;
}

EmberStatus vcGetManufactoryName(char *name)
{
  VcBasic_t v;
  uint8_t len;

  halCommonGetToken(&v, TOKEN_VC_BASIC_INFO);
  len = strlen(v.manufactureName);
  if (VC_MANUFACTURE_NAME_SIZE - 1 < len) {
    len = VC_MANUFACTURE_NAME_SIZE - 1;
  }
  memcpy(name, v.manufactureName, len);
  name[len] = '\0';

  return EMBER_SUCCESS;
}

EmberStatus vcSetModelId(char *name)
{
  VcBasic_t v;

  halCommonGetToken(&v, TOKEN_VC_BASIC_INFO);
  memset(v.modelId, 0, VC_MODE_ID_SIZE);
  uint8_t len;
  len = strlen(name);
  if (VC_MODE_ID_SIZE - 1 < len) {
    len = VC_MODE_ID_SIZE - 1;
  }
  memcpy(v.modelId, name, len);
  halCommonSetToken(TOKEN_VC_BASIC_INFO, &v);

  return EMBER_SUCCESS;
}

EmberStatus vcGetModelId(char *name)
{
  VcBasic_t v;

  halCommonGetToken(&v, TOKEN_VC_BASIC_INFO);
  uint8_t len;
  len = strlen(v.modelId);
  if (VC_MODE_ID_SIZE - 1 < len) {
    len = VC_MODE_ID_SIZE - 1;
  }
  memcpy(name, v.modelId, len);
  name[len] = '\0';
  return EMBER_SUCCESS;
}

EmberStatus vcSetNodeConfig(char *data, uint8_t dataSize)
{
  VcEpInfo_t v;

  halCommonGetToken(&v, TOKEN_VC_EP_INFO);
  memset(v.nodeConfig, 0, VC_NODE_CONFIG_SIZE);
  memcpy(v.nodeConfig, data, VC_NODE_CONFIG_SIZE);
  halCommonSetToken(TOKEN_VC_EP_INFO, &v);

  return EMBER_SUCCESS;
}

EmberStatus vcGetNodeConfig(char *data, uint8_t *dataSize)
{
  VcEpInfo_t v;

  halCommonGetToken(&v, TOKEN_VC_EP_INFO);
  memcpy(data, v.nodeConfig, VC_NODE_CONFIG_SIZE);

  return EMBER_SUCCESS;
}

