#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define LOCATION_LENS_CAVE_CENTER 0x060701
#define LOCATION_LENS_CAVE_INVISIBLE 0x060703
#define LOCATION_LENS_CAVE_ROCK 0x060706

#define OWL_FLYING_LIMB_MAX 0x15
#define OWL_PERCHING_LIMB_MAX 0x10

struct EnOwl;

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

#define THIS ((EnOwl*)thisx)

typedef void (*EnOwlActionFunc)(struct EnOwl*, PlayState*);
typedef void (*EnOwlFunc)(struct EnOwl*);

#define ENOWL_GET_PATH_INDEX(thisx) (((thisx)->params & 0xF000) >> 0xC)
#define ENOWL_GET_TYPE(thisx) (((thisx)->params & 0xF80) >> 7)
#define ENOWL_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)

#define ENOWL_PATH_INDEX_NONE 0xF
#define ENOWL_SWITCH_FLAG_NONE 0x7F

typedef enum {
    /* 0x001 */ ENOWL_GET_TYPE_1 = 1,
    /* 0x002 */ ENOWL_GET_TYPE_2,
    /* 0x003 */ ENOWL_GET_TYPE_3,
    /* 0x01E */ ENOWL_GET_TYPE_30 = 30,
    /* 0x3E8 */ ENOWL_GET_TYPE_1000 = 1000
} EnOwlType;

typedef struct EnOwl {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnimeFlying;
    /* 0x1D4 */ Vec3s jointTableFlying[OWL_FLYING_LIMB_MAX];
    /* 0x252 */ Vec3s morphTableFlying[OWL_FLYING_LIMB_MAX];
    /* 0x2D0 */ SkelAnime skelAnimePerching;
    /* 0x314 */ Vec3s jointTablePerching[OWL_PERCHING_LIMB_MAX];
    /* 0x374 */ Vec3s morphTablePerching[OWL_PERCHING_LIMB_MAX];
    /* 0x3D4 */ SkelAnime* curSkelAnime; // Points to the currently active SkelAnime
    /* 0x3D8 */ s16 unk_3D8;
    /* 0x3DA */ s16 unk_3DA;
    /* 0x3DC */ s16 unk_3DC;
    /* 0x3DE */ s16 unk_3DE;
    /* 0x3E0 */ s16 eyeTexIndex;
    /* 0x3E2 */ s16 blinkTimer;
    /* 0x3E4 */ f32 unk_3E4;
    /* 0x3E8 */ u16 actionFlags;
    /* 0x3EA */ s16 unk_3EA;
    /* 0x3EC */ s16 unk_3EC;
    /* 0x3F0 */ f32 unk_3F0;
    /* 0x3F4 */ Path* path;
    /* 0x3F8 */ s32 unk_3F8;
    /* 0x3FC */ s32 pathIndex;
    /* 0x400 */ s16 csIdList[3];
    /* 0x406 */ s16 csIdIndex;
    /* 0x408 */ u8 unk_408;
    /* 0x409 */ u8 unk_409;
    /* 0x40A */ u8 unk_40A;
    /* 0x40B */ u8 unk_40B;
    /* 0x40C */ u8 unk_40C;
    /* 0x40D */ u8 unk_40D;
    /* 0x410 */ EnOwlActionFunc actionFunc;
    /* 0x414 */ EnOwlFunc unk_414;
} EnOwl; // size = 0x418

extern FlexSkeletonHeader gOwlFlyingSkel;
extern FlexSkeletonHeader gOwlPerchingSkel;
extern AnimationHeader gOwlFlyAnim;
extern AnimationHeader gOwlPerchAnim;

void func_8095AB1C(EnOwl* this, PlayState* play);
void func_8095ABF0(EnOwl* this, PlayState* play);
void func_8095AD54(EnOwl* this, PlayState* play);
void func_8095AE00(EnOwl* this, PlayState* play);
void func_8095AE60(EnOwl* this, PlayState* play);
void func_8095AEC0(EnOwl* this, PlayState* play);
void func_8095AF2C(EnOwl* this, PlayState* play);
void func_8095AFEC(EnOwl* this, PlayState* play);
void func_8095B254(EnOwl* this, PlayState* play);
void func_8095B2F8(EnOwl* this, PlayState* play);
void func_8095B650(EnOwl* this, PlayState* play);
void func_8095B6C8(EnOwl* this, PlayState* play);
void func_8095B76C(EnOwl* this, PlayState* play);
void func_8095B960(EnOwl* this, PlayState* play);
void func_8095BA84(EnOwl* this, PlayState* play);
void func_8095BE0C(EnOwl* this, PlayState* play);
void func_8095BF20(EnOwl* this, PlayState* play);
void func_8095BF58(EnOwl* this, PlayState* play);
void func_8095C1C8(EnOwl* this, PlayState* play);
void func_8095C328(EnOwl* this);
void func_8095C408(EnOwl* this);
void func_8095C484(EnOwl* this);
void func_8095CCF4(Actor* thisx, PlayState* play);
void func_8095D074(Actor* thisx, PlayState* play);
void EnOwl_ChangeMode(EnOwl* this, EnOwlActionFunc actionFunc, EnOwlFunc unkFunc, SkelAnime* skelAnime,
                      AnimationHeader* animation, f32 morphFrames);

void func_8095A510(EnOwl* this, PlayState* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_ENEMY,
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
    { 30, 40, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 25, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDistance, 1400, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeScale, 2000, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 2400, ICHAIN_STOP),
};

RECOMP_PATCH void EnOwl_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnOwl* this = THIS;
    s32 i;
    s16 csId = this->actor.csId;
    s32 owlType;
    s32 switchFlag;

    for (i = 0; i < ARRAY_COUNT(this->csIdList); i++) {
        this->csIdList[i] = csId;
        if (csId != CS_ID_NONE) {
            this->actor.csId = csId;
            csId = CutsceneManager_GetAdditionalCsId(this->actor.csId);
        }
    }

    this->actor.csId = this->csIdList[0];
    Actor_ProcessInitChain(&this->actor, sInitChain);
    if (ENOWL_GET_TYPE(&this->actor) == ENOWL_GET_TYPE_30) {
        Actor_SetScale(&this->actor, 0.1f);
        this->actor.update = func_8095CCF4;
        this->actor.draw = func_8095D074;
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        this->unk_3D8 = 0;
        this->unk_3DA = 0x320;
        this->unk_3DC = 0x12C;
        return;
    }

    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 36.0f);
    SkelAnime_InitFlex(play, &this->skelAnimeFlying, &gOwlFlyingSkel, &gOwlFlyAnim, this->jointTableFlying,
                       this->morphTableFlying, OWL_FLYING_LIMB_MAX);
    SkelAnime_InitFlex(play, &this->skelAnimePerching, &gOwlPerchingSkel, &gOwlPerchAnim, this->jointTablePerching,
                       this->morphTablePerching, OWL_PERCHING_LIMB_MAX);
    Collider_InitAndSetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    this->actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->actor.terminalVelocity = -10.0f;
    this->actor.lockOnArrowOffset = 500.0f;
    EnOwl_ChangeMode(this, func_8095BF58, func_8095C484, &this->skelAnimePerching, &gOwlPerchAnim, 0.0f);

    this->actionFlags = 0;
    this->unk_40D = 0;
    this->unk_40A = 0;
    this->unk_409 = 4;
    this->unk_40B = this->unk_408 = 0;
    this->unk_40C = 4;
    this->csIdIndex = -1;
    owlType = ENOWL_GET_TYPE(&this->actor);
    switchFlag = ENOWL_GET_SWITCH_FLAG(&this->actor);

    switch (owlType) {
        case ENOWL_GET_TYPE_1:
            if ((switchFlag < ENOWL_SWITCH_FLAG_NONE) && Flags_GetSwitch(play, switchFlag)) {
                Actor_Kill(&this->actor);
                return;
            }
            break;

        case ENOWL_GET_TYPE_2:
            if (rando_location_is_checked(LOCATION_LENS_CAVE_CENTER) && rando_location_is_checked(LOCATION_LENS_CAVE_INVISIBLE) && rando_location_is_checked(LOCATION_LENS_CAVE_ROCK)) {
                Actor_Kill(&this->actor);
                return;
            }
            break;

        case ENOWL_GET_TYPE_3:
            //if (CHECK_QUEST_ITEM(QUEST_SONG_SOARING)) {
                Actor_Kill(&this->actor);
                return;
            //}
            break;

        default:
            break;
    }

    this->unk_3DA = 0;
    this->unk_3F0 = this->actor.home.pos.y;
    this->unk_3EC = this->actor.home.rot.y;
    func_8095A510(this, play);

    switch (owlType) {
        case ENOWL_GET_TYPE_1:
            this->actionFunc = func_8095AEC0;
            break;

        case ENOWL_GET_TYPE_2:
            this->actionFunc = func_8095BE0C;
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_09_20)) {
                this->actor.textId = 0xBF0;
            } else {
                this->actor.textId = 0xBEA;
            }
            break;

        case ENOWL_GET_TYPE_3:
            this->actionFunc = func_8095AFEC;
            break;

        case ENOWL_GET_TYPE_1000:
            this->actionFunc = func_8095BF20;
            break;

        default:
            this->actionFlags |= 2;
            this->unk_3DA = 0x20;
            this->actionFunc = func_8095BF58;
            break;
    }
}