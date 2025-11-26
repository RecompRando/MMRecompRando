#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define YB_LIMB_MAX 0x16

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_10 | ACTOR_FLAG_2000000)

#define THIS ((EnYb*)thisx)

struct EnYb;

typedef void (*EnYbActionFunc)(struct EnYb*, PlayState*);

typedef struct EnYb {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[YB_LIMB_MAX];
    /* 0x20C */ UNK_TYPE1 unkPadding20C[0x94];
    /* 0x2A0 */ Vec3s morphTable[YB_LIMB_MAX];
    /* 0x324 */ UNK_TYPE1 unkPadding324[0x94];
    /* 0x3B8 */ ColliderCylinder collider;
    /* 0x404 */ Vec3f shadowPos;
    /* 0x410 */ u16 playerOcarinaOut;
    /* 0x412 */ s16 animIndex;
    /* 0x414 */ s16 alpha;
    /* 0x416 */ s16 csIdList[2];
    /* 0x41A */ s16 csIdIndex;
    /* 0x41C */ s16 teachingCutsceneTimer;
    /* 0x420 */ EnYbActionFunc actionFunc;
} EnYb; // size = 0x424

void EnYb_Idle(EnYb* this, PlayState* play);
void EnYb_TeachingDanceFinish(EnYb* this, PlayState* play);
void EnYb_SetupLeaving(EnYb* this, PlayState* play);

void EnYb_UpdateAnimation(EnYb* this, PlayState* play);
void EnYb_FinishTeachingCutscene(EnYb* this);
void EnYb_Disappear(EnYb* this, PlayState* play);
void EnYb_ReceiveMask(EnYb* this, PlayState* play);
void EnYb_Talk(EnYb* this, PlayState* play);
void EnYb_TeachingDance(EnYb* this, PlayState* play);
void EnYb_WaitForMidnight(EnYb* this, PlayState* play);

void EnYb_ActorShadowFunc(Actor* thisx, Lights* mapper, PlayState* play);
void EnYb_ChangeAnim(PlayState* play, EnYb* this, s16 animIndex, u8 animMode, f32 morphFrames);
s32 EnYb_CanTalk(EnYb* this, PlayState* play);

void func_80BFA2FC(PlayState* play);
void EnYb_EnableProximityMusic(EnYb* this);

RECOMP_PATCH void EnYb_Talk(EnYb* this, PlayState* play) {
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 2, 0x1000, 0x200);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    EnYb_UpdateAnimation(this, play);

    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_5) && Message_ShouldAdvance(play)) {
        switch (play->msgCtx.currentTextId) {
            case 0x147D: // I am counting on you
                Message_CloseTextbox(play);
                this->actionFunc = EnYb_Disappear;
                SET_WEEKEVENTREG(WEEKEVENTREG_82_04);
                break;

            case 0x147C: // Spread my dance across the world
                /*if (Player_GetMask(play) == PLAYER_MASK_KAMARO) {
                    Message_CloseTextbox(play);
                    this->actionFunc = EnYb_Idle;

                } else if (INV_CONTENT(ITEM_MASK_KAMARO) == ITEM_MASK_KAMARO) {
                } else */if (rando_location_is_checked(0x000089)) {
                    Message_ContinueTextbox(play, 0x147D); // I am counting on you
                    func_80BFA2FC(play);

                } else {
                    Message_CloseTextbox(play);
                    this->actionFunc = EnYb_ReceiveMask;
                    EnYb_ReceiveMask(this, play);
                }
                break;

            default:
                Message_CloseTextbox(play);
                this->actionFunc = EnYb_Idle;
                break;
        }
    }
    EnYb_EnableProximityMusic(this);
}