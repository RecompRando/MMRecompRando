#ifndef __APCOMMON_H__
#define __APCOMMON_H__

#include "modding.h"
#include "global.h"

#include "rando_glue.h"

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

#define GI_STRAY_FAIRY_CLOCKTOWN GI_B2
#define GI_STRAY_FAIRY_WOODFALL GI_46
#define GI_STRAY_FAIRY_SNOWHEAD GI_47
#define GI_STRAY_FAIRY_GREATBAY GI_48
#define GI_STRAY_FAIRY_STONETOWER GI_49

#define GI_SONG_TIME GI_A6
#define GI_SONG_HEALING GI_AF
#define GI_SONG_EPONA GI_A5
#define GI_SONG_SOARING GI_A3
#define GI_SONG_STORMS GI_A2
#define GI_SONG_SONATA GI_AE
#define GI_SONG_LULLABY GI_AD
#define GI_SONG_NOVA GI_AC
#define GI_SONG_ELEGY GI_A8
#define GI_SONG_OATH GI_A7

#define GI_SPIN_ATTACK GI_71
#define GI_OCEAN_SKULL_TOKEN GI_72
#define GI_DEFENSE_DOUBLE GI_73
#define GI_TRUE_SKULL_TOKEN GI_75

// this enum MUST match sGetItemTable_ap in item_give.c
typedef enum {
    GI_OFFSET = GI_MAX,
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
    // end
    GID_EXTENDED_MAX,
} ExtendedGIDTable;

#define AP_ITEM_ID_STRAY_FAIRY_WOODFALL 0x010000
#define AP_ITEM_ID_STRAY_FAIRY_SNOWHEAD 0x010001
#define AP_ITEM_ID_STRAY_FAIRY_GREATBAY 0x010002
#define AP_ITEM_ID_STRAY_FAIRY_STONETOWER 0x010003

#define AP_ITEM_ID_KEY_SMALL_WOODFALL 0x090078
#define AP_ITEM_ID_KEY_SMALL_SNOWHEAD 0x090178
#define AP_ITEM_ID_KEY_SMALL_GREATBAY 0x090278
#define AP_ITEM_ID_KEY_SMALL_STONETOWER 0x090378

#define AP_ITEM_ID_MAGIC 0x020000

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

#define MAX_BOMBCHUS ((s8) (10*rando_has_item(GI_BAG_BOMBCHU) + 10))

// WEEKEVENTREG_17_80: we have given the Moon's Tear to the scrub
// WEEKEVENTREG_74_80: the Moon's Tear has fallen
// WEEKEVENTREG_74_40: we have collected the Moon's Tear

extern GetItemId noShuffleList[];

extern PlayState* gPlay;
extern bool saveOpened;
extern bool playing;

extern bool justDied;

bool loadObject(PlayState* play, void** objectSegment, OSMesgQueue* objectLoadQueue, s16 objectId);
void GetItem_DrawDynamic(PlayState* play, void* objectSegment, s16 objectId);
s32 Actor_OfferGetItemHook(Actor* actor, PlayState* play, GetItemId getItemId, u32 location, f32 xzRange, f32 yRange, bool use_workaround, bool item_is_shuffled);

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

typedef struct GetItemEntryAP {
    /* 0x0 */ u8 itemId;
    /* 0x1 */ u8 field; // various bit-packed data
    /* 0x2 */ s16 gid;   // defines the draw id and chest opening animation
    /* 0x3 */ u16 textId;
    /* 0x4 */ u16 objectId;
} GetItemEntryAP; // size = 0x6

bool isAP(s16 gi);

u16 getObjectId(s16 gi);

s16 getGid(s16 gi);

u16 getTextId(s16 gi);

extern s8 giToItemId[];

typedef enum {
    RANDO_ITEM_CLASS_JUNK           = 0b000,
    RANDO_ITEM_CLASS_PROGRESSION    = 0b001,
    RANDO_ITEM_CLASS_USEFUL         = 0b010,
    RANDO_ITEM_CLASS_TRAP           = 0b100
} RandoItemClassification;

void randoCreateStartMenu();
void randoShowStartMenu();
void randoCreateSoloMenu();
void randoShowSoloMenu();
void randoCreateYamlConfigMenu();
void randoShowYamlConfigMenu();
void randoCreateAPConnectMenu();
void randoShowAPConnectMenu();
void randoCreateImportMenu();
void randoShowImportMenu();
void randoCreateModCheckMenu();
bool randoGenerateMenuOpen();
bool randoCheckRequiredMods();
void randoStart(bool multiworld);

void randoScout();

typedef enum {
    RANDO_NOTIFICATION_NORMAL,
    RANDO_NOTIFICATION_ERROR,
    RANDO_NOTIFICATION_RECEIVE,
    RANDO_NOTIFICATION_SEND
} RandoNotificationType;

void notificationUpdateCycle();
void randoCreateNotificationContainer();
void randoEmitReceiveNotification(const char* item_name, const char* from_name, const ItemId item, RandoItemClassification item_class);
void randoCreateReceiveNotification(const char* item_name, const char* from_name, const ItemId item, RandoItemClassification item_class);
void randoEmitSendNotification(const char* item_name, const char* to_name, const ItemId item, RandoItemClassification item_class);
void randoCreateSendNotification(const char* item_name, const char* to_name, const ItemId item, RandoItemClassification item_class);
void randoAddAPNotificationToQueue(RandoNotificationType notif_type, const char* item_name, const char* player_name, const ItemId item, RandoItemClassification item_class);
void randoEmitNormalNotification(const char* notif_text);
void randoCreateNormalNotification(const char* notif_text);
void randoEmitErrorNotification(const char* error_text);
void randoCreateErrorNotification(const char* error_text);
void randoAddNotificationToQueue(RandoNotificationType notif_type, const char* notif_text);

#endif