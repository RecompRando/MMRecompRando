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

RECOMP_HOOK("Play_Init")
void onPlayInit(GameState* thisx) {
    PlayState* play = (PlayState*)thisx;

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

    // Entrance Rando
    if (rando_get_slotdata_u32("dungeon_entrance_rando")) {
        u32 placements = rando_get_slotdata_u32("er_placements");
        s32 index = -1;
        u8 ERType = 0; // 0 - none | 1 - entrance | 2 - exit

        switch (gSaveContext.save.entrance) {
            // dungeon entrances
            case ENTRANCE(WOODFALL_TEMPLE, 0):
                index = 0;
                ERType = 1;
                break;
            case ENTRANCE(SNOWHEAD_TEMPLE, 0):
                index = 1;
                ERType = 1;
                break;
            case ENTRANCE(GREAT_BAY_TEMPLE, 0):
                index = 2;
                ERType = 1;
                break;
            case ENTRANCE(STONE_TOWER_TEMPLE_INVERTED, 0):
                index = 3;
                ERType = 1;
                break;

            // dungeon exits
            case ENTRANCE(WOODFALL, 1):
                index = 0;
                ERType = 2;
                break;
            case ENTRANCE(SNOWHEAD, 1):
                index = 1;
                ERType = 2;
                break;
            case ENTRANCE(ZORA_CAPE, 7):
                index = 2;
                ERType = 2;
                break;
            case ENTRANCE(STONE_TOWER_INVERTED, 1):
                index = 3;
                ERType = 2;
                break;
        }
        
        if (ERType == 1) {
            s32 real_index = (placements >> (index * 4)) & 0xF;
            gSaveContext.save.entrance = entrance_rando_lookup[real_index];
            return;
        } else if (ERType == 2) {
            s32 real_index = reverseERLookup(placements, index);
            gSaveContext.save.entrance = entrance_rando_exit_lookup[real_index];
            return;
        }
    }

    if (rando_get_slotdata_u32("boss_entrance_rando")) {
        u32 placements = rando_get_slotdata_u32("er_placements");
        s32 index = -1;

        switch (gSaveContext.save.entrance) {
            case ENTRANCE(ODOLWAS_LAIR, 0):
                index = 4;
                break;
            case ENTRANCE(GOHTS_LAIR, 0):
                index = 5;
                break;
            case ENTRANCE(GYORGS_LAIR, 0):
                index = 6;
                break;
            case ENTRANCE(TWINMOLDS_LAIR, 0):
                index = 7;
                break;
        }

        if (index >= 4) {
            s32 real_index = (placements >> (index * 4)) & 0xF;
            gSaveContext.save.entrance = entrance_rando_lookup[real_index];
            return;
        }
    }
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