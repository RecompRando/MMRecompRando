#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define LOCATION_STONE_SOLDIER GI_MASK_STONE

#define SOLDIER_LIMB_MAX 0x11

struct EnStoneheishi;

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_REACT_TO_LENS)

#define THIS ((EnStoneheishi*)thisx)

typedef void (*EnStoneheishiActionFunc)(struct EnStoneheishi*, PlayState*);

typedef struct EnStoneheishi {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[SOLDIER_LIMB_MAX];
    /* 0x1EE */ Vec3s morphTable[SOLDIER_LIMB_MAX];
    /* 0x254 */ EnStoneheishiActionFunc actionFunc;
    /* 0x258 */ Vec3s headRot;
    /* 0x25E */ Vec3s targetHeadRot;
    /* 0x264 */ s32 animIndex;
    /* 0x268 */ s16 timer;
    /* 0x26A */ s16 drinkBottleState;
    /* 0x26C */ f32 animEndFrame;
    /* 0x270 */ s16 action;
    /* 0x272 */ s16 textIdIndex;
    /* 0x274 */ s16 textIdSet;
    /* 0x276 */ u8 bottleDisplay;
    /* 0x277 */ u8 playerGivesBluePotion;
    /* 0x278 */ ColliderCylinder collider;
} EnStoneheishi; // size = 0x2C4

static u16 sEnStoneHeishiTextIds[] = { 0x1473, 0x1474, 0x1475, 0x1476, 0x1477, 0x1478, 0x1479, 0x147A, 0x1472 };

typedef enum EnStoneHeishiAnimation {
    /* 0 */ EN_STONE_HEISHI_ANIM_STAND_HAND_ON_HIP,
    /* 1 */ EN_STONE_HEISHI_ANIM_DRINK_1,
    /* 2 */ EN_STONE_HEISHI_ANIM_CHEER_WITH_SPEAR,
    /* 3 */ EN_STONE_HEISHI_ANIM_WAVE,
    /* 4 */ EN_STONE_HEISHI_ANIM_SIT_AND_REACH,
    /* 5 */ EN_STONE_HEISHI_ANIM_DRINK_2,
    /* 6 */ EN_STONE_HEISHI_ANIM_STAND_UP,
    /* 7 */ EN_STONE_HEISHI_ANIM_MAX
} EnStoneHeishiAnimation;

static u8 sAnimationModes[EN_STONE_HEISHI_ANIM_MAX] = {
    ANIMMODE_LOOP, // EN_STONE_HEISHI_ANIM_STAND_HAND_ON_HIP
    ANIMMODE_ONCE, // EN_STONE_HEISHI_ANIM_DRINK_1
    ANIMMODE_LOOP, // EN_STONE_HEISHI_ANIM_CHEER_WITH_SPEAR
    ANIMMODE_LOOP, // EN_STONE_HEISHI_ANIM_WAVE
    ANIMMODE_LOOP, // EN_STONE_HEISHI_ANIM_SIT_AND_REACH
    ANIMMODE_LOOP, // EN_STONE_HEISHI_ANIM_DRINK_2
    ANIMMODE_ONCE, // EN_STONE_HEISHI_ANIM_STAND_UP
};

typedef enum {
    /* 0 */ EN_STONE_ACTION_0,
    /* 1 */ EN_STONE_ACTION_1,
    /* 2 */ EN_STONE_ACTION_CHECK_ITEM,
    /* 3 */ EN_STONE_ACTION_DRINK_BOTTLE,
    /* 4 */ EN_STONE_ACTION_4
} EnStoneHeishiAction;

typedef enum {
    /* 0 */ EN_STONE_DRINK_BOTTLE_INITIAL,
    /* 1 */ EN_STONE_DRINK_BOTTLE_DRINKING,
    /* 2 */ EN_STONE_DRINK_BOTTLE_EMPTY,
    /* 3 */ EN_STONE_DRINK_BOTTLE_STAND_UP,
    /* 4 */ EN_STONE_DRINK_BOTTLE_STANDING
} EnStoneHeishiDrinkBottle;

typedef enum {
    /* 0 */ EN_STONE_BOTTLE_NONE,
    /* 1 */ EN_STONE_BOTTLE_RED_POTION,
    /* 2 */ EN_STONE_BOTTLE_EMPTY,
    /* 3 */ EN_STONE_BOTTLE_BLUE_POTION
} EnStoneHeishiBottle;

s32 Actor_TalkOfferAccepted(Actor* actor, GameState* gameState);

void func_80BC9D28(EnStoneheishi* this, PlayState* play);
void func_80BC9E50(EnStoneheishi* this, PlayState* play);

RECOMP_PATCH void EnStoneheishi_GiveItemReward(EnStoneheishi* this, PlayState* play) {
    Message_CloseTextbox(play);

    if (rando_location_is_checked(LOCATION_STONE_SOLDIER)) {
        Actor_OfferGetItemHook(&this->actor, play, GI_RUPEE_BLUE, 0, 300.0f, 300.0f, false, false);
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_MASK_STONE, 300.0f, 300.0f);
    }

    this->action = EN_STONE_ACTION_4;
    this->actionFunc = func_80BC9D28;
}

RECOMP_PATCH void func_80BC9D28(EnStoneheishi* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->textIdIndex++;
        this->actor.textId = sEnStoneHeishiTextIds[this->textIdIndex];
        SET_WEEKEVENTREG(WEEKEVENTREG_41_40);
        Actor_TalkOfferAccepted(&this->actor, &play->state);
        Actor_OfferTalkExchange(&this->actor, play, 400.0f, 400.0f, PLAYER_IA_MINUS1);
        this->actionFunc = func_80BC9E50;
    } else if (rando_location_is_checked(LOCATION_STONE_SOLDIER)) {
        Actor_OfferGetItemHook(&this->actor, play, GI_RUPEE_BLUE, 0, 300.0f, 300.0f, false, false);
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_MASK_STONE, 300.0f, 300.0f);
    }
}