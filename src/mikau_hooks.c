#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY)

#define THIS ((EnZog*)thisx)

struct EnZog;

typedef void (*EnZogActionFunc)(struct EnZog*, PlayState*);

#define ENZOG_GET_F(thisx) ((thisx)->params & 0xF)
#define ENZOG_GET_FC00(thisx) (((thisx)->params & 0xFC00) >> 0xA)

#define ENZOG_F_2 2
#define ENZOG_FC00_63 63

typedef struct EnZog {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ Vec3s jointTable[23];
    /* 0x25E */ Vec3s morphTable[23];
    /* 0x2E8 */ Path* unk_2E8;
    /* 0x2EC */ s32 unk_2EC;
    /* 0x2F0 */ Vec3f unk_2F0;
    /* 0x2FC */ s16 unk_2FC;
    /* 0x2FE */ s16 unk_2FE;
    /* 0x300 */ s16 unk_300;
    /* 0x302 */ s16 unk_302;
    /* 0x304 */ s16 unk_304;
    /* 0x306 */ s16 cueId;
    /* 0x308 */ s16 unk_308;
    /* 0x30A */ u16 unk_30A;
    /* 0x30C */ s16 csIdList[7];
    /* 0x31A */ s16 csIdIndex;
    /* 0x31C */ s16 unk_31C;
    /* 0x31E */ s16 unk_31E;
    /* 0x320 */ s16 unk_320;
    /* 0x322 */ s16 unk_322;
    /* 0x324 */ s16 unk_324;
    /* 0x328 */ EnZogActionFunc actionFunc;
} EnZog; // size = 0x32C

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 30, 40, 0, { 0, 0, 0 } },
};

static u8 D_80B95E10;

extern u64 object_zog_Tex_024750[];
extern u64 object_zog_Tex_024F50[];
extern u64 object_zog_Tex_025750[];

static TexturePtr D_80B958AC[] = { object_zog_Tex_024750, object_zog_Tex_024F50, object_zog_Tex_025750 };

extern u64 object_zog_Tex_025F50[];
extern u64 object_zog_Tex_026750[];

static TexturePtr D_80B958B8[] = { object_zog_Tex_025F50, object_zog_Tex_026750 };

extern FlexSkeletonHeader object_zog_Skel_029170;
extern AnimationHeader object_zog_Anim_00FC0C;

extern AnimationHeader object_zog_Anim_00CA94;
extern AnimationHeader object_zog_Anim_00F110;

static AnimationHeader* D_80B958DC[] = { &object_zog_Anim_00CA94, &object_zog_Anim_00F110 };

void func_80B93310(Actor* thisx, Lights* mapper, PlayState* play);
void func_80B93468(EnZog* this, PlayState* play);
void func_80B9451C(EnZog* this, PlayState* play);
void func_80B948A8(EnZog* this, PlayState* play);
void func_80B95128(EnZog* this, PlayState* play);
void func_80B95240(EnZog* this, PlayState* play);

RECOMP_PATCH void EnZog_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnZog* this = THIS;
    s32 i;
    s16 csId;

    if (!D_80B95E10) {
        for (i = 0; i < ARRAY_COUNT(D_80B958AC); i++) {
            D_80B958AC[i] = Lib_SegmentedToVirtual(D_80B958AC[i]);
        }

        for (i = 0; i < ARRAY_COUNT(D_80B958B8); i++) {
            D_80B958B8[i] = Lib_SegmentedToVirtual(D_80B958B8[i]);
        }

        D_80B95E10 = true;
    }

    ActorShape_Init(&this->actor.shape, 0.0f, func_80B93310, 24.0f);
    Actor_SetScale(&this->actor, 0.01f);
    this->actionFunc = func_80B95128;
    this->actor.textId = 0x1004;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_zog_Skel_029170, &object_zog_Anim_00FC0C, this->jointTable,
                       this->morphTable, 23);
    Animation_PlayOnce(&this->skelAnime, &object_zog_Anim_00FC0C);
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;

    if ((ENZOG_GET_F(&this->actor) != ENZOG_F_2) && (rando_location_is_checked(GI_MASK_ZORA)) &&
        ((play->csCtx.scriptIndex != 2) || (gSaveContext.sceneLayer != 0) || (play->sceneId != SCENE_30GYOSON))) {
        Actor_Kill(&this->actor);
        return;
    }

    this->actor.terminalVelocity = -4.0f;
    this->actor.gravity = -1.0f;
    this->actor.cullingVolumeScale = 3000.0f;
    this->actor.shape.yOffset = 1000.0f;
    this->unk_308 = 0;
    this->unk_30A = 0;
    this->unk_31C = 2;
    this->unk_31E = 0;
    this->csIdIndex = -1;
    this->unk_322 = 100;

    Math_Vec3f_Copy(&this->unk_2F0, &this->actor.world.pos);

    if (ENZOG_GET_F(&this->actor) == ENZOG_F_2) {
        this->actionFunc = func_80B95240;
        this->unk_322 = 255;
        this->unk_31C = 0;
        this->unk_31E = 0;
        this->actor.shape.yOffset = 0.0f;
    }

    this->unk_2EC = 0;
    if (ENZOG_GET_FC00(&this->actor) != ENZOG_FC00_63) {
        func_80B93468(this, play);
    } else {
        this->unk_2E8 = NULL;
    }

    this->unk_2FC = 0;
    this->unk_302 = 0;
    this->unk_2EC++;
    this->unk_304 = 0;
    this->unk_2FE = this->unk_2FC;
    this->unk_300 = this->unk_302;
    csId = this->actor.csId;

    for (i = 0; i < ARRAY_COUNT(this->csIdList); i++) {
        this->csIdList[i] = csId;
        if (csId != CS_ID_NONE) {
            this->actor.csId = csId;
            csId = CutsceneManager_GetAdditionalCsId(this->actor.csId);
        }
    }

    this->actor.flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
    this->actor.home.rot.z = 0;
    if (ENZOG_GET_F(&this->actor) != ENZOG_F_2) {
        for (i = 0; i < 5; i++) {
            Actor_SpawnAsChild(&play->actorCtx, &this->actor, play, ACTOR_EN_TANRON4, this->actor.world.pos.x,
                               this->actor.world.pos.y + 500.0f, this->actor.world.pos.z, 0, 0, 0, 100);
        }
    }

    if (ENZOG_GET_F(&this->actor) != ENZOG_F_2) {
        this->unk_302 = this->unk_300 = 0;
        this->unk_2FC = this->unk_2FE = 3;
        this->actor.flags |= ACTOR_FLAG_UPDATE_DURING_SOARING_AND_SOT_CS;
        this->actor.flags &= ~ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
        this->unk_31C = 2;
        this->unk_31E = 0;

        Animation_PlayLoop(&this->skelAnime, D_80B958DC[0]);
        this->actor.textId = 0x1009;
        if (CHECK_WEEKEVENTREG(WEEKEVENTREG_91_02)) {
            this->actor.textId = 0x103C;
            this->actionFunc = func_80B9451C;
        } else {
            this->actionFunc = func_80B948A8;
        }
        this->actor.shape.rot.y = 0x4000;
        this->actor.world.rot.y = 0x4000;
        this->actor.world.pos.x = -376.0f;
        this->actor.world.pos.y = 80.0f;
        this->actor.world.pos.z = 4793.0f;
        this->actor.shape.yOffset = 0.0f;
    }
}

void func_80B93A48(EnZog* this, PlayState* play);
s32 func_80B93BE0(EnZog* this, PlayState* play);

RECOMP_PATCH void func_80B94A00(EnZog* this, PlayState* play) {
    s32 pad;
    Vec3f sp30;

    if (func_80B93BE0(this, play)) {
        this->actionFunc = func_80B948A8;
        this->actor.flags |= ACTOR_FLAG_UPDATE_DURING_OCARINA;
        if (CHECK_WEEKEVENTREG(WEEKEVENTREG_29_20)) {
            this->actor.textId = 0x1009;
        } else {
            this->actor.textId = 0x1008;
            SET_WEEKEVENTREG(WEEKEVENTREG_29_20);
        }
        this->unk_300 = 2;
        this->unk_31C = 2;
        this->unk_31E = 0;
        return;
    }

    if ((this->skelAnime.curFrame >= 35.0f) ||
        ((this->skelAnime.curFrame >= 10.0f) && (this->skelAnime.curFrame <= 24.0f))) {
        this->actor.speed = 0.0f;
    } else {
        this->actor.speed = 7.5f;
    }

    if ((this->actor.depthInWater > 0.0f) && ((play->gameplayFrames % 8) == 0)) {
        sp30 = this->actor.world.pos;
        sp30.y += this->actor.depthInWater;
        EffectSsGRipple_Spawn(play, &sp30, 150, 500, 0);
    }

    if (this->actor.shape.yOffset > 0.0f) {
        this->actor.shape.yOffset -= 20.0f;
    }

    func_80B93A48(this, play);

    if ((this->unk_304 == 4) &&
        (Animation_OnFrame(&this->skelAnime, 136.0f) || Animation_OnFrame(&this->skelAnime, 155.0f))) {
        Actor_PlaySfx(&this->actor, NA_SE_PL_WALK_GROUND + SURFACE_SFX_OFFSET_WATER_SHALLOW);
    }

    if ((this->unk_304 == 5) &&
        (Animation_OnFrame(&this->skelAnime, 12.0f) || Animation_OnFrame(&this->skelAnime, 37.0f))) {
        if (this->actor.depthInWater > 0.0f) {
            Actor_PlaySfx(&this->actor, NA_SE_PL_WALK_GROUND + SURFACE_SFX_OFFSET_WATER_SHALLOW);
        } else {
            Actor_PlaySfx(&this->actor, NA_SE_PL_WALK_GROUND + SURFACE_SFX_OFFSET_SAND);
        }
    }
}

static Vec3f D_80B959AC = { 0.0f, 0.0f, 15.0f };

void func_80B93BA8(EnZog* this, s16 csIdIndex);
void func_80B93D2C(EnZog* this, PlayState* play);
void func_80B94C5C(EnZog* this, PlayState* play);
void func_80B94D0C(EnZog* this, PlayState* play);

RECOMP_PATCH void func_80B94E34(EnZog* this, PlayState* play) {
    s32 pad;
    Player* player = GET_PLAYER(play);

    func_80B93D2C(this, play);
    func_80B93BE0(this, play);
    if (this->actor.speed < 0.1f) {
        this->actor.speed = 0.0f;
    } else {
        if (this->actor.speed > 0.1f) {
            WaterBox* sp44;
            Vec3f sp38;

            Lib_Vec3f_TranslateAndRotateY(&this->actor.world.pos, this->actor.shape.rot.y, &D_80B959AC, &sp38);
            sp38.x += Rand_CenteredFloat(30.0f);
            sp38.y += 20.0f;
            sp38.z += Rand_CenteredFloat(30.0f);
            if (WaterBox_GetSurface1(play, &play->colCtx, sp38.x, sp38.z, &sp38.y, &sp44) &&
                (this->actor.world.pos.y < sp38.y)) {
                EffectSsGSplash_Spawn(play, &sp38, NULL, NULL, 1,
                                      Rand_ZeroFloat(this->actor.speed * 40.0f) + (this->actor.speed * 60.0f));
            }

            if ((player->actor.speed > 3.0f) && (this->unk_324 == 0)) {
                this->unk_324 = 25;
                Player_PlaySfx(player, player->ageProperties->voiceSfxIdOffset + NA_SE_VO_LI_PUSH);
            }
        }
        this->actor.speed *= 0.3f;
    }

    if (ABS_ALT(this->actor.yawTowardsPlayer - this->actor.world.rot.y) > 0x5000) {
        Actor_OfferGetItem(&this->actor, play, GI_NONE, 60.0f, 40.0f);
    }

    if (this->unk_324 > 0) {
        this->unk_324--;
    }

    if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        this->actionFunc = func_80B94D0C;
        this->actor.speed = 0.0f;
        this->unk_300 = 2;
    } else if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        this->actor.home.rot.z = 1;
        this->actionFunc = func_80B94C5C;
        this->actor.speed = 0.0f;
        this->unk_2FE = 1;
        this->actor.velocity.y = 0.0f;
        this->actor.terminalVelocity = 0.0f;
        this->actor.gravity = 0.0f;
        this->unk_31C = 1;
        this->unk_31E = 0;
        func_80B93BA8(this, 0);
        SET_WEEKEVENTREG(WEEKEVENTREG_88_10);
    } else if ((this->actor.yawTowardsPlayer > 16000) && (this->actor.yawTowardsPlayer < 32000) &&
               (this->unk_302 == 0)) {
        Actor_OfferTalk(&this->actor, play, 150.0f);
    }

    this->actor.shape.rot.y = this->actor.world.rot.y;
    func_80B93A48(this, play);
}