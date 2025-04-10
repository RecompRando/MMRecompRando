#include "modding.h"
#include "global.h"

#include "apcommon.h"

// @ap send death link when moon crashes
RECOMP_PATCH void Interface_StartMoonCrash(PlayState* play) {
    if (play->actorCtx.flags & ACTORCTX_FLAG_1) {
        SEQCMD_DISABLE_PLAY_SEQUENCES(false);
    }

    if (rando_get_death_link_enabled() && !rando_get_death_link_pending()) {
        rando_send_death_link();
    }

    gSaveContext.save.day = 4;
    gSaveContext.save.eventDayCount = 4;
    gSaveContext.save.time = CLOCK_TIME(6, 0) + 10;
    play->nextEntrance = ENTRANCE(TERMINA_FIELD, 12);
    gSaveContext.nextCutsceneIndex = 0;
    play->transitionTrigger = TRANS_TRIGGER_START;
    play->transitionType = TRANS_TYPE_FADE_WHITE;
}