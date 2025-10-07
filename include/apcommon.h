#ifndef __APCOMMON_H__
#define __APCOMMON_H__

#include "modding.h"
#include "global.h"

#define GI_BAG_BOMBCHU GI_54

#define GI_SPIN_ATTACK GI_71
#define GI_OCEAN_SKULL_TOKEN GI_72
#define GI_DEFENSE_DOUBLE GI_73
#define GI_TRUE_SKULL_TOKEN GI_75

#define GI_KEY_BOSS_WOODFALL (GI_MAX + 1)
#define GI_KEY_SMALL_WOODFALL (GI_MAX + 2)
#define GI_MAP_WOODFALL (GI_MAX + 3)
#define GI_COMPASS_WOODFALL (GI_MAX + 4)
#define GI_KEY_BOSS_SNOWHEAD (GI_MAX + 5)
#define GI_KEY_SMALL_SNOWHEAD (GI_MAX + 6)
#define GI_MAP_SNOWHEAD (GI_MAX + 7)
#define GI_COMPASS_SNOWHEAD (GI_MAX + 8)
#define GI_KEY_BOSS_GREATBAY (GI_MAX + 9)
#define GI_KEY_SMALL_GREATBAY (GI_MAX + 10)
#define GI_MAP_GREATBAY (GI_MAX + 11)
#define GI_COMPASS_GREATBAY (GI_MAX + 12)
#define GI_KEY_BOSS_STONETOWER (GI_MAX + 13)
#define GI_KEY_SMALL_STONETOWER (GI_MAX + 14)
#define GI_MAP_STONETOWER (GI_MAX + 15)
#define GI_COMPASS_STONETOWER (GI_MAX + 16)

#define GI_MAGIC_UPGRADE (GI_MAX + 17)

#define GI_OWL_GREAT_BAY_COAST (GI_MAX + 18)
#define GI_OWL_ZORA_CAPE (GI_MAX + 19)
#define GI_OWL_SNOWHEAD (GI_MAX + 20)
#define GI_OWL_MOUNTAIN_VILLAGE (GI_MAX + 21)
#define GI_OWL_CLOCK_TOWN (GI_MAX + 22)
#define GI_OWL_MILK_ROAD (GI_MAX + 23)
#define GI_OWL_WOODFALL (GI_MAX + 24)
#define GI_OWL_SOUTHERN_SWAMP (GI_MAX + 25)
#define GI_OWL_IKANA_CANYON (GI_MAX + 26)
#define GI_OWL_STONE_TOWER (GI_MAX + 27)
#define GI_OWL_HIDDEN (GI_MAX + 28)

#define GI_FROG_YELLOW (GI_MAX + 29)
#define GI_FROG_CYAN (GI_MAX + 30)
#define GI_FROG_PINK (GI_MAX + 31)
#define GI_FROG_BLUE (GI_MAX + 32)
#define GI_FROG_WHITE (GI_MAX + 33)

#define GI_BOSS_SOUL_ODOLWA (GI_MAX + 34)
#define GI_BOSS_SOUL_GOHT (GI_MAX + 35)
#define GI_BOSS_SOUL_GYORG (GI_MAX + 36)
#define GI_BOSS_SOUL_TWINMOLD (GI_MAX + 37)
#define GI_BOSS_SOUL_MAJORA (GI_MAX + 38)

#define GI_MISC_SOUL_COW (GI_MAX + 39)
#define GI_MISC_SOUL_KEATON (GI_MAX + 40)
#define GI_MISC_SOUL_GOLD_SKULLTULAS (GI_MAX + 41)
#define GI_MISC_SOUL_BUTTERFLY (GI_MAX + 42)

#define GI_NPC_GENERIC (GI_MAX + 43)
#define GI_UTILITY_GENERIC (GI_MAX + 44)
#define GI_ABSURD_GENERIC (GI_MAX + 45)

#define AP_ITEM_ID_STRAY_FAIRY_WOODFALL 0x010000
#define AP_ITEM_ID_STRAY_FAIRY_SNOWHEAD 0x010001
#define AP_ITEM_ID_STRAY_FAIRY_GREATBAY 0x010002
#define AP_ITEM_ID_STRAY_FAIRY_STONETOWER 0x010003

#define AP_ITEM_ID_KEY_SMALL_WOODFALL 0x090078
#define AP_ITEM_ID_KEY_SMALL_SNOWHEAD 0x090178
#define AP_ITEM_ID_KEY_SMALL_GREATBAY 0x090278
#define AP_ITEM_ID_KEY_SMALL_STONETOWER 0x090378

#define AP_ITEM_ID_MAGIC 0x020000

#define AP_ITEM_ID_SOUL_BOSS_ODOLWA 0x0B0000 | ACTOR_BOSS_01
#define AP_ITEM_ID_SOUL_BOSS_GOHT 0x0B0000 | ACTOR_BOSS_HAKUGIN
#define AP_ITEM_ID_SOUL_BOSS_GYORG 0x0B0000 | ACTOR_BOSS_03
#define AP_ITEM_ID_SOUL_BOSS_TWINMOLD 0x0B0000 | ACTOR_BOSS_02
#define AP_ITEM_ID_SOUL_BOSS_MAJORA 0x0B0000 | ACTOR_BOSS_07

#define AP_ITEM_ID_SOUL_MINIBOSS_WART 0x0C0000 | ACTOR_BOSS_04
#define AP_ITEM_ID_SOUL_MINIBOSS_GEKKO 0x0C0000 | ACTOR_EN_PAMETFROG // unsure if not having the next two is a problem
#define AP_ITEM_ID_SOUL_MINIBOSS_SNAPPER 0x0C0000 | ACTOR_EN_BIGPAMET // check gekko as well
#define AP_ITEM_ID_SOUL_MINIBOSS_JELLY 0x0C0000 | ACTOR_EN_BIGSLIME // check gekko as well
#define AP_ITEM_ID_SOUL_MINIBOSS_IGOS 0x0C0000 | ACTOR_EN_KNIGHT

#define AP_ITEM_ID_SOUL_MISC_COW 0x0A0000 | ACTOR_EN_COW
#define AP_ITEM_ID_SOUL_MISC_KEATON 0x0A0000 | ACTOR_EN_KITAN
#define AP_ITEM_ID_SOUL_MISC_GOLD_SKULLTULA 0x0A0000 | ACTOR_EN_SW
#define AP_ITEM_ID_SOUL_MISC_BUTTERFLY 0x0A0000 | ACTOR_EN_BUTTE

#define GI_AP_PROG GI_77
#define GI_AP_FILLER GI_90
#define GI_AP_USEFUL GI_B3

#define ITEM_AP_PROG 0xCD
#define ITEM_AP_FILLER 0xCE
#define ITEM_AP_USEFUL 0xCF

#define GID_SONG_SONATA (GID_MASK_FIERCE_DEITY + 1)
#define GID_SONG_LULLABY (GID_MASK_FIERCE_DEITY + 2)
#define GID_SONG_NOVA (GID_MASK_FIERCE_DEITY + 3)
#define GID_SONG_ELEGY (GID_MASK_FIERCE_DEITY + 4)
#define GID_SONG_OATH (GID_MASK_FIERCE_DEITY + 5)

#define GID_SONG_TIME (GID_MASK_FIERCE_DEITY + 6)
#define GID_SONG_HEALING (GID_MASK_FIERCE_DEITY + 7)
#define GID_SONG_EPONA (GID_MASK_FIERCE_DEITY + 8)
#define GID_SONG_SOARING (GID_MASK_FIERCE_DEITY + 9)
#define GID_SONG_STORMS (GID_MASK_FIERCE_DEITY + 10)

#define GID_BAG_BOMBCHU (GID_MASK_FIERCE_DEITY + 11)
#define GID_DEFENSE_DOUBLE (GID_MASK_FIERCE_DEITY + 12)
#define GID_SWAMP_SKULL_TOKEN (GID_MASK_FIERCE_DEITY + 35) // oops
#define GID_OCEAN_SKULL_TOKEN (GID_MASK_FIERCE_DEITY + 13)

#define GID_SF_WOODFALL (GID_MASK_FIERCE_DEITY + 14)
#define GID_SF_SNOWHEAD (GID_MASK_FIERCE_DEITY + 15)
#define GID_SF_GREATBAY (GID_MASK_FIERCE_DEITY + 16)
#define GID_SF_STONETOWER (GID_MASK_FIERCE_DEITY + 17)
#define GID_SF_CLOCKTOWN (GI_MAX + 56) // fml

#define GID_KEY_BOSS_WOODFALL (GID_MASK_FIERCE_DEITY + 18)
#define GID_KEY_SMALL_WOODFALL (GID_MASK_FIERCE_DEITY + 19)
#define GID_MAP_WOODFALL (GID_MASK_FIERCE_DEITY + 20)
#define GID_COMPASS_WOODFALL (GID_MASK_FIERCE_DEITY + 21)
#define GID_KEY_BOSS_SNOWHEAD (GID_MASK_FIERCE_DEITY + 22)
#define GID_KEY_SMALL_SNOWHEAD (GID_MASK_FIERCE_DEITY + 23)
#define GID_MAP_SNOWHEAD (GID_MASK_FIERCE_DEITY + 24)
#define GID_COMPASS_SNOWHEAD (GID_MASK_FIERCE_DEITY + 25)
#define GID_KEY_BOSS_GREATBAY (GID_MASK_FIERCE_DEITY + 26)
#define GID_KEY_SMALL_GREATBAY (GID_MASK_FIERCE_DEITY + 27)
#define GID_MAP_GREATBAY (GID_MASK_FIERCE_DEITY + 28)
#define GID_COMPASS_GREATBAY (GID_MASK_FIERCE_DEITY + 29)
#define GID_KEY_BOSS_STONETOWER (GID_MASK_FIERCE_DEITY + 30)
#define GID_KEY_SMALL_STONETOWER (GID_MASK_FIERCE_DEITY + 31)
#define GID_MAP_STONETOWER (GID_MASK_FIERCE_DEITY + 32)
#define GID_COMPASS_STONETOWER (GID_MASK_FIERCE_DEITY + 33)

#define GID_RUPOOR (GID_MASK_FIERCE_DEITY + 34)

#define GID_SPIN_ATTACK (GID_MASK_FIERCE_DEITY + 36)
#define GID_MAGIC_UPGRADE (GID_MASK_FIERCE_DEITY + 37)

#define GID_OWL_STATUE (GID_MASK_FIERCE_DEITY + 38)
#define GID_FROG_YELLOW (GID_MASK_FIERCE_DEITY + 39)
#define GID_FROG_CYAN (GID_MASK_FIERCE_DEITY + 40)
#define GID_FROG_PINK (GID_MASK_FIERCE_DEITY + 41)
#define GID_FROG_BLUE (GID_MASK_FIERCE_DEITY + 42)
#define GID_FROG_WHITE (GID_MASK_FIERCE_DEITY + 43)

#define GID_BOSS_SOUL_ODOLWA (GID_MASK_FIERCE_DEITY + 44)
#define GID_BOSS_SOUL_GOHT (GID_MASK_FIERCE_DEITY + 45)
#define GID_BOSS_SOUL_GYORG (GID_MASK_FIERCE_DEITY + 46)
#define GID_BOSS_SOUL_TWINMOLD (GID_MASK_FIERCE_DEITY + 47)
#define GID_BOSS_SOUL_MAJORA (GID_MASK_FIERCE_DEITY + 48)

#define GID_MISC_SOUL_COW (GI_MAX + 49)
#define GID_MISC_SOUL_KEATON (GI_MAX + 50)
#define GID_MISC_SOUL_GOLD_SKULLTULAS (GI_MAX + 51)
#define GID_MISC_SOUL_BUTTERFLY (GI_MAX + 52)

#define GID_NPC_SOUL_GENERIC (GI_MAX + 53)
#define GID_UTILITY_SOUL_GENERIC (GI_MAX + 54)
#define GID_ABSURD_SOUL_GENERIC (GI_MAX + 55)

#define GID_APLOGO_FILLER GID_37
#define GID_APLOGO_PROG GID_46
#define GID_APLOGO_USEFUL GID_4C

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
RECOMP_IMPORT(".", bool rando_skulltulas_enabled());
RECOMP_IMPORT(".", bool rando_shopsanity_enabled());
RECOMP_IMPORT(".", bool rando_advanced_shops_enabled());
RECOMP_IMPORT(".", bool rando_get_curiostity_shop_trades());
RECOMP_IMPORT(".", bool rando_scrubs_enabled());
RECOMP_IMPORT(".", bool rando_cows_enabled());
RECOMP_IMPORT(".", u32 rando_get_slotdata_u32(char* key));
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