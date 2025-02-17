#include "modding.h"
#include "global.h"

#include "apcommon.h"

#include "z64rumble.h"
#include "z64shrink_window.h"
#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"
#include "overlays/actors/ovl_En_Clear_Tag/z_en_clear_tag.h"
#include "overlays/actors/ovl_En_Tanron1/z_en_tanron1.h"
#include "overlays/actors/ovl_Item_B_Heart/z_item_b_heart.h"

#define ODOLWA_LIMB_NONE 0x00
#define ODOLWA_LIMB_ROOT 0x01
#define ODOLWA_LIMB_MAX 0x34
#define ODOLWA_BUG_LIMB_MAX 0x15

struct Boss01;

#define FLAGS                                                                                 \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_HOSTILE | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_DRAW_CULLING_DISABLED)

#define THIS ((Boss01*)thisx)

#define ODOLWA_EFFECT_COUNT 100

// This actor has an array of timers in its instance, but it only ever uses the first entry.
#define TIMER_CURRENT_ACTION 0
#define TIMER_BUG_CURRENT_ACTION 0
#define TIMER_AFTERIMAGE_DESPAWN 0

#define ODOLWA_GET_TYPE(thisx) ((thisx)->params)
#define ODOLWA_GET_AFTERIMAGE_DESPAWN_TIMER(thisx) ((thisx)->world.rot.z)

#define ODOLWA_SHADOW_TEX_WIDTH 64
#define ODOLWA_SHADOW_TEX_HEIGHT 64
#define ODOLWA_SHADOW_TEX_SIZE ((s32)sizeof(u8[ODOLWA_SHADOW_TEX_HEIGHT][ODOLWA_SHADOW_TEX_WIDTH]))

typedef void (*Boss01ActionFunc)(struct Boss01*, PlayState*);

typedef enum OdolwaType {
    /*  0 */ ODOLWA_TYPE_ODOLWA,
    /* 10 */ ODOLWA_TYPE_BUG = 10,
    /* 35 */ ODOLWA_TYPE_AFTERIMAGE = 35
} OdolwaType;

typedef enum OdolwaBodyPart {
    /*  0 */ ODOLWA_BODYPART_HEAD,
    /*  1 */ ODOLWA_BODYPART_TORSO,
    /*  2 */ ODOLWA_BODYPART_PELVIS,
    /*  3 */ ODOLWA_BODYPART_LEFT_UPPER_ARM,
    /*  4 */ ODOLWA_BODYPART_LEFT_FOREARM,
    /*  5 */ ODOLWA_BODYPART_SHIELD,
    /*  6 */ ODOLWA_BODYPART_RIGHT_UPPER_ARM,
    /*  7 */ ODOLWA_BODYPART_RIGHT_FOREARM,
    /*  8 */ ODOLWA_BODYPART_SWORD,
    /*  9 */ ODOLWA_BODYPART_RIGHT_THIGH,
    /* 10 */ ODOLWA_BODYPART_RIGHT_SHIN,
    /* 11 */ ODOLWA_BODYPART_RIGHT_FOOT,
    /* 12 */ ODOLWA_BODYPART_LEFT_THIGH,
    /* 13 */ ODOLWA_BODYPART_LEFT_SHIN,
    /* 14 */ ODOLWA_BODYPART_LEFT_FOOT,
    /* 15 */ ODOLWA_BODYPART_MAX
} OdolwaBodyPart;

typedef enum OdolwaColliderBodyPart {
    /*  0 */ ODOLWA_COLLIDER_BODYPART_HEAD,
    /*  1 */ ODOLWA_COLLIDER_BODYPART_TORSO,
    /*  2 */ ODOLWA_COLLIDER_BODYPART_PELVIS,
    /*  3 */ ODOLWA_COLLIDER_BODYPART_LEFT_UPPER_ARM,
    /*  4 */ ODOLWA_COLLIDER_BODYPART_LEFT_FOREARM,
    /*  5 */ ODOLWA_COLLIDER_BODYPART_RIGHT_UPPER_ARM,
    /*  6 */ ODOLWA_COLLIDER_BODYPART_RIGHT_FOREARM,
    /*  7 */ ODOLWA_COLLIDER_BODYPART_LEFT_THIGH,
    /*  8 */ ODOLWA_COLLIDER_BODYPART_LEFT_SHIN,
    /*  9 */ ODOLWA_COLLIDER_BODYPART_RIGHT_THIGH,
    /* 10 */ ODOLWA_COLLIDER_BODYPART_RIGHT_SHIN,
    /* 11 */ ODOLWA_COLLIDER_BODYPART_MAX
} OdolwaColliderBodyPart;

typedef enum OdolwaSwordCollider {
    /* 0 */ ODOLWA_SWORD_COLLIDER_SWORD_BASE,
    /* 1 */ ODOLWA_SWORD_COLLIDER_SWORD_TIP,
    /* 2 */ ODOLWA_SWORD_COLLIDER_PELVIS,
    /* 3 */ ODOLWA_SWORD_COLLIDER_MAX
} OdolwaSwordCollider;

typedef enum OdolwaShieldCollider {
    /* 0 */ ODOLWA_SHIELD_COLLIDER_SHIELD,
    /* 1 */ ODOLWA_SHIELD_COLLIDER_MAX
} OdolwaShieldCollider;

typedef enum OdolwaKickAndShieldBashCollider {
    /* 0 */ ODOLWA_KICK_AND_SHIELD_BASH_COLLIDER_SHIELD,
    /* 1 */ ODOLWA_KICK_AND_SHIELD_BASH_COLLIDER_LEFT_FOOT,
    /* 2 */ ODOLWA_KICK_AND_SHIELD_BASH_COLLIDER_MAX
} OdolwaKickAndShieldBashCollider;

typedef struct Boss01 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ s16 frameCounter;
    /* 0x148 */ s32 phaseFrameCounter;
    /* 0x14C */ union {
                    u8 jumpIfPlayerIsClose;
                    u8 shouldPerformFallingSlash;
                    u8 bugDrawDmgEffType;
                };
    /* 0x14E */ s16 timers[3];
    /* 0x154 */ f32 animMorphFrames1;
    /* 0x158 */ f32 animMorphFrames2;
    /* 0x15C */ s16 damageTimer;
    /* 0x15E */ s16 damageFlashTimer;
    /* 0x160 */ u8 isPerformingVerticalSlash; // set, but never checked
    /* 0x160 */ u8 landedFromJump;
    /* 0x162 */ u8 waitType;
    /* 0x163 */ u8 lookAtPlayer;
    /* 0x164 */ SkelAnime skelAnime;
    /* 0x1A8 */ f32 animEndFrame;
    /* 0x1AC */ f32 prevJumpVelocityY;
    /* 0x1B0 */ f32 runTargetPosAngularVelocityY;
    /* 0x1B4 */ f32 runTargetPosRotY;
    /* 0x1B8 */ u8 swordState;
    /* 0x1B9 */ u8 kickAndShieldBashCollisionEnabled;
    /* 0x1BA */ u8 swordAndShieldCollisionEnabled;
    /* 0x1BB */ u8 canGuardOrEvade;
    /* 0x1BC */ u8 bodyInvincibilityTimer;
    /* 0x1BE */ s16 disableCollisionTimer;
    /* 0x1C0 */ u8 afterimageSpawnFrameMask; // used as a bitmask with frameCounter to control how often to spawn afterimages
    /* 0x1C1 */ u8 hasPlayedSummonBugCs;
    /* 0x1C4 */ f32 additionalVelocityX;
    /* 0x1C8 */ f32 additionalVelocityZ;
    /* 0x1CC */ s16 headRotY;
    /* 0x1CE */ s16 headRotX;
    /* 0x1D0 */ Vec3s jointTable[ODOLWA_LIMB_MAX];
    /* 0x308 */ Vec3s morphTable[ODOLWA_LIMB_MAX];
    /* 0x440 */ Vec3f feetPos[2];
    /* 0x458 */ Vec3f pelvisPos;
    /* 0x464 */ Boss01ActionFunc actionFunc;
    /* 0x468 */ ColliderJntSph swordCollider;
    /* 0x488 */ ColliderJntSphElement swordColliderElements[ODOLWA_SWORD_COLLIDER_MAX];
    /* 0x548 */ ColliderJntSph shieldCollider;
    /* 0x568 */ ColliderJntSphElement shieldColliderElements[ODOLWA_SHIELD_COLLIDER_MAX];
    /* 0x5A8 */ ColliderJntSph bodyCollider;
    /* 0x5C8 */ ColliderJntSphElement bodyColliderElements[ODOLWA_COLLIDER_BODYPART_MAX];
    /* 0x888 */ ColliderJntSph kickAndShieldBashCollider;
    /* 0x8A8 */ ColliderJntSphElement kickAndShieldBashColliderElements[ODOLWA_KICK_AND_SHIELD_BASH_COLLIDER_MAX];
    /* 0x928 */ Vec3f bodyPartsPos[ODOLWA_BODYPART_MAX];
    /* 0x9DC */ union {
                    u32 cutsceneTimer;
                    u32 waitTimer;
                    u32 summonMothsTimer;
                };
    /* 0x9E0 */ s16 cutsceneState;
    /* 0x9E2 */ s16 subCamId;
    /* 0x9E4 */ Vec3f subCamEye;
    /* 0x9F0 */ Vec3f subCamAt;
    /* 0x9FC */ Vec3f subCamUp;
    /* 0xA08 */ Vec3f subCamEyeNext;
    /* 0xA14 */ f32 deathCsInitialSubCamRot;
    /* 0xA18 */ f32 deathCsSubCamRot;
    /* 0xA1C */ f32 subCamVelocity;
    /* 0xA20 */ f32 deathShrinkSpeed;
    /* 0xA24 */ f32 screenShakeOffsetY;
    /* 0xA28 */ f32 screenShakeMagnitude;
    /* 0xA2C */ ColliderCylinder bugACCollider;
    /* 0xA78 */ ColliderCylinder bugATCollider;
    /* 0xAC4 */ f32 bugDrawDmgEffScale;
    /* 0xAC8 */ f32 bugDrawDmgEffAlpha;
    /* 0xACC */ f32 drawDmgEffScale;
    /* 0xAD0 */ f32 drawDmgEffFrozenSteamScale;
    /* 0xAD4 */ f32 drawDmgEffAlpha;
    /* 0xAD8 */ u8 drawDmgEffType; 
    /* 0xAD9 */ u8 drawDmgEffState;
    /* 0xADA */ s16 drawDmgEffTimer;
} Boss01; // size = 0xADC

typedef enum {
    // Named based on the fact that everything with this damage effect deals zero damage. If this effect is given to an
    // attack that deals non-zero damage, it will behave exactly like ODOLWA_DMGEFF_DAMAGE.
    /* 0x0 */ ODOLWA_DMGEFF_IMMUNE,

    // Deals no damage, but turns Odolwa blue, stops all animations, and makes him wait in place for 40 frames.
    /* 0x1 */ ODOLWA_DMGEFF_STUN,

    // Deals damage and surrounds Odolwa with fire.
    /* 0x2 */ ODOLWA_DMGEFF_FIRE,

    // Behaves exactly like ODOLWA_DMGEFF_STUN, but also surrounds Odolwa with ice.
    /* 0x3 */ ODOLWA_DMGEFF_FREEZE,

    // Deals damage and surrounds Odolwa with yellow light orbs.
    /* 0x4 */ ODOLWA_DMGEFF_LIGHT_ORB,

    // Behaves exactly like ODOLWA_DMGEFF_STUN, but also surrounds Odolwa in electric sparks.
    /* 0xB */ ODOLWA_DMGEFF_ELECTRIC_STUN = 0xB,

    // Deals damage and surrounds Odolwa with blue light orbs.
    /* 0xC */ ODOLWA_DMGEFF_BLUE_LIGHT_ORB,

    // Deals damage and has no special effect.
    /* 0xD */ ODOLWA_DMGEFF_DAMAGE,

    // Deals damage and checks the timer that tracks how long Odolwa should be in his damaged state. If the timer is 7
    // or more, it will reset the timer to 20 frames keep Odolwa in the damaged state for longer. If the timer is 6 or
    // less, it will disable Odolwa's collision for 20 frames to ensure he can jump away without taking further damage.
    /* 0xE */ ODOLWA_DMGEFF_DAMAGE_TIMER_CHECK,

    // Deals no damage, but makes Odolwa play his dazed animation for 70 frames and be vulnerable to attacks.
    /* 0xF */ ODOLWA_DMGEFF_DAZE
} OdolwaDamageEffect;

static DamageTable sOdolwaDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, ODOLWA_DMGEFF_STUN),
    /* Deku Stick     */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAZE),
    /* Horse trample  */ DMG_ENTRY(0, ODOLWA_DMGEFF_IMMUNE),
    /* Explosives     */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAMAGE_TIMER_CHECK),
    /* Zora boomerang */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAZE),
    /* Normal arrow   */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAZE),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, ODOLWA_DMGEFF_IMMUNE),
    /* Hookshot       */ DMG_ENTRY(0, ODOLWA_DMGEFF_IMMUNE),
    /* Goron punch    */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAMAGE_TIMER_CHECK),
    /* Sword          */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAMAGE_TIMER_CHECK),
    /* Goron pound    */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAZE),
    /* Fire arrow     */ DMG_ENTRY(2, ODOLWA_DMGEFF_FIRE),
    /* Ice arrow      */ DMG_ENTRY(2, ODOLWA_DMGEFF_FREEZE),
    /* Light arrow    */ DMG_ENTRY(2, ODOLWA_DMGEFF_LIGHT_ORB),
    /* Goron spikes   */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAZE),
    /* Deku spin      */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAZE),
    /* Deku bubble    */ DMG_ENTRY(0, ODOLWA_DMGEFF_STUN),
    /* Deku launch    */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAMAGE_TIMER_CHECK),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, ODOLWA_DMGEFF_STUN),
    /* Zora barrier   */ DMG_ENTRY(0, ODOLWA_DMGEFF_ELECTRIC_STUN),
    /* Normal shield  */ DMG_ENTRY(0, ODOLWA_DMGEFF_IMMUNE),
    /* Light ray      */ DMG_ENTRY(0, ODOLWA_DMGEFF_IMMUNE),
    /* Thrown object  */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAZE),
    /* Zora punch     */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAMAGE_TIMER_CHECK),
    /* Spin attack    */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAMAGE),
    /* Sword beam     */ DMG_ENTRY(2, ODOLWA_DMGEFF_BLUE_LIGHT_ORB),
    /* Normal Roll    */ DMG_ENTRY(69, ODOLWA_DMGEFF_BLUE_LIGHT_ORB),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, ODOLWA_DMGEFF_IMMUNE),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, ODOLWA_DMGEFF_IMMUNE),
    /* Unblockable    */ DMG_ENTRY(0, ODOLWA_DMGEFF_IMMUNE),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, ODOLWA_DMGEFF_IMMUNE),
    /* Powder Keg     */ DMG_ENTRY(1, ODOLWA_DMGEFF_DAMAGE_TIMER_CHECK),
};

// The limbs referenced here are not used. The spheres are positioned manually by Boss01_PostLimbDraw.
static ColliderJntSphElementInit sSwordColliderJntSphElementsInit[3] = {
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x04, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_NONE, { { 0, 0, 0 }, 35 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x04, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_NONE, { { 0, 0, 0 }, 35 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x04, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_NONE, { { 0, 0, 0 }, 70 }, 100 },
    },
};

static ColliderJntSphInit sSwordColliderJntSphInit = {
    {
        COL_MATERIAL_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sSwordColliderJntSphElementsInit),
    sSwordColliderJntSphElementsInit,
};

// The limbs referenced here are not used. The spheres are positioned manually by Boss01_PostLimbDraw.
static ColliderJntSphElementInit sShieldColliderJntSphElementsInit[1] = {
    {
        {
            ELEM_MATERIAL_UNK2,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_NONE, { { 0, 0, 0 }, 36 }, 100 },
    },
};

static ColliderJntSphInit sShieldColliderJntSphInit = {
    {
        COL_MATERIAL_METAL,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_HARD | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sShieldColliderJntSphElementsInit),
    sShieldColliderJntSphElementsInit,
};

// The limbs referenced here are not used. The spheres are positioned manually by Boss01_PostLimbDraw.
static ColliderJntSphElementInit sBodyColliderJntSphElementsInit[11] = {
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_NONE, { { 0, 0, 0 }, 20 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_ROOT, { { 0, 0, 0 }, 30 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_ROOT, { { 0, 0, 0 }, 25 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_ROOT, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_ROOT, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_ROOT, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_ROOT, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_ROOT, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_ROOT, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_ROOT, { { 0, 0, 0 }, 15 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x00, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_NORMAL,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_ROOT, { { 0, 0, 0 }, 15 }, 100 },
    },
};

static ColliderJntSphInit sBodyColliderJntSphInit = {
    {
        COL_MATERIAL_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sBodyColliderJntSphElementsInit),
    sBodyColliderJntSphElementsInit,
};

// The limbs referenced here are not used. The spheres are positioned manually by Boss01_PostLimbDraw.
static ColliderJntSphElementInit sKickAndShieldBashColliderJntSphElementsInit[2] = {
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x04, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_HARD,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_NONE, { { 0, 0, 0 }, 36 }, 100 },
    },
    {
        {
            ELEM_MATERIAL_UNK3,
            { 0xF7CFFFFF, 0x04, 0x04 },
            { 0xF7CFFFFF, 0x00, 0x00 },
            ATELEM_ON | ATELEM_SFX_HARD,
            ACELEM_ON,
            OCELEM_ON,
        },
        { ODOLWA_LIMB_NONE, { { 0, 0, 0 }, 36 }, 100 },
    },
};

static ColliderJntSphInit sKickAndShieldBashColliderJntSphInit = {
    {
        COL_MATERIAL_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_JNTSPH,
    },
    ARRAY_COUNT(sKickAndShieldBashColliderJntSphElementsInit),
    sKickAndShieldBashColliderJntSphElementsInit,
};

static ColliderCylinderInit sBugACColliderCylinderInit = {
    {
        COL_MATERIAL_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK3,
        { 0xF7CFFFFF, 0x00, 0x04 },
        { 0xF7FFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 15, 15, 10, { 0, 0, 0 } },
};

static ColliderCylinderInit sBugATColliderCylinderInit = {
    {
        COL_MATERIAL_HIT3,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK3,
        { 0xF7CFFFFF, 0x00, 0x04 },
        { 0xF7FFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 8, 15, 10, { 0, 0, 0 } },
};

typedef enum {
    // Named based on the fact that everything with this damage effect deals zero damage. If this effect is given to an
    // attack that deals non-zero damage, it will behave exactly like BUG_DMGEFF_DAMAGE.
    /* 0x0 */ BUG_DMGEFF_IMMUNE,

    // Deals no damage, but turns the bug blue, stops all animations, and makes it wait in place for 40 frames.
    /* 0x1 */ BUG_DMGEFF_STUN,

    // Named after the only attack that uses it. Behaves exactly like BUG_DMGEFF_DAMAGE.
    /* 0x2 */ BUG_DMGEFF_FIRE_ARROW,

    // Named after the only attack that uses it. Behaves exactly like BUG_DMGEFF_DAMAGE.
    /* 0x3 */ BUG_DMGEFF_ICE_ARROW,

    // Named after the only attack that uses it. Behaves exactly like BUG_DMGEFF_DAMAGE.
    /* 0x4 */ BUG_DMGEFF_LIGHT_ARROW,

    // Named after the only two attacks that use it. Behaves exactly like BUG_DMGEFF_DAMAGE.
    /* 0xD */ BUG_DMGEFF_SPIN_ATTACK_AND_SWORD_BEAM = 0xD,

    // Named after the only attack that uses it. Behaves exactly like BUG_DMGEFF_DAMAGE.
    /* 0xE */ BUG_DMGEFF_ZORA_PUNCH,

    // Deals damage and has no special effect.
    /* 0xF */ BUG_DMGEFF_DAMAGE
} BugDamageEffect;

static DamageTable sBugDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, BUG_DMGEFF_STUN),
    /* Deku Stick     */ DMG_ENTRY(1, BUG_DMGEFF_DAMAGE),
    /* Horse trample  */ DMG_ENTRY(0, BUG_DMGEFF_IMMUNE),
    /* Explosives     */ DMG_ENTRY(2, BUG_DMGEFF_DAMAGE),
    /* Zora boomerang */ DMG_ENTRY(0, BUG_DMGEFF_STUN),
    /* Normal arrow   */ DMG_ENTRY(2, BUG_DMGEFF_DAMAGE),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, BUG_DMGEFF_IMMUNE),
    /* Hookshot       */ DMG_ENTRY(0, BUG_DMGEFF_IMMUNE),
    /* Goron punch    */ DMG_ENTRY(1, BUG_DMGEFF_DAMAGE),
    /* Sword          */ DMG_ENTRY(1, BUG_DMGEFF_DAMAGE),
    /* Goron pound    */ DMG_ENTRY(1, BUG_DMGEFF_DAMAGE),
    /* Fire arrow     */ DMG_ENTRY(2, BUG_DMGEFF_FIRE_ARROW),
    /* Ice arrow      */ DMG_ENTRY(2, BUG_DMGEFF_ICE_ARROW),
    /* Light arrow    */ DMG_ENTRY(2, BUG_DMGEFF_LIGHT_ARROW),
    /* Goron spikes   */ DMG_ENTRY(1, BUG_DMGEFF_DAMAGE),
    /* Deku spin      */ DMG_ENTRY(0, BUG_DMGEFF_STUN),
    /* Deku bubble    */ DMG_ENTRY(0, BUG_DMGEFF_STUN),
    /* Deku launch    */ DMG_ENTRY(1, BUG_DMGEFF_DAMAGE),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, BUG_DMGEFF_STUN),
    /* Zora barrier   */ DMG_ENTRY(0, BUG_DMGEFF_STUN),
    /* Normal shield  */ DMG_ENTRY(0, BUG_DMGEFF_IMMUNE),
    /* Light ray      */ DMG_ENTRY(0, BUG_DMGEFF_IMMUNE),
    /* Thrown object  */ DMG_ENTRY(1, BUG_DMGEFF_DAMAGE),
    /* Zora punch     */ DMG_ENTRY(1, BUG_DMGEFF_ZORA_PUNCH),
    /* Spin attack    */ DMG_ENTRY(2, BUG_DMGEFF_SPIN_ATTACK_AND_SWORD_BEAM),
    /* Sword beam     */ DMG_ENTRY(2, BUG_DMGEFF_SPIN_ATTACK_AND_SWORD_BEAM),
    /* Normal Roll    */ DMG_ENTRY(69, BUG_DMGEFF_DAMAGE),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, BUG_DMGEFF_IMMUNE),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, BUG_DMGEFF_IMMUNE),
    /* Unblockable    */ DMG_ENTRY(0, BUG_DMGEFF_IMMUNE),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, BUG_DMGEFF_IMMUNE),
    /* Powder Keg     */ DMG_ENTRY(1, BUG_DMGEFF_DAMAGE),
};

typedef enum {
    /* 0 */ ODOLWA_EFFECT_NONE,
    /* 1 */ ODOLWA_EFFECT_FALLING_BLOCK,   // The blocks that fall from the ceiling and their fragments
    /* 3 */ ODOLWA_EFFECT_RING_OF_FIRE = 3 // The ring of fire that surrounds the player
} OdolwaEffectType;

typedef struct {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ Vec3f velocity;
    /* 0x18 */ f32 gravity;
    /* 0x1C */ s16 rotX;
    /* 0x1E */ s16 rotY;
    /* 0x20 */ UNK_TYPE1 unk_20[0x8];
    /* 0x28 */ u8 type;
    /* 0x2A */ s16 timer;
    /* 0x2C */ s16 isFallingBlockFragment;
    /* 0x2E */ s16 angularVelocityX;
    /* 0x30 */ s16 angularVelocityY;
    /* 0x34 */ f32 scale;
    /* 0x38 */ f32 alpha;
} OdolwaEffect; // size = 0x3C

typedef enum {
    /*   0 */ ODOLWA_WAIT_READY,
    /*   1 */ ODOLWA_WAIT_SPIN_SWORD,
    /*   2 */ ODOLWA_WAIT_VERTICAL_HOP,
    /*   3 */ ODOLWA_WAIT_SHAKE_DANCE,
    /*   4 */ ODOLWA_WAIT_UP_AND_DOWN_DANCE,
    /*   5 */ ODOLWA_WAIT_ARM_SWING_DANCE,
    /*   6 */ ODOLWA_WAIT_THRUST_ATTACK,
    /*   7 */ ODOLWA_WAIT_DOUBLE_SLASH,
    /*   8 */ ODOLWA_WAIT_SIDE_TO_SIDE_HOP,
    /*   9 */ ODOLWA_WAIT_SIDE_TO_SIDE_DANCE,
    /*  10 */ ODOLWA_WAIT_SPIN_DANCE,
    /*  11 */ ODOLWA_WAIT_JUMP_DANCE,
    /*  12 */ ODOLWA_WAIT_MAX,

    // This doesn't correspond to an actual wait action that Odolwa can perform, but it can be passed as a parameter to
    // Boss01_SetupWait to randomly select between one of Odolwa's available wait types (as well as having a random
    // chance to summon moths instead), assuming that the fight is in its second phase.
    /* 100 */ ODOLWA_WAIT_RANDOM = 100
} OdolwaWaitType;

typedef enum {
    /*  0 */ ODOLWA_DRAW_DMGEFF_STATE_NONE,
    /*  1 */ ODOLWA_DRAW_DMGEFF_STATE_FIRE_INIT,
    /*  2 */ ODOLWA_DRAW_DMGEFF_STATE_FIRE_ACTIVE,
    /* 10 */ ODOLWA_DRAW_DMGEFF_STATE_FROZEN_INIT = 10,
    /* 11 */ ODOLWA_DRAW_DMGEFF_STATE_FROZEN_ACTIVE,
    /* 20 */ ODOLWA_DRAW_DMGEFF_STATE_LIGHT_ORB_INIT = 20,
    /* 21 */ ODOLWA_DRAW_DMGEFF_STATE_LIGHT_ORB_ACTIVE,
    /* 30 */ ODOLWA_DRAW_DMGEFF_STATE_BLUE_LIGHT_ORB_INIT = 30,
    /* 40 */ ODOLWA_DRAW_DMGEFF_STATE_ELECTRIC_SPARKS_INIT = 40,
    /* 41 */ ODOLWA_DRAW_DMGEFF_STATE_ELECTRIC_SPARKS_ACTIVE
} OdolwaDrawDmgEffState;

extern Boss01* sOdolwa;
extern EnTanron1* sMothSwarm;

extern OdolwaEffect sOdolwaEffects[];

extern Vec3f sOdolwaDamageSfxPos;

extern u8 sOdolwaMusicStartTimer;
extern s16 sOdolwaBugCount;

extern FlexSkeletonHeader gOdolwaSkel;
extern AnimationHeader gOdolwaReadyAnim;

extern FlexSkeletonHeader gOdolwaBugSkel;
extern AnimationHeader gOdolwaBugCrawlAnim;

void Boss01_SetupIntroCutscene(Boss01* this, PlayState* play);
void Boss01_Afterimage_SetupWaitToDespawn(Boss01* this, PlayState* play);
void Boss01_SetupWait(Boss01* this, PlayState* play, u8 waitType);
void Boss01_SetupDeathCutscene(Boss01* this, PlayState* play);
void Boss01_SetupStunned(Boss01* this, PlayState* play);
void Boss01_Afterimage_Draw(Actor* thisx, PlayState* play);
void Boss01_Bug_SetupCrawl(Boss01* this, PlayState* play);
void Boss01_Bug_Update(Actor* thisx, PlayState* play);
void Boss01_Bug_Draw(Actor* thisx, PlayState* play);

void Boss01_SetupDazed(Boss01* this, PlayState* play);
void Boss01_SetupDamaged(Boss01* this, PlayState* play, u8 damageEffect);

RECOMP_PATCH void Boss01_Init(Actor* thisx, PlayState* play) {
    Boss01* this = THIS;
    s32 pad;
    s16 i;

    Actor_SetScale(&this->actor, 0.015f);
    if (ODOLWA_GET_TYPE(&this->actor) == ODOLWA_TYPE_BUG) {
        SkelAnime_InitFlex(play, &this->skelAnime, &gOdolwaBugSkel, &gOdolwaBugCrawlAnim, this->jointTable,
                           this->morphTable, ODOLWA_BUG_LIMB_MAX);
        this->actor.update = Boss01_Bug_Update;
        this->actor.draw = Boss01_Bug_Draw;
        this->bugDrawDmgEffType = ACTOR_DRAW_DMGEFF_BLUE_FIRE;
        Boss01_Bug_SetupCrawl(this, play);
        this->actor.gravity = -2.0f;
        Collider_InitAndSetCylinder(play, &this->bugACCollider, &this->actor, &sBugACColliderCylinderInit);
        Collider_InitAndSetCylinder(play, &this->bugATCollider, &this->actor, &sBugATColliderCylinderInit);
        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 12.0f);
        Actor_SetScale(&this->actor, 0.025f);
        this->actor.colChkInfo.health = 2;
        sOdolwaBugCount++;
        Actor_PlaySfx(&this->actor, NA_SE_EV_ROCK_FALL);
        this->actor.colChkInfo.damageTable = &sBugDamageTable;
        sOdolwa->actor.hintId = TATL_HINT_ID_ODOLWA_PHASE_TWO;
    } else if (ODOLWA_GET_TYPE(&this->actor) == ODOLWA_TYPE_AFTERIMAGE) {
        SkelAnime_InitFlex(play, &this->skelAnime, &gOdolwaSkel, &gOdolwaReadyAnim, this->jointTable, this->morphTable,
                           ODOLWA_LIMB_MAX);
        Boss01_Afterimage_SetupWaitToDespawn(this, play);
        this->timers[TIMER_AFTERIMAGE_DESPAWN] = ODOLWA_GET_AFTERIMAGE_DESPAWN_TIMER(&this->actor);
        this->actor.world.rot.z = 0;
        this->actor.draw = Boss01_Afterimage_Draw;
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    } else {
        if (CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE)) {
            Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_DOOR_WARP1, 0.0f, 0.0f, 0.0f, 0, 0, 0,
                               ENDOORWARP1_FF_1);
            Actor_Spawn(&play->actorCtx, play, ACTOR_ITEM_B_HEART, 0.0f, 0.0f, 250.0f, 0, 0, 0, BHEART_PARAM_NORMAL);
            Actor_Kill(&this->actor);
            return;
        }

        play->envCtx.lightSettingOverride = 0;
        play->envCtx.lightBlendOverride = LIGHT_BLEND_OVERRIDE_FULL_CONTROL;
        play->envCtx.lightBlend = 0.0f;
        sOdolwa = this;
        sOdolwaBugCount = 0;
        play->specialEffects = sOdolwaEffects;

        for (i = 0; i < ODOLWA_EFFECT_COUNT; i++) {
            sOdolwaEffects[i].type = ODOLWA_EFFECT_NONE;
        }

        this->actor.hintId = TATL_HINT_ID_ODOLWA_PHASE_ONE;
        this->actor.attentionRangeType = ATTENTION_RANGE_5;
        this->actor.colChkInfo.mass = MASS_HEAVY;
        this->actor.colChkInfo.damageTable = &sOdolwaDamageTable;
        this->actor.colChkInfo.health = 20;

        ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.0f);
        Collider_InitAndSetJntSph(play, &this->swordCollider, &this->actor, &sSwordColliderJntSphInit,
                                  this->swordColliderElements);
        Collider_InitAndSetJntSph(play, &this->shieldCollider, &this->actor, &sShieldColliderJntSphInit,
                                  this->shieldColliderElements);
        Collider_InitAndSetJntSph(play, &this->bodyCollider, &this->actor, &sBodyColliderJntSphInit,
                                  this->bodyColliderElements);
        Collider_InitAndSetJntSph(play, &this->kickAndShieldBashCollider, &this->actor,
                                  &sKickAndShieldBashColliderJntSphInit, this->kickAndShieldBashColliderElements);
        SkelAnime_InitFlex(play, &this->skelAnime, &gOdolwaSkel, &gOdolwaReadyAnim, this->jointTable, this->morphTable,
                           ODOLWA_LIMB_MAX);

        if (true || (KREG(64) != 0) || CHECK_EVENTINF(EVENTINF_INTRO_CS_WATCHED_ODOLWA)) {
            Boss01_SetupWait(this, play, ODOLWA_WAIT_READY);
            this->actor.gravity = -2.5f;
            sOdolwaMusicStartTimer = 0;
            SEQCMD_PLAY_SEQUENCE(SEQ_PLAYER_BGM_MAIN, 0, NA_BGM_BOSS | SEQ_FLAG_ASYNC);
        } else {
            Boss01_SetupIntroCutscene(this, play);
        }

        sMothSwarm = (EnTanron1*)Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_TANRON1, 0.0f, 437.0f,
                                                    -750.0f, 0, 0, 0, 0x100);
    }

    this->animMorphFrames1 = -2.0f;
    this->animMorphFrames2 = -4.0f;
}

RECOMP_PATCH void Boss01_UpdateDamage(Boss01* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    u8 damage;
    s32 i;

    if (this->shieldCollider.elements[ODOLWA_SHIELD_COLLIDER_SHIELD].base.acElemFlags & ACELEM_HIT) {
        this->bodyInvincibilityTimer = 5;
        if (this->damageTimer == 0) {
            ColliderElement* acHitElem = this->shieldCollider.elements[ODOLWA_SHIELD_COLLIDER_SHIELD].base.acHitElem;

            if (acHitElem->atDmgInfo.dmgFlags == DMG_SWORD_BEAM) {
                Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->actor.focus.pos.x, this->actor.focus.pos.y,
                            this->actor.focus.pos.z, 0, 0, 3, CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
                Actor_PlaySfx(&this->actor, NA_SE_IT_SHIELD_BOUND);
                this->damageTimer = 5;
            }
        }
    } else if (this->damageTimer == 0) {
        for (i = 0; i < ODOLWA_SWORD_COLLIDER_MAX; i++) {
            if (this->swordCollider.elements[i].base.atElemFlags & ATELEM_HIT) {
                this->swordCollider.elements[i].base.atElemFlags &= ~ATELEM_HIT;
                player->pushedYaw = this->actor.yawTowardsPlayer;
                player->pushedSpeed = 15.0f;
            }
        }

        for (i = 0; i < ODOLWA_KICK_AND_SHIELD_BASH_COLLIDER_MAX; i++) {
            if (this->kickAndShieldBashCollider.elements[i].base.atElemFlags & ATELEM_HIT) {
                this->kickAndShieldBashCollider.elements[i].base.atElemFlags &= ~ATELEM_HIT;
                player->pushedYaw = this->actor.yawTowardsPlayer;
                player->pushedSpeed = 20.0f;
            }
        }

        for (i = 0; i < ODOLWA_COLLIDER_BODYPART_MAX; i++) {
            if (this->bodyCollider.elements[i].base.acElemFlags & ACELEM_HIT) {
                this->bodyCollider.elements[i].base.acElemFlags &= ~ACELEM_HIT;

                switch (this->actor.colChkInfo.damageEffect) {
                    case ODOLWA_DMGEFF_FREEZE:
                        this->drawDmgEffState = ODOLWA_DRAW_DMGEFF_STATE_FROZEN_INIT;
                        goto stunned;

                    case ODOLWA_DMGEFF_FIRE:
                        this->drawDmgEffState = ODOLWA_DRAW_DMGEFF_STATE_FIRE_INIT;
                        break;

                    case ODOLWA_DMGEFF_LIGHT_ORB:
                        this->drawDmgEffState = ODOLWA_DRAW_DMGEFF_STATE_LIGHT_ORB_INIT;
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->actor.focus.pos.x,
                                    this->actor.focus.pos.y, this->actor.focus.pos.z, 0, 0, 0,
                                    CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
                        break;

                    case ODOLWA_DMGEFF_ELECTRIC_STUN:
                        this->drawDmgEffState = ODOLWA_DRAW_DMGEFF_STATE_ELECTRIC_SPARKS_INIT;
                        goto stunned;

                    case ODOLWA_DMGEFF_BLUE_LIGHT_ORB:
                        this->drawDmgEffState = ODOLWA_DRAW_DMGEFF_STATE_BLUE_LIGHT_ORB_INIT;
                        Actor_Spawn(&play->actorCtx, play, ACTOR_EN_CLEAR_TAG, this->actor.focus.pos.x,
                                    this->actor.focus.pos.y, this->actor.focus.pos.z, 0, 0, 3,
                                    CLEAR_TAG_PARAMS(CLEAR_TAG_LARGE_LIGHT_RAYS));
                        break;

                    case ODOLWA_DMGEFF_STUN:
                    stunned:
                        Boss01_SetupStunned(this, play);
                        this->damageTimer = 15;
                        Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_FREEZE);
                        this->canGuardOrEvade = false;
                        return;

                    default:
                        break;
                }

                damage = this->actor.colChkInfo.damage;
                sMothSwarm->unk_148 = 0;

                if (damage == 3) {
                    this->actor.colChkInfo.health = 0;
                }

                if (this->actor.colChkInfo.damageEffect == ODOLWA_DMGEFF_STUN) {
                    //! @bug: unreachable code. If Odolwa's damage effect is ODOLWA_DMGEFF_STUN, we early-return out of
                    //! the function in the above switch statement.
                    Boss01_SetupStunned(this, play);
                    this->damageTimer = 15;
                } else if (this->actor.colChkInfo.damageEffect == ODOLWA_DMGEFF_DAZE) {
                    Boss01_SetupDazed(this, play);
                    Audio_PlaySfx_AtPos(&sOdolwaDamageSfxPos, NA_SE_EN_MIBOSS_DAMAGE_OLD);
                    this->damageTimer = 15;
                } else {
                    this->damageFlashTimer = 15;
                    this->damageTimer = 5;
                    this->actor.colChkInfo.health -= damage;
                    if ((s8)this->actor.colChkInfo.health <= 0) {
                        Boss01_SetupDeathCutscene(this, play);
                        Actor_PlaySfx(&this->actor, NA_SE_EN_MIBOSS_DEAD_OLD);
                        Enemy_StartFinishingBlow(play, &this->actor);
                    } else {
                        Boss01_SetupDamaged(this, play, this->actor.colChkInfo.damageEffect);
                        Audio_PlaySfx_AtPos(&sOdolwaDamageSfxPos, NA_SE_EN_MIBOSS_DAMAGE_OLD);
                    }
                }

                this->canGuardOrEvade = false;
                break;
            }
        }
    }
}