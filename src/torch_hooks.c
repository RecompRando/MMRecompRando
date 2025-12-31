/**
 * Torch (ObjSyokudai) Randomization
 */

#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Syokudai/z_obj_syokudai.h"

#define LOCATION_TORCH (0x070000 | (play->sceneId << 8) | OBJ_SYOKUDAI_GET_SWITCH_FLAG(thisx))

// Track which torches have dropped items this session
#define MAX_TORCHES 32
static Actor* sDroppedTorches[MAX_TORCHES];

static bool HasDropped(Actor* torch) {
    for (s32 i = 0; i < MAX_TORCHES; i++) {
        if (sDroppedTorches[i] == torch) {
            return true;
        }
    }
    return false;
}

static void MarkDropped(Actor* torch) {
    for (s32 i = 0; i < MAX_TORCHES; i++) {
        if (sDroppedTorches[i] == NULL) {
            sDroppedTorches[i] = torch;
            return;
        }
    }
}

/**
 * Clear START_LIT flag so torches start unlit
 */
RECOMP_HOOK("ObjSyokudai_Init")
void ObjSyokudai_ClearStartLit(Actor* thisx, PlayState* play) {
    if (rando_get_slotdata_u32("oneoffs")) {
        thisx->params &= ~0x800;
    }
}

/**
 * Drop item when torch is lit by player (not by switch)
 */
RECOMP_HOOK("ObjSyokudai_Update")
void ObjSyokudai_CheckLit(Actor* thisx, PlayState* play) {
    ObjSyokudai* this = (ObjSyokudai*)thisx;
    
    // Already dropped for this torch
    if (HasDropped(thisx)) {
        return;
    }
    
    // Torch is lit and not type 0 (switch-lit)
    if (this->snuffTimer != 0 && OBJ_SYOKUDAI_GET_TYPE(thisx) != 0) {
        if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(LOCATION_TORCH)) {
            Item_RandoDropCollectible(play, &thisx->world.pos, ITEM00_APITEM, LOCATION_TORCH);
            MarkDropped(thisx);
        }
    }
}

/**
 * Reset tracking on scene change
 */
RECOMP_HOOK("Play_Init")
void ObjSyokudai_ResetTracking(PlayState* play) {
    for (s32 i = 0; i < MAX_TORCHES; i++) {
        sDroppedTorches[i] = NULL;
    }
}