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

bool rando_met_star_fox_condition(u32 required_amount) {
    u8 starfoxMaskCount = 0;
    
    // Check the 5 Star Fox masks
    if (gSaveContext.save.saveInfo.inventory.items[SLOT_MASK_KEATON] != ITEM_NONE) {
        starfoxMaskCount++;
    }
    if (gSaveContext.save.saveInfo.inventory.items[SLOT_MASK_BREMEN] != ITEM_NONE) {
        starfoxMaskCount++;
    }
    if (gSaveContext.save.saveInfo.inventory.items[SLOT_MASK_BUNNY] != ITEM_NONE) {
        starfoxMaskCount++;
    }
    if (gSaveContext.save.saveInfo.inventory.items[SLOT_MASK_SCENTS] != ITEM_NONE) {
        starfoxMaskCount++;
    }
    if (gSaveContext.save.saveInfo.inventory.items[SLOT_MASK_DON_GERO] != ITEM_NONE) {
        starfoxMaskCount++;
    }
    
    return starfoxMaskCount >= required_amount;
}

bool rando_met_owl_condition(u32 required_amount) {
    u8 owlCount = 0;
    
    // Count all activated owl statues
    for (int i = 0; i < 16; i++) {
        if ((gSaveContext.save.saveInfo.playerData.owlActivationFlags >> i) & 1) {
            owlCount++;
        }
    }
    
    return owlCount >= required_amount;
}

bool rando_met_moon_condition() {
    return rando_met_remains_condition(rando_get_moon_remains_required()) &&
           rando_met_masks_condition(rando_get_slotdata_u32("moon_masks_required")) &&
           rando_met_star_fox_condition(rando_get_slotdata_u32("moon_star_fox"));
}

bool rando_met_majora_condition() {
    return rando_met_remains_condition(rando_get_majora_remains_required()) &&
           rando_met_masks_condition(rando_get_slotdata_u32("majora_masks_required")) &&
           rando_met_star_fox_condition(rando_get_slotdata_u32("majora_star_fox"));
}

bool rando_met_all_goal() {
    return rando_met_remains_condition(4) &&
            rando_met_masks_condition(24) &&
            // owl statues
            rando_met_owl_condition(10) &&
            // all heart pieces/containers
            gSaveContext.save.saveInfo.playerData.healthCapacity >= 0x140 && // make sure this is right
            // stray fairies
            gSaveContext.save.saveInfo.inventory.strayFairies[0] == 15 &&
            gSaveContext.save.saveInfo.inventory.strayFairies[1] == 15 &&
            gSaveContext.save.saveInfo.inventory.strayFairies[2] == 15 &&
            gSaveContext.save.saveInfo.inventory.strayFairies[3] == 15 &&
            gSaveContext.save.saveInfo.inventory.strayFairies[3] == 15 &&

            // scarecrows
            // skulltulas
            Inventory_GetSkullTokenCount(SCENE_KINSTA1) == 30 &&
            Inventory_GetSkullTokenCount(SCENE_KINDAN2) == 30;
}
