#include "global.h"
#include "modding.h"
#include "recomputils.h"

#include "apcommon.h"

s32 entrance_rando_lookup[] = {
    // dungeon entrances
    ENTRANCE(WOODFALL_TEMPLE, 0),
    ENTRANCE(SNOWHEAD_TEMPLE, 0),
    ENTRANCE(GREAT_BAY_TEMPLE, 0),
    ENTRANCE(STONE_TOWER_TEMPLE_INVERTED, 0),
    // boss entrances
    ENTRANCE(ODOLWAS_LAIR, 0),
    ENTRANCE(GOHTS_LAIR, 0),
    ENTRANCE(GYORGS_LAIR, 0),
    ENTRANCE(TWINMOLDS_LAIR, 0),
};

// dungeon -> exit
s32 entrance_rando_exit_lookup[] = {
    ENTRANCE(WOODFALL, 1),
    ENTRANCE(SNOWHEAD, 1),
    ENTRANCE(ZORA_CAPE, 7),
    ENTRANCE(STONE_TOWER_INVERTED, 1),
};

#define MAX_ER 8

u32 reverseERLookup(u32 placements, u32 index) {
    u32 searchIndex = 0;
    while (index != ((placements >> (searchIndex * 4)) & 0xF) && searchIndex < MAX_ER) {
        searchIndex++;
    }

    // only return dungeon entrances (rightmost four digits)
    if (searchIndex >= 4) {
        return reverseERLookup(placements, searchIndex - 4);
    }

    return searchIndex;
}

extern s16 savedSceneId;

RECOMP_HOOK("Play_Init")
void onPlayInit(GameState* thisx) {
    PlayState* play = (PlayState*)thisx;
    gPlay = play;

    switch (gSaveContext.save.entrance) {
        // change the intro cutscene where you fall down into a new cycle
        case ENTRANCE(OPENING_DUNGEON, 0):
            // copied from the osn patch lol
            play->nextEntrance = ENTRANCE(SOUTH_CLOCK_TOWN, 0);
            gSaveContext.save.entrance = play->nextEntrance;
            play->state.running = false;
            play->state.init = gGameStateOverlayTable[GAMESTATE_PLAY].init;
            play->state.size = gGameStateOverlayTable[GAMESTATE_PLAY].instanceSize;
            gSaveContext.save.day = 0;
            gSaveContext.save.time = CLOCK_TIME(6, 0);
            Sram_SaveEndOfCycle(play);
            break;
        // change the respawn point in the pre-clock town area
        case ENTRANCE(OPENING_DUNGEON, 3):
            gSaveContext.save.entrance = ENTRANCE(OPENING_DUNGEON, 1);
            break;
        default:
            break;
    }

    // recomp_printf("ENTRANCE RANDO IDS\n");
    // for (int i = 0; i < ARRAY_COUNT(entrance_rando_lookup); i++) {
    //     recomp_printf("0x%04X\n", entrance_rando_lookup[i]);
    // }
    // for (int i = 0; i < ARRAY_COUNT(entrance_rando_exit_lookup); i++) {
    //     recomp_printf("0x%04X\n", entrance_rando_exit_lookup[i]);
    // }
    // recomp_printf("ENTRANCE RANDO END\n");

    // Entrance Rando
    if (saveOpened && (rando_get_slotdata_u32("dungeon_entrance_rando") || rando_get_slotdata_u32("boss_entrance_rando"))) {
        REPY_FN_SETUP_RANDO;

        REPY_FN_SET_S32("current_entrance", gSaveContext.save.entrance); // where we're meant to be going to

        recomp_printf("current entrance 0x%04X %d\n", gSaveContext.save.entrance, gSaveContext.save.entrance); // %d due to python print in apworld
        recomp_printf("respawn flag %d\n", gSaveContext.respawnFlag);
        
        // song of soaring out of dungeons/bosses? (this doesn't work lmao)
        if (gSaveContext.respawnFlag == -6) {
            s8 boss_region;
            if (play->sceneId == SCENE_MITURIN_BS) {
                boss_region = 0;
            } else if (play->sceneId == SCENE_HAKUGIN_BS) {
                boss_region = 1;
            } else if (play->sceneId == SCENE_SEA_BS) {
                boss_region = 3;
            } else if (play->sceneId == SCENE_INISIE_BS) {
                boss_region = 2;
            } else {
                boss_region = -1;
            }

            // warping out of a boss room warps to the start of the chain
            if (boss_region >= 0) {
                recomp_printf("warping out of a boss room\n");

                REPY_FN_SET_S32("current_boss", boss_region);

                REPY_FN_EXEC_CACHE(
                    rando_get_boss_soaring_warp,
                    "boss_placements = recomp_data.ctx.slot_data[\"boss_regions\"]\n"
                    "real_region = boss_placements[str(current_boss)]"
                );

                s8 real_region = REPY_FN_GET_S8("real_region");

                switch(real_region) {
                    case 0:
                        gSaveContext.save.entrance = ENTRANCE(WOODFALL, 1);
                        break;
                    case 1:
                        gSaveContext.save.entrance = ENTRANCE(SNOWHEAD, 1);
                        break;
                    case 2:
                        gSaveContext.save.entrance = ENTRANCE(ZORA_CAPE, 7);
                        break;
                    case 3:
                        gSaveContext.save.entrance = ENTRANCE(STONE_TOWER_INVERTED, 1);
                        break;
                }
            }

            REPY_FN_CLEANUP; // do nothing when inside of dungeons (warp to the start of them)
            return;
        }

        REPY_FN_EXEC_CACHE(
            rando_check_entrance_rando,
            "er_placements = recomp_data.ctx.slot_data[\"entrance_rando_results\"]\n"
            "in_lookup = str(current_entrance) in er_placements"
        );

        if (!REPY_FN_GET_BOOL("in_lookup") ||
            gSaveContext.respawnFlag == -5) { // voidout? (-6 seems to be owl warps)
            REPY_FN_CLEANUP;
            return;
        }

        recomp_printf("doing entrance rando\n");

        REPY_FN_EVAL_CACHE_S32(
            rando_do_entrance_rando,
            "er_placements[str(current_entrance)]",
            new_entrance
        );

        gSaveContext.save.entrance = new_entrance;

        REPY_FN_CLEANUP;
    }
}

RECOMP_HOOK_RETURN("Play_Init")
void postPlayInit() {
    // send the current scene id to datastorage
    rando_datastorage_replace_u32("scene", (u32)gPlay->sceneId);
}

// returns a fake sceneId based on different parameters
u16 getCurrentGrotto(PlayState* play) {
    s8 entrance = (gSaveContext.save.entrance >> 4) & 0x1F;
    switch (entrance) {
        case 0x4: // chest grotto
            // 0x11 is max grotto entrance + 1, 0x1F is ENBOX_GET_CHEST_FLAG
            entrance = 0x11 + (gSaveContext.respawn[RESPAWN_MODE_UNK_3].data & 0x1F);
            break;
        case 0xA: // cows
            entrance = Entrance_GetSceneIdAbsolute(((void)0, gSaveContext.respawn[RESPAWN_MODE_UNK_3].entrance));
            break;
    }
    return 0x80 + entrance; // 0x80 is outside the range of real sceneIds
}

s16 randoGetScene(PlayState* play) {
    s16 sceneId = play->sceneId;
    
    if (sceneId == SCENE_KAKUSIANA) {
        return getCurrentGrotto(play);
    }

    // Play_GetOriginalSceneId()
    // TODO: change to check for "alternate region" option
    if (true) {
        // Purified Southern Swamp -> Poisoned Sothern Swamp
        if (sceneId == SCENE_20SICHITAI2) {
            return SCENE_20SICHITAI;
        }

        // Spring Mountain Village -> Winter Mountain Village
        if (sceneId == SCENE_10YUKIYAMANOMURA2) {
            return SCENE_10YUKIYAMANOMURA;
        }

        // Spring Goron Village -> Winter Goron Village
        if (sceneId == SCENE_11GORONNOSATO2) {
            return SCENE_11GORONNOSATO;
        }

        // Spring Path to Goron Village -> Winter Path to Goron Village
        if (sceneId == SCENE_17SETUGEN2) {
            return SCENE_17SETUGEN;
        }
    }

    return sceneId;
}