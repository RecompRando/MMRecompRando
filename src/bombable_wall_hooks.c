#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define LOCATION_BOMBWALL (AP_PREFIX_ONEOFFS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) | 0x0F)

// BgAstrBombwall - Astral Observatory Bombable Wall
#include "overlays/actors/ovl_Bg_Astr_Bombwall/z_bg_astr_bombwall.h"

RECOMP_HOOK("func_80C0A458")
void BgAstrBombwall_DropRandoItem(BgAstrBombwall* this, PlayState* play) {
    u32 location = LOCATION_BOMBWALL;
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
    }
}

// BgHakaBombwall - Beneath the Grave Bombable Wall
#include "overlays/actors/ovl_Bg_Haka_Bombwall/z_bg_haka_bombwall.h"

RECOMP_HOOK("BgHakaBombwall_PlayCutscene")
void BgHakaBombwall_DropRandoItem(BgHakaBombwall* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        u32 location = LOCATION_BOMBWALL;
        if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
            Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
        }
    }
}

// BgHakuginBombwall - Snowhead Temple Bombable Wall
#include "overlays/actors/ovl_Bg_Hakugin_Bombwall/z_bg_hakugin_bombwall.h"

RECOMP_HOOK("func_80ABCD98")
void BgHakuginBombwall_DropRandoItem(BgHakuginBombwall* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        u32 location = LOCATION_BOMBWALL;
        if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
            Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
        }
    }
}

// BgIkanaBombwall - Stone Tower Temple Bombable Wall/Floor
#include "overlays/actors/ovl_Bg_Ikana_Bombwall/z_bg_ikana_bombwall.h"

RECOMP_HOOK("func_80BD503C")
void BgIkanaBombwall_DropRandoItem(BgIkanaBombwall* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        u32 location = LOCATION_BOMBWALL;
        if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
            Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
        }
    }
}

// BgKin2Bombwall - Ocean Spider House Bombable Wall
#include "overlays/actors/ovl_Bg_Kin2_Bombwall/z_bg_kin2_bombwall.h"

RECOMP_HOOK("BgKin2Bombwall_PlayCutscene")
void BgKin2Bombwall_DropRandoItem(BgKin2Bombwall* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        u32 location = LOCATION_BOMBWALL;
        if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
            Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
        }
    }
}

// BgLastBwall - Link Moon Dungeon Bombable Wall
#include "overlays/actors/ovl_Bg_Last_Bwall/z_bg_last_bwall.h"

RECOMP_HOOK("func_80C188C4")
void BgLastBwall_DropRandoItem(BgLastBwall* this, PlayState* play) {
    u32 location = LOCATION_BOMBWALL;
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
    }
}