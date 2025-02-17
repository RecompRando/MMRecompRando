#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct BgIngate;

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

#define THIS ((BgIngate*)thisx)

typedef void (*BgIngateActionFunc)(struct BgIngate*, PlayState*);

#define BGINGATE_GET_PATH_INDEX(thisx) ((thisx)->params & 0xFF)

typedef struct BgIngate {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ BgIngateActionFunc actionFunc;
    /* 0x160 */ u16 unk160;
    /* 0x164 */ Path* timePath;
    /* 0x168 */ s16 timePathTimeSpeed;
    /* 0x16A */ s16 unk16A;
    /* 0x16C */ s16 unk16C;
    /* 0x16E */ s16 csId;
    /* 0x170 */ Vec3f timePathTargetPos;
    /* 0x17C */ f32 timePathProgress;
    /* 0x180 */ s32 timePathTotalTime;
    /* 0x184 */ s32 timePathWaypointTime;
    /* 0x188 */ s32 timePathWaypoint;
    /* 0x18C */ s32 timePathElapsedTime;
} BgIngate; // size = 0x190

void func_809543D4(BgIngate *this, PlayState *play);
void func_809542A0(BgIngate *this, PlayState *play);
void func_80953E38(PlayState *play);
s32 func_80953BEC(BgIngate *this);

RECOMP_PATCH void BgIngate_Update(Actor* thisx, PlayState* play) {
    BgIngate* this = THIS;

    if (this->timePathElapsedTime > 30 && !CHECK_EVENTINF(EVENTINF_35)) {
        this->timePathTotalTime = 4 * 100;
        this->timePathTimeSpeed = 4;
        this->timePathWaypointTime = this->timePathTotalTime / (this->timePath->count - (SUBS_TIME_PATHING_ORDER - 1));
    }

    this->actionFunc(this, play);
}

RECOMP_PATCH void func_80953B40(BgIngate* this) {
    s32 temp;

    if (!CHECK_EVENTINF(EVENTINF_35)) {
        this->timePathTotalTime = 4 * 1000;
        this->timePathTimeSpeed = 4;
    } else {
        this->timePathTotalTime = 1 * 2000;
        this->timePathTimeSpeed = 1;
    }
    temp = this->timePath->count - (SUBS_TIME_PATHING_ORDER - 1);
    this->timePathWaypointTime = this->timePathTotalTime / temp;
    this->timePathWaypoint = SUBS_TIME_PATHING_ORDER - 1;
    this->timePathElapsedTime = 0;
    this->unk160 &= ~0x1;
    this->unk160 &= ~0x2;
}

RECOMP_PATCH void func_80953F9C(BgIngate* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    Camera* mainCam = Play_GetCamera(play, CAM_ID_MAIN);

    if (!CHECK_EVENTINF(EVENTINF_40)) {

        if (!CHECK_EVENTINF(EVENTINF_35) && (this->unk160 & 0x10) && (this->unk16C == 0)) {
            this->dyna.actor.textId = 0x9E3;
            Message_StartTextbox(play, this->dyna.actor.textId, NULL);
            this->unk160 &= ~0x10;
        }

        if (this->unk160 & 2 || (CHECK_EVENTINF(EVENTINF_35) && (gSaveContext.minigameScore >= 20))) {

            if (this->timePath->additionalPathIndex != ADDITIONAL_PATH_INDEX_NONE) {
                func_80953E38(play);
                Player_SetCsActionWithHaltedActors(play, &this->dyna.actor, PLAYER_CSACTION_WAIT);
                this->dyna.actor.textId = 0x9E4;
                Message_StartTextbox(play, this->dyna.actor.textId, NULL);
                this->unk16C += 1;
                SET_WEEKEVENTREG(WEEKEVENTREG_90_40);
                this->actionFunc = func_809543D4;
            } else {
                if (!CHECK_EVENTINF(EVENTINF_35)) {
                    CLEAR_EVENTINF(EVENTINF_41);
                } else {
                    SET_EVENTINF(EVENTINF_40);
                }
                this->actionFunc = func_809542A0;
            }
        } else if ((CutsceneManager_GetCurrentCsId() == CS_ID_NONE) && (this->timePath != NULL)) {
            Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_CRUISER - SFX_FLAG);
            func_80953BEC(this);
        }
    }
    if (CutsceneManager_GetCurrentCsId() != this->csId) {
        if (CutsceneManager_GetCurrentCsId() != CS_ID_NONE) {
            Camera_ChangeSetting(mainCam, CAM_SET_NORMAL0);
            player->stateFlags1 |= PLAYER_STATE1_20;
            play->actorCtx.flags &= ~ACTORCTX_FLAG_PICTO_BOX_ON;
        } else {
            Camera_ChangeSetting(mainCam, CAM_SET_BOAT_CRUISE);
            player->stateFlags1 &= ~PLAYER_STATE1_20;
        }
    }
    this->csId = CutsceneManager_GetCurrentCsId();
}