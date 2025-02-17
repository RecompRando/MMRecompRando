#include "global.h"
#include "modding.h"

#include "apcommon.h"

#define OBJECT_GG_LIMB_MAX 0x14

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_REACT_TO_LENS)

#define THIS ((EnGg*)thisx)

struct EnGg;
struct EnGgStruct;

typedef void (*EnGgActionFunc)(struct EnGg*, PlayState*);
typedef void (*EnGgUnkFunc)(struct EnGgStruct*, PlayState*);

typedef struct EnGgStruct {
    /* 0x00 */ Vec3f unk_00;
    /* 0x0C */ Vec3f unk_0C;
    /* 0x18 */ Vec3f unk_18;
    /* 0x24 */ Vec3f unk_24;
    /* 0x30 */ u8 animIndex;
    /* 0x34 */ EnGgUnkFunc unk_34;
    /* 0x38 */ EnGgUnkFunc unk_38;
    /* 0x3C */ s32 unk_3C;
    /* 0x40 */ s32 unk_40;
    /* 0x44 */ s32 unk_44;
    /* 0x48 */ u8 unk_48;
} EnGgStruct; // size = 0x4C

typedef struct EnGg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnGgActionFunc actionFunc;
    /* 0x1D8 */ Vec3s unk_1D8;
    /* 0x1DE */ Vec3s unk_1DE;
    /* 0x1E4 */ Vec3s jointTable[OBJECT_GG_LIMB_MAX];
    /* 0x25C */ Vec3s morphTable[OBJECT_GG_LIMB_MAX];
    /* 0x2D4 */ UNK_TYPE1 unk_2D4[0x6];
    /* 0x2DA */ u8 csAnimIndex;
    /* 0x2DB */ u8 cueId;
    /* 0x2DC */ s16 csId;
    /* 0x2DE */ UNK_TYPE1 unk2DE[4];
    /* 0x2E2 */ s16 unk_2E2;
    /* 0x2E4 */ s16 unk_2E4;
    /* 0x2E6 */ s16 animIndex;
    /* 0x2E8 */ s16 unk_2E8;
    /* 0x2EA */ UNK_TYPE1 unk2EA[0x1A];
    /* 0x304 */ s16 unk_304;
    /* 0x306 */ u8 unk_306;
    /* 0x307 */ u8 unk_307;
    /* 0x308 */ u8 unk_308;
    /* 0x309 */ u8 unk_309;
    /* 0x30A */ s16 unk_30A;
    /* 0x30C */ UNK_TYPE1 unk30C[4];
    /* 0x310 */ f32 unk_310;
    /* 0x314 */ UNK_TYPE1 unk314[0xC];
    /* 0x320 */ Vec3f unk_320;
    /* 0x32C */ Vec3f unk_32C;
    /* 0x338 */ Vec3f unk_338;
    /* 0x344 */ EnGgStruct unk_344;
} EnGg; // size = 0x390

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
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
    { 24, 72, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 24, 72, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(0, 0x0),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(0, 0x0),
    /* Zora boomerang */ DMG_ENTRY(0, 0x0),
    /* Normal arrow   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(0, 0x0),
    /* Goron punch    */ DMG_ENTRY(0, 0x0),
    /* Sword          */ DMG_ENTRY(0, 0x0),
    /* Goron pound    */ DMG_ENTRY(0, 0x0),
    /* Fire arrow     */ DMG_ENTRY(0, 0x0),
    /* Ice arrow      */ DMG_ENTRY(0, 0x0),
    /* Light arrow    */ DMG_ENTRY(0, 0x0),
    /* Goron spikes   */ DMG_ENTRY(0, 0x0),
    /* Deku spin      */ DMG_ENTRY(0, 0x0),
    /* Deku bubble    */ DMG_ENTRY(0, 0x0),
    /* Deku launch    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x0),
    /* Zora barrier   */ DMG_ENTRY(0, 0x0),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(0, 0x0),
    /* Zora punch     */ DMG_ENTRY(0, 0x0),
    /* Spin attack    */ DMG_ENTRY(0, 0x0),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(0, 0x0),
};

extern FlexSkeletonHeader object_gg_Skel_00F578;
extern FlexSkeletonHeader object_gg_Skel_00F6C0;

extern AnimationHeader object_gg_Anim_00F578;

void func_80B35250(EnGg* this);
void func_80B35B24(EnGgStruct* ptr, PlayState* play);

RECOMP_PATCH void EnGg_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnGg* this = THIS;

    if (rando_location_is_checked(GI_MASK_GORON)) {
        Actor_Kill(&this->actor);
        return;
    }

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 30.0f);
    this->actor.bgCheckFlags |= BGCHECKFLAG_PLAYER_400;
    SkelAnime_InitFlex(play, &this->skelAnime, &object_gg_Skel_00F6C0, &object_gg_Anim_00F578, this->jointTable,
                       this->morphTable, OBJECT_GG_LIMB_MAX);

    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);

    CLEAR_WEEKEVENTREG(WEEKEVENTREG_20_04);
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_20_08);
    CLEAR_WEEKEVENTREG(WEEKEVENTREG_20_10);
    this->actor.flags &= ~ACTOR_FLAG_REACT_TO_LENS;
    this->unk_310 = this->actor.home.pos.y;
    this->csId = this->actor.csId;
    this->actor.flags |= ACTOR_FLAG_UPDATE_DURING_OCARINA;
    this->unk_308 = 0;
    this->unk_309 = 0;
    this->unk_304 = 0;
    this->unk_30A = 0;
    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
    func_80B35B24(&this->unk_344, play);
    func_80B35250(this);
}