// This file is generated by Simplicity Studio.  Please do not edit manually.
//
//

// This file contains the tokens for attributes stored in flash


// Identifier tags for tokens
// Creator for attribute: OEM Select, singleton.
#define CREATOR_SIMON_EXT_OEM_SELECT_SINGLETON 0xB000
#define NVM3KEY_SIMON_EXT_OEM_SELECT_SINGLETON ( NVM3KEY_DOMAIN_ZIGBEE | 0xB000 )
// Creator for attribute: Tuya Authorization attr1, singleton.
#define CREATOR_TUYA_AUTH_1_SINGLETON 0xB001
#define NVM3KEY_TUYA_AUTH_1_SINGLETON ( NVM3KEY_DOMAIN_ZIGBEE | 0xB001 )
// Creator for attribute: Tuya Authorization attr2, singleton.
#define CREATOR_TUYA_AUTH_2_SINGLETON 0xB002
#define NVM3KEY_TUYA_AUTH_2_SINGLETON ( NVM3KEY_DOMAIN_ZIGBEE | 0xB002 )
// Creator for attribute: on/off, endpoint: 1
#define CREATOR_ON_OFF_1 0xB003
#define NVM3KEY_ON_OFF_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB003 )
// Creator for attribute: start up on off, endpoint: 1
#define CREATOR_START_UP_ON_OFF_1 0xB004
#define NVM3KEY_START_UP_ON_OFF_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB004 )
// Creator for attribute: current level, endpoint: 1
#define CREATOR_CURRENT_LEVEL_1 0xB005
#define NVM3KEY_CURRENT_LEVEL_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB005 )
// Creator for attribute: start up current level, endpoint: 1
#define CREATOR_START_UP_CURRENT_LEVEL_1 0xB006
#define NVM3KEY_START_UP_CURRENT_LEVEL_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB006 )
// Creator for attribute: closed limit, endpoint: 1
#define CREATOR_SHADE_CONFIG_CLOSED_LIMIT_1 0xB007
#define NVM3KEY_SHADE_CONFIG_CLOSED_LIMIT_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB007 )
// Creator for attribute: LedState, endpoint: 1
#define CREATOR_ATTRIBUTE_MFG_INDICATOR_STATE_1 0xB008
#define NVM3KEY_ATTRIBUTE_MFG_INDICATOR_STATE_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB008 )
// Creator for attribute: TimeScale, endpoint: 1
#define CREATOR_ATTRIBUTE_MFG_TIME_SCALE_1 0xB009
#define NVM3KEY_ATTRIBUTE_MFG_TIME_SCALE_1 ( NVM3KEY_DOMAIN_ZIGBEE | 0xB009 )


// Types for the tokens
#ifdef DEFINETYPES
typedef uint8_t  tokType_on_off;
typedef uint8_t  tokType_start_up_on_off;
typedef uint8_t  tokType_current_level;
typedef uint8_t  tokType_start_up_current_level;
typedef uint16_t  tokType_color_control_color_temperature;
typedef uint16_t  tokType_start_up_color_temperature_mireds;
typedef uint16_t  tokType_shade_config_closed_limit;
typedef uint8_t  tokType_attribute_mfg_time_scale;
typedef uint8_t  tokType_attribute_mfg_indicator_state;
typedef uint8_t  tokType_simon_ext_oem_select;
typedef uint8_t  tokType_tuya_auth_1[41];
typedef uint8_t  tokType_tuya_auth_2[41];
#endif // DEFINETYPES


// Actual token definitions
#ifdef DEFINETOKENS
DEFINE_BASIC_TOKEN(SIMON_EXT_OEM_SELECT_SINGLETON, tokType_simon_ext_oem_select, 0x01)
DEFINE_BASIC_TOKEN(TUYA_AUTH_1_SINGLETON, tokType_tuya_auth_1, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
DEFINE_BASIC_TOKEN(TUYA_AUTH_2_SINGLETON, tokType_tuya_auth_2, {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00})
DEFINE_BASIC_TOKEN(ON_OFF_1, tokType_on_off, 0x00)
DEFINE_BASIC_TOKEN(START_UP_ON_OFF_1, tokType_start_up_on_off, 0xFF)
DEFINE_BASIC_TOKEN(CURRENT_LEVEL_1, tokType_current_level, 0xFE)
DEFINE_BASIC_TOKEN(START_UP_CURRENT_LEVEL_1, tokType_start_up_current_level, 0xFF)
DEFINE_BASIC_TOKEN(SHADE_CONFIG_CLOSED_LIMIT_1, tokType_shade_config_closed_limit, 1800)
DEFINE_BASIC_TOKEN(ATTRIBUTE_MFG_INDICATOR_STATE_1, tokType_attribute_mfg_indicator_state, 0)
DEFINE_BASIC_TOKEN(ATTRIBUTE_MFG_TIME_SCALE_1, tokType_attribute_mfg_time_scale, 0)
#endif // DEFINETOKENS


// Macro snippet that loads all the attributes from tokens
#define GENERATED_TOKEN_LOADER(endpoint) do {\
  uint8_t ptr[41]; \
  uint8_t curNetwork = emberGetCurrentNetwork(); \
  uint8_t epNetwork; \
  halCommonGetToken((tokType_simon_ext_oem_select *)ptr, TOKEN_SIMON_EXT_OEM_SELECT_SINGLETON); \
  emberAfWriteServerAttribute(1, ZCL_BASIC_CLUSTER_ID, ZCL_SIMON_EXT_OEM_SELECT_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
  halCommonGetToken((tokType_tuya_auth_1 *)ptr, TOKEN_TUYA_AUTH_1_SINGLETON); \
  emberAfWriteServerAttribute(1, ZCL_BASIC_CLUSTER_ID, ZCL_TUYA_AUTH_1_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_ARRAY_ATTRIBUTE_TYPE); \
  halCommonGetToken((tokType_tuya_auth_2 *)ptr, TOKEN_TUYA_AUTH_2_SINGLETON); \
  emberAfWriteServerAttribute(1, ZCL_BASIC_CLUSTER_ID, ZCL_TUYA_AUTH_2_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_ARRAY_ATTRIBUTE_TYPE); \
  epNetwork = emberAfNetworkIndexFromEndpoint(1); \
  if((endpoint) == 1 || ((endpoint) == EMBER_BROADCAST_ENDPOINT && epNetwork == curNetwork)) { \
    halCommonGetToken((tokType_on_off *)ptr, TOKEN_ON_OFF_1); \
    emberAfWriteServerAttribute(1, ZCL_ON_OFF_CLUSTER_ID, ZCL_ON_OFF_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_BOOLEAN_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_start_up_on_off *)ptr, TOKEN_START_UP_ON_OFF_1); \
    emberAfWriteServerAttribute(1, ZCL_ON_OFF_CLUSTER_ID, ZCL_START_UP_ON_OFF_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_ENUM8_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_current_level *)ptr, TOKEN_CURRENT_LEVEL_1); \
    emberAfWriteServerAttribute(1, ZCL_LEVEL_CONTROL_CLUSTER_ID, ZCL_CURRENT_LEVEL_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_start_up_current_level *)ptr, TOKEN_START_UP_CURRENT_LEVEL_1); \
    emberAfWriteServerAttribute(1, ZCL_LEVEL_CONTROL_CLUSTER_ID, ZCL_START_UP_CURRENT_LEVEL_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_shade_config_closed_limit *)ptr, TOKEN_SHADE_CONFIG_CLOSED_LIMIT_1); \
    emberAfWriteServerAttribute(1, ZCL_SHADE_CONFIG_CLUSTER_ID, ZCL_SHADE_CONFIG_CLOSED_LIMIT_ATTRIBUTE_ID, (uint8_t*)ptr, ZCL_INT16U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_attribute_mfg_indicator_state *)ptr, TOKEN_ATTRIBUTE_MFG_INDICATOR_STATE_1); \
    emberAfWriteManufacturerSpecificServerAttribute(1, ZCL_VC_MFG_EXTENSION_CLUSTER_ID, ZCL_ATTRIBUTE_MFG_INDICATOR_STATE_ATTRIBUTE_ID, 0x1254, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
    halCommonGetToken((tokType_attribute_mfg_time_scale *)ptr, TOKEN_ATTRIBUTE_MFG_TIME_SCALE_1); \
    emberAfWriteManufacturerSpecificServerAttribute(1, ZCL_VC_MFG_EXTENSION_CLUSTER_ID, ZCL_ATTRIBUTE_MFG_TIME_SCALE_ATTRIBUTE_ID, 0x1254, (uint8_t*)ptr, ZCL_INT8U_ATTRIBUTE_TYPE); \
  } \
} while(false)


// Macro snippet that saves the attribute to token
#define GENERATED_TOKEN_SAVER do {\
  uint8_t allZeroData[41]; \
  MEMSET(allZeroData, 0, 41); \
  if ( data == NULL ) data = allZeroData; \
  if ( clusterId == 0x00 ) { \
    if ( metadata->attributeId == 0xFC00 && 0x0000 == getManufacturerCode( (EmberAfManufacturerCodeEntry *) attributeManufacturerCodes, attributeManufacturerCodeCount, (metadata - generatedAttributes)) &&!emberAfAttributeIsClient(metadata) ) \
      halCommonSetToken(TOKEN_SIMON_EXT_OEM_SELECT_SINGLETON, data); \
    if ( metadata->attributeId == 0xFFE0 && 0x0000 == getManufacturerCode( (EmberAfManufacturerCodeEntry *) attributeManufacturerCodes, attributeManufacturerCodeCount, (metadata - generatedAttributes)) &&!emberAfAttributeIsClient(metadata) ) \
      halCommonSetToken(TOKEN_TUYA_AUTH_1_SINGLETON, data); \
    if ( metadata->attributeId == 0xFFE1 && 0x0000 == getManufacturerCode( (EmberAfManufacturerCodeEntry *) attributeManufacturerCodes, attributeManufacturerCodeCount, (metadata - generatedAttributes)) &&!emberAfAttributeIsClient(metadata) ) \
      halCommonSetToken(TOKEN_TUYA_AUTH_2_SINGLETON, data); \
  }\
  if ( endpoint == 1 ) { \
    if ( clusterId == 0x06 ) { \
      if ( metadata->attributeId == 0x0000 && 0x0000 == getManufacturerCode( (EmberAfManufacturerCodeEntry *) attributeManufacturerCodes, attributeManufacturerCodeCount, (metadata - generatedAttributes)) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_ON_OFF_1, data); \
      if ( metadata->attributeId == 0x4003 && 0x0000 == getManufacturerCode( (EmberAfManufacturerCodeEntry *) attributeManufacturerCodes, attributeManufacturerCodeCount, (metadata - generatedAttributes)) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_START_UP_ON_OFF_1, data); \
    } else if ( clusterId == 0x08 ) { \
      if ( metadata->attributeId == 0x0000 && 0x0000 == getManufacturerCode( (EmberAfManufacturerCodeEntry *) attributeManufacturerCodes, attributeManufacturerCodeCount, (metadata - generatedAttributes)) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_CURRENT_LEVEL_1, data); \
      if ( metadata->attributeId == 0x4000 && 0x0000 == getManufacturerCode( (EmberAfManufacturerCodeEntry *) attributeManufacturerCodes, attributeManufacturerCodeCount, (metadata - generatedAttributes)) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_START_UP_CURRENT_LEVEL_1, data); \
    } else if ( clusterId == 0x0100 ) { \
      if ( metadata->attributeId == 0x0010 && 0x0000 == getManufacturerCode( (EmberAfManufacturerCodeEntry *) attributeManufacturerCodes, attributeManufacturerCodeCount, (metadata - generatedAttributes)) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_SHADE_CONFIG_CLOSED_LIMIT_1, data); \
    } else if ( clusterId == 0xFC56 ) { \
      if ( metadata->attributeId == 0x0000 && 0x1254 == getManufacturerCode( (EmberAfManufacturerCodeEntry *) attributeManufacturerCodes, attributeManufacturerCodeCount, (metadata - generatedAttributes)) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_ATTRIBUTE_MFG_INDICATOR_STATE_1, data); \
      if ( metadata->attributeId == 0x0004 && 0x1254 == getManufacturerCode( (EmberAfManufacturerCodeEntry *) attributeManufacturerCodes, attributeManufacturerCodeCount, (metadata - generatedAttributes)) &&!emberAfAttributeIsClient(metadata) ) \
        halCommonSetToken(TOKEN_ATTRIBUTE_MFG_TIME_SCALE_1, data); \
    } \
  } \
} while(false)


