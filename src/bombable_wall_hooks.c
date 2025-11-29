#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define LOCATION_BOMBWALL (AP_PREFIX_ONEOFFS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) | 0x0F)

// BgAstrBombwall - Astral Observatory Bombable Wall
typedef struct BgAstrBombwall {
    DynaPolyActor dyna;
    ColliderTris collider;
    ColliderTrisElement colliderElements[2];
    void* actionFunc;
    s16 csIdList[1];
} BgAstrBombwall;

RECOMP_HOOK("func_80C0A458")
void BgAstrBombwall_Hook(BgAstrBombwall* this, PlayState* play) {
    u32 location = LOCATION_BOMBWALL;
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
    }
}

// BgHakaBombwall - Beneath the Grave Bombable Wall
typedef struct BgHakaBombwall {
    DynaPolyActor dyna;
    ColliderCylinder collider;
    void* actionFunc;
    s16 csTimer;
} BgHakaBombwall;

RECOMP_HOOK("BgHakaBombwall_PlayCutscene")
void BgHakaBombwall_Hook(BgHakaBombwall* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        u32 location = LOCATION_BOMBWALL;
        if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
            Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
        }
    }
}

// BgHakuginBombwall - Snowhead Temple Bombable Wall
typedef struct BgHakuginBombwall {
    DynaPolyActor dyna;
    ColliderCylinder collider;
    void* actionFunc;
    s16 unk_1AC;
} BgHakuginBombwall;

RECOMP_HOOK("func_80ABCD98")
void BgHakuginBombwall_Hook(BgHakuginBombwall* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        u32 location = LOCATION_BOMBWALL;
        if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
            Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
        }
    }
}

// BgIkanaBombwall - Stone Tower Temple Bombable Wall/Floor
typedef struct BgIkanaBombwall {
    DynaPolyActor dyna;
    ColliderCylinder collider;
    void* actionFunc;
    s16 unk_1AC;
} BgIkanaBombwall;

RECOMP_HOOK("func_80BD503C")
void BgIkanaBombwall_Hook(BgIkanaBombwall* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        u32 location = LOCATION_BOMBWALL;
        if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
            Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
        }
    }
}

// BgKin2Bombwall - Ocean Spider House Bombable Wall
typedef struct BgKin2Bombwall {
    DynaPolyActor dyna;
    ColliderCylinder collider;
    void* actionFunc;
    s16 timer;
} BgKin2Bombwall;

RECOMP_HOOK("BgKin2Bombwall_PlayCutscene")
void BgKin2Bombwall_Hook(BgKin2Bombwall* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        u32 location = LOCATION_BOMBWALL;
        if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
            Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
        }
    }
}

// BgLastBwall - Link Moon Dungeon Bombable Wall
typedef struct BgLastBwall {
    DynaPolyActor dyna;
    ColliderTris colliderTris;
    ColliderTrisElement colliderTrisElement[2];
    void* actionFunc;
    s16 csIdList[1];
    s16 type;
} BgLastBwall;

RECOMP_HOOK("func_80C188C4")
void BgLastBwall_Hook(BgLastBwall* this, PlayState* play) {
    u32 location = LOCATION_BOMBWALL;
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(location)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, location);
    }
}