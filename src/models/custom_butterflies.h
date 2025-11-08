#include "ultra64.h"
#include "global.h"

typedef enum ButterflyLimb {
    /* 0x00 */ BUTTERFLY_LIMB_NONE,
    /* 0x01 */ BUTTERFLY_LIMB_01,
    /* 0x02 */ BUTTERFLY_LIMB_02,
    /* 0x03 */ BUTTERFLY_LIMB_03,
    /* 0x04 */ BUTTERFLY_LIMB_04,
    /* 0x05 */ BUTTERFLY_LIMB_05,
    /* 0x06 */ BUTTERFLY_LIMB_06,
    /* 0x07 */ BUTTERFLY_LIMB_07,
    /* 0x08 */ BUTTERFLY_LIMB_MAX
} ButterflyLimb;

extern Gfx randoButterflyLimb04DL[];
extern Gfx randoButterflyLimb07DL[];