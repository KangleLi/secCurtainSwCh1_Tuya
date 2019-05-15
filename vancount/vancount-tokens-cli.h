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
#ifndef VANCOUNT_TOKENS_CLI_H
#define VANCOUNT_TOKENS_CLI_H

#include "app/framework/include/af.h"

extern EmberStatus vcSetManufactoryName(char *name);
extern EmberStatus vcGetManufactoryName(char *name);

extern EmberStatus vcSetModelId(char *name);
extern EmberStatus vcGetModelId(char *name);

extern EmberStatus vcSetNodeConfig(uint8_t *data, uint8_t dataSize);
extern EmberStatus vcGetNodeConfig(uint8_t *data, uint8_t *dataSize);

#endif
