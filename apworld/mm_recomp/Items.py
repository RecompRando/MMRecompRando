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
        type=ItemClassification.useful
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
    "Garo Mask": MMRItemData(
        code=0x000081,
        type=ItemClassification.progression
    ),
    "Romani Mask": MMRItemData(
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
    "Great Fairy Mask": MMRItemData(
        code=0x000086,
        type=ItemClassification.progression,
        can_create=lambda options: options.shuffle_great_fairy_rewards.value
    ),
    "Gibdo Mask": MMRItemData(
        code=0x000087,
        type=ItemClassification.progression
    ),
    "Don Gero Mask": MMRItemData(
        code=0x000088,
        type=ItemClassification.progression
    ),
    "Kamaro Mask": MMRItemData(
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
        num_exist=2
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
    "Bundle of 30 Arrows": MMRItemData(
        code=0x00001F,
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
        type=ItemClassification.progression,
        num_exist=4,
        can_create=lambda options: False
    ),
    "Bombchu (5)": MMRItemData(
        code=0x00003A,
        type=ItemClassification.progression,
        num_exist=2,
        can_create=lambda options: False
    ),
    "Bombchu (10)": MMRItemData(
        code=0x00001A,
        type=ItemClassification.progression,
        num_exist=2,
        can_create=lambda options: False
    ),
    "Blue Rupee": MMRItemData(
        code=0x000002,
        type=ItemClassification.filler,
        num_exist=14
        # ~ num_exist=6
    ),
    "Red Rupee": MMRItemData(
        code=0x000004,
        type=ItemClassification.filler,
        num_exist=45
        # ~ num_exist=29
    ),
    "Purple Rupee": MMRItemData(
        code=0x000005,
        type=ItemClassification.filler,
        num_exist=11
    ),
    "Silver Rupee": MMRItemData(
        code=0x000006,
        type=ItemClassification.useful,
        num_exist=10
    ),
    "Gold Rupee": MMRItemData(
        code=0x000007,
        type=ItemClassification.useful,
        num_exist=2
    ),
    "Victory": MMRItemData(
        type=ItemClassification.progression,
        can_create=lambda options: False
    ),
}

item_table = {name: data.code for name, data in item_data_table.items() if data.code is not None}
code_to_item_table = {data.code: name for name, data in item_data_table.items() if data.code is not None}
