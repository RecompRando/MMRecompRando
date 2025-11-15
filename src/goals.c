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
bool rando_met_scarecrow_condition(u32 required_amount) {
    u8 scarecrowCount = 0;
    
    // All scarecrow IDs from APworld 
    static const u32 scarecrowLocations[] = {
        0x305000,  // Mountain Village Rooftop Scarecrow
        0x305A00,  // Mountain Village Spring Rooftop Scarecrow
        0x305B00,  // Path to Snowhead Scarecrow
        0x305C00,  // Path to Snowhead Spring Scarecrow
        0x305D00,  // Twin Islands Scarecrow
        0x305E00,  // Twin Islands Spring Scarecrow
        0x302140,  // Snowhead Temple Lower Scarecrow
        0x302141,  // Snowhead Temple Hidden Alcove Scarecrow
        0x303700,  // Great Bay Coast Rock Wall Scarecrow
        0x303800,  // Zora Cape Beavers Scarecrow
        0x303801,  // Zora Cape Island Scarecrow
        0x305300,  // Road to Ikana Scarecrow
        0x305800,  // Stone Tower Lower Scarecrow
        0x305801,  // Stone Tower Upper Scarecrow
    };
    
    for (int i = 0; i < ARRAY_COUNT(scarecrowLocations); i++) {
        if (rando_has_item(scarecrowLocations[i])) {
            scarecrowCount++;
        }
    }
    
    return scarecrowCount >= required_amount;
}
bool rando_met_frog_condition(u32 required_amount) {
    u8 frogCount = 0;
    
    // All frog IDs from APworld
    static const u32 frogLocations[] = {
        0,  // Yellow Frog (0xFF0000)
        1,  // Blue Frog (0xFF0001)
        2,  // Pink Frog (0xFF0002)
        3,  // Cyan Frog (0xFF0003)
        4,  // White Frog (0xFF0004)
    };
    
    for (int i = 0; i < ARRAY_COUNT(frogLocations); i++) {
        if (rando_has_item(frogLocations[i])) {
            frogCount++;
        }
    }
    
    return frogCount >= required_amount;
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

bool rando_met_items_condition(u32 required_amount) {
    u8 itemCount = 0;
    
    // Trade Items
    static const u32 tradeItems[] = {
        GI_ROOM_KEY,              // Room Key
        GI_LETTER_TO_MAMA,        // Priority Mail
        GI_LETTER_TO_KAFEI,       // Letter to Kafei
        GI_PENDANT_OF_MEMORIES,   // Pendant of Memories
        GI_MOONS_TEAR,            // Moon's Tear
        GI_DEED_LAND,             // Land Title Deed
        GI_DEED_SWAMP,            // Swamp Title Deed
        GI_DEED_MOUNTAIN,         // Mountain Title Deed
        GI_DEED_OCEAN,            // Ocean Title Deed
    };
    
    // Count regular inventory items, skipping trade item slots
    for (InventorySlot slot = SLOT_OCARINA; slot <= SLOT_BOTTLE_6; slot++) {
        // Skip trade item slots 
        if (slot == SLOT_TRADE_DEED) continue;
        if (slot == SLOT_TRADE_KEY_MAMA) continue;
        if (slot == SLOT_TRADE_COUPLE) continue;
        
        if (gSaveContext.save.saveInfo.inventory.items[slot] != ITEM_NONE) {
            itemCount++;
        }
    }
    
    // Count trade items
    for (int i = 0; i < ARRAY_COUNT(tradeItems); i++) {
        if (rando_has_item(tradeItems[i])) {
            itemCount++;
        }
    }
    
    return itemCount >= required_amount;
}

bool rando_met_moon_condition() {
    return rando_met_remains_condition(rando_get_moon_remains_required()) &&
           rando_met_masks_condition(rando_get_slotdata_u32("moon_masks_required")) &&
           rando_met_star_fox_condition(rando_get_slotdata_u32("moon_star_fox")) &&
           rando_met_owl_condition(rando_get_slotdata_u32("moon_owls_required")) &&
           rando_met_scarecrow_condition(rando_get_slotdata_u32("moon_scarecrows_required")) &&
           rando_met_frog_condition(rando_get_slotdata_u32("moon_frogs_required")) &&
           rando_met_items_condition(rando_get_slotdata_u32("moon_items_required"));
}

bool rando_met_majora_condition() {
    return rando_met_remains_condition(rando_get_majora_remains_required()) &&
           rando_met_masks_condition(rando_get_slotdata_u32("majora_masks_required")) &&
           rando_met_star_fox_condition(rando_get_slotdata_u32("majora_star_fox")) &&
           rando_met_owl_condition(rando_get_slotdata_u32("majora_owls_required")) &&
           rando_met_scarecrow_condition(rando_get_slotdata_u32("majora_scarecrows_required")) &&
           rando_met_frog_condition(rando_get_slotdata_u32("majora_frogs_required")) &&
           rando_met_items_condition(rando_get_slotdata_u32("majora_items_required"));
}


bool rando_met_all_goal() {
    return  (rando_get_slotdata_u32("completion_goal")) &&
            rando_met_remains_condition(4) &&           // All 4 remains
            rando_met_masks_condition(24) &&            // All 24 masks
            rando_met_owl_condition(10) &&              // All 10 owls
            rando_met_scarecrow_condition(14) &&        // All 14 scarecrows
            rando_met_frog_condition(5) &&              // All 5 frogs
            rando_met_items_condition(31) &&            // All items (adjust number as needed)
            // all heart pieces/containers
            gSaveContext.save.saveInfo.playerData.healthCapacity >= 0x140 &&
            // stray fairies
            gSaveContext.save.saveInfo.inventory.strayFairies[0] == 15 &&
            gSaveContext.save.saveInfo.inventory.strayFairies[1] == 15 &&
            gSaveContext.save.saveInfo.inventory.strayFairies[2] == 15 &&
            gSaveContext.save.saveInfo.inventory.strayFairies[3] == 15 &&
            // gSaveContext.save.saveInfo.inventory.strayFairies[4] == 1 &&
            // skulltulas
            Inventory_GetSkullTokenCount(SCENE_KINSTA1) == 30 &&
            Inventory_GetSkullTokenCount(SCENE_KINDAN2) == 30;
}
