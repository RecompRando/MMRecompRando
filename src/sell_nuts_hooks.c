#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnSellnuts;

#define FLAGS                                                                                  \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_DRAW_CULLING_DISABLED)

#define THIS ((EnSellnuts*)thisx)

#define ENSELLNUTS_GET_1(thisx) ((thisx)->params & 1)
#define ENSELLNUTS_GET_PATH_INDEX(thisx) (((thisx)->params & 0xFC00) >> 0xA)

#define ENSELLNUTS_PATH_INDEX_NONE 0x3F

typedef void (*EnSellnutsActionFunc)(struct EnSellnuts*, PlayState*);

typedef struct EnSellnuts {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[28];
    /* 0x230 */ Vec3s morphTable[28];
    /* 0x2D8 */ EnSellnutsActionFunc actionFunc;
    /* 0x2DC */ ColliderCylinder collider;
    /* 0x328 */ s16 unk_328;
    /* 0x32C */ f32 unk_32C;
    /* 0x330 */ Path* path;
    /* 0x334 */ s32 unk_334;
    /* 0x338 */ u16 unk_338;
    /* 0x33A */ u16 unk_33A;
    /* 0x33C */ s16 csId;
    /* 0x33E */ UNK_TYPE1 unk33E[0x2];
    /* 0x340 */ u16 unk_340;
    /* 0x342 */ u16 unk_342;
    /* 0x344 */ s16 unk_344;
    /* 0x346 */ UNK_TYPE1 unk346[0x4];
    /* 0x34A */ s16 unk_34A;
    /* 0x34C */ s16 unk_34C;
    /* 0x34E */ s16 unk_34E;
    /* 0x350 */ s16 unk_350;
    /* 0x354 */ f32 unk_354;
    /* 0x358 */ f32 unk_358;
    /* 0x35C */ f32 unk_35C;
    /* 0x360 */ f32 unk_360;
    /* 0x364 */ s16 unk_364;
    /* 0x366 */ s16 unk_366;
    /* 0x368 */ s16 unk_368;
    /* 0x36A */ UNK_TYPE1 unk36A[0x2];
    /* 0x36C */ f32 unk_36C;
    /* 0x370 */ f32 unk_370;
    /* 0x374 */ f32 unk_374;
} EnSellnuts; // size = 0x378

extern AnimationHeader gBusinessScrubStandingAnim;
extern AnimationHeader gBusinessScrubWalkAnim;
extern AnimationHeader gBusinessScrubRiseUpAnim;
extern AnimationHeader gBusinessScrubJumpAnim;
extern AnimationHeader gBusinessScrubBurrowAnim;
extern AnimationHeader gBusinessScrubExcitedStartAnim;
extern AnimationHeader gBusinessScrubExcitedLoopAnim;
extern AnimationHeader gBusinessScrubExcitedEndAnim;
extern AnimationHeader gBusinessScrubTakeOffHatAnim;
extern AnimationHeader gBusinessScrubFlyStartAnim;
extern AnimationHeader gBusinessScrubFlyLoopAnim;
extern AnimationHeader gBusinessScrubShockedStartAnim;
extern AnimationHeader gBusinessScrubShockedShakeHeadAnim;
extern AnimationHeader gBusinessScrubShockedPoundAnim;
extern AnimationHeader gBusinessScrubShockedEndAnim;
extern AnimationHeader gBusinessScrubThinkAnim;
extern AnimationHeader gBusinessScrubBobAnim;
extern AnimationHeader gBusinessScrubBurrowAnim;
extern AnimationHeader gBusinessScrubBurrowAnim;
extern AnimationHeader gBusinessScrubBurrowAnim;
extern AnimationHeader gBusinessScrubFlyLoopAnim;
extern AnimationHeader gBusinessScrubTakeOffHatAnim;
extern AnimationHeader gBusinessScrubFlyEndAnim;

static AnimationInfoS sAnimationInfo[] = {
    { &gBusinessScrubStandingAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gBusinessScrubWalkAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gBusinessScrubRiseUpAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubJumpAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubBurrowAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubExcitedStartAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubExcitedLoopAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gBusinessScrubExcitedEndAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gBusinessScrubTakeOffHatAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubFlyStartAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubFlyLoopAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gBusinessScrubShockedStartAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubShockedShakeHeadAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gBusinessScrubShockedPoundAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gBusinessScrubShockedEndAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubThinkAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gBusinessScrubBobAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },
    { &gBusinessScrubBurrowAnim, 1.0f, 8, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubBurrowAnim, 1.0f, 4, -1, ANIMMODE_ONCE, -4 },
    { &gBusinessScrubBurrowAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubFlyLoopAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },
    { &gBusinessScrubTakeOffHatAnim, -1.0f, 0, -1, ANIMMODE_ONCE, 0 },
    { &gBusinessScrubFlyEndAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },
};

static u16 D_80ADD930[] = { 0x0619, 0x0613, 0x0613 };

bool given = false;

void func_80ADAE64(EnSellnuts* this);
void func_80ADB0D8(EnSellnuts* this, PlayState* play);
void func_80ADBCE4(EnSellnuts* this, PlayState* play);
void func_80ADC2CC(EnSellnuts* this, PlayState* play);

RECOMP_PATCH void func_80ADBBEC(EnSellnuts* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_LAND_TITLE_DEED);
        this->actionFunc = func_80ADBCE4;
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_DEED_LAND, 300.0f, 300.0f);
    }
}

void func_80ADC580(EnSellnuts* this, PlayState* play);
s32 EnSellnuts_HasReachedPoint(EnSellnuts* this, Path* path, s32 pointIndex);
f32 func_80ADCFE8(Path* path, s32 arg1, Vec3f* pos, Vec3s* arg3);

RECOMP_PATCH void func_80ADC37C(EnSellnuts* this, PlayState* play) {
    Vec3s sp30;
    f32 sp2C;

    this->actor.velocity.y = 0.0f;
    this->actor.gravity = 0.0f;
    if (this->path != NULL) {
        sp2C = func_80ADCFE8(this->path, this->unk_334, &this->actor.world.pos, &sp30);
        if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
            sp30.y = this->actor.wallYaw;
        }
        Math_SmoothStepToS(&this->actor.world.rot.y, sp30.y, 0xA, 0x12C, 0);
        this->actor.shape.rot.y = this->actor.world.rot.y;
        this->unk_342 = 0x1000;
        this->unk_344 += this->unk_364;
        this->actor.world.rot.x = -sp30.x;
        if (EnSellnuts_HasReachedPoint(this, this->path, this->unk_334) && (sp2C < 500.0f)) {
            if (this->unk_334 >= (this->path->count - 1)) {
                CutsceneManager_Stop(this->csId);
                this->actionFunc = func_80ADC580;
            } else {
                this->unk_334++;
            }
        }
    } else if (this->actor.playerHeightRel > 500.0f) {
        CutsceneManager_Stop(this->csId);
        this->actionFunc = func_80ADC580;
    }

    Math_ApproachF(&this->actor.speed, 2.0f, 0.2f, 1.0f);
    Actor_MoveWithoutGravity(&this->actor);
    if (this->unk_366 == 2) {
        if (CutsceneManager_IsNext(this->csId)) {
            Player_SetCsActionWithHaltedActors(play, NULL, PLAYER_CSACTION_END);
            this->unk_366 = 3;
        } else {
            CutsceneManager_Queue(this->csId);
            this->unk_366 = 3;
            return;
        }
    } else if ((this->unk_366 == 1) && (this->unk_368 == 20)) {
        CutsceneManager_Stop(this->csId);
        this->csId = CutsceneManager_GetAdditionalCsId(this->csId);
        CutsceneManager_Queue(this->csId);
        this->unk_366 = 2;
    }

    this->unk_368++;
}

void func_80ADC8C4(EnSellnuts* this, PlayState* play);

RECOMP_PATCH void func_80ADC7B4(EnSellnuts* this, PlayState* play) {
    s32 talkState = Message_GetState(&play->msgCtx);

    if (this->unk_366 == 0) {
        if (CutsceneManager_IsNext(this->csId)) {
            Player_SetCsActionWithHaltedActors(play, NULL, PLAYER_CSACTION_END);
            this->unk_366 = 1;
        } else {
            if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
                CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
            }
            CutsceneManager_Queue(this->csId);
        }
    } else if ((this->unk_366 == 1) && (talkState == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        play->msgCtx.msgMode = MSGMODE_TEXT_CLOSING;
        play->msgCtx.stateTimer = 4;
        this->unk_366 = 0;
        CutsceneManager_Stop(this->csId);
        this->csId = CutsceneManager_GetAdditionalCsId(this->csId);
        CutsceneManager_Queue(this->csId);
        this->unk_338 |= 8;
        this->actionFunc = func_80ADC8C4;
    }
}

s32 func_808387A0(PlayState* play, Player* this);

RECOMP_PATCH void func_80ADBE80(EnSellnuts* this, PlayState* play) {
    s16 currentFrame = this->skelAnime.curFrame;
    s16 frameCount = Animation_GetLastFrame(sAnimationInfo[this->unk_34C].animation);
    Player* player = GET_PLAYER(play);

    func_80ADAE64(this);
    if (this->unk_366 == 0) {
        if (CutsceneManager_IsNext(this->csId)) {
            Player_SetCsActionWithHaltedActors(play, NULL, PLAYER_CSACTION_END);
            this->unk_366 = 1;
        } else {
            if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
                CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
            }
            CutsceneManager_Queue(this->csId);
            return;
        }
    }

    if (currentFrame == frameCount) {
        this->unk_350 = 4;
        this->unk_34C = 19;
        SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, 19);
        Actor_PlaySfx(&this->actor, NA_SE_EN_NUTS_DOWN);
        this->unk_338 &= ~1;
        this->unk_338 |= 8;
        this->unk_32C = this->actor.world.pos.y;
        this->actionFunc = func_80ADC2CC;
    }
}

void func_80ADCA64(EnSellnuts* this, PlayState* play);

RECOMP_PATCH void func_80ADC8C4(EnSellnuts* this, PlayState* play) {
    Vec3s sp30;

    if (this->unk_366 == 0) {
        if (CutsceneManager_IsNext(this->csId)) {
            Player_SetCsActionWithHaltedActors(play, NULL, PLAYER_CSACTION_END);
            this->unk_366 = 1;
        } else {
            if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
                CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
            }
            CutsceneManager_Queue(this->csId);
            return;
        }
    }

    if (this->path != NULL) {
        func_80ADCFE8(this->path, this->unk_334, &this->actor.world.pos, &sp30);
        if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
            sp30.y = this->actor.wallYaw;
        }
        Math_SmoothStepToS(&this->actor.world.rot.y, sp30.y, 0xA, 0x12C, 0);
        this->actor.shape.rot.y = this->actor.world.rot.y;
        this->unk_342 = 0x1000;
        this->unk_344 += 0x1C71;
        this->actor.world.rot.x = -sp30.x;
        if (EnSellnuts_HasReachedPoint(this, this->path, this->unk_334)) {
            if (this->unk_334 >= (this->path->count - 1)) {
                this->unk_34C = 22;
                this->actor.gravity = -1.0f;
                this->actor.velocity.y = -1.0f;
                this->actor.speed = 0.0f;
                SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, this->unk_34C);
                this->unk_338 &= ~1;
                this->unk_338 &= ~2;
                this->actionFunc = func_80ADCA64;
                return;
            }
            this->unk_334++;
        }
        Math_ApproachF(&this->actor.speed, 2.0f, 0.2f, 1.0f);
        Actor_MoveWithoutGravity(&this->actor);
    }
}