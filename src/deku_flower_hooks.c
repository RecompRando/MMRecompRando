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

    if (waitingForOffer && !(player->stateFlags3 & PLAYER_STATE3_100)) {
        if (Actor_HasParent(&this->dyna.actor, play)) {
            recomp_printf("flower location: 0x%06X\n", flowerLocation);
            this->dyna.actor.parent = NULL;
            flowerLocation = 0;
            waitingForOffer = false;
        } else {
            Actor_OfferGetItemHook(&this->dyna.actor, play, rando_get_item_id(flowerLocation), flowerLocation, 1000.0f, 1000.0f, true, true);
        }
    }

    if ((player->stateFlags3 & PLAYER_STATE3_200) && (this->dyna.actor.xzDistToPlayer < 20.0f)) {
        if (!rando_location_is_checked_async(LOCATION_FLOWER)) {
            flowerLocation = LOCATION_FLOWER;
            waitingForOffer = true;
        }
    }
}