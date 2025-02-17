#include "modding.h"
#include "global.h"
#include "z64snap.h"

#include "apcommon.h"

#define TINGLE_LIMB_MAX 0x21
#define TINGLE_LIMB_BALLOON 0x06

#define TINGLE_GET_MAP_ID(thisx) ((thisx)->params & 0xFF)

struct EnBal;

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

#define THIS ((EnBal*)thisx)

typedef enum {
    /* 0 */ TINGLE_EYETEX_OPEN,
    /* 1 */ TINGLE_EYETEX_CLOSED
} TingleEyeTexture;

typedef enum {
    /* -1 */ TINGLE_ANIM_NONE = -1,
    /*  0 */ TINGLE_ANIM_FLOAT_IDLE,
    /*  1 */ TINGLE_ANIM_FALL_LOOP,
    /*  2 */ TINGLE_ANIM_FALL_ONCE,
    /*  3 */ TINGLE_ANIM_LAND,
    /*  4 */ TINGLE_ANIM_TWIST,
    /*  5 */ TINGLE_ANIM_TALK,
    /*  6 */ TINGLE_ANIM_MAGIC,
    /*  7 */ TINGLE_ANIM_HAPPY_DANCE_LOOP,
    /*  8 */ TINGLE_ANIM_HAPPY_DANCE_ONCE,
    /*  9 */ TINGLE_ANIM_MAGIC_REVERSE,
    /* 10 */ TINGLE_ANIM_IDLE,
    /* 11 */ TINGLE_ANIM_SPIN,
    /* 12 */ TINGLE_ANIM_HIDE_FACE,
    /* 13 */ TINGLE_ANIM_CONFETTI,
    /* 14 */ TINGLE_ANIM_MAX
} TingleAnimation;

typedef enum {
    /* 0 */ TINGLE_MAPCHOICE_PROXIMAL,
    /* 1 */ TINGLE_MAPCHOICE_DISTAL,
    /* 2 */ TINGLE_MAPCHOICE_CANCEL
} TingleBuyMapChoice;

typedef enum {
    /* 0 */ TINGLE_WATCH_TARGET_NONE,
    /* 1 */ TINGLE_WATCH_TARGET_PLAYER,
    /* 2 */ TINGLE_WATCH_TARGET_FAIRY
} TingleWatchTarget;

typedef enum {
    /* 0 */ TINGLE_BALLOON_ACTION_NONE,
    /* 1 */ TINGLE_BALLOON_ACTION_POP,
    /* 2 */ TINGLE_BALLOON_ACTION_FALL,
    /* 4 */ TINGLE_BALLOON_ACTION_INFLATE = 4,
    /* 5 */ TINGLE_BALLOON_ACTION_RISE
} TingleBalloonAction;

typedef enum {
    /* 0 */ TINGLE_IDLESTAGE_ACTIVITY,
    /* 2 */ TINGLE_IDLESTAGE_PREP_WAIT = 2,
    /* 3 */ TINGLE_IDLESTAGE_WAIT
} TingleIdleAnimStage;

typedef void (*EnBalActionFunc)(struct EnBal*, PlayState*);

typedef struct EnBal {
    /* 0x000 */ PictoActor picto;
    /* 0x148 */ SkelAnime skelAnime;
    /* 0x18C */ EnBalActionFunc actionFunc;
    /* 0x190 */ ColliderJntSph collider;
    /* 0x1B0 */ ColliderJntSphElement colliderElements[1];
    /* 0x1F0 */ Vec3f balloonScale;
    /* 0x1FC */ s32 eyeTexIndex;
    /* 0x200 */ Vec3s jointTable[TINGLE_LIMB_MAX];
    /* 0x2C6 */ Vec3s morphTable[TINGLE_LIMB_MAX];
    /* 0x38C */ Vec3s headRot;
    /* 0x392 */ Vec3s torsoRot;
    /* 0x398 */ s16 eyeTimer;
    /* 0x39A */ s16 forceEyesShut;
    /* 0x39C */ s16 watchTarget;
    /* 0x39E */ s16 skyFloatPhase;
    /* 0x3A0 */ s16 textId;
    /* 0x3A2 */ UNK_TYPE1 unk3A2[0x2];
    /* 0x3A4 */ s16 balloonAction;
    /* 0x3A6 */ s16 idleAnimStage;
    /* 0x3A8 */ s16 timer;
    /* 0x3AA */ UNK_TYPE1 unk3AA[0x2]; 
    /* 0x3AC */ s16 purchaseMapId;
    /* 0x3AE */ s16 locationMapId;    
    /* 0x3B0 */ s16 inflateEarly;
    /* 0x3B2 */ u8 isTalking;
} EnBal; /* size = 0x3B4 */

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(0, 0x0),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(0, 0x0),
    /* Zora boomerang */ DMG_ENTRY(1, 0xE),
    /* Normal arrow   */ DMG_ENTRY(1, 0xE),
    /* UNK_DMG_0x06   */ DMG_ENTRY(1, 0xF),
    /* Hookshot       */ DMG_ENTRY(1, 0xF),
    /* Goron punch    */ DMG_ENTRY(1, 0x0),
    /* Sword          */ DMG_ENTRY(1, 0x0),
    /* Goron pound    */ DMG_ENTRY(1, 0x0),
    /* Fire arrow     */ DMG_ENTRY(2, 0x2),
    /* Ice arrow      */ DMG_ENTRY(2, 0x3),
    /* Light arrow    */ DMG_ENTRY(2, 0x4),
    /* Goron spikes   */ DMG_ENTRY(1, 0xF),
    /* Deku spin      */ DMG_ENTRY(0, 0x0),
    /* Deku bubble    */ DMG_ENTRY(1, 0xE),
    /* Deku launch    */ DMG_ENTRY(2, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x0),
    /* Zora barrier   */ DMG_ENTRY(0, 0x0),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(1, 0xF),
    /* Zora punch     */ DMG_ENTRY(1, 0xF),
    /* Spin attack    */ DMG_ENTRY(1, 0xF),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(0, 0x0),
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 1, 0, 0, 0, MASS_IMMOVABLE };

static ColliderJntSphElementInit sJntSphElementsInit[1] = {
    {
        {
            ELEM_MATERIAL_UNK0,
            { 0xF7CFFFFF, 0x00, 0x00 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_NONE | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { TINGLE_LIMB_BALLOON, { { 2400, 0, 0 }, 50 }, 100 },
    },
};

static ColliderJntSphInit sJntSphInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sJntSphElementsInit),
    sJntSphElementsInit,
};

extern FlexSkeletonHeader gTingleSkel;
extern AnimationHeader gTingleFloatIdleAnim;
extern AnimationHeader gTingleFallAnim;
extern AnimationHeader gTingleLandAnim;
extern AnimationHeader gTingleTwistAnim;
extern AnimationHeader gTingleTalkAnim;
extern AnimationHeader gTingleThrowConfettiAnim;
extern AnimationHeader gTingleHappyDanceAnim;
extern AnimationHeader gTingleIdleAnim;
extern AnimationHeader gTingleSpinAnim;
extern AnimationHeader gTingleFloatHideFaceAnim;
extern AnimationHeader gTingleFloatThrowConfettiAnim;

static AnimationInfo sAnimationInfo[TINGLE_ANIM_MAX] = {
    { &gTingleFloatIdleAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },          // TINGLE_ANIM_FLOAT_IDLE
    { &gTingleFallAnim, 1.5f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },               // TINGLE_ANIM_FALL_LOOP
    { &gTingleFallAnim, 1.5f, 0.0f, 0.0f, ANIMMODE_ONCE, -4.0f },               // TINGLE_ANIM_FALL_ONCE
    { &gTingleLandAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, -4.0f },               // TINGLE_ANIM_LAND
    { &gTingleTwistAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },              // TINGLE_ANIM_TWIST
    { &gTingleTalkAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },               // TINGLE_ANIM_TALK
    { &gTingleThrowConfettiAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, -8.0f },      // TINGLE_ANIM_MAGIC
    { &gTingleHappyDanceAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },         // TINGLE_ANIM_HAPPY_DANCE_LOOP
    { &gTingleHappyDanceAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, -8.0f },         // TINGLE_ANIM_HAPPY_DANCE_ONCE
    { &gTingleThrowConfettiAnim, 1.0f, 23.0f, 0.0f, ANIMMODE_ONCE, -8.0f },     // TINGLE_ANIM_MAGIC_REVERSE
    { &gTingleIdleAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -8.0f },               // TINGLE_ANIM_IDLE
    { &gTingleSpinAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -2.0f },               // TINGLE_ANIM_SPIN
    { &gTingleFloatHideFaceAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -2.0f },      // TINGLE_ANIM_HIDE_FACE
    { &gTingleFloatThrowConfettiAnim, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, -2.0f }, // TINGLE_ANIM_CONFETTI
};

void EnBal_SetMainColliderToHead(EnBal* this);
s32 EnBal_ValidatePictograph(PlayState* play, Actor* thisx);
void EnBal_SetupFloatIdle(EnBal* this);

void EnBal_SetupGroundIdle(EnBal* this);

// @ap always start on the ground
RECOMP_PATCH void EnBal_Init(Actor* thisx, PlayState* play) {
    EnBal* this = (EnBal*)thisx;
    s32 pad;
    f32 endFrame = Animation_GetLastFrame(&gTingleFloatIdleAnim);

    this->locationMapId = TINGLE_GET_MAP_ID(&this->picto.actor);
    this->picto.actor.attentionRangeType = 1;
    this->picto.actor.cullingVolumeDistance = 3000.0f;
    Actor_SetScale(&this->picto.actor, 0.02f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gTingleSkel, &gTingleFloatIdleAnim, this->jointTable, this->morphTable,
                       TINGLE_LIMB_MAX);

    Animation_Change(&this->skelAnime, &gTingleTalkAnim, 1.0f, 0.0f, endFrame, ANIMMODE_LOOP, -10.0f);

    ActorShape_Init(&this->picto.actor.shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
    this->skyFloatPhase = 0;
    this->textId = 0;
    this->balloonAction = TINGLE_BALLOON_ACTION_NONE;
    this->timer = 0;
    this->watchTarget = TINGLE_WATCH_TARGET_NONE;
    this->inflateEarly = false;
    this->isTalking = false;
    this->eyeTexIndex = TINGLE_EYETEX_OPEN;
    this->eyeTimer = 60;
    this->forceEyesShut = false;
    Collider_InitJntSph(play, &this->collider);
    Collider_SetJntSph(play, &this->collider, &this->picto.actor, &sJntSphInit, this->colliderElements);
    CollisionCheck_SetInfo2(&this->picto.actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    Actor_UpdateBgCheckInfo(play, &this->picto.actor, 0.0f, 0.0f, 0.0f, UPDBGCHECKINFO_FLAG_4);
    this->picto.validationFunc = EnBal_ValidatePictograph;

    this->picto.actor.world.pos.y = this->picto.actor.floorHeight;
    EnBal_SetMainColliderToHead(this);
    EnBal_SetupGroundIdle(this);
}

static s16 sBuyMapOptions[TINGLE_MAP_MAX][2] = {
    { TINGLE_MAP_CLOCK_TOWN, TINGLE_MAP_WOODFALL },    // TINGLE_MAP_CLOCK_TOWN
    { TINGLE_MAP_WOODFALL, TINGLE_MAP_SNOWHEAD },      // TINGLE_MAP_WOODFALL
    { TINGLE_MAP_SNOWHEAD, TINGLE_MAP_ROMANI_RANCH },  // TINGLE_MAP_SNOWHEAD
    { TINGLE_MAP_ROMANI_RANCH, TINGLE_MAP_GREAT_BAY }, // TINGLE_MAP_ROMANI_RANCH
    { TINGLE_MAP_GREAT_BAY, TINGLE_MAP_STONE_TOWER },  // TINGLE_MAP_GREAT_BAY
    { TINGLE_MAP_STONE_TOWER, TINGLE_MAP_CLOCK_TOWN }, // TINGLE_MAP_STONE_TOWER
};

void EnBal_SetupOfferGetItem(EnBal* this);

RECOMP_PATCH s32 EnBal_CheckIfMapUnlocked(EnBal* this, PlayState* play) {
    this->purchaseMapId = sBuyMapOptions[this->locationMapId][play->msgCtx.choiceIndex];
    
    return rando_location_is_checked(this->purchaseMapId + 0xB4);
}

RECOMP_PATCH void EnBal_TryPurchaseMap(EnBal* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 price;

    if (Message_ShouldAdvance(play)) {
        if (play->msgCtx.choiceIndex != TINGLE_MAPCHOICE_CANCEL) {
            // Get price depending on which map player wants to buy
            if (play->msgCtx.choiceIndex == TINGLE_MAPCHOICE_PROXIMAL) {
                price = play->msgCtx.unk1206C;
            } else {
                price = play->msgCtx.unk12070;
            }

            if (gSaveContext.save.saveInfo.playerData.rupees < price) {
                // Can't buy map because player doesn't have the money
                Audio_PlaySfx(NA_SE_SY_ERROR);
                Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, TINGLE_ANIM_TALK);
                Message_StartTextbox(play, 0x1D0A, &this->picto.actor);
                this->textId = 0x1D0A;
            } else if (EnBal_CheckIfMapUnlocked(this, play)) {
                // Can't buy map because player already has it
                Audio_PlaySfx(NA_SE_SY_ERROR);
                Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, TINGLE_ANIM_TALK);
                Message_StartTextbox(play, 0x1D09, &this->picto.actor);
                this->textId = 0x1D09;
            } else {
                // Proceed with map purchase
                Audio_PlaySfx_MessageDecide();
                Rupees_ChangeBy(-price);
                Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, TINGLE_ANIM_MAGIC_REVERSE);
                this->forceEyesShut = true;
                Message_StartTextbox(play, 0x1D0B, &this->picto.actor);
                this->textId = 0x1D0B;
                // EnBal_UnlockSelectedAreaMap(this);
                player->stateFlags1 |= PLAYER_STATE1_20;
                EnBal_SetupOfferGetItem(this);
            }
        } else {
            // Cancel
            Audio_PlaySfx_MessageCancel();
            Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, TINGLE_ANIM_TALK);
            Message_StartTextbox(play, 0x1D06, &this->picto.actor);
            this->textId = 0x1D06;
        }
    }
}