#include "modding.h"
#include "global.h"

#define TAKKURI_LIMB_MAX 0x11

#include "overlays/actors/ovl_En_Thiefbird/z_en_thiefbird.h"

// disable takkuri item stealing
RECOMP_PATCH s32 func_80C10B0C(EnThiefbird* this, PlayState* play) {
    return false;
}