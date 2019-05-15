
#include <stdint.h>

#define CREATOR_VC_BASIC_INFO   ((uint16_t)0x0010)
#define CREATOR_VC_EP_INFO      ((uint16_t)0x000A)
//#define CREATOR_VC_MANU_SET     ((uint16_t)0x000B)
//#define CREATOR_VC_POWER_CYCLE  ((uint16_t)0x000C)

// Types for the tokens
#ifdef DEFINETYPES
#define VC_MANUFACTURE_NAME_SIZE      ((uint8_t)32)
#define VC_MODE_ID_SIZE               ((uint8_t)32)
#define VC_NODE_CONFIG_SIZE           ((uint8_t)32)

typedef struct {
  char manufactureName[VC_MANUFACTURE_NAME_SIZE];
  char modelId[VC_MODE_ID_SIZE];
} VcBasic_t;

typedef struct {
  uint8_t nodeConfig[VC_NODE_CONFIG_SIZE];
} VcEpInfo_t;

///typedef struct {
//  uint8_t powerSource;
//  uint8_t modelType;
//  int8_t  txPower;
//} VcManuSet_t;
//power cycle
#endif // DEFINETYPES

// Actual token definitions
#ifdef DEFINETOKENS
DEFINE_BASIC_TOKEN(VC_BASIC_INFO, VcBasic_t, {0})
DEFINE_BASIC_TOKEN(VC_EP_INFO, VcEpInfo_t, {0})
//DEFINE_BASIC_TOKEN(VC_MANU_SET, VcManuSet_t, {0})
//DEFINE_COUNTER_TOKEN(VC_POWER_CYCLE, uint8_t, 0)
#endif // DEFINETOKENS

//#include "./../VancountForEuControls_3In1_tokens.h"
