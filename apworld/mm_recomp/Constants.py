from enum import Enum, auto

SHOP_ID_WITCH_POTION_3 = 0x00 # SI_POTION_RED_1
SHOP_ID_WITCH_POTION_2 = 0x01 # SI_POTION_GREEN_1
SHOP_ID_WITCH_POTION_1 = 0x02 # SI_POTION_BLUE
SHOP_ID_TRADING_POST_4 = 0x03 # SI_FAIRY_1
SHOP_ID_TRADING_POST_8 = 0x04 # SI_ARROWS_LARGE_1
SHOP_ID_TRADING_POST_2 = 0x05 # SI_POTION_GREEN_2
SHOP_ID_TRADING_POST_3 = 0x06 # SI_SHIELD_HERO_1
SHOP_ID_TRADING_POST_5 = 0x07 # SI_STICK_1
SHOP_ID_TRADING_POST_6 = 0x08 # SI_ARROWS_MEDIUM_1
SHOP_ID_TRADING_POST_7 = 0x09 # SI_NUTS_1
SHOP_ID_TRADING_POST_1 = 0x0A # SI_POTION_RED_2
SHOP_ID_TRADING_POST_NIGHT_4 = 0x0B # SI_FAIRY_2
SHOP_ID_TRADING_POST_NIGHT_6 = 0x0C # SI_ARROWS_MEDIUM_2
SHOP_ID_TRADING_POST_NIGHT_8 = 0x0D # SI_ARROWS_LARGE_2
SHOP_ID_TRADING_POST_NIGHT_2 = 0x0E # SI_POTION_GREEN_3
SHOP_ID_TRADING_POST_NIGHT_7 = 0x0F # SI_NUTS_2
SHOP_ID_TRADING_POST_NIGHT_5 = 0x10 # SI_STICK_2
SHOP_ID_TRADING_POST_NIGHT_3 = 0x11 # SI_SHIELD_HERO_2
SHOP_ID_TRADING_POST_NIGHT_1 = 0x12 # SI_POTION_RED_3
SHOP_ID_CURIOSITY_SHOP_MASK = 0x13 # SI_MASK_ALL_NIGHT
SHOP_ID_CURIOSITY_SHOP_BOMB_BAG = 0x15 # SI_BOMB_BAG_30_1
SHOP_ID_BOMB_SHOP_3 = 0x17 # SI_BOMB_BAG_20_2
SHOP_ID_BOMB_SHOP_3_UPGRADE = 0x18 # SI_BOMB_BAG_30_2
SHOP_ID_BOMB_SHOP_2 = 0x19 # SI_BOMBCHU
SHOP_ID_BOMB_SHOP_1 = 0x1A # SI_BOMB_1
SHOP_ID_ZORA_SHOP_1 = 0x1B # SI_SHIELD_HERO_3
SHOP_ID_ZORA_SHOP_2 = 0x1C # SI_ARROWS_SMALL_1
SHOP_ID_ZORA_SHOP_3 = 0x1D # SI_POTION_RED_4
SHOP_ID_GORON_SHOP_1 = 0x1E # SI_BOMB_2
SHOP_ID_GORON_SHOP_2 = 0x1F # SI_ARROWS_SMALL_2
SHOP_ID_GORON_SHOP_3 = 0x20 # SI_POTION_RED_5
SHOP_ID_GORON_SHOP_SPRING_1 = 0x21 # SI_BOMB_3
SHOP_ID_GORON_SHOP_SPRING_2 = 0x22 # SI_ARROWS_SMALL_3
SHOP_ID_GORON_SHOP_SPRING_3 = 0x23 # SI_POTION_RED_6
SHOP_ID_CURIOSITY_STOLEN_BOTTLE = 0x24 # SI_BOTTLE
SHOP_ID_CURIOSITY_STOLEN_GFS = 0x25 # SI_SWORD_GREAT_FAIRY
SHOP_ID_CURIOSITY_STOLEN_SWORD_KOKIRI = 0x26 # SI_SWORD_KOKIRI
SHOP_ID_CURIOSITY_STOLEN_SWORD_RAZOR = 0x27 # SI_SWORD_RAZOR
SHOP_ID_CURIOSITY_STOLEN_SWORD_GILDED = 0x28 # SI_SWORD_GILDED

default_shop_prices = [
    20,     # SI_POTION_RED_1
    10,     # SI_POTION_GREEN_1
    60,     # SI_POTION_BLUE
    50,     # SI_FAIRY_1
    40,     # SI_ARROWS_LARGE_1
    30,     # SI_POTION_GREEN_2
    80,     # SI_SHIELD_HERO_1
    10,     # SI_STICK_1
    30,     # SI_ARROWS_MEDIUM_1
    30,     # SI_NUTS_1
    30,     # SI_POTION_RED_2
    50,     # SI_FAIRY_2
    30,     # SI_ARROWS_MEDIUM_2
    40,     # SI_ARROWS_LARGE_2
    30,     # SI_POTION_GREEN_3
    30,     # SI_NUTS_2
    10,     # SI_STICK_2
    80,     # SI_SHIELD_HERO_2
    30,     # SI_POTION_RED_3
    500,    # SI_MASK_ALL_NIGHT
    100,    # SI_BOMB_BAG_20_1 (unused)
    100,    # SI_BOMB_BAG_30_1
    100,    # SI_BOMB_BAG_40 (unused)
    50,     # SI_BOMB_BAG_20_2
    90,     # SI_BOMB_BAG_30_2
    40,     # SI_BOMBCHU
    30,     # SI_BOMB_1
    90,     # SI_SHIELD_HERO_3
    20,     # SI_ARROWS_SMALL_1
    60,     # SI_POTION_RED_4
    40,     # SI_BOMB_2
    40,     # SI_ARROWS_SMALL_2
    80,     # SI_POTION_RED_5
    10,     # SI_BOMB_3
    20,     # SI_ARROWS_SMALL_3
    50,     # SI_POTION_RED_6
    20,     # SI_BOTTLE (curiosity shop)
    100,    # SI_SWORD_GREAT_FAIRY (curiosity shop)
    50,     # SI_SWORD_KOKIRI (curiosity shop)
    50,     # SI_SWORD_RAZOR (curiosity shop)
    50,     # SI_SWORD_GILDED (curiosity shop)
    0,      # SI_SHIELD_HERO_4 (curiosity shop, unused)
    0,      # SI_SHIELD_MIRROR (curiosity shop, unused)
]

shop_location_to_id = {
    # Trading Post
    "Clock Town Trading Post Shop Item 1": SHOP_ID_TRADING_POST_1,
    "Clock Town Trading Post Shop Item 2": SHOP_ID_TRADING_POST_2,
    "Clock Town Trading Post Shop Item 3": SHOP_ID_TRADING_POST_3,
    "Clock Town Trading Post Shop Item 4": SHOP_ID_TRADING_POST_4,
    "Clock Town Trading Post Shop Item 5": SHOP_ID_TRADING_POST_5,
    "Clock Town Trading Post Shop Item 6": SHOP_ID_TRADING_POST_6,
    "Clock Town Trading Post Shop Item 7": SHOP_ID_TRADING_POST_7,
    "Clock Town Trading Post Shop Item 8": SHOP_ID_TRADING_POST_8,
    # Trading Post (Night)
    "Clock Town Trading Post Shop (Night) Item 1": SHOP_ID_TRADING_POST_NIGHT_1,
    "Clock Town Trading Post Shop (Night) Item 2": SHOP_ID_TRADING_POST_NIGHT_2,
    "Clock Town Trading Post Shop (Night) Item 3": SHOP_ID_TRADING_POST_NIGHT_3,
    "Clock Town Trading Post Shop (Night) Item 4": SHOP_ID_TRADING_POST_NIGHT_4,
    "Clock Town Trading Post Shop (Night) Item 5": SHOP_ID_TRADING_POST_NIGHT_5,
    "Clock Town Trading Post Shop (Night) Item 6": SHOP_ID_TRADING_POST_NIGHT_6,
    "Clock Town Trading Post Shop (Night) Item 7": SHOP_ID_TRADING_POST_NIGHT_7,
    "Clock Town Trading Post Shop (Night) Item 8": SHOP_ID_TRADING_POST_NIGHT_8,
    # Bomb Shop
    "Clock Town Bomb Shop Item 1": SHOP_ID_BOMB_SHOP_1,
    "Clock Town Bomb Shop Item 2": SHOP_ID_BOMB_SHOP_2,
    "Clock Town Bomb Shop Item 3": SHOP_ID_BOMB_SHOP_3,
    "Clock Town Bomb Shop Item 3 (Stop Thief)": SHOP_ID_BOMB_SHOP_3_UPGRADE,
    # Curiosity Shop
    "Curiosity Shop Night 3 Thief Stolen Item": SHOP_ID_CURIOSITY_SHOP_BOMB_BAG,
    "Curiosity Shop Night 3 (Stop Thief)": SHOP_ID_CURIOSITY_SHOP_MASK,
    # Magic Hags' Potion Shop
    "Southern Swamp Witch Shop Item 1": SHOP_ID_WITCH_POTION_1,
    "Southern Swamp Witch Shop Item 2": SHOP_ID_WITCH_POTION_2,
    "Southern Swamp Witch Shop Item 3": SHOP_ID_WITCH_POTION_3,
    # Goron Village Shop
    "Goron Village Shop Item 1": SHOP_ID_GORON_SHOP_1,
    "Goron Village Shop Item 2": SHOP_ID_GORON_SHOP_2,
    "Goron Village Shop Item 3": SHOP_ID_GORON_SHOP_3,
    # Goron Village Shop (Spring)
    "Goron Village Shop (Spring) Item 1": SHOP_ID_GORON_SHOP_SPRING_1,
    "Goron Village Shop (Spring) Item 2": SHOP_ID_GORON_SHOP_SPRING_2,
    "Goron Village Shop (Spring) Item 3": SHOP_ID_GORON_SHOP_SPRING_3,
	# Zora Hall Shop
	"Zora Hall Shop Item 1": SHOP_ID_ZORA_SHOP_1,
	"Zora Hall Shop Item 2": SHOP_ID_ZORA_SHOP_2,
	"Zora Hall Shop Item 3": SHOP_ID_ZORA_SHOP_3,
}

# hint helpers
class HintEnum(Enum):
    NONE = 0
    MOON = auto()
    # priority
    DEKU = auto()
    SONATA = auto()
    GORON = auto()
    LULLABY = auto()
    ZORA = auto()
    NOVA = auto()
    ELEGY = auto()
    OATH = auto()
    # etc
    FD = auto()
    MAGIC = auto()
    HD_3 = auto() # honey and darling all days
    DP_3 = auto() # deku playground all days
    BEAVERS = auto() # both beaver rewards
    ANJU_KAFEI = auto() # final anju and kafei reward

priority_hints = [
    HintEnum.DEKU,
    HintEnum.SONATA,
    HintEnum.GORON,
    HintEnum.LULLABY,
    HintEnum.ZORA,
    HintEnum.NOVA,
    HintEnum.ELEGY,
    HintEnum.OATH,
]

other_hints = [
    HintEnum.FD,
    HintEnum.MAGIC,
    HintEnum.HD_3,
    HintEnum.DP_3,
    HintEnum.BEAVERS,
    HintEnum.ANJU_KAFEI,
]

gossip_stones = {
    0x20B0: {
        "name": "Termina Field Gossip Stone Behind Tree",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20B8: {
        "name": "Termina Field Gossip Stone Near Takkuri",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20B9: {
        "name": "Termina Field Gossip Stone Near Kamaro",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20BA: {
        "name": "Termina Field Gossip Stone Near Songwall",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20BB: {
        "name": "Termina Field Gossip Stone Near Road to Ikana",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20C1: {
        "name": "Termina Field Gossip Stone Outside Observatory",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20F3: {
        "name": "Termina Field Gossip Grotto Big Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20F7: {
        "name": "Termina Field Gossip Grotto Small Gossip Stone 1", # New Wave Bossa Nova
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20F8: {
        "name": "Termina Field Gossip Grotto Small Gossip Stone 2", # Sonata
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20F9: {
        "name": "Termina Field Gossip Grotto Small Gossip Stone 3", # Lullaby
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Milk Road
    0x20BE: {
        "name": "Milk Road Keaton Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Romani Ranch
    0x20C4: {
        "name": "Romani Ranch Entrance Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20BC: {
        "name": "Romani Ranch Tree Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20BD: {
        "name": "Romani Ranch Ouside Barn Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20C2: {
        "name": "Romani Ranch Cuccoo Shack Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20C3: {
        "name": "Romani Ranch Doggy Race Track Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Road to Southern Swamp
    0x20C0: {
        "name": "Road to Southern Swamp Near Archery Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Southern Swamp
    0x20B1: {
        "name": "Southern Swamp Outside Woods of Mystery Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Road to Ikana
    0x20B5: {
        "name": "Road to Ikana Outside Graveyard Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Ikana Canyon
    0x20B6: {
        "name": "Ikana Canyon Dock Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20C5: {
        "name": "Ikana Canyon Scrub Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20B7: {
        "name": "Ikana Canyon Ledge Above Poe Hut Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Path to Mountain Village
    0x20B3: {
        "name": "Path to Mountain Village Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20C6: {
        "name": "Mountain Village Spring Near Frog Choir Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x20B2: {
        "name": "Mountain Village Spring Ramp to Goron Graveyard Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Great Bay Coast
    0x20BF: {
        "name": "Great Bay Coast Rock Wall Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Zora Cape
    0x20B4: {
        "name": "Zora Cape Near Logs Gossip Stone",
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Moon
    # Deku Trial
    0x2109: {
        "name": "Deku Trial Front Right Gossip Stone", # Romani's Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },    
    0x2108: {
        "name": "Deku Trial Front Left Gossip Stone", # Garo's Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },    
    0x210A: {
        "name": "Deku Trial Back Left Gossip Stone", # Circus Leader's Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },   
    0x210B: {
        "name": "Deku Trial Back Right Gossip Stone", # Postman's Hat
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },   
    0x210C: {
        "name": "Deku Trial Heart Piece Gossip Stone", # Couple's Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },   
    # Goron Trial
    0x2111: {
        "name": "Goron Trial 1st Gazebo Right Gossip Stone", # Captain's Hat
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x2110: {
        "name": "Goron Trial 1st Gazebo Left Gossip Stone", # Kamaro's Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },    
    0x210F: {
        "name": "Goron Trial 2nd Gazebo Right Gossip Stone", # Don Gero's Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },   
    0x210E: {
        "name": "Goron Trial 2nd Gazebo Left Gossip Stone", # Gibdo Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    }, 
    0x210D: {
        "name": "Goron Trial Heart Piece Gossip Stone", # Great Fairy's Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Zora Trial
    0x2112: {
        "name": "Zora Trial LLL Gossip Stone", # Stone Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x2113: {
        "name": "Zora Trial LLRL Gossip Stone", # Bremen Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x2114: {
        "name": "Zora Trial RLR Gossip Stone", # Blast Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x2115: {
        "name": "Zora Trial RRL Gossip Stone", # Mask of Scents
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x2116: {
        "name": "Zora Trial RRR Gossip Stone", # Giant's Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    # Link Trial
    0x2103: {
        "name": "Link Trial Dinolfos Gossip Stone", # Mask of Truth
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x2104: {
        "name": "Link Trial Garo Master Gossip Stone", # Kafei's Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x2105: {
        "name": "Link Trial Iron Knuckle Left Gossip Stone", # All-Night Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x2106: {
        "name": "Link Trial Iron Knuckle Right Gossip Stone", # Bunny Hood
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
    0x2107: {
        "name": "Link Trial Heart Piece Gossip Stone", # Keaton Mask
        "item": "",
        "item_type": 0,
        "location": "",
        "location_id": "",
        "from_player": "",
        "to_player": "",
        "region": "",
        "type": HintEnum.NONE.value,
        "extra": {},
        "filled": False,
    },
}

moon_gossip_lookup = {
	# Moon
    # Deku Trial
    0x2109: "Romani's Mask",
    0x2108: "Garo's Mask",
    0x210A: "Circus Leader's Mask",
    0x210B: "Postman's Hat",
    0x210C: "Couple's Mask",
    # Goron Trial
    0x2111: "Captain's Hat",
    0x2110: "Kamaro's Mask",
    0x210F: "Don Gero's Mask",
    0x210F: "Gibdo Mask",
    0x210D: "Great Fairy's Mask",
    # Zora Trial
    0x2112: "Stone Mask",
    0x2113: "Bremen Mask",
    0x2114: "Blast Mask",
    0x2115: "Mask of Scents",
    0x2116: "Giant's Mask",
    # Link Trial
    0x2103: "Mask of Truth",
    0x2104: "Kafei's Mask",
    0x2105: "All-Night Mask",
    0x2106: "Bunny Hood",
    0x2107: "Keaton Mask",
}

# rough grouping for entrance rando
DUNGEON_WOODFALL = 0
DUNGEON_SNOWHEAD = 1
DUNGEON_GREAT_BAY = 2
DUNGEON_STONE_TOWER = 3

dungeon_entrances_er = [
	"Woodfall -> Woodfall Temple",
    "Snowhead -> Snowhead Temple",
    "Zora Cape -> Great Bay Temple",
    "Stone Tower (Inverted) -> Stone Tower Temple (Inverted)",
]

dungeon_bosses_er = [
	"Woodfall Temple -> Odolwa's Lair",
    "Snowhead Temple -> Goht's Lair",
    "Great Bay Temple -> Gyorg's Lair",
    "Stone Tower Temple (Inverted) -> Twinmold's Lair",
]

# extra_entrances_er = [
# 	"Southern Swamp (Deku Palace) -> Swamp Spider House",
# 	"Great Bay -> Ocean Spider House",
# 	"Upper Ikana Canyon -> Beneath the Well",
# ]

# direct ENTRANCE() ids from the game
# note: dungeon exits omit the "dungeon ->" part as they do not have apworld entrances
entrance_to_id_lookup = {
	"Woodfall": 0x8610,
	"Woodfall -> Woodfall Temple": 0x3000,
    "Woodfall Temple -> Odolwa's Lair": 0x3800,
	
	"Snowhead": 0xB210,
	"Snowhead -> Snowhead Temple": 0x3C00,
	"Snowhead Temple -> Goht's Lair": 0x8200,
	
	"Zora Cape": 0x6A70,
    "Zora Cape -> Great Bay Temple": 0x8C00,
	"Great Bay Temple -> Gyorg's Lair": 0xB800,
	
	"Stone Tower (Inverted)": 0xAC10,
    "Stone Tower (Inverted) -> Stone Tower Temple (Inverted)": 0x2A00,
	"Stone Tower Temple (Inverted) -> Twinmold's Lair": 0x6600,
}

# this only works under standard dungeon entrance rando
original_entrance_lookup = {
	"Woodfall": "Woodfall Temple -> Woodfall",
	"Woodfall Temple": "Woodfall -> Woodfall Temple",
    "Odolwa's Lair": "Woodfall Temple -> Odolwa's Lair",
	
    "Snowhead": "Snowhead Temple -> Snowhead",
	"Snowhead Temple": "Snowhead -> Snowhead Temple",
	"Goht's Lair": "Snowhead Temple -> Goht's Lair",
	
	"Zora Cape": "Great Bay Temple -> Zora Cape",
	"Great Bay Temple": "Zora Cape -> Great Bay Temple",
	"Gyorg's Lair": "Great Bay Temple -> Gyorg's Lair",
	
    "Stone Tower (Inverted)": "Stone Tower Temple (Inverted) -> Stone Tower (Inverted)",
	"Stone Tower Temple (Inverted)": "Stone Tower (Inverted) -> Stone Tower Temple (Inverted)",
	"Twinmold's Lair": "Stone Tower Temple (Inverted) -> Twinmold's Lair",
}

dungeon_entrances = [
	"Woodfall",
	"Snowhead",
	"Zora Cape",
	"Stone Tower (Inverted)",
]

mm_dungeons = [
	"Woodfall Temple",
	"Snowhead Temple",
	"Great Bay Temple",
	"Stone Tower Temple (Inverted)",
]

mm_bosses = [
	"Odolwa's Lair",
	"Goht's Lair",
	"Gyorg's Lair",
	"Twinmold's Lair",
]

GRASS_OFF = 0
GRASS_NORMAL = 1
GRASS_NO_TERMINA_FIELD = 2
GRASS_GROTTO_AND_CAVE_ONLY = 3
GRASS_DUNGEON_ONLY = 4

POT_OFF = 0
POT_ALL = 1
POT_OVERWORLD_ONLY = 2
POT_DUNGEON_ONLY = 3
