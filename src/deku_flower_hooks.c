#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define PINK_DEKU_FLOWER_LIMB_MAX 0x0B
#include "overlays/actors/ovl_Obj_Etcetera/z_obj_etcetera.h"

#define LOCATION_FLOWER (0x3F0000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->dyna.actor))

static u32 flowerLocation;
static bool waitingForOffer;

void EnItem00_RandoTextAndFreeze(EnItem00* this, PlayState* play);

RECOMP_HOOK("ObjEtcetera_Init")
void OnObjEtcetera_Init(Actor* thisx, PlayState* play) {
    waitingForOffer = false;
}

RECOMP_HOOK("ObjEtcetera_Idle")
void OnObjEtcetera_Idle(ObjEtcetera* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    // if (!rando_flowers_enabled()) {
    //     return;
    // }

    if (waitingForOffer && !(player->stateFlags3 & PLAYER_STATE3_100) && !rando_location_is_checked_async(flowerLocation)) {
        recomp_printf("flower location: 0x%06X\n", flowerLocation);
        Actor* item = Item_RandoDropCollectible(play, &player->actor.world.pos, ITEM00_APITEM, flowerLocation);
        ((EnItem00*)item)->actionFunc = EnItem00_RandoTextAndFreeze;
        flowerLocation = 0;
        waitingForOffer = false;
    }

    if ((player->stateFlags3 & PLAYER_STATE3_200) && (this->dyna.actor.xzDistToPlayer < 20.0f)) {
        if (!rando_location_is_checked_async(LOCATION_FLOWER)) {
            flowerLocation = LOCATION_FLOWER;
            waitingForOffer = true;
        }
    }
}