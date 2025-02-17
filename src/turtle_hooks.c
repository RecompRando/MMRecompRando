#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct DmChar08;

typedef void (*DmChar08ActionFunc)(struct DmChar08*, PlayState*);

typedef struct DmChar08 {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ SkelAnime skelAnime;
    /* 0x1A0 */ DmChar08ActionFunc actionFunc;
    /* 0x1A4 */ Actor* palmTree1;
    /* 0x1A8 */ Actor* palmTree2;
    /* 0x1AC */ Vec3f tree1Pos;
    /* 0x1B8 */ Vec3f tree2Pos;
    /* 0x1C4 */ Vec3f bubblePos;
    /* 0x1D0 */ Vec3f focusPos;
    /* 0x1DC */ UNK_TYPE1 unk_1DC[8];
    /* 0x1E4 */ f32 targetYPos;
    /* 0x1E8 */ UNK_TYPE1 unk_1E8[8];
    /* 0x1F0 */ f32 unk_1F0;
    /* 0x1F4 */ s16 unk_1F4;
    /* 0x1F6 */ s16 cueId;
    /* 0x1F6 */ s16 blinkTimer;
    /* 0x1FA */ s16 unk_1FA;
    /* 0x1FC */ u16 unk_1FC;
    /* 0x1FE */ u8 bubbleCount;
    /* 0x1FF */ u8 unk_1FF;
    /* 0x200 */ u8 eyeMode;
    /* 0x201 */ u8 eyeIndex;
    /* 0x202 */ u8 animIndex;
    /* 0x203 */ u8 prevAnimIndex;
    /* 0x204 */ UNK_TYPE1 unk_204;
    /* 0x205 */ u8 alpha;
    /* 0x206 */ u8 unk_206;
    /* 0x207 */ u8 unk_207;
    /* 0x208 */ u8 unk_208;
    /* 0x209 */ u8 dynapolyInitialized;
} DmChar08; // size = 0x20C

#define FLAGS (ACTOR_FLAG_UPDATE_DURING_OCARINA)

#define THIS ((DmChar08*)thisx)

void DmChar08_Init(Actor* thisx, PlayState* play2);
void DmChar08_Destroy(Actor* thisx, PlayState* play);
void DmChar08_Update(Actor* thisx, PlayState* play);
void DmChar08_Draw(Actor* thisx, PlayState* play);

void func_80AAFAC4(DmChar08* this, PlayState* play);
void DmChar08_WaitForSong(DmChar08* this, PlayState* play);
void func_80AAF8F4(DmChar08* this, PlayState* play);
void func_80AAFAE4(DmChar08* this, PlayState* play);
void DmChar08_DoNothing(DmChar08* this, PlayState* play);
void func_80AAFA18(DmChar08* this, PlayState* play);
void DmChar08_SetupAppearCs(DmChar08* this, PlayState* play);
void func_80AAF884(DmChar08* this, PlayState* play);
void func_80AAFB04(DmChar08* this, PlayState* play);
void func_80AAFB94(DmChar08* this, PlayState* play);
void DmChar08_ChangeAnim(SkelAnime* skelAnime, AnimationInfo* animInfo, u16 animIndex);

typedef enum {
    /* 0 */ TURTLE_EYEMODE_BLINK_LEFT,
    /* 1 */ TURTLE_EYEMODE_BLINK_STRAIGHT,
    /* 2 */ TURTLE_EYEMODE_CLOSED,
    /* 3 */ TURTLE_EYEMODE_LOOK_STRAIGHT,
    /* 4 */ TURTLE_EYEMODE_UNUSED,
    /* 5 */ TURTLE_EYEMODE_LOOK_RIGHT
} TurtleEyeMode;

extern ActorProfile Dm_Char08_Profile;

typedef enum {
    /*  0 */ TURTLE_ANIM_IDLE,
    /*  1 */ TURTLE_ANIM_SWIM,
    /*  2 */ TURTLE_ANIM_FLOAT,
    /*  3 */ TURTLE_ANIM_SPEAK1,
    /*  4 */ TURTLE_ANIM_COUGH,
    /*  5 */ TURTLE_ANIM_SPEAK2,
    /*  6 */ TURTLE_ANIM_YAWN,
    /*  7 */ TURTLE_ANIM_MAX,
    /* 99 */ TURTLE_ANIM_NONE = 99
} TurtleAnimation;

extern AnimationHeader gTurtleIdleAnim;
extern AnimationHeader gTurtleSwimAnim;
extern AnimationHeader gTurtleFloatAnim;
extern AnimationHeader gTurtleSpeak1Anim;
extern AnimationHeader gTurtleCoughAnim;
extern AnimationHeader gTurtleSpeak2Anim;
extern AnimationHeader gTurtleYawnAnim;
extern FlexSkeletonHeader gTurtleSkel;
extern CollisionHeader gTurtleZoraCapeAwakeCol;
extern CollisionHeader gTurtleZoraCapeAsleepCol;
extern CollisionHeader sTurtleGreatBayTempleCol;

static AnimationInfo sAnimationInfo[TURTLE_ANIM_MAX] = {
    { &gTurtleIdleAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -24.0f },   // TURTLE_ANIM_IDLE
    { &gTurtleSwimAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -24.0f },   // TURTLE_ANIM_SWIM
    { &gTurtleFloatAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -24.0f },  // TURTLE_ANIM_FLOAT
    { &gTurtleSpeak1Anim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -24.0f }, // TURTLE_ANIM_SPEAK1
    { &gTurtleCoughAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -24.0f },  // TURTLE_ANIM_COUGH
    { &gTurtleSpeak2Anim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -24.0f }, // TURTLE_ANIM_SPEAK2
    { &gTurtleYawnAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, -24.0f },   // TURTLE_ANIM_YAWN
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(cullingVolumeDistance, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeScale, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 4000, ICHAIN_STOP),
};

// TODO: skip the spawning cutscene
RECOMP_PATCH void DmChar08_SetupAppearCs(DmChar08* this, PlayState* play) {
    s16 csId = this->dyna.actor.csId;
    s16 additionalCsId =
        CutsceneManager_GetAdditionalCsId(CutsceneManager_GetAdditionalCsId(CutsceneManager_GetAdditionalCsId(csId)));

    // if (CHECK_WEEKEVENTREG(WEEKEVENTREG_93_08)) {
    //     csId = additionalCsId;
    // }
    csId = additionalCsId;

    if (CutsceneManager_IsNext(csId)) {
        CutsceneManager_Start(csId, &this->dyna.actor);
        SET_WEEKEVENTREG(WEEKEVENTREG_53_20);
        SET_WEEKEVENTREG(WEEKEVENTREG_93_08);
        DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, this->dyna.bgId);
        this->actionFunc = func_80AAF884;
    } else {
        CutsceneManager_Queue(csId);
    }
}

RECOMP_PATCH void func_80AAF8F4(DmChar08* this, PlayState* play) {
    s32 pad;
    f32 yOffset;

    if (fabsf(this->dyna.actor.xzDistToPlayer) < 200.0f) {
        Audio_PlaySfx(NA_SE_OC_SECRET_WARP_OUT);
        this->actionFunc = func_80AAFA18;
    }
    this->unk_1FA += 200;
    yOffset = 50.0f;
    yOffset *= Math_SinS(this->unk_1FA);
    Math_SmoothStepToF(&this->dyna.actor.world.pos.x, -6400.0f, 0.2f, 2.0f, 0.1f);
    Math_SmoothStepToF(&this->targetYPos, -180.0f + yOffset, 0.5f, 5.0f, 0.1f);
    Math_SmoothStepToF(&this->dyna.actor.world.pos.z, 1750.0f, 0.5f, 20.0f, 0.1f);
    Math_SmoothStepToS(&this->dyna.actor.world.rot.y, -0x7234, 0xA, 0xDC, 1);
    Math_SmoothStepToS(&this->dyna.actor.shape.rot.y, -0x7234, 0xA, 0xDC, 1);
}

RECOMP_PATCH void func_80AAFA18(DmChar08* this, PlayState* play) {
    play->nextEntrance = ENTRANCE(GREAT_BAY_TEMPLE, 0);
    play->transitionType = TRANS_TYPE_FADE_BLUE;
    gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLUE;
    play->transitionTrigger = TRANS_TRIGGER_START;
}