#include "modding.h"
#include "global.h"

#include "z64snap.h"

struct EnZov;

typedef void (*EnZovActionFunc)(struct EnZov*, PlayState*);

#define THIS ((EnZov*)thisx)

#define ENZOV_GET_F(thisx) ((thisx)->params & 0xF)
#define ENZOV_GET_FE00(thisx) (((thisx)->params & 0xFE00) >> 9)

#define ENZOV_F_1 1
#define ENZOV_F_2 2

#define LULU_LIMB_MAX 0x17

typedef struct EnZov {
    /* 0x000 */ PictoActor picto;
    /* 0x148 */ Vec3s jointTable[LULU_LIMB_MAX];
    /* 0x1D2 */ Vec3s morphTable[LULU_LIMB_MAX];
    /* 0x25C */ SkelAnime skelAnime;
    /* 0x2A0 */ ColliderCylinder collider;
    /* 0x2EC */ s16 unk_2EC;
    /* 0x2EE */ s16 unk_2EE;
    /* 0x2F0 */ Vec3s unk_2F0;
    /* 0x2F6 */ Vec3s unk_2F6;
    /* 0x2FC */ Vec3f unk_2FC;
    /* 0x308 */ Vec3f unk_308;
    /* 0x314 */ Vec3f unk_314;
    /* 0x320 */ u16 unk_320;
    /* 0x322 */ s16 unk_322;
    /* 0x324 */ s16 unk_324;
    /* 0x326 */ s16 cueId;
    /* 0x328 */ s16 csIdList[2];
    /* 0x32C */ s16 csIdIndex;
    /* 0x330 */ EnZovActionFunc actionFunc;
} EnZov; // size = 0x334

extern ActorInit En_Zov_InitVars;

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_ON,
    },
    { 20, 40, 0, { 0, 0, 0 } },
};

extern FlexSkeletonHeader gLuluSkel;
extern AnimationHeader gLuluLookDownAnim;

void func_80BD1570(EnZov* this, s16 index, u8 mode);
void func_80BD1C38(EnZov* this, PlayState* play);
void func_80BD1C84(EnZov* this, PlayState* play);
void func_80BD1F1C(EnZov* this, PlayState* play);
s32 EnZov_ValidatePictograph(PlayState* play, Actor* thisx);

RECOMP_PATCH void EnZov_Init(Actor* thisx, PlayState* play) {
    EnZov* this = THIS;

    ActorShape_Init(&this->picto.actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    this->picto.actor.colChkInfo.mass = MASS_IMMOVABLE;
    Actor_SetScale(&this->picto.actor, 0.01f);
    Collider_InitAndSetCylinder(play, &this->collider, &this->picto.actor, &sCylinderInit);
    SkelAnime_InitFlex(play, &this->skelAnime, &gLuluSkel, &gLuluLookDownAnim, this->jointTable, this->morphTable,
                       LULU_LIMB_MAX);
    Animation_PlayLoop(&this->skelAnime, &gLuluLookDownAnim);

    this->unk_320 = 0;
    this->csIdIndex = -1;
    this->cueId = -1;
    this->csIdList[0] = this->picto.actor.csId;
    this->csIdList[1] = CS_ID_GLOBAL_TALK;
    this->unk_322 = 0;
    this->actionFunc = func_80BD1C84;
    this->picto.validationFunc = EnZov_ValidatePictograph;

    Math_Vec3f_Copy(&this->unk_2FC, &this->picto.actor.world.pos);
    Math_Vec3f_Copy(&this->unk_308, &this->picto.actor.world.pos);
    Math_Vec3f_Copy(&this->unk_314, &this->picto.actor.world.pos);

    switch (ENZOV_GET_F(&this->picto.actor)) {
        case ENZOV_F_1:
            this->actionFunc = func_80BD1F1C;
            func_80BD1570(this, 9, ANIMMODE_LOOP);
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_GREAT_BAY_TEMPLE)) {
                Actor_Kill(&this->picto.actor);
                return;
            }
            break;

        case ENZOV_F_2:
            this->actionFunc = func_80BD1C38;
            this->picto.actor.shape.shadowDraw = NULL;
            break;

        default:
            this->unk_320 |= 2;
            // if (CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_GREAT_BAY_TEMPLE) || CHECK_WEEKEVENTREG(WEEKEVENTREG_53_20)) {
            // @rando don't kill lulu when turtle is spawned
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_GREAT_BAY_TEMPLE)) {
                Actor_Kill(&this->picto.actor);
            }
            break;
    }
}