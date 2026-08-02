from typing import List
from typing import Dict
from typing import TextIO

from BaseClasses import Region, Location, Tutorial, EntranceType, ItemClassification
from worlds.AutoWorld import WebWorld, World
from entrance_rando import randomize_entrances, disconnect_entrance_for_randomization
from .Items import MMRItem, item_data_table, item_table, code_to_item_table
from .Locations import MMRLocation, location_data_table, location_table, code_to_location_table, locked_locations, location_name_groups
from .Options import MMROptions, mm_option_groups
from .Regions import region_data_table, get_exit
from .Rules import *
from .NormalRules import *
from .Constants import *

import copy

class MMRWebWorld(WebWorld):
    # ~ theme = "partyTime"
    
    setup_en = Tutorial(
        tutorial_name="Start Guide",
        description="A guide to playing Majora's Mask Recompiled in Archipelago.",
        language="English",
        file_name="guide_en.md",
        link="guide/en",
        authors=["LittleCube and the Recomp Rando Bois! ThatHypedPerson, PixelShake92 & Muervo_"]
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
    location_name_groups = location_name_groups
    item_name_to_id = item_table
    
    shop_prices = List[int]
    
    entrance_rando_results: Dict[int, int]
    boss_regions: Dict[int, int] # what region a boss clears

    hints: Dict[int, Dict[int, any]] # hints are up here to populate them easier

    def generate_early(self):
        # initialize empty data
        self.shop_prices = []
        self.entrance_rando_results = {}
        self.boss_regions = {}
        self.hints = {}
        
        # Create shop prices.
        if self.options.shopsanity.value != 0:
            price_max = 0

            if self.options.shop_prices.value == 2:
                price_max = 99
            elif self.options.shop_prices.value == 3:
                price_max = 200
            elif self.options.shop_prices.value == 4:
                price_max = 500
                if self.options.deity_wallet.value:
                    price_max = 999

            # There are 34 (+2 fake) shop locations that need prices
            for i in range(0, 36):
                if self.options.shop_prices.value == 0:
                    price = default_shop_prices[i]
                else:
                    price = self.random.randint(0, price_max)
                self.shop_prices.append(price)
        else:
            # populate stored prices with default prices if shopsanity is disabled
            for i in range(0, 36):
                price = default_shop_prices[i]
                self.shop_prices.append(price)
    
    def create_item(self, name: str) -> MMRItem:
        return MMRItem(name, item_data_table[name].type, item_data_table[name].code, self.player)

    def place(self, location, item):
        player = self.player
        mw = self.multiworld

        mw.get_location(location, player).place_locked_item(self.create_item(item))

    def create_items(self) -> None:
        mw = self.multiworld
        filler_amount = 0

        item_pool: List[MMRItem] = []
        item_pool_count: Dict[str, int] = {}
        for name, item in item_data_table.items():
            item_pool_count[name] = 0
            if item.code and item.can_create(self.options):
                while item_pool_count[name] < item.num_exist:
                    item_pool.append(self.create_item(name))
                    item_pool_count[name] += 1

        mw.itempool += item_pool

        if self.options.ocarinaless.value:
            mw.itempool.append(self.create_item("Ocarina of Time"))
        else:
            mw.push_precollected(self.create_item("Ocarina of Time"))

        if self.options.timeless.value:
            mw.itempool.append(self.create_item("Song of Time"))
        else:
            mw.push_precollected(self.create_item("Song of Time"))

        filler_amount += 19

        if self.options.swordless.value:
            mw.itempool.append(self.create_item("Progressive Sword"))

        if self.options.shieldless.value:
            mw.itempool.append(self.create_item("Progressive Shield"))
            
        if self.options.start_with_soaring.value:
            mw.push_precollected(self.create_item("Song of Soaring"))
            filler_amount += 1

        if self.options.child_wallet.value:
            mw.itempool.append(self.create_item("Progressive Wallet"))

        if self.options.deity_wallet.value:
            mw.itempool.append(self.create_item("Progressive Wallet"))
            filler_amount -= 1
        
        if self.options.shuffle_spiderhouse_reward.value:
            mw.itempool.append(self.create_item("Progressive Wallet"))

        if self.options.shuffle_regional_maps.value == 1:
            mw.push_precollected(self.create_item("Clock Town Map"))
            mw.push_precollected(self.create_item("Woodfall Map"))
            mw.push_precollected(self.create_item("Snowhead Map"))
            mw.push_precollected(self.create_item("Romani Ranch Map"))
            mw.push_precollected(self.create_item("Great Bay Map"))
            mw.push_precollected(self.create_item("Stone Tower Map"))
            filler_amount += 6
        
        if self.options.curiostity_shop_trades.value:
            mw.itempool.append(self.create_item("Blue Rupee"))
            mw.itempool.append(self.create_item("Red Rupee"))
            mw.itempool.append(self.create_item("Purple Rupee"))
            mw.itempool.append(self.create_item("Gold Rupee"))

        if self.options.scrubsanity.value != 0:
            filler_amount += 4
        
        if self.options.shopsanity.value != 0:
            filler_amount += 27

        if self.options.shopsanity.value == 2:
            filler_amount += 12
        
        if self.options.cowsanity.value != 0:
            filler_amount += 8
    
        if self.options.intro_checks.value:
            filler_amount += 1
    
        grass_mode = self.options.grasssanity.value
        if grass_mode == 1:  # normal
            grass_filler = 1022
        elif grass_mode == 2:  # no_termina_field
            grass_filler = 682
        elif grass_mode == 3:  # grotto_and_cave_only
            grass_filler = 415
        elif grass_mode == 4:  # dungeon_only
            grass_filler = 112
        else:
            grass_filler = 0

        if self.options.intro_checks.value and grass_mode in (1, 2):
            filler_amount += 51

        filler_amount += grass_filler

        pot_mode = self.options.potsanity.value
        if pot_mode == 1:  # all
            filler_amount += 540
        elif pot_mode == 2:  # overworld_only
            filler_amount += 183
        elif pot_mode == 3:  # dungeon_only
            filler_amount += 357
        
        if self.options.rocksanity.value != 0:
            filler_amount += 129

        if self.options.soilsanity.value != 0:
            filler_amount += 29

        if self.options.hitsanity.value != 0:
            filler_amount += 73

        if self.options.invisisanity.value != 0:
            filler_amount += 22

        if self.options.rupeesanity.value != 0:
            filler_amount += 215

        if self.options.snowsanity.value != 0:
            filler_amount += 119

        if self.options.woodsanity.value != 0:
            filler_amount += 127
                                 
        if self.options.realfairysanity.value != 0:
            filler_amount += 92

        if self.options.iciclesanity.value != 0:
            filler_amount += 41

        if self.options.hivesanity.value != 0:
            filler_amount += 17

        if self.options.notebooksanity.value != 0:
            filler_amount += 55

        if self.options.treesanity.value != 0:
            filler_amount += 96

        if self.options.flowersanity.value != 0:
            filler_amount += 156

        if self.options.signsanity.value != 0:
            filler_amount += 61
        
        if self.options.websanity.value != 0:
            filler_amount += 18
        
        if self.options.oneoffs.value != 0:
            filler_amount += 14

        if self.options.boss_souls.value:
            filler_amount -= 4

        if self.options.boss_souls.value == 2:
            filler_amount -= 1

        if self.options.misc_souls.value:
            filler_amount -= 4

        if self.options.npc_souls.value:
            filler_amount -= 74

        if self.options.utility_souls.value:
            filler_amount -= 1

        if self.options.absurd_souls.value:
            filler_amount -= 1

        filler_amount += 100 #temp

        self.create_and_add_filler_items(filler_amount)

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

        if not self.options.shuffle_spiderhouse_reward.value:
            self.place("Swamp Spider House Reward", "Mask of Truth")
            self.place("Ocean Spider House Reward", "Progressive Wallet")

        if self.options.skullsanity.value == 0:
            for i in range(0, 31):
                if i != 3:
                    self.place(code_to_location_table[0x062700 | i], "Swamp Skulltula Token")
                if i != 0:
                    self.place(code_to_location_table[0x062800 | i], "Ocean Skulltula Token")
                

        if not self.options.shuffle_great_fairy_rewards.value:
            self.place("North Clock Town Great Fairy Reward", "Progressive Magic")
            self.place("North Clock Town Great Fairy Reward (Has Transformation Mask)", "Great Fairy's Mask")
            self.place("Woodfall Great Fairy Reward", "Great Spin Attack")
            self.place("Snowhead Great Fairy Reward", "Progressive Magic")
            self.place("Great Bay Great Fairy Reward", "Double Defense")
            self.place("Stone Tower Great Fairy Reward", "Great Fairy Sword")

        if not self.options.keysanity.value:
            self.place("Woodfall Temple Ledge Chest", "Small Key (Woodfall)")

            self.place("Snowhead Temple Behind Stacked Block Chest", "Small Key (Snowhead)")
            self.place("Snowhead Temple Icicle Room Snowball Chest", "Small Key (Snowhead)")
            self.place("Snowhead Temple Bridge Room Freezard Chest", "Small Key (Snowhead)")

            self.place("Great Bay Temple Before Gekko Room Underwater Chest", "Small Key (Great Bay)")

            self.place("Stone Tower Temple Armos Room Lava Chest", "Small Key (Stone Tower)")
            self.place("Stone Tower Temple Eyegore Room Dexi Hand Ledge Chest", "Small Key (Stone Tower)")
            self.place("Stone Tower Temple Inverted Eastern Air Gust Room Switch Chest", "Small Key (Stone Tower)")
            self.place("Stone Tower Temple Inverted Death Armos Maze Chest", "Small Key (Stone Tower)")
        
        if not self.options.bosskeysanity.value:
            self.place("Woodfall Temple Gekko Chest", "Boss Key (Woodfall)")
            self.place("Snowhead Temple Upper Wizzrobe Chest", "Boss Key (Snowhead)")
            self.place("Great Bay Temple Mad Jellied Gekko Chest", "Boss Key (Great Bay)")
            self.place("Stone Tower Temple Inverted Gomess Chest", "Boss Key (Stone Tower)")

        if not self.options.fairysanity.value:
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
            self.place("Great Bay Temple Before Gekko Room Pot SF", "Stray Fairy (Great Bay)")
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

      
        if not self.options.ocarinaless.value:
            ocarina_location = mw.get_location("Link's Inventory (Ocarina of Time)", player)
            ocarina_location.place_locked_item(self.create_item("Ocarina of Time"))

        if not self.options.timeless.value:
            song_of_time_location = mw.get_location("Link's Inventory (Song of Time)", player)
            song_of_time_location.place_locked_item(self.create_item("Song of Time"))

        if not self.options.swordless.value:
            sword_location = mw.get_location("Link's Inventory (Kokiri Sword)", player)
            sword_location.place_locked_item(self.create_item("Progressive Sword"))

        if not self.options.shieldless.value:
            shield_location = mw.get_location("Link's Inventory (Hero's Shield)", player)
            shield_location.place_locked_item(self.create_item("Progressive Shield"))

        if self.options.completion_goal.value:
            giga_location = mw.get_location("Majora's Soul", player)
            giga_location.place_locked_item(self.create_item("Soul of Majora"))

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
        filler_items = ["Blue Rupee", "Red Rupee", "Purple Rupee", "Silver Rupee", "Gold Rupee", 
                        "Bundle of 30 Arrows", "Bomb Refill 10", "Bomb Refill 30", "Deku Nuts 10", "Recovery Heart",
                        "Small Magic Jar", "Large Magic Jar"]
        return self.random.choice(filler_items)
        # filler_weights = (50, 25, 10, 5, 1)
        # return self.random.choices(filler_items, weights=filler_weights)[0]

    def set_rules(self) -> None:
        player = self.player
        mw = self.multiworld
        options = self.options
        prices = self.shop_prices
        boss_regions = self.boss_regions

        # Set "boss_regions" to their vanilla values when dungeon and boss randomization is disabled
        # this has to be here instead of "connect_entrances" to not fail generation
        if not self.options.dungeon_entrance_rando.value and not self.options.boss_entrance_rando.value:
            self.boss_regions = {
                DUNGEON_WOODFALL: DUNGEON_WOODFALL,
                DUNGEON_SNOWHEAD: DUNGEON_SNOWHEAD,
                DUNGEON_GREAT_BAY: DUNGEON_GREAT_BAY,
                DUNGEON_STONE_TOWER: DUNGEON_STONE_TOWER
            }
            boss_regions = self.boss_regions

        # Completion condition.
        mw.completion_condition[player] = lambda state: state.has("Victory", player)

        if (self.options.logic_difficulty.value == 4):
            return

        # ~ if (self.options.logic_difficulty.value == 0):
            # ~ region_rules = get_baby_region_rules(player, options)
            # ~ location_rules = get_baby_location_rules(player, options)
        if (self.options.logic_difficulty.value == 1):
            region_rules = get_region_rules(player, options)
            location_rules = get_location_rules(player, options, prices, boss_regions)

        for entrance_name, rule in region_rules.items():
            entrance = mw.get_entrance(entrance_name, player)
            entrance.access_rule = rule

        for location in mw.get_locations(player):
            name = location.name

            # Debug Printing
            # if name not in location_rules:
            #     print(f"Location '{name}' does not have any logic")

            if self.options.skullsanity.value == 2 and (name == "Swamp Spider House Reward" or name == "Ocean Spider House Reward"):
                continue
            if name in location_rules and location_data_table[name].can_create(self.options):
                location.access_rule = location_rules[name]

    def connect_entrances(self) -> None:
        player = self.player
        mw = self.multiworld
        no_target_groups = {0: [0]} # unsure how target groups work

        entrance_rando_dungeon_results = []
        entrance_rando_boss_results = []

        if not self.options.dungeon_entrance_rando.value and not self.options.boss_entrance_rando.value:
            return

        # could be a lot nicer, but PoC (this won't work for full entrance rando)
        # Dungeon Chaining
        if self.options.dungeon_chaining.value and self.options.dungeon_entrance_rando.value and self.options.boss_entrance_rando.value:
            # disconnect dungeons
            for entrance in mw.get_entrances(player):
                if entrance.name in dungeon_entrances_er:
                    disconnect_entrance_for_randomization(entrance, None, entrance.connected_region.name)
                    entrance.access_rule = entrance.access_rule
            
            # disconnect bosses
            for entrance in mw.get_entrances(player):
                    if entrance.name in dungeon_bosses_er:
                        disconnect_entrance_for_randomization(entrance, None, entrance.connected_region.name)
                        entrance.access_rule = entrance.access_rule
            
            placement = randomize_entrances(self, coupled=True, target_group_lookup=no_target_groups)

            # scuffed way of ordering results
            for dungeon_entrance in dungeon_entrances_er:
                for pairing in placement.pairings:
                    if pairing[0] == dungeon_entrance:
                        entrance_rando_dungeon_results.append(pairing)
                        continue
            for boss_entrance in dungeon_bosses_er:
                for pairing in placement.pairings:
                    if pairing[0] == boss_entrance:
                        entrance_rando_boss_results.append(pairing)
                        continue
        else:
            # Dungeon Entrances
            if self.options.dungeon_entrance_rando.value:
                for entrance in mw.get_entrances(player):
                    if entrance.name in dungeon_entrances_er:
                        # entrance.randomization_type = EntranceType.TWO_WAY # why do you not work
                        disconnect_entrance_for_randomization(entrance, None, entrance.connected_region.name)
                        entrance.access_rule = entrance.access_rule # i don't know the purpose of this
                
                placement = randomize_entrances(self, coupled=True, target_group_lookup=no_target_groups)

                # scuffed way of ordering results
                for dungeon_entrance in dungeon_entrances_er:
                    for pairing in placement.pairings:
                        if pairing[0] == dungeon_entrance:
                            entrance_rando_dungeon_results.append(pairing)
                            continue
            else:
                for entrance in dungeon_entrances_er:
                    original_exit = entrance[entrance.index("->") + 3:]
                    entrance_rando_dungeon_results.append((entrance, original_exit))

            # Boss Entrances
            if self.options.boss_entrance_rando.value:
                for entrance in mw.get_entrances(player):
                    if entrance.name in dungeon_bosses_er:
                        disconnect_entrance_for_randomization(entrance, None, entrance.connected_region.name)
                        entrance.access_rule = entrance.access_rule
                
                placement = randomize_entrances(self, coupled=True, target_group_lookup=no_target_groups)

                # scuffed way of ordering results
                for boss_entrance in dungeon_bosses_er:
                    for pairing in placement.pairings:
                        if pairing[0] == boss_entrance:
                            entrance_rando_boss_results.append(pairing)
                            continue
            else:
                for entrance in dungeon_bosses_er:
                    original_exit = entrance[entrance.index("->") + 3:]
                    entrance_rando_dungeon_results.append((entrance, original_exit))

        # save entrance rando results
        for pairing in (entrance_rando_dungeon_results + entrance_rando_boss_results):
            original_entrance_full = pairing[0]
            original_entrance = original_entrance_full[:original_entrance_full.index(" ->")]
            original_exit = original_entrance_full[original_entrance_full.index("-> ") + 3:]
            replaced_exit = pairing[1]
            
            replaced_entrance = original_entrance_lookup[replaced_exit]

            original_entrance_id = entrance_to_id_lookup[original_entrance_full]
            replaced_entrance_id = entrance_to_id_lookup[replaced_entrance]
            self.entrance_rando_results[original_entrance_id] = replaced_entrance_id

            self.multiworld.spoiler.set_entrance(original_entrance, replaced_exit, "", self.player)

            # reversing entrances (going back to dungeon entrances)
            is_boss = replaced_exit in mm_bosses
            new_exit = original_entrance_full
            count = 0
            while self.get_entrance(new_exit).parent_region.name not in dungeon_entrances:                
                if count > 10: # arbitrary number, shouldn't occur unless something goes really wrong
                    raise Exception("reverse entrances looped too much")
                
                new_exit = self.get_entrance(new_exit).parent_region.entrances[0].name # assuming there's only one entrance
                count += 1

            new_exit_region = self.get_entrance(new_exit).parent_region.name
            
            if is_boss:
                original_boss_region = mm_bosses.index(replaced_exit)
                new_boss_region = dungeon_entrances.index(new_exit_region)
                
                self.boss_regions[original_boss_region] = new_boss_region
            else:
                original_dungeon_exit = replaced_entrance[:replaced_entrance.index(" ->")]
                
                original_exit_id = entrance_to_id_lookup[original_dungeon_exit]
                replaced_exit_id = entrance_to_id_lookup[new_exit_region] # dungeon at the start of chain's exit

                self.entrance_rando_results[original_exit_id] = replaced_exit_id

        # import json
        # print(json.dumps(self.entrance_rando_results, indent=4))
        # print(json.dumps(self.boss_regions, indent=4))

    def location_to_slotdata(self, location: Location):
        mw = self.multiworld
        return {
            "location_name": location.name,
            "item_name": location.item.name,
            "player": location.player,
            "address": location.address,
        }
    
    def locations_to_slotdata(self, locations: List[Location]):
        formatted = []
        for location in locations:
            formatted.append(self.location_to_slotdata(location))
        return formatted
    
    # Grab hints for gossip stones
    def generate_hints(self):
        mw = self.multiworld
        player = self.player
        hints = self.hints

        # Fill moon gossip stones with their original mask hints
        for gossip_stone, item in moon_gossip_lookup.items():
            try:
                location = mw.find_item(item, player)
                hints[gossip_stone]["item"] = item
                hints[gossip_stone]["item_type"] = ItemClassification.progression
                hints[gossip_stone]["location"] = location.name
                hints[gossip_stone]["location_id"] = location.address
                hints[gossip_stone]["from_player"] = location.player
                hints[gossip_stone]["to_player"] = location.item.player # redundant for moon gossips
                hints[gossip_stone]["region"] = location.parent_region.name or "" # might not exist sometimes?
                hints[gossip_stone]["type"] = HintEnum.MOON.value
                hints[gossip_stone]["filled"] = True
            except StopIteration:
                continue # replaces hint with junk (starting item from pool)

        hint_count = 0
        hint_pool = []
        fill_amount = len([text_id for text_id, hint in hints.items() if not hint["filled"]]) * self.options.hint_percentage.value / 100
        important_hints_filled = False

        # fill percentage of remaining unfilled locations
        while hint_count < fill_amount:
            # go through priority hints first, then the rest, and cycle back if both pools are empty
            if not hint_pool:
                if not important_hints_filled:
                    hint_pool.extend(priority_hints)
                    important_hints_filled = True
                else:
                    hint_pool.extend(other_hints)
                    important_hints_filled = False # reset to false if we need to grab more hints
            
            fill_choice = self.random.choice([text_id for text_id, hint in hints.items() if not hint["filled"]])
            hint_choice = self.random.choice(hint_pool)
            hint_pool.remove(hint_choice)

            hint_item = None
            hint_location = None

            # priority
            if hint_choice == HintEnum.DEKU:
                hint_item = "Deku Mask"
            elif hint_choice == HintEnum.SONATA:
                hint_item = "Sonata of Awakening"
            elif hint_choice == HintEnum.GORON:
                hint_item = "Goron Mask"
            elif hint_choice == HintEnum.LULLABY:
                hint_item = "Goron Lullaby"
            elif hint_choice == HintEnum.ZORA:
                hint_item = "Zora Mask"
            elif hint_choice == HintEnum.NOVA:
                hint_item = "New Wave Bossa Nova"
            elif hint_choice == HintEnum.ELEGY:
                hint_item = "Elegy of Emptiness"
            elif hint_choice == HintEnum.OATH:
                hint_item = "Oath to Order"
            # other
            elif hint_choice == HintEnum.FD:
                hint_item = "Fierce Deity's Mask"
            elif hint_choice == HintEnum.MAGIC:
                hint_item = "Progressive Magic" # will only give a single magic
            elif hint_choice == HintEnum.HD_3:
                hint_location = "East Clock Town Honey and Darling All Days"
            elif hint_choice == HintEnum.DP_3:
                hint_location = "North Clock Town Deku Playground All Days"
            elif hint_choice == HintEnum.BEAVERS:
                hint_location = "Beaver Bros. Race 1"
                extra_location = mw.get_location("Beaver Bros. Race 2 HP", player)
                hints[fill_choice]["extra"] = {
                    "item": extra_location.item.name,
                    "item_type": extra_location.item.classification,
                    "player": extra_location.item.player,
                    "location_id": extra_location.address
                }
            elif hint_choice == HintEnum.ANJU_KAFEI:
                hint_location = "Stock Pot Inn Anju and Kafei"

            # hint revolves around an item
            if hint_item:
                try:
                    location = mw.find_item(hint_item, player)
                    hints[fill_choice]["item"] = hint_item
                    hints[fill_choice]["location"] = location.name
                    hints[fill_choice]["item_type"] = location.item.classification
                    hints[fill_choice]["location_id"] = location.address
                    hints[fill_choice]["from_player"] = location.player
                    hints[fill_choice]["to_player"] = location.item.player # equal to current slot
                    hints[fill_choice]["region"] = location.parent_region.name # might not exist sometimes?
                    hints[fill_choice]["type"] = hint_choice.value
                    hints[fill_choice]["filled"] = True
                except StopIteration:
                    continue # ignore hint where item isn't found (starting item from pool)
            elif hint_location:
                location = mw.get_location(hint_location, player)
                hints[fill_choice]["item"] = location.item.name
                hints[fill_choice]["item_type"] = location.item.classification
                hints[fill_choice]["location"] = location.name
                hints[fill_choice]["location_id"] = location.address
                hints[fill_choice]["from_player"] = location.player # equal to current slot
                hints[fill_choice]["to_player"] = location.item.player
                hints[fill_choice]["region"] = location.parent_region.name
                hints[fill_choice]["type"] = hint_choice.value
                hints[fill_choice]["filled"] = True

            hint_item = None
            hint_location = None
            
            hint_count += 1

    def fill_slot_data(self):
        mw = self.multiworld
        self.hints = copy.deepcopy(gossip_stones) # create a copy of the base dictionary

        shp = self.options.starting_hearts.value
        starting_containers = int(shp/4) - 1
        starting_pieces = shp % 4
        shuffled_containers = int((12 - shp)/4)
        shuffled_pieces = (12 - shp) % 4

        self.generate_hints()

        fairy_locations = {
            "Clock Town": self.location_to_slotdata(mw.find_item("Stray Fairy (Clock Town)", self.player)),
            "Woodfall": self.locations_to_slotdata(mw.find_item_locations("Stray Fairy (Woodfall)", self.player)),
            "Snowhead": self.locations_to_slotdata(mw.find_item_locations("Stray Fairy (Snowhead)", self.player)),
            "Great Bay": self.locations_to_slotdata(mw.find_item_locations("Stray Fairy (Great Bay)", self.player)),
            "Stone Tower": self.locations_to_slotdata(mw.find_item_locations("Stray Fairy (Stone Tower)", self.player)),
        }

        skull_locations = {
            "Swamp": self.locations_to_slotdata(mw.find_item_locations("Swamp Skulltula Token", self.player)),
            "Ocean": self.locations_to_slotdata(mw.find_item_locations("Ocean Skulltula Token", self.player)),
        }

        remain_locations = [
            self.location_to_slotdata(mw.find_item("Odolwa's Remains", self.player)),
            self.location_to_slotdata(mw.find_item("Goht's Remains", self.player)),
            self.location_to_slotdata(mw.find_item("Gyorg's Remains", self.player)),
            self.location_to_slotdata(mw.find_item("Twinmold's Remains", self.player)),
        ]
        
        # print()
        # print(fairy_locations)
        # print(skull_locations)
        # import json
        # print(json.dumps(fairy_locations, indent=4))
        # print(json.dumps(skull_locations, indent=4))

        # print()
        # # print(list(mw.get_locations()))
        # print()
        # item_location = mw.find_item("Fierce Deity's Mask", self.player)
        # print(item_location.name, mw.get_player_name(item_location.player))

        # item_locations = mw.find_item_locations("Stray Fairy (Woodfall)", self.player)
        # print(item_locations) # prints all 15 Locations

        # mw.spoiler.create_playthrough()
        # playthrough = mw.spoiler.playthrough

        # import json
        # print(json.dumps(playthrough, indent=4))

        return {
            "skullsanity": self.options.skullsanity.value,
            "fairysanity": self.options.fairysanity.value,
            "shopsanity": self.options.shopsanity.value,                                                                
            "scrubsanity": self.options.scrubsanity.value,
            "shop_prices": self.shop_prices,
            "cowsanity": self.options.cowsanity.value,
            "keysanity": self.options.keysanity.value,
            "bosskeysanity": self.options.bosskeysanity.value,
            "curiostity_shop_trades": self.options.curiostity_shop_trades.value, 
            "intro_checks": self.options.intro_checks.value,
            "grasssanity": self.options.grasssanity.value,
            "potsanity": self.options.potsanity.value,                        
            "rocksanity": self.options.rocksanity.value,
            "hitsanity": self.options.hitsanity.value,
            "soilsanity": self.options.soilsanity.value,  
            "invisisanity": self.options.invisisanity.value,
            "rupeesanity": self.options.rupeesanity.value,
            "snowsanity": self.options.snowsanity.value,
            "woodsanity": self.options.woodsanity.value,
            "realfairysanity": self.options.realfairysanity.value,
            "iciclesanity":  self.options.iciclesanity.value,
            "scarecrowsanity": self.options.scarecrowsanity.value,
            "hivesanity": self.options.hivesanity.value,
            "notebooksanity": self.options.notebooksanity.value,
            "owlsanity": self.options.owlsanity.value,
            "frogsanity": self.options.frogsanity.value,
            "treesanity": self.options.treesanity.value,
            "flowersanity": self.options.flowersanity.value,
            "signsanity": self.options.signsanity.value,
            "websanity": self.options.websanity.value,
            "oneoffs": self.options.oneoffs.value,
            "boss_souls": self.options.boss_souls.value,
            "misc_souls": self.options.misc_souls.value,
            "npc_souls": self.options.npc_souls.value,
            "absurd_souls": self.options.absurd_souls.value,
            "utility_souls": self.options.utility_souls.value,
            "enemy_souls": self.options.enemy_souls.value,
            "damage_multiplier": self.options.damage_multiplier.value,
            "death_behavior": self.options.death_behavior.value,
            "death_link": self.options.death_link.value,
            "camc": self.options.camc.value,
            "child_wallet": self.options.child_wallet.value,
            "deity_wallet": self.options.deity_wallet.value,
            "starting_heart_locations": 8 if self.options.starting_hearts_are_containers_or_pieces.value == 1 else starting_containers + starting_pieces + shuffled_containers + shuffled_pieces,
            "majora_remains_required": self.options.majora_remains_required.value,
            "majora_masks_required": self.options.majora_masks_required.value,
            "majora_star_fox": self.options.majora_star_fox.value,
            "majora_owls_required": self.options.majora_owls_required.value,
            "majora_scarecrows_required": self.options.majora_scarecrows_required.value,
            "majora_frogs_required": self.options.majora_frogs_required.value,
            "majora_items_required": self.options.majora_items_required.value,
            "moon_remains_required": self.options.moon_remains_required.value,
            "moon_masks_required": self.options.moon_masks_required.value,
            "moon_star_fox": self.options.moon_star_fox.value,
            "moon_owls_required": self.options.moon_owls_required.value,
            "moon_scarecrows_required": self.options.moon_scarecrows_required.value,
            "moon_frogs_required": self.options.moon_frogs_required.value,
            "moon_items_required": self.options.moon_items_required.value,
            "completion_goal": self.options.completion_goal.value,
            "required_skull_tokens": self.options.required_skull_tokens.value,
            "required_stray_fairies": self.options.required_stray_fairies.value,
            "start_with_consumables": self.options.start_with_consumables.value,
            "permanent_chateau_romani": self.options.permanent_chateau_romani.value,
            "start_with_inverted_time": self.options.start_with_inverted_time.value,
            "receive_filled_wallets": self.options.receive_filled_wallets.value,
            "remains_allow_boss_warps": self.options.remains_allow_boss_warps.value,
            "magic_is_a_trap": self.options.magic_is_a_trap.value,
            "shuffle_regional_maps": self.options.shuffle_regional_maps.value,
            "shuffle_spiderhouse_reward": self.options.shuffle_spiderhouse_reward.value,
            "shuffle_great_fairy_rewards": self.options.shuffle_great_fairy_rewards.value,
            "link_tunic_color": ((self.options.link_tunic_color.value[0] & 0xFF) << 16) | ((self.options.link_tunic_color.value[1] & 0xFF) << 8) | (self.options.link_tunic_color.value[2] & 0xFF),
            "random_seed": self.random.getrandbits(32),
            "dungeon_entrance_rando": self.options.dungeon_entrance_rando.value,
            "boss_entrance_rando": self.options.boss_entrance_rando.value,
            "dungeon_chaining": self.options.dungeon_chaining.value,
            "entrance_rando_results": self.entrance_rando_results,
            "boss_regions": self.boss_regions,
            "fairy_locations": fairy_locations,
            "skull_locations": skull_locations,
            "remain_locations": remain_locations,
            "hints": self.hints,
            "logic_difficulty": self.options.logic_difficulty.value
        }

    def write_spoiler(self, spoiler_handle: TextIO) -> None:
        mw = self.multiworld
        
        # Shopsanity Spoilers
        if self.options.shopsanity.value:
            spoiler_handle.write("\n\nShop Prices:\n")
            for location, shop_id in shop_location_to_id.items():
                spoiler_handle.write(f"\n{location}: {self.shop_prices[shop_id]} Rupees")

        spoiler_handle.write("\n\nIn-Game Hints:\n")
        for text_id, hint in self.hints.items():
            if hint["filled"]:
                spoiler_handle.write(f"\n{hint["name"]}: {mw.get_player_name(hint["to_player"])}'s {hint["item"]} at {mw.get_player_name(hint["from_player"])}'s {hint["location"]}")
            else:
                spoiler_handle.write(f"\n{hint["name"]}: Filler")