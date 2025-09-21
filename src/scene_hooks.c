#include "global.h"
#include "modding.h"
#include "recomputils.h"

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
        // dungeon entrance rando
        // case ENTRANCE(STONE_TOWER_TEMPLE, 0): // stone tower -> stone tower temple
        //     gSaveContext.save.entrance = ENTRANCE(WOODFALL_TEMPLE, 0);
        //     break;
        // case ENTRANCE(WOODFALL, 1): // woodfall temple -> woodfall
        //     gSaveContext.save.entrance = ENTRANCE(STONE_TOWER, 2);
        //     break;
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