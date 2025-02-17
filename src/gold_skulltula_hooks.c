#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define OBJECT_ST_LIMB_MAX 0x1E

struct EnSw;

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_UNFRIENDLY)

#define THIS ((EnSw*)thisx)

typedef void (*EnSwActionFunc)(struct EnSw*, PlayState*);

#define ENSW_GETS_3(params) ((params & 3) & 0xFF)
#define ENSW_GET_3(thisx) (ENSW_GETS_3((thisx)->params))
#define ENSW_GETS_FC(params) (((params & 0xFC) >> 2) & 0xFF)
#define ENSW_GET_FC(thisx) (ENSW_GETS_FC((thisx)->params))
#define ENSW_GETS_3FC(params) (((params & 0x3FC) >> 2) & 0xFF)
#define ENSW_GET_3FC(thisx) (ENSW_GETS_3FC((thisx)->params))
#define ENSW_GET_PATH_INDEX(thisx) ((((thisx)->params & 0xFF00) >> 8) & 0xFF)

#define ENSW_PATH_INDEX_NONE 0xFF

#define LOCATION_SKULL_TOKEN (0x060000 | (play->sceneId << 8) | ENSW_GET_FC(&this->actor))

typedef enum EnSwBodyPart {
    /*  0 */ ENSW_BODYPART_0,
    /*  1 */ ENSW_BODYPART_1,
    /*  2 */ ENSW_BODYPART_2,
    /*  3 */ ENSW_BODYPART_3,
    /*  4 */ ENSW_BODYPART_4,
    /*  5 */ ENSW_BODYPART_5,
    /*  6 */ ENSW_BODYPART_6,
    /*  7 */ ENSW_BODYPART_7,
    /*  8 */ ENSW_BODYPART_8,
    /*  9 */ ENSW_BODYPART_9,
    /* 10 */ ENSW_BODYPART_10,
    /* 11 */ ENSW_BODYPART_11,
    /* 12 */ ENSW_BODYPART_MAX
} EnSwBodyPart;

typedef struct EnSw {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnSwActionFunc actionFunc;
    /* 0x18C */ ColliderSphere collider;
    /* 0x1E4 */ Path* path;
    /* 0x1E8 */ Vec3s jointTable[OBJECT_ST_LIMB_MAX];
    /* 0x29C */ Vec3s morphTable[OBJECT_ST_LIMB_MAX];
    /* 0x350 */ Vec3f unk_350;
    /* 0x35C */ Vec3f unk_35C;
    /* 0x368 */ Vec3f unk_368;
    /* 0x374 */ Vec3f unk_374;
    /* 0x380 */ Vec3f bodyPartsPos[ENSW_BODYPART_MAX];
    /* 0x410 */ u16 unk_410;
    /* 0x412 */ u8 drawDmgEffType;
    /* 0x414 */ f32 unk_414;
    /* 0x418 */ f32 drawDmgEffFrozenSteamScales[ENSW_BODYPART_MAX];
    /* 0x448 */ f32 unk_448;
    /* 0x44C */ f32 unk_44C;
    /* 0x450 */ f32 unk_450;
    /* 0x454 */ s16 unk_454;
    /* 0x456 */ s16 unk_456;
    /* 0x458 */ s16 unk_458;
    /* 0x45A */ s16 unk_45A;
    /* 0x45C */ s16 unk_45C;
    /* 0x45E */ s16 unk_45E;
    /* 0x460 */ s16 unk_460;
    /* 0x462 */ s16 unk_462;
    /* 0x464 */ s16 unk_464[ENSW_BODYPART_MAX];
    /* 0x47C */ s16 unk_47C[ENSW_BODYPART_MAX];
    /* 0x494 */ s16 unk_494;
    /* 0x496 */ s16 unk_496;
    /* 0x498 */ s16 unk_498;
    /* 0x49C */ s32 unk_49C;
    /* 0x4A0 */ s32 unk_4A0;
} EnSw; // size = 0x4A4

static ColliderSphereInit sSphereInit = {
    {
        COL_MATERIAL_NONE,
        AT_ON | AT_TYPE_ENEMY,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_SPHERE,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0xF7CFFFFF, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_ON | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 0, { { 0, 0, 0 }, 16 }, 100 },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 1, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(1, 0x0),
    /* Deku Stick     */ DMG_ENTRY(1, 0x0),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(1, 0x0),
    /* Zora boomerang */ DMG_ENTRY(1, 0x0),
    /* Normal arrow   */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(1, 0x0),
    /* Goron punch    */ DMG_ENTRY(1, 0x0),
    /* Sword          */ DMG_ENTRY(1, 0x0),
    /* Goron pound    */ DMG_ENTRY(2, 0x0),
    /* Fire arrow     */ DMG_ENTRY(2, 0x2),
    /* Ice arrow      */ DMG_ENTRY(2, 0x3),
    /* Light arrow    */ DMG_ENTRY(2, 0x4),
    /* Goron spikes   */ DMG_ENTRY(1, 0x0),
    /* Deku spin      */ DMG_ENTRY(1, 0x0),
    /* Deku bubble    */ DMG_ENTRY(1, 0x0),
    /* Deku launch    */ DMG_ENTRY(2, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(1, 0x0),
    /* Zora barrier   */ DMG_ENTRY(1, 0x5),
    /* Normal shield  */ DMG_ENTRY(1, 0x0),
    /* Light ray      */ DMG_ENTRY(1, 0x0),
    /* Thrown object  */ DMG_ENTRY(1, 0x0),
    /* Zora punch     */ DMG_ENTRY(1, 0x0),
    /* Spin attack    */ DMG_ENTRY(1, 0x0),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(1, 0x0),
};

static CollisionCheckInfoInit2 sColChkInfoInit2 = { 1, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable2 = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x1),
    /* Deku Stick     */ DMG_ENTRY(1, 0x0),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(1, 0x0),
    /* Zora boomerang */ DMG_ENTRY(1, 0x0),
    /* Normal arrow   */ DMG_ENTRY(1, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(1, 0x0),
    /* Goron punch    */ DMG_ENTRY(1, 0x0),
    /* Sword          */ DMG_ENTRY(1, 0x0),
    /* Goron pound    */ DMG_ENTRY(2, 0x0),
    /* Fire arrow     */ DMG_ENTRY(2, 0x2),
    /* Ice arrow      */ DMG_ENTRY(2, 0x3),
    /* Light arrow    */ DMG_ENTRY(2, 0x4),
    /* Goron spikes   */ DMG_ENTRY(1, 0x0),
    /* Deku spin      */ DMG_ENTRY(1, 0x0),
    /* Deku bubble    */ DMG_ENTRY(1, 0x0),
    /* Deku launch    */ DMG_ENTRY(2, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x1),
    /* Zora barrier   */ DMG_ENTRY(1, 0x5),
    /* Normal shield  */ DMG_ENTRY(1, 0x0),
    /* Light ray      */ DMG_ENTRY(1, 0x0),
    /* Thrown object  */ DMG_ENTRY(1, 0x0),
    /* Zora punch     */ DMG_ENTRY(1, 0x0),
    /* Spin attack    */ DMG_ENTRY(1, 0x0),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(1, 0x0),
};

typedef enum EnSwAnimation {
    /* -1 */ ENSW_ANIM_NONE = -1,
    /*  0 */ ENSW_ANIM_0,
    /*  1 */ ENSW_ANIM_1,
    /*  2 */ ENSW_ANIM_2,
    /*  3 */ ENSW_ANIM_3,
    /*  4 */ ENSW_ANIM_MAX
} EnSwAnimation;

extern AnimationHeader object_st_Anim_000304;
extern AnimationHeader object_st_Anim_0055A8;
extern AnimationHeader object_st_Anim_005B98;

extern SkeletonHeader object_st_Skel_005298;

static AnimationInfoS sAnimationInfo[ENSW_ANIM_MAX] = {
    { &object_st_Anim_000304, 1.0f, 0, -1, ANIMMODE_ONCE_INTERP, 0 },  // ENSW_ANIM_0
    { &object_st_Anim_000304, 1.0f, 0, -1, ANIMMODE_ONCE_INTERP, -4 }, // ENSW_ANIM_1
    { &object_st_Anim_0055A8, 1.0f, 0, -1, ANIMMODE_LOOP_INTERP, -4 }, // ENSW_ANIM_2
    { &object_st_Anim_005B98, 1.0f, 0, -1, ANIMMODE_LOOP_INTERP, -4 }, // ENSW_ANIM_3
};

s32 func_808D9968(EnSw* this, PlayState* play);
void func_808D9F08(EnSw* this);
void func_808D9F78(EnSw* this, PlayState* play, s32 arg2);

void func_808DA024(EnSw* this, PlayState* play);
void func_808DA350(EnSw* this, PlayState* play);
void func_808DAA60(EnSw* this, PlayState* play);
void func_808DB100(EnSw* this, PlayState* play);

RECOMP_PATCH void EnSw_Init(Actor* thisx, PlayState* play) {
    EnSw* this = THIS;
    s32 pad;

    if (rando_location_is_checked(LOCATION_SKULL_TOKEN)) {
        Actor_Kill(thisx);
        return;
    }

    if (!func_808D9968(this, play)) {
        ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.0f);
        SkelAnime_Init(play, &this->skelAnime, &object_st_Skel_005298, NULL, this->jointTable, this->morphTable,
                       OBJECT_ST_LIMB_MAX);
        SubS_ChangeAnimationByInfoS(&this->skelAnime, sAnimationInfo, ENSW_ANIM_0);
        this->skelAnime.playSpeed = 4.0f;

        Collider_InitAndSetSphere(play, &this->collider, &this->actor, &sSphereInit);
        if (!ENSW_GET_3(&this->actor)) {
            this->actor.hintId = TATL_HINT_ID_SKULLWALLTULA;
            CollisionCheck_SetInfo2(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
            this->collider.elem.atDmgInfo.damage = 8;
        } else {
            this->actor.hintId = TATL_HINT_ID_GOLD_SKULLTULA;
            CollisionCheck_SetInfo2(&this->actor.colChkInfo, &sDamageTable2, &sColChkInfoInit2);
            this->collider.elem.atDmgInfo.damage = 16;
        }

        this->path =
            SubS_GetDayDependentPath(play, ENSW_GET_PATH_INDEX(&this->actor), ENSW_PATH_INDEX_NONE, &this->unk_4A0);
        if (this->path != NULL) {
            this->unk_4A0 = 1;
        }

        switch (ENSW_GET_3(&this->actor)) {
            case 0:
                func_808D9F78(this, play, true);
                this->actionFunc = func_808DA350;
                break;

            case 1:
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;

                this->unk_460 = ABS_ALT(thisx->world.rot.z);

                if (this->actor.world.rot.z >= 0) {
                    this->unk_410 |= 8;
                }

                func_808DA024(this, play);
                this->unk_410 |= (0x10 | 0x1);
                this->unk_410 &= ~4;
                this->actionFunc = func_808DB100;
                break;

            case 2:
            case 3:
                this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;

                this->unk_460 = ABS_ALT(thisx->world.rot.z);

                if (this->actor.world.rot.z >= 0) {
                    this->unk_410 |= 8;
                }

                func_808D9F78(this, play, true);
                if (this->path != NULL) {
                    this->unk_49C = 1;
                    func_808D9F08(this);
                    this->actionFunc = func_808DAA60;
                } else {
                    this->actionFunc = func_808DB100;
                }
                break;
        }
    } else {
        Actor_Kill(&this->actor);
    }
}