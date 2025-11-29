#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Bg_Kin2_Picture/z_bg_kin2_picture.h"

#define LOCATION_SKULL_TOKEN (0x060000 | (play->sceneId << 8) | ((skulltulaParams >> 2) & 0x1F))
#define LOCATION_PAINTING (AP_PREFIX_ONEOFFS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->dyna.actor))

RECOMP_PATCH bool BgKin2Picture_IsSkulltulaCollected(PlayState* play, s32 skulltulaParams) {
    s32 flag = -1;

    if (rando_location_is_checked(LOCATION_SKULL_TOKEN)) {
        return true;
    }

    if ((u8)skulltulaParams & 3) {
        flag = BG_KIN2_PICTURE_GET_3FC(skulltulaParams);
    }

    return (flag >= 0) && Flags_GetTreasure(play, flag);
}

RECOMP_HOOK("BgKin2Picture_SetupFall")
void BgKin2Picture_DropRandoItem(BgKin2Picture* this) {
    PlayState* play = gPlay;
    
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(LOCATION_PAINTING)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, LOCATION_PAINTING);
    }
}