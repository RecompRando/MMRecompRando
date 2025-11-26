#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define CREMIA_LIMB_MAX 0x16

struct EnMaYto;

typedef void (*EnMaYtoActionFunc)(struct EnMaYto*, PlayState*);

typedef struct EnMaYto {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnMaYtoActionFunc actionFunc;
    /* 0x18C */ ColliderCylinder collider;
    /* 0x1D8 */ NpcInteractInfo interactInfo;
    /* 0x200 */ s32 unk200; // unused
    /* 0x204 */ s32 type;
    /* 0x208 */ Vec3s jointTable[CREMIA_LIMB_MAX];
    /* 0x28C */ Vec3s morphTable[CREMIA_LIMB_MAX];
    /* 0x310 */ s16 unk310; // state?
    /* 0x312 */ UNK_TYPE1 unk_312[0x2];
    /* 0x314 */ s16 animIndex; // Used only in DefaultWait
    /* 0x316 */ s16 overrideEyeTexIndex; // If non-zero, then this index will be used instead of eyeTexIndex
    /* 0x318 */ s16 mouthTexIndex;
    /* 0x31A */ s16 eyeTexIndex;
    /* 0x31C */ s16 blinkTimer;
    /* 0x31E */ s16 unk31E;
    /* 0x320 */ s16 unk320;
    /* 0x322 */ u16 textId;
} EnMaYto; // size = 0x324

typedef enum {
    /* 0 */ MA_YTO_TYPE_DEFAULT, // Strokes cow on day 1
    /* 1 */ MA_YTO_TYPE_BARN,
    /* 2 */ MA_YTO_TYPE_DINNER,
    /* 3 */ MA_YTO_TYPE_AFTERMILKRUN, // After Milk Run minigame
    /* 4 */ MA_YTO_TYPE_4  // HugCutscene? Doesn't seem to work properly in-game
} EnMaYtoType;


void EnMaYto_SetupDefaultWait(EnMaYto *this);
void EnMaYto_SetupDinnerWait(EnMaYto *this);
void EnMaYto_SetupBarnWait(EnMaYto *this);
void EnMaYto_SetupAfterMilkRunInit(EnMaYto *this);
void EnMaYto_SetupBeginWarmFuzzyFeelingCs(EnMaYto *this);
void EnMaYto_SetupWarmFuzzyFeelingCs(EnMaYto *this);

// @ap cremia always hugs after escort
void EnMaYto_PostMilkRunGiveRandoReward(EnMaYto* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
		EnMaYto_SetupBeginWarmFuzzyFeelingCs(this);
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_MASK_ROMANI, 500.0f, 100.0f);
        this->unk310 = 1;
    }
}

RECOMP_PATCH void EnMaYto_ChooseAction(EnMaYto* this, PlayState* play) {
    switch (this->type) {
        case MA_YTO_TYPE_DEFAULT:
            EnMaYto_SetupDefaultWait(this);
            break;

        case MA_YTO_TYPE_DINNER:
            this->actor.targetMode = TARGET_MODE_6;
            EnMaYto_SetupDinnerWait(this);
            break;

        case MA_YTO_TYPE_BARN:
            EnMaYto_SetupBarnWait(this);
            break;

        case MA_YTO_TYPE_AFTERMILKRUN:
            this->unk310 = 0;
            // if ((INV_CONTENT(ITEM_MASK_ROMANI) == ITEM_MASK_ROMANI) &&
            //     CHECK_WEEKEVENTREG(WEEKEVENTREG_ESCORTED_CREMIA) && (Rand_Next() & 0x80)) {
            //     EnMaYto_SetupBeginWarmFuzzyFeelingCs(this);
            // } else {
            //     EnMaYto_SetupAfterMilkRunInit(this);
            // }
            // EnMaYto_PostMilkRunGiveRandoReward(this, play);
            this->actionFunc = EnMaYto_PostMilkRunGiveRandoReward;
            break;

        case MA_YTO_TYPE_4:
            this->actor.flags |= ACTOR_FLAG_10;
            EnMaYto_SetupWarmFuzzyFeelingCs(this);
            break;

        default:
            EnMaYto_SetupDefaultWait(this);
            break;
    }
}