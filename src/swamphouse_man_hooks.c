#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define STH_LIMB_MAX 0x10

struct EnSth;

typedef void (*EnSthActionFunc)(struct EnSth*, PlayState*);

typedef enum {
    /* 1 */ STH_TYPE_UNUSED_1 = 1,
    /* 2 */ STH_TYPE_SWAMP_SPIDER_HOUSE_CURED, // cursed is EnSsh
    /* 3 */ STH_TYPE_MOON_LOOKING, // South Clock Town, looking up at the moon
    /* 4 */ STH_TYPE_OCEANSIDE_SPIDER_HOUSE_GREET, // looking for shelter
    /* 5 */ STH_TYPE_OCEANSIDE_SPIDER_HOUSE_PANIC // shelter was not enough
    // Other values: Actor will spawn and animate with arm waving, no further interaction.
} EnSthType;

// Note: Vanilla types usually have 0xFEXX typing, but this upper section is unused by the code, reason unknown
#define STH_GET_TYPE(thisx) ((thisx)->params & 0xF)
#define STH_GET_SWAMP_BODY(thisx) ((thisx)->params & 0x100)

// The get item ID for the reward for Oceanside Spider House (wallet, or rupees) is set here
#define STH_GI_ID(thisx) ((thisx)->home.rot.z)

// This actor has its own flags system
#define STH_FLAG_DRAW_MASK_OF_TRUTH             (1 << 0)
#define STH_FLAG_OCEANSIDE_SPIDER_HOUSE_GREET   (1 << 1)
#define STH_FLAG_SWAMP_SPIDER_HOUSE_SAVED       (1 << 2) // set, but not read 
#define STH_FLAG_DISABLE_HEAD_TRACK             (1 << 3)

typedef struct EnSth {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ Vec3s jointTable[STH_LIMB_MAX];
    /* 0x234 */ Vec3s morphTable[STH_LIMB_MAX];
    /* 0x294 */ Vec3s headRot;
    /* 0x29A */ s16 animIndex;
    /* 0x29C */ u16 sthFlags;
    /* 0x29E */ u8 mainObjectSlot;
    /* 0x29F */ u8 maskOfTruthObjectSlot;
    /* 0x2A0 */ EnSthActionFunc actionFunc;
} EnSth; // size = 0x2A4

typedef enum EnSthAnimation {
    /* -1 */ STH_ANIM_NONE = -1,
    /*  0 */ STH_ANIM_SIGNALLING,   // default, waving arms at you from telescope, OOT: cured happy animation
    /*  1 */ STH_ANIM_BENDING_DOWN, // default anim of cured spider house, but never seen before wait overrides it
    /*  2 */ STH_ANIM_TALK,
    /*  3 */ STH_ANIM_WAIT,
    /*  4 */ STH_ANIM_LOOK_UP,     // South Clock Town, looking at moon
    /*  5 */ STH_ANIM_LOOK_AROUND, // checking out Oceanside Spider House
    /*  6 */ STH_ANIM_PLEAD,       // wants to buy Oceanside Spider House
    /*  7 */ STH_ANIM_PANIC,       // after buying Oceanside Spider House, can be found at bottom of slide,
    /*  8 */ STH_ANIM_MAX          // set in init, not an actual index to the array
} EnSthAnimation;

void EnSth_ChangeAnim(EnSth* this, s16 animIndex);

RECOMP_PATCH void EnSth_GetInitialSwampSpiderHouseText(EnSth* this, PlayState* play) {
    u16 nextTextId;

    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_34_10)) {
        nextTextId = 0x903; // (does not exist)
        EnSth_ChangeAnim(this, STH_ANIM_TALK);
    } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_34_20)) {
        nextTextId = 0x90F; // (does not exist)
        EnSth_ChangeAnim(this, STH_ANIM_TALK);
    } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_MASK_OF_TRUTH)) {
        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_TALKED_SWAMP_SPIDER_HOUSE_MAN)) {
            nextTextId = 0x91B; // As soon as I calm down, getting rid of it
        } else {
            nextTextId = 0x918; // I've had enough of this, going home
        }
        EnSth_ChangeAnim(this, STH_ANIM_TALK);
    } else if (Inventory_GetSkullTokenCount(0x27) >= 30) {
        if (rando_location_is_checked(GI_MASK_TRUTH)) {
            this->sthFlags |= STH_FLAG_SWAMP_SPIDER_HOUSE_SAVED;
            nextTextId = 0x919; // I've been saved!
        } else {
            nextTextId = 0x916; // I've been saved! (Duplicate)
        }
    } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_34_02)) {
        nextTextId = 0x8FF; // (does not exist)
    } else {
        nextTextId = 0x8FC; // (does not exist)
        SET_WEEKEVENTREG(WEEKEVENTREG_34_02);
    }

    Message_StartTextbox(play, nextTextId, &this->actor);
}

void EnSth_TalkAfterSwampSpiderHouseGiveMask(EnSth* this, PlayState* play);

RECOMP_PATCH void EnSth_SwampSpiderHouseGiveMask(EnSth* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnSth_TalkAfterSwampSpiderHouseGiveMask;
        this->actor.flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
    } else {
        this->sthFlags &= ~STH_FLAG_DRAW_MASK_OF_TRUTH;
        // This flag is used to keep track if the player has already spoken to the actor, triggering secondary dialogue.
        SET_WEEKEVENTREG(WEEKEVENTREG_TALKED_SWAMP_SPIDER_HOUSE_MAN);
        Actor_OfferGetItem(&this->actor, play, GI_MASK_TRUTH, 10000.0f, 50.0f);
    }
}

void EnSth_PostOceanspiderhouseReward(EnSth* this, PlayState* play);

RECOMP_PATCH void EnSth_GiveOceansideSpiderHouseReward(EnSth* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnSth_PostOceanspiderhouseReward;
        this->actor.flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
        if (CURRENT_DAY == 3) {
            EnSth_ChangeAnim(this, STH_ANIM_PLEAD);
        } else {
            EnSth_ChangeAnim(this, STH_ANIM_WAIT);
        }
    } else {
        // Actor_OfferGetItem(&this->actor, play, STH_GI_ID(&this->actor), 10000.0f, 500.0f);
        if (rando_location_is_checked(GI_WALLET_GIANT)) {
            Actor_OfferGetItem(&this->actor, play, GI_RUPEE_SILVER, 10000.0f, 500.0f);
        } else {
            Actor_OfferGetItem(&this->actor, play, GI_WALLET_GIANT, 10000.0f, 500.0f);
        }
    }
}