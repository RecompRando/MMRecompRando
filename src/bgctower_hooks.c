#include "global.h"
#include "modding.h"
#include "recomputils.h"

#include "overlays/actors/ovl_Bg_Ctower_Rot/z_bg_ctower_rot.h"

#define IS_DOOR_MAIN(this) ((this)->dyna.actor.params == BGCTOWERROT_STONE_DOOR_MAIN)

static bool isOpening;
BgCtowerRot* gBgCtowerRot;

void BgCtowerRot_DoorIdle(BgCtowerRot *this, PlayState *play);
void BgCtowerRot_DoorOpen(BgCtowerRot *this, PlayState *play);

RECOMP_HOOK("BgCtowerRot_Init")
void OnBgCtowerRot_Init(Actor* thisx, PlayState* play) {
    gBgCtowerRot = ((BgCtowerRot*)thisx);
}

RECOMP_HOOK_RETURN("BgCtowerRot_Init")
void AfterBgCtowerRot_Init(Actor* thisx, PlayState* play) {
    if (gBgCtowerRot->actionFunc == BgCtowerRot_DoorIdle) {
        gBgCtowerRot->actionFunc = BgCtowerRot_DoorOpen;
    }
}

RECOMP_PATCH void BgCtowerRot_DoorDoNothing(BgCtowerRot* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Input* controller = CONTROLLER1(&play->state);

    if(!Player_InCsMode(play) && (this->dyna.actor.xzDistToPlayer < 100.0f)) {
        player->doorActor = &this->dyna.actor;
        player->doorType = PLAYER_DOORTYPE_SLIDING;
        if(CHECK_BTN_ALL(controller->press.button, BTN_A)) {
            this->actionFunc = BgCtowerRot_DoorOpen;
            isOpening = true;
        }
    } else {
        player->doorActor = NULL;
    }
}

void BgCtowerRot_DoorOpen(BgCtowerRot *this, PlayState *play) {
    if (!Math_SmoothStepToF(&this->timer, 80.0f, 0.1f, 15.0f, 0.1f) && isOpening) {
        if (IS_DOOR_MAIN(this)) {
            Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_STONEDOOR_STOP);
            isOpening = false;
        }
    } else if (IS_DOOR_MAIN(this) && isOpening) {
        Actor_PlaySfx_Flagged(&this->dyna.actor, NA_SE_EV_STONE_STATUE_OPEN - SFX_FLAG);
    }

    this->dyna.actor.world.pos.x =
        this->dyna.actor.home.pos.x + (Math_SinS(this->dyna.actor.world.rot.y) * this->timer);
    this->dyna.actor.world.pos.z =
        this->dyna.actor.home.pos.z + (Math_CosS(this->dyna.actor.world.rot.y) * this->timer);
}
