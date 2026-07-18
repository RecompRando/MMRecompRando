from typing import Callable, Dict, NamedTuple, Optional

from BaseClasses import Location, MultiWorld

from .Constants import (
    GRASS_OFF, GRASS_NORMAL, GRASS_NO_TERMINA_FIELD,
    GRASS_GROTTO_AND_CAVE_ONLY, GRASS_DUNGEON_ONLY,
    POT_OFF, POT_ALL, POT_OVERWORLD_ONLY, POT_DUNGEON_ONLY,
)


class MMRLocation(Location):
    game = "Majora's Mask Recompiled"


class MMRLocationData(NamedTuple):
    region: str
    address: Optional[int] = None
    can_create: Callable = lambda options: True
    locked_item: Optional[str] = None


def can_create_heart_location(shp, c_or_p, loc_index):
    if c_or_p == 0:
        starting_containers = int(shp/4) - 1
        starting_pieces = shp % 4
        shuffled_containers = int((12 - shp)/4)
        shuffled_pieces = (12 - shp) % 4
        return starting_containers + starting_pieces + shuffled_containers + shuffled_pieces >= loc_index
    else:
        return True


def grass_enabled(options, is_termina_field=False, is_grotto_or_cave=False, is_dungeon=False):
    mode = options.grasssanity.value
    if mode == GRASS_OFF:
        return False
    if mode == GRASS_NORMAL:
        return True
    if mode == GRASS_NO_TERMINA_FIELD:
        return not is_termina_field
    if mode == GRASS_GROTTO_AND_CAVE_ONLY:
        return is_grotto_or_cave
    if mode == GRASS_DUNGEON_ONLY:
        return is_dungeon
    return False


POT_DUNGEON_REGIONS = {
    "Woodfall Temple", "Snowhead Temple", "Great Bay Temple",
    "Stone Tower Temple", "Stone Tower Temple (Inverted)",
    "Odolwa's Lair", "Goht's Lair", "Gyorg's Lair", "Twinmold's Lair",
    "Beneath the Well",
    "Swamp Spider House", "Ocean Spider House",
    "Pirates' Fortress (Interior)", "Pirates' Fortress Sewers",
    "Ikana Castle",
    "Secret Shrine", "The Moon",
}


def pot_enabled(options, is_dungeon=False):
    mode = options.potsanity.value
    if mode == POT_OFF:
        return False
    if mode == POT_ALL:
        return True
    if mode == POT_OVERWORLD_ONLY:
        return not is_dungeon
    if mode == POT_DUNGEON_ONLY:
        return is_dungeon
    return False

location_data_table: Dict[str, MMRLocationData] = {
    "Link's Inventory (Ocarina of Time)": MMRLocationData(
        region="Clock Town",
        address=0x0D004C
    ),
    "Link's Inventory (Song of Time)": MMRLocationData(
        region="Clock Town",
        address=0x0D0067
    ),    
    "Link's Inventory (Kokiri Sword)": MMRLocationData(
        region="Clock Town",
        address=0x000037
    ),
    "Link's Inventory (Hero's Shield)": MMRLocationData(
        region="Clock Town",
        address=0x000032
    ),
    "Link's Inventory (Heart Item #1)": MMRLocationData(
        region="Clock Town",
        address=0x0D0000,
        can_create=lambda options: can_create_heart_location(options.starting_hearts.value, options.starting_hearts_are_containers_or_pieces.value, 1)
    ),
    "Link's Inventory (Heart Item #2)": MMRLocationData(
        region="Clock Town",
        address=0x0D0001,
        can_create=lambda options: can_create_heart_location(options.starting_hearts.value, options.starting_hearts_are_containers_or_pieces.value, 2)
    ),
    "Link's Inventory (Heart Item #3)": MMRLocationData(
        region="Clock Town",
        address=0x0D0002,
        can_create=lambda options: can_create_heart_location(options.starting_hearts.value, options.starting_hearts_are_containers_or_pieces.value, 3)
    ),
    "Link's Inventory (Heart Item #4)": MMRLocationData(
        region="Clock Town",
        address=0x0D0003,
        can_create=lambda options: can_create_heart_location(options.starting_hearts.value, options.starting_hearts_are_containers_or_pieces.value, 4)
    ),
    "Link's Inventory (Heart Item #5)": MMRLocationData(
        region="Clock Town",
        address=0x0D0004,
        can_create=lambda options: can_create_heart_location(options.starting_hearts.value, options.starting_hearts_are_containers_or_pieces.value, 5)
    ),
    "Link's Inventory (Heart Item #6)": MMRLocationData(
        region="Clock Town",
        address=0x0D0005,
        can_create=lambda options: can_create_heart_location(options.starting_hearts.value, options.starting_hearts_are_containers_or_pieces.value, 6)
    ),
    "Link's Inventory (Heart Item #7)": MMRLocationData(
        region="Clock Town",
        address=0x0D0006,
        can_create=lambda options: can_create_heart_location(options.starting_hearts.value, options.starting_hearts_are_containers_or_pieces.value, 7)
    ),
    "Link's Inventory (Heart Item #8)": MMRLocationData(
        region="Clock Town",
        address=0x0D0007,
        can_create=lambda options: can_create_heart_location(options.starting_hearts.value, options.starting_hearts_are_containers_or_pieces.value, 8)
    ),
    "Keaton Quiz": MMRLocationData(
        region="Clock Town",
        address=0x07028C
    ),
    "Clock Tower Happy Mask Salesman #1": MMRLocationData(
        region="Clock Town",
        address=0x040068
    ),
    "Clock Tower Happy Mask Salesman #2": MMRLocationData(
        region="Clock Town",
        address=0x000078
    ),
    "Before Clock Town Chest": MMRLocationData(
        region="Clock Town",
        address=0x061A00,
        can_create=lambda options: options.intro_checks.value
    ),
    "Clock Town Postbox": MMRLocationData(
        region="Clock Town",
        address=0x0701F2
    ),
    "Clock Town Hide-and-Seek": MMRLocationData(
        region="Clock Town",
        address=0x000050
    ),
    "Clock Town Trading Post Shop Item 1": MMRLocationData(
        region="Clock Town",
        address=0x09000A,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Trading Post Shop Item 2": MMRLocationData(
        region="Clock Town",
        address=0x090005,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Trading Post Shop Item 3": MMRLocationData(
        region="Clock Town",
        address=0x090006,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Trading Post Shop Item 4": MMRLocationData(
        region="Clock Town",
        address=0x090003,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Trading Post Shop Item 5": MMRLocationData(
        region="Clock Town",
        address=0x090007,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Trading Post Shop Item 6": MMRLocationData(
        region="Clock Town",
        address=0x090008,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Trading Post Shop Item 7": MMRLocationData(
        region="Clock Town",
        address=0x090009,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Trading Post Shop Item 8": MMRLocationData(
        region="Clock Town",
        address=0x090004,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Trading Post Shop (Night) Item 1": MMRLocationData(
        region="Clock Town",
        address=0x090012,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Clock Town Trading Post Shop (Night) Item 2": MMRLocationData(
        region="Clock Town",
        address=0x09000E,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Clock Town Trading Post Shop (Night) Item 3": MMRLocationData(
        region="Clock Town",
        address=0x090011,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Clock Town Trading Post Shop (Night) Item 4": MMRLocationData(
        region="Clock Town",
        address=0x09000B,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Clock Town Trading Post Shop (Night) Item 5": MMRLocationData(
        region="Clock Town",
        address=0x090010,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Clock Town Trading Post Shop (Night) Item 6": MMRLocationData(
        region="Clock Town",
        address=0x09000C,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Clock Town Trading Post Shop (Night) Item 7": MMRLocationData(
        region="Clock Town",
        address=0x09000F,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Clock Town Trading Post Shop (Night) Item 8": MMRLocationData(
        region="Clock Town",
        address=0x09000D,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Clock Town Bomb Shop Item 1": MMRLocationData(
        region="Clock Town",
        address=0x09001A,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Bomb Shop Item 2": MMRLocationData(
        region="Clock Town",
        address=0x090019,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Bomb Shop Item 3": MMRLocationData(
        region="Clock Town",
        address=0x090017,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Bomb Shop Item 3 (Stop Thief)": MMRLocationData(
        region="Clock Town",
        address=0x090018,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Clock Town Bomb Shop Powder Keg Goron": MMRLocationData(
        region="Clock Town",
        address=0x024234,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Curiosity Shop Blue Rupee Trade": MMRLocationData(
        region="Clock Town",
        address=0x07C402,
        can_create=lambda options: options.curiostity_shop_trades.value
    ),
    "Curiosity Shop Red Rupee Trade": MMRLocationData(
        region="Clock Town",
        address=0x07C404,
        can_create=lambda options: options.curiostity_shop_trades.value
    ),
    "Curiosity Shop Purple Rupee Trade": MMRLocationData(
        region="Clock Town",
        address=0x07C405,
        can_create=lambda options: options.curiostity_shop_trades.value
    ),
    "Curiosity Shop Gold Rupee Trade": MMRLocationData(
        region="Clock Town",
        address=0x07C407,
        can_create=lambda options: options.curiostity_shop_trades.value
    ),
    "Curiosity Shop Night 3 (Stop Thief)": MMRLocationData(
        region="Clock Town",
        address=0x090013,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Curiosity Shop Night 3 Thief Stolen Item": MMRLocationData(
        region="Clock Town",
        address=0x090015,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Laundry Pool Stray Fairy (Clock Town)": MMRLocationData(
        region="Clock Town",
        address=0x01007F
    ),
    "Laundry Pool Musician": MMRLocationData(
        region="Clock Town",
        address=0x00008C
    ),
    "Laundry Pool Kafei's Request": MMRLocationData(
        region="Clock Town",
        address=0x0000AB
    ),
    "Laundry Pool Curiosity Shop Salesman #1": MMRLocationData(
        region="Clock Town",
        address=0x000080
    ),
    "Laundry Pool Curiosity Shop Salesman #2": MMRLocationData(
        region="Clock Town",
        address=0x0000A1
    ),
    "South Clock Town Moon's Tear Trade": MMRLocationData(
        region="Clock Town",
        address=0x000097
    ),
    "South Clock Town Clock Tower Freestanding HP": MMRLocationData(
        region="Clock Town",
        address=0x056F0A
    ),
    "South Clock Town Corner Chest": MMRLocationData(
        region="Clock Town",
        address=0x066F00
    ),
    "South Clock Town Final Day Tower Chest": MMRLocationData(
        region="Clock Town",
        address=0x066F01
    ),
    "East Clock Town Archery Roof Chest": MMRLocationData(
        region="Clock Town",
        address=0x066C0A
    ),
    "East Clock Town Mayors Wife": MMRLocationData(
        region="Clock Town",
        address=0x00008F
    ),
    "East Clock Town Couples Mask on Mayor": MMRLocationData(
        region="Clock Town",
        address=0x07026F
    ),
    "East Clock Town Shooting Gallery 40-49 Points": MMRLocationData(
        region="Clock Town",
        address=0x000023
    ),
    "East Clock Town Shooting Gallery Perfect 50 Points": MMRLocationData(
        region="Clock Town",
        address=0x07011D
    ),
    "East Clock Town Honey and Darling Any Day": MMRLocationData(
        region="Clock Town",
        address=0x0800B5
    ),
    "East Clock Town Honey and Darling All Days": MMRLocationData(
        region="Clock Town",
        address=0x0700B5
    ),
    "East Clock Town Treasure Game Chest (Human)": MMRLocationData(
        region="Clock Town",
        address=0x061705
    ),
    "East Clock Town Treasure Game Chest (Deku)": MMRLocationData(
        region="Clock Town",
        address=0x06172A
    ),
    "East Clock Town Treasure Game Chest (Goron)": MMRLocationData(
        region="Clock Town",
        address=0x06170C
    ),
    "East Clock Town Treasure Game Chest (Zora)": MMRLocationData(
        region="Clock Town",
        address=0x061704
    ),
    "Bomber's Hideout Chest": MMRLocationData(
        region="Clock Town",
        address=0x062900
    ),
    "Bomber's Hideout Astral Observatory": MMRLocationData(
        region="Clock Town",
        address=0x000096
    ),
    "North Clock Town Tree HP": MMRLocationData(
        region="Clock Town",
        address=0x056E0A
    ),
    "North Clock Town Deku Playground Any Day": MMRLocationData(
        region="Clock Town",
        address=0x0801C9
    ),
    "North Clock Town Deku Playground All Days": MMRLocationData(
        region="Clock Town",
        address=0x0701C9
    ),
    "North Clock Town Save Old Lady": MMRLocationData(
        region="Clock Town",
        address=0x00008D
    ),
    "North Clock Town Great Fairy Reward": MMRLocationData(
        region="Clock Town",
        address=0x030000
    ),
    "North Clock Town Great Fairy Reward (Has Transformation Mask)": MMRLocationData(
        region="Clock Town",
        address=0x000086
    ),
    "West Clock Town Lottery Any Day": MMRLocationData(
        region="Clock Town",
        address=0x080239
    ),
    "West Clock Town Swordsman Expert Course": MMRLocationData(
        region="Clock Town",
        address=0x0701EF
    ),
    "West Clock Town Postman Counting": MMRLocationData(
        region="Clock Town",
        address=0x07017D
    ),
    "West Clock Town Dancing Sisters": MMRLocationData(
        region="Clock Town",
        address=0x07027B
    ),
    "West Clock Town Bank 200 Rupees": MMRLocationData(
        region="Clock Town",
        address=0x000008
    ),
    "West Clock Town Bank 500 Rupees": MMRLocationData(
        region="Clock Town",
        address=0x080177
    ),
    "West Clock Town Bank 1000 Rupees": MMRLocationData(
        region="Clock Town",
        address=0x070177
    ),
    "West Clock Town Priority Mail to Postman": MMRLocationData(
        region="Clock Town",
        address=0x000084
    ),
    "Top of Clock Tower (Ocarina of Time)": MMRLocationData(
        region="Clock Town",
        address=0x00004C
    ),
    "Top of Clock Tower (Song of Time)": MMRLocationData(
        region="Clock Town",
        address=0x040067
    ),
    "Stock Pot Inn Reservation": MMRLocationData(
        region="Clock Town",
        address=0x0000A0
    ),
    "Stock Pot Inn Midnight Meeting": MMRLocationData(
        region="Clock Town",
        address=0x0000AA
    ),
    "Stock Pot Inn Locked Room Chest": MMRLocationData(
        region="Clock Town",
        address=0x066100
    ),
    "Stock Pot Inn Employee Room Chest": MMRLocationData(
        region="Clock Town",
        address=0x066101
    ),
    "Stock Pot Inn Midnight Toilet Hand": MMRLocationData(
        region="Clock Town",
        address=0x07027D
    ),
    "Stock Pot Inn Granny Story #1": MMRLocationData(
        region="Clock Town",
        address=0x070243
    ),
    "Stock Pot Inn Granny Story #2": MMRLocationData(
        region="Clock Town",
        address=0x080243
    ),
    "Stock Pot Inn Anju and Kafei": MMRLocationData(
        region="Clock Town",
        address=0x000085
    ),
    "Milk Bar Show": MMRLocationData(
        region="Clock Town",
        address=0x000083
    ),
    "Milk Bar Priority Mail to Aroma": MMRLocationData(
        region="Clock Town",
        address=0x00006F
    ),
    "East Clock Town Milk Bar Milk Purchase": MMRLocationData(
        region="Clock Town",
        address=0x026392,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "East Clock Town Milk Bar Chateau Romani Purchase": MMRLocationData(
        region="Clock Town",
        address=0x000091,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Tingle Clock Town Map Purchase": MMRLocationData(
        region="Clock Town",
        address=0x0000B4
    ),
    "Tingle Woodfall Map Purchase": MMRLocationData(
        region="Clock Town",
        address=0x0000B5
    ),
    "Tingle Snowhead Map Purchase": MMRLocationData(
        region="Clock Town",
        address=0x0000B6
    ),
    "Tingle Romani Ranch Map Purchase": MMRLocationData(
        region="Clock Town",
        address=0x0000B7
    ),
    "Tingle Great Bay Map Purchase": MMRLocationData(
        region="Clock Town",
        address=0x0000B8
    ),
    "Tingle Stone Tower Map Purchase": MMRLocationData(
        region="Clock Town",
        address=0x0000B9
    ),
    "Termina Stump Chest": MMRLocationData(
        region="Termina Field",
        address=0x062D02
    ),
    "Termina Grass Chest": MMRLocationData(
        region="Termina Field",
        address=0x062D01
    ),
    "Termina Underwater Chest": MMRLocationData(
        region="Termina Field",
        address=0x062D00
    ),
    "Termina Grass Grotto Chest": MMRLocationData(
        region="Termina Field",
        address=0x06071F
    ),
    "Termina Peehat Grotto Chest": MMRLocationData(
        region="Termina Field",
        address=0x060704
    ),
    "Termina Dodongo Grotto Chest": MMRLocationData(
        region="Termina Field",
        address=0x060700
    ),
    "Termina Log Bombable Grotto Left Cow": MMRLocationData(
        region="Termina Field",
        address=0xBEEF14,
        can_create=lambda options: options.cowsanity.value
    ),
    "Termina Log Bombable Grotto Right Cow": MMRLocationData(
        region="Termina Field",
        address=0xBEEF13,
        can_create=lambda options: options.cowsanity.value
    ),
    "Termina Ikana Pillar Grotto Chest": MMRLocationData(
        region="Termina Field",
        address=0x06071A
    ),
    "Termina Healing Kamaro": MMRLocationData(
        region="Termina Field",
        address=0x000089
    ),
    "Termina Bio Baba Grotto HP": MMRLocationData(
        region="Termina Field",
        address=0x050702
    ),
    "Termina Gossip Stones HP": MMRLocationData(
        region="Termina Field",
        address=0x0700EF
    ),
    "Termina Scrub Grotto HP": MMRLocationData(
        region="Termina Field",
        address=0x07024C
    ),
    "Road to Swamp Tree HP": MMRLocationData(
        region="Southern Swamp",
        address=0x054001
    ),
    "Road to Swamp Grotto Chest": MMRLocationData(
        region="Southern Swamp",
        address=0x06071E
    ),
    "Swamp Shooting Gallery 2120 Points": MMRLocationData(
        region="Southern Swamp",
        address=0x000024
    ),
    "Swamp Shooting Gallery 2180 Points": MMRLocationData(
        region="Southern Swamp",
        address=0x08011D
    ),
    "Southern Swamp Deku Trade": MMRLocationData(
        region="Southern Swamp",
        address=0x000098
    ),    
    "Southern Swamp Deku Scrub Purchase": MMRLocationData(
        region="Southern Swamp",
        address=0x090135,
        can_create=lambda options: options.scrubsanity.value
    ),
    "Southern Swamp Freestanding HP": MMRLocationData(
        region="Southern Swamp",
        address=0x05451E
    ),
    "Southern Swamp Kotake Item": MMRLocationData(
        region="Southern Swamp",
        address=0x000059
    ),
    "Southern Swamp Day 2 Grotto Chest": MMRLocationData(
        region="Southern Swamp",
        address=0x06071C
    ),
    "Southern Swamp Healing Koume": MMRLocationData(
        region="Southern Swamp",
        address=0x000043
    ),
    "Southern Swamp Winning Picture": MMRLocationData(
        region="Southern Swamp",
        address=0x0701C5
    ),
    "Southern Swamp Good Picture": MMRLocationData(
        region="Southern Swamp",
        address=0x071C54
    ),
    "Southern Swamp Okay Picture": MMRLocationData(
        region="Southern Swamp",
        address=0x071C52
    ),
    "Southern Swamp Witch Shop Mushroom Item": MMRLocationData(
        region="Southern Swamp",
        address=0x00005D
    ),
    "Southern Swamp Witch Shop Item 1": MMRLocationData(
        region="Southern Swamp",
        address=0x090002,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Southern Swamp Witch Shop Item 2": MMRLocationData(
        region="Southern Swamp",
        address=0x090001,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Southern Swamp Witch Shop Item 3": MMRLocationData(
        region="Southern Swamp",
        address=0x090000,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Swamp Spider House First Room Pot Near Entrance Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06271E,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House First Room Crawling In Water Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062708,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House First Room Crawling Right Column Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06270F,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House First Room Crawling Left Column Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062713,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House First Room Against Far Wall Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062700,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House First Room Lower Left Bugpatch Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062709,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House First Room Lower Right Bugpatch Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06270C,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House First Room Upper Right Bugpatch Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06270B,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Monument Room Left Crate Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06270A,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Monument Room Right Crate Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06271B,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Monument Room Crawling Wall Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06270D,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Monument Room Crawling On Monument Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06270E,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Monument Room Behind Torch Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062702,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Pottery Room Beehive #1 Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062717,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Pottery Room Beehive #2 Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06271C,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Pottery Room Small Pot Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062705,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Pottery Room Left Large Pot Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062710,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Pottery Room Right Large Pot Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062711,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Pottery Room Behind Vines Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062714,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Pottery Room Upper Wall Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062716,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Golden Room Crawling Left Wall Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062719,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Golden Room Crawling Right Column Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062704,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Golden Room Against Far Wall Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062701,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Golden Room Beehive Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062712,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Tree Room Tall Grass #1 Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062707,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Tree Room Tall Grass #2 Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062706,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Tree Room Tree #1 Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062715,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Tree Room Tree #2 Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x062718,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Tree Room Tree #3 Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06271D,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Tree Room Beehive Token": MMRLocationData(
        region="Swamp Spider House",
        address=0x06271A,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Swamp Spider House Reward": MMRLocationData(
        region="Swamp Spider House",
        address=0x00008A
    ),
    "Southern Swamp Grotto Chest": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x06071D
    ),
    "Southern Swamp Song Tablet": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x04006A
    ),
    "Deku Palace HP": MMRLocationData(
        region="Deku Palace",
        address=0x052B1E
    ),
    "Deku Palace Bean Seller": MMRLocationData(
        region="Deku Palace",
        address=0x0800A5
    ),
    "Deku Palace Bean Grotto Chest": MMRLocationData(
        region="Deku Palace",
        address=0x060705
    ),
    "Deku Palace Monkey Song": MMRLocationData(
        region="Deku Palace",
        address=0x040061
    ),
    "Deku Palace Butler Race": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x00008E
    ),
    "Woodfall Owl Statue Chest": MMRLocationData(
        region="Woodfall",
        address=0x064602
    ),
    "Woodfall Bridge Chest": MMRLocationData(
        region="Woodfall",
        address=0x064601
    ),
    "Woodfall Entrance Chest": MMRLocationData(
        region="Woodfall",
        address=0x064600
    ),
    "Woodfall Great Fairy Reward": MMRLocationData(
        region="Woodfall",
        address=0x030001
    ),
    "Woodfall Temple Entrance Chest SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x061B18
    ),
    "Woodfall Temple Ledge Chest": MMRLocationData(
        region="Woodfall Temple",
        address=0x061B01
    ),
    "Woodfall Temple Turtle Chest": MMRLocationData(
        region="Woodfall Temple",
        address=0x061B1D
    ),
    "Woodfall Temple Dragonfly Chest": MMRLocationData(
        region="Woodfall Temple",
        address=0x061B1C
    ),
    "Woodfall Temple Dark Room Chest SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x061B19
    ),
    "Woodfall Temple Switch Chest SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x061B17
    ),
    "Woodfall Temple Dinolfos Chest": MMRLocationData(
        region="Woodfall Temple",
        address=0x061B1B
    ),
    "Woodfall Temple Gekko Chest": MMRLocationData(
        region="Woodfall Temple",
        address=0x061B1E
    ),
    "Woodfall Temple Entrance Freestanding SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B2B
    ),
    "Woodfall Temple Deku Baba SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B2E
    ),
    "Woodfall Temple Pot SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B1C
    ),
    "Woodfall Temple Platform Hive SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B1E
    ),
    "Woodfall Temple Main Room Bubble SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B30
    ),
    "Woodfall Temple Skulltula SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B31
    ),
    "Woodfall Temple Bridge Room Bubble SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B2F
    ),
    "Woodfall Temple Bridge Room Hive SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B1D
    ),
    "Woodfall Temple Pre-Boss Lower Right Bubble SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B2A
    ),
    "Woodfall Temple Pre-Boss Upper Right Bubble SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B32
    ),
    "Woodfall Temple Pre-Boss Upper Left Bubble SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B2C
    ),
    "Woodfall Temple Pre-Boss Pillar Bubble SF": MMRLocationData(
        region="Woodfall Temple",
        address=0x011B2D
    ),
    "Woodfall Temple Heart Container": MMRLocationData(
        region="Odolwa's Lair",
        address=0x051F00
    ),
    "Woodfall Temple Odolwa's Remains": MMRLocationData(
        region="Odolwa's Lair",
        address=0x000055
    ),
    "Southern Swamp Boat Archery": MMRLocationData(
        region="Southern Swamp",
        address=0x070168
    ),
    "Mountain Village Spring Waterfall Chest": MMRLocationData(
        region="Mountain Village",
        address=0x065A00
    ),
    "Mountain Village Spring Ramp Grotto": MMRLocationData(
        region="Mountain Village",
        address=0x06071B
    ),
    "Mountain Village Healing Darmani": MMRLocationData(
        region="Mountain Village",
        address=0x000079
    ),
    "Mountain Village Hungry Goron": MMRLocationData(
        region="Mountain Village",
        address=0x000088
    ),
    "Mountain Village Smithy Upgrade": MMRLocationData(
        region="Mountain Village",
        address=0x000038
    ),
    "Mountain Village Smithy Gold Dust Upgrade": MMRLocationData(
        region="Mountain Village",
        address=0x000039
    ),
    "Mountain Village Spring Frog Choir HP": MMRLocationData(
        region="Mountain Village",
        address=0x070022
    ),
    "Twin Islands Spring Underwater Cave Chest": MMRLocationData(
        region="Twin Islands",
        address=0x065E00
    ),
    "Twin Islands Spring Underwater Ramp Chest": MMRLocationData(
        region="Twin Islands",
        address=0x065E06
    ),
    "Twin Islands Ramp Grotto Chest": MMRLocationData(
        region="Twin Islands",
        address=0x060719
    ),
    "Twin Islands Goron Elder Request": MMRLocationData(
        region="Twin Islands",
        address=0x0001AD
    ),
    "Twin Islands Hot Water Grotto Chest": MMRLocationData(
        region="Twin Islands",
        address=0x060702
    ),
    "Goron Racetrack Prize": MMRLocationData(
        region="Twin Islands",
        address=0x00006A
    ),
    "Goron Village Lens Cave Rock Chest": MMRLocationData(
        region="Goron Village",
        address=0x060706
    ),
    "Goron Village Lens Cave Invisible Chest": MMRLocationData(
        region="Goron Village",
        address=0x060703
    ),
    "Goron Village Lens Cave Center Chest": MMRLocationData(
        region="Goron Village",
        address=0x060701
    ),
    "Goron Village Baby Goron Lullaby": MMRLocationData(
        region="Goron Village",
        address=0x0000AD
    ),
    "Goron Village Shop Item 1": MMRLocationData(
        region="Goron Village",
        address=0x09001E,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Goron Village Shop Item 2": MMRLocationData(
        region="Goron Village",
        address=0x09001F,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Goron Village Shop Item 3": MMRLocationData(
        region="Goron Village",
        address=0x090020,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Goron Village Shop (Spring) Item 1": MMRLocationData(
        region="Goron Village",
        address=0x090021,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Goron Village Shop (Spring) Item 2": MMRLocationData(
        region="Goron Village",
        address=0x090022,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Goron Village Shop (Spring) Item 3": MMRLocationData(
        region="Goron Village",
        address=0x090023,
        can_create=lambda options: options.shopsanity.value == 2
    ),    
    "Goron Village Scrub Purchase": MMRLocationData(
        region="Goron Village",
        address=0x09011D,
        can_create=lambda options: options.scrubsanity.value
    ),
    "Goron Village Deku Trade": MMRLocationData(
        region="Goron Village",
        address=0x000099
    ),
    "Goron Village Freestanding HP": MMRLocationData(
        region="Goron Village",
        address=0x054D1E
    ),
    "Goron Village Freestanding HP (Spring)": MMRLocationData(
        region="Goron Village",
        address=0x05481E,
        can_create=lambda options: options.shopsanity.value == 2
    ),
    "Powder Keg Goron Reward": MMRLocationData(
        region="Goron Village",
        address=0x000034
    ),
    "Path to Snowhead Grotto Chest": MMRLocationData(
        region="Path to Snowhead",
        address=0x060713
    ),
    "Path to Snowhead Scarecrow Pillar HP": MMRLocationData(
        region="Path to Snowhead",
        address=0x055B08
    ),
    "Snowhead Great Fairy Reward": MMRLocationData(
        region="Snowhead Temple",
        address=0x030002
    ),
    "Snowhead Temple Elevator Room Invisible Platform Chest SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x062113
    ),
    "Snowhead Temple Lower Wizzrobe Chest": MMRLocationData(
        region="Snowhead Temple",
        address=0x06211B
    ),
    "Snowhead Temple Bridge Room Under Platform Bubble SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x01212F
    ),
    "Snowhead Temple Bridge Room Pillar Bubble SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x012130
    ),
    "Snowhead Temple Elevator Freestanding SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x012132
    ),
    "Snowhead Temple Bombable Stairs Crate SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x01211E
    ),
    "Snowhead Temple Timed Switch Room Bubble SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x01212C
    ),
    "Snowhead Temple Snowmen Bubble SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x01212B
    ),
    "Snowhead Temple Dinolfos Room First SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x012131
    ),
    "Snowhead Temple Dinolfos Room Second SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x01212D
    ),
    "Snowhead Temple Bridge Room Freezard Chest": MMRLocationData(
        region="Snowhead Temple",
        address=0x062101
    ),
    "Snowhead Temple Elevator Room Lower Chest": MMRLocationData(
        region="Snowhead Temple",
        address=0x06211D
    ),
    "Snowhead Temple Basement Switch Chest SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x062114
    ),
    "Snowhead Temple Freezard Torch Room Chest SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x062118
    ),
    "Snowhead Temple Behind Stacked Block Chest": MMRLocationData(
        region="Snowhead Temple",
        address=0x062103
    ),
    "Snowhead Temple Stacked Block Upper Chest SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x062115
    ),
    "Snowhead Temple Frozen Block Chest": MMRLocationData(
        region="Snowhead Temple",
        address=0x06211C
    ),
    "Snowhead Temple Frozen Block Upper Chest SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x062119
    ),
    "Snowhead Temple Icicle Room Hidden Chest SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x062116
    ),
    "Snowhead Temple Icicle Room Snowball Chest": MMRLocationData(
        region="Snowhead Temple",
        address=0x062104
    ),
    "Snowhead Temple Upper Wizzrobe Chest": MMRLocationData(
        region="Snowhead Temple",
        address=0x06211E
    ),
    "Snowhead Temple Main Room Wall Chest SF": MMRLocationData(
        region="Snowhead Temple",
        address=0x062117
    ),
    "Snowhead Temple Heart Container": MMRLocationData(
        region="Goht's Lair",
        address=0x054400
    ),
    "Snowhead Temple Goht's Remains": MMRLocationData(
        region="Goht's Lair",
        address=0x000056
    ),
    "Milk Road Gorman Ranch Race": MMRLocationData(
        region="Gorman Brothers Track",
        address=0x000081
    ),
    "Milk Road Gorman Ranch Purchase": MMRLocationData(
        region="Gorman Brothers Track",
        address=0x006792,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Romani Ranch Baby Cuccos March": MMRLocationData(
        region="Romani Ranch",
        address=0x00007F
    ),
    "Romani Ranch Doggy Racetrack Rooftop Chest": MMRLocationData(
        region="Romani Ranch",
        address=0x064100
    ),
    "Romani Ranch Doggy Race": MMRLocationData(
        region="Romani Ranch",
        address=0x070117
    ),
    "Romani Ranch Barn Free Cow": MMRLocationData(
        region="Romani Ranch",
        address=0xBEEF10,
        can_create=lambda options: options.cowsanity.value
    ),
    "Romani Ranch Barn Stables Front Cow": MMRLocationData(
        region="Romani Ranch",
        address=0xBEEF11,
        can_create=lambda options: options.cowsanity.value
    ),
    "Romani Ranch Barn Stables Back Cow": MMRLocationData(
        region="Romani Ranch",
        address=0xBEEF12,
        can_create=lambda options: options.cowsanity.value
    ),
    "Romani Ranch Romani Game": MMRLocationData(
        region="Romani Ranch",
        address=0x0000A5
    ),
    "Romani Ranch Aliens": MMRLocationData(
        region="Romani Ranch",
        address=0x000060
    ),
    "Romani Ranch Helping Cremia": MMRLocationData(
        region="Romani Ranch",
        address=0x000082
    ),
    "Great Bay Healing Zora": MMRLocationData(
        region="Great Bay",
        address=0x00007A
    ),
    "Great Bay Fisherman's Grotto Chest": MMRLocationData(
        region="Great Bay",
        address=0x060717
    ),
    "Great Bay Baby Zora Song": MMRLocationData(
        region="Great Bay",
        address=0x0000AC
    ),
    "Great Bay Feeding Lab Fish": MMRLocationData(
        region="Great Bay",
        address=0x0701D9
    ),
    "Great Bay Ledge Grotto Left Cow": MMRLocationData(
        region="Great Bay",
        address=0xBEEF16,
        can_create=lambda options: options.cowsanity.value
    ),
    "Great Bay Ledge Grotto Right Cow": MMRLocationData(
        region="Great Bay",
        address=0xBEEF15,
        can_create=lambda options: options.cowsanity.value
    ),
    "Great Bay Scarecrow Ledge HP": MMRLocationData(
        region="Great Bay",
        address=0x053705
    ),
    "Great Bay Fisherman Game": MMRLocationData(
        region="Great Bay",
        address=0x070292
    ),
    "Zora Cape Underwater Like-Like HP": MMRLocationData(
        region="Zora Cape",
        address=0x053807
    ),
    "Zora Cape Underwater Chest": MMRLocationData(
        region="Zora Cape",
        address=0x063800
    ),
    "Zora Cape Pot Game": MMRLocationData(
        region="Zora Cape",
        address=0x072806
    ),
    "Zora Cape Deku Flower Chest": MMRLocationData(
        region="Zora Cape",
        address=0x063801
    ),
    "Zora Cape Scarecrow Chest": MMRLocationData(
        region="Zora Cape",
        address=0x063802
    ),
    "Zora Cape Grotto Chest": MMRLocationData(
        region="Zora Cape",
        address=0x060715
    ),
    "Beaver Bros. Race 1": MMRLocationData(
        region="Zora Cape",
        address=0x09018D
    ),
    "Beaver Bros. Race 2 HP": MMRLocationData(
        region="Zora Cape",
        address=0x07018D
    ),
    "Great Bay Great Fairy Reward": MMRLocationData(
        region="Zora Cape",
        address=0x030003
    ),
    "Zora Hall Shop Item 1": MMRLocationData(
        region="Zora Hall",
        address=0x09001B,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Zora Hall Shop Item 2": MMRLocationData(
        region="Zora Hall",
        address=0x09001C,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Zora Hall Shop Item 3": MMRLocationData(
        region="Zora Hall",
        address=0x09001D,
        can_create=lambda options: options.shopsanity.value != 0
    ),
    "Zora Hall Deku Scrub Purchase": MMRLocationData(
        region="Zora Hall",
        address=0x09015C,
        can_create=lambda options: options.scrubsanity.value
    ),
    "Zora Hall Goron Scrub Trade": MMRLocationData(
        region="Zora Hall",
        address=0x00009A
    ),
    "Zora Hall Goron Scrub Trade Freestanding HP": MMRLocationData(
        region="Zora Hall",
        address=0x054C1E
    ),
    "Zora Hall Evan's Song": MMRLocationData(
        region="Zora Hall",
        address=0x070241
    ),
    "Zora Hall Torches Reward": MMRLocationData(
        region="Zora Hall",
        address=0x072802
    ),
    "Zora Hall Good Picture of Lulu": MMRLocationData(
        region="Zora Hall",
        address=0x082284
    ),
    "Zora Hall Bad Picture of Lulu": MMRLocationData(
        region="Zora Hall",
        address=0x082282
    ),
    "Pirates' Fortress Sewers Cage HP": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x05230C
    ),
    "Pirates' Fortress Sewers Maze Chest": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x062301
    ),
    "Pirates' Fortress Sewers Underwater Lower Chest": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x062306
    ),
    "Pirates' Fortress Sewers Underwater Upper Chest": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x062304
    ),
    "Pirates' Fortress Exterior Underwater Log Chest": MMRLocationData(
        region="Pirates' Fortress",
        address=0x063B00
    ),
    "Pirates' Fortress Exterior Underwater Near Entrance Chest": MMRLocationData(
        region="Pirates' Fortress",
        address=0x063B01
    ),
    "Pirates' Fortress Exterior Underwater Corner Chest": MMRLocationData(
        region="Pirates' Fortress",
        address=0x063B02
    ),
    "Pirates' Fortress Interior Tank Chest": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x062300
    ),
    "Pirates' Fortress Interior Guarded Chest": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x062303
    ),
    "Pirates' Fortress Hub Lower Chest": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x061400
    ),
    "Pirates' Fortress Hub Upper Chest": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x061401
    ),
    "Pirates' Fortress Leader's Room Chest": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x062302
    ),
    "Pinnacle Rock Upper Eel Chest": MMRLocationData(
        region="Pinnacle Rock",
        address=0x062502
    ),
    "Pinnacle Rock Lower Eel Chest": MMRLocationData(
        region="Pinnacle Rock",
        address=0x062501
    ),
    "Pinnacle Rock Seahorse HP": MMRLocationData(
        region="Pinnacle Rock",
        address=0x070205
    ),
    "Ocean Spider House Ramp Upper Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06280C,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Ramp Lower Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06280D,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Lobby Ceiling Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06280F,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House First Room Rafter Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062806,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House First Room Open Pot #1 Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062818,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House First Room Open Pot #2 Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062817,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House First Room Wall Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06281D,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Library Top Bookcase Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062804,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Library Passage Behind Bookcase Front Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06281C,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Library Passage Behind Bookcase Rear Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062815,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Library Painting #1 Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062814,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Library Painting #2 Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062802,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Library Rafter Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062808,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Library Bookshelf Hole Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062803,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House First Room Downstairs Rafter Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062805,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House First Room Downstairs Open Pot Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06281B,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House First Room Downstairs Behind Staircase Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06281E,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House First Room Downstairs Crate Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06280B,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House First Room Downstairs Wall Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06280E,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Dining Room Open Pot Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062819,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Dining Room Painting Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062813,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Dining Room Ceiling Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062807,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Dining Room Chandelier #1 Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062810,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Dining Room Chandelier #2 Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062811,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Dining Room Chandelier #3 Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062812,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Storage Room Web Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062809,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Storage Room North Wall Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062801,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Storage Room Crate Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x062816,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Storage Room Hidden Hole Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06280A,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Storage Room Ceiling Pot Token": MMRLocationData(
        region="Ocean Spider House",
        address=0x06281A,
        can_create=lambda options: options.skullsanity.value != 2
    ),
    "Ocean Spider House Coloured Mask Sequence HP": MMRLocationData(
        region="Ocean Spider House",
        address=0x062800
    ),
    "Ocean Spider House Reward": MMRLocationData(
        region="Ocean Spider House",
        address=0x000009
    ),
    "Great Bay Temple Blender Pot SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x01491B
    ),
    "Great Bay Temple Waterwheel Room Skulltula SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x014932
    ),
    "Great Bay Temple Waterwheel Room Bubble SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x014930
    ),
    "Great Bay Temple Blender Room Barrel SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x01491C
    ),
    "Great Bay Temple Before Red Valve Room Pot SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x01491E
    ),
    "Great Bay Temple Before Gekko Room Pot SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x01491D
    ),
    "Great Bay Temple Seesaw Room Underwater Barrel SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x01491A
    ),
    "Great Bay Temple Entrance Torches Chest SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x064918
    ),
    "Great Bay Temple Behind Locked Door Chest": MMRLocationData(
        region="Great Bay Temple",
        address=0x06491B
    ),
    "Great Bay Temple Before Red Valve Room Chest": MMRLocationData(
        region="Great Bay Temple",
        address=0x06491D
    ),
    "Great Bay Temple Bio-Baba Hall Chest SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x064919
    ),
    "Great Bay Temple Before Gekko Room Upper Chest": MMRLocationData(
        region="Great Bay Temple",
        address=0x06491C
    ),
    "Great Bay Temple Before Gekko Room Underwater Chest": MMRLocationData(
        region="Great Bay Temple",
        address=0x064901
    ),
    "Great Bay Temple Mad Jellied Gekko Chest": MMRLocationData(
        region="Great Bay Temple",
        address=0x06491E
    ),
    "Great Bay Temple Room Behind Waterfall Ceiling Chest SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x064915
    ),
    "Great Bay Temple Freezable Waterwheel Upper Chest SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x064914
    ),
    "Great Bay Temple Freezable Waterwheel Lower Chest SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x064917
    ),
    "Great Bay Temple Seesaw Room Chest SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x064916
    ),
    "Great Bay Temple Pre-Boss Room Platform Bubble SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x014931
    ),
    "Great Bay Temple Pre-Boss Room Tunnel Bubble SF": MMRLocationData(
        region="Great Bay Temple",
        address=0x01492F
    ),
    "Great Bay Temple Heart Container": MMRLocationData(
        region="Gyorg's Lair",
        address=0x055F00
    ),
    "Great Bay Temple Gyorg's Remains": MMRLocationData(
        region="Gyorg's Lair",
        address=0x000057
    ),
    "Road to Ikana Pillar Chest": MMRLocationData(
        region="Road to Ikana",
        address=0x065300
    ),
    "Road to Ikana Rock Grotto Chest": MMRLocationData(
        region="Road to Ikana",
        address=0x060716
    ),
    "Road to Ikana Invisible Soldier": MMRLocationData(
        region="Road to Ikana",
        address=0x00008B
    ),
    "Ikana Graveyard Bombable Grotto Chest": MMRLocationData(
        region="Ikana Graveyard",
        address=0x060718
    ),
    "Graveyard Day 1 Bats Chest": MMRLocationData(
        region="Ikana Graveyard",
        address=0x060C03
    ),
    "Graveyard Day 1 Iron Knuckle Song": MMRLocationData(
        region="Ikana Graveyard",
        address=0x0000A2
    ),
    "Graveyard Day 2 Dampe Bats": MMRLocationData(
        region="Ikana Graveyard",
        address=0x0043CA
    ),
    "Graveyard Day 2 Iron Knuckle Chest": MMRLocationData(
        region="Ikana Graveyard",
        address=0x060C00
    ),
    "Graveyard Day 3 Dampe Big Poe Chest": MMRLocationData(
        region="Ikana Graveyard",
        address=0x063000
    ),
    "Graveyard Captain Keeta Chest": MMRLocationData(
        region="Ikana Graveyard",
        address=0x064300
    ),
    "Secret Shrine Dinolfos Chest": MMRLocationData(
        region="Secret Shrine",
        address=0x066000
    ),
    "Secret Shrine Wizzrobe Chest": MMRLocationData(
        region="Secret Shrine",
        address=0x066001
    ),
    "Secret Shrine Wart Chest": MMRLocationData(
        region="Secret Shrine",
        address=0x066002
    ),
    "Secret Shrine Garo Master Chest": MMRLocationData(
        region="Secret Shrine",
        address=0x066003
    ),
    "Secret Shrine Completion Chest": MMRLocationData(
        region="Secret Shrine",
        address=0x06600A
    ),
    "Ikana Canyon Grotto Chest": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x060714
    ),
    "Ikana Canyon Scrub Purchase": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x09015D,
        can_create=lambda options: options.scrubsanity.value
    ),
    "Ikana Canyon Zora Scrub Trade": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x001307
    ),
    "Ikana Canyon Zora Trade Freestanding HP": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x05131E
    ),
    "Ikana Canyon Healing Pamela's Father": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x000087
    ),
    "Ikana Canyon Spirit House": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x0701DE
    ),
    "Stone Tower Great Fairy Reward": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x030004
    ),
    "Ikana Well Final Chest": MMRLocationData(
        region="Beneath the Well",
        address=0x064B1B
    ),
    "Ikana Well Invisible Chest": MMRLocationData(
        region="Beneath the Well",
        address=0x064B02
    ),
    "Ikana Well Rightside Torch Chest": MMRLocationData(
        region="Beneath the Well",
        address=0x064B01
    ),
    "Ikana Well Cow": MMRLocationData(
        region="Beneath the Well",
        address=0xBEEF17,
        can_create=lambda options: options.cowsanity.value
    ),
    "Ikana Castle Pillar Freestanding HP": MMRLocationData(
        region="Ikana Castle",
        address=0x051D0A
    ),
    "Ikana Castle King Song": MMRLocationData(
        region="Ikana Castle",
        address=0x040064
    ),
    # ~ "Stone Tower Temple 1F Bridge Room Underwater Switch Chest Glitched": MMRLocationData(
        # ~ region="Stone Tower Temple",
        # ~ address=0x06160E
    # ~ ),
    "Stone Tower Inverted Left Chest": MMRLocationData(
        region="Stone Tower (Inverted)",
        address=0x06591F
    ),
    "Stone Tower Inverted Middle Chest": MMRLocationData(
        region="Stone Tower (Inverted)",
        address=0x06591E
    ),
    "Stone Tower Inverted Right Chest": MMRLocationData(
        region="Stone Tower (Inverted)",
        address=0x06591D
    ),
    "Stone Tower Temple Entrance Room Eye Switch Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x061616
    ),
    "Stone Tower Temple Entrance Room Lower Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x061612
    ),
    "Stone Tower Temple Armos Room Lava Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x061602
    ),
    "Stone Tower Temple Armos Room Back Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x06161D
    ),
    "Stone Tower Temple Armos Room Upper Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x061615
    ),
    "Stone Tower Temple Eyegore Room Switch Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x061618
    ),
    "Stone Tower Temple Eastern Water Room Sun Block Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x06161C
    ),
    "Stone Tower Temple Eastern Water Room Underwater Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x061617
    ),
    "Stone Tower Temple Eyegore Room Dexi Hand Ledge Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x061601
    ),
    "Stone Tower Temple Mirror Room Sun Block Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x06160B
    ),
    "Stone Tower Temple Mirror Room Sun Face Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x06160F
    ),
    "Stone Tower Temple Air Gust Room Side Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x061619
    ),
    "Stone Tower Temple Air Gust Room Goron Switch Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x06160D
    ),
    "Stone Tower Temple Garo Master Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x06161B
    ),
    "Stone Tower Temple After Garo Upside Down Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x061614
    ),
    "Stone Tower Temple Eyegore Chest": MMRLocationData(
        region="Stone Tower Temple",
        address=0x06160C
    ),
    "Stone Tower Temple Inverted Entrance Room Sun Face Chest": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x061810
    ),
    "Stone Tower Temple Inverted Eastern Air Gust Room Fire Chest": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x06180E
    ),
    "Stone Tower Temple Inverted Eastern Air Gust Room Frozen Switch Chest": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x061813
    ),
    "Stone Tower Temple Inverted Eastern Air Gust Room Switch Chest": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x061804
    ),
    "Stone Tower Temple Inverted Wizzrobe Chest": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x061811
    ),
    "Stone Tower Temple Inverted Death Armos Maze Chest": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x061805
    ),
    "Stone Tower Temple Inverted Gomess Chest": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x06181E
    ),
    "Stone Tower Temple Inverted Eyegore Chest": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x06181A
    ),
    "Stone Tower Temple Inverted Heart Container": MMRLocationData(
        region="Twinmold's Lair",
        address=0x053600
    ),
    "Stone Tower Temple Inverted Twinmold's Remains": MMRLocationData(
        region="Twinmold's Lair",
        address=0x000058
    ),
    "Oath to Order": MMRLocationData(
        region="Clock Town", # there isn't really a set location for this
        address=0x040065
    ),
    "Moon Deku Trial HP": MMRLocationData(
        region="The Moon",
        address=0x052A01
    ),
    "Moon Goron Trial HP": MMRLocationData(
        region="The Moon",
        address=0x053F01
    ),
    "Moon Zora Trial HP": MMRLocationData(
        region="The Moon",
        address=0x054701
    ),
    "Moon Link Trial Garo Master Chest": MMRLocationData(
        region="The Moon",
        address=0x066601
    ),
    "Moon Link Trial Iron Knuckle Chest": MMRLocationData(
        region="The Moon",
        address=0x066602
    ),
    "Moon Link Trial HP": MMRLocationData(
        region="The Moon",
        address=0x056601
    ),
    "Moon Trade All Masks": MMRLocationData(
        region="The Moon",
        address=0x00007B
    ),
    "Defeat Majora": MMRLocationData(
        region="The Moon",
        locked_item="Victory"
    ),
    
    # Grass/Pots/Hitspots/Hidden Rupees etc past this point

    # Before Clock Town Grass
    "Before Clock Town Keaton Grass (1)": MMRLocationData(
        region="Clock Town",
        address=0x131A09,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Keaton Grass (2)": MMRLocationData(
        region="Clock Town",
        address=0x131A10,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),    
    "Before Clock Town Keaton Grass (3)": MMRLocationData(
        region="Clock Town",
        address=0x131A0A,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Keaton Grass (4)": MMRLocationData(
        region="Clock Town",
        address=0x131A0B,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Keaton Grass (5)": MMRLocationData(
        region="Clock Town",
        address=0x131A0C,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Keaton Grass (6)": MMRLocationData(
        region="Clock Town",
        address=0x131A0D,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Keaton Grass (7)": MMRLocationData(
        region="Clock Town",
        address=0x131A0E,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Keaton Grass (8)": MMRLocationData(
        region="Clock Town",
        address=0x131A0F,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Keaton Grass (9)": MMRLocationData(
        region="Clock Town",
        address=0x131A11,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),        
    "Before Clock Town Skullkid Keaton Grass (1)": MMRLocationData(
        region="Clock Town",
        address=0x131A00,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Skullkid Keaton Grass (2)": MMRLocationData(
        region="Clock Town",
        address=0x131A01,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Skullkid Keaton Grass (3)": MMRLocationData(
        region="Clock Town",
        address=0x131A02,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Skullkid Keaton Grass (4)": MMRLocationData(
        region="Clock Town",
        address=0x131A03,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),    
    "Before Clock Town Skullkid Keaton Grass (5)": MMRLocationData(
        region="Clock Town",
        address=0x131A04,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),    
    "Before Clock Town Skullkid Keaton Grass (6)": MMRLocationData(
        region="Clock Town",
        address=0x131A05,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),    
    "Before Clock Town Skullkid Keaton Grass (7)": MMRLocationData(
        region="Clock Town",
        address=0x131A06,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Skullkid Keaton Grass (8)": MMRLocationData(
        region="Clock Town",
        address=0x131A07,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Before Clock Town Skullkid Keaton Grass (9)": MMRLocationData(
        region="Clock Town",
        address=0x131A08,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (1)": MMRLocationData(
        region="Clock Town",
        address=0x146511,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (2)": MMRLocationData(
        region="Clock Town",
        address=0x146516,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (3)": MMRLocationData(
        region="Clock Town",
        address=0x146515,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (4)": MMRLocationData(
        region="Clock Town",
        address=0x146510,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (5)": MMRLocationData(
        region="Clock Town",
        address=0x146514,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (6)": MMRLocationData(
        region="Clock Town",
        address=0x146512,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (7)": MMRLocationData(
        region="Clock Town",
        address=0x146519,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (8)": MMRLocationData(
        region="Clock Town",
        address=0x146518,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (9)": MMRLocationData(
        region="Clock Town",
        address=0x146513,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (10)": MMRLocationData(
        region="Clock Town",
        address=0x146517,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (11)": MMRLocationData(
        region="Clock Town",
        address=0x14651A,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass (12)": MMRLocationData(
        region="Clock Town",
        address=0x14651B,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),

    "Lost Woods Grass Patch 2 (1)": MMRLocationData(
        region="Clock Town",
        address=0x146505,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (2)": MMRLocationData(
        region="Clock Town",
        address=0x146500,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (3)": MMRLocationData(
        region="Clock Town",
        address=0x14650B,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (4)": MMRLocationData(
        region="Clock Town",
        address=0x146504,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (5)": MMRLocationData(
        region="Clock Town",
        address=0x14650A,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (6)": MMRLocationData(
        region="Clock Town",
        address=0x146509,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (7)": MMRLocationData(
        region="Clock Town",
        address=0x146508,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (8)": MMRLocationData(
        region="Clock Town",
        address=0x146503,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (9)": MMRLocationData(
        region="Clock Town",
        address=0x146502,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (10)": MMRLocationData(
        region="Clock Town",
        address=0x146501,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (11)": MMRLocationData(
        region="Clock Town",
        address=0x146506,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Grass Patch 2 (12)": MMRLocationData(
        region="Clock Town",
        address=0x146507,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Keaton Grass (1)": MMRLocationData(
        region="Clock Town",
        address=0x136502,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Keaton Grass (2)": MMRLocationData(
        region="Clock Town",
        address=0x136501,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Keaton Grass (3)": MMRLocationData(
        region="Clock Town",
        address=0x136507,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Keaton Grass (4)": MMRLocationData(
        region="Clock Town",
        address=0x136506,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Keaton Grass (5)": MMRLocationData(
        region="Clock Town",
        address=0x136505,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Keaton Grass (6)": MMRLocationData(
        region="Clock Town",
        address=0x136504,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Keaton Grass (7)": MMRLocationData(
        region="Clock Town",
        address=0x136503,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Keaton Grass (8)": MMRLocationData(
        region="Clock Town",
        address=0x136500,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Lost Woods Keaton Grass (9)": MMRLocationData(
        region="Clock Town",
        address=0x136508,
        can_create=lambda options: options.intro_checks.value and grass_enabled(options)
    ),
    "Laundry Pool Grass (1)": MMRLocationData(
        region="Clock Town",
        address=0x127000,
        can_create=lambda options: grass_enabled(options)
    ),
    "Laundry Pool Grass (2)": MMRLocationData(
        region="Clock Town",
        address=0x127001,
        can_create=lambda options: grass_enabled(options)
    ),
    "Laundry Pool Grass (3)": MMRLocationData(
        region="Clock Town",
        address=0x127002,
        can_create=lambda options: grass_enabled(options)
    ),
    # North Clock Town Keaton Grass
    "North Clock Town Keaton Grass (1)": MMRLocationData(
        region="Clock Town",
        address=0x136E00,
        can_create=lambda options: grass_enabled(options)
    ),
    "North Clock Town Keaton Grass (2)": MMRLocationData(
        region="Clock Town",
        address=0x136E01,
        can_create=lambda options: grass_enabled(options)
    ),
    "North Clock Town Keaton Grass (3)": MMRLocationData(
        region="Clock Town",
        address=0x136E02,
        can_create=lambda options: grass_enabled(options)
    ),
    "North Clock Town Keaton Grass (4)": MMRLocationData(
        region="Clock Town",
        address=0x136E03,
        can_create=lambda options: grass_enabled(options)
    ),
    "North Clock Town Keaton Grass (5)": MMRLocationData(
        region="Clock Town",
        address=0x136E04,
        can_create=lambda options: grass_enabled(options)
    ),
    "North Clock Town Keaton Grass (6)": MMRLocationData(
        region="Clock Town",
        address=0x136E05,
        can_create=lambda options: grass_enabled(options)
    ),
    "North Clock Town Keaton Grass (7)": MMRLocationData(
        region="Clock Town",
        address=0x136E06,
        can_create=lambda options: grass_enabled(options)
    ),
    "North Clock Town Keaton Grass (8)": MMRLocationData(
        region="Clock Town",
        address=0x136E07,
        can_create=lambda options: grass_enabled(options)
    ),
    "North Clock Town Keaton Grass (9)": MMRLocationData(
        region="Clock Town",
        address=0x136E08,
        can_create=lambda options: grass_enabled(options)
    ),
    # Termina Field 

    "Termina Field Grass Near Western Water Ramp (1)": MMRLocationData(
        region="Termina Field",
        address=0x1002D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (2)": MMRLocationData(
        region="Termina Field",
        address=0x1002D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (3)": MMRLocationData(
        region="Termina Field",
        address=0x1002D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (4)": MMRLocationData(
        region="Termina Field",
        address=0x1002D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (5)": MMRLocationData(
        region="Termina Field",
        address=0x1002D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (6)": MMRLocationData(
        region="Termina Field",
        address=0x1002D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (7)": MMRLocationData(
        region="Termina Field",
        address=0x1002D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (8)": MMRLocationData(
        region="Termina Field",
        address=0x1002D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (9)": MMRLocationData(
        region="Termina Field",
        address=0x1002D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (10)": MMRLocationData(
        region="Termina Field",
        address=0x1002D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (11)": MMRLocationData(
        region="Termina Field",
        address=0x1002DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Water Ramp (12)": MMRLocationData(
        region="Termina Field",
        address=0x1002DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (1)": MMRLocationData(
        region="Termina Field",
        address=0x1012D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (2)": MMRLocationData(
        region="Termina Field",
        address=0x1012D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (3)": MMRLocationData(
        region="Termina Field",
        address=0x1012D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (4)": MMRLocationData(
        region="Termina Field",
        address=0x1012D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (5)": MMRLocationData(
        region="Termina Field",
        address=0x1012D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (6)": MMRLocationData(
        region="Termina Field",
        address=0x1012D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (7)": MMRLocationData(
        region="Termina Field",
        address=0x1012D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (8)": MMRLocationData(
        region="Termina Field",
        address=0x1012D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (9)": MMRLocationData(
        region="Termina Field",
        address=0x1012D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (10)": MMRLocationData(
        region="Termina Field",
        address=0x1012D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (11)": MMRLocationData(
        region="Termina Field",
        address=0x1012DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri Gossip Tree (12)": MMRLocationData(
        region="Termina Field",
        address=0x1012DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (1)": MMRLocationData(
        region="Termina Field",
        address=0x1022D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (2)": MMRLocationData(
        region="Termina Field",
        address=0x1022D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (3)": MMRLocationData(
        region="Termina Field",
        address=0x1022D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (4)": MMRLocationData(
        region="Termina Field",
        address=0x1022D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (5)": MMRLocationData(
        region="Termina Field",
        address=0x1022D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (6)": MMRLocationData(
        region="Termina Field",
        address=0x1022D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (7)": MMRLocationData(
        region="Termina Field",
        address=0x1022D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (8)": MMRLocationData(
        region="Termina Field",
        address=0x1022D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (9)": MMRLocationData(
        region="Termina Field",
        address=0x1022D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (10)": MMRLocationData(
        region="Termina Field",
        address=0x1022D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (11)": MMRLocationData(
        region="Termina Field",
        address=0x1022DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Southwest Above Rock Ledge (12)": MMRLocationData(
        region="Termina Field",
        address=0x1022DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (1)": MMRLocationData(
        region="Termina Field",
        address=0x1032D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (2)": MMRLocationData(
        region="Termina Field",
        address=0x1032D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (3)": MMRLocationData(
        region="Termina Field",
        address=0x1032D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (4)": MMRLocationData(
        region="Termina Field",
        address=0x1032D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (5)": MMRLocationData(
        region="Termina Field",
        address=0x1032D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (6)": MMRLocationData(
        region="Termina Field",
        address=0x1032D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (7)": MMRLocationData(
        region="Termina Field",
        address=0x1032D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (8)": MMRLocationData(
        region="Termina Field",
        address=0x1032D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (9)": MMRLocationData(
        region="Termina Field",
        address=0x1032D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (10)": MMRLocationData(
        region="Termina Field",
        address=0x1032D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (11)": MMRLocationData(
        region="Termina Field",
        address=0x1032DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Western Fountains (12)": MMRLocationData(
        region="Termina Field",
        address=0x1032DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (1)": MMRLocationData(
        region="Termina Field",
        address=0x1042D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (2)": MMRLocationData(
        region="Termina Field",
        address=0x1042D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (3)": MMRLocationData(
        region="Termina Field",
        address=0x1042D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (4)": MMRLocationData(
        region="Termina Field",
        address=0x1042D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (5)": MMRLocationData(
        region="Termina Field",
        address=0x1042D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (6)": MMRLocationData(
        region="Termina Field",
        address=0x1042D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (7)": MMRLocationData(
        region="Termina Field",
        address=0x1042D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (8)": MMRLocationData(
        region="Termina Field",
        address=0x1042D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (9)": MMRLocationData(
        region="Termina Field",
        address=0x1042D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (10)": MMRLocationData(
        region="Termina Field",
        address=0x1042D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (11)": MMRLocationData(
        region="Termina Field",
        address=0x1042DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Takkuri (12)": MMRLocationData(
        region="Termina Field",
        address=0x1042DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (1)": MMRLocationData(
        region="Termina Field",
        address=0x1052D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (2)": MMRLocationData(
        region="Termina Field",
        address=0x1052D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (3)": MMRLocationData(
        region="Termina Field",
        address=0x1052D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (4)": MMRLocationData(
        region="Termina Field",
        address=0x1052D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (5)": MMRLocationData(
        region="Termina Field",
        address=0x1052D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (6)": MMRLocationData(
        region="Termina Field",
        address=0x1052D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (7)": MMRLocationData(
        region="Termina Field",
        address=0x1052D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (8)": MMRLocationData(
        region="Termina Field",
        address=0x1052D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (9)": MMRLocationData(
        region="Termina Field",
        address=0x1052D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (10)": MMRLocationData(
        region="Termina Field",
        address=0x1052D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (11)": MMRLocationData(
        region="Termina Field",
        address=0x1052DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Kamaro (12)": MMRLocationData(
        region="Termina Field",
        address=0x1052DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (1)": MMRLocationData(
        region="Termina Field",
        address=0x1062D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (2)": MMRLocationData(
        region="Termina Field",
        address=0x1062D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (3)": MMRLocationData(
        region="Termina Field",
        address=0x1062D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (4)": MMRLocationData(
        region="Termina Field",
        address=0x1062D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (5)": MMRLocationData(
        region="Termina Field",
        address=0x1062D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (6)": MMRLocationData(
        region="Termina Field",
        address=0x1062D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (7)": MMRLocationData(
        region="Termina Field",
        address=0x1062D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (8)": MMRLocationData(
        region="Termina Field",
        address=0x1062D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (9)": MMRLocationData(
        region="Termina Field",
        address=0x1062D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (10)": MMRLocationData(
        region="Termina Field",
        address=0x1062D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (11)": MMRLocationData(
        region="Termina Field",
        address=0x1062DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Peehat Grotto (12)": MMRLocationData(
        region="Termina Field",
        address=0x1062DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (1)": MMRLocationData(
        region="Termina Field",
        address=0x1072D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (2)": MMRLocationData(
        region="Termina Field",
        address=0x1072D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (3)": MMRLocationData(
        region="Termina Field",
        address=0x1072D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (4)": MMRLocationData(
        region="Termina Field",
        address=0x1072D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (5)": MMRLocationData(
        region="Termina Field",
        address=0x1072D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (6)": MMRLocationData(
        region="Termina Field",
        address=0x1072D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (7)": MMRLocationData(
        region="Termina Field",
        address=0x1072D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (8)": MMRLocationData(
        region="Termina Field",
        address=0x1072D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (9)": MMRLocationData(
        region="Termina Field",
        address=0x1072D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (10)": MMRLocationData(
        region="Termina Field",
        address=0x1072D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (11)": MMRLocationData(
        region="Termina Field",
        address=0x1072DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass In Front of Log (12)": MMRLocationData(
        region="Termina Field",
        address=0x1072DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (1)": MMRLocationData(
        region="Termina Field",
        address=0x1082D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (2)": MMRLocationData(
        region="Termina Field",
        address=0x1082D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (3)": MMRLocationData(
        region="Termina Field",
        address=0x1082D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (4)": MMRLocationData(
        region="Termina Field",
        address=0x1082D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (5)": MMRLocationData(
        region="Termina Field",
        address=0x1082D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (6)": MMRLocationData(
        region="Termina Field",
        address=0x1082D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (7)": MMRLocationData(
        region="Termina Field",
        address=0x1082D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (8)": MMRLocationData(
        region="Termina Field",
        address=0x1082D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (9)": MMRLocationData(
        region="Termina Field",
        address=0x1082D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (10)": MMRLocationData(
        region="Termina Field",
        address=0x1082D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (11)": MMRLocationData(
        region="Termina Field",
        address=0x1082DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Northern Ramp (12)": MMRLocationData(
        region="Termina Field",
        address=0x1082DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (1)": MMRLocationData(
        region="Termina Field",
        address=0x1092D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (2)": MMRLocationData(
        region="Termina Field",
        address=0x1092D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (3)": MMRLocationData(
        region="Termina Field",
        address=0x1092D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (4)": MMRLocationData(
        region="Termina Field",
        address=0x1092D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (5)": MMRLocationData(
        region="Termina Field",
        address=0x1092D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (6)": MMRLocationData(
        region="Termina Field",
        address=0x1092D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (7)": MMRLocationData(
        region="Termina Field",
        address=0x1092D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (8)": MMRLocationData(
        region="Termina Field",
        address=0x1092D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (9)": MMRLocationData(
        region="Termina Field",
        address=0x1092D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (10)": MMRLocationData(
        region="Termina Field",
        address=0x1092D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (11)": MMRLocationData(
        region="Termina Field",
        address=0x1092DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Grass Grotto (12)": MMRLocationData(
        region="Termina Field",
        address=0x1092DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (1)": MMRLocationData(
        region="Termina Field",
        address=0x10A2D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (2)": MMRLocationData(
        region="Termina Field",
        address=0x10A2D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (3)": MMRLocationData(
        region="Termina Field",
        address=0x10A2D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (4)": MMRLocationData(
        region="Termina Field",
        address=0x10A2D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (5)": MMRLocationData(
        region="Termina Field",
        address=0x10A2D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (6)": MMRLocationData(
        region="Termina Field",
        address=0x10A2D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (7)": MMRLocationData(
        region="Termina Field",
        address=0x10A2D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (8)": MMRLocationData(
        region="Termina Field",
        address=0x10A2D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (9)": MMRLocationData(
        region="Termina Field",
        address=0x10A2D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (10)": MMRLocationData(
        region="Termina Field",
        address=0x10A2D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (11)": MMRLocationData(
        region="Termina Field",
        address=0x10A2DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Below Southeast Hill Fence (12)": MMRLocationData(
        region="Termina Field",
        address=0x10A2DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (1)": MMRLocationData(
        region="Termina Field",
        address=0x10B2D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (2)": MMRLocationData(
        region="Termina Field",
        address=0x10B2D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (3)": MMRLocationData(
        region="Termina Field",
        address=0x10B2D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (4)": MMRLocationData(
        region="Termina Field",
        address=0x10B2D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (5)": MMRLocationData(
        region="Termina Field",
        address=0x10B2D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (6)": MMRLocationData(
        region="Termina Field",
        address=0x10B2D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (7)": MMRLocationData(
        region="Termina Field",
        address=0x10B2D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (8)": MMRLocationData(
        region="Termina Field",
        address=0x10B2D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (9)": MMRLocationData(
        region="Termina Field",
        address=0x10B2D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (10)": MMRLocationData(
        region="Termina Field",
        address=0x10B2D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (11)": MMRLocationData(
        region="Termina Field",
        address=0x10B2DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Bottom of Southeast Hill Grass (12)": MMRLocationData(
        region="Termina Field",
        address=0x10B2DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (1)": MMRLocationData(
        region="Termina Field",
        address=0x10C2D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (2)": MMRLocationData(
        region="Termina Field",
        address=0x10C2D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (3)": MMRLocationData(
        region="Termina Field",
        address=0x10C2D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (4)": MMRLocationData(
        region="Termina Field",
        address=0x10C2D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (5)": MMRLocationData(
        region="Termina Field",
        address=0x10C2D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (6)": MMRLocationData(
        region="Termina Field",
        address=0x10C2D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (7)": MMRLocationData(
        region="Termina Field",
        address=0x10C2D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (8)": MMRLocationData(
        region="Termina Field",
        address=0x10C2D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (9)": MMRLocationData(
        region="Termina Field",
        address=0x10C2D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (10)": MMRLocationData(
        region="Termina Field",
        address=0x10C2D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (11)": MMRLocationData(
        region="Termina Field",
        address=0x10C2DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillars (12)": MMRLocationData(
        region="Termina Field",
        address=0x10C2DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (1)": MMRLocationData(
        region="Termina Field",
        address=0x10D2D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (2)": MMRLocationData(
        region="Termina Field",
        address=0x10D2D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (3)": MMRLocationData(
        region="Termina Field",
        address=0x10D2D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (4)": MMRLocationData(
        region="Termina Field",
        address=0x10D2D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (5)": MMRLocationData(
        region="Termina Field",
        address=0x10D2D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (6)": MMRLocationData(
        region="Termina Field",
        address=0x10D2D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (7)": MMRLocationData(
        region="Termina Field",
        address=0x10D2D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (8)": MMRLocationData(
        region="Termina Field",
        address=0x10D2D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (9)": MMRLocationData(
        region="Termina Field",
        address=0x10D2D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (10)": MMRLocationData(
        region="Termina Field",
        address=0x10D2D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (11)": MMRLocationData(
        region="Termina Field",
        address=0x10D2DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Pillar Grotto (12)": MMRLocationData(
        region="Termina Field",
        address=0x10D2DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (1)": MMRLocationData(
        region="Termina Field",
        address=0x10E2D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (2)": MMRLocationData(
        region="Termina Field",
        address=0x10E2D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (3)": MMRLocationData(
        region="Termina Field",
        address=0x10E2D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (4)": MMRLocationData(
        region="Termina Field",
        address=0x10E2D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (5)": MMRLocationData(
        region="Termina Field",
        address=0x10E2D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (6)": MMRLocationData(
        region="Termina Field",
        address=0x10E2D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (7)": MMRLocationData(
        region="Termina Field",
        address=0x10E2D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (8)": MMRLocationData(
        region="Termina Field",
        address=0x10E2D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (9)": MMRLocationData(
        region="Termina Field",
        address=0x10E2D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (10)": MMRLocationData(
        region="Termina Field",
        address=0x10E2D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (11)": MMRLocationData(
        region="Termina Field",
        address=0x10E2DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Middle of Southeast Hill Grass (12)": MMRLocationData(
        region="Termina Field",
        address=0x10E2DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (1)": MMRLocationData(
        region="Termina Field",
        address=0x10F2D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (2)": MMRLocationData(
        region="Termina Field",
        address=0x10F2D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (3)": MMRLocationData(
        region="Termina Field",
        address=0x10F2D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (4)": MMRLocationData(
        region="Termina Field",
        address=0x10F2D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (5)": MMRLocationData(
        region="Termina Field",
        address=0x10F2D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (6)": MMRLocationData(
        region="Termina Field",
        address=0x10F2D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (7)": MMRLocationData(
        region="Termina Field",
        address=0x10F2D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (8)": MMRLocationData(
        region="Termina Field",
        address=0x10F2D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (9)": MMRLocationData(
        region="Termina Field",
        address=0x10F2D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (10)": MMRLocationData(
        region="Termina Field",
        address=0x10F2D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (11)": MMRLocationData(
        region="Termina Field",
        address=0x10F2DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Business Scrub Grotto (12)": MMRLocationData(
        region="Termina Field",
        address=0x10F2DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (1)": MMRLocationData(
        region="Termina Field",
        address=0x1102D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (2)": MMRLocationData(
        region="Termina Field",
        address=0x1102D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (3)": MMRLocationData(
        region="Termina Field",
        address=0x1102D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (4)": MMRLocationData(
        region="Termina Field",
        address=0x1102D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (5)": MMRLocationData(
        region="Termina Field",
        address=0x1102D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (6)": MMRLocationData(
        region="Termina Field",
        address=0x1102D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (7)": MMRLocationData(
        region="Termina Field",
        address=0x1102D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (8)": MMRLocationData(
        region="Termina Field",
        address=0x1102D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (9)": MMRLocationData(
        region="Termina Field",
        address=0x1102D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (10)": MMRLocationData(
        region="Termina Field",
        address=0x1102D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (11)": MMRLocationData(
        region="Termina Field",
        address=0x1102DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Observatory Fence (12)": MMRLocationData(
        region="Termina Field",
        address=0x1102DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (1)": MMRLocationData(
        region="Termina Field",
        address=0x1112D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (2)": MMRLocationData(
        region="Termina Field",
        address=0x1112D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (3)": MMRLocationData(
        region="Termina Field",
        address=0x1112D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (4)": MMRLocationData(
        region="Termina Field",
        address=0x1112D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (5)": MMRLocationData(
        region="Termina Field",
        address=0x1112D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (6)": MMRLocationData(
        region="Termina Field",
        address=0x1112D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (7)": MMRLocationData(
        region="Termina Field",
        address=0x1112D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (8)": MMRLocationData(
        region="Termina Field",
        address=0x1112D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (9)": MMRLocationData(
        region="Termina Field",
        address=0x1112D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (10)": MMRLocationData(
        region="Termina Field",
        address=0x1112D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (11)": MMRLocationData(
        region="Termina Field",
        address=0x1112DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    "Termina Field Grass Near Eastern Gossip Grotto (12)": MMRLocationData(
        region="Termina Field",
        address=0x1112DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True)
    ),
    # Termina Field Grass Grotto Grass

    "Termina Field Grass Grotto Grass (1)": MMRLocationData(
        region="Termina Field",
        address=0x12B040,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (2)": MMRLocationData(
        region="Termina Field",
        address=0x12B041,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (3)": MMRLocationData(
        region="Termina Field",
        address=0x12B042,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (4)": MMRLocationData(
        region="Termina Field",
        address=0x12B043,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (5)": MMRLocationData(
        region="Termina Field",
        address=0x12B044,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (6)": MMRLocationData(
        region="Termina Field",
        address=0x12B045,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (7)": MMRLocationData(
        region="Termina Field",
        address=0x12B046,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (8)": MMRLocationData(
        region="Termina Field",
        address=0x12B047,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (9)": MMRLocationData(
        region="Termina Field",
        address=0x12B048,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (10)": MMRLocationData(
        region="Termina Field",
        address=0x12B049,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (11)": MMRLocationData(
        region="Termina Field",
        address=0x12B04A,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (12)": MMRLocationData(
        region="Termina Field",
        address=0x12B04B,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (13)": MMRLocationData(
        region="Termina Field",
        address=0x12B04C,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Grass Grotto Grass (14)": MMRLocationData(
        region="Termina Field",
        address=0x12B04D,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    # Termina Field Cow Grotto Grass

    "Termina Field Cow Grotto Grass Group 1 (1)": MMRLocationData(
        region="Termina Field",
        address=0x100AD0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (2)": MMRLocationData(
        region="Termina Field",
        address=0x100AD1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (3)": MMRLocationData(
        region="Termina Field",
        address=0x100AD2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (4)": MMRLocationData(
        region="Termina Field",
        address=0x100AD3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (5)": MMRLocationData(
        region="Termina Field",
        address=0x100AD4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (6)": MMRLocationData(
        region="Termina Field",
        address=0x100AD5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (7)": MMRLocationData(
        region="Termina Field",
        address=0x100AD6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (8)": MMRLocationData(
        region="Termina Field",
        address=0x100AD7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (9)": MMRLocationData(
        region="Termina Field",
        address=0x100AD8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (10)": MMRLocationData(
        region="Termina Field",
        address=0x100AD9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (11)": MMRLocationData(
        region="Termina Field",
        address=0x100ADA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 1 (12)": MMRLocationData(
        region="Termina Field",
        address=0x100ADB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (1)": MMRLocationData(
        region="Termina Field",
        address=0x101AD0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (2)": MMRLocationData(
        region="Termina Field",
        address=0x101AD1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (3)": MMRLocationData(
        region="Termina Field",
        address=0x101AD2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (4)": MMRLocationData(
        region="Termina Field",
        address=0x101AD3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (5)": MMRLocationData(
        region="Termina Field",
        address=0x101AD4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (6)": MMRLocationData(
        region="Termina Field",
        address=0x101AD5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (7)": MMRLocationData(
        region="Termina Field",
        address=0x101AD6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (8)": MMRLocationData(
        region="Termina Field",
        address=0x101AD7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (9)": MMRLocationData(
        region="Termina Field",
        address=0x101AD8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (10)": MMRLocationData(
        region="Termina Field",
        address=0x101AD9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (11)": MMRLocationData(
        region="Termina Field",
        address=0x101ADA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 2 (12)": MMRLocationData(
        region="Termina Field",
        address=0x101ADB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (1)": MMRLocationData(
        region="Termina Field",
        address=0x102AD0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (2)": MMRLocationData(
        region="Termina Field",
        address=0x102AD1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (3)": MMRLocationData(
        region="Termina Field",
        address=0x102AD2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (4)": MMRLocationData(
        region="Termina Field",
        address=0x102AD3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (5)": MMRLocationData(
        region="Termina Field",
        address=0x102AD4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (6)": MMRLocationData(
        region="Termina Field",
        address=0x102AD5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (7)": MMRLocationData(
        region="Termina Field",
        address=0x102AD6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (8)": MMRLocationData(
        region="Termina Field",
        address=0x102AD7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (9)": MMRLocationData(
        region="Termina Field",
        address=0x102AD8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (10)": MMRLocationData(
        region="Termina Field",
        address=0x102AD9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (11)": MMRLocationData(
        region="Termina Field",
        address=0x102ADA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 3 (12)": MMRLocationData(
        region="Termina Field",
        address=0x102ADB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (1)": MMRLocationData(
        region="Termina Field",
        address=0x103AD0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (2)": MMRLocationData(
        region="Termina Field",
        address=0x103AD1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (3)": MMRLocationData(
        region="Termina Field",
        address=0x103AD2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (4)": MMRLocationData(
        region="Termina Field",
        address=0x103AD3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (5)": MMRLocationData(
        region="Termina Field",
        address=0x103AD4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (6)": MMRLocationData(
        region="Termina Field",
        address=0x103AD5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (7)": MMRLocationData(
        region="Termina Field",
        address=0x103AD6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (8)": MMRLocationData(
        region="Termina Field",
        address=0x103AD7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (9)": MMRLocationData(
        region="Termina Field",
        address=0x103AD8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (10)": MMRLocationData(
        region="Termina Field",
        address=0x103AD9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (11)": MMRLocationData(
        region="Termina Field",
        address=0x103ADA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 4 (12)": MMRLocationData(
        region="Termina Field",
        address=0x103ADB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (1)": MMRLocationData(
        region="Termina Field",
        address=0x104AD0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (2)": MMRLocationData(
        region="Termina Field",
        address=0x104AD1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (3)": MMRLocationData(
        region="Termina Field",
        address=0x104AD2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (4)": MMRLocationData(
        region="Termina Field",
        address=0x104AD3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (5)": MMRLocationData(
        region="Termina Field",
        address=0x104AD4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (6)": MMRLocationData(
        region="Termina Field",
        address=0x104AD5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (7)": MMRLocationData(
        region="Termina Field",
        address=0x104AD6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (8)": MMRLocationData(
        region="Termina Field",
        address=0x104AD7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (9)": MMRLocationData(
        region="Termina Field",
        address=0x104AD8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (10)": MMRLocationData(
        region="Termina Field",
        address=0x104AD9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (11)": MMRLocationData(
        region="Termina Field",
        address=0x104ADA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 5 (12)": MMRLocationData(
        region="Termina Field",
        address=0x104ADB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (1)": MMRLocationData(
        region="Termina Field",
        address=0x105AD0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (2)": MMRLocationData(
        region="Termina Field",
        address=0x105AD1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (3)": MMRLocationData(
        region="Termina Field",
        address=0x105AD2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (4)": MMRLocationData(
        region="Termina Field",
        address=0x105AD3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (5)": MMRLocationData(
        region="Termina Field",
        address=0x105AD4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (6)": MMRLocationData(
        region="Termina Field",
        address=0x105AD5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (7)": MMRLocationData(
        region="Termina Field",
        address=0x105AD6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (8)": MMRLocationData(
        region="Termina Field",
        address=0x105AD7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (9)": MMRLocationData(
        region="Termina Field",
        address=0x105AD8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (10)": MMRLocationData(
        region="Termina Field",
        address=0x105AD9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (11)": MMRLocationData(
        region="Termina Field",
        address=0x105ADA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Cow Grotto Grass Group 6 (12)": MMRLocationData(
        region="Termina Field",
        address=0x105ADB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    # Termina Field Peehat Grotto Grass

    "Termina Field Peehat Grotto Grass (1)": MMRLocationData(
        region="Termina Field",
        address=0x1008D0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (2)": MMRLocationData(
        region="Termina Field",
        address=0x1008D1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (3)": MMRLocationData(
        region="Termina Field",
        address=0x1008D2,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (4)": MMRLocationData(
        region="Termina Field",
        address=0x1008D3,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (5)": MMRLocationData(
        region="Termina Field",
        address=0x1008D4,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (6)": MMRLocationData(
        region="Termina Field",
        address=0x1008D5,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (7)": MMRLocationData(
        region="Termina Field",
        address=0x1008D6,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (8)": MMRLocationData(
        region="Termina Field",
        address=0x1008D7,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (9)": MMRLocationData(
        region="Termina Field",
        address=0x1008D8,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (10)": MMRLocationData(
        region="Termina Field",
        address=0x1008D9,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (11)": MMRLocationData(
        region="Termina Field",
        address=0x1008DA,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Peehat Grotto Grass (12)": MMRLocationData(
        region="Termina Field",
        address=0x1008DB,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    # Termina Field Bio Baba Grotto Grass

    "Termina Field Bio Baba Grotto Grass (1)": MMRLocationData(
        region="Termina Field",
        address=0x128BB0,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Bio Baba Grotto Grass (2)": MMRLocationData(
        region="Termina Field",
        address=0x128BB1,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    # Termina Field Eastern Gossip Grotto Grass

    "Termina Field Eastern Gossip Grotto Grass (1)": MMRLocationData(
        region="Termina Field",
        address=0x128220,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Gossip Grotto Grass (2)": MMRLocationData(
        region="Termina Field",
        address=0x128221,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Gossip Grotto Grass (3)": MMRLocationData(
        region="Termina Field",
        address=0x128222,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Gossip Grotto Grass (4)": MMRLocationData(
        region="Termina Field",
        address=0x128223,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Gossip Grotto Grass (5)": MMRLocationData(
        region="Termina Field",
        address=0x128224,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    # Termina Field Eastern Pillar Grotto Grass
    "Termina Field Eastern Pillar Grotto Grass (1)": MMRLocationData(
        region="Termina Field",
        address=0x12AB46,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (2)": MMRLocationData(
        region="Termina Field",
        address=0x12AB40,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (3)": MMRLocationData(
        region="Termina Field",
        address=0x12AB48,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (4)": MMRLocationData(
        region="Termina Field",
        address=0x12AB43,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (5)": MMRLocationData(
        region="Termina Field",
        address=0x12AB41,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (6)": MMRLocationData(
        region="Termina Field",
        address=0x12AB47,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (7)": MMRLocationData(
        region="Termina Field",
        address=0x12AB4B,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (8)": MMRLocationData(
        region="Termina Field",
        address=0x12AB4D,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (9)": MMRLocationData(
        region="Termina Field",
        address=0x12AB45,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (10)": MMRLocationData(
        region="Termina Field",
        address=0x12AB4A,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (11)": MMRLocationData(
        region="Termina Field",
        address=0x12AB42,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (12)": MMRLocationData(
        region="Termina Field",
        address=0x12AB44,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (13)": MMRLocationData(
        region="Termina Field",
        address=0x12AB49,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Eastern Pillar Grotto Grass (14)": MMRLocationData(
        region="Termina Field",
        address=0x12AB4C,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    # Termina Field Bombable Rock Grass

    "Termina Field Bombable Rock Grass (1)": MMRLocationData(
        region="Termina Field",
        address=0x128000,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Bombable Rock Grass (2)": MMRLocationData(
        region="Termina Field",
        address=0x128001,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Bombable Rock Grass (3)": MMRLocationData(
        region="Termina Field",
        address=0x128002,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Bombable Rock Grass (4)": MMRLocationData(
        region="Termina Field",
        address=0x128003,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    "Termina Field Bombable Rock Grass (5)": MMRLocationData(
        region="Termina Field",
        address=0x128004,
        can_create=lambda options: grass_enabled(options, is_termina_field=True, is_grotto_or_cave=True)
    ),
    # Road to Southern Swamp
     
    "Road to Southern Swamp Outside Archery Grass (1)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x124000,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Outside Archery Grass (2)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x124001,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (1)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x100400,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (2)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x100401,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (3)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x100402,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (4)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x100403,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (5)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x100404,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (6)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x100405,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (7)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x100406,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (8)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x100407,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (9)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x100408,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (10)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x101400,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (11)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x101401,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (12)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x101402,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (13)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x101403,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (14)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x101404,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (15)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x101405,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (16)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x101406,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (17)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x101407,
        can_create=lambda options: grass_enabled(options)
    ),
    "Road to Southern Swamp Grass (18)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x101408,
        can_create=lambda options: grass_enabled(options)
    ),
    # Road to Southern Swamp Grotto
    "Road to Southern Swamp Grotto Grass (1)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF40,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (2)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF41,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (3)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF42,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (4)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF43,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (5)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF44,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (6)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF45,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (7)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF46,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (8)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF47,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (9)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF48,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (10)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF49,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (11)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF4A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (12)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF4B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (13)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF4C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road to Southern Swamp Grotto Grass (14)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x12AF4D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    # Southern Swamp

    "Southern Swamp Owl Grass (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x124500,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Owl Grass (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x124501,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x100450,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x100451,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x100452,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (4)": MMRLocationData(
        region="Southern Swamp",
        address=0x100453,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (5)": MMRLocationData(
        region="Southern Swamp",
        address=0x100454,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (6)": MMRLocationData(
        region="Southern Swamp",
        address=0x100455,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (7)": MMRLocationData(
        region="Southern Swamp",
        address=0x100456,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (8)": MMRLocationData(
        region="Southern Swamp",
        address=0x100457,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (9)": MMRLocationData(
        region="Southern Swamp",
        address=0x100458,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (10)": MMRLocationData(
        region="Southern Swamp",
        address=0x100459,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (11)": MMRLocationData(
        region="Southern Swamp",
        address=0x10045A,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Tourist Centre (12)": MMRLocationData(
        region="Southern Swamp",
        address=0x10045B,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x102450,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x102451,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x102452,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (4)": MMRLocationData(
        region="Southern Swamp",
        address=0x102453,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (5)": MMRLocationData(
        region="Southern Swamp",
        address=0x102454,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (6)": MMRLocationData(
        region="Southern Swamp",
        address=0x102455,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (7)": MMRLocationData(
        region="Southern Swamp",
        address=0x102456,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (8)": MMRLocationData(
        region="Southern Swamp",
        address=0x102457,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (9)": MMRLocationData(
        region="Southern Swamp",
        address=0x102458,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (10)": MMRLocationData(
        region="Southern Swamp",
        address=0x103450,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (11)": MMRLocationData(
        region="Southern Swamp",
        address=0x103451,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (12)": MMRLocationData(
        region="Southern Swamp",
        address=0x103452,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (13)": MMRLocationData(
        region="Southern Swamp",
        address=0x103453,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (14)": MMRLocationData(
        region="Southern Swamp",
        address=0x103454,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (15)": MMRLocationData(
        region="Southern Swamp",
        address=0x103455,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (16)": MMRLocationData(
        region="Southern Swamp",
        address=0x103456,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (17)": MMRLocationData(
        region="Southern Swamp",
        address=0x103457,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Grass Near Witch Shop (18)": MMRLocationData(
        region="Southern Swamp",
        address=0x103458,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Near Gossip Stone Grass (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x124520,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Near Gossip Stone Grass (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x124521,
        can_create=lambda options: grass_enabled(options)
    ),              

    # Woods of Mystery

    "Woods of Mystery Grass (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x126410,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x126412,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x126411,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (4)": MMRLocationData(
        region="Southern Swamp",
        address=0x126400,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (5)": MMRLocationData(
        region="Southern Swamp",
        address=0x126401,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (6)": MMRLocationData(
        region="Southern Swamp",
        address=0x126431,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (7)": MMRLocationData(
        region="Southern Swamp",
        address=0x126430,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (8)": MMRLocationData(
        region="Southern Swamp",
        address=0x126442,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (9)": MMRLocationData(
        region="Southern Swamp",
        address=0x126440,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (10)": MMRLocationData(
        region="Southern Swamp",
        address=0x126441,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (11)": MMRLocationData(
        region="Southern Swamp",
        address=0x126443,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (12)": MMRLocationData(
        region="Southern Swamp",
        address=0x126450,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (13)": MMRLocationData(
        region="Southern Swamp",
        address=0x126451,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (14)": MMRLocationData(
        region="Southern Swamp",
        address=0x126482,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (15)": MMRLocationData(
        region="Southern Swamp",
        address=0x126484,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (16)": MMRLocationData(
        region="Southern Swamp",
        address=0x126481,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (17)": MMRLocationData(
        region="Southern Swamp",
        address=0x126480,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (18)": MMRLocationData(
        region="Southern Swamp",
        address=0x126483,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (19)": MMRLocationData(
        region="Southern Swamp",
        address=0x126471,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Grass (20)": MMRLocationData(
        region="Southern Swamp",
        address=0x126470,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Day 2 Unique Grass": MMRLocationData(
        region="Southern Swamp",
        address=0x126420,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Day 3 Unique Grass (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x126460,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Day 3 Unique Grass (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x126461,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woods of Mystery Day 2 Grotto Grass (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD48,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD49,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD4D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (4)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD4B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (5)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD44,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (6)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD42,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (7)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD45,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (8)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD47,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (9)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD43,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (10)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD41,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (11)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD4A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (12)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD40,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (13)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD46,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Woods of Mystery Day 2 Grotto Grass (14)": MMRLocationData(
        region="Southern Swamp",
        address=0x12AD4C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    # Southern Swamp Grotto
    "Southern Swamp Grotto Grass (1)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE40,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (2)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE41,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (3)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE42,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (4)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE43,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (5)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE44,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (6)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE45,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (7)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE46,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (8)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE47,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (9)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE48,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (10)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE49,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (11)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE4A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (12)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE4B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (13)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE4C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Southern Swamp Grotto Grass (14)": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x12AE4D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    #Deku Palace Bean Grotto Grass
    "Deku Palace Bean Grotto Grass (1)": MMRLocationData(
        region="Deku Palace",
        address=0x1008C0,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (2)": MMRLocationData(
        region="Deku Palace",
        address=0x1008C1,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (3)": MMRLocationData(
        region="Deku Palace",
        address=0x1008C2,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (4)": MMRLocationData(
        region="Deku Palace",
        address=0x1008C3,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (5)": MMRLocationData(
        region="Deku Palace",
        address=0x1008C4,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (6)": MMRLocationData(
        region="Deku Palace",
        address=0x1008C5,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (7)": MMRLocationData(
        region="Deku Palace",
        address=0x1008C6,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (8)": MMRLocationData(
        region="Deku Palace",
        address=0x1008C7,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (9)": MMRLocationData(
        region="Deku Palace",
        address=0x1008C8,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (10)": MMRLocationData(
        region="Deku Palace",
        address=0x1008C9,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (11)": MMRLocationData(
        region="Deku Palace",
        address=0x1008CA,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Deku Palace Bean Grotto Grass (12)": MMRLocationData(
        region="Deku Palace",
        address=0x1008CB,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    # Woodfall Grass
    "Woodfall Grass (1)": MMRLocationData(
        region="Woodfall",
        address=0x124600,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woodfall Grass (2)": MMRLocationData(
        region="Woodfall",
        address=0x124601,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woodfall Grass (3)": MMRLocationData(
        region="Woodfall",
        address=0x124602,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woodfall Grass (4)": MMRLocationData(
        region="Woodfall",
        address=0x124603,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woodfall Grass (5)": MMRLocationData(
        region="Woodfall",
        address=0x124604,
        can_create=lambda options: grass_enabled(options)
    ),
    "Woodfall Grass (6)": MMRLocationData(
        region="Woodfall",
        address=0x124605,
        can_create=lambda options: grass_enabled(options)
    ),
    # Southern Swamp After Dungeon Clear
    "Southern Swamp Owl Post Dungeon Grass (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x120000,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Owl Post Dungeon Grass (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x120001,
        can_create=lambda options: grass_enabled(options)
    ),

    # Milk Road Owl Grass
    "Milk Road Owl Grass (1)": MMRLocationData(
        region="Milk Road",
        address=0x122200,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Owl Grass (2)": MMRLocationData(
        region="Milk Road",
        address=0x122201,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Owl Grass (3)": MMRLocationData(
        region="Milk Road",
        address=0x122202,
        can_create=lambda options: grass_enabled(options)
    ),
    # Milk Road Keaton Grass
    "Milk Road Keaton Grass (1)": MMRLocationData(
        region="Milk Road",
        address=0x132200,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Keaton Grass (2)": MMRLocationData(
        region="Milk Road",
        address=0x132201,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Keaton Grass (3)": MMRLocationData(
        region="Milk Road",
        address=0x132202,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Keaton Grass (4)": MMRLocationData(
        region="Milk Road",
        address=0x132203,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Keaton Grass (5)": MMRLocationData(
        region="Milk Road",
        address=0x132204,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Keaton Grass (6)": MMRLocationData(
        region="Milk Road",
        address=0x132205,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Keaton Grass (7)": MMRLocationData(
        region="Milk Road",
        address=0x132206,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Keaton Grass (8)": MMRLocationData(
        region="Milk Road",
        address=0x132207,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Keaton Grass (9)": MMRLocationData(
        region="Milk Road",
        address=0x132208,
        can_create=lambda options: grass_enabled(options)
    ),
    # Milk Road Gorman Racetrack Grass
    "Milk Road Gorman Racetrack Grass Group 1 (1)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006A0,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (2)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006A1,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (3)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006A2,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (4)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006A3,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (5)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006A4,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (6)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006A5,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (7)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006A6,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (8)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006A7,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (9)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006A8,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (10)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006A9,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (11)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006AA,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 1 (12)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1006AB,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (1)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016A0,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (2)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016A1,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (3)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016A2,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (4)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016A3,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (5)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016A4,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (6)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016A5,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (7)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016A6,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (8)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016A7,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (9)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016A8,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (10)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016A9,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (11)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016AA,
        can_create=lambda options: grass_enabled(options)
    ),
    "Milk Road Gorman Racetrack Grass Group 2 (12)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x1016AB,
        can_create=lambda options: grass_enabled(options)
    ),
    # Romani Ranch Grass
    "Romani Ranch Grass In Front of Gossip Tree (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x100350,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x100351,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x100352,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x100353,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (5)": MMRLocationData(
        region="Romani Ranch",
        address=0x100354,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (6)": MMRLocationData(
        region="Romani Ranch",
        address=0x100355,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (7)": MMRLocationData(
        region="Romani Ranch",
        address=0x100356,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (8)": MMRLocationData(
        region="Romani Ranch",
        address=0x100357,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (9)": MMRLocationData(
        region="Romani Ranch",
        address=0x100358,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (10)": MMRLocationData(
        region="Romani Ranch",
        address=0x100359,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (11)": MMRLocationData(
        region="Romani Ranch",
        address=0x10035A,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass In Front of Gossip Tree (12)": MMRLocationData(
        region="Romani Ranch",
        address=0x10035B,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x101350,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x101351,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x101352,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x101353,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (5)": MMRLocationData(
        region="Romani Ranch",
        address=0x101354,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (6)": MMRLocationData(
        region="Romani Ranch",
        address=0x101355,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (7)": MMRLocationData(
        region="Romani Ranch",
        address=0x101356,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (8)": MMRLocationData(
        region="Romani Ranch",
        address=0x101357,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (9)": MMRLocationData(
        region="Romani Ranch",
        address=0x101358,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (10)": MMRLocationData(
        region="Romani Ranch",
        address=0x101359,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (11)": MMRLocationData(
        region="Romani Ranch",
        address=0x10135A,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Gossip Tree (12)": MMRLocationData(
        region="Romani Ranch",
        address=0x10135B,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x102350,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x102351,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x102352,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x102353,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (5)": MMRLocationData(
        region="Romani Ranch",
        address=0x102354,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (6)": MMRLocationData(
        region="Romani Ranch",
        address=0x102355,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (7)": MMRLocationData(
        region="Romani Ranch",
        address=0x102356,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (8)": MMRLocationData(
        region="Romani Ranch",
        address=0x102357,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (9)": MMRLocationData(
        region="Romani Ranch",
        address=0x102358,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (10)": MMRLocationData(
        region="Romani Ranch",
        address=0x102359,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (11)": MMRLocationData(
        region="Romani Ranch",
        address=0x10235A,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Near Entrance (12)": MMRLocationData(
        region="Romani Ranch",
        address=0x10235B,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x103350,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x103351,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x103352,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x103353,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (5)": MMRLocationData(
        region="Romani Ranch",
        address=0x103354,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (6)": MMRLocationData(
        region="Romani Ranch",
        address=0x103355,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (7)": MMRLocationData(
        region="Romani Ranch",
        address=0x103356,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (8)": MMRLocationData(
        region="Romani Ranch",
        address=0x103357,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (9)": MMRLocationData(
        region="Romani Ranch",
        address=0x103358,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (10)": MMRLocationData(
        region="Romani Ranch",
        address=0x103359,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (11)": MMRLocationData(
        region="Romani Ranch",
        address=0x10335A,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Between Entrance and Barn (12)": MMRLocationData(
        region="Romani Ranch",
        address=0x10335B,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Barn (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x104350,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Barn (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x104351,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Barn (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x104352,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Barn (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x104353,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Barn (5)": MMRLocationData(
        region="Romani Ranch",
        address=0x104354,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Barn (6)": MMRLocationData(
        region="Romani Ranch",
        address=0x104355,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Barn (7)": MMRLocationData(
        region="Romani Ranch",
        address=0x104356,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Barn (8)": MMRLocationData(
        region="Romani Ranch",
        address=0x104357,
        can_create=lambda options: grass_enabled(options)
    ),
    "Romani Ranch Grass Behind Barn (9)": MMRLocationData(
        region="Romani Ranch",
        address=0x104358,
        can_create=lambda options: grass_enabled(options)
    ),

    # Twin Isles Grotto Grass

    "Twin Isles Grotto Grass (1)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA40,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (2)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA41,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (3)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA42,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (4)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA43,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (5)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA44,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (6)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA45,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (7)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA46,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (8)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA47,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (9)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA48,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (10)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA49,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (11)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA4A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (12)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA4B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (13)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA4C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Twin Isles Grotto Grass (14)": MMRLocationData(
        region="Twin Islands",
        address=0x12AA4D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    # Goron Village Lens Cave Grass
    "Goron Village Lens Cave Grass (1)": MMRLocationData(
        region="Goron Village",
        address=0x100900,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (2)": MMRLocationData(
        region="Goron Village",
        address=0x100901,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (3)": MMRLocationData(
        region="Goron Village",
        address=0x100902,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (4)": MMRLocationData(
        region="Goron Village",
        address=0x100903,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (5)": MMRLocationData(
        region="Goron Village",
        address=0x100904,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (6)": MMRLocationData(
        region="Goron Village",
        address=0x100905,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (7)": MMRLocationData(
        region="Goron Village",
        address=0x100906,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (8)": MMRLocationData(
        region="Goron Village",
        address=0x100907,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (9)": MMRLocationData(
        region="Goron Village",
        address=0x100908,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (10)": MMRLocationData(
        region="Goron Village",
        address=0x100909,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (11)": MMRLocationData(
        region="Goron Village",
        address=0x10090A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (12)": MMRLocationData(
        region="Goron Village",
        address=0x10090B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (13)": MMRLocationData(
        region="Goron Village",
        address=0x101900,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (14)": MMRLocationData(
        region="Goron Village",
        address=0x101901,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (15)": MMRLocationData(
        region="Goron Village",
        address=0x101902,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (16)": MMRLocationData(
        region="Goron Village",
        address=0x101903,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (17)": MMRLocationData(
        region="Goron Village",
        address=0x101904,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (18)": MMRLocationData(
        region="Goron Village",
        address=0x101905,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (19)": MMRLocationData(
        region="Goron Village",
        address=0x101906,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (20)": MMRLocationData(
        region="Goron Village",
        address=0x101907,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (21)": MMRLocationData(
        region="Goron Village",
        address=0x101908,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (22)": MMRLocationData(
        region="Goron Village",
        address=0x101909,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (23)": MMRLocationData(
        region="Goron Village",
        address=0x10190A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Goron Village Lens Cave Grass (24)": MMRLocationData(
        region="Goron Village",
        address=0x10190B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    # Path To Snowhead Grotto Grass

    "Path To Snowhead Grotto Grass (1)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A440,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (2)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A441,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (3)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A442,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (4)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A443,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (5)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A444,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (6)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A445,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (7)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A446,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (8)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A447,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (9)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A448,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (10)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A449,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (11)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A44A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (12)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A44B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (13)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A44C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Path To Snowhead Grotto Grass (14)": MMRLocationData(
        region="Path to Snowhead",
        address=0x12A44D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    # Mountain Village Spring Grass

    "Mountain Village Springtime Grass (1)": MMRLocationData( 
        region="Mountain Village",
        address=0x125A00,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (2)": MMRLocationData(
        region="Mountain Village",
        address=0x125A01,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (3)": MMRLocationData(
        region="Mountain Village",
        address=0x125A02,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (4)": MMRLocationData(
        region="Mountain Village",
        address=0x145A00,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (5)": MMRLocationData(
        region="Mountain Village",
        address=0x145A01,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (6)": MMRLocationData(
        region="Mountain Village",
        address=0x145A02,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (7)": MMRLocationData(
        region="Mountain Village",
        address=0x145A03,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (8)": MMRLocationData(
        region="Mountain Village",
        address=0x145A04,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (9)": MMRLocationData(
        region="Mountain Village",
        address=0x145A05,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (10)": MMRLocationData(
        region="Mountain Village",
        address=0x145A06,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (11)": MMRLocationData(
        region="Mountain Village",
        address=0x145A07,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (12)": MMRLocationData(
        region="Mountain Village",
        address=0x145A08,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (13)": MMRLocationData(
        region="Mountain Village",
        address=0x145A10,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (14)": MMRLocationData(
        region="Mountain Village",
        address=0x145A11,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (15)": MMRLocationData(
        region="Mountain Village",
        address=0x145A12,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (16)": MMRLocationData(
        region="Mountain Village",
        address=0x145A13,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (17)": MMRLocationData(
        region="Mountain Village",
        address=0x145A14,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (18)": MMRLocationData(
        region="Mountain Village",
        address=0x145A15,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (19)": MMRLocationData(
        region="Mountain Village",
        address=0x145A16,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (20)": MMRLocationData(
        region="Mountain Village",
        address=0x145A17,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (21)": MMRLocationData(
        region="Mountain Village",
        address=0x145A18,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (22)": MMRLocationData(
        region="Mountain Village",
        address=0x145A20,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (23)": MMRLocationData(
        region="Mountain Village",
        address=0x145A21,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (24)": MMRLocationData(
        region="Mountain Village",
        address=0x145A22,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (25)": MMRLocationData(
        region="Mountain Village",
        address=0x145A23,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (26)": MMRLocationData(
        region="Mountain Village",
        address=0x145A24,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (27)": MMRLocationData(
        region="Mountain Village",
        address=0x145A25,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (28)": MMRLocationData(
        region="Mountain Village",
        address=0x145A26,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (29)": MMRLocationData(
        region="Mountain Village",
        address=0x145A27,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Springtime Grass (30)": MMRLocationData(
        region="Mountain Village",
        address=0x145A28,
        can_create=lambda options: grass_enabled(options)
    ),

    # Mountain Village Keaton Grass 

    "Mountain Village Keaton Grass (0)": MMRLocationData(
        region="Mountain Village",
        address=0x135A00,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Keaton Grass (1)": MMRLocationData(
        region="Mountain Village",
        address=0x135A01,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Keaton Grass (2)": MMRLocationData(
        region="Mountain Village",
        address=0x135A02,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Keaton Grass (3)": MMRLocationData(
        region="Mountain Village",
        address=0x135A03,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Keaton Grass (4)": MMRLocationData(
        region="Mountain Village",
        address=0x135A04,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Keaton Grass (5)": MMRLocationData(
        region="Mountain Village",
        address=0x135A05,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Keaton Grass (6)": MMRLocationData(
        region="Mountain Village",
        address=0x135A06,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Keaton Grass (7)": MMRLocationData(
        region="Mountain Village",
        address=0x135A07,
        can_create=lambda options: grass_enabled(options)
    ),
    "Mountain Village Keaton Grass (8)": MMRLocationData(
        region="Mountain Village",
        address=0x135A08,
        can_create=lambda options: grass_enabled(options)
    ),

    # Mountain Village Spring Grotto Grass 

    "Mountain Village Spring Grotto Grass (1)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC40,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (2)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC41,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (3)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC42,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (4)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC43,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (5)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC44,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (6)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC45,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (7)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC46,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (8)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC47,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (9)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC48,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (10)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC49,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (11)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC4A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (12)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC4B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (13)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC4C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Mountain Village Spring Grotto Grass (14)": MMRLocationData(
        region="Mountain Village",
        address=0x12AC4D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    # Twin Isles Spring Grass

    "Twin Islands Springtime Grass Group 1 (1)": MMRLocationData(
        region="Twin Islands",
        address=0x1005E0,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (2)": MMRLocationData(
        region="Twin Islands",
        address=0x1005E1,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (3)": MMRLocationData(
        region="Twin Islands",
        address=0x1005E2,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (4)": MMRLocationData(
        region="Twin Islands",
        address=0x1005E3,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (5)": MMRLocationData(
        region="Twin Islands",
        address=0x1005E4,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (6)": MMRLocationData(
        region="Twin Islands",
        address=0x1005E5,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (7)": MMRLocationData(
        region="Twin Islands",
        address=0x1005E6,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (8)": MMRLocationData(
        region="Twin Islands",
        address=0x1005E7,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (9)": MMRLocationData(
        region="Twin Islands",
        address=0x1005E8,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (10)": MMRLocationData(
        region="Twin Islands",
        address=0x1005E9,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (11)": MMRLocationData(
        region="Twin Islands",
        address=0x1005EA,
        can_create=lambda options: grass_enabled(options)
    ),
    "Twin Islands Springtime Grass Group 1 (12)": MMRLocationData(
        region="Twin Islands",
        address=0x1005EB,
        can_create=lambda options: grass_enabled(options)
    ),
    # Great Bay Coast Grotto Grass

    "Great Bay Coast Grotto Grass (1)": MMRLocationData(
        region="Great Bay",
        address=0x12A840,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Grotto Grass (2)": MMRLocationData(
        region="Great Bay",
        address=0x12A841,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Grotto Grass (3)": MMRLocationData(
        region="Great Bay",
        address=0x12A842,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Grotto Grass (4)": MMRLocationData(
        region="Great Bay",
        address=0x12A843,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Grotto Grass (5)": MMRLocationData(
        region="Great Bay",
        address=0x12A844,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Grotto Grass (6)": MMRLocationData(
        region="Great Bay",
        address=0x12A845,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Grotto Grass (7)": MMRLocationData(
        region="Great Bay",
        address=0x12A846,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Grotto Grass (8)": MMRLocationData(
        region="Great Bay",
        address=0x12A847,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Grotto Grass (9)": MMRLocationData(
        region="Great Bay",
        address=0x12A848,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Grotto Grass (10)": MMRLocationData(
        region="Great Bay",
        address=0x12A849,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ), 
    "Great Bay Coast Grotto Grass (11)": MMRLocationData(
        region="Great Bay",
        address=0x12A84A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ), 
    "Great Bay Coast Grotto Grass (12)": MMRLocationData(
        region="Great Bay",
        address=0x12A84B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ), 
    "Great Bay Coast Grotto Grass (13)": MMRLocationData(
        region="Great Bay",
        address=0x12A84C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ), 
    "Great Bay Coast Grotto Grass (14)": MMRLocationData(
        region="Great Bay",
        address=0x12A84D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ), 

    # Great Bay Coast Grass - Requires Epona's Song
    "Great Bay Coast Grass (1)": MMRLocationData(
        region="Great Bay",
        address=0x123700,
        can_create=lambda options: grass_enabled(options)
    ),
    "Great Bay Coast Grass (2)": MMRLocationData(
        region="Great Bay",
        address=0x123701,
        can_create=lambda options: grass_enabled(options)
    ),
    "Great Bay Coast Grass (3)": MMRLocationData(
        region="Great Bay",
        address=0x123702,
        can_create=lambda options: grass_enabled(options)
    ),
    "Great Bay Coast Grass (4)": MMRLocationData(
        region="Great Bay",
        address=0x123703,
        can_create=lambda options: grass_enabled(options)
    ),
    "Great Bay Coast Grass (5)": MMRLocationData(
        region="Great Bay",
        address=0x123704,
        can_create=lambda options: grass_enabled(options)
    ),

    # Great Bay Coast Cow Grotto Grass

    "Great Bay Coast Cow Grotto Grass Group 1 (1)": MMRLocationData(
        region="Great Bay",
        address=0x100B70,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (2)": MMRLocationData(
        region="Great Bay",
        address=0x100B71,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (3)": MMRLocationData(
        region="Great Bay",
        address=0x100B72,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (4)": MMRLocationData(
        region="Great Bay",
        address=0x100B73,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (5)": MMRLocationData(
        region="Great Bay",
        address=0x100B74,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (6)": MMRLocationData(
        region="Great Bay",
        address=0x100B75,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (7)": MMRLocationData(
        region="Great Bay",
        address=0x100B76,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (8)": MMRLocationData(
        region="Great Bay",
        address=0x100B77,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (9)": MMRLocationData(
        region="Great Bay",
        address=0x100B78,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (10)": MMRLocationData(
        region="Great Bay",
        address=0x100B79,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (11)": MMRLocationData(
        region="Great Bay",
        address=0x100B7A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 1 (12)": MMRLocationData(
        region="Great Bay",
        address=0x100B7B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (1)": MMRLocationData(
        region="Great Bay",
        address=0x101B70,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (2)": MMRLocationData(
        region="Great Bay",
        address=0x101B71,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (3)": MMRLocationData(
        region="Great Bay",
        address=0x101B72,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (4)": MMRLocationData(
        region="Great Bay",
        address=0x101B73,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (5)": MMRLocationData(
        region="Great Bay",
        address=0x101B74,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (6)": MMRLocationData(
        region="Great Bay",
        address=0x101B75,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (7)": MMRLocationData(
        region="Great Bay",
        address=0x101B76,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (8)": MMRLocationData(
        region="Great Bay",
        address=0x101B77,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (9)": MMRLocationData(
        region="Great Bay",
        address=0x101B78,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (10)": MMRLocationData(
        region="Great Bay",
        address=0x101B79,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (11)": MMRLocationData(
        region="Great Bay",
        address=0x101B7A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 2 (12)": MMRLocationData(
        region="Great Bay",
        address=0x101B7B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (1)": MMRLocationData(
        region="Great Bay",
        address=0x102B70,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (2)": MMRLocationData(
        region="Great Bay",
        address=0x102B71,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (3)": MMRLocationData(
        region="Great Bay",
        address=0x102B72,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (4)": MMRLocationData(
        region="Great Bay",
        address=0x102B73,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (5)": MMRLocationData(
        region="Great Bay",
        address=0x102B74,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (6)": MMRLocationData(
        region="Great Bay",
        address=0x102B75,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (7)": MMRLocationData(
        region="Great Bay",
        address=0x102B76,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (8)": MMRLocationData(
        region="Great Bay",
        address=0x102B77,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (9)": MMRLocationData(
        region="Great Bay",
        address=0x102B78,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (10)": MMRLocationData(
        region="Great Bay",
        address=0x102B79,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (11)": MMRLocationData(
        region="Great Bay",
        address=0x102B7A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 3 (12)": MMRLocationData(
        region="Great Bay",
        address=0x102B7B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (1)": MMRLocationData(
        region="Great Bay",
        address=0x103B70,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (2)": MMRLocationData(
        region="Great Bay",
        address=0x103B71,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (3)": MMRLocationData(
        region="Great Bay",
        address=0x103B72,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (4)": MMRLocationData(
        region="Great Bay",
        address=0x103B73,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (5)": MMRLocationData(
        region="Great Bay",
        address=0x103B74,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (6)": MMRLocationData(
        region="Great Bay",
        address=0x103B75,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (7)": MMRLocationData(
        region="Great Bay",
        address=0x103B76,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (8)": MMRLocationData(
        region="Great Bay",
        address=0x103B77,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (9)": MMRLocationData(
        region="Great Bay",
        address=0x103B78,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (10)": MMRLocationData(
        region="Great Bay",
        address=0x103B79,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (11)": MMRLocationData(
        region="Great Bay",
        address=0x103B7A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 4 (12)": MMRLocationData(
        region="Great Bay",
        address=0x103B7B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (1)": MMRLocationData(
        region="Great Bay",
        address=0x104B70,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (2)": MMRLocationData(
        region="Great Bay",
        address=0x104B71,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (3)": MMRLocationData(
        region="Great Bay",
        address=0x104B72,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (4)": MMRLocationData(
        region="Great Bay",
        address=0x104B73,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (5)": MMRLocationData(
        region="Great Bay",
        address=0x104B74,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (6)": MMRLocationData(
        region="Great Bay",
        address=0x104B75,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (7)": MMRLocationData(
        region="Great Bay",
        address=0x104B76,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (8)": MMRLocationData(
        region="Great Bay",
        address=0x104B77,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (9)": MMRLocationData(
        region="Great Bay",
        address=0x104B78,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (10)": MMRLocationData(
        region="Great Bay",
        address=0x104B79,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (11)": MMRLocationData(
        region="Great Bay",
        address=0x104B7A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 5 (12)": MMRLocationData(
        region="Great Bay",
        address=0x104B7B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (1)": MMRLocationData(
        region="Great Bay",
        address=0x105B70,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (2)": MMRLocationData(
        region="Great Bay",
        address=0x105B71,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (3)": MMRLocationData(
        region="Great Bay",
        address=0x105B72,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (4)": MMRLocationData(
        region="Great Bay",
        address=0x105B73,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (5)": MMRLocationData(
        region="Great Bay",
        address=0x105B74,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (6)": MMRLocationData(
        region="Great Bay",
        address=0x105B75,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (7)": MMRLocationData(
        region="Great Bay",
        address=0x105B76,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (8)": MMRLocationData(
        region="Great Bay",
        address=0x105B77,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (9)": MMRLocationData(
        region="Great Bay",
        address=0x105B78,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (10)": MMRLocationData(
        region="Great Bay",
        address=0x105B79,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (11)": MMRLocationData(
        region="Great Bay",
        address=0x105B7A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Great Bay Coast Cow Grotto Grass Group 6 (12)": MMRLocationData(
        region="Great Bay",
        address=0x105B7B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    # Zora Cape Grotto Grass

    "Zora Cape Grotto Grass (1)": MMRLocationData(
        region="Zora Cape",
        address=0x12A640,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (2)": MMRLocationData(
        region="Zora Cape",
        address=0x12A641,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (3)": MMRLocationData(
        region="Zora Cape",
        address=0x12A642,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (4)": MMRLocationData(
        region="Zora Cape",
        address=0x12A643,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (5)": MMRLocationData(
        region="Zora Cape",
        address=0x12A644,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (6)": MMRLocationData(
        region="Zora Cape",
        address=0x12A645,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (7)": MMRLocationData(
        region="Zora Cape",
        address=0x12A646,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (8)": MMRLocationData(
        region="Zora Cape",
        address=0x12A647,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (9)": MMRLocationData(
        region="Zora Cape",
        address=0x12A648,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (10)": MMRLocationData(
        region="Zora Cape",
        address=0x12A649,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (11)": MMRLocationData(
        region="Zora Cape",
        address=0x12A64A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (12)": MMRLocationData(
        region="Zora Cape",
        address=0x12A64B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (13)": MMRLocationData(
        region="Zora Cape",
        address=0x12A64C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Zora Cape Grotto Grass (14)": MMRLocationData(
        region="Zora Cape",
        address=0x12A64D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    # Road To Ikana Grotto Grass

    "Road To Ikana Grotto Grass (1)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A740,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (2)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A741,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (3)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A742,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (4)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A743,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (5)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A744,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (6)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A745,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (7)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A746,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (8)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A747,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (9)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A748,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (10)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A749,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (11)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A74A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (12)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A74B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (13)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A74C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Road To Ikana Grotto Grass (14)": MMRLocationData(
        region="Road to Ikana",
        address=0x12A74D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    # Ikana Graveyard Lower Region Grass

    "Ikana Graveyard Lower Grass (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x124300,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Lower Grass (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x124301,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Lower Grass (3)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x124302,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Lower Grass (4)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x124303,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Lower Grass (5)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x124304,
        can_create=lambda options: grass_enabled(options)
    ),

    # Ikana Graveyard Upper Region Grass

    "Ikana Graveyard Upper Grass (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x144320,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Upper Grass (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x144321,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Upper Grass (3)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x144322,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Upper Grass (4)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x144323,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Upper Grass (5)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x144324,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Upper Grass (6)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x144325,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Upper Grass (7)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x144326,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Upper Grass (8)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x144327,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Graveyard Upper Grass (9)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x144328,
        can_create=lambda options: grass_enabled(options)
    ),

    # Ikana Graveyard Bombable Grotto Grass

    "Ikana Graveyard Bombable Grotto Grass (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A940,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A941,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (3)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A942,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (4)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A943,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (5)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A944,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (6)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A945,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (7)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A946,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (8)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A947,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (9)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A948,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (10)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A949,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (11)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A94A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (12)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A94B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (13)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A94C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Graveyard Bombable Grotto Grass (14)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x12A94D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    # Ikana Canyon Grass

    "Ikana Canyon Grass (1)": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x121300,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Canyon Grass (2)": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x121301,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Canyon Grass (3)": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x121302,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Canyon Grass (4)": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x121303,
        can_create=lambda options: grass_enabled(options)
    ),

    # Ikana Canyon Grotto Grass

    "Ikana Canyon Grotto Grass (1)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A540,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (2)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A541,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (3)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A542,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (4)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A543,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (5)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A544,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (6)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A545,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (7)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A546,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (8)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A547,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (9)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A548,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (10)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A549,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (11)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A54A,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (12)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A54B,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (13)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A54C,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),
    "Ikana Canyon Grotto Grass (14)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x12A54D,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True)
    ),

    # Secret Shrine Grass

    "Secret Shrine Entrance Grass (1)": MMRLocationData(
        region="Secret Shrine",
        address=0x126000,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Entrance Grass (2)": MMRLocationData(
        region="Secret Shrine",
        address=0x126001,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Entrance Grass (3)": MMRLocationData(
        region="Secret Shrine",
        address=0x126002,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Entrance Grass (4)": MMRLocationData(
        region="Secret Shrine",
        address=0x126003,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Entrance Grass (5)": MMRLocationData(
        region="Secret Shrine",
        address=0x126004,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Entrance Grass (6)": MMRLocationData(
        region="Secret Shrine",
        address=0x126005,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Dinolfos Grass (1)": MMRLocationData(
        region="Secret Shrine",
        address=0x126020,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Dinolfos Grass (2)": MMRLocationData(
        region="Secret Shrine",
        address=0x126021,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Dinolfos Grass (3)": MMRLocationData(
        region="Secret Shrine",
        address=0x126022,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Dinolfos Grass (4)": MMRLocationData(
        region="Secret Shrine",
        address=0x126023,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wizzrobe Grass (1)": MMRLocationData(
        region="Secret Shrine",
        address=0x126033,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wizzrobe Grass (2)": MMRLocationData(
        region="Secret Shrine",
        address=0x126032,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wizzrobe Grass (3)": MMRLocationData(
        region="Secret Shrine",
        address=0x126030,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wizzrobe Grass (4)": MMRLocationData(
        region="Secret Shrine",
        address=0x126031,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wizzrobe Grass (5)": MMRLocationData(
        region="Secret Shrine",
        address=0x126034,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wart Grass (1)": MMRLocationData(
        region="Secret Shrine",
        address=0x126042,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wart Grass (2)": MMRLocationData(
        region="Secret Shrine",
        address=0x126043,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wart Grass (3)": MMRLocationData(
        region="Secret Shrine",
        address=0x126045,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wart Grass (4)": MMRLocationData(
        region="Secret Shrine",
        address=0x126044,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wart Grass (5)": MMRLocationData(
        region="Secret Shrine",
        address=0x126047,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wart Grass (6)": MMRLocationData(
        region="Secret Shrine",
        address=0x126046,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wart Grass (7)": MMRLocationData(
        region="Secret Shrine",
        address=0x126040,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Wart Grass (8)": MMRLocationData(
        region="Secret Shrine",
        address=0x126041,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Garo Master Grass (1)": MMRLocationData(
        region="Secret Shrine",
        address=0x126055,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Garo Master Grass (2)": MMRLocationData(
        region="Secret Shrine",
        address=0x126052,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Garo Master Grass (3)": MMRLocationData(
        region="Secret Shrine",
        address=0x126051,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Garo Master Grass (4)": MMRLocationData(
        region="Secret Shrine",
        address=0x126050,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Garo Master Grass (5)": MMRLocationData(
        region="Secret Shrine",
        address=0x126054,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),
    "Secret Shrine Garo Master Grass (6)": MMRLocationData(
        region="Secret Shrine",
        address=0x126053,
        can_create=lambda options: grass_enabled(options, is_grotto_or_cave=True, is_dungeon=True)
    ),

    # Beneath the Well Grass

    "Beneath the Well Left Side Back Room Grass (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B51,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Left Side Back Room Grass (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B50,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Before Big Poe and Cow Grass (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B30,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Before Big Poe and Cow Grass (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B31,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Before Big Poe and Cow Grass (3)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B32,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Before Big Poe and Cow Grass (4)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B33,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Cow Grass (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B92,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Cow Grass (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B91,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Cow Grass (3)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B90,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Back Room Grass (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B71,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Back Room Grass (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B72,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Back Room Grass (3)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B74,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Back Room Grass (4)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B73,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Beneath the Well Right Side Back Room Grass (5)": MMRLocationData(
        region="Beneath the Well",
        address=0x124B70,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),

    # Ikana Castle Grass

    "Ikana Castle Grass (1)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D0B,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (2)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D01,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (3)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D02,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (4)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D03,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (5)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D04,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (6)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D05,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (7)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D06,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (8)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D07,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (9)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D08,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (10)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D09,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (11)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D0A,
        can_create=lambda options: grass_enabled(options)
    ),
    "Ikana Castle Grass (12)": MMRLocationData(
        region="Ikana Castle",
        address=0x121D00,
        can_create=lambda options: grass_enabled(options)
    ),
    # Dungeon Grass
    # Woodfall Temple
    "Woodfall Temple Entrance Room Grass (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B20,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Entrance Room Grass (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B21,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Entrance Room Grass (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B22,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Entrance Room Grass (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B23,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Entrance Room Grass (5)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B24,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Grass (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B10,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Grass (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B11,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Grass (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B12,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Deku Elevator Room Grass (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B50,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Deku Elevator Room Grass (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B51,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Snapping Turtle Grass (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B60,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Snapping Turtle Grass (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B61,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Snapping Turtle Grass (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B62,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Snapping Turtle Grass (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B63,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Snapping Turtle Grass (5)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B64,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Dragonfly Chest Room Grass (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B40,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Dragonfly Chest Room Grass (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B41,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Dragonfly Chest Room Grass (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B42,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BA1,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BA3,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BA5,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BA4,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (5)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BA9,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (6)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BA7,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (7)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BA0,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (8)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BA2,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (9)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BAA,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (10)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BA8,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple 2F Moving Flower Platform Room Grass (11)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121BA6,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Pre Boss Room Grass (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B00,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Pre Boss Room Grass (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B01,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Pre Boss Room Grass (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B02,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Pre Boss Room Grass (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B03,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Pre Boss Room Grass (5)": MMRLocationData(
        region="Woodfall Temple",
        address=0x121B04,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (1)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F00,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (2)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F01,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (3)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F02,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (4)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F03,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (5)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F04,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (6)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F05,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (7)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F06,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (8)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F07,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (9)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F08,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (10)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F09,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (11)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F0A,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (12)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F0B,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (13)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F0C,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (14)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F0D,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (15)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F0E,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Odolwas Lair Grass (16)": MMRLocationData(
        region="Odolwa's Lair",
        address=0x121F0F,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),

    # Southern Swamp Post Dungeon Grass Near Tourist Centre

    "Southern Swamp Post Dungeon Grass Near Tourist Centre (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x100000,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x100001,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x100002,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (4)": MMRLocationData(
        region="Southern Swamp",
        address=0x100003,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (5)": MMRLocationData(
        region="Southern Swamp",
        address=0x100004,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (6)": MMRLocationData(
        region="Southern Swamp",
        address=0x100005,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (7)": MMRLocationData(
        region="Southern Swamp",
        address=0x100006,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (8)": MMRLocationData(
        region="Southern Swamp",
        address=0x100007,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (9)": MMRLocationData(
        region="Southern Swamp",
        address=0x100008,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (10)": MMRLocationData(
        region="Southern Swamp",
        address=0x100009,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (11)": MMRLocationData(
        region="Southern Swamp",
        address=0x10000A,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Tourist Centre (12)": MMRLocationData(
        region="Southern Swamp",
        address=0x10000B,
        can_create=lambda options: grass_enabled(options)
    ),

    # Southern Swamp Post Dungeon Grass Near Witch Shop

    "Southern Swamp Post Dungeon Grass Near Witch Shop (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x102000,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x102001,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x102002,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (4)": MMRLocationData(
        region="Southern Swamp",
        address=0x102003,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (5)": MMRLocationData(
        region="Southern Swamp",
        address=0x102004,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (6)": MMRLocationData(
        region="Southern Swamp",
        address=0x102005,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (7)": MMRLocationData(
        region="Southern Swamp",
        address=0x102006,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (8)": MMRLocationData(
        region="Southern Swamp",
        address=0x102007,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (9)": MMRLocationData(
        region="Southern Swamp",
        address=0x102008,
        can_create=lambda options: grass_enabled(options)
    ),

    # Southern Swamp Post Dungeon Grass Near Witch Shop

    "Southern Swamp Post Dungeon Grass Near Witch Shop (10)": MMRLocationData(
        region="Southern Swamp",
        address=0x103000,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (11)": MMRLocationData(
        region="Southern Swamp",
        address=0x103001,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (12)": MMRLocationData(
        region="Southern Swamp",
        address=0x103002,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (13)": MMRLocationData(
        region="Southern Swamp",
        address=0x103003,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (14)": MMRLocationData(
        region="Southern Swamp",
        address=0x103004,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (15)": MMRLocationData(
        region="Southern Swamp",
        address=0x103005,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (16)": MMRLocationData(
        region="Southern Swamp",
        address=0x103006,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (17)": MMRLocationData(
        region="Southern Swamp",
        address=0x103007,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Grass Near Witch Shop (18)": MMRLocationData(
        region="Southern Swamp",
        address=0x103008,
        can_create=lambda options: grass_enabled(options)
    ),

    # Southern Swamp Post Dungeon Gossip Grass

    "Southern Swamp Post Dungeon Gossip Grass (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x120020,
        can_create=lambda options: grass_enabled(options)
    ),
    "Southern Swamp Post Dungeon Gossip Grass (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x120021,
        can_create=lambda options: grass_enabled(options)
    ),


        # Snowhead Temple Grass
    "Snowhead Temple Basement Grass (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x122140,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Basement Grass (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x122141,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Basement Grass (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x122142,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Basement Grass (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x122143,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Basement Grass (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x122144,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Basement Grass (6)": MMRLocationData(
        region="Snowhead Temple",
        address=0x122145,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Basement Grass (7)": MMRLocationData(
        region="Snowhead Temple",
        address=0x122146,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Basement Grass (8)": MMRLocationData(
        region="Snowhead Temple",
        address=0x122147,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Basement Grass (9)": MMRLocationData(
        region="Snowhead Temple",
        address=0x122148,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Basement Grass (10)": MMRLocationData(
        region="Snowhead Temple",
        address=0x122149,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
        # Stone Tower Temple Grass

    "Stone Tower Temple Entrance Room Grass (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x121600,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),

    "Stone Tower Temple Entrance Room Grass (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x121601,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Entrance Room Grass (3)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x121602,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Elegy Maze Grass (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x121623,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Elegy Maze Grass (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x121621,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Elegy Maze Grass (3)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x121624,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Elegy Maze Grass (4)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x121620,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Elegy Maze Grass (5)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x121622,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Elegy Maze Grass (6)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x121625,
        can_create=lambda options: grass_enabled(options, is_dungeon=True)
    ),
      # Clock Town Pots
    "Trading Post Pot": MMRLocationData(
        region="Clock Town",
        address=0x203400,
        can_create=lambda options: pot_enabled(options)
    ),
    "Sword School Night 3 Midnight Pots (1)": MMRLocationData(
        region="Clock Town",
        address=0x205403,
        can_create=lambda options: pot_enabled(options)
    ),
    "Sword School Night 3 Midnight Pots (2)": MMRLocationData(
        region="Clock Town",
        address=0x205402,
        can_create=lambda options: pot_enabled(options)
    ),
    "Sword School Night 3 Midnight Pots (3)": MMRLocationData(
        region="Clock Town",
        address=0x205404,
        can_create=lambda options: pot_enabled(options)
    ),
    "Sword School Night 3 Midnight Pots (4)": MMRLocationData(
        region="Clock Town",
        address=0x205401,
        can_create=lambda options: pot_enabled(options)
    ),
    "Sword School Night 3 Midnight Pots (5)": MMRLocationData(
        region="Clock Town",
        address=0x205400,
        can_create=lambda options: pot_enabled(options)
    ),
    "Top of Clock Tower Pots (1)": MMRLocationData(
        region="Clock Town",
        address=0x201900,
        can_create=lambda options: pot_enabled(options)
    ),
    "Top of Clock Tower Pots (2)": MMRLocationData(
        region="Clock Town",
        address=0x201901,
        can_create=lambda options: pot_enabled(options)
    ),
    "Top of Clock Tower Pots (3)": MMRLocationData(
        region="Clock Town",
        address=0x201902,
        can_create=lambda options: pot_enabled(options)
    ), 
    "Top of Clock Tower Pots (4)": MMRLocationData(
        region="Clock Town",
        address=0x201903,
        can_create=lambda options: pot_enabled(options)
    ),
    "Bombers Hideout Pots (1)": MMRLocationData(
        region="Clock Town",
        address=0x202901,
        can_create=lambda options: pot_enabled(options)
    ),
    "Bombers Hideout Pots (2)": MMRLocationData(
        region="Clock Town",
        address=0x202900,
        can_create=lambda options: pot_enabled(options)
    ),
    "Bombers Hideout Pots (3)": MMRLocationData(
        region="Clock Town",
        address=0x202902,
        can_create=lambda options: pot_enabled(options)
    ),
    "Bombers Hideout Pots (4)": MMRLocationData(
        region="Clock Town",
        address=0x202903,
        can_create=lambda options: pot_enabled(options)
    ),
    "Astral Observatory Pots (1)": MMRLocationData(
        region="Clock Town",
        address=0x202911,
        can_create=lambda options: pot_enabled(options)
    ),
    "Astral Observatory Pots (2)": MMRLocationData(
        region="Clock Town",
        address=0x202910,
        can_create=lambda options: pot_enabled(options)
    ),
    "Astral Observatory Pots (3)": MMRLocationData(
        region="Clock Town",
        address=0x202912,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Termina Field Pots
    
    "Termina Field Eastern Pillar Pot": MMRLocationData(
        region="Termina Field",
        address=0x202D00,
        can_create=lambda options: pot_enabled(options)
    ),
    "Termina Field Deku Business Scrub Grotto Pot": MMRLocationData(
        region="Termina Field",
        address=0x200790,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Southern Swamp Pots
    
    "Road to Southern Swamp Outside Archery Pots (1)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x2040F1,
        can_create=lambda options: pot_enabled(options)
    ),
    "Road to Southern Swamp Outside Archery Pots (2)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x2040F0,
        can_create=lambda options: pot_enabled(options)
    ),
    "Southern Swamp Beneath Witch Shop Pots (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x204522,
        can_create=lambda options: pot_enabled(options)
    ),
    "Southern Swamp Beneath Witch Shop Pots (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x204521,
        can_create=lambda options: pot_enabled(options)
    ),
    "Southern Swamp Beneath Witch Shop Pots (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x204520,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Swamp Spider House Pots
    
    "Swamp Spider House Main Room Pots (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202710,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Main Room Pots (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202711,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Main Room Pots (3)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202712,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Main Room Pots (4)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202713,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Main Room Pots (5)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202714,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Main Room Pots (6)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202715,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Main Room Pots (7)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202716,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Main Room Pots (8)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202717,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),    
    "Swamp Spider House Tablet Room Pots (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202730,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Tablet Room Pots (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202731,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),        

    "Swamp Spider House Giant Jar Room Pots (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202740,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Giant Jar Room Pots (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202741,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Giant Jar Room Pots (3)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202742,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Giant Jar Room Pots (4)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202743,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Giant Jar Room Pots (5)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202744,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Giant Jar Room Pots (6)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202745,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Giant Jar Room Pots (7)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202746,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Giant Jar Room Pots (8)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202747,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),    
    "Swamp Spider House Gold Room Pots (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202720,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Gold Room Pots (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202721,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Gold Room Pots (3)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202722,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Gold Room Pots (4)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202723,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Gold Room Pots (5)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202724,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Swamp Spider House Gold Room Pots (6)": MMRLocationData(
        region="Swamp Spider House",
        address=0x202725,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    
    # Deku Palace Pots
    "Deku Palace Right Side Upper Pots (1)": MMRLocationData(
        region="Deku Palace",
        address=0x202B10,
        can_create=lambda options: pot_enabled(options)
    ),
    "Deku Palace Right Side Upper Pots (2)": MMRLocationData(
        region="Deku Palace",
        address=0x202B11,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Butler Race Pots
    
    "Deku Butler Race Pots (1)": MMRLocationData(
        region="Deku Palace",
        address=0x205200,
        can_create=lambda options: pot_enabled(options)
    ),
    "Deku Butler Race Pots (2)": MMRLocationData(
        region="Deku Palace",
        address=0x205201,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Woodfall Pots
    
    "Woodfall Owl Pots (1)": MMRLocationData(
        region="Woodfall",
        address=0x204601,
        can_create=lambda options: pot_enabled(options)
    ),
    "Woodfall Owl Pots (2)": MMRLocationData(
        region="Woodfall",
        address=0x204600,
        can_create=lambda options: pot_enabled(options)
    ),
    "Woodfall Owl Pots (3)": MMRLocationData(
        region="Woodfall",
        address=0x204602,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Woodfall Temple Pots
    
    "Woodfall Temple Entrance Pot": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B20,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Pots (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B10,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Pots (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B11,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Pots (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B12,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Pots (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B13,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Pots (5)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B14,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Pots (6)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B15,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Pots (7)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B17,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Main Room Pots (8)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B18,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Deku Elevator Pots (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B51,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Deku Elevator Pots (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B50,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Deku Elevator Pots (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B53,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Deku Elevator Pots (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B52,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Gekko Pots (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B83,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Gekko Pots (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B82,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Gekko Pots (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B81,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Gekko Pots (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B80,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Left Side Bridge Pots (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B30,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Left Side Bridge Pots (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B31,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Pre Boss Pots (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B01,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Woodfall Temple Pre Boss Pots (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x201B00,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Southern Swamp Post Dungeon Witch Pot (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x200020,
        can_create=lambda options: pot_enabled(options)
    ),
    "Southern Swamp Post Dungeon Witch Pot (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x200021,
        can_create=lambda options: pot_enabled(options)
    ),
    "Southern Swamp Post Dungeon Witch Pot (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x200022,
        can_create=lambda options: pot_enabled(options)
    ),
    # Mountain Village Pots
    
    "Mountain Village Rooftop Pot": MMRLocationData(
        region="Mountain Village",
        address=0x205000,
        can_create=lambda options: pot_enabled(options)
    ),
    "Mountain Village Pots (1)": MMRLocationData(
        region="Mountain Village",
        address=0x2050F0,
        can_create=lambda options: pot_enabled(options)
    ),
    "Mountain Village Pots (2)": MMRLocationData(
        region="Mountain Village",
        address=0x2050F1,
        can_create=lambda options: pot_enabled(options)
    ),
    "Mountain Smithy Pots Inside at Night (1)": MMRLocationData(
        region="Mountain Village",
        address=0x202CF0,
        can_create=lambda options: pot_enabled(options)
    ),
    "Mountain Smithy Pots Inside at Night (2)": MMRLocationData(
        region="Mountain Village",
        address=0x202CF1,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Goron Racetrack Pots
    
    "Goron Racetrack Pots (1)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B14,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (2)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B15,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (3)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B17,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (4)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B16,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (5)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B12,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (6)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B10,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (7)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B13,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (8)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B0F,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (9)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B11,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (10)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B00,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (11)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B02,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (12)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B05,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (13)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B04,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (14)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B03,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (15)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B07,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (16)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B06,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (17)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B09,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (18)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B08,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (19)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B0A,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (20)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B01,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (21)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B0E,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (22)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B18,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (23)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B19,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (24)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B1A,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (25)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B1D,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (26)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B1B,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (27)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B1C,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (28)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B0D,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (29)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B0C,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Racetrack Pots (30)": MMRLocationData(
        region="Goron Racetrack",
        address=0x206B0B,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Goron Shrine Pots
    
    "Goron Shrine Pots (1)": MMRLocationData(
        region="Goron Shrine",
        address=0x203204,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Shrine Pots (2)": MMRLocationData(
        region="Goron Shrine",
        address=0x203203,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Shrine Pots (3)": MMRLocationData(
        region="Goron Shrine",
        address=0x203207,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Shrine Pots (4)": MMRLocationData(
        region="Goron Shrine",
        address=0x203202,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Shrine Pots (5)": MMRLocationData(
        region="Goron Shrine",
        address=0x203200,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Shrine Pots (6)": MMRLocationData(
        region="Goron Shrine",
        address=0x203205,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Shrine Pots (7)": MMRLocationData(
        region="Goron Shrine",
        address=0x203201,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Shrine Pots (8)": MMRLocationData(
        region="Goron Shrine",
        address=0x203206,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Shrine Pots (9)": MMRLocationData(
        region="Goron Shrine",
        address=0x203210,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Shrine Pots (10)": MMRLocationData(
        region="Goron Shrine",
        address=0x203211,
        can_create=lambda options: pot_enabled(options)
    ),
    "Goron Shrine Pots (11)": MMRLocationData(
        region="Goron Shrine",
        address=0x203212,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Snowhead Temple Pots
    "Snowhead Temple Entrance Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202100,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Entrance Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202101,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),

    "Snowhead Temple Blue Door Lava Bridge Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202123,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Blue Door Lava Bridge Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202125,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Blue Door Lava Bridge Pots (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202124,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Blue Door Lava Bridge Pots (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202126,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Blue Door Lava Bridge Pots (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202122,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Blue Door Lava Bridge Pots (6)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202120,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Blue Door Lava Bridge Pots (7)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202121,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Main Room Pots Basement (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202146,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Main Room Pots Basement (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202147,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Main Room Scarecrow Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202140,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Main Room Scarecrow Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202141,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x20215B,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x20215C,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x20215A,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202157,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202158,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (6)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202159,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (7)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202156,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (8)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202153,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (9)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202151,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (10)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202155,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (11)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202154,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (12)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202150,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Frozen Green Door Pots (13)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202152,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Orange Door Push Block Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202131,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Orange Door Push Block Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202130,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Orange Door Push Block Ghost Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202132,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Orange Door Push Block Ghost Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202133,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),         
    "Snowhead Temple Locked Grey Door Wolfos Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202112,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Locked Grey Door Wolfos Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202113,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Locked Grey Door Wolfos Pots (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202110,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Locked Grey Door Wolfos Pots (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202114,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Locked Grey Door Wolfos Pots (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202111,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Goron Pound Puzzle Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202181,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Goron Pound Puzzle Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202180,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Main Room 2nd Floor Bridge Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202144,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Main Room 2nd Floor Bridge Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202145,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Main Room 4th Floor Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202143,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple Main Room 4th Floor Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x202142,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple 4th Floor Wizzrobe Pots (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2021C0,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple 4th Floor Wizzrobe Pots (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2021C1,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple 4th Floor Wizzrobe Pots (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2021C2,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple 4th Floor Wizzrobe Pots (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2021C3,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Snowhead Temple 4th Floor Wizzrobe Pots (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2021C4,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),        
    "Goht Boss Room Pots (1)": MMRLocationData(
        region="Goht's Lair",
        address=0x20440C,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (2)": MMRLocationData(
        region="Goht's Lair",
        address=0x20440D,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (3)": MMRLocationData(
        region="Goht's Lair",
        address=0x204408,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (4)": MMRLocationData(
        region="Goht's Lair",
        address=0x204400,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (5)": MMRLocationData(
        region="Goht's Lair",
        address=0x204402,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (6)": MMRLocationData(
        region="Goht's Lair",
        address=0x204409,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (7)": MMRLocationData(
        region="Goht's Lair",
        address=0x204407,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (8)": MMRLocationData(
        region="Snowhead Temple",
        address=0x204406,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (9)": MMRLocationData(
        region="Goht's Lair",
        address=0x20440A,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (10)": MMRLocationData(
        region="Goht's Lair",
        address=0x204401,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (11)": MMRLocationData(
        region="Goht's Lair",
        address=0x204403,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (12)": MMRLocationData(
        region="Goht's Lair",
        address=0x20440B,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (13)": MMRLocationData(
        region="Goht's Lair",
        address=0x204404,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Goht Boss Room Pots (14)": MMRLocationData(
        region="Goht's Lair",
        address=0x204405,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    
    "Mountain Village Springtime Pots (1)": MMRLocationData(
        region="Mountain Village",
        address=0x205AF0,
        can_create=lambda options: pot_enabled(options)
    ),         
    "Mountain Village Springtime Pots (2)": MMRLocationData(
        region="Mountain Village",
        address=0x205AF1   ,
        can_create=lambda options: pot_enabled(options)
    ),   
    "Mountain Village Springtime Pots (3)": MMRLocationData(
        region="Mountain Village",
        address=0x205A00,
        can_create=lambda options: pot_enabled(options)
    ),   


    # Romani Ranch Pots
    
    "Romani Ranch Baby Cuccoos Pots (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x2042F0,
        can_create=lambda options: pot_enabled(options)
    ),
    "Romani Ranch Baby Cuccoos Pots (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x2042F1,
        can_create=lambda options: pot_enabled(options)
    ),
    "Romani Ranch Doggy Racetrack Pots (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x204102,
        can_create=lambda options: pot_enabled(options)
    ),
    "Romani Ranch Doggy Racetrack Pots (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x204101,
        can_create=lambda options: pot_enabled(options)
    ),
    "Romani Ranch Doggy Racetrack Pots (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x204100,
        can_create=lambda options: pot_enabled(options)
    ),
    "Romani Ranch Doggy Racetrack Pots (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x204103,
        can_create=lambda options: pot_enabled(options)
    ),
    # Great Bay Coast Pots
    
    "Great Bay Coast Behind Marine Lab Pots (1)": MMRLocationData(
        region="Great Bay",
        address=0x203707,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Behind Marine Lab Pots (2)": MMRLocationData(
        region="Great Bay",
        address=0x203709,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Behind Marine Lab Pots (3)": MMRLocationData(
        region="Great Bay",
        address=0x203708,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Behind Marine Lab Pots (4)": MMRLocationData(
        region="Great Bay",
        address=0x20370E,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Rock Pools Pots (1)": MMRLocationData(
        region="Great Bay",
        address=0x203704,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Rock Pools Pots (2)": MMRLocationData(
        region="Great Bay",
        address=0x20370B,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Lower Rock Wall Pots (1)": MMRLocationData(
        region="Great Bay",
        address=0x20370D,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Lower Rock Wall Pots (2)": MMRLocationData(
        region="Great Bay",
        address=0x203706,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Lower Rock Wall Pots (3)": MMRLocationData(
        region="Great Bay",
        address=0x203705,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Lower Rock Wall Pots (4)": MMRLocationData(
        region="Great Bay",
        address=0x20370C,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Upper Rock Wall Pots (1)": MMRLocationData(
        region="Great Bay",
        address=0x203702,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Upper Rock Wall Pots (2)": MMRLocationData(
        region="Great Bay",
        address=0x203701,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Upper Rock Wall Pots (3)": MMRLocationData(
        region="Great Bay",
        address=0x203700,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Beside Pirates Fortress Pots (1)": MMRLocationData(
        region="Great Bay",
        address=0x203703,
        can_create=lambda options: pot_enabled(options)
    ),
    "Great Bay Coast Beside Pirates Fortress Pots (2)": MMRLocationData(
        region="Great Bay",
        address=0x20370A,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Ocean Spiderhouse Pots
    
    "Ocean Spiderhouse Bottom of Ramp Pots (1)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202803,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Bottom of Ramp Pots (2)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202800,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Bottom of Ramp Pots (3)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202801,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Bottom of Ramp Pots (4)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202802,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Main Room Lower Pots (1)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202813,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Main Room Lower Pots (2)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202814,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Main Room Lower Pots (3)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202810,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Main Room Lower Pots (4)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202811,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Main Room Lower Pots (5)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202812,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Crate Room Pots (1)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202857,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Crate Room Pots (2)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202851,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Crate Room Pots (3)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202856,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Crate Room Pots (4)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202855,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Crate Room Pots (5)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202850,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Crate Room Pots (6)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202854,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Crate Room Pots (7)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202852,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Crate Room Pots (8)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202853,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Coloured Skulls Room Pots (1)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202831,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ocean Spiderhouse Coloured Skulls Room Pots (2)": MMRLocationData(
        region="Ocean Spider House",
        address=0x202830,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),


    # Pinnacle Rock
    
    "Pinnacle Rock Pots (1)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x202500,
        can_create=lambda options: pot_enabled(options)
    ),
    "Pinnacle Rock Pots (2)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x202501,
        can_create=lambda options: pot_enabled(options)
    ),
    "Pinnacle Rock Pots (3)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x202502,
        can_create=lambda options: pot_enabled(options)
    ),
    "Pinnacle Rock Pots (4)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x202503,
        can_create=lambda options: pot_enabled(options)
    ),
    "Pinnacle Rock Pots (5)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x202504,
        can_create=lambda options: pot_enabled(options)
    ),
    "Pinnacle Rock Pots (6)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x202505,
        can_create=lambda options: pot_enabled(options)
    ),
    "Pinnacle Rock Pots (7)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x202506,
        can_create=lambda options: pot_enabled(options)
    ),
    "Pinnacle Rock Pots (8)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x202507,
        can_create=lambda options: pot_enabled(options)
    ),
    "Pinnacle Rock Pots (9)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x202508,
        can_create=lambda options: pot_enabled(options)
    ),
    "Pinnacle Rock Pots (10)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x202509,
        can_create=lambda options: pot_enabled(options)
    ),
    "Pinnacle Rock Pots (11)": MMRLocationData(
        region="Pinnacle Rock",
        address=0x20250A,
        can_create=lambda options: pot_enabled(options)
    ),
    
        # Pirates' Fortress Pots
    
    "Pirates Fortress Sewers Cage Room Pots (1)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2023B0,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Sewers Cage Room Pots (2)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2023B1,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Sewers After Gate Hidden Ladder Pots (1)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2023A1,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Sewers After Gate Hidden Ladder Pots (2)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2023A0,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Sewers Exit Pots (1)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x202391,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Sewers Exit Pots (2)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x202390,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Sewers Exit Pots (3)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x202392,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Interior Underwater Chest Room Pots (1)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x202360,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Interior Underwater Chest Room Pots (2)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x202361,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Interior Underwater Chest Room Pots (3)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x202362,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Interior Room Past Green Guard Pots (1)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x202380,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Interior Room Past Green Guard Pots (2)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x202381,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Interior Room Past Green Guard Pots (3)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x202382,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Interior Upper Beehive Room Pots (1)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x202331,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Interior Upper Beehive Room Pots (2)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x202330,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Interior Room Past Pink Guard Pots (1)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x2023D1,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Pirates Fortress Interior Room Past Pink Guard Pots (2)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x2023D0,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),

    # Zora Cape Pots


    "Zora Cape Like Like Pool Pots (1)": MMRLocationData(
        region="Zora Cape",
        address=0x203800,
        can_create=lambda options: pot_enabled(options)
    ),
    "Zora Cape Like Like Pool Pots (2)": MMRLocationData(
        region="Zora Cape",
        address=0x203801,
        can_create=lambda options: pot_enabled(options)
    ),
    "Zora Cape Owl Pots (1)": MMRLocationData(
        region="Zora Cape",
        address=0x203802,
        can_create=lambda options: pot_enabled(options)
    ),
    "Zora Cape Owl Pots (2)": MMRLocationData(
        region="Zora Cape",
        address=0x203803,
        can_create=lambda options: pot_enabled(options)
    ),
    "Zora Cape Owl Pots (3)": MMRLocationData(
        region="Zora Cape",
        address=0x203804,
        can_create=lambda options: pot_enabled(options)
    ),
    "Zora Cape Owl Pots (4)": MMRLocationData(
        region="Zora Cape",
        address=0x203805,
        can_create=lambda options: pot_enabled(options)
    ),
    "Zora Cape Jar Game Pots (1)": MMRLocationData(
        region="Zora Cape",
        address=0x203811,
        can_create=lambda options: pot_enabled(options)
    ),
    "Zora Cape Jar Game Pots (2)": MMRLocationData(
        region="Zora Cape",
        address=0x203812,
        can_create=lambda options: pot_enabled(options)
    ),
    "Zora Cape Jar Game Pots (3)": MMRLocationData(
        region="Zora Cape",
        address=0x203813,
        can_create=lambda options: pot_enabled(options)
    ),
    "Zora Cape Jar Game Pots (4)": MMRLocationData(
        region="Zora Cape",
        address=0x203814,
        can_create=lambda options: pot_enabled(options)
    ),
    "Zora Cape Jar Game Pots (5)": MMRLocationData(
        region="Zora Cape",
        address=0x203815,
        can_create=lambda options: pot_enabled(options)
    ),        

    # Great Bay Temple Pots
    
    "Great Bay Temple Above Blender Pots (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204901,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Above Blender Pots (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204900,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Room Behind 1F Waterfall Pots (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049C3,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Room Behind 1F Waterfall Pots (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049C2,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Room Behind 1F Waterfall Pots (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049C0,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Room Behind 1F Waterfall Pots (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049C1,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204910,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204911,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    #Fairy Pot so no item
    # "Great Bay Temple Red Green Pipe Tunnel Room Pots (3)": MMRLocationData(
    #     region="Great Bay Temple",
    #     address=0x20491B,
    #     can_create=lambda options: pot_enabled(options, is_dungeon=True)
    # ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x20491A,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (5)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204913,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (6)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204915,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (7)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204916,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (8)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204917,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (9)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204914,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (10)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204912,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (11)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204918,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Green Pipe Tunnel Room Pots (12)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204919,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Before Gekko Room Pots (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204945,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Before Gekko Room Pots (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204940,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Before Gekko Room Pots (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204946,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Before Gekko Room Pots (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204941,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Before Gekko Room Pots (5)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204943,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Before Gekko Room Pots (6)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204944,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Before Gekko Room Pots (7)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204942,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Valve Underwater Pots (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204963,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Valve Underwater Pots (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204962,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Valve Underwater Pots (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204961,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Red Valve Underwater Pots (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204960,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049E3,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049EA,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049E8,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049E1,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (5)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049E5,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (6)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049E0,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (7)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049E4,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (8)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049E6,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (9)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049E7,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (10)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049E2,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (11)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049EB,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Behind Locked Door Pots (12)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049E9,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Wart Room Pots (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204972,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Wart Room Pots (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204970,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Wart Room Pots (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204976,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Wart Room Pots (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204971,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Wart Room Pots (5)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204977,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Wart Room Pots (6)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204974,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Wart Room Pots (7)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204975,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Wart Room Pots (8)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204973,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Green Pipe Frozen Waterwheel Pots (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204996,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Green Pipe Frozen Waterwheel Pots (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204997,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Green Pipe Frozen Waterwheel Pots (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204990,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Green Pipe Frozen Waterwheel Pots (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204991,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Green Pipe Frozen Waterwheel Pots (5)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204994,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Green Pipe Frozen Waterwheel Pots (6)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204995,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Green Pipe Frozen Waterwheel Pots (7)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204993,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Green Pipe Frozen Waterwheel Pots (8)": MMRLocationData(
        region="Great Bay Temple",
        address=0x204992,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Seesaw Room Pots (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049A2,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Seesaw Room Pots (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049A0,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Seesaw Room Pots (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049A1,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Pots Underneath Boss Door Platform (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049B5,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Pots Underneath Boss Door Platform (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049B3,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Pots Underneath Boss Door Platform (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049B1,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Pots Underneath Boss Door Platform (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049B7,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Pots Underneath Boss Door Platform (5)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049B6,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Pots Underneath Boss Door Platform (6)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049B0,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Pots Underneath Boss Door Platform (7)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049B4,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Pots Underneath Boss Door Platform (8)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2049B2,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Gyorg Pots (1)": MMRLocationData(
        region="Gyorg's Lair",
        address=0x205F07,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Gyorg Pots (2)": MMRLocationData(
        region="Gyorg's Lair",
        address=0x205F06,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Gyorg Pots (3)": MMRLocationData(
        region="Gyorg's Lair",
        address=0x205F05,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Gyorg Pots (4)": MMRLocationData(
        region="Gyorg's Lair",
        address=0x205F04,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Gyorg Pots (5)": MMRLocationData(
        region="Gyorg's Lair",
        address=0x205F00,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Gyorg Pots (6)": MMRLocationData(
        region="Gyorg's Lair",
        address=0x205F03,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Gyorg Pots (7)": MMRLocationData(
        region="Gyorg's Lair",
        address=0x205F02,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Great Bay Temple Gyorg Pots (8)": MMRLocationData(
        region="Gyorg's Lair",
        address=0x205F01,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),

    # Ikana Road and Gravyard Pots
    
    "Road To Ikana Scarecrow Pillar Pot": MMRLocationData(
        region="Road to Ikana",
        address=0x205300,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 1 Grave Pots (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C01,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 1 Grave Pots (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C00,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 1 Grave Pots (3)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C10,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 1 Grave Pots (4)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C11,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 1 Grave Pots (5)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C12,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 2 Entrance Grave Pot": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C02,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 2 Invisible Path Entryway Pots (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C32,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 2 Invisible Path Entryway Pots (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C33,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 2 Invisible Path Pots (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C30,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 2 Invisible Path Pots (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C31,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 2 Invisible Path Pots (3)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C35,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 2 Invisible Path Pots (4)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x200C34,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 3 Pots (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x203001,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 3 Pots (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x203003,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 3 Pots (3)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x203005,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 3 Pots (4)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x203002,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 3 Pots (5)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x203000,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 3 Pots (6)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x203004,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 3 Pots (7)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x203009,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 3 Pots (8)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x203007,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 3 Pots (9)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x203006,
        can_create=lambda options: pot_enabled(options)
    ),
    "Ikana Graveyard Day 3 Pots (10)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x203008,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Secret Shrine Pots
    
    "Secret Shrine Entrance Pots (1)": MMRLocationData(
        region="Secret Shrine",
        address=0x206001,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Secret Shrine Entrance Pots (2)": MMRLocationData(
        region="Secret Shrine",
        address=0x206002,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Secret Shrine Entrance Pots (3)": MMRLocationData(
        region="Secret Shrine",
        address=0x206000,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Secret Shrine Underwater Pots (1)": MMRLocationData(
        region="Secret Shrine",
        address=0x206015,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Secret Shrine Underwater Pots (2)": MMRLocationData(
        region="Secret Shrine",
        address=0x206014,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Secret Shrine Underwater Pots (3)": MMRLocationData(
        region="Secret Shrine",
        address=0x206013,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Secret Shrine Underwater Pots (4)": MMRLocationData(
        region="Secret Shrine",
        address=0x206012,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Secret Shrine Underwater Pots (5)": MMRLocationData(
        region="Secret Shrine",
        address=0x206010,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Secret Shrine Underwater Pots (6)": MMRLocationData(
        region="Secret Shrine",
        address=0x206011,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),

    #Sakons Hideout
    "Sakons Hideout Pots (1)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x204F10,
        can_create=lambda options: pot_enabled(options)
    ),
    "Sakons Hideout Pots (2)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x204F11,
        can_create=lambda options: pot_enabled(options)
    ),          
    "Sakons Hideout Pots (3)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x204F12,
        can_create=lambda options: pot_enabled(options)
    ),
    "Sakons Hideout Pots (4)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x204F13,
        can_create=lambda options: pot_enabled(options)
    ),
    "Sakons Hideout Pots (5)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x204F14,
        can_create=lambda options: pot_enabled(options)
    ),      

    
    # Ikana Castle
    "Ikana Castle Exterior Corner Pot": MMRLocationData(
        region="Ikana Castle",
        address=0x201D00,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Frozen Eyes Room Pots (1)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D11,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Frozen Eyes Room Pots (2)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D10,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Left Side Falling Ceiling Room Pots (1)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D20,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Left Side Falling Ceiling Room Pots (2)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D21,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Left Side Broken Floor Room Pots (1)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D42,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Left Side Broken Floor Room Pots (2)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D41,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Left Side Broken Floor Room Pots (3)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D40,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Left Side Broken Floor Room Pots (4)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D43,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Left Side Staircase Pots (1)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D60,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Left Side Staircase Pots (2)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D61,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Right Side Staircase Pots (1)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D71,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Right Side Staircase Pots (2)": MMRLocationData(
        region="Ikana Castle",
        address=0x201D70,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Throne Room Pots (1)": MMRLocationData(
        region="Ikana Castle",
        address=0x205601,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Throne Room Pots (2)": MMRLocationData(
        region="Ikana Castle",
        address=0x205600,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Throne Room Pots (3)": MMRLocationData(
        region="Ikana Castle",
        address=0x205603,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Throne Room Pots (4)": MMRLocationData(
        region="Ikana Castle",
        address=0x205602,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Throne Room Pots (5)": MMRLocationData(
        region="Ikana Castle",
        address=0x205611,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Throne Room Pots (6)": MMRLocationData(
        region="Ikana Castle",
        address=0x205612,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Throne Room Pots (7)": MMRLocationData(
        region="Ikana Castle",
        address=0x205613,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Ikana Castle Throne Room Pots (8)": MMRLocationData(
        region="Ikana Castle",
        address=0x205610,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    
    # Well Pots
    
    "Well Left Side Back Room Pots (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B52,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Left Side Back Room Pots (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B50,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Left Side Back Room Pots (3)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B54,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Left Side Back Room Pots (4)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B51,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Left Side Back Room Pots (5)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B53,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Right Side Before Chest Room Pots (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B69,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Right Side Before Chest Room Pots (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B68,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Right Side Before Chest Room Pots (3)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B67,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Right Side Before Chest Room Pots (4)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B66,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Right Side Before Chest Room Pots (5)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B65,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Right Side Before Chest Room Pots (6)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B64,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Right Side Before Chest Room Pots (7)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B63,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Right Side Before Chest Room Pots (8)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B62,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Right Side Before Chest Room Pots (9)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B61,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Right Side Before Chest Room Pots (10)": MMRLocationData(
        region="Beneath the Well",
        address=0x204B60,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Big Poe Pots (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x204BC2,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Big Poe Pots (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x204BC3,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Big Poe Pots (3)": MMRLocationData(
        region="Beneath the Well",
        address=0x204BC1,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Well Big Poe Pots (4)": MMRLocationData(
        region="Beneath the Well",
        address=0x204BC0,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),


    # Stone Tower Climb Pots
    "Stone Tower Climb Pots (1)": MMRLocationData(
        region="Stone Tower",
        address=0x205815,
        can_create=lambda options: pot_enabled(options)    
     ),
    "Stone Tower Climb Pots (2)": MMRLocationData(
        region="Stone Tower",
        address=0x205816,
        can_create=lambda options: pot_enabled(options)    
    ),  
    # Stone Tower Lower Scarecrow Pots
    "Stone Tower Lower Scarecrow Pots (1)": MMRLocationData(
        region="Stone Tower",
        address=0x205805,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (2)": MMRLocationData(
        region="Stone Tower",
        address=0x205814,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (3)": MMRLocationData(
        region="Stone Tower",
        address=0x205808,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (4)": MMRLocationData(
        region="Stone Tower",
        address=0x205807,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (5)": MMRLocationData(
        region="Stone Tower",
        address=0x205804,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (6)": MMRLocationData(
        region="Stone Tower",
        address=0x205806,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (7)": MMRLocationData(
        region="Stone Tower",
        address=0x205802,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (8)": MMRLocationData(
        region="Stone Tower",
        address=0x205801,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (9)": MMRLocationData(
        region="Stone Tower",
        address=0x205803,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (10)": MMRLocationData(
        region="Stone Tower",
        address=0x205800,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (11)": MMRLocationData(
        region="Stone Tower",
        address=0x205809,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Lower Scarecrow Pots (12)": MMRLocationData(
        region="Stone Tower",
        address=0x20580A,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Upper Scarecrow Pots (1)": MMRLocationData(
        region="Stone Tower",
        address=0x205812,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Upper Scarecrow Pots (2)": MMRLocationData(
        region="Stone Tower",
        address=0x205813,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Upper Scarecrow Pots (3)": MMRLocationData(
        region="Stone Tower",
        address=0x205811,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Upper Scarecrow Pots (4)": MMRLocationData(
        region="Stone Tower",
        address=0x20580F,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Upper Scarecrow Pots (5)": MMRLocationData(
        region="Stone Tower",
        address=0x205810,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Upper Scarecrow Pots (6)": MMRLocationData(
        region="Stone Tower",
        address=0x20580E,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Upper Scarecrow Pots (7)": MMRLocationData(
        region="Stone Tower",
        address=0x20580C,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Upper Scarecrow Pots (8)": MMRLocationData(
        region="Stone Tower",
        address=0x20580D,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Upper Scarecrow Pots (9)": MMRLocationData(
        region="Stone Tower",
        address=0x20580B,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Owl Pots (1)": MMRLocationData(
        region="Stone Tower",
        address=0x20581A,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Owl Pots (2)": MMRLocationData(
        region="Stone Tower",
        address=0x205817,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Owl Pots (3)": MMRLocationData(
        region="Stone Tower",
        address=0x205818,
        can_create=lambda options: pot_enabled(options)
    ),
    "Stone Tower Owl Pots (4)": MMRLocationData(
        region="Stone Tower",
        address=0x205819,
        can_create=lambda options: pot_enabled(options)
    ),
    
    # Stone Tower Temple Pots
    
    "Stone Tower Temple Entrance Pots (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201601,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Entrance Pots (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201600,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Basement Armos Pots (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201645,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Basement Armos Pots (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201642,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Basement Armos Pots (3)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201644,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Basement Armos Pots (4)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201643,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Basement Armos Pots (5)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201646,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Basement Armos Pots (6)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201641,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Basement Armos Pots (7)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201640,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Basement Armos Pots (8)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201647,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Right Side Near Locked Door Pots (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201635,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Right Side Near Locked Door Pots (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201636,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Right Side Underwater Pots (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201633,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Right Side Underwater Pots (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201634,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Right Side Underwater Pots (3)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201632,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Right Side Underwater Pots (4)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201630,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Right Side Underwater Pots (5)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201631,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Mirror Room Pots (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201671,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Mirror Room Pots (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201670,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Deku Updraft Pots (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201693,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Deku Updraft Pots (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201690,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Deku Updraft Pots (3)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201692,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Deku Updraft Pots (4)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201691,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Spike Roller Pots (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201680,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Spike Roller Pots (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201681,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Spike Roller Pots (3)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201682,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Spike Roller Pots (4)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201686,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Spike Roller Pots (5)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201683,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Spike Roller Pots (6)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201687,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Spike Roller Pots (7)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201685,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Stone Tower Temple Lower Spike Roller Pots (8)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201684,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    
    # Inverted Stone Tower Temple Pots
    
    "Inverted Stone Tower Bean Pots (1)": MMRLocationData(
        region="Stone Tower",
        address=0x205903,
        can_create=lambda options: pot_enabled(options)
    ),
    "Inverted Stone Tower Bean Pots (2)": MMRLocationData(
        region="Stone Tower",
        address=0x205904,
        can_create=lambda options: pot_enabled(options)
    ),
    "Inverted Stone Tower Bean Pots (3)": MMRLocationData(
        region="Stone Tower",
        address=0x205900,
        can_create=lambda options: pot_enabled(options)
    ),
    "Inverted Stone Tower Bean Pots (4)": MMRLocationData(
        region="Stone Tower",
        address=0x205901,
        can_create=lambda options: pot_enabled(options)
    ),
    "Inverted Stone Tower Bean Pots (5)": MMRLocationData(
        region="Stone Tower",
        address=0x205902,
        can_create=lambda options: pot_enabled(options)
    ),
    "Inverted Stone Tower Temple Updraft Pots (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x201832,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Updraft Pots (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201833,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Updraft Pots (3)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201835,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Updraft Pots (4)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201834,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Updraft Pots (5)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201831,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Updraft Pots (6)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201830,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Gomess Pots (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x2018B3,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Gomess Pots (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x2018B0,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Gomess Pots (3)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x2018B1,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Gomess Pots (4)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x2018B2,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Lower Bridge Room Pots (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201811,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Lower Bridge Room Pots (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201810,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Small Poe Room Pots (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201822,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Small Poe Room Pots (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201820,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Small Poe Room Pots (3)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201821,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Small Poe Room Pots (4)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201823,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Wizzrobe Room Pots (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201840,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Wizzrobe Room Pots (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201843,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Wizzrobe Room Pots (3)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201844,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Wizzrobe Room Pots (4)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201841,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Wizzrobe Room Pots (5)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201842,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (Flying) (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x20188A,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (Flying) (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x20188B,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (Flying) (3)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201889,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (Flying) (4)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201888,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201883,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201881,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (3)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201885,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (4)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201887,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (5)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201884,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (6)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201886,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (7)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201880,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Inverted Stone Tower Temple Pre Boss Pots (8)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x201882,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),

    
    # Moon Trial Pots
    
    "Moon Goron Trial Pots (1)": MMRLocationData(
        region="The Moon",
        address=0x203F08,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (2)": MMRLocationData(
        region="The Moon",
        address=0x203F07,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (3)": MMRLocationData(
        region="The Moon",
        address=0x203F09,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (4)": MMRLocationData(
        region="The Moon",
        address=0x203F0A,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (5)": MMRLocationData(
        region="The Moon",
        address=0x203F00,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (6)": MMRLocationData(
        region="The Moon",
        address=0x203F01,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (7)": MMRLocationData(
        region="The Moon",
        address=0x203F04,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (8)": MMRLocationData(
        region="The Moon",
        address=0x203F02,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (9)": MMRLocationData(
        region="The Moon",
        address=0x203F03,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (10)": MMRLocationData(
        region="The Moon",
        address=0x203F0E,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (11)": MMRLocationData(
        region="The Moon",
        address=0x203F0C,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (12)": MMRLocationData(
        region="The Moon",
        address=0x203F0B,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (13)": MMRLocationData(
        region="The Moon",
        address=0x203F0D,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (14)": MMRLocationData(
        region="The Moon",
        address=0x203F06,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Goron Trial Pots (15)": MMRLocationData(
        region="The Moon",
        address=0x203F05,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Link Trial Pots (1)": MMRLocationData(
        region="The Moon",
        address=0x206602,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Link Trial Pots (2)": MMRLocationData(
        region="The Moon",
        address=0x206603,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Link Trial Pots (3)": MMRLocationData(
        region="The Moon",
        address=0x206601,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Link Trial Pots (4)": MMRLocationData(
        region="The Moon",
        address=0x206600,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Link Trial Pots (5)": MMRLocationData(
        region="The Moon",
        address=0x206606,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Link Trial Pots (6)": MMRLocationData(
        region="The Moon",
        address=0x206604,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Link Trial Pots (7)": MMRLocationData(
        region="The Moon",
        address=0x206605,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Moon Link Trial Pots (8)": MMRLocationData(
        region="The Moon",
        address=0x206607,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Majora Lair Pots (1)": MMRLocationData(
        region="The Moon",
        address=0x200B00,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),
    "Majora Lair Pots (2)": MMRLocationData(
        region="The Moon",
        address=0x200B01,
        can_create=lambda options: pot_enabled(options, is_dungeon=True)
    ),

    # Hitspots

    # South Clock Town Targets Hitspot
    "South Clock Town Targets Hitspot (0)": MMRLocationData(
        region="Clock Town",
        address=0x156F02,
        can_create=lambda options: options.hitsanity.value
    ),
    "South Clock Town Targets Hitspot (1)": MMRLocationData(
        region="Clock Town",
        address=0x156F00,
        can_create=lambda options: options.hitsanity.value
    ),
    "South Clock Town Targets Hitspot (2)": MMRLocationData(
        region="Clock Town",
        address=0x156F01,
        can_create=lambda options: options.hitsanity.value
    ),

    # East Clock Town Targets And Basket Hitspots
    "East Clock Town Targets And Basket Hitspots (0)": MMRLocationData(
        region="Clock Town",
        address=0x156C22,
        can_create=lambda options: options.hitsanity.value
    ),
    "East Clock Town Targets And Basket Hitspots (1)": MMRLocationData(
        region="Clock Town",
        address=0x156C21,
        can_create=lambda options: options.hitsanity.value
    ),
    "East Clock Town Targets And Basket Hitspots (2)": MMRLocationData(
        region="Clock Town",
        address=0x156C20,
        can_create=lambda options: options.hitsanity.value
    ),
    "East Clock Town Targets And Basket Hitspots (3)": MMRLocationData(
        region="Clock Town",
        address=0x156C02,
        can_create=lambda options: options.hitsanity.value
    ),
    "East Clock Town Targets And Basket Hitspots (4)": MMRLocationData(
        region="Clock Town",
        address=0x156C10,
        can_create=lambda options: options.hitsanity.value
    ),
    "East Clock Town Targets And Basket Hitspots (5)": MMRLocationData(
        region="Clock Town",
        address=0x156C00,
        can_create=lambda options: options.hitsanity.value
    ),
    "East Clock Town Targets And Basket Hitspots (6)": MMRLocationData(
        region="Clock Town",
        address=0x156C01,
        can_create=lambda options: options.hitsanity.value
    ),
    "East Clock Town Targets And Basket Hitspots (7)": MMRLocationData(
        region="Clock Town",
        address=0x156C11,
        can_create=lambda options: options.hitsanity.value
    ),
    "East Clock Town Targets And Basket Hitspots (8)": MMRLocationData(
        region="Clock Town",
        address=0x156C12,
        can_create=lambda options: options.hitsanity.value
    ),

    # Sword School Gong
    "Sword School Gong": MMRLocationData(
        region="Clock Town",
        address=0x155400,
        can_create=lambda options: options.hitsanity.value
    ),

    # Termina Field West Wall Hitspot
    "Termina Field West Wall Hitspot (0)": MMRLocationData(
        region="Termina Field",
        address=0x152D11,
        can_create=lambda options: options.hitsanity.value
    ),
    "Termina Field West Wall Hitspot (1)": MMRLocationData(
        region="Termina Field",
        address=0x152D10,
        can_create=lambda options: options.hitsanity.value
    ),
    "Termina Field West Wall Hitspot (2)": MMRLocationData(
        region="Termina Field",
        address=0x152D12,
        can_create=lambda options: options.hitsanity.value
    ),

    # Termina Field Above West Clock Town Entry Hitspot
    "Termina Field Above West Clock Town Entry Hitspot (0)": MMRLocationData(
        region="Termina Field",
        address=0x152D02,
        can_create=lambda options: options.hitsanity.value
    ),
    "Termina Field Above West Clock Town Entry Hitspot (1)": MMRLocationData(
        region="Termina Field",
        address=0x152D00,
        can_create=lambda options: options.hitsanity.value
    ),
    "Termina Field Above West Clock Town Entry Hitspot (2)": MMRLocationData(
        region="Termina Field",
        address=0x152D01,
        can_create=lambda options: options.hitsanity.value
    ),

    # Stock Pot Inn Mask Hitspot
    "Stock Pot Inn Mask Hitspot (0)": MMRLocationData(
        region="Clock Town",
        address=0x156102,
        can_create=lambda options: options.hitsanity.value
    ),
    "Stock Pot Inn Mask Hitspot (1)": MMRLocationData(
        region="Clock Town",
        address=0x156101,
        can_create=lambda options: options.hitsanity.value
    ),
    "Stock Pot Inn Mask Hitspot (2)": MMRLocationData(
        region="Clock Town",
        address=0x156100,
        can_create=lambda options: options.hitsanity.value
    ),

    # Skull Kid Drawing Hitspot
    "Skull Kid Drawing Hitspot (0)": MMRLocationData(
        region="Termina Field",
        address=0x152D22,
        can_create=lambda options: options.hitsanity.value
    ),
    "Skull Kid Drawing Hitspot (1)": MMRLocationData(
        region="Termina Field",
        address=0x152D21,
        can_create=lambda options: options.hitsanity.value
    ),
    "Skull Kid Drawing Hitspot (2)": MMRLocationData(
        region="Termina Field",
        address=0x152D20,
        can_create=lambda options: options.hitsanity.value
    ),

    # Romani Ranch Baby Cuccoos Hitspots
    "Romani Ranch Baby Cuccoos Hitspots (0)": MMRLocationData(
        region="Romani Ranch",
        address=0x154202,
        can_create=lambda options: options.hitsanity.value
    ),
    "Romani Ranch Baby Cuccoos Hitspots (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x154201,
        can_create=lambda options: options.hitsanity.value
    ),
    "Romani Ranch Baby Cuccoos Hitspots (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x154211,
        can_create=lambda options: options.hitsanity.value
    ),
    "Romani Ranch Baby Cuccoos Hitspots (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x154212,
        can_create=lambda options: options.hitsanity.value
    ),
    "Romani Ranch Baby Cuccoos Hitspots (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x154210,
        can_create=lambda options: options.hitsanity.value
    ),
    "Romani Ranch Baby Cuccoos Hitspots (5)": MMRLocationData(
        region="Romani Ranch",
        address=0x154200,
        can_create=lambda options: options.hitsanity.value
    ),

    # Swamp Spiderhouse Totem Eye Hitspots
    "Swamp Spiderhouse Totem Eye Hitspots (0)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152810,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152811,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152812,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (3)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152820,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (4)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152821,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (5)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152822,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (6)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152830,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (7)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152831,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (8)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152832,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (9)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152800,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (10)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152801,
        can_create=lambda options: options.hitsanity.value
    ),
    "Swamp Spiderhouse Totem Eye Hitspots (11)": MMRLocationData(
        region="Swamp Spider House",
        address=0x152802,
        can_create=lambda options: options.hitsanity.value
    ),

    # Ocean Spiderhouse Mask Hitspots
    "Ocean Spiderhouse Mask Hitspots (0)": MMRLocationData(
        region="Ocean Spider House",
        address=0x152900,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ocean Spiderhouse Mask Hitspots (1)": MMRLocationData(
        region="Ocean Spider House",
        address=0x152901,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ocean Spiderhouse Mask Hitspots (2)": MMRLocationData(
        region="Ocean Spider House",
        address=0x152902,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ocean Spiderhouse Mask Hitspots (3)": MMRLocationData(
        region="Ocean Spider House",
        address=0x152910,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ocean Spiderhouse Mask Hitspots (4)": MMRLocationData(
        region="Ocean Spider House",
        address=0x152911,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ocean Spiderhouse Mask Hitspots (5)": MMRLocationData(
        region="Ocean Spider House",
        address=0x152912,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ocean Spiderhouse Mask Hitspots (6)": MMRLocationData(
        region="Ocean Spider House",
        address=0x152920,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ocean Spiderhouse Mask Hitspots (7)": MMRLocationData(
        region="Ocean Spider House",
        address=0x152921,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ocean Spiderhouse Mask Hitspots (8)": MMRLocationData(
        region="Ocean Spider House",
        address=0x152922,
        can_create=lambda options: options.hitsanity.value
    ),

    # Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots
    "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (0)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x151410,
        can_create=lambda options: options.hitsanity.value
    ),
    "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (1)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x151402,
        can_create=lambda options: options.hitsanity.value
    ),
    "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (2)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x151412,
        can_create=lambda options: options.hitsanity.value
    ),
    "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (3)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x151411,
        can_create=lambda options: options.hitsanity.value
    ),
    "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (4)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x151400,
        can_create=lambda options: options.hitsanity.value
    ),
    "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (5)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x151401,
        can_create=lambda options: options.hitsanity.value
    ),

    # Pirates Fortress Interior Indoor Pirate Flag Eye Hitspot
    "Pirates Fortress Interior Indoor Pirate Flag Eye Hitspot (0)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x152600,
        can_create=lambda options: options.hitsanity.value
    ),
    "Pirates Fortress Interior Indoor Pirate Flag Eye Hitspot (1)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x152601,
        can_create=lambda options: options.hitsanity.value
    ),
    "Pirates Fortress Interior Indoor Pirate Flag Eye Hitspot (2)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x152602,
        can_create=lambda options: options.hitsanity.value
    ),

    # Ikana Graveyard Lantern Hitspots - 
    "Ikana Graveyard Lantern Hitspots (0)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154310,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154311,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154312,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (3)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154321,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (4)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154320,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (5)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154322,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (6)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154300,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (7)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154301,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (8)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154302,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (9)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154400,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (10)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154401,
        can_create=lambda options: options.hitsanity.value
    ),
    "Ikana Graveyard Lantern Hitspots (11)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x154402,
        can_create=lambda options: options.hitsanity.value
    ),            

    # Invisible Rupees

    # Termina Field Invisible Rupee Over Ramp Near Fountains
    "Termina Field Invisible Rupee Over Ramp Near Fountains": MMRLocationData(
        region="Termina Field",
        address=0x162D09,
        can_create=lambda options: options.invisisanity.value
    ),

    # Termina Field Fountain Rupees
    "Termina Field Fountain Rupees (0)": MMRLocationData(
        region="Termina Field",
        address=0x162D06,
        can_create=lambda options: options.invisisanity.value
    ),
    "Termina Field Fountain Rupees (1)": MMRLocationData(
        region="Termina Field",
        address=0x162D07,
        can_create=lambda options: options.invisisanity.value
    ),

    # Termina Field Invisible Rupee Over Water
    "Termina Field Invisible Rupee Over Water": MMRLocationData(
        region="Termina Field",
        address=0x162D0A,
        can_create=lambda options: options.invisisanity.value
    ),

    # Termina Field Invisible Rupee Over Stump
    "Termina Field Invisible Rupee Over Stump": MMRLocationData(
        region="Termina Field",
        address=0x162D05,
        can_create=lambda options: options.invisisanity.value
    ),

    # Termina Field Invisible Rupees In Long Grass
    # Duplicate removed
    "Termina Field Invisible Rupees In Long Grass (0)": MMRLocationData(
        region="Termina Field",
        address=0x162D00,
        can_create=lambda options: options.invisisanity.value
    ),
    "Termina Field Invisible Rupees In Long Grass (1)": MMRLocationData(
        region="Termina Field",
        address=0x162D01,
        can_create=lambda options: options.invisisanity.value
    ),
    "Termina Field Invisible Rupees In Long Grass (2)": MMRLocationData(
        region="Termina Field",
        address=0x162D02,
        can_create=lambda options: options.invisisanity.value
    ),
    "Termina Field Invisible Rupees In Long Grass (3)": MMRLocationData(
        region="Termina Field",
        address=0x162D03,
        can_create=lambda options: options.invisisanity.value
    ),
    "Termina Field Invisible Rupees In Long Grass (4)": MMRLocationData(
        region="Termina Field",
        address=0x162D04,
        can_create=lambda options: options.invisisanity.value
    ),

    # Termina Field Invisible Rupee Over Northern Ramp
    "Termina Field Invisible Rupee Over Northern Ramp": MMRLocationData(
        region="Termina Field",
        address=0x162D08,
        can_create=lambda options: options.invisisanity.value
    ),

    # Romani Ranch Invisible Fence Rupees
    "Romani Ranch Invisible Fence Rupees (0)": MMRLocationData(
        region="Romani Ranch",
        address=0x163505,
        can_create=lambda options: options.invisisanity.value
    ),
    "Romani Ranch Invisible Fence Rupees (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x163504,
        can_create=lambda options: options.invisisanity.value
    ),
    "Romani Ranch Invisible Fence Rupees (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x163501,
        can_create=lambda options: options.invisisanity.value
    ),
    "Romani Ranch Invisible Fence Rupees (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x163500,
        can_create=lambda options: options.invisisanity.value
    ),
    "Romani Ranch Invisible Fence Rupees (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x163503,
        can_create=lambda options: options.invisisanity.value
    ),
    "Romani Ranch Invisible Fence Rupees (5)": MMRLocationData(
        region="Romani Ranch",
        address=0x163502,
        can_create=lambda options: options.invisisanity.value
    ),

    # Swamp Spiderhouse Invisible Rupees Above Giant Jars
    "Swamp Spiderhouse Invisible Rupees Above Giant Jars (0)": MMRLocationData(
        region="Swamp Spider House",
        address=0x162744,
        can_create=lambda options: options.invisisanity.value
    ),
    "Swamp Spiderhouse Invisible Rupees Above Giant Jars (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x162743,
        can_create=lambda options: options.invisisanity.value
    ),
    "Swamp Spiderhouse Invisible Rupees Above Giant Jars (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x162742,
        can_create=lambda options: options.invisisanity.value
    ),
    "Swamp Spiderhouse Invisible Rupees Above Giant Jars (3)": MMRLocationData(
        region="Swamp Spider House",
        address=0x162741,
        can_create=lambda options: options.invisisanity.value
    ),
    "Swamp Spiderhouse Invisible Rupees Above Giant Jars (4)": MMRLocationData(
        region="Swamp Spider House",
        address=0x162740,
        can_create=lambda options: options.invisisanity.value
    ),

    # Freestanding Rupees

    # Songwall Rupees
    "Termina Field 6am Songwall (0)": MMRLocationData(
        region="Clock Town",
        address=0x1A0011,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 6am Songwall (1)": MMRLocationData(
        region="Clock Town",
        address=0x1A0022,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 6am Songwall (2)": MMRLocationData(
        region="Clock Town",
        address=0x1A0000,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 7am Songwall (0)": MMRLocationData(
        region="Clock Town",
        address=0x1A00C0,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 7am Songwall (1)": MMRLocationData(
        region="Clock Town",
        address=0x1A00D1,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 7am Songwall (2)": MMRLocationData(
        region="Clock Town",
        address=0x1A00E2,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 8am Songwall (0)": MMRLocationData(
        region="Clock Town",
        address=0x1A0030,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 8am Songwall (1)": MMRLocationData(
        region="Clock Town",
        address=0x1A0041,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 8am Songwall (2)": MMRLocationData(
        region="Clock Town",
        address=0x1A0052,
        can_create=lambda options: options.rupeesanity.value
    ),
    # "Termina Field 9am Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A0052,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 9am Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A0052,
    #     can_create=lambda options: options.rupeesanity.value
    # ), 
    # "Termina Field 9am Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A0052,
    #     can_create=lambda options: options.rupeesanity.value
    # ),          
    "Termina Field 10am Songwall (0)": MMRLocationData(
        region="Clock Town",
        address=0x1A0060,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 10am Songwall (1)": MMRLocationData(
        region="Clock Town",
        address=0x1A0071,
        can_create=lambda options: options.rupeesanity.value
    ),  
    "Termina Field 10am Songwall (2)": MMRLocationData(
        region="Clock Town",
        address=0x1A0082,
        can_create=lambda options: options.rupeesanity.value
    ),
    # "Termina Field 11am Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A0052,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 11am Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A0052,
    #     can_create=lambda options: options.rupeesanity.value
    # ), 
    # "Termina Field 11am Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A0052,
    #     can_create=lambda options: options.rupeesanity.value
    # ),          
    # "Termina Field 12pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 12pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 12pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 12pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 12pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 12pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 1pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 1pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 1pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),        
    # "Termina Field 2pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 2pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 2pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 3pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 3pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 3pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 4pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 4pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 4pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 5pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 5pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 5pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ), 
    # "Termina Field 6pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 6pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 6pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 7pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 7pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 7pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A82,
    #     can_create=lambda options: options.rupeesanity.value
    # ), 
    "Termina Field 8pm Songwall (0)": MMRLocationData(
        region="Clock Town",
        address=0x1A0090,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 8pm Songwall (1)": MMRLocationData(
        region="Clock Town",
        address=0x1A00A1,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field 8pm Songwall (2)": MMRLocationData(
        region="Clock Town",
        address=0x1A00B2,
        can_create=lambda options: options.rupeesanity.value
    ),
    # "Termina Field 9pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A90,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 9pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AA1,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 9pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AB2,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 10pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A90,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 10pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AA1,
    #     can_create=lambda options: options.rupeesanity.value
    # ), 
    # "Termina Field 10pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AB2,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 11pm Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A90,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 11pm Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AA1,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 11pm Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AB2,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 12am Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A90,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 12am Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AA1,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 12am Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AB2,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 1am Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A90,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 1am Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AA1,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 1am Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AB2,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 2am Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A90,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 3am Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AA1,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 3am Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AB2,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 4am Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A90,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 4am Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AA1,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 4am Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AB2,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 5am Songwall (0)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1A90,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 5am Songwall (1)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AA1,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # "Termina Field 5am Songwall (2)": MMRLocationData(
    #     region="Clock Town",
    #     address=0x1AB2,
    #     can_create=lambda options: options.rupeesanity.value
    # ),
    # Laundry Pool Night 2 Rupees
    "Laundry Pool Night 2 Rupees (0)": MMRLocationData(
        region="Clock Town",
        address=0x177000,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Laundry Pool Night 2 Rupees (1)": MMRLocationData(
        region="Clock Town",
        address=0x177001,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Laundry Pool Night 2 Rupees (2)": MMRLocationData(
        region="Clock Town",
        address=0x177002,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Termina Field Eastern Pillar Rupees
    "Termina Field Eastern Pillar Rupees": MMRLocationData(
        region="Termina Field",
        address=0x172D00,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Termina Field Tree Rupees
    "Termina Field Tree Rupees (1)": MMRLocationData(
        region="Termina Field",
        address=0x172DE1,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Tree Rupees (2)": MMRLocationData(
        region="Termina Field",
        address=0x172DE2,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Observatory Secret Guay Rupee (1)": MMRLocationData(
        region="Termina Field",
        address=0x172DC2,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Observatory Secret Guay Rupee (2)": MMRLocationData(
        region="Termina Field",
        address=0x172DC3,
        can_create=lambda options: options.rupeesanity.value
    ),    
    # Termina Field Song Guay Rupees
    "Termina Field Song Guay Rupees (0)": MMRLocationData(
        region="Termina Field",
        address=0x17FF00,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (1)": MMRLocationData(
        region="Termina Field",
        address=0x17FF01,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (2)": MMRLocationData(
        region="Termina Field",
        address=0x17FF02,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (3)": MMRLocationData(
        region="Termina Field",
        address=0x17FF03,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (4)": MMRLocationData(
        region="Termina Field",
        address=0x17FF04,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (5)": MMRLocationData(
        region="Termina Field",
        address=0x17FF05,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (6)": MMRLocationData(
        region="Termina Field",
        address=0x17FF06,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (7)": MMRLocationData(
        region="Termina Field",
        address=0x17FF07,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (8)": MMRLocationData(
        region="Termina Field",
        address=0x17FF08,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (9)": MMRLocationData(
        region="Termina Field",
        address=0x17FF09,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (10)": MMRLocationData(
        region="Termina Field",
        address=0x17FF0A,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (11)": MMRLocationData(
        region="Termina Field",
        address=0x17FF0B,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (12)": MMRLocationData(
        region="Termina Field",
        address=0x17FF0C,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (13)": MMRLocationData(
        region="Termina Field",
        address=0x17FF0D,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (14)": MMRLocationData(
        region="Termina Field",
        address=0x17FF0E,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (15)": MMRLocationData(
        region="Termina Field",
        address=0x17FF0F,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (16)": MMRLocationData(
        region="Termina Field",
        address=0x17FF10,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (17)": MMRLocationData(
        region="Termina Field",
        address=0x17FF11,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (18)": MMRLocationData(
        region="Termina Field",
        address=0x17FF12,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Rupees (19)": MMRLocationData(
        region="Termina Field",
        address=0x17FF13,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Thieving Bird Rupee": MMRLocationData(
        region="Termina Field",
        address=0x172DD3,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Termina Field Song Guay Goron/Epona Rupees
    "Termina Field Song Guay Goron/Epona Rupees (0)": MMRLocationData(
        region="Termina Field",
        address=0x17FF18,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Goron/Epona Rupees (1)": MMRLocationData(
        region="Termina Field",
        address=0x17FF1D,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Termina Field Song Guay Goron/Epona Rupees (2)": MMRLocationData(
        region="Termina Field",
        address=0x17FF22,
        can_create=lambda options: options.rupeesanity.value
    ),


    # Deku PlayGround Day 1 Rupees
    "Deku PlayGround Day 1 Rupees (0)": MMRLocationData(
        region="Clock Town",
        address=0x171E13,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 1 Rupees (1)": MMRLocationData(
        region="Clock Town",
        address=0x171E10,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 1 Rupees (2)": MMRLocationData(
        region="Clock Town",
        address=0x171E14,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 1 Rupees (3)": MMRLocationData(
        region="Clock Town",
        address=0x171E12,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 1 Rupees (4)": MMRLocationData(
        region="Clock Town",
        address=0x171E11,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 1 Rupees (5)": MMRLocationData(
        region="Clock Town",
        address=0x171E15,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Deku PlayGround Day 2 Rupees
    "Deku PlayGround Day 2 Rupees (0)": MMRLocationData(
        region="Clock Town",
        address=0x171E25,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 2 Rupees (1)": MMRLocationData(
        region="Clock Town",
        address=0x171E21,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 2 Rupees (2)": MMRLocationData(
        region="Clock Town",
        address=0x171E20,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 2 Rupees (3)": MMRLocationData(
        region="Clock Town",
        address=0x171E23,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 2 Rupees (4)": MMRLocationData(
        region="Clock Town",
        address=0x171E24,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 2 Rupees (5)": MMRLocationData(
        region="Clock Town",
        address=0x171E22,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Deku PlayGround Day 3 Rupees
    "Deku PlayGround Day 3 Rupees (0)": MMRLocationData(
        region="Clock Town",
        address=0x171E32,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 3 Rupees (1)": MMRLocationData(
        region="Clock Town",
        address=0x171E31,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 3 Rupees (2)": MMRLocationData(
        region="Clock Town",
        address=0x171E30,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 3 Rupees (3)": MMRLocationData(
        region="Clock Town",
        address=0x171E33,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 3 Rupees (4)": MMRLocationData(
        region="Clock Town",
        address=0x171E35,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku PlayGround Day 3 Rupees (5)": MMRLocationData(
        region="Clock Town",
        address=0x171E34,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Southern Swamp Flower Rupees
    "Southern Swamp Flower Rupees (0)": MMRLocationData(
        region="Southern Swamp",
        address=0x174500,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Southern Swamp Flower Rupees (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x174501,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Southern Swamp Witch Shop Rupee (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x170A00,
        can_create=lambda options: options.rupeesanity.value    
    ),

    # Deku Palace Right Side Rupees
    "Deku Palace Right Side Rupees (0)": MMRLocationData(
        region="Deku Palace",
        address=0x172BF2,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (1)": MMRLocationData(
        region="Deku Palace",
        address=0x172BF1,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (2)": MMRLocationData(
        region="Deku Palace",
        address=0x172BF6,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (3)": MMRLocationData(
        region="Deku Palace",
        address=0x172BF0,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (4)": MMRLocationData(
        region="Deku Palace",
        address=0x172BF5,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (5)": MMRLocationData(
        region="Deku Palace",
        address=0x172BF4,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (6)": MMRLocationData(
        region="Deku Palace",
        address=0x172BF3,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (7)": MMRLocationData(
        region="Deku Palace",
        address=0x172B10,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (8)": MMRLocationData(
        region="Deku Palace",
        address=0x172B11,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (9)": MMRLocationData(
        region="Deku Palace",
        address=0x172B15,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (10)": MMRLocationData(
        region="Deku Palace",
        address=0x172B14,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (11)": MMRLocationData(
        region="Deku Palace",
        address=0x172B13,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Right Side Rupees (12)": MMRLocationData(
        region="Deku Palace",
        address=0x172B12,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Deku Palace Left Side Rupees
    "Deku Palace Left Side Rupees (0)": MMRLocationData(
        region="Deku Palace",
        address=0x172B24,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Left Side Rupees (1)": MMRLocationData(
        region="Deku Palace",
        address=0x172B25,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Left Side Rupees (2)": MMRLocationData(
        region="Deku Palace",
        address=0x172B26,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Left Side Rupees (3)": MMRLocationData(
        region="Deku Palace",
        address=0x172B21,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Left Side Rupees (4)": MMRLocationData(
        region="Deku Palace",
        address=0x172B20,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Left Side Rupees (5)": MMRLocationData(
        region="Deku Palace",
        address=0x172B22,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Left Side Rupees (6)": MMRLocationData(
        region="Deku Palace",
        address=0x172B2A,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Left Side Rupees (7)": MMRLocationData(
        region="Deku Palace",
        address=0x172B2B,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Left Side Rupees (8)": MMRLocationData(
        region="Deku Palace",
        address=0x172B27,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Left Side Rupees (9)": MMRLocationData(
        region="Deku Palace",
        address=0x172B28,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Palace Left Side Rupees (10)": MMRLocationData(
        region="Deku Palace",
        address=0x172B29,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Deku Butler Rupees
    "Deku Butler Rupees (0)": MMRLocationData(
        region="Deku Palace",
        address=0x175225,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (1)": MMRLocationData(
        region="Deku Palace",
        address=0x175224,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (2)": MMRLocationData(
        region="Deku Palace",
        address=0x175221,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (3)": MMRLocationData(
        region="Deku Palace",
        address=0x175220,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (4)": MMRLocationData(
        region="Deku Palace",
        address=0x175222,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (5)": MMRLocationData(
        region="Deku Palace",
        address=0x175223,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (6)": MMRLocationData(
        region="Deku Palace",
        address=0x175227,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (7)": MMRLocationData(
        region="Deku Palace",
        address=0x175226,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (8)": MMRLocationData(
        region="Deku Palace",
        address=0x175230,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (9)": MMRLocationData(
        region="Deku Palace",
        address=0x175231,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (10)": MMRLocationData(
        region="Deku Palace",
        address=0x175232,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (11)": MMRLocationData(
        region="Deku Palace",
        address=0x175233,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (12)": MMRLocationData(
        region="Deku Palace",
        address=0x175234,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (13)": MMRLocationData(
        region="Deku Palace",
        address=0x175235,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (14)": MMRLocationData(
        region="Deku Palace",
        address=0x175240,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (15)": MMRLocationData(
        region="Deku Palace",
        address=0x175241,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (16)": MMRLocationData(
        region="Deku Palace",
        address=0x175242,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (17)": MMRLocationData(
        region="Deku Palace",
        address=0x175244,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (18)": MMRLocationData(
        region="Deku Palace",
        address=0x175245,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (19)": MMRLocationData(
        region="Deku Palace",
        address=0x175243,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (20)": MMRLocationData(
        region="Deku Palace",
        address=0x175289,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (21)": MMRLocationData(
        region="Deku Palace",
        address=0x175288,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (22)": MMRLocationData(
        region="Deku Palace",
        address=0x175287,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (23)": MMRLocationData(
        region="Deku Palace",
        address=0x175286,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (24)": MMRLocationData(
        region="Deku Palace",
        address=0x175285,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (25)": MMRLocationData(
        region="Deku Palace",
        address=0x175282,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (26)": MMRLocationData(
        region="Deku Palace",
        address=0x175281,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (27)": MMRLocationData(
        region="Deku Palace",
        address=0x175280,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (28)": MMRLocationData(
        region="Deku Palace",
        address=0x175283,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Deku Butler Rupees (29)": MMRLocationData(
        region="Deku Palace",
        address=0x175284,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Woodfall Stump Rupee
    "Woodfall Stump Rupee": MMRLocationData(
        region="Woodfall",
        address=0x174600,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Romani Ranch Haystack Rupees
    "Romani Ranch Haystack Rupees (0)": MMRLocationData(
        region="Romani Ranch",
        address=0x171001,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Romani Ranch Haystack Rupees (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x171000,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Woodfall Temple Pre Boss Rupees
    "Woodfall Temple Pre Boss Rupees (0)": MMRLocationData(
        region="Woodfall Temple",
        address=0x171B03,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Woodfall Temple Pre Boss Rupees (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x171B05,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Woodfall Temple Pre Boss Rupees (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x171B04,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Woodfall Temple Pre Boss Rupees (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x171B02,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Woodfall Temple Pre Boss Rupees (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x171B00,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Woodfall Temple Pre Boss Rupees (5)": MMRLocationData(
        region="Woodfall Temple",
        address=0x171B01,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Snowhead Temple Icicle Rupees
    "Snowhead Temple Icicle Rupees (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x172170,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Snowhead Temple Icicle Rupees (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x172172,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Snowhead Temple Icicle Rupees (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x172171,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Mountain Village Spring Boulder Under Smithy Rupee
    "Mountain Village Spring Boulder Under Smithy Rupee": MMRLocationData(
        region="Mountain Village",
        address=0x175A00,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Twin Islands Spring Underwater Rupees
    "Twin Islands Spring Underwater Rupees (0)": MMRLocationData(
        region="Twin Islands",
        address=0x175E02,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Twin Islands Spring Underwater Rupees (1)": MMRLocationData(
        region="Twin Islands",
        address=0x175E03,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Twin Islands Spring Underwater Rupees (2)": MMRLocationData(
        region="Twin Islands",
        address=0x175E00,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Twin Islands Spring Underwater Rupees (3)": MMRLocationData(
        region="Twin Islands",
        address=0x175E01,
        can_create=lambda options: options.rupeesanity.value
    ),
    # Pirates' Fortress Sewers Rupees (Under Barrels)
    "Pirates' Fortress Sewers Rupees Under Barrel (0)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x1723B0,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Pirates' Fortress Sewers Rupees Under Barrel (1)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x1723B1,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Pirates' Fortress Sewers Rupees Under Barrel (2)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x1723B2,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Pirates' Fortress Sewers Rupees Under Barrel (3)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x1723B3,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Pirates' Fortress Sewers Rupees Under Barrel (4)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x1723B4,
        can_create=lambda options: options.rupeesanity.value
    ),    
    "Pirates Fortress' Sewers Exit Barrel Rupee (0)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x172390,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Pirates Fortress' Sewers Exit Barrel Rupee (1)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x172391,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Pirates Fortress' Sewers Exit Barrel Rupee (2)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x172392,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Pirates Fortress' Interior Ledge Recovery Hearts (0)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x171400,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Pirates Fortress' Interior Ledge Recovery Hearts (1)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x171401,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Pirates Fortress' Interior Ledge Recovery Hearts (2)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x171402,
        can_create=lambda options: options.rupeesanity.value
    ),           
    # Great Bay Temple Waterwheel Rupees
    "Great Bay Temple Waterwheel Rupees (0)": MMRLocationData(
        region="Great Bay Temple",
        address=0x174982,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Great Bay Temple Waterwheel Rupees (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x174984,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Great Bay Temple Waterwheel Rupees (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x174983,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Great Bay Temple Waterwheel Rupees (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x174980,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Great Bay Temple Waterwheel Rupees (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x174981,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Great Bay Temple Room Behind Waterfall Rupees (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x1749C1,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Great Bay Temple Room Behind Waterfall Rupees (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x1749C0,
        can_create=lambda options: options.rupeesanity.value
    ),
    # Great Bay Temple Before Gekko Room Underwater Rupees
    "Great Bay Temple Before Gekko Room Underwater Rupees (0)": MMRLocationData(
        region="Great Bay Temple",
        address=0x174941,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Great Bay Temple Before Gekko Room Underwater Rupees (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x174940,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Ikana Graveyard Day 2 Rupees
    "Ikana Graveyard Day 2 Rupees (0)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x170CF0,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Ikana Graveyard Day 2 Rupees (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x170CF5,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Ikana Graveyard Day 2 Rupees (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x170CF6,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Ikana Graveyard Day 2 Rupees (3)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x170CF1,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Ikana Graveyard Day 2 Rupees (4)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x170CF2,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Ikana Graveyard Day 2 Rupees (5)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x170CF3,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Ikana Graveyard Day 2 Rupees (6)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x170CF4,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Secret Shrine Rupees
    "Secret Shrine Rupees (0)": MMRLocationData(
        region="Secret Shrine",
        address=0x17600A,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (1)": MMRLocationData(
        region="Secret Shrine",
        address=0x17600B,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (2)": MMRLocationData(
        region="Secret Shrine",
        address=0x176004,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (3)": MMRLocationData(
        region="Secret Shrine",
        address=0x176002,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (4)": MMRLocationData(
        region="Secret Shrine",
        address=0x176009,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (5)": MMRLocationData(
        region="Secret Shrine",
        address=0x176000,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (6)": MMRLocationData(
        region="Secret Shrine",
        address=0x176001,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (7)": MMRLocationData(
        region="Secret Shrine",
        address=0x176010,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (8)": MMRLocationData(
        region="Secret Shrine",
        address=0x17600F,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (9)": MMRLocationData(
        region="Secret Shrine",
        address=0x17600E,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (10)": MMRLocationData(
        region="Secret Shrine",
        address=0x17600D,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (11)": MMRLocationData(
        region="Secret Shrine",
        address=0x17600C,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (12)": MMRLocationData(
        region="Secret Shrine",
        address=0x176008,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (13)": MMRLocationData(
        region="Secret Shrine",
        address=0x176007,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (14)": MMRLocationData(
        region="Secret Shrine",
        address=0x176006,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (15)": MMRLocationData(
        region="Secret Shrine",
        address=0x176005,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Secret Shrine Rupees (16)": MMRLocationData(
        region="Secret Shrine",
        address=0x176003,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Stone Tower Bridge Room Rupees
    "Stone Tower Bridge Room Rupees (0)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171687,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Bridge Room Rupees (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171685,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Bridge Room Rupees (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171683,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Bridge Room Rupees (3)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171681,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Bridge Room Rupees (4)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171682,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Bridge Room Rupees (5)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171680,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Bridge Room Rupees (6)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171684,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Bridge Room Rupees (7)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171686,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Deku Updraft Rupees (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171690,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Deku Updraft Rupees (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171691,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Deku Updraft Rupees (3)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171692,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Deku Updraft Rupees (4)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171693,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Deku Updraft Rupees (5)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171694,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Deku Updraft Rupees (6)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171695,
        can_create=lambda options: options.rupeesanity.value
    ),                           
    # Stone Tower Eyegore Room Light Block Rupees
    "Stone Tower Eyegore Room Light Block Rupees (0)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171611,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Stone Tower Eyegore Room Light Block Rupees (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x171610,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Inverted Stone Tower Right Side Light Block Rupees
    "Inverted Stone Tower Right Side Light Block Rupees (0)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171832,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Right Side Light Block Rupees (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171831,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Inverted Stone Tower Dexihand Rupees
    "Inverted Stone Tower Dexihand Rupees (0)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171811,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Dexihand Rupees (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171810,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Dexihand Rupees (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171812,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Inverted Stone Tower Pre Boss Rupees
    "Inverted Stone Tower Pre Boss Rupees (0)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171881,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Pre Boss Rupees (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171880,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Pre Boss Rupees (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171882,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Pre Boss Rupees (3)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171886,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Pre Boss Rupees (4)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171883,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Pre Boss Rupees (5)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171889,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Pre Boss Rupees (6)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x17188A,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Pre Boss Rupees (7)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171888,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Pre Boss Rupees (8)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171887,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Pre Boss Rupees (9)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171885,
        can_create=lambda options: options.rupeesanity.value
    ),
    "Inverted Stone Tower Pre Boss Rupees (10)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x171884,
        can_create=lambda options: options.rupeesanity.value
    ),

    # Rocksanity

    # Termina Field 
    "Termina Field Boulder Over Gossip Grotto (0)": MMRLocationData(
        region="Termina Field",
        address=0x1B2D00,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field Boulder Over Bio Baba Grotto (0)": MMRLocationData(
        region="Termina Field",
        address=0x1B2D01,
        can_create=lambda options: options.rocksanity.value
    ),        
    "Termina Field Kamaro Rock Circle (0)": MMRLocationData(
        region="Termina Field",
        address=0x262D00,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field Kamaro Rock Circle (1)": MMRLocationData(
        region="Termina Field",
        address=0x262D01,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field Kamaro Rock Circle (2)": MMRLocationData(
        region="Termina Field",
        address=0x262D02,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field Kamaro Rock Circle (3)": MMRLocationData(
        region="Termina Field",
        address=0x262D03,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field Kamaro Rock Circle (4)": MMRLocationData(
        region="Termina Field",
        address=0x262D04,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field Kamaro Rock Circle (5)": MMRLocationData(
        region="Termina Field",
        address=0x262D05,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field Kamaro Rock Circle (6)": MMRLocationData(
        region="Termina Field",
        address=0x262D06,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field Kamaro Rock Circle (7)": MMRLocationData(
        region="Termina Field",
        address=0x262D07,
        can_create=lambda options: options.rocksanity.value
    ),

    # Termina Field North West Rock Wall
    "Termina Field North West Rock Wall (0)": MMRLocationData(
        region="Termina Field",
        address=0x182D07,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field North West Rock Wall (1)": MMRLocationData(
        region="Termina Field",
        address=0x182D03,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field North West Rock Wall (2)": MMRLocationData(
        region="Termina Field",
        address=0x182D01,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field North West Rock Wall (3)": MMRLocationData(
        region="Termina Field",
        address=0x182D04,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field North West Rock Wall (4)": MMRLocationData(
        region="Termina Field",
        address=0x182D0C,
        can_create=lambda options: options.rocksanity.value
    ),

    # Termina Field Rock Behind Coast Wall
    "Termina Field Rock Behind Coast Wall": MMRLocationData(
        region="Termina Field",
        address=0x182D00,
        can_create=lambda options: options.rocksanity.value
    ),
    # Termina Field Bio Baba Rock
    "Termina Field Bio Baba Rock": MMRLocationData(
        region="Termina Field",
        address=0x1807B0,
        can_create=lambda options: options.rocksanity.value
    ),    
    # South West Rock Wall
    "Termina Field South West Rock Wall (1)": MMRLocationData(
        region="Termina Field",
        address=0x182D0B,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field South West Rock Wall (2)": MMRLocationData(
        region="Termina Field",
        address=0x182D05,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field South West Rock Wall (3)": MMRLocationData(
        region="Termina Field",
        address=0x182D06,
        can_create=lambda options: options.rocksanity.value
    ),
    "Termina Field South West Rock Wall (4)": MMRLocationData(
        region="Termina Field",
        address=0x182D02,
        can_create=lambda options: options.rocksanity.value
    ),
    "Deku Palace Guarded Boulder (1)": MMRLocationData(
        region="Deku Palace",
        address=0x1B2B10,
        can_create=lambda options: options.rocksanity.value
    ),
    "Deku Palace Guarded Boulder (2)": MMRLocationData(
        region="Deku Palace",
        address=0x1B2B11,
        can_create=lambda options: options.rocksanity.value
    ),
    "Deku Palace Guarded Boulder (3)": MMRLocationData(
        region="Deku Palace",
        address=0x1B2B12,
        can_create=lambda options: options.rocksanity.value
    ),
    # Swamp Spider Entry Rocks
    "Swamp Spider Entry Rocks (0)": MMRLocationData(
        region="Swamp Spider House",
        address=0x182701,
        can_create=lambda options: options.rocksanity.value
    ),
    "Swamp Spider Entry Rocks (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x182700,
        can_create=lambda options: options.rocksanity.value
    ),
    "Swamp Spider Monument Room Boulder": MMRLocationData(
        region="Swamp Spider House",
        address=0x1B2730,
        can_create=lambda options: options.rocksanity.value
    ),    

    # Swamp Spider Large Pots Rock
    "Swamp Spider Large Pots Rock": MMRLocationData(
        region="Swamp Spider House",
        address=0x182740,
        can_create=lambda options: options.rocksanity.value
    ),
    # Twin islands 
    "Twin Isles Hot Spring Water Grotto Bomb Boulders (0)": MMRLocationData(
        region="Twin Islands",
        address=0x1B07E0,
        can_create=lambda options: options.rocksanity.value
    ),
    "Twin Isles Hot Spring Water Grotto Bomb Boulders (1)": MMRLocationData(
        region="Twin Islands",
        address=0x1B07E1,
        can_create=lambda options: options.rocksanity.value
    ),
    "Twin Isles Hot Spring Water Grotto Bomb Boulders (2)": MMRLocationData(
        region="Twin Islands",
        address=0x1B07E2,
        can_create=lambda options: options.rocksanity.value
    ),
    "Twin Isles Hot Spring Water Grotto Bomb Boulders (3)": MMRLocationData(
        region="Twin Islands",
        address=0x1B07E3,
        can_create=lambda options: options.rocksanity.value
    ),
    "Twin Isles Hot Spring Water Grotto Bomb Boulders (4)": MMRLocationData(
        region="Twin Islands",
        address=0x1B07E4,
        can_create=lambda options: options.rocksanity.value
    ),                
    # Goron Shrine Rocks
    "Goron Shrine Rocks (0)": MMRLocationData(
        region="Goron Village",
        address=0x183202,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (1)": MMRLocationData(
        region="Goron Village",
        address=0x183201,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (2)": MMRLocationData(
        region="Goron Village",
        address=0x183203,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (3)": MMRLocationData(
        region="Goron Village",
        address=0x183200,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (4)": MMRLocationData(
        region="Goron Village",
        address=0x183206,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (5)": MMRLocationData(
        region="Goron Village",
        address=0x183207,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (6)": MMRLocationData(
        region="Goron Village",
        address=0x183205,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (7)": MMRLocationData(
        region="Goron Village",
        address=0x183204,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (8)": MMRLocationData(
        region="Goron Village",
        address=0x18320D,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (9)": MMRLocationData(
        region="Goron Village",
        address=0x18320C,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (10)": MMRLocationData(
        region="Goron Village",
        address=0x18320E,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (11)": MMRLocationData(
        region="Goron Village",
        address=0x18320F,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (12)": MMRLocationData(
        region="Goron Village",
        address=0x18320A,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (13)": MMRLocationData(
        region="Goron Village",
        address=0x18320B,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (14)": MMRLocationData(
        region="Goron Village",
        address=0x183208,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Shrine Rocks (15)": MMRLocationData(
        region="Goron Village",
        address=0x183209,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Village Lens Cave Bomb Boulder (1)": MMRLocationData(
        region="Goron Village",
        address=0x1B0750,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Village Lens Cave Bomb Boulder (2)": MMRLocationData(
        region="Goron Village",
        address=0x1B0751,
        can_create=lambda options: options.rocksanity.value
    ),
    "Goron Village Lens Cave Bomb Boulder (3)": MMRLocationData(
        region="Goron Village",
        address=0x1B0752,
        can_create=lambda options: options.rocksanity.value
    ),              

    # Mountain Village Spring Rocks
    "Mountain Village Boulders Under Stairs (0)": MMRLocationData(
        region="Mountain Village",
        address=0x1B5A00,
        can_create=lambda options: options.rocksanity.value
    ),
    "Mountain Village Boulders Under Stairs (1)": MMRLocationData(
        region="Mountain Village",
        address=0x1B5A01,
        can_create=lambda options: options.rocksanity.value
    ),  
    "Mountain Village Boulders Under Stairs (2)": MMRLocationData(
        region="Mountain Village",
        address=0x1B5A02,
        can_create=lambda options: options.rocksanity.value
    ),              
    "Mountain Village Spring Rock Triangle (0)": MMRLocationData(
        region="Mountain Village",
        address=0x185A02,
        can_create=lambda options: options.rocksanity.value
    ),
    "Mountain Village Spring Rock Triangle (1)": MMRLocationData(
        region="Mountain Village",
        address=0x185A03,
        can_create=lambda options: options.rocksanity.value
    ),
    "Mountain Village Spring Rock Triangle (2)": MMRLocationData(
        region="Mountain Village",
        address=0x185A04,
        can_create=lambda options: options.rocksanity.value
    ),
    "Mountain Village Spring Rock Triangle (3)": MMRLocationData(
        region="Mountain Village",
        address=0x185A05,
        can_create=lambda options: options.rocksanity.value
    ),
    "Mountain Village Spring Rock Triangle (4)": MMRLocationData(
        region="Mountain Village",
        address=0x185A06,
        can_create=lambda options: options.rocksanity.value
    ),

     # Mountain Village Spring Outside Goron Graveyard
    "Mountain Village Spring Outside Goron Graveyard Rocks (0)": MMRLocationData(
        region="Mountain Village",
        address=0x185A00,
        can_create=lambda options: options.rocksanity.value
    ),
    "Mountain Village Spring Outside Goron Graveyard Rocks (1)": MMRLocationData(
        region="Mountain Village",
        address=0x185A01,
        can_create=lambda options: options.rocksanity.value
    ),

    # Twin Isles Spring Above Grotto Rocks
    "Twin Isles Spring Above Grotto Rocks (0)": MMRLocationData(
        region="Twin Islands",
        address=0x185E00,
        can_create=lambda options: options.rocksanity.value
    ),
    "Twin Isles Spring Above Grotto Rocks (1)": MMRLocationData(
        region="Twin Islands",
        address=0x185E01,
        can_create=lambda options: options.rocksanity.value
    ),
    "Twin Isles Spring Above Grotto Rocks (2)": MMRLocationData(
        region="Twin Islands",
        address=0x185E02,
        can_create=lambda options: options.rocksanity.value
    ),


    # Great Bay Coast Scattered Beach Rocks
    "Great Bay Coast Scattered Beach Rocks (0)": MMRLocationData(
        region="Great Bay",
        address=0x183727,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Scattered Beach Rocks (1)": MMRLocationData(
        region="Great Bay",
        address=0x183728,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Scattered Beach Rocks (2)": MMRLocationData(
        region="Great Bay",
        address=0x183729,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Scattered Beach Rocks (3)": MMRLocationData(
        region="Great Bay",
        address=0x183726,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Scattered Beach Rocks (4)": MMRLocationData(
        region="Great Bay",
        address=0x18372A,
        can_create=lambda options: options.rocksanity.value
    ),

    # Great Bay Coast Rock Wall Rocks
    "Great Bay Coast Rock Wall Rocks (1)": MMRLocationData(
        region="Great Bay",
        address=0x18372D,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Rock Wall Rocks (2)": MMRLocationData(
        region="Great Bay",
        address=0x18372B,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Rock Wall Rocks (3)": MMRLocationData(
        region="Great Bay",
        address=0x18372C,
        can_create=lambda options: options.rocksanity.value
    ),
    # Great Bay Coast Underwater Rocks (Bombchus only)
    "Great Bay Coast Underwater Rocks (Bombchus only) Below Rock Pools (1)": MMRLocationData(
        region="Great Bay",
        address=0x18371C,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Underwater Rocks (Bombchus only) Next to Like-Like": MMRLocationData(
        region="Great Bay",
        address=0x18372E,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Underwater Rocks (Bombchus only) Below Rock Pools (2)": MMRLocationData(
        region="Great Bay",
        address=0x18371B,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Underwater Rocks (Bombchus only) Near Rock Wall": MMRLocationData(
        region="Great Bay",
        address=0x18371A,
        can_create=lambda options: options.rocksanity.value
    ),    
    # Rocks Underwater Easy to get
    "Great Bay Coast Beach Rocks Underwater (1)": MMRLocationData(
        region="Great Bay",
        address=0x18370A,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Beach Rocks Underwater (2)": MMRLocationData(
        region="Great Bay",
        address=0x183724,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Beach Rocks Underwater (3)": MMRLocationData(
        region="Great Bay",
        address=0x183723,
        can_create=lambda options: options.rocksanity.value
    ),
    "Great Bay Coast Beach Rocks Underwater (4)": MMRLocationData(
        region="Great Bay",
        address=0x18370B,
        can_create=lambda options: options.rocksanity.value
    ),

    # Zora Cape Beach Rocks
    "Zora Cape Beach Rocks (0)": MMRLocationData(
        region="Zora Cape",
        address=0x18380B,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Beach Rocks (1)": MMRLocationData(
        region="Zora Cape",
        address=0x18380A,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Beach Rocks (2)": MMRLocationData(
        region="Zora Cape",
        address=0x18380C,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Beach Rocks (3)": MMRLocationData(
        region="Zora Cape",
        address=0x18380D,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Beach Rocks (4)": MMRLocationData(
        region="Zora Cape",
        address=0x18380E,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Beach Rocks (5)": MMRLocationData(
        region="Zora Cape",
        address=0x183806,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Beach Rocks (6)": MMRLocationData(
        region="Zora Cape",
        address=0x183807,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Beach Rocks (7)": MMRLocationData(
        region="Zora Cape",
        address=0x183805,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Beach Rocks (8)": MMRLocationData(
        region="Zora Cape",
        address=0x183808,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Beach Rocks (9)": MMRLocationData(
        region="Zora Cape",
        address=0x183809,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Boulder Over Grotto": MMRLocationData(
        region="Zora Cape",
        address=0x1B3802,
        can_create=lambda options: options.rocksanity.value
    ),    

    # Zora Cape Island Rocks (Req Hook)
    "Zora Cape Island Rocks (0)": MMRLocationData(
        region="Zora Cape",
        address=0x183801,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Island Rocks (1)": MMRLocationData(
        region="Zora Cape",
        address=0x183803,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Island Rocks (2)": MMRLocationData(
        region="Zora Cape",
        address=0x183802,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Island Rocks (3)": MMRLocationData(
        region="Zora Cape",
        address=0x183804,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Island Rocks (4)": MMRLocationData(
        region="Zora Cape",
        address=0x183800,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Great Fairy Boulders (0)": MMRLocationData(
        region="Zora Cape",
        address=0x1B3800,
        can_create=lambda options: options.rocksanity.value
    ),
    "Zora Cape Great Fairy Boulders (1)": MMRLocationData(
        region="Zora Cape",
        address=0x1B3801,
        can_create=lambda options: options.rocksanity.value
    ),          

    # Road To Ikana Rocks
    "Road To Ikana Grotto Boulder": MMRLocationData(
        region="Road to Ikana",
        address=0x1B5300,
        can_create=lambda options: options.rocksanity.value
    ),          
    "Road To Ikana Rock Circle (1)": MMRLocationData(
        region="Road to Ikana",
        address=0x265300,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Rock Circle (2)": MMRLocationData(
        region="Road to Ikana",
        address=0x265301,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Rock Circle (3)": MMRLocationData(
        region="Road to Ikana",
        address=0x265302,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Rock Circle (4)": MMRLocationData(
        region="Road to Ikana",
        address=0x265303,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Rock Circle (5)": MMRLocationData(
        region="Road to Ikana",
        address=0x265304,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Rock Circle (6)": MMRLocationData(
        region="Road to Ikana",
        address=0x265305,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Rock Circle (7)": MMRLocationData(
        region="Road to Ikana",
        address=0x265306,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Rock Circle (8)": MMRLocationData(
        region="Road to Ikana",
        address=0x265307,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Bomb Boulder (1)": MMRLocationData(
        region="Road to Ikana",
        address=0x1B5301,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Bomb Boulder (2)": MMRLocationData(
        region="Road to Ikana",
        address=0x1B5302,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Bomb Boulder (3)": MMRLocationData(
        region="Road to Ikana",
        address=0x1B5303,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Bomb Boulder (4)": MMRLocationData(
        region="Road to Ikana",
        address=0x1B5304,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Bomb Boulder (5)": MMRLocationData(
        region="Road to Ikana",
        address=0x1B5305,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Bomb Boulder (6)": MMRLocationData(
        region="Road to Ikana",
        address=0x1B5306,
        can_create=lambda options: options.rocksanity.value
    ),
    "Road To Ikana Bomb Boulder (7)": MMRLocationData(
        region="Road to Ikana",
        address=0x1B5307,
        can_create=lambda options: options.rocksanity.value
    ),                        

    # Ikana Graveyard Rock Circle
    "Ikana Graveyard Rock Circle (0)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x264310,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Rock Circle (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x264311,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Rock Circle (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x264312,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Rock Circle (3)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x264313,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Rock Circle (4)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x264314,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Rock Circle (5)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x264315,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Rock Circle (6)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x264316,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Rock Circle (7)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x264317,
        can_create=lambda options: options.rocksanity.value
    ),

    # Ikana Graveyard Captain Rockwall 
    "Ikana Graveyard Captain Rockwall (0)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x184310,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Captain Rockwall (1)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x184311,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Captain Rockwall (2)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x184312,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Captain Rockwall (3)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x184313,
        can_create=lambda options: options.rocksanity.value
    ),
    "Ikana Graveyard Captain Rockwall (4)": MMRLocationData(
        region="Ikana Graveyard",
        address=0x184314,
        can_create=lambda options: options.rocksanity.value
    ),

    # Inverted Stone Lower Tower Rocks
    "Inverted Stone Tower Rocks (1)": MMRLocationData(
        region="Stone Tower (Inverted)",
        address=0x185900,
        can_create=lambda options: options.rocksanity.value
    ),
    "Inverted Stone Tower Rocks (2)": MMRLocationData(
        region="Stone Tower (Inverted)",
        address=0x185901,
        can_create=lambda options: options.rocksanity.value
    ),

    # Soil

    # Termina Field Stump Soil
    "Termina Field Stump Soil": MMRLocationData(
        region="Termina Field",
        address=0x192DE3,
        can_create=lambda options: options.soilsanity.value
    ),

    # Termina Field Wall Soil
    "Termina Field Wall Soil (0)": MMRLocationData(
        region="Termina Field",
        address=0x192D42,
        can_create=lambda options: options.soilsanity.value
    ),
    "Termina Field Wall Soil (1)": MMRLocationData(
        region="Termina Field",
        address=0x192D40,
        can_create=lambda options: options.soilsanity.value
    ),
    "Termina Field Wall Soil (2)": MMRLocationData(
        region="Termina Field",
        address=0x192D41,
        can_create=lambda options: options.soilsanity.value
    ),

    # Termina Field Eastern Soil
    "Termina Field Eastern Soil": MMRLocationData(
        region="Termina Field",
        address=0x192DB3,
        can_create=lambda options: options.soilsanity.value
    ),

    # Termina Field Observatory Soil
    "Termina Field Observatory Soil (0)": MMRLocationData(
        region="Termina Field",
        address=0x192D50,
        can_create=lambda options: options.soilsanity.value
    ),
    "Termina Field Observatory Soil (1)": MMRLocationData(
        region="Termina Field",
        address=0x192D51,
        can_create=lambda options: options.soilsanity.value
    ),
    "Termina Field Observatory Soil (2)": MMRLocationData(
        region="Termina Field",
        address=0x192D52,
        can_create=lambda options: options.soilsanity.value
    ),

    # Swamp Spider House Rock Soil
    "Swamp Spider House Rock Soil (0)": MMRLocationData(
        region="Swamp Spider House",
        address=0x192760,
        can_create=lambda options: options.soilsanity.value
    ),
    "Swamp Spider House Rock Soil (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x192762,
        can_create=lambda options: options.soilsanity.value
    ),
    "Swamp Spider House Rock Soil (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x192761,
        can_create=lambda options: options.soilsanity.value
    ),

    # Swamp Spider House Gold Room Soil
    "Swamp Spider House Gold Room Soil": MMRLocationData(
        region="Swamp Spider House",
        address=0x192723,
        can_create=lambda options: options.soilsanity.value
    ),

    # Deku Palace Bean Seller Soil
    "Deku Palace Bean Seller Soil (0)": MMRLocationData(
        region="Deku Palace",
        address=0x1907D0,
        can_create=lambda options: options.soilsanity.value
    ),
    "Deku Palace Bean Seller Soil (1)": MMRLocationData(
        region="Deku Palace",
        address=0x1907D1,
        can_create=lambda options: options.soilsanity.value
    ),
    "Deku Palace Bean Seller Soil (2)": MMRLocationData(
        region="Deku Palace",
        address=0x1907D2,
        can_create=lambda options: options.soilsanity.value
    ),

    # Deku Palace Exterior Soil
    "Deku Palace Exterior Soil (0)": MMRLocationData(
        region="Deku Palace",
        address=0x192B30,
        can_create=lambda options: options.soilsanity.value
    ),
    "Deku Palace Exterior Soil (1)": MMRLocationData(
        region="Deku Palace",
        address=0x192B32,
        can_create=lambda options: options.soilsanity.value
    ),
    "Deku Palace Exterior Soil (2)": MMRLocationData(
        region="Deku Palace",
        address=0x192B31,
        can_create=lambda options: options.soilsanity.value
    ),

    # Romani Ranch Day 1 Soil
    "Romani Ranch Day 1 Soil": MMRLocationData(
        region="Romani Ranch",
        address=0x193543,
        can_create=lambda options: options.soilsanity.value
    ),

    # Romani Ranch Day 2/3 Soil
    "Romani Ranch Day 2/3 Soil (0)": MMRLocationData(
        region="Romani Ranch",
        address=0x193540,
        can_create=lambda options: options.soilsanity.value
    ),
    "Romani Ranch Day 2/3 Soil (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x193542,
        can_create=lambda options: options.soilsanity.value
    ),
    "Romani Ranch Day 2/3 Soil (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x193541,
        can_create=lambda options: options.soilsanity.value
    ),

    # Romani Ranch Doggy Racetrack Soil
    "Romani Ranch Doggy Racetrack Soil": MMRLocationData(
        region="Romani Ranch",
        address=0x194103,
        can_create=lambda options: options.soilsanity.value
    ),

    # Great Bay Coast Soil
    "Great Bay Coast Soil": MMRLocationData(
        region="Great Bay",
        address=0x193793,
        can_create=lambda options: options.soilsanity.value
    ),

    # Secret Shrine Soil
    "Secret Shrine Soil (0)": MMRLocationData(
        region="Secret Shrine",
        address=0x196000,
        can_create=lambda options: options.soilsanity.value
    ),
    "Secret Shrine Soil (1)": MMRLocationData(
        region="Secret Shrine",
        address=0x196001,
        can_create=lambda options: options.soilsanity.value
    ),
    "Secret Shrine Soil (2)": MMRLocationData(
        region="Secret Shrine",
        address=0x196002,
        can_create=lambda options: options.soilsanity.value
    ),

    # Stone Tower Inverted Soils
    "Stone Tower Inverted Soils (0)": MMRLocationData(
        region="Stone Tower (Inverted)",
        address=0x195903,
        can_create=lambda options: options.soilsanity.value
    ),
    "Stone Tower Inverted Soils (1)": MMRLocationData(
        region="Stone Tower (Inverted)",
        address=0x195933,
        can_create=lambda options: options.soilsanity.value
    ),

    # Snowballs

    # Path to Mountains Snowballs
    "Path to Mountains Snowballs (1)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C00,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (2)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C0C,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (3)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C07,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (4)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C01,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (5)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C09,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (6)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C02,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (7)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C08,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (8)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C04,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (9)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C05,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (10)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C06,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (11)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C03,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (12)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C0A,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (13)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C0E,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (14)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C0B,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Mountains Snowballs (15)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x211C0D,
        can_create=lambda options: options.snowsanity.value
    ),

    # Mountain Village Day 1 Snowballs
    "Mountain Village Day 1 Snowballs (0)": MMRLocationData(
        region="Mountain Village",
        address=0x215000,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (1)": MMRLocationData(
        region="Mountain Village",
        address=0x215001,
        can_create=lambda options: options.snowsanity.value
    ),    
    "Mountain Village Day 1 Snowballs (2)": MMRLocationData(
        region="Mountain Village",
        address=0x215002,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (3)": MMRLocationData(
        region="Mountain Village",
        address=0x215003,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (4)": MMRLocationData(
        region="Mountain Village",
        address=0x215004,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (5)": MMRLocationData(
        region="Mountain Village",
        address=0x215005,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (6)": MMRLocationData(
        region="Mountain Village",
        address=0x215006,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (7)": MMRLocationData(
        region="Mountain Village",
        address=0x215007,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (8)": MMRLocationData(
        region="Mountain Village",
        address=0x215008,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (9)": MMRLocationData(
        region="Mountain Village",
        address=0x21500B,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (10)": MMRLocationData(
        region="Mountain Village",
        address=0x21500C,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (11)": MMRLocationData(
        region="Mountain Village",
        address=0x21500D,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (12)": MMRLocationData(
        region="Mountain Village",
        address=0x21500E,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Day 1 Snowballs (13)": MMRLocationData(
        region="Mountain Village",
        address=0x21500F,
        can_create=lambda options: options.snowsanity.value
    ),

    # Snowballs Outside Goron Graveyard
    "Snowballs Outside Goron Graveyard (0)": MMRLocationData(
        region="Mountain Village",
        address=0x21500A,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowballs Outside Goron Graveyard (1)": MMRLocationData(
        region="Mountain Village",
        address=0x215009,
        can_create=lambda options: options.snowsanity.value
    ),

    # Twin Islands Day 1 Snowballs
    "Twin Islands Day 1 Snowballs (0)": MMRLocationData(
        region="Twin Islands",
        address=0x215D0D,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (1)": MMRLocationData(
        region="Twin Islands",
        address=0x215D0E,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (2)": MMRLocationData(
        region="Twin Islands",
        address=0x215D05,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (3)": MMRLocationData(
        region="Twin Islands",
        address=0x215D04,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (4)": MMRLocationData(
        region="Twin Islands",
        address=0x215D07,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (5)": MMRLocationData(
        region="Twin Islands",
        address=0x215D01,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (6)": MMRLocationData(
        region="Twin Islands",
        address=0x215D0B,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (7)": MMRLocationData(
        region="Twin Islands",
        address=0x215D06,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (8)": MMRLocationData(
        region="Twin Islands",
        address=0x215D08,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (9)": MMRLocationData(
        region="Twin Islands",
        address=0x215D09,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (10)": MMRLocationData(
        region="Twin Islands",
        address=0x215D02,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (11)": MMRLocationData(
        region="Twin Islands",
        address=0x215D0C,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (12)": MMRLocationData(
        region="Twin Islands",
        address=0x215D03,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (13)": MMRLocationData(
        region="Twin Islands",
        address=0x215D0A,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (14)": MMRLocationData(
        region="Twin Islands",
        address=0x215D0F,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Islands Day 1 Snowballs (15)": MMRLocationData(
        region="Twin Islands",
        address=0x215D00,
        can_create=lambda options: options.snowsanity.value
    ),

    # Twin Isles Snowballs Near Grotto
    "Twin Isles Snowballs Near Grotto (1)": MMRLocationData(
        region="Twin Islands",
        address=0x215D11,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Isles Snowballs Near Grotto (2)": MMRLocationData(
        region="Twin Islands",
        address=0x215D12,
        can_create=lambda options: options.snowsanity.value
    ),
    "Twin Isles Snowballs Near Grotto (3)": MMRLocationData(
        region="Twin Islands",
        address=0x215D10,
        can_create=lambda options: options.snowsanity.value
    ),

    # Goron Village Snowballs
    "Goron Village Snowballs (0)": MMRLocationData(
        region="Goron Village",
        address=0x214D00,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (1)": MMRLocationData(
        region="Goron Village",
        address=0x214D0B,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (2)": MMRLocationData(
        region="Goron Village",
        address=0x214D14,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (3)": MMRLocationData(
        region="Goron Village",
        address=0x214D0D,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (4)": MMRLocationData(
        region="Goron Village",
        address=0x214D10,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (5)": MMRLocationData(
        region="Goron Village",
        address=0x214D0E,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (6)": MMRLocationData(
        region="Goron Village",
        address=0x214D13,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (7)": MMRLocationData(
        region="Goron Village",
        address=0x214D11,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (8)": MMRLocationData(
        region="Goron Village",
        address=0x214D0C,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (9)": MMRLocationData(
        region="Goron Village",
        address=0x214D03,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (10)": MMRLocationData(
        region="Goron Village",
        address=0x214D0F,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (11)": MMRLocationData(
        region="Goron Village",
        address=0x214D05,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (12)": MMRLocationData(
        region="Goron Village",
        address=0x214D12,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (13)": MMRLocationData(
        region="Goron Village",
        address=0x214D07,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (14)": MMRLocationData(
        region="Goron Village",
        address=0x214D09,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (15)": MMRLocationData(
        region="Goron Village",
        address=0x214D02,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (16)": MMRLocationData(
        region="Goron Village",
        address=0x214D04,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (17)": MMRLocationData(
        region="Goron Village",
        address=0x214D06,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (18)": MMRLocationData(
        region="Goron Village",
        address=0x214D08,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (19)": MMRLocationData(
        region="Goron Village",
        address=0x214D0A,
        can_create=lambda options: options.snowsanity.value
    ),
    "Goron Village Snowballs (20)": MMRLocationData(
        region="Goron Village",
        address=0x214D01,
        can_create=lambda options: options.snowsanity.value
    ),


    # Path to Snowhead Snowballs
    "Path to Snowhead Snowballs (0)": MMRLocationData(
        region="Path to Snowhead",
        address=0x215B06,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Snowhead Snowballs (1)": MMRLocationData(
        region="Path to Snowhead",
        address=0x215B05,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Snowhead Snowballs (2)": MMRLocationData(
        region="Path to Snowhead",
        address=0x215B04,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Snowhead Snowballs (3)": MMRLocationData(
        region="Path to Snowhead",
        address=0x215B00,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Snowhead Snowballs (4)": MMRLocationData(
        region="Path to Snowhead",
        address=0x215B01,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Snowhead Snowballs (5)": MMRLocationData(
        region="Path to Snowhead",
        address=0x215B02,
        can_create=lambda options: options.snowsanity.value
    ),
    "Path to Snowhead Snowballs (6)": MMRLocationData(
        region="Path to Snowhead",
        address=0x215B03,
        can_create=lambda options: options.snowsanity.value
    ),

    # Outside Snowhead Temple Snowballs
    "Outside Snowhead Temple Snowballs (0)": MMRLocationData(
        region="Snowhead",
        address=0x215C01,
        can_create=lambda options: options.snowsanity.value
    ),
    "Outside Snowhead Temple Snowballs (1)": MMRLocationData(
        region="Snowhead",
        address=0x215C03,
        can_create=lambda options: options.snowsanity.value
    ),
    "Outside Snowhead Temple Snowballs (2)": MMRLocationData(
        region="Snowhead",
        address=0x215C00,
        can_create=lambda options: options.snowsanity.value
    ),
    "Outside Snowhead Temple Snowballs (3)": MMRLocationData(
        region="Snowhead",
        address=0x215C02,
        can_create=lambda options: options.snowsanity.value
    ),
    "Outside Snowhead Temple Snowballs (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x215C04,
        can_create=lambda options: options.snowsanity.value
    ),
    "Outside Snowhead Temple Snowballs (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x215C06,
        can_create=lambda options: options.snowsanity.value
    ),
    "Outside Snowhead Temple Snowballs (6)": MMRLocationData(
        region="Snowhead Temple",
        address=0x215C07,
        can_create=lambda options: options.snowsanity.value
    ),
    "Outside Snowhead Temple Snowballs (7)": MMRLocationData(
        region="Snowhead Temple",
        address=0x215C08,
        can_create=lambda options: options.snowsanity.value
    ),
    "Outside Snowhead Temple Snowballs (8)": MMRLocationData(
        region="Snowhead Temple",
        address=0x215C09,
        can_create=lambda options: options.snowsanity.value
    ),
    "Outside Snowhead Temple Snowballs (9)": MMRLocationData(
        region="Snowhead Temple",
        address=0x215C05,
        can_create=lambda options: options.snowsanity.value
    ),

    # Snowhead Temple Lower Runway Room Snowballs
    "Snowhead Temple Lower Runway Room Snowballs (0)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212121,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Lower Runway Room Snowballs (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212120,
        can_create=lambda options: options.snowsanity.value
    ),    
    "Snowhead Temple Icicle Room Snowballs (0)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212172,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Icicle Room Snowballs (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212171,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Icicle Room Snowballs (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212175,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Icicle Room Snowballs (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212170,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Icicle Room Snowballs (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212174,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Icicle Room Snowballs (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212173,
        can_create=lambda options: options.snowsanity.value
    ),

    # Snowhead Temple Main Room 2nd Floor Snowballs
    "Snowhead Temple Main Room 2nd Floor Snowballs (0)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212145,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Main Room 2nd Floor Snowballs (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212141,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Main Room 2nd Floor Snowballs (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212140,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Main Room 2nd Floor Snowballs (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212142,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Main Room 2nd Floor Snowballs (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212143,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple Main Room 2nd Floor Snowballs (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212144,
        can_create=lambda options: options.snowsanity.value
    ),

    # Snowhead Temple 3rd Floor Bridge Snowballs
    "Snowhead Temple 3rd Floor Bridge Snowballs (0)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212147,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple 3rd Floor Bridge Snowballs (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212146,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple 3rd Floor Bridge Snowballs (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212148,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple 3rd Floor Bridge Snowballs (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x212149,
        can_create=lambda options: options.snowsanity.value
    ),

    # Snowhead Temple 3rd Floor Behind Locked Door Snowballs
    "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (0)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2121A0,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2121A3,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2121A2,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2121A4,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2121A1,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2121A7,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (6)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2121A5,
        can_create=lambda options: options.snowsanity.value
    ),
    "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (7)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2121A6,
        can_create=lambda options: options.snowsanity.value
    ),

    # Mountain Village Spring Snowballs
    "Mountain Village Spring Snowballs (0)": MMRLocationData(
        region="Mountain Village",
        address=0x215A03,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Spring Snowballs (1)": MMRLocationData(
        region="Mountain Village",
        address=0x215A02,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Spring Snowballs (2)": MMRLocationData(
        region="Mountain Village",
        address=0x215A01,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Spring Snowballs (3)": MMRLocationData(
        region="Mountain Village",
        address=0x215A04,
        can_create=lambda options: options.snowsanity.value
    ),
    "Mountain Village Spring Snowballs (4)": MMRLocationData(
        region="Mountain Village",
        address=0x215A00,
        can_create=lambda options: options.snowsanity.value
    ),
    
    # Crates/ Barrels Woodsanity
    "Laundry Pool Crate (0)": MMRLocationData(
        region="Clock Town",
        address=0x257000,
        can_create=lambda options: options.woodsanity.value
    ),    
    "East Clock Town Crates (0)": MMRLocationData(
        region="Clock Town",
        address=0x256C00,
        can_create=lambda options: options.woodsanity.value
    ),
    "East Clock Town Crates (1)": MMRLocationData(
        region="Clock Town",
        address=0x256C01,
        can_create=lambda options: options.woodsanity.value
    ),
    "Termina Field Business Scrub Grotto Crate (1)": MMRLocationData(
        region="Termina Field",
        address=0x270790,
        can_create=lambda options: options.woodsanity.value
    ),
    "Gorman Racetrack Behind Fence Crate": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x276A00,
        can_create=lambda options: options.woodsanity.value
    ),
    # Romani Ranch Crates
    "Romani Ranch Crate Next To Romani (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x273500,
        can_create=lambda options: options.woodsanity.value
    ), 
    "Romani Ranch Baby Cuccoo Crates (0)": MMRLocationData(
        region="Romani Ranch",
        address=0x274200,
        can_create=lambda options: options.woodsanity.value
    ),
    "Romani Ranch Baby Cuccoo Crates (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x274201,
        can_create=lambda options: options.woodsanity.value
    ),
    "Romani Ranch Baby Cuccoo Crates (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x274202,
        can_create=lambda options: options.woodsanity.value
    ),
    # Swamp Spider House 
    "Swamp Spider House Monument Room Crates (0)": MMRLocationData(
        region="Swamp Spider House",
        address=0x272730,
        can_create=lambda options: options.woodsanity.value
    ),
    "Swamp Spider House Monument Room Crates (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x272731,
        can_create=lambda options: options.woodsanity.value
    ),                  
    "Swamp Spider House Monument Room Crates (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x272732,
        can_create=lambda options: options.woodsanity.value
    ),                  
    "Swamp Spider House Monument Room Crates (3)": MMRLocationData(
        region="Swamp Spider House",
        address=0x272733,
        can_create=lambda options: options.woodsanity.value
    ),                  
    "Swamp Spider House Monument Room Crates (4)": MMRLocationData(
        region="Swamp Spider House",
        address=0x272734,
        can_create=lambda options: options.woodsanity.value
    ),                  
    "Swamp Spider House Monument Room Crates (5)": MMRLocationData(
        region="Swamp Spider House",
        address=0x272735,
        can_create=lambda options: options.woodsanity.value
    ),                          
    "Swamp Spider House Gold Room Crates (0)": MMRLocationData(
        region="Swamp Spider House",
        address=0x272720,
        can_create=lambda options: options.woodsanity.value
    ),
    "Swamp Spider House Gold Room Crates (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x272721,
        can_create=lambda options: options.woodsanity.value
    ),

    # Goron Village Keg Goron Crate
    "Goron Village Keg Goron Crate (1)": MMRLocationData(
        region="Goron Village",
        address=0x274D10,
        can_create=lambda options: options.woodsanity.value
    ),
    "Goron Village Keg Goron Crate (Spring) (1)": MMRLocationData(
        region="Goron Village",
        address=0x274810,
        can_create=lambda options: options.woodsanity.value
    ),
    "Ocean Spiderhouse Basement Crate (1)": MMRLocationData(
        region="Ocean Spider House",
        address=0x272850,
        can_create=lambda options: options.woodsanity.value
    ),
    # Pirates Fortress Entrance Wood Barrier
    "Pirates Fortress Entrance Bonk Board (0)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x223700,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates Fortress Entrance Bonk Board (1)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x223701,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates Fortress Entrance Bonk Board (2)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x223702,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates Fortress Entrance Bonk Board (3)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x223703,
        can_create=lambda options: options.woodsanity.value
    ),
    # Pirates Fortress Sewers Wooden Barriers
    "Pirates Fortress Sewers Bonk Board (0)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223C0,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates Fortress Sewers Bonk Board (1)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223C1,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates Fortress Sewers Bonk Board (2)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223C2,
        can_create=lambda options: options.woodsanity.value
    ),                      
    # Pirates' Fortress Sewers Barrels/Crates
    "Pirates' Fortress Sewers Barrel (1)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223B0,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (2)": MMRLocationData(
        region="Pirates' Fortress Sewers", 
        address=0x2223B1,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (3)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223B2,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (4)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223B3,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (5)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223B4,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (6)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223B5,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (7)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223B6,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (8)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223B7,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (9)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223B8,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (10)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223B9,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (11)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223BA,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (12)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223BB,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (13)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223BC,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (14)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223BD,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (15)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223BE,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Barrel (16)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x2223BF,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Exit Barrel (1)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x222390,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Exit Barrel (2)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x222391,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Exit Barrel (3)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x222392,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Exit Barrel (4)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x222393,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates' Fortress Sewers Exit Barrel (5)": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x222394,
        can_create=lambda options: options.woodsanity.value
    ),                                                                
    # Pirates Fortress Interior Crates
    "Pirates Fortress' Interior Crates (0)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x271400,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates Fortress' Interior Crates (1)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x271401,
        can_create=lambda options: options.woodsanity.value
    ),
    "Pirates Fortress' Interior Crates (2)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x271402,
        can_create=lambda options: options.woodsanity.value
    ), 
    "Pirates Fortress' Exterior Balcony Barrel (1)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x223B00,
        can_create=lambda options: options.woodsanity.value
    ),         
    "Pirates' Fortress Leader's Room Barrel (1)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x222330,
        can_create=lambda options: options.woodsanity.value        
    ),
    "Pirates' Fortress Leader's Room Barrel (2)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x222331,
        can_create=lambda options: options.woodsanity.value        
    ),
    "Pirates' Fortress Guarded Bridge Barrel (0)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x221400,
        can_create=lambda options: options.woodsanity.value        
    ),
    "Pirates Fortress Interior Room Past Pink Guard Barrel (0)": MMRLocationData(
        region="Pirates' Fortress (Interior)",
        address=0x2223D0,
        can_create=lambda options: options.woodsanity.value        
    ),           
    #Dungeon Woodsanity
    #Snowhead Temple
    
    "Snowhead Temple Lava Bridge Room Crate (0)": MMRLocationData(
        region="Snowhead Temple",
        address=0x272120,
        can_create=lambda options: options.woodsanity.value
    ),    
    "Snowhead Temple Elevator Room Crates (0)": MMRLocationData(
        region="Snowhead Temple",
        address=0x272190,
        can_create=lambda options: options.woodsanity.value
    ),
    "Snowhead Temple Elevator Room Crates (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x272191,
        can_create=lambda options: options.woodsanity.value
    ),
    "Snowhead Temple Elevator Room Crates (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x272192,
        can_create=lambda options: options.woodsanity.value
    ),
    "Snowhead Temple Elevator Room Crates (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x272193,
        can_create=lambda options: options.woodsanity.value
    ),
    "Snowhead Temple Elevator Room Crates (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x272194,
        can_create=lambda options: options.woodsanity.value
    ),
    "Snowhead Temple Timed Switch Puzzle Room Crate (0)": MMRLocationData(
        region="Snowhead Temple",
        address=0x272180,
        can_create=lambda options: options.woodsanity.value
    ),
    "Snowhead Temple Timed Switch Puzzle Room Crate (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x272181,
        can_create=lambda options: options.woodsanity.value
    ),        
    # Great Bay Temple Crates/Barrels
    "Great Bay Temple Entrance Barrels (0)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249D0,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Entrance Barrels (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249D1,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Entrance Barrels (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249D2,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Entrance Barrels (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249D3,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Entrance Barrels (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249D4,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Entrance Barrels (5)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249D5,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Entrance Barrels (6)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249D6,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Entrance Barrels (7)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249D7,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Room Behind 1F Waterfall Barrels (0)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249C0,
        can_create=lambda options: options.woodsanity.value
    ),

    "Great Bay Temple Room Behind 1F Waterfall Barrels (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249C1,
        can_create=lambda options: options.woodsanity.value
    ),

    "Great Bay Temple Room Behind 1F Waterfall Barrels (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2249C2,
        can_create=lambda options: options.woodsanity.value
    ),    
    "Great Bay Temple 1F Red Valve Room Barrels (0)": MMRLocationData(
        region="Great Bay Temple",
        address=0x224920,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Red Valve Room Barrels (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x224921,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Red Valve Room Barrels (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x224922,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Red Valve Room Barrels (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x224923,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Red Valve Room Barrels (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x224924,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Red Valve Room Crates (0)": MMRLocationData(
        region="Great Bay Temple",
        address=0x274920,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Red Valve Room Crates (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x274921,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Red Valve Room Crates (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x274922,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Red Valve Room Crates (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x274923,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Red Valve Room Crates (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x274924,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (0)": MMRLocationData(
        region="Great Bay Temple",
        address=0x254950,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x254951,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x254952,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x254953,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x254954,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (5)": MMRLocationData(
        region="Great Bay Temple",
        address=0x254955,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (6)": MMRLocationData(
        region="Great Bay Temple",
        address=0x254956,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (7)": MMRLocationData(
        region="Great Bay Temple",
        address=0x254957,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (8)": MMRLocationData(
        region="Great Bay Temple",
        address=0x254958,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (9)": MMRLocationData(
        region="Great Bay Temple",
        address=0x254959,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (10)": MMRLocationData(
        region="Great Bay Temple",
        address=0x25495A,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (11)": MMRLocationData(
        region="Great Bay Temple",
        address=0x25495B,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (12)": MMRLocationData(
        region="Great Bay Temple",
        address=0x25495C,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (13)": MMRLocationData(
        region="Great Bay Temple",
        address=0x25495D,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (14)": MMRLocationData(
        region="Great Bay Temple",
        address=0x25495E,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple 1F Frog Miniboss Crates (15)": MMRLocationData(
        region="Great Bay Temple",
        address=0x25495F,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Seesaw Room Crates (0)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2749A0,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Seesaw Room Crates (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2749A1,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Seesaw Room Crates (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2749A2,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Seesaw Room Crates (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2749A3,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Seesaw Room Crates (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2749A4,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Seesaw Room Crates (5)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2749A5,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Seesaw Room Crates (6)": MMRLocationData(
        region="Great Bay Temple",
        address=0x2749A6,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Green Pipe Frozen Waterwheel Crates (0)": MMRLocationData(
        region="Great Bay Temple",
        address=0x224990,
        can_create=lambda options: options.woodsanity.value
    ),
    "Great Bay Temple Green Pipe Frozen Waterwheel Crates (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x224991,
        can_create=lambda options: options.woodsanity.value
    ),
    # Stone Tower Temple Crates
    "Stone Tower Temple Entrance Room Crates (0)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x251600,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Entrance Room Crates (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x251601,
        can_create=lambda options: options.woodsanity.value
    ),        
    "Stone Tower Temple Mirror Room Crates (0)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x271670,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Mirror Room Crates (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x271671,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Eyegore Room Crates (0)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x251610,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Eyegore Room Crates (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x251611,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Eyegore Room Crates (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x251612,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Behind Bombable Wall Crates (0)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x271620,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Behind Bombable Wall Crates (1)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x271621,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Behind Bombable Wall Crates (2)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x271622,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Behind Bombable Wall Crates (3)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x271623,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Behind Bombable Wall Crates (4)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x271624,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Behind Bombable Wall Crates (5)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x251620,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Behind Bombable Wall Crates (6)": MMRLocationData(
        region="Stone Tower Temple",
        address=0x251621,
        can_create=lambda options: options.woodsanity.value
    ),         
    # Stone Tower Temple Inverted Crates
    "Stone Tower Temple Inverted Entry Crates (0)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251800,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Inverted Entry Crates (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251801,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Inverted Entry Crates (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251802,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Inverted Entry Crates (3)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251803,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Inverted Entry Crates (4)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251804,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Inverted Thin Hallway Crates (0)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251830,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Inverted Thin Hallway Crates (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251831,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Inverted Thin Hallway Crates (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251832,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Inverted Thin Hallway Crates (3)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251833,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Inverted Thin Hallway Crates (4)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251834,
        can_create=lambda options: options.woodsanity.value
    ),
    "Stone Tower Temple Inverted Thin Hallway Crates (5)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x251835,
        can_create=lambda options: options.woodsanity.value
    ),

    # Scarecrow Items
    "Clock Town Trading Post Scarecrow": MMRLocationData(
        region="Clock Town",
        address=0x303400,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Astral Observatory Scarecrow": MMRLocationData(
        region="Clock Town",
        address=0x302910,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Mountain Village Rooftop Scarecrow": MMRLocationData(
        region="Mountain Village",
        address=0x305000,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Mountain Village Spring Rooftop Scarecrow": MMRLocationData(
        region="Mountain Village",
        address=0x305A00,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Path to Snowhead Scarecrow": MMRLocationData(
        region="Path to Snowhead",
        address=0x305B00,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Path to Snowhead Spring Scarecrow": MMRLocationData(
        region="Path to Snowhead",
        address=0x305C00,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Twin Islands Scarecrow": MMRLocationData(
        region="Twin Islands",
        address=0x305D00,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Twin Islands (Spring) Scarecrow": MMRLocationData(
        region="Twin Islands",
        address=0x305E00,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Snowhead Temple Lower Scarecrow": MMRLocationData(
        region="Snowhead Temple",
        address=0x302140,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Snowhead Temple Hidden Alcove Scarecrow": MMRLocationData(
        region="Snowhead Temple",
        address=0x302141,
        can_create=lambda options: options.scarecrowsanity.value
    ),        
    "Great Bay Coast Rock Wall Scarecrow": MMRLocationData(
        region="Great Bay",
        address=0x303700,
        can_create=lambda options: options.scarecrowsanity.value
    ),    
    "Zora Cape Beavers Scarecrow": MMRLocationData(
        region="Zora Cape",
        address=0x303800,
        can_create=lambda options: options.scarecrowsanity.value
    ), 
    "Zora Cape Island Scarecrow": MMRLocationData(
        region="Zora Cape",
        address=0x303801,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Zora Hall Pervert Scarecrow": MMRLocationData(
        region="Zora Hall",
        address=0x303300,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Road to Ikana Scarecrow": MMRLocationData(
        region="Road to Ikana",
        address=0x305300,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Stone Tower Lower Scarecrow": MMRLocationData(
        region="Stone Tower",
        address=0x305800,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Stone Tower Upper Scarecrow": MMRLocationData(
        region="Stone Tower",
        address=0x305801,
        can_create=lambda options: options.scarecrowsanity.value
    ),

    # Icicles

    # Snowhead Temple
    "Snowhead Temple Entry Block Icicles (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232100,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Entry Block Icicles (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232105,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Entry Block Icicles (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232101,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Entry Block Icicles (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232106,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Entry Block Icicles (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232104,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Grey Door Icicles (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232103,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Grey Door Icicles (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232102,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Grey Door Ceiling Icicles (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232108,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Grey Door Ceiling Icicles (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232107,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Frozen Block Ceiling Icicle (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232110,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Frozen Block Ceiling Icicle (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232111,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Frozen Block Ceiling Icicle (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232112,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Frozen Block Ceiling Icicle (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232113,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Frozen Block Ceiling Icicle (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232114,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Icicle Room Ceiling Icicle (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232170,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Icicle Room Ceiling Icicle (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232171,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Icicle Room Ceiling Icicle (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232172,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Icicle Room Ceiling Icicle (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232173,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Icicle Room Ceiling Icicle (5)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232174,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple 3F Behind Locked Door Icicles (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2321A0,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple 3F Behind Locked Door Icicles (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2321A1,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple 3F Behind Locked Door Icicles (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2321A2,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple 3F Behind Locked Door Ceiling Icicles (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2321A3,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple 3F Behind Locked Door Ceiling Icicles (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2321A4,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple 3F Behind Locked Door Ceiling Icicles (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2321A5,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple 3F Behind Locked Door Ceiling Icicles (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x2321A6,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple 4F Outside Wizzrobe Icicles (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232140,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple 4F Outside Wizzrobe Icicles (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232144,
        can_create=lambda options: options.iciclesanity.value
    ),
    # Snowhead Temple Outside Boss Icicles
    "Snowhead Temple Outside Boss Door Icicles (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232141,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Outside Boss Door Icicles (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232142,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Outside Boss Door Icicles (3)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232143,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Snowhead Temple Outside Boss Door Icicles (4)": MMRLocationData(
        region="Snowhead Temple",
        address=0x232145,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Great Bay Temple Outside Frog Miniboss Door Icicles (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x234940,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Great Bay Temple Outside Frog Miniboss Door Icicles (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x234941,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Great Bay Temple Outside Frog Miniboss Door Icicles (3)": MMRLocationData(
        region="Great Bay Temple",
        address=0x234942,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Great Bay Temple Outside Frog Miniboss Door Icicles (4)": MMRLocationData(
        region="Great Bay Temple",
        address=0x234943,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Great Bay Temple Outside Frog Miniboss Door Icicles (5)": MMRLocationData(
        region="Great Bay Temple",
        address=0x234944,
        can_create=lambda options: options.iciclesanity.value
    ),         
    "Bottom of the Well Icicle (1)": MMRLocationData(
        region="Great Bay Temple",
        address=0x234B11,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Bottom of the Well Icicle (2)": MMRLocationData(
        region="Great Bay Temple",
        address=0x234B10,
        can_create=lambda options: options.iciclesanity.value
    ),              
    "Goron Trial Icicles (1)": MMRLocationData(
        region="The Moon",
        address=0x233F00,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Goron Trial Icicles (2)": MMRLocationData(
        region="The Moon",
        address=0x233F01,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Goron Trial Icicles (3)": MMRLocationData(
        region="The Moon",
        address=0x233F02,
        can_create=lambda options: options.iciclesanity.value
    ),
    "Goron Trial Icicles (4)": MMRLocationData(
        region="The Moon",
        address=0x233F03,
        can_create=lambda options: options.iciclesanity.value
    ),

    # Hivesanity

    # Termina Field
    "Termina Field Bombable Rock Grotto Hive (1)": MMRLocationData(
        region="Termina Field",
        address=0x248000,
        can_create=lambda options: options.hivesanity.value
    ),
    "Termina Field Bio Baba Grotto Hive (1)": MMRLocationData(
        region="Termina Field",
        address=0x248BB1,
        can_create=lambda options: options.hivesanity.value
    ),
    "Termina Field Bio Baba Grotto Hive (2)": MMRLocationData(
        region="Termina Field",
        address=0x248BB2,
        can_create=lambda options: options.hivesanity.value
    ),   
    "Termina Field Cow Grotto Hive": MMRLocationData(
        region="Termina Field",
        address=0x24ADA0,
        can_create=lambda options: options.hivesanity.value
    ),   
    # Southern Swamp Hives
    "Southern Swamp Hive Near Frog (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x244500,
        can_create=lambda options: options.hivesanity.value
    ),
    # Swamp Spider House Hives
    "Swamp Spider House Giant Pot Room Hives (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x242740,
        can_create=lambda options: options.hivesanity.value
    ),
    "Swamp Spider House Giant Pot Room Hives (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x242741,
        can_create=lambda options: options.hivesanity.value
    ),
    "Swamp Spider House Giant Pot Room Hives (3)": MMRLocationData(
        region="Swamp Spider House",
        address=0x242744,
        can_create=lambda options: options.hivesanity.value
    ),
    "Swamp Spider House Gold Room Hives (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x242720,
        can_create=lambda options: options.hivesanity.value
    ),
    "Swamp Spider House Gold Room Hives (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x242721,
        can_create=lambda options: options.hivesanity.value
    ),    
    "Swamp Spider House Gold Room Hives (3)": MMRLocationData(
        region="Swamp Spider House",
        address=0x242722,
        can_create=lambda options: options.hivesanity.value
    ),
    "Swamp Spider Tree Room Hives (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x242750,
        can_create=lambda options: options.hivesanity.value
    ),
    "Swamp Spider Tree Room Hives (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x242751,
        can_create=lambda options: options.hivesanity.value
    ),
    "Swamp Spider Tree Room Hives (3)": MMRLocationData(
        region="Swamp Spider House",
        address=0x242753,
        can_create=lambda options: options.hivesanity.value
    ),            
    # Woodfall Temple Hives
    "Woodfall Temple Entrance Hive (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x241B20,
        can_create=lambda options: options.hivesanity.value
    ),
    "Woodfall Temple Push Block Hive (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x241B31,
        can_create=lambda options: options.hivesanity.value
    ),
    # Mountain Village Spring Hives   
    "Mountain Village Spring Tree Hive (1)": MMRLocationData(
        region="Mountain Village",
        address=0x245A00,
        can_create=lambda options: options.hivesanity.value
    ),
    "Great Bay Coast Cow Grotto Hive": MMRLocationData(
        region="Great Bay",
        address=0x24B7A0,
        can_create=lambda options: options.hivesanity.value
    ),
    #People complained so its removed now.

    # # Pirates Fortress Interior Guarded Hive From Barrel  
    # "Pirates Fortress Interior Leaders Hive From Lower Barrels": MMRLocationData(
    #     region="Pirates' Fortress (Interior)",
    #     address=0x242330,
    #     can_create=lambda options: options.hivesanity.value
    # ),

    # Real Fairies including Gossips/Butterfly Fairies
    
    # Termina Field Gossip Fairies
    "Termina Field Southern Tree Gossip Fairy": MMRLocationData(
        region="Termina Field",
        address=0x1F2D30,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Termina Field Near Thieving Bird Gossip Fairy": MMRLocationData(
        region="Termina Field",
        address=0x1F2D38,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Termina Field Near Bombable Rock Grotto Gossip Fairy": MMRLocationData(
        region="Termina Field",
        address=0x1F2D39,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Termina Field Gossip Grotto Gossip Fairy": MMRLocationData(
        region="Termina Field",
        address=0x1F0710,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Termina Field Near Songwall Gossip Fairy": MMRLocationData(
        region="Termina Field",
        address=0x1F2D3A,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Termina Field Eastern Corner Gossip Fairy": MMRLocationData(
        region="Termina Field",
        address=0x1F2D3B,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Termina Field Observatory Gossip Fairy": MMRLocationData(
        region="Termina Field",
        address=0x1F2D31,
        can_create=lambda options: options.realfairysanity.value
    ), 

    # Road to Southern Swamp Gossip Fairy
    "Road to Southern Swamp Gossip Fairy": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x1F4030,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Southern Swamp Gossip Fairies
    "Southern Swamp Near Witch Gossip Fairy": MMRLocationData(
        region="Southern Swamp",
        address=0x1F4531,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Southern Swamp Near Witch Post Dungeon Gossip Fairy": MMRLocationData(
        region="Southern Swamp",
        address=0x1F0031,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Swamp Spider House Gossip Fairy": MMRLocationData(
        region="Swamp Spider House",
        address=0x1F2737,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Milk Road Gossip Fairy
    "Milk Road Gossip Fairy": MMRLocationData(
        region="Milk Road",
        address=0x1F223E,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Romani Ranch Gossip Fairies
    "Romani Ranch Entry Gossip Fairy": MMRLocationData(
        region="Romani Ranch",
        address=0x1F3534,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Romani Ranch Tree Gossip Fairy": MMRLocationData(
        region="Romani Ranch",
        address=0x1F353C,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Romani Ranch Near Barn Gossip Fairy": MMRLocationData(
        region="Romani Ranch",
        address=0x1F353D,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Romani Ranch Baby Cuccoos Gossip Fairy": MMRLocationData(
        region="Romani Ranch",
        address=0x1F4232,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Romani Ranch Doggy Racetrack Gossip Fairy": MMRLocationData(
        region="Romani Ranch",
        address=0x1F4133,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Path To Mountain Village Gossip Fairy
    "Path To Mountain Village Gossip Fairy": MMRLocationData(
        region="Path to Mountain Village",
        address=0x1F1C33,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Mountain Village Gossip Fairies
    "Mountain Village Spring Waterfall Gossip Fairy": MMRLocationData(
        region="Mountain Village",
        address=0x1F5A36,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Mountain Village Spring Ramps To Goron Graveyard Gossip Fairy": MMRLocationData(
        region="Mountain Village",
        address=0x1F5A32,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Great Bay Gossip Fairy
    "Great Bay Coast Rock Wall Gossip Fairy": MMRLocationData(
        region="Great Bay",
        address=0x1F373F,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Zora Cape Gossip Fairy
    "Zora Cape Gossip Fairy": MMRLocationData(
        region="Zora Cape",
        address=0x1F3834,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Road To Ikana Gossip Fairy
    "Road To Ikana Gossip Fairy": MMRLocationData(
        region="Road to Ikana",
        address=0x1F5335,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Ikana Canyon Gossip Fairies
    "Ikana Canyon Near Octoroks Gossip Fairy": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x1F1336,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Ikana Canyon Across Ocean Deed Ravine Gossip Fairy": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x1F1335,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Ikana Canyon Near Ghost House Gossip Fairy": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x1F1337,
        can_create=lambda options: options.realfairysanity.value
    ),
    # Well Fairies 
    "Fairy Fountain Left Side Well (0)": MMRLocationData(
        region="Beneath the Well",
        address=0x0F4BB1,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Fairy Fountain Left Side Well (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x0F4BB2,
        can_create=lambda options: options.realfairysanity.value
    ),   
    "Fairy Fountain Left Side Well (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x0F4BB3,
        can_create=lambda options: options.realfairysanity.value
    ),   
    "Fairy Fountain Left Side Well (3)": MMRLocationData(
        region="Beneath the Well",
        address=0x0F4BB4,
        can_create=lambda options: options.realfairysanity.value
    ),   
    "Fairy Fountain Left Side Well (4)": MMRLocationData(
        region="Beneath the Well",
        address=0x0F4BB5,
        can_create=lambda options: options.realfairysanity.value
    ),   
    "Fairy Fountain Left Side Well (5)": MMRLocationData(
        region="Beneath the Well",
        address=0x0F4BB6,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Fairy Fountain Left Side Well (6)": MMRLocationData(
        region="Beneath the Well",
        address=0x0F4BB7,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Fairy Fountain Left Side Well (7)": MMRLocationData(
        region="Beneath the Well",
        address=0x0F4BB8,
        can_create=lambda options: options.realfairysanity.value
    ),                                             
    # The Moon Gossip Fairies
    
    # Goron Trial Gossip Fairies
    "Goron Trial 1st Gazebo Gossip (0)": MMRLocationData(
        region="The Moon",
        address=0x1F3F0F,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Goron Trial 1st Gazebo Gossip (1)": MMRLocationData(
        region="The Moon",
        address=0x1F3F0E,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Goron Trial 2nd Gazebo Gossip (0)": MMRLocationData(
        region="The Moon",
        address=0x1F3F0C,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Goron Trial 2nd Gazebo Gossip (1)": MMRLocationData(
        region="The Moon",
        address=0x1F3F0D,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Goron Trial Near Heart Piece Gossip (1)": MMRLocationData(
        region="The Moon",
        address=0x1F3F0B,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Zora Trial Gossip Fairies
    "Zora Trial RRR Path Gossip": MMRLocationData(
        region="The Moon",
        address=0x1F4714,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Zora Trial RRL Path Gossip": MMRLocationData(
        region="The Moon",
        address=0x1F4713,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Zora Trial LRR Path Gossip": MMRLocationData(
        region="The Moon",
        address=0x1F4712,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Zora Trial LRLL Path Gossip": MMRLocationData(
        region="The Moon",
        address=0x1F4711,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Zora Trial LLL Path Gossip": MMRLocationData(
        region="The Moon",
        address=0x1F4710,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Deku Trial Gossip Fairies
    "Deku Trial Front Left Gossip": MMRLocationData(
        region="The Moon",
        address=0x1F2A06,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Deku Trial Back Left Gossip": MMRLocationData(
        region="The Moon",
        address=0x1F2A08,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Deku Trial Front Right Gossip": MMRLocationData(
        region="The Moon",
        address=0x1F2A07,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Deku Trial Back Right Gossip": MMRLocationData(
        region="The Moon",
        address=0x1F2A09,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Deku Trial Furthest Back Gossip": MMRLocationData(
        region="The Moon",
        address=0x1F2A0A,
        can_create=lambda options: options.realfairysanity.value
    ),
    
    # Link Trial Gossip Fairies
    "Link Trial Gossip (1)": MMRLocationData(
        region="The Moon",
        address=0x1F6601,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Link Trial Gossip (2)": MMRLocationData(
        region="The Moon",
        address=0x1F6602,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Link Trial Gossip (3)": MMRLocationData(
        region="The Moon",
        address=0x1F6603,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Link Trial Gossip (4)": MMRLocationData(
        region="The Moon",
        address=0x1F6604,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Link Trial Gossip (5)": MMRLocationData(
        region="The Moon",
        address=0x1F6605,
        can_create=lambda options: options.realfairysanity.value
    ),
    #Butterfly Fairies
    #Termina Field Butterflies
    "Termina Field Near Peehat Grotto Butterfly Fairy (1)": MMRLocationData(
        region="Termina Field",
        address=0xBF2D00,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Termina Field Near Peehat Grotto Butterfly Fairy (2)": MMRLocationData(
        region="Termina Field",
        address=0xBF2D01,
        can_create=lambda options: options.realfairysanity.value
    ),    
    "Termina Field Cow Grotto Butterfly Fairy (1)": MMRLocationData(
        region="Termina Field",
        address=0xBFADA0,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Termina Field Cow Grotto Butterfly Fairy (2)": MMRLocationData(
        region="Termina Field",
        address=0xBFADA1,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Termina Field Cow Grotto Butterfly Fairy (3)": MMRLocationData(
        region="Termina Field",
        address=0xBFADA2,
        can_create=lambda options: options.realfairysanity.value
    ),                
    "Termina Field Bombable Rock Grotto Butterfly Fairy (1)": MMRLocationData(
        region="Termina Field",
        address=0xBF8000,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Termina Field Bombable Rock Grotto Butterfly Fairy (2)": MMRLocationData(
        region="Termina Field",
        address=0xBF8001,
        can_create=lambda options: options.realfairysanity.value
    ),    
    #Deku Palace Butterfly
    "Deku Palace Bean Seller Butterfly Fairy (1)": MMRLocationData(
        region="Deku Palace",
        address=0xBF8CC0,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Deku Palace Bean Seller Butterfly Fairy (2)": MMRLocationData(
        region="Deku Palace",
        address=0xBF8CC1,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Deku Palace Bean Seller Butterfly Fairy (3)": MMRLocationData(
        region="Deku Palace",
        address=0xBF8CC2,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Deku Palace Bean Seller Butterfly Fairy (4)": MMRLocationData(
        region="Deku Palace",
        address=0xBF8CC3,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Mountain Village Spring Day Butterfly (1)": MMRLocationData(
        region="Mountain Village",
        address=0xBF5A00,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Mountain Village Spring Day Butterfly (2)": MMRLocationData(
        region="Mountain Village",
        address=0xBF5A01,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Mountain Village Spring Day Butterfly (3)": MMRLocationData(
        region="Mountain Village",
        address=0xBF5A02,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Mountain Village Spring Day Butterfly (4)": MMRLocationData(
        region="Mountain Village",
        address=0xBF5A03,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Mountain Village Spring Day Butterfly (5)": MMRLocationData(
        region="Mountain Village",
        address=0xBF5A04,
        can_create=lambda options: options.realfairysanity.value
    ),                
    "Mountain Village Spring Day Butterfly (6)": MMRLocationData(
        region="Mountain Village",
        address=0xBF5A05,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Mountain Village Spring Day Butterfly (7)": MMRLocationData(
        region="Mountain Village",
        address=0xBF5A06,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Mountain Village Spring Day Butterfly (8)": MMRLocationData(
        region="Mountain Village",
        address=0xBF5A07,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Mountain Village Spring Day Butterfly (9)": MMRLocationData(
        region="Mountain Village",
        address=0xBF5A08,
        can_create=lambda options: options.realfairysanity.value
    ),                       
    #Great Bay Coast Butterflies
    "Great Bay Coast Outside Fisherman Hut Butterfly Fairy (1)": MMRLocationData(
        region="Great Bay",
        address=0xBF3700,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Great Bay Coast Outside Fisherman Hut Butterfly Fairy (2)": MMRLocationData(
        region="Great Bay",
        address=0xBF3701,
        can_create=lambda options: options.realfairysanity.value
    ),    
    "Great Bay Coast Cow Grotto Butterfly Fairy (1)": MMRLocationData(
        region="Great Bay",
        address=0xBFB7A0,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Great Bay Coast Cow Grotto Butterfly Fairy (2)": MMRLocationData(
        region="Great Bay",
        address=0xBFB7A1,
        can_create=lambda options: options.realfairysanity.value
    ), 
    "Great Bay Coast Cow Grotto Butterfly Fairy (3)": MMRLocationData(
        region="Great Bay",
        address=0xBFB7A2,
        can_create=lambda options: options.realfairysanity.value
    ),           
    #Moon Butterflies
    "Moon Butterfly Fairy (1)": MMRLocationData(
        region="The Moon",
        address=0xBF6700,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (2)": MMRLocationData(
        region="The Moon",
        address=0xBF6701,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (3)": MMRLocationData(
        region="The Moon",
        address=0xBF6702,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (4)": MMRLocationData(
        region="The Moon",
        address=0xBF6703,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (5)": MMRLocationData(
        region="The Moon",
        address=0xBF6704,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (6)": MMRLocationData(
        region="The Moon",
        address=0xBF6705,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (7)": MMRLocationData(
        region="The Moon",
        address=0xBF6706,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (8)": MMRLocationData(
        region="The Moon",
        address=0xBF6707,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (9)": MMRLocationData(
        region="The Moon",
        address=0xBF6708,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (10)": MMRLocationData(
        region="The Moon",
        address=0xBF6709,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (11)": MMRLocationData(
        region="The Moon",
        address=0xBF670A,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (12)": MMRLocationData(
        region="The Moon",
        address=0xBF670B,
        can_create=lambda options: options.realfairysanity.value
    ),
    "Moon Butterfly Fairy (13)": MMRLocationData(
        region="The Moon",
        address=0xBF670C,
        can_create=lambda options: options.realfairysanity.value
    ),
    # Notebook Entries                
    "Notebook Meeting Bombers": MMRLocationData(
        region="Clock Town",
        address=0x0B0000,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Anju": MMRLocationData(
        region="Clock Town",
        address=0x0B0001,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Kafei": MMRLocationData(
        region="Clock Town",
        address=0x0B0002,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Curiosity Shop Man": MMRLocationData(
        region="Clock Town",
        address=0x0B0003,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Bomb Shop Lady": MMRLocationData(
        region="Clock Town",
        address=0x0B0004,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Romani": MMRLocationData(
        region="Romani Ranch",
        address=0x0B0005,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Cremia": MMRLocationData(
        region="Romani Ranch",
        address=0x0B0006,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Mayor Dotour": MMRLocationData(
        region="Clock Town",
        address=0x0B0007,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Madame Aroma": MMRLocationData(
        region="Clock Town",
        address=0x0B0008,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Africa (Toto)": MMRLocationData(
        region="Clock Town",
        address=0x0B0009,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Gorman": MMRLocationData(
        region="Clock Town",
        address=0x0B000A,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Postman": MMRLocationData(
        region="Clock Town",
        address=0x0B000B,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Rosa Sisters": MMRLocationData(
        region="Clock Town",
        address=0x0B000C,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Toilet Hand": MMRLocationData(
        region="Clock Town",
        address=0x0B000D,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Anju's Grandmother": MMRLocationData(
        region="Clock Town",
        address=0x0B000E,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Kamaro": MMRLocationData(
        region="Termina Field",
        address=0x0B000F,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Grog": MMRLocationData(
        region="Romani Ranch",
        address=0x0B0010,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Gorman Brothers": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x0B0011,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Shiro": MMRLocationData(
        region="Road to Ikana",
        address=0x0B0012,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Meeting Guru Guru": MMRLocationData(
        region="Clock Town",
        address=0x0B0013,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Room Key": MMRLocationData(
        region="Clock Town",
        address=0x0B0014,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Promised Midnight Meeting": MMRLocationData(
        region="Clock Town",
        address=0x0B0015,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Promised To Meet Kafei": MMRLocationData(
        region="Clock Town",
        address=0x0B0016,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Letter To Kafei": MMRLocationData(
        region="Clock Town",
        address=0x0B0017,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Deposited Letter To Kafei": MMRLocationData(
        region="Clock Town",
        address=0x0B0018,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Pendant of Memories": MMRLocationData(
        region="Clock Town",
        address=0x0B0019,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Delivered Pendant of Memories": MMRLocationData(
        region="Clock Town",
        address=0x0B001A,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Escaped Sakons Hideout": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x0B001B,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Promised To Help With Aliens": MMRLocationData(
        region="Romani Ranch",
        address=0x0B001C,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Defended Against Aliens": MMRLocationData(
        region="Romani Ranch",
        address=0x0B001D,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Milk Bottle": MMRLocationData(
        region="Romani Ranch",
        address=0x0B001E,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Escorted Cremia": MMRLocationData(
        region="Romani Ranch",
        address=0x0B001F,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Romanis Mask": MMRLocationData(
        region="Romani Ranch",
        address=0x0B0020,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Keaton Mask": MMRLocationData(
        region="Clock Town",
        address=0x0B0021,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Priority Mail": MMRLocationData(
        region="Clock Town",
        address=0x0B0022,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Delivered Priority Mail": MMRLocationData(
        region="Clock Town",
        address=0x0B0023,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Learned Secret Code": MMRLocationData(
        region="Clock Town",
        address=0x0B0024,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Bombers NotebooK": MMRLocationData(
        region="Clock Town",
        address=0x0B0025,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Mayor HP": MMRLocationData(
        region="Clock Town",
        address=0x0B0026,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Rosa Sisters HP": MMRLocationData(
        region="Clock Town",
        address=0x0B0027,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Toilet Hand HP": MMRLocationData(
        region="Clock Town",
        address=0x0B0028,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Grandma Short Story HP": MMRLocationData(
        region="Clock Town",
        address=0x0B0029,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Grandma Long Story HP": MMRLocationData(
        region="Clock Town",
        address=0x0B002A,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Postman HP": MMRLocationData(
        region="Clock Town",
        address=0x0B002B,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Kafeis Mask": MMRLocationData(
        region="Clock Town",
        address=0x0B002C,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received All Night Mask": MMRLocationData(
        region="Clock Town",
        address=0x0B002D,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Bunny Hood": MMRLocationData(
        region="Romani Ranch",
        address=0x0B002E,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Garos Mask": MMRLocationData(
        region="Gorman Brothers Track",
        address=0x0B002F,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Circus Leaders Mask": MMRLocationData(
        region="Clock Town",
        address=0x0B0030,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Postmans Hat": MMRLocationData(
        region="Clock Town",
        address=0x0B0031,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Couples Mask": MMRLocationData(
        region="Clock Town",
        address=0x0B0032,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Blast Mask": MMRLocationData(
        region="Clock Town",
        address=0x0B0033,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Kamaros Mask": MMRLocationData(
        region="Termina Field",
        address=0x0B0034,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Stone Mask": MMRLocationData(
        region="Road to Ikana",
        address=0x0B0035,
        can_create=lambda options: options.notebooksanity.value
    ),
    "Notebook Event Received Bremen Mask": MMRLocationData(
        region="Clock Town",
        address=0x0B0036,
        can_create=lambda options: options.notebooksanity.value
    ),

    #Owlsanity 
    "Clock Town Owl Statue": MMRLocationData(
        region="Clock Town",
        address=0xFF1504,
        can_create=lambda options: options.owlsanity.value
    ),
    "Milk Road Owl Statue": MMRLocationData(
        region="Milk Road",
        address=0xFF1505,
        can_create=lambda options: options.owlsanity.value
    ),
    "Southern Swamp Owl Statue": MMRLocationData(
        region="Southern Swamp",
        address=0xFF1507,
        can_create=lambda options: options.owlsanity.value
    ),
    "Woodfall Owl Statue": MMRLocationData(
        region="Woodfall",
        address=0xFF1506,
        can_create=lambda options: options.owlsanity.value
    ),
    "Mountain Village Owl Statue": MMRLocationData(
        region="Mountain Village",
        address=0xFF1503,
        can_create=lambda options: options.owlsanity.value
    ),
    "Snowhead Owl Statue": MMRLocationData(
        region="Snowhead",
        address=0xFF1502,
        can_create=lambda options: options.owlsanity.value
    ),
    "Great Bay Coast Owl Statue": MMRLocationData(
        region="Great Bay",
        address=0xFF1500,
        can_create=lambda options: options.owlsanity.value
    ),
    "Zora Cape Owl Statue": MMRLocationData(
        region="Zora Cape",
        address=0xFF1501,
        can_create=lambda options: options.owlsanity.value
    ),
    "Ikana Canyon Owl Statue": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0xFF1508,
        can_create=lambda options: options.owlsanity.value
    ),
    "Stone Tower Owl Statue": MMRLocationData(
        region="Stone Tower",
        address=0xFF1509,
        can_create=lambda options: options.owlsanity.value
    ),

    #Frogs
    "Laundry Pool Frog": MMRLocationData(
        region="Clock Town",
        address=0xFF0004,
        can_create=lambda options: options.frogsanity.value
    ), 
    "Southern Swamp Frog": MMRLocationData(
        region="Southern Swamp",
        address=0xFF0003,
        can_create=lambda options: options.frogsanity.value
    ),
    "Woodfall Temple Miniboss Frog": MMRLocationData(
        region="Woodfall Temple",
        address=0xFF0001,
        can_create=lambda options: options.frogsanity.value
    ),    
    "Great Bay Temple Miniboss Frog": MMRLocationData(
        region="Great Bay Temple",
        address=0xFF0002,
        can_create=lambda options: options.frogsanity.value
    ),  
    # Treesanity Locations
    "North Clock Town Tree (1)": MMRLocationData(
        region="Clock Town",
        address=0x2D6E00,
        can_create=lambda options: options.treesanity.value
    ),
    "North Clock Town Tree (2)": MMRLocationData(
        region="Clock Town",
        address=0x2D6E01,
        can_create=lambda options: options.treesanity.value
    ),
    "West Clock Town Trading Post Bush (1)": MMRLocationData(
        region="Clock Town",
        address=0x2C343F,
        can_create=lambda options: options.treesanity.value
    ),
    "West Clock Town Trading Post Bush (2)": MMRLocationData(
        region="Clock Town",
        address=0x2C348C,
        can_create=lambda options: options.treesanity.value
    ),
    "West Clock Town Trading Post Bush (3)": MMRLocationData(
        region="Clock Town",
        address=0x2C34CA,
        can_create=lambda options: options.treesanity.value
    ),
    "West Clock Town Trading Post Bush (4)": MMRLocationData(
        region="Clock Town",
        address=0x2C34E8,
        can_create=lambda options: options.treesanity.value
    ),
    "Termina Field Tree Near Observatory (1)": MMRLocationData(
        region="Termina Field",
        address=0x2C2DED,
        can_create=lambda options: options.treesanity.value
    ),
    "Termina Field Tree Near Observatory (2)": MMRLocationData(
        region="Termina Field",
        address=0x2C2DAA,
        can_create=lambda options: options.treesanity.value
    ),            
    "Termina Field Tree Near Observatory (3)": MMRLocationData(
        region="Termina Field",
        address=0x2C2DB3,
        can_create=lambda options: options.treesanity.value
    ),
    "Road to Southern Swamp Trees (1)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x2D4000,
        can_create=lambda options: options.treesanity.value
    ),
    "Road to Southern Swamp Trees (2)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x2D4001,
        can_create=lambda options: options.treesanity.value
    ),
    "Road to Southern Swamp Trees (3)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x2D4002,
        can_create=lambda options: options.treesanity.value
    ),
    "Road to Southern Swamp Trees (4)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x2D4003,
        can_create=lambda options: options.treesanity.value
    ),
    "Road to Southern Swamp Trees (5)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x2D4004,
        can_create=lambda options: options.treesanity.value
    ),
    "Road to Southern Swamp Trees (6)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x2D4005,
        can_create=lambda options: options.treesanity.value
    ),
    "Path To Mountains Tree (1)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x2B1C00,
        can_create=lambda options: options.treesanity.value
    ),
    "Path To Mountains Tree (2)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x2B1C01,
        can_create=lambda options: options.treesanity.value
    ),
    "Path To Mountains Tree (3)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x2B1C02,
        can_create=lambda options: options.treesanity.value
    ),
    "Path To Mountains Tree (4)": MMRLocationData(
        region="Path to Mountain Village",
        address=0x2B1C03,
        can_create=lambda options: options.treesanity.value
    ),
    "Twin Islands Tree (1)": MMRLocationData(
        region="Twin Islands",
        address=0x2B5D00,
        can_create=lambda options: options.treesanity.value
    ),
    "Twin Islands Tree (2)": MMRLocationData(
        region="Twin Islands",
        address=0x2B5D01,
        can_create=lambda options: options.treesanity.value
    ),
    "Twin Islands Tree (3)": MMRLocationData(
        region="Twin Islands",
        address=0x2B5D02,
        can_create=lambda options: options.treesanity.value
    ),
    "Path To Snowhead Tree Near Ledge": MMRLocationData(
        region="Path to Snowhead",
        address=0x2B5B03,
        can_create=lambda options: options.treesanity.value
    ),
    "Path To Snowhead Tree (1)": MMRLocationData(
        region="Path to Snowhead",
        address=0x2B5B00,
        can_create=lambda options: options.treesanity.value
    ),
    "Path To Snowhead Tree (2)": MMRLocationData(
        region="Path to Snowhead",
        address=0x2B5B01,
        can_create=lambda options: options.treesanity.value
    ),
    "Path To Snowhead Tree (3)": MMRLocationData(
        region="Path to Snowhead",
        address=0x2B5B02,
        can_create=lambda options: options.treesanity.value
    ),
    "Goron Racetrack Trees (1)": MMRLocationData(
        region="Goron Racetrack",
        address=0x2D6B00,
        can_create=lambda options: options.treesanity.value
    ),
    "Goron Racetrack Trees (2)": MMRLocationData(
        region="Goron Racetrack",
        address=0x2D6B01,
        can_create=lambda options: options.treesanity.value
    ),
    "Goron Racetrack Trees (3)": MMRLocationData(
        region="Goron Racetrack",
        address=0x2D6B02,
        can_create=lambda options: options.treesanity.value
    ),
    "Goron Racetrack Trees (4)": MMRLocationData(
        region="Goron Racetrack",
        address=0x2D6B03,
        can_create=lambda options: options.treesanity.value
    ),
    "Goron Racetrack Trees (5)": MMRLocationData(
        region="Goron Racetrack",
        address=0x2D6B04,
        can_create=lambda options: options.treesanity.value
    ),
    "Goron Racetrack Trees (6)": MMRLocationData(
        region="Goron Racetrack",
        address=0x2D6B05,
        can_create=lambda options: options.treesanity.value
    ),
    "Goron Racetrack Trees (7)": MMRLocationData(
        region="Goron Racetrack",
        address=0x2D6B06,
        can_create=lambda options: options.treesanity.value
    ),
    "Goron Racetrack Trees (8)": MMRLocationData(
        region="Goron Racetrack",
        address=0x2D6B07,
        can_create=lambda options: options.treesanity.value
    ),
    "Goron Racetrack Trees (9)": MMRLocationData(
        region="Goron Racetrack",
        address=0x2D6B08,
        can_create=lambda options: options.treesanity.value
    ),
    "Goron Racetrack Trees (10)": MMRLocationData(
        region="Goron Racetrack",
        address=0x2D6B09,
        can_create=lambda options: options.treesanity.value
    ),
    "Twin Islands (Spring) Tree (1)": MMRLocationData(
        region="Twin Islands",
        address=0x2C5E94,
        can_create=lambda options: options.treesanity.value
    ),
    "Twin Islands (Spring) Tree (2)": MMRLocationData(
        region="Twin Islands",
        address=0x2C5E66,
        can_create=lambda options: options.treesanity.value
    ),
    "Twin Islands (Spring) Tree (3)": MMRLocationData(
        region="Twin Islands",
        address=0x2C5E0B,
        can_create=lambda options: options.treesanity.value
    ),
    "Romani Ranch Bush (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C35B9,
        can_create=lambda options: options.treesanity.value
    ),  
    "Romani Ranch Bush (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C3596,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Bush (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C3520,
        can_create=lambda options: options.treesanity.value
    ),
    "Romani Ranch Bush (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C3540,
        can_create=lambda options: options.treesanity.value
    ),
    "Romani Ranch Tree (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C356E,
        can_create=lambda options: options.treesanity.value
    ),   
    "Romani Ranch Tree (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C3554,
        can_create=lambda options: options.treesanity.value
    ),   
    "Romani Ranch Tree (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C356A,
        can_create=lambda options: options.treesanity.value
    ),    
    "Romani Ranch Tree (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C35E8,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Tree (5)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C3579,
        can_create=lambda options: options.treesanity.value
    ),
    "Romani Ranch Tree (6)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C353A,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Tree (7)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C351E,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Baby Cucoo Tree": MMRLocationData(
        region="Romani Ranch",
        address=0x2C426D,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Baby Cucoo Bush (1)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C42C2,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Baby Cucoo Bush (2)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C426A,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Baby Cucoo Bush (3)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C42DF,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Baby Cucoo Bush (4)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C42C1,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Baby Cucoo Bush (5)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C424A,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Baby Cucoo Bush (6)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C42AA,
        can_create=lambda options: options.treesanity.value
    ), 
    "Romani Ranch Baby Cucoo Bush (7)": MMRLocationData(
        region="Romani Ranch",
        address=0x2C427D,
        can_create=lambda options: options.treesanity.value
    ), 
    "Gorman Racetrack Tree Group 1 (1)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A01,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (2)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A06,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (3)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A14,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (4)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A41,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (5)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A77,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (6)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A99,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (7)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6AC7,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (8)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6ACF,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (9)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6AD3,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (10)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6AD7,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (11)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6AEF,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 1 (12)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A02,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (1)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A04,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (2)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A0A,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (3)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A13,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (4)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A1D,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (5)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A44,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (6)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A47,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (7)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A51,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (8)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A8F,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (9)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6AA8,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (10)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6ADA,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (11)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6ADC,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (12)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6AF2,
        can_create=lambda options: options.treesanity.value
    ),
    "Gorman Racetrack Tree Group 2 (13)": MMRLocationData(
        region="Inside Gorman Brothers Track",
        address=0x2C6A20,
        can_create=lambda options: options.treesanity.value
    ),
    "Great Bay Coast Nut Tree (1)": MMRLocationData(
        region="Great Bay",
        address=0x2A3701,
        can_create=lambda options: options.treesanity.value
    ),    
    "Great Bay Coast Nut Tree (2)": MMRLocationData(
        region="Great Bay",
        address=0x2A3702,
        can_create=lambda options: options.treesanity.value
    ),    
    "Great Bay Coast Nut Tree (3)": MMRLocationData(
        region="Great Bay",
        address=0x2A3703,
        can_create=lambda options: options.treesanity.value
    ),    
    "Great Bay Coast Fisherman Island Nut Tree (1)": MMRLocationData(
        region="Great Bay",
        address=0x2A3700,
        can_create=lambda options: options.treesanity.value
    ),   
    "Zora Cape Nut Tree Near Jars (1)": MMRLocationData(
        region="Zora Cape",
        address=0x2A3800,
        can_create=lambda options: options.treesanity.value
    ),   
    "Zora Cape Nut Tree Near Jars (2)": MMRLocationData(
        region="Zora Cape",
        address=0x2A3801,
        can_create=lambda options: options.treesanity.value
    ),   
    "Zora Cape Nut Tree On Islands (1)": MMRLocationData(
        region="Zora Cape",
        address=0x2A3802,
        can_create=lambda options: options.treesanity.value
    ),  
    "Zora Cape Nut Tree On Islands (2)": MMRLocationData(
        region="Zora Cape",
        address=0x2A3803,
        can_create=lambda options: options.treesanity.value
    ),  
    "Zora Cape Nut Tree On Islands (3)": MMRLocationData(
        region="Zora Cape",
        address=0x2A3804,
        can_create=lambda options: options.treesanity.value
    ),   
    "Zora Cape Nut Tree On Turtle Island (1)": MMRLocationData(
        region="Zora Cape",
        address=0x2A3805,
        can_create=lambda options: options.treesanity.value
    ),   
    "Zora Cape Nut Tree On Turtle Island (2)": MMRLocationData(
        region="Zora Cape",
        address=0x2A3806,
        can_create=lambda options: options.treesanity.value
    ), 
    "Beneath The Well Tree Near Cow": MMRLocationData(
        region="Beneath the Well",
        address=0x2C4BCB,
        can_create=lambda options: options.treesanity.value
    ), 
    "Beneath The Well Bush Near Cow (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x2C4B25,
        can_create=lambda options: options.treesanity.value
    ),
    "Beneath The Well Bush Near Cow (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x2C4B93,
        can_create=lambda options: options.treesanity.value
    ),

    # Flowersanity
   
    "Before Clock Town Flower (1)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A10,
        can_create=lambda options: options.flowersanity.value
    ),
    "Before Clock Town Flower (2)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A11,
        can_create=lambda options: options.flowersanity.value
    ),
    "Before Clock Town Flower (3)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A12,
        can_create=lambda options: options.flowersanity.value
    ),
    "Before Clock Town Flower (4)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A13,
        can_create=lambda options: options.flowersanity.value
    ),
    "Before Clock Town Flower (5)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A14,
        can_create=lambda options: options.flowersanity.value
    ),
    "Before Clock Town Flower (6)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A15,
        can_create=lambda options: options.flowersanity.value
    ),
    "Before Clock Town Flower (7)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A00,
        can_create=lambda options: options.flowersanity.value
    ),
    "Before Clock Town Flower (8)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A01,
        can_create=lambda options: options.flowersanity.value
    ),
    "Before Clock Town Flower (9)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A02,
        can_create=lambda options: options.flowersanity.value
    ),
    "Before Clock Town Flower (10)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A03,
        can_create=lambda options: options.flowersanity.value
    ),
    "Before Clock Town Flower (11)": MMRLocationData(
        region="Clock Town",
        address=0x3F1A04,
        can_create=lambda options: options.flowersanity.value
    ),
    "South Clock Town Business Scrub Flower": MMRLocationData(
        region="Clock Town",
        address=0x3F6F00,
        can_create=lambda options: options.flowersanity.value
    ),
    "East Clock Town Flower": MMRLocationData(
        region="Clock Town",
        address=0x3F6C00,
        can_create=lambda options: options.flowersanity.value
    ),
    "North Clock Town Flower (1)": MMRLocationData(
        region="Clock Town",
        address=0x3F6E00,
        can_create=lambda options: options.flowersanity.value
    ),
    "North Clock Town Flower (2)": MMRLocationData(
        region="Clock Town",
        address=0x3F6E01,
        can_create=lambda options: options.flowersanity.value
    ),
    "North Clock Town Deku Playground Flower (1)": MMRLocationData(
        region="Clock Town",
        address=0x3F1E00,
        can_create=lambda options: options.flowersanity.value
    ),
    "North Clock Town Deku Playground Flower (2)": MMRLocationData(
        region="Clock Town",
        address=0x3F1E01,
        can_create=lambda options: options.flowersanity.value
    ),
    "North Clock Town Deku Playground Flower (3)": MMRLocationData(
        region="Clock Town",
        address=0x3F1E02,
        can_create=lambda options: options.flowersanity.value
    ),
    "North Clock Town Deku Playground Flower (4)": MMRLocationData(
        region="Clock Town",
        address=0x3F1E03,
        can_create=lambda options: options.flowersanity.value
    ),
    "North Clock Town Deku Playground Flower (5)": MMRLocationData(
        region="Clock Town",
        address=0x3F1E04,
        can_create=lambda options: options.flowersanity.value
    ),
    "North Clock Town Deku Playground Flower (6)": MMRLocationData(
        region="Clock Town",
        address=0x3F1E05,
        can_create=lambda options: options.flowersanity.value
    ),
    "North Clock Town Deku Playground Flower (7)": MMRLocationData(
        region="Clock Town",
        address=0x3F1E06,
        can_create=lambda options: options.flowersanity.value
    ),
    "North Clock Town Deku Playground Flower (8)": MMRLocationData(
        region="Clock Town",
        address=0x3F1E07,
        can_create=lambda options: options.flowersanity.value
    ),
    "Termina Field Flower Near Observatory": MMRLocationData(
        region="Termina Field",
        address=0x3F2D03,
        can_create=lambda options: options.flowersanity.value
    ),
    "Termina Field Flower Near Skullkid Drawing": MMRLocationData(
        region="Termina Field",
        address=0x3F2D02,
        can_create=lambda options: options.flowersanity.value
    ),
    "Termina Field Flower Near Stump": MMRLocationData(
        region="Termina Field",
        address=0x3F2D01,
        can_create=lambda options: options.flowersanity.value
    ),
    "Termina Field Flower Near Giant Log": MMRLocationData(
        region="Termina Field",
        address=0x3F2D00,
        can_create=lambda options: options.flowersanity.value
    ),
    # Road To Swamp Flowers
    "Road to Southern Swamp Flowers (1)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x3F4000,
        can_create=lambda options: options.flowersanity.value
    ),
    "Road to Southern Swamp Flowers (2)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x3F4001,
        can_create=lambda options: options.flowersanity.value
    ),
    "Road to Southern Swamp Flowers (3)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x3F4002,
        can_create=lambda options: options.flowersanity.value
    ),
    "Road to Southern Swamp Flowers (4)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x3F4003,
        can_create=lambda options: options.flowersanity.value
    ),
    "Road to Southern Swamp Flowers (5)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x3F4004,
        can_create=lambda options: options.flowersanity.value
    ),
    "Road to Southern Swamp Flowers (6)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x3F4005,
        can_create=lambda options: options.flowersanity.value
    ),
    "Road to Southern Swamp Flowers (7)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x3F4006,
        can_create=lambda options: options.flowersanity.value
    ),
    "Road to Southern Swamp Flowers (8)": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x3F4007,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Business Scrub Flower": MMRLocationData(
        region="Southern Swamp",
        address=0x3F4500,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Post Dungeon Business Scrub Flower": MMRLocationData(
        region="Southern Swamp",
        address=0x3F0000,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Woods of Mystery Flower Day 1/3 (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F6430,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Woods of Mystery Flower Day 1/3 (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F6431,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Woods of Mystery Flower Any Day (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F6440,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Woods of Mystery Flower Any Day (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F6450,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Woods of Mystery Flower Any Day (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F6451,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Woods of Mystery Flower Day 2 (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F6470,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Woods of Mystery Flower Day 2 (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F6471,
        can_create=lambda options: options.flowersanity.value
    ),
    #Deku Palace Flowers
    "Deku Palace Flower (1)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B10,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace Flower (2)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B11,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace Flower (3)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B12,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace Flower (4)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B13,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace Flower (5)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B14,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace Flower (6)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B15,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace Flower (7)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B20,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace Flower (8)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B21,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace Flower (9)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B22,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace Flower (10)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B23,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace Flower (11)": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B24,
        can_create=lambda options: options.flowersanity.value
    ),
    "Deku Palace To Swamp Flower": MMRLocationData(
        region="Deku Palace",
        address=0x3F2B00,
        can_create=lambda options: options.flowersanity.value
    ),
    # Swamp Spider Flowers
    "Swamp Spiderhouse Main Room Flowers (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x3F2710,
        can_create=lambda options: options.flowersanity.value
    ),
    "Swamp Spiderhouse Main Room Flowers (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x3F2711,
        can_create=lambda options: options.flowersanity.value
    ),
    "Swamp Spiderhouse Main Room Flower (3)": MMRLocationData(
        region="Swamp Spider House",
        address=0x3F2712,
        can_create=lambda options: options.flowersanity.value
    ),
    "Swamp Spiderhouse Giant Pot Room Flower (1)": MMRLocationData(
        region="Swamp Spider House",
        address=0x3F2740,
        can_create=lambda options: options.flowersanity.value
    ),
    # Requires Sonata 
    "Swamp Spiderhouse Giant Pot Room Flower (2)": MMRLocationData(
        region="Swamp Spider House",
        address=0x3F2741,
        can_create=lambda options: options.flowersanity.value
    ),
    #Heading to Woodfall
    "Southern Swamp Path To Woodfall Flower (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F4510,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Path To Woodfall Flower (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F4511,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Path To Woodfall Flower (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F4512,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Path To Woodfall Flower (4)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F4513,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Path To Woodfall Flower (5)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F4514,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Path To Woodfall Flower (6)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F4515,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Flower (1)": MMRLocationData(
        region="Woodfall",
        address=0x3F4600,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Flower (2)": MMRLocationData(
        region="Woodfall",
        address=0x3F4601,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Flower (3)": MMRLocationData(
        region="Woodfall",
        address=0x3F4602,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Flower (4)": MMRLocationData(
        region="Woodfall",
        address=0x3F4603,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Flower (5)": MMRLocationData(
        region="Woodfall",
        address=0x3F4604,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Flower (6)": MMRLocationData(
        region="Woodfall",
        address=0x3F4605,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Flower (7)": MMRLocationData(
        region="Woodfall",
        address=0x3F4606,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Flower (8)": MMRLocationData(
        region="Woodfall",
        address=0x3F4607,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Flower (9)": MMRLocationData(
        region="Woodfall",
        address=0x3F4608,
        can_create=lambda options: options.flowersanity.value
    ),
    # Woodfall Temple Flowers
    "Woodfall Temple Entrance Flower (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B20,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Entrance Flower (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B21,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Entrance Flower (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B22,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Entrance Flower (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B23,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Main Room Flower": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B10,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Elevator Room Flower": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B50,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Elevator Room Upper Flower (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B51,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Elevator Room Upper Flower (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B52,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Snapping Turtle Flower (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B60,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Snapping Turtle Flower (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B61,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Snapping Turtle Flower (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B62,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Gekko Flower (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B80,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Gekko Flower (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B81,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Gekko Flower (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B82,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Gekko Flower (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B83,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Gekko Flower (5)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B84,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Dinolfos Flower (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B70,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Dinolfos Flower (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B71,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple 2F Moving Platform Flower (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1BA0,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple 2F Moving Platform Flower (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1BA1,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple 2F Moving Platform Flower (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1BA2,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple 2F Moving Platform Flower (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1BA3,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Dragonfly Room Flower (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B40,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Dragonfly Room Flower (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B41,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Dragonfly Room Flower (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B42,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Pre Boss Room Flower (1)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B00,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Pre Boss Room Flower (2)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B01,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Pre Boss Room Flower (3)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B02,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Pre Boss Room Flower (4)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B03,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Pre Boss Room Flower (5)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B04,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Pre Boss Room Flower (6)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B05,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Pre Boss Room Flower (7)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B06,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Pre Boss Room Flower (8)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B07,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Pre Boss Room Flower (9)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B08,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Pre Boss Room Flower (10)": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1B09,
        can_create=lambda options: options.flowersanity.value
    ),
    "Woodfall Temple Odolwa Golden Flower": MMRLocationData(
        region="Woodfall Temple",
        address=0x3F1F00,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Post Dungeon Flower (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F0010,
        can_create=lambda options: options.flowersanity.value
    ),
    "Southern Swamp Post Dungeon Flower (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x3F0011,
        can_create=lambda options: options.flowersanity.value
    ),
    "Goron Village Business Scrub Flower": MMRLocationData(
        region="Goron Village",
        address=0x3F4D00,
        can_create=lambda options: options.flowersanity.value
    ),
    "Snowhead Temple Frozen Green Door Flower (1)": MMRLocationData(
        region="Snowhead Temple",
        address=0x3F2150,
        can_create=lambda options: options.flowersanity.value
    ),
    "Snowhead Temple Frozen Green Door Flower (2)": MMRLocationData(
        region="Snowhead Temple",
        address=0x3F2151,
        can_create=lambda options: options.flowersanity.value
    ),
    "Snowhead Temple Main Room Wall Chest Flower": MMRLocationData(
        region="Snowhead Temple",
        address=0x3F2140,
        can_create=lambda options: options.flowersanity.value
    ),
    "Snowhead Temple Main Room Wall Chest Flower": MMRLocationData(
        region="Snowhead Temple",
        address=0x3F2140,
        can_create=lambda options: options.flowersanity.value
    ),
    "Snowhead Temple Flower Outside Goht": MMRLocationData(
        region="Snowhead Temple",
        address=0x3F2141,
        can_create=lambda options: options.flowersanity.value
    ),
    "Zora Cape Lower Wall Flower Near Beavers": MMRLocationData(
        region="Zora Cape",
        address=0x3F3800,
        can_create=lambda options: options.flowersanity.value
    ),
    "Zora Hall Business Scrub Flower": MMRLocationData(
        region="Zora Hall",
        address=0x3F4C20,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Canyon Business Scrub Flower (1)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x3F1340,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Canyon Business Scrub Flower (2)": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x3F1341,
        can_create=lambda options: options.flowersanity.value
    ),
    "Well Deku Flower (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x3F4B20,
        can_create=lambda options: options.flowersanity.value
    ),
    "Well Deku Flower (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x3F4B21,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Left Side Falling Ceiling Room Flower (1)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D20,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Left Side Falling Ceiling Room Flower (2)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D21,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Left Side Falling Ceiling Room Flower (3)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D22,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Left Side Falling Ceiling Room Flower (4)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D23,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Left Side Falling Ceiling Room Flower (5)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D24,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Left Side Broken Floor Room Flower (1)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D40,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Left Side Broken Floor Room Flower (2)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D41,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Left Side Broken Floor Room Flower (3)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D42,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Exterior Flower (1)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D00,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Exterior Flower (2)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D01,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Exterior Flower (3)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D02,
        can_create=lambda options: options.flowersanity.value
    ),
    "Ikana Castle Exterior Flower (4)": MMRLocationData(
        region="Ikana Castle",
        address=0x3F1D03,
        can_create=lambda options: options.flowersanity.value
    ),
    "Stone Tower Temple Deku Updraft Flower": MMRLocationData(
        region="Stone Tower Temple",
        address=0x3F1690,
        can_create=lambda options: options.flowersanity.value
    ),
    "Stone Tower Temple Inverted Eastern Air Gust Room Flower (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x3F1830,
        can_create=lambda options: options.flowersanity.value
    ),
    "Stone Tower Temple Inverted Eastern Air Gust Room Flower (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x3F1831,
        can_create=lambda options: options.flowersanity.value
    ),
    "Inverted Stone Tower Temple Small Poe Room Flower (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x3F1820,
        can_create=lambda options: options.flowersanity.value
    ),
    "Inverted Stone Tower Temple Small Poe Room Flower (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x3F1821,
        can_create=lambda options: options.flowersanity.value
    ),
    "Inverted Stone Tower Temple Lower Bridge Room Flower (1)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x3F1810,
        can_create=lambda options: options.flowersanity.value
    ),
    "Inverted Stone Tower Temple Lower Bridge Room Flower (2)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x3F1811,
        can_create=lambda options: options.flowersanity.value
    ),
    "Inverted Stone Tower Temple Lower Bridge Room Flower (3)": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
        address=0x3F1812,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (1)": MMRLocationData(
        region="The Moon",
        address=0x3F2A00,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (2)": MMRLocationData(
        region="The Moon",
        address=0x3F2A01,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (3)": MMRLocationData(
        region="The Moon",
        address=0x3F2A02,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (4)": MMRLocationData(
        region="The Moon",
        address=0x3F2A03,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (5)": MMRLocationData(
        region="The Moon",
        address=0x3F2A04,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (6)": MMRLocationData(
        region="The Moon",
        address=0x3F2A05,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (7)": MMRLocationData(
        region="The Moon",
        address=0x3F2A06,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (8)": MMRLocationData(
        region="The Moon",
        address=0x3F2A07,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (9)": MMRLocationData(
        region="The Moon",
        address=0x3F2A08,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (10)": MMRLocationData(
        region="The Moon",
        address=0x3F2A09,
        can_create=lambda options: options.flowersanity.value
    ),
    "The Moon Deku Trial Flower (11)": MMRLocationData(
        region="The Moon",
        address=0x3F2A0A,
        can_create=lambda options: options.flowersanity.value
    ),
    # "The Moon Majora Flower": MMRLocationData(
    #     region="The Moon",
    #     address=0x,
    #     can_create=lambda options: options.flowersanity.value
    # ),
    
    # Signs
    "North Clock Town Cut the Sign": MMRLocationData(
        region="Clock Town",
        address=0x310311,
        can_create=lambda options: options.signsanity.value
    ),
    "West Clock Town Sword School Night 3 Midnight Cut the Sign": MMRLocationData(
        region="Clock Town",
        address=0x310310,
        can_create=lambda options: options.signsanity.value
    ),
    "East Clock Town Milk Bar Roof Cut the Sign": MMRLocationData(
        region="Clock Town",
        address=0x31033F,
        can_create=lambda options: options.signsanity.value
    ),
    "Termina Field Takkuri Cut the Sign": MMRLocationData(
        region="Termina Field",
        address=0x31033C,
        can_create=lambda options: options.signsanity.value
    ),
    "Road to Southern Swamp Entry Cut the Sign": MMRLocationData(
        region="Road to Southern Swamp",
        address=0x31030A,
        can_create=lambda options: options.signsanity.value
    ),
    "Southern Swamp Tourist Centre Cut the Sign": MMRLocationData(
        region="Southern Swamp",
        address=0x310307,
        can_create=lambda options: options.signsanity.value
    ),
    "Southern Swamp Witch's Hut Cut the Sign": MMRLocationData(
        region="Southern Swamp",
        address=0x31030C,
        can_create=lambda options: options.signsanity.value
    ),
    "Southern Swamp Outside Woods of Mystery Cut the Sign": MMRLocationData(
        region="Southern Swamp",
        address=0x31030B,
        can_create=lambda options: options.signsanity.value
    ),
    "Woods of Mystery Cut the Sign Day 1 (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x31033D,
        can_create=lambda options: options.signsanity.value
    ),
    "Woods of Mystery Cut the Sign Day 1 (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x31133D,
        can_create=lambda options: options.signsanity.value
    ),
    "Woods of Mystery Cut the Sign Day 1 (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x31233D,
        can_create=lambda options: options.signsanity.value
    ),
    "Woods of Mystery Cut the Sign Day 2 (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x31333D,
        can_create=lambda options: options.signsanity.value
    ),
    "Woods of Mystery Cut the Sign Day 2 (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x31433D,
        can_create=lambda options: options.signsanity.value
    ),
    "Woods of Mystery Cut the Sign Day 2 (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x31533D,
        can_create=lambda options: options.signsanity.value
    ),
    "Woods of Mystery Cut the Sign Day 3 (1)": MMRLocationData(
        region="Southern Swamp",
        address=0x31633D,
        can_create=lambda options: options.signsanity.value
    ),
    "Woods of Mystery Cut the Sign Day 3 (2)": MMRLocationData(
        region="Southern Swamp",
        address=0x31733D,
        can_create=lambda options: options.signsanity.value
    ),
    "Woods of Mystery Cut the Sign Day 3 (3)": MMRLocationData(
        region="Southern Swamp",
        address=0x31833D,
        can_create=lambda options: options.signsanity.value
    ),
    "Southern Swamp Log Cut the Sign": MMRLocationData(
        region="Southern Swamp",
        address=0x310309,
        can_create=lambda options: options.signsanity.value
    ),
    "Southern Swamp Outside Spider House": MMRLocationData(
        region="Southern Swamp (Deku Palace)",
        address=0x31030E,
        can_create=lambda options: options.signsanity.value
    ),
    "Swamp Spider House Behind Statue Cut the Sign": MMRLocationData(
        region="Swamp Spider House",
        address=0x31130C,
        can_create=lambda options: options.signsanity.value
    ),
    "Deku Palace Behind Entrance Guards Cut Left Sign": MMRLocationData(
        region="Deku Palace",
        address=0x310338,
        can_create=lambda options: options.signsanity.value
    ),
    "Deku Palace Behind Entrance Guards Cut Right Sign": MMRLocationData(
        region="Deku Palace",
        address=0x311338,
        can_create=lambda options: options.signsanity.value
    ),
    "Deku Palace Behind Entrance Guards Cut Sign Near King's Chamber": MMRLocationData(
        region="Deku Palace",
        address=0x31033A,
        can_create=lambda options: options.signsanity.value
    ),
    "Deku Palace Bean Daddy Grotto Cut the Sign": MMRLocationData(
        region="Deku Palace",
        address=0x31230C,
        can_create=lambda options: options.signsanity.value
    ),
    "Path to Mountain Village Cut the Sign": MMRLocationData(
        region="Path to Mountain Village",
        address=0x310303,
        can_create=lambda options: options.signsanity.value
    ),
    "Mountain Village Owl Statue Cut the Sign": MMRLocationData(
        region="Mountain Village",
        address=0x311314,
        can_create=lambda options: options.signsanity.value
    ),
    "Mountain Village Owl Statue Spring Cut the Sign": MMRLocationData(
        region="Mountain Village",
        address=0x310317,
        can_create=lambda options: options.signsanity.value
    ),
    "Mountain Village Bridge Cut the Sign": MMRLocationData(
        region="Mountain Village",
        address=0x310314,
        can_create=lambda options: options.signsanity.value
    ),
    "Mountain Village Outside Smithy Cut the Sign": MMRLocationData(
        region="Mountain Village",
        address=0x310305,
        can_create=lambda options: options.signsanity.value
    ),
    "Mountain Village Pond Cut the Sign": MMRLocationData(
        region="Mountain Village",
        address=0x310315,
        can_create=lambda options: options.signsanity.value
    ),
    "Mountain Village Twin Island Entrance Cut the Sign": MMRLocationData(
        region="Mountain Village",
        address=0x310313,
        can_create=lambda options: options.signsanity.value
    ),
    "Twin Islands Outside Goron Racetrack Cut the Sign": MMRLocationData(
        region="Twin Islands",
        address=0x310319,
        can_create=lambda options: options.signsanity.value
    ),
    "Goron Village Outside Lens Cave Cut the Sign": MMRLocationData(
        region="Goron Village",
        address=0x31031C,
        can_create=lambda options: options.signsanity.value
    ),
    "Goron Village Outside Keg Goron Cut the Sign": MMRLocationData(
        region="Goron Village",
        address=0x31031D,
        can_create=lambda options: options.signsanity.value
    ),
    "Goron Village Outside Goron Shrine": MMRLocationData(
        region="Goron Village",
        address=0x31031B,
        can_create=lambda options: options.signsanity.value
    ),
    "Path to Snowhead Cut the Sign": MMRLocationData(
        region="Path to Snowhead",
        address=0x31031E,
        can_create=lambda options: options.signsanity.value
    ),
    "Path to Snowhead Upper Cut the Sign": MMRLocationData(
        region="Path to Snowhead",
        address=0x31031F,
        can_create=lambda options: options.signsanity.value
    ),
    "Outside Snowhead Temple Cut the Sign": MMRLocationData(
        region="Snowhead",
        address=0x310320,
        can_create=lambda options: options.signsanity.value
    ),
    "Mountain Village Spring Near Graveyard Pond Cut the Sign": MMRLocationData(
        region="Mountain Village",
        address=0x310318,
        can_create=lambda options: options.signsanity.value
    ),
    "Mountain Village Spring Path to Twin Islands Cut the Sign": MMRLocationData(
        region="Mountain Village",
        address=0x310316,
        can_create=lambda options: options.signsanity.value
    ),
    "Twin Islands Spring Outside Goron Racetrack Cut the Sign": MMRLocationData(
        region="Twin Islands",
        address=0x31031A,
        can_create=lambda options: options.signsanity.value
    ),
    "Romani Ranch Epona Stable Cut the Sign": MMRLocationData(
        region="Romani Ranch",
        address=0x31033E,
        can_create=lambda options: options.signsanity.value
    ),
    "Romani Ranch Doggy Racetrack Cut the Sign": MMRLocationData(
        region="Romani Ranch",
        address=0x310302,
        can_create=lambda options: options.signsanity.value
    ),
    "Gorman Racetrack Fence Day 3 Cut the Sign": MMRLocationData(
        region="Gorman Brothers Track",
        address=0x310341,
        can_create=lambda options: options.signsanity.value
    ),
    "Great Bay Coast Entrance Cut the Sign": MMRLocationData(
        region="Great Bay",
        address=0x310326,
        can_create=lambda options: options.signsanity.value
    ),
    "Great Bay Coast Outside Fisherman Hut Cut the Sign": MMRLocationData(
        region="Great Bay",
        address=0x310321, 
        can_create=lambda options: options.signsanity.value
    ),
    "Great Bay Coast Beachfront Cut the Sign": MMRLocationData(
        region="Great Bay",
        address=0x310327, 
        can_create=lambda options: options.signsanity.value
    ),
    "Great Bay Coast Entrance to Zora Cape Cut the Sign": MMRLocationData(
        region="Great Bay",
        address=0x310325, 
        can_create=lambda options: options.signsanity.value
    ),
    "Great Bay Coast Marine Lab Cut the Sign": MMRLocationData(
        region="Great Bay",
        address=0x310323, 
        can_create=lambda options: options.signsanity.value
    ),
    "Great Bay Coast Rock Pools Cut the Sign": MMRLocationData(
        region="Great Bay",
        address=0x310322, 
        can_create=lambda options: options.signsanity.value
    ),
    "Great Bay Coast (Clear) Fisherman Boat Cut the Sign": MMRLocationData(
        region="Zora Cape",
        address=0x310312, 
        can_create=lambda options: options.signsanity.value
    ),
    "Zora Cape Jar Game Cut the Sign": MMRLocationData(
        region="Zora Cape",
        address=0x31032B, 
        can_create=lambda options: options.signsanity.value
    ),
    "Zora Cape Waterfall Cut the Sign": MMRLocationData(
        region="Zora Cape",
        address=0x310328,
        can_create=lambda options: options.signsanity.value
    ),
    "Zora Cape Turtle Cut the Sign": MMRLocationData(
        region="Zora Cape",
        address=0x31032A, 
        can_create=lambda options: options.signsanity.value
    ),
    "Ikana Canyon River Cut the Sign": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x31032E,
        can_create=lambda options: options.signsanity.value
    ),
    "Ikana Canyon Outside Sakon's Hideout Cut the Sign": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x310334,  
        can_create=lambda options: options.signsanity.value
    ),
    "Ikana Canyon Outside Secret Shrine Cut the Sign": MMRLocationData(
        region="Lower Ikana Canyon",
        address=0x310333,  
        can_create=lambda options: options.signsanity.value
    ),
    "Ikana Canyon Outside Spring Cut the Sign": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x310330, 
        can_create=lambda options: options.signsanity.value
    ),
    "Ikana Canyon Outside Ikana Castle Cut the Sign": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x310332,
        can_create=lambda options: options.signsanity.value
    ),
    "Ikana Canyon Poe Hut Cut the Sign": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x310331,
        can_create=lambda options: options.signsanity.value
    ),
    "Ikana Canyon Outside Well Cut the Sign": MMRLocationData(
        region="Upper Ikana Canyon",
        address=0x31032F,
        can_create=lambda options: options.signsanity.value
    ),

    # Websanity
    "Swamp Spider House Entrance Web": MMRLocationData(
        region="Southern Swamp",
        address=0x2E4510,
        can_create=lambda options: options.websanity.value
    ),
    "Swamp Spider House Entrance Web Cleared Swamp": MMRLocationData(
        region="Southern Swamp",
        address=0x2E0010,
        can_create=lambda options: options.websanity.value
    ),
    "Woodfall Temple Web Leading to Dark Room": MMRLocationData(
        region="Woodfall Temple",
        address=0x2E1B30,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Entrance Web (1)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2800,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Entrance Web (2)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2801,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Library Web": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2913,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Web Above Door 1st Floor Door": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2910,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Web Over 1st Floor Pot": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2915,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Web 1st Floor Near Staircase": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2911,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Web Basement Near Staircase": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2917,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Web Basement Covering Crates": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2916,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Web Basement Covering Hole": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2914,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Web Basement Covering Door": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2912,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Web Boat Room Covering Crate": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2950,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Boat Room Ceiling Web": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2951,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Coloured Mask Ceiling Web (1)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2830,
        can_create=lambda options: options.websanity.value
    ),
    "Ocean Spider House Coloured Mask Ceiling Web (2)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2E2930,
        can_create=lambda options: options.websanity.value
    ),
    "Beneath the Well Left Side Web Near Fairy Fountain (1)": MMRLocationData(
        region="Beneath the Well",
        address=0x2E4B50,
        can_create=lambda options: options.websanity.value
    ),
    "Beneath the Well Left Side Web Near Fairy Fountain (2)": MMRLocationData(
        region="Beneath the Well",
        address=0x2E4B51,
        can_create=lambda options: options.websanity.value
    ),
    "Beneath the Well Right Side Web Near Milk Gibdo": MMRLocationData(
        region="Beneath the Well",
        address=0x2E4B70,
        can_create=lambda options: options.websanity.value
    ),
    
    #Oneoffs
    #Paintings
    "Ocean Spider House Behind Top Webbed Door Painting (1)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2F2820,
        can_create=lambda options: options.oneoffs.value
    ),
    "Ocean Spider House Behind Top Webbed Door Painting (2)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2F2821,
        can_create=lambda options: options.oneoffs.value
    ),
    "Ocean Spider House Behind Top Webbed Door Painting (3)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2F2822,
        can_create=lambda options: options.oneoffs.value
    ),
    "Ocean Spider House Behind Top Webbed Door Painting (4)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2F2823,
        can_create=lambda options: options.oneoffs.value
    ),
    "Ocean Spider House Coloured Mask Painting (1)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2F2830,
        can_create=lambda options: options.oneoffs.value
    ),
    "Ocean Spider House Coloured Mask Painting (2)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2F2831,
        can_create=lambda options: options.oneoffs.value
    ),
    "Ocean Spider House Coloured Mask Painting (3)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2F2832,
        can_create=lambda options: options.oneoffs.value
    ),
    "Ocean Spider House Coloured Mask Painting (4)": MMRLocationData(
        region="Ocean Spider House",
        address=0x2F2833,
        can_create=lambda options: options.oneoffs.value
    ),
    # Bombable Walls
    "Bombers Bombable Wall": MMRLocationData(
        region="Bomber's Hideout",
        address=0x2F290F,
        can_create=lambda options: options.oneoffs.value,
    ),
    "Snowhead Temple Bombable Wall": MMRLocationData(
        region="Snowhead Temple",
        address=0x2F211F,
        can_create=lambda options: options.oneoffs.value,
    ),
    "Ocean Spider House Entrance Wall": MMRLocationData(
        region="Ocean Spider House",
        address=0x2F280F,
        can_create=lambda options: options.oneoffs.value,
    ),
    "Stone Tower Temple Elegy Maze Bombable Wall": MMRLocationData(
        region="Stone Tower Temple",
        address=0x2F162F,
        can_create=lambda options: options.oneoffs.value,
    ),
    "Graveyard Day 2 Bombable Wall": MMRLocationData(
        region="Ikana Graveyard",
        address=0x2F0C3F,
        can_create=lambda options: options.oneoffs.value,
    ),
    "Link Trial Bombable Wall Iron Knuckle": MMRLocationData(
        region="The Moon",
        address=0x2F663F,
        can_create=lambda options: options.oneoffs.value,
    ),
    "Link Trial Bombable Wall Final Door": MMRLocationData(
        region="The Moon",
        address=0x2F664F,
        can_create=lambda options: options.oneoffs.value,
    ),
    "Goron Trial Chests": MMRLocationData(
        region="The Moon",
        address=0x063F00,
        can_create=lambda options: options.oneoffs.value,
    ),

        # 100% Completion
    "Majora's Soul": MMRLocationData(
        region="Clock Town",
        address=0x0B012F,
        can_create=lambda options: options.completion_goal.value
    ),
}

location_table = {name: data.address for name, data in location_data_table.items() if data.address is not None}
code_to_location_table = {data.address: name for name, data in location_data_table.items() if data.address is not None}
locked_locations = {name: data for name, data in location_data_table.items() if data.locked_item}


_DUNGEON_GRASS_REGIONS = {
    "Woodfall Temple", "Snowhead Temple", "Stone Tower Temple",
    "Odolwa's Lair", "Beneath the Well", "Secret Shrine",
}

_grass_pool = {
    name for name, data in location_data_table.items()
    if "grass_enabled" in data.can_create.__code__.co_names
}

_pot_pool = {
    name for name, data in location_data_table.items()
    if "pot_enabled" in data.can_create.__code__.co_names
}

location_name_groups: Dict[str, set] = {
    "Grass": _grass_pool,
    "Termina Field Grass": {n for n in _grass_pool if location_data_table[n].region == "Termina Field"},
    "Grotto and Cave Grass": {
        n for n in _grass_pool
        if ("Grotto" in n and "Near" not in n)
        or "Lens Cave" in n
        or location_data_table[n].region == "Secret Shrine"
        or ("Bombable Rock Grass" in n and location_data_table[n].region == "Termina Field")
    },
    "Dungeon Grass": {n for n in _grass_pool if location_data_table[n].region in _DUNGEON_GRASS_REGIONS},
    "Pots": _pot_pool,
    "Dungeon Pots": {n for n in _pot_pool if location_data_table[n].region in POT_DUNGEON_REGIONS},
    "Overworld Pots": {n for n in _pot_pool if location_data_table[n].region not in POT_DUNGEON_REGIONS},
}
