from typing import Callable, Dict, NamedTuple, Optional

from BaseClasses import Location, MultiWorld


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

location_data_table: Dict[str, MMRLocationData] = {
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
    "Termina Peahat Grotto Chest": MMRLocationData(
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
        region="Woodfall Temple",
        address=0x051F00
    ),
    "Woodfall Temple Odolwa's Remains": MMRLocationData(
        region="Woodfall Temple",
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
    # "Goron Village Freestanding HP (Spring)": MMRLocationData(
    #     region="Goron Village",
    #     address=0x05481E,
    #     can_create=lambda options: options.shopsanity.value == 2
    # ),
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
        region="Snowhead Temple",
        address=0x054400
    ),
    "Snowhead Temple Goht's Remains": MMRLocationData(
        region="Snowhead Temple",
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
        address=0x062304
    ),
    "Pirates' Fortress Sewers Underwater Upper Chest": MMRLocationData(
        region="Pirates' Fortress Sewers",
        address=0x062306
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
    "Ocean Spider House Libary Painting #1 Token": MMRLocationData(
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
    "Great Bay Temple Caged Chest Room Pot SF": MMRLocationData(
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
    "Great Bay Temple Caged Chest Room Upper Chest": MMRLocationData(
        region="Great Bay Temple",
        address=0x06491C
    ),
    "Great Bay Temple Caged Chest Room Underwater Chest": MMRLocationData(
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
        region="Great Bay Temple",
        address=0x055F00
    ),
    "Great Bay Temple Gyorg's Remains": MMRLocationData(
        region="Great Bay Temple",
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
        region="Ikana Canyon",
        address=0x060714
    ),
    "Ikana Canyon Scrub Purchase": MMRLocationData(
        region="Ikana Canyon",
        address=0x09015D,
        can_create=lambda options: options.scrubsanity.value
    ),
    "Ikana Canyon Zora Scrub Trade": MMRLocationData(
        region="Ikana Canyon",
        address=0x001307
    ),
    "Ikana Canyon Zora Trade Freestanding HP": MMRLocationData(
        region="Ikana Canyon",
        address=0x05131E
    ),
    "Ikana Canyon Healing Pamela's Father": MMRLocationData(
        region="Ikana Canyon",
        address=0x000087
    ),
    "Ikana Canyon Spirit House": MMRLocationData(
        region="Ikana Canyon",
        address=0x0701DE
    ),
    "Stone Tower Great Fairy Reward": MMRLocationData(
        region="Ikana Canyon",
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
        region="Stone Tower Temple (Inverted)",
        address=0x053600
    ),
    "Stone Tower Temple Inverted Twinmold's Remains": MMRLocationData(
        region="Stone Tower Temple (Inverted)",
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
}

location_table = {name: data.address for name, data in location_data_table.items() if data.address is not None}
code_to_location_table = {data.address: name for name, data in location_data_table.items() if data.address is not None}
locked_locations = {name: data for name, data in location_data_table.items() if data.locked_item}
