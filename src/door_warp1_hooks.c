#include "modding.h"
#include "global.h"

#include "apcommon.h"

RECOMP_IMPORT(".", bool rando_get_remains_allow_boss_warps_enabled());

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
#define LOCATION_SONG_OATH 0x040065

#define IN_BOSS_ROOM ((play->sceneId == SCENE_MITURIN_BS) || (play->sceneId == SCENE_HAKUGIN_BS) || \
                      (play->sceneId == SCENE_INISIE_BS) || (play->sceneId == SCENE_SEA_BS))

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
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 4000, ICHAIN_STOP),
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

void func_808B9E94(DoorWarp1* this, PlayState* play);
void func_808B9ED8(DoorWarp1* this, PlayState* play);

RECOMP_PATCH void func_808B9E94(DoorWarp1* this, PlayState* play) {
    // @rando don't count down warp unless oath is checked in DmHina (remains)
    if (IN_BOSS_ROOM && !rando_location_is_checked(LOCATION_SONG_OATH)) {
        return;
    }

    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        this->unk_1CE = 110;
        DoorWarp1_SetupAction(this, func_808B9ED8);
    }
}

void func_808B9FD0(DoorWarp1* this, PlayState* play);
s32 func_808B866C(DoorWarp1* this, PlayState* play);

RECOMP_PATCH void func_808B9F10(DoorWarp1* this, PlayState* play) {
    // @rando backup offer in case remains location was checked without oath location (i.e. from a release)
    if (IN_BOSS_ROOM && !Actor_HasParent(&this->dyna.actor, play) && !rando_location_is_checked(LOCATION_SONG_OATH)) {
        Actor_OfferGetItemHook(&this->dyna.actor, play, rando_get_item_id(LOCATION_SONG_OATH), LOCATION_SONG_OATH, 30.0f, 80.0f, true, true);
        return;
    }
    
    Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_WARP_HOLE - SFX_FLAG);
    if ((this->unk_203 == 0) && func_808B866C(this, play) && !Play_InCsMode(play) && (this->unk_203 == 0)) {
        Player* player = GET_PLAYER(play);

        Interface_SetHudVisibility(HUD_VISIBILITY_NONE);
        func_800B7298(play, &this->dyna.actor, PLAYER_CSACTION_9);
        player->unk_3A0.x = this->dyna.actor.world.pos.x;
        player->unk_3A0.z = this->dyna.actor.world.pos.z;
        this->unk_1CA = 20;
        DoorWarp1_SetupAction(this, func_808B9FD0);
    }
}

void func_808BAAF4(DoorWarp1* this, PlayState* play);
void func_808BABF4(DoorWarp1* this, PlayState* play);

RECOMP_PATCH void func_808B8C48(DoorWarp1* this, PlayState* play) {
    this->dyna.actor.shape.yOffset = 0.0f;
    Actor_SetScale(&this->dyna.actor, 0.1f);
    Lights_PointNoGlowSetInfo(&this->unk_1E0, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 200, 255, 255, 255);
    Lights_PointNoGlowSetInfo(&this->unk_1F4, this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y,
                              this->dyna.actor.world.pos.z, 200, 255, 255, 255);
    if (((DOORWARP1_GET_FF(&this->dyna.actor) == ENDOORWARP1_FF_2)
            && ((CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA) && rando_get_remains_allow_boss_warps_enabled())
                || rando_location_is_checked(LOCATION_REMAINS_ODOLWA))) ||
        ((DOORWARP1_GET_FF(&this->dyna.actor) == ENDOORWARP1_FF_3)
            && ((CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT) && rando_get_remains_allow_boss_warps_enabled())
                || rando_location_is_checked(LOCATION_REMAINS_GOHT))) ||
        ((DOORWARP1_GET_FF(&this->dyna.actor) == ENDOORWARP1_FF_4)
            && ((CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG) && rando_get_remains_allow_boss_warps_enabled())
                || rando_location_is_checked(LOCATION_REMAINS_GYORG))) ||
        ((DOORWARP1_GET_FF(&this->dyna.actor) == ENDOORWARP1_FF_5)
            && ((CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD) && rando_get_remains_allow_boss_warps_enabled())
                || rando_location_is_checked(LOCATION_REMAINS_TWINMOLD)))) {
        s16 params = DOORWARP1_GET_FF00_2(&this->dyna.actor);

        params |= 6;
        Actor_SpawnAsChild(&play->actorCtx, &this->dyna.actor, play, ACTOR_DOOR_WARP1, this->dyna.actor.world.pos.x,
                           this->dyna.actor.world.pos.y + 10.0f, this->dyna.actor.world.pos.z,
                           this->dyna.actor.world.rot.x, this->dyna.actor.world.rot.y, this->dyna.actor.world.rot.z,
                           params);
        DoorWarp1_SetupAction(this, func_808BAAF4);
    } else {
        DoorWarp1_SetupAction(this, func_808BABF4);
    }
}

#include "recomputils.h"

s16 doorWarp1SavedSceneID;
// s16 doorWarp1ReplaceSceneID;

// probably broken, logic is mostly there just needs a bit more tweaking
// todo: fix cleared boss warps as well
RECOMP_HOOK("func_808BA10C")
void DoorWarp1_ChangeERExit(DoorWarp1* this, PlayState* play) {
    doorWarp1SavedSceneID = play->sceneId;
    
    if (!rando_get_slotdata_u32("entrance_rando")) return;

    u32 bossSceneIndex;

    if (!(play->sceneId == SCENE_MITURIN_BS) || (play->sceneId == SCENE_HAKUGIN_BS) ||
        (play->sceneId == SCENE_INISIE_BS) || (play->sceneId == SCENE_SEA_BS)) {
        return;
    }

    if (play->sceneId == SCENE_MITURIN_BS) {
        bossSceneIndex = 0;
    } else if (play->sceneId == SCENE_HAKUGIN_BS) {
        bossSceneIndex = 1;
    } else if (play->sceneId == SCENE_SEA_BS) {
        bossSceneIndex = 2;
    } else if (play->sceneId == SCENE_INISIE_BS) {
        bossSceneIndex = 3;
    } else {
        bossSceneIndex = 0;
    }

    // double reverse er index adventure

    u32 er_placements[rando_get_slotdata_u32("er_placement_count")];
    u32 incomingPtr[2]; // not sure why this needs to be 2
    u32 boss = 0;
    u32 original_boss = 0;
    
    rando_get_slotdata_raw_o32("er_placement_ids", er_placements);

    // figure out what the original dungeon was
    for (int i = 4; i < 8; i++) { // 4 - 7 for boss er indexes
        rando_access_slotdata_raw_array_o32(er_placements, i, incomingPtr);

        boss = rando_access_slotdata_raw_u32_o32(incomingPtr) & 0xF;

        if (boss == bossSceneIndex + 4) {
            original_boss = i;
            break;
        }
    }

    rando_access_slotdata_raw_array_o32(er_placements, original_boss - 4, incomingPtr);
    u32 incoming_dungeon = rando_access_slotdata_raw_u32_o32(incomingPtr) & 0xF;
    u32 dungeon = 0;
    u32 dungeon_entrance = 0;

    // figure out what the original dungeon entrance was
    for (int i = 0; i < 4; i++) { // 1 - 3 for boss er indexes
        rando_access_slotdata_raw_array_o32(er_placements, i, incomingPtr);

        dungeon = rando_access_slotdata_raw_u32_o32(incomingPtr) & 0xF;

        if (dungeon == incoming_dungeon) {
            dungeon_entrance = i;
            break;
        }
    }

    switch (dungeon_entrance) {
        case 0:
            play->sceneId = SCENE_MITURIN_BS;
            break;
        case 1:
            play->sceneId = SCENE_HAKUGIN_BS;
            break;
        case 2:
            play->sceneId = SCENE_SEA_BS;
            break;
        case 3:
            play->sceneId = SCENE_INISIE_BS;
            break;
    }

    recomp_printf("results %d %d\n", original_boss, dungeon_entrance);
}

// RECOMP_HOOK("func_808BA10C")
// void DoorWarp1_FixSceneAfterER() {
//     recomp_printf("cur scene id 0x%02X\n", gPlay->sceneId);
//     gPlay->sceneId = doorWarp1SavedSceneID;
//     recomp_printf("fixed scene id 0x%02X\n", gPlay->sceneId);
// }