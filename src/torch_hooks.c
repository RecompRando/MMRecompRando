/**
 * Torch (ObjSyokudai) Randomization
 */

#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Syokudai/z_obj_syokudai.h"

#define LOCATION_TORCH (AP_PREFIX_TORCHES | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->actor))

// Track which torches have dropped items this session
#define MAX_TORCHES 32
static Actor* sDroppedTorches[MAX_TORCHES];

// Torchsanity options
typedef enum {
    TORCHSANITY_DISABLED,
    TORCHSANITY_ENABLED,
    TORCHSANITY_SNUFFED,
} TorchsanityOptions;

bool ObjSyokudai_HasDropped(Actor* torch) {
    for (s32 i = 0; i < MAX_TORCHES; i++) {
        if (sDroppedTorches[i] == torch) {
            return true;
        }
    }
    return false;
}

void ObjSyokudai_MarkDropped(Actor* torch) {
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
    if (recomp_get_config_u32("torchsanity") == TORCHSANITY_SNUFFED) {
        thisx->params &= ~0x800;
    }
}

/**
 * Drop item when torch is lit by player (not by switch)
 */
RECOMP_HOOK("ObjSyokudai_Update")
void ObjSyokudai_CheckLit(Actor* thisx, PlayState* play) {
    ObjSyokudai* this = (ObjSyokudai*)thisx;
    
    // Torchsanity is disabled
    if (!recomp_get_config_u32("torchsanity")) return;

    // Torch was already dropped/collected
    if (ObjSyokudai_HasDropped(&this->actor) || rando_location_is_checked(LOCATION_TORCH)) return;

    Vec3f itemSpawnPos = this->actor.world.pos;
    itemSpawnPos.y += OBJ_SYOKUDAI_FLAME_HEIGHT;
    
    // Torch is lit and does not start lit
    if (this->snuffTimer != 0 && !OBJ_SYOKUDAI_GET_START_LIT(&this->actor)) {
        Item_RandoDropCollectible(play, &itemSpawnPos, ITEM00_APITEM, LOCATION_TORCH);
        ObjSyokudai_MarkDropped(&this->actor);
        return;
    }

    // Check for when a lit torch is touched by a deku stick/arrow
    Player* player = GET_PLAYER(play);

    // Ignore the rest when torch is not lit yet
    if (this->snuffTimer <= 0) return;

    // Arrow Detection
    if (this->flameCollider.base.acFlags & AC_HIT) {
        if (this->flameCollider.info.acHitInfo->toucher.dmgFlags & 0x820) {
            Item_RandoDropCollectible(play, &itemSpawnPos, ITEM00_APITEM, LOCATION_TORCH);
            ObjSyokudai_MarkDropped(&this->actor);
            return;
        }
    }
    
    // Stick Detection
    if (player->heldItemAction == PLAYER_IA_DEKU_STICK) {
        Vec3f stickTipSeparationVec;

        Math_Vec3f_Diff(&player->meleeWeaponInfo[0].tip, &thisx->world.pos, &stickTipSeparationVec);
        stickTipSeparationVec.y -= OBJ_SYOKUDAI_STICK_IGNITION_HEIGHT;
        if (SQXYZ(stickTipSeparationVec) < SQ(OBJ_SYOKUDAI_STICK_IGNITION_RADIUS)) {
            Item_RandoDropCollectible(play, &itemSpawnPos, ITEM00_APITEM, LOCATION_TORCH);
            ObjSyokudai_MarkDropped(&this->actor);
            return;
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