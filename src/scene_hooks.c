#include "global.h"
#include "modding.h"
#include "recomputils.h"

#include "apcommon.h"

#define ER_AMOUNT rando_get_slotdata_u32("er_placement_count")

// this only handles MMR style entrance rando for now
void handleER(u32 er_lookup, u32 type) {
    u32 er_placements[ER_AMOUNT];
    u32 incomingPtr[2]; // not sure why this needs to be 2
    u32 er_result;

    if (!rando_get_slotdata_u32("entrance_rando")) return;

    // TODO: ignore respawns
    
    rando_get_slotdata_raw_o32("er_placement_ids", er_placements);

    rando_access_slotdata_raw_array_o32(er_placements, er_lookup, incomingPtr);
    er_result = rando_access_slotdata_raw_u32_o32(incomingPtr) & 0xF;

    if (type == 0) { // dungeon entrances
        switch (er_result) {
            case 0:
                gSaveContext.save.entrance = ENTRANCE(WOODFALL_TEMPLE, 0);
                return;
            case 1:
                gSaveContext.save.entrance = ENTRANCE(SNOWHEAD_TEMPLE, 0);
                return;
            case 2:
                gSaveContext.save.entrance = ENTRANCE(GREAT_BAY_TEMPLE, 0);
                return;
            case 3:
                gSaveContext.save.entrance = ENTRANCE(STONE_TOWER_TEMPLE_INVERTED, 0);
                return;
        }
    } else if (type == 1) { // dungeon exits
        // reverse id to figure out original entrance
        u32 reverse_result = 0;
        for (int i = 0; i < 4; i++) {
            rando_access_slotdata_raw_array_o32(er_placements, i, incomingPtr);

            er_result = rando_access_slotdata_raw_u32_o32(incomingPtr) & 0xF;

            if (er_result == er_lookup) {
                reverse_result = i;
                break;
            }
        }
        
        switch (reverse_result) {
            case 0:
                gSaveContext.save.entrance = ENTRANCE(WOODFALL, 1);
                return;
            case 1:
                gSaveContext.save.entrance = ENTRANCE(SNOWHEAD, 1);
                return;
            case 2:
                gSaveContext.save.entrance = ENTRANCE(ZORA_CAPE, 7);
                return;
            case 3:
                gSaveContext.save.entrance = ENTRANCE(STONE_TOWER_INVERTED, 1);
                return;
        }
    } else { // boss entrances
        switch (er_result) {
            case 4:
                gSaveContext.save.entrance = ENTRANCE(ODOLWAS_LAIR, 0);
                return;
            case 5:
                gSaveContext.save.entrance = ENTRANCE(GOHTS_LAIR, 0);
                return;
            case 6:
                gSaveContext.save.entrance = ENTRANCE(GYORGS_LAIR, 0);
                return;
            case 7:
                gSaveContext.save.entrance = ENTRANCE(TWINMOLDS_LAIR, 0);
                return;
        }
    }
}

RECOMP_HOOK("Play_Init")
void onPlayInit(GameState* thisx) {
    PlayState* play = (PlayState*)thisx;

    recomp_printf("current entrance: 0x%04X 0x%02X %d\n", gSaveContext.save.entrance, (gSaveContext.save.entrance >> 9) & 0x7F, (gSaveContext.save.entrance >> 4) & 0x1F);

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

        // entrance rando

        // dungeon entrances
        case ENTRANCE(WOODFALL_TEMPLE, 0):
            handleER(0, 0);
            break;
        case ENTRANCE(SNOWHEAD_TEMPLE, 0):
            handleER(1, 0);
            break;
        case ENTRANCE(GREAT_BAY_TEMPLE, 0):
            handleER(2, 0);
            break;
        case ENTRANCE(STONE_TOWER_TEMPLE_INVERTED, 0):
            handleER(3, 0);
            break;

        // dungeon exits
        case ENTRANCE(WOODFALL, 1):
            handleER(0, 1);
            break;
        case ENTRANCE(SNOWHEAD, 1):
            handleER(1, 1);
            break;
        case ENTRANCE(ZORA_CAPE, 7):
            handleER(2, 1);
            break;
        case ENTRANCE(STONE_TOWER_INVERTED, 1):
            handleER(3, 1);
            break;

        // boss entrances
        case ENTRANCE(ODOLWAS_LAIR, 0):
            handleER(4, 2);
            break;
        case ENTRANCE(GOHTS_LAIR, 0):
            handleER(5, 2);
            break;
        case ENTRANCE(GYORGS_LAIR, 0):
            handleER(6, 2);
            break;
        // case ENTRANCE(TWINMOLDS_LAIR, 0):
        case ENTRANCE(STONE_TOWER_TEMPLE_INVERTED, 1):
            handleER(7, 2);
            break;

        default:
            break;
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