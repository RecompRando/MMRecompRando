#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define LOCATION_SONG_ELEGY 0x040064

#define ACTOR_FLAG_ATTENTION_ENABLED ACTOR_FLAG_TARGETABLE

#define KNIGHT_LIMB_MAX 0x1D

struct EnKnight;

typedef void (*EnKnightActionFunc)(struct EnKnight*, PlayState*);

typedef enum {
    /* 0 */ KNIGHT_CLOSE_CURTAIN_ACTION_0,
    /* 1 */ KNIGHT_CLOSE_CURTAIN_ACTION_1,
    /* 2 */ KNIGHT_CLOSE_CURTAIN_ACTION_2
} EnKnightCloseCurtainAction;

typedef struct EnKnight {
    /* 0x000 */ Actor actor;
    /* 0x144 */ u16 curtainsLightRayAngle;
    /* 0x146 */ s16 randTimer;
    /* 0x148 */ u8 subAction;
    /* 0x14A */ s16 timers[3];
    /* 0x150 */ UNK_TYPE1 unk_150;
    /* 0x151 */ u8 inCurtainCutscene;
    /* 0x152 */ u8 doBgChecks;
    /* 0x153 */ u8 closeCurtainAction;
    /* 0x154 */ u8 roomLightingState;
    /* 0x156 */ s16 invincibilityTimer;
    /* 0x158 */ s16 damageFlashTimer;
    /* 0x15A */ s16 strafeTarget;
    /* 0x15C */ s16 strafeMaxStep;
    /* 0x15E */ s16 effectTimer;
    /* 0x160 */ s16 breathAlpha;
    /* 0x162 */ u8 isHeadless;
    /* 0x164 */ Vec3f afterImageOffset;
    /* 0x170 */ s16 pitchToPlayer;
    /* 0x172 */ s16 yawToPlayer;
    /* 0x174 */ s16 leftLegUpperRotation;
    /* 0x176 */ s16 rightLegUpperRotation;
    /* 0x178 */ s16 leftLegLowerRotation;
    /* 0x17A */ s16 rightLegLowerRotation;
    /* 0x17C */ f32 bodyAlpha;
    /* 0x180 */ f32 shadowAlphaFactor;
    /* 0x184 */ s16 neckYaw;
    /* 0x186 */ s16 neckYawTarget;
    /* 0x188 */ s16 neckRoll;
    /* 0x18A */ s16 neckRollTarget;
    /* 0x18C */ s16 neckRotationMaxStep;
    /* 0x18E */ s16 neckRotationStepScale;
    /* 0x190 */ u8 swordColliderActive;
    /* 0x191 */ u8 shieldColliderActive;
    /* 0x192 */ u8 canRetreat;
    /* 0x193 */ u8 shieldingInvulnerabilityTimer;
    /* 0x194 */ SkelAnime skelAnime;
    /* 0x1D8 */ f32 animEndFrame;
    /* 0x1DC */ Vec3f bodyPartsPos[15];
    /* 0x290 */ u8 igosCurtainReaction;
    /* 0x291 */ u8 lightRayDamageTimer;
    /* 0x292 */ u8 retreatWhileShielding;
    /* 0x294 */ f32 drawDmgEffScale;
    /* 0x298 */ f32 drawDmgEffIceMeltingScale;
    /* 0x29C */ f32 drawDmgEffAlpha;
    /* 0x2A0 */ u8 drawDmgEffType;
    /* 0x2A1 */ u8 drawDmgEffState;
    /* 0x2A2 */ s16 drawDmgEffDuration;
    /* 0x2A4 */ f32 animMovementX;
    /* 0x2A8 */ f32 animMovementZ;
    /* 0x2AC */ Vec3f feetPositions[2];
    /* 0x2C4 */ Vec3s jointTable[KNIGHT_LIMB_MAX];
    /* 0x372 */ Vec3s morphTable[KNIGHT_LIMB_MAX];
    /* 0x420 */ EnKnightActionFunc actionFunc;
    /* 0x424 */ EnKnightActionFunc prevActionFunc;
    /* 0x428 */ s16 jawRotation;
    /* 0x42C */ f32 jawRotationAmplitude;
    /* 0x430 */ f32 jawRotationAmplitudeTarget;
    /* 0x434 */ Vec3f breathBasePos;
    /* 0x440 */ Vec3f retreatTowards;
    /* 0x44C */ u8 blureState;
    /* 0x450 */ f32 blureAlpha;
    /* 0x454 */ Vec3f blureTranslation;
    /* 0x460 */ Vec3f blureRotation;
    /* 0x46C */ f32 swordScale;
    /* 0x470 */ f32 shieldScale;
    /* 0x474 */ f32 shieldLightReflectionScale;
    /* 0x478 */ s16 shieldHitTimer;
    /* 0x47C */ Vec3f shieldParticlesPos;
    /* 0x488 */ ColliderCylinder headAttackCollider;
    /* 0x4D4 */ ColliderJntSph swordCollider;
    /* 0x4F4 */ ColliderJntSphElement swordColliderElements[1];
    /* 0x534 */ ColliderJntSph shieldCollider;
    /* 0x554 */ ColliderJntSphElement shieldColliderElements[1];
    /* 0x594 */ ColliderJntSph bodyCollider;
    /* 0x5B4 */ ColliderJntSphElement bodyColliderElements[2];
    /* 0x634 */ ColliderCylinder headCollider;
    /* 0x680 */ u8 inCurtainReaction;
    /* 0x684 */ u32 csTimer;
    /* 0x688 */ s16 csState;
    /* 0x68A */ s16 subCamId;
    /* 0x68C */ Vec3f subCamEye;
    /* 0x698 */ Vec3f subCamAt;
    /* 0x6A4 */ f32 csStepValue;
    /* 0x6A8 */ UNK_TYPE4 unk_6A8;
    /* 0x6AC */ s16 csPlayerYaw;
    /* 0x6B0 */ f32 subCamFov;
    /* 0x6B4 */ u16 walkSfx;
    /* 0x6B6 */ u16 pauseSfx;
    /* 0x6B8 */ u16 laughSfx;
    /* 0x6BA */ UNK_TYPE2 unk_6BA; // unused sfx id space?
    /* 0x6BC */ u16 attackSfx;
    /* 0x6BE */ u16 damagedSfx;
    /* 0x6C0 */ u16 defeatedSfx;
    /* 0x6C2 */ u16 voiceSfx;
    /* 0x6C4 */ u8 seenCaptainsHat;
    /* 0x6C8 */ Vec3f projectedPos;
} EnKnight; // size = 0x6D4

// extern EnKnight* sIgosInstance;
// extern EnKnight* sThinKnightInstance;
// extern EnKnight* sWideKnightInstance;
// extern EnKnight* sIgosHeadInstance;
// extern EnKnight* sTargetKnight; // During some actions, the other knight will use this knight as a target
// extern EnKnightEffect sEnKnightEffects[100];
extern EnKnight* D_809BEFD0;
extern EnKnight* D_809BEFD4;
extern EnKnight* D_809BEFD8;
extern EnKnight* D_809BEFDC;
extern EnKnight* D_809BEFE0; // During some actions, the other knight will use this knight as a target
// extern u8 sKnightMusicStartTimer;
extern u8 D_809BDB00;

static Vec3f sZeroVec = { 0.0f, 0.0f, 0.0f };

#define KNIGHT_INTRO_CS_STATE_0 0x00
#define EVENTINF_INTRO_CS_WATCHED_IGOS_DU_IKANA 0x57

// void EnKnight_SetupIgosSitting(EnKnight* this, PlayState* play);
// void EnKnight_TurnToPlayer(EnKnight* this, PlayState* play);
void func_809B6528(EnKnight* this, PlayState* play);
void func_809B41F8(EnKnight* this, PlayState* play);

// void EnKnight_IntroCutscene(EnKnight* this, PlayState* play) {
RECOMP_PATCH void func_809B8458(EnKnight* this, PlayState* play) {
    f32 scale = 0.0f;
    Player* player = GET_PLAYER(play);

    this->csTimer++;
    SkelAnime_Update(&this->skelAnime);

    switch (this->csState) {
        case KNIGHT_INTRO_CS_STATE_0:
            // If the room clear flag is set, or there is a cs playing, don't do anything
            if (Flags_GetClear(play, play->roomCtx.curRoom.num) || (CutsceneManager_GetCurrentCsId() != CS_ID_NONE)) {
                break;
            }

            // fancy effect to darken room over time
            if (this->csTimer < 70) {
                play->envCtx.lightSettingOverride = ((this->csTimer / 10) % 10) + 1;
            }

            D_809BEFD4->actor.world.pos.x = (BREG(30) + 1363.0f + 30.0f) - 40.0f;
            D_809BEFD8->actor.world.pos.x = (BREG(30) + 1363.0f + 30.0f) - 40.0f;
            D_809BEFD4->actor.world.pos.y = 45.0f;
            D_809BEFD8->actor.world.pos.y = 45.0f;
            D_809BEFD4->actor.world.pos.z = ((BREG(31) + 2864.0f) - 60.0f) - 10.0f;
            D_809BEFD8->actor.world.pos.z = BREG(31) + 2864.0f + 60.0f + 10.0f;
            this->actor.world.pos.x = BREG(27) + 1376.0f;
            this->actor.world.pos.y = 45.0f;
            this->actor.world.pos.z = BREG(28) + 2864.0f;

            // disable targeting when too far
            this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            D_809BEFD4->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            D_809BEFD8->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;

            // this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_BGM_MAIN, 80);

            this->csTimer = 0;
            this->closeCurtainAction = KNIGHT_CLOSE_CURTAIN_ACTION_2;
            // play->envCtx.lightSettingOverride = 7;
            play->envCtx.lightBlendOverride = 0;
            // sIgosInstance->roomLightingState = 0;
            D_809BEFD0->roomLightingState = 0;

            this->csState = KNIGHT_INTRO_CS_STATE_0;

            // If the player is too far away, don't start the fight
            if (!(player->actor.world.pos.x >= 870.0f)) {
                break;
            }

            // reenable targetting
            this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            D_809BEFD4->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
            D_809BEFD8->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;

            // EnKnight_SetupIgosSitting(this, play);
            // EnKnight_SetupTurnToPlayer(D_809BEFD4, play);
            // EnKnight_SetupTurnToPlayer(D_809BEFD8, play);
            func_809B6528(this, play);
            func_809B41F8(D_809BEFD4, play);
            func_809B41F8(D_809BEFD8, play);

            D_809BEFD4->actor.gravity = D_809BEFD8->actor.gravity = -1.5f;
            D_809BEFD4->doBgChecks = D_809BEFD8->doBgChecks = true;

            // sKnightMusicStartTimer = 1;
            D_809BDB00 = 1;
            SET_EVENTINF(EVENTINF_INTRO_CS_WATCHED_IGOS_DU_IKANA);
            break;
    }
}

// void EnKnight_Dead(EnKnight* this, PlayState* play);
void func_809B58D4(EnKnight* this, PlayState* play);

void EnKnight_OfferElegy(EnKnight* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = func_809B58D4;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_SONG_ELEGY), LOCATION_SONG_ELEGY, 1000.0f, 1000.0f, true, true);
    }
}

// void EnKnight_Die(EnKnight* this, PlayState* play) {
RECOMP_PATCH void func_809B5698(EnKnight* this, PlayState* play) {
    Vec3f accel;
    Vec3f pos;
    s16 bodyPartIndex;

    if ((this->timers[2] >= 10) && (this->timers[2] < 40)) {
        if (this->timers[2] == 10) {
            // sTargetKnight = NULL;
            D_809BEFE0 = NULL;
        } else {
            // sTargetKnight = this;
            D_809BEFE0 = this;
        }
    }

    if (this->timers[2] == 0) {
        this->doBgChecks = false;
        this->actor.draw = NULL;
        this->actor.world.pos.y = 1000.0f;
        this->actor.world.pos.x = 1300.0f;
        this->actor.world.pos.z = 2864.0f;
        this->actor.gravity = 0.0f;
    }

    Math_ApproachZeroF(&this->actor.scale.y, 0.1f, 0.0015f);

    if (this->timers[2] != 0) {
        bodyPartIndex = (s32)Rand_ZeroFloat(ARRAY_COUNT(this->bodyPartsPos) - 0.1f);
        accel = sZeroVec;
        accel.y = 0.03f;
        pos.x = this->bodyPartsPos[bodyPartIndex].x + Rand_CenteredFloat(20.0f);
        pos.y = this->bodyPartsPos[bodyPartIndex].y - 5.0f;
        pos.z = this->bodyPartsPos[bodyPartIndex].z + Rand_CenteredFloat(20.0f);
        EffectSsKFire_Spawn(play, &pos, &sZeroVec, &accel, Rand_ZeroFloat(20.0f) + 20.0f, 0);
        Actor_PlaySfx(&this->actor, NA_SE_EN_COMMON_EXTINCT_LEV - SFX_FLAG);
    }

    if (this->timers[2] == 0) {
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;

        // if (this == sIgosInstance) {
        if (this == D_809BEFD0) {
            if (!rando_location_is_checked(LOCATION_SONG_ELEGY)) {
                this->actionFunc = EnKnight_OfferElegy;
            } else {
                // this->actionFunc = EnKnight_Dead;
                this->actionFunc = func_809B58D4;
            }
        } else {
            // this->actionFunc = EnKnight_Dead;
            this->actionFunc = func_809B58D4;
        }

        // Actor_ChangeCategory(play, &play->actorCtx, &this->actor, ACTORCAT_ENEMY);
        func_800BC154(play, &play->actorCtx, &this->actor, ACTORCAT_ENEMY);
        this->timers[0] = 30;
    }
}

#include "overlays/actors/ovl_Boss_06/z_boss_06.h"

// extern EnKnight* sIgosInstance;
extern EnKnight* D_809F4970;

void func_809F2E14(Boss06* this, PlayState* play);

// slight bug where lighting doesn't update after burning if igos isn't active
RECOMP_PATCH void func_809F2C44(Boss06* this, PlayState* play) {
    if (D_809F4970->closeCurtainAction != KNIGHT_CLOSE_CURTAIN_ACTION_0) {
        this->unk_A28 = 0.008f * 2;
        Math_ApproachF(&this->unk_1E4, 0.0f, 0.2f, this->unk_A28 * 30.0f);
        Math_ApproachF(&this->unk_1AC, -70.0f, 0.2f, this->unk_A28 * 70.0f);
        Math_ApproachF(&this->unk_1A8, 0.0f, 0.2f, this->unk_A28 * 110.0f);
        Math_ApproachF(&this->unk_1A0, -900.0f, 0.2f, this->unk_A28 * 900.0f);
        Math_ApproachF(&this->unk_1A4, 1350.0f, 0.2f, this->unk_A28 * 1350.0f);
        Math_ApproachF(&this->unk_1E0, 100.0f, 0.2f, this->unk_A28 * 100.0f);

        if (this->unk_1E4 < 5.0f) {
            Math_ApproachF(&this->unk_19C, 0.0f, 1.0f, 0.03f);
        }

        if ((this->unk_1E4 > 0.1f) && ENBOSS06_GET_PARAMS(&this->actor) == 0) {
            Audio_PlaySfx(NA_SE_EV_CURTAIN_DOWN - SFX_FLAG);
        }

        if (this->unk_1E4 <= 0.1f) {
            Actor_Kill(this->actor.child);
            this->actor.child = NULL;
            func_809F2E14(this, play);
        }
    }
}