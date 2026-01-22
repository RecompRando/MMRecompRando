#include "modding.h"
#include "global.h"
#include "recompconfig.h"
#include "recomputils.h"

#include "apcommon.h"

#include "overlays/actors/ovl_Door_Warp1/z_door_warp1.h"

RECOMP_IMPORT(".", bool rando_get_remains_allow_boss_warps_enabled());

#define FLAGS 0x00000000

#define THIS ((DoorWarp1*)thisx)

#define LOCATION_REMAINS_ODOLWA GI_REMAINS_ODOLWA
#define LOCATION_REMAINS_GOHT GI_REMAINS_GOHT
#define LOCATION_REMAINS_GYORG GI_REMAINS_GYORG
#define LOCATION_REMAINS_TWINMOLD GI_REMAINS_TWINMOLD
#define LOCATION_SONG_OATH 0x040065

#define IN_BOSS_ROOM ((play->sceneId == SCENE_MITURIN_BS) || (play->sceneId == SCENE_HAKUGIN_BS) || \
                      (play->sceneId == SCENE_INISIE_BS) || (play->sceneId == SCENE_SEA_BS))

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

RECOMP_HOOK("DoorWarp1_Init")
void DoorWarp1_SetFlag(Actor* thisx, PlayState* play) {
    SET_WEEKEVENTREG(WEEKEVENTREG_86_80);
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

// @rando override boss warps for entrance rando
bool originalDungeonFlags[4];
bool shouldOverrideDungeonWarp;
bool shouldOverrideDungeonFlag;
s32 curBossDungeon;
static s32 realBossDungeon;

RECOMP_HOOK("func_808BA10C")
void DoorWarp1_BeforeSettingWarp(DoorWarp1* this, PlayState* play) {
    shouldOverrideDungeonWarp = false;
    shouldOverrideDungeonFlag = false;
    curBossDungeon = -1;
    realBossDungeon = -1;

    originalDungeonFlags[0] = CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE);
    originalDungeonFlags[1] = CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_SNOWHEAD_TEMPLE);
    originalDungeonFlags[2] = CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_GREAT_BAY_TEMPLE);
    originalDungeonFlags[3] = CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_STONE_TOWER_TEMPLE);
    
    if ((play->sceneId == SCENE_MITURIN_BS) || (play->sceneId == SCENE_HAKUGIN_BS) ||
        (play->sceneId == SCENE_INISIE_BS) || (play->sceneId == SCENE_SEA_BS)) {
        gDungeonBossWarpSceneId = play->sceneId;
        // Confusingly, Great Bay Temple and Stone Tower have their numbers swapped around
        if (play->sceneId == SCENE_MITURIN_BS) {
            curBossDungeon = 0;
        } else if (play->sceneId == SCENE_HAKUGIN_BS) {
            curBossDungeon = 1;
        } else if (play->sceneId == SCENE_SEA_BS) {
            curBossDungeon = 3;
        } else if (play->sceneId == SCENE_INISIE_BS) {
            curBossDungeon = 2;
        } else {
            curBossDungeon = -1;
        }

        if (this->unk_202 == 0 && recomp_get_config_u32("dungeon_warp_override")) { // add check for entrance rando
            shouldOverrideDungeonWarp = true;
            shouldOverrideDungeonFlag = true;
            realBossDungeon = recomp_get_config_u32("dungeon_warp_override") - 1;
        }
    }
}

RECOMP_HOOK_RETURN("func_808BA10C")
void DoorWarp1_AfterSettingWarp() {
    PlayState* play = gPlay;

    if (curBossDungeon < 0) return; // invalid warp that should never occur

    if (shouldOverrideDungeonFlag) {
        // Clear the dungeon flag set by the vanilla function
        if (!originalDungeonFlags[curBossDungeon]) {
            switch (curBossDungeon) {
                // case 0: // this actually should never happen, keeping it commented out just in case
                //     CLEAR_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE);
                //     break;
                case 1:
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_CLEARED_SNOWHEAD_TEMPLE);
                    break;
                case 3:
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_CLEARED_GREAT_BAY_TEMPLE);
                    break;
                case 2:
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_CLEARED_STONE_TOWER_TEMPLE);
                    break;
            }
        }
    }

    if (!shouldOverrideDungeonWarp) return;

    switch (realBossDungeon) {
        case 0:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_WOODFALL_TEMPLE)) {
                // Skips the entrance cutscene as this flag is attached to `ENTRANCE(WOODFALL_TEMPLE, 1)`
                SET_WEEKEVENTREG(WEEKEVENTREG_ENTERED_WOODFALL_TEMPLE_PRISON);
                play->nextEntrance = ENTRANCE(WOODFALL_TEMPLE, 1);
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
            } else {
                play->nextEntrance = ENTRANCE(WOODFALL, 0);
                gSaveContext.nextCutsceneIndex = 0xFFF0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
            }
            break;

        case 1:
            SET_WEEKEVENTREG(WEEKEVENTREG_CLEARED_SNOWHEAD_TEMPLE);
            play->nextEntrance = ENTRANCE(MOUNTAIN_VILLAGE_SPRING, 7);
            gSaveContext.nextCutsceneIndex = 0xFFEF; // prevents a crash
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_FADE_WHITE;
            gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
            break;

        case 3:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_CLEARED_GREAT_BAY_TEMPLE)) {
                play->nextEntrance = ENTRANCE(ZORA_CAPE, 9);
                gSaveContext.nextCutsceneIndex = 0xFFF0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
            } else {
                SET_WEEKEVENTREG(WEEKEVENTREG_CLEARED_GREAT_BAY_TEMPLE);
                play->nextEntrance = ENTRANCE(ZORA_CAPE, 8);
                gSaveContext.nextCutsceneIndex = 0xFFF0;
                play->transitionTrigger = TRANS_TRIGGER_START;
                play->transitionType = TRANS_TYPE_FADE_WHITE;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
            }
            break;

        case 2:
            SET_WEEKEVENTREG(WEEKEVENTREG_CLEARED_STONE_TOWER_TEMPLE);
            play->nextEntrance = ENTRANCE(IKANA_CANYON, 15);
            gSaveContext.nextCutsceneIndex = 0xFFF2;
            play->transitionTrigger = TRANS_TRIGGER_START;
            play->transitionType = TRANS_TYPE_FADE_WHITE;
            gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE;
            break;
    }
}

// override the boss warp cutscene if needed
RECOMP_HOOK("CutsceneCmd_Destination")
void CutsceneCmd_OverrideDestination(PlayState* play, CutsceneContext* csCtx, CsCmdDestination* cmd) {
    if (cmd->type == CS_DESTINATION_BOSS_WARP) {
        switch (realBossDungeon) {
            case 0: // wft
                gDungeonBossWarpSceneId = SCENE_MITURIN_BS;
                break;
            case 1: // sht
                gDungeonBossWarpSceneId = SCENE_HAKUGIN_BS;
                break;
            case 3: // gbt
                gDungeonBossWarpSceneId = SCENE_SEA_BS;
                break;
            case 2: // stt
                gDungeonBossWarpSceneId = SCENE_INISIE_BS;
                break;
        }
    }
}