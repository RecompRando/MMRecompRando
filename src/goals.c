#include "modding.h"
#include "global.h"
#include "apcommon.h"

void randoTriggerVictory(PlayState* play) {
    gSaveContext.save.entrance = ENTRANCE(TERMINA_FIELD, 0);
    play->nextEntrance = ENTRANCE(TERMINA_FIELD, 0);
    gSaveContext.nextCutsceneIndex = 0xFFF7; // this part actually sets the win con cutscene
    play->transitionTrigger = TRANS_TRIGGER_START;
}

bool rando_met_remains_condition(u32 required_amount) {
    u8 remainsCount = 0;

    for (int i = 0; i < 4; i++) {
        if (rando_has_item(GI_REMAINS_ODOLWA + i)) {
            remainsCount++;
        }
    }
    return remainsCount >= required_amount;
}

bool rando_met_masks_condition(u32 required_amount) {
    u8 maskCount = 0;

    // GI_MASK_DEKU..GI_MASK_KAFEI is every mask
    for (u32 id = GI_MASK_DEKU; id <= GI_MASK_KAFEIS_MASK; id++) {
        if (rando_has_item(id)) {
            maskCount++;
        }
    }
    return maskCount >= required_amount;
}

// moon_star_fox / majora_star_fox are Toggle options so if on all 5 are req.
bool rando_met_star_fox_condition(u32 required) {
    if (!required) {
        return true;
    }

    return rando_has_item(GI_MASK_KEATON) &&
           rando_has_item(GI_MASK_BREMEN) &&
           rando_has_item(GI_MASK_BUNNY) &&
           rando_has_item(GI_MASK_SCENTS) &&
           rando_has_item(GI_MASK_DON_GERO);
}

bool rando_met_scarecrow_condition(u32 required_amount) {
    u8 scarecrowCount = 0;

    // Scarecrows only exist as AP items when scarecrowsanity is on.
    if (!rando_get_slotdata_u32("scarecrowsanity")) {
        return true;
    }

    // Scarecrow IDs
    u32 scarecrowLocations[] = {
        0x302910,  // Astral Observatory Scarecrow
        0x303300,  // Zora Hall Pervert Scarecrow
        0x303400,  // Clock Town Trading Post Scarecrow
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

    // Frogs only exist as AP items when frogsanity is on.
    if (!rando_get_slotdata_u32("frogsanity")) {
        return true;
    }

    // frogs are ids 0xFF0000 - 0xFF0004
    for (int i = 0; i < 5; i++) {
        if (rando_has_item(AP_PREFIX_FROGS | i)) {
            frogCount++;
        }
    }
    
    return frogCount >= required_amount;
}

bool rando_met_owl_condition(u32 required_amount) {
    u8 owlCount = 0;

    // Owl statues only exist as AP items when owlsanity is on.
    if (!rando_get_slotdata_u32("owlsanity")) {
        return true;
    }

    // Counting AP items rather than activations sidesteps hidden owl potentially counting.
    for (int i = 0; i < 10; i++) {
        if (rando_has_item(AP_PREFIX_OWLS | i)) {
            owlCount++;
        }
    }
    return owlCount >= required_amount;
}

bool rando_met_items_condition(u32 required_amount) {
    u8 itemCount = 0;
    
    // Trade Items
    u32 tradeItems[] = {
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
    return rando_met_remains_condition(rando_get_slotdata_u32("moon_remains_required")) &&
           rando_met_masks_condition(rando_get_slotdata_u32("moon_masks_required")) &&
           rando_met_star_fox_condition(rando_get_slotdata_u32("moon_star_fox")) &&
           rando_met_owl_condition(rando_get_slotdata_u32("moon_owls_required")) &&
           rando_met_scarecrow_condition(rando_get_slotdata_u32("moon_scarecrows_required")) &&
           rando_met_frog_condition(rando_get_slotdata_u32("moon_frogs_required")) &&
           rando_met_items_condition(rando_get_slotdata_u32("moon_items_required"));
}

bool rando_met_majora_condition() {
    return rando_met_remains_condition(rando_get_slotdata_u32("majora_remains_required")) &&
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
            rando_met_items_condition(29) &&            // All items
            rando_met_masks_condition(24) &&            // All 24 masks
            rando_met_owl_condition(10) &&              // All 10 owls
            rando_met_scarecrow_condition(17) &&        // All 17 scarecrows
            rando_met_frog_condition(5) &&              // All 5 frogs
            rando_met_star_fox_condition(1) &&          // All 5 Star Fox masks
            // all heart pieces/containers
            gSaveContext.save.saveInfo.playerData.healthCapacity >= 0x140 &&
            // I don't know what the fuck I was on about when I thought allowing 
            // players to choose their own '100%' was a good idea.
            (rando_has_item(AP_ITEM_ID_STRAY_FAIRY_WOODFALL) >= 15) &&
            (rando_has_item(AP_ITEM_ID_STRAY_FAIRY_SNOWHEAD) >= 15) &&
            (rando_has_item(AP_ITEM_ID_STRAY_FAIRY_GREATBAY) >= 15) &&
            (rando_has_item(AP_ITEM_ID_STRAY_FAIRY_STONETOWER) >= 15) &&
            // see above
            ((rando_get_slotdata_u32("skullsanity") != 2)
                ? ((rando_has_item(GI_TRUE_SKULL_TOKEN) >= 30) &&
                   (rando_has_item(GI_OCEAN_SKULL_TOKEN) >= 30))
                : ((Inventory_GetSkullTokenCount(SCENE_KINSTA1) >= 30) &&
                   (Inventory_GetSkullTokenCount(SCENE_KINDAN2) >= 30)));
}