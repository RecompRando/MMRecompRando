#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define BUSINESS_SCRUB_LIMB_MAX 0x1C

struct EnAkindonuts;

typedef void (*EnAkindonutsActionFunc)(struct EnAkindonuts*, PlayState*);
typedef void (*EnAkindonutsUnkFunc)(struct EnAkindonuts*, PlayState*);

#define ENAKINDONUTS_GET_3(thisx) ((thisx)->params & 3)
#define ENAKINDONUTS_GET_4(thisx) (((thisx)->params & 4) >> 2)
#define ENAKINDONUTS_GET_PATH_INDEX(thisx) (((thisx)->params & 0xFC00) >> 0xA)

#define ENAKINDONUTS_PATH_INDEX_NONE 0x3F

typedef enum {
    /* 1 */ ENAKINDONUTS_3_1 = 1,
    /* 2 */ ENAKINDONUTS_3_2
} EnAkindonutsParam;

typedef struct EnAkindonuts {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[BUSINESS_SCRUB_LIMB_MAX];
    /* 0x230 */ Vec3s morphTable[BUSINESS_SCRUB_LIMB_MAX];
    /* 0x2D8 */ EnAkindonutsActionFunc actionFunc;
    /* 0x2DC */ EnAkindonutsUnkFunc unk_2DC;
    /* 0x2E0 */ ColliderCylinder collider;
    /* 0x32C */ u16 unk_32C;
    /* 0x330 */ Path* path;
    /* 0x334 */ s32 unk_334;
    /* 0x338 */ s16 animIndex;
    /* 0x33A */ s16 unk_33A;
    /* 0x33C */ u16 unk_33C;
    /* 0x33E */ s16 unk_33E;
    /* 0x340 */ f32 unk_340;
    /* 0x344 */ f32 unk_344;
    /* 0x348 */ f32 unk_348;
    /* 0x34C */ f32 unk_34C;
    /* 0x350 */ s16 unk_350;
    /* 0x352 */ s16 unk_352;
    /* 0x354 */ UNK_TYPE1 unk354[0x2];
    /* 0x356 */ s16 unk_356;
    /* 0x358 */ f32 unk_358;
    /* 0x35C */ s16 unk_35C;
    /* 0x35E */ s16 unk_35E;
    /* 0x360 */ s16 csId;
    /* 0x362 */ s16 unk_362;
    /* 0x364 */ s16 unk_364;
    /* 0x366 */ s8 unk_366;
    /* 0x368 */ s16 unk_368;
} EnAkindonuts; // size = 0x36C

typedef enum {
    /*   -1 */ ENAKINDONUTS_ANIM_NONE = -1,
    /* 0x00 */ ENAKINDONUTS_ANIM_0,
    /* 0x01 */ ENAKINDONUTS_ANIM_1,
    /* 0x02 */ ENAKINDONUTS_ANIM_2,
    /* 0x03 */ ENAKINDONUTS_ANIM_3,
    /* 0x04 */ ENAKINDONUTS_ANIM_4,
    /* 0x05 */ ENAKINDONUTS_ANIM_5,
    /* 0x06 */ ENAKINDONUTS_ANIM_6,
    /* 0x07 */ ENAKINDONUTS_ANIM_7,
    /* 0x08 */ ENAKINDONUTS_ANIM_8,
    /* 0x09 */ ENAKINDONUTS_ANIM_9,
    /* 0x0A */ ENAKINDONUTS_ANIM_10,
    /* 0x0B */ ENAKINDONUTS_ANIM_11,
    /* 0x0C */ ENAKINDONUTS_ANIM_12,
    /* 0x0D */ ENAKINDONUTS_ANIM_13,
    /* 0x0E */ ENAKINDONUTS_ANIM_14,
    /* 0x0F */ ENAKINDONUTS_ANIM_15,
    /* 0x10 */ ENAKINDONUTS_ANIM_16,
    /* 0x11 */ ENAKINDONUTS_ANIM_17,
    /* 0x12 */ ENAKINDONUTS_ANIM_18,
    /* 0x13 */ ENAKINDONUTS_ANIM_19,
    /* 0x14 */ ENAKINDONUTS_ANIM_20,
    /* 0x15 */ ENAKINDONUTS_ANIM_21,
    /* 0x16 */ ENAKINDONUTS_ANIM_22,
    /* 0x17 */ ENAKINDONUTS_ANIM_MAX
} EnAkindonutsAnimation;

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

static AnimationInfoS sAnimationInfo[ENAKINDONUTS_ANIM_MAX] = {
    { &gBusinessScrubStandingAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },         // ENAKINDONUTS_ANIM_0
    { &gBusinessScrubWalkAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },             // ENAKINDONUTS_ANIM_1
    { &gBusinessScrubRiseUpAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },           // ENAKINDONUTS_ANIM_2
    { &gBusinessScrubJumpAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },             // ENAKINDONUTS_ANIM_3
    { &gBusinessScrubBurrowAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },           // ENAKINDONUTS_ANIM_4
    { &gBusinessScrubExcitedStartAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },     // ENAKINDONUTS_ANIM_5
    { &gBusinessScrubExcitedLoopAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },      // ENAKINDONUTS_ANIM_6
    { &gBusinessScrubExcitedEndAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },       // ENAKINDONUTS_ANIM_7
    { &gBusinessScrubTakeOffHatAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },       // ENAKINDONUTS_ANIM_8
    { &gBusinessScrubFlyStartAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },         // ENAKINDONUTS_ANIM_9
    { &gBusinessScrubFlyLoopAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },          // ENAKINDONUTS_ANIM_10
    { &gBusinessScrubShockedStartAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },     // ENAKINDONUTS_ANIM_11
    { &gBusinessScrubShockedShakeHeadAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 }, // ENAKINDONUTS_ANIM_12
    { &gBusinessScrubShockedPoundAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },     // ENAKINDONUTS_ANIM_13
    { &gBusinessScrubShockedEndAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },       // ENAKINDONUTS_ANIM_14
    { &gBusinessScrubThinkAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },            // ENAKINDONUTS_ANIM_15
    { &gBusinessScrubBobAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -4 },             // ENAKINDONUTS_ANIM_16
    { &gBusinessScrubBurrowAnim, 1.0f, 8, -1, ANIMMODE_ONCE, 0 },           // ENAKINDONUTS_ANIM_17
    { &gBusinessScrubBurrowAnim, 1.0f, 4, -1, ANIMMODE_ONCE, -4 },          // ENAKINDONUTS_ANIM_18
    { &gBusinessScrubBurrowAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },           // ENAKINDONUTS_ANIM_19
    { &gBusinessScrubFlyLoopAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },          // ENAKINDONUTS_ANIM_20
    { &gBusinessScrubTakeOffHatAnim, -1.0f, 0, -1, ANIMMODE_ONCE, 0 },      // ENAKINDONUTS_ANIM_21
    { &gBusinessScrubFlyEndAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },           // ENAKINDONUTS_ANIM_22
};

void func_80BEF770(EnAkindonuts* this, PlayState* play);

RECOMP_PATCH void func_80BEF518(EnAkindonuts* this, PlayState* play) {
    s16 curFrame = this->skelAnime.curFrame;
    s16 endFrame = Animation_GetLastFrame(&sAnimationInfo[this->animIndex].animation->common);

    switch (curFrame) {
        case 10:
            this->unk_33E = 1;
            this->unk_340 = 0.1f;
            this->unk_344 = 0.1f;
            this->unk_348 = 0.1f;
            break;

        case 11:
        case 12:
            this->unk_340 += 0.15f;
            this->unk_344 += 0.32f;
            this->unk_348 += 0.15f;
            break;

        case 13:
            this->unk_340 = 0.55f;
            this->unk_344 = 1.05f;
            this->unk_348 = 0.55f;
            break;

        case 14:
            this->unk_340 = 1.0f;
            this->unk_344 = 2.0f;
            this->unk_348 = 1.0f;
            break;

        case 15:
        case 16:
            this->unk_344 -= 0.33f;
            break;

        case 17:
            this->unk_344 = 1.0f;
            break;

        case 18:
        case 19:
            this->unk_344 += 0.27f;
            break;
        case 20:
            this->unk_33E = 2;
            this->unk_344 = 1.8f;
            break;

        case 21:
        case 22:
        case 23:
            this->unk_344 -= 0.2f;
            break;

        case 24:
            this->unk_344 = 1.0f;
            break;

        default:
            break;
    }

    if (this->unk_35E == 0) {
        if (CutsceneManager_IsNext(this->csId)) {
            Player_SetCsActionWithHaltedActors(play, NULL, PLAYER_CSACTION_END);
            this->unk_35E = 1;
        } else {
            if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
                CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
            }
            CutsceneManager_Queue(this->csId);
            return;
        }
    }

    if (curFrame == endFrame) {
        this->unk_33E = 3;
        this->animIndex = ENAKINDONUTS_ANIM_19;
        SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, this->animIndex);
        Actor_PlaySfx(&this->actor, NA_SE_EN_NUTS_DOWN);
        this->unk_32C &= ~2;
        this->unk_32C |= 0x80;
        this->unk_358 = this->actor.world.pos.y;
        this->actionFunc = func_80BEF770;
    }
}

f32 func_80BECEAC(Path* path, s32 arg1, Vec3f* pos, Vec3s* arg3);
s32 EnAkindonuts_HasReachedPoint(EnAkindonuts* this, Path* path, s32 pointIndex);
void func_80BEFD74(EnAkindonuts* this, PlayState* play);
void func_80BECBE0(EnAkindonuts* this, s16 arg1);

RECOMP_PATCH void func_80BEFAF0(EnAkindonuts* this, PlayState* play) {
    Vec3s sp38;
    f32 sp34;
    s16 sp32 = false;

    this->actor.velocity.y = 0.0f;
    this->actor.gravity = 0.0f;

    if (this->path != NULL) {
        sp34 = func_80BECEAC(this->path, this->unk_334, &this->actor.world.pos, &sp38);
        if (this->actor.bgCheckFlags & BGCHECKFLAG_WALL) {
            sp38.y = this->actor.wallYaw;
        }

        if (ENAKINDONUTS_GET_3(&this->actor) == ENAKINDONUTS_3_1) {
            Math_SmoothStepToS(&this->actor.world.rot.y, sp38.y, 10, 1000, 0);
        } else {
            Math_SmoothStepToS(&this->actor.world.rot.y, sp38.y, 10, 300, 0);
        }

        this->actor.shape.rot.y = this->actor.world.rot.y;
        this->unk_350 = 0x1000;
        this->unk_352 += this->unk_362;
        this->actor.world.rot.x = -sp38.x;

        if (EnAkindonuts_HasReachedPoint(this, this->path, this->unk_334) && (sp34 < 10.0f)) {
            if (this->unk_334 >= (this->path->count - 1)) {
                CutsceneManager_Stop(this->csId);
                this->actionFunc = func_80BEFD74;
            } else {
                this->unk_334++;
            }
        }
    } else if (this->actor.playerHeightRel > 500.0f) {
        CutsceneManager_Stop(this->csId);
        this->actionFunc = func_80BEFD74;
    }

    if (ENAKINDONUTS_GET_3(&this->actor) == ENAKINDONUTS_3_2) {
        if (this->unk_334 >= 3) {
            sp32 = true;
        }
        Math_ApproachF(&this->actor.speed, 1.5f, 0.2f, 1.0f);
    } else {
        Math_ApproachF(&this->actor.speed, 2.0f, 0.2f, 1.0f);
    }

    func_80BECBE0(this, sp32);

    if (this->unk_35E == 2) {
        if (CutsceneManager_IsNext(this->csId)) {
            Player_SetCsActionWithHaltedActors(play, NULL, PLAYER_CSACTION_END);
            this->unk_35E = 3;
        } else {
            CutsceneManager_Queue(this->csId);
            return;
        }
    } else if ((this->unk_35E == 1) && (this->unk_356 == 20)) {
        CutsceneManager_Stop(this->csId);
        this->csId = CutsceneManager_GetAdditionalCsId(this->csId);
        CutsceneManager_Queue(this->csId);
        this->unk_35E = 2;
    }
    this->unk_356++;
}