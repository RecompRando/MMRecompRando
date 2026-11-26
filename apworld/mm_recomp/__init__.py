from typing import List
from typing import Dict
from typing import TextIO, ClassVar

from BaseClasses import Region, Tutorial, ItemClassification
from worlds.AutoWorld import WebWorld, World
from .Items import MMRItem, item_data_table, item_table, code_to_item_table
from .Locations import MMRLocation, location_data_table, location_table, code_to_location_table, locked_locations
from .Options import MMROptions, mm_option_groups
from .Regions import region_data_table, get_exit
from .Rules import *
from .NormalRules import *
from .Constants import default_shop_prices

class MMRWebWorld(WebWorld):
    # ~ theme = "partyTime"

    setup_en = Tutorial(
        tutorial_name="Start Guide",
        description="A guide to playing Majora's Mask Recompiled in Archipelago.",
        language="English",
        file_name="guide_en.md",
        link="guide/en",
        authors=["LittleCube", "ThatHypedPerson", "PixelShake92", "Muervo_"]
    )
    
    tutorials = [setup_en]

    option_groups = mm_option_groups


class MMRWorld(World):
    """A Zelda game we're not completely burnt out on."""

    game = "Majora's Mask Recompiled"
    data_version = 1
    web = MMRWebWorld()
    options_dataclass = MMROptions
    options = MMROptions
    location_name_to_id = location_table
    item_name_to_id = item_table

    shop_prices = List[int]

    def generate_early(self):

        if self.options.shuffle_stray_fairies.value == 3:
            self.options.local_items.value.add("Stray Fairy (Clock Town)")
            self.options.local_items.value.add("Stray Fairy (Woodfall)")
            self.options.local_items.value.add("Stray Fairy (Snowhead)")
            self.options.local_items.value.add("Stray Fairy (Great Bay)")
            self.options.local_items.value.add("Stray Fairy (Stone Tower)")

        if self.options.shuffle_maps_and_compasses.value == 4:
            self.options.local_items.value.add("Dungeon Map (Woodfall)")
            self.options.local_items.value.add("Dungeon Map (Snowhead)")
            self.options.local_items.value.add("Dungeon Map (Great Bay)")
            self.options.local_items.value.add("Dungeon Map (Stone Tower)")
            self.options.local_items.value.add("Compass (Woodfall)")
            self.options.local_items.value.add("Compass (Snowhead)")
            self.options.local_items.value.add("Compass (Great Bay)")
            self.options.local_items.value.add("Compass (Stone Tower)")

        if self.options.shuffle_small_keys.value == 4:
            self.options.local_items.value.add("Small Key (Woodfall)")
            self.options.local_items.value.add("Small Key (Snowhead)")
            self.options.local_items.value.add("Small Key (Great Bay)")
            self.options.local_items.value.add("Small Key (Stone Tower)")

        if self.options.shuffle_boss_keys.value == 4:
            self.options.local_items.value.add("Boss Key (Woodfall)")
            self.options.local_items.value.add("Boss Key (Snowhead)")
            self.options.local_items.value.add("Boss Key (Great Bay)")
            self.options.local_items.value.add("Boss Key (Stone Tower)")

        # Create shop prices.
        self.shop_prices = []
        self.prices = ""

        if self.options.shopsanity.value != 0:
            # There are 34 (+2 fake) shop locations that need prices
            for i in range(0, 36):
                if self.options.shop_prices.value == 0:
                    price = default_shop_prices[i]
                else:
                    price = self.random.randrange(0, self.options.max_shop_prices.value, 5)
                self.shop_prices.append(price)
        else:
            # populate stored prices with default prices if shopsanity is disabled
            for i in range(0, 36):
                price = default_shop_prices[i]
                self.shop_prices.append(price)

    def create_item(self, name: str) -> MMRItem:
        if (name == "Stray Fairy (Clock Town)" or
            name == "Stray Fairy (Woodfall)" or
            name == "Stray Fairy (Snowhead)" or
            name == "Stray Fairy (Great Bay)" or
            name == "Stray Fairy (Stone Tower)") and self.options.shuffle_great_fairy_rewards == 0:
            return MMRItem(name, ItemClassification.filler, item_data_table[name].code, self.player)
        elif (name == "Swamp Skulltula Token" or
              name == "Ocean Skulltula Token") and self.options.shuffle_spiderhouse_reward == 0:
            return MMRItem(name, ItemClassification.filler, item_data_table[name].code, self.player)
        else:
            return MMRItem(name, item_data_table[name].type, item_data_table[name].code, self.player)

    def place(self, location, item):
        player = self.player
        mw = self.multiworld

        mw.get_location(location, player).place_locked_item(self.create_item(item))

    def create_items(self) -> None:
        mw = self.multiworld

        item_pool: List[MMRItem] = []
        item_pool_count: Dict[str, int] = {}
        for name, item in item_data_table.items():
            item_pool_count[name] = 0
            if item.code and item.can_create(self.options):
                while item_pool_count[name] < item.num_exist:
                    item_pool.append(self.create_item(name))
                    item_pool_count[name] += 1

        mw.itempool += item_pool

        mw.push_precollected(self.create_item("Ocarina of Time"))
        mw.push_precollected(self.create_item("Song of Time"))

        if self.options.swordless.value:
            mw.itempool.append(self.create_item("Progressive Sword"))

        if self.options.shieldless.value:
            mw.itempool.append(self.create_item("Progressive Shield"))

        if self.options.start_with_soaring.value:
            mw.push_precollected(self.create_item("Song of Soaring"))
            self.create_and_add_filler_items()

        if self.options.shuffle_spiderhouse_reward.value != 1:
            mw.itempool.append(self.create_item("Progressive Wallet"))

        if self.options.shuffle_regional_maps.value == 1:
            mw.push_precollected(self.create_item("Clock Town Map"))
            mw.push_precollected(self.create_item("Woodfall Map"))
            mw.push_precollected(self.create_item("Snowhead Map"))
            mw.push_precollected(self.create_item("Romani Ranch Map"))
            mw.push_precollected(self.create_item("Great Bay Map"))
            mw.push_precollected(self.create_item("Stone Tower Map"))
            self.create_and_add_filler_items(6)

        if self.options.shuffle_maps_and_compasses.value == 0:
            mw.push_precollected(self.create_item("Dungeon Map (Woodfall)"))
            mw.push_precollected(self.create_item("Compass (Woodfall)"))
            mw.push_precollected(self.create_item("Dungeon Map (Snowhead)"))
            mw.push_precollected(self.create_item("Compass (Snowhead)"))
            mw.push_precollected(self.create_item("Dungeon Map (Great Bay)"))
            mw.push_precollected(self.create_item("Compass (Great Bay)"))
            mw.push_precollected(self.create_item("Dungeon Map (Stone Tower)"))
            mw.push_precollected(self.create_item("Compass (Stone Tower)"))

        if self.options.shuffle_small_keys.value == 0:
            mw.push_precollected(self.create_item("Small Key (Woodfall)"))
            mw.push_precollected(self.create_item("Small Key (Snowhead)"))
            mw.push_precollected(self.create_item("Small Key (Snowhead)"))
            mw.push_precollected(self.create_item("Small Key (Snowhead)"))
            mw.push_precollected(self.create_item("Small Key (Great Bay)"))
            mw.push_precollected(self.create_item("Small Key (Stone Tower)"))
            mw.push_precollected(self.create_item("Small Key (Stone Tower)"))
            mw.push_precollected(self.create_item("Small Key (Stone Tower)"))
            mw.push_precollected(self.create_item("Small Key (Stone Tower)"))

        if self.options.shuffle_boss_keys == 0:
            mw.push_precollected(self.create_item("Boss Key (Woodfall)"))
            mw.push_precollected(self.create_item("Boss Key (Snowhead)"))
            mw.push_precollected(self.create_item("Boss Key (Great Bay)"))
            mw.push_precollected(self.create_item("Boss Key (Stone Tower)"))

        if self.options.curiosity_shop_trades.value:
            mw.itempool.append(self.create_item("Blue Rupee"))
            mw.itempool.append(self.create_item("Red Rupee"))
            mw.itempool.append(self.create_item("Purple Rupee"))
            mw.itempool.append(self.create_item("Gold Rupee"))

        if self.options.scrubsanity.value != 0:
            self.create_and_add_filler_items(4)

        if self.options.shopsanity.value != 0:
            self.create_and_add_filler_items(27)

        if self.options.shopsanity.value == 2:
            self.create_and_add_filler_items(11)

        if self.options.cowsanity.value != 0:
            self.create_and_add_filler_items(8)

        if self.options.intro_checks.value:
            self.create_and_add_filler_items(1)

        if self.options.shuffle_great_fairy_rewards.value != 0:
            self.create_and_add_filler_items(6)

        if self.options.shuffle_spiderhouse_reward.value != 0:
            self.create_and_add_filler_items(2)

        if self.options.shuffle_minigames.value == 1:
            self.create_and_add_filler_items(5)
        elif self.options.shuffle_minigames.value == 2:
            self.create_and_add_filler_items(9)

        if self.options.shuffle_treasure_chest_game.value == 1:
            self.create_and_add_filler_items(1)
        elif self.options.shuffle_treasure_chest_game.value == 2:
            self.create_and_add_filler_items(4)

        if self.options.shuffle_zora_pot_game.value == 0:
            self.create_and_add_filler_items(1)

        if self.options.shuffle_beaver_races.value:
            self.create_and_add_filler_items(1)
            if self.options.shuffle_beaver_races.value == 2:
                self.create_and_add_filler_items(1)

        if self.options.shuffle_lottery.value:
            self.create_and_add_filler_items(1)

        if self.options.shuffle_picture_rewards.value:
            self.create_and_add_filler_items(1)
            if self.options.shuffle_picture_rewards.value == 2:
                self.create_and_add_filler_items(4)

        if self.options.infinite_magic_behavior. value == 2:
            mw.itempool.append(self.create_item("Progressive Magic"))
        else:
            self.create_and_add_filler_items(1)

        if self.options.shuffle_maps_and_compasses.value == 0:
            self.create_and_add_filler_items(8)

        if self.options.shuffle_small_keys.value == 0:
            self.create_and_add_filler_items(9)

        if self.options.shuffle_boss_keys == 0:
            self.create_and_add_filler_items(4)

        shp = self.options.starting_hearts.value
        if self.options.starting_hearts_are_containers_or_pieces.value == 0:
            for i in range(0, int((12 - shp)/4)):
                mw.itempool.append(self.create_item("Heart Container"))
            for i in range(0, (12 - shp) % 4):
                mw.itempool.append(self.create_item("Heart Piece"))
        else:
            for i in range(0, 12 - shp):
                mw.itempool.append(self.create_item("Heart Piece"))

    def create_regions(self) -> None:
        player = self.player
        mw = self.multiworld

        # Create regions.
        for region_name in region_data_table.keys():
            region = Region(region_name, player, mw)
            mw.regions.append(region)

        # Create locations.
        for region_name, region_data in region_data_table.items():
            region = mw.get_region(region_name, player)
            region.add_locations({
                location_name: location_data.address for location_name, location_data in location_data_table.items()
                if location_data.region == region_name and location_data.can_create(self.options)
            }, MMRLocation)
            region.add_exits(region_data.connecting_regions)

        # Place locked locations.
        for location_name, location_data in locked_locations.items():
            # Ignore locations we never created.
            if not location_data.can_create(self.options):
                continue

            self.place(location_name, location_data_table[location_name].locked_item)

        if self.options.shuffle_regional_maps.value == 0:
            self.place("Tingle Clock Town Map Purchase", "Clock Town Map")
            self.place("Tingle Woodfall Map Purchase", "Woodfall Map")
            self.place("Tingle Snowhead Map Purchase", "Snowhead Map")
            self.place("Tingle Romani Ranch Map Purchase", "Romani Ranch Map")
            self.place("Tingle Great Bay Map Purchase", "Great Bay Map")
            self.place("Tingle Stone Tower Map Purchase", "Stone Tower Map")

        if self.options.shuffle_boss_remains.value == 0:
            self.place("Woodfall Temple Odolwa's Remains", "Odolwa's Remains")
            self.place("Snowhead Temple Goht's Remains", "Goht's Remains")
            self.place("Great Bay Temple Gyorg's Remains", "Gyorg's Remains")
            self.place("Stone Tower Temple Inverted Twinmold's Remains", "Twinmold's Remains")

        if self.options.shuffle_boss_remains.value == 2:
            remains_list = ["Odolwa's Remains", "Goht's Remains", "Gyorg's Remains", "Twinmold's Remains"]
            
            self.place("Woodfall Temple Odolwa's Remains", remains_list.pop(self.random.randint(0, 3)))
            self.place("Snowhead Temple Goht's Remains", remains_list.pop(self.random.randint(0, 2)))
            self.place("Great Bay Temple Gyorg's Remains", remains_list.pop(self.random.randint(0, 1)))
            self.place("Stone Tower Temple Inverted Twinmold's Remains", remains_list[0])

        if self.options.shuffle_spiderhouse_reward.value == 1:
            self.place("Swamp Spider House Reward", "Mask of Truth")
            self.place("Ocean Spider House Reward", "Progressive Wallet")

        if self.options.skullsanity.value == 0:
            for i in range(0, 31):
                if i != 3:
                    self.place(code_to_location_table[0x062700 | i], "Swamp Skulltula Token")
                if i != 0:
                    self.place(code_to_location_table[0x062800 | i], "Ocean Skulltula Token")

        if self.options.shuffle_great_fairy_rewards.value == 1: #vanilla
            self.place("North Clock Town Great Fairy Reward", "Progressive Magic")
            self.place("North Clock Town Great Fairy Reward (Has Transformation Mask)", "Great Fairy Mask")
            self.place("Woodfall Great Fairy Reward", "Great Spin Attack")
            self.place("Snowhead Great Fairy Reward", "Progressive Magic")
            self.place("Great Bay Great Fairy Reward", "Double Defense")
            self.place("Stone Tower Great Fairy Reward", "Great Fairy Sword")

        # Check if any of the dungeon related options are set to vanilla.
        # This is to make sure that the generator doesn't try to place items in locations that are already occupied.
        if self.options.shuffle_maps_and_compasses.value == 1:
            self.place("Woodfall Temple Turtle Chest", "Dungeon Map (Woodfall)")
            self.place("Woodfall Temple Dragonfly Chest", "Compass (Woodfall)")
            self.place("Snowhead Temple Elevator Room Lower Chest", "Dungeon Map (Snowhead)")
            self.place("Snowhead Temple Frozen Block Chest", "Compass (Snowhead)")
            self.place("Great Bay Temple Before Red Valve Room Chest", "Dungeon Map (Great Bay)")
            self.place("Great Bay Temple Caged Chest Room Upper Chest", "Compass (Great Bay)")
            self.place("Stone Tower Temple Armos Room Back Chest", "Dungeon Map (Stone Tower)")
            self.place("Stone Tower Temple Eastern Water Room Sun Block Chest", "Compass (Stone Tower)")
        if self.options.shuffle_small_keys.value == 1:
            self.place("Woodfall Temple Ledge Chest", "Small Key (Woodfall)")

            self.place("Snowhead Temple Behind Stacked Block Chest", "Small Key (Snowhead)")
            self.place("Snowhead Temple Icicle Room Snowball Chest", "Small Key (Snowhead)")
            self.place("Snowhead Temple Bridge Room Freezard Chest", "Small Key (Snowhead)")

            self.place("Great Bay Temple Caged Chest Room Underwater Chest", "Small Key (Great Bay)")

            self.place("Stone Tower Temple Armos Room Lava Chest", "Small Key (Stone Tower)")
            self.place("Stone Tower Temple Eyegore Room Dexi Hand Ledge Chest", "Small Key (Stone Tower)")
            self.place("Stone Tower Temple Inverted Eastern Air Gust Room Switch Chest", "Small Key (Stone Tower)")
            self.place("Stone Tower Temple Inverted Death Armos Maze Chest", "Small Key (Stone Tower)")
        
        if self.options.shuffle_boss_keys.value == 1:
            self.place("Woodfall Temple Gekko Chest", "Boss Key (Woodfall)")
            self.place("Snowhead Temple Upper Wizzrobe Chest", "Boss Key (Snowhead)")
            self.place("Great Bay Temple Mad Jellied Gekko Chest", "Boss Key (Great Bay)")
            self.place("Stone Tower Temple Inverted Gomess Chest", "Boss Key (Stone Tower)")

        if self.options.shuffle_stray_fairies.value == 0:
            self.place("Laundry Pool Stray Fairy (Clock Town)", "Stray Fairy (Clock Town)")

            self.place("Woodfall Temple Entrance Chest SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Switch Chest SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Dark Room Chest SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Entrance Freestanding SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Deku Baba SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Pot SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Platform Hive SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Main Room Bubble SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Skulltula SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Bridge Room Bubble SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Bridge Room Hive SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Pre-Boss Lower Right Bubble SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Pre-Boss Upper Right Bubble SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Pre-Boss Upper Left Bubble SF", "Stray Fairy (Woodfall)")
            self.place("Woodfall Temple Pre-Boss Pillar Bubble SF", "Stray Fairy (Woodfall)")

            self.place("Snowhead Temple Basement Switch Chest SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Elevator Room Invisible Platform Chest SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Stacked Block Upper Chest SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Freezard Torch Room Chest SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Frozen Block Upper Chest SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Icicle Room Hidden Chest SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Main Room Wall Chest SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Bridge Room Pillar Bubble SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Bridge Room Under Platform Bubble SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Elevator Freestanding SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Bombable Stairs Crate SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Timed Switch Room Bubble SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Snowmen Bubble SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Dinolfos Room First SF", "Stray Fairy (Snowhead)")
            self.place("Snowhead Temple Dinolfos Room Second SF", "Stray Fairy (Snowhead)")

            self.place("Great Bay Temple Entrance Torches Chest SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Bio-Baba Hall Chest SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Freezable Waterwheel Upper Chest SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Freezable Waterwheel Lower Chest SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Seesaw Room Chest SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Room Behind Waterfall Ceiling Chest SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Waterwheel Room Skulltula SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Waterwheel Room Bubble SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Blender Pot SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Blender Room Barrel SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Before Red Valve Room Pot SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Caged Chest Room Pot SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Seesaw Room Underwater Barrel SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Pre-Boss Room Platform Bubble SF", "Stray Fairy (Great Bay)")
            self.place("Great Bay Temple Pre-Boss Room Tunnel Bubble SF", "Stray Fairy (Great Bay)")

            self.place("Stone Tower Temple Entrance Room Eye Switch Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Armos Room Upper Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Eyegore Room Switch Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Mirror Room Sun Face Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Mirror Room Sun Block Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Air Gust Room Side Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Air Gust Room Goron Switch Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Eyegore Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Eastern Water Room Underwater Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Inverted Entrance Room Sun Face Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Inverted Eastern Air Gust Room Frozen Switch Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Inverted Wizzrobe Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Inverted Eastern Air Gust Room Fire Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple Entrance Room Lower Chest", "Stray Fairy (Stone Tower)")
            self.place("Stone Tower Temple After Garo Upside Down Chest", "Stray Fairy (Stone Tower)")

        if (self.options.shuffle_maps_and_compasses.value == 2 or self.options.shuffle_small_keys.value == 2 or
        self.options.shuffle_boss_keys.value == 2 or self.options.shuffle_stray_fairies.value == 1):
            unfilled_wft_locations = []
            unfilled_sht_locations = []
            unfilled_gbt_locations = []
            unfilled_stt_locations = []
            for unfilled_location_name in self.multiworld.get_unfilled_locations(self.player):
                for location_name, location_data in location_data_table.items():
                    if location_data.region == "Woodfall Temple":
                        if self.multiworld.players > 1:
                            updated_unfilled_location_name = str(unfilled_location_name).replace(
                                                                 (f" ({self.player_name})"), "")
                            if updated_unfilled_location_name == location_name:
                                unfilled_wft_locations.append(updated_unfilled_location_name)
                        else:
                            if str(unfilled_location_name) == location_name:
                                unfilled_wft_locations.append(str(unfilled_location_name))
                    elif location_data.region == "Snowhead Temple":
                        if self.multiworld.players > 1:
                            updated_unfilled_location_name = str(unfilled_location_name).replace(
                                                                 (f" ({self.player_name})"), "")
                            if updated_unfilled_location_name == location_name:
                                unfilled_sht_locations.append(updated_unfilled_location_name)
                        else:
                            if str(unfilled_location_name) == location_name:
                                unfilled_sht_locations.append(str(unfilled_location_name))
                    elif location_data.region == "Great Bay Temple":
                        if self.multiworld.players > 1:
                            updated_unfilled_location_name = str(unfilled_location_name).replace(
                                                                 (f" ({self.player_name})"), "")
                            if updated_unfilled_location_name == location_name:
                                unfilled_gbt_locations.append(updated_unfilled_location_name)
                        else:
                            if str(unfilled_location_name) == location_name:
                                unfilled_gbt_locations.append(str(unfilled_location_name))
                    elif location_data.region in ["Stone Tower Temple", "Stone Tower Temple (Inverted)"]:
                        if self.multiworld.players > 1:
                            updated_unfilled_location_name = str(unfilled_location_name).replace(
                                                                 (f" ({self.player_name})"), "")
                            if updated_unfilled_location_name == location_name:
                                unfilled_stt_locations.append(updated_unfilled_location_name)
                        else:
                            if str(unfilled_location_name) == location_name:
                                unfilled_stt_locations.append(str(unfilled_location_name))
            self.random.shuffle(unfilled_wft_locations)
            self.random.shuffle(unfilled_sht_locations)
            self.random.shuffle(unfilled_gbt_locations)
            self.random.shuffle(unfilled_stt_locations)
            if self.options.shuffle_maps_and_compasses.value == 2:
                self.place(unfilled_wft_locations.pop(0), "Dungeon Map (Woodfall)")
                self.place(unfilled_wft_locations.pop(0), "Compass (Woodfall)")
                self.place(unfilled_sht_locations.pop(0), "Dungeon Map (Snowhead)")
                self.place(unfilled_sht_locations.pop(0), "Compass (Snowhead)")
                self.place(unfilled_gbt_locations.pop(0), "Dungeon Map (Great Bay)")
                self.place(unfilled_gbt_locations.pop(0), "Compass (Great Bay)")
                self.place(unfilled_stt_locations.pop(0), "Dungeon Map (Stone Tower)")
                self.place(unfilled_stt_locations.pop(0), "Compass (Stone Tower)")
            if self.options.shuffle_small_keys.value == 2:
                self.place(unfilled_wft_locations.pop(0), "Small Key (Woodfall)")
                self.place(unfilled_sht_locations.pop(0), "Small Key (Snowhead)")
                self.place(unfilled_sht_locations.pop(0), "Small Key (Snowhead)")
                self.place(unfilled_sht_locations.pop(0), "Small Key (Snowhead)")
                self.place(unfilled_gbt_locations.pop(0), "Small Key (Great Bay)")
                self.place(unfilled_stt_locations.pop(0), "Small Key (Stone Tower)")
                self.place(unfilled_stt_locations.pop(0), "Small Key (Stone Tower)")
                self.place(unfilled_stt_locations.pop(0), "Small Key (Stone Tower)")
                self.place(unfilled_stt_locations.pop(0), "Small Key (Stone Tower)")
            if self.options.shuffle_boss_keys.value == 2:
                self.place(unfilled_wft_locations.pop(0), "Boss Key (Woodfall)")
                self.place(unfilled_sht_locations.pop(0), "Boss Key (Snowhead)")
                self.place(unfilled_gbt_locations.pop(0), "Boss Key (Great Bay)")
                self.place(unfilled_stt_locations.pop(0), "Boss Key (Stone Tower)")
            if self.options.shuffle_stray_fairies.value == 1:
                count = 0
                while count < 15:
                    count = count + 1
                    self.place(unfilled_wft_locations.pop(0), "Stray Fairy (Woodfall)")
                    self.place(unfilled_sht_locations.pop(0), "Stray Fairy (Snowhead)")
                    self.place(unfilled_gbt_locations.pop(0), "Stray Fairy (Great Bay)")
                    self.place(unfilled_stt_locations.pop(0), "Stray Fairy (Stone Tower)")

        if (self.options.shuffle_maps_and_compasses.value == 3 or self.options.shuffle_small_keys.value == 3 or
        self.options.shuffle_boss_keys.value == 3 or self.options.shuffle_stray_fairies.value == 2):
            unfilled_dungeon_locations = []
            for unfilled_location_name in self.multiworld.get_unfilled_locations(self.player):
                for location_name, location_data in location_data_table.items():
                    if location_data.region in [
                        "Woodfall Temple", "Snowhead Temple", "Great Bay Temple",
                        "Stone Tower Temple", "Stone Tower Temple (Inverted)"
                    ]:
                        if self.multiworld.players > 1:
                            updated_unfilled_location_name = str(unfilled_location_name).replace(
                                                             (f" ({self.player_name})"), "")
                            if updated_unfilled_location_name == location_name:
                                unfilled_dungeon_locations.append(updated_unfilled_location_name)
                        else:
                            if str(unfilled_location_name) == location_name:
                                unfilled_dungeon_locations.append(str(unfilled_location_name))
            self.random.shuffle(unfilled_dungeon_locations)
            if self.options.shuffle_maps_and_compasses.value == 3:
                for item_to_place in [
                    "Dungeon Map (Woodfall)",    "Compass (Woodfall)",
                    "Dungeon Map (Snowhead)",    "Compass (Snowhead)",
                    "Dungeon Map (Great Bay)",   "Compass (Great Bay)",
                    "Dungeon Map (Stone Tower)", "Compass (Stone Tower)"
                ]:
                    self.place(unfilled_dungeon_locations.pop(0), item_to_place)
            if self.options.shuffle_small_keys.value == 3:
                for item_to_place in [
                    "Small Key (Woodfall)",
                    "Small Key (Snowhead)", "Small Key (Snowhead)", "Small Key (Snowhead)",
                    "Small Key (Great Bay)",
                    "Small Key (Stone Tower)", "Small Key (Stone Tower)",
                    "Small Key (Stone Tower)", "Small Key (Stone Tower)"
                ]:
                    self.place(unfilled_dungeon_locations.pop(0), item_to_place)
            if self.options.shuffle_boss_keys.value == 3:
                self.place(unfilled_dungeon_locations.pop(0), "Boss Key (Woodfall)")
                self.place(unfilled_dungeon_locations.pop(0), "Boss Key (Snowhead)")
                self.place(unfilled_dungeon_locations.pop(0), "Boss Key (Great Bay)")
                self.place(unfilled_dungeon_locations.pop(0), "Boss Key (Stone Tower)")
            if self.options.shuffle_stray_fairies.value == 2:
                count = 0
                while count < 15:
                    count = count + 1
                    self.place(unfilled_dungeon_locations.pop(0), "Stray Fairy (Woodfall)")
                    self.place(unfilled_dungeon_locations.pop(0), "Stray Fairy (Snowhead)")
                    self.place(unfilled_dungeon_locations.pop(0), "Stray Fairy (Great Bay)")
                    self.place(unfilled_dungeon_locations.pop(0), "Stray Fairy (Stone Tower)")
            
        sword_location = mw.get_location("Link's Inventory (Kokiri Sword)", player)
        if self.options.swordless.value:
            sword_location.item_rule = lambda item: item.name != "Progressive Sword"
        else:
            sword_location.place_locked_item(self.create_item("Progressive Sword"))

        shield_location = mw.get_location("Link's Inventory (Hero's Shield)", player)
        if self.options.shieldless.value:
            shield_location.item_rule = lambda item: item.name != "Progressive Shield"
        else:
            shield_location.place_locked_item(self.create_item("Progressive Shield"))

        shp = self.options.starting_hearts.value
        if self.options.starting_hearts_are_containers_or_pieces.value == 0:
            containers = int(shp/4) - 1
            for i in range(0, containers):
                self.place(code_to_location_table[0x0D0000 | i], "Heart Container")

            hearts_left = shp % 4
            for i in range(0, hearts_left):
                self.place(code_to_location_table[0x0D0000 | (containers + i)], "Heart Piece")

            if (shp % 4) != 0:
                for i in range(containers + hearts_left, containers + 4):
                    mw.get_location(code_to_location_table[0x0D0000 | i], player).item_rule = lambda item: item.name != "Heart Piece" and item.name != "Heart Container"
        else:
            for i in range(0, shp - 4):
                self.place(code_to_location_table[0x0D0000 | i], "Heart Piece")

            for i in range(shp - 4, 8):
                mw.get_location(code_to_location_table[0x0D0000 | i], player).item_rule = lambda item: item.name != "Heart Piece" and item.name != "Heart Container"

        # TODO: check options to see what player starts with
        # ~ mw.get_location("Top of Clock Tower (Ocarina of Time)", player).place_locked_item(self.create_item(self.get_filler_item_name()))
        # ~ mw.get_location("Top of Clock Tower (Song of Time)", player).place_locked_item(self.create_item(self.get_filler_item_name()))

    def create_and_add_filler_items(self, count: int = 1):
        for i in range(count):
            self.multiworld.itempool.append(self.create_item(self.get_filler_item_name()))

    def get_filler_item_name(self) -> str:
        filler_items = ["Blue Rupee", "Red Rupee", "Purple Rupee", "Silver Rupee", "Gold Rupee"]
        return self.random.choice(filler_items)
        # filler_weights = (50, 25, 10, 5, 1)
        # return self.random.choices(filler_items, weights=filler_weights)[0]

    def set_rules(self) -> None:
        player = self.player
        mw = self.multiworld
        options = self.options
        prices = self.shop_prices

        # Completion condition.
        mw.completion_condition[player] = lambda state: state.has("Victory", player)

        if (self.options.logic_difficulty.value == 4):
            return

        # ~ if (self.options.logic_difficulty.value == 0):
            # ~ region_rules = get_baby_region_rules(player, options)
            # ~ location_rules = get_baby_location_rules(player, options)
        if (self.options.logic_difficulty.value == 1):
            region_rules = get_region_rules(player, options)
            location_rules = get_location_rules(player, options, prices)

        for entrance_name, rule in region_rules.items():
            entrance = mw.get_entrance(entrance_name, player)
            entrance.access_rule = rule

        for location in mw.get_locations(player):
            name = location.name

            if name not in location_rules:
                print(f"Location '{name}' does not have any logic")

            if name in location_rules and location_data_table[name].can_create(self.options):
                location.access_rule = location_rules[name]

    def write_spoiler_header(self, spoiler_handle: TextIO) -> None:
        if self.options.shopsanity.value:
            spoiler_handle.write("\nShop Prices:\n")
            for location, shop_id in shop_location_to_id.items():
                spoiler_handle.write(f"\n{location}: {self.shop_prices[shop_id]} Rupees")

    def fill_slot_data(self):
        shp = self.options.starting_hearts.value
        starting_containers = int(shp/4) - 1
        starting_pieces = shp % 4
        shuffled_containers = int((12 - shp)/4)
        shuffled_pieces = (12 - shp) % 4
        return {
            "skullsanity": self.options.skullsanity.value,
            "shuffle_stray_fairies": self.options.shuffle_stray_fairies.value,
            "shopsanity": self.options.shopsanity.value,                                                                
            "scrubsanity": self.options.scrubsanity.value,
            "shop_prices": self.shop_prices,
            "cowsanity": self.options.cowsanity.value,
            "frogsanity": self.options.frogsanity.value,
            "shuffle_maps_and_compasses": self.options.shuffle_maps_and_compasses.value,
            "shuffle_small_keys": self.options.shuffle_small_keys.value,
            "shuffle_boss_keys": self.options.shuffle_boss_keys.value,
            "intro_checks": self.options.intro_checks.value,
            "curiosity_shop_trades": self.options.curiosity_shop_trades.value,
            "damage_multiplier": self.options.damage_multiplier.value,
            "death_behavior": self.options.death_behavior.value,
            "starting_heart_locations": 8 if self.options.starting_hearts_are_containers_or_pieces.value == 1 else starting_containers + starting_pieces + shuffled_containers + shuffled_pieces,
            "majora_remains_required": self.options.majora_remains_required.value,
            "moon_remains_required": self.options.moon_remains_required.value,
            "required_skull_tokens": self.options.required_skull_tokens.value,
            "required_stray_fairies": self.options.required_stray_fairies.value,
            "start_with_consumables": self.options.start_with_consumables.value,
            "infinite_magic_behavior": self.options.infinite_magic_behavior.value,
            "skip_digging": self.options.skip_digging.value,
            "start_with_inverted_time": self.options.start_with_inverted_time.value,
            "receive_filled_wallets": self.options.receive_filled_wallets.value,
            "remains_allow_boss_warps": self.options.remains_allow_boss_warps.value,
            "magic_is_a_trap": self.options.magic_is_a_trap.value,
            "shuffle_regional_maps": self.options.shuffle_regional_maps.value,
            "shuffle_spiderhouse_reward": self.options.shuffle_spiderhouse_reward.value,
            "shuffle_great_fairy_rewards": self.options.shuffle_great_fairy_rewards.value,
            "shuffle_minigames": self.options.shuffle_minigames.value,
            "shuffle_treasure_chest_game": self.options.shuffle_treasure_chest_game.value,
            "shuffle_zora_pot_game": self.options.shuffle_zora_pot_game.value,
            "shuffle_beaver_races": self.options.shuffle_beaver_races.value,
            "shuffle_picture_rewards": self.options.shuffle_picture_rewards.value,
            "shuffle_lottery": self.options.shuffle_lottery.value,
            "link_tunic_color": ((self.options.link_tunic_color.value[0] & 0xFF) << 16) | ((self.options.link_tunic_color.value[1] & 0xFF) << 8) | (self.options.link_tunic_color.value[2] & 0xFF),
            "logic_difficulty": self.options.logic_difficulty.value
        }
