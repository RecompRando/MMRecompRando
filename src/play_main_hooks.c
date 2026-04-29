#include "modding.h"
#include "global.h"
#include "recompconfig.h"
#include "recomputils.h"

#include "apcommon.h"
#include "yaml_generation.h"

#include "../mm-decomp/src/overlays/actors/ovl_En_GirlA/z_en_girla.h"

#define LOCATION_INVENTORY_SWORD GI_SWORD_KOKIRI
#define LOCATION_INVENTORY_SHIELD GI_SHIELD_HERO

#define C_TO_PARAMS(c) (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF

RECOMP_IMPORT("*", int recomp_set_moon_crash_resets_save(bool new_val));
RECOMP_IMPORT("*", int recomp_set_fd_anywhere(bool new_val));
RECOMP_IMPORT("*", int recomp_set_no_bow_epona_fix(bool new_val));
RECOMP_IMPORT("*", int recomp_set_allow_no_ocarina_tf(bool new_val));
RECOMP_IMPORT("*", int recomp_set_h_and_d_no_sword_fix(bool new_val));

// temp
bool rando_get_game_is_oot(u32 player_id) {return false;}
bool rando_get_game_is_ww(u32 player_id) {return false;}

RECOMP_IMPORT("mm_recomp_better_double_sot", void dsot_set_skip_dsot_cutscene(bool new_val));
RECOMP_IMPORT("mm_recomp_better_double_sot", void dsot_set_time(PlayState* play, s32 day, u16 time));

RECOMP_IMPORT("mm_recomp_colors", void colors_set_human_tunic(u8 r, u8 g, u8 b));

void registerActorExtensions();

PlayState* gPlay;

RECOMP_CALLBACK("*", recomp_on_init)
void init_rando()
{
    // Set a dummy save file on init just to make recomp change the save folder. The real save
    // file will be determined when starting solo or multiworld.
    recomp_change_save_file("save.bin");
    recomp_set_moon_crash_resets_save(false);
    recomp_set_fd_anywhere(true);
    recomp_set_no_bow_epona_fix(true);
    recomp_set_allow_no_ocarina_tf(true);
    recomp_set_h_and_d_no_sword_fix(true);

    dsot_set_skip_dsot_cutscene(true);
    registerActorExtensions();

    randoCreateNotificationContainer();
}

void removeAllScoutsWithPrefix(u32 prefix) {
    for (int i = 0x00; i <= 0xFFFF; ++i)
    {
        u32 location_id = prefix | i;
        rando_remove_queued_scout(location_id);
    }
}

void randoScout() {
    rando_queue_scouts_all();

    // if (rando_get_slotdata_u32("skullsanity") == 2)
    // {
    //     for (int i = 0x00; i <= 0x1E; ++i)
    //     {
    //         if (i == 0x03)
    //         {
    //             continue;
    //         }
            
    //         u32 location_id = 0x062700 | i;
    //         rando_remove_queued_scout(location_id);
    //     }
    //     for (int i = 0x01; i <= 0x1E; ++i)
    //     {
    //         u32 location_id = 0x062800 | i;
    //         rando_remove_queued_scout(location_id);
    //     }
    // }
    
    // for (u32 i = rando_get_slotdata_u32("starting_heart_locations"); i < 8; ++i)
    // {
    //     u32 location_id = 0x0D0000 | i;
    //     rando_remove_queued_scout(location_id);
    // }

    // if (rando_get_slotdata_u32("cowsanity") == 0)
    // {
    //     for (int i = 0x10; i <= 0x17; ++i)
    //     {
    //         u32 location_id = 0xBEEF00 | i;
    //         rando_remove_queued_scout(location_id);
    //     }
    // }
    
    // if (rando_get_slotdata_u32("scrubsanity") == 0)
    // {
    //     rando_remove_queued_scout(0x090100 | GI_MAGIC_BEANS);
    //     rando_remove_queued_scout(0x090100 | GI_BOMB_BAG_40);
    //     rando_remove_queued_scout(0x090100 | GI_POTION_GREEN);
    //     rando_remove_queued_scout(0x090100 | GI_POTION_BLUE);
    // }
    
    // if (rando_get_slotdata_u32("shopsanity") != 2)
    // {
    //     rando_remove_queued_scout(0x05481E);
    //     rando_remove_queued_scout(0x024234);
        
    //     if (rando_get_slotdata_u32("shopsanity") == 1)
    //     {
    //         for (int i = SI_FAIRY_2; i <= SI_POTION_RED_3; ++i)
    //         {
    //             u32 location_id = 0x090000 | i;
    //             rando_remove_queued_scout(location_id);
    //         }
            
    //         rando_remove_queued_scout(0x090000 | SI_BOMB_3);
    //         rando_remove_queued_scout(0x090000 | SI_ARROWS_SMALL_3);
    //         rando_remove_queued_scout(0x090000 | SI_POTION_RED_6);
    //     }
    //     else
    //     {
    //         for (int i = SI_POTION_RED_1; i <= SI_POTION_RED_6; ++i)
    //         {
    //             if (i == SI_BOMB_BAG_20_1 || i == SI_BOMB_BAG_40)
    //             {
    //                 continue;
    //             }
                
    //             u32 location_id = 0x090000 | i;
    //             rando_remove_queued_scout(location_id);
    //         }
            
    //         rando_remove_queued_scout(0x090013);
    //         rando_remove_queued_scout(0x090015);
            
    //         rando_remove_queued_scout(0x026392);
    //         rando_remove_queued_scout(0x090000 | GI_CHATEAU);
    //         rando_remove_queued_scout(0x006792);
    //         rando_remove_queued_scout(0x000091);
    //     }
    // }
    
    // if (rando_get_slotdata_u32("curiostity_shop_trades") == 0)
    // {
    //     rando_remove_queued_scout(0x07C402);
    //     rando_remove_queued_scout(0x07C404);
    //     rando_remove_queued_scout(0x07C405);
    //     rando_remove_queued_scout(0x07C407);
    // }
    
    // if (rando_get_slotdata_u32("intro_checks") == 0)
    // {
    //     rando_remove_queued_scout(0x061A00);
    //     // ignore intro grass
    //     for (int j = 0; j <= 0xFF; j++) {
    //         rando_remove_queued_scout(AP_PREFIX_GRASS_KEATON | (SCENE_OPENINGDAN << 8) | j);
    //         rando_remove_queued_scout(AP_PREFIX_GRASS_KEATON | (SCENE_LOST_WOODS << 8) | j);
    //         rando_remove_queued_scout(AP_PREFIX_MURE_GRASS | (SCENE_LOST_WOODS << 8) | j);
    //     }
    // }
    
    // if (rando_get_slotdata_u32("grasssanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_GRASS_GROUP);
    //     removeAllScoutsWithPrefix(AP_PREFIX_GRASS_GROUP2);
    //     removeAllScoutsWithPrefix(AP_PREFIX_GRASS_KUSA);
    //     removeAllScoutsWithPrefix(AP_PREFIX_GRASS_KEATON);
    //     removeAllScoutsWithPrefix(AP_PREFIX_MURE_GRASS);
    // }
    
    // if (rando_get_slotdata_u32("potsanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_POTS);
    // }
    
    // if (rando_get_slotdata_u32("hitsanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_HITSPOT);
    // }
    
    // if (rando_get_slotdata_u32("rocksanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_ROCKS);
    //     removeAllScoutsWithPrefix(AP_PREFIX_BOULDERS);
    //     removeAllScoutsWithPrefix(AP_PREFIX_MURE_ROCKS);
    // }
    
    // if (rando_get_slotdata_u32("soilsanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_SOIL);
    // }
    
    // if (rando_get_slotdata_u32("rupeesanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_FREESTANDING);
    //     removeAllScoutsWithPrefix(AP_PREFIX_SONGWALL);
    // }
    
    // if (rando_get_slotdata_u32("invisisanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_INVISRUPEES);
    // }
    
    // if (rando_get_slotdata_u32("snowsanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_SNOWBALLS);
    // }
    
    // if (rando_get_slotdata_u32("woodsanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_SMALL_CRATES);
    //     removeAllScoutsWithPrefix(AP_PREFIX_BIG_CRATES);
    //     removeAllScoutsWithPrefix(AP_PREFIX_BARRELS);
    // }
    
    // if (rando_get_slotdata_u32("realfairysanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_FAIRIES);
    //     removeAllScoutsWithPrefix(AP_PREFIX_GOSSIP);
    //     removeAllScoutsWithPrefix(AP_PREFIX_BUTTERFLIES);
    // }
    
    // if (rando_get_slotdata_u32("iciclesanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_ICICLES);
    // }
    
    // if (rando_get_slotdata_u32("scarecrowsanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_SCARECROW);
    // }
    
    // if (rando_get_slotdata_u32("hivesanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_BEEHIVES);
    // }
    
    // if (rando_get_slotdata_u32("notebooksanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_NOTEBOOK);
    // }
    
    // if (rando_get_slotdata_u32("owlsanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_OWLS);
    // }
    
    // if (rando_get_slotdata_u32("frogsanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_FROGS);
    // }
    
    // if (rando_get_slotdata_u32("treesanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_PALM_TREES);
    //     removeAllScoutsWithPrefix(AP_PREFIX_SNOW_TREES);
    //     removeAllScoutsWithPrefix(AP_PREFIX_WOOD_TREES);
    //     removeAllScoutsWithPrefix(AP_PREFIX_TREE_TREES);
    // }
    
    // if (rando_get_slotdata_u32("websanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_WEBS);
    // }
    
    // if (rando_get_slotdata_u32("flowersanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_FLOWERS);
    // }
    
    // if (rando_get_slotdata_u32("signsanity") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_SIGNS);
    // }

    // if (rando_get_slotdata_u32("oneoffs") == 0)
    // {
    //     removeAllScoutsWithPrefix(AP_PREFIX_ONEOFFS);
    //     removeAllScoutsWithPrefix(0x063F00); // goron trial chest
    // }
    
    // if (rando_get_slotdata_u32("completion_goal") == 0)
    // {
    //     rando_remove_queued_scout(0x0B012F);
    // }

    rando_send_queued_scouts(0);
}

s8 giToItemId[GI_MAX] = {
    0x00,
    0x84,
    0x85,
    0x86,
    0x87,
    0x88,
    0x89,
    0x8A,
    0x5A,
    0x5B,
    0x83,
    0x83,
    0x7B,
    0x6F,
    0x79,
    0x7A,
    0x83,
    0xFF,
    0x83,
    0x83,
    0x06,
    0x8F,
    0x90,
    0x91,
    0x92,
    0x08,
    0x98,
    0x56,
    0x57,
    0x58,
    0x93,
    0x94,
    0x95,
    0x95,
    0x01,
    0x54,
    0x55,
    0x02,
    0x03,
    0x04,
    0x09,
    0x8D,
    0x8E,
    0x9D,
    0x9D,
    0x9E,
    0x97,
    0x9B,
    0x9B,
    0x9C,
    0x51,
    0x52,
    0x0C,
    0x0A,
    0x99,
    0x4D,
    0x4E,
    0x4F,
    0x9A,
    0x10,
    0x78,
    0x74,
    0x76,
    0x75,
    0xFF,
    0x0F,
    0x0E,
    0x0D,
    0x0D,
    0x83,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0x6D,
    0xFF,
    0x6E,
    0xFF,
    0xFF,
    0x5D,
    0x5E,
    0x5F,
    0x60,
    0x11,
    0x12,
    0x13,
    0x14,
    0x15,
    0x16,
    0x16,
    0x18,
    0x19,
    0x1A,
    0x1B,
    0x1C,
    0x12,
    0x1E,
    0x1F,
    0x20,
    0x21,
    0x22,
    0x23,
    0xFF,
    0x12,
    0x24,
    0x25,
    0x12,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x32,
    0x33,
    0x34,
    0x35,
    0x44,
    0x49,
    0x38,
    0x39,
    0x3A,
    0x3B,
    0x3C,
    0x3D,
    0x3E,
    0x3F,
    0x40,
    0x41,
    0x42,
    0x43,
    0x36,
    0x45,
    0x46,
    0x47,
    0x48,
    0x37,
    0xFF,
    0x9F,
    0xA0,
    0xA1,
    0xA2,
    0xA3,
    0x28,
    0x29,
    0x2A,
    0x2B,
    0x2C,
    0x10,
    0x4D,
    0x4E,
    0x4F,
    0x51,
    0x2D,
    0x2E,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x12,
    0x2F,
    0x30,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x31,
    0x31,
    0x31,
    0x31,
    0x31,
    0x31
};

// @rando don't make time go faster when you don't have ocarina
// I couldn't think of a better place to put this lol
u8 oldOcarinaSlotItem;

RECOMP_HOOK("Scene_CommandTimeSettings")
void FixOcarinalessTimeSpeed(PlayState* play, SceneCmd* cmd) {
    oldOcarinaSlotItem = gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA];
    gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] = ITEM_OCARINA_OF_TIME;
}

RECOMP_HOOK_RETURN("Scene_CommandTimeSettings")
void ResetOcarinalessTimeSpeed() {
    gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] = oldOcarinaSlotItem;
}

void Play_KillPlayer() {
    gSaveContext.save.saveInfo.playerData.health = 0;
}

bool initItems = false;
u32 old_items_size;
bool waiting_death_link = false;
bool sending_death_link = false;

s16 rando_damage_multiplier() {
    u32 multiplier_option = rando_get_slotdata_u32("damage_multiplier");
    if (multiplier_option == 4) {
        return 0xF;
    }
    return multiplier_option;
}

/**
 * @return false if player is out of health
 */
RECOMP_PATCH s32 Health_ChangeBy(PlayState* play, s16 healthChange) {
    if (healthChange > 0) {
        Audio_PlaySfx(NA_SE_SY_HP_RECOVER);
    } else if (gSaveContext.save.saveInfo.playerData.doubleDefense && (healthChange < 0)) {
        healthChange >>= 1;
    }

    if (healthChange < 0) {
        s16 damageMult = rando_damage_multiplier();
        if (damageMult == 0xF) {
            gSaveContext.save.saveInfo.playerData.health = 0;
        }

        if (damageMult == 0) {
            healthChange *= 0.5;
        } else {
            healthChange *= damageMult;
        }
    }

    gSaveContext.save.saveInfo.playerData.health += healthChange;

    if (((void)0, gSaveContext.save.saveInfo.playerData.health) >
        ((void)0, gSaveContext.save.saveInfo.playerData.healthCapacity)) {
        gSaveContext.save.saveInfo.playerData.health = gSaveContext.save.saveInfo.playerData.healthCapacity;
    }

    if (gSaveContext.save.saveInfo.playerData.health <= 0) {
        gSaveContext.save.saveInfo.playerData.health = 0;
        if (rando_get_death_link_enabled()) {
            rando_send_death_link();
        }

        if (rando_get_slotdata_u32("death_behavior") == 3) {
            Interface_StartMoonCrash(play);
        }

        return false;
    } else {
        return true;
    }
}

s32 Health_ChangeBy_NoSound(PlayState* play, s16 healthChange) {
    if (gSaveContext.save.saveInfo.playerData.doubleDefense && (healthChange < 0)) {
        healthChange >>= 1;
    }

    if (healthChange < 0) {
        s16 damageMult = rando_damage_multiplier();
        if (damageMult == 0xF) {
            gSaveContext.save.saveInfo.playerData.health = 0;
        }

        if (damageMult == 0) {
            healthChange *= 0.5;
        } else {
            healthChange *= damageMult;
        }
    }

    gSaveContext.save.saveInfo.playerData.health += healthChange;

    if (((void)0, gSaveContext.save.saveInfo.playerData.health) >
        ((void)0, gSaveContext.save.saveInfo.playerData.healthCapacity)) {
        gSaveContext.save.saveInfo.playerData.health = gSaveContext.save.saveInfo.playerData.healthCapacity;
    }

    if (gSaveContext.save.saveInfo.playerData.health <= 0) {
        gSaveContext.save.saveInfo.playerData.health = 0;
        if (rando_get_death_link_enabled()) {
            rando_send_death_link();
        }

        if (rando_get_slotdata_u32("death_behavior") == 3) {
            Interface_StartMoonCrash(play);
        }

        return false;
    } else {
        return true;
    }
}

bool rando_get_camc_enabled() {
    return recomp_get_config_u32("camc_enabled");
}

// TODO: properly account for all different types of saves
RECOMP_HOOK("Sram_StartWriteToFlashDefault")
void rando_save_state_normally() {
    recomp_printf("saving rando state from normal saves\n");
    rando_save_current_state();
}

RECOMP_HOOK("Sram_StartWriteToFlashOwlSave")
void rando_save_state_from_owl() {
    recomp_printf("saving rando state from owls\n");
    rando_save_current_state();
}

RECOMP_CALLBACK("*", recomp_on_autosave)
void rando_handle_autosaves(PlayState* play) {
    recomp_printf("saving rando state from autosave\n");
    rando_save_current_state();
}

ItemId randoConvertItemId(u32 ap_item_id) {
    ap_item_id &= 0xFFFFFF;

    switch (ap_item_id & 0xFF0000) {
        case 0x010000:
            return ITEM_FAIRY;
        case 0x020000:
            switch (ap_item_id & 0xFF) {
                case 0x00:
                    if (!gSaveContext.save.saveInfo.playerData.isMagicAcquired) {
                        return ITEM_MAGIC_JAR_SMALL;
                    }
                    return ITEM_MAGIC_JAR_BIG;
                case 0x01:
                    return ITEM_SWORD_KOKIRI;
                case 0x03:
                default:
                    return ITEM_HEART_CONTAINER;
            }
        case 0x040000:
            return ITEM_OCARINA_FAIRY; // could use music notes instead, but this is funnier
        case 0x090000:
            return ap_item_id & 0xFF;
        case 0x0A0000:
        case 0x0B0000:
        case 0x0C0000:
        case 0x0D0000:
        case 0x0E0000:
        case 0x0F0000:
            return ITEM_BLUE_FIRE; // temp
        case 0xFF0000:
            return ITEM_MASK_DON_GERO; // temp
        case 0x000000:
            switch (ap_item_id & 0xFF) {
                case GI_TRUE_SKULL_TOKEN:
                case GI_OCEAN_SKULL_TOKEN:
                    return ITEM_SKULL_TOKEN;
                case GI_BOMBCHUS_1:
                case GI_BOMBCHUS_5:
                case GI_BOMBCHUS_10:
                case GI_BOMBCHUS_20: // tbh this feels like bait without chu bag
                case GI_BAG_BOMBCHU:
                    return ITEM_BOMBCHU;
                case GI_POTION_RED_BOTTLE:
                    return ITEM_POTION_RED;
                case GI_HEART_PIECE:
                    return ITEM_HEART_PIECE;
                case GI_SWORD_KOKIRI:
                    return MIN(ITEM_SWORD_KOKIRI + rando_has_item(GI_SWORD_KOKIRI) - 1, ITEM_SWORD_GILDED);
                case GI_SHIELD_HERO:
                    return MIN(ITEM_SHIELD_HERO + rando_has_item(GI_SHIELD_HERO) - 1, ITEM_SHIELD_MIRROR);
                case GI_BOMB_BAG_20:
                    return MIN(ITEM_BOMB_BAG_20 + rando_has_item(GI_BOMB_BAG_20) - 1, ITEM_BOMB_BAG_40);
                case GI_QUIVER_30:
                    if (rando_has_item(GI_QUIVER_30) == 1) {
                        return ITEM_BOW;
                    }
                    return MIN(ITEM_QUIVER_30 + rando_has_item(GI_QUIVER_30) - 1, ITEM_QUIVER_50);
                case GI_ARROWS_30:
                    return ITEM_ARROWS_30;
            }
        default:
            return (ItemId) giToItemId[ap_item_id & 0xFF];
    }
}

RECOMP_EXPORT u32 rando_get_item_id(u32 location)
{
    if (location == 0) return GI_NONE;
    
    if (rando_get_location_has_local_item(location))
    {
        u32 item = rando_get_item_at_location(location);
        
        if ((item & 0xFF0000) == 0x000000) {
            u8 gi = item & 0xFF;
            
            if (gi == GI_SWORD_KOKIRI)
            {
                return MIN(GI_SWORD_KOKIRI + rando_has_item(GI_SWORD_KOKIRI), GI_SWORD_GILDED);
            }
            
            else if (gi == GI_QUIVER_30)
            {
                return MIN(GI_QUIVER_30 + rando_has_item(GI_QUIVER_30), GI_QUIVER_50);
            }
            
            else if (gi == GI_BOMB_BAG_20)
            {
                return MIN(GI_BOMB_BAG_20 + rando_has_item(GI_BOMB_BAG_20), GI_BOMB_BAG_40);
            }
            
            else if (gi == GI_WALLET_ADULT)
            {
                return MIN(GI_WALLET_ADULT + rando_has_item(GI_WALLET_ADULT), GI_WALLET_GIANT);
            }
            
            else if (gi == GI_RECOVERY_HEART)
            {
                return GI_RECOVERY_HEART_RANDO;
            }
            
            return gi;
        }
        switch (item & 0xFF0000)
        {
            case 0x010000:
                switch (item & 0xFF)
                {
                    case 0x7F:
                        return GI_B2;
                    case 0x00:
                        return GI_46;
                    case 0x01:
                        return GI_47;
                    case 0x02:
                        return GI_48;
                    case 0x03:
                        return GI_49;
                }
                return GI_NONE;
            case 0x020000:
                switch (item & 0xFF)
                {
                    case 0x00:
                        return GI_MAGIC_UPGRADE;
                    case 0x01:
                        return GI_SPIN_ATTACK;
                    case 0x03:
                        return GI_DEFENSE_DOUBLE;
                }
                return GI_NONE;
            case 0x040000:
                switch (item & 0xFF)
                {
                    case ITEM_SONG_TIME:
                        return GI_A6;
                    case ITEM_SONG_HEALING:
                        return GI_AF;
                    case ITEM_SONG_EPONA:
                        return GI_A5;
                    case ITEM_SONG_SOARING:
                        return GI_A3;
                    case ITEM_SONG_STORMS:
                        return GI_A2;
                    case ITEM_SONG_SONATA:
                        return GI_AE;
                    case ITEM_SONG_LULLABY:
                        return GI_AD;
                    case ITEM_SONG_NOVA:
                        return GI_AC;
                    case ITEM_SONG_ELEGY:
                        return GI_A8;
                    case ITEM_SONG_OATH:
                        return GI_A7;
                }
                return GI_NONE;
            case 0x090000:
                switch (item & 0xFF)
                {
                    case ITEM_KEY_BOSS:
                        return (GI_KEY_BOSS_WOODFALL + (((item >> 8) & 0xF) * 4));
                    case ITEM_KEY_SMALL:
                        return (GI_KEY_SMALL_WOODFALL + (((item >> 8) & 0xF) * 4));
                    case ITEM_DUNGEON_MAP:
                        return (GI_MAP_WOODFALL + (((item >> 8) & 0xF) * 4));
                    case ITEM_COMPASS:
                        return (GI_COMPASS_WOODFALL + (((item >> 8) & 0xF) * 4));
                }
                return GI_NONE;
            case 0xFF0000:
                switch (item & 0x00FF00)
                {
                    case (0x001500): // 0xFF1500 for owls
                        switch (item & 0xF)
                        {
                            case 0xF:
                                return GI_OWL_HIDDEN;
                            default:
                                return GI_OWL_GREAT_BAY_COAST + (item & 0xF);
                        }
                    case (0x000000): // 0xFF0000 for frogs
                        return GI_FROG_YELLOW + (item & 0xF);
                }
                return GI_NONE;
            case AP_PREFIX_SCARECROW:
                return GI_SCARECROW;
            case AP_ITEM_PREFIX_SOUL_MISC:
                switch (item)
                {
                    case (AP_ITEM_ID_SOUL_MISC_COW):
                        return GI_MISC_SOUL_COW;
                    case (AP_ITEM_ID_SOUL_MISC_KEATON):
                        return GI_MISC_SOUL_KEATON;
                    case (AP_ITEM_ID_SOUL_MISC_GOLD_SKULLTULA):
                        return GI_MISC_SOUL_GOLD_SKULLTULAS;
                    case (AP_ITEM_ID_SOUL_MISC_BUTTERFLY):
                        return GI_MISC_SOUL_BUTTERFLY;
                }
                return GI_NONE;
            case AP_ITEM_PREFIX_SOUL_BOSS:
                switch (item)
                {
                    case (AP_ITEM_ID_SOUL_BOSS_ODOLWA):
                        return GI_BOSS_SOUL_ODOLWA;
                    case (AP_ITEM_ID_SOUL_BOSS_GOHT):
                        return GI_BOSS_SOUL_GOHT;
                    case (AP_ITEM_ID_SOUL_BOSS_GYORG):
                        return GI_BOSS_SOUL_GYORG;
                    case (AP_ITEM_ID_SOUL_BOSS_TWINMOLD):
                        return GI_BOSS_SOUL_TWINMOLD;
                    case (AP_ITEM_ID_SOUL_BOSS_MAJORA):
                        return GI_BOSS_SOUL_MAJORA;
                }
                return GI_NONE;
            case AP_ITEM_PREFIX_SOUL_NPC:
                return GI_NPC_GENERIC;
            case AP_ITEM_PREFIX_SOUL_UTILITY:
                return GI_UTILITY_GENERIC;
            case AP_ITEM_PREFIX_SOUL_ENEMY:
                return GI_ENEMY_GENERIC;
            case AP_ITEM_PREFIX_SOUL_ABSURD:
                return GI_ABSURD_GENERIC;
        }
    }
    
    // // TODO: make this not work
    // u32 receiving_player = rando_get_location_item_player_id(location);

    // if (rando_get_game_is_oot(receiving_player)) {
    //     switch (rando_get_location_type(location))
    //     {
    //         case RANDO_ITEM_CLASS_PROGRESSION:
    //         case RANDO_ITEM_CLASS_TRAP:
    //             return GI_OOT_ITEM_PROG;
    //         case RANDO_ITEM_CLASS_USEFUL:
    //             return GI_OOT_ITEM_USEFUL;
    //         case RANDO_ITEM_CLASS_JUNK:
    //         default:
    //             return GI_OOT_ITEM_FILLER;
    //     }
    // }
    
    // if (rando_get_game_is_ww(receiving_player)) {
    //     switch (rando_get_location_type(location))
    //     {
    //         case RANDO_ITEM_CLASS_PROGRESSION:
    //         case RANDO_ITEM_CLASS_TRAP:
    //             return GI_WW_ITEM_PROG;
    //         case RANDO_ITEM_CLASS_USEFUL:
    //             return GI_WW_ITEM_USEFUL;
    //         case RANDO_ITEM_CLASS_JUNK:
    //         default:
    //             return GI_WW_ITEM_FILLER;
    //     }
    // }

    u32 item_type = rando_get_location_type(location);
    if (item_type & RANDO_ITEM_CLASS_PROGRESSION ||
            item_type & RANDO_ITEM_CLASS_TRAP) {
        return GI_AP_PROG;
    } else if (item_type & RANDO_ITEM_CLASS_USEFUL) {
        return GI_AP_USEFUL;
    } else { // RANDO_ITEM_CLASS_JUNK
        return GI_AP_FILLER;
    }
}

RECOMP_DECLARE_EVENT(rando_on_start());

bool rando_met_all_goal();

bool last_deathlink_status;

RECOMP_CALLBACK("*", recomp_on_play_main)
void update_rando(PlayState* play) {
    u32 new_items_size;
    u32 i;
    u8* save_ptr;

    gPlay = play;

    // @glue push the interpreter on the stack to reduce lag from deactivations
    REPY_PushInterpreter(rando_interp);

    REPY_FN_SETUP_RANDO;

    notificationUpdateCycle();
    rando_populate_locations(); // in an ideal world this only runs after LocationInfo is recieved
    rando_update_cache();

    if (saveOpened) {
        if (!initItems) {
            // TODO: account for each different type of save
            REPY_FN_EXEC_CACHE(
                py_rando_load_saved_state,
                "RecompClient.run_async_task_and_wait_once(RecompClient.load_saved_state())\n" // async due to sending offline locations
            );

            // below is left over from our old system, keeping this here for safety

            if (gSaveContext.save.playerForm == PLAYER_FORM_FIERCE_DEITY) {
                CUR_FORM_EQUIP(EQUIP_SLOT_B) = ITEM_SWORD_DEITY;
                Interface_LoadItemIconImpl(play, EQUIP_SLOT_B);
            }

            rando_send_location(LOCATION_INVENTORY_SWORD);
            rando_send_location(LOCATION_INVENTORY_SHIELD);
            rando_send_location(0x0D0000 | GI_OCARINA_OF_TIME);
            rando_send_location(0x0D0067);

            for (int i = 0; i < rando_get_slotdata_u32("starting_heart_locations"); ++i)
            {
                rando_send_location(0x0D0000 | i);
            }

            initItems = true;
            rando_on_start();
        }

        REPY_FN_EXEC_CACHE(
            py_rando_get_new_items,
            "from collections import Counter\n"
            "ctx = recomp_data.ctx\n"
            "new_items = list((Counter(ctx.items_received) - Counter(ctx.local_received)).elements())\n" // sure man
            // "if new_items:\n"
            // "    print(new_items)\n"
        );

        REPY_FN_FOREACH_CACHE(py_rando_handle_new_items, "network_item", "new_items") {
            REPY_FN_EXEC_CACHE(
                py_rando_give_new_item,
                "item_id = network_item.item\n"
                "location = network_item.player\n"
                "player = network_item.player\n"
                "item_type = network_item.flags\n"
                "ctx.local_received.append(network_item)\n"
            );

            u32 item_id = REPY_FN_GET_U32("item_id");
            u32 location = REPY_FN_GET_U32("location");
            u32 player = REPY_FN_GET_U32("player");
            u32 item_type = REPY_FN_GET_U32("item_type");

            // note: this could probably be done differently, but partially uses old systems for now
            if (player != rando_get_own_slot_id() || recomp_get_config_u32("local_notifications")) {
                char* item_name;
                char* player_name;
                rando_get_item_name_from_id(item_id, &item_name);
                rando_get_sending_player_name(location, &player_name);
                randoEmitRecieveNotification(item_name, player_name, randoConvertItemId(item_id), item_type);
                recomp_free(item_name);
                recomp_free(player_name);
            }
            randoItemGive(item_id);
        }

        if (recomp_get_config_u32("deathlink") != last_deathlink_status) {
            last_deathlink_status = recomp_get_config_u32("deathlink");
            rando_toggle_death_link(recomp_get_config_u32("deathlink"));
        }

        if (play->pauseCtx.state == 0 && rando_get_death_link_enabled() && rando_get_death_link_pending()) {
            Play_KillPlayer();
            if (rando_get_slotdata_u32("death_behavior") == 3) {
                Interface_StartMoonCrash(play);
            }
            rando_reset_death_link_pending();
        }

        // check for 100% condition to give majora soul (gigarando)
        // TODO add checks for the rest of the conditions
        if (rando_met_all_goal() && !rando_location_is_checked(AP_ITEM_ID_SOUL_BOSS_MAJORA)) {
            Player* player = GET_PLAYER(play);
            Actor_OfferGetItemHook(player->tatlActor, play, rando_get_item_id(AP_ITEM_ID_SOUL_BOSS_MAJORA), AP_ITEM_ID_SOUL_BOSS_MAJORA, 100.0f, 100.0f, true, true);
            // rando_complete_goal();
        }
    }

    // testing for time skips
    if (recomp_get_config_u32("skip_time") && CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_L)) {
        // dsot_set_time(play, day, CLOCK_TIME(hour, 0));
        dsot_set_time(play, recomp_get_config_u32("time_day"), CLOCK_TIME(recomp_get_config_u32("time_hour"), 0));
        // macros to maybe use later
        // TIME_UNTIL_MOON_CRASH
        // TIME_UNTIL_NEW_DAY
        // CURRENT_DAY
        
    }

    // @glue deactivate the interpreter
    REPY_FN_CLEANUP;
    REPY_PopInterpreter();
}

RECOMP_HOOK("FileSelect_Main")
void fileselect_update() {
    notificationUpdateCycle();
}