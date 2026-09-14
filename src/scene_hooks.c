#include "global.h"
#include "modding.h"
#include "recomputils.h"

#include "apcommon.h"

RECOMP_HOOK("Play_Init")
void onPlayInit(GameState* thisx) {
    PlayState* play = (PlayState*)thisx;
    gPlay = play;
}

RECOMP_HOOK_RETURN("Play_Init")
void postPlayInit() {
    // send the current scene id to datastorage
    rando_datastorage_replace_u32("scene", (u32)gPlay->sceneId);
}