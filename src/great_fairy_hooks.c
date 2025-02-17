#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnDyExtra;

typedef void (*EnDyExtraActionFunc)(struct EnDyExtra*, PlayState*);

typedef struct EnDyExtra {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnDyExtraActionFunc actionFunc;
    /* 0x148 */ s16 type;
    /* 0x14A */ s16 trigger;
    /* 0x14C */ s16 timer;
    /* 0x150 */ f32 alphaScale;
    /* 0x154 */ Vec3f scale; // not used, leftover from OoT
    /* 0x160 */ Vec3f initPos; // set and not used
} EnDyExtra; // size = 0x16C

struct BgDyYoseizo;

typedef void (*BgDyYoseizoActionFunc)(struct BgDyYoseizo*, PlayState*);

#define GREAT_FAIRY_GET_SWITCHFLAG(thisx) (((thisx)->params & 0xFE00) >> 9)
#define GREAT_FAIRY_GET_TYPE(thisx) ((thisx)->params & 0xF)
#define GREAT_FAIRY_ROTZ(thisx) ((thisx)->home.rot.z)

#define GREAT_FAIRY_LIMB_MAX 0x1C

#define LOCATION_GREAT_FAIRY (0x030000 | GREAT_FAIRY_GET_TYPE(&this->actor))
#define LOCATION_GREAT_FAIRY_HUMAN GI_MASK_GREAT_FAIRY

typedef enum GreatFairyType {
    /* 0 */ GREAT_FAIRY_TYPE_MAGIC,
    /* 1 */ GREAT_FAIRY_TYPE_POWER,
    /* 2 */ GREAT_FAIRY_TYPE_WISDOM,
    /* 3 */ GREAT_FAIRY_TYPE_COURAGE,
    /* 4 */ GREAT_FAIRY_TYPE_KINDNESS
} GreatFairyType;

#define BG_DY_YOSEIZO_EFFECT_COUNT 200

typedef struct BgDyYoseizoEffect {
    /* 0x00 */ u8 alive; // drawn if 1, respawn if 0
    /* 0x04 */ Vec3f pos;
    /* 0x10 */ Vec3f velocity;
    /* 0x1C */ Vec3f accel;
    /* 0x28 */ Color_RGB8 primColor;
    /* 0x2B */ Color_RGB8 envColor;
    /* 0x2E */ s16 alpha;
    /* 0x30 */ f32 scale;
    /* 0x34 */ s16 timer; // lifetime
    /* 0x36 */ s16 type; // 0 is general radiance, else is directed towards Player
    /* 0x38 */ s16 pitch;
    /* 0x3A */ s16 yaw;
    /* 0x3C */ s16 roll; // all three are f32 in OoT
} BgDyYoseizoEffect; // size = 0x40

typedef struct BgDyYoseizo {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ BgDyYoseizoActionFunc actionFunc;
    /* 0x0148 */ SkelAnime skelAnime;
    /* 0x018C */ Vec3s jointTable[GREAT_FAIRY_LIMB_MAX];
    /* 0x0234 */ Vec3s morphTable[GREAT_FAIRY_LIMB_MAX];
    /* 0x02DC */ Vec3s headRot;
    /* 0x02DC */ Vec3s torsoRot;
    /* 0x02E8 */ EnDyExtra* beam;
    /* 0x02EC */ f32 unk2EC; // unused
    /* 0x02F0 */ f32 unk2F0;
    /* 0x02F4 */ f32 unk2F4;
    /* 0x02F8 */ union {
        s16 unk2F8;
        s16 csId; // used on BgDyYoseizo_TrainPlayer
    };
    /* 0x02FA */ s16 eyeIndex;
    /* 0x02FC */ s16 mouthIndex;
    /* 0x02FE */ s16 blinkTimer;
    /* 0x0300 */ s16 timer;
    /* 0x0302 */ u16 unk302;
    /* 0x0304 */ BgDyYoseizoEffect effects[BG_DY_YOSEIZO_EFFECT_COUNT];
} BgDyYoseizo; // size = 0x3504

void BgDyYoseizo_Bob(BgDyYoseizo* this, PlayState* play);
s32 SkelAnime_Update(SkelAnime* skelAnime);
void func_80A0B290(BgDyYoseizo* this, PlayState* play);

struct EnElfgrp;

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED)

#define THIS ((EnElfgrp*)thisx)

typedef void (*EnElfgrpActionFunc)(struct EnElfgrp*, PlayState*);

#define ENELFGRP_GET_TYPE(thisx) ((thisx)->params & 0xF) //!< Same type as Great Fairies
#define ENELFGRP_GET_SWITCH_FLAG_PARAMS(thisx) (((thisx)->params & 0xFE00) >> 9)
#define ENELFGRP_GET_SWITCHFLAG_ROT(thisx) ((thisx)->home.rot.z)

typedef enum ElfgrpType {
    /* 0 */ ENELFGRP_TYPE_MAGIC = GREAT_FAIRY_TYPE_MAGIC,
    /* 1 */ ENELFGRP_TYPE_POWER = GREAT_FAIRY_TYPE_POWER,
    /* 2 */ ENELFGRP_TYPE_WISDOM = GREAT_FAIRY_TYPE_WISDOM,
    /* 3 */ ENELFGRP_TYPE_COURAGE = GREAT_FAIRY_TYPE_COURAGE,
    /* 4 */ ENELFGRP_TYPE_KINDNESS = GREAT_FAIRY_TYPE_KINDNESS,
    /* 5 */ ENELFGRP_TYPE_MAX
} ElfgrpType;

typedef struct EnElfgrp {
    /* 0x000 */ Actor actor;
    /* 0x144 */ s16 timer;
    /* 0x146 */ u8 talkedOnceFlag;
    /* 0x147 */ u8 type;
    /* 0x148 */ s8 unk_148; // set and not used
    /* 0x14A */ u16 stateFlags;
    /* 0x14C */ EnElfgrpActionFunc actionFunc;
} EnElfgrp; // size = 0x150

// State flags
#define ELFGRP_STATE_0 (1 << 0)
#define ELFGRP_STATE_1 (1 << 1)
#define ELFGRP_STATE_2 (1 << 2)
#define ELFGRP_STATE_3 (1 << 3)
#define ELFGRP_STATE_4 (1 << 4)

// Used for the type in EnElfgrp_SpawnStrayFairies
typedef enum ElfgrpSpawnedFairyTypes {
    /* 0 */ SPAWNED_STRAY_FAIRY_TYPE_PRESENT,  // STRAY_FAIRY_TYPE_FAIRY_FOUNTAIN
    /* 1 */ SPAWNED_STRAY_FAIRY_TYPE_RETURNING // STRAY_FAIRY_TYPE_RETURNING_TO_FOUNTAIN
} ElfgrpSpawnedFairyTypes;

bool Player_HasTransformationMask(PlayState* play) {
    Player* player = GET_PLAYER(play);

    return INV_HAS(ITEM_MASK_DEKU) || INV_HAS(ITEM_MASK_GORON) || INV_HAS(ITEM_MASK_ZORA) || INV_HAS(ITEM_MASK_FIERCE_DEITY);
}

s32 EnElfgrp_GetHeldFairiesCount(PlayState* play, s32 type);
s32 EnElfgrp_GetFountainFairiesCount(PlayState* play, s32 type);
void EnElfgrp_SpawnStrayFairies(EnElfgrp* this, PlayState* play, s32 count, s32 fairyType);
void func_80A3A398(EnElfgrp* this, PlayState* play);
void func_80A3A520(EnElfgrp* this, PlayState* play);
void EnElfgrp_DoNothing(EnElfgrp* this, PlayState* play);
void func_80A3A7FC(EnElfgrp* this, PlayState* play);
void func_80A3A8F8(EnElfgrp* this, PlayState* play);

void EnElfgrp_SetCutscene(EnElfgrp* this, s32 numCutscenes);

void EnElfgrp_OfferLoop(EnElfgrp* this, PlayState* play);
void EnElfgrp_OfferTransformItem(EnElfgrp* this, PlayState* play);

RECOMP_PATCH void EnElfgrp_Init(Actor* thisx, PlayState* play) {
    EnElfgrp* this = THIS;
    s32 numberInFountain = EnElfgrp_GetFountainFairiesCount(play, this->type);

    this->type = ENELFGRP_GET_TYPE(&this->actor);
    this->unk_148 = 0;
    this->stateFlags = 0;
    this->actor.focus.pos.y += 40.0f;
    this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;

    if (numberInFountain < STRAY_FAIRY_TOTAL) {
        EnElfgrp_SpawnStrayFairies(this, play, numberInFountain, SPAWNED_STRAY_FAIRY_TYPE_PRESENT);
    }

    this->actionFunc = EnElfgrp_OfferLoop;
}

void EnElfgrp_OfferLoop(EnElfgrp* this, PlayState* play) {
    s32 type = ENELFGRP_GET_TYPE(&this->actor);
    bool hasFairies = type == ENELFGRP_TYPE_MAGIC ? rando_has_item(0x01007F) : rando_has_item(0x010000 | (type - 1)) >= 15;

    if (hasFairies && !rando_location_is_checked(LOCATION_GREAT_FAIRY)) {
        func_80A3A398(this, play);
    } else if (type == ENELFGRP_TYPE_MAGIC && Player_HasTransformationMask(play) && !rando_location_is_checked(LOCATION_GREAT_FAIRY_HUMAN)) {
        EnElfgrp_OfferTransformItem(this, play);
    }
}

void EnElfgrp_OfferTransformItem(EnElfgrp* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_GREAT_FAIRY_HUMAN), LOCATION_GREAT_FAIRY_HUMAN, 350.0f, 350.0f, true, true);
    }
}

void func_80A3A274(EnElfgrp* this, PlayState* play);

RECOMP_PATCH void func_80A3A398(EnElfgrp* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_GREAT_FAIRY), LOCATION_GREAT_FAIRY, 350.0f, 350.0f, true, true);
    }
}