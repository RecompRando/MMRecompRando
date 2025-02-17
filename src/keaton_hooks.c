#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define LOCATION_KEATON_QUIZ 0x07028C

#define KEATON_LIMB_MAX 0x15

#define ENKITAN_GET_COLLECT_FLAG(thisx) (((thisx)->params & 0xFE00) >> 9)

struct EnKitan;

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY)

#define THIS ((EnKitan*)thisx)

typedef void (*EnKitanActionFunc)(struct EnKitan*, PlayState*);

typedef struct EnKitan {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[KEATON_LIMB_MAX];
    /* 0x206 */ Vec3s morphTable[KEATON_LIMB_MAX];
    /* 0x284 */ ColliderCylinder collider;
    /* 0x2D0 */ s32 textBitSet;
    /* 0x2D4 */ UNK_TYPE1 unk2D4[2];
    /* 0x2D6 */ s16 timer;
    /* 0x2D8 */ EnKitanActionFunc actionFunc;
} EnKitan; // size = 0x2DC

void EnKitan_WaitForPrizeTextboxClosed(EnKitan* this, PlayState* play);

RECOMP_PATCH void EnKitan_OfferPrize(EnKitan* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnKitan_WaitForPrizeTextboxClosed;
        //SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KEATON_HEART_PIECE);
        return;
    }

    // Reward the player with a heart piece, or a red rupee if the heart piece was already obtained.
    //if (CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KEATON_HEART_PIECE)) {
    if (rando_location_is_checked(LOCATION_KEATON_QUIZ)) {
        Actor_OfferGetItem(&this->actor, play, GI_RUPEE_HUGE, 2000.0f, 1000.0f);
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_HEART_PIECE, 2000.0f, 1000.0f);
    }
}

RECOMP_PATCH void EnKitan_Destroy(Actor* thisx, PlayState* play) {
    EnKitan* this = THIS;
    s32 flag = ENKITAN_GET_COLLECT_FLAG(thisx);

    play->actorCtx.sceneFlags.collectible[(flag & ~0x1F) >> 5] &= ~(1 << (flag & 0x1F));
    Collider_DestroyCylinder(play, &this->collider);
}