#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct DmChar03;

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

#define THIS ((DmChar03*)thisx)

typedef enum {
    /* 0 */ DMCHAR03_ANIM_FALL_OVER,
    /* 1 */ DMCHAR03_ANIM_MAX
} DmChar03Animation;

typedef void (*DmChar03ActionFunc)(struct DmChar03*, PlayState*);

typedef struct DmChar03 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ DmChar03ActionFunc actionFunc;
    /* 0x18C */ s16 animIndex;
    /* 0x18E */ u8 unk_18E;
    /* 0x18F */ u8 objectSlot;
    /* 0x190 */ Vec3f offset;
} DmChar03; // size = 0x19C

extern AnimationHeader gDekuMaskFallOverAnim;
extern FlexSkeletonHeader gDekuMaskSkel;

void DmChar03_ChangeAnim(SkelAnime* skelAnime, AnimationInfo* animInfo, u16 animIndex);

void func_80AAB644(DmChar03* this, PlayState* play);
void DmChar03_DoNothing(DmChar03* this, PlayState* play);
void func_80AABA84(PlayState* play, DmChar03* this);

AnimationInfo sAnimationInfo[DMCHAR03_ANIM_MAX] = {
    { &gDekuMaskFallOverAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f }, // DMCHAR03_ANIM_FALL_OVER
};

RECOMP_PATCH void DmChar03_Init(Actor* thisx, PlayState* play) {
    DmChar03* this = THIS;

    //gSaveContext.save.playerForm = PLAYER_FORM_DEKU;

    this->animIndex = DMCHAR03_ANIM_FALL_OVER;
    this->actor.lockOnArrowOffset = 3000.0f;
    this->unk_18E = false;
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gDekuMaskSkel, NULL, NULL, NULL, 0);
    DmChar03_ChangeAnim(&this->skelAnime, &sAnimationInfo[DMCHAR03_ANIM_FALL_OVER], 0);
    Actor_SetScale(&this->actor, 0.01f);
    this->actionFunc = DmChar03_DoNothing;
}