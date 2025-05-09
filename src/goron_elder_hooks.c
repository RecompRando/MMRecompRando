#include "modding.h"
#include "global.h"

#include "apcommon.h"

// TODO: change location to lullaby intro GI / other solution
#define LOCATION_LULLABY_INTRO (0x000100 | GI_AD)

#define EN_JG_IS_IN_GORON_SHRINE(thisx) ((thisx)->params & 0x1)
#define EN_JG_GET_PATH_INDEX(thisx) (((thisx)->params & 0xFC00) >> 10)

#define EN_JG_PATH_INDEX_NONE 0x3F

#define FLAG_SHRINE_GORON_ARMS_RAISED (1 << 0)
#define FLAG_LOOKING_AT_PLAYER (1 << 2)
#define FLAG_DRUM_SPAWNED (1 << 3)

#define THIS ((EnJg*)thisx)

#define EN_JG_ACTION_FIRST_THAW 0x0
#define GORON_ELDER_LIMB_MAX 0x23

struct EnJg;

typedef void (*EnJgActionFunc)(struct EnJg*, PlayState*);

typedef struct EnJg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ Actor* shrineGoron;
    /* 0x148 */ Actor* icePoly;
    /* 0x14C */ ColliderCylinder collider;
    /* 0x198 */ SkelAnime skelAnime;
    /* 0x1DC */ EnJgActionFunc actionFunc;
    /* 0x1E0 */ Path* path;
    /* 0x1E4 */ s32 currentPoint;
    /* 0x1E8 */ Actor* drum;
    /* 0x1EC */ Vec3s unusedRotation1; // probably meant to be a head rotation to look at the player
    /* 0x1F2 */ Vec3s unusedRotation2; // probably meant to be a body rotation to look at the player
    /* 0x1F8 */ Vec3s jointTable[GORON_ELDER_LIMB_MAX];
    /* 0x2CA */ Vec3s morphTable[GORON_ELDER_LIMB_MAX];
    /* 0x39C */ s16 rootRotationWhenTalking;
    /* 0x39E */ s16 animIndex;
    /* 0x3A0 */ s16 action;
    /* 0x3A2 */ s16 freezeTimer;
    /* 0x3A4 */ Vec3f breathPos;
    /* 0x3B0 */ Vec3f breathVelocity;
    /* 0x3BC */ Vec3f breathAccel;
    /* 0x3C8 */ s16 csId;
    /* 0x3CA */ u8 csAnimIndex;
    /* 0x3CB */ u8 cueId;
    /* 0x3CC */ u16 flags;
    /* 0x3CE */ u16 textId;
    /* 0x3D0 */ u8 focusedShrineGoronParam;
} EnJg; // size = 0x3D4

typedef enum EnJgAnimation {
    /*  0 */ EN_JG_ANIM_IDLE,
    /*  1 */ EN_JG_ANIM_WALK,
    /*  2 */ EN_JG_ANIM_WAVING,
    /*  3 */ EN_JG_ANIM_SHAKING_HEAD,
    /*  4 */ EN_JG_ANIM_SURPRISE_START,
    /*  5 */ EN_JG_ANIM_SURPRISE_LOOP,
    /*  6 */ EN_JG_ANIM_ANGRY,
    /*  7 */ EN_JG_ANIM_FROZEN_START,
    /*  8 */ EN_JG_ANIM_FROZEN_LOOP,
    /*  9 */ EN_JG_ANIM_WALK_2,
    /* 10 */ EN_JG_ANIM_TAKING_OUT_DRUM,
    /* 11 */ EN_JG_ANIM_DRUM_IDLE,
    /* 12 */ EN_JG_ANIM_PLAYING_DRUM,
    /* 13 */ EN_JG_ANIM_THINKING,
    /* 14 */ EN_JG_ANIM_REMEMBERING,
    /* 15 */ EN_JG_ANIM_STRONG_REMEMBERING,
    /* 16 */ EN_JG_ANIM_DEPRESSED,
    /* 17 */ EN_JG_ANIM_CUTSCENE_IDLE,
    /* 18 */ EN_JG_ANIM_CRADLE,
    /* 19 */ EN_JG_ANIM_MAX
} EnJgAnimation;

extern AnimationHeader gGoronElderIdleAnim;
extern AnimationHeader gGoronElderWalkAnim;
extern AnimationHeader gGoronElderWavingAnim;
extern AnimationHeader gGoronElderHeadShakeAnim;
extern AnimationHeader gGoronElderSurpriseStartAnim;
extern AnimationHeader gGoronElderSurpriseLoopAnim;
extern AnimationHeader gGoronElderAngryAnim;
extern AnimationHeader gGoronElderTakeOutDrumAnim;
extern AnimationHeader gGoronElderDrumIdleAnim;
extern AnimationHeader gGoronElderPlayingDrumAnim;
extern AnimationHeader gGoronElderThinkingAnim;
extern AnimationHeader gGoronElderRememberingAnim;
extern AnimationHeader gGoronElderStrongRememberingAnim;
extern AnimationHeader gGoronElderDepressedAnim;
extern AnimationHeader gGoronElderCradleAnim;

static AnimationInfoS sAnimationInfo[EN_JG_ANIM_MAX] = {
    { &gGoronElderIdleAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -10 },            // EN_JG_ANIM_IDLE
    { &gGoronElderWalkAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -10 },            // EN_JG_ANIM_WALK
    { &gGoronElderWavingAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -10 },          // EN_JG_ANIM_WAVING
    { &gGoronElderHeadShakeAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -10 },       // EN_JG_ANIM_SHAKING_HEAD
    { &gGoronElderSurpriseStartAnim, 1.0f, 0, -1, ANIMMODE_ONCE, -10 },   // EN_JG_ANIM_SURPRISE_START
    { &gGoronElderSurpriseLoopAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -10 },    // EN_JG_ANIM_SURPRISE_LOOP
    { &gGoronElderAngryAnim, 1.0f, 0, -1, ANIMMODE_LOOP, -10 },           // EN_JG_ANIM_ANGRY
    { &gGoronElderSurpriseStartAnim, 2.0f, 0, -1, ANIMMODE_ONCE, 0 },     // EN_JG_ANIM_FROZEN_START
    { &gGoronElderSurpriseStartAnim, -2.0f, 0, -1, ANIMMODE_ONCE, 0 },    // EN_JG_ANIM_FROZEN_LOOP
    { &gGoronElderWalkAnim, -1.0f, 0, -1, ANIMMODE_LOOP, -10 },           // EN_JG_ANIM_WALK_2
    { &gGoronElderTakeOutDrumAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },       // EN_JG_ANIM_TAKING_OUT_DRUM
    { &gGoronElderDrumIdleAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },          // EN_JG_ANIM_DRUM_IDLE
    { &gGoronElderPlayingDrumAnim, 1.0f, 1, 44, ANIMMODE_ONCE, 0 },       // EN_JG_ANIM_PLAYING_DRUM
    { &gGoronElderThinkingAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },          // EN_JG_ANIM_THINKING
    { &gGoronElderRememberingAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },       // EN_JG_ANIM_REMEMBERING
    { &gGoronElderStrongRememberingAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 }, // EN_JG_ANIM_STRONG_REMEMBERING
    { &gGoronElderDepressedAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },         // EN_JG_ANIM_DEPRESSED
    { &gGoronElderIdleAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },              // EN_JG_ANIM_CUTSCENE_IDLE
    { &gGoronElderCradleAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },            // EN_JG_ANIM_CRADLE
};

static Vec3f sSfxPos = { 0.0f, 0.0f, 0.0f };

EnJg* gEnJg;

void EnJg_SetupWalk(EnJg* this, PlayState* play);
void EnJg_Talk(EnJg* this, PlayState* play);
void EnJg_AlternateTalkOrWalkInPlace(EnJg* this, PlayState* play);

void EnJg_FinishOffer(EnJg* this, PlayState* play) {
    if (play->msgCtx.msgMode == MSGMODE_TEXT_CLOSING) {
        this->flags &= ~FLAG_LOOKING_AT_PLAYER;
        this->actionFunc = EnJg_SetupWalk;
    }
}

void EnJg_OfferLullabyIntro(EnJg* this, PlayState* play) {
    this->animIndex = EN_JG_ANIM_IDLE;
    SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, this->animIndex);
    
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnJg_FinishOffer;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_LULLABY_INTRO), LOCATION_LULLABY_INTRO, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void EnJg_SetupTalk(EnJg* this, PlayState* play) {
    // @rando prevent lullaby cutscene if location is checked
    if (rando_location_is_checked(LOCATION_LULLABY_INTRO)) {
        SET_WEEKEVENTREG(WEEKEVENTREG_24_40);
    }
    
    Player* player = GET_PLAYER(play);
    switch (this->textId) {
        case 0xDAC: // What was I doing?
            if (!rando_location_is_checked(LOCATION_LULLABY_INTRO) && player->transformation == PLAYER_FORM_GORON && CHECK_WEEKEVENTREG(WEEKEVENTREG_24_80)) {
                Message_CloseTextbox(play);
                SET_WEEKEVENTREG(WEEKEVENTREG_24_40);
                this->actionFunc = EnJg_OfferLullabyIntro;
                break;
            }

            this->animIndex = EN_JG_ANIM_SHAKING_HEAD;
            SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, this->animIndex);
            this->actionFunc = EnJg_Talk;
            break;

        case 0xDAD: // I must hurry!
            this->animIndex = EN_JG_ANIM_SURPRISE_START;
            SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, this->animIndex);
            this->actionFunc = EnJg_AlternateTalkOrWalkInPlace;
            break;

        case 0xDB7: // You're Darmani!
            this->animIndex = EN_JG_ANIM_SURPRISE_START;
            SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, this->animIndex);
            this->actionFunc = EnJg_Talk;
            break;

        case 0xDAE: // Do you have business with the Elder?
        case 0xDB3: // As Elder, I must do something
        case 0xDB6: // "Hunh???"
        case 0xDBA: // I've been made a fool of!
        case 0xDBD: // "...What?"
        case 0xDC4: // It's so cold I can't play
            this->animIndex = EN_JG_ANIM_IDLE;
            SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, this->animIndex);
            this->actionFunc = EnJg_Talk;
            break;

        case 0xDB0: // It's this cold snap
        case 0xDBB: // If I can see past the illusion, you'll vanish
        case 0xDBC: // Following me won't do you any good
        case 0xDC6: // I am counting on you
            this->animIndex = EN_JG_ANIM_ANGRY;
            SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, this->animIndex);
            this->actionFunc = EnJg_Talk;
            break;

        case 0xDB4: // This is our problem (first)
        case 0xDB5: // This is our problem (repeat)
            this->animIndex = EN_JG_ANIM_WAVING;
            SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, this->animIndex);
            this->actionFunc = EnJg_Talk;
            break;

        default:
            break;
    }
}