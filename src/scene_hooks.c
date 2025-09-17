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