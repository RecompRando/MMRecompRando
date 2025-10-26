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

RECOMP_IMPORT(".", int rando_get_starting_heart_locations());
RECOMP_IMPORT(".", int rando_get_tunic_color());

RECOMP_IMPORT("mm_recomp_better_double_sot", void dsot_set_skip_dsot_cutscene(bool new_val));

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

    randoCreateStartMenu();
    randoCreateSoloMenu();
    randoCreateYamlConfigMenu();
    randoCreateAPConnectMenu();

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

    if (rando_get_slotdata_u32("skullsanity") == 2)
    {
        for (int i = 0x00; i <= 0x1E; ++i)
        {
            if (i == 0x03)
            {
                continue;
            }
            
            u32 location_id = 0x062700 | i;
            rando_remove_queued_scout(location_id);
        }
        for (int i = 0x01; i <= 0x1E; ++i)
        {
            u32 location_id = 0x062800 | i;
            rando_remove_queued_scout(location_id);
        }
    }
    
    for (u32 i = rando_get_slotdata_u32("starting_heart_locations"); i < 8; ++i)
    {
        u32 location_id = 0x0D0000 | i;
        rando_remove_queued_scout(location_id);
    }

    if (rando_get_slotdata_u32("cowsanity") == 0)
    {
        for (int i = 0x10; i <= 0x17; ++i)
        {
            u32 location_id = 0xBEEF00 | i;
            rando_remove_queued_scout(location_id);
        }
    }
    
    if (rando_get_slotdata_u32("scrubsanity") == 0)
    {
        rando_remove_queued_scout(0x090100 | GI_MAGIC_BEANS);
        rando_remove_queued_scout(0x090100 | GI_BOMB_BAG_40);
        rando_remove_queued_scout(0x090100 | GI_POTION_GREEN);
        rando_remove_queued_scout(0x090100 | GI_POTION_BLUE);
    }
    
    if (rando_get_slotdata_u32("shopsanity") != 2)
    {
        rando_remove_queued_scout(0x05481E);
        rando_remove_queued_scout(0x024234);
        
        if (rando_get_slotdata_u32("shopsanity") == 1)
        {
            for (int i = SI_FAIRY_2; i <= SI_POTION_RED_3; ++i)
            {
                u32 location_id = 0x090000 | i;
                rando_remove_queued_scout(location_id);
            }
            
            rando_remove_queued_scout(0x090000 | SI_BOMB_3);
            rando_remove_queued_scout(0x090000 | SI_ARROWS_SMALL_3);
            rando_remove_queued_scout(0x090000 | SI_POTION_RED_6);
        }
        else
        {
            for (int i = SI_POTION_RED_1; i <= SI_POTION_RED_6; ++i)
            {
                if (i == SI_BOMB_BAG_20_1 || i == SI_BOMB_BAG_40)
                {
                    continue;
                }
                
                u32 location_id = 0x090000 | i;
                rando_remove_queued_scout(location_id);
            }
            
            rando_remove_queued_scout(0x090013);
            rando_remove_queued_scout(0x090015);
            
            rando_remove_queued_scout(0x026392);
            rando_remove_queued_scout(0x090000 | GI_CHATEAU);
            rando_remove_queued_scout(0x006792);
            rando_remove_queued_scout(0x000091);
        }
    }
    
    if (rando_get_slotdata_u32("curiostity_shop_trades") == 0)
    {
        rando_remove_queued_scout(0x07C402);
        rando_remove_queued_scout(0x07C404);
        rando_remove_queued_scout(0x07C405);
        rando_remove_queued_scout(0x07C407);
    }
    
    if (rando_get_slotdata_u32("intro_checks") == 0)
    {
        rando_remove_queued_scout(0x061A00);
    }
    
    if (rando_get_slotdata_u32("grasssanity") == 0)
    {
        removeAllScoutsWithPrefix(AP_PREFIX_GRASS_GROUP);
        removeAllScoutsWithPrefix(AP_PREFIX_GRASS_GROUP2);
        removeAllScoutsWithPrefix(AP_PREFIX_GRASS_KUSA);
        removeAllScoutsWithPrefix(AP_PREFIX_GRASS_KEATON);
        removeAllScoutsWithPrefix(AP_PREFIX_MURE_GRASS);
    }
    
    if (rando_get_slotdata_u32("potsanity") == 0)
    {
        removeAllScoutsWithPrefix(AP_PREFIX_POTS);
    }
    
    if (rando_get_slotdata_u32("scarecrowsanity") == 0)
    {
        removeAllScoutsWithPrefix(AP_PREFIX_SCARECROW);
    }

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

u32 rando_get_item_id(u32 location)
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
                        return (GI_MAX + (((item >> 8) & 0xF) * 4) + 1);
                    case ITEM_KEY_SMALL:
                        return (GI_MAX + (((item >> 8) & 0xF) * 4) + 2);
                    case ITEM_DUNGEON_MAP:
                        return (GI_MAX + (((item >> 8) & 0xF) * 4) + 3);
                    case ITEM_COMPASS:
                        return (GI_MAX + (((item >> 8) & 0xF) * 4) + 4);
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
            case AP_ITEM_PREFIX_SOUL_ABSURD:
                return GI_ABSURD_GENERIC;
        }
    }
    
    switch (rando_get_location_type(location)) {
        case RANDO_ITEM_CLASS_PROGRESSION:
        case RANDO_ITEM_CLASS_TRAP:
            return GI_AP_PROG;
        case RANDO_ITEM_CLASS_USEFUL:
            return GI_AP_USEFUL;
        case RANDO_ITEM_CLASS_JUNK:
        default:
            return GI_AP_FILLER;
    }
}

RECOMP_DECLARE_EVENT(rando_on_start());

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
                    switch (item_id & 0xFF0000) {
                        case 0x0A0000:
                        case 0x0B0000:
                        case 0x0C0000:
                        case 0x0D0000:
                        case 0x0E0000:
                        case 0x0F0000:
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
            rando_send_location(0x0D0000 | GI_OCARINA_OF_TIME);
            rando_send_location(0x0D0067);

            for (int i = 0; i < rando_get_starting_heart_locations(); ++i)
            {
                rando_send_location(0x0D0000 | i);
            }

            old_items_size = new_items_size;
            initItems = true;

            rando_on_start();
        }

        if (new_items_size > old_items_size) {
            u32 item_id = rando_get_item(old_items_size);
            if (rando_get_sending_player(old_items_size) != rando_get_own_slot_id() || rando_get_item_location(old_items_size) <= 0) {
                char item_name[33];
                char player_name[36];
                rando_get_item_name_from_id(item_id, item_name);
                rando_get_sending_player_name(old_items_size, player_name);
                randoEmitRecieveNotification(item_name, player_name, randoConvertItemId(item_id), RANDO_ITEM_CLASS_PROGRESSION); // TODO: fix item classification
            }
            randoItemGive(item_id);

            old_items_size += 1;
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