#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Door_Shutter/z_door_shutter.h"
#include "overlays/actors/ovl_Bg_Open_Shutter/z_bg_open_shutter.h"

s32 savedDoorClearFlag;
DoorShutter* sDoorShutter;
u16 savedIgosSeqId;

RECOMP_HOOK("DoorShutter_SetupDoor")
void OnDoorShutter_SetupDoor(DoorShutter* this, PlayState* play) {
    sDoorShutter = this;
    savedDoorClearFlag = true;

    // set locked door in before clock town to always be open
    if (play->sceneId == SCENE_OPENINGDAN) {
        this->doorType = 0;
    }

    // always unlock the door out of igos's room
    if (play->sceneId == SCENE_IKNINSIDE) {
        savedIgosSeqId = AudioSeq_GetActiveSeqId(SEQ_PLAYER_BGM_MAIN);
        savedDoorClearFlag = Flags_GetClear(play, this->slidingDoor.dyna.actor.room);
        Flags_SetClear(play, this->slidingDoor.dyna.actor.room);
        return;
    }

    if (!rando_get_slotdata_u32("enemy_souls")) return;

    // unlock doors when you don't have an enemy's soul
    switch (play->sceneId) {
        // Woodfall Temple
        case SCENE_MITURIN:
            switch (play->roomCtx.curRoom.num) {
                // Dragonflies Chest Room
                case 4:
                    if (!rando_has_item(AP_ITEM_ID_SOUL_ENEMY_DRAGONFLIES)) {
                        savedDoorClearFlag = Flags_GetClear(play, this->slidingDoor.dyna.actor.room);
                        Flags_SetClear(play, this->slidingDoor.dyna.actor.room);
                    }
                    break;
                // Snapper Chest Room
                case 6:
                    if (!rando_has_item(AP_ITEM_ID_SOUL_ENEMY_SNAPPERS)) {
                        savedDoorClearFlag = Flags_GetClear(play, this->slidingDoor.dyna.actor.room);
                        Flags_SetClear(play, this->slidingDoor.dyna.actor.room);
                    }
                    break;
                // Dinolfos Chest Room
                case 7:
                    if (!rando_has_item(AP_ITEM_ID_SOUL_ENEMY_DINOLFOS)) {
                        savedDoorClearFlag = Flags_GetClear(play, this->slidingDoor.dyna.actor.room);
                        Flags_SetClear(play, this->slidingDoor.dyna.actor.room);
                    }
                    break;
                // Gekko & Snapper Miniboss Room
                case 8:
                    if (randoGetLoadedActorNumInSameRoom(play, &this->slidingDoor.dyna.actor) == 0) {
                        if (!rando_has_item(AP_ITEM_ID_SOUL_MINIBOSS_GEKKO)) {
                            savedDoorClearFlag = Flags_GetClear(play, this->slidingDoor.dyna.actor.room);
                            Flags_SetClear(play, this->slidingDoor.dyna.actor.room);
                        }
                    }
                    break;
            }
            break;
    }

    recomp_printf("room %d\n", play->roomCtx.curRoom.num);
}

RECOMP_HOOK_RETURN("DoorShutter_SetupDoor")
void AfterDoorShutter_SetupDoor() {
    DoorShutter* this = sDoorShutter;
    PlayState* play = gPlay;

    // unset room flag if the room hasn't actually been cleared
    if (!savedDoorClearFlag) {
        Flags_UnsetClear(play, this->slidingDoor.dyna.actor.room);
    }
}

RECOMP_HOOK("Player_Door_Sliding")
void FixIgosBGM(PlayState* play, Player* this, Actor* door) {
    // reset bgm when leaving igos's room
    if (play->sceneId == SCENE_IKNINSIDE && this->doorDirection == 1) {
        play->envCtx.lightSettingOverride = 0;
        SEQCMD_PLAY_SEQUENCE(SEQ_PLAYER_BGM_MAIN, 20, NA_BGM_IKANA_CASTLE + SEQ_FLAG_ASYNC);
    }
}

void func_80ACAE5C(BgOpenShutter* this, PlayState* play);
s8 func_80ACABA8(BgOpenShutter* this, PlayState* play);

// allow opening door to be openable
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
        if (doorDirection != 0) {
            player->doorType = PLAYER_DOORTYPE_SLIDING;
            player->doorDirection = doorDirection;
            player->doorActor = &this->slidingDoor.dyna.actor;
            func_80122F28(player);
        }
    }
}