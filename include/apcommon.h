#ifndef __APCOMMON_H__
#define __APCOMMON_H__

#include "modding.h"
#include "global.h"

#define GI_AP_PROG GI_77
#define GI_AP_FILLER GI_90
#define GI_AP_USEFUL GI_B3

#define ITEM_AP_PROG 0xCD
#define ITEM_AP_FILLER 0xCE
#define ITEM_AP_USEFUL 0xCF

#define GID_APLOGO_FILLER GID_37
#define GID_APLOGO_PROG GID_46
#define GID_APLOGO_USEFUL GID_4C

#define GI_BAG_BOMBCHU GI_54

#define GI_SPIN_ATTACK GI_71
#define GI_OCEAN_SKULL_TOKEN GI_72
#define GI_DEFENSE_DOUBLE GI_73
#define GI_TRUE_SKULL_TOKEN GI_75

// this enum MUST match sGetItemTable_ap in item_give.c
typedef enum {
    GI_OFFSET = GI_MAX,
    // junk fix
    GI_RECOVERY_HEART_RANDO,
    // dungeon items
    GI_KEY_BOSS_WOODFALL,
    GI_KEY_SMALL_WOODFALL,
    GI_MAP_WOODFALL,
    GI_COMPASS_WOODFALL,
    GI_KEY_BOSS_SNOWHEAD,
    GI_KEY_SMALL_SNOWHEAD,
    GI_MAP_SNOWHEAD,
    GI_COMPASS_SNOWHEAD,
    GI_KEY_BOSS_GREATBAY,
    GI_KEY_SMALL_GREATBAY,
    GI_MAP_GREATBAY,
    GI_COMPASS_GREATBAY,
    GI_KEY_BOSS_STONETOWER,
    GI_KEY_SMALL_STONETOWER,
    GI_MAP_STONETOWER,
    GI_COMPASS_STONETOWER,
    // upgrades
    GI_MAGIC_UPGRADE,
    // owls
    GI_OWL_GREAT_BAY_COAST,
    GI_OWL_ZORA_CAPE,
    GI_OWL_SNOWHEAD,
    GI_OWL_MOUNTAIN_VILLAGE,
    GI_OWL_CLOCK_TOWN,
    GI_OWL_MILK_ROAD,
    GI_OWL_WOODFALL,
    GI_OWL_SOUTHERN_SWAMP,
    GI_OWL_IKANA_CANYON,
    GI_OWL_STONE_TOWER,
    GI_OWL_HIDDEN,
    // frogs
    GI_FROG_YELLOW,
    GI_FROG_CYAN,
    GI_FROG_PINK,
    GI_FROG_BLUE,
    GI_FROG_WHITE,
    // scarecrows
    GI_SCARECROW,
    // boss souls
    GI_BOSS_SOUL_ODOLWA,
    GI_BOSS_SOUL_GOHT,
    GI_BOSS_SOUL_GYORG,
    GI_BOSS_SOUL_TWINMOLD,
    GI_BOSS_SOUL_MAJORA,
    // misc souls
    GI_MISC_SOUL_COW,
    GI_MISC_SOUL_KEATON,
    GI_MISC_SOUL_GOLD_SKULLTULAS,
    GI_MISC_SOUL_BUTTERFLY,
    // generic souls
    GI_NPC_GENERIC,
    GI_UTILITY_GENERIC,
    GI_ABSURD_GENERIC,
    // custom oot/ww items
    GI_OOT_ITEM,
    GI_WW_ITEM,
    // end
    GI_EXTENDED_MAX,
} ExtendedGITable;

typedef enum {
    GID_OFFSET = GID_MASK_FIERCE_DEITY,
    // dungeon songs
    GID_SONG_SONATA,
    GID_SONG_LULLABY,
    GID_SONG_NOVA,
    GID_SONG_ELEGY,
    GID_SONG_OATH,
    // normal songs
    GID_SONG_TIME,
    GID_SONG_HEALING,
    GID_SONG_EPONA,
    GID_SONG_SOARING,
    GID_SONG_STORMS,
    // upgrades
    GID_SPIN_ATTACK,
    GID_MAGIC_UPGRADE,
    // stray fairies
    GID_SF_WOODFALL,
    GID_SF_SNOWHEAD,
    GID_SF_GREATBAY,
    GID_SF_STONETOWER,
    GID_SF_CLOCKTOWN,
    // dungeon items
    GID_KEY_BOSS_WOODFALL,
    GID_KEY_SMALL_WOODFALL,
    GID_MAP_WOODFALL,
    GID_COMPASS_WOODFALL,
    GID_KEY_BOSS_SNOWHEAD,
    GID_KEY_SMALL_SNOWHEAD,
    GID_MAP_SNOWHEAD,
    GID_COMPASS_SNOWHEAD,
    GID_KEY_BOSS_GREATBAY,
    GID_KEY_SMALL_GREATBAY,
    GID_MAP_GREATBAY,
    GID_COMPASS_GREATBAY,
    GID_KEY_BOSS_STONETOWER,
    GID_KEY_SMALL_STONETOWER,
    GID_MAP_STONETOWER,
    GID_COMPASS_STONETOWER,
    // custom items
    GID_BAG_BOMBCHU,
    GID_DEFENSE_DOUBLE,
    GID_SWAMP_SKULL_TOKEN,
    GID_OCEAN_SKULL_TOKEN,
    GID_RUPOOR, // unimplemented
    // owls + frogs + scarecrows
    GID_OWL_STATUE,
    GID_FROG_YELLOW,
    GID_FROG_CYAN,
    GID_FROG_PINK,
    GID_FROG_BLUE,
    GID_FROG_WHITE,
    GID_SCARECROW,
    // boss souls
    GID_BOSS_SOUL_ODOLWA,
    GID_BOSS_SOUL_GOHT,
    GID_BOSS_SOUL_GYORG,
    GID_BOSS_SOUL_TWINMOLD,
    GID_BOSS_SOUL_MAJORA,
    // misc souls
    GID_MISC_SOUL_COW,
    GID_MISC_SOUL_KEATON,
    GID_MISC_SOUL_GOLD_SKULLTULAS,
    GID_MISC_SOUL_BUTTERFLY,
    // generic souls
    GID_NPC_SOUL_GENERIC,
    GID_UTILITY_SOUL_GENERIC,
    GID_ABSURD_SOUL_GENERIC,
    // custom oot/ww items
    GID_OOT_ITEM,
    GID_WW_ITEM,
    // end
    GID_EXTENDED_MAX,
} ExtendedGIDTable;

#define AP_PREFIX_GRASS_GROUP 0x100000
#define AP_PREFIX_GRASS_GROUP2 0x110000
#define AP_PREFIX_GRASS_KUSA 0x120000
#define AP_PREFIX_GRASS_KEATON 0x130000
#define AP_PREFIX_MURE_GRASS 0x140000
#define AP_PREFIX_POTS 0x200000
#define AP_PREFIX_HITSPOT 0x150000
#define AP_PREFIX_INVISRUPEES 0x160000
#define AP_PREFIX_ROCKS 0x180000
#define AP_PREFIX_MURE_ROCKS 0x260000
#define AP_PREFIX_BOULDERS 0x1B0000
#define AP_PREFIX_SOIL 0x190000
#define AP_PREFIX_FREESTANDING 0x170000
#define AP_PREFIX_SNOWBALLS 0x210000
#define AP_PREFIX_SMALL_CRATES 0x250000
#define AP_PREFIX_BIG_CRATES 0x270000
#define AP_PREFIX_BARRELS 0x220000
#define AP_PREFIX_FAIRIES 0x0F0000
#define AP_PREFIX_GOSSIP 0x1F0000
#define AP_PREFIX_BUTTERFLIES 0xBF0000
#define AP_PREFIX_ICICLES 0x230000
#define AP_PREFIX_SCARECROW 0x300000
#define AP_PREFIX_BEEHIVES 0x240000
#define AP_PREFIX_NOTEBOOK 0x0B0000
#define AP_PREFIX_OWLS 0xFF1500
#define AP_PREFIX_FROGS 0xFF0000
#define AP_PREFIX_PALM_TREES 0x2A0000
#define AP_PREFIX_SNOW_TREES 0x2B0000
#define AP_PREFIX_WOOD_TREES 0x2C0000
#define AP_PREFIX_TREE_TREES 0x2D0000
#define AP_PREFIX_FLOWERS 0x3F0000

#define AP_ITEM_PREFIX_SOUL_MISC 0x0A0000
#define AP_ITEM_PREFIX_SOUL_BOSS 0x0B0000
#define AP_ITEM_PREFIX_SOUL_NPC 0x0C0000
#define AP_ITEM_PREFIX_SOUL_UTILITY 0x0D0000
#define AP_ITEM_PREFIX_SOUL_ABSURD 0x0E0000

#define AP_ITEM_ID_STRAY_FAIRY_WOODFALL 0x010000
#define AP_ITEM_ID_STRAY_FAIRY_SNOWHEAD 0x010001
#define AP_ITEM_ID_STRAY_FAIRY_GREATBAY 0x010002
#define AP_ITEM_ID_STRAY_FAIRY_STONETOWER 0x010003

#define AP_ITEM_ID_KEY_SMALL_WOODFALL 0x090078
#define AP_ITEM_ID_KEY_SMALL_SNOWHEAD 0x090178
#define AP_ITEM_ID_KEY_SMALL_GREATBAY 0x090278
#define AP_ITEM_ID_KEY_SMALL_STONETOWER 0x090378

#define AP_ITEM_ID_MAGIC 0x020000

#define AP_ITEM_ID_SOUL_BOSS_ODOLWA AP_ITEM_PREFIX_SOUL_BOSS | ACTOR_BOSS_01
#define AP_ITEM_ID_SOUL_BOSS_GOHT AP_ITEM_PREFIX_SOUL_BOSS | ACTOR_BOSS_HAKUGIN
#define AP_ITEM_ID_SOUL_BOSS_GYORG AP_ITEM_PREFIX_SOUL_BOSS | ACTOR_BOSS_03
#define AP_ITEM_ID_SOUL_BOSS_TWINMOLD AP_ITEM_PREFIX_SOUL_BOSS | ACTOR_BOSS_02
#define AP_ITEM_ID_SOUL_BOSS_MAJORA AP_ITEM_PREFIX_SOUL_BOSS | ACTOR_BOSS_07

#define AP_ITEM_ID_SOUL_MINIBOSS_WART 0x0C0000 | ACTOR_BOSS_04
#define AP_ITEM_ID_SOUL_MINIBOSS_GEKKO 0x0C0000 | ACTOR_EN_PAMETFROG // unsure if not having the next two is a problem
#define AP_ITEM_ID_SOUL_MINIBOSS_SNAPPER 0x0C0000 | ACTOR_EN_BIGPAMET // check gekko as well
#define AP_ITEM_ID_SOUL_MINIBOSS_JELLY 0x0C0000 | ACTOR_EN_BIGSLIME // check gekko as well
#define AP_ITEM_ID_SOUL_MINIBOSS_IGOS 0x0C0000 | ACTOR_EN_KNIGHT

#define AP_ITEM_ID_SOUL_MISC_COW AP_ITEM_PREFIX_SOUL_MISC | ACTOR_EN_COW
#define AP_ITEM_ID_SOUL_MISC_KEATON AP_ITEM_PREFIX_SOUL_MISC | ACTOR_EN_KITAN
#define AP_ITEM_ID_SOUL_MISC_GOLD_SKULLTULA AP_ITEM_PREFIX_SOUL_MISC | ACTOR_EN_SW
#define AP_ITEM_ID_SOUL_MISC_BUTTERFLY AP_ITEM_PREFIX_SOUL_MISC | ACTOR_EN_BUTTE

#define INV_HAS(x) (INV_CONTENT(x) == x)

#define LOCATION_GRANNY_STORY_1 0x070243
#define LOCATION_GRANNY_STORY_2 0x080243
#define LOCATION_PLAYGROUND_ANY_DAY 0x0801C9
#define LOCATION_PLAYGROUND_ALL_DAYS 0x0701C9
#define LOCATION_HONEY_AND_DARLING_ANY_DAY 0x0800B5
#define LOCATION_HONEY_AND_DARLING_ALL_DAYS 0x0700B5
#define LOCATION_BANK_200_REWARD GI_WALLET_ADULT
#define LOCATION_BANK_500_REWARD 0x080177
#define LOCATION_BANK_1000_REWARD 0x070177
#define LOCATION_LOTTERY_SHOP 0x080239
#define LOCATION_TOWN_GALLERY_PERFECT 0x07011D
#define LOCATION_SWAMP_GALLERY_PERFECT 0x08011D
#define LOCATION_BEAN_DADDY 0x0800A5
#define LOCATION_EVAN_SONG 0x070241
#define LOCATION_SWAMP_GUIDE_GOOD 0x071C54
#define LOCATION_SWAMP_GUIDE_OKAY 0x071C52

#define ITEM00_BOMBCHU 0x1E
#define ITEM00_APITEM 0x1F

#define MAX_BOMBCHUS ((s8) (10*rando_has_item_async(GI_BAG_BOMBCHU) + 10))

RECOMP_IMPORT(".", bool rando_location_is_checked(u32 location_id));
RECOMP_IMPORT(".", bool rando_location_is_checked_async(u32 location_id));
RECOMP_IMPORT(".", bool rando_get_death_link_pending());
RECOMP_IMPORT(".", void rando_reset_death_link_pending());
RECOMP_IMPORT(".", bool rando_get_death_link_enabled());
RECOMP_IMPORT(".", void rando_send_death_link());
RECOMP_IMPORT(".", u32 rando_damage_multiplier());
RECOMP_IMPORT(".", u32 rando_death_behavior());
RECOMP_IMPORT(".", u32 rando_get_moon_remains_required());
RECOMP_IMPORT(".", u32 rando_get_majora_remains_required());
RECOMP_IMPORT(".", u32 rando_get_random_seed());
RECOMP_IMPORT(".", bool rando_is_magic_trap());
// RECOMP_IMPORT(".", bool rando_get_camc_enabled());
RECOMP_IMPORT(".", bool rando_skulltulas_enabled());
RECOMP_IMPORT(".", bool rando_shopsanity_enabled());
RECOMP_IMPORT(".", bool rando_advanced_shops_enabled());
RECOMP_IMPORT(".", bool rando_get_curiostity_shop_trades());
RECOMP_IMPORT(".", bool rando_scrubs_enabled());
RECOMP_IMPORT(".", bool rando_cows_enabled());
RECOMP_IMPORT(".", u32 rando_get_slotdata_u32(char* key));
RECOMP_IMPORT(".", void rando_get_slotdata_raw_o32(const char* key, u32* out_handle_ptr));
RECOMP_IMPORT(".", u32 rando_access_slotdata_raw_u32_o32(u32* in_handle_ptr));
RECOMP_IMPORT(".", void rando_access_slotdata_raw_array_o32(u32* in_handle_ptr, u32 index, u32* out_handle_ptr));
RECOMP_IMPORT(".", void rando_access_slotdata_raw_dict_o32(u32* in_handle_ptr, const char* key, u32* out_handle_ptr));
RECOMP_IMPORT(".", u32 rando_get_location_type(u32 location_id));
RECOMP_IMPORT(".", u32 rando_get_location_has_local_item(u32 location_id));
RECOMP_IMPORT(".", u32 rando_get_item_at_location(u32 location_id));
RECOMP_IMPORT(".", void rando_broadcast_location_hint(u32 location_id));
RECOMP_IMPORT(".", void rando_send_location(u32 location_id));
RECOMP_IMPORT(".", void rando_complete_goal());
RECOMP_IMPORT(".", u32 rando_has_item(u32 item_id));
RECOMP_IMPORT(".", u32 rando_has_item_async(u32 item_id));
RECOMP_IMPORT(".", u32 rando_get_own_slot_id());
RECOMP_IMPORT(".", s16 rando_get_shop_price(u32 shop_item_id));
RECOMP_IMPORT(".", u32 rando_get_items_size());
RECOMP_IMPORT(".", u32 rando_get_item(u32 items_i));
RECOMP_IMPORT(".", s32 rando_get_item_location(u32 items_i));
RECOMP_IMPORT(".", u32 rando_get_sending_player(u32 items_i));
RECOMP_IMPORT(".", void rando_get_item_name_from_id(u32 item_id, char* out_str));
RECOMP_IMPORT(".", void rando_get_sending_player_name(u32 items_i, char* out_str));
RECOMP_IMPORT(".", void rando_get_location_item_player(u32 location_id, char* out_str));
RECOMP_IMPORT(".", void rando_get_location_item_name(u32 location_id, char* out_str));
RECOMP_IMPORT(".", u32 rando_get_last_location_sent());
RECOMP_IMPORT(".", u32 rando_get_seed_name(char* seed_name_out, u32 buffer_size));
RECOMP_IMPORT(".", void rando_get_own_slot_name(char* out_str));
RECOMP_IMPORT(".", void rando_get_saved_apconnect(u8* save_dir, char* address, char* player_name, char* password));
RECOMP_IMPORT(".", void rando_set_saved_apconnect(u8* save_dir, char* address, char* player_name, char* password));
RECOMP_IMPORT(".", void rando_queue_scout(u32 location));
RECOMP_IMPORT(".", void rando_queue_scouts_all());
RECOMP_IMPORT(".", void rando_remove_queued_scout(u32 location));
RECOMP_IMPORT(".", void rando_send_queued_scouts(int hint));

// WEEKEVENTREG_17_80: we have given the Moon's Tear to the scrub
// WEEKEVENTREG_74_80: the Moon's Tear has fallen
// WEEKEVENTREG_74_40: we have collected the Moon's Tear

extern GetItemId noShuffleList[];

extern PlayState* gPlay;
extern bool saveOpened;
extern bool playing;

extern bool justDied;

bool ObjLoad(PlayState* play, u8 segment, s16 objectId);
void ObjUnload(PlayState* play, u8 segment, s16 objectId);
bool loadObject(PlayState* play, void** objectSegment, OSMesgQueue* objectLoadQueue, s16 objectId);
void GetItem_DrawDynamic(PlayState* play, void* objectSegment, s16 objectId);
s32 Actor_OfferGetItemHook(Actor* actor, PlayState* play, GetItemId getItemId, u32 location, f32 xzRange, f32 yRange, bool use_workaround, bool item_is_shuffled);
Actor* Item_RandoDropCollectible(PlayState* play, Vec3f* spawnPos, u32 params, u32 location);

bool rando_get_camc_enabled();

typedef enum {
    CAMC_DISABLED,
    CAMC_SHOW_UNCHECKED,
    CAMC_ENABLED,
} CAMCType;

typedef enum {
    CAMC_DRAW_DISABLED,
    CAMC_DRAW_UNCHECKED,
    CAMC_DRAW_ENABLED,
    CAMC_DRAW_CUSTOM,
} CAMCDrawType;

u8 randoItemGive(u32 gi);
u32 rando_get_item_id(u32 location_id);

typedef struct GetItemEntry {
    /* 0x0 */ u8 itemId;
    /* 0x1 */ u8 field; // various bit-packed data
    /* 0x2 */ s16 gid;   // defines the draw id and chest opening animation
    /* 0x3 */ u8 textId;
    /* 0x4 */ u16 objectId;
} GetItemEntry; // size = 0x6

bool isAP(s16 gi);

u16 getObjectId(s16 gi);

s16 getGid(s16 gi);

u8 getTextId(s16 gi);

extern s8 giToItemId[];

typedef enum {
    RANDO_ITEM_CLASS_JUNK,
    RANDO_ITEM_CLASS_PROGRESSION,
    RANDO_ITEM_CLASS_USEFUL,
    RANDO_ITEM_CLASS_TRAP
} RandoItemClassification;

void randoCreateStartMenu();
void randoShowStartMenu();
void randoCreateSoloMenu();
void randoShowSoloMenu();
void randoCreateYamlConfigMenu();
void randoShowYamlConfigMenu();
void randoCreateAPConnectMenu();
void randoShowAPConnectMenu();
void randoStart(bool multiworld);

void randoScout();

void notificationUpdateCycle();
void randoCreateNotificationContainer();
void randoEmitRecieveNotification(const char* item_name, const char* from_name, const ItemId item, RandoItemClassification item_class);
void randoEmitSendNotification(const char* item_name, const char* to_name, const ItemId item, RandoItemClassification item_class);
void randoEmitNormalNotification(const char* notif_text);
void randoEmitErrorNotification(const char* error_text);
bool randoGenerateMenuOpen();

#endif