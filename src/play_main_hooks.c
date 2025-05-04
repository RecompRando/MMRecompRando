#include "modding.h"
#include "global.h"
#include "recompconfig.h"

#include "apcommon.h"
#include "yaml_generation.h"

#define LOCATION_INVENTORY_SWORD 0x000037
#define LOCATION_INVENTORY_SHIELD 0x000032

#define C_TO_PARAMS(c) (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF

RECOMP_IMPORT("*", int recomp_set_moon_crash_resets_save(bool new_val));
RECOMP_IMPORT("*", int recomp_set_fd_anywhere(bool new_val));
RECOMP_IMPORT("*", int recomp_set_no_bow_epona_fix(bool new_val));
RECOMP_IMPORT("*", int recomp_set_allow_no_ocarina_tf(bool new_val));
RECOMP_IMPORT("*", int recomp_set_h_and_d_no_sword_fix(bool new_val));

RECOMP_IMPORT(".", int rando_get_starting_heart_locations());
RECOMP_IMPORT(".", int rando_get_tunic_color());

RECOMP_IMPORT("mm_recomp_better_double_sot", void dsot_set_skip_dsot_cutscene(bool new_val));

RECOMP_IMPORT("mm_recomp_colors", void colors_set_human_tunic(u8 r, u8 g, u8 b));

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

    randoCreateStartMenu();
    randoCreateSoloMenu();
    randoCreateYamlConfigMenu();
    randoCreateAPConnectMenu();

    randoCreateNotificationContainer();
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

void Play_KillPlayer() {
    gSaveContext.save.saveInfo.playerData.health = 0;
}

bool initItems = false;
u32 old_items_size;
bool waiting_death_link = false;
bool sending_death_link = false;

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

        if (rando_death_behavior() == 3) {
            Interface_StartMoonCrash(play);
        }

        return false;
    } else {
        return true;
    }
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

RECOMP_CALLBACK("*", recomp_on_play_main)
void update_rando(PlayState* play) {
    u32 new_items_size;
    u32 i;
    u8* save_ptr;

    gPlay = play;

    notificationUpdateCycle();

    if (saveOpened) {
        new_items_size = rando_get_items_size();

        if (!initItems) {
            u8 new_bow_level = rando_has_item(GI_QUIVER_30);
            u8 new_bomb_level = rando_has_item(GI_BOMB_BAG_20);
            u8 new_wallet_level = rando_has_item(GI_WALLET_ADULT);
            u8 new_sword_level = rando_has_item(GI_SWORD_KOKIRI);
            u8 new_shield_level = rando_has_item(GI_SHIELD_HERO);

            u8 bottle_count = 0;

            s16 old_health = gSaveContext.save.saveInfo.playerData.health;

            u8 new_magic_level = rando_has_item_async(AP_ITEM_ID_MAGIC);

            if (new_magic_level >= 1 && !gSaveContext.save.saveInfo.playerData.isMagicAcquired) {
                randoItemGive(AP_ITEM_ID_MAGIC);
            }

            if (new_magic_level >= 2 && !gSaveContext.save.saveInfo.playerData.isDoubleMagicAcquired) {
                randoItemGive(AP_ITEM_ID_MAGIC);
            }

            if (!rando_is_magic_trap()) {
                if (new_magic_level < 1) {
                    gSaveContext.save.saveInfo.playerData.magic = 0;
                }
            }

            gSaveContext.save.saveInfo.playerData.healthCapacity = 0x10;
            gSaveContext.save.saveInfo.playerData.health = 0x10;

            if (GET_QUEST_HEART_PIECE_COUNT > 0) {
                DECREMENT_QUEST_HEART_PIECE_COUNT;
            }

            SET_EQUIP_VALUE(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_NONE);
            if (gSaveContext.save.playerForm == PLAYER_FORM_HUMAN) {
                CUR_FORM_EQUIP(EQUIP_SLOT_B) = ITEM_NONE;
            } else {
                BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_B) = ITEM_NONE;
            }
            Interface_LoadItemIconImpl(play, EQUIP_SLOT_B);

            new_bow_level -= CUR_UPG_VALUE(UPG_QUIVER);
            for (i = 0; i < new_bow_level; ++i) {
                randoItemGive(GI_QUIVER_30);
            }

            new_bomb_level -= CUR_UPG_VALUE(UPG_BOMB_BAG);
            for (i = 0; i < new_bomb_level; ++i) {
                randoItemGive(GI_BOMB_BAG_20);
            }

            new_wallet_level -= CUR_UPG_VALUE(UPG_WALLET);
            for (i = 0; i < new_wallet_level; ++i) {
                randoItemGive(GI_WALLET_ADULT);
            }

            new_sword_level -= GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD);
            for (i = 0; i < new_sword_level; ++i) {
                randoItemGive(GI_SWORD_KOKIRI);
            }

            new_shield_level -= GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD);
            for (i = 0; i < new_shield_level; ++i) {
                randoItemGive(GI_SHIELD_HERO);
            }

            for (i = SLOT_BOTTLE_1; i <= SLOT_BOTTLE_6; ++i) {
                if ((gSaveContext.save.saveInfo.inventory.items[i] >= ITEM_POTION_RED && gSaveContext.save.saveInfo.inventory.items[i] <= ITEM_OBABA_DRINK) || gSaveContext.save.saveInfo.inventory.items[i] == ITEM_BOTTLE) {
                    bottle_count += 1;
                }
            }

            if (rando_has_item(GI_BOMBCHUS_1) || rando_has_item(GI_BOMBCHUS_5) || rando_has_item(GI_BOMBCHUS_10) || rando_has_item(GI_BOMBCHUS_20)) {
                randoItemGive(GI_BOMBCHUS_20);
            }

            gSaveContext.save.saveInfo.skullTokenCount = 0;
            gSaveContext.save.saveInfo.skullTokenCount |= rando_has_item(GI_TRUE_SKULL_TOKEN) << 0x10;
            gSaveContext.save.saveInfo.skullTokenCount |= rando_has_item(GI_OCEAN_SKULL_TOKEN);

            gSaveContext.save.saveInfo.inventory.strayFairies[0] = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_WOODFALL);
            gSaveContext.save.saveInfo.inventory.strayFairies[1] = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_SNOWHEAD);
            gSaveContext.save.saveInfo.inventory.strayFairies[2] = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_GREATBAY);
            gSaveContext.save.saveInfo.inventory.strayFairies[3] = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_STONETOWER);

            DUNGEON_KEY_COUNT(0) = rando_has_item(0x090078);
            DUNGEON_KEY_COUNT(1) = rando_has_item(0x090178);
            DUNGEON_KEY_COUNT(2) = rando_has_item(0x090278);
            DUNGEON_KEY_COUNT(3) = rando_has_item(0x090378);

            for (i = old_items_size; i < new_items_size; ++i) {
                u32 item_id = rando_get_item(i) & 0xFFFFFF;
                u8 gi = item_id & 0xFF;
                bool is_gi = (item_id & 0xFF0000) == 0;
                if (is_gi) {
                    if ((gi == GI_BOMBCHUS_1 || gi == GI_BOMBCHUS_5 || gi == GI_BOMBCHUS_10 || gi == GI_BOMBCHUS_20) && INV_HAS(ITEM_BOMBCHU)) {
                        continue;
                    }
                    switch (gi) {
                        case GI_POTION_RED_BOTTLE:
                        case GI_CHATEAU_BOTTLE:
                        case GI_BOTTLE:
                        case GI_MILK_BOTTLE:
                            if (bottle_count > 0) {
                                bottle_count -= 1;
                                continue;
                            }
                            break;
                        case GI_QUIVER_30:
                        case GI_BOMB_BAG_20:
                        case GI_WALLET_ADULT:
                        case GI_SWORD_KOKIRI:
                        case GI_SHIELD_HERO:
                        case GI_TRUE_SKULL_TOKEN:
                        case GI_OCEAN_SKULL_TOKEN:
                            continue;
                    }
                    if (gi == GI_HEART_CONTAINER || gi == GI_HEART_PIECE) {
                        old_health = 0x140;
                    }
                } else {
                    switch (item_id) {
                        case AP_ITEM_ID_MAGIC:
                        case AP_ITEM_ID_STRAY_FAIRY_WOODFALL:
                        case AP_ITEM_ID_STRAY_FAIRY_SNOWHEAD:
                        case AP_ITEM_ID_STRAY_FAIRY_GREATBAY:
                        case AP_ITEM_ID_STRAY_FAIRY_STONETOWER:
                        case AP_ITEM_ID_KEY_SMALL_WOODFALL:
                        case AP_ITEM_ID_KEY_SMALL_SNOWHEAD:
                        case AP_ITEM_ID_KEY_SMALL_GREATBAY:
                        case AP_ITEM_ID_KEY_SMALL_STONETOWER:
                            continue;
                    }
                }
                randoItemGive(item_id);
            }

            gSaveContext.save.saveInfo.playerData.health = MIN(old_health, gSaveContext.save.saveInfo.playerData.healthCapacity);

            if (gSaveContext.save.playerForm == PLAYER_FORM_FIERCE_DEITY) {
                CUR_FORM_EQUIP(EQUIP_SLOT_B) = ITEM_SWORD_DEITY;
                Interface_LoadItemIconImpl(play, EQUIP_SLOT_B);
            }

            rando_send_location(LOCATION_INVENTORY_SWORD);
            rando_send_location(LOCATION_INVENTORY_SHIELD);

            for (int i = 0; i < rando_get_starting_heart_locations(); ++i)
            {
                rando_send_location(0x0D0000 | i);
            }

            old_items_size = new_items_size;
            initItems = true;
        }

        if (new_items_size > old_items_size) {
            for (u32 i = old_items_size; i < new_items_size; ++i) {
                u32 item_id = rando_get_item(i);
                if (rando_get_sending_player(i) != rando_get_own_slot_id()) {
                    char item_name[33];
                    char player_name[17];
                    rando_get_item_name_from_id(item_id, item_name);
                    rando_get_sending_player_name(i, player_name);
                    randoEmitRecieveNotification(item_name, player_name, randoConvertItemId(item_id), RANDO_ITEM_CLASS_PROGRESSION);
                }
                randoItemGive(item_id);
            }

            old_items_size = new_items_size;
        }

        if (play->pauseCtx.state == 0 && rando_get_death_link_enabled() && rando_get_death_link_pending()) {
            Play_KillPlayer();
            if (rando_death_behavior() == 3) {
                Interface_StartMoonCrash(play);
            }
            rando_reset_death_link_pending();
        }
    }
}

RECOMP_HOOK("FileSelect_Main")
void fileselect_update() {
    notificationUpdateCycle();
}