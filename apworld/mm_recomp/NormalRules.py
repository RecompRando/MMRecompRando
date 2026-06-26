from .Constants import *

def can_play_song(song, state, player):
    return state.has(song, player) and state.has("Ocarina of Time", player)

def can_get_magic_beans(state, player, options):
    return (
            has_soul_npc(state, player, options, "Bean Daddy") and
            has_soul_absurd(state, player, options, "Grottos") and
            state.has("Deku Mask", player) and 
            state.can_reach("Deku Palace", 'Region', player)
            )

def has_bombchus(state, player):
    return state.has("Progressive Bombchu Bag", player)

def has_explosives(state, player):
    return (state.has("Progressive Bomb Bag", player) or 
            has_bombchus(state, player) or 
            state.has("Blast Mask", player))

def has_hard_projectiles(state, player):
    return (state.has("Progressive Bow", player) or 
            state.has("Zora Mask", player) or 
            state.has("Hookshot", player))

def has_projectiles(state, player):
    return (state.has("Deku Mask", player) and state.has("Progressive Magic", player) or 
            has_hard_projectiles(state, player))

def can_smack_hard(state, player):
    return (state.has("Progressive Sword", player) or 
            state.has("Fierce Deity's Mask", player) or 
            state.has("Great Fairy Sword", player) or 
            state.has("Goron Mask", player) or 
            state.has("Zora Mask", player))

def can_smack(state, player):
    return can_smack_hard(state, player) or state.has("Deku Mask", player)

def can_clear_area(state, player, dungeon):
    if dungeon == DUNGEON_WOODFALL:
        return state.can_reach("Woodfall Temple Odolwa's Remains", "Location", player)
    elif dungeon == DUNGEON_SNOWHEAD:
        return state.can_reach("Snowhead Temple Goht's Remains", "Location", player)
    elif dungeon == DUNGEON_GREAT_BAY:
        return state.can_reach("Great Bay Temple Gyorg's Remains", "Location", player)
    elif dungeon == DUNGEON_STONE_TOWER:
        return state.can_reach("Stone Tower Temple Inverted Twinmold's Remains", "Location", player)

def can_clear_woodfall(state, player, boss_placements):
    return can_clear_area(state, player, boss_placements[DUNGEON_WOODFALL])
    
def can_clear_snowhead(state, player, boss_placements):
    return can_clear_area(state, player, boss_placements[DUNGEON_SNOWHEAD])
    
def can_clear_greatbay(state, player, boss_placements):
    return can_clear_area(state, player, boss_placements[DUNGEON_GREAT_BAY])
    
def can_clear_stonetower(state, player, boss_placements):
    return can_clear_area(state, player, boss_placements[DUNGEON_STONE_TOWER])

def has_notebook(state, player):
    return (state.has("Bomber's Notebook", player))

def has_paper(state, player):
    return (state.has("Land Title Deed", player) or 
            state.has("Swamp Title Deed", player) or 
            state.has("Mountain Title Deed", player) or 
            state.has("Ocean Title Deed", player) or 
            state.has("Letter to Kafei", player) or 
            state.has("Priority Mail", player))

def can_get_cow_milk(state, player, options):
    return (
        has_soul_misc(state, player, options, "Cows") and
        has_soul_absurd(state, player, options, "Grottos") and
        has_bottle(state, player) and 
        can_play_song("Epona's Song", state, player) and 
        (
            has_explosives(state, player) or
            can_use_powder_keg(state, player, options) or 
            state.has("Hookshot", player) or
            (
                has_soul_npc(state, player, options, "Barten") and 
                state.has("Romani's Mask", player)
            ) or
            (
                state.has("Gibdo Mask", player) and 
                has_soul_npc(state, player, options, "Gibdos") and
                has_bottle(state, player) and 
                can_plant_beans(state, player, options) and 
                state.can_reach("Twin Islands Hot Water Grotto Chest", "Location", player) or 
                can_use_light_arrows(state, player) and 
                (
                    state.can_reach("Twin Islands Hot Water Grotto Chest", "Location", player) or 
                    (
                        state.has("Goron Mask", player) and 
                        can_use_lens(state, player)
                    ) or 
                    state.can_reach("Ikana Well Invisible Chest", "Location", player)
                )
            )
        )
    )

def has_bottle(state, player, need_count=1):
    bottle_count = 0
    if state.has("Bottle", player, 2):
        bottle_count += 2
    elif state.has("Bottle", player):
        bottle_count += 1
    if state.has("Bottle of Milk", player):
        bottle_count += 1
    if state.has("Bottle of Chateau Romani", player):
        bottle_count += 1
    if state.has("Bottle of Red Potion", player):
        bottle_count += 1
    return bottle_count >= need_count

def can_get_red_potion(state, player, prices, options):
    return (
            has_bottle(state, player) and
            (
                has_soul_npc(state, player, options, "Koume") or
                (
                    has_soul_npc(state, player, options, "Kotake") and
                    can_purchase(state, player, prices, SHOP_ID_WITCH_POTION_3)
                ) or
                (
                    state.can_reach("Clock Town Trading Post Shop Item 1", "Location", player) or
                    (
                        options.shopsanity.value == 2 and
                        state.can_reach("Clock Town Trading Post Shop (Night) Item 1", "Location", player)
                    )
                )
            )
    )

def can_get_blue_potion(state, player, prices, options):
    return (
            has_bottle(state, player) and
            (
                state.can_reach("Southern Swamp Witch Shop Item 1", "Location", player) or
                state.can_reach("Ikana Canyon Scrub Purchase", "Location", player)
            )
    )

def can_plant_beans(state, player, options):
    return (
        can_get_magic_beans(state, player, options) and
        (
            has_bottle(state, player) or
            can_play_song("Song of Storms", state, player)
        )
    )

def can_use_powder_keg(state, player, options):
    return (
        state.has("Goron Mask", player) and
        state.has("Powder Keg", player) and
        (
            has_soul_npc(state, player, options, "Keg Selling Goron") or
            (
                has_soul_npc(state, player, options, "Gatekeeper & Medigoron") and
                can_use_fire_arrows(state, player) and
                can_afford_price(state, player, 100)
            )
        )
    )

def can_use_magic_arrow(item, state, player):
    return (state.has(item, player) and 
            state.has("Progressive Bow", player) and 
            state.has("Progressive Magic", player))

def can_use_fire_arrows(state, player):
    return can_use_magic_arrow("Fire Arrow", state, player)

def can_use_ice_arrows(state, player):
    return can_use_magic_arrow("Ice Arrow", state, player)

def can_use_light_arrows(state, player):
    return can_use_magic_arrow("Light Arrow", state, player)

def has_gilded_sword(state, player):
    return state.has("Progressive Sword", player, 3)

def has_mirror_shield(state, player):
    return state.has("Progressive Shield", player, 2)

def can_use_lens(state, player):
    return state.has("Lens of Truth", player) and state.has("Progressive Magic", player)

def can_bring_to_player(state, player):
    return state.has("Hookshot", player) or state.has("Zora Mask", player)

def can_break_balls(state, player):
    return (
            state.has("Goron Mask", player) or
            can_use_fire_arrows(state, player) or
            has_explosives(state, player)
    )

def can_reach_scarecrow(state, player, options):
    return (
        has_soul_npc(state, player, options, "Scarecrow") and
        state.has("Ocarina of Time", player) and
        (
            state.can_reach("Astral Observatory", "Region", player) or
            state.can_reach("Trading Post", "Region", player)
        )
    )

def can_reach_seahorse(state, player, options):
    return (
        has_soul_npc(state, player, options, "Fisherman") and
        has_soul_enemy(state, player, options, "Pirate Guards") and
        state.can_reach("Fisherman's House", 'Region', player) and 
        state.has("Zora Mask", player) and 
        state.has("Pictograph Box", player) and 
        (
            state.has("Hookshot", player) or 
            state.has("Goron Mask", player)
        )
    )

def can_afford_price(state, player, price):
    if price > 200:
        return state.has("Progressive Wallet", player, 2)
    elif price > 99:
        return state.has("Progressive Wallet", player)
    return True

def can_purchase(state, player, prices, price_index):
    price = prices[price_index]
    if price > 200:
        return state.has("Progressive Wallet", player, 2)
    elif price > 99:
        return state.has("Progressive Wallet", player)
    return True

def can_get_frog_choir_hp(state, player, options, boss_placements):
    if not state.has("Don Gero's Mask", player):
        return False
    if not can_clear_snowhead(state, player, boss_placements):
        return False
    
    if options.frogsanity.value:
        frogs = ["Yellow Frog", "White Frog", "Cyan Frog", "Blue Frog", "Pink Frog"]
        return all(state.has(frog, player) for frog in frogs)
    else:
        return (
            state.can_reach("Woodfall Temple Gekko Chest", "Location", player) and
            state.can_reach("Great Bay Temple", "Region", player) and
            can_use_ice_arrows(state, player) and
            can_use_fire_arrows(state, player)
        )

def has_enough_remains(state, player, need_count):
    remains_count = 0
    if state.has("Odolwa's Remains", player):
        remains_count += 1
    if state.has("Goht's Remains", player):
        remains_count += 1
    if state.has("Gyorg's Remains", player):
        remains_count += 1
    if state.has("Twinmold's Remains", player):
        remains_count += 1
    return remains_count >= need_count

def has_enough_masks(state, player, need_count, exclude_transformation=False):
    mask_count = 0
    if state.has("Captain's Hat", player):
        mask_count += 1
    if state.has("All-Night Mask", player):
        mask_count += 1
    if state.has("Bunny Hood", player):
        mask_count += 1
    if state.has("Keaton Mask", player):
        mask_count += 1
    if state.has("Garo's Mask", player):
        mask_count += 1
    if state.has("Romani's Mask", player):
        mask_count += 1
    if state.has("Circus Leader's Mask", player):
        mask_count += 1
    if state.has("Postman's Hat", player):
        mask_count += 1
    if state.has("Couple's Mask", player):
        mask_count += 1
    if state.has("Great Fairy's Mask", player):
        mask_count += 1
    if state.has("Gibdo Mask", player):
        mask_count += 1
    if state.has("Don Gero's Mask", player):
        mask_count += 1
    if state.has("Kamaro's Mask", player):
        mask_count += 1
    if state.has("Mask of Truth", player):
        mask_count += 1
    if state.has("Stone Mask", player):
        mask_count += 1
    if state.has("Bremen Mask", player):
        mask_count += 1
    if state.has("Blast Mask", player):
        mask_count += 1
    if state.has("Mask of Scents", player):
        mask_count += 1
    if state.has("Kafei's Mask", player):
        mask_count += 1
    if state.has("Giant's Mask", player):
        mask_count += 1
    # Add transformation masks unless excluded (for Moon Kids)
    if not exclude_transformation:
        if state.has("Deku Mask", player):
            mask_count += 1
        if state.has("Goron Mask", player):
            mask_count += 1
        if state.has("Zora Mask", player):
            mask_count += 1
        if state.has("Fierce Deity's Mask", player):
            mask_count += 1
    return mask_count >= need_count

def has_star_fox(state, player, options, goal_type="majora"):
    required = options.majora_star_fox.value if goal_type == "majora" else options.moon_star_fox.value
    
    if not required:
        return True
    
    return (
        state.has("Keaton Mask", player) and
        state.has("Bremen Mask", player) and
        state.has("Mask of Scents", player) and
        state.has("Don Gero's Mask", player) and
        state.has("Bunny Hood", player)
    )
def has_all_scarecrows(state, player, options, goal_type="majora"):
    required = options.majora_scarecrows_required.value if goal_type == "majora" else options.moon_scarecrows_required.value
    
    if required == 0:
        return True
    
    scarecrow_count = 0
    scarecrows = [
        "Clock Town Trading Post Scarecrow",
        "Astral Observatory Scarecrow"
        "Mountain Village Rooftop Scarecrow",
        "Mountain Village Spring Rooftop Scarecrow",
        "Path to Snowhead Scarecrow",
        "Path to Snowhead Spring Scarecrow",
        "Twin Islands Scarecrow",
        "Twin Islands Spring Scarecrow",
        "Snowhead Temple Lower Scarecrow",
        "Snowhead Temple Hidden Alcove Scarecrow",
        "Great Bay Coast Rock Wall Scarecrow",
        "Zora Cape Beavers Scarecrow",
        "Zora Cape Island Scarecrow",
        "Zora Hall Pervert Scarecrow",
        "Road to Ikana Scarecrow",
        "Stone Tower Lower Scarecrow",
        "Stone Tower Upper Scarecrow"
    ]
    
    for scarecrow in scarecrows:
        if state.has(scarecrow, player):
            scarecrow_count += 1
    
    return scarecrow_count >= required

def has_all_owls(state, player, options, goal_type="majora"):
    required = options.majora_owls_required.value if goal_type == "majora" else options.moon_owls_required.value
    
    if required == 0:
        return True
    
    owl_count = 0
    owls = [
        "Clock Town Owl Statue",
        "Milk Road Owl Statue",
        "Southern Swamp Owl Statue",
        "Woodfall Owl Statue",
        "Mountain Village Owl Statue",
        "Snowhead Owl Statue",
        "Great Bay Coast Owl Statue",
        "Zora Cape Owl Statue",
        "Ikana Canyon Owl Statue",
        "Stone Tower Owl Statue"
    ]
    
    for owl in owls:
        if state.has(owl, player):
            owl_count += 1
    
    return owl_count >= required

def has_all_frogs(state, player, options, goal_type="majora"):
    required = options.majora_frogs_required.value if goal_type == "majora" else options.moon_frogs_required.value
    
    if required == 0:
        return True
    
    frog_count = 0
    frogs = [
        "Yellow Frog",
        "White Frog",
        "Cyan Frog",
        "Blue Frog",
        "Pink Frog"
    ]
    
    for frog in frogs:
        if state.has(frog, player):
            frog_count += 1
    
    return frog_count >= required

def can_use_owl(state, player, options, owl_region):
    if not options.owlsanity.value:
        return False  
    return (
        state.has(owl_region + " Owl Statue", player) and 
        can_play_song("Song of Soaring", state, player)
    )

def can_warp_out(state, player, options):
    return (
        (
            can_play_song("Song of Soaring", state, player) 
        ) and 
        (
            can_use_owl(state, player, options, "Clock Town") or
            can_use_owl(state, player, options, "Milk Road") or
            can_use_owl(state, player, options, "Southern Swamp") or
            can_use_owl(state, player, options, "Ikana Canyon") or
            can_use_owl(state, player, options, "Stone Tower")
        )
    )

def has_enough_items(state, player, required_amount):
    if required_amount == 0:
        return True
    
    item_count = 0
    
    # Regular inventory slots 
    inventory_slots = [
        "Ocarina of Time",           
        "Progressive Bow",           
        "Fire Arrow",                
        "Ice Arrow",                 
        "Light Arrow",               
        "Progressive Bomb Bag",      
        "Progressive Bombchu Bag",   
        "Magic Bean",                
        "Powder Keg",                
        "Pictograph Box",            
        "Lens of Truth",             
        "Hookshot",                  
        "Great Fairy Sword",         
        "Progressive Shield",        
        # Note: Deku Stick and Deku Nut are consumables, not permanent inventory items
    ]
    
    # Count filled bottle slots (up to 6 total)
    item_count += min(state.count("Bottle", player), 3)  # 3 empty bottles
    if state.has("Bottle of Milk", player):
        item_count += 1
    if state.has("Bottle of Chateau Romani", player):
        item_count += 1
    if state.has("Bottle of Red Potion", player):
        item_count += 1
    
    # Trade items (9 total)
    trade_items = [
        "Room Key",
        "Priority Mail",
        "Letter to Kafei",
        "Pendant of Memories",
        "Moon's Tear",
        "Land Title Deed",
        "Swamp Title Deed",
        "Mountain Title Deed",
        "Ocean Title Deed",
    ]
    
    for item in inventory_slots:
        if state.has(item, player):
            item_count += 1
    
    for item in trade_items:
        if state.has(item, player):
            item_count += 1
    
    return item_count >= required_amount

def has_soul_boss(state, player, options, soul_name):
    if not options.boss_souls.value:
        return True
    if options.boss_souls.value != 2 and soul_name == "Majora":
        return True
    return state.has("Soul of " + soul_name, player)

def has_soul_misc(state, player, options, soul_name):
    if not options.misc_souls.value:
        return True
    return state.has("Soul of " + soul_name, player)

def has_soul_npc(state, player, options, soul_name):
    if not options.npc_souls.value:
        return True
    return state.has("Soul of " + soul_name, player)

def has_soul_absurd(state, player, options, soul_name):
    if not options.absurd_souls.value:
        return True
    return state.has("Soul of " + soul_name, player)

def has_soul_utility(state, player, options, soul_name):
    if not options.utility_souls.value:
        return True
    return state.has("Soul of " + soul_name, player)

def has_soul_enemy(state, player, options, soul_name):
    if not options.enemy_souls.value:
        return True
    return state.has("Soul of " + soul_name, player)

def has_all_trade_items(state, player):
    return (
        state.has("Room Key", player) and
        state.has("Priority Mail", player) and
        state.has("Letter to Kafei", player) and
        state.has("Pendant of Memories", player) and
        state.has("Moon's Tear", player) and
        state.has("Land Title Deed", player) and
        state.has("Swamp Title Deed", player) and
        state.has("Mountain Title Deed", player) and
        state.has("Ocean Title Deed", player)
    )

def get_region_rules(player, options):
    return {
        "Clock Town -> The Moon":
            lambda state: (
                state.has("Ocarina of Time", player) and 
                state.has("Oath to Order", player) and
                # Goal Requirements
                has_enough_remains(state, player, options.moon_remains_required.value) and
                has_enough_masks(state, player, options.moon_masks_required.value) and
                has_enough_items(state, player, options.moon_items_required.value) and
                (not options.moon_star_fox.value or has_star_fox(state, player, options, "moon")) and
                has_all_frogs(state, player, options, "moon") and
                has_all_scarecrows(state, player, options, "moon") and
                has_all_owls(state, player, options, "moon")
            ),
        "Southern Swamp -> Southern Swamp (Deku Palace)":
            lambda state: (
                has_soul_npc(state, player, options, "Koume") and
                state.has("Bottle of Red Potion", player) or 
                (
                    has_hard_projectiles(state, player) and 
                    state.has("Deku Mask", player)
                ) or 
                (
                    has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                    state.has("Pictograph Box", player) and 
                    state.has("Deku Mask", player)
                )
            ),
        "Southern Swamp (Deku Palace) -> Swamp Spider House":
            lambda state: state.has("Deku Mask", player),
        "Southern Swamp (Deku Palace) -> Deku Palace":
            lambda state: state.has("Deku Mask", player),
        "Southern Swamp (Deku Palace) -> Woodfall":
            lambda state: (
                (
                    state.has("Deku Mask", player) and
                    has_soul_absurd(state, player, options, "Deku Flowers")
                ) 
                or ( 
                    can_use_owl(state, player, options, "Woodfall")
                   )
            ),
        "Woodfall -> Southern Swamp (Deku Palace)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall -> Woodfall Temple":
            lambda state: (
                state.has("Deku Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_play_song("Sonata of Awakening", state, player)
            ),
        "Woodfall Temple -> Odolwa's Lair":
            lambda state: (
                can_smack(state, player) and 
                state.has("Progressive Bow", player) and 
                (
                    state.has("Boss Key (Woodfall)", player) or 
                    (
                        state.has("Odolwa's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Termina Field -> Path to Mountain Village":
            lambda state: (
                state.has("Progressive Bow", player) or
                can_use_owl(state, player, options, "Mountain Village")
            ),
        "Path to Mountain Village -> Mountain Village":
            lambda state: (
                state.has("Goron Mask", player) or 
                has_explosives(state, player) or 
                can_use_fire_arrows(state, player) or
                can_use_owl(state, player, options, "Mountain Village")
            ),
        "Mountain Village -> Termina Field":
            lambda state: (
                state.has("Progressive Bow", player) or
                    options.owlsanity.value and
                    can_warp_out(state, player, options)
            ),
        "Twin Islands -> Goron Village":
            lambda state: True,
        "Goron Village -> Goron Shrine":
            lambda state: (
                state.has("Goron Mask", player) or
                has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
            ),
        "Twin Islands -> Goron Racetrack":
            lambda state: (
                state.has("Goron Mask", player) and 
                (state.has("Powder Keg", player) or
                can_use_fire_arrows(state, player))
            ),
        "Path to Snowhead -> Snowhead":
            lambda state: (
                (
                    state.has("Goron Mask", player) and 
                    can_play_song("Goron Lullaby", state, player) and 
                    state.has("Progressive Magic", player)
                ) or
                can_use_owl(state, player, options, "Snowhead") and
                can_warp_out(state, player, options)
            ),
        "Path to Snowhead -> Mountain Village":
            lambda state: (
                (
                    state.has("Goron Mask", player) and 
                    state.has("Progressive Magic", player)
                ) or
                can_use_owl(state, player, options, "Mountain Village") and
                can_warp_out(state, player, options)
            ),
        "Snowhead -> Snowhead Temple": lambda state: (
                state.has("Goron Mask", player) and 
                can_play_song("Goron Lullaby", state, player) and 
                state.has("Progressive Magic", player)
            ),
        "Snowhead Temple -> Goht's Lair": lambda state: (
                    can_use_fire_arrows(state, player) and 
                    state.has("Small Key (Snowhead)", player, 1) and 
                    state.has("Boss Key (Snowhead)", player)
            ),
        "Termina Field -> Milk Road":
            lambda state: True,
        "Milk Road -> Gorman Brothers Track":
            lambda state: True,
        "Gorman Brothers Track -> Inside Gorman Brothers Track":
            lambda state: (
                (
                    state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                    can_play_song("Song of Time", state, player) and 
                    can_warp_out(state, player, options)
                ) or
                (
                    options.absurd_souls.value and
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Termina Field -> Great Bay":
            lambda state: 
                (
                    can_play_song("Epona's Song", state, player) or
                    can_warp_out(state, player, options) 
                and (
                        options.owlsanity.value 
                    and (
                            can_use_owl(state, player, options, "Great Bay Coast") or
                            can_use_owl(state, player, options, "Zora Cape")
                        )
                    )
                ),
        "Great Bay -> Termina Field":
            lambda state:
                (
                    can_play_song("Epona's Song", state, player) or
                    (
                        options.owlsanity.value and
                        can_warp_out(state, player, options)
                    )
                ),
        "Great Bay -> Ocean Spider House":
            lambda state: True,
        "Great Bay -> Pirates' Fortress":
            lambda state: state.has("Zora Mask", player),
        "Pirates' Fortress -> Pirates' Fortress (Interior)":
            lambda state: state.has("Hookshot", player),
        "Pirates' Fortress -> Pirates' Fortress Sewers":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and 
                state.has("Goron Mask", player)
            ),
        "Pirates' Fortress Sewers -> Pirates' Fortress (Interior)":
            lambda state: state.has("Zora Mask", player),
        "Great Bay -> Zora Cape":
            lambda state: True,
        "Zora Cape -> Zora Hall":
            lambda state: (
                state.has("Zora Mask", player) or
                        options.owlsanity.value and
                        can_warp_out(state, player, options)
            ),
        "Zora Cape -> Great Bay Temple":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and 
                can_play_song("New Wave Bossa Nova", state, player) and 
                state.has("Hookshot", player) and 
                state.has("Zora Mask", player)
            ),
        "Great Bay Temple -> Gyorg's Lair":
            lambda state: (
                can_play_song("New Wave Bossa Nova", state, player) and 
                state.has("Hookshot", player) and 
                state.has("Zora Mask", player) and
                    (
                        state.can_reach("Great Bay Temple Pre-Boss Room Platform Bubble SF", "Location", player) and 
                        state.has("Boss Key (Great Bay)", player)
                    ) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
            ),
        "Road to Ikana -> Termina Field":
            lambda state: (
                can_play_song("Epona's Song", state, player) or
                (
                    options.owlsanity.value and
                    (
                        can_use_owl(state, player, options, "Ikana Canyon") or
                        can_use_owl(state, player, options, "Stone Tower")
                    )
                )
            ),
        "Road to Ikana -> Ikana Graveyard":
            lambda state: (
                can_play_song("Epona's Song", state, player) or
                can_use_owl(state, player, options, "Ikana Canyon")
            ),
        "Road to Ikana -> Lower Ikana Canyon":
            lambda state:
                (
                    (
                        has_soul_npc(state, player, options, "Spirit House Owner") and
                        can_play_song("Epona's Song", state, player) and 
                        state.has("Hookshot", player) and
                        (
                            state.has("Garo's Mask", player) or
                            state.has("Gibdo Mask", player)
                        )
                    ) or
                    can_use_owl(state, player, options, "Ikana Canyon")
                ),
        "Lower Ikana Canyon -> Secret Shrine":
            lambda state: True,
        "Lower Ikana Canyon -> Upper Ikana Canyon":
            lambda state:
                (
                    (
                        has_soul_enemy(state, player, options, "Ocotoroks") and
                        can_use_ice_arrows(state, player) and 
                        state.has("Hookshot", player)
                    ) or
                        can_use_owl(state, player, options, "Ikana Canyon") or
                        can_use_owl(state, player, options, "Stone Tower")
                ),
        "Upper Ikana Canyon -> Beneath the Well":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player)
            ),
        "Upper Ikana Canyon -> Ikana Castle":
            lambda state: 
            (
                    can_use_light_arrows(state, player) or 
                    has_mirror_shield(state, player)
            ),
        "Beneath the Well -> Ikana Castle":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                can_plant_beans(state, player, options) and
                state.has("Progressive Bow", player) and
                (
                    state.has("Progressive Bomb Bag", player) or 
                    (
                        state.has("Captain's Hat", player) and 
                        has_soul_npc(state, player, options, "Stalchildren")
                    )
                ) and
                (
                    can_use_light_arrows(state, player) or 
                    has_mirror_shield(state, player)
                )
            ),
        "Ikana Castle -> Beneath the Well":
            lambda state: 
                can_use_light_arrows(state, player),
        "Stone Tower -> Upper Ikana Canyon":
            lambda state: 
                can_use_owl(state, player, options, "Stone Tower"),
        "Stone Tower -> Stone Tower Temple":
            lambda state:
                (  
                    can_play_song("Elegy of Emptiness", state, player) and 
                    (
                        (
                            state.has("Hookshot", player) and
                            state.has("Goron Mask", player) and 
                            state.has("Zora Mask", player)
                        ) or
                        (
                            can_use_owl(state, player, options, "Stone Tower")
                        )
                    )
                ),
        "Stone Tower -> Stone Tower (Inverted)":
            lambda state: (
                state.can_reach("Stone Tower Temple", 'Region', player) and 
                can_use_light_arrows(state, player)
            ),
        "Stone Tower (Inverted) -> Stone Tower Temple (Inverted)":
            lambda state: True,    
        "Stone Tower Temple (Inverted) -> Twinmold's Lair":
            lambda state: (
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player) and 
                (
                    state.has("Progressive Bow", player) or 
                    state.has("Fierce Deity's Mask", player) or 
                    (
                        state.has("Giant's Mask", player) and 
                        state.has("Progressive Magic", player) and 
                        state.has("Progressive Sword", player)
                    )
                ) and 
                (
                    state.has("Boss Key (Stone Tower)", player) or 
                    (
                        state.has("Twinmold's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
                
            ),
    }

def get_location_rules(player, options, prices, boss_placements):
    return {
        "Link's Inventory (Kokiri Sword)":
            lambda state: True,
        "Link's Inventory (Hero's Shield)":
            lambda state: True,  
        "Link's Inventory (Heart Item #1)":
            lambda state: True,    
        "Link's Inventory (Heart Item #2)":
            lambda state: True,
        "Link's Inventory (Heart Item #3)":
            lambda state: True,
        "Link's Inventory (Heart Item #4)":
            lambda state: True,
        "Link's Inventory (Heart Item #5)":
            lambda state: True,    
        "Link's Inventory (Heart Item #6)":
            lambda state: True,
        "Link's Inventory (Heart Item #7)":
            lambda state: True,
        "Link's Inventory (Heart Item #8)":
            lambda state: True,
        
        "Keaton Quiz":
            lambda state: (
                state.has("Keaton Mask", player) and 
                has_soul_misc(state, player, options, "Keaton")
            ),
        "Clock Town Postbox":
            lambda state: (
                has_soul_utility(state, player, options, "Postboxes") and
                state.has("Postman's Hat", player)
            ),
        "Top of Clock Tower (Ocarina of Time)":
            lambda state: (
                has_soul_npc(state, player, options, "Skull Kid") and
                has_projectiles(state, player)
            ),
        "Top of Clock Tower (Song of Time)":
            lambda state: (
                has_soul_npc(state, player, options, "Skull Kid") and
                has_projectiles(state, player)
            ),
         "North Clock Town Tree HP":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "North Clock Town Deku Playground Any Day":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground All Days":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Save Old Lady":
            lambda state: (
                has_soul_npc(state, player, options, "Bomb Granny") and
                has_soul_npc(state, player, options, "Sakon") and
                (state.has("Progressive Sword", player) or 
                state.has("Great Fairy Sword", player) or 
                state.has("Zora Mask", player) or 
                state.has("Goron Mask", player))
            ),
        "North Clock Town Great Fairy Reward (Has Transformation Mask)":
            lambda state: (
                has_soul_npc(state, player, options, "Great Fairies") and
                state.has("Stray Fairy (Clock Town)", player) and 
                (
                    state.has("Deku Mask", player) or 
                    state.has("Goron Mask", player) or 
                    state.has("Zora Mask", player)
                )
            ),
        "North Clock Town Great Fairy Reward":
            lambda state: (
                has_soul_npc(state, player, options, "Great Fairies") and
                state.has("Stray Fairy (Clock Town)", player)
            ),
        "Clock Town Hide-and-Seek":
            lambda state: (
                has_soul_npc(state, player, options, "Bomber Kids") and
                has_projectiles(state, player)
            ),
        "Tingle Clock Town Map Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Tingle") and
                has_projectiles(state, player)
            ),
        
        "South Clock Town Clock Tower Freestanding HP":
            lambda state: True,        
        "South Clock Town Moon's Tear Trade":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Moon's Tear", player)
            ),
        "South Clock Town Corner Chest":
            lambda state: state.has("Hookshot", player),
        "South Clock Town Final Day Tower Chest":
            lambda state: (
                state.has("Hookshot", player) or 
                (
                    state.has("Deku Mask", player) and 
                    state.has("Moon's Tear", player)
                )
            ),
        "Clock Tower Happy Mask Salesman #1":
            lambda state: (
                has_soul_npc(state, player, options, "Happy Mask Salesman") and
                state.has("Ocarina of Time", player)
            ),
        "Clock Tower Happy Mask Salesman #2":
            lambda state: (
                has_soul_npc(state, player, options, "Happy Mask Salesman") and
                state.has("Ocarina of Time", player)
            ),
        "Before Clock Town Chest":
            lambda state: (
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player)
            ),
                
        "East Clock Town Archery Roof Chest":
            lambda state: True,
        "East Clock Town Mayors Wife":
            lambda state: has_soul_npc(state, player, options, "Madame Aroma"),
        "East Clock Town Couples Mask on Mayor":
            lambda state: (
                has_soul_npc(state, player, options, "Council") and 
                state.has("Couple's Mask", player)
            ),
        "East Clock Town Shooting Gallery 40-49 Points":
            lambda state: (
                has_soul_npc(state, player, options, "Archery Man") and 
                state.has("Progressive Bow", player)
            ),
        "East Clock Town Shooting Gallery Perfect 50 Points":
            lambda state: (
                has_soul_npc(state, player, options, "Archery Man") and 
                state.has("Progressive Bow", player)
            ),
        "East Clock Town Honey and Darling Any Day":
            lambda state: (
                has_soul_npc(state, player, options, "Honey and Darling") and
                (
                    state.has("Progressive Bow", player) or
                    state.has("Progressive Bomb Bag", player) or
                    has_bombchus(state, player) or
                    (
                        state.has("Deku Mask", player) and
                        state.has("Progressive Magic", player)
                    )
                )
            ),
        "East Clock Town Honey and Darling All Days":
            lambda state: (
                has_soul_npc(state, player, options, "Honey and Darling") and
                state.has("Progressive Bow", player) and 
                state.has("Progressive Bomb Bag", player) and 
                has_bombchus(state, player)
            ),
        "East Clock Town Treasure Game Chest (Human)":
            lambda state: has_soul_npc(state, player, options, "Treasure Game Lady"),
        "East Clock Town Treasure Game Chest (Deku)":
            lambda state: (
                has_soul_npc(state, player, options, "Treasure Game Lady") and
                state.has("Deku Mask", player)
            ),
        "East Clock Town Treasure Game Chest (Goron)":
            lambda state: (
                has_soul_npc(state, player, options, "Treasure Game Lady") and
                state.has("Goron Mask", player)
            ),
        "East Clock Town Treasure Game Chest (Zora)":
            lambda state: (
                has_soul_npc(state, player, options, "Treasure Game Lady") and
                state.has("Zora Mask", player)
            ),
        "Bomber's Hideout Chest":
            lambda state: (
                state.can_reach("Clock Town Hide-and-Seek", "Location", player) and 
                has_explosives(state, player)
            ),
        "Bomber's Hideout Astral Observatory":
            lambda state: (
                has_soul_npc(state, player, options, "Astral Observatory Man") and
                has_soul_npc(state, player, options,"Skull Kid") and
                (
                    has_projectiles(state, player) 
                or
                    (state.has("Deku Mask", player) and 
                    has_soul_absurd(state, player, options, "Deku Flowers"))
                )
            ),
        "Milk Bar Show":
            lambda state: (
                has_soul_npc(state, player, options, "Toto & Gorman") and
                state.has("Romani's Mask", player) and 
                state.has("Deku Mask", player) and 
                state.has("Goron Mask", player) and 
                state.has("Zora Mask", player) and 
                state.has("Ocarina of Time", player)
            ),
        "Milk Bar Priority Mail to Aroma":
            lambda state: (
                has_soul_npc(state, player, options, "Madame Aroma") and
                state.has("Romani's Mask", player) and 
                state.has("Kafei's Mask", player) and 
                state.has("Priority Mail", player)
            ),
        "East Clock Town Milk Bar Milk Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Barten") and
                state.has("Romani's Mask", player) and 
                can_afford_price(state, player, 40)
            ),
        "East Clock Town Milk Bar Chateau Romani Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Barten") and
                state.has("Romani's Mask", player) and 
                can_afford_price(state, player, 200)
            ),
        "West Clock Town Lottery Any Day":
            lambda state: has_soul_npc(state, player, options, "Lottery"),       
        "West Clock Town Swordsman Expert Course":
            lambda state: (
                has_soul_npc(state, player, options, "Swordsman") and
                state.has("Progressive Sword", player)
            ),
        "West Clock Town Postman Counting":
            lambda state: (
                has_soul_npc(state, player, options, "Postman") and
                state.has("Bunny Hood", player)
            ),
        "West Clock Town Dancing Sisters":
            lambda state: (
                has_soul_npc(state, player, options, "Rosa Sisters") and 
                state.has("Kamaro's Mask", player)
            ),
        "West Clock Town Bank 200 Rupees":
            lambda state: has_soul_npc(state, player, options, "Banker"),
        "West Clock Town Bank 500 Rupees":
            lambda state: (
                has_soul_npc(state, player, options, "Banker") and 
                state.has("Progressive Wallet", player)
            ),
        "West Clock Town Bank 1000 Rupees":
            lambda state: (
                has_soul_npc(state, player, options, "Banker") and
                state.has("Progressive Wallet", player, 2)
            ),
        "West Clock Town Priority Mail to Postman":
            lambda state: (
                has_soul_npc(state, player, options, "Postman") and
                state.has("Priority Mail", player)
            ),
        "Clock Town Trading Post Shop Item 1":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_1)
            ),
        "Clock Town Trading Post Shop Item 2":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_2)
            ),
        "Clock Town Trading Post Shop Item 3":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_3)
            ),
        "Clock Town Trading Post Shop Item 4":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_4)
            ),
        "Clock Town Trading Post Shop Item 5":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_5)
            ),
        "Clock Town Trading Post Shop Item 6":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_6)
            ),
        "Clock Town Trading Post Shop Item 7":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_7)
            ),
        "Clock Town Trading Post Shop Item 8":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_8)
            ),
        "Clock Town Trading Post Shop (Night) Item 1":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_NIGHT_1)
            ),
        "Clock Town Trading Post Shop (Night) Item 2":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_NIGHT_2)
            ),
        "Clock Town Trading Post Shop (Night) Item 3":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_NIGHT_3)
            ),
        "Clock Town Trading Post Shop (Night) Item 4":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_NIGHT_4)
            ),
        "Clock Town Trading Post Shop (Night) Item 5":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_NIGHT_5)
            ),
        "Clock Town Trading Post Shop (Night) Item 6":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_NIGHT_6)
            ),
        "Clock Town Trading Post Shop (Night) Item 7":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_NIGHT_7)
            ),
        "Clock Town Trading Post Shop (Night) Item 8":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_TRADING_POST_NIGHT_8)
            ),
        "Clock Town Bomb Shop Item 1":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_BOMB_SHOP_1)
            ),
        "Clock Town Bomb Shop Item 2":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_BOMB_SHOP_2)
            ),
        "Clock Town Bomb Shop Item 3":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                can_purchase(state, player, prices, SHOP_ID_BOMB_SHOP_3)
            ),
        "Clock Town Bomb Shop Powder Keg Goron":
            lambda state: (
                has_soul_npc(state, player, options, "Keg Selling Goron") and
                state.has("Goron Mask", player) and state.has("Powder Keg", player)
            ),
        "Clock Town Bomb Shop Item 3 (Stop Thief)":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                state.can_reach("North Clock Town Save Old Lady", "Location", player) and 
                can_purchase(state, player, prices, SHOP_ID_BOMB_SHOP_3_UPGRADE)
            ),
        "Curiosity Shop Blue Rupee Trade":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                has_bottle(state, player) and 
                (
                    state.has("Mask of Scents", player) or 
                    can_get_cow_milk(state, player, options)
                )
            ),
        "Curiosity Shop Red Rupee Trade":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                has_bottle(state, player)
            ),
        "Curiosity Shop Purple Rupee Trade":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                has_bottle(state, player) and 
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player)
            ),
        "Curiosity Shop Gold Rupee Trade":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                has_bottle(state, player) and 
                (
                    (
                        state.can_reach("Graveyard Day 3 Dampe Big Poe Chest", "Location", player) or 
                        (
                            state.can_reach("Ikana Well Rightside Torch Chest", "Location", player) and 
                            state.has("Progressive Bomb Bag", player)
                        )
                    ) or 
                    (
                        state.has("Romani's Mask", player) and 
                        can_afford_price(state, player, 200)
                    ) or 
                    state.can_reach("Goron Racetrack Prize", "Location", player)
                )
            ),
        "Curiosity Shop Night 3 (Stop Thief)":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                can_purchase(state, player, prices, SHOP_ID_CURIOSITY_SHOP_MASK) and
                state.can_reach("North Clock Town Save Old Lady", "Location", player)
            ),
        "Curiosity Shop Night 3 Thief Stolen Item":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                can_purchase(state, player, prices, SHOP_ID_CURIOSITY_SHOP_BOMB_BAG)
            ),
        "Stock Pot Inn Reservation":
            lambda state: has_soul_npc(state, player, options, "Anju"),        
        "Stock Pot Inn Midnight Meeting":
            lambda state: (
                has_soul_npc(state, player, options, "Anju") and
                state.has("Kafei's Mask", player) and 
                (
                    state.has("Deku Mask", player) or 
                    state.has("Room Key", player)
                )
            ),
        "Stock Pot Inn Employee Room Chest":
            lambda state: True,    
        "Stock Pot Inn Locked Room Chest":
            lambda state: state.has("Room Key", player),
        "Stock Pot Inn Midnight Toilet Hand":
            lambda state: (
                has_soul_npc(state, player, options, "Toilet Hand") and
                has_paper(state, player)
            ),
        "Stock Pot Inn Granny Story #1":
            lambda state: (
                has_soul_npc(state, player, options, "Anju's Grandmother") and
                state.has("All-Night Mask", player)
            ),
        "Stock Pot Inn Granny Story #2":
            lambda state: (
                has_soul_npc(state, player, options, "Anju's Grandmother") and
                state.has("All-Night Mask", player)
            ),
        "Stock Pot Inn Anju and Kafei":
            lambda state: (
                has_soul_utility(state, player, options, "Postboxes") and
                has_soul_npc(state, player, options, "Anju") and
                has_soul_npc(state, player, options, "Kafei") and
                state.has("Kafei's Mask", player) and 
                can_play_song("Epona's Song", state, player) and 
                state.has("Letter to Kafei", player) and 
                state.has("Pendant of Memories", player) and 
                state.has("Hookshot", player) and 
                (
                    state.has("Garo's Mask", player) or 
                    state.has("Gibdo Mask", player)
                )
            ),
        
        "Laundry Pool Kafei's Request":
            lambda state: (
                has_soul_utility(state, player, options, "Postboxes") and
                has_soul_npc(state, player, options, "Kafei") and
                state.has("Letter to Kafei", player)
            ),
        "Laundry Pool Curiosity Shop Salesman #1":
            lambda state: (
                has_soul_utility(state, player, options, "Postboxes") and
                has_soul_npc(state, player, options, "Kafei") and
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                state.has("Letter to Kafei", player)
            ),
        "Laundry Pool Curiosity Shop Salesman #2":
            lambda state: (
                has_soul_utility(state, player, options, "Postboxes") and
                has_soul_npc(state, player, options, "Kafei") and
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                state.has("Letter to Kafei", player)
            ),
        "Laundry Pool Musician":
            lambda state: has_soul_npc(state, player, options, "Guru-Guru"),
        "Laundry Pool Stray Fairy (Clock Town)":
            lambda state: True,        

        "Termina Ikana Pillar Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos")
            ),      
        "Termina Grass Chest":
            lambda state: True,
        "Termina Grass Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos")
            ),   
        "Termina Stump Chest":
            lambda state: (
                state.has("Hookshot", player) or 
                can_plant_beans(state, player, options)
            ),
        "Termina Underwater Chest":
            lambda state: state.has("Zora Mask", player),
        "Termina Peehat Grotto Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Peehats") and
                has_soul_absurd(state, player, options, "Grottos")
            ),   
        "Termina Dodongo Grotto Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Dodongos") and
                has_soul_absurd(state, player, options, "Grottos")
            ),   
        "Termina Bio Baba Grotto HP":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                (
                    has_explosives(state, player) or 
                    state.has("Goron Mask", player)
                ) and 
                state.has("Zora Mask", player)
            ),
        "Termina Healing Kamaro":
            lambda state: (
                has_soul_npc(state, player, options, "Kamaro") and
                state.has("Ocarina of Time", player) and 
                state.has("Song of Healing", player)
            ),
        "Termina Gossip Stones HP":
            lambda state: (
                (
                    has_soul_absurd(state, player, options, "Grottos") and
                    has_explosives(state, player) or 
                    state.has("Goron Mask", player)
                ) and 
                (
                    (
                        state.has("Deku Mask", player) and 
                        can_play_song("Sonata of Awakening", state, player)
                    ) or 
                    (
                        state.has("Goron Mask", player) and 
                        can_play_song("Goron Lullaby", state, player)
                    ) or 
                    (
                        state.has("Zora Mask", player) and 
                        can_play_song("New Wave Bossa Nova", state, player)
                    )
                )
            ),
        "Termina Scrub Grotto HP":
            lambda state: (
                # Always required
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Business Scrubs") and
                has_soul_npc(state, player, options, "Astral Observatory Man") and
                state.has("Ocarina of Time", player) and
                can_afford_price(state, player, 100) and
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player)
            ),
        "Termina Log Bombable Grotto Left Cow":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Cows") and
                has_explosives(state, player) and 
                can_play_song("Epona's Song", state, player)
            ),
        "Termina Log Bombable Grotto Right Cow":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Cows") and
                has_explosives(state, player) and 
                can_play_song("Epona's Song", state, player)
            ),

        "Milk Road Gorman Ranch Race":
            lambda state: (
                has_soul_npc(state, player, options, "Gorman Brothers") and
                state.has("Ocarina of Time", player) and 
                state.has("Epona's Song", player)
            ),
        "Milk Road Gorman Ranch Purchase":
            lambda state: has_soul_npc(state, player, options, "Gorman Brothers"),
        "Tingle Romani Ranch Map Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Tingle") and
                has_projectiles(state, player) and 
                (
                    state.can_reach("Milk Road", 'Region', player) or 
                    state.can_reach("Twin Islands", 'Region', player)
                )
            ),
        
        "Road to Swamp Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos")
            ),          
        "Road to Swamp Tree HP":
            lambda state: has_projectiles(state, player),
        "Tingle Woodfall Map Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Tingle") and
                has_projectiles(state, player) and 
                (
                    state.can_reach("Southern Swamp", 'Region', player) or 
                    state.can_reach("Clock Town", 'Region', player)
                )
            ),
        "Swamp Shooting Gallery 2120 Points":
            lambda state: (
                has_soul_npc(state, player, options, "Archery Man") and 
                state.has("Progressive Bow", player)
            ),
        "Swamp Shooting Gallery 2180 Points":
            lambda state: (
                has_soul_npc(state, player, options, "Archery Man") and 
                state.has("Progressive Bow", player)
            ),

        "Southern Swamp Deku Scrub Purchase":
            lambda state: (
                (
                    has_soul_npc(state, player, options, "Business Scrubs") and
                    state.has("Deku Mask", player) and 
                    can_plant_beans(state, player, options)
                ) or 
                (
                    state.has("Land Title Deed", player) and 
                    state.has("Moon's Tear", player) and 
                    can_plant_beans(state, player, options)
                )
            ),
        "Southern Swamp Deku Trade":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Land Title Deed", player)
            ),
        "Southern Swamp Freestanding HP":
            lambda state: (
                state.has("Land Title Deed", player) and 
                state.has("Deku Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Southern Swamp Kotake Item":
            lambda state: (
                has_soul_npc(state, player, options, "Koume")
            ),    
        "Southern Swamp Healing Koume":
            lambda state: (
                has_soul_npc(state, player, options, "Koume") and
                has_bottle(state, player)
            ),     
        "Southern Swamp Winning Picture":
            lambda state: (
                state.has("Pictograph Box", player) and
                has_soul_npc(state, player, options, "Swamp Tourist Guide") and 
                has_soul_npc(state, player, options, "Tingle") 
            ),
        "Southern Swamp Good Picture":
            lambda state: (
                state.has("Pictograph Box", player) and
                has_soul_npc(state, player, options, "Swamp Tourist Guide")
            ),
        "Southern Swamp Okay Picture":
            lambda state: (
                state.has("Pictograph Box", player) and
                has_soul_npc(state, player, options, "Swamp Tourist Guide")
            ),
        "Southern Swamp Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                (
                    state.has("Deku Mask", player) and
                    has_hard_projectiles(state, player)
                ) or
                (
                    has_bottle(state, player) and
                    has_soul_npc(state, player, options, "Kotake")
                ) or
                (
                    state.has("Pictograph Box", player) and
                    has_soul_npc(state, player, options, "Swamp Tourist Guide")
                )
            ),
        "Southern Swamp Song Tablet":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player),
            ),
            
        "Southern Swamp Day 2 Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos")
            ),   
        "Southern Swamp Witch Shop Mushroom Item":
            lambda state: (
                has_soul_npc(state, player, options, "Kotake") and
                state.has("Mask of Scents", player) and 
                has_bottle(state, player)
            ),
        "Southern Swamp Witch Shop Item 1":
            lambda state: (
                has_soul_npc(state, player, options, "Kotake") and
                state.has("Mask of Scents", player) and 
                has_bottle(state, player) and 
                can_purchase(state, player, prices, SHOP_ID_WITCH_POTION_1)
            ),
        "Southern Swamp Witch Shop Item 2":
            lambda state: (
                has_soul_npc(state, player, options, "Kotake") and
                can_purchase(state, player, prices, SHOP_ID_WITCH_POTION_2)
            ),
        "Southern Swamp Witch Shop Item 3":
            lambda state: (
                has_soul_npc(state, player, options, "Kotake") and
                can_purchase(state, player, prices, SHOP_ID_WITCH_POTION_3)
            ),
        "Swamp Spider House First Room Pot Near Entrance Token":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House First Room Crawling In Water Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House First Room Crawling Right Column Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House First Room Crawling Left Column Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House First Room Against Far Wall Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                (
                    (
                        can_bring_to_player(state, player) and
                        has_projectiles(state, player)
                    ) or
                    (
                        has_soul_absurd(state, player, options, "Deku Flowers") and
                        state.has("Deku Mask", player) and
                        (
                            state.has("Progressive Magic", player) or
                            state.has("Progressive Bow", player)
                        )
                    )
                )
            ),
        "Swamp Spider House First Room Lower Left Bugpatch Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player) and 
                has_bottle(state, player)
            ),
        "Swamp Spider House First Room Lower Right Bugpatch Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player) and 
                has_bottle(state, player)
            ),
        "Swamp Spider House First Room Upper Right Bugpatch Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player) and 
                has_bottle(state, player)
            ),
        "Swamp Spider House Monument Room Left Crate Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Monument Room Right Crate Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Monument Room Crawling Wall Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_bring_to_player(state, player) or 
                (
                    has_soul_misc(state, player, options, "Gold Skulltulas") and
                    can_smack(state, player) and 
                    can_plant_beans(state, player, options) and 
                    (
                        has_explosives(state, player) or 
                        state.has("Goron Mask", player)
                    )
                )
            ),
        "Swamp Spider House Monument Room Crawling On Monument Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Monument Room Behind Torch Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Pottery Room Beehive #1 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player) and 
                has_projectiles(state, player)
            ),
        "Swamp Spider House Pottery Room Beehive #2 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player) and 
                has_projectiles(state, player)
            ),
        "Swamp Spider House Pottery Room Small Pot Token":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Pottery Room Left Large Pot Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Pottery Room Right Large Pot Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Pottery Room Behind Vines Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                (
                    state.has("Progressive Sword", player) or
                    state.has("Great Fairy Sword", player) or
                    state.has("Fierce Deity's Mask", player)
                )
            ),
        "Swamp Spider House Pottery Room Upper Wall Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Golden Room Crawling Left Wall Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Golden Room Crawling Right Column Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Golden Room Against Far Wall Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player) and 
                (
                    (can_bring_to_player(state, player) or 
                    can_plant_beans(state, player, options))
                )
            ),
        "Swamp Spider House Golden Room Beehive Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player) and 
                has_projectiles(state, player)
            ),
        "Swamp Spider House Tree Room Tall Grass #1 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Tree Room Tall Grass #2 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Tree Room Tree #1 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Tree Room Tree #2 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Tree Room Tree #3 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player)
            ),
        "Swamp Spider House Tree Room Beehive Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                can_smack(state, player) and 
                has_projectiles(state, player)
            ),
        "Swamp Spider House Reward":
            lambda state: (
                state.has("Swamp Skulltula Token", player, options.required_skull_tokens.value) and
                has_soul_npc(state, player, options, "Spider Man")
            ), 
        "Deku Palace Bean Seller":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Bean Daddy") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                (
                    can_plant_beans(state, player, options) or
                    state.has("Hookshot", player)
                )
            ),
        "Deku Palace HP":
            lambda state: state.has("Deku Mask", player),    
        "Deku Palace Monkey Song":
            lambda state: (
                has_soul_npc(state, player, options, "Monkey") and
                state.has("Ocarina of Time", player) and 
                can_plant_beans(state, player, options) and 
                state.has("Deku Mask", player)
            ),
        "Deku Palace Butler Race":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_bottle(state, player) and 
                (
                    state.has("Progressive Sword", player) or 
                    state.has("Great Fairy Sword", player) or 
                    state.has("Fierce Deity's Mask", player)
                )
            ),

        "Woodfall Great Fairy Reward":
            lambda state: (
                has_soul_npc(state, player, options, "Great Fairies") and
                state.has("Stray Fairy (Woodfall)", player, options.required_stray_fairies.value)
            ),
        "Woodfall Owl Statue Chest":
            lambda state: (
                (
                    state.has("Deku Mask", player) and
                    has_soul_absurd(state, player, options, "Deku Flowers")
                ) 
                or ( 
                    can_use_owl(state, player, options, "Woodfall") and
                    state.has("Hookshot", player)
                   )
            ),
        "Woodfall Bridge Chest":
            lambda state: (
                (
                    state.has("Deku Mask", player) and
                    has_soul_absurd(state, player, options, "Deku Flowers")
                ) 
                or ( 
                    state.can_reach("Woodfall", 'Region', player) and
                    state.has("Hookshot", player)
                   )
            ),
        "Woodfall Entrance Chest":
            lambda state: state.has("Deku Mask", player),
        
        # Woodfall Temple Chests
        "Woodfall Temple Entrance Chest SF":
            lambda state: True,
        "Woodfall Temple Ledge Chest":
            lambda state: True,
        "Woodfall Temple Turtle Chest":
            lambda state: has_soul_enemy(state, player, options, "Snappers"),     
        "Woodfall Temple Dragonfly Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Dragonflies") and
                (   
                    state.has("Small Key (Woodfall)", player) or 
                    state.has("Progressive Bow", player)
                )
            ),
        "Woodfall Temple Dark Room Chest SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Boes") and
                
                (
                    state.has("Small Key (Woodfall)", player) or 
                    state.has("Progressive Bow", player)
                )
            ),
        "Woodfall Temple Switch Chest SF":
            lambda state: state.has("Progressive Bow", player),
        "Woodfall Temple Dinolfos Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Dinolfos") and
                (
                    (
                        state.has("Small Key (Woodfall)", player) and
                        can_smack(state, player)
                    ) or
                    state.has("Progressive Bow", player)
                )
            ),
        "Woodfall Temple Gekko Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Gekko") and
                state.has("Progressive Bow", player) and 
                can_smack(state, player)
            ),
        "Woodfall Temple Entrance Freestanding SF":
            lambda state: True,
        "Woodfall Temple Deku Baba SF":
            lambda state: has_soul_enemy(state, player, options, "Deku Baba"),
        "Woodfall Temple Pot SF":
            lambda state: has_soul_absurd(state, player, options, "Pots"),           
        "Woodfall Temple Main Room Bubble SF":
            lambda state: (
                (
                    state.has("Progressive Bow", player) and 
                    state.has("Great Fairy's Mask", player)
                ) or 
                can_use_fire_arrows(state, player)
            ),
        "Woodfall Temple Platform Hive SF":
            lambda state: (
                (
                    state.has("Progressive Bow", player) or 
                    (
                        state.has("Deku Mask", player) and 
                        state.has("Progressive Magic", player)
                    )
                ) or 
                (
                    state.has("Great Fairy's Mask", player) and 
                    (
                        state.has("Hookshot", player) or 
                        state.has("Zora Mask", player)
                    )
                )
            ),
        "Woodfall Temple Skulltula SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Hanging Skulltulas") and
                (
                    (
                        state.has("Small Key (Woodfall)", player) and
                        can_smack(state, player)
                    ) or
                    state.has("Progressive Bow", player)
                )
            ),
        "Woodfall Temple Bridge Room Bubble SF":
            lambda state: (
                state.has("Great Fairy's Mask", player) and 
                (
                    (
                        state.has("Small Key (Woodfall)", player) and 
                        has_projectiles(state, player)
                    ) or 
                    state.has("Progressive Bow", player)
                )
            ),
        "Woodfall Temple Bridge Room Hive SF":
            lambda state: (
                state.has("Great Fairy's Mask", player) and 
                (
                    (
                        state.has("Small Key (Woodfall)", player) and 
                        has_projectiles(state, player)
                    ) or 
                    state.has("Progressive Bow", player)
                )
            ),
        "Woodfall Temple Pre-Boss Lower Right Bubble SF":
            lambda state: (
                state.has("Progressive Bow", player) or 
                can_use_fire_arrows(state, player)
            ),
        "Woodfall Temple Pre-Boss Upper Right Bubble SF":
            lambda state: (
                state.has("Progressive Bow", player) or 
                can_use_fire_arrows(state, player)
            ),
        "Woodfall Temple Pre-Boss Upper Left Bubble SF":
            lambda state: (
                state.has("Progressive Bow", player) or 
                can_use_fire_arrows(state, player)
            ),
        "Woodfall Temple Pre-Boss Pillar Bubble SF":
            lambda state: (
                state.has("Progressive Bow", player) or 
                can_use_fire_arrows(state, player)
            ),
        "Woodfall Temple Heart Container":
            lambda state: (
                has_soul_boss(state, player, options, "Odolwa") and
                can_smack(state, player) and 
                state.has("Progressive Bow", player) and 
                (
                    state.has("Boss Key (Woodfall)", player) or 
                    (
                        state.has("Odolwa's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Woodfall Temple Odolwa's Remains":
            lambda state: (
                has_soul_boss(state, player, options, "Odolwa") and
                can_smack(state, player) and 
                state.has("Progressive Bow", player) and 
                (
                    state.has("Boss Key (Woodfall)", player) or 
                    (
                        state.has("Odolwa's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
            
        "Southern Swamp Boat Archery":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_bottle(state, player) and 
                state.has("Progressive Bow", player) and
                has_soul_npc(state, player, options, "Koume")
            ),
        "Mountain Village Healing Darmani":
            lambda state: (
                can_use_lens(state, player) and 
                can_play_song("Song of Healing", state, player)
            ),
        "Mountain Village Hungry Goron":
            lambda state: (
                has_soul_npc(state, player, options, "Hungry Goron") and
                state.has("Goron Mask", player) and 
                state.has("Progressive Magic", player) and 
                (
                    can_play_song("Goron Lullaby", state, player) or 
                    can_use_fire_arrows(state, player)
                )
            ),
        "Mountain Village Spring Waterfall Chest":
            lambda state: can_clear_snowhead(state, player, boss_placements),
        "Mountain Village Spring Ramp Grotto":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Frog Choir HP":
            lambda state: can_get_frog_choir_hp(state, player, options, boss_placements),

        "Mountain Village Smithy Upgrade":
            lambda state: (
                has_soul_npc(state, player, options, "Mountain Smithy") and
                can_afford_price(state, player, 100) and 
                (
                    can_use_fire_arrows(state, player) or 
                    state.can_reach("Twin Islands Hot Water Grotto Chest", "Location", player) or 
                    can_clear_snowhead(state, player, boss_placements)
                )
            ),
        "Mountain Village Smithy Gold Dust Upgrade":
            lambda state: (
                has_soul_npc(state, player, options, "Mountain Smithy") and
                state.can_reach("Mountain Village Smithy Upgrade", "Location", player) and 
                state.can_reach("Goron Racetrack Prize", "Location", player) and 
                has_bottle(state, player)
            ),
            
        "Tingle Snowhead Map Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Tingle") and
                has_projectiles(state, player) and 
                (
                    state.can_reach("Twin Islands", 'Region', player) or 
                    state.can_reach("Southern Swamp", 'Region', player)
                )
            ),
        "Twin Islands Ramp Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_explosives(state, player) 
                and (
                        state.has("Goron Mask", player) 
                    or (
                        state.has("Hookshot", player) 
                        and state.has("Twin Islands Scarecrow", player) 
                        and has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Islands Goron Elder Request":
            lambda state: (
                state.has("Goron Mask", player) and 
                (
                    can_use_fire_arrows(state, player) or 
                    (
                        (
                            can_use_lens(state, player) or 
                            (
                                state.can_reach("Ikana Well Invisible Chest", "Location", player) and 
                                can_play_song("Song of Soaring", state, player) and 
                                options.owlsanity.value and
                                can_use_owl(state, player, options, "Mountain Village")
                            )
                        ) and 
                        has_bottle(state, player)
                    )
                )
            ),
        "Twin Islands Hot Water Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                (
                    has_explosives(state, player) and 
                    can_use_fire_arrows(state, player)
                ) or 
                (
                    can_use_lens(state, player) and 
                    has_bottle(state, player) and 
                    state.has("Goron Mask", player) and 
                    has_explosives(state, player)
                ) or 
                (
                    can_clear_snowhead(state, player, boss_placements) or 
                    (
                        state.can_reach("Ikana Well Invisible Chest", "Location", player) and 
                        can_play_song("Song of Soaring", state, player) and
                        options.owlsanity.value and
                        can_use_owl(state, player, options, "Mountain Village")
                    )
                )
            ),
        "Twin Islands Spring Underwater Cave Chest":
            lambda state: (
                state.has("Zora Mask", player) and 
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Spring Underwater Ramp Chest":
            lambda state: (
                state.has("Zora Mask", player) and 
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Goron Racetrack Prize":
            lambda state: (
                (
                    can_use_powder_keg(state, player, options) or 
                    state.can_reach("Powder Keg Goron Reward", "Location", player)
                ) and 
                can_clear_snowhead(state, player, boss_placements)
            ),
            
        "Goron Village Lens Cave Rock Chest":
            lambda state: has_explosives(state, player),
        "Goron Village Lens Cave Invisible Chest":
            lambda state: True,
        "Goron Village Lens Cave Center Chest":
            lambda state: True,
        "Goron Village Scrub Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and
                can_afford_price(state, player, 200) and 
                (
                    state.has("Goron Mask", player) or 
                    (
                        state.can_reach("Goron Village Freestanding HP", "Location", player) and 
                        state.can_reach("Southern Swamp Freestanding HP", "Location", player) and 
                        state.has("Moon's Tear", player)
                    )
                )
            ),
        "Goron Village Deku Trade":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Deku Mask", player) and 
                state.has("Swamp Title Deed", player)
            ),
        "Goron Village Freestanding HP":
            lambda state: (
                state.can_reach("Goron Village Deku Trade", "Location", player) and 
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Powder Keg Goron Reward":
            lambda state: (
                has_soul_npc(state, player, options, "Gatekeeper & Medigoron") and
                (can_clear_snowhead(state, player, boss_placements) or 
                (
                    can_use_fire_arrows(state, player) and 
                    state.has("Goron Mask", player))
                )
            ),
        "Goron Village Baby Goron Lullaby":
            lambda state: (
                state.has("Goron Mask", player) and 
                can_play_song("Goron Lullaby", state, player)
            ),
        "Goron Village Shop Item 1":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                (has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or
                state.has("Goron Mask", player)) and 
                can_purchase(state, player, prices, SHOP_ID_GORON_SHOP_1)
            ),
        "Goron Village Shop Item 2":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                (has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or
                state.has("Goron Mask", player)) and 
                can_purchase(state, player, prices, SHOP_ID_GORON_SHOP_1)
            ),
        "Goron Village Shop Item 3":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                (has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or
                state.has("Goron Mask", player)) and 
                can_purchase(state, player, prices, SHOP_ID_GORON_SHOP_1)
            ),
        "Goron Village Shop (Spring) Item 1":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                (has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or
                state.has("Goron Mask", player)) and 
                can_purchase(state, player, prices, SHOP_ID_GORON_SHOP_1) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Goron Village Shop (Spring) Item 2":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                (has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or
                state.has("Goron Mask", player)) and 
                can_purchase(state, player, prices, SHOP_ID_GORON_SHOP_1) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Goron Village Shop (Spring) Item 3":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                (has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or
                state.has("Goron Mask", player)) and 
                can_purchase(state, player, prices, SHOP_ID_GORON_SHOP_1) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Goron Village Freestanding HP (Spring)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                state.has("Deku Mask", player) and 
                state.has("Swamp Title Deed", player)
            ),

        "Path to Snowhead Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                state.can_reach("Snowhead", 'Region', player) and
                has_explosives(state, player)
            ),
        "Path to Snowhead Scarecrow Pillar HP":
            lambda state: (
                can_reach_scarecrow(state, player, options) and 
                state.has("Path to Snowhead Scarecrow", player) and
                state.can_reach("Snowhead", 'Region', player) and 
                can_use_lens(state, player) and 
                state.has("Hookshot", player)
            ),
            
        "Snowhead Great Fairy Reward":
            lambda state: (
                has_soul_npc(state, player, options, "Great Fairies") and
                state.has("Stray Fairy (Snowhead)", player, options.required_stray_fairies.value)
            ),
        "Snowhead Temple Bridge Room Under Platform Bubble SF":
            lambda state: (
                state.has("Progressive Bow", player) and 
                state.has("Great Fairy's Mask", player)
            ),
        "Snowhead Temple Bridge Room Pillar Bubble SF":
            lambda state: (
                state.has("Progressive Bow", player) and 
                state.has("Great Fairy's Mask", player)
            ),
        "Snowhead Temple Bombable Stairs Crate SF":
            lambda state: (
                has_explosives(state, player) and
                (
                    (
                        state.has("Small Key (Snowhead)", player, 1) and 
                        state.has("Great Fairy's Mask", player)
                    ) or 
                    (
                        state.has("Hookshot", player) and 
                        state.can_reach("Snowhead Temple Bridge Room Under Platform Bubble SF", "Location", player)
                    )
                )
            ),
        "Snowhead Temple Timed Switch Room Bubble SF":
            lambda state: (
                state.has("Great Fairy's Mask", player) and 
                can_use_lens(state, player) and 
                (
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Progressive Bow", player) and 
                        has_explosives(state, player)
                    ) or 
                    (
                        can_reach_scarecrow(state, player, options) and
                        state.has("Snowhead Temple Lower Scarecrow", player) and 
                        state.has("Hookshot", player) and 
                        state.has("Progressive Bow", player)
                    ) or 
                    can_use_fire_arrows(state, player)
                )
            ),
        "Snowhead Temple Snowmen Bubble SF":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 1) and 
                can_use_fire_arrows(state, player) and 
                state.has("Great Fairy's Mask", player)
            ),
        "Snowhead Temple Dinolfos Room First SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Dinolfos") and
                state.has("Small Key (Snowhead)", player, 1) and 
                can_use_fire_arrows(state, player)
            ),
        "Snowhead Temple Dinolfos Room Second SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Dinolfos") and
                state.has("Small Key (Snowhead)", player, 1) and 
                can_use_fire_arrows(state, player)
            ),
        "Snowhead Temple Bridge Room Freezard Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Freezard") and
                can_use_fire_arrows(state, player) or 
                state.has("Hookshot", player)
            ),
        "Snowhead Temple Basement Switch Chest SF":
            lambda state: True,    
        "Snowhead Temple Freezard Torch Room Chest SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Freezard") and
                can_use_fire_arrows(state, player)
            ),
        "Snowhead Temple Stacked Block Upper Chest SF":
            lambda state: (
                state.has("Hookshot", player) or 
                (
                    state.has("Small Key (Snowhead)", player, 1) and 
                    can_use_fire_arrows(state, player)
                )
            ),
        "Snowhead Temple Behind Stacked Block Chest":
            lambda state: True,
        "Snowhead Temple Frozen Block Chest":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 1) or 
                state.has("Hookshot", player)
            ),
        "Snowhead Temple Frozen Block Upper Chest SF":
            lambda state: (
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 1) or 
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Hidden Chest SF":
            lambda state: (
                can_use_lens(state, player) and 
                has_explosives(state, player) and 
                state.has("Progressive Bow", player) and 
                (
                    state.has("Small Key (Snowhead)", player, 1) or 
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Snowball Chest":
            lambda state: (
                has_explosives(state, player) and 
                (
                    (
                        state.has("Small Key (Snowhead)", player, 1) and 
                        state.has("Progressive Bow", player)
                    ) or 
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Elevator Room Invisible Platform Chest SF":
            lambda state: (
                can_use_lens(state, player) and 
                (
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        has_explosives(state, player)
                    ) or 
                    can_use_fire_arrows(state, player) or 
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Elevator Room Lower Chest":
            lambda state: True,
        "Snowhead Temple Elevator Freestanding SF":
            lambda state: True,    
        "Snowhead Temple Lower Wizzrobe Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Wizrobe") and
                (
                    state.has("Small Key (Snowhead)", player, 2) and 
                    has_explosives(state, player)
                ) or 
                can_use_fire_arrows(state, player)
            ),
        "Snowhead Temple Main Room Wall Chest SF":
            lambda state: (
                can_use_fire_arrows(state, player) and 
                can_use_lens(state, player) and 
                (
                    (
                        state.has("Small Key (Snowhead)", player, 3) and 
                        has_explosives(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        can_reach_scarecrow(state, player, options) and

                        state.has("Hookshot", player)
                    )
                )
            ),
        "Snowhead Temple Upper Wizzrobe Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Wizrobe") and
                can_use_fire_arrows(state, player) and 
                (
                    (
                        state.has("Small Key (Snowhead)", player, 3) and 
                        has_explosives(state, player)
                    ) or 
                    (
                        state.has("Small Key (Snowhead)", player, 1) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Snowhead Temple Heart Container":
            lambda state: (
                has_soul_boss(state, player, options, "Goht") and
                can_use_fire_arrows(state, player) and 
                (
                    (
                        state.has("Small Key (Snowhead)", player, 1) and 
                        state.has("Boss Key (Snowhead)", player)
                    ) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Snowhead Temple Goht's Remains":
            lambda state: (
                has_soul_boss(state, player, options, "Goht") and
                can_use_fire_arrows(state, player) and 
                (
                    (
                        state.has("Small Key (Snowhead)", player, 1) and 
                        state.has("Boss Key (Snowhead)", player)
                    ) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),

        "Romani Ranch Baby Cuccos March":
            lambda state: has_soul_npc(state, player, options, "Grog") and 
                            state.has("Bremen Mask", player),
        "Romani Ranch Helping Cremia":
            lambda state: (
                has_soul_npc(state, player, options, "Romani & Cremia") and
                can_use_powder_keg(state, player, options) and 
                state.has("Progressive Bow", player)
            ),
        "Romani Ranch Doggy Racetrack Rooftop Chest":
            lambda state: (
                state.has("Hookshot", player) or 
                can_plant_beans(state, player, options) or 
                state.has("Zora Mask", player)
            ),
        "Romani Ranch Doggy Race":
            lambda state: (
                has_soul_npc(state, player, options, "Doggy Race Lady & Dogs") and
                state.has("Mask of Truth", player)
            ),
        "Romani Ranch Romani Game":
            lambda state: (
                has_soul_npc(state, player, options, "Romani & Cremia") and
                can_use_powder_keg(state, player, options) and 
                state.has("Progressive Bow", player)
            ),
        "Romani Ranch Aliens":
            lambda state: (
                has_soul_npc(state, player, options, "Romani & Cremia") and
                can_use_powder_keg(state, player, options) and 
                state.has("Progressive Bow", player)
            ),
        "Romani Ranch Barn Free Cow":
            lambda state: (
                has_soul_misc(state, player, options, "Cows") and
                can_use_powder_keg(state, player, options) and 
                can_play_song("Epona's Song", state, player)
            ),
        "Romani Ranch Barn Stables Front Cow":
            lambda state: (
                has_soul_misc(state, player, options, "Cows") and
                can_use_powder_keg(state, player, options) and 
                can_play_song("Epona's Song", state, player)
            ),
        "Romani Ranch Barn Stables Back Cow":
            lambda state: (
                has_soul_misc(state, player, options, "Cows") and
                can_use_powder_keg(state, player, options) and 
                can_play_song("Epona's Song", state, player)
            ),
        "Great Bay Healing Zora":
            lambda state: can_play_song("Song of Healing", state, player),
        "Great Bay Scarecrow Ledge HP":
            lambda state: (
                can_plant_beans(state, player, options) and 
                can_reach_scarecrow(state, player, options) and 
                state.has("Great Bay Coast Rock Wall Scarecrow", player) and
                state.has("Hookshot", player)
            ),
        "Tingle Great Bay Map Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Tingle") and
                has_projectiles(state, player) and 
                (
                    state.can_reach("Great Bay", 'Region', player) or 
                    state.can_reach("Milk Road", 'Region', player)
                )
            ),
        "Great Bay Fisherman's Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos")
            ),    
        "Great Bay Ledge Grotto Left Cow":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Cows") and
                state.has("Hookshot", player) and 
                can_play_song("Epona's Song", state, player)
            ),
        "Great Bay Ledge Grotto Right Cow":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_misc(state, player, options, "Cows") and
                state.has("Hookshot", player) and 
                can_play_song("Epona's Song", state, player)
            ),
        "Pinnacle Rock Seahorse HP":
            lambda state: (
                has_soul_npc(state, player, options, "Fisherman") and
                has_soul_enemy(state, player, options, "Pirate Guards") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Upper Eel Chest":
            lambda state: (
                has_soul_npc(state, player, options, "Fisherman") and
                has_soul_enemy(state, player, options, "Pirate Guards") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Lower Eel Chest":
            lambda state: (
                has_soul_npc(state, player, options, "Fisherman") and
                has_soul_enemy(state, player, options, "Pirate Guards") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Great Bay Baby Zora Song":
            lambda state: (
                has_soul_npc(state, player, options, "Marine Lab Researcher") and
                has_bottle(state, player) and 
                (
                    can_reach_seahorse(state, player, options) or
                    state.can_reach("Pirates' Fortress Leader's Room Chest", "Location", player) and
                    state.has("Hookshot", player)
                )
            ),
        "Great Bay Feeding Lab Fish":
            lambda state: (
                has_soul_npc(state, player, options, "Marine Lab Fish") and
                has_bottle(state, player)
            ),
        "Great Bay Fisherman Game":
            lambda state: (
                has_soul_npc(state, player, options, "Fisherman") and
                can_clear_greatbay(state, player, boss_placements)
            ),
        "Ocean Spider House Ramp Upper Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ), 
        "Ocean Spider House Ramp Lower Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ), 
        "Ocean Spider House Lobby Ceiling Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and 
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House First Room Rafter Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House First Room Open Pot #1 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House First Room Open Pot #2 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House First Room Wall Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Library Top Bookcase Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Library Passage Behind Bookcase Front Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Library Passage Behind Bookcase Rear Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Library Painting #1 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Library Painting #2 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Library Rafter Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Library Bookshelf Hole Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House First Room Downstairs Rafter Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House First Room Downstairs Open Pot Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House First Room Downstairs Behind Staircase Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House First Room Downstairs Crate Token":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House First Room Downstairs Wall Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Dining Room Open Pot Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Dining Room Painting Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Dining Room Ceiling Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Dining Room Chandelier #1 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Dining Room Chandelier #2 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Dining Room Chandelier #3 Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Storage Room Web Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and 
                can_use_fire_arrows(state, player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Storage Room North Wall Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Storage Room Crate Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Storage Room Hidden Hole Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Storage Room Ceiling Pot Token":
             lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_misc(state, player, options, "Gold Skulltulas") and
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Coloured Mask Sequence HP":
            lambda state: (
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Hookshot", player) and 
                state.has("Captain's Hat", player) and 
                state.has("Progressive Bow", player) and 
                has_explosives(state, player)
            ),
        "Ocean Spider House Reward":
            lambda state: (
                state.has("Ocean Skulltula Token", player, options.required_skull_tokens.value) and
                has_soul_npc(state, player, options, "Spider Man")
            ),
        "Pirates' Fortress Exterior Underwater Log Chest":
            lambda state: state.has("Zora Mask", player),
        "Pirates' Fortress Exterior Underwater Near Entrance Chest":
            lambda state: state.has("Zora Mask", player),
        "Pirates' Fortress Exterior Underwater Corner Chest":
            lambda state: state.has("Zora Mask", player),
        
        "Pirates' Fortress Sewers Maze Chest":
            lambda state: state.has("Goron Mask", player),
        "Pirates' Fortress Sewers Cage HP":
            lambda state: state.has("Goron Mask", player),
        "Pirates' Fortress Sewers Underwater Upper Chest":
            lambda state: state.has("Goron Mask", player),
        "Pirates' Fortress Sewers Underwater Lower Chest":
            lambda state: state.has("Goron Mask", player),
        
        "Pirates' Fortress Hub Lower Chest":
            lambda state: True,
        "Pirates' Fortress Hub Upper Chest":
            lambda state: state.has("Hookshot", player),
        "Pirates' Fortress Leader's Room Chest":
            lambda state: (
                (
                    state.has("Hookshot", player) or 
                    state.has("Goron Mask", player)
                ) and 
                (
                    state.has("Progressive Bow", player) or 
                    (
                        state.has("Deku Mask", player) and 
                        state.has("Progressive Magic", player)
                    )
                )
            ),
        "Pirates' Fortress Interior Tank Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Coloured Pirates") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        "Pirates' Fortress Interior Guarded Chest":
            lambda state: state.has("Hookshot", player),
            
        "Zora Cape Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                (
                    state.has("Goron Mask", player) or 
                    has_explosives(state, player)
                )
            ),
        "Zora Cape Underwater Chest":
            lambda state: state.has("Zora Mask", player),
        "Zora Cape Underwater Like-Like HP":
            lambda state: state.has("Zora Mask", player),
        "Zora Cape Pot Game":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
                state.has("Zora Mask", player)
            ),
        "Zora Cape Deku Flower Chest":
            lambda state: state.has("Hookshot", player),
        "Zora Cape Scarecrow Chest":
            lambda state: (
                can_reach_scarecrow(state, player, options) and 
                state.has("Zora Cape Beavers Scarecrow", player) and
                state.has("Hookshot", player) and 
                state.has("Deku Mask", player)
            ),
        "Beaver Bros. Race 1":
            lambda state: (
                state.has("Hookshot", player) and 
                state.has("Zora Mask", player)
            ),
        "Beaver Bros. Race 2 HP":
            lambda state: (
                state.has("Hookshot", player) and 
                state.has("Zora Mask", player)
            ),
            
        "Zora Hall Evan's Song":
            lambda state: (
                has_soul_npc(state, player, options, "Evan") and
                state.has("Ocarina of Time", player) and
                state.has("Zora Mask", player)
            ),
        "Zora Hall Torches Reward":
            lambda state: (
                has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
                can_use_fire_arrows(state, player)
            ),
        "Zora Hall Good Picture of Lulu":
           lambda state: (
               has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
               has_soul_npc(state, player, options, "Lulu") and
               state.has("Pictograph Box", player) and 
               state.has("Zora Mask", player)
           ),
        "Zora Hall Bad Picture of Lulu":
           lambda state: (
               has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
               has_soul_npc(state, player, options, "Lulu") and
               state.has("Pictograph Box", player) and 
               state.has("Zora Mask", player)
           ),
        "Zora Hall Deku Scrub Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Zora Mask", player)
            ),
        "Zora Hall Goron Scrub Trade":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Zora Mask", player) and 
                state.has("Mountain Title Deed", player) and 
                state.has("Goron Mask", player)
            ),
        "Zora Hall Goron Scrub Trade Freestanding HP":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Deku Mask", player) and 
                state.can_reach("Zora Hall Goron Scrub Trade", "Location", player)
            ),
        "Zora Hall Shop Item 1":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                state.has("Zora Mask", player) and 
                can_purchase(state, player, prices, SHOP_ID_ZORA_SHOP_1)
            ),
        "Zora Hall Shop Item 2":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                state.has("Zora Mask", player) and 
                can_purchase(state, player, prices, SHOP_ID_ZORA_SHOP_2)
            ),
        "Zora Hall Shop Item 3":
            lambda state: (
                has_soul_npc(state, player, options, "Shop Owners") and
                state.has("Zora Mask", player) and 
                can_purchase(state, player, prices, SHOP_ID_ZORA_SHOP_3)
            ),

        "Great Bay Great Fairy Reward":
            lambda state: (
                has_soul_npc(state, player, options, "Great Fairies") and
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.has("Stray Fairy (Great Bay)", player, options.required_stray_fairies.value) and 
                state.has("Hookshot", player)
            ),
            
        "Great Bay Temple Entrance Torches Chest SF":
            lambda state: True,
        "Great Bay Temple Waterwheel Room Skulltula SF":
            lambda state: 
            has_soul_enemy(state, player, options, "Hanging Skulltulas") and 
            can_smack_hard(state, player),
        "Great Bay Temple Waterwheel Room Bubble SF":
            lambda state: (
                state.has("Zora Mask", player) or 
                (
                    has_projectiles(state, player) and 
                    state.has("Great Fairy's Mask", player)
                )
            ),
        "Great Bay Temple Blender Room Barrel SF":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Blender Pot SF":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Before Red Valve Room Chest":
            lambda state: can_use_ice_arrows(state, player),
        "Great Bay Temple Before Red Valve Room Pot SF":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_ice_arrows(state, player) or
                    (
                        has_projectiles(state, player) and 
                        state.has("Great Fairy's Mask", player)
                    ) or 
                    state.has("Deku Mask", player)
                )
            ),
        "Great Bay Temple Bio-Baba Hall Chest SF":
            lambda state: has_soul_enemy(state, player, options, "Deku Baba"),
        "Great Bay Temple Before Gekko Room Pot SF":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Before Gekko Room Upper Chest":
            lambda state: True,
        "Great Bay Temple Mad Jellied Gekko Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Jellied Gekko") and
                can_use_ice_arrows(state, player) and 
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Before Gekko Room Underwater Chest":
            lambda state: True,
        "Great Bay Temple Behind Locked Door Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Wart") and
                state.has("Small Key (Great Bay)", player) and 
                (
                    can_smack_hard(state, player) or
                    has_explosives(state, player) or
                    state.has("Progressive Bow", player)
                )
            ),
        "Great Bay Temple Room Behind Waterfall Ceiling Chest SF":
            lambda state: (
                state.has("Hookshot", player) and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Freezable Waterwheel Upper Chest SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and 
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Freezable Waterwheel Lower Chest SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and 
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Seesaw Room Underwater Barrel SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and 
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Seesaw Room Chest SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and 
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Pre-Boss Room Platform Bubble SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and 
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Pre-Boss Room Tunnel Bubble SF":
            lambda state: state.can_reach("Great Bay Temple Pre-Boss Room Platform Bubble SF", "Location", player),
        "Great Bay Temple Heart Container":
            lambda state: (
                has_soul_boss(state, player, options, "Gyorg") and
                state.has("Hookshot", player) and 
                (
                    (
                        state.can_reach("Great Bay Temple Pre-Boss Room Platform Bubble SF", "Location", player) and 
                        state.has("Boss Key (Great Bay)", player)
                    ) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Great Bay Temple Gyorg's Remains":
            lambda state: (
                has_soul_boss(state, player, options, "Gyorg") and
                state.has("Hookshot", player) and 
                (
                    (
                        state.can_reach("Great Bay Temple Pre-Boss Room Platform Bubble SF", "Location", player) and 
                        state.has("Boss Key (Great Bay)", player)
                    ) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        
        "Road to Ikana Pillar Chest":
            lambda state: state.has("Hookshot", player),
        "Road to Ikana Rock Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                state.has("Goron Mask", player)
            ),
        "Road to Ikana Invisible Soldier":
            lambda state: (
                has_soul_npc(state, player, options, "Shiro") and
                can_use_lens(state, player) and
                (
                    can_play_song("Epona's Song", state, player) or
                    options.owlsanity.value 
                    and
                    (
                        can_use_owl(state, player, options, "Ikana Canyon") or
                        can_use_owl(state, player, options, "Stone Tower")
                    )                
                ) and
                (
                    can_get_red_potion(state, player, prices, options) or
                    can_get_blue_potion(state, player, prices, options)
                )
            ),
            
        "Ikana Graveyard Bombable Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_explosives(state, player)
            ),
        "Graveyard Day 1 Bats Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Bad Bats") and
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and 
                can_smack(state, player)
            ),
        "Graveyard Day 2 Dampe Bats":
            lambda state: (
                has_soul_npc(state, player, options, "Dampe") and
                has_projectiles(state, player)
            ),
        "Graveyard Day 2 Iron Knuckle Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Iron Knuckle") and
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and 
                can_smack_hard(state, player) and 
                has_explosives(state, player) and 
                can_use_lens(state, player)
            ),
        "Graveyard Day 3 Dampe Big Poe Chest":
            lambda state: (
                # Always required
                has_soul_npc(state, player, options, "Stalchildren") and
                has_soul_npc(state, player, options, "Dampe") and
                state.has("Captain's Hat", player) and
                # Alternative methods
                (
                    state.has("Progressive Bow", player) or
                    state.has("Zora Mask", player)
                )
            ),
        "Graveyard Captain Keeta Chest":
            lambda state: (
                has_soul_npc(state, player, options, "Stalchildren") and
                can_play_song("Sonata of Awakening", state, player) and 
                can_smack_hard(state, player)
            ),
        "Graveyard Day 1 Iron Knuckle Song":
            lambda state: (
                has_soul_enemy(state, player, options, "Iron Knuckle") and
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and 
                can_smack_hard(state, player)
            ),

        "Tingle Stone Tower Map Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Tingle") and
                has_projectiles(state, player) and 
                (
                    (
                        state.can_reach("Upper Ikana Canyon", 'Region', player) 
                    ) or 
                    state.can_reach("Great Bay", 'Region', player)
                )
            ),
        "Ikana Canyon Spirit House":
            lambda state: (
                has_soul_npc(state, player, options, "Spirit House Owner") and
                can_use_ice_arrows(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ikana Canyon Healing Pamela's Father":
            lambda state: (
                has_soul_npc(state, player, options, "Pamela's Father") and
                has_soul_npc(state, player, options, "Composer Poe") and
                can_use_ice_arrows(state, player) and 
                can_play_song("Song of Healing", state, player) and 
                can_play_song("Song of Storms", state, player)
            ),
        "Ikana Canyon Scrub Purchase":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and
                has_bottle(state, player) and 
                can_afford_price(state, player, 100)
            ),
        "Ikana Canyon Zora Scrub Trade":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Zora Mask", player) and 
                state.has("Ocean Title Deed", player)
            ),
        "Ikana Canyon Zora Trade Freestanding HP":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                state.has("Zora Mask", player) and 
                state.has("Ocean Title Deed", player)
            ),
        "Ikana Canyon Grotto Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos")
            ),   

        "Stone Tower Great Fairy Reward":
            lambda state: (
                has_soul_npc(state, player, options, "Great Fairies") and
                state.has("Stray Fairy (Stone Tower)", player, options.required_stray_fairies.value) and 
                can_use_ice_arrows(state, player)
            ),
            
        "Secret Shrine Dinolfos Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Dinolfos") and
                can_smack_hard(state, player) and 
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wizzrobe Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Wizrobe") and
                can_smack_hard(state, player) and 
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wart Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Wart") and
                can_smack_hard(state, player) and 
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Garo Master Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Garo Master") and
                can_use_light_arrows(state, player) and 
                can_smack_hard(state, player)
            ),
        "Secret Shrine Completion Chest":
            lambda state: (
                has_soul_npc(state, player, options, "Spirit House Owner") and
                state.can_reach("Secret Shrine Dinolfos Chest", "Location", player) and 
                state.can_reach("Secret Shrine Wizzrobe Chest", "Location", player) and 
                state.can_reach("Secret Shrine Wart Chest", "Location", player) and 
                state.can_reach("Secret Shrine Garo Master Chest", "Location", player)
            ),
            
        "Ikana Well Rightside Torch Chest":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_plant_beans(state, player, options) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Well Invisible Chest":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_afford_price(state, player, 100) or 
                    state.has("Mask of Scents", player) and
                    has_soul_npc(state, player, options, "Kotake")
                )
            ),
        "Ikana Well Final Chest":
            lambda state: (
                (
                    has_soul_npc(state, player, options, "Gibdos") and
                    state.has("Gibdo Mask", player) and 
                    has_bottle(state, player) and 
                    can_plant_beans(state, player, options) and 
                    (
                        state.has("Progressive Bomb Bag", player) or 
                        (
                            state.has("Captain's Hat", player) and 
                            state.has("Progressive Bow", player)
                        )
                    )
                ) or 
                (
                    can_use_light_arrows(state, player) and 
                    can_use_fire_arrows(state, player)
                )
            ),
        "Ikana Well Cow":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_misc(state, player, options, "Cows") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_play_song("Epona's Song", state, player) and 
                    (
                        can_plant_beans(state, player, options) or 
                        can_use_light_arrows(state, player)
                    ) and 
                    (
                        (
                            can_play_song("Song of Soaring", state, player) and 
                            (
                                (
                                    state.can_reach("Twin Islands", 'Region', player) and 
                                    can_use_fire_arrows(state, player)
                                ) or 
                                (
                                    can_use_lens(state, player) and 
                                    state.has("Goron Mask", player)
                                )
                            )
                        ) or 
                        state.can_reach("Ikana Well Invisible Chest", "Location", player)
                    )
                )
            ),
            
        "Ikana Castle Pillar Freestanding HP":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_lens(state, player) and 
                can_use_fire_arrows(state, player)
            ),
        "Ikana Castle King Song":
            lambda state: (
                has_soul_enemy(state, player, options, "Igos") and
                (
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player) and 
                    can_use_lens(state, player) and 
                    can_use_fire_arrows(state, player) and 
                    state.has("Powder Keg", player) and 
                    state.has("Goron Mask", player) and 
                    has_mirror_shield(state, player)
                ) or 
                (
                    can_use_fire_arrows(state, player) and 
                    has_mirror_shield(state, player) and 
                    can_use_light_arrows(state, player)
                )
            ),

        "Stone Tower Inverted Left Chest":
            lambda state: can_plant_beans(state, player, options),
        "Stone Tower Inverted Middle Chest":
            lambda state: can_plant_beans(state, player, options),
        "Stone Tower Inverted Right Chest":
            lambda state: can_plant_beans(state, player, options),
        
        "Stone Tower Temple Entrance Room Eye Switch Chest":
            lambda state: state.has("Progressive Bow", player),
        "Stone Tower Temple Entrance Room Lower Chest":
            lambda state: (
                state.has("Small Key (Stone Tower)", player, 4) and 
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_light_arrows(state, player) and
                state.has("Hookshot", player)
            ),
        "Stone Tower Temple Armos Room Back Chest":
            lambda state: (
                state.has("Goron Mask", player) and
                state.has("Zora Mask", player) and
                has_explosives(state, player) and
                (
                    has_mirror_shield(state, player) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Stone Tower Temple Armos Room Upper Chest":
            lambda state: state.has("Hookshot", player),
        "Stone Tower Temple Armos Room Lava Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Armos") and
                state.has("Goron Mask", player) and
                state.has("Zora Mask", player) and
                has_explosives(state, player) and
                (
                    has_mirror_shield(state, player) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Stone Tower Temple Eyegore Room Switch Chest":
            lambda state: (
                can_use_light_arrows(state, player) and
                state.has("Zora Mask", player)
            ),
        "Stone Tower Temple Eyegore Room Dexi Hand Ledge Chest":
            lambda state: (
                state.has("Zora Mask", player) and
                (
                    can_use_light_arrows(state, player) or
                    (
                        state.has("Small Key (Stone Tower)", player, 1) and
                        state.has("Goron Mask", player) and
                        has_explosives(state, player)
                    )
                )
            ),
        "Stone Tower Temple Eastern Water Room Underwater Chest":
            lambda state: (
                can_use_light_arrows(state, player) and
                state.has("Zora Mask", player)
            ),
        "Stone Tower Temple Eastern Water Room Sun Block Chest":
            lambda state: (
                can_use_light_arrows(state, player) or 
                (
                    state.has("Small Key (Stone Tower)", player, 1) and 
                    has_mirror_shield(state, player) and
                    state.has("Goron Mask", player) and
                    state.has("Zora Mask", player) and
                    has_explosives(state, player)
                )
            ),
        "Stone Tower Temple Mirror Room Sun Block Chest":
            lambda state: (
                (
                    state.has("Small Key (Stone Tower)", player, 2) and 
                    has_mirror_shield(state, player) and
                    state.has("Goron Mask", player) and
                    state.has("Zora Mask", player) and
                    has_explosives(state, player)
                ) or 
                (
                    can_use_light_arrows(state, player) and 
                    state.has("Small Key (Stone Tower)", player, 1)
                )
            ),
        "Stone Tower Temple Mirror Room Sun Face Chest":
            lambda state: (
                (
                    state.has("Small Key (Stone Tower)", player, 2) and 
                    has_mirror_shield(state, player) and
                    state.has("Goron Mask", player) and
                    state.has("Zora Mask", player) and
                    has_explosives(state, player)
                ) or 
                (
                    can_use_light_arrows(state, player) and 
                    state.has("Small Key (Stone Tower)", player, 1)
                )
            ),
        "Stone Tower Temple Air Gust Room Side Chest":
            lambda state: (
                (
                    state.has("Small Key (Stone Tower)", player, 2) and 
                    has_mirror_shield(state, player) and 
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player) and 
                    state.has("Goron Mask", player) and
                    state.has("Zora Mask", player) and
                    has_explosives(state, player)
                ) or 
                (
                    can_use_light_arrows(state, player) and 
                    state.has("Small Key (Stone Tower)", player, 1) and 
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player)
                )
            ),
        "Stone Tower Temple Air Gust Room Goron Switch Chest":
            lambda state: (
                state.can_reach("Stone Tower Temple Mirror Room Sun Block Chest", "Location", player) and 
                state.has("Goron Mask", player)
            ),
        "Stone Tower Temple Garo Master Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Garo Master") and
                can_smack_hard(state, player) and
                (
                    state.has("Small Key (Stone Tower)", player, 2) and 
                    has_mirror_shield(state, player) and 
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player) and 
                    state.has("Goron Mask", player) and
                    state.has("Zora Mask", player) and
                    has_explosives(state, player)
                ) or 
                (
                    can_use_light_arrows(state, player) and 
                    state.has("Small Key (Stone Tower)", player, 1) 
                )
            ),
        "Stone Tower Temple After Garo Upside Down Chest":
            lambda state: state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player),
        "Stone Tower Temple Eyegore Chest":
            lambda state: (
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player) and
                state.has("Hookshot", player)
            ),
        "Stone Tower Temple Inverted Entrance Room Sun Face Chest":
            lambda state: can_use_light_arrows(state, player),
        "Stone Tower Temple Inverted Eastern Air Gust Room Fire Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_light_arrows(state, player)
            ),
        "Stone Tower Temple Inverted Eastern Air Gust Room Frozen Switch Chest":
            lambda state: (
                can_use_light_arrows(state, player) and 
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                can_use_fire_arrows(state, player)
            ),
        "Stone Tower Temple Inverted Eastern Air Gust Room Switch Chest":
            lambda state: (
                can_use_light_arrows(state, player) and 
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Stone Tower Temple Inverted Wizzrobe Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Wizrobe") and
                can_use_light_arrows(state, player) and 
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                state.has("Small Key (Stone Tower)", player, 3) and
                state.has("Hookshot", player)
            ),
        "Stone Tower Temple Inverted Death Armos Maze Chest":
            lambda state: state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player),
        "Stone Tower Temple Inverted Gomess Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Gomess") and
                state.can_reach("Stone Tower Temple Inverted Death Armos Maze Chest", "Location", player) and 
                can_use_light_arrows(state, player) and 
                can_smack_hard(state, player)
            ),
        "Stone Tower Temple Inverted Eyegore Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Eyegore") and
                state.can_reach("Stone Tower Temple Inverted Death Armos Maze Chest", "Location", player) and 
                state.has("Small Key (Stone Tower)", player, 4)
            ),
        "Stone Tower Temple Inverted Heart Container":
            lambda state: (
                has_soul_boss(state, player, options, "Twinmold") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player) and
                (
                    state.has("Progressive Bow", player) or
                    state.has("Fierce Deity's Mask", player) or
                    (
                        state.has("Giant's Mask", player) and
                        state.has("Progressive Magic", player) and
                        state.has("Progressive Sword", player)
                    )
                ) and
                (
                    state.has("Boss Key (Stone Tower)", player) or
                    (
                        state.has("Twinmold's Remains", player) and
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Stone Tower Temple Inverted Twinmold's Remains":
            lambda state: (
                has_soul_boss(state, player, options, "Twinmold") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player) and
                (
                    state.has("Progressive Bow", player) or
                    state.has("Fierce Deity's Mask", player) or
                    (
                        state.has("Giant's Mask", player) and
                        state.has("Progressive Magic", player) and
                        state.has("Progressive Sword", player)
                    )
                ) and
                (
                    state.has("Boss Key (Stone Tower)", player) or
                    (
                        state.has("Twinmold's Remains", player) and
                        options.remains_allow_boss_warps.value
                    )
                )
            ),

        "Oath to Order":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) or 
                can_clear_snowhead(state, player, boss_placements) or 
                can_clear_greatbay(state, player, boss_placements) or 
                can_clear_stonetower(state, player, boss_placements)
            ),

        "Moon Deku Trial HP":
            lambda state: ( 
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Moon Goron Trial HP":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                state.has("Goron Mask", player) and 
                state.has("Progressive Magic", player)
            ),
        "Moon Zora Trial HP":
            lambda state: ( 
                has_soul_npc(state, player, options, "Moon Kids") and
                state.has("Zora Mask", player)
            ),    
            
        "Moon Link Trial Garo Master Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Garo Master") and
                has_soul_npc(state, player, options, "Moon Kids") and
                can_smack_hard(state, player) and 
                state.has("Hookshot", player)
            ),
        "Moon Link Trial Iron Knuckle Chest":
            lambda state:(
                has_soul_enemy(state, player, options, "Iron Knuckle") and 
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("Moon Link Trial Garo Master Chest", "Location", player)
            ),    
        "Moon Link Trial HP":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("Moon Link Trial Garo Master Chest", "Location", player) and 
                has_bombchus(state, player) and 
                state.has("Progressive Bow", player)
            ),
        "Moon Trade All Masks":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("Moon Deku Trial HP", "Location", player) and 
                state.can_reach("Moon Goron Trial HP", "Location", player) and 
                state.can_reach("Moon Zora Trial HP", "Location", player) and 
                state.can_reach("Moon Link Trial HP", "Location", player) and 
                can_use_fire_arrows(state, player) and 
                has_enough_masks(state, player, 20, exclude_transformation=True)
            ),
        "Defeat Majora":
            lambda state: (
                # Goal Requirements - Optional challenges
                # Remains Goal
                has_enough_remains(state, player, options.majora_remains_required.value) and
                
                # Masks Goal
                has_enough_masks(state, player, options.majora_masks_required.value) and
                
                # Items Goal
                has_enough_items(state, player, options.majora_items_required.value) and
                
                # Star Fox Goal (5 specific masks)
                (not options.majora_star_fox.value or 
                has_star_fox(state, player, options, "majora")) and
                
                # Frogs Goal
                has_all_frogs(state, player, options, "majora") and
                
                # Scarecrows Goal
                has_all_scarecrows(state, player, options, "majora") and
                
                # Owl Statues Goal
                has_all_owls(state, player, options, "majora") and
                
                # Soul Requirements
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_boss(state, player, options, "Majora") and
                
                # Combat Requirements
                can_smack_hard(state, player) and
                
                # Equipment Requirements - Either Light Arrows OR Fierce Deity
                (
                    (
                        (state.has("Zora Mask", player) or has_mirror_shield(state, player)) and
                        can_use_light_arrows(state, player)
                    ) or
                    (
                        state.has("Fierce Deity's Mask", player) and
                        state.has("Progressive Magic", player)
                    )
                )
            ),
                     
            # Grass Location Rules
            # Clock Town Area Grass
        "Before Clock Town Keaton Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),         
        "Before Clock Town Keaton Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),   
        "Before Clock Town Keaton Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),   
        "Before Clock Town Keaton Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),   
        "Before Clock Town Keaton Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),   
        "Before Clock Town Keaton Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),   
        "Before Clock Town Keaton Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),   
        "Before Clock Town Keaton Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ), 
        "Before Clock Town Keaton Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),        
        "Before Clock Town Skullkid Keaton Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Skullkid Keaton Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Skullkid Keaton Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Before Clock Town Skullkid Keaton Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Skullkid Keaton Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Skullkid Keaton Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Before Clock Town Skullkid Keaton Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Skullkid Keaton Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Skullkid Keaton Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        # Lost Woods Grass
        "Lost Woods Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Grass Patch 2 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Keaton Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Keaton Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Keaton Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Keaton Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Keaton Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Keaton Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Keaton Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Keaton Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),  
        "Lost Woods Keaton Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Laundry Pool Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Laundry Pool Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Laundry Pool Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),   

        "North Clock Town Keaton Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "North Clock Town Keaton Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "North Clock Town Keaton Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "North Clock Town Keaton Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "North Clock Town Keaton Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "North Clock Town Keaton Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "North Clock Town Keaton Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "North Clock Town Keaton Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "North Clock Town Keaton Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
                                                                              
        # Termina Field Grass Near Western Water Ramp
        "Termina Field Grass Near Western Water Ramp (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Water Ramp (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Takkuri Gossip Tree
        "Termina Field Grass Near Takkuri Gossip Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri Gossip Tree (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Southwest Above Rock Ledge
        "Termina Field Grass Southwest Above Rock Ledge (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Southwest Above Rock Ledge (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Western Fountains
        "Termina Field Grass Near Western Fountains (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Western Fountains (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Grass Near Takkuri
        "Termina Field Grass Near Takkuri (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Takkuri (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Kamaro
        "Termina Field Grass Near Kamaro (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Kamaro (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Peehat Grotto
        "Termina Field Grass Near Peehat Grotto (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Peehat Grotto (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass In Front of Log
        "Termina Field Grass In Front of Log (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass In Front of Log (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Northern Ramp 
        "Termina Field Grass Near Northern Ramp (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Northern Ramp (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Grass Grotto
        "Termina Field Grass Near Grass Grotto (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Grass Grotto (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Below Southeast Hill Fence
        "Termina Field Grass Below Southeast Hill Fence (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Below Southeast Hill Fence (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Bottom of Southeast Hill Grass
        "Termina Field Bottom of Southeast Hill Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Bottom of Southeast Hill Grass (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Eastern Pillars
        "Termina Field Grass Near Eastern Pillars (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillars (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Eastern Pillar Grotto
        "Termina Field Grass Near Eastern Pillar Grotto (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Pillar Grotto (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Middle of Southeast Hill Grass
        "Termina Field Middle of Southeast Hill Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Middle of Southeast Hill Grass (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Business Scrub Grotto
        "Termina Field Grass Near Business Scrub Grotto (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Business Scrub Grotto (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Observatory Fence
        "Termina Field Grass Near Observatory Fence (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Observatory Fence (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Near Eastern Gossip Grotto
        "Termina Field Grass Near Eastern Gossip Grotto (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Termina Field Grass Near Eastern Gossip Grotto (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Termina Field Grass Grotto Grass
        "Termina Field Grass Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Grass Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        # Termina Field Cow Grotto Grass Groups
        "Termina Field Cow Grotto Grass Group 1 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 1 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 2 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        
        "Termina Field Cow Grotto Grass Group 3 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (8)":
              lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 3 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        
        "Termina Field Cow Grotto Grass Group 4 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 4 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        
        "Termina Field Cow Grotto Grass Group 5 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 5 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        
        "Termina Field Cow Grotto Grass Group 6 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Termina Field Cow Grotto Grass Group 6 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        
        # Termina Field Peehat Grotto Grass
        "Termina Field Peehat Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Peehat Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        
        # Termina Field Bio Baba Grotto Grass
        "Termina Field Bio Baba Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Termina Field Bio Baba Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        
        # Termina Field Eastern Gossip Grotto Grass
        "Termina Field Eastern Gossip Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Gossip Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Gossip Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Gossip Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Gossip Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        
        # Termina Field Eastern Pillar Grotto Grass
        "Termina Field Eastern Pillar Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Eastern Pillar Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        
        # Termina Field Bombable Rock Grass 
        "Termina Field Bombable Rock Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Termina Field Bombable Rock Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Termina Field Bombable Rock Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Termina Field Bombable Rock Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Termina Field Bombable Rock Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        # Road To Southern Swamp Grass
        "Road to Southern Swamp Outside Archery Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Outside Archery Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Road to Southern Swamp Grass Near Tourist Centre
        "Road to Southern Swamp Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Road to Southern Swamp Grass 
        "Road to Southern Swamp Grass (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (13)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (14)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (15)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (16)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (17)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grass (18)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ), 
        "Road to Southern Swamp Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ), 
        "Road to Southern Swamp Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),           

        # Southern Swamp Grass
        "Southern Swamp Owl Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Owl Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Southern Swamp Grass Near Tourist Centre
        "Southern Swamp Grass Near Tourist Centre (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Tourist Centre (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (13)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (14)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (15)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (16)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (17)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Grass Near Witch Shop (18)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Near Gossip Stone Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Near Gossip Stone Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),         
        # Woods of Mystery Grass
        "Woods of Mystery Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (13)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (14)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (15)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (16)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (17)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (18)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (19)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Grass (20)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Woods of Mystery Day-specific Grass
        "Woods of Mystery Day 2 Unique Grass":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Day 3 Unique Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Day 3 Unique Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Woods of Mystery Day 2 Grotto Grass
        "Woods of Mystery Day 2 Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woods of Mystery Day 2 Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        
        # Southern Swamp Grotto Grass - Requires access to Swamp Spider House
        "Southern Swamp Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        "Southern Swamp Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                (
                    has_soul_npc(state, player, options, "Koume") and
                    state.has("Bottle of Red Potion", player) or 
                    (
                        has_hard_projectiles(state, player) and 
                        state.has("Deku Mask", player)
                    ) or 
                    (
                        has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                        state.has("Pictograph Box", player) and 
                        state.has("Deku Mask", player)
                    )
                )
            ),
        # Woodfall Temple Post Dungeon

        "Southern Swamp Owl Post Dungeon Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Owl Post Dungeon Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (15)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (16)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (17)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (18)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Gossip Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Gossip Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_woodfall(state, player, boss_placements)
            ),
        
        # Deku Palace Bean Grotto Grass
        "Deku Palace Bean Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Bean Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        
        # Woodfall Grass
        "Woodfall Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),

        # Milk Road Owl Grass
        "Milk Road Owl Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Owl Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Owl Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Milk Road Keaton Grass
        "Milk Road Keaton Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Keaton Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Keaton Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Keaton Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Keaton Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Keaton Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Keaton Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Keaton Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Keaton Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Milk Road Gorman Racetrack Grass Group 1 - Requires Epona's Song
        "Milk Road Gorman Racetrack Grass Group 1 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 1 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        
        # Milk Road Gorman Racetrack Grass Group 2 - Requires Epona's Song
        "Milk Road Gorman Racetrack Grass Group 2 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Milk Road Gorman Racetrack Grass Group 2 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),

        # Romani Ranch Grass In Front of Gossip Tree
        "Romani Ranch Grass In Front of Gossip Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass In Front of Gossip Tree (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Romani Ranch Grass Behind Gossip Tree
        "Romani Ranch Grass Behind Gossip Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Gossip Tree (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Romani Ranch Grass Near Entrance
        "Romani Ranch Grass Near Entrance (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Near Entrance (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Romani Ranch Grass Between Entrance and Barn
        "Romani Ranch Grass Between Entrance and Barn (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Between Entrance and Barn (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Romani Ranch Grass Behind Barn
        "Romani Ranch Grass Behind Barn (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Barn (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Barn (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Barn (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Barn (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Barn (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Barn (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Barn (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Romani Ranch Grass Behind Barn (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),

        # Twin Isles Grotto Grass 

        "Twin Isles Grotto Grass (1)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (2)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (3)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (4)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (5)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (6)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (7)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (8)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (9)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (10)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (11)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (12)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (13)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        "Twin Isles Grotto Grass (14)":
            lambda state:
            (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player) and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        state.has("Twin Islands Scarecrow", player) and
                        can_reach_scarecrow(state, player, options) and
                        has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        
        # Twin Islands Springtime Grass - Requires clearing Snowhead
        "Twin Islands Springtime Grass Group 1 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Springtime Grass Group 1 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),

        # Goron Village Lens Cave Grass
        "Goron Village Lens Cave Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Goron Village Lens Cave Grass continued
        "Goron Village Lens Cave Grass (13)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (14)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (15)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (16)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (17)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (18)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (19)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (20)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (21)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (22)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (23)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Goron Village Lens Cave Grass (24)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),

        # Path To Snowhead Grotto Grass - Requires Explosives and either Goron and Magic or Snowhead
        "Path To Snowhead Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),
        "Path To Snowhead Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.can_reach("Snowhead", 'Region', player) and 
                has_explosives(state, player)
            ),

        # Mountain Village Springtime Grass
        "Mountain Village Springtime Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (15)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (16)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (17)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (18)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (19)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (20)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (21)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (22)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (23)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (24)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (25)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (26)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (27)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (28)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (29)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Grass (30)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        
        # Mountain Village Springtime Keaton Grass
        "Mountain Village Keaton Grass (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Keaton Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Keaton Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Keaton Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Keaton Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Keaton Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Keaton Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Keaton Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Keaton Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        
        # Mountain Village Spring Grotto Grass
        "Mountain Village Spring Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                can_clear_snowhead(state, player, boss_placements)
            ),

        # Great Bay Coast Grotto Grass -
        "Great Bay Coast Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),    
        "Great Bay Coast Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),     
        "Great Bay Coast Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),    
        "Great Bay Coast Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),    
        "Great Bay Coast Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),    
        "Great Bay Coast Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),     
        "Great Bay Coast Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),    
        "Great Bay Coast Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),    
        "Great Bay Coast Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),    
        "Great Bay Coast Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),    
        "Great Bay Coast Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),       
        "Great Bay Coast Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),      
        "Great Bay Coast Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),    
        "Great Bay Coast Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass")
            ),    

        # Great Bay Coast Grass - Requires Epona's Song                                                                                                                                                                      
        "Great Bay Coast Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),  
        "Great Bay Coast Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),  
        "Great Bay Coast Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),   
        "Great Bay Coast Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),  
        "Great Bay Coast Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),  
        
        # Great Bay Coast Cow Grotto Grass Group 1 - Requires Epona's Song and Hookshot
        "Great Bay Coast Cow Grotto Grass Group 1 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        
        # Great Bay Coast Cow Grotto Grass Group 2
        "Great Bay Coast Cow Grotto Grass Group 2 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        
        # Great Bay Coast Cow Grotto Grass Group 3
        "Great Bay Coast Cow Grotto Grass Group 3 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        
        # Great Bay Coast Cow Grotto Grass Group 4
        "Great Bay Coast Cow Grotto Grass Group 4 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        
        # Great Bay Coast Cow Grotto Grass Group 5
        "Great Bay Coast Cow Grotto Grass Group 5 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        
        # Great Bay Coast Cow Grotto Grass Group 6
        "Great Bay Coast Cow Grotto Grass Group 6 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Hookshot", player)
            ),            

        # Zora Cape Grotto Grass - Requires explosives or Goron Mask
        "Zora Cape Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),
        "Zora Cape Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                (has_explosives(state, player) or 
                state.has("Goron Mask", player))
            ),       
        # Road To Ikana Grotto Grass - Requires Goron Mask
        "Road To Ikana Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),
        "Road To Ikana Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player)
            ),             
        # Ikana Graveyard Lower Grass
        "Ikana Graveyard Lower Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Lower Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Lower Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Lower Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Lower Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Ikana Graveyard Upper Grass
        "Ikana Graveyard Upper Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Upper Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Upper Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Upper Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Upper Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Upper Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Upper Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Upper Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Graveyard Upper Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Ikana Graveyard Bombable Grotto Grass - Requires explosives
        "Ikana Graveyard Bombable Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        "Ikana Graveyard Bombable Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") and
                has_explosives(state, player)
            ),
        # Ikana Canyon Grass - Requires Ice Arrows and Hookshot
        "Ikana Canyon Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_ice_arrows(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ikana Canyon Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_ice_arrows(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ikana Canyon Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_ice_arrows(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ikana Canyon Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_ice_arrows(state, player) and 
                state.has("Hookshot", player)
            ),
        
        # Ikana Canyon Grotto Grass
        "Ikana Canyon Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),
        "Ikana Canyon Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and 
                has_soul_absurd(state, player, options, "Grass") 
            ),

        # Secret Shrine Entrance Grass
        "Secret Shrine Entrance Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Entrance Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Entrance Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Entrance Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Entrance Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Entrance Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        # Secret Shrine Dinolfos Grass - Requires Light Arrows
        "Secret Shrine Dinolfos Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Dinolfos Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Dinolfos Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Dinolfos Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        
        # Secret Shrine Wizzrobe Grass - Requires Light Arrows
        "Secret Shrine Wizzrobe Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wizzrobe Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wizzrobe Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wizzrobe Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wizzrobe Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        
        # Secret Shrine Wart Grass - Requires Light Arrows
        "Secret Shrine Wart Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wart Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wart Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wart Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wart Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wart Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wart Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Wart Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        
        # Secret Shrine Garo Master Grass - Requires Light Arrows
        "Secret Shrine Garo Master Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Garo Master Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Garo Master Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Garo Master Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Garo Master Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Garo Master Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                can_use_light_arrows(state, player)
            ),

        # Beneath the Well Left Side Back Room Grass - Matches Invisible Chest rules
        "Beneath the Well Left Side Back Room Grass (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_afford_price(state, player, 100) or 
                    state.has("Mask of Scents", player) and
                    has_soul_npc(state, player, options, "Kotake") 
                )
            ),
        "Beneath the Well Left Side Back Room Grass (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_afford_price(state, player, 100) or 
                    state.has("Mask of Scents", player) and
                    has_soul_npc(state, player, options, "Kotake") 
                )
            ),
        
        # Beneath the Well Right Side Before Big Poe and Cow Grass 
        "Beneath the Well Right Side Before Big Poe and Cow Grass (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_plant_beans(state, player, options) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Beneath the Well Right Side Before Big Poe and Cow Grass (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_plant_beans(state, player, options) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Beneath the Well Right Side Before Big Poe and Cow Grass (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_plant_beans(state, player, options) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Beneath the Well Right Side Before Big Poe and Cow Grass (4)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_plant_beans(state, player, options) or 
                    can_use_light_arrows(state, player)
                )
            ),
        
        # Beneath the Well Right Side Cow Grass 
        "Beneath the Well Right Side Cow Grass (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_play_song("Epona's Song", state, player) and 
                    (
                        can_plant_beans(state, player, options) or 
                        can_use_light_arrows(state, player)
                    ) and 
                    (
                        (
                            can_play_song("Song of Soaring", state, player) and 
                            (
                                (
                                    state.can_reach("Twin Islands", 'Region', player) and 
                                    can_use_fire_arrows(state, player)
                                ) or 
                                (
                                    can_use_lens(state, player) and 
                                    state.has("Goron Mask", player)
                                )
                            )
                        ) or 
                        state.can_reach("Ikana Well Invisible Chest", "Location", player)
                    )
                )
            ),
        "Beneath the Well Right Side Cow Grass (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_play_song("Epona's Song", state, player) and 
                    (
                        can_plant_beans(state, player, options) or 
                        can_use_light_arrows(state, player)
                    ) and 
                    (
                        (
                            can_play_song("Song of Soaring", state, player) and 
                            (
                                (
                                    state.can_reach("Twin Islands", 'Region', player) and 
                                    can_use_fire_arrows(state, player)
                                ) or 
                                (
                                    can_use_lens(state, player) and 
                                    state.has("Goron Mask", player)
                                )
                            )
                        ) or 
                        state.can_reach("Ikana Well Invisible Chest", "Location", player)
                    )
                )
            ),
        "Beneath the Well Right Side Cow Grass (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_play_song("Epona's Song", state, player) and 
                    (
                        can_plant_beans(state, player, options) or 
                        can_use_light_arrows(state, player)
                    ) and 
                    (
                        (
                            can_play_song("Song of Soaring", state, player) and 
                            (
                                (
                                    state.can_reach("Twin Islands", 'Region', player) and 
                                    can_use_fire_arrows(state, player)
                                ) or 
                                (
                                    can_use_lens(state, player) and 
                                    state.has("Goron Mask", player)
                                )
                            )
                        ) or 
                        state.can_reach("Ikana Well Invisible Chest", "Location", player)
                    )
                )
            ),
        
        # Beneath the Well Right Side Back Room Grass
        "Beneath the Well Right Side Back Room Grass (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    (
                        can_plant_beans(state, player, options) and
                        (
                            state.has("Progressive Bomb Bag", player) or
                            (
                                state.has("Captain's Hat", player) and 
                                state.has("Progressive Bow", player)
                            )
                        )
                    ) or 
                    (
                        can_use_light_arrows(state, player) and
                        can_use_fire_arrows(state, player)
                    )
                )
            ),
        "Beneath the Well Right Side Back Room Grass (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    (
                        can_plant_beans(state, player, options) and
                        (
                            state.has("Progressive Bomb Bag", player) or
                            (
                                state.has("Captain's Hat", player) and 
                                state.has("Progressive Bow", player)
                            )
                        )
                    ) or 
                    (
                        can_use_light_arrows(state, player) and
                        can_use_fire_arrows(state, player)
                    )
                )
            ),
        "Beneath the Well Right Side Back Room Grass (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    (
                        can_plant_beans(state, player, options) and
                        (
                            state.has("Progressive Bomb Bag", player) or
                            (
                                state.has("Captain's Hat", player) and 
                                state.has("Progressive Bow", player)
                            )
                        )
                    ) or 
                    (
                        can_use_light_arrows(state, player) and
                        can_use_fire_arrows(state, player)
                    )
                )
            ),
        "Beneath the Well Right Side Back Room Grass (4)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    (
                        can_plant_beans(state, player, options) and
                        (
                            state.has("Progressive Bomb Bag", player) or
                            (
                                state.has("Captain's Hat", player) and 
                                state.has("Progressive Bow", player)
                            )
                        )
                    ) or 
                    (
                        can_use_light_arrows(state, player) and
                        can_use_fire_arrows(state, player)
                    )
                )
            ),
        "Beneath the Well Right Side Back Room Grass (5)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    (
                        can_plant_beans(state, player, options) and
                        (
                            state.has("Progressive Bomb Bag", player) or
                            (
                                state.has("Captain's Hat", player) and 
                                state.has("Progressive Bow", player)
                            )
                        )
                    ) or 
                    (
                        can_use_light_arrows(state, player) and
                        can_use_fire_arrows(state, player)
                    )
                )
            ),
        # Ikana Castle Grass - Requires Mirror Shield or Light Arrows
        "Ikana Castle Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),
        "Ikana Castle Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (
                    has_mirror_shield(state, player) or 
                    can_use_light_arrows(state, player)
                )
            ),

        # Woodfall Temple Grass
        "Woodfall Temple Entrance Room Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Entrance Room Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Entrance Room Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                (
                    state.has("Deku Mask", player) or
                    state.has("Hookshot", player)
                )
            ),
        "Woodfall Temple Entrance Room Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                (
                    state.has("Deku Mask", player) or
                    state.has("Hookshot", player)
                )
            ),
        "Woodfall Temple Entrance Room Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Main Room Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                (
                    state.has("Deku Mask", player) or
                    state.has("Hookshot", player)
                )
            ),
        "Woodfall Temple Main Room Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                (
                    state.has("Deku Mask", player) or
                    state.has("Hookshot", player)
                )
            ),
        "Woodfall Temple Main Room Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                (
                    state.has("Deku Mask", player) or
                    state.has("Hookshot", player)
                )
            ),
        "Woodfall Temple Deku Elevator Room Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Deku Elevator Room Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Snapping Turtle Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Snapping Turtle Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Snapping Turtle Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Snapping Turtle Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Snapping Turtle Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Dragonfly Chest Room Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (state.has("Small Key (Woodfall)", player) or 
                (
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player) and
                    state.has("Progressive Bow", player))
                )
            ),
        "Woodfall Temple Dragonfly Chest Room Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (state.has("Small Key (Woodfall)", player) or 
                (
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player) and
                    state.has("Progressive Bow", player))
                )
            ),
        "Woodfall Temple Dragonfly Chest Room Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                (state.has("Small Key (Woodfall)", player) or 
                (
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player) and
                    state.has("Progressive Bow", player))
                )
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Pre Boss Room Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Pre Boss Room Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Pre Boss Room Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Pre Boss Room Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (15)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
        "Woodfall Temple Odolwas Lair Grass (16)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Progressive Bow", player) and
                state.has("Boss Key (Woodfall)", player)
            ),
            
        # Snowhead Temple Basement Grass
        "Snowhead Temple Basement Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Snowhead Temple Basement Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Snowhead Temple Basement Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Snowhead Temple Basement Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Snowhead Temple Basement Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Snowhead Temple Basement Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Snowhead Temple Basement Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Snowhead Temple Basement Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Snowhead Temple Basement Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Snowhead Temple Basement Grass (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        # Stone Tower Temple Entrance Room Grass
        "Stone Tower Temple Entrance Room Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Stone Tower Temple Entrance Room Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Stone Tower Temple Entrance Room Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        
        # Stone Tower Temple Elegy Maze Grass - Requires Goron, Zora, Elegy and Explosives
        "Stone Tower Temple Elegy Maze Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player) and 
                state.has("Zora Mask", player) and 
                can_play_song("Elegy of Emptiness", state, player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Elegy Maze Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player) and 
                state.has("Zora Mask", player) and 
                can_play_song("Elegy of Emptiness", state, player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Elegy Maze Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player) and 
                state.has("Zora Mask", player) and 
                can_play_song("Elegy of Emptiness", state, player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Elegy Maze Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player) and 
                state.has("Zora Mask", player) and 
                can_play_song("Elegy of Emptiness", state, player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Elegy Maze Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player) and 
                state.has("Zora Mask", player) and 
                can_play_song("Elegy of Emptiness", state, player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Elegy Maze Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                state.has("Goron Mask", player) and 
                state.has("Zora Mask", player) and 
                can_play_song("Elegy of Emptiness", state, player) and
                has_explosives(state, player)
            ),

        # Clock Town Pots
        "Trading Post Pot":
            lambda state: has_soul_absurd(state, player, options, "Pots"),

        # Sword School Night 3 Midnight Pots - Requires Progressive Sword
        "Sword School Night 3 Midnight Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Progressive Sword", player)
            ),
        "Sword School Night 3 Midnight Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Progressive Sword", player)
            ),
        "Sword School Night 3 Midnight Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Progressive Sword", player)
            ),
        "Sword School Night 3 Midnight Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Progressive Sword", player)
            ),
        "Sword School Night 3 Midnight Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Progressive Sword", player)
            ),

        # Clock Tower Pots Night 3
        "Top of Clock Tower Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_warp_out(state, player, options) or
                    can_play_song("Song of Time", state, player)
                )
            ),    
        "Top of Clock Tower Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_warp_out(state, player, options) or
                    can_play_song("Song of Time", state, player)
                )
            ),    
        "Top of Clock Tower Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_warp_out(state, player, options) or
                    can_play_song("Song of Time", state, player)
                )
            ),    
        "Top of Clock Tower Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_warp_out(state, player, options) or
                    can_play_song("Song of Time", state, player)
                )
            ),                              
        # Bombers Hideout Pots - Requires access to Bomber's Hideout Astral Observatory
        "Bombers Hideout Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player)
            ),
        "Bombers Hideout Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player)
            ),
        "Bombers Hideout Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player)
            ),
        "Bombers Hideout Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player)
            ),
        
        # Astral Observatory Pots - Requires access to Bomber's Hideout Astral Observatory
        "Astral Observatory Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player)
            ),
        "Astral Observatory Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player)
            ),
        "Astral Observatory Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player)
            ),

        # Termina Field Pots
        
        # Termina Field Eastern Pillar Pot
        "Termina Field Eastern Pillar Pot":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_projectiles(state, player)
            ),
        # Termina Field Deku Business Scrub Grotto Pot
        "Termina Field Deku Business Scrub Grotto Pot":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Pots")
            ),
        # Southern Swamp Pots
        
        # Road to Southern Swamp Outside Archery Pots
        "Road to Southern Swamp Outside Archery Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Road to Southern Swamp Outside Archery Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        # Southern Swamp Beneath Witch Shop Pots
        "Southern Swamp Beneath Witch Shop Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Southern Swamp Beneath Witch Shop Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Southern Swamp Beneath Witch Shop Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        # Swamp Spider House Pots
        
        # Swamp Spider House Main Room Pots
        "Swamp Spider House Main Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Main Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Main Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Main Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Main Room Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Main Room Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Main Room Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Main Room Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),

        # Swamp Spider House Tablet Room Pots
        "Swamp Spider House Tablet Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Tablet Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),

        # Swamp Spider House Giant Jar Room Pots
        "Swamp Spider House Giant Jar Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Giant Jar Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Giant Jar Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Giant Jar Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Giant Jar Room Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Giant Jar Room Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Giant Jar Room Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Giant Jar Room Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),           
        
        # Swamp Spider House Gold Room Pots
        "Swamp Spider House Gold Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Gold Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Gold Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Gold Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Gold Room Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Gold Room Pots (6)":    
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        # Deku Palace Pots
        
        # Deku Palace Right Side Upper Pots
        "Deku Palace Right Side Upper Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.can_reach("Deku Palace", 'Region', player)
            ),
        "Deku Palace Right Side Upper Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.can_reach("Deku Palace", 'Region', player)
            ),
        # Deku Butler Race Pots
        "Deku Butler Race Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_clear_woodfall(state, player, boss_placements) and 
                state.has("Progressive Sword", player) and 
                has_bottle(state, player)
            ),
        "Deku Butler Race Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_clear_woodfall(state, player, boss_placements) and 
                state.has("Progressive Sword", player) and 
                has_bottle(state, player)
            ),
        # Woodfall Pots
        
        # Woodfall Owl Pots - Requires access to Woodfall region as Deku
        "Woodfall Owl Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    state.has("Deku Mask", player) or
                    options.owlsanity.value and
                    can_use_owl(state, player, options, "Woodfall") and 
                    can_warp_out(state, player, options)
                )
            ),
        "Woodfall Owl Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    state.has("Deku Mask", player) or
                    options.owlsanity.value and
                    can_use_owl(state, player, options, "Woodfall") and 
                    can_warp_out(state, player, options)
                )
            ),
        "Woodfall Owl Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    state.has("Deku Mask", player) or
                    options.owlsanity.value and
                    can_use_owl(state, player, options, "Woodfall") and 
                    can_warp_out(state, player, options)
                )
            ),
        # Woodfall Temple Pots
        
        # Woodfall Temple Entrance Pot
        "Woodfall Temple Entrance Pot":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        # Woodfall Temple Main Room Pots
        "Woodfall Temple Main Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Main Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Main Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Main Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Main Room Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Main Room Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Main Room Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Main Room Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        # Woodfall Temple Deku Elevator Pots
        "Woodfall Temple Deku Elevator Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Deku Elevator Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Deku Elevator Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Deku Elevator Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Progressive Bow", player)
            ),
        
        # Woodfall Temple Gekko Pots - Requires bow to access this area
        "Woodfall Temple Gekko Pots (1)":
            lambda state: (
                has_soul_enemy(state, player, options, "Gekko") and
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Gekko Pots (2)":
            lambda state: (
                has_soul_enemy(state, player, options, "Gekko") and
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Gekko Pots (3)":
            lambda state: (
                has_soul_enemy(state, player, options, "Gekko") and
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Gekko Pots (4)":
            lambda state: (
                has_soul_enemy(state, player, options, "Gekko") and
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Progressive Bow", player)
            ),
        # Woodfall Temple Left Side Bridge Pots - Requires small key or bow
        "Woodfall Temple Left Side Bridge Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Small Key (Woodfall)", player) or 
                    state.has("Progressive Bow", player)
                )
            ),
        "Woodfall Temple Left Side Bridge Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Small Key (Woodfall)", player) or 
                    state.has("Progressive Bow", player)
                )
            ),
        
        # Woodfall Temple Pre Boss Pots - Requires bow to reach pre-boss area
        "Woodfall Temple Pre Boss Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Pre Boss Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Progressive Bow", player)
            ),
        #Southern  Swamp Post Dungeon Pots
        "Southern Swamp Post Dungeon Witch Pot (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Witch Pot (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                can_clear_woodfall(state, player, boss_placements)
            ),
        "Southern Swamp Post Dungeon Witch Pot (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                can_clear_woodfall(state, player, boss_placements)
            ),
        # Mountain Village Pots
        
        "Mountain Village Rooftop Pot":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_hard_projectiles(state, player)
            ),
        "Mountain Village Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Mountain Village Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        # Mountain Smithy Pots
        "Mountain Smithy Pots Inside at Night (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Mountain Smithy Pots Inside at Night (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        # Goron Village Pots
        
        "Goron Racetrack Pots (1)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (2)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (3)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (4)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (5)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (6)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (7)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (8)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (9)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (10)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (11)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (12)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (13)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (14)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (15)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (16)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (17)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (18)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (19)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (20)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (21)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (22)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (23)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (24)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (25)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (26)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (27)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (28)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (29)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Goron Racetrack Pots (30)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    state.has("Goron Mask", player) and 
                (
                    state.has("Powder Keg", player) or
                    can_use_fire_arrows(state, player)
                )
            ),
        
        # Goron Shrine Pots 
        "Goron Shrine Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
        "Goron Shrine Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
        "Goron Shrine Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
        "Goron Shrine Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
        "Goron Shrine Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
        "Goron Shrine Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
        "Goron Shrine Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
        "Goron Shrine Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
        "Goron Shrine Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
        "Goron Shrine Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
        "Goron Shrine Pots (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    has_soul_npc(state, player, options, "Gatekeeper & Medigoron") or 
                    state.has("Goron Mask", player)
                )
            ),
                        
        # Snowhead Temple Pots
        
        # Snowhead Temple Blue Door Lava Bridge Pots 
        "Snowhead Temple Entrance Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),      
        "Snowhead Temple Entrance Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"), 
        # Snowhead Temple Pots
        
        # Snowhead Temple Blue Door Lava Bridge Pots - Basic temple access
        "Snowhead Temple Blue Door Lava Bridge Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"), 
        "Snowhead Temple Blue Door Lava Bridge Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"), 
        "Snowhead Temple Blue Door Lava Bridge Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"), 
        "Snowhead Temple Blue Door Lava Bridge Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"), 
        "Snowhead Temple Blue Door Lava Bridge Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"), 
        "Snowhead Temple Blue Door Lava Bridge Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"), 
        "Snowhead Temple Blue Door Lava Bridge Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"), 
        
        # Snowhead Temple Main Room Pots Basement - Basic temple access
        "Snowhead Temple Main Room Pots Basement (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"), 
        "Snowhead Temple Main Room Pots Basement (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"), 
        
        # Snowhead Temple Main Room Scarecrow Pots - Requires Hookshot or Fire Arrows
        "Snowhead Temple Main Room Scarecrow Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Hookshot", player) and 
                    state.has("Snowhead Temple Lower Scarecrow", player) and
                    has_soul_npc(state, player, options, "Scarecrow") or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Snowhead Temple Main Room Scarecrow Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Hookshot", player) and 
                    state.has("Snowhead Temple Lower Scarecrow", player) and
                    has_soul_npc(state, player, options, "Scarecrow") or
                    can_use_fire_arrows(state, player)
                )
            ),
        
        # Snowhead Temple Frozen Green Door Pots - Basic temple access (pots are at bottom)
        "Snowhead Temple Frozen Green Door Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (8)":
            lambda state:
            (  
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player)
            ),
        "Snowhead Temple Frozen Green Door Pots (9)":
            lambda state:
            (  
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player)
            ),
        "Snowhead Temple Frozen Green Door Pots (10)":
            lambda state:
            (  
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player)
            ),
        "Snowhead Temple Frozen Green Door Pots (11)":
            lambda state:
            (  
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player)
            ),
        "Snowhead Temple Frozen Green Door Pots (12)":
            lambda state:
            (  
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player)
            ),
        "Snowhead Temple Frozen Green Door Pots (13)":
            lambda state:
            (  
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player)
            ),
        
        # Snowhead Temple Orange Door Push Block Pots - Requires Fire Arrows and (Small Key or Hookshot)
        "Snowhead Temple Orange Door Push Block Pots (1)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    can_use_fire_arrows(state, player) and
                (
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),  
        "Snowhead Temple Orange Door Push Block Pots (2)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    can_use_fire_arrows(state, player) and
                (
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),  
        "Snowhead Temple Orange Door Push Block Ghost Pots (1)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    can_use_fire_arrows(state, player) and
                (
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),  
        "Snowhead Temple Orange Door Push Block Ghost Pots (2)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Pots") and
                    can_use_fire_arrows(state, player) and
                (
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),                        

        # Snowhead Temple Locked Grey Door Wolfos Pots - Requires 2 Small Keys
        "Snowhead Temple Locked Grey Door Wolfos Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple Locked Grey Door Wolfos Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple Locked Grey Door Wolfos Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple Locked Grey Door Wolfos Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple Locked Grey Door Wolfos Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Small Key (Snowhead)", player, 2)
            ),
        
        # Snowhead Temple Goron Pound Puzzle Pots - Requires explosives and (Goron Mask or Fire Arrows)
        "Snowhead Temple Goron Pound Puzzle Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and
                (
                    state.has("Small Key (Snowhead)", player, 2) or
                    can_use_fire_arrows(state, player)
                )
            ),
        "Snowhead Temple Goron Pound Puzzle Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and
                (
                    state.has("Small Key (Snowhead)", player, 2) or
                    can_use_fire_arrows(state, player)
                )
            ),
        
        # Snowhead Temple Main Room Pots 2nd Floor Bridge - Basic temple access
        "Snowhead Temple Main Room 2nd Floor Bridge Pots (1)":
            lambda state: ( 
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Small Key (Snowhead)", player, 2) or
                    can_use_fire_arrows(state, player)
                )
            ),        
        "Snowhead Temple Main Room 2nd Floor Bridge Pots (2)":
            lambda state: ( 
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Small Key (Snowhead)", player, 2) or
                    can_use_fire_arrows(state, player)
                )
            ),   
        # Snowhead Temple Main Room 4th Floor Pots - Same as Boss Key Chest requirements
        "Snowhead Temple Main Room 4th Floor Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                )
            ),
        "Snowhead Temple Main Room 4th Floor Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                )
            ),
        
        # Snowhead Temple 4th Floor Wizzrobe Pots - Same as Boss Key Chest requirements
        "Snowhead Temple 4th Floor Wizzrobe Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                )
            ),
        "Snowhead Temple 4th Floor Wizzrobe Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                )
            ),
        "Snowhead Temple 4th Floor Wizzrobe Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                )
            ),
        "Snowhead Temple 4th Floor Wizzrobe Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                )
            ),
        "Snowhead Temple 4th Floor Wizzrobe Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                )
            ),                      
        
        # Goht Boss Room Pots - Top floor requirements plus Boss Key or Remains warp
        "Goht Boss Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Goht Boss Room Pots (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options)
                    )
                ) and 
                (
                    state.has("Boss Key (Snowhead)", player) or 
                    (
                        state.has("Goht's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),

        "Mountain Village Springtime Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Springtime Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_clear_snowhead(state, player, boss_placements)
            ),

        # Romani Ranch Pots
        
        # Romani Ranch Baby Chickens Pots
        "Romani Ranch Baby Cuccoos Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Romani Ranch Baby Cuccoos Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        # Romani Ranch Doggy Racetrack Pots
        "Romani Ranch Doggy Racetrack Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Romani Ranch Doggy Racetrack Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Romani Ranch Doggy Racetrack Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Romani Ranch Doggy Racetrack Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),

        # Great Bay Coast Pots
        "Great Bay Coast Behind Marine Lab Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Coast Behind Marine Lab Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),           
        "Great Bay Coast Behind Marine Lab Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Coast Behind Marine Lab Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),

        "Great Bay Coast Rock Pools Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Coast Rock Pools Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),        
            
        "Great Bay Coast Lower Rock Wall Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Coast Lower Rock Wall Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),       
        "Great Bay Coast Lower Rock Wall Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),       
        "Great Bay Coast Lower Rock Wall Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),       

        "Great Bay Coast Upper Rock Wall Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Upper Rock Wall Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Upper Rock Wall Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player)
            ),  

        "Great Bay Coast Beside Pirates Fortress Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),                   
        "Great Bay Coast Beside Pirates Fortress Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),           
        # Ocean Spider House Pots - All require region access, explosives, and hookshot

        # Ocean Spiderhouse Bottom of Ramp Pots
        "Ocean Spiderhouse Bottom of Ramp Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_explosives(state, player)
            ),
        "Ocean Spiderhouse Bottom of Ramp Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_explosives(state, player)
            ),
        "Ocean Spiderhouse Bottom of Ramp Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_explosives(state, player)
            ),
        "Ocean Spiderhouse Bottom of Ramp Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_explosives(state, player)
            ),
        
        # Ocean Spiderhouse Main Room Lower Pots
        "Ocean Spiderhouse Main Room Lower Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Main Room Lower Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Main Room Lower Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Main Room Lower Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Main Room Lower Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        
        # Ocean Spiderhouse Crate Room Pots
        "Ocean Spiderhouse Crate Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Crate Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Crate Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Crate Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Crate Room Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Crate Room Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Crate Room Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Crate Room Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        
        # Ocean Spiderhouse Coloured Skulls Room Pots
        "Ocean Spiderhouse Coloured Skulls Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Coloured Skulls Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_explosives(state, player) and 
                state.has("Hookshot", player)
            ),

            
        # Pinnacle Rock Pots - Requires reaching seahorse, bottle, and Zora Mask
        
        "Pinnacle Rock Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Pinnacle Rock Pots (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Fisherman") and
                can_reach_seahorse(state, player, options) and 
                has_bottle(state, player) and 
                state.has("Zora Mask", player)
            ),
        # Pirates' Fortress Pots
        
        # Pirates Fortress Sewers Cage Room Pots - Requires Goron to reach cage room
        "Pirates Fortress Sewers Cage Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Barrels") and
                state.has("Goron Mask", player)
            ),
        "Pirates Fortress Sewers Cage Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Barrels") and
                state.has("Goron Mask", player)
            ),
        
        # Pirates Fortress Sewers After Gate Hidden Ladder Pots
        "Pirates Fortress Sewers After Gate Hidden Ladder Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Goron Mask", player) and
                    has_soul_absurd(state, player, options, "Barrels") or 
                    state.has("Hookshot", player)
                )
            ),
        "Pirates Fortress Sewers After Gate Hidden Ladder Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Goron Mask", player) and
                    has_soul_absurd(state, player, options, "Barrels") or 
                    state.has("Hookshot", player)
                )
            ),
        
        # Pirates Fortress Sewers Exit Pots
        "Pirates Fortress Sewers Exit Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Goron Mask", player) and
                    has_soul_absurd(state, player, options, "Barrels") or 
                    state.has("Hookshot", player)
                )
            ),
        "Pirates Fortress Sewers Exit Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Goron Mask", player) and
                    has_soul_absurd(state, player, options, "Barrels") or 
                    state.has("Hookshot", player)
                )
            ),
        "Pirates Fortress Sewers Exit Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Goron Mask", player) and
                    has_soul_absurd(state, player, options, "Barrels") or 
                    state.has("Hookshot", player)
                )
            ),
        
        # Pirates Fortress Interior Underwater Chest Room Pots - Requires Hookshot and can_smack_hard
        "Pirates Fortress Interior Underwater Chest Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        "Pirates Fortress Interior Underwater Chest Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        "Pirates Fortress Interior Underwater Chest Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        
        # Pirates Fortress Interior Room Past Green Guard Pots
        "Pirates Fortress Interior Room Past Green Guard Pots (1)":
            lambda state: (
                has_soul_enemy(state, player, options, "Coloured Pirates") and
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        "Pirates Fortress Interior Room Past Green Guard Pots (2)":
            lambda state: (
                has_soul_enemy(state, player, options, "Coloured Pirates") and
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        "Pirates Fortress Interior Room Past Green Guard Pots (3)":
            lambda state: (
                has_soul_enemy(state, player, options, "Coloured Pirates") and
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        
        # Pirates Fortress Interior Upper Beehive Room Pots
        "Pirates Fortress Interior Upper Beehive Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        "Pirates Fortress Interior Upper Beehive Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        
        # Pirates Fortress Interior Room Past Pink Guard Pots
        "Pirates Fortress Interior Room Past Pink Guard Pots (1)":
            lambda state: (
                has_soul_enemy(state, player, options, "Coloured Pirates") and
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        "Pirates Fortress Interior Room Past Pink Guard Pots (2)":
            lambda state: (
                has_soul_enemy(state, player, options, "Coloured Pirates") and
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and 
                can_smack_hard(state, player)
            ),
        # Zora Cape Pots
        # Zora Cape Like Like Pool Pots 
        "Zora Cape Like Like Pool Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Zora Cape Like Like Pool Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        # Zora Cape Owl Pots - Requires Zora Mask
        "Zora Cape Owl Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Zora Hall", 'Region', player)
            ),
        "Zora Cape Owl Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Zora Hall", 'Region', player)
            ),
        "Zora Cape Owl Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Zora Hall", 'Region', player)
            ),
        "Zora Cape Owl Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Zora Hall", 'Region', player)
            ),

        "Zora Cape Jar Game Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
                (
                    state.has("Zora Mask", player) or
                    state.has("Goron Mask", player) and
                    can_use_powder_keg(state, player, options)
                )
            ),        
        "Zora Cape Jar Game Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
                (
                    state.has("Zora Mask", player) or
                    state.has("Goron Mask", player) and
                    can_use_powder_keg(state, player, options)
                )
            ), 
        "Zora Cape Jar Game Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
                (
                    state.has("Zora Mask", player) or
                    state.has("Goron Mask", player) and
                    can_use_powder_keg(state, player, options)
                )
            ), 
        "Zora Cape Jar Game Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
                (
                    state.has("Zora Mask", player) or
                    state.has("Goron Mask", player) and
                    can_use_powder_keg(state, player, options)
                )
            ), 
        "Zora Cape Jar Game Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
                (
                    state.has("Zora Mask", player) or
                    state.has("Goron Mask", player) and
                    can_use_powder_keg(state, player, options)
                )
            ), 


# Great Bay Temple Pots
        
        # Great Bay Temple Above Whirlpool Pots
        "Great Bay Temple Above Blender Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Above Blender Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        # Great Bay Temple Room Behind 1F Waterfall 
        "Great Bay Temple Room Behind 1F Waterfall Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Room Behind 1F Waterfall Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Room Behind 1F Waterfall Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Room Behind 1F Waterfall Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        
        # Great Bay Temple Red Green Pipe Tunnel Room Pots 
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_ice_arrows(state, player)
            ),
        
        # Great Bay Temple Before Gekko Room Pots 
        "Great Bay Temple Before Gekko Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Before Gekko Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Before Gekko Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Before Gekko Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Before Gekko Room Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Before Gekko Room Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Before Gekko Room Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        # Great Bay Temple Red Valve Underwater Pots 
        "Great Bay Temple Red Valve Underwater Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Valve Underwater Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Valve Underwater Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Valve Underwater Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        # Great Bay Temple Behind Locked Door Pots - Requires Small Key
        "Great Bay Temple Behind Locked Door Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Behind Locked Door Pots (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        
        # Great Bay Temple Wart Room Pots 
        "Great Bay Temple Wart Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Wart Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Wart Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Wart Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Wart Room Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Wart Room Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Wart Room Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        "Great Bay Temple Wart Room Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Small Key (Great Bay)", player)
            ),
        
        # Great Bay Temple Green Pipe Frozen Waterwheel Pots 
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (1)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (2)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (3)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (4)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (5)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (6)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (7)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (8)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player)
            ),
        
        # Great Bay Temple Seesaw Room Pots - Requires both Ice and Fire Arrows
        "Great Bay Temple Seesaw Room Pots (1)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Seesaw Room Pots (2)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Seesaw Room Pots (3)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        
        # Great Bay Temple Pots Underneath Boss Door Platform - Requires both Ice and Fire Arrows
        "Great Bay Temple Pots Underneath Boss Door Platform (1)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (2)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (3)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (4)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (5)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (6)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (7)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (8)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        
        # Great Bay Temple Gyorg Pots - Requires Boss Key or Remains warp
        "Great Bay Temple Gyorg Pots (1)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    state.has("Boss Key (Great Bay)", player) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Great Bay Temple Gyorg Pots (2)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    state.has("Boss Key (Great Bay)", player) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Great Bay Temple Gyorg Pots (3)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    state.has("Boss Key (Great Bay)", player) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Great Bay Temple Gyorg Pots (4)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    state.has("Boss Key (Great Bay)", player) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Great Bay Temple Gyorg Pots (5)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    state.has("Boss Key (Great Bay)", player) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Great Bay Temple Gyorg Pots (6)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    state.has("Boss Key (Great Bay)", player) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Great Bay Temple Gyorg Pots (7)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    state.has("Boss Key (Great Bay)", player) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),
        "Great Bay Temple Gyorg Pots (8)":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                has_soul_absurd(state, player, options, "Pots") and 
                (
                    state.has("Boss Key (Great Bay)", player) or 
                    (
                        state.has("Gyorg's Remains", player) and 
                        options.remains_allow_boss_warps.value
                    )
                )
            ),

        
        # IKANA POTS
        "Road To Ikana Scarecrow Pillar Pot":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                state.has("Hookshot", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Road to Ikana Scarecrow", player) and
                (
                    can_play_song("Epona's Song", state, player) or
                    can_use_owl(state, player, options, "Ikana Canyon") or
                    can_use_owl(state, player, options, "Stone Tower")
                )
            ),
        # Ikana Graveyard Day 1 Grave Pots
        "Ikana Graveyard Day 1 Grave Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 1 Grave Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 1 Grave Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 1 Grave Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 1 Grave Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        
        # Ikana Graveyard Day 2 Entrance Grave Pot
        "Ikana Graveyard Day 2 Entrance Grave Pot":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        # Ikana Graveyard Day 2 Invisible Path Entryway Pots
        "Ikana Graveyard Day 2 Invisible Path Entryway Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 2 Invisible Path Entryway Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and
                has_explosives(state, player)
            ),
        
        # Ikana Graveyard Day 2 Invisible Path Pots
        "Ikana Graveyard Day 2 Invisible Path Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and
                can_use_lens(state, player)
            ),
        "Ikana Graveyard Day 2 Invisible Path Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and
                can_use_lens(state, player)
            ),
        "Ikana Graveyard Day 2 Invisible Path Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and
                can_use_lens(state, player)
            ),
        "Ikana Graveyard Day 2 Invisible Path Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and
                can_use_lens(state, player)
            ),
        
        # Ikana Graveyard Day 3 Pots
        "Ikana Graveyard Day 3 Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 3 Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 3 Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 3 Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 3 Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 3 Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 3 Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 3 Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 3 Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        "Ikana Graveyard Day 3 Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and 
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player)
            ),
        # SECRET SHRINE POTS
        "Secret Shrine Entrance Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Secret Shrine Entrance Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Secret Shrine Entrance Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        "Secret Shrine Underwater Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_light_arrows(state, player)
            ),
        "Secret Shrine Underwater Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_light_arrows(state, player) and
                can_smack_hard(state, player) and
                state.has("Zora Mask", player)
            ),
        "Secret Shrine Underwater Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_light_arrows(state, player) and
                can_smack_hard(state, player) and
                state.has("Zora Mask", player)
            ),
        "Secret Shrine Underwater Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_light_arrows(state, player) and
                can_smack_hard(state, player) and
                state.has("Zora Mask", player)
            ),
        "Secret Shrine Underwater Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_light_arrows(state, player) and
                can_smack_hard(state, player) and
                state.has("Zora Mask", player)
            ),
        "Secret Shrine Underwater Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_light_arrows(state, player) and
                can_smack_hard(state, player) and
                state.has("Zora Mask", player)
            ),

        "Sakons Hideout Pots (1)":
            lambda state: (
                has_soul_enemy(state, player, options, "Deku Baba") and
                has_soul_enemy(state, player, options, "Wolfos") and
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stock Pot Inn Anju and Kafei", "Location", player)
            ),
        "Sakons Hideout Pots (2)":
            lambda state: (
                has_soul_enemy(state, player, options, "Deku Baba") and
                has_soul_enemy(state, player, options, "Wolfos") and
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stock Pot Inn Anju and Kafei", "Location", player)
            ),
        "Sakons Hideout Pots (3)":
            lambda state: (
                has_soul_enemy(state, player, options, "Deku Baba") and
                has_soul_enemy(state, player, options, "Wolfos") and
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stock Pot Inn Anju and Kafei", "Location", player)
            ),
        "Sakons Hideout Pots (4)":
            lambda state: (
                has_soul_enemy(state, player, options, "Deku Baba") and
                has_soul_enemy(state, player, options, "Wolfos") and
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stock Pot Inn Anju and Kafei", "Location", player)
            ),
        "Sakons Hideout Pots (5)":
            lambda state: (
                has_soul_enemy(state, player, options, "Deku Baba") and
                has_soul_enemy(state, player, options, "Wolfos") and
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stock Pot Inn Anju and Kafei", "Location", player)
            ),

        # IKANA CASTLE POTS
        "Ikana Castle Exterior Corner Pot":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
                ),
        "Ikana Castle Frozen Eyes Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Castle Frozen Eyes Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and
                (
                    can_use_light_arrows(state, player) or
                    has_mirror_shield(state, player)
                )
            ),
        "Ikana Castle Left Side Falling Ceiling Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player)
            ),
        "Ikana Castle Left Side Falling Ceiling Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player)
            ),
        "Ikana Castle Left Side Broken Floor Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and
                state.has("Deku Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Ikana Castle Left Side Broken Floor Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and
                state.has("Deku Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Ikana Castle Left Side Broken Floor Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and
                state.has("Deku Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Ikana Castle Left Side Broken Floor Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and
                state.has("Deku Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Ikana Castle Left Side Staircase Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and
                state.has("Deku Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Ikana Castle Left Side Staircase Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player) and
                state.has("Deku Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        
        "Ikana Castle Right Side Staircase Pots (1)":
            lambda state: (
                has_soul_enemy(state, player, options, "Wizrobe") and
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player)
            ),
        "Ikana Castle Right Side Staircase Pots (2)":
            lambda state: (
                has_soul_enemy(state, player, options, "Wizrobe") and
                has_soul_absurd(state, player, options, "Pots") and
                can_use_fire_arrows(state, player)
            ),
        # Ikana Castle Throne Room Pots
        "Ikana Castle Throne Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_light_arrows(state, player) or
                    (
                        has_mirror_shield(state, player) and
                        has_soul_absurd(state, player, options, "Deku Flowers") and
                        state.has("Deku Mask", player) and
                        can_use_lens(state, player) and
                        can_use_fire_arrows(state, player) and
                        state.has("Powder Keg", player) and
                        state.has("Goron Mask", player)
                    )
                )
            ),
        "Ikana Castle Throne Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_light_arrows(state, player) or
                    (
                        has_mirror_shield(state, player) and
                        has_soul_absurd(state, player, options, "Deku Flowers") and
                        state.has("Deku Mask", player) and
                        can_use_lens(state, player) and
                        can_use_fire_arrows(state, player) and
                        state.has("Powder Keg", player) and
                        state.has("Goron Mask", player)
                    )
                )
            ),
        "Ikana Castle Throne Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_light_arrows(state, player) or
                    (
                        has_mirror_shield(state, player) and
                        has_soul_absurd(state, player, options, "Deku Flowers") and
                        state.has("Deku Mask", player) and
                        can_use_lens(state, player) and
                        can_use_fire_arrows(state, player) and
                        state.has("Powder Keg", player) and
                        state.has("Goron Mask", player)
                    )
                )
            ),
        "Ikana Castle Throne Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_light_arrows(state, player) or
                    (
                        has_mirror_shield(state, player) and
                        has_soul_absurd(state, player, options, "Deku Flowers") and
                        state.has("Deku Mask", player) and
                        can_use_lens(state, player) and
                        can_use_fire_arrows(state, player) and
                        state.has("Powder Keg", player) and
                        state.has("Goron Mask", player)
                    )
                )
            ),
        "Ikana Castle Throne Room Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_light_arrows(state, player) or
                    (
                        has_mirror_shield(state, player) and
                        has_soul_absurd(state, player, options, "Deku Flowers") and
                        state.has("Deku Mask", player) and
                        can_use_lens(state, player) and
                        can_use_fire_arrows(state, player) and
                        state.has("Powder Keg", player) and
                        state.has("Goron Mask", player)
                    )
                )
            ),
        "Ikana Castle Throne Room Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_light_arrows(state, player) or
                    (
                        has_mirror_shield(state, player) and
                        has_soul_absurd(state, player, options, "Deku Flowers") and
                        state.has("Deku Mask", player) and
                        can_use_lens(state, player) and
                        can_use_fire_arrows(state, player) and
                        state.has("Powder Keg", player) and
                        state.has("Goron Mask", player)
                    )
                )
            ),
        "Ikana Castle Throne Room Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_light_arrows(state, player) or
                    (
                        has_mirror_shield(state, player) and
                        has_soul_absurd(state, player, options, "Deku Flowers") and
                        state.has("Deku Mask", player) and
                        can_use_lens(state, player) and
                        can_use_fire_arrows(state, player) and
                        state.has("Powder Keg", player) and
                        state.has("Goron Mask", player)
                    )
                )
            ),
        "Ikana Castle Throne Room Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_light_arrows(state, player) or
                    (
                        has_mirror_shield(state, player) and
                        has_soul_absurd(state, player, options, "Deku Flowers") and
                        state.has("Deku Mask", player) and
                        can_use_lens(state, player) and
                        can_use_fire_arrows(state, player) and
                        state.has("Powder Keg", player) and
                        state.has("Goron Mask", player)
                    )
                )
            ),
        # Well Pots
        "Well Left Side Back Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_afford_price(state, player, 100) or
                    state.has("Mask of Scents", player)
                )
            ),
        "Well Left Side Back Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_afford_price(state, player, 100) or
                    state.has("Mask of Scents", player)
                )
            ),
        "Well Left Side Back Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_afford_price(state, player, 100) or
                    state.has("Mask of Scents", player)
                )
            ),
        "Well Left Side Back Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_afford_price(state, player, 100) or
                    state.has("Mask of Scents", player)
                )
            ),
        "Well Left Side Back Room Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_afford_price(state, player, 100) or
                    state.has("Mask of Scents", player)
                )
            ),
        
        "Well Right Side Before Chest Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_plant_beans(state, player, options) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Right Side Before Chest Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_plant_beans(state, player, options) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Right Side Before Chest Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_plant_beans(state, player, options) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Right Side Before Chest Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_plant_beans(state, player, options) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Right Side Before Chest Room Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_plant_beans(state, player, options) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Right Side Before Chest Room Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_plant_beans(state, player, options) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Right Side Before Chest Room Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_plant_beans(state, player, options) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Right Side Before Chest Room Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_plant_beans(state, player, options) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Right Side Before Chest Room Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_plant_beans(state, player, options) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Right Side Before Chest Room Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_bottle(state, player) and
                (
                    can_plant_beans(state, player, options) or
                    can_use_light_arrows(state, player)
                )
            ),
        
        # Well Big Poe Pots
        "Well Big Poe Pots (1)":
            lambda state: (
                state.has("Progressive Bomb Bag", player) and
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_bottle(state, player) and
                        can_plant_beans(state, player, options)
                    ) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Big Poe Pots (2)":
            lambda state: (
                state.has("Progressive Bomb Bag", player) and
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_bottle(state, player) and
                        can_plant_beans(state, player, options)
                    ) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Big Poe Pots (3)":
            lambda state: (
                state.has("Progressive Bomb Bag", player) and
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_bottle(state, player) and
                        can_plant_beans(state, player, options)
                    ) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Well Big Poe Pots (4)":
            lambda state: (
                state.has("Progressive Bomb Bag", player) and
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_bottle(state, player) and
                        can_plant_beans(state, player, options)
                    ) or
                    can_use_light_arrows(state, player)
                )
            ),
        
        #Stone Tower Climb Pots
        "Stone Tower Climb Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and  
                (
                    can_play_song("Elegy of Emptiness", state, player) and 
                    state.has("Goron Mask", player) and 
                    state.has("Zora Mask", player)
                ) or
                state.has("Stone Tower Owl Statue", player) and 
                can_play_song("Song of Soaring", state, player)
            ),
        "Stone Tower Climb Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Hookshot", player) and  
                (
                    can_play_song("Elegy of Emptiness", state, player) and 
                    state.has("Goron Mask", player) and 
                    state.has("Zora Mask", player)
                ) or
                state.has("Stone Tower Owl Statue", player) and 
                can_play_song("Song of Soaring", state, player)
            ),
            #Stone Tower Lower Scarecrow Pots            
        "Stone Tower Lower Scarecrow Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Lower Scarecrow Pots (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Lower Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        
        # Stone Tower Upper Scarecrow Pots
        "Stone Tower Upper Scarecrow Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Upper Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Upper Scarecrow Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Upper Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Upper Scarecrow Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Upper Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Upper Scarecrow Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Upper Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Upper Scarecrow Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Upper Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Upper Scarecrow Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Upper Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Upper Scarecrow Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Upper Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Upper Scarecrow Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Upper Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        "Stone Tower Upper Scarecrow Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Stone Tower Upper Scarecrow", player) and
                can_reach_scarecrow(state, player, options) and
                state.has("Hookshot", player) and
                (
                    (
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player)
                    )
                )
            ),
        
        # Stone Tower Owl Pots
        "Stone Tower Owl Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Hookshot", player) and  
                    can_play_song("Elegy of Emptiness", state, player) and 
                    state.has("Goron Mask", player) and 
                    state.has("Zora Mask", player)
                )
                or 
                (
                    state.has("Stone Tower Owl Statue", player) and 
                    can_play_song("Song of Soaring", state, player)
                )
            ),
        "Stone Tower Owl Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Hookshot", player) and  
                    can_play_song("Elegy of Emptiness", state, player) and 
                    state.has("Goron Mask", player) and 
                    state.has("Zora Mask", player)
                )
                or 
                (
                    state.has("Stone Tower Owl Statue", player) and 
                    can_play_song("Song of Soaring", state, player)
                )
            ),
        "Stone Tower Owl Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Hookshot", player) and  
                    can_play_song("Elegy of Emptiness", state, player) and 
                    state.has("Goron Mask", player) and 
                    state.has("Zora Mask", player)
                )
                or 
                (
                    state.has("Stone Tower Owl Statue", player) and 
                    can_play_song("Song of Soaring", state, player)
                )
            ),
        "Stone Tower Owl Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Hookshot", player) and  
                    can_play_song("Elegy of Emptiness", state, player) and 
                    state.has("Goron Mask", player) and 
                    state.has("Zora Mask", player)
                )
                or 
                (
                    state.has("Stone Tower Owl Statue", player) and 
                    can_play_song("Song of Soaring", state, player)
                )
            ),
        
        # Stone Tower Temple Pots
        "Stone Tower Temple Entrance Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Entrance Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        
        "Stone Tower Temple Lower Basement Armos Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Goron Mask", player) and
                has_explosives(state, player)
             ),
        "Stone Tower Temple Lower Basement Armos Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Goron Mask", player) and
                has_explosives(state, player)
             ),
        "Stone Tower Temple Lower Basement Armos Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Goron Mask", player) and
                has_explosives(state, player)
             ),
        "Stone Tower Temple Lower Basement Armos Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Goron Mask", player) and
                has_explosives(state, player)
             ),
        "Stone Tower Temple Lower Basement Armos Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Goron Mask", player) and
                has_explosives(state, player) and
                    (
                        can_use_light_arrows(state, player) or
                        has_mirror_shield(state, player)
                    )
            ),
        "Stone Tower Temple Lower Basement Armos Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Goron Mask", player) and
                has_explosives(state, player) and
                    (
                        can_use_light_arrows(state, player) or
                        has_mirror_shield(state, player)
                    )
            ),
        "Stone Tower Temple Lower Basement Armos Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Goron Mask", player) and
                has_explosives(state, player) and
                    (
                        can_use_light_arrows(state, player) or
                        has_mirror_shield(state, player)
                    )
            ),
        "Stone Tower Temple Lower Basement Armos Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Goron Mask", player) and
                has_explosives(state, player) and
                    (
                        can_use_light_arrows(state, player) or
                        has_mirror_shield(state, player)
                    )
            ),
        "Stone Tower Temple Right Side Near Locked Door Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_light_arrows(state, player) or
                    (
                        state.has("Small Key (Stone Tower)", player, 1) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player) and
                        has_explosives(state, player)
                    )
                )
            ),
        "Stone Tower Temple Right Side Near Locked Door Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    can_use_light_arrows(state, player) or
                    (
                        state.has("Small Key (Stone Tower)", player, 1) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player) and
                        has_explosives(state, player)
                    )
                )
            ),
        
        "Stone Tower Temple Right Side Underwater Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Zora Mask", player) and
                (
                    can_use_light_arrows(state, player) or
                    (
                        state.has("Small Key (Stone Tower)", player, 1) and
                        state.has("Goron Mask", player) and
                        has_explosives(state, player)
                    )
                )
            ),
        "Stone Tower Temple Right Side Underwater Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Zora Mask", player) and
                (
                    can_use_light_arrows(state, player) or
                    (
                        state.has("Small Key (Stone Tower)", player, 1) and
                        state.has("Goron Mask", player) and
                        has_explosives(state, player)
                    )
                )
            ),
        "Stone Tower Temple Right Side Underwater Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Zora Mask", player) and
                (
                    can_use_light_arrows(state, player) or
                    (
                        state.has("Small Key (Stone Tower)", player, 1) and
                        state.has("Goron Mask", player) and
                        has_explosives(state, player)
                    )
                )
            ),
        "Stone Tower Temple Right Side Underwater Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Zora Mask", player) and
                (
                    can_use_light_arrows(state, player) or
                    (
                        state.has("Small Key (Stone Tower)", player, 1) and
                        state.has("Goron Mask", player) and
                        has_explosives(state, player)
                    )
                )
            ),
        "Stone Tower Temple Right Side Underwater Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.has("Zora Mask", player) and
                (
                    can_use_light_arrows(state, player) or
                    (
                        state.has("Small Key (Stone Tower)", player, 1) and
                        state.has("Goron Mask", player) and
                        has_explosives(state, player)
                    )
                )
            ),
        
        # Stone Tower Temple Mirror Room Pots
        "Stone Tower Temple Mirror Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Small Key (Stone Tower)", player, 2) and
                    state.has("Zora Mask", player) and
                    state.has("Goron Mask", player) and
                    has_mirror_shield(state, player)
                ) or
                (
                    can_use_light_arrows(state, player) and
                    state.has("Small Key (Stone Tower)", player, 1)
                )
            ),
        "Stone Tower Temple Mirror Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (
                    state.has("Small Key (Stone Tower)", player, 2) and
                    state.has("Zora Mask", player) and
                    state.has("Goron Mask", player) and
                    has_mirror_shield(state, player)
                ) or
                (
                    can_use_light_arrows(state, player) and
                    state.has("Small Key (Stone Tower)", player, 1)
                )
            ),
        
        # Stone Tower Temple Deku Updraft Pots
        "Stone Tower Temple Deku Updraft Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Mirror Room Sun Block Chest", "Location", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Stone Tower Temple Deku Updraft Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Mirror Room Sun Block Chest", "Location", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Stone Tower Temple Deku Updraft Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Mirror Room Sun Block Chest", "Location", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Stone Tower Temple Deku Updraft Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Mirror Room Sun Block Chest", "Location", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        
        # Stone Tower Temple Lower Spike Roller Pots
        "Stone Tower Temple Lower Spike Roller Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player)
            ),
        "Stone Tower Temple Lower Spike Roller Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player)
            ),
        "Stone Tower Temple Lower Spike Roller Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player)
            ),
        "Stone Tower Temple Lower Spike Roller Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player)
            ),
        "Stone Tower Temple Lower Spike Roller Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player)
            ),
        "Stone Tower Temple Lower Spike Roller Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player)
            ),
        "Stone Tower Temple Lower Spike Roller Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player)
            ),
        "Stone Tower Temple Lower Spike Roller Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player)
            ),
        
        # Inverted Stone Tower Pots
        "Inverted Stone Tower Bean Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower (Inverted)", 'Region', player) and
                can_plant_beans(state, player, options)
            ),
        "Inverted Stone Tower Bean Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower (Inverted)", 'Region', player) and
                can_plant_beans(state, player, options)
            ),
        "Inverted Stone Tower Bean Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower (Inverted)", 'Region', player) and
                can_plant_beans(state, player, options)
            ),
        "Inverted Stone Tower Bean Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower (Inverted)", 'Region', player) and
                can_plant_beans(state, player, options)
            ),
        "Inverted Stone Tower Bean Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower (Inverted)", 'Region', player) and
                can_plant_beans(state, player, options)
            ),
        
        # Inverted Stone Tower Temple Updraft Pots
        "Inverted Stone Tower Temple Updraft Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eastern Air Gust Room Fire Chest", "Location", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Inverted Stone Tower Temple Updraft Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eastern Air Gust Room Fire Chest", "Location", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Inverted Stone Tower Temple Updraft Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eastern Air Gust Room Fire Chest", "Location", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Inverted Stone Tower Temple Updraft Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eastern Air Gust Room Fire Chest", "Location", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Inverted Stone Tower Temple Updraft Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eastern Air Gust Room Fire Chest", "Location", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Inverted Stone Tower Temple Updraft Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eastern Air Gust Room Fire Chest", "Location", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        
        # Inverted Stone Tower Temple Miniboss Pots
        "Inverted Stone Tower Temple Gomess Pots (1)":
            lambda state: (
                can_use_light_arrows(state, player) and
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Small Key (Stone Tower)", player, 3) and
                state.has("Hookshot", player)
            ),
        "Inverted Stone Tower Temple Gomess Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_light_arrows(state, player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Small Key (Stone Tower)", player, 3) and
                state.has("Hookshot", player)
            ),
        "Inverted Stone Tower Temple Gomess Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_light_arrows(state, player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Small Key (Stone Tower)", player, 3) and
                state.has("Hookshot", player)
            ),
        "Inverted Stone Tower Temple Gomess Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                can_use_light_arrows(state, player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Small Key (Stone Tower)", player, 3) and
                state.has("Hookshot", player)
            ),
        
        # Inverted Stone Tower Temple Lower Bridge Room Pots
        "Inverted Stone Tower Temple Lower Bridge Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Lower Bridge Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player)
            ),
        # Inverted Stone Tower Temple Small Poe Room Pots
        "Inverted Stone Tower Temple Small Poe Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Death Armos Maze Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Small Poe Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Death Armos Maze Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Small Poe Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Small Poe Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player)
            ),
        
        # Inverted Stone Tower Temple Wizzrobe Room Pots
        "Inverted Stone Tower Temple Wizzrobe Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player) and
                can_smack_hard(state, player)
            ),
        "Inverted Stone Tower Temple Wizzrobe Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player) and
                can_smack_hard(state, player)
            ),
        "Inverted Stone Tower Temple Wizzrobe Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player) and
                can_smack_hard(state, player)
            ),
        "Inverted Stone Tower Temple Wizzrobe Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player) and
                can_smack_hard(state, player)
            ),
        "Inverted Stone Tower Temple Wizzrobe Room Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player) and
                can_smack_hard(state, player)
            ),
        
        # Inverted Stone Tower Temple Pre Boss Pots (Flying)
        "Inverted Stone Tower Temple Pre Boss Pots (Flying) (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Pre Boss Pots (Flying) (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Pre Boss Pots (Flying) (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Pre Boss Pots (Flying) (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        
        # Inverted Stone Tower Temple Pre Boss Pots
        "Inverted Stone Tower Temple Pre Boss Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Pre Boss Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Pre Boss Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Pre Boss Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Pre Boss Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Pre Boss Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Pre Boss Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Pre Boss Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player)
            ),
        
        # MOON POTS
        "Moon Goron Trial Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        "Moon Goron Trial Pots (15)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player) and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),
        # Moon Link Trial Pots
        "Moon Link Trial Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player)
            ),
        "Moon Link Trial Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player)
            ),
        "Moon Link Trial Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player)
            ),
        "Moon Link Trial Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player)
            ),
        "Moon Link Trial Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player)
            ),
        "Moon Link Trial Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player)
            ),
        "Moon Link Trial Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player)
            ),
        "Moon Link Trial Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                state.can_reach("The Moon", 'Region', player)
            ),
        
        # Majora Lair Pots
        "Majora Lair Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (options.majora_remains_required.value == 0 or has_enough_remains(state, player, options.majora_remains_required.value)) and
                (options.majora_masks_required.value == 0 or has_enough_masks(state, player, options.majora_masks_required.value)) and
                (not options.majora_star_fox.value == 0 or has_star_fox(state, player, options, "majora")) and
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_boss(state, player, options, "Majora") and
                can_smack_hard(state, player)
            ),
        "Majora Lair Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                (options.majora_remains_required.value == 0 or has_enough_remains(state, player, options.majora_remains_required.value)) and
                (options.majora_masks_required.value == 0 or has_enough_masks(state, player, options.majora_masks_required.value)) and
                (not options.majora_star_fox.value == 0 or has_star_fox(state, player, options, "majora")) and
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_boss(state, player, options, "Majora") and
                can_smack_hard(state, player)
            ),
        # Clock Town Hitspots 

        # South Clock Town Targets Hitspot
        "South Clock Town Targets Hitspot (0)":
            lambda state: has_projectiles(state, player),
        "South Clock Town Targets Hitspot (1)":
            lambda state: has_projectiles(state, player),
        "South Clock Town Targets Hitspot (2)":
            lambda state: has_projectiles(state, player),

        # East Clock Town Targets And Basket Hitspots
        "East Clock Town Targets And Basket Hitspots (0)":
            lambda state: True,
        "East Clock Town Targets And Basket Hitspots (1)":
            lambda state: True,
        "East Clock Town Targets And Basket Hitspots (2)":
            lambda state: True,
        "East Clock Town Targets And Basket Hitspots (3)":
            lambda state: True,
        "East Clock Town Targets And Basket Hitspots (4)":
            lambda state: True,
        "East Clock Town Targets And Basket Hitspots (5)":
            lambda state: True,
        "East Clock Town Targets And Basket Hitspots (6)":
            lambda state: True,
        "East Clock Town Targets And Basket Hitspots (7)":
            lambda state: True,
        "East Clock Town Targets And Basket Hitspots (8)":
            lambda state: True,

        # Sword School Gong
        "Sword School Gong":
            lambda state: can_smack(state, player),

        # Stock Pot Inn Mask Hitspot
        "Stock Pot Inn Mask Hitspot (0)":
            lambda state:(             
                state.has("Zora Mask", player) and 
                state.has("Progressive Magic", player)),
        "Stock Pot Inn Mask Hitspot (1)":
            lambda state:(             
                state.has("Zora Mask", player) and 
                state.has("Progressive Magic", player)),
        "Stock Pot Inn Mask Hitspot (2)":
            lambda state:(             
                state.has("Zora Mask", player) and 
                state.has("Progressive Magic", player)),

        # Termina Field West Wall Hitspot
        "Termina Field West Wall Hitspot (0)":
            lambda state: True,
        "Termina Field West Wall Hitspot (1)":
            lambda state: True,
        "Termina Field West Wall Hitspot (2)":
            lambda state: True,

        # Termina Field Above West Clock Town Entry Hitspot
        "Termina Field Above West Clock Town Entry Hitspot (0)":
            lambda state: has_projectiles(state, player),
        "Termina Field Above West Clock Town Entry Hitspot (1)":
            lambda state: has_projectiles(state, player),
        "Termina Field Above West Clock Town Entry Hitspot (2)":
            lambda state: has_projectiles(state, player),

        # Skull Kid Drawing Hitspot
        "Skull Kid Drawing Hitspot (0)":
            lambda state: True,
        "Skull Kid Drawing Hitspot (1)":
            lambda state: True,
        "Skull Kid Drawing Hitspot (2)":
            lambda state: True,
        
        # Romani Ranch Hitspots
        "Romani Ranch Baby Cuccoos Hitspots (0)":
            lambda state: (
                state.can_reach("Romani Ranch", 'Region', player)
            ),
        "Romani Ranch Baby Cuccoos Hitspots (1)":
            lambda state: (
                state.can_reach("Romani Ranch", 'Region', player)
            ),
        "Romani Ranch Baby Cuccoos Hitspots (2)":
            lambda state: (
                state.can_reach("Romani Ranch", 'Region', player)
            ),
        "Romani Ranch Baby Cuccoos Hitspots (3)":
            lambda state: (
                state.can_reach("Romani Ranch", 'Region', player)
            ),
        "Romani Ranch Baby Cuccoos Hitspots (4)":
            lambda state: (
                state.can_reach("Romani Ranch", 'Region', player)
            ),
        "Romani Ranch Baby Cuccoos Hitspots (5)":
            lambda state: (
                state.can_reach("Romani Ranch", 'Region', player)
            ),
        
        # Swamp Spider House Hitspots
        "Swamp Spiderhouse Totem Eye Hitspots (0)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (1)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (2)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (3)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (4)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (5)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (6)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (7)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (8)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (9)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (10)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Swamp Spiderhouse Totem Eye Hitspots (11)":
            lambda state: (
                state.can_reach("Swamp Spider House", 'Region', player) and
                has_projectiles(state, player)
            ),
        
        # Ocean Spider House Hitspots
        "Ocean Spiderhouse Mask Hitspots (0)":
            lambda state: (
                state.can_reach("Ocean Spider House", 'Region', player) and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Mask Hitspots (1)":
            lambda state: (
                state.can_reach("Ocean Spider House", 'Region', player) and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Mask Hitspots (2)":
            lambda state: (
                state.can_reach("Ocean Spider House", 'Region', player) and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Mask Hitspots (3)":
            lambda state: (
                state.can_reach("Ocean Spider House", 'Region', player) and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Mask Hitspots (4)":
            lambda state: (
                state.can_reach("Ocean Spider House", 'Region', player) and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Mask Hitspots (5)":
            lambda state: (
                state.can_reach("Ocean Spider House", 'Region', player) and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Mask Hitspots (6)":
            lambda state: (
                state.can_reach("Ocean Spider House", 'Region', player) and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Mask Hitspots (7)":
            lambda state: (
                state.can_reach("Ocean Spider House", 'Region', player) and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ),
        "Ocean Spiderhouse Mask Hitspots (8)":
            lambda state: (
                state.can_reach("Ocean Spider House", 'Region', player) and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ),
        
        # Pirates Fortress Interior Hitspots
        "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (0)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (1)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (2)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (3)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (4)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Pirates Fortress Interior Outdoor Pirate Flag Eye Hitspots (5)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                has_projectiles(state, player)
            ),
        
        "Pirates Fortress Interior Indoor Pirate Flag Eye Hitspot (0)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Pirates Fortress Interior Indoor Pirate Flag Eye Hitspot (1)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Pirates Fortress Interior Indoor Pirate Flag Eye Hitspot (2)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                has_projectiles(state, player)
            ),
        
        # Ikana Graveyard Hitspots
        "Ikana Graveyard Lantern Hitspots (0)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (1)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (2)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (3)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (4)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (5)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (6)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (7)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (8)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (9)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (10)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),
        "Ikana Graveyard Lantern Hitspots (11)":
            lambda state: (
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_projectiles(state, player)
            ),

        # FREESTANDING RUPEES

        # Termina Field Song Wall

        "Termina Field 6am Songwall (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 6am Songwall (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 6am Songwall (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 7am Songwall (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 7am Songwall (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 7am Songwall (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 8am Songwall (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 8am Songwall (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 8am Songwall (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        # "Termina Field 9am Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 9am Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 9am Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        "Termina Field 10am Songwall (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 10am Songwall (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 10am Songwall (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        # "Termina Field 11am Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 11am Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 11am Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        "Termina Field 12pm Songwall (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 12pm Songwall (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 12pm Songwall (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        # "Termina Field 1pm Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 1pm Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 1pm Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 2pm Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 2pm Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 2pm Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 3pm Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 3pm Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 3pm Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 4pm Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 4pm Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 4pm Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 5pm Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 5pm Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 5pm Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 6pm Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 6pm Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 6pm Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 7pm Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 7pm Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 7pm Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
         "Termina Field 8pm Songwall (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 8pm Songwall (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        "Termina Field 8pm Songwall (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Songwall") and
                state.has("Ocarina of Time", player)
            ),
        # "Termina Field 9pm Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 9pm Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 9pm Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 10pm Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 10pm Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 10pm Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 11pm Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 11pm Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 11pm Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 12am Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 12am Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 12am Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 1am Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 1am Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 1am Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 2am Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 2am Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 2am Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 3am Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 3am Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 3am Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 4am Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 4am Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 4am Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 5am Songwall (0)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 5am Songwall (1)":
        #     lambda state: state.has("Ocarina of Time", player),
        # "Termina Field 5am Songwall (2)":
        #     lambda state: state.has("Ocarina of Time", player),

        # Laundry Pool Night 2 Rupees
        "Laundry Pool Night 2 Rupees (0)":
            lambda state: True,
        "Laundry Pool Night 2 Rupees (1)":
            lambda state: True,
        "Laundry Pool Night 2 Rupees (2)":
            lambda state: True,


        # Termina Field Eastern Pillar Rupees
        "Termina Field Eastern Pillar Rupees":
            lambda state: True,

        # Termina Field Tree Rupees
        "Termina Field Tree Rupees (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Termina Field Tree Rupees (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),

        # Observatory Guay
        "Observatory Secret Guay Rupee (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Astral Observatory Man") and
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player)
            ),
        "Observatory Secret Guay Rupee (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Astral Observatory Man") and
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player)
            ),
        # Termina Field Song Guay Rupees
        "Termina Field Song Guay Rupees (0)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (1)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (2)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (3)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (4)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (5)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (6)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (7)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (8)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (9)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (10)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (11)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (12)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (13)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (14)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (15)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (16)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (17)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (18)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),
        "Termina Field Song Guay Rupees (19)":
            lambda state: (
                state.can_reach("Termina Field", 'Region', player) and
                (can_play_song("Sonata of Awakening", state, player) or
                 can_play_song("Goron Lullaby", state, player) or
                 can_play_song("New Wave Bossa Nova", state, player))
            ),

        # Termina Field Song Guay Goron/Epona Rupees
        "Termina Field Song Guay Goron/Epona Rupees (0)":
            lambda state: (
                (
                    can_play_song("Sonata of Awakening", state, player) or
                    can_play_song("Goron Lullaby", state, player) or
                    can_play_song("New Wave Bossa Nova", state, player)
                ) and
                (
                    can_play_song("Epona's Song", state, player) or
                    state.has("Goron Mask", player)
                )
            ),
        "Termina Field Song Guay Goron/Epona Rupees (1)":
            lambda state: (
                (
                    can_play_song("Sonata of Awakening", state, player) or
                    can_play_song("Goron Lullaby", state, player) or
                    can_play_song("New Wave Bossa Nova", state, player)
                ) and
                (
                    can_play_song("Epona's Song", state, player) or
                    state.has("Goron Mask", player)
                )
            ),
        "Termina Field Song Guay Goron/Epona Rupees (2)":
            lambda state: (
                (
                    can_play_song("Sonata of Awakening", state, player) or
                    can_play_song("Goron Lullaby", state, player) or
                    can_play_song("New Wave Bossa Nova", state, player)
                ) and
                (
                    can_play_song("Epona's Song", state, player) or
                    state.has("Goron Mask", player)
                )
            ),         

        # Takkuri - thieving lil' shit

        "Termina Field Thieving Bird Rupee":
            lambda state: (
                    has_soul_enemy(state, player, options, "Takkuri") and
                    (   has_soul_absurd(state, player, options, "Grass") or
                        has_projectiles(state, player)
                    )
            ),
            

        # Deku PlayGround Day 1 Rupees
        "Deku PlayGround Day 1 Rupees (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 1 Rupees (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 1 Rupees (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 1 Rupees (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 1 Rupees (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 1 Rupees (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),

        # Deku PlayGround Day 2 Rupees
        "Deku PlayGround Day 2 Rupees (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 2 Rupees (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 2 Rupees (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 2 Rupees (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 2 Rupees (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 2 Rupees (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        # Deku PlayGround Day 3 Rupees
        "Deku PlayGround Day 3 Rupees (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 3 Rupees (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 3 Rupees (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 3 Rupees (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 3 Rupees (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku PlayGround Day 3 Rupees (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        # Southern Swamp Flower Rupees
        "Southern Swamp Flower Rupees (0)":
            lambda state: (
                state.has("Deku Mask", player) or
                state.has("Zora Mask", player)
            ),
        "Southern Swamp Flower Rupees (1)":
            lambda state: (
                state.has("Deku Mask", player) or 
                state.has("Zora Mask", player)
            ),

        "Southern Swamp Witch Shop Rupee (1)":
            lambda state: True,            

        # Deku Palace Right Side Rupees
        "Deku Palace Right Side Rupees (0)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (1)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (2)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (3)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (4)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (5)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (6)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (7)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (8)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (9)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (10)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (11)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Right Side Rupees (12)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),

        # Deku Palace Left Side Rupees
        "Deku Palace Left Side Rupees (0)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Left Side Rupees (1)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Left Side Rupees (2)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Left Side Rupees (3)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Left Side Rupees (4)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Left Side Rupees (5)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Left Side Rupees (6)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Left Side Rupees (7)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Left Side Rupees (8)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Left Side Rupees (9)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),
        "Deku Palace Left Side Rupees (10)":
            lambda state: state.can_reach("Deku Palace", 'Region', player),

        # Deku Butler Rupees
        "Deku Butler Rupees (0)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (1)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (2)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (3)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (4)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (5)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (6)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (7)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (8)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (9)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (10)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (11)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (12)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (13)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (14)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (15)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (16)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (17)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (18)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (19)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (20)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (21)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (22)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (23)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (24)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (25)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (26)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (27)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (28)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),
        "Deku Butler Rupees (29)":
            lambda state: (
                state.can_reach("Deku Palace", 'Region', player) and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Progressive Sword", player) and
                has_bottle(state, player)
            ),

        # Woodfall Stump Rupee
        "Woodfall Stump Rupee":
            lambda state: state.can_reach("Woodfall", 'Region', player) and 
            has_soul_absurd(state, player, options, "Deku Flowers") and
            state.has("Deku Mask", player),

        # Romani Ranch Haystack Rupees
        "Romani Ranch Haystack Rupees (0)":
            lambda state: state.can_reach("Romani Ranch", 'Region', player),
        "Romani Ranch Haystack Rupees (1)":
            lambda state: state.can_reach("Romani Ranch", 'Region', player),

        # Woodfall Temple Pre Boss Rupees 
        "Woodfall Temple Pre Boss Rupees (0)":
            lambda state: (
                state.can_reach("Woodfall Temple", 'Region', player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Pre Boss Rupees (1)":
            lambda state: (
                state.can_reach("Woodfall Temple", 'Region', player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Pre Boss Rupees (2)":
            lambda state: (
                state.can_reach("Woodfall Temple", 'Region', player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Pre Boss Rupees (3)":
            lambda state: (
                state.can_reach("Woodfall Temple", 'Region', player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Pre Boss Rupees (4)":
            lambda state: (
                state.can_reach("Woodfall Temple", 'Region', player) and
                state.has("Progressive Bow", player) and
                state.has("Zora Mask", player)
            ),
        "Woodfall Temple Pre Boss Rupees (5)":
            lambda state: (
                state.can_reach("Woodfall Temple", 'Region', player) and
                state.has("Progressive Bow", player) and
                state.has("Zora Mask", player)
            ),
        # Snowhead Temple Icicle Rupees
        "Snowhead Temple Icicle Rupees (1)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Rupees (2)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Rupees (3)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        # Mountain Village Spring Boulder Under Smithy Rupee
        "Mountain Village Spring Boulder Under Smithy Rupee":
            lambda state: can_clear_snowhead(state, player, boss_placements),

        # Twin Islands Spring Underwater Rupees
        "Twin Islands Spring Underwater Rupees (0)":
            lambda state: (
                state.can_reach("Twin Islands", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements) and
                state.has("Zora Mask", player)
            ),
        "Twin Islands Spring Underwater Rupees (1)":
            lambda state: (
                state.can_reach("Twin Islands", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements) and
                state.has("Zora Mask", player)
            ),
        "Twin Islands Spring Underwater Rupees (2)":
            lambda state: (
                state.can_reach("Twin Islands", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements) and
                state.has("Zora Mask", player)
            ),
        "Twin Islands Spring Underwater Rupees (3)":
            lambda state: (
                state.can_reach("Twin Islands", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements) and
                state.has("Zora Mask", player)
            ),

        # Great Bay Temple Waterwheel Rupees
        "Great Bay Temple Waterwheel Rupees (0)":
            lambda state: state.can_reach("Great Bay Temple", 'Region', player),
        "Great Bay Temple Waterwheel Rupees (1)":
            lambda state: state.can_reach("Great Bay Temple", 'Region', player),
        "Great Bay Temple Waterwheel Rupees (2)":
            lambda state: state.can_reach("Great Bay Temple", 'Region', player),
        "Great Bay Temple Waterwheel Rupees (3)":
            lambda state: state.can_reach("Great Bay Temple", 'Region', player),
        "Great Bay Temple Waterwheel Rupees (4)":
            lambda state: state.can_reach("Great Bay Temple", 'Region', player),
        "Great Bay Temple Room Behind Waterfall Rupees (1)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Room Behind Waterfall Rupees (2)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player)
            ),

        # Great Bay Temple Before Gekko Room Underwater Rupees
        "Great Bay Temple Before Gekko Room Underwater Rupees (0)":
            lambda state: state.can_reach("Great Bay Temple", 'Region', player),
        "Great Bay Temple Before Gekko Room Underwater Rupees (1)":
            lambda state: state.can_reach("Great Bay Temple", 'Region', player),

        "Pirates' Fortress Sewers Rupees Under Barrel (0)":
            lambda state: (
                state.has("Zora Mask", player) and
                state.has("Goron Mask", player)
            ),
        "Pirates' Fortress Sewers Rupees Under Barrel (1)":
            lambda state: (
                state.has("Zora Mask", player) and
                state.has("Goron Mask", player)
            ),
        "Pirates' Fortress Sewers Rupees Under Barrel (2)":
            lambda state: (
                state.has("Zora Mask", player) and
                state.has("Goron Mask", player)
            ),
        "Pirates' Fortress Sewers Rupees Under Barrel (3)":
            lambda state: (
                state.has("Zora Mask", player) and
                state.has("Goron Mask", player)
            ),
        "Pirates' Fortress Sewers Rupees Under Barrel (4)":
            lambda state: (
                state.has("Zora Mask", player) and
                state.has("Goron Mask", player)
            ),
        "Pirates Fortress' Sewers Exit Barrel Rupee (0)":
            lambda state: (
                state.has("Zora Mask", player) and
                state.has("Goron Mask", player)
            ),
        "Pirates Fortress' Sewers Exit Barrel Rupee (1)":
            lambda state: (
                state.has("Zora Mask", player) and
                state.has("Goron Mask", player)
            ),
        "Pirates Fortress' Sewers Exit Barrel Rupee (2)":
            lambda state: (
                state.has("Zora Mask", player) and
                state.has("Goron Mask", player)
            ),
        "Pirates Fortress' Interior Ledge Recovery Hearts (0)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                state.has("Hookshot", player) 
            ),
        "Pirates Fortress' Interior Ledge Recovery Hearts (1)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                state.has("Hookshot", player) 
            ),
        "Pirates Fortress' Interior Ledge Recovery Hearts (2)":
            lambda state: (
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                state.has("Hookshot", player) 
            ),

        # Ikana Graveyard Day 2 Rupees
        "Ikana Graveyard Day 2 Rupees (0)":
            lambda state: (
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and 
                can_smack_hard(state, player)
            ),
        "Ikana Graveyard Day 2 Rupees (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and 
                can_smack_hard(state, player)
            ),
        "Ikana Graveyard Day 2 Rupees (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and 
                can_smack_hard(state, player)
            ),
        "Ikana Graveyard Day 2 Rupees (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and 
                can_smack_hard(state, player)
            ),
        "Ikana Graveyard Day 2 Rupees (4)":
            lambda state: (
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and 
                can_smack_hard(state, player)
            ),
        "Ikana Graveyard Day 2 Rupees (5)":
            lambda state: (
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and 
                can_smack_hard(state, player)
            ),
        "Ikana Graveyard Day 2 Rupees (6)":
            lambda state: (
                has_soul_npc(state, player, options, "Stalchildren") and
                state.has("Captain's Hat", player) and 
                can_smack_hard(state, player)
            ),

        # Secret Shrine Rupees
        "Secret Shrine Rupees (0)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (1)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (2)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (3)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (4)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (5)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (6)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (7)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (8)":
             lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (9)":
             lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (10)":
             lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (11)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (12)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (13)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (14)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (15)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),
        "Secret Shrine Rupees (16)":
            lambda state: (
                (state.has("Zora Mask", player) or 
                can_plant_beans(state, player, options))
            ),

        # Stone Tower Rupees
        "Stone Tower Bridge Room Rupees (0)":
            lambda state: (
                state.has("Small Key (Stone Tower)", player, 2) and 
                (has_mirror_shield(state, player)
                or 
                can_use_light_arrows(state, player))
            ),
        "Stone Tower Bridge Room Rupees (1)":
            lambda state: (
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player) and
                state.has("Hookshot", player)
            ),
        "Stone Tower Bridge Room Rupees (2)":
            lambda state: (
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player) and
                state.has("Hookshot", player)
            ),
        "Stone Tower Bridge Room Rupees (3)":
            lambda state: (
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player) and
                state.has("Hookshot", player)
            ),
        "Stone Tower Bridge Room Rupees (4)":
            lambda state: (
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player) and
                state.has("Hookshot", player)
            ),
        "Stone Tower Bridge Room Rupees (5)":
            lambda state: (
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player) and
                state.has("Hookshot", player)
            ),
        "Stone Tower Bridge Room Rupees (6)":
            lambda state: (
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player) and
                state.has("Hookshot", player)
            ),
        "Stone Tower Bridge Room Rupees (7)":
            lambda state: (
                state.can_reach("Stone Tower Temple Garo Master Chest", "Location", player) and
                state.has("Hookshot", player)
            ),
        "Stone Tower Deku Updraft Rupees (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Mirror Room Sun Face Chest", "Location", player)
            ),
        "Stone Tower Deku Updraft Rupees (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Mirror Room Sun Face Chest", "Location", player)
            ),
        "Stone Tower Deku Updraft Rupees (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Mirror Room Sun Face Chest", "Location", player)
            ),
        "Stone Tower Deku Updraft Rupees (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Mirror Room Sun Face Chest", "Location", player)
            ),
        "Stone Tower Deku Updraft Rupees (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Mirror Room Sun Face Chest", "Location", player)
            ),
        "Stone Tower Deku Updraft Rupees (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Mirror Room Sun Face Chest", "Location", player)
            ),
        # Stone Tower Eyegore Room Light Block Rupees
        "Stone Tower Eyegore Room Light Block Rupees (0)":
            lambda state: (
                can_use_light_arrows(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Stone Tower Eyegore Room Light Block Rupees (1)":
            lambda state: (
                can_use_light_arrows(state, player) and 
                state.has("Zora Mask", player)
            ),
        # Inverted Stone Tower Right Side Light Block Rupees
        "Inverted Stone Tower Right Side Light Block Rupees (0)":
            lambda state: True,
        "Inverted Stone Tower Right Side Light Block Rupees (1)":
            lambda state: True,
        # Inverted Stone Tower Dexihand Rupees
        "Inverted Stone Tower Dexihand Rupees (0)":
            lambda state: True,
        "Inverted Stone Tower Dexihand Rupees (1)":
            lambda state: True,
        "Inverted Stone Tower Dexihand Rupees (2)":
            lambda state: True,

        # Inverted Stone Tower Pre Boss Rupees
        "Inverted Stone Tower Pre Boss Rupees (0)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player),
        "Inverted Stone Tower Pre Boss Rupees (1)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player),
        "Inverted Stone Tower Pre Boss Rupees (2)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player),
        "Inverted Stone Tower Pre Boss Rupees (3)":
            lambda state: (
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player) and
                state.has("Zora Mask", player)
            ),
        "Inverted Stone Tower Pre Boss Rupees (4)":
            lambda state: (
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player) and
                state.has("Zora Mask", player)
            ),
        "Inverted Stone Tower Pre Boss Rupees (5)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player),
        "Inverted Stone Tower Pre Boss Rupees (6)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player),
        "Inverted Stone Tower Pre Boss Rupees (7)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player),
        "Inverted Stone Tower Pre Boss Rupees (8)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player),
        "Inverted Stone Tower Pre Boss Rupees (9)":
            lambda state: (
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player) and
                state.has("Zora Mask", player)
            ),
        "Inverted Stone Tower Pre Boss Rupees (10)":
            lambda state: (
                state.can_reach("Stone Tower Temple Inverted Eyegore Chest", "Location", player) and
                state.has("Zora Mask", player)
            ),

        # INVISIBLE RUPEES

        # Termina Field Invisible Rupee Over Ramp Near Fountains
        "Termina Field Invisible Rupee Over Ramp Near Fountains":
            lambda state: True,

        # Termina Field Fountain Rupees
        "Termina Field Fountain Rupees (0)":
            lambda state: True,
        "Termina Field Fountain Rupees (1)":
            lambda state: True,

        # Termina Field Invisible Rupee Over Water
        "Termina Field Invisible Rupee Over Water":
            lambda state: True,

        # Termina Field Invisible Rupee Over Stump
        "Termina Field Invisible Rupee Over Stump":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) or
                (state.has("Goron Mask", player) and
                 state.has("Progressive Magic", player))
            ),

        # Termina Field Invisible Rupees In Long Grass
        "Termina Field Invisible Rupees In Long Grass (0)":
            lambda state: True,
        "Termina Field Invisible Rupees In Long Grass (1)":
            lambda state: True,
        "Termina Field Invisible Rupees In Long Grass (2)":
            lambda state: True,
        "Termina Field Invisible Rupees In Long Grass (3)":
            lambda state: True,
        "Termina Field Invisible Rupees In Long Grass (4)":
            lambda state: True,

        # Termina Field Invisible Rupee Over Northern Ramp
        "Termina Field Invisible Rupee Over Northern Ramp":
            lambda state: (
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player)
            ),

        # Romani Ranch Invisible Fence Rupees
        "Romani Ranch Invisible Fence Rupees (0)":
            lambda state: state.can_reach("Romani Ranch", 'Region', player),
        "Romani Ranch Invisible Fence Rupees (1)":
            lambda state: state.can_reach("Romani Ranch", 'Region', player),
        "Romani Ranch Invisible Fence Rupees (2)":
            lambda state: state.can_reach("Romani Ranch", 'Region', player),
        "Romani Ranch Invisible Fence Rupees (3)":
            lambda state: state.can_reach("Romani Ranch", 'Region', player),
        "Romani Ranch Invisible Fence Rupees (4)":
            lambda state: state.can_reach("Romani Ranch", 'Region', player),
        "Romani Ranch Invisible Fence Rupees (5)":
            lambda state: state.can_reach("Romani Ranch", 'Region', player),

        # Swamp Spiderhouse Invisible Rupees Above Giant Jars
        "Swamp Spiderhouse Invisible Rupees Above Giant Jars (0)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spiderhouse Invisible Rupees Above Giant Jars (1)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spiderhouse Invisible Rupees Above Giant Jars (2)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spiderhouse Invisible Rupees Above Giant Jars (3)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spiderhouse Invisible Rupees Above Giant Jars (4)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),    
        
        # SOILS

        # Termina Field Stump Soil
        "Termina Field Stump Soil":
            lambda state: has_bottle(state, player),

        # Termina Field Wall Soil
        "Termina Field Wall Soil (0)":
            lambda state: has_bottle(state, player),
        "Termina Field Wall Soil (1)":
            lambda state: has_bottle(state, player),
        "Termina Field Wall Soil (2)":
            lambda state: has_bottle(state, player),

        # Termina Field Eastern Soil
        "Termina Field Eastern Soil":
            lambda state: has_bottle(state, player),

        # Termina Field Observatory Soil
        "Termina Field Observatory Soil (0)":
            lambda state: (
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player) and
                has_bottle(state, player)
            ),
        "Termina Field Observatory Soil (1)":
            lambda state: (
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player) and
                has_bottle(state, player)
            ),
        "Termina Field Observatory Soil (2)":
            lambda state: (
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player) and
                has_bottle(state, player)
            ),

        # Swamp Spider House Rock Soil
        "Swamp Spider House Rock Soil (0)":
            lambda state: has_bottle(state, player),
        "Swamp Spider House Rock Soil (1)":
            lambda state: has_bottle(state, player),
        "Swamp Spider House Rock Soil (2)":
            lambda state: has_bottle(state, player),
        # Swamp Spider House Gold Room Soil
        "Swamp Spider House Gold Room Soil":
            lambda state: has_bottle(state, player),
        # Deku Palace Bean Seller Soil
        "Deku Palace Bean Seller Soil (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_bottle(state, player)
            ),
        "Deku Palace Bean Seller Soil (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_bottle(state, player)
            ),
        "Deku Palace Bean Seller Soil (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_bottle(state, player)
            ),
        # Deku Palace Exterior Soil
        "Deku Palace Exterior Soil (0)":
            lambda state: has_bottle(state, player),
        "Deku Palace Exterior Soil (1)":
            lambda state: has_bottle(state, player),
        "Deku Palace Exterior Soil (2)":
            lambda state: has_bottle(state, player),
        # Romani Ranch Soils
        "Romani Ranch Day 1 Soil":
            lambda state: (
                state.can_reach("Romani Ranch", 'Region', player) and
                has_bottle(state, player) and
                state.has("Goron Mask", player) and
                can_use_powder_keg(state, player, options)
            ),
        # Romani Ranch Day 2/3 Soil
        "Romani Ranch Day 2/3 Soil (0)":
            lambda state: has_bottle(state, player),
        "Romani Ranch Day 2/3 Soil (1)":
            lambda state: has_bottle(state, player),
        "Romani Ranch Day 2/3 Soil (2)":
            lambda state: has_bottle(state, player),

        # Romani Ranch Doggy Racetrack Soil
        "Romani Ranch Doggy Racetrack Soil":
            lambda state: has_bottle(state, player),
        # Great Bay Coast Soil
        "Great Bay Coast Soil":
            lambda state: has_bottle(state, player),
        # Secret Shrine Soil
        "Secret Shrine Soil (0)":
            lambda state: has_bottle(state, player),
        "Secret Shrine Soil (1)":
            lambda state: has_bottle(state, player),
        "Secret Shrine Soil (2)":
            lambda state: has_bottle(state, player),
        # Stone Tower Inverted Soils
        "Stone Tower Inverted Soils (0)":
            lambda state: has_bottle(state, player),
        "Stone Tower Inverted Soils (1)":
            lambda state: (
                has_bottle(state, player) and
                can_plant_beans(state, player, options)
            ),
              

        # SNOWBALLS

        # Path to Mountains Snowballs
        "Path to Mountains Snowballs (1)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),
        "Path to Mountains Snowballs (2)":
            lambda state: True,
        "Path to Mountains Snowballs (3)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),
        "Path to Mountains Snowballs (4)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),
        "Path to Mountains Snowballs (5)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),
        "Path to Mountains Snowballs (6)":
            lambda state: True,
        "Path to Mountains Snowballs (7)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),
        "Path to Mountains Snowballs (8)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),
        "Path to Mountains Snowballs (9)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),
        "Path to Mountains Snowballs (10)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),
        "Path to Mountains Snowballs (11)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),
        "Path to Mountains Snowballs (12)":
            lambda state: True,
        "Path to Mountains Snowballs (13)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),
        "Path to Mountains Snowballs (14)":
            lambda state: True,
        "Path to Mountains Snowballs (15)":
            lambda state: (
                (state.has("Goron Mask", player) or
                 has_explosives(state, player) or
                 can_use_fire_arrows(state, player))
            ),

        # Mountain Village Day 1 Snowballs
        "Mountain Village Day 1 Snowballs (0)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (1)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (2)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (3)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (4)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (5)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (6)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (7)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (8)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (9)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (10)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (11)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (12)":
            lambda state: can_break_balls(state, player),
        "Mountain Village Day 1 Snowballs (13)":
            lambda state: can_break_balls(state, player),

        # Snowballs Outside Goron Graveyard
        "Snowballs Outside Goron Graveyard (0)":
            lambda state: (
                state.can_reach("Mountain Village", 'Region', player) and
                state.has("Progressive Magic", player) and
                state.has("Lens of Truth", player)
            ),
        "Snowballs Outside Goron Graveyard (1)":
            lambda state: (
                state.can_reach("Mountain Village", 'Region', player) and
                state.has("Progressive Magic", player) and
                state.has("Lens of Truth", player)
            ),

        # Twin Islands Day 1 Snowballs
        "Twin Islands Day 1 Snowballs (0)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (1)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (2)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (3)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (4)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (5)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (6)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (7)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (8)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (9)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (10)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (11)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (12)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (13)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (14)":
            lambda state: can_break_balls(state, player),
        "Twin Islands Day 1 Snowballs (15)":
            lambda state: can_break_balls(state, player),

        # Twin Isles Snowballs Near Grotto
        "Twin Isles Snowballs Near Grotto (1)":
            lambda state: 
            (
                state.has("Goron Mask", player) 
                    or (
                        state.has("Hookshot", player) 
                        and state.has("Twin Islands Scarecrow", player) 
                        and has_soul_npc(state, player, options, "Scarecrow")
                    )
            ),
        "Twin Isles Snowballs Near Grotto (2)":
            lambda state: 
            (
                state.has("Goron Mask", player) 
                    or (
                        state.has("Hookshot", player) 
                        and state.has("Twin Islands Scarecrow", player) 
                        and has_soul_npc(state, player, options, "Scarecrow")
                    )
            ),
        "Twin Isles Snowballs Near Grotto (3)":
            lambda state: 
            (
                state.has("Goron Mask", player) 
                    or (
                        state.has("Hookshot", player) 
                        and state.has("Twin Islands Scarecrow", player) 
                        and has_soul_npc(state, player, options, "Scarecrow")
                    )
            ),
        # Goron Village Snowballs
        "Goron Village Snowballs (0)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (1)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (2)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (3)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (4)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (5)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (6)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (7)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (8)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (9)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (10)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (11)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (12)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (13)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (14)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (15)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (16)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (17)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (18)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (19)":
            lambda state: can_break_balls(state, player),
        "Goron Village Snowballs (20)":
            lambda state: can_break_balls(state, player),

        # Path to Snowhead Snowballs
        "Path to Snowhead Snowballs (0)":
            lambda state: (
                state.has("Goron Mask", player) and
                 state.has("Progressive Magic", player)
        ),
        "Path to Snowhead Snowballs (1)":
            lambda state: (
                state.has("Goron Mask", player) and
                 state.has("Progressive Magic", player)
        ),
        "Path to Snowhead Snowballs (2)":
            lambda state: (
                state.can_reach("Snowhead", 'Region', player) and
                (
                    state.has("Goron Mask", player) or
                    has_explosives(state, player) or
                    can_use_fire_arrows(state, player)
                )
        ),
        "Path to Snowhead Snowballs (3)":
            lambda state: state.can_reach("Snowhead", 'Region', player),
        "Path to Snowhead Snowballs (4)":
            lambda state: state.can_reach("Snowhead", 'Region', player),
        "Path to Snowhead Snowballs (5)":
            lambda state: state.can_reach("Snowhead", 'Region', player),
        "Path to Snowhead Snowballs (6)":
            lambda state: (
                state.can_reach("Snowhead", 'Region', player) and
                (
                    state.has("Goron Mask", player) or
                    has_explosives(state, player) or
                    can_use_fire_arrows(state, player)
                )
        ),
        # Outside Snowhead Temple Snowballs
        "Outside Snowhead Temple Snowballs (0)":
             lambda state: True,
        "Outside Snowhead Temple Snowballs (1)":
             lambda state: True,
        "Outside Snowhead Temple Snowballs (2)":
             lambda state: True,
        "Outside Snowhead Temple Snowballs (3)":
             lambda state: True,
        "Outside Snowhead Temple Snowballs (4)":
             lambda state: True,
        "Outside Snowhead Temple Snowballs (5)":
             lambda state: True,
        "Outside Snowhead Temple Snowballs (6)":
             lambda state: True,
        "Outside Snowhead Temple Snowballs (7)":
             lambda state: True,
        "Outside Snowhead Temple Snowballs (8)":
             lambda state: True,
        "Outside Snowhead Temple Snowballs (9)":
             lambda state: True,

        # Snowhead Temple Lower Runway Room Snowballs             
        "Snowhead Temple Lower Runway Room Snowballs (0)":
            lambda state: True,
        "Snowhead Temple Lower Runway Room Snowballs (1)":
        lambda state: True,
        
        # Snowhead Temple Icicle Room Snowballs
        "Snowhead Temple Icicle Room Snowballs (0)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Snowballs (1)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Snowballs (2)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Snowballs (3)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Snowballs (4)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Snowballs (5)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),

        # Snowhead Temple Main Room 2nd Floor Snowballs
        "Snowhead Temple Main Room 2nd Floor Snowballs (0)":
            lambda state: (
                can_use_fire_arrows(state, player) or
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple Main Room 2nd Floor Snowballs (1)":
            lambda state: (
                can_use_fire_arrows(state, player) or
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple Main Room 2nd Floor Snowballs (2)":
            lambda state: (
                can_use_fire_arrows(state, player) or
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple Main Room 2nd Floor Snowballs (3)":
            lambda state: (
                can_use_fire_arrows(state, player) or
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple Main Room 2nd Floor Snowballs (4)":
            lambda state: (
                can_use_fire_arrows(state, player) or
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple Main Room 2nd Floor Snowballs (5)":
            lambda state: (
                can_use_fire_arrows(state, player) or
                 state.has("Small Key (Snowhead)", player, 2)
            ),

        # Snowhead Temple 3rd Floor Bridge Snowballs
        "Snowhead Temple 3rd Floor Bridge Snowballs (0)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple 3rd Floor Bridge Snowballs (1)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple 3rd Floor Bridge Snowballs (2)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple 3rd Floor Bridge Snowballs (3)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),

        # Snowhead Temple 3rd Floor Behind Locked Door Snowballs
        "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (0)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (1)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (2)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (3)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (4)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (5)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (6)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),
        "Snowhead Temple 3rd Floor Behind Locked Door Snowballs (7)":
            lambda state: (
                can_use_fire_arrows(state, player) and
                 state.has("Small Key (Snowhead)", player, 2)
            ),

        # Mountain Village Spring Snowballs
        "Mountain Village Spring Snowballs (0)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Snowballs (1)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Snowballs (2)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Snowballs (3)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Snowballs (4)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements)
            ),    
        # ROCKS

        # Termina Field Kamaro Rock Circle
        "Termina Field Kamaro Rock Circle (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field Kamaro Rock Circle (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field Kamaro Rock Circle (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field Kamaro Rock Circle (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field Kamaro Rock Circle (4)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field Kamaro Rock Circle (5)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field Kamaro Rock Circle (6)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field Kamaro Rock Circle (7)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field Bio Baba Rock":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Rocks") and
                (
                    has_explosives(state, player) or
                    state.has("Goron Mask", player)
                )
            ),
        # Termina Field North West Rock Wall
        "Termina Field North West Rock Wall (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_explosives(state, player)
            ),
        "Termina Field North West Rock Wall (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_explosives(state, player)
            ),
        "Termina Field North West Rock Wall (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_explosives(state, player)
            ),
        "Termina Field North West Rock Wall (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_explosives(state, player)
            ),
        "Termina Field North West Rock Wall (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_explosives(state, player)
            ),

        # Termina Field Rock Behind Coast Wall
        "Termina Field Rock Behind Coast Wall":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
            can_play_song("Epona's Song", state, player)
            ),
        # South West Rock Wall
        "Termina Field South West Rock Wall (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_explosives(state, player)
            ),
        "Termina Field South West Rock Wall (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_explosives(state, player)
            ),
        "Termina Field South West Rock Wall (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_explosives(state, player)
            ),
        "Termina Field South West Rock Wall (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_explosives(state, player)
            ),

        "Termina Field Boulder Over Bio Baba Grotto (0)":
            lambda state: has_explosives(state, player),

        "Termina Field Boulder Over Gossip Grotto (0)":
            lambda state: has_explosives(state, player),
        # Deku Palace Goron Boulders 
        "Deku Palace Guarded Boulder (1)":
            lambda state: state.has("Goron Mask", player),
        "Deku Palace Guarded Boulder (2)":
            lambda state: state.has("Goron Mask", player),
        "Deku Palace Guarded Boulder (3)":
            lambda state: state.has("Goron Mask", player),

        # Swamp Spider Entry Rocks
        "Swamp Spider Entry Rocks (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                state.can_reach("Swamp Spider House", 'Region', player)
            ),
        "Swamp Spider Entry Rocks (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                state.can_reach("Swamp Spider House", 'Region', player)
            ),
        # Swamp Spider Monument Room Boulder 
        "Swamp Spider Monument Room Boulder":
            lambda state: True,
        # Swamp Spider Large Pots Rock
        "Swamp Spider Large Pots Rock":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                state.can_reach("Swamp Spider House", 'Region', player)
            ),
        "Twin Isles Hot Spring Water Grotto Bomb Boulders (0)":
            lambda state: state.can_reach("Twin Islands Hot Water Grotto Chest", "Location", player),
        "Twin Isles Hot Spring Water Grotto Bomb Boulders (1)":
            lambda state: state.can_reach("Twin Islands Hot Water Grotto Chest", "Location", player),     
        "Twin Isles Hot Spring Water Grotto Bomb Boulders (2)":
            lambda state: state.can_reach("Twin Islands Hot Water Grotto Chest", "Location", player),
        "Twin Isles Hot Spring Water Grotto Bomb Boulders (3)":
            lambda state: state.can_reach("Twin Islands Hot Water Grotto Chest", "Location", player),
        "Twin Isles Hot Spring Water Grotto Bomb Boulders (4)":
            lambda state: state.can_reach("Twin Islands Hot Water Grotto Chest", "Location", player),                   
               
        # Goron Shrine Rocks
        "Goron Shrine Rocks (0)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (1)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (2)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (3)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (4)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (5)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (6)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (7)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (8)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (9)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (10)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (11)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (12)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (13)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (14)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),
        "Goron Shrine Rocks (15)":
            lambda state: (
                    has_soul_absurd(state, player, options, "Rocks") 
                and
                    (
                        state.has("Goron Mask", player) or
                        has_soul_npc(state, player, options, "Gatekeeper & Medigoron")
                    )
            ),

        # Lens Cave Boulders
        "Goron Village Lens Cave Bomb Boulder (1)":
            lambda state: has_explosives(state, player),
        "Goron Village Lens Cave Bomb Boulder (2)":
            lambda state: has_explosives(state, player),
        "Goron Village Lens Cave Bomb Boulder (3)":
            lambda state: has_explosives(state, player),

        # Mountain Village Spring Rock Triangle

        "Mountain Village Boulders Under Stairs (0)":
            lambda state: (
                state.can_reach("Mountain Village", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Boulders Under Stairs (1)":
            lambda state: (
                state.can_reach("Mountain Village", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements)
            ),   
        "Mountain Village Boulders Under Stairs (2)":
            lambda state: (
                state.can_reach("Mountain Village", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements)
            ),                                      
        
        "Mountain Village Spring Rock Triangle (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")  and
                state.can_reach("Mountain Village", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Rock Triangle (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")  and
                state.can_reach("Mountain Village", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Rock Triangle (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")  and
                state.can_reach("Mountain Village", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Rock Triangle (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")  and
                state.can_reach("Mountain Village", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Rock Triangle (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")  and
                state.can_reach("Mountain Village", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements)
            ),

        # Mountain Village Spring Outside Goron Graveyard
        "Mountain Village Spring Outside Goron Graveyard Rocks (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")  and
                state.can_reach("Mountain Village", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Outside Goron Graveyard Rocks (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")  and
                state.can_reach("Mountain Village", 'Region', player) and
                can_clear_snowhead(state, player, boss_placements)
            ),

        # Twin Isles Spring Above Grotto Rocks
        "Twin Isles Spring Above Grotto Rocks (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")  and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Isles Spring Above Grotto Rocks (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")  and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Isles Spring Above Grotto Rocks (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")  and
                can_clear_snowhead(state, player, boss_placements)
            ),
        # Great Bay Coast Scattered Beach Rocks
        "Great Bay Coast Scattered Beach Rocks (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Great Bay Coast Scattered Beach Rocks (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Great Bay Coast Scattered Beach Rocks (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Great Bay Coast Scattered Beach Rocks (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Great Bay Coast Scattered Beach Rocks (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        # Great Bay Coast Rock Wall Rocks
        "Great Bay Coast Rock Wall Rocks (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Great Bay Coast Rock Wall Rocks (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Great Bay Coast Rock Wall Rocks (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        # Great Bay Coast Underwater Rocks (Bombchus only)
        "Great Bay Coast Underwater Rocks (Bombchus only) Below Rock Pools (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_bombchus(state, player) and
                state.has("Zora Mask", player)
            ),
        "Great Bay Coast Underwater Rocks (Bombchus only) Next to Like-Like":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_bombchus(state, player) and
                state.has("Zora Mask", player)
            ),
        "Great Bay Coast Underwater Rocks (Bombchus only) Below Rock Pools (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_bombchus(state, player) and
                state.has("Zora Mask", player)
            ),
        "Great Bay Coast Underwater Rocks (Bombchus only) Near Rock Wall":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_bombchus(state, player) and
                state.has("Zora Mask", player)
            ),              

        # Rocks Underwater at Beach
        "Great Bay Coast Beach Rocks Underwater (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_bombchus(state, player)
            ),
        "Great Bay Coast Beach Rocks Underwater (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_bombchus(state, player)
            ),
        "Great Bay Coast Beach Rocks Underwater (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_bombchus(state, player)
            ),
        "Great Bay Coast Beach Rocks Underwater (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_bombchus(state, player)
            ),
        # Zora Cape Beach Rocks
        "Zora Cape Beach Rocks (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Beach Rocks (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Beach Rocks (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Beach Rocks (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Beach Rocks (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Beach Rocks (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Beach Rocks (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Beach Rocks (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Beach Rocks (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Beach Rocks (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Boulder Over Grotto":
            lambda state: (
                state.has("Goron Mask", player) or 
                has_explosives(state, player)
            ),

        # Zora Cape Island Rocks (Req Hook)
        "Zora Cape Island Rocks (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Zora Cape", 'Region', player) and
                state.has("Hookshot", player)
            ),
        "Zora Cape Island Rocks (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Zora Cape", 'Region', player) and
                state.has("Hookshot", player)
            ),
        "Zora Cape Island Rocks (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Zora Cape", 'Region', player) and
                state.has("Hookshot", player)
            ),
        "Zora Cape Island Rocks (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Zora Cape", 'Region', player) and
                state.has("Hookshot", player)
            ),
        "Zora Cape Island Rocks (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Zora Cape", 'Region', player) and
                state.has("Hookshot", player)
            ),

        "Zora Cape Great Fairy Boulders (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Zora Cape", 'Region', player) and
                state.has("Hookshot", player) and 
                has_explosives(state, player)
            ),
        "Zora Cape Great Fairy Boulders (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Zora Cape", 'Region', player) and
                state.has("Hookshot", player) and 
                has_explosives(state, player)
            ),            
        # Road To Ikana Rocks
        "Road To Ikana Grotto Boulder":
            lambda state: state.has("Goron Mask", player),
            
        "Road To Ikana Rock Circle (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") 
                and
                (
                    can_play_song("Epona's Song", state, player) or
                    can_use_owl(state, player, options, "Ikana Canyon") or
                    can_use_owl(state, player, options, "Stone Tower")
                )
            ),
        "Road To Ikana Rock Circle (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") 
                and
                (
                    can_play_song("Epona's Song", state, player) or
                    can_use_owl(state, player, options, "Ikana Canyon") or
                    can_use_owl(state, player, options, "Stone Tower")
                )
            ),
        "Road To Ikana Rock Circle (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") 
                and
                (
                    can_play_song("Epona's Song", state, player) or
                    can_use_owl(state, player, options, "Ikana Canyon") or
                    can_use_owl(state, player, options, "Stone Tower")
                )
            ),
        "Road To Ikana Rock Circle (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") 
                and
                (
                    can_play_song("Epona's Song", state, player) or
                    can_use_owl(state, player, options, "Ikana Canyon") or
                    can_use_owl(state, player, options, "Stone Tower")
                )
            ),
        "Road To Ikana Rock Circle (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") 
                and
                (
                    can_play_song("Epona's Song", state, player) or
                    can_use_owl(state, player, options, "Ikana Canyon") or
                    can_use_owl(state, player, options, "Stone Tower")
                )
            ),
        "Road To Ikana Rock Circle (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") 
                and
                (
                    can_play_song("Epona's Song", state, player) or
                    can_use_owl(state, player, options, "Ikana Canyon") or
                    can_use_owl(state, player, options, "Stone Tower")
                )
            ),
        "Road To Ikana Rock Circle (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") 
                and
                (
                    can_play_song("Epona's Song", state, player) or
                    can_use_owl(state, player, options, "Ikana Canyon") or
                    can_use_owl(state, player, options, "Stone Tower")
                )
            ),
        "Road To Ikana Rock Circle (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") 
                and
                (
                    can_play_song("Epona's Song", state, player) or
                    can_use_owl(state, player, options, "Ikana Canyon") or
                    can_use_owl(state, player, options, "Stone Tower")
                )
            ),
        
        "Road To Ikana Bomb Boulder (1)":
            lambda state: (
                has_explosives(state, player) and 
                (
                    (
                        has_soul_npc(state, player, options, "Spirit House Owner") and
                        can_play_song("Epona's Song", state, player) and
                        state.has("Hookshot", player) and
                        (
                            state.has("Garo's Mask", player) or
                            state.has("Gibdo Mask", player)
                        )
                    ) or 
                    (
                        can_use_owl(state, player, options, "Ikana Canyon") or
                        can_use_owl(state, player, options, "Stone Tower")
                    )
                )
            ),
        "Road To Ikana Bomb Boulder (2)":
            lambda state: (
                has_explosives(state, player) and 
                (
                    (
                        has_soul_npc(state, player, options, "Spirit House Owner") and
                        can_play_song("Epona's Song", state, player) and
                        state.has("Hookshot", player) and
                        (
                            state.has("Garo's Mask", player) or
                            state.has("Gibdo Mask", player)
                        )
                    ) or 
                    (
                        can_use_owl(state, player, options, "Ikana Canyon") or
                        can_use_owl(state, player, options, "Stone Tower")
                    )
                )
            ),      
        "Road To Ikana Bomb Boulder (3)":
            lambda state: (
                has_explosives(state, player) and 
                (
                    (
                        has_soul_npc(state, player, options, "Spirit House Owner") and
                        can_play_song("Epona's Song", state, player) and
                        state.has("Hookshot", player) and
                        (
                            state.has("Garo's Mask", player) or
                            state.has("Gibdo Mask", player)
                        )
                    ) or 
                    (
                        can_use_owl(state, player, options, "Ikana Canyon") or
                        can_use_owl(state, player, options, "Stone Tower")
                    )
                )
            ),
        "Road To Ikana Bomb Boulder (4)":
            lambda state: (
                has_explosives(state, player) and 
                (
                    (
                        has_soul_npc(state, player, options, "Spirit House Owner") and
                        can_play_song("Epona's Song", state, player) and
                        state.has("Hookshot", player) and
                        (
                            state.has("Garo's Mask", player) or
                            state.has("Gibdo Mask", player)
                        )
                    ) or 
                    (
                        can_use_owl(state, player, options, "Ikana Canyon") or
                        can_use_owl(state, player, options, "Stone Tower")
                    )
                )
            ),
        "Road To Ikana Bomb Boulder (5)":
            lambda state: (
                has_explosives(state, player) and 
                (
                    (
                        has_soul_npc(state, player, options, "Spirit House Owner") and
                        can_play_song("Epona's Song", state, player) and
                        state.has("Hookshot", player) and
                        (
                            state.has("Garo's Mask", player) or
                            state.has("Gibdo Mask", player)
                        )
                    ) or 
                    (
                        can_use_owl(state, player, options, "Ikana Canyon") or
                        can_use_owl(state, player, options, "Stone Tower")
                    )
                )
            ),
        "Road To Ikana Bomb Boulder (6)":
            lambda state: (
                has_explosives(state, player) and 
                (
                    (
                        has_soul_npc(state, player, options, "Spirit House Owner") and
                        can_play_song("Epona's Song", state, player) and
                        state.has("Hookshot", player) and
                        (
                            state.has("Garo's Mask", player) or
                            state.has("Gibdo Mask", player)
                        )
                    ) or 
                    (
                        can_use_owl(state, player, options, "Ikana Canyon") or
                        can_use_owl(state, player, options, "Stone Tower")
                    )
                )
            ),
        "Road To Ikana Bomb Boulder (7)":
            lambda state: (
                has_explosives(state, player) and 
                (
                    (
                        has_soul_npc(state, player, options, "Spirit House Owner") and
                        can_play_song("Epona's Song", state, player) and
                        state.has("Hookshot", player) and
                        (
                            state.has("Garo's Mask", player) or
                            state.has("Gibdo Mask", player)
                        )
                    ) or 
                    (
                        can_use_owl(state, player, options, "Ikana Canyon") or
                        can_use_owl(state, player, options, "Stone Tower")
                    )
                )
            ),
        # Ikana Graveyard Rock Circle
        "Ikana Graveyard Rock Circle (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Rock Circle (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Rock Circle (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Rock Circle (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Rock Circle (4)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Rock Circle (5)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Rock Circle (6)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Rock Circle (7)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),

        # Ikana Graveyard Captain Rockwall
        "Ikana Graveyard Captain Rockwall (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_explosives(state, player) and
                can_play_song("Sonata of Awakening", state, player)
            ),
        "Ikana Graveyard Captain Rockwall (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_explosives(state, player) and
                can_play_song("Sonata of Awakening", state, player)
            ),
        "Ikana Graveyard Captain Rockwall (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_explosives(state, player) and
                can_play_song("Sonata of Awakening", state, player)
            ),
        "Ikana Graveyard Captain Rockwall (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_explosives(state, player) and
                can_play_song("Sonata of Awakening", state, player)
            ),
        "Ikana Graveyard Captain Rockwall (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and
                state.can_reach("Ikana Graveyard", 'Region', player) and
                has_explosives(state, player) and
                can_play_song("Sonata of Awakening", state, player)
            ),

        # Inverted Stone Lower Tower Rocks
        "Inverted Stone Tower Rocks (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Inverted Stone Tower Rocks (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),

        # Woodsanity (Crates/Barrels/Bonk Boards)
        # Clock Town
        "Laundry Pool Crate (0)":
            lambda state: True,
        "East Clock Town Crates (0)":
            lambda state: True,
        "East Clock Town Crates (1)":
            lambda state: True,
        "Termina Field Business Scrub Grotto Crate (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos")
            ),

        "Gorman Racetrack Behind Fence Crate":
            lambda state: (
                has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer") or
                state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                can_play_song("Song of Time", state, player)
            ),
        # Romani Ranch Crates

        "Romani Ranch Crate Next To Romani (1)":
            lambda state: True,
        "Romani Ranch Baby Cuccoo Crates (0)":
            lambda state: True,
        "Romani Ranch Baby Cuccoo Crates (1)":
            lambda state: True,
        "Romani Ranch Baby Cuccoo Crates (2)":
            lambda state: True,

        # Swamp Spider House

        "Swamp Spider House Monument Room Crates (0)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spider House Monument Room Crates (1)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spider House Monument Room Crates (2)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spider House Monument Room Crates (3)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spider House Monument Room Crates (4)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spider House Monument Room Crates (5)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spider House Gold Room Crates (0)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),
        "Swamp Spider House Gold Room Crates (1)":
            lambda state: state.can_reach("Swamp Spider House", 'Region', player),

        # Goron Village

        "Goron Village Keg Goron Crate (1)":
            lambda state: can_use_fire_arrows(state,player),
        "Goron Village Keg Goron Crate (Spring) (1)":
            lambda state: can_clear_snowhead(state, player, boss_placements),

        # Ocean Spider House

        "Ocean Spiderhouse Basement Crate (1)":
            lambda state: (
                state.can_reach("Ocean Spider House", 'Region', player) and
                has_explosives(state, player) and
                state.has("Hookshot", player)
            ),

        # Pirates' Fortress Related

        "Pirates Fortress Entrance Bonk Board (0)":
            lambda state: (
                state.can_reach("Great Bay", 'Region', player) and
                state.has("Zora Mask", player)
            ),
        "Pirates Fortress Entrance Bonk Board (1)":
            lambda state: (
                state.can_reach("Great Bay", 'Region', player) and
                state.has("Zora Mask", player)
            ),
        "Pirates Fortress Entrance Bonk Board (2)":
            lambda state: (
                state.can_reach("Great Bay", 'Region', player) and
                state.has("Zora Mask", player)
            ),
        "Pirates Fortress Entrance Bonk Board (3)":
            lambda state: (
                state.can_reach("Great Bay", 'Region', player) and
                state.has("Zora Mask", player)
            ),
        "Pirates Fortress Sewers Bonk Board (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates Fortress Sewers Bonk Board (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates Fortress Sewers Bonk Board (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),

        # Pirate Fortress Sewers Barrels/Crates

        "Pirates' Fortress Sewers Barrel (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (15)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Barrel (16)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Exit Barrel (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Exit Barrel (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Exit Barrel (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Exit Barrel (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),
        "Pirates' Fortress Sewers Exit Barrel (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress Sewers", 'Region', player)
            ),

        # Pirates' Fortress Interior Crates

        "Pirates Fortress' Interior Crates (0)":
            lambda state: state.can_reach("Pirates' Fortress (Interior)", 'Region', player),
        "Pirates Fortress' Interior Crates (1)":
            lambda state: state.can_reach("Pirates' Fortress (Interior)", 'Region', player),
        "Pirates Fortress' Interior Crates (2)":
            lambda state: state.can_reach("Pirates' Fortress (Interior)", 'Region', player),
        "Pirates Fortress' Exterior Balcony Barrel (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                state.has("Hookshot", player)
            ),
        "Pirates' Fortress Leader's Room Barrel (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                state.has("Progressive Bow", player)
            ),
        "Pirates' Fortress Leader's Room Barrel (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                state.has("Progressive Bow", player)
            ),
        "Pirates' Fortress Guarded Bridge Barrel (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                state.has("Hookshot", player)
            ),
        "Pirates Fortress Interior Room Past Pink Guard Barrel (0)":
            lambda state: (
                has_soul_enemy(state, player, options, "Coloured Pirates") and
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Pirates' Fortress (Interior)", 'Region', player) and
                state.has("Hookshot", player)
            ),

        # Dungeon Woodsanity

        # Snowhead

        "Snowhead Temple Lava Bridge Room Crate (0)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),
        "Snowhead Temple Elevator Room Crates (0)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),
        "Snowhead Temple Elevator Room Crates (1)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),
        "Snowhead Temple Elevator Room Crates (2)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),
        "Snowhead Temple Elevator Room Crates (3)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),
        "Snowhead Temple Elevator Room Crates (4)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),

        "Snowhead Temple Timed Switch Puzzle Room Crate (0)":
            lambda state: (
                can_use_fire_arrows(state, player) or
                state.has("Small Key (Snowhead)", player, 2) and
                has_explosives(state, player)
            ),
        "Snowhead Temple Timed Switch Puzzle Room Crate (1)":
            lambda state: (
                can_use_fire_arrows(state, player) or
                state.has("Small Key (Snowhead)", player, 2) and
                has_explosives(state, player)
            ),
        # Great Bay

        "Great Bay Temple Entrance Barrels (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Great Bay Temple", 'Region', player)
            ),
        "Great Bay Temple Entrance Barrels (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Great Bay Temple", 'Region', player)
            ),
        "Great Bay Temple Entrance Barrels (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Great Bay Temple", 'Region', player)
            ),
        "Great Bay Temple Entrance Barrels (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Great Bay Temple", 'Region', player)
            ),
        "Great Bay Temple Entrance Barrels (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Great Bay Temple", 'Region', player)
            ),
        "Great Bay Temple Entrance Barrels (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Great Bay Temple", 'Region', player)
            ),
        "Great Bay Temple Entrance Barrels (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Great Bay Temple", 'Region', player)
            ),
        "Great Bay Temple Entrance Barrels (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels") and
                state.can_reach("Great Bay Temple", 'Region', player)
            ),
        "Great Bay Temple Room Behind 1F Waterfall Barrels (0)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                has_soul_absurd(state, player, options, "Barrels") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Room Behind 1F Waterfall Barrels (1)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                has_soul_absurd(state, player, options, "Barrels") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple Room Behind 1F Waterfall Barrels (2)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                has_soul_absurd(state, player, options, "Barrels") and
                can_use_ice_arrows(state, player)
            ),

        "Great Bay Temple 1F Red Valve Room Barrels (0)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                has_soul_absurd(state, player, options, "Barrels") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple 1F Red Valve Room Barrels (1)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                has_soul_absurd(state, player, options, "Barrels") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple 1F Red Valve Room Barrels (2)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                has_soul_absurd(state, player, options, "Barrels") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple 1F Red Valve Room Barrels (3)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                has_soul_absurd(state, player, options, "Barrels") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple 1F Red Valve Room Barrels (4)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                has_soul_absurd(state, player, options, "Barrels") and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple 1F Red Valve Room Crates (0)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple 1F Red Valve Room Crates (1)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple 1F Red Valve Room Crates (2)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple 1F Red Valve Room Crates (3)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player)
            ),
        "Great Bay Temple 1F Red Valve Room Crates (4)":
            lambda state: (
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player)
            ),

        "Great Bay Temple 1F Frog Miniboss Crates (0)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (1)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (2)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (3)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (4)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (5)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (6)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (7)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (8)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (9)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (10)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (11)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (12)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (13)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (14)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple 1F Frog Miniboss Crates (15)":
            lambda state: ( 
                state.can_reach("Great Bay Temple", 'Region', player) and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),

        "Great Bay Temple Seesaw Room Crates (0)":
            lambda state: ( 
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Seesaw Room Crates (1)":
            lambda state: ( 
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Seesaw Room Crates (2)":
            lambda state: ( 
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Seesaw Room Crates (3)":
            lambda state: ( 
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Seesaw Room Crates (4)":
            lambda state: ( 
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Seesaw Room Crates (5)":
            lambda state: ( 
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Seesaw Room Crates (6)":
            lambda state: ( 
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Crates (0)":
            lambda state: ( 
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Crates (1)":
            lambda state: ( 
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly") and
                can_use_ice_arrows(state, player) and
                can_use_fire_arrows(state, player)
            ),

        # Stone Tower Temple

        "Stone Tower Temple Entrance Room Crates (0)":
            lambda state: True,
        "Stone Tower Temple Entrance Room Crates (1)":
            lambda state: True,

        "Stone Tower Temple Mirror Room Crates (0)":
            lambda state: state.can_reach("Stone Tower Temple Mirror Room Sun Face Chest", "Location", player),
        "Stone Tower Temple Mirror Room Crates (1)":
            lambda state: state.can_reach("Stone Tower Temple Mirror Room Sun Face Chest", "Location", player),
        "Stone Tower Temple Eyegore Room Crates (0)":
            lambda state: (
                state.has("Zora Mask", player) and
                (
                    state.has("Goron Mask", player) or
                    can_use_light_arrows(state, player)
                )
            ),
        "Stone Tower Temple Eyegore Room Crates (1)":
            lambda state: (
                state.has("Zora Mask", player) and
                (
                    state.has("Goron Mask", player) or
                    can_use_light_arrows(state, player)
                ),
            ),
        "Stone Tower Temple Eyegore Room Crates (2)":
            lambda state: (
                state.has("Zora Mask", player) and
                (
                    state.has("Goron Mask", player) or
                    can_use_light_arrows(state, player)
                )
            ),

        "Stone Tower Temple Behind Bombable Wall Crates (0)":
            lambda state: (
                state.can_reach("Stone Tower Temple", 'Region', player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Behind Bombable Wall Crates (1)":
            lambda state: (
                state.can_reach("Stone Tower Temple", 'Region', player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Behind Bombable Wall Crates (2)":
            lambda state: (
                state.can_reach("Stone Tower Temple", 'Region', player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Behind Bombable Wall Crates (3)":
            lambda state: (
                state.can_reach("Stone Tower Temple", 'Region', player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Behind Bombable Wall Crates (4)":
            lambda state: (
                state.can_reach("Stone Tower Temple", 'Region', player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Behind Bombable Wall Crates (5)":
            lambda state: (
                state.can_reach("Stone Tower Temple", 'Region', player) and
                has_explosives(state, player)
            ),
        "Stone Tower Temple Behind Bombable Wall Crates (6)":
            lambda state: (
                state.can_reach("Stone Tower Temple", 'Region', player) and
                has_explosives(state, player)
            ),

        # Inverted Stone Tower

        "Stone Tower Temple Inverted Entry Crates (0)":
            lambda state: True,
        "Stone Tower Temple Inverted Entry Crates (1)":
            lambda state: True,
        "Stone Tower Temple Inverted Entry Crates (2)":
            lambda state: True,
        "Stone Tower Temple Inverted Entry Crates (3)":
            lambda state: True,
        "Stone Tower Temple Inverted Entry Crates (4)":
            lambda state: True,

        "Stone Tower Temple Inverted Thin Hallway Crates (0)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Death Armos Maze Chest", 'Location', player),
        "Stone Tower Temple Inverted Thin Hallway Crates (1)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Death Armos Maze Chest", 'Location', player),
        "Stone Tower Temple Inverted Thin Hallway Crates (2)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Death Armos Maze Chest", 'Location', player),
        "Stone Tower Temple Inverted Thin Hallway Crates (3)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Death Armos Maze Chest", 'Location', player),
        "Stone Tower Temple Inverted Thin Hallway Crates (4)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Death Armos Maze Chest", 'Location', player),
        "Stone Tower Temple Inverted Thin Hallway Crates (5)":
            lambda state: state.can_reach("Stone Tower Temple Inverted Death Armos Maze Chest", 'Location', player),

        # Icicles (ice ice baby)

        # Snowhead Temple

        "Snowhead Temple Entry Block Icicles (1)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),
        "Snowhead Temple Entry Block Icicles (2)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),
        "Snowhead Temple Entry Block Icicles (3)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),
        "Snowhead Temple Entry Block Icicles (4)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),
        "Snowhead Temple Entry Block Icicles (5)":
            lambda state: state.can_reach("Snowhead Temple", 'Region', player),

        "Snowhead Temple Grey Door Icicles (1)":
            lambda state: True,
        "Snowhead Temple Grey Door Icicles (2)":
            lambda state: True,
        "Snowhead Temple Grey Door Ceiling Icicles (1)":
            lambda state: True,
        "Snowhead Temple Grey Door Ceiling Icicles (2)":
            lambda state: True,

        "Snowhead Temple Frozen Block Ceiling Icicle (1)":
            lambda state: (
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 1) or 
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Frozen Block Ceiling Icicle (2)":
            lambda state: (
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 1) or 
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Frozen Block Ceiling Icicle (3)":
            lambda state: (
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 1) or 
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Frozen Block Ceiling Icicle (4)":
            lambda state: (
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 1) or 
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Frozen Block Ceiling Icicle (5)":
            lambda state: (
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 1) or 
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Ceiling Icicle (1)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Ceiling Icicle (2)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Ceiling Icicle (3)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Ceiling Icicle (4)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple Icicle Room Ceiling Icicle (5)":
            lambda state: (
                has_explosives(state, player) and
                (    
                    state.has("Small Key (Snowhead)", player, 1) or
                    state.has("Hookshot", player)
                )
            ),
        "Snowhead Temple 3F Behind Locked Door Icicles (1)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 1) and
                (
                state.has("Hookshot", player) and 
                state.has("Snowhead Temple Lower Scarecrow", player) and
                has_soul_npc(state, player, options, "Scarecrow") or
                can_use_fire_arrows(state, player)
                )
            ),

        "Snowhead Temple 3F Behind Locked Door Icicles (2)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 1) and
                (
                state.has("Hookshot", player) and 
                state.has("Snowhead Temple Lower Scarecrow", player) and
                has_soul_npc(state, player, options, "Scarecrow") or
                can_use_fire_arrows(state, player)
                )
            ),

        "Snowhead Temple 3F Behind Locked Door Icicles (3)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 1) and
                (
                state.has("Hookshot", player) and 
                state.has("Snowhead Temple Lower Scarecrow", player) and
                has_soul_npc(state, player, options, "Scarecrow") or
                can_use_fire_arrows(state, player)
                )
            ),
        "Snowhead Temple 3F Behind Locked Door Ceiling Icicles (1)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 1) and
                (
                state.has("Hookshot", player) and 
                state.has("Snowhead Temple Lower Scarecrow", player) and
                has_soul_npc(state, player, options, "Scarecrow") or
                can_use_fire_arrows(state, player)
                )
            ),
        "Snowhead Temple 3F Behind Locked Door Ceiling Icicles (2)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 1) and
                (
                state.has("Hookshot", player) and 
                state.has("Snowhead Temple Lower Scarecrow", player) and
                has_soul_npc(state, player, options, "Scarecrow") or
                can_use_fire_arrows(state, player)
                )
            ),
        "Snowhead Temple 3F Behind Locked Door Ceiling Icicles (3)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 1) and
                (
                state.has("Hookshot", player) and 
                state.has("Snowhead Temple Lower Scarecrow", player) and
                has_soul_npc(state, player, options, "Scarecrow") or
                can_use_fire_arrows(state, player)
                )
            ),
        "Snowhead Temple 3F Behind Locked Door Ceiling Icicles (4)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 1) and
                (
                state.has("Hookshot", player) and 
                state.has("Snowhead Temple Lower Scarecrow", player) and
                has_soul_npc(state, player, options, "Scarecrow") or
                can_use_fire_arrows(state, player)
                )
            ),
        "Snowhead Temple 4F Outside Wizzrobe Icicles (1)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 3) and
                can_use_fire_arrows(state, player) and
                state.has("Deku Mask", player)
            ),
        "Snowhead Temple 4F Outside Wizzrobe Icicles (2)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 3) and
                can_use_fire_arrows(state, player) and
                state.has("Deku Mask", player)
            ),

        "Snowhead Temple Outside Boss Door Icicles (1)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 3) and
                can_use_fire_arrows(state, player)
            ),
        "Snowhead Temple Outside Boss Door Icicles (2)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 3) and
                can_use_fire_arrows(state, player)
            ),        
        "Snowhead Temple Outside Boss Door Icicles (3)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 3) and
                can_use_fire_arrows(state, player)
            ),        
        "Snowhead Temple Outside Boss Door Icicles (4)":
            lambda state: (
                state.has("Small Key (Snowhead)", player, 3) and
                can_use_fire_arrows(state, player)
            ),
        # Great Bay Temple

        "Great Bay Temple Outside Frog Miniboss Door Icicles (1)":
            lambda state: can_use_ice_arrows(state, player),
        "Great Bay Temple Outside Frog Miniboss Door Icicles (2)":
            lambda state: can_use_ice_arrows(state, player),
        "Great Bay Temple Outside Frog Miniboss Door Icicles (3)":
            lambda state: can_use_ice_arrows(state, player),
        "Great Bay Temple Outside Frog Miniboss Door Icicles (4)":
            lambda state: can_use_ice_arrows(state, player),
        "Great Bay Temple Outside Frog Miniboss Door Icicles (5)":
            lambda state: can_use_ice_arrows(state, player),
        
        # Well

       "Bottom of the Well Icicle (1)":
            lambda state: state.can_reach("Ikana Well Rightside Torch Chest", "Location", player),
       "Bottom of the Well Icicle (2)":
            lambda state: state.can_reach("Ikana Well Rightside Torch Chest", "Location", player),

        # Goron Trial

        "Goron Trial Icicles (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                state.has("Goron Mask", player) and 
                state.has("Progressive Magic", player)
            ),
        "Goron Trial Icicles (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                state.has("Goron Mask", player) and 
                state.has("Progressive Magic", player)
            ),
        "Goron Trial Icicles (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                state.has("Goron Mask", player) and 
                state.has("Progressive Magic", player)
            ),
        "Goron Trial Icicles (4)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                state.has("Goron Mask", player) and 
                state.has("Progressive Magic", player)
            ),

        # Hivesanity

        # Termina Field

        "Termina Field Bombable Rock Grotto Hive (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_explosives(state, player) and 
                has_projectiles(state, player)
            ),


        "Termina Field Bio Baba Grotto Hive (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_explosives(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Termina Field Bio Baba Grotto Hive (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_explosives(state, player) and 
                state.has("Zora Mask", player)
            ),
        "Termina Field Cow Grotto Hive":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_explosives(state, player) and 
                has_projectiles(state, player)
            ),
        # Southern Swamp Hive

        "Southern Swamp Hive Near Frog (1)":
            lambda state: has_projectiles(state, player),

        # Swamp Spider House

        "Swamp Spider House Giant Pot Room Hives (1)":
            lambda state: has_projectiles(state, player),
        "Swamp Spider House Giant Pot Room Hives (2)":
            lambda state: has_projectiles(state, player),
        "Swamp Spider House Giant Pot Room Hives (3)":
            lambda state: has_projectiles(state, player),

        "Swamp Spider House Gold Room Hives (1)":
            lambda state: has_projectiles(state, player),
        "Swamp Spider House Gold Room Hives (2)":
            lambda state: has_projectiles(state, player),
        "Swamp Spider House Gold Room Hives (3)":
            lambda state: has_projectiles(state, player),

        "Swamp Spider Tree Room Hives (1)":
            lambda state: has_projectiles(state, player),
        "Swamp Spider Tree Room Hives (2)":
            lambda state: has_projectiles(state, player),
        "Swamp Spider Tree Room Hives (3)":
            lambda state: has_projectiles(state, player),

        # Woodfall Temple Hives

        "Woodfall Temple Entrance Hive (1)":
            lambda state: has_projectiles(state, player),

        "Woodfall Temple Push Block Hive (1)":
            lambda state: has_projectiles(state, player),

        # Mountain Village Spring Hives

        "Mountain Village Spring Tree Hive (1)":
            lambda state: can_clear_snowhead(state, player, boss_placements),
        "Great Bay Coast Cow Grotto Hive":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                state.has("Hookshot", player)
            ),
        # "Pirates Fortress Interior Leaders Hive From Lower Barrels":
        #     lambda state: (
        #         state.has("Hookshot", player) and 
        #         state.has("Stone Mask", player)
        #     ),

        #Scarecrowsanity
        "Clock Town Trading Post Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        "Astral Observatory Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        # Mountain Village Scarecrows
        "Mountain Village Rooftop Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        "Mountain Village Spring Rooftop Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        # Path to Snowhead Scarecrows
        "Path to Snowhead Scarecrow":
            lambda state: (
                can_use_lens(state, player) and
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player) and
                state.can_reach("Snowhead", 'Region', player)
            ),
        "Path to Snowhead Spring Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player) and
                can_use_lens(state, player) and
                can_clear_snowhead(state, player, boss_placements) and
                state.can_reach("Snowhead", 'Region', player)

            ),
        # Twin Islands Scarecrows
        "Twin Islands Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        "Twin Islands (Spring) Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player) and
                can_clear_snowhead(state, player, boss_placements)
            ),
        # Snowhead Temple Scarecrows
        "Snowhead Temple Lower Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        "Snowhead Temple Hidden Alcove Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        # Great Bay Scarecrows
        "Great Bay Coast Rock Wall Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        "Zora Cape Beavers Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        "Zora Cape Island Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        "Zora Hall Pervert Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        # Ikana Scarecrows
        "Road to Ikana Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player)
            ),
        # Stone Tower Scarecrows
        "Stone Tower Lower Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player) and
                (
                    (
                        can_use_ice_arrows(state, player) and
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player) and
                        can_play_song("Elegy of Emptiness", state, player)
                    )
                )
            ),
            
        "Stone Tower Upper Scarecrow":
            lambda state: (
                has_soul_npc(state, player, options, "Scarecrow") and
                state.has("Ocarina of Time", player) and
                (
                    (
                        can_use_ice_arrows(state, player) and
                        can_play_song("Elegy of Emptiness", state, player) and
                        state.has("Goron Mask", player) and
                        state.has("Zora Mask", player)
                    ) or
                    (
                        state.has("Stone Tower Owl Statue", player) and
                        can_play_song("Song of Soaring", state, player) and
                        can_play_song("Elegy of Emptiness", state, player)
                    )
                )
            ),

        # Real Fairysanity/Gossip Faires/Butterly Fairies

        # Termina Field Gossip Fairies

        "Termina Field Southern Tree Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        "Termina Field Near Thieving Bird Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        "Termina Field Near Bombable Rock Grotto Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        "Termina Field Gossip Grotto Gossip Fairy":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                (
                    can_play_song("Song of Healing", state, player) or
                    can_play_song("Epona's Song", state, player)
                )
            ),
        "Termina Field Near Songwall Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        "Termina Field Eastern Corner Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        "Termina Field Observatory Gossip Fairy":
            lambda state: (
                state.can_reach("Bomber's Hideout Astral Observatory", "Location", player) and
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),

        # Road to Southern Swamp Gossip Fairy

        "Road to Southern Swamp Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),

        # Southern Swamp Gossip Fairies

        "Southern Swamp Near Witch Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        
        "Southern Swamp Near Witch Post Dungeon Gossip Fairy":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and
                (
                    can_play_song("Song of Healing", state, player) or
                    can_play_song("Epona's Song", state, player)
                )
            ),


        "Swamp Spider House Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),

        # Milk Road Gossip Fairy

        "Milk Road Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),

        # Romani Ranch Gossip Fairies

        "Romani Ranch Entry Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        "Romani Ranch Tree Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        "Romani Ranch Near Barn Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        "Romani Ranch Baby Cuccoos Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        "Romani Ranch Doggy Racetrack Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),

        # Path to Mountain Village Gossip Fairy

        "Path To Mountain Village Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),

        # Mountain Village Gossip Fairies

        "Mountain Village Spring Waterfall Gossip Fairy":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                (
                    can_play_song("Song of Healing", state, player) or
                    can_play_song("Epona's Song", state, player)
                )
            ),
        "Mountain Village Spring Ramps To Goron Graveyard Gossip Fairy":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                (
                    can_play_song("Song of Healing", state, player) or
                    can_play_song("Epona's Song", state, player)
                )
            ),

        # Great Bay Coast Gossip Fairy

        "Great Bay Coast Rock Wall Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),

        # Zora Cape

        "Zora Cape Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),

        # Road to Ikana Gossip Fairy

        "Road To Ikana Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),

        # Ikana Canyon Gossip Fairy

        "Ikana Canyon Near Octoroks Gossip Fairy":
            lambda state: (
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),

        "Ikana Canyon Across Ocean Deed Ravine Gossip Fairy":
            lambda state:
            (
                state.has("Zora Mask", player) and
                state.has("Ocean Title Deed", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (
                        can_play_song("Song of Healing", state, player) or
                        can_play_song("Epona's Song", state, player)
                  )
            ),
        "Ikana Canyon Near Ghost House Gossip Fairy":
            lambda state:
            (
                (
                        can_play_song("Song of Healing", state, player) or
                        can_play_song("Epona's Song", state, player)
                )
            ),

        # Moon Gossip Fairies

        "Deku Trial Front Left Gossip":
            lambda state:
        (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
            (
                    can_play_song("Song of Healing", state, player) or
                    can_play_song("Epona's Song", state, player)
            )
        ),
        "Deku Trial Back Left Gossip":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Deku Trial Front Right Gossip":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Deku Trial Back Right Gossip":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Deku Trial Furthest Back Gossip":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    has_soul_absurd(state, player, options, "Deku Flowers") and
                    state.has("Deku Mask", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),

        # Goron Trial Gossip Fairies

        "Goron Trial 1st Gazebo Gossip (0)":
            lambda state:
            (
                has_soul_npc(state, player, options, "Moon Kids") and
                state.has("Goron Mask", player) and
                state.has("Progressive Magic", player) and
                (
                        can_play_song("Song of Healing", state, player) or
                        can_play_song("Epona's Song", state, player)
                )
            ),
        "Goron Trial 1st Gazebo Gossip (1)":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.has("Goron Mask", player) and
                    state.has("Progressive Magic", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Goron Trial 2nd Gazebo Gossip (0)":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.has("Goron Mask", player) and
                    state.has("Progressive Magic", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Goron Trial 2nd Gazebo Gossip (1)":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.has("Goron Mask", player) and
                    state.has("Progressive Magic", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Goron Trial Near Heart Piece Gossip (1)":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.has("Goron Mask", player) and
                    state.has("Progressive Magic", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),

        # Zora Trial
        "Zora Trial RRR Path Gossip":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.has("Zora Mask", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Zora Trial RRL Path Gossip":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.has("Zora Mask", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Zora Trial LRR Path Gossip":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.has("Zora Mask", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Zora Trial LRLL Path Gossip":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.has("Zora Mask", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Zora Trial LLL Path Gossip":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.has("Zora Mask", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),

        # Link Trial Gossip Fairies

        "Link Trial Gossip (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                can_play_song("Song of Healing", state, player) or
                can_play_song("Epona's Song", state, player)
            ),
        "Link Trial Gossip (2)":
            lambda state:
            (
                has_soul_npc(state, player, options, "Moon Kids") and
                state.has("Hookshot", player) and
                (
                        can_play_song("Song of Healing", state, player) or
                        can_play_song("Epona's Song", state, player)
                )
            ),

        "Link Trial Gossip (3)":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.has("Hookshot", player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),
        "Link Trial Gossip (4)":
            lambda state:
            (       
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.can_reach("Link Trial Gossip (3)", "Location", player) and
                    can_use_fire_arrows(state, player) and
                    has_bombchus(state, player) and
                        (
                        can_play_song("Song of Healing", state, player) or
                        can_play_song("Epona's Song", state, player)
                    )
            ),

        "Link Trial Gossip (5)":
            lambda state:
            (
                    has_soul_npc(state, player, options, "Moon Kids") and
                    state.can_reach("Link Trial Gossip (3)", "Location", player) and
                    can_use_fire_arrows(state, player) and
                    has_bombchus(state, player) and
                    (
                            can_play_song("Song of Healing", state, player) or
                            can_play_song("Epona's Song", state, player)
                    )
            ),


        # Well Fairies

        "Fairy Fountain Left Side Well (0)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_bottle(state, player, 1) and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_soul_npc(state, player, options, "Kotake") and
                        state.has("Mask of Scents", player)
                    ) or
                    (
                        has_soul_npc(state, player, options, "Business Scrubs") and
                        can_afford_price(state, player, 100)
                    )
                )
            ),
        "Fairy Fountain Left Side Well (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_bottle(state, player, 1) and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_soul_npc(state, player, options, "Kotake") and
                        state.has("Mask of Scents", player)
                    ) or
                    (
                        has_soul_npc(state, player, options, "Business Scrubs") and
                        can_afford_price(state, player, 100)
                    )
                )
            ),

        "Fairy Fountain Left Side Well (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_bottle(state, player, 1) and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_soul_npc(state, player, options, "Kotake") and
                        state.has("Mask of Scents", player)
                    ) or
                    (
                        has_soul_npc(state, player, options, "Business Scrubs") and
                        can_afford_price(state, player, 100)
                    )
                )
            ),

        "Fairy Fountain Left Side Well (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_bottle(state, player, 1) and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_soul_npc(state, player, options, "Kotake") and
                        state.has("Mask of Scents", player)
                    ) or
                    (
                        has_soul_npc(state, player, options, "Business Scrubs") and
                        can_afford_price(state, player, 100)
                    )
                )
            ),

        "Fairy Fountain Left Side Well (4)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_bottle(state, player, 1) and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_soul_npc(state, player, options, "Kotake") and
                        state.has("Mask of Scents", player)
                    ) or
                    (
                        has_soul_npc(state, player, options, "Business Scrubs") and
                        can_afford_price(state, player, 100)
                    )
                )
            ),

        "Fairy Fountain Left Side Well (5)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_bottle(state, player, 1) and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_soul_npc(state, player, options, "Kotake") and
                        state.has("Mask of Scents", player)
                    ) or
                    (
                        has_soul_npc(state, player, options, "Business Scrubs") and
                        can_afford_price(state, player, 100)
                    )
                )
            ),

        "Fairy Fountain Left Side Well (6)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_bottle(state, player, 1) and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_soul_npc(state, player, options, "Kotake") and
                        state.has("Mask of Scents", player)
                    ) or
                    (
                        has_soul_npc(state, player, options, "Business Scrubs") and
                        can_afford_price(state, player, 100)
                    )
                )
            ),

        "Fairy Fountain Left Side Well (7)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_bottle(state, player, 1) and
                state.has("Gibdo Mask", player) and
                (
                    (
                        has_soul_npc(state, player, options, "Kotake") and
                        state.has("Mask of Scents", player)
                    ) or
                    (
                        has_soul_npc(state, player, options, "Business Scrubs") and
                        can_afford_price(state, player, 100)
                    )
                )
            ),

        # Butterfly Fairies

        "Termina Field Near Peehat Grotto Butterfly Fairy (1)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Termina Field Near Peehat Grotto Butterfly Fairy (2)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),

        "Termina Field Cow Grotto Butterfly Fairy (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_explosives(state, player) and 
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Termina Field Cow Grotto Butterfly Fairy (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_explosives(state, player) and 
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Termina Field Cow Grotto Butterfly Fairy (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_explosives(state, player) and 
                has_soul_misc(state, player, options, "Butterflies")
            ),               

        "Termina Field Bombable Rock Grotto Butterfly Fairy (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies") and
                (
                    has_explosives(state, player) or
                    state.has("Goron Mask", player)
                )
            ), 
        "Termina Field Bombable Rock Grotto Butterfly Fairy (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies") and
                (
                    has_explosives(state, player) or
                    state.has("Goron Mask", player)
                )
            ),         
        # Deku Palace Butterflies

        "Deku Palace Bean Seller Butterfly Fairy (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Deku Palace Bean Seller Butterfly Fairy (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Deku Palace Bean Seller Butterfly Fairy (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Deku Palace Bean Seller Butterfly Fairy (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        # Mountain Village Spring Day Butterflies
        "Mountain Village Spring Day Butterfly (1)":
            lambda state: (
                has_soul_misc(state, player, options, "Butterflies") and
                can_clear_snowhead(state, player, boss_placements)
            ), 
        "Mountain Village Spring Day Butterfly (2)":
            lambda state: (
                has_soul_misc(state, player, options, "Butterflies") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Day Butterfly (3)":
            lambda state: (
                has_soul_misc(state, player, options, "Butterflies") and
                can_clear_snowhead(state, player, boss_placements)
            ),   
        "Mountain Village Spring Day Butterfly (4)":
            lambda state: (
                has_soul_misc(state, player, options, "Butterflies") and
                can_clear_snowhead(state, player, boss_placements)
            ),   
        "Mountain Village Spring Day Butterfly (5)":
            lambda state: (
                has_soul_misc(state, player, options, "Butterflies") and
                can_clear_snowhead(state, player, boss_placements)
            ),   
        "Mountain Village Spring Day Butterfly (6)":
            lambda state: (
                has_soul_misc(state, player, options, "Butterflies") and
                can_clear_snowhead(state, player, boss_placements)
            ),   
        "Mountain Village Spring Day Butterfly (7)":
            lambda state: (
                has_soul_misc(state, player, options, "Butterflies") and
                can_clear_snowhead(state, player, boss_placements)
            ),   
        "Mountain Village Spring Day Butterfly (8)":
            lambda state: (
                has_soul_misc(state, player, options, "Butterflies") and
                can_clear_snowhead(state, player, boss_placements)
            ),  
        "Mountain Village Spring Day Butterfly (9)":
            lambda state: (
                has_soul_misc(state, player, options, "Butterflies") and
                can_clear_snowhead(state, player, boss_placements)
            ),

        # Great Bay Coast Butterflies

        "Great Bay Coast Outside Fisherman Hut Butterfly Fairy (1)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Great Bay Coast Outside Fisherman Hut Butterfly Fairy (2)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),

        "Great Bay Coast Cow Grotto Butterfly Fairy (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Butterfly Fairy (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies") and
                state.has("Hookshot", player)
            ),
        "Great Bay Coast Cow Grotto Butterfly Fairy (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies") and
                state.has("Hookshot", player)
            ),                    

        # Moon Butterflies

        "Moon Butterfly Fairy (1)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (2)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (3)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (4)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (5)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (6)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (7)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (8)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (9)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (10)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (11)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (12)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Moon Butterfly Fairy (13)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),

        # Frogs

        "Laundry Pool Frog":
            lambda state: state.has("Don Gero's Mask", player),

        "Southern Swamp Frog":
            lambda state: state.has("Don Gero's Mask", player),

        "Woodfall Temple Miniboss Frog":
            lambda state: (
                has_soul_enemy(state, player, options, "Gekko") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and 
                can_smack(state, player) and
                state.has("Don Gero's Mask", player)
            ),
        "Great Bay Temple Miniboss Frog":
            lambda state: (
                state.has("Zora Mask", player) and 
                can_use_ice_arrows(state, player) and 
                can_use_fire_arrows(state, player) and
                state.has("Don Gero's Mask", player)
            ),
    # Owls

        "Clock Town Owl Statue":
            lambda state: True,
        "Milk Road Owl Statue":
            lambda state: True,
        "Southern Swamp Owl Statue":
            lambda state: True,
        "Woodfall Owl Statue":
            lambda state: state.can_reach("Woodfall", 'Region', player),
        "Mountain Village Owl Statue":
            lambda state: state.can_reach("Mountain Village", 'Region', player),
        "Snowhead Owl Statue":
            lambda state: state.can_reach("Snowhead", 'Region', player),
        "Great Bay Coast Owl Statue":
            lambda state: state.can_reach("Great Bay", 'Region', player),
        "Zora Cape Owl Statue":
            lambda state: state.can_reach("Zora Hall", 'Region', player),
        "Ikana Canyon Owl Statue":
            lambda state: state.can_reach("Upper Ikana Canyon", 'Region', player),
        "Stone Tower Owl Statue":
            lambda state: (
                (
                    state.has("Hookshot", player) and
                    can_play_song("Elegy of Emptiness", state, player) and
                    state.has("Goron Mask", player) and
                    state.has("Zora Mask", player)
                ) or
                (
                    state.has("Stone Tower Owl Statue", player) and
                    can_play_song("Song of Soaring", state, player)
                )
            ),

    # Notebook Events

        "Notebook Event Defended Against Aliens":
            lambda state: state.can_reach("Romani Ranch Aliens", "Location", player),
        "Notebook Event Delivered Pendant of Memories":
            lambda state: (
                state.has("Kafei's Mask", player) and
                state.can_reach("Stock Pot Inn Midnight Meeting", "Location", player)
            ),
        "Notebook Event Delivered Priority Mail":
            lambda state: (
                state.has("Priority Mail", player) and
                has_soul_npc(state, player, options, "Madame Aroma") and
                state.has("Kafei's Mask", player)
            ),
        "Notebook Event Deposited Letter To Kafei":
            lambda state: (
                has_soul_utility(state, player, options, "Postboxes") and
                state.has("Letter to Kafei", player)
            ),
        "Notebook Event Escaped Sakons Hideout":
            lambda state: (
                has_soul_utility(state, player, options, "Postboxes") and
                has_soul_npc(state, player, options, "Kafei") and
                state.has("Letter to Kafei", player) and
                state.can_reach("Lower Ikana Canyon", 'Region', player)
            ),
        "Notebook Event Escorted Cremia":
            lambda state: state.can_reach("Romani Ranch Helping Cremia", "Location", player),
        "Notebook Event Learned Secret Code":
            lambda state: (
                has_soul_npc(state, player, options, "Bomber Kids") and
                has_projectiles(state, player)
            ),
        "Notebook Event Promised Midnight Meeting":
            lambda state: (
                has_soul_npc(state, player, options, "Anju") and
                state.has("Kafei's Mask", player)
            ),
        "Notebook Event Promised To Help With Aliens":
            lambda state: state.can_reach("Romani Ranch Romani Game", "Location", player),
        "Notebook Event Promised To Meet Kafei":
            lambda state: (
                has_soul_npc(state, player, options, "Anju") and
                state.has("Kafei's Mask", player) and
                state.can_reach("Stock Pot Inn Midnight Meeting", "Location", player)
            ),
        "Notebook Event Received All Night Mask":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                can_purchase(state, player, prices, SHOP_ID_CURIOSITY_SHOP_MASK) and 
                state.can_reach("North Clock Town Save Old Lady", "Location", player)
            ),
        "Notebook Event Received Blast Mask":
            lambda state: state.can_reach("North Clock Town Save Old Lady", "Location", player),
        "Notebook Event Received Bombers NotebooK":
            lambda state: (
                has_soul_npc(state, player, options, "Bomber Kids") and
                has_projectiles(state, player)
            ),
        "Notebook Event Received Bremen Mask":
            lambda state: has_soul_npc(state, player, options, "Guru-Guru"),
        "Notebook Event Received Bunny Hood":
            lambda state: (
                has_soul_npc(state, player, options, "Grog") and
                state.has("Bremen Mask", player)
            ),
        "Notebook Event Received Circus Leaders Mask":
            lambda state: state.can_reach("Milk Bar Show", "Location", player),
        "Notebook Event Received Couples Mask":
            lambda state: state.can_reach("Stock Pot Inn Anju and Kafei", "Location", player),
        "Notebook Event Received Garos Mask":
            lambda state: (
                    has_soul_npc(state, player, options, "Gorman Brothers") and
                    can_play_song("Epona's Song", state, player)
            ),
                
        "Notebook Event Received Grandma Long Story HP":
            lambda state: (
                has_soul_npc(state, player, options, "Anju's Grandmother") and
                state.has("All-Night Mask", player)
            ),
        "Notebook Event Received Grandma Short Story HP":
            lambda state: (
                has_soul_npc(state, player, options, "Anju's Grandmother") and
                state.has("All-Night Mask", player)
            ),
        "Notebook Event Received Kafeis Mask":
            lambda state: has_soul_npc(state, player, options, "Madame Aroma"),
        "Notebook Event Received Kamaros Mask":
            lambda state: (
                has_soul_npc(state, player, options, "Kamaro") and
                can_play_song("Song of Healing", state, player)
            ),
        "Notebook Event Received Keaton Mask":
            lambda state: state.can_reach("Laundry Pool Curiosity Shop Salesman #1", "Location", player),
        "Notebook Event Received Letter To Kafei":
            lambda state: (
                state.has("Kafei's Mask", player) and
                state.can_reach("Stock Pot Inn Midnight Meeting", "Location", player)
            ),
        "Notebook Event Received Mayor HP":
            lambda state: (
                has_soul_npc(state, player, options, "Council") and
                state.has("Couple's Mask", player)
            ),
        "Notebook Event Received Milk Bottle":
            lambda state: state.can_reach("Romani Ranch Aliens", "Location", player),
        "Notebook Event Received Pendant of Memories":
            lambda state:(
                has_soul_npc(state, player, options, "Kafei") and
                has_soul_utility(state, player, options, "Postboxes") and
                state.has("Letter to Kafei", player)
            ),
        "Notebook Event Received Postman HP":
            lambda state: (
                has_soul_npc(state, player, options, "Postman") and
                state.has("Bunny Hood", player)
            ),
        "Notebook Event Received Postmans Hat":
            lambda state: (
                has_soul_npc(state, player, options, "Postman") and
                state.has("Priority Mail", player)
            ),
        "Notebook Event Received Priority Mail":
            lambda state: (
                has_soul_npc(state, player, options, "Kafei") and
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                has_soul_utility(state, player, options, "Postboxes") and
                state.has("Letter to Kafei", player)
            ),
        "Notebook Event Received Romanis Mask":
            lambda state: state.can_reach("Romani Ranch Helping Cremia", "Location", player),
        "Notebook Event Received Room Key":
            lambda state: has_soul_npc(state, player, options, "Anju"),
        "Notebook Event Received Rosa Sisters HP":
            lambda state: (
                has_soul_npc(state, player, options, "Rosa Sisters") and
                state.has("Kamaro's Mask", player)
            ),
        "Notebook Event Received Stone Mask":
            lambda state: state.can_reach("Road to Ikana Invisible Soldier", "Location", player),
        "Notebook Event Received Toilet Hand HP":
            lambda state: state.can_reach("Stock Pot Inn Midnight Toilet Hand", "Location", player),

        # Notebook Meetings

        "Notebook Meeting Africa (Toto)":
            lambda state: (
                has_soul_npc(state, player, options, "Toto & Gorman") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Anju":
            lambda state: (
                has_soul_npc(state, player, options, "Anju") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Anju's Grandmother":
            lambda state: (
                has_soul_npc(state, player, options, "Anju's Grandmother") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Bomb Shop Lady":
            lambda state: (
                has_soul_npc(state, player, options, "Bomb Granny") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Bombers":
            lambda state: (
                has_soul_npc(state, player, options, "Bomber Kids") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Cremia":
            lambda state: (
                has_soul_npc(state, player, options, "Romani & Cremia") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Curiosity Shop Man":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Gorman":
            lambda state: (
                has_soul_npc(state, player, options, "Toto & Gorman") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Gorman Brothers":
            lambda state: (
                has_soul_npc(state, player, options, "Gorman Brothers") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Grog":
            lambda state: (
                has_soul_npc(state, player, options, "Grog") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Guru Guru":
            lambda state: (
                has_soul_npc(state, player, options, "Guru-Guru") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Kafei":
            lambda state: (
                has_soul_npc(state, player, options, "Kafei") and
                has_soul_utility(state, player, options, "Postboxes") and
                state.has("Bomber's Notebook", player) and
                state.has("Letter to Kafei", player)
            ),
        "Notebook Meeting Kamaro":
            lambda state: (
                has_soul_npc(state, player, options, "Kamaro") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Madame Aroma":
            lambda state: (
                has_soul_npc(state, player, options, "Madame Aroma") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Mayor Dotour":
            lambda state: (
                has_soul_npc(state, player, options, "Council") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Postman":
            lambda state: (
                has_soul_npc(state, player, options, "Postman") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Romani":
            lambda state: (
                has_soul_npc(state, player, options, "Romani & Cremia") and
                has_notebook(state, player)
            ),
        "Notebook Meeting Rosa Sisters":
            lambda state: (
                has_soul_npc(state, player, options, "Rosa Sisters") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Shiro":
            lambda state: (
                has_soul_npc(state, player, options, "Shiro") and
                has_notebook(state, player) and
                state.has("Lens of Truth", player) and
                state.has("Progressive Magic", player) and
                (
                    can_play_song("Epona's Song", state, player) or
                    options.owlsanity.value 
                    and can_use_owl(state, player, options, "Ikana Canyon")
                )
            ),
                
        "Notebook Meeting Toilet Hand":
            lambda state: (
                has_soul_npc(state, player, options, "Toilet Hand") and
                state.has("Bomber's Notebook", player)
            ),
        #Treesanity 
        "North Clock Town Tree (1)":
        lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "North Clock Town Tree (2)":
        lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "West Clock Town Trading Post Bush (1)":
        lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "West Clock Town Trading Post Bush (2)":
        lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "West Clock Town Trading Post Bush (3)":
        lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "West Clock Town Trading Post Bush (4)":
        lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        # Termina Field -
        "Termina Field Tree Near Observatory (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Termina Field Tree Near Observatory (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Termina Field Tree Near Observatory (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),

        # Road to Southern Swamp  
        "Road to Southern Swamp Trees (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Road to Southern Swamp Trees (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Road to Southern Swamp Trees (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Road to Southern Swamp Trees (4)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Road to Southern Swamp Trees (5)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Road to Southern Swamp Trees (6)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),

        # Romani Ranch Trees and Bushes 
        "Romani Ranch Bush (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Bush (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Bush (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Bush (4)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Tree (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Tree (4)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Tree (5)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Tree (6)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Tree (7)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Baby Cucoo Tree":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Baby Cucoo Bush (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Baby Cucoo Bush (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Baby Cucoo Bush (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Baby Cucoo Bush (4)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Baby Cucoo Bush (5)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Baby Cucoo Bush (6)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Romani Ranch Baby Cucoo Bush (7)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),

        # Path To Mountains 
        "Path To Mountains Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Path To Mountains Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Path To Mountains Tree (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and 
                (
                    state.has("Goron Mask", player) or
                    has_explosives(state, player) or
                    can_use_fire_arrows(state, player) or 
                    can_use_owl(state, player, options, "Mountain Village")
                )
            ),
        "Path To Mountains Tree (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and 
                (
                    state.has("Goron Mask", player) or
                    has_explosives(state, player) or
                    can_use_fire_arrows(state, player) or 
                    can_use_owl(state, player, options, "Mountain Village")
                )
            ),

        # Twin Islands 
        "Twin Islands Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Twin Islands Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Twin Islands Tree (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") 
                and (
                    state.has("Goron Mask", player) 
                    or (
                        state.has("Hookshot", player) 
                        and state.has("Twin Islands Scarecrow", player) 
                        and has_soul_npc(state, player, options, "Scarecrow")
                    )
                )
            ),
        # Twin Islands Spring - Requires clearing Snowhead Temple (spring access)
        "Twin Islands (Spring) Tree (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands (Spring) Tree (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands (Spring) Tree (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                can_clear_snowhead(state, player, boss_placements)
            ),

        # Path To Snowhead - 
        "Path To Snowhead Tree Near Ledge":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Mountain Village", 'Region', player)
            ),
        "Path To Snowhead Tree (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Mountain Village", 'Region', player)
            ),
        "Path To Snowhead Tree (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Mountain Village", 'Region', player)
            ),
        "Path To Snowhead Tree (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Mountain Village", 'Region', player)
            ),

        # Goron Racetrack - 
        "Goron Racetrack Trees (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Goron Racetrack", 'Region', player)
            ),
        "Goron Racetrack Trees (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Goron Racetrack", 'Region', player)
            ),
        "Goron Racetrack Trees (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Goron Racetrack", 'Region', player)
            ),
        "Goron Racetrack Trees (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Goron Racetrack", 'Region', player)
            ),
        "Goron Racetrack Trees (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Goron Racetrack", 'Region', player)
            ),
        "Goron Racetrack Trees (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Goron Racetrack", 'Region', player)
            ),
        "Goron Racetrack Trees (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Goron Racetrack", 'Region', player)
            ),
        "Goron Racetrack Trees (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Goron Racetrack", 'Region', player)
            ),
        "Goron Racetrack Trees (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Goron Racetrack", 'Region', player)
            ),
        "Goron Racetrack Trees (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Goron Racetrack", 'Region', player)
            ),

        # Gorman Brothers Track -
        "Gorman Racetrack Tree Group 1 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 1 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 1 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 1 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 1 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 1 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 1 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 1 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 1 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 1 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 1 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                can_play_song("Song of Time", state, player)
            ),
        "Gorman Racetrack Tree Group 1 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        "Gorman Racetrack Tree Group 2 (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                (
                    (
                        state.can_reach("Romani Ranch Helping Cremia", "Location", player) and
                        can_play_song("Song of Time", state, player)
                    ) or
                    has_soul_absurd(state, player, options, "Gorman Ranch Bulldozer")
                )
            ),
        # Great Bay Coast Nut Trees - Requires access to the region
        "Great Bay Coast Nut Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Great Bay Coast Nut Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Great Bay Coast Nut Tree (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Great Bay Coast Fisherman Island Nut Tree (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                can_clear_greatbay(state, player, boss_placements)
            ),

        # Zora Cape Nut Trees 
        "Zora Cape Nut Tree Near Jars (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Zora Cape Nut Tree Near Jars (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Zora Cape Nut Tree On Islands (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.has("Hookshot", player)
            ),
        "Zora Cape Nut Tree On Islands (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.has("Hookshot", player)
            ),
        "Zora Cape Nut Tree On Islands (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.has("Hookshot", player)
            ),
        "Zora Cape Nut Tree On Turtle Island (1)":
            lambda state: (
                state.has("Zora Mask", player) and
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.has("Hookshot", player)
            ),
        "Zora Cape Nut Tree On Turtle Island (2)":
            lambda state: (
                state.has("Zora Mask", player) and
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                state.has("Hookshot", player)
            ),
        "Beneath The Well Tree Near Cow":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_play_song("Epona's Song", state, player) and 
                    (
                        can_plant_beans(state, player, options) or 
                        can_use_light_arrows(state, player)
                    ) and 
                    (
                        (
                            can_play_song("Song of Soaring", state, player) and 
                            (
                                (
                                    state.can_reach("Twin Islands", 'Region', player) and 
                                    can_use_fire_arrows(state, player)
                                ) or 
                                (
                                    can_use_lens(state, player) and 
                                    state.has("Goron Mask", player)
                                )
                            )
                        ) or 
                        state.can_reach("Ikana Well Invisible Chest", "Location", player)
                    )
                )
            ),
        "Beneath The Well Bush Near Cow (1)": 
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_play_song("Epona's Song", state, player) and 
                    (
                        can_plant_beans(state, player, options) or 
                        can_use_light_arrows(state, player)
                    ) and 
                    (
                        (
                            can_play_song("Song of Soaring", state, player) and 
                            (
                                (
                                    state.can_reach("Twin Islands", 'Region', player) and 
                                    can_use_fire_arrows(state, player)
                                ) or 
                                (
                                    can_use_lens(state, player) and 
                                    state.has("Goron Mask", player)
                                )
                            )
                        ) or 
                        state.can_reach("Ikana Well Invisible Chest", "Location", player)
                    )
                )
            ),
        "Beneath The Well Bush Near Cow (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (
                    can_play_song("Epona's Song", state, player) and 
                    (
                        can_plant_beans(state, player, options) or 
                        can_use_light_arrows(state, player)
                    ) and 
                    (
                        (
                            can_play_song("Song of Soaring", state, player) and 
                            (
                                (
                                    state.can_reach("Twin Islands", 'Region', player) and 
                                    can_use_fire_arrows(state, player)
                                ) or 
                                (
                                    can_use_lens(state, player) and 
                                    state.has("Goron Mask", player)
                                )
                            )
                        ) or 
                        state.can_reach("Ikana Well Invisible Chest", "Location", player)
                    )
                )
            ),
        #Flower Sanity
        "Before Clock Town Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Flower (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Flower (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Flower (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Flower (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Flower (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Before Clock Town Flower (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "South Clock Town Business Scrub Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                state.has("Moon's Tear", player)

            ),
        "East Clock Town Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Termina Field Flower Near Observatory":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Termina Field Flower Near Skullkid Drawing":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Termina Field Flower Near Stump":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Termina Field Flower Near Giant Log":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        # Road To Swamp Flowers
        "Road to Southern Swamp Flowers (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Road to Southern Swamp Flowers (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Road to Southern Swamp Flowers (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Road to Southern Swamp Flowers (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Road to Southern Swamp Flowers (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Road to Southern Swamp Flowers (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Road to Southern Swamp Flowers (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Road to Southern Swamp Flowers (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Business Scrub Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                state.has("Land Title Deed", player)
            ), 
        "Southern Swamp Woods of Mystery Flower Day 1/3 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Woods of Mystery Flower Day 1/3 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Woods of Mystery Flower Any Day (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Woods of Mystery Flower Any Day (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Woods of Mystery Flower Any Day (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Woods of Mystery Flower Day 2 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Woods of Mystery Flower Day 2 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        # Deku Palace Flowers
        "Deku Palace Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_plant_beans(state, player, options) and
                state.has("Deku Mask", player)
            ),
        "Deku Palace To Swamp Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        # Swamp Spider Flowers
        "Swamp Spiderhouse Main Room Flowers (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        
        "Swamp Spiderhouse Main Room Flowers (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Swamp Spiderhouse Main Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        
        "Swamp Spiderhouse Giant Pot Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
            # Requires Sonata
        "Swamp Spiderhouse Giant Pot Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                can_play_song("Sonata of Awakening", state, player)
            ),
        # Heading to Woodfall
        "Southern Swamp Path To Woodfall Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Path To Woodfall Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Path To Woodfall Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Path To Woodfall Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Path To Woodfall Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Path To Woodfall Flower (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Flower (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Flower (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Flower (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Flower (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        # Woodfall Temple Flowers
        "Woodfall Temple Entrance Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Entrance Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Entrance Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Entrance Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Main Room Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Elevator Room Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Elevator Room Upper Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Elevator Room Upper Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Snapping Turtle Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Snapping Turtle Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Snapping Turtle Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Gekko Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Gekko Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Gekko Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Gekko Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Gekko Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player)
            ),
        "Woodfall Temple Dinolfos Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Dinolfos Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple 2F Moving Platform Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple 2F Moving Platform Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple 2F Moving Platform Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple 2F Moving Platform Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Dragonfly Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Dragonfly Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Dragonfly Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Pre Boss Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Pre Boss Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Pre Boss Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Pre Boss Room Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Pre Boss Room Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Pre Boss Room Flower (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Pre Boss Room Flower (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Pre Boss Room Flower (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Pre Boss Room Flower (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Pre Boss Room Flower (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                (state.has("Small Key (Woodfall)", player, 1) or
                state.has("Hookshot", player))
            ),
        "Woodfall Temple Odolwa Golden Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.has("Progressive Bow", player) and
                    (state.has("Boss Key (Woodfall)", player) or 
                    (
                        state.has("Odolwa's Remains", player) and 
                        options.remains_allow_boss_warps.value))
            ),
        "Southern Swamp Post Dungeon Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Deku Mask", player) 
            ),
        "Southern Swamp Post Dungeon Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                can_clear_woodfall(state, player, boss_placements) and
                state.has("Deku Mask", player) 
            ),
        "Southern Swamp Post Dungeon Business Scrub Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_clear_woodfall(state, player, boss_placements)
            ), 
            #Snowhead Flowers
        "Goron Village Business Scrub Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                state.has("Swamp Title Deed", player)
            ), 
        "Snowhead Temple Frozen Green Door Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Snowhead Temple Frozen Green Door Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Snowhead Temple Main Room Wall Chest Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                (can_use_fire_arrows(state, player) or
                state.has("Small Key (Snowhead)", player, 2)) 
            ),
        "Snowhead Temple Flower Outside Goht":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player) and 
                (
                    state.has("Small Key (Snowhead)", player, 3) or 
                    (
                        state.has("Small Key (Snowhead)", player, 2) and 
                        state.has("Hookshot", player) and 
                        can_reach_scarecrow(state, player, options) and
                        state.has("Snowhead Temple Lower Scarecrow", player)
                    )
                )
            ),
            #Great Bay Flowers
        "Zora Cape Lower Wall Flower Near Beavers":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                state.has("Hookshot", player)
            ), 
        "Zora Hall Business Scrub Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                state.has("Zora Mask", player) and 
                state.has("Goron Mask", player) and
                state.has("Mountain Title Deed", player)
            ), 
            #Ikana Flowers
        "Ikana Canyon Business Scrub Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Deku Mask", player) and 
                state.has("Zora Mask", player) and 
                state.has("Ocean Title Deed", player)
            ), 
        "Ikana Canyon Business Scrub Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Deku Mask", player) and 
                state.has("Zora Mask", player) and 
                state.has("Ocean Title Deed", player)
            ), 
        "Well Deku Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Deku Mask", player) and 
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (   
                    (
                        can_afford_price(state, player, 100) and
                        has_soul_npc(state, player, options, "Business Scrubs")
                    ) or 
                    (
                        state.has("Mask of Scents", player) and
                        has_soul_npc(state, player, options, "Kotake")
                    )
                )
            ),
        "Well Deku Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Deku Mask", player) and 
                state.has("Gibdo Mask", player) and 
                has_bottle(state, player) and 
                (   
                    (
                        can_afford_price(state, player, 100) and
                        has_soul_npc(state, player, options, "Business Scrubs")
                    ) or 
                    (
                        state.has("Mask of Scents", player) and
                        has_soul_npc(state, player, options, "Kotake")
                    )
                )
            ),
        "Ikana Castle Left Side Falling Ceiling Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Left Side Falling Ceiling Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Left Side Falling Ceiling Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Left Side Falling Ceiling Room Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Left Side Falling Ceiling Room Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Left Side Broken Floor Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Left Side Broken Floor Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Left Side Broken Floor Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Exterior Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Exterior Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Exterior Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Ikana Castle Exterior Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and 
                can_use_fire_arrows(state, player)
            ), 
        "Stone Tower Temple Deku Updraft Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                (
                    (
                        state.has("Zora Mask", player) and
                        state.has("Goron Mask", player) and
                        has_mirror_shield(state, player) and
                        state.has("Small Key (Stone Tower)", player, 2)
                    ) or
                    (
                        can_use_light_arrows(state, player) and
                        state.has("Small Key (Stone Tower)", player, 1)
                    )
                )
            ),
        "Stone Tower Temple Inverted Eastern Air Gust Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Stone Tower Temple Inverted Eastern Air Gust Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Inverted Stone Tower Temple Small Poe Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Small Poe Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Lower Bridge Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Lower Bridge Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player)
            ),
        "Inverted Stone Tower Temple Lower Bridge Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                state.can_reach("Stone Tower Temple Inverted Wizzrobe Chest", "Location", player)
            ),
        "The Moon Deku Trial Flower (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "The Moon Deku Trial Flower (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "The Moon Deku Trial Flower (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "The Moon Deku Trial Flower (4)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "The Moon Deku Trial Flower (5)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "The Moon Deku Trial Flower (6)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "The Moon Deku Trial Flower (7)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "The Moon Deku Trial Flower (8)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "The Moon Deku Trial Flower (9)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "The Moon Deku Trial Flower (10)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "The Moon Deku Trial Flower (11)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),

                # Signs (BREAK THE TARGETS)

        "North Clock Town Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "West Clock Town Sword School Night 3 Midnight Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and
                has_soul_npc(state, player, options, "Swordsman") and
                    (
                        state.has("Goron Mask", player) or
                        state.has("Zora Mask", player) or
                        state.has("Deku Mask", player) or
                        state.has("Blast Mask", player) or
                        state.has("Progressive Sword", player)
                    )
            ),
        "East Clock Town Milk Bar Roof Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Termina Field Takkuri Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Road to Southern Swamp Entry Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Southern Swamp Tourist Centre Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Southern Swamp Witch's Hut Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Southern Swamp Outside Woods of Mystery Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Southern Swamp (Clear State) Witch's Hut Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs") and can_clear_woodfall(state, player, boss_placements),
        "Woods of Mystery Cut the Sign Day 1 (1)":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Woods of Mystery Cut the Sign Day 1 (2)":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Woods of Mystery Cut the Sign Day 1 (3)":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Woods of Mystery Cut the Sign Day 2 (1)":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Woods of Mystery Cut the Sign Day 2 (2)":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Woods of Mystery Cut the Sign Day 2 (3)":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Woods of Mystery Cut the Sign Day 3 (1)":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Woods of Mystery Cut the Sign Day 3 (2)":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Woods of Mystery Cut the Sign Day 3 (3)":
            lambda state: has_soul_absurd(state, player, options, "Signs"),

        "Southern Swamp Log Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and
                (
                    state.has("Zora Mask", player) or
                    state.has("Deku Mask", player)
                )
            ),
        "Southern Swamp Outside Spider House":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Swamp Spider House Behind Statue Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),

        "Deku Palace Behind Entrance Guards Cut Left Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Deku Palace Behind Entrance Guards Cut Right Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Deku Palace Behind Entrance Guards Cut Sign Near King's Chamber":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Deku Palace Bean Daddy Grotto Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Signs")
            ),

        "Path to Mountain Village Cut the Sign":
            lambda state:
                (
                    has_soul_absurd(state, player, options, "Signs") and
                    (
                        state.has("Goron Mask", player) or
                        has_explosives(state, player) or
                        can_use_fire_arrows(state, player)
                    )
                ),
        "Mountain Village Owl Statue Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Mountain Village Outside Smithy Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Mountain Village Bridge Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Mountain Village Pond Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Mountain Village Twin Island Entrance Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Twin Islands Outside Goron Racetrack Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and
                (
                    state.has("Goron Mask", player) or
                    (
                        state.has("Hookshot", player) and
                        has_soul_npc(state, player, options, "Scarecrow") and
                        state.has("Twin Islands Scarecrow", player)
                    )
                )
            ),
        "Goron Village Outside Lens Cave Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Goron Village Outside Keg Goron Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Goron Village Outside Goron Shrine":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Path to Snowhead Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and
                state.can_reach("Mountain Village", 'Region', player)
            ),
        "Path to Snowhead Upper Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and
                state.can_reach("Snowhead", 'Region', player)
            ),
        "Outside Snowhead Temple Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Mountain Village Owl Statue Spring Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and 
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Near Graveyard Pond Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and 
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Mountain Village Spring Path to Twin Islands Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and 
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Twin Islands Spring Outside Goron Racetrack Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and 
                can_clear_snowhead(state, player, boss_placements)
            ),
        "Romani Ranch Epona Stable Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and
                can_play_song("Epona's Song", state, player)
            ),
        "Romani Ranch Doggy Racetrack Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Gorman Racetrack Fence Day 3 Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Great Bay Coast Entrance Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Great Bay Coast Outside Fisherman Hut Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Great Bay Coast Beachfront Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Great Bay Coast Entrance to Zora Cape Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Great Bay Coast Marine Lab Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Great Bay Coast Rock Pools Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Great Bay Coast (Clear) Fisherman Boat Cut the Sign":
            lambda state: can_clear_greatbay(state, player, boss_placements) and has_soul_absurd(state, player, options, "Signs"),
        "Zora Cape Jar Game Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Zora Cape Waterfall Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Zora Cape Turtle Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),

        "Ikana Canyon River Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Ikana Canyon Outside Sakon's Hideout Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Ikana Canyon Outside Secret Shrine Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Ikana Canyon Outside Spring Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Ikana Canyon Outside Ikana Castle Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Ikana Canyon Poe Hut Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Ikana Canyon Outside Well Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        
        # Websanity
        "Swamp Spider House Entrance Web":
            lambda state: (
                (
                    state.has("Deku Mask", player) and
                    has_hard_projectiles(state, player)
                ) or
                (
                    has_bottle(state, player) and
                    has_soul_npc(state, player, options, "Kotake")
                ) or
                (
                    state.has("Pictograph Box", player) and
                    has_soul_npc(state, player, options, "Swamp Tourist Guide")
                )
            ),
        "Swamp Spider House Entrance Web Cleared Swamp":
            lambda state: can_clear_woodfall(state, player, boss_placements),
        "Woodfall Temple Web Leading to Dark Room":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ocean Spider House Entrance Web (1)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Entrance Web (2)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Library Web":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Web Above Door 1st Floor Door":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Web Over 1st Floor Pot":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Web 1st Floor Near Staircase":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Web Basement Near Staircase":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Web Basement Covering Crates":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Web Basement Covering Hole":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Web Basement Covering Door":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Web Boat Room Covering Crate":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Boat Room Ceiling Web":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Coloured Mask Ceiling Web (1)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Ocean Spider House Coloured Mask Ceiling Web (2)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player) and
                can_use_fire_arrows(state, player)
            ),
        "Beneath the Well Left Side Web Near Fairy Fountain (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                has_bottle(state, player) and
                (
                    (
                        can_afford_price(state, player, 100) and
                        has_soul_npc(state, player, options, "Business Scrubs")
                    ) or
                    (
                        state.has("Mask of Scents", player) and
                        has_soul_npc(state, player, options, "Kotake")
                    )
                ) and
                can_use_fire_arrows(state, player)
            ),
        "Beneath the Well Left Side Web Near Fairy Fountain (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                has_bottle(state, player) and
                (
                    (
                        can_afford_price(state, player, 100) and
                        has_soul_npc(state, player, options, "Business Scrubs")
                    ) or
                    (
                        state.has("Mask of Scents", player) and
                        has_soul_npc(state, player, options, "Kotake")
                    )
                ) and
                can_use_fire_arrows(state, player)
            ),
        "Beneath the Well Right Side Web Near Milk Gibdo":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                state.has("Gibdo Mask", player) and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player) and
                has_bottle(state, player) and
                (
                    (
                        can_afford_price(state, player, 100) and
                        has_soul_npc(state, player, options, "Business Scrubs")
                    ) or
                    (
                        state.has("Mask of Scents", player) and
                        has_soul_npc(state, player, options, "Kotake")
                    )
                ) and
                can_use_fire_arrows(state, player)
            ),

        # Oneoffs
        "Ocean Spider House Behind Top Webbed Door Painting (1)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Behind Top Webbed Door Painting (2)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Behind Top Webbed Door Painting (3)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Behind Top Webbed Door Painting (4)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Coloured Mask Painting (1)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Coloured Mask Painting (2)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Coloured Mask Painting (3)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        "Ocean Spider House Coloured Mask Painting (4)":
            lambda state: (
                state.has("Hookshot", player) and
                has_explosives(state, player)
            ),
        # Bombable Walls 
        "Bombers Bombable Wall":
            lambda state: (
                has_hard_projectiles(state, player) and
                has_explosives(state, player)
            ),
        "Snowhead Temple Bombable Wall":
            lambda state: (
                has_explosives(state, player)
            ),
        "Ocean Spider House Entrance Wall":
            lambda state: (
                has_explosives(state, player)
            ),
        "Stone Tower Temple Elegy Maze Bombable Wall":
            lambda state: (
                has_explosives(state, player)
            ),
        "Graveyard Day 2 Bombable Wall":
            lambda state: (
                has_explosives(state, player)
            ),
        "Link Trial Bombable Wall Iron Knuckle":
            lambda state: (
                has_bombchus(state, player)
            ),
        "Link Trial Bombable Wall Final Door":
            lambda state: (
                has_bombchus(state, player)
            ),
        "Goron Trial Chests":
            lambda state: (
                state.has("Goron Mask", player)
            ),
        "Majora's Soul":
            lambda state: (
                has_enough_remains(state, player, 4) and
                has_enough_masks(state, player, 24) and
                has_all_owls(state, player, options) and
                has_all_scarecrows(state, player, options) and
                has_all_frogs(state, player, options) and
                has_all_trade_items(state, player) and
                state.has("Stray Fairy (Clock Town)", player, 1) and
                state.has("Stray Fairy (Woodfall)", player, 15) and
                state.has("Stray Fairy (Snowhead)", player, 15) and
                state.has("Stray Fairy (Great Bay)", player, 15) and
                state.has("Stray Fairy (Stone Tower)", player, 15) and
                state.has("Swamp Skulltula Token", player, 30) and
                state.has("Ocean Skulltula Token", player, 30)
            ),
    }
