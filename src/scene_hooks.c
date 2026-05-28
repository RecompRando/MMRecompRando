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

    // Entrance Rando (system will need to change with full entrance rando)
    if (saveOpened && (rando_get_slotdata_u32("dungeon_entrance_rando") || rando_get_slotdata_u32("boss_entrance_rando"))) {
        REPY_FN_SETUP_RANDO;

        // TODO: move to a python file
        REPY_FN_SET_S16("last_scene", savedSceneId); // technically "last scene", but runs before that gets changed
        REPY_FN_SET_S32("current_entrance", gSaveContext.save.entrance); // where we're meant to be going to

        REPY_FN_EXEC_CACHE(
            rando_set_entrance_rando,
            "er_placements = recomp_data.ctx.slot_data[\"entrance_rando_results\"]\n"
            "entrance_id_to_entrance_lookup = {\n"
            "    0x8610: \"Woodfall\",\n"
            "    0x3000: \"Woodfall Temple\",\n"
            "    0x3800: \"Odolwa's Lair\",\n"
            "    0xB210: \"Snowhead\",\n"
            "    0x3C00: \"Snowhead Temple\",\n"
            "    0x8200: \"Goht's Lair\",\n"
            "    0x6A70: \"Zora Cape\",\n"
            "    0x8C00: \"Great Bay Temple\",\n"
            "    0xB800: \"Gyorg's Lair\",\n"
            "    0xAC10: \"Stone Tower (Inverted)\",\n"
            "    0x2A00: \"Stone Tower Temple (Inverted)\",\n"
            "    0x6600: \"Twinmold's Lair\",\n"
            "}\n"
            "scene_id_to_name = {\n"
            "    0x46: \"Woodfall\",\n"
            "    0x1B: \"Woodfall Temple\",\n"
            "    0x1F: \"Odolwa's Lair\",\n"
            "    0x5C: \"Snowhead\",\n"
            "    0x21: \"Snowhead Temple\",\n"
            "    0x44: \"Goht's Lair\",\n"
            "    0x38: \"Zora Cape\",\n"
            "    0x49: \"Great Bay Temple\",\n"
            "    0x5F: \"Gyorg's Lair\",\n"
            "    0x59: \"Stone Tower (Inverted)\",\n"
            "    0x18: \"Stone Tower Temple (Inverted)\",\n"
            "    0x36: \"Twinmold's Lair\",\n"
            "}\n"
            "in_lookup = current_entrance in entrance_id_to_entrance_lookup and last_scene in scene_id_to_name" // TODO: not enough to prevent some debug crashes
        );

        if (!REPY_FN_GET_BOOL("in_lookup")) {
            REPY_FN_CLEANUP;
            return;
        }

        bool entering = true; // entering dungeon/boss room, exiting dungeon on false

        // determine dungeon directionality (exiting or going to boss)
        switch (savedSceneId) {
            case SCENE_MITURIN: // woodfall temple
                if (gSaveContext.save.entrance == ENTRANCE(WOODFALL, 1)) {
                    entering = false;
                }
                break;
            case SCENE_HAKUGIN: // snowhead temple
                if (gSaveContext.save.entrance == ENTRANCE(SNOWHEAD, 1)) {
                    entering = false;
                }
                break;
            case SCENE_SEA: // great bay temple
                if (gSaveContext.save.entrance == ENTRANCE(ZORA_CAPE, 7)) {
                    entering = false;
                }
                break;
            case SCENE_INISIE_R: // inverted stone tower temple
                if (gSaveContext.save.entrance == ENTRANCE(STONE_TOWER_INVERTED, 1)) {
                    entering = false;
                }
                break;
        }

        if (entering) {
            REPY_FN_EXEC_CACHE(
                rando_do_entering_entrance_rando,
                "new_entrance_name = er_placements[scene_id_to_name[last_scene]]['to']\n"
                "new_entrance = er_placements[new_entrance_name]['entrance_id']\n"
                "print(er_placements[new_entrance_name])\n"
            );
            gSaveContext.save.entrance = REPY_FN_GET_S32("new_entrance");
        } else {
            REPY_FN_EXEC_CACHE(
                rando_do_exiting_entrance_rando,
                "entrance_name = scene_id_to_name[last_scene]\n"
                "while er_placements[entrance_name]['from']:\n"
                "   entrance_name = er_placements[entrance_name]['from']\n"
                "new_entrance = er_placements[entrance_name]['entrance_id']\n"
            );
            gSaveContext.save.entrance = REPY_FN_GET_S32("new_entrance");
        }

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