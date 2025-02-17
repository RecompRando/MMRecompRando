#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct DoorWarp1;

#define FLAGS 0x00000000

#define THIS ((DoorWarp1*)thisx)

typedef void (*DoorWarp1ActionFunc)(struct DoorWarp1*, PlayState*);

#define DOORWARP1_GET_FF(thisx) ((thisx)->params & 0xFF)
#define DOORWARP1_GET_FF00_1(thisx) (((thisx)->params >> 8) & 0xFF)
#define DOORWARP1_GET_FF00_2(thisx) ((thisx)->params & 0xFF00)
#define DOORWARP1_GET_FF00_3(thisx) ((thisx)->params >> 8)

#define LOCATION_REMAINS_ODOLWA GI_REMAINS_ODOLWA
#define LOCATION_REMAINS_GOHT GI_REMAINS_GOHT
#define LOCATION_REMAINS_GYORG GI_REMAINS_GYORG
#define LOCATION_REMAINS_TWINMOLD GI_REMAINS_TWINMOLD

typedef enum {
    /* 0 */ ENDOORWARP1_FF_0,
    /* 1 */ ENDOORWARP1_FF_1,
    /* 2 */ ENDOORWARP1_FF_2,
    /* 3 */ ENDOORWARP1_FF_3,
    /* 4 */ ENDOORWARP1_FF_4,
    /* 5 */ ENDOORWARP1_FF_5,
    /* 6 */ ENDOORWARP1_FF_6
} DoorWarp1Param;

typedef struct DmHina DmHina;

typedef struct DoorWarp1 {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ SkelAnime skelAnime;
    /* 0x1A0 */ DmHina* unk_1A0;
    /* 0x1A4 */ f32 unk_1A4;
    /* 0x1A8 */ f32 unk_1A8;
    /* 0x1AC */ f32 unk_1AC;
    /* 0x1B0 */ f32 unk_1B0;
    /* 0x1B4 */ f32 unk_1B4;
    /* 0x1B8 */ f32 unk_1B8;
    /* 0x1BC */ f32 unk_1BC;
    /* 0x1C0 */ f32 unk_1C0;
    /* 0x1C4 */ s16 unk_1C4;
    /* 0x1C6 */ s16 unk_1C6;
    /* 0x1C8 */ s16 unk_1C8;
    /* 0x1CA */ s16 unk_1CA;
    /* 0x1CC */ s16 unk_1CC;
    /* 0x1CE */ s16 unk_1CE;
    /* 0x1D0 */ u16 unk_1D0;
    /* 0x1D2 */ s8 unk_1D2;
    /* 0x1D3 */ u8 unk_1D3;
    /* 0x1D4 */ u8 unk_1D4;
    /* 0x1D8 */ DoorWarp1ActionFunc actionFunc;
    /* 0x1DC */ LightNode* unk_1DC;
    /* 0x1E0 */ LightInfo unk_1E0;
    /* 0x1F0 */ LightNode* unk_1F0;
    /* 0x1F4 */ LightInfo unk_1F4;
    /* 0x202 */ u8 unk_202;
    /* 0x203 */ u8 unk_203;
    /* 0x204 */ f32 unk_204;
    /* 0x208 */ u8 cueId;
} DoorWarp1; // size = 0x20C

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 1000, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDistance, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 4000, ICHAIN_STOP),
};

s32 func_808B849C(DoorWarp1* this, PlayState* play);
void func_808B9E94(DoorWarp1* this, PlayState* play);
void DoorWarp1_SetupAction(DoorWarp1* this, DoorWarp1ActionFunc actionFunc);

RECOMP_PATCH s32 func_808B849C(DoorWarp1* this, PlayState* play) {
    s32 ret = 0;

    //if ((play->sceneId == SCENE_MITURIN_BS) && !CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA)) {
    if ((play->sceneId == SCENE_MITURIN_BS) && !rando_location_is_checked(LOCATION_REMAINS_ODOLWA)) {
        ret = 1;
    //} else if ((play->sceneId == SCENE_HAKULOCATIONN_BS) && !CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT)) {
    } else if ((play->sceneId == SCENE_HAKUGIN_BS) && !rando_location_is_checked(LOCATION_REMAINS_GOHT)) {
        ret = 2;
    //} else if ((play->sceneId == SCENE_SEA_BS) && !CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG)) {
    } else if ((play->sceneId == SCENE_SEA_BS) && !rando_location_is_checked(LOCATION_REMAINS_GYORG)) {
        ret = 3;
    //} else if ((play->sceneId == SCENE_INISIE_BS) && !CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD)) {
    } else if ((play->sceneId == SCENE_INISIE_BS) && !rando_location_is_checked(LOCATION_REMAINS_TWINMOLD)) {
        ret = 4;
    }
    return ret;
}

void func_808B9CE8(DoorWarp1* this, PlayState* play);
void func_808B9F10(DoorWarp1* this, PlayState* play);

RECOMP_PATCH void func_808B9BE8(DoorWarp1* this, PlayState* play) {
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    Math_SmoothStepToF(&this->unk_1B0, 255.0f, 0.2f, 2.0f, 0.1f);
    Math_SmoothStepToF(&this->unk_1B4, 255.0f, 0.2f, 2.0f, 0.1f);
    if (this->unk_1C4 < 10) {
        this->unk_1C4 += 2;
    }

    if (this->unk_1C6 < 120) {
        this->unk_1C6 += 4;
    }

    if (this->unk_1C8 < 230) {
        this->unk_1C8 += 4;
        return;
    }

    this->dyna.actor.parent = NULL;
    if (func_808B849C(this, play)) {
        //this->unk_202 = 1;
        DoorWarp1_SetupAction(this, func_808B9CE8);
    } else {
        DoorWarp1_SetupAction(this, func_808B9F10);
    }
}

extern CollisionHeader gWarpBossWarpPlatformCol;

void func_808B8568(DoorWarp1* this, PlayState* play);
void func_808B8924(DoorWarp1* this, PlayState* play);
void func_808B8A7C(DoorWarp1* this, PlayState* play);
void func_808B8C48(DoorWarp1* this, PlayState* play);
void func_808B8E78(DoorWarp1* this, PlayState* play);

RECOMP_PATCH void DoorWarp1_Init(Actor* thisx, PlayState* play) {
    DoorWarp1* this = THIS;

    this->unk_1CC = 0;
    this->unk_202 = 0;
    this->unk_203 = 0;
    this->unk_1A0 = NULL;
    this->unk_1C0 = 0.0f;
    Actor_ProcessInitChain(&this->dyna.actor, sInitChain);
    ActorShape_Init(&this->dyna.actor.shape, 0.0f, NULL, 0.0f);

    this->unk_1D3 = 0;
    this->unk_1D4 = 0;
    this->unk_203 = 0;
    this->unk_204 = 1.0f;

    SET_WEEKEVENTREG(WEEKEVENTREG_86_80);

    switch (DOORWARP1_GET_FF(&this->dyna.actor)) {
        case ENDOORWARP1_FF_0:
        case ENDOORWARP1_FF_1:
        case ENDOORWARP1_FF_2:
        case ENDOORWARP1_FF_3:
        case ENDOORWARP1_FF_4:
        case ENDOORWARP1_FF_5:
            func_808B8568(this, play);
            break;
    }

    switch (DOORWARP1_GET_FF(&this->dyna.actor)) {
        case ENDOORWARP1_FF_0:
            func_808B8924(this, play);
            break;

        case ENDOORWARP1_FF_1:
            func_808B8A7C(this, play);
            break;

        case ENDOORWARP1_FF_2:
        case ENDOORWARP1_FF_3:
        case ENDOORWARP1_FF_4:
        case ENDOORWARP1_FF_5:
            this->unk_1D3 = 1;
            DynaPolyActor_Init(&this->dyna, 0);
            DynaPolyActor_LoadMesh(play, &this->dyna, &gWarpBossWarpPlatformCol);
            func_808B8C48(this, play);
            break;

        case ENDOORWARP1_FF_6:
            func_808B8E78(this, play);
            break;
    }

    if ((play->sceneId == SCENE_MITURIN_BS) || (play->sceneId == SCENE_HAKUGIN_BS) ||
        (play->sceneId == SCENE_INISIE_BS) || (play->sceneId == SCENE_SEA_BS)) {
        Environment_StopTime();
        play->interfaceCtx.restrictions.songOfTime = 1;
        play->interfaceCtx.restrictions.songOfSoaring = 1;
    }
}
/*
void func_808B9CE8(DoorWarp1* this, PlayState* play) {
    if (this->unk_203 != 0) {
        if (1) {}
        return;
    }

    if (!Actor_HasParent(&this->dyna.actor, play)) {
        Actor_OfferGetItem(&this->dyna.actor, play, (GI_REMAINS_ODOLWA - 1) + func_808B849C(this, play), 30.0f, 80.0f);
        return;
    }

    switch (play->sceneId) {
        case SCENE_MITURIN_BS:
            gSaveContext.save.saveInfo.unk_EA8[0] = (((void)0, gSaveContext.save.saveInfo.unk_EA8[0]) & 0xFFFFFF00) |
                                                    (((u8)gSaveContext.save.saveInfo.unk_EA8[1]) & 0xFF);
            break;

        case SCENE_HAKUGIN_BS:
            gSaveContext.save.saveInfo.unk_EA8[0] = (((void)0, gSaveContext.save.saveInfo.unk_EA8[0]) & 0xFFFF00FF) |
                                                    ((((u8)gSaveContext.save.saveInfo.unk_EA8[1]) & 0xFF) << 8);
            break;

        case SCENE_INISIE_BS:
            gSaveContext.save.saveInfo.unk_EA8[0] = (((void)0, gSaveContext.save.saveInfo.unk_EA8[0]) & 0xFF00FFFF) |
                                                    ((((u8)gSaveContext.save.saveInfo.unk_EA8[1]) & 0xFF) << 0x10);
            break;

        case SCENE_SEA_BS:
            gSaveContext.save.saveInfo.unk_EA8[0] = (((void)0, gSaveContext.save.saveInfo.unk_EA8[0]) & 0x00FFFFFF) |
                                                    ((((u8)gSaveContext.save.saveInfo.unk_EA8[1]) & 0xFF) << 0x18);
            break;

        default:
            break;
    }

    gSaveContext.save.saveInfo.unk_EA8[1] = (gSaveContext.save.saveInfo.unk_EA8[1] & 0xFFFFFF00) |
                                            ((((u8)gSaveContext.save.saveInfo.unk_EA8[1]) + 1) & 0xFF);
    //Item_Give(play, func_808B849C(this, play) + (ITEM_REMAINS_ODOLWA - 1));
    DoorWarp1_SetupAction(this, func_808B9E94);
}*/