// TODO: add ability to turn this off lmao

#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "recomputils.h"

#define LOCATION_NOTEBOOK(event) (0x0B0000 | event)

extern u16 sBombersNotebookEventMessages[BOMBERS_NOTEBOOK_EVENT_MAX];

RECOMP_PATCH void Message_BombersNotebookQueueEvent(PlayState* play, u8 event) {
    MessageContext* msgCtx = &play->msgCtx;

    if (CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
        if (!CHECK_WEEKEVENTREG(gBombersNotebookWeekEventFlags[event]) || 
            !rando_location_is_checked(LOCATION_NOTEBOOK(event))) {
            msgCtx->bombersNotebookEventQueue[msgCtx->bombersNotebookEventQueueCount] = event;
            msgCtx->bombersNotebookEventQueueCount++;
        }
    } else if (event >= BOMBERS_NOTEBOOK_PERSON_MAX) {
        // Non MET events are processed even if the player does not have the notebook yet
        if (!CHECK_WEEKEVENTREG(gBombersNotebookWeekEventFlags[event]) || 
            !rando_location_is_checked(LOCATION_NOTEBOOK(event))) {
            msgCtx->bombersNotebookEventQueue[msgCtx->bombersNotebookEventQueueCount] = event;
            msgCtx->bombersNotebookEventQueueCount++;
        }
    }
}

/**
 * @return s32 true if an entry to be shown was found and triggered
 */
RECOMP_PATCH s32 Message_BombersNotebookProcessEventQueue(PlayState* play) {
    MessageContext* msgCtx = &play->msgCtx;

    Player* player = GET_PLAYER(play);

    while (true) {
        if (msgCtx->bombersNotebookEventQueueCount == 0) {
            return false;
        }
        msgCtx->bombersNotebookEventQueueCount--;

        // add option check here
        if (sBombersNotebookEventMessages[msgCtx->bombersNotebookEventQueue[msgCtx->bombersNotebookEventQueueCount]] != 0) {
            u32 location = LOCATION_NOTEBOOK(msgCtx->bombersNotebookEventQueue[msgCtx->bombersNotebookEventQueueCount]);
            recomp_printf("notebook location: 0x%06X\n", location);
            if (!rando_location_is_checked(location)) {
                rando_send_location(location);
                Message_ContinueTextbox(play, getTextId(rando_get_item_id(location)));
                Audio_PlaySfx(NA_SE_SY_SCHEDULE_WRITE);
                return true;
            } else { // temp
                return true;
            }
        }

        if (!CHECK_WEEKEVENTREG(gBombersNotebookWeekEventFlags
                                    [msgCtx->bombersNotebookEventQueue[msgCtx->bombersNotebookEventQueueCount]])) {
            SET_WEEKEVENTREG(gBombersNotebookWeekEventFlags
                                 [msgCtx->bombersNotebookEventQueue[msgCtx->bombersNotebookEventQueueCount]]);

            if ((sBombersNotebookEventMessages
                     [msgCtx->bombersNotebookEventQueue[msgCtx->bombersNotebookEventQueueCount]] != 0) &&
                CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
                Message_ContinueTextbox(
                    play, sBombersNotebookEventMessages
                              [msgCtx->bombersNotebookEventQueue[msgCtx->bombersNotebookEventQueueCount]]);
                Audio_PlaySfx(NA_SE_SY_SCHEDULE_WRITE);
                return true;
            }
        }
    }
}