from .Constants import *
from .NormalRules import *


def get_nearly_region_rules(player, options):
    return {
        "Clock Town -> The Moon":
            lambda state: (
                state.has("Oath to Order", player) and
                has_enough_remains(state, player, options.moon_remains_required.value) and
                has_enough_masks(state, player, options.moon_masks_required.value) and
                has_enough_items(state, player, options.moon_items_required.value) and
                (not options.moon_star_fox.value or has_star_fox(state, player, options, "moon")) and
                has_all_frogs(state, player, options, "moon") and
                has_all_scarecrows(state, player, options, "moon") and
                has_all_owls(state, player, options, "moon")
            ),
        "Woodfall -> Woodfall Temple":
            lambda state: (
                state.has("Deku Mask", player) and
                state.has("Sonata of Awakening", player) and
                (
                    state.has("Progressive Magic", player) or
                    state.has("Ocarina of Time", player)
                )
            ),
        "Pirates' Fortress -> Pirates' Fortress Sewers":
            lambda state: True,
        "Zora Cape -> Great Bay Temple":
            lambda state: (
                state.has("Zora Mask", player) and
                state.has("New Wave Bossa Nova", player)
            ),
        "Upper Ikana Canyon -> Beneath the Well":
            lambda state: has_soul_npc(state, player, options, "Gibdos"),
        "Beneath the Well -> Ikana Castle":
            lambda state: has_soul_npc(state, player, options, "Gibdos"),
    }


def get_nearly_location_rules(player, options, prices, boss_placements):
    return {
        "Keaton Quiz":
            lambda state: has_soul_misc(state, player, options, "Keaton"),
        "Clock Town Postbox":
            lambda state: has_soul_utility(state, player, options, "Postboxes"),
        "Top of Clock Tower (Ocarina of Time)":
            lambda state: has_soul_npc(state, player, options, "Skull Kid"),
        "Top of Clock Tower (Song of Time)":
            lambda state: has_soul_npc(state, player, options, "Skull Kid"),
        "North Clock Town Tree HP":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "North Clock Town Deku Playground Any Day":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "North Clock Town Deku Playground All Days":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "North Clock Town Save Old Lady":
            lambda state: (
                has_soul_npc(state, player, options, "Bomb Granny") and
                has_soul_npc(state, player, options, "Sakon")
            ),
        "North Clock Town Great Fairy Reward (Has Transformation Mask)":
            lambda state: has_soul_npc(state, player, options, "Great Fairies"),
        "North Clock Town Great Fairy Reward":
            lambda state: has_soul_npc(state, player, options, "Great Fairies"),
        "Clock Town Hide-and-Seek":
            lambda state: has_soul_npc(state, player, options, "Bomber Kids"),
        "Tingle Clock Town Map Purchase":
            lambda state: has_soul_npc(state, player, options, "Tingle"),
        "South Clock Town Moon's Tear Trade":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and 
                state.has("Moon's Tear", player)
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
            lambda state: True,
        "East Clock Town Mayors Wife":
            lambda state: has_soul_npc(state, player, options, "Madame Aroma"),
        "East Clock Town Couples Mask on Mayor":
            lambda state: (
                has_soul_npc(state, player, options, "Council") and
                state.has("Couple's Mask", player)
            ),
        "East Clock Town Shooting Gallery 40-49 Points":
            lambda state: has_soul_npc(state, player, options, "Archery Man"),
        "East Clock Town Shooting Gallery Perfect 50 Points":
            lambda state: has_soul_npc(state, player, options, "Archery Man"),
        "East Clock Town Honey and Darling Any Day":
            lambda state: has_soul_npc(state, player, options, "Honey and Darling"),
        "East Clock Town Honey and Darling All Days":
            lambda state: has_soul_npc(state, player, options, "Honey and Darling"),
        "East Clock Town Treasure Game Chest (Human)":
            lambda state: has_soul_npc(state, player, options, "Treasure Game Lady"),
        "East Clock Town Treasure Game Chest (Deku)":
            lambda state: has_soul_npc(state, player, options, "Treasure Game Lady"),
        "East Clock Town Treasure Game Chest (Goron)":
            lambda state: has_soul_npc(state, player, options, "Treasure Game Lady"),
        "East Clock Town Treasure Game Chest (Zora)":
            lambda state: has_soul_npc(state, player, options, "Treasure Game Lady"),
        "Bomber's Hideout Astral Observatory":
            lambda state: (
                has_soul_npc(state, player, options, "Astral Observatory Man") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Milk Bar Show":
            lambda state: has_soul_npc(state, player, options, "Toto & Gorman"),
        "Milk Bar Priority Mail to Aroma":
            lambda state: (
                has_soul_npc(state, player, options, "Madame Aroma") and
                state.has("Kafei's Mask", player) and
                state.has("Priority Mail", player)
            ),
        "East Clock Town Milk Bar Milk Purchase":
            lambda state: has_soul_npc(state, player, options, "Barten"),
        "East Clock Town Milk Bar Chateau Romani Purchase":
            lambda state: has_soul_npc(state, player, options, "Barten"),
        "West Clock Town Lottery Any Day":
            lambda state: has_soul_npc(state, player, options, "Lottery"),
        "West Clock Town Swordsman Expert Course":
            lambda state: has_soul_npc(state, player, options, "Swordsman"),
        "West Clock Town Postman Counting":
            lambda state: has_soul_npc(state, player, options, "Postman"),
        "West Clock Town Dancing Sisters":
            lambda state: has_soul_npc(state, player, options, "Rosa Sisters"),
        "West Clock Town Bank 200 Rupees":
            lambda state: has_soul_npc(state, player, options, "Banker"),
        "West Clock Town Bank 500 Rupees":
            lambda state: has_soul_npc(state, player, options, "Banker"),
        "West Clock Town Bank 1000 Rupees":
            lambda state: has_soul_npc(state, player, options, "Banker"),
        "West Clock Town Priority Mail to Postman":
            lambda state: (
                has_soul_npc(state, player, options, "Postman") and
                state.has("Priority Mail", player)
            ),
        "Clock Town Trading Post Shop Item 1":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop Item 2":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop Item 3":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop Item 4":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop Item 5":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop Item 6":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop Item 7":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop Item 8":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop (Night) Item 1":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop (Night) Item 2":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop (Night) Item 3":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop (Night) Item 4":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop (Night) Item 5":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop (Night) Item 6":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop (Night) Item 7":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Trading Post Shop (Night) Item 8":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Bomb Shop Item 1":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Bomb Shop Item 2":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Bomb Shop Item 3":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Clock Town Bomb Shop Powder Keg Goron":
            lambda state: has_soul_npc(state, player, options, "Keg Selling Goron"),
        "Clock Town Bomb Shop Item 3 (Stop Thief)":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Curiosity Shop Blue Rupee Trade":
            lambda state: has_soul_npc(state, player, options, "Curiosity Shop Man"),
        "Curiosity Shop Red Rupee Trade":
            lambda state: has_soul_npc(state, player, options, "Curiosity Shop Man"),
        "Curiosity Shop Purple Rupee Trade":
            lambda state: has_soul_npc(state, player, options, "Curiosity Shop Man"),
        "Curiosity Shop Gold Rupee Trade":
            lambda state: has_soul_npc(state, player, options, "Curiosity Shop Man"),
        "Curiosity Shop Night 3 (Stop Thief)":
            lambda state: has_soul_npc(state, player, options, "Curiosity Shop Man"),
        "Curiosity Shop Night 3 Thief Stolen Item":
            lambda state: has_soul_npc(state, player, options, "Curiosity Shop Man"),
        "Stock Pot Inn Reservation":
            lambda state: has_soul_npc(state, player, options, "Anju"),
        "Stock Pot Inn Midnight Meeting":
            lambda state: has_soul_npc(state, player, options, "Anju") and state.has("Kafei's Mask", player),
        "Stock Pot Inn Midnight Toilet Hand":
            lambda state: has_soul_npc(state, player, options, "Toilet Hand"),
        "Stock Pot Inn Granny Story #1":
            lambda state: has_soul_npc(state, player, options, "Anju's Grandmother"),
        "Stock Pot Inn Granny Story #2":
            lambda state: has_soul_npc(state, player, options, "Anju's Grandmother"),
        "Stock Pot Inn Anju and Kafei":
            lambda state: (
                has_soul_utility(state, player, options, "Postboxes") and
                has_soul_npc(state, player, options, "Anju") and
                has_soul_npc(state, player, options, "Kafei") and
                state.has("Kafei's Mask", player) and
                state.has("Letter to Kafei", player) and
                state.has("Pendant of Memories", player)
            ),
        "Laundry Pool Kafei's Request":
            lambda state: (
                has_soul_utility(state, player, options, "Postboxes") and
                has_soul_npc(state, player, options, "Kafei") and
                state.has("Letter to Kafei", player)
            ),
        "Laundry Pool Curiosity Shop Salesman #1":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                state.has("Letter to Kafei", player)
            ),
        "Laundry Pool Curiosity Shop Salesman #2":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                state.has("Letter to Kafei", player)
            ),
        "Laundry Pool Musician":
            lambda state: has_soul_npc(state, player, options, "Guru-Guru"),
        "Termina Ikana Pillar Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Termina Grass Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
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
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Termina Healing Kamaro":
            lambda state: has_soul_npc(state, player, options, "Kamaro"),
        "Termina Gossip Stones HP":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Termina Scrub Grotto HP":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Business Scrubs") and
                has_soul_npc(state, player, options, "Astral Observatory Man")
            ),
        "Termina Log Bombable Grotto Left Cow":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Cows")
            ),
        "Termina Log Bombable Grotto Right Cow":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Cows")
            ),
        "Milk Road Gorman Ranch Race":
            lambda state: has_soul_npc(state, player, options, "Gorman Brothers"),
        "Milk Road Gorman Ranch Purchase":
            lambda state: has_soul_npc(state, player, options, "Gorman Brothers"),
        "Tingle Romani Ranch Map Purchase":
            lambda state: has_soul_npc(state, player, options, "Tingle"),
        "Road to Swamp Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Tingle Woodfall Map Purchase":
            lambda state: has_soul_npc(state, player, options, "Tingle"),
        "Swamp Shooting Gallery 2120 Points":
            lambda state: has_soul_npc(state, player, options, "Archery Man"),
        "Swamp Shooting Gallery 2180 Points":
            lambda state: has_soul_npc(state, player, options, "Archery Man"),
        "Southern Swamp Deku Trade":
            lambda state: has_soul_npc(state, player, options, "Business Scrubs"),
        "Southern Swamp Freestanding HP":
            lambda state: has_soul_absurd(state, player, options, "Deku Flowers"),
        "Southern Swamp Kotake Item":
            lambda state: has_soul_npc(state, player, options, "Koume"),
        "Southern Swamp Healing Koume":
            lambda state: has_soul_npc(state, player, options, "Koume"),
        "Southern Swamp Winning Picture":
            lambda state: (
                has_soul_npc(state, player, options, "Swamp Tourist Guide") and
                has_soul_npc(state, player, options, "Tingle")
            ),
        "Southern Swamp Good Picture":
            lambda state: has_soul_npc(state, player, options, "Swamp Tourist Guide"),
        "Southern Swamp Okay Picture":
            lambda state: has_soul_npc(state, player, options, "Swamp Tourist Guide"),
        "Southern Swamp Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Southern Swamp Song Tablet":
            lambda state: has_soul_absurd(state, player, options, "Deku Flowers"),
        "Southern Swamp Day 2 Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Southern Swamp Witch Shop Mushroom Item":
            lambda state: has_soul_npc(state, player, options, "Kotake"),
        "Southern Swamp Witch Shop Item 1":
            lambda state: has_soul_npc(state, player, options, "Kotake"),
        "Southern Swamp Witch Shop Item 2":
            lambda state: has_soul_npc(state, player, options, "Kotake"),
        "Southern Swamp Witch Shop Item 3":
            lambda state: has_soul_npc(state, player, options, "Kotake"),
        "Swamp Spider House First Room Pot Near Entrance Token":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_misc(state, player, options, "Gold Skulltulas")
            ),
        "Swamp Spider House First Room Crawling In Water Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House First Room Crawling Right Column Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House First Room Crawling Left Column Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House First Room Against Far Wall Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House First Room Lower Left Bugpatch Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House First Room Lower Right Bugpatch Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House First Room Upper Right Bugpatch Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Monument Room Left Crate Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Monument Room Right Crate Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Monument Room Crawling Wall Token":
            lambda state: (
                has_soul_misc(state, player, options, "Gold Skulltulas")
            ),
        "Swamp Spider House Monument Room Crawling On Monument Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Monument Room Behind Torch Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Pottery Room Beehive #1 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Pottery Room Beehive #2 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Pottery Room Small Pot Token":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_misc(state, player, options, "Gold Skulltulas")
            ),
        "Swamp Spider House Pottery Room Left Large Pot Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Pottery Room Right Large Pot Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Pottery Room Behind Vines Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Pottery Room Upper Wall Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Golden Room Crawling Left Wall Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Golden Room Crawling Right Column Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Golden Room Against Far Wall Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Golden Room Beehive Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Tree Room Tall Grass #1 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Tree Room Tall Grass #2 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Tree Room Tree #1 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Tree Room Tree #2 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Tree Room Tree #3 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Tree Room Beehive Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Swamp Spider House Reward":
            lambda state: has_soul_npc(state, player, options, "Spider Man"),
        "Deku Palace Bean Seller":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Bean Daddy")
            ),
        "Deku Palace Bean Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Deku Palace Monkey Song":
            lambda state: has_soul_npc(state, player, options, "Monkey"),
        "Woodfall Great Fairy Reward":
            lambda state: has_soul_npc(state, player, options, "Great Fairies"),
        "Woodfall Temple Turtle Chest":
            lambda state: has_soul_enemy(state, player, options, "Snappers"),
        "Woodfall Temple Dragonfly Chest":
            lambda state: has_soul_enemy(state, player, options, "Dragonflies"),
        "Woodfall Temple Dark Room Chest SF":
            lambda state: has_soul_enemy(state, player, options, "Boes"),
        "Woodfall Temple Dinolfos Chest":
            lambda state: has_soul_enemy(state, player, options, "Dinolfos"),
        "Woodfall Temple Gekko Chest":
            lambda state: has_soul_enemy(state, player, options, "Gekko"),
        "Woodfall Temple Deku Baba SF":
            lambda state: has_soul_enemy(state, player, options, "Deku Baba"),
        "Woodfall Temple Pot SF":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Skulltula SF":
            lambda state: has_soul_enemy(state, player, options, "Hanging Skulltulas"),
        "Woodfall Temple Heart Container":
            lambda state: has_soul_boss(state, player, options, "Odolwa"),
        "Woodfall Temple Odolwa's Remains":
            lambda state: has_soul_boss(state, player, options, "Odolwa"),
        "Southern Swamp Boat Archery":
            lambda state: has_soul_npc(state, player, options, "Koume"),
        "Mountain Village Hungry Goron":
            lambda state: has_soul_npc(state, player, options, "Hungry Goron"),
        "Mountain Village Spring Ramp Grotto":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Mountain Village Smithy Upgrade":
            lambda state: has_soul_npc(state, player, options, "Mountain Smithy"),
        "Mountain Village Smithy Gold Dust Upgrade":
            lambda state: has_soul_npc(state, player, options, "Mountain Smithy"),
        "Tingle Snowhead Map Purchase":
            lambda state: has_soul_npc(state, player, options, "Tingle"),
        "Twin Islands Ramp Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Twin Islands Goron Elder Request":
            lambda state: has_soul_npc(state, player, options, "Goron Elder"),
        "Twin Islands Hot Water Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Goron Village Scrub Purchase":
            lambda state: has_soul_npc(state, player, options, "Business Scrubs"),
        "Goron Village Deku Trade":
            lambda state: has_soul_npc(state, player, options, "Business Scrubs"),
        "Goron Village Freestanding HP":
            lambda state: has_soul_absurd(state, player, options, "Deku Flowers"),
        "Powder Keg Goron Reward":
            lambda state: has_soul_npc(state, player, options, "Gatekeeper & Medigoron"),
        "Goron Village Shop Item 1":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Goron Village Shop Item 2":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Goron Village Shop Item 3":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Goron Village Shop (Spring) Item 1":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Goron Village Shop (Spring) Item 2":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Goron Village Shop (Spring) Item 3":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Path to Snowhead Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Snowhead Great Fairy Reward":
            lambda state: has_soul_npc(state, player, options, "Great Fairies"),
        "Snowhead Temple Dinolfos Room First SF":
            lambda state: has_soul_enemy(state, player, options, "Dinolfos"),
        "Snowhead Temple Dinolfos Room Second SF":
            lambda state: has_soul_enemy(state, player, options, "Dinolfos"),
        "Snowhead Temple Bridge Room Freezard Chest":
            lambda state: has_soul_enemy(state, player, options, "Freezard"),
        "Snowhead Temple Freezard Torch Room Chest SF":
            lambda state: has_soul_enemy(state, player, options, "Freezard"),
        "Snowhead Temple Lower Wizzrobe Chest":
            lambda state: has_soul_enemy(state, player, options, "Wizrobe"),
        "Snowhead Temple Upper Wizzrobe Chest":
            lambda state: has_soul_enemy(state, player, options, "Wizrobe"),
        "Snowhead Temple Heart Container":
            lambda state: has_soul_boss(state, player, options, "Goht"),
        "Snowhead Temple Goht's Remains":
            lambda state: has_soul_boss(state, player, options, "Goht"),
        "Romani Ranch Baby Cuccos March":
            lambda state: has_soul_npc(state, player, options, "Grog"),
        "Romani Ranch Helping Cremia":
            lambda state: has_soul_npc(state, player, options, "Romani & Cremia"),
        "Romani Ranch Doggy Race":
            lambda state: has_soul_npc(state, player, options, "Doggy Race Lady & Dogs"),
        "Romani Ranch Romani Game":
            lambda state: has_soul_npc(state, player, options, "Romani & Cremia"),
        "Romani Ranch Aliens":
            lambda state: has_soul_npc(state, player, options, "Romani & Cremia"),
        "Romani Ranch Barn Free Cow":
            lambda state: has_soul_misc(state, player, options, "Cows"),
        "Romani Ranch Barn Stables Front Cow":
            lambda state: has_soul_misc(state, player, options, "Cows"),
        "Romani Ranch Barn Stables Back Cow":
            lambda state: has_soul_misc(state, player, options, "Cows"),
        "Tingle Great Bay Map Purchase":
            lambda state: has_soul_npc(state, player, options, "Tingle"),
        "Great Bay Fisherman's Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Great Bay Ledge Grotto Left Cow":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Cows")
            ),
        "Great Bay Ledge Grotto Right Cow":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Cows")
            ),
        "Pinnacle Rock Seahorse HP":
            lambda state: (
                has_soul_npc(state, player, options, "Fisherman") and
                has_soul_enemy(state, player, options, "Pirate Guards")
            ),
        "Pinnacle Rock Upper Eel Chest":
            lambda state: (
                has_soul_npc(state, player, options, "Fisherman") and
                has_soul_enemy(state, player, options, "Pirate Guards")
            ),
        "Pinnacle Rock Lower Eel Chest":
            lambda state: (
                has_soul_npc(state, player, options, "Fisherman") and
                has_soul_enemy(state, player, options, "Pirate Guards")
            ),
        "Great Bay Baby Zora Song":
            lambda state: has_soul_npc(state, player, options, "Marine Lab Researcher"),
        "Great Bay Feeding Lab Fish":
            lambda state: has_soul_npc(state, player, options, "Marine Lab Fish"),
        "Great Bay Fisherman Game":
            lambda state: has_soul_npc(state, player, options, "Fisherman"),
        "Ocean Spider House Ramp Upper Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Ramp Lower Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Lobby Ceiling Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House First Room Rafter Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House First Room Open Pot #1 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House First Room Open Pot #2 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House First Room Wall Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Library Top Bookcase Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Library Passage Behind Bookcase Front Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Library Passage Behind Bookcase Rear Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Library Painting #1 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Library Painting #2 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Library Rafter Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Library Bookshelf Hole Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House First Room Downstairs Rafter Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House First Room Downstairs Open Pot Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House First Room Downstairs Behind Staircase Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House First Room Downstairs Crate Token":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_misc(state, player, options, "Gold Skulltulas")
            ),
        "Ocean Spider House First Room Downstairs Wall Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Dining Room Open Pot Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Dining Room Painting Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Dining Room Ceiling Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Dining Room Chandelier #1 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Dining Room Chandelier #2 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Dining Room Chandelier #3 Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Storage Room Web Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Storage Room North Wall Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Storage Room Crate Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Storage Room Hidden Hole Token":
            lambda state: has_soul_misc(state, player, options, "Gold Skulltulas"),
        "Ocean Spider House Storage Room Ceiling Pot Token":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_misc(state, player, options, "Gold Skulltulas")
            ),
        "Ocean Spider House Coloured Mask Sequence HP":
            lambda state: has_soul_npc(state, player, options, "Stalchildren"),
        "Ocean Spider House Reward":
            lambda state: has_soul_npc(state, player, options, "Spider Man"),
        "Pirates' Fortress Interior Tank Chest":
            lambda state: has_soul_enemy(state, player, options, "Coloured Pirates"),
        "Zora Cape Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Zora Cape Pot Game":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora")
            ),
        "Zora Hall Evan's Song":
            lambda state: has_soul_npc(state, player, options, "Evan"),
        "Zora Hall Torches Reward":
            lambda state: has_soul_npc(state, player, options, "Pot Game & Hall Zora"),
        "Zora Hall Good Picture of Lulu":
            lambda state: (
                has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
                has_soul_npc(state, player, options, "Lulu")
            ),
        "Zora Hall Bad Picture of Lulu":
            lambda state: (
                has_soul_npc(state, player, options, "Pot Game & Hall Zora") and
                has_soul_npc(state, player, options, "Lulu")
            ),
        "Zora Hall Deku Scrub Purchase":
            lambda state: has_soul_npc(state, player, options, "Business Scrubs"),
        "Zora Hall Goron Scrub Trade":
            lambda state: has_soul_npc(state, player, options, "Business Scrubs"),
        "Zora Hall Goron Scrub Trade Freestanding HP":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Business Scrubs")
            ),
        "Zora Hall Shop Item 1":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Zora Hall Shop Item 2":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Zora Hall Shop Item 3":
            lambda state: has_soul_npc(state, player, options, "Shop Owners"),
        "Great Bay Great Fairy Reward":
            lambda state: (
                has_soul_npc(state, player, options, "Great Fairies") and
                has_soul_absurd(state, player, options, "Trees & Bushes")
            ),
        "Great Bay Temple Waterwheel Room Skulltula SF":
            lambda state: has_soul_enemy(state, player, options, "Hanging Skulltulas"),
        "Great Bay Temple Blender Room Barrel SF":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Blender Pot SF":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Before Red Valve Room Pot SF":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Bio-Baba Hall Chest SF":
            lambda state: has_soul_enemy(state, player, options, "Deku Baba"),
        "Great Bay Temple Before Gekko Room Pot SF":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Mad Jellied Gekko Chest":
            lambda state: has_soul_enemy(state, player, options, "Jellied Gekko"),
        "Great Bay Temple Behind Locked Door Chest":
            lambda state: has_soul_enemy(state, player, options, "Wart"),
        "Great Bay Temple Freezable Waterwheel Upper Chest SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly")
            ),
        "Great Bay Temple Freezable Waterwheel Lower Chest SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly")
            ),
        "Great Bay Temple Seesaw Room Underwater Barrel SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly")
            ),
        "Great Bay Temple Seesaw Room Chest SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly")
            ),
        "Great Bay Temple Pre-Boss Room Platform Bubble SF":
            lambda state: (
                has_soul_enemy(state, player, options, "Ocotoroks") and
                has_soul_enemy(state, player, options, "Chu Jelly")
            ),
        "Great Bay Temple Heart Container":
            lambda state: has_soul_boss(state, player, options, "Gyorg"),
        "Great Bay Temple Gyorg's Remains":
            lambda state: has_soul_boss(state, player, options, "Gyorg"),
        "Road to Ikana Rock Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Road to Ikana Invisible Soldier":
            lambda state: has_soul_npc(state, player, options, "Shiro"),
        "Ikana Graveyard Bombable Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Graveyard Day 1 Bats Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Bad Bats") and
                has_soul_npc(state, player, options, "Stalchildren")
            ),
        "Graveyard Day 2 Dampe Bats":
            lambda state: (
                has_soul_npc(state, player, options, "Dampe") and
                has_soul_enemy(state, player, options, "Bad Bats")
            ),
        "Graveyard Day 2 Iron Knuckle Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Iron Knuckle") and
                has_soul_npc(state, player, options, "Stalchildren")
            ),
        "Graveyard Day 3 Dampe Big Poe Chest":
            lambda state: (
                has_soul_npc(state, player, options, "Stalchildren") and
                has_soul_npc(state, player, options, "Dampe")
            ),
        "Graveyard Captain Keeta Chest":
            lambda state: has_soul_npc(state, player, options, "Stalchildren"),
        "Graveyard Day 1 Iron Knuckle Song":
            lambda state: (
                has_soul_enemy(state, player, options, "Iron Knuckle") and
                has_soul_npc(state, player, options, "Stalchildren")
            ),
        "Tingle Stone Tower Map Purchase":
            lambda state: has_soul_npc(state, player, options, "Tingle"),
        "Ikana Canyon Spirit House":
            lambda state: has_soul_npc(state, player, options, "Spirit House Owner"),
        "Ikana Canyon Healing Pamela's Father":
            lambda state: (
                has_soul_npc(state, player, options, "Pamela's Father") and
                has_soul_npc(state, player, options, "Composer Poe")
            ),
        "Ikana Canyon Scrub Purchase":
            lambda state: has_soul_npc(state, player, options, "Business Scrubs"),
        "Ikana Canyon Zora Scrub Trade":
            lambda state: has_soul_npc(state, player, options, "Business Scrubs"),
        "Ikana Canyon Zora Trade Freestanding HP":
            lambda state: (
                has_soul_npc(state, player, options, "Business Scrubs") and
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Ikana Canyon Grotto Chest":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Stone Tower Great Fairy Reward":
            lambda state: has_soul_npc(state, player, options, "Great Fairies"),
        "Secret Shrine Dinolfos Chest":
            lambda state: has_soul_enemy(state, player, options, "Dinolfos"),
        "Secret Shrine Wizzrobe Chest":
            lambda state: has_soul_enemy(state, player, options, "Wizrobe"),
        "Secret Shrine Wart Chest":
            lambda state: has_soul_enemy(state, player, options, "Wart"),
        "Secret Shrine Garo Master Chest":
            lambda state: has_soul_enemy(state, player, options, "Garo Master"),
        "Secret Shrine Completion Chest":
            lambda state: has_soul_npc(state, player, options, "Spirit House Owner"),
        "Ikana Well Rightside Torch Chest":
            lambda state: has_soul_npc(state, player, options, "Gibdos"),
        "Ikana Well Invisible Chest":
            lambda state: has_soul_npc(state, player, options, "Gibdos"),
        "Ikana Well Cow":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_misc(state, player, options, "Cows")
            ),
        "Ikana Castle Pillar Freestanding HP":
            lambda state: has_soul_absurd(state, player, options, "Deku Flowers"),
        "Stone Tower Temple Entrance Room Lower Chest":
            lambda state: has_soul_absurd(state, player, options, "Deku Flowers"),
        "Stone Tower Temple Armos Room Lava Chest":
            lambda state: has_soul_enemy(state, player, options, "Armos"),
        "Stone Tower Temple Air Gust Room Side Chest":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") or
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Stone Tower Temple Inverted Eastern Air Gust Room Fire Chest":
            lambda state: has_soul_absurd(state, player, options, "Deku Flowers"),
        "Stone Tower Temple Inverted Eastern Air Gust Room Frozen Switch Chest":
            lambda state: has_soul_absurd(state, player, options, "Deku Flowers"),
        "Stone Tower Temple Inverted Eastern Air Gust Room Switch Chest":
            lambda state: has_soul_absurd(state, player, options, "Deku Flowers"),
        "Stone Tower Temple Inverted Wizzrobe Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Wizrobe") and
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Stone Tower Temple Inverted Gomess Chest":
            lambda state: has_soul_enemy(state, player, options, "Gomess"),
        "Stone Tower Temple Inverted Eyegore Chest":
            lambda state: has_soul_enemy(state, player, options, "Eyegore"),
        "Stone Tower Temple Inverted Heart Container":
            lambda state: has_soul_boss(state, player, options, "Twinmold"),
        "Stone Tower Temple Inverted Twinmold's Remains":
            lambda state: has_soul_boss(state, player, options, "Twinmold"),
        "Moon Deku Trial HP":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_absurd(state, player, options, "Deku Flowers")
            ),
        "Moon Goron Trial HP":
            lambda state: has_soul_npc(state, player, options, "Moon Kids"),
        "Moon Zora Trial HP":
            lambda state: has_soul_npc(state, player, options, "Moon Kids"),
        "Moon Link Trial Garo Master Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Garo Master") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Link Trial Iron Knuckle Chest":
            lambda state: (
                has_soul_enemy(state, player, options, "Iron Knuckle") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Link Trial HP":
            lambda state: has_soul_npc(state, player, options, "Moon Kids"),
        "Moon Trade All Masks":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                has_enough_masks(state, player, 20, exclude_transformation=True)
            ),
        "Defeat Majora":
            lambda state: (
                has_enough_remains(state, player, options.majora_remains_required.value) and
                has_enough_masks(state, player, options.majora_masks_required.value) and
                has_enough_items(state, player, options.majora_items_required.value) and
                (
                    not options.majora_star_fox.value or
                    has_star_fox(state, player, options, "majora")
                ) and
                has_all_frogs(state, player, options, "majora") and
                has_all_scarecrows(state, player, options, "majora") and
                has_all_owls(state, player, options, "majora") and
                has_soul_npc(state, player, options, "Moon Kids") and
                has_soul_boss(state, player, options, "Majora")
            ),
        "Before Clock Town Keaton Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Keaton Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Keaton Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Keaton Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Keaton Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Keaton Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Keaton Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Keaton Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Keaton Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Skullkid Keaton Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Skullkid Keaton Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Skullkid Keaton Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Skullkid Keaton Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Skullkid Keaton Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Skullkid Keaton Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Skullkid Keaton Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Skullkid Keaton Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Before Clock Town Skullkid Keaton Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Lost Woods Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Grass Patch 2 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Keaton Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Keaton Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Keaton Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Keaton Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Keaton Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Keaton Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Keaton Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Keaton Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
            ),
        "Lost Woods Keaton Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass") and
                has_soul_npc(state, player, options, "Skull Kid")
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
        "Termina Field Cow Grotto Grass Group 1 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 1 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 2 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 3 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 4 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 5 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Cow Grotto Grass Group 6 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
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
        "Termina Field Bio Baba Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Bio Baba Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
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
        "Termina Field Bombable Rock Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Bombable Rock Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Bombable Rock Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Bombable Rock Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Termina Field Bombable Rock Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road to Southern Swamp Outside Archery Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Road to Southern Swamp Outside Archery Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
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
        "Southern Swamp Owl Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Southern Swamp Owl Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
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
        "Woods of Mystery Day 2 Unique Grass":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Day 3 Unique Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woods of Mystery Day 3 Unique Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
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
        "Southern Swamp Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Owl Post Dungeon Grass (1)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Owl Post Dungeon Grass (2)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (1)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (2)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (3)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (4)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (5)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (6)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (7)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (8)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (9)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (10)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (11)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Tourist Centre (12)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (1)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (2)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (3)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (4)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (5)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (6)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (7)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (8)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (9)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (10)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (11)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (12)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (13)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (14)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (15)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (16)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (17)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Grass Near Witch Shop (18)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Gossip Grass (1)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Southern Swamp Post Dungeon Gossip Grass (2)":
            lambda state: (
                can_clear_woodfall(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Deku Palace Bean Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
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
        "Milk Road Owl Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Owl Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Owl Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
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
        "Milk Road Gorman Racetrack Grass Group 1 (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 1 (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Milk Road Gorman Racetrack Grass Group 2 (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
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
        "Twin Isles Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Isles Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (1)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (2)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (3)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (4)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (5)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (6)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (7)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (8)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (9)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (10)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (11)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Twin Islands Springtime Grass Group 1 (12)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
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
        "Path To Snowhead Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Path To Snowhead Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (1)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (2)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (3)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (4)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (5)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (6)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (7)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (8)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (9)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (10)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (11)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (12)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (13)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (14)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (15)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (16)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (17)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (18)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (19)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (20)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (21)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (22)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (23)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (24)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (25)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (26)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (27)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (28)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (29)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Springtime Grass (30)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Keaton Grass (0)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Keaton Grass (1)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Keaton Grass (2)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Keaton Grass (3)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Keaton Grass (4)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Keaton Grass (5)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Keaton Grass (6)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Keaton Grass (7)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Keaton Grass (8)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Mountain Village Spring Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                can_clear_snowhead(state, player, boss_placements) and 
                has_soul_absurd(state, player, options, "Grass")
            ),
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
        "Great Bay Coast Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Great Bay Coast Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Great Bay Coast Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Great Bay Coast Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Great Bay Coast Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Great Bay Coast Cow Grotto Grass Group 1 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 1 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 2 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 3 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 4 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 5 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Great Bay Coast Cow Grotto Grass Group 6 (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Zora Cape Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Road To Ikana Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
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
        "Ikana Graveyard Bombable Grotto Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Graveyard Bombable Grotto Grass (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Canyon Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Canyon Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Canyon Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Canyon Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
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
        "Secret Shrine Dinolfos Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Dinolfos Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Dinolfos Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Dinolfos Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wizzrobe Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wizzrobe Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wizzrobe Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wizzrobe Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wizzrobe Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wart Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wart Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wart Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wart Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wart Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wart Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wart Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Wart Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Garo Master Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Garo Master Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Garo Master Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Garo Master Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Garo Master Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Secret Shrine Garo Master Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Beneath the Well Left Side Back Room Grass (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Left Side Back Room Grass (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Before Big Poe and Cow Grass (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Before Big Poe and Cow Grass (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Before Big Poe and Cow Grass (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Before Big Poe and Cow Grass (4)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Cow Grass (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Cow Grass (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Cow Grass (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Back Room Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Back Room Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Back Room Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Back Room Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Beneath the Well Right Side Back Room Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Ikana Castle Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Ikana Castle Grass (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Entrance Room Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Entrance Room Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Entrance Room Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Entrance Room Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Entrance Room Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Main Room Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Main Room Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Main Room Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Deku Elevator Room Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Deku Elevator Room Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Snapping Turtle Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Snapping Turtle Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Snapping Turtle Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Snapping Turtle Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Snapping Turtle Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Dragonfly Chest Room Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Dragonfly Chest Room Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Dragonfly Chest Room Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple 2F Moving Flower Platform Room Grass (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass") and state.has("Deku Mask", player),
        "Woodfall Temple Pre Boss Room Grass (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Pre Boss Room Grass (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Pre Boss Room Grass (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Pre Boss Room Grass (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Pre Boss Room Grass (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grass")
            ),
        "Woodfall Temple Odolwas Lair Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (7)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (8)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (9)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (10)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (11)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (12)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (13)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (14)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (15)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Woodfall Temple Odolwas Lair Grass (16)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
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
        "Stone Tower Temple Entrance Room Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Stone Tower Temple Entrance Room Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Stone Tower Temple Entrance Room Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Stone Tower Temple Elegy Maze Grass (1)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Stone Tower Temple Elegy Maze Grass (2)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Stone Tower Temple Elegy Maze Grass (3)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Stone Tower Temple Elegy Maze Grass (4)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Stone Tower Temple Elegy Maze Grass (5)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Stone Tower Temple Elegy Maze Grass (6)":
            lambda state: has_soul_absurd(state, player, options, "Grass"),
        "Trading Post Pot":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Sword School Night 3 Midnight Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Sword School Night 3 Midnight Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Sword School Night 3 Midnight Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Sword School Night 3 Midnight Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Sword School Night 3 Midnight Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Top of Clock Tower Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Top of Clock Tower Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Top of Clock Tower Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Top of Clock Tower Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Bombers Hideout Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Bombers Hideout Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Bombers Hideout Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Bombers Hideout Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Astral Observatory Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Astral Observatory Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Astral Observatory Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Termina Field Eastern Pillar Pot":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Termina Field Deku Business Scrub Grotto Pot":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Road to Southern Swamp Outside Archery Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Road to Southern Swamp Outside Archery Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Southern Swamp Beneath Witch Shop Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Southern Swamp Beneath Witch Shop Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Southern Swamp Beneath Witch Shop Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
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
        "Swamp Spider House Tablet Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Swamp Spider House Tablet Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
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
        "Deku Palace Right Side Upper Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Deku Palace Right Side Upper Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Deku Butler Race Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Deku Butler Race Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Owl Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Woodfall Owl Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Woodfall Owl Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Woodfall Temple Entrance Pot":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
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
        "Woodfall Temple Deku Elevator Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Deku Elevator Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Deku Elevator Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Deku Elevator Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Gekko Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Woodfall Temple Gekko Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Woodfall Temple Gekko Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Woodfall Temple Gekko Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Woodfall Temple Left Side Bridge Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Left Side Bridge Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Pre Boss Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Woodfall Temple Pre Boss Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Southern Swamp Post Dungeon Witch Pot (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Southern Swamp Post Dungeon Witch Pot (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Southern Swamp Post Dungeon Witch Pot (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Mountain Village Rooftop Pot":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Mountain Village Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Mountain Village Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Mountain Smithy Pots Inside at Night (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Mountain Smithy Pots Inside at Night (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (9)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (10)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (11)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (12)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (13)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (14)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (15)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (16)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (17)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (18)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (19)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (20)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (21)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (22)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (23)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (24)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (25)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (26)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (27)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (28)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (29)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Racetrack Pots (30)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (9)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (10)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goron Shrine Pots (11)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Entrance Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Entrance Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
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
        "Snowhead Temple Main Room Pots Basement (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Main Room Pots Basement (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Main Room Scarecrow Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Main Room Scarecrow Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
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
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (9)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (10)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (11)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (12)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Frozen Green Door Pots (13)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Orange Door Push Block Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Orange Door Push Block Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Orange Door Push Block Ghost Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Orange Door Push Block Ghost Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Locked Grey Door Wolfos Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Locked Grey Door Wolfos Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Locked Grey Door Wolfos Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Locked Grey Door Wolfos Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Locked Grey Door Wolfos Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Goron Pound Puzzle Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Goron Pound Puzzle Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Main Room 2nd Floor Bridge Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Main Room 2nd Floor Bridge Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Main Room 4th Floor Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple Main Room 4th Floor Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple 4th Floor Wizzrobe Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple 4th Floor Wizzrobe Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple 4th Floor Wizzrobe Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple 4th Floor Wizzrobe Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Snowhead Temple 4th Floor Wizzrobe Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (9)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (10)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (11)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (12)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (13)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Goht Boss Room Pots (14)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Mountain Village Springtime Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Mountain Village Springtime Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Mountain Village Springtime Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Romani Ranch Baby Cuccoos Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Romani Ranch Baby Cuccoos Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Romani Ranch Doggy Racetrack Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Romani Ranch Doggy Racetrack Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Romani Ranch Doggy Racetrack Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Romani Ranch Doggy Racetrack Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
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
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Coast Upper Rock Wall Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Coast Upper Rock Wall Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Coast Beside Pirates Fortress Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Coast Beside Pirates Fortress Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Bottom of Ramp Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Bottom of Ramp Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Bottom of Ramp Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Bottom of Ramp Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Main Room Lower Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Main Room Lower Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Main Room Lower Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Main Room Lower Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Main Room Lower Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Crate Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Crate Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Crate Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Crate Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Crate Room Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Crate Room Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Crate Room Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Crate Room Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Coloured Skulls Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ocean Spiderhouse Coloured Skulls Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pinnacle Rock Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pinnacle Rock Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pinnacle Rock Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pinnacle Rock Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pinnacle Rock Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pinnacle Rock Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pinnacle Rock Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pinnacle Rock Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pinnacle Rock Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pinnacle Rock Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pinnacle Rock Pots (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pirates Fortress Sewers Cage Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pirates Fortress Sewers Cage Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pirates Fortress Sewers After Gate Hidden Ladder Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pirates Fortress Sewers After Gate Hidden Ladder Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pirates Fortress Sewers Exit Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pirates Fortress Sewers Exit Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pirates Fortress Sewers Exit Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pirates Fortress Interior Underwater Chest Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pirates Fortress Interior Underwater Chest Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pirates Fortress Interior Underwater Chest Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pirates Fortress Interior Room Past Green Guard Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pirates Fortress Interior Room Past Green Guard Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pirates Fortress Interior Room Past Green Guard Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pirates Fortress Interior Upper Beehive Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pirates Fortress Interior Upper Beehive Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Pirates Fortress Interior Room Past Pink Guard Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Pirates Fortress Interior Room Past Pink Guard Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Zora Cape Like Like Pool Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Zora Cape Like Like Pool Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Zora Cape Owl Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Zora Cape Owl Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Zora Cape Owl Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Zora Cape Owl Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Zora Cape Jar Game Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora")
            ),
        "Zora Cape Jar Game Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora")
            ),
        "Zora Cape Jar Game Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora")
            ),
        "Zora Cape Jar Game Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora")
            ),
        "Zora Cape Jar Game Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Pot Game & Hall Zora")
            ),
        "Great Bay Temple Above Blender Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Above Blender Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Room Behind 1F Waterfall Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Room Behind 1F Waterfall Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Room Behind 1F Waterfall Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Room Behind 1F Waterfall Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (9)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (10)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (11)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Green Pipe Tunnel Room Pots (12)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
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
        "Great Bay Temple Red Valve Underwater Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Valve Underwater Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Valve Underwater Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Great Bay Temple Red Valve Underwater Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
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
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Green Pipe Frozen Waterwheel Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Seesaw Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Seesaw Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Seesaw Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Pots Underneath Boss Door Platform (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Gyorg Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Gyorg Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Gyorg Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Gyorg Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Gyorg Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Gyorg Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Gyorg Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Great Bay Temple Gyorg Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Road To Ikana Scarecrow Pillar Pot":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Graveyard Day 1 Grave Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 1 Grave Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 1 Grave Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 1 Grave Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 1 Grave Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 2 Entrance Grave Pot":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 2 Invisible Path Entryway Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 2 Invisible Path Entryway Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 2 Invisible Path Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 2 Invisible Path Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 2 Invisible Path Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 2 Invisible Path Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 3 Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 3 Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 3 Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 3 Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 3 Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 3 Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 3 Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 3 Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 3 Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Graveyard Day 3 Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Secret Shrine Entrance Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Secret Shrine Entrance Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Secret Shrine Entrance Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Secret Shrine Underwater Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Secret Shrine Underwater Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Secret Shrine Underwater Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Secret Shrine Underwater Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Secret Shrine Underwater Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Secret Shrine Underwater Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Sakons Hideout Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Sakons Hideout Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Sakons Hideout Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Sakons Hideout Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Sakons Hideout Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Castle Exterior Corner Pot":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Frozen Eyes Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Frozen Eyes Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Left Side Falling Ceiling Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Left Side Falling Ceiling Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Left Side Broken Floor Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Castle Left Side Broken Floor Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Castle Left Side Broken Floor Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Castle Left Side Broken Floor Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Castle Left Side Staircase Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Castle Left Side Staircase Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Castle Right Side Staircase Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Castle Right Side Staircase Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Ikana Castle Throne Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Throne Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Throne Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Throne Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Throne Room Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Throne Room Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Throne Room Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Ikana Castle Throne Room Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Well Left Side Back Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Well Left Side Back Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Well Left Side Back Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Well Left Side Back Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Well Left Side Back Room Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Well Right Side Before Chest Room Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Well Right Side Before Chest Room Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Well Right Side Before Chest Room Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Well Right Side Before Chest Room Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Well Right Side Before Chest Room Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Well Right Side Before Chest Room Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Well Right Side Before Chest Room Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Well Right Side Before Chest Room Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Well Right Side Before Chest Room Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Well Right Side Before Chest Room Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Well Big Poe Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Well Big Poe Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Well Big Poe Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Well Big Poe Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Stone Tower Climb Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Climb Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (9)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (10)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (11)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Lower Scarecrow Pots (12)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Upper Scarecrow Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Upper Scarecrow Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Upper Scarecrow Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Upper Scarecrow Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Upper Scarecrow Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Upper Scarecrow Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Upper Scarecrow Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Upper Scarecrow Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Upper Scarecrow Pots (9)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Entrance Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Entrance Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Basement Armos Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Basement Armos Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Basement Armos Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Basement Armos Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Basement Armos Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Basement Armos Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Basement Armos Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Basement Armos Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Right Side Near Locked Door Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Right Side Near Locked Door Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Right Side Underwater Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Right Side Underwater Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Right Side Underwater Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Right Side Underwater Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Right Side Underwater Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Deku Updraft Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Stone Tower Temple Deku Updraft Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Stone Tower Temple Deku Updraft Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Stone Tower Temple Deku Updraft Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Stone Tower Temple Lower Spike Roller Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Spike Roller Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Spike Roller Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Spike Roller Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Spike Roller Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Spike Roller Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Spike Roller Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Stone Tower Temple Lower Spike Roller Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Bean Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Bean Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Bean Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Bean Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Bean Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Updraft Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Inverted Stone Tower Temple Updraft Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Inverted Stone Tower Temple Updraft Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Inverted Stone Tower Temple Updraft Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Inverted Stone Tower Temple Updraft Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Inverted Stone Tower Temple Updraft Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Inverted Stone Tower Temple Gomess Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Inverted Stone Tower Temple Gomess Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Inverted Stone Tower Temple Gomess Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Inverted Stone Tower Temple Gomess Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots")
            ),
        "Inverted Stone Tower Temple Lower Bridge Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Lower Bridge Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Small Poe Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Small Poe Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Small Poe Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Small Poe Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Wizzrobe Room Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Wizzrobe Room Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Wizzrobe Room Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Wizzrobe Room Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Wizzrobe Room Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (Flying) (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (Flying) (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (Flying) (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (Flying) (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (1)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (2)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (3)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (4)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (5)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (6)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (7)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Inverted Stone Tower Temple Pre Boss Pots (8)":
            lambda state: has_soul_absurd(state, player, options, "Pots"),
        "Moon Goron Trial Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (12)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (13)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (14)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Goron Trial Pots (15)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Link Trial Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Link Trial Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Link Trial Pots (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Link Trial Pots (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Link Trial Pots (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Link Trial Pots (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Link Trial Pots (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Moon Link Trial Pots (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids")
            ),
        "Majora Lair Pots (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    options.majora_remains_required.value == 0 or
                    has_enough_remains(state, player, options.majora_remains_required.value)
                ) and
                (
                    options.majora_masks_required.value == 0 or
                    has_enough_masks(state, player, options.majora_masks_required.value)
                ) and
                (
                    not options.majora_star_fox.value or
                    has_star_fox(state, player, options, "majora")
                ) 
            ),
        "Majora Lair Pots (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Pots") and
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    options.majora_remains_required.value == 0 or
                    has_enough_remains(state, player, options.majora_remains_required.value)
                ) and
                (
                    options.majora_masks_required.value == 0 or
                    has_enough_masks(state, player, options.majora_masks_required.value)
                ) and
                (
                    not options.majora_star_fox.value or
                    has_star_fox(state, player, options, "majora")
                ) 
            ),
        "Termina Field 6am Songwall (0)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 6am Songwall (1)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 6am Songwall (2)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 7am Songwall (0)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 7am Songwall (1)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 7am Songwall (2)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 8am Songwall (0)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 8am Songwall (1)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 8am Songwall (2)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 10am Songwall (0)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 10am Songwall (1)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 10am Songwall (2)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 12pm Songwall (0)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 12pm Songwall (1)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 12pm Songwall (2)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 8pm Songwall (0)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 8pm Songwall (1)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field 8pm Songwall (2)":
            lambda state: has_soul_absurd(state, player, options, "Songwall"),
        "Termina Field Tree Rupees (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Termina Field Tree Rupees (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Observatory Secret Guay Rupee (1)":
            lambda state: has_soul_npc(state, player, options, "Astral Observatory Man"),
        "Observatory Secret Guay Rupee (2)":
            lambda state: has_soul_npc(state, player, options, "Astral Observatory Man"),
        "Termina Field Thieving Bird Rupee":
            lambda state: has_soul_enemy(state, player, options, "Takkuri"),
        "Deku Playground Day 1 Rupees (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 1 Rupees (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 1 Rupees (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 1 Rupees (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 1 Rupees (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 1 Rupees (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 2 Rupees (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 2 Rupees (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 2 Rupees (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 2 Rupees (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 2 Rupees (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 2 Rupees (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 3 Rupees (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 3 Rupees (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 3 Rupees (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 3 Rupees (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 3 Rupees (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Deku Playground Day 3 Rupees (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_npc(state, player, options, "Deku Playground Employee")
            ),
        "Woodfall Stump Rupee":
            lambda state: True,
        "Ikana Graveyard Day 2 Rupees (0)":
            lambda state: True,
        "Ikana Graveyard Day 2 Rupees (1)":
            lambda state: True,
        "Ikana Graveyard Day 2 Rupees (2)":
            lambda state: True,
        "Ikana Graveyard Day 2 Rupees (3)":
            lambda state: True,
        "Ikana Graveyard Day 2 Rupees (4)":
            lambda state: True,
        "Ikana Graveyard Day 2 Rupees (5)":
            lambda state: True,
        "Ikana Graveyard Day 2 Rupees (6)":
            lambda state: True,
        "Stone Tower Deku Updraft Rupees (1)":
            lambda state: True,
        "Stone Tower Deku Updraft Rupees (2)":
            lambda state: True,
        "Stone Tower Deku Updraft Rupees (3)":
            lambda state: True,
        "Stone Tower Deku Updraft Rupees (4)":
            lambda state: True,
        "Stone Tower Deku Updraft Rupees (5)":
            lambda state: True,
        "Stone Tower Deku Updraft Rupees (6)":
            lambda state: True,
        "Deku Palace Bean Seller Soil (0)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Deku Palace Bean Seller Soil (1)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Deku Palace Bean Seller Soil (2)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
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
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Termina Field North West Rock Wall (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field North West Rock Wall (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field North West Rock Wall (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field North West Rock Wall (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field North West Rock Wall (4)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field Rock Behind Coast Wall":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field South West Rock Wall (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field South West Rock Wall (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field South West Rock Wall (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field South West Rock Wall (4)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Swamp Spider Entry Rocks (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Swamp Spider Entry Rocks (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Swamp Spider Large Pots Rock":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Twin Isles Hot Spring Water Grotto Bomb Boulders (0)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Twin Isles Hot Spring Water Grotto Bomb Boulders (1)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Twin Isles Hot Spring Water Grotto Bomb Boulders (2)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Twin Isles Hot Spring Water Grotto Bomb Boulders (3)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Twin Isles Hot Spring Water Grotto Bomb Boulders (4)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Goron Shrine Rocks (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (4)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (5)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (6)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (7)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (8)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (9)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (10)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (11)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (12)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (13)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (14)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Goron Shrine Rocks (15)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Mountain Village Spring Rock Triangle (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Mountain Village Spring Rock Triangle (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Mountain Village Spring Rock Triangle (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Mountain Village Spring Rock Triangle (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Mountain Village Spring Rock Triangle (4)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Mountain Village Spring Outside Goron Graveyard Rocks (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Mountain Village Spring Outside Goron Graveyard Rocks (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Twin Isles Spring Above Grotto Rocks (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Twin Isles Spring Above Grotto Rocks (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Twin Isles Spring Above Grotto Rocks (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Scattered Beach Rocks (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Scattered Beach Rocks (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Scattered Beach Rocks (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Scattered Beach Rocks (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Scattered Beach Rocks (4)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Rock Wall Rocks (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Rock Wall Rocks (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Rock Wall Rocks (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Underwater Rocks (Bombchus only) Below Rock Pools (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Underwater Rocks (Bombchus only) Next to Like-Like":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Underwater Rocks (Bombchus only) Below Rock Pools (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Great Bay Coast Underwater Rocks (Bombchus only) Near Rock Wall":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and 
                (
                    has_bombchus(state, player) or 
                    state.has("Zora Mask", player)
                )
            ),
        "Great Bay Coast Beach Rocks Underwater (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and 
                (
                    has_bombchus(state, player) or 
                    state.has("Zora Mask", player)
                )
            ),
        "Great Bay Coast Beach Rocks Underwater (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and 
                (
                    has_bombchus(state, player) or 
                    state.has("Zora Mask", player)
                )
            ),
        "Great Bay Coast Beach Rocks Underwater (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and 
                (
                    has_bombchus(state, player) or 
                    state.has("Zora Mask", player)
                )
            ),
        "Great Bay Coast Beach Rocks Underwater (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks") and 
                (
                    has_bombchus(state, player) or 
                    state.has("Zora Mask", player)
                )
            ),
        "Zora Cape Beach Rocks (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Zora Cape Beach Rocks (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Zora Cape Beach Rocks (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Zora Cape Beach Rocks (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Zora Cape Beach Rocks (4)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Zora Cape Beach Rocks (5)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Zora Cape Beach Rocks (6)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Zora Cape Beach Rocks (7)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Zora Cape Beach Rocks (8)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Zora Cape Beach Rocks (9)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Zora Cape Island Rocks (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Island Rocks (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Island Rocks (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Island Rocks (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Island Rocks (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Rocks")
            ),
        "Zora Cape Great Fairy Boulders (0)":
            lambda state: True,
        "Zora Cape Great Fairy Boulders (1)":
            lambda state: True,
        "Road To Ikana Rock Circle (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Road To Ikana Rock Circle (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Road To Ikana Rock Circle (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Road To Ikana Rock Circle (4)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Road To Ikana Rock Circle (5)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Road To Ikana Rock Circle (6)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Road To Ikana Rock Circle (7)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Road To Ikana Rock Circle (8)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
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
        "Ikana Graveyard Captain Rockwall (0)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Captain Rockwall (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Captain Rockwall (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Captain Rockwall (3)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Ikana Graveyard Captain Rockwall (4)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Inverted Stone Tower Rocks (1)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Inverted Stone Tower Rocks (2)":
            lambda state: has_soul_absurd(state, player, options, "Rocks"),
        "Termina Field Business Scrub Grotto Crate (1)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Pirates Fortress Sewers Bonk Board (0)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates Fortress Sewers Bonk Board (1)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates Fortress Sewers Bonk Board (2)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (1)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (2)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (3)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (4)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (5)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (6)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (7)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (8)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (9)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (10)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (11)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (12)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (13)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (14)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (15)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Barrel (16)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Exit Barrel (1)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Exit Barrel (2)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Exit Barrel (3)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Exit Barrel (4)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Sewers Exit Barrel (5)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates Fortress' Exterior Balcony Barrel (1)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Leader's Room Barrel (1)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Leader's Room Barrel (2)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates' Fortress Guarded Bridge Barrel (0)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Pirates Fortress Interior Room Past Pink Guard Barrel (0)":
            lambda state: (
                has_soul_absurd(state, player, options, "Barrels")
            ),
        "Great Bay Temple Entrance Barrels (0)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Entrance Barrels (1)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Entrance Barrels (2)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Entrance Barrels (3)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Entrance Barrels (4)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Entrance Barrels (5)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Entrance Barrels (6)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Entrance Barrels (7)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Room Behind 1F Waterfall Barrels (0)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Room Behind 1F Waterfall Barrels (1)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Room Behind 1F Waterfall Barrels (2)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple 1F Red Valve Room Barrels (0)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple 1F Red Valve Room Barrels (1)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple 1F Red Valve Room Barrels (2)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple 1F Red Valve Room Barrels (3)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple 1F Red Valve Room Barrels (4)":
            lambda state: has_soul_absurd(state, player, options, "Barrels"),
        "Great Bay Temple Seesaw Room Crates (0)":
            lambda state: True,
        "Great Bay Temple Seesaw Room Crates (1)":
            lambda state: True,
        "Great Bay Temple Seesaw Room Crates (2)":
            lambda state: True,
        "Great Bay Temple Seesaw Room Crates (3)":
            lambda state: True,
        "Great Bay Temple Seesaw Room Crates (4)":
            lambda state: True,
        "Great Bay Temple Seesaw Room Crates (5)":
            lambda state: True,
        "Great Bay Temple Seesaw Room Crates (6)":
            lambda state: True,
        "Great Bay Temple Green Pipe Frozen Waterwheel Crates (0)":
            lambda state: True,
        "Great Bay Temple Green Pipe Frozen Waterwheel Crates (1)":
            lambda state: True,
        "Goron Trial Icicles (1)":
            lambda state: has_soul_npc(state, player, options, "Moon Kids"),
        "Goron Trial Icicles (2)":
            lambda state: has_soul_npc(state, player, options, "Moon Kids"),
        "Goron Trial Icicles (3)":
            lambda state: has_soul_npc(state, player, options, "Moon Kids"),
        "Goron Trial Icicles (4)":
            lambda state: has_soul_npc(state, player, options, "Moon Kids"),
        "Termina Field Bombable Rock Grotto Hive (1)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Termina Field Bio Baba Grotto Hive (1)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Termina Field Bio Baba Grotto Hive (2)":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Termina Field Cow Grotto Hive":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Great Bay Coast Cow Grotto Hive":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Clock Town Trading Post Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Astral Observatory Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Mountain Village Rooftop Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Mountain Village Spring Rooftop Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Path to Snowhead Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Path to Snowhead Spring Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Twin Islands Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Twin Islands (Spring) Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Snowhead Temple Lower Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Snowhead Temple Hidden Alcove Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Great Bay Coast Rock Wall Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Zora Cape Beavers Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Zora Cape Island Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Zora Hall Pervert Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Road to Ikana Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Stone Tower Lower Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Stone Tower Upper Scarecrow":
            lambda state: has_soul_npc(state, player, options, "Scarecrow"),
        "Termina Field Gossip Grotto Gossip Fairy":
            lambda state: has_soul_absurd(state, player, options, "Grottos"),
        "Ikana Canyon Across Ocean Deed Ravine Gossip Fairy":
            lambda state: (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player)
            ),
        "Deku Trial Front Left Gossip":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Deku Trial Back Left Gossip":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Deku Trial Front Right Gossip":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Deku Trial Back Right Gossip":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Deku Trial Furthest Back Gossip":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Goron Trial 1st Gazebo Gossip (0)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Goron Trial 1st Gazebo Gossip (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Goron Trial 2nd Gazebo Gossip (0)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Goron Trial 2nd Gazebo Gossip (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Goron Trial Near Heart Piece Gossip (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Zora Trial RRR Path Gossip":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Zora Trial RRL Path Gossip":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Zora Trial LRR Path Gossip":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Zora Trial LRLL Path Gossip":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Zora Trial LLL Path Gossip":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Link Trial Gossip (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Link Trial Gossip (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Link Trial Gossip (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Link Trial Gossip (4)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Link Trial Gossip (5)":
            lambda state: (
                has_soul_npc(state, player, options, "Moon Kids") and
                (
                    state.has("Song of Healing", player) or
                    state.has("Epona's Song", player) or 
                    state.has("Song of Storms", player)
                )
            ),
        "Fairy Fountain Left Side Well (0)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Kotake") or
                    has_soul_npc(state, player, options, "Business Scrubs")
                )
            ),
        "Fairy Fountain Left Side Well (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Kotake") or
                    has_soul_npc(state, player, options, "Business Scrubs")
                )
            ),
        "Fairy Fountain Left Side Well (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Kotake") or
                    has_soul_npc(state, player, options, "Business Scrubs")
                )
            ),
        "Fairy Fountain Left Side Well (3)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Kotake") or
                    has_soul_npc(state, player, options, "Business Scrubs")
                )
            ),
        "Fairy Fountain Left Side Well (4)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Kotake") or
                    has_soul_npc(state, player, options, "Business Scrubs")
                )
            ),
        "Fairy Fountain Left Side Well (5)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Kotake") or
                    has_soul_npc(state, player, options, "Business Scrubs")
                )
            ),
        "Fairy Fountain Left Side Well (6)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Kotake") or
                    has_soul_npc(state, player, options, "Business Scrubs")
                )
            ),
        "Fairy Fountain Left Side Well (7)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Kotake") or
                    has_soul_npc(state, player, options, "Business Scrubs")
                )
            ),
        "Termina Field Near Peehat Grotto Butterfly Fairy (1)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Termina Field Near Peehat Grotto Butterfly Fairy (2)":
            lambda state: has_soul_misc(state, player, options, "Butterflies"),
        "Termina Field Cow Grotto Butterfly Fairy (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Termina Field Cow Grotto Butterfly Fairy (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Termina Field Cow Grotto Butterfly Fairy (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Termina Field Bombable Rock Grotto Butterfly Fairy (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Termina Field Bombable Rock Grotto Butterfly Fairy (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
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
        "Mountain Village Spring Day Butterfly (1)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Mountain Village Spring Day Butterfly (2)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Mountain Village Spring Day Butterfly (3)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Mountain Village Spring Day Butterfly (4)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Mountain Village Spring Day Butterfly (5)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Mountain Village Spring Day Butterfly (6)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Mountain Village Spring Day Butterfly (7)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Mountain Village Spring Day Butterfly (8)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Mountain Village Spring Day Butterfly (9)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Great Bay Coast Outside Fisherman Hut Butterfly Fairy (1)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Great Bay Coast Outside Fisherman Hut Butterfly Fairy (2)":
            lambda state: (
                can_clear_snowhead(state, player, boss_placements) and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Great Bay Coast Cow Grotto Butterfly Fairy (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Great Bay Coast Cow Grotto Butterfly Fairy (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
        "Great Bay Coast Cow Grotto Butterfly Fairy (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_misc(state, player, options, "Butterflies")
            ),
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
        "Woodfall Temple Miniboss Frog":
            lambda state: (
                has_soul_enemy(state, player, options, "Gekko") and
                state.has("Don Gero's Mask", player)
            ),
        "Notebook Event Delivered Priority Mail":
            lambda state: (
                has_soul_npc(state, player, options, "Madame Aroma") and
                state.has("Kafei's Mask", player) and
                state.has("Priority Mail", player)
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
                has_soul_enemy(state, player, options, "Deku Baba") and
                has_soul_enemy(state, player, options, "Wolfos") and
                state.has("Letter to Kafei", player)
            ),
        "Notebook Event Learned Secret Code":
            lambda state: has_soul_npc(state, player, options, "Bomber Kids"),
        "Notebook Event Promised Midnight Meeting":
            lambda state: has_soul_npc(state, player, options, "Anju") and state.has("Kafei's Mask", player),
        "Notebook Event Promised To Meet Kafei":
            lambda state: has_soul_npc(state, player, options, "Anju") and state.has("Kafei's Mask", player),
        "Notebook Event Received All Night Mask":
            lambda state: has_soul_npc(state, player, options, "Curiosity Shop Man"),
        "Notebook Event Received Bombers NotebooK":
            lambda state: has_soul_npc(state, player, options, "Bomber Kids"),
        "Notebook Event Received Bremen Mask":
            lambda state: has_soul_npc(state, player, options, "Guru-Guru"),
        "Notebook Event Received Bunny Hood":
            lambda state: has_soul_npc(state, player, options, "Grog"),
        "Notebook Event Received Garos Mask":
            lambda state: has_soul_npc(state, player, options, "Gorman Brothers"),
        "Notebook Event Received Grandma Long Story HP":
            lambda state: has_soul_npc(state, player, options, "Anju's Grandmother"),
        "Notebook Event Received Grandma Short Story HP":
            lambda state: has_soul_npc(state, player, options, "Anju's Grandmother"),
        "Notebook Event Received Kafeis Mask":
            lambda state: has_soul_npc(state, player, options, "Madame Aroma"),
        "Notebook Event Received Kamaros Mask":
            lambda state: has_soul_npc(state, player, options, "Kamaro"),
        "Notebook Event Received Mayor HP":
            lambda state: has_soul_npc(state, player, options, "Council"),
        "Notebook Event Received Pendant of Memories":
            lambda state: (
                has_soul_npc(state, player, options, "Kafei") and
                has_soul_utility(state, player, options, "Postboxes") and
                state.has("Letter to Kafei", player)
            ),
        "Notebook Event Received Postman HP":
            lambda state: has_soul_npc(state, player, options, "Postman"),
        "Notebook Event Received Postmans Hat":
            lambda state: has_soul_npc(state, player, options, "Postman"),
        "Notebook Event Received Priority Mail":
            lambda state: (
                has_soul_npc(state, player, options, "Kafei") and
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                has_soul_utility(state, player, options, "Postboxes") and
                state.has("Letter to Kafei", player)
            ),
        "Notebook Event Received Room Key":
            lambda state: has_soul_npc(state, player, options, "Anju"),
        "Notebook Event Received Rosa Sisters HP":
            lambda state: has_soul_npc(state, player, options, "Rosa Sisters"),
        "Notebook Meeting Africa (Toto)":
            lambda state: (
                has_soul_npc(state, player, options, "Toto & Gorman") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Anju":
            lambda state: (
                has_soul_npc(state, player, options, "Anju") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Anju's Grandmother":
            lambda state: (
                has_soul_npc(state, player, options, "Anju's Grandmother") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Bomb Shop Lady":
            lambda state: (
                has_soul_npc(state, player, options, "Bomb Granny") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Bombers":
            lambda state: (
                has_soul_npc(state, player, options, "Bomber Kids") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Cremia":
            lambda state: (
                has_soul_npc(state, player, options, "Romani & Cremia") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Curiosity Shop Man":
            lambda state: (
                has_soul_npc(state, player, options, "Curiosity Shop Man") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Gorman":
            lambda state: (
                has_soul_npc(state, player, options, "Toto & Gorman") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Gorman Brothers":
            lambda state: (
                has_soul_npc(state, player, options, "Gorman Brothers") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Grog":
            lambda state: (
                has_soul_npc(state, player, options, "Grog") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Guru Guru":
            lambda state: (
                has_soul_npc(state, player, options, "Guru-Guru") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Kafei":
            lambda state: (
                has_soul_npc(state, player, options, "Kafei") and
                has_soul_utility(state, player, options, "Postboxes") and
                state.has("Letter to Kafei", player) and
                state.has("Bomber's Notebook", player)
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
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Rosa Sisters":
            lambda state: (
                has_soul_npc(state, player, options, "Rosa Sisters") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Shiro":
            lambda state: (
                has_soul_npc(state, player, options, "Shiro") and
                state.has("Bomber's Notebook", player)
            ),
        "Notebook Meeting Toilet Hand":
            lambda state: (
                has_soul_npc(state, player, options, "Toilet Hand") and
                state.has("Bomber's Notebook", player)
            ),
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
        "Termina Field Tree Near Observatory (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Termina Field Tree Near Observatory (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Termina Field Tree Near Observatory (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
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
        "Path To Mountains Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Path To Mountains Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Path To Mountains Tree (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Path To Mountains Tree (4)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Twin Islands Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Twin Islands Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Twin Islands Tree (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Twin Islands (Spring) Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Twin Islands (Spring) Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Twin Islands (Spring) Tree (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Path To Snowhead Tree Near Ledge":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Path To Snowhead Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Path To Snowhead Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Path To Snowhead Tree (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Goron Racetrack Trees (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Goron Racetrack Trees (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Goron Racetrack Trees (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Goron Racetrack Trees (4)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Goron Racetrack Trees (5)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Goron Racetrack Trees (6)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Goron Racetrack Trees (7)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Goron Racetrack Trees (8)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Goron Racetrack Trees (9)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Goron Racetrack Trees (10)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (4)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (5)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (6)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (7)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (8)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (9)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (10)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (11)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 1 (12)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (4)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (5)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (6)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (7)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (8)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (9)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (10)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (11)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (12)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Gorman Racetrack Tree Group 2 (13)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Great Bay Coast Nut Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Great Bay Coast Nut Tree (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Great Bay Coast Nut Tree (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Great Bay Coast Fisherman Island Nut Tree (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Zora Cape Nut Tree Near Jars (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Zora Cape Nut Tree Near Jars (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Zora Cape Nut Tree On Islands (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Zora Cape Nut Tree On Islands (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Zora Cape Nut Tree On Islands (3)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Zora Cape Nut Tree On Turtle Island (1)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Zora Cape Nut Tree On Turtle Island (2)":
            lambda state: has_soul_absurd(state, player, options, "Trees & Bushes"),
        "Beneath The Well Tree Near Cow":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Beneath The Well Bush Near Cow (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
        "Beneath The Well Bush Near Cow (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Trees & Bushes") and
                has_soul_npc(state, player, options, "Gibdos")
            ),
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
                state.has("Deku Mask", player)
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
                (
                    not options.child_wallet.value or
                    has_soul_npc(state, player, options, "Deku Playground Employee")
                ) and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    not options.child_wallet.value or
                    has_soul_npc(state, player, options, "Deku Playground Employee")
                ) and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    not options.child_wallet.value or
                    has_soul_npc(state, player, options, "Deku Playground Employee")
                ) and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    not options.child_wallet.value or
                    has_soul_npc(state, player, options, "Deku Playground Employee")
                ) and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    not options.child_wallet.value or
                    has_soul_npc(state, player, options, "Deku Playground Employee")
                ) and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    not options.child_wallet.value or
                    has_soul_npc(state, player, options, "Deku Playground Employee")
                ) and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    not options.child_wallet.value or
                    has_soul_npc(state, player, options, "Deku Playground Employee")
                ) and
                state.has("Deku Mask", player)
            ),
        "North Clock Town Deku Playground Flower (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Grottos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    not options.child_wallet.value or
                    has_soul_npc(state, player, options, "Deku Playground Employee")
                ) and
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
                state.has("Deku Mask", player)
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
        "Deku Palace Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace Flower (11)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Deku Palace To Swamp Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
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
        "Swamp Spiderhouse Giant Pot Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
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
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Gekko Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Gekko Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Gekko Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Gekko Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Dinolfos Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Dinolfos Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Platform Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Platform Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Platform Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple 2F Moving Platform Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Dragonfly Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Dragonfly Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Dragonfly Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Flower (6)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Flower (7)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Flower (8)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Flower (9)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Pre Boss Room Flower (10)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Woodfall Temple Odolwa Golden Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Post Dungeon Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Post Dungeon Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Southern Swamp Post Dungeon Business Scrub Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Goron Village Business Scrub Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Snowhead Temple Frozen Green Door Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Snowhead Temple Frozen Green Door Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Snowhead Temple Main Room Wall Chest Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Snowhead Temple Flower Outside Goht":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Zora Cape Lower Wall Flower Near Beavers":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Zora Hall Business Scrub Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Canyon Business Scrub Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Deku Mask", player)
            ),
        "Ikana Canyon Business Scrub Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Business Scrubs") and
                state.has("Deku Mask", player)
            ),
        "Well Deku Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Business Scrubs") or
                    has_soul_npc(state, player, options, "Kotake")
                ) and
                state.has("Deku Mask", player)
            ),
        "Well Deku Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Business Scrubs") or
                    has_soul_npc(state, player, options, "Kotake")
                ) and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Left Side Falling Ceiling Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Left Side Falling Ceiling Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Left Side Falling Ceiling Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Left Side Falling Ceiling Room Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Left Side Falling Ceiling Room Flower (5)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Left Side Broken Floor Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Left Side Broken Floor Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Left Side Broken Floor Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Exterior Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Exterior Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Exterior Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Ikana Castle Exterior Flower (4)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Stone Tower Temple Deku Updraft Flower":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
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
                state.has("Deku Mask", player)
            ),
        "Inverted Stone Tower Temple Small Poe Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Inverted Stone Tower Temple Lower Bridge Room Flower (1)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Inverted Stone Tower Temple Lower Bridge Room Flower (2)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
            ),
        "Inverted Stone Tower Temple Lower Bridge Room Flower (3)":
            lambda state: (
                has_soul_absurd(state, player, options, "Deku Flowers") and
                state.has("Deku Mask", player)
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
        "North Clock Town Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "West Clock Town Sword School Night 3 Midnight Cut the Sign":
            lambda state: (
                has_soul_absurd(state, player, options, "Signs") and
                has_soul_npc(state, player, options, "Swordsman")
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
            lambda state: has_soul_absurd(state, player, options, "Signs"),
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
            lambda state: has_soul_absurd(state, player, options, "Signs"),
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
            lambda state: has_soul_absurd(state, player, options, "Signs"),
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
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Goron Village Outside Lens Cave Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Goron Village Outside Keg Goron Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Goron Village Outside Goron Shrine":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Path to Snowhead Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Path to Snowhead Upper Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Outside Snowhead Temple Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Mountain Village Owl Statue Spring Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Mountain Village Spring Near Graveyard Pond Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Mountain Village Spring Path to Twin Islands Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Twin Islands Spring Outside Goron Racetrack Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
        "Romani Ranch Epona Stable Cut the Sign":
            lambda state: has_soul_absurd(state, player, options, "Signs"),
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
            lambda state: has_soul_absurd(state, player, options, "Signs"),
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
        "Woodfall Temple Web Leading to Dark Room":
            lambda state: has_soul_absurd(state, player, options, "Deku Flowers"),
        "Beneath the Well Left Side Web Near Fairy Fountain (1)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    has_soul_npc(state, player, options, "Business Scrubs") or
                    has_soul_npc(state, player, options, "Kotake")
                )
            ),
        "Beneath the Well Left Side Web Near Fairy Fountain (2)":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                has_soul_absurd(state, player, options, "Deku Flowers") and
                (
                    has_soul_npc(state, player, options, "Business Scrubs") or
                    has_soul_npc(state, player, options, "Kotake")
                )
            ),
        "Beneath the Well Right Side Web Near Milk Gibdo":
            lambda state: (
                has_soul_npc(state, player, options, "Gibdos") and
                (
                    has_soul_npc(state, player, options, "Business Scrubs") or
                    has_soul_npc(state, player, options, "Kotake")
                )
            ),
        "Majora's Soul":
            lambda state: (
                has_enough_remains(state, player, 4) and
                has_enough_masks(state, player, 24) and
                has_all_owls(state, player, options) and
                has_all_scarecrows(state, player, options) and
                has_all_frogs(state, player, options)
            ),
    }