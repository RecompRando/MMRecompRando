#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define LOCATION_BOMBWALL (AP_PREFIX_ONEOFFS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) | 0x0F)

// BgAstrBombwall - Astral Observatory Bombable Wall
#include "overlays/actors/ovl_Bg_Astr_Bombwall/z_bg_astr_bombwall.h"

RECOMP_HOOK("func_80C0A458")
void BgAstrBombwall_DropRandoItem(BgAstrBombwall* this, PlayState* play) {
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(LOCATION_BOMBWALL)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, LOCATION_BOMBWALL);
    }
}

// BgHakaBombwall - Beneath the Grave Bombable Wall
#include "overlays/actors/ovl_Bg_Haka_Bombwall/z_bg_haka_bombwall.h"

RECOMP_HOOK("BgHakaBombwall_SetupPlayCutscene")
void BgHakaBombwall_DropRandoItem(BgHakaBombwall* this) {
    PlayState* play = gPlay;
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(LOCATION_BOMBWALL)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, LOCATION_BOMBWALL);
    }
}

// BgHakuginBombwall - Snowhead Temple Bombable Wall
#include "overlays/actors/ovl_Bg_Hakugin_Bombwall/z_bg_hakugin_bombwall.h"

RECOMP_HOOK("func_80ABCD98")
void BgHakuginBombwall_DropRandoItem(BgHakuginBombwall* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(LOCATION_BOMBWALL)) {
            Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, LOCATION_BOMBWALL);
        }
    }
}

// BgIkanaBombwall - Stone Tower Temple Bombable Wall/Floor
#include "overlays/actors/ovl_Bg_Ikana_Bombwall/z_bg_ikana_bombwall.h"

RECOMP_HOOK("func_80BD4FF8")
void BgIkanaBombwall_DropRandoItem(BgIkanaBombwall* this) {
    PlayState* play = gPlay;
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(LOCATION_BOMBWALL)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, LOCATION_BOMBWALL);
    }
}

// BgKin2Bombwall - Ocean Spider House Bombable Wall
#include "overlays/actors/ovl_Bg_Kin2_Bombwall/z_bg_kin2_bombwall.h"

RECOMP_HOOK("BgKin2Bombwall_SetupPlayCutscene")
void BgKin2Bombwall_DropRandoItem(BgKin2Bombwall* this) {
    PlayState* play = gPlay;
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(LOCATION_BOMBWALL)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, LOCATION_BOMBWALL);
    }
}

// BgLastBwall - Link Moon Dungeon Bombable Wall
#include "overlays/actors/ovl_Bg_Last_Bwall/z_bg_last_bwall.h"

RECOMP_HOOK("func_80C188C4")
void BgLastBwall_DropRandoItem(BgLastBwall* this, PlayState* play) {
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(LOCATION_BOMBWALL)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, LOCATION_BOMBWALL);
    }
}