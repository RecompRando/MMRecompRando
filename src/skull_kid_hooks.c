#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct DmStk;

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED | ACTOR_FLAG_UPDATE_DURING_OCARINA)

#define THIS ((DmStk*)thisx)

typedef void (*DmStkActionFunc)(struct DmStk*, PlayState*);

#define ACTORCTX_FLAG_1 (1 << 1)

#define DM_STK_GET_TYPE(thisx) ((thisx)->params)

#define SKULL_KID_LIMB_MAX 0x16

typedef enum {
    /*  0 */ SK_ANIM_SHAKE_HEAD,
    /*  1 */ SK_ANIM_WALK,
    /*  2 */ SK_ANIM_LOOK_AROUND, // Not used in the final game
    /*  3 */ SK_ANIM_IDLE,
    /*  4 */ SK_ANIM_UNUSED_IDLE, // Unused duplicate of SK_ANIM_IDLE
    /*  5 */ SK_ANIM_BENT_OVER_HEAD_TWITCH,
    /*  6 */ SK_ANIM_GLITCHY_HORSE_RIDE_START, // Not used in the final game
    /*  7 */ SK_ANIM_RECLINING_FLOAT,
    /*  8 */ SK_ANIM_CURSE_START,
    /*  9 */ SK_ANIM_CURSE_LOOP,
    /* 10 */ SK_ANIM_LAUGH_START,
    /* 11 */ SK_ANIM_LAUGH_LOOP,
    /* 12 */ SK_ANIM_HIDE_OCARINA_START,
    /* 13 */ SK_ANIM_HIDE_OCARINA_LOOP,
    /* 14 */ SK_ANIM_PICK_UP_OCARINA_AND_START_PLAYING,
    /* 15 */ SK_ANIM_PLAY_OCARINA_AND_LAUGH,
    /* 16 */ SK_ANIM_PICK_UP_OCARINA, // Not used in the final game
    /* 17 */ SK_ANIM_LOOK_AT_OCARINA, // Not used in the final game
    /* 18 */ SK_ANIM_KICK_OVER_LINK,
    /* 19 */ SK_ANIM_SEARCH_LINK,
    /* 20 */ SK_ANIM_UNUSED_KICK_OVER_LINK, // Unused duplicate of SK_ANIM_KICK_OVER_LINK
    /* 21 */ SK_ANIM_UNUSED_SEARCH_LINK,    // Unused duplicate of SK_ANIM_SEARCH_LINK
    /* 22 */ SK_ANIM_HORSE_RIDE_START,
    /* 23 */ SK_ANIM_HORSE_RIDE_LOOP,
    /* 24 */ SK_ANIM_RAISE_MASK_START,
    /* 25 */ SK_ANIM_RAISE_MASK_LOOP,
    /* 26 */ SK_ANIM_LOWER_MASK,
    /* 27 */ SK_ANIM_JUMP_WHILE_HIDING_OCARINA,
    /* 28 */ SK_ANIM_HORSE_RIDE_AND_ROTATE, // Used when Link gets thrown off his horse
    /* 29 */ SK_ANIM_PLAY_OCARINA_WHILE_FLOATING,
    /* 30 */ SK_ANIM_FLOATING_TURN_AROUND,
    /* 31 */ SK_ANIM_OCARINA_JUGGLE,
    /* 32 */ SK_ANIM_CALL_DOWN_MOON_START,
    /* 33 */ SK_ANIM_CALL_DOWN_MOON_LOOP,
    /* 34 */ SK_ANIM_SMACK_FAIRY_START,
    /* 35 */ SK_ANIM_SMACK_FAIRY_LOOP,
    /* 36 */ SK_ANIM_HIT_BY_BUBBLE,
    /* 37 */ SK_ANIM_DROP_OCARINA,
    /* 38 */ SK_ANIM_FLOATING_ARMS_CROSSED,
    /* 39 */ SK_ANIM_DEFLECT_ATTACK,
    /* 40 */ SK_ANIM_TELESCOPE_LOOK_UP_START,
    /* 41 */ SK_ANIM_TELESCOPE_LOOK_UP_LOOP,
    /* 42 */ SK_ANIM_SURPRISE_START,
    /* 43 */ SK_ANIM_SURPRISE_LOOP,
    /* 44 */ SK_ANIM_LOOK_AROUND_FOR_GIANTS_START,
    /* 45 */ SK_ANIM_LOOK_AROUND_FOR_GIANTS_LOOP,
    /* 46 */ SK_ANIM_HOLD_HEAD_AND_SHAKE_START,
    /* 47 */ SK_ANIM_HOLD_HEAD_AND_SHAKE_LOOP,
    /* 48 */ SK_ANIM_HOLD_HEAD_AND_SCREAM_START,
    /* 49 */ SK_ANIM_HOLD_HEAD_AND_SCREAM_LOOP,
    /* 50 */ SK_ANIM_HUDDLE_WITH_FAIRIES,
    /* 51 */ SK_ANIM_SEARCH_MASK_SALESMAN,
    /* 52 */ SK_ANIM_HOLD_UP_MASK_START,
    /* 53 */ SK_ANIM_HOLD_UP_MASK_LOOP,
    /* 54 */ SK_ANIM_SHIVER,
    /* 55 */ SK_ANIM_DRAW,
    /* 56 */ SK_ANIM_BENT_OVER_LOOK_UP,
    /* 57 */ SK_ANIM_SPANK,
    /* 58 */ SK_ANIM_HIP_SHAKE_AND_JUMP,
    /* 59 */ SK_ANIM_PLAY_FLUTE,
    /* 60 */ SK_ANIM_CARTWHEEL,
    /* 61 */ SK_ANIM_LIE_FLAT,
    /* 62 */ SK_ANIM_DANGLE_FROM_MASK_START,
    /* 63 */ SK_ANIM_DANGLE_FROM_MASK_LOOP,
    /* 64 */ SK_ANIM_DROPPED_FROM_MASK,
    /* 65 */ SK_ANIM_LOOK_UP_AT_GIANTS,
    /* 66 */ SK_ANIM_ASHAMED_START,
    /* 67 */ SK_ANIM_ASHAMED_LOOP,
    /* 68 */ SK_ANIM_LOOK_LEFT_START,
    /* 69 */ SK_ANIM_LOOK_LEFT_LOOP,
    /* 70 */ SK_ANIM_SNIFF,
    /* 71 */ SK_ANIM_LAUGH_AFTER_SNIFF,
    /* 72 */ SK_ANIM_LAUGH_AFTER_MAX
} SkullKidAnimation;

typedef enum {
    /* 0 */ DM_STK_TYPE_SKULL_KID,
    /* 1 */ DM_STK_TYPE_MAJORAS_MASK
} DmStkType;

typedef struct DmStk {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[SKULL_KID_LIMB_MAX]; // Not used, since it's allocated dynamically instead.
    /* 0x20C */ Vec3s morphTable[SKULL_KID_LIMB_MAX]; // Not used, since it's allocated dynamically instead.
    /* 0x290 */ DmStkActionFunc actionFunc;
    /* 0x294 */ ColliderCylinder collider;
    /* 0x2E0 */ s16 animIndex;
    /* 0x2E4 */ s32 alpha;
    /* 0x2E8 */ u32 fogR;
    /* 0x2EC */ u32 fogG;
    /* 0x2F0 */ u32 fogB;
    /* 0x2F4 */ s32 fogA;
    /* 0x2F8 */ s32 fogN;
    /* 0x2FC */ s32 fogF;
    /* 0x300 */ f32 fogScale;
    /* 0x304 */ Vec3f headPos; // set but never used
    /* 0x310 */ Vec3f oathToOrderCutsceneVoicePos;
    /* 0x31C */ UNK_TYPE1 unk31C[0xC];
    /* 0x328 */ u16 tatlMessageTimer;
    /* 0x32A */ u16 bobPhase;
    /* 0x32C */ u8 maskType;
    /* 0x32D */ u8 handType;
    /* 0x32E */ u8 fadeInState;
    /* 0x32F */ u8 fadeOutState;
    /* 0x330 */ s32 fadeOutTimer;
    /* 0x334 */ u8 cueId;
    /* 0x335 */ u8 hasBeenHit; // set but never used
    /* 0x336 */ s8 objectStkObjectSlot;
    /* 0x337 */ s8 objectStk2ObjectSlot;
    /* 0x338 */ s8 objectStk3ObjectSlot;
    /* 0x339 */ u8 deflectCount;
    /* 0x33A */ u8 dekuPipesCutsceneState;
    /* 0x33B */ u8 shouldDraw;
} DmStk; // size = 0x33C

typedef enum {
    /* 0 */ SK_MASK_TYPE_NO_MASK,
    /* 1 */ SK_MASK_TYPE_NORMAL,
    /* 2 */ SK_MASK_TYPE_GLOWING_EYES,
    /* 3 */ SK_MASK_TYPE_FADING_OUT,
    /* 9 */ SK_MASK_TYPE_RAISED = 9
} SkullKidMaskType;

typedef enum {
    /* 0 */ SK_HAND_TYPE_HOLDING_LINK_MASK_AND_FLUTE,
    /* 1 */ SK_HAND_TYPE_HOLDING_LINK_MASK,
    /* 2 */ SK_HAND_TYPE_HOLDING_MAJORAS_MASK,
    /* 3 */ SK_HAND_TYPE_HOLDING_OCARINA,
    /* 4 */ SK_HAND_TYPE_JUGGLING_OR_DROPPING_OCARINA,
    /* 5 */ SK_HAND_TYPE_HOLDING_MAJORAS_MASK_AND_FLUTE,
    /* 6 */ SK_HAND_TYPE_HOLDING_FLUTE,
    /* 9 */ SK_HAND_TYPE_DEFAULT = 9
} SkullKidHandType;

typedef enum {
    /* 0 */ SK_FADE_IN_STATE_NONE,
    /* 1 */ SK_FADE_IN_STATE_START,
    /* 2 */ SK_FADE_IN_STATE_INCREASE_FOG,
    /* 3 */ SK_FADE_IN_STATE_INCREASE_ALPHA
} SkullKidFadeInState;

typedef enum {
    /* 0 */ SK_FADE_OUT_STATE_NONE,
    /* 1 */ SK_FADE_OUT_STATE_FADING_OUT
} SkullKidFadeOutState;

typedef enum {
    /* 0 */ SK_DEKU_PIPES_CS_STATE_NOT_READY,
    /* 1 */ SK_DEKU_PIPES_CS_STATE_READY,
    /* 2 */ SK_DEKU_PIPES_CS_STATE_PLAYER_USED_OCARINA,
    /* 3 */ SK_DEKU_PIPES_CS_STATE_START,
    /* 4 */ SK_DEKU_PIPES_CS_STATE_END
} SkullKidDekuPipesCutsceneState;

static CollisionCheckInfoInit2 sColChkInfoInit = { 1, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(1, 0xF),
    /* Deku Stick     */ DMG_ENTRY(1, 0xF),
    /* Horse trample  */ DMG_ENTRY(1, 0xF),
    /* Explosives     */ DMG_ENTRY(1, 0xF),
    /* Zora boomerang */ DMG_ENTRY(1, 0xF),
    /* Normal arrow   */ DMG_ENTRY(1, 0xF),
    /* UNK_DMG_0x06   */ DMG_ENTRY(1, 0xF),
    /* Hookshot       */ DMG_ENTRY(1, 0xF),
    /* Goron punch    */ DMG_ENTRY(1, 0xF),
    /* Sword          */ DMG_ENTRY(1, 0xF),
    /* Goron pound    */ DMG_ENTRY(1, 0xF),
    /* Fire arrow     */ DMG_ENTRY(1, 0xF),
    /* Ice arrow      */ DMG_ENTRY(1, 0xF),
    /* Light arrow    */ DMG_ENTRY(1, 0xF),
    /* Goron spikes   */ DMG_ENTRY(1, 0xF),
    /* Deku spin      */ DMG_ENTRY(1, 0xF),
    /* Deku bubble    */ DMG_ENTRY(1, 0xF),
    /* Deku launch    */ DMG_ENTRY(1, 0xF),
    /* UNK_DMG_0x12   */ DMG_ENTRY(1, 0xF),
    /* Zora barrier   */ DMG_ENTRY(1, 0xF),
    /* Normal shield  */ DMG_ENTRY(1, 0xF),
    /* Light ray      */ DMG_ENTRY(1, 0xF),
    /* Thrown object  */ DMG_ENTRY(1, 0xF),
    /* Zora punch     */ DMG_ENTRY(1, 0xF),
    /* Spin attack    */ DMG_ENTRY(1, 0xF),
    /* Sword beam     */ DMG_ENTRY(1, 0xF),
    /* Normal Roll    */ DMG_ENTRY(1, 0xF),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(1, 0xF),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(1, 0xF),
    /* Unblockable    */ DMG_ENTRY(1, 0xF),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(1, 0xF),
    /* Powder Keg     */ DMG_ENTRY(1, 0xF),
};

extern AnimationHeader gSkullKidShakeHeadAnim;
extern AnimationHeader gSkullKidWalkAnim;
extern AnimationHeader gSkullKidLookAroundAnim;
extern AnimationHeader gSkullKidIdleAnim;
extern AnimationHeader gSkullKidIdleAnim;
extern AnimationHeader gSkullKidBentOverHeadTwitchAnim;
extern AnimationHeader gSkullKidGlitchyHorseRideStartAnim;
extern AnimationHeader gSkullKidRecliningFloatAnim;
extern AnimationHeader gSkullKidCurseStartAnim;
extern AnimationHeader gSkullKidCurseLoopAnim;
extern AnimationHeader gSkullKidLaughStartAnim;
extern AnimationHeader gSkullKidLaughLoopAnim;
extern AnimationHeader gSkullKidHideOcarinaStartAnim;
extern AnimationHeader gSkullKidHideOcarinaLoopAnim;
extern AnimationHeader gSkullKidPickUpOcarinaAndStartPlayingAnim;
extern AnimationHeader gSkullKidPlayOcarinaAndLaughAnim;
extern AnimationHeader gSkullKidPickUpOcarinaAnim;
extern AnimationHeader gSkullKidLookAtOcarinaAnim;
extern AnimationHeader gSkullKidKickOverLinkAnim;
extern AnimationHeader gSkullKidSearchLinkAnim;
extern AnimationHeader gSkullKidKickOverLinkAnim;
extern AnimationHeader gSkullKidSearchLinkAnim;
extern AnimationHeader gSkullKidHorseRideStartAnim;
extern AnimationHeader gSkullKidHorseRideLoopAnim;
extern AnimationHeader gSkullKidRaiseMaskStartAnim;
extern AnimationHeader gSkullKidRaiseMaskLoopAnim;
extern AnimationHeader gSkullKidLowerMaskAnim;
extern AnimationHeader gSkullKidJumpWhileHidingOcarinaAnim;
extern AnimationHeader gSkullKidHorseRideAndRotateAnim;
extern AnimationHeader gSkullKidPlayOcarinaWhileFloatingAnim;
extern AnimationHeader gSkullKidFloatingTurnAroundAnim;
extern AnimationHeader gSkullKidOcarinaJuggleAnim;
extern AnimationHeader gSkullKidCallDownMoonStartAnim;
extern AnimationHeader gSkullKidCallDownMoonLoopAnim;
extern AnimationHeader gSkullKidSmackFairyStartAnim;
extern AnimationHeader gSkullKidSmackFairyLoopAnim;
extern AnimationHeader gSkullKidHitByBubbleAnim;
extern AnimationHeader gSkullKidDropOcarinaAnim;
extern AnimationHeader gSkullKidFloatingArmsCrossedAnim;
extern AnimationHeader gSkullKidDeflectAttackAnim;
extern AnimationHeader gSkullKidTelescopeLookUpStartAnim;
extern AnimationHeader gSkullKidTelescopeLookUpLoopAnim;
extern AnimationHeader gSkullKidSurpriseStartAnim;
extern AnimationHeader gSkullKidSurpriseLoopAnim;
extern AnimationHeader gSkullKidLookAroundForGiantsStartAnim;
extern AnimationHeader gSkullKidLookAroundForGiantsLoopAnim;
extern AnimationHeader gSkullKidHoldHeadAndShakeStartAnim;
extern AnimationHeader gSkullKidHoldHeadAndShakeLoopAnim;
extern AnimationHeader gSkullKidHoldHeadAndScreamStartAnim;
extern AnimationHeader gSkullKidHoldHeadAndScreamLoopAnim;
extern AnimationHeader gSkullKidHuddleWithFairiesAnim;
extern AnimationHeader gSkullKidSearchMaskSalesmanAnim;
extern AnimationHeader gSkullKidHoldUpMaskStartAnim;
extern AnimationHeader gSkullKidHoldUpMaskLoopAnim;
extern AnimationHeader gSkullKidShiverAnim;
extern AnimationHeader gSkullKidDrawAnim;
extern AnimationHeader gSkullKidBentOverLookUpAnim;
extern AnimationHeader gSkullKidSpankAnim;
extern AnimationHeader gSkullKidHipShakeAndJumpAnim;
extern AnimationHeader gSkullKidPlayFluteAnim;
extern AnimationHeader gSkullKidCartwheelAnim;
extern AnimationHeader gSkullKidDangleFromMaskStartAnim;
extern AnimationHeader gSkullKidDangleFromMaskStartAnim;
extern AnimationHeader gSkullKidDangleFromMaskLoopAnim;
extern AnimationHeader gSkullKidDroppedFromMaskAnim;
extern AnimationHeader gSkullKidLookUpAtGiantsAnim;
extern AnimationHeader gSkullKidAshamedStartAnim;
extern AnimationHeader gSkullKidAshamedLoopAnim;
extern AnimationHeader gSkullKidLookLeftStartAnim;
extern AnimationHeader gSkullKidLookLeftLoopAnim;
extern AnimationHeader gSkullKidSniffAnim;
extern AnimationHeader gSkullKidLaughAfterSniffAnim;

extern FlexSkeletonHeader gSkullKidSkel;

static AnimationInfo sAnimationInfo[SK_ANIM_LAUGH_AFTER_MAX] = {
    { &gSkullKidShakeHeadAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },             // SK_ANIM_SHAKE_HEAD
    { &gSkullKidWalkAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },                  // SK_ANIM_WALK
    { &gSkullKidLookAroundAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },            // SK_ANIM_LOOK_AROUND
    { &gSkullKidIdleAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },                  // SK_ANIM_IDLE
    { &gSkullKidIdleAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },                  // SK_ANIM_UNUSED_IDLE
    { &gSkullKidBentOverHeadTwitchAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },    // SK_ANIM_BENT_OVER_HEAD_TWITCH
    { &gSkullKidGlitchyHorseRideStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f }, // SK_ANIM_GLITCHY_HORSE_RIDE_START
    { &gSkullKidRecliningFloatAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },        // SK_ANIM_RECLINING_FLOAT
    { &gSkullKidCurseStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },            // SK_ANIM_CURSE_START
    { &gSkullKidCurseLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },             // SK_ANIM_CURSE_LOOP
    { &gSkullKidLaughStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },            // SK_ANIM_LAUGH_START
    { &gSkullKidLaughLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },             // SK_ANIM_LAUGH_LOOP
    { &gSkullKidHideOcarinaStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },      // SK_ANIM_HIDE_OCARINA_START
    { &gSkullKidHideOcarinaLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },       // SK_ANIM_HIDE_OCARINA_LOOP
    // SK_ANIM_PICK_UP_OCARINA_AND_START_PLAYING
    { &gSkullKidPickUpOcarinaAndStartPlayingAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    { &gSkullKidPlayOcarinaAndLaughAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f }, // SK_ANIM_PLAY_OCARINA_AND_LAUGH
    { &gSkullKidPickUpOcarinaAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },       // SK_ANIM_PICK_UP_OCARINA
    { &gSkullKidLookAtOcarinaAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },       // SK_ANIM_LOOK_AT_OCARINA
    { &gSkullKidKickOverLinkAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },        // SK_ANIM_KICK_OVER_LINK
    { &gSkullKidSearchLinkAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },          // SK_ANIM_SEARCH_LINK
    { &gSkullKidKickOverLinkAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },        // SK_ANIM_UNUSED_KICK_OVER_LINK
    { &gSkullKidSearchLinkAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },          // SK_ANIM_UNUSED_SEARCH_LINK
    { &gSkullKidHorseRideStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },      // SK_ANIM_HORSE_RIDE_START
    { &gSkullKidHorseRideLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },       // SK_ANIM_HORSE_RIDE_LOOP
    { &gSkullKidRaiseMaskStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },      // SK_ANIM_RAISE_MASK_START
    { &gSkullKidRaiseMaskLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },       // SK_ANIM_RAISE_MASK_LOOP
    { &gSkullKidLowerMaskAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },           // SK_ANIM_LOWER_MASK
    // SK_ANIM_JUMP_WHILE_HIDING_OCARINA
    { &gSkullKidJumpWhileHidingOcarinaAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    { &gSkullKidHorseRideAndRotateAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f }, // SK_ANIM_HORSE_RIDE_AND_ROTATE
    // SK_ANIM_PLAY_OCARINA_WHILE_FLOATING
    { &gSkullKidPlayOcarinaWhileFloatingAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    { &gSkullKidFloatingTurnAroundAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },   // SK_ANIM_FLOATING_TURN_AROUND
    { &gSkullKidOcarinaJuggleAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },        // SK_ANIM_OCARINA_JUGGLE
    { &gSkullKidCallDownMoonStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },    // SK_ANIM_CALL_DOWN_MOON_START
    { &gSkullKidCallDownMoonLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },     // SK_ANIM_CALL_DOWN_MOON_LOOP
    { &gSkullKidSmackFairyStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },      // SK_ANIM_SMACK_FAIRY_START
    { &gSkullKidSmackFairyLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },       // SK_ANIM_SMACK_FAIRY_LOOP
    { &gSkullKidHitByBubbleAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },          // SK_ANIM_HIT_BY_BUBBLE
    { &gSkullKidDropOcarinaAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },          // SK_ANIM_DROP_OCARINA
    { &gSkullKidFloatingArmsCrossedAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },  // SK_ANIM_FLOATING_ARMS_CROSSED
    { &gSkullKidDeflectAttackAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },        // SK_ANIM_DEFLECT_ATTACK
    { &gSkullKidTelescopeLookUpStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f }, // SK_ANIM_TELESCOPE_LOOK_UP_START
    { &gSkullKidTelescopeLookUpLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },  // SK_ANIM_TELESCOPE_LOOK_UP_LOOP
    { &gSkullKidSurpriseStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },        // SK_ANIM_SURPRISE_START
    { &gSkullKidSurpriseLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },         // SK_ANIM_SURPRISE_LOOP
    // SK_ANIM_LOOK_AROUND_FOR_GIANTS_START
    { &gSkullKidLookAroundForGiantsStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    // SK_ANIM_LOOK_AROUND_FOR_GIANTS_LOOP
    { &gSkullKidLookAroundForGiantsLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    { &gSkullKidHoldHeadAndShakeStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE,
      0.0f },                                                                       // SK_ANIM_HOLD_HEAD_AND_SHAKE_START
    { &gSkullKidHoldHeadAndShakeLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f }, // SK_ANIM_HOLD_HEAD_AND_SHAKE_LOOP
    // SK_ANIM_HOLD_HEAD_AND_SCREAM_START
    { &gSkullKidHoldHeadAndScreamStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },
    // SK_ANIM_HOLD_HEAD_AND_SCREAM_LOOP
    { &gSkullKidHoldHeadAndScreamLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },
    { &gSkullKidHuddleWithFairiesAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },   // SK_ANIM_HUDDLE_WITH_FAIRIES
    { &gSkullKidSearchMaskSalesmanAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },  // SK_ANIM_SEARCH_MASK_SALESMAN
    { &gSkullKidHoldUpMaskStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },     // SK_ANIM_HOLD_UP_MASK_START
    { &gSkullKidHoldUpMaskLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },      // SK_ANIM_HOLD_UP_MASK_LOOP
    { &gSkullKidShiverAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },              // SK_ANIM_SHIVER
    { &gSkullKidDrawAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },                // SK_ANIM_DRAW
    { &gSkullKidBentOverLookUpAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },      // SK_ANIM_BENT_OVER_LOOK_UP
    { &gSkullKidSpankAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },               // SK_ANIM_SPANK
    { &gSkullKidHipShakeAndJumpAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },     // SK_ANIM_HIP_SHAKE_AND_JUMP
    { &gSkullKidPlayFluteAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },           // SK_ANIM_PLAY_FLUTE
    { &gSkullKidCartwheelAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },           // SK_ANIM_CARTWHEEL
    { &gSkullKidDangleFromMaskStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f }, // SK_ANIM_LIE_FLAT
    { &gSkullKidDangleFromMaskStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f }, // SK_ANIM_DANGLE_FROM_MASK_START
    { &gSkullKidDangleFromMaskLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },  // SK_ANIM_DANGLE_FROM_MASK_LOOP
    { &gSkullKidDroppedFromMaskAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },     // SK_ANIM_DROPPED_FROM_MASK
    { &gSkullKidLookUpAtGiantsAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },      // SK_ANIM_LOOK_UP_AT_GIANTS
    { &gSkullKidAshamedStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },        // SK_ANIM_ASHAMED_START
    { &gSkullKidAshamedLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },         // SK_ANIM_ASHAMED_LOOP
    { &gSkullKidLookLeftStartAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },       // SK_ANIM_LOOK_LEFT_START
    { &gSkullKidLookLeftLoopAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },        // SK_ANIM_LOOK_LEFT_LOOP
    { &gSkullKidSniffAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_ONCE, 0.0f },               // SK_ANIM_SNIFF
    { &gSkullKidLaughAfterSniffAnim, 1.0f, 0.0f, -1.0f, ANIMMODE_LOOP, 0.0f },     // SK_ANIM_LAUGH_AFTER_SNIFF
};

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_HIT1,
        AT_NONE,
        AC_ON | AC_HARD | AC_TYPE_PLAYER | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK1,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 14, 38, 0, { 0, 0, 0 } },
};

bool skull_kid_has_ocarina;

void DmStk_DoNothing(DmStk* this, PlayState* play);
void DmStk_ClockTower_DoNothing(DmStk* this, PlayState* play);
void DmStk_WaitForTelescope(DmStk* this, PlayState* play);
void DmStk_ChangeAnim(DmStk* this, PlayState* play, SkelAnime* skelAnime, AnimationInfo* animInfo, u16 animIndex);
void DmStk_ClockTower_StartIntroCutsceneVersion1(DmStk* this, PlayState* play);
void DmStk_ClockTower_WaitForIntroCutsceneVersion1ToEnd(DmStk* this, PlayState* play);
void DmStk_ClockTower_StartIntroCutsceneVersion2(DmStk* this, PlayState* play);
void DmStk_ClockTower_WaitForIntroCutsceneVersion2ToEnd(DmStk* this, PlayState* play);
void DmStk_ClockTower_WaitForDropOcarinaCutsceneToEnd(DmStk* this, PlayState* play);
void DmStk_ClockTower_IdleWithOcarina(DmStk* this, PlayState* play);
void DmStk_ClockTower_Idle(DmStk* this, PlayState* play);

RECOMP_PATCH void DmStk_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    DmStk* this = (DmStk*)thisx;

    this->shouldDraw = true;
    if (DM_STK_GET_TYPE(&this->actor) != DM_STK_TYPE_MAJORAS_MASK) {
        this->dekuPipesCutsceneState = SK_DEKU_PIPES_CS_STATE_NOT_READY;
        this->objectStkObjectSlot = Object_GetSlot(&play->objectCtx, OBJECT_STK);
        this->objectStk2ObjectSlot = Object_GetSlot(&play->objectCtx, OBJECT_STK2);
        this->objectStk3ObjectSlot = Object_GetSlot(&play->objectCtx, OBJECT_STK3);
        if (this->objectStkObjectSlot <= OBJECT_SLOT_NONE) {
            Actor_Kill(&this->actor);
        }

        this->tatlMessageTimer = 0;
        this->deflectCount = 0;
        this->maskType = SK_MASK_TYPE_NORMAL;
        this->animIndex = SK_ANIM_IDLE;
        this->fogR = play->lightCtx.fogColor[0];
        this->fogG = play->lightCtx.fogColor[1];
        this->fogB = play->lightCtx.fogColor[2];

        if ((play->sceneId == SCENE_LOST_WOODS) && (gSaveContext.sceneLayer == 1)) {
            this->alpha = 0;
            this->fogN = 0;
            this->fogF = 1000;
            this->fogScale = 1.0f;
            this->actionFunc = DmStk_DoNothing;
        } else if (play->sceneId == SCENE_OKUJOU) {
            this->alpha = 255;
            this->fogN = 996;
            this->fogF = 1000;
            this->fogScale = 0.7f;
            this->hasBeenHit = false;

            Collider_InitCylinder(play, &this->collider);

            if (gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_ROOFTOP, 0)) {
                if (gSaveContext.sceneLayer == 0) {
                    if (gSaveContext.timerStates[TIMER_ID_MOON_CRASH] == TIMER_STATE_OFF) {
                        // Starts a 5 minute (300 second) timer until the moon falls.
                        Interface_StartTimer(TIMER_ID_MOON_CRASH, 300);
                        R_MOON_CRASH_TIMER_Y = 200;
                        R_MOON_CRASH_TIMER_X = 115;
                    }

                    this->actor.world.pos.y = 120.0f;
                    Audio_PlaySubBgm(NA_BGM_MINI_BOSS);

                    sCylinderInit.base.colMaterial = COL_MATERIAL_WOOD;
                    this->animIndex = SK_ANIM_CALL_DOWN_MOON_LOOP;
                    this->handType = SK_HAND_TYPE_HOLDING_OCARINA;
                    DmStk_ChangeAnim(this, play, &this->skelAnime, &sAnimationInfo[this->animIndex], 0);
                    this->actionFunc = DmStk_ClockTower_IdleWithOcarina;

                } else if (gSaveContext.sceneLayer == 3) {
                    this->animIndex = SK_ANIM_FLOATING_ARMS_CROSSED;
                    if (gSaveContext.timerStates[TIMER_ID_MOON_CRASH] == TIMER_STATE_OFF) {
                        // This code is called when the Giants fail to stop the moon.
                        // Starts a 1 minute (60 second) timer until the moon falls.
                        Interface_StartTimer(TIMER_ID_MOON_CRASH, 60);
                        R_MOON_CRASH_TIMER_Y = 200;
                        R_MOON_CRASH_TIMER_X = 115;
                    }

                    this->actor.world.pos.y = 120.0f;
                    sCylinderInit.base.colMaterial = COL_MATERIAL_WOOD;
                    this->actionFunc = DmStk_ClockTower_Idle;
                } else {
                    this->animIndex = SK_ANIM_FLOATING_ARMS_CROSSED;
                    this->actionFunc = DmStk_ClockTower_DoNothing;
                }
            } else {
                this->dekuPipesCutsceneState = SK_DEKU_PIPES_CS_STATE_READY;
                this->animIndex = SK_ANIM_FLOATING_ARMS_CROSSED;
                this->actor.world.pos.y = 120.0f;
                sCylinderInit.base.colMaterial = COL_MATERIAL_WOOD;
                this->actionFunc = DmStk_ClockTower_Idle;
            }

            Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
            CollisionCheck_SetInfo2(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);

        } else if ((play->sceneId == SCENE_00KEIKOKU) && (gSaveContext.sceneLayer == 0)) {
            if (!(play->actorCtx.flags & ACTORCTX_FLAG_TELESCOPE_ON)) {
                Actor_Kill(&this->actor);
            }

            this->maskType = SK_MASK_TYPE_GLOWING_EYES;
            this->alpha = 255;
            this->fogN = 996;
            this->fogF = 1000;
            this->fogScale = 0.7f;
            this->animIndex = SK_ANIM_BENT_OVER_HEAD_TWITCH;
            this->actionFunc = DmStk_WaitForTelescope;
        } else {
            if ((play->sceneId == SCENE_LOST_WOODS) && !Cutscene_IsPlaying(play)) {
                Actor_Kill(&this->actor);
            }

            this->maskType = SK_MASK_TYPE_GLOWING_EYES;
            this->alpha = 255;
            this->fogN = 996;
            this->fogF = 1000;
            this->fogScale = 0.7f;
            this->actionFunc = DmStk_DoNothing;
        }

        this->handType = SK_HAND_TYPE_DEFAULT;
        this->fadeInState = SK_FADE_IN_STATE_NONE;
        this->fadeOutState = SK_FADE_OUT_STATE_NONE;
        this->fadeOutTimer = 0;
        this->alpha = this->alpha; // Set to itself
        this->actor.lockOnArrowOffset = 1100.0f;
        this->cueId = 99;
        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
        SkelAnime_InitFlex(play, &this->skelAnime, &gSkullKidSkel, NULL, NULL, NULL, 0);
        DmStk_ChangeAnim(this, play, &this->skelAnime, &sAnimationInfo[this->animIndex], 0);
    }

    Actor_SetScale(&this->actor, 0.01f);

    if ((play->sceneId == SCENE_00KEIKOKU) && (gSaveContext.sceneLayer == 3) && (play->csCtx.scriptIndex > 0)) {
        play->envCtx.skyboxConfig = SKYBOX_CONFIG_15;
        play->envCtx.changeSkyboxNextConfig = SKYBOX_CONFIG_15;
    }
}

void DmStk_ClockTower_AdjustHeightAndRotation(DmStk* this, PlayState* play);
void DmStk_ClockTower_StartDropOcarinaCutscene(DmStk* this, PlayState* play);

/**
 * Handles Skull Kid when he is at the top of the Clock Tower with the Ocarina of Time.
 * If he is hit in this state, he will drop the Ocarina.
 *
 * If the player waits a while while Skull Kid is in this state, they will see a message
 * from Tatl telling them to hurry up and do something.
 */
RECOMP_PATCH void DmStk_ClockTower_IdleWithOcarina(DmStk* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (play->csCtx.state == CS_STATE_IDLE) {
        if (rando_location_is_checked(GI_OCARINA_OF_TIME) && rando_location_is_checked(0x040067)) {
            sCylinderInit.base.colMaterial = COL_MATERIAL_WOOD;
            this->animIndex = SK_ANIM_FLOATING_ARMS_CROSSED;
            this->handType = SK_HAND_TYPE_DEFAULT;
            DmStk_ChangeAnim(this, play, &this->skelAnime, &sAnimationInfo[this->animIndex], 0);
            this->hasBeenHit = true;
            this->actionFunc = DmStk_ClockTower_Idle;
            return;
        }
        DmStk_ClockTower_AdjustHeightAndRotation(this, play);
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        this->tatlMessageTimer++;
        if (this->tatlMessageTimer > 800) {
            this->tatlMessageTimer = 0;
            if (!(player->stateFlags2 & ACTOR_FLAG_LOCK_ON_DISABLED)) {
                // Why are you just standing around?
                Message_StartTextbox(play, 0x2014, &this->actor);
            }
        }

        if ((this->collider.base.acFlags & AC_HIT) && (this->actor.colChkInfo.damageEffect == 0xF)) {
            this->hasBeenHit = true;
            this->actionFunc = DmStk_ClockTower_StartDropOcarinaCutscene;
        }
    }
}
/*
void DmStk_UpdateCollision(DmStk* this, PlayState* play) {
    s32 pad;

    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);

    if (play->sceneId == SCENE_OKUJOU && !rando_location_is_checked(GI_OCARINA_OF_TIME)) {
        this->handType = SK_HAND_TYPE_HOLDING_OCARINA;
    }
}*/

/**
 * Updates a variety of states based on Skull Kid's current cutscene, including his current
 * animation, his hand/mask type, his fade in/fade out state, and his current cutscene action.
 */
/*void DmStk_HandleCutscene(DmStk* this, PlayState* play) {
    s32 pad;
    s32 cueChannel;

    if (Cutscene_IsCueInChannel(play, CS_CMD_ACTOR_CUE_107)) {
        cueChannel = Cutscene_GetCueChannel(play, CS_CMD_ACTOR_CUE_107);

        if (play->csCtx.curFrame == play->csCtx.actorCues[cueChannel]->startFrame) {
            if (this->cueId != play->csCtx.actorCues[cueChannel]->id) {
                this->cueId = play->csCtx.actorCues[cueChannel]->id;
                if (play->sceneId == SCENE_CLOCKTOWER) {
                    this->handType = SK_HAND_TYPE_HOLDING_FLUTE;
                } else {
                    this->handType = SK_HAND_TYPE_DEFAULT;
                }

                switch (play->csCtx.actorCues[cueChannel]->id) {
                    case 0:
                    case 1:
                        this->animIndex = SK_ANIM_IDLE;
                        break;

                    case 2:
                        this->animIndex = SK_ANIM_WALK;
                        break;

                    case 3:
                        this->animIndex = SK_ANIM_UNUSED_KICK_OVER_LINK;
                        break;

                    case 4:
                        this->animIndex = SK_ANIM_KICK_OVER_LINK;
                        break;

                    case 6:
                        this->animIndex = SK_ANIM_PICK_UP_OCARINA;
                        this->handType = SK_HAND_TYPE_HOLDING_OCARINA;
                        break;

                    case 7:
                        this->animIndex = SK_ANIM_PICK_UP_OCARINA_AND_START_PLAYING;
                        this->handType = SK_HAND_TYPE_HOLDING_OCARINA;
                        break;

                    case 8:
                        this->animIndex = SK_ANIM_SHAKE_HEAD;
                        break;

                    case 9:
                        this->animIndex = SK_ANIM_IDLE;
                        this->fadeInState = SK_FADE_IN_STATE_START;
                        break;

                    case 12:
                        this->animIndex = SK_ANIM_HIDE_OCARINA_START;
                        this->handType = SK_HAND_TYPE_HOLDING_OCARINA;
                        break;

                    case 13:
                        this->animIndex = SK_ANIM_JUMP_WHILE_HIDING_OCARINA;
                        this->handType = SK_HAND_TYPE_HOLDING_OCARINA;
                        break;

                    case 14:
                        this->animIndex = SK_ANIM_HORSE_RIDE_START;
                        break;

                    case 15:
                        this->animIndex = SK_ANIM_HORSE_RIDE_LOOP;
                        break;

                    case 16:
                        this->animIndex = SK_ANIM_HORSE_RIDE_AND_ROTATE;
                        break;

                    case 17:
                        this->animIndex = SK_ANIM_RECLINING_FLOAT;
                        break;

                    case 18:
                        this->animIndex = SK_ANIM_CURSE_START;
                        break;

                    case 19:
                        this->animIndex = SK_ANIM_LAUGH_START;
                        break;

                    case 20:
                        this->animIndex = SK_ANIM_RAISE_MASK_START;
                        break;

                    case 21:
                        this->animIndex = SK_ANIM_LOWER_MASK;
                        break;

                    case 22:
                        this->animIndex = SK_ANIM_PLAY_OCARINA_WHILE_FLOATING;
                        //if (gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] == ITEM_NONE) {
                        if (!rando_location_is_checked(GI_OCARINA_OF_TIME) || !rando_location_is_checked(0x040067)) {
                            this->handType = SK_HAND_TYPE_HOLDING_OCARINA;
                        }
                        break;

                    case 23:
                        this->animIndex = SK_ANIM_FLOATING_TURN_AROUND;
                        this->handType = SK_HAND_TYPE_JUGGLING_OR_DROPPING_OCARINA;
                        break;

                    case 24:
                        this->animIndex = SK_ANIM_CALL_DOWN_MOON_START;
                        //if (gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] == ITEM_NONE) {
                        if (!rando_location_is_checked(GI_OCARINA_OF_TIME) || !rando_location_is_checked(0x040067)) {
                            this->handType = SK_HAND_TYPE_HOLDING_OCARINA;
                        }
                        break;

                    case 25:
                        Actor_Kill(&this->actor);
                        break;

                    case 26:
                        this->animIndex = SK_ANIM_SMACK_FAIRY_START;
                        //if (gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] == ITEM_NONE) {
                        if (!rando_location_is_checked(GI_OCARINA_OF_TIME) || !rando_location_is_checked(0x040067)) {
                            this->handType = SK_HAND_TYPE_HOLDING_OCARINA;
                        }
                        break;

                    case 27:
                        this->animIndex = SK_ANIM_HIT_BY_BUBBLE;
                        //if (gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] == ITEM_NONE) {
                        if (!rando_location_is_checked(GI_OCARINA_OF_TIME) || !rando_location_is_checked(0x040067)) {
                            this->handType = SK_HAND_TYPE_HOLDING_OCARINA;
                        }
                        break;

                    case 28:
                        this->animIndex = SK_ANIM_DROP_OCARINA;
                        this->handType = SK_HAND_TYPE_JUGGLING_OR_DROPPING_OCARINA;
                        break;

                    case 30:
                        this->animIndex = SK_ANIM_FLOATING_ARMS_CROSSED;
                        break;

                    case 31:
                        this->animIndex = SK_ANIM_DEFLECT_ATTACK;
                        break;

                    case 32:
                        this->animIndex = SK_ANIM_SURPRISE_START;
                        break;

                    case 33:
                        this->animIndex = SK_ANIM_LOOK_AROUND_FOR_GIANTS_START;
                        break;

                    case 34:
                        this->animIndex = SK_ANIM_HOLD_HEAD_AND_SHAKE_START;
                        break;

                    case 35:
                        this->animIndex = SK_ANIM_HOLD_HEAD_AND_SCREAM_START;
                        break;

                    case 36:
                        this->animIndex = SK_ANIM_HUDDLE_WITH_FAIRIES;
                        break;

                    case 37:
                        this->animIndex = SK_ANIM_SEARCH_MASK_SALESMAN;
                        break;

                    case 38:
                        this->animIndex = SK_ANIM_HOLD_UP_MASK_START;
                        break;

                    case 39:
                        this->animIndex = SK_ANIM_SHIVER;
                        break;

                    case 40:
                        this->animIndex = SK_ANIM_DRAW;
                        break;

                    case 41:
                        this->animIndex = SK_ANIM_TELESCOPE_LOOK_UP_START;
                        break;

                    case 42:
                        this->animIndex = SK_ANIM_BENT_OVER_HEAD_TWITCH;
                        break;

                    case 43:
                        this->animIndex = SK_ANIM_BENT_OVER_LOOK_UP;
                        break;

                    case 44:
                        this->animIndex = SK_ANIM_SPANK;
                        break;

                    case 45:
                        this->animIndex = SK_ANIM_HIP_SHAKE_AND_JUMP;
                        this->fadeOutState = SK_FADE_OUT_STATE_FADING_OUT;
                        break;

                    case 46:
                        this->handType = SK_HAND_TYPE_HOLDING_FLUTE;
                        this->animIndex = SK_ANIM_PLAY_FLUTE;
                        break;

                    case 47:
                        this->animIndex = SK_ANIM_CARTWHEEL;
                        break;

                    case 48:
                        this->animIndex = SK_ANIM_CARTWHEEL;
                        break;

                    case 49:
                        this->animIndex = SK_ANIM_LOOK_UP_AT_GIANTS;
                        break;

                    case 50:
                        this->animIndex = SK_ANIM_ASHAMED_START;
                        break;

                    case 51:
                        this->animIndex = SK_ANIM_LOOK_LEFT_START;
                        break;

                    case 52:
                        this->animIndex = SK_ANIM_SNIFF;
                        break;

                    case 53:
                        this->animIndex = SK_ANIM_CARTWHEEL;
                        break;

                    case 54:
                        this->animIndex = SK_ANIM_LIE_FLAT;
                        break;

                    case 55:
                        this->animIndex = SK_ANIM_DANGLE_FROM_MASK_START;
                        break;

                    case 56:
                        this->animIndex = SK_ANIM_DROPPED_FROM_MASK;
                        break;

                    case 57:
                        this->animIndex = SK_ANIM_LOOK_UP_AT_GIANTS;
                        break;

                    case 58:
                        this->animIndex = SK_ANIM_ASHAMED_START;
                        break;

                    case 59:
                        this->animIndex = SK_ANIM_LOOK_LEFT_START;
                        break;

                    case 60:
                        this->animIndex = SK_ANIM_SNIFF;
                        break;

                    case 5:
                    case 10:
                        break;

                    default:
                        this->animIndex = SK_ANIM_SHAKE_HEAD;
                        break;
                }

                DmStk_ChangeAnim(this, play, &this->skelAnime, &sAnimationInfo[this->animIndex], 0);
            }
        }

        Cutscene_ActorTranslateAndYaw(&this->actor, play, cueChannel);
    } else {
        this->cueId = 99;
    }

    if (this->fadeInState == SK_FADE_IN_STATE_START) {
        Math_SmoothStepToF(&this->fogScale, 0.7f, 0.1f, 0.007f, 0.005f);
        if (this->fogScale < 0.71f) {
            this->fogScale = 0.7f;
            this->fogN = 800;
            this->fadeInState++;
        }

        this->fogR = play->lightCtx.fogColor[0] * this->fogScale;
        this->fogG = play->lightCtx.fogColor[1] * this->fogScale;
        this->fogB = play->lightCtx.fogColor[2] * this->fogScale;
    } else if (this->fadeInState == SK_FADE_IN_STATE_INCREASE_FOG) {
        if (this->fogN < 996) {
            this->fogN += 10;
        }

        if (this->fogN > 996) {
            this->fadeInState++;
            this->fogN = 996;
        }
    } else if (this->fadeInState == SK_FADE_IN_STATE_INCREASE_ALPHA) {
        if (this->alpha < 128) {
            this->alpha += 3;
        }

        if (this->alpha < 255) {
            this->alpha += 20;
        } else {
            this->alpha = 255;
            this->fadeInState = SK_FADE_IN_STATE_NONE;
        }
    }

    if (this->fadeOutState == SK_FADE_OUT_STATE_FADING_OUT) {
        if (this->fadeOutTimer > 40) {
            this->maskType = SK_MASK_TYPE_FADING_OUT;
        }

        this->fadeOutTimer++;
        if (this->fadeOutTimer >= 44) {
            this->alpha -= 35;
            if (this->alpha < 0) {
                this->alpha = 0;
                this->fadeOutState = SK_FADE_OUT_STATE_NONE;
                SET_WEEKEVENTREG(WEEKEVENTREG_12_04);
                if (!(play->actorCtx.flags & ACTORCTX_FLAG_1)) {
                    Actor_Kill(&this->actor);
                } else {
                    this->shouldDraw = false;
                }
            }
        }
    }

    if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
        switch (this->animIndex) {
            case SK_ANIM_LOOK_AROUND:
            case SK_ANIM_CURSE_START:
            case SK_ANIM_LAUGH_START:
            case SK_ANIM_HIDE_OCARINA_START:
            case SK_ANIM_PICK_UP_OCARINA_AND_START_PLAYING:
            case SK_ANIM_PICK_UP_OCARINA:
            case SK_ANIM_KICK_OVER_LINK:
            case SK_ANIM_UNUSED_KICK_OVER_LINK:
            case SK_ANIM_HORSE_RIDE_START:
            case SK_ANIM_RAISE_MASK_START:
            case SK_ANIM_FLOATING_TURN_AROUND:
            case SK_ANIM_CALL_DOWN_MOON_START:
            case SK_ANIM_SMACK_FAIRY_START:
            case SK_ANIM_TELESCOPE_LOOK_UP_START:
            case SK_ANIM_SURPRISE_START:
            case SK_ANIM_LOOK_AROUND_FOR_GIANTS_START:
            case SK_ANIM_HOLD_HEAD_AND_SHAKE_START:
            case SK_ANIM_HOLD_HEAD_AND_SCREAM_START:
            case SK_ANIM_HOLD_UP_MASK_START:
            case SK_ANIM_DANGLE_FROM_MASK_START:
            case SK_ANIM_ASHAMED_START:
            case SK_ANIM_LOOK_LEFT_START:
            case SK_ANIM_SNIFF:
                this->animIndex++;
                DmStk_ChangeAnim(this, play, &this->skelAnime, &sAnimationInfo[this->animIndex], 0);
                break;

            case SK_ANIM_LOWER_MASK:
                this->animIndex = SK_ANIM_IDLE;
                this->maskType = SK_MASK_TYPE_NORMAL;
                this->handType = SK_HAND_TYPE_DEFAULT;
                DmStk_ChangeAnim(this, play, &this->skelAnime, &sAnimationInfo[this->animIndex], 0);
                break;

            default:
                break;
        }
    }

    if (((this->animIndex == SK_ANIM_RAISE_MASK_START) && (this->skelAnime.curFrame >= 16.0f)) ||
        (this->animIndex == SK_ANIM_RAISE_MASK_LOOP) || (this->animIndex == SK_ANIM_LOWER_MASK)) {
        this->maskType = SK_MASK_TYPE_RAISED;
        this->handType = SK_HAND_TYPE_HOLDING_MAJORAS_MASK;
    } else if (((this->animIndex >= SK_ANIM_HUDDLE_WITH_FAIRIES) && (this->animIndex <= SK_ANIM_DRAW)) ||
               ((this->animIndex >= SK_ANIM_PLAY_FLUTE) && (this->animIndex <= SK_ANIM_CARTWHEEL)) ||
               ((play->sceneId == SCENE_00KEIKOKU) && (gSaveContext.sceneLayer == 7))) {
        this->maskType = SK_MASK_TYPE_NO_MASK;
        if ((this->animIndex == SK_ANIM_HOLD_UP_MASK_START) || (this->animIndex == SK_ANIM_HOLD_UP_MASK_LOOP)) {
            this->handType = SK_HAND_TYPE_HOLDING_MAJORAS_MASK_AND_FLUTE;
        }
    }

    if (this->animIndex == SK_ANIM_DROPPED_FROM_MASK) {
        this->maskType = SK_MASK_TYPE_NO_MASK;
    }
}*/

extern Gfx gSkullKidMajorasMask1DL[];

s32 DmStk_OverrideLimbDrawOpa(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx);
void DmStk_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx, Gfx** gfx);
void DmStk_PostLimbDrawOpa(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx);

RECOMP_PATCH void DmStk_Draw(Actor* thisx, PlayState* play) {
    DmStk* this = THIS;

    if (this->shouldDraw) {
        if (DM_STK_GET_TYPE(&this->actor) == DM_STK_TYPE_MAJORAS_MASK) {
            Gfx_DrawDListOpa(play, gSkullKidMajorasMask1DL);
            return;
        }

        gSegments[0x06] = OS_K0_TO_PHYSICAL(play->objectCtx.slots[this->objectStkObjectSlot].segment);

        OPEN_DISPS(play->state.gfxCtx);

        this->alpha = this->alpha; // Set to itself
        Gfx_SetupDL25_Opa(play->state.gfxCtx);

        if (this->alpha < 255) {
            Gfx_SetupDL25_Xlu(play->state.gfxCtx);
            Scene_SetRenderModeXlu(play, 1, 2);

            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetEnvColor(POLY_XLU_DISP++, 0, 0, 0, this->alpha);

            POLY_XLU_DISP =
                SkelAnime_DrawFlex(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                   this->skelAnime.dListCount, NULL, DmStk_PostLimbDraw, &this->actor, POLY_XLU_DISP);
        } else {
            Scene_SetRenderModeXlu(play, 0, 1);

            gDPPipeSync(POLY_OPA_DISP++);
            gDPSetEnvColor(POLY_OPA_DISP++, 255, 255, 255, 255);

            if (this->skelAnime.skeleton) {
                SkelAnime_DrawFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                      this->skelAnime.dListCount, DmStk_OverrideLimbDrawOpa, DmStk_PostLimbDrawOpa,
                                      &this->actor);
            }
        }

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

/*void DmStk_Update(Actor* thisx, PlayState* play) {
    DmStk* this = THIS;

    if (DM_STK_GET_TYPE(&this->actor) != DM_STK_TYPE_MAJORAS_MASK) {
        if (this->animIndex == SK_ANIM_CALL_DOWN_MOON_LOOP) {
            Actor_SetFocus(&this->actor, 40.0f);
        } else {
            Actor_SetFocus(&this->actor, 6.0f);
        }

        DmStk_LoadObjectForAnimation(this, play);

        if (this->animIndex != SK_ANIM_LIE_FLAT) {
            SkelAnime_Update(&this->skelAnime);
        }

        this->alpha = this->alpha;

        this->actionFunc(this, play);

        if (play->sceneId == SCENE_OKUJOU) {
            DmStk_UpdateCollision(this, play);
        }

        DmStk_HandleCutscene(this, play);
        DmStk_PlaySfxForCutscenes(this, play);

        // This handles the cutscene where the player takes out the Deku Pipes for the first time.
        switch (this->dekuPipesCutsceneState) {
            case SK_DEKU_PIPES_CS_STATE_READY:
                if (Actor_OcarinaInteractionAccepted(&this->actor, &play->state)) {
                    this->dekuPipesCutsceneState = SK_DEKU_PIPES_CS_STATE_PLAYER_USED_OCARINA;
                } else {
                    Actor_OfferOcarinaInteraction(&this->actor, play, this->actor.xzDistToPlayer, fabsf(this->actor.playerHeightRel));
                }
                break;

            case SK_DEKU_PIPES_CS_STATE_PLAYER_USED_OCARINA:
                if (CutsceneManager_IsNext(16)) {
                    this->dekuPipesCutsceneState = SK_DEKU_PIPES_CS_STATE_START;
                    CutsceneManager_Start(16, &this->actor);
                    this->actionFunc = DmStk_ClockTower_Idle;
                } else {
                    CutsceneManager_Queue(16);
                }
                break;

            case SK_DEKU_PIPES_CS_STATE_START:
                if (play->csCtx.state == CS_STATE_IDLE) {
                    this->dekuPipesCutsceneState = SK_DEKU_PIPES_CS_STATE_END;
                }
                break;

            default:
                break;
        }

        // This code is responsible for making in-game time pass while using the telescope in the Astral Observatory.
        // Skull Kid is always loaded in the scene, even if he isn't visible, hence why time always passes.
        if ((play->actorCtx.flags & ACTORCTX_FLAG_1) && (play->msgCtx.msgMode != MSGMODE_NONE) &&
            (play->msgCtx.currentTextId == 0x5E6) && !FrameAdvance_IsEnabled(&play->state) &&
            (play->transitionTrigger == TRANS_TRIGGER_OFF) && (CutsceneManager_GetCurrentCsId() == CS_ID_NONE) &&
            (play->csCtx.state == CS_STATE_IDLE)) {
            gSaveContext.save.time = ((void)0, gSaveContext.save.time) + (u16)R_TIME_SPEED;
            if (R_TIME_SPEED != 0) {
                gSaveContext.save.time =
                    ((void)0, gSaveContext.save.time) + (u16)((void)0, gSaveContext.save.timeSpeedOffset);
            }
        }
    }

    if ((play->sceneId == SCENE_00KEIKOKU) && (gSaveContext.sceneLayer == 3) && (play->csCtx.scriptIndex > 0)) {
        play->envCtx.skyboxConfig = 15;
        play->envCtx.changeSkyboxNextConfig = 15;
    }
}*/
/*
void DmStk_ClockTower_StartIntroCutsceneVersion1(DmStk* this, PlayState* play) {
    if (rando_location_is_checked(0x00004C)) {
        skull_kid_has_ocarina = INV_CONTENT(ITEM_OCARINA_OF_TIME) == ITEM_NONE;
        DmStk_ClockTower_StartIntroCutsceneVersion2(this, play);
        return;
    }
    if (CutsceneManager_IsNext(9)) {
        CutsceneManager_Start(9, &this->actor);
        if (play->csCtx.state == CS_STATE_IDLE) {
            if (!skull_kid_has_ocarina) {
                INV_CONTENT(ITEM_OCARINA_OF_TIME) = ITEM_OCARINA_OF_TIME;
            }
            this->actionFunc = DmStk_ClockTower_WaitForIntroCutsceneVersion1ToEnd;
        }
    } else {
        CutsceneManager_Queue(9);
    }
}

void DmStk_ClockTower_StartIntroCutsceneVersion2(DmStk* this, PlayState* play) {
    if (!rando_location_is_checked(0x00004C)) {
        skull_kid_has_ocarina = INV_CONTENT(ITEM_OCARINA_OF_TIME) == ITEM_NONE;
        DmStk_ClockTower_StartIntroCutsceneVersion1(this, play);
        return;
    }
    if (CutsceneManager_IsNext(11)) {
        CutsceneManager_Start(11, &this->actor);
        this->actionFunc = DmStk_ClockTower_WaitForIntroCutsceneVersion2ToEnd;
    } else {
        CutsceneManager_Queue(11);
    }
}

void DmStk_ClockTower_StartDropOcarinaCutscene(DmStk* this, PlayState* play) {
    if (CutsceneManager_IsNext(10)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_STALKIDS_DAMAGE);
        CutsceneManager_Start(10, &this->actor);
        this->actor.shape.rot.x = 0;
        this->actor.world.rot.x = this->actor.shape.rot.x;
        this->actor.shape.rot.y = this->actor.shape.rot.x;
        this->actor.world.rot.y = this->actor.shape.rot.x;
        this->actionFunc = DmStk_ClockTower_WaitForDropOcarinaCutsceneToEnd;
    } else {
        if (!skull_kid_has_ocarina) {
            INV_CONTENT(ITEM_OCARINA_OF_TIME) = ITEM_NONE;
        }
        CutsceneManager_Queue(10);
    }
}

void DmStk_ClockTower_WaitForDropOcarinaCutsceneToEnd(DmStk* this, PlayState* play) {
    if ((play->csCtx.state != CS_STATE_IDLE) && (play->csCtx.curFrame > 20)) {
        if (!skull_kid_has_ocarina) {
            INV_CONTENT(ITEM_OCARINA_OF_TIME) = ITEM_OCARINA_OF_TIME;
        }
        this->actionFunc = DmStk_ClockTower_Idle;
    }
}*/