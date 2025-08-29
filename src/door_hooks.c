#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "overlays/actors/ovl_Door_Shutter/z_door_shutter.h"
#include "overlays/actors/ovl_Bg_Open_Shutter/z_bg_open_shutter.h"

RECOMP_HOOK("DoorShutter_SetupDoor")
void OnDoorShutter_SetupDoor(DoorShutter* this, PlayState* play) {
    if (play->sceneId == SCENE_OPENINGDAN) {
        this->doorType = 0;
    }
}

void func_80ACAE5C(BgOpenShutter* this, PlayState* play);
s8 func_80ACABA8(BgOpenShutter* this, PlayState* play);

RECOMP_PATCH void func_80ACAD88(BgOpenShutter* this, PlayState* play) {
    s32 doorDirection;

    if (this->slidingDoor.unk_15C != 0) {
        Player* player = GET_PLAYER(play);

        Actor_PlaySfx(&this->slidingDoor.dyna.actor, NA_SE_EV_SLIDE_DOOR_OPEN);
        Camera_ChangeDoorCam(play->cameraPtrs[CAM_ID_MAIN], &this->slidingDoor.dyna.actor, player->cv.doorBgCamIndex,
                             0.0f, 12, 15, 10);
        this->unk_164 = 0;
        this->actionFunc = func_80ACAE5C;
        this->slidingDoor.dyna.actor.velocity.y = 0.0f;
    } else {
        Player* player = GET_PLAYER(play);

        doorDirection = func_80ACABA8(this, play);
        // if (doorDirection > 0) {
        if (doorDirection != 0) {
            player->doorType = PLAYER_DOORTYPE_SLIDING;
            player->doorDirection = doorDirection;
            player->doorActor = &this->slidingDoor.dyna.actor;
            func_80122F28(player);
        }
    }
}