#include "modding.h"
#include "global.h"

#include "apcommon.h"


bool rando_met_remains_condition(u32 required_amount) {
    return ((CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA) > 0) +
            (CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT) > 0) +
            (CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG) > 0) +
            (CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD) > 0)) >= required_amount;
}

bool rando_met_masks_condition(u32 required_amount) {
    u8 maskCount = 0;
    
    for (InventorySlot slot = SLOT_MASK_POSTMAN; slot <= SLOT_MASK_FIERCE_DEITY; slot++) {
        if (gSaveContext.save.saveInfo.inventory.items[slot] != ITEM_NONE) {
            maskCount++;
        }
    }
    
    return maskCount >= required_amount;
}

bool rando_met_moon_condition() {
    return rando_met_remains_condition(rando_get_moon_remains_required()) &&
           rando_met_masks_condition(rando_get_slotdata_u32("moon_masks_required"));
}

bool rando_met_majora_condition() {
    return rando_met_remains_condition(rando_get_majora_remains_required()) &&
           rando_met_masks_condition(rando_get_slotdata_u32("majora_masks_required"));
}