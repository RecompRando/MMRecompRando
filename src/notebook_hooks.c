#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "recomputils.h"

#define LOCATION_NOTEBOOK(event) (AP_PREFIX_NOTEBOOK | event)

extern u16 sBombersNotebookEventMessages[BOMBERS_NOTEBOOK_EVENT_MAX];

RECOMP_PATCH void Message_BombersNotebookQueueEvent(PlayState* play, u8 event) {
    MessageContext* msgCtx = &play->msgCtx;

    if (CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
        if (!CHECK_WEEKEVENTREG(gBombersNotebookWeekEventFlags[event]) || 
            (rando_get_slotdata_u32("notebooksanity") && !rando_location_is_checked(LOCATION_NOTEBOOK(event)))) {
            msgCtx->bombersNotebookEventQueue[msgCtx->bombersNotebookEventQueueCount] = event;
            msgCtx->bombersNotebookEventQueueCount++;
        }
    } else if (event >= BOMBERS_NOTEBOOK_PERSON_MAX) {
        // Non MET events are processed even if the player does not have the notebook yet
        if (!CHECK_WEEKEVENTREG(gBombersNotebookWeekEventFlags[event]) || 
            (rando_get_slotdata_u32("notebooksanity") && !rando_location_is_checked(LOCATION_NOTEBOOK(event)))) {
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
    u8 event;
    u32 location;
    u16 messageId;
    
    while (msgCtx->bombersNotebookEventQueueCount > 0) {
        msgCtx->bombersNotebookEventQueueCount--;
        event = msgCtx->bombersNotebookEventQueue[msgCtx->bombersNotebookEventQueueCount];
        
        // Handle notebooksanity
        if (rando_get_slotdata_u32("notebooksanity")) {
            location = LOCATION_NOTEBOOK(event);
            messageId = sBombersNotebookEventMessages[event];
            
            // Always set the in-game event flag
            if (!CHECK_WEEKEVENTREG(gBombersNotebookWeekEventFlags[event])) {
                SET_WEEKEVENTREG(gBombersNotebookWeekEventFlags[event]);
            }
            
            // Check if this location needs to be sent to AP
            if (!rando_location_is_checked(location)) {
                // EVENT checks (>= BOMBERS_NOTEBOOK_PERSON_MAX) can be sent without notebook
                // MET checks (< BOMBERS_NOTEBOOK_PERSON_MAX) require notebook
                if (event >= BOMBERS_NOTEBOOK_PERSON_MAX || CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
                    recomp_printf("Sending notebook location: 0x%06X\n", location);
                    rando_send_location(location);
                    
                    // Show message if there is one AND either:
                    // - It's an EVENT check (doesn't need notebook), OR
                    // - Player has the notebook
                    if (messageId != 0 && (event >= BOMBERS_NOTEBOOK_PERSON_MAX || CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK))) {
                        Message_ContinueTextbox(play, getTextId(rando_get_item_id(location)));
                        Audio_PlaySfx(NA_SE_SY_SCHEDULE_WRITE);
                        return true;
                    }
                }
            } else {
                // Location already checked - show vanilla notebook message to avoid softlock
                if (messageId != 0 && (event >= BOMBERS_NOTEBOOK_PERSON_MAX || CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK))) {
                    Message_ContinueTextbox(play, messageId);
                    Audio_PlaySfx(NA_SE_SY_SCHEDULE_WRITE);
                    return true;
                }
            }
            // Event was processed but no message shown - continue to next
            continue;
        }
        
        // Original vanilla behavior (no notebooksanity)
        if (!CHECK_WEEKEVENTREG(gBombersNotebookWeekEventFlags[event])) {
            SET_WEEKEVENTREG(gBombersNotebookWeekEventFlags[event]);
            
            if ((sBombersNotebookEventMessages[event] != 0) && CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
                Message_ContinueTextbox(play, sBombersNotebookEventMessages[event]);
                Audio_PlaySfx(NA_SE_SY_SCHEDULE_WRITE);
                return true;
            }
        }
    }
    
    return false;
}