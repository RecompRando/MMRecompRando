#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "actor_helpers.h"
                            
#define BG_KIN2_PICTURE_SKULLTULA_COLLECTED(thisx) (((thisx)->params >> 5) & 1)
#define BG_KIN2_PICTURE_GET_3FC(thisx) ((u8)(((thisx & 0x3FC)) >> 2))
#define BG_KIN2_PICTURE_SKULLTULA_SPAWN_PARAM(thisx) ((((thisx)->params & 0x1F) << 2) | 0xFF03)

#define LOCATION_SKULL_TOKEN (0x060000 | (play->sceneId << 8) | ((skulltulaParams >> 2) & 0x1F))

#define LOCATION_PAINTING (AP_PREFIX_ONEOFFS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->dyna.actor))
                            
struct BgKin2Picture;

typedef void (*BgKin2PictureActionFunc)(struct BgKin2Picture*, PlayState*);

typedef struct BgKin2Picture {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ ColliderTris colliderTris;
    /* 0x17C */ ColliderTrisElement colliderElement[2];
    /* 0x234 */ BgKin2PictureActionFunc actionFunc;
    /* 0x238 */ s16 step;
    /* 0x23A */ s8 paintingTimer; // Used for when painting is shaking and for timing Gold Skulltula spawn.
    /* 0x23B */ s8 landTimer;
    /* 0x23C */ s16 xOffsetAngle;
    /* 0x23E */ s16 yOffsetAngle;
    /* 0x240 */ s8 cutsceneStarted;
    /* 0x241 */ s8 hasSpawnedDust;
    /* 0x242 */ s8 skulltulaNoiseTimer;
} BgKin2Picture; // size = 0x244

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
void BgKin2Picture_SetupFall_Hook(BgKin2Picture* this) {
    PlayState* play = gPlay;
    
    if (rando_get_slotdata_u32("oneoffs") && !rando_location_is_checked(LOCATION_PAINTING)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, LOCATION_PAINTING);
    }
}