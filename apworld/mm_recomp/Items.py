from typing import Callable, Dict, NamedTuple, Optional

from BaseClasses import Item, ItemClassification, MultiWorld


class MMRItem(Item):
    game = "Majora's Mask Recompiled"


class MMRItemData(NamedTuple):
    code: Optional[int] = None
    type: ItemClassification = ItemClassification.filler
    num_exist: int = 1
    can_create: Callable = lambda options: True


item_data_table: Dict[str, MMRItemData] = {
    "Stray Fairy (Clock Town)": MMRItemData(
        code=0x01007F,
        type=ItemClassification.progression,
        can_create=lambda options: options.fairysanity.value
    ),
    "Progressive Magic": MMRItemData(
        code=0x020000,
        type=ItemClassification.progression,
        can_create=lambda options: options.shuffle_great_fairy_rewards.value,
        num_exist=2
    ),
    "Great Spin Attack": MMRItemData(
        code=0x020001,
        type=ItemClassification.useful,
        can_create=lambda options: options.shuffle_great_fairy_rewards.value
    ),
    "Double Defense": MMRItemData(
        code=0x020003,
        type=ItemClassification.useful,
        can_create=lambda options: options.shuffle_great_fairy_rewards.value
    ),
    "Bomber's Notebook": MMRItemData(
        code=0x000050,
        type=ItemClassification.progression,
    ),
    "Moon's Tear": MMRItemData(
        code=0x000096,
        type=ItemClassification.progression
    ),
    "Land Title Deed": MMRItemData(
        code=0x000097,
        type=ItemClassification.progression
    ),
    "Swamp Title Deed": MMRItemData(
        code=0x000098,
        type=ItemClassification.progression
    ),
    "Mountain Title Deed": MMRItemData(
        code=0x000099,
        type=ItemClassification.progression
    ),
    "Ocean Title Deed": MMRItemData(
        code=0x00009A,
        type=ItemClassification.progression
    ),
    "Ocarina of Time": MMRItemData(
        code=0x00004C,
        type=ItemClassification.progression,
        can_create=lambda options: False
    ),
    "Heart Piece": MMRItemData(
        code=0x00000C,
        type=ItemClassification.useful,
        num_exist=36
        # ~ num_exist=52
    ),
    "Heart Container": MMRItemData(
        code=0x00000D,
        type=ItemClassification.useful,
        num_exist=8
        # ~ num_exist=4
    ),
    "Swamp Skulltula Token": MMRItemData(
        code=0x000075,
        type=ItemClassification.progression,
        num_exist=30,
        can_create=lambda options: options.skullsanity.value == 1
    ),
    "Ocean Skulltula Token": MMRItemData(
        code=0x000072,
        type=ItemClassification.progression,
        num_exist=30,
        can_create=lambda options: options.skullsanity.value == 1
    ),
    "Progressive Wallet": MMRItemData(
        code=0x000008,
        type=ItemClassification.progression,
        num_exist=1
    ),
    "Sonata of Awakening": MMRItemData(
        code=0x040061,
        type=ItemClassification.progression
    ),
    "Goron Lullaby": MMRItemData(
        code=0x040062,
        type=ItemClassification.progression
    ),
    "New Wave Bossa Nova": MMRItemData(
        code=0x040063,
        type=ItemClassification.progression
    ),
    "Elegy of Emptiness": MMRItemData(
        code=0x040064,
        type=ItemClassification.progression
    ),
    "Oath to Order": MMRItemData(
        code=0x040065,
        type=ItemClassification.progression
    ),
    "Song of Time": MMRItemData(
        code=0x040067,
        type=ItemClassification.progression,
        can_create=lambda options: False
    ),
    "Song of Healing": MMRItemData(
        code=0x040068,
        type=ItemClassification.progression
    ),
    "Epona's Song": MMRItemData(
        code=0x040069,
        type=ItemClassification.progression
    ),
    "Song of Soaring": MMRItemData(
        code=0x04006A,
        type=ItemClassification.progression,
        can_create=lambda options: options.start_with_soaring.value == 0
    ),
    "Song of Storms": MMRItemData(
        code=0x04006B,
        type=ItemClassification.progression
    ),
    "Deku Mask": MMRItemData(
        code=0x000078,
        type=ItemClassification.progression
    ),
    "Goron Mask": MMRItemData(
        code=0x000079,
        type=ItemClassification.progression
    ),
    "Zora Mask": MMRItemData(
        code=0x00007A,
        type=ItemClassification.progression
    ),
    "Fierce Deity's Mask": MMRItemData(
        code=0x00007B,
        type=ItemClassification.progression
    ),
    "Captain's Hat": MMRItemData(
        code=0x00007C,
        type=ItemClassification.progression
    ),
    "Giant's Mask": MMRItemData(
        code=0x00007D,
        type=ItemClassification.progression
    ),
    "All-Night Mask": MMRItemData(
        code=0x00007E,
        type=ItemClassification.progression
    ),
    "Bunny Hood": MMRItemData(
        code=0x00007F,
        type=ItemClassification.progression
    ),
    "Keaton Mask": MMRItemData(
        code=0x000080,
        type=ItemClassification.progression
    ),
    "Garo's Mask": MMRItemData(
        code=0x000081,
        type=ItemClassification.progression
    ),
    "Romani's Mask": MMRItemData(
        code=0x000082,
        type=ItemClassification.progression
    ),
    "Circus Leader's Mask": MMRItemData(
        code=0x000083,
        type=ItemClassification.progression
    ),
    "Postman's Hat": MMRItemData(
        code=0x000084,
        type=ItemClassification.progression
    ),
    "Couple's Mask": MMRItemData(
        code=0x000085,
        type=ItemClassification.progression
    ),
    "Great Fairy's Mask": MMRItemData(
        code=0x000086,
        type=ItemClassification.progression,
        can_create=lambda options: options.shuffle_great_fairy_rewards.value
    ),
    "Gibdo Mask": MMRItemData(
        code=0x000087,
        type=ItemClassification.progression
    ),
    "Don Gero's Mask": MMRItemData(
        code=0x000088,
        type=ItemClassification.progression
    ),
    "Kamaro's Mask": MMRItemData(
        code=0x000089,
        type=ItemClassification.progression
    ),
    "Mask of Truth": MMRItemData(
        code=0x00008A,
        type=ItemClassification.progression,
        can_create=lambda options: options.shuffle_spiderhouse_reward.value
    ),
    "Stone Mask": MMRItemData(
        code=0x00008B,
        type=ItemClassification.progression
    ),
    "Bremen Mask": MMRItemData(
        code=0x00008C,
        type=ItemClassification.progression
    ),
    "Blast Mask": MMRItemData(
        code=0x00008D,
        type=ItemClassification.progression
    ),
    "Mask of Scents": MMRItemData(
        code=0x00008E,
        type=ItemClassification.progression
    ),
    "Kafei's Mask": MMRItemData(
        code=0x00008F,
        type=ItemClassification.progression
    ),
    "Room Key": MMRItemData(
        code=0x0000A0,
        type=ItemClassification.progression
    ),
    "Letter to Kafei": MMRItemData(
        code=0x0000AA,
        type=ItemClassification.progression
    ),
    "Pendant of Memories": MMRItemData(
        code=0x0000AB,
        type=ItemClassification.progression
    ),
    "Priority Mail": MMRItemData(
        code=0x0000A1,
        type=ItemClassification.progression
    ),
    "Bottle": MMRItemData(
        code=0x00005A,
        type=ItemClassification.progression,
        num_exist=3
    ),
    "Bottle of Milk": MMRItemData(
        code=0x000060,
        type=ItemClassification.progression
    ),
    "Bottle of Chateau Romani": MMRItemData(
        code=0x00006F,
        type=ItemClassification.progression
    ),
    "Progressive Sword": MMRItemData(
        code=0x000037,
        type=ItemClassification.progression,
        num_exist=2
    ),
    "Great Fairy Sword": MMRItemData(
        code=0x00003B,
        type=ItemClassification.progression,
        can_create=lambda options: options.shuffle_great_fairy_rewards.value
    ),
    "Progressive Bow": MMRItemData(
        code=0x000022,
        type=ItemClassification.progression,
        num_exist=3
    ),
    "Fire Arrow": MMRItemData(
        code=0x000025,
        type=ItemClassification.progression
    ),
    "Ice Arrow": MMRItemData(
        code=0x000026,
        type=ItemClassification.progression
    ),
    "Light Arrow": MMRItemData(
        code=0x000027,
        type=ItemClassification.progression
    ),
    "Pictograph Box": MMRItemData(
        code=0x000043,
        type=ItemClassification.progression
    ),
    "Lens of Truth": MMRItemData(
        code=0x000042,
        type=ItemClassification.progression
    ),
    "Hookshot": MMRItemData(
        code=0x000041,
        type=ItemClassification.progression
    ),
    "Progressive Shield": MMRItemData(
        code=0x000032,
        type=ItemClassification.progression
    ),
    "Powder Keg": MMRItemData(
        code=0x000034,
        type=ItemClassification.progression
    ),
    "Magic Bean": MMRItemData(
        code=0x000035,
        type=ItemClassification.progression
    ),
    "Bottle of Red Potion": MMRItemData(
        code=0x000059,
        type=ItemClassification.progression
    ),
    # ~ "Blue Potion": MMRItemData(
        # ~ code=0x00005D,
        # ~ type=ItemClassification.progression
    # ~ ),
    "Clock Town Map": MMRItemData(
        code=0x0000B4,
        type=ItemClassification.useful,
        can_create=lambda options: options.shuffle_regional_maps.value == 2
    ),
    "Woodfall Map": MMRItemData(
        code=0x0000B5,
        type=ItemClassification.useful,
        can_create=lambda options: options.shuffle_regional_maps.value == 2
    ),
    "Snowhead Map": MMRItemData(
        code=0x0000B6,
        type=ItemClassification.useful,
        can_create=lambda options: options.shuffle_regional_maps.value == 2
    ),
    "Romani Ranch Map": MMRItemData(
        code=0x0000B7,
        type=ItemClassification.useful,
        can_create=lambda options: options.shuffle_regional_maps.value == 2
    ),
    "Great Bay Map": MMRItemData(
        code=0x0000B8,
        type=ItemClassification.useful,
        can_create=lambda options: options.shuffle_regional_maps.value == 2
    ),
    "Stone Tower Map": MMRItemData(
        code=0x0000B9,
        type=ItemClassification.useful,
        can_create=lambda options: options.shuffle_regional_maps.value == 2
    ),
    "Stray Fairy (Woodfall)": MMRItemData(
        code=0x010000,
        type=ItemClassification.progression,
        num_exist=15,
        can_create=lambda options: options.fairysanity.value
    ),
    "Stray Fairy (Snowhead)": MMRItemData(
        code=0x010001,
        type=ItemClassification.progression,
        num_exist=15,
        can_create=lambda options: options.fairysanity.value
    ),
    "Stray Fairy (Great Bay)": MMRItemData(
        code=0x010002,
        type=ItemClassification.progression,
        num_exist=15,
        can_create=lambda options: options.fairysanity.value
    ),
    "Stray Fairy (Stone Tower)": MMRItemData(
        code=0x010003,
        type=ItemClassification.progression,
        num_exist=15,
        can_create=lambda options: options.fairysanity.value
    ),
    "Small Key (Woodfall)": MMRItemData(
        code=0x090078,
        type=ItemClassification.progression,
        num_exist=1,
        can_create=lambda options: options.keysanity.value
    ),
    "Small Key (Snowhead)": MMRItemData(
        code=0x090178,
        type=ItemClassification.progression,
        num_exist=3,
        can_create=lambda options: options.keysanity.value
    ),
    "Small Key (Great Bay)": MMRItemData(
        code=0x090278,
        type=ItemClassification.progression,
        num_exist=1,
        can_create=lambda options: options.keysanity.value
    ),
    "Small Key (Stone Tower)": MMRItemData(
        code=0x090378,
        type=ItemClassification.progression,
        num_exist=4,
        can_create=lambda options: options.keysanity.value
    ),
    "Dungeon Map (Woodfall)": MMRItemData(
        code=0x090076,
        type=ItemClassification.useful
    ),
    "Dungeon Map (Snowhead)": MMRItemData(
        code=0x090176,
        type=ItemClassification.useful
    ),
    "Dungeon Map (Great Bay)": MMRItemData(
        code=0x090276,
        type=ItemClassification.useful
    ),
    "Dungeon Map (Stone Tower)": MMRItemData(
        code=0x090376,
        type=ItemClassification.useful
    ),
    "Compass (Woodfall)": MMRItemData(
        code=0x090075,
        type=ItemClassification.useful
    ),
    "Compass (Snowhead)": MMRItemData(
        code=0x090175,
        type=ItemClassification.useful
    ),
    "Compass (Great Bay)": MMRItemData(
        code=0x090275,
        type=ItemClassification.useful
    ),
    "Compass (Stone Tower)": MMRItemData(
        code=0x090375,
        type=ItemClassification.useful
    ),
    "Boss Key (Woodfall)": MMRItemData(
        code=0x090074,
        type=ItemClassification.progression,
        can_create=lambda options: options.bosskeysanity.value
    ),
    "Boss Key (Snowhead)": MMRItemData(
        code=0x090174,
        type=ItemClassification.progression,
        can_create=lambda options: options.bosskeysanity.value
    ),
    "Boss Key (Great Bay)": MMRItemData(
        code=0x090274,
        type=ItemClassification.progression,
        can_create=lambda options: options.bosskeysanity.value
    ),
    "Boss Key (Stone Tower)": MMRItemData(
        code=0x090374,
        type=ItemClassification.progression,
        can_create=lambda options: options.bosskeysanity.value
    ),
    "Odolwa's Remains": MMRItemData(
        code=0x000055,
        type=ItemClassification.progression,
        can_create=lambda options: options.shuffle_boss_remains.value == 1
    ),
    "Goht's Remains": MMRItemData(
        code=0x000056,
        type=ItemClassification.progression,
        can_create=lambda options: options.shuffle_boss_remains.value == 1
    ),
    "Gyorg's Remains": MMRItemData(
        code=0x000057,
        type=ItemClassification.progression,
        can_create=lambda options: options.shuffle_boss_remains.value == 1
    ),
    "Twinmold's Remains": MMRItemData(
        code=0x000058,
        type=ItemClassification.progression,
        can_create=lambda options: options.shuffle_boss_remains.value == 1
    ),
    "Progressive Bomb Bag": MMRItemData(
        code=0x00001B,
        type=ItemClassification.progression,
        num_exist=3
    ),
    "Bundle of 10 Arrows": MMRItemData(
        code=0x00001E,
        type=ItemClassification.filler,
        num_exist=1
    ),
    "Bundle of 30 Arrows": MMRItemData(
        code=0x00001F,
        type=ItemClassification.filler,
        num_exist=1
    ),
    "Small Magic Jar": MMRItemData(
        code=0x00000E,
        type=ItemClassification.filler,
        num_exist=1
    ),
    "Large Magic Jar": MMRItemData(
        code=0x00000F,
        type=ItemClassification.filler,
        num_exist=1
    ),
    "Bomb Refill 10": MMRItemData(
        code=0x000016,
        type=ItemClassification.filler,
        num_exist=1
    ),
    "Bomb Refill 30": MMRItemData(
        code=0x000018,
        type=ItemClassification.filler,
        num_exist=1
    ),
    "Deku Nuts 10": MMRItemData(
        code=0x00002A,
        type=ItemClassification.filler,
        num_exist=1
    ),
    # "Deku Nuts Upgrade 30": MMRItemData(
    #     code=0x00009D,
    #     type=ItemClassification.filler,
    #     num_exist=1
    # ),
    # "Deku Nuts Upgrade 40": MMRItemData(
    #     code=0x00009E,
    #     type=ItemClassification.filler,
    #     num_exist=1
    # ),
    "Deku Stick": MMRItemData(
        code=0x000019,
        type=ItemClassification.filler,
        num_exist=1
    ),    
    # "Deku Stick Upgrade 20": MMRItemData(
    #     code=0x00009B,
    #     type=ItemClassification.filler,
    #     num_exist=1
    # ),
    # "Deku Stick Upgrade 30": MMRItemData(
    #     code=0x00009C,
    #     type=ItemClassification.filler,
    #     num_exist=1
    # ),     
    "Recovery Heart": MMRItemData(
        code=0x00000A,
        type=ItemClassification.filler,
        num_exist=1
    ),                                    
    "Progressive Bombchu Bag": MMRItemData(
        code=0x000054,
        type=ItemClassification.progression,
        num_exist=3
    ), 
    "Bombchu (1)": MMRItemData(
        code=0x000036,
        type=ItemClassification.filler,
        num_exist=4,
        can_create=lambda options: False
    ),
    "Bombchu (5)": MMRItemData(
        code=0x00003A,
        type=ItemClassification.filler,
        num_exist=2,
        can_create=lambda options: False
    ),
    "Bombchu (10)": MMRItemData(
        code=0x00001A,
        type=ItemClassification.filler,
        num_exist=5,
        can_create=lambda options: False
    ),
    "Blue Rupee": MMRItemData(
        code=0x000002,
        type=ItemClassification.filler,
        num_exist=6
        # ~ num_exist=6
    ),
    "Crimson Rupee": MMRItemData(
        code=0x000003,
        type=ItemClassification.progression,
        num_exist=1
    ),
    "Red Rupee": MMRItemData(
        code=0x000004,
        type=ItemClassification.filler,
        num_exist=29
        # ~ num_exist=29
    ),
    "Purple Rupee": MMRItemData(
        code=0x000005,
        type=ItemClassification.filler,
        num_exist=11
    ),
    "Silver Rupee": MMRItemData(
        code=0x000006,
        type=ItemClassification.filler,
        num_exist=7
    ),
    "Gold Rupee": MMRItemData(
        code=0x000007,
        type=ItemClassification.filler,
        num_exist=3
    ),
    "Victory": MMRItemData(
        type=ItemClassification.progression,
        can_create=lambda options: False
    ),
    # Owl Statues
    "Clock Town Owl Statue": MMRItemData(
        code=0xFF1504,
        type=ItemClassification.progression,
        can_create=lambda options: options.owlsanity.value
    ),
    "Milk Road Owl Statue": MMRItemData(
        code=0xFF1505,
        type=ItemClassification.progression,
        can_create=lambda options: options.owlsanity.value
    ),
    "Southern Swamp Owl Statue": MMRItemData(
        code=0xFF1507,
        type=ItemClassification.progression,
        can_create=lambda options: options.owlsanity.value
    ),
    "Woodfall Owl Statue": MMRItemData(
        code=0xFF1506,
        type=ItemClassification.progression,
        can_create=lambda options: options.owlsanity.value
    ),
    "Mountain Village Owl Statue": MMRItemData(
        code=0xFF1503,
        type=ItemClassification.progression,
        can_create=lambda options: options.owlsanity.value
    ),
    "Snowhead Owl Statue": MMRItemData(
        code=0xFF1502,
        type=ItemClassification.progression,
        can_create=lambda options: options.owlsanity.value
    ),
    "Great Bay Coast Owl Statue": MMRItemData(
        code=0xFF1500,
        type=ItemClassification.progression,
        can_create=lambda options: options.owlsanity.value
    ),
    "Zora Cape Owl Statue": MMRItemData(
        code=0xFF1501,
        type=ItemClassification.progression,
        can_create=lambda options: options.owlsanity.value
    ),
    "Ikana Canyon Owl Statue": MMRItemData(
        code=0xFF1508,
        type=ItemClassification.progression,
        can_create=lambda options: options.owlsanity.value
    ),
    "Stone Tower Owl Statue": MMRItemData(
        code=0xFF1509,
        type=ItemClassification.progression,
        can_create=lambda options: options.owlsanity.value
    ),
    # Frogs
    "Yellow Frog": MMRItemData(
        code=0xFF0000,
        type=ItemClassification.progression,
        can_create=lambda options: options.frogsanity.value
    ),
    "White Frog": MMRItemData(
        code=0xFF0004,
        type=ItemClassification.progression,
        can_create=lambda options: options.frogsanity.value
    ),
    "Cyan Frog": MMRItemData(
        code=0xFF0003,
        type=ItemClassification.progression,
        can_create=lambda options: options.frogsanity.value
    ),
    "Blue Frog": MMRItemData(
        code=0xFF0001,
        type=ItemClassification.progression,
        can_create=lambda options: options.frogsanity.value
    ),
    "Pink Frog": MMRItemData(
        code=0xFF0002,
        type=ItemClassification.progression,
        can_create=lambda options: options.frogsanity.value
    ),
    # Scarecrows
    "Clock Town Trading Post Scarecrow": MMRItemData(
        code=0x303400,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Astral Observatory Scarecrow": MMRItemData(
        code=0x302910,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Mountain Village Rooftop Scarecrow": MMRItemData(
        code=0x305000,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Mountain Village Spring Rooftop Scarecrow": MMRItemData(
        code=0x305A00,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Path to Snowhead Scarecrow": MMRItemData(
        code=0x305B00,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Path to Snowhead Spring Scarecrow": MMRItemData(
        code=0x305C00,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Twin Islands Scarecrow": MMRItemData(
        code=0x305D00,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Twin Islands Spring Scarecrow": MMRItemData(
        code=0x305E00,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Snowhead Temple Lower Scarecrow": MMRItemData(
        code=0x302140,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Snowhead Temple Hidden Alcove Scarecrow": MMRItemData(
        code=0x302141,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),        
    "Great Bay Coast Rock Wall Scarecrow": MMRItemData(
        code=0x303700,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),    
    "Zora Cape Beavers Scarecrow": MMRItemData(
        code=0x303800,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ), 
    "Zora Cape Island Scarecrow": MMRItemData(
        code=0x303801,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Zora Hall Pervert Scarecrow": MMRItemData(
        code=0x303300,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ), 
    "Road to Ikana Scarecrow": MMRItemData(
        code=0x305300,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Stone Tower Lower Scarecrow": MMRItemData(
        code=0x305800,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    "Stone Tower Upper Scarecrow": MMRItemData(
        code=0x305801,
        type=ItemClassification.progression,
        can_create=lambda options: options.scarecrowsanity.value
    ),
    # Souls
    # Boss Souls
    "Soul of Odolwa": MMRItemData(
        code=0x0B0129,
        type=ItemClassification.progression,
        can_create=lambda options: options.boss_souls.value
    ),
    "Soul of Goht": MMRItemData(
        code=0x0B01DD,
        type=ItemClassification.progression,
        can_create=lambda options: options.boss_souls.value
    ),
    "Soul of Gyorg": MMRItemData(
        code=0x0B012B,
        type=ItemClassification.progression,
        can_create=lambda options: options.boss_souls.value
    ),
    "Soul of Twinmold": MMRItemData(
        code=0x0B012A,
        type=ItemClassification.progression,
        can_create=lambda options: options.boss_souls.value
    ),
    "Soul of Majora": MMRItemData(
        code=0x0B012F,
        type=ItemClassification.progression,
        can_create=lambda options: options.boss_souls.value == 2 and not options.completion_goal.value
    ),
    # Misc. Souls
    "Soul of Cows": MMRItemData(
        code=0x0A00F3,
        type=ItemClassification.progression,
        can_create=lambda options: options.misc_souls.value
    ),
    "Soul of Keaton": MMRItemData(
        code=0x0A028C,
        type=ItemClassification.progression,
        can_create=lambda options: options.misc_souls.value
    ),
    "Soul of Gold Skulltulas": MMRItemData(
        code=0x0A0050,
        type=ItemClassification.progression,
        can_create=lambda options: options.misc_souls.value
    ),
    "Soul of Butterflies": MMRItemData(
        code=0x0A0015,
        type=ItemClassification.progression,
        can_create=lambda options: options.misc_souls.value
    ),
    # NPC Souls
    "Soul of Anju": MMRItemData(
        code=0x0C0202,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Anju's Grandmother": MMRItemData(
        code=0x0C0243,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Anju's Mother": MMRItemData(
        code=0x0C0253,
        type=ItemClassification.filler,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Kafei": MMRItemData(
        code=0x0C0159,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Toilet Hand": MMRItemData(
        code=0x0C027D,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Guru-Guru": MMRItemData(
        code=0x0C0248,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Honey and Darling": MMRItemData(
        code=0x0C00B5,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Treasure Game Lady": MMRItemData(
        code=0x0C01C1,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Toto & Gorman": MMRItemData(
        code=0x0C0234,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Barten": MMRItemData(
        code=0x0C0263,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Madame Aroma": MMRItemData(
        code=0x0C0262,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Receptionist": MMRItemData(
        code=0x0C0290,
        type=ItemClassification.filler,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Council": MMRItemData(
        code=0x0C026F,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Guards": MMRItemData(
        code=0x0C01C7,
        type=ItemClassification.filler,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Bomber Kids": MMRItemData(
        code=0x0C027E,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Tingle": MMRItemData(
        code=0x0C0176,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Bomb Granny": MMRItemData(
        code=0x0C0236,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Sakon": MMRItemData(
        code=0x0C0237,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Deku Playground Employee": MMRItemData(
        code=0x0C01C9,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Great Fairies": MMRItemData(
        code=0x0C0130,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Archery Man": MMRItemData(
        code=0x0C011D,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Postman": MMRItemData(
        code=0x0C01D5,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    # "Soul of Bomb Shop Owner": MMRItemData(
    #     code=0x0C0016,
    #     type=ItemClassification.progression,
    #     can_create=lambda options: options.npc_souls.value
    # ),
    "Soul of Shop Owners": MMRItemData(
        code=0x0C002A,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    # "Soul of Trading Post Night Worker": MMRItemData(
    #     code=0x0C0018,
    #     type=ItemClassification.progression,
    #     can_create=lambda options: options.npc_souls.value
    # ),
    "Soul of Scarecrow": MMRItemData(
        code=0x0C00CA,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Curiosity Shop Man": MMRItemData(
        code=0x0C01C4,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Happy Mask Salesman": MMRItemData(
        code=0x0C01B5,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Skull Kid": MMRItemData(
        code=0x0C0191,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Kamaro": MMRItemData(
        code=0x0C027A,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    # "Soul of Shooting Gallery Man": MMRItemData(
    #     code=0x0C001E,
    #     type=ItemClassification.progression,
    #     can_create=lambda options: options.npc_souls.value
    # ),
    "Soul of Jugglers": MMRItemData(
        code=0x0C0244,
        type=ItemClassification.filler,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Carpenters": MMRItemData(
        code=0x0C009C,
        type=ItemClassification.filler,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Spider Man": MMRItemData(
        code=0x0C00D4,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Business Scrubs": MMRItemData(
        code=0x0C0274,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Astral Observatory Man": MMRItemData(
        code=0x0C0124,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Keg Selling Goron": MMRItemData(
        code=0x0C0242,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Inn Goron": MMRItemData(
        code=0x0C0276,
        type=ItemClassification.filler,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Swordsman": MMRItemData(
        code=0x0C01EF,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Banker": MMRItemData(
        code=0x0C0177,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Rosa Sisters": MMRItemData(
        code=0x0C027B,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Lottery": MMRItemData(
        code=0x0C0239,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Romani & Cremia": MMRItemData(
        code=0x0C021F,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Grog": MMRItemData(
        code=0x0C00A6,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Doggy Race Lady & Dogs": MMRItemData(
        code=0x0C0117,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Gorman Brothers": MMRItemData(
        code=0x0C0067,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Swamp Tourist Guide": MMRItemData(
        code=0x0C01C5,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Kotake": MMRItemData(
        code=0x0C0188,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Koume": MMRItemData(
        code=0x0C0187,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Deku Palace Entry Guards": MMRItemData(
        code=0x0C01A0,
        type=ItemClassification.useful,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Deku Palace Searching Guards": MMRItemData(
        code=0x0C017A,
        type=ItemClassification.useful,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Monkey": MMRItemData(
        code=0x0C019E,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Bean Daddy": MMRItemData(
        code=0x0C00A5,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Goron Elder": MMRItemData(
        code=0x0C0213,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Gatekeeper & Medigoron": MMRItemData(
        code=0x0C0138,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Hungry Goron": MMRItemData(
        code=0x0C023A,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Mountain Smithy": MMRItemData(
        code=0x0C01FF,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Fisherman": MMRItemData(
        code=0x0C01C2,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Marine Lab Researcher": MMRItemData(
        code=0x0C00AE,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Marine Lab Fish": MMRItemData(
        code=0x0C01F1,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Coast Zora": MMRItemData(
        code=0x0C0260,
        type=ItemClassification.useful,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Pot Game & Hall Zora": MMRItemData(
        code=0x0C0228,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Lulu": MMRItemData(
        code=0x0C0252,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Evan": MMRItemData(
        code=0x0C0241,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Japas": MMRItemData(
        code=0x0C0231,
        type=ItemClassification.useful,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Tijo": MMRItemData(
        code=0x0C0238,
        type=ItemClassification.useful,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Shiro": MMRItemData(
        code=0x0C024A,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Stalchildren": MMRItemData(
        code=0x0C00ED,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Dampe": MMRItemData(
        code=0x0C01CA,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Composer Poe": MMRItemData(
        code=0x0C0247,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Pamela's Father": MMRItemData(
        code=0x0C0250,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Gibdos": MMRItemData(
        code=0x0C01DA,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Spirit House Owner": MMRItemData(
        code=0x0C01DE,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    "Soul of Moon Kids": MMRItemData(
        code=0x0C00BF,
        type=ItemClassification.progression,
        can_create=lambda options: options.npc_souls.value
    ),
    # Utility Souls
    "Soul of Postboxes": MMRItemData(
        code=0x0D01F2,
        type=ItemClassification.progression,
        can_create=lambda options: options.utility_souls.value
    ),
    # Absurd Souls
    "Soul of Songwall": MMRItemData(
        code=0x0F01D6,
        type=ItemClassification.progression,
        can_create=lambda options: options.absurd_souls.value
    ),
    "Soul of Trees & Bushes": MMRItemData(
        code=0x0F0041,
        type=ItemClassification.progression,
        can_create=lambda options: options.absurd_souls.value
    ),
    "Soul of Grass": MMRItemData(
        code=0x0F010B,
        type=ItemClassification.progression,
        can_create=lambda options: options.absurd_souls.value
    ),
    "Soul of Grottos": MMRItemData(
        code=0x0F0055,
        type=ItemClassification.progression,
        can_create=lambda options: options.absurd_souls.value
    ),
    "Soul of Deku Flowers": MMRItemData(
        code=0x0F0183,
        type=ItemClassification.progression,
        can_create=lambda options: options.absurd_souls.value
    ),
    "Soul of Barrels": MMRItemData(
        code=0x0F022D,
        type=ItemClassification.progression,
        can_create=lambda options: options.absurd_souls.value
    ),
    "Soul of Gorman Ranch Bulldozer": MMRItemData(
        code=0x0F0287,
        type=ItemClassification.progression,
        can_create=lambda options: options.absurd_souls.value
    ),
    "Soul of Pots": MMRItemData(
        code=0x0F0082,
        type=ItemClassification.progression,
        can_create=lambda options: options.absurd_souls.value
    ),
    "Soul of Rocks": MMRItemData(
        code=0x0F00B0,
        type=ItemClassification.progression,
        can_create=lambda options: options.absurd_souls.value
    ),
    "Soul of Signs": MMRItemData(
        code=0x0F00A8,
        type=ItemClassification.progression,
        can_create=lambda options: options.absurd_souls.value
    ),
    # Enemy Souls
    "Soul of Guays": MMRItemData(
        code=0x0E00F1,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Takkuri": MMRItemData(
        code=0x0E0291,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Wolfos": MMRItemData(
        code=0x0E00EC,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Chu Jelly": MMRItemData(
        code=0x0E014A,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Deku Baba": MMRItemData(
        code=0x0E0033,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Real Bombchu": MMRItemData(
        code=0x0E016F,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Eenos": MMRItemData(
        code=0x0E01E6,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Tektite": MMRItemData(
        code=0x0E0012,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Peehats": MMRItemData(
        code=0x0E0014,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Dodongos": MMRItemData(
        code=0x0E000B,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Dragonflies": MMRItemData(
        code=0x0E0109,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Gekko": MMRItemData(
        code=0x0E0007,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Boes": MMRItemData(
        code=0x0E0164,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Hanging Skulltulas": MMRItemData(
        code=0x0E0024,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Dinolfos": MMRItemData(
        code=0x0E0019,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Snappers": MMRItemData(
        code=0x0E01BA,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Carnivorous Lilypad": MMRItemData(
        code=0x0E013A,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Freezard": MMRItemData(
        code=0x0E008F,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Wizrobe": MMRItemData(
        code=0x0E015D,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Pirate Guards": MMRItemData(
        code=0x0E021E,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Coloured Pirates": MMRItemData(
        code=0x0E021D,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Giant Bonefish": MMRItemData(
        code=0x0E014B,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Wart": MMRItemData(
        code=0x0E012C,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Ocotoroks": MMRItemData(
        code=0x0E0008,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Jellied Gekko": MMRItemData(
        code=0x0E0065,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Iron Knuckle": MMRItemData(
        code=0x0E0084,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Bad Bats": MMRItemData(
        code=0x0E015B,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Keese": MMRItemData(
        code=0x0E000C,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Leevers": MMRItemData(
        code=0x0E0216,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Garo": MMRItemData(
        code=0x0E0113,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Garo Master": MMRItemData(
        code=0x0E0182,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Armos": MMRItemData(
        code=0x0E0032,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Death Armos": MMRItemData(
        code=0x0E002D,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Eyegore": MMRItemData(
        code=0x0E0184,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Redead": MMRItemData(
        code=0x0E004C,
        type=ItemClassification.filler,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Igos": MMRItemData(
        code=0x0E0115,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),
    "Soul of Gomess": MMRItemData(
        code=0x0E0043,
        type=ItemClassification.progression,
        can_create=lambda options: options.enemy_souls.value
    ),


}

item_table = {name: data.code for name, data in item_data_table.items() if data.code is not None}
code_to_item_table = {data.code: name for name, data in item_data_table.items() if data.code is not None}
