from dataclasses import dataclass

from typing import Dict

from Options import Choice, Option, DefaultOnToggle, Toggle, Range, OptionList, StartInventoryPool, DeathLink, OptionGroup, PerGameCommonOptions


class LogicDifficulty(Choice):
    """Set the logic difficulty used when generating."""
    display_name = "Logic Difficulty"
    # ~ option_easy = 0
    option_normal = 1
    #option_obscure_glitchless = 2
    #option_glitched = 3
    option_no_logic = 4
    # ~ alias_baby = option_easy
    default = 1


# Moon Goal Options
class MoonRemainsRequired(Range):
    """Set the amount of boss remains required to reach the Moon after playing Oath to Order."""
    display_name = "Moon Boss Remains Required"
    range_start = 0
    range_end = 4
    default = 4


class MoonMasksRequired(Range):
    """Set the amount of masks required to reach the Moon after playing Oath to Order."""
    display_name = "Moon Masks Required"
    range_start = 0
    range_end = 24
    default = 0


class MoonStarFox(Toggle):
    """Set whether you require Keaton, Scents, Bremen, Bunny & Gero masks to reach the Moon 
    after playing Oath to Order."""
    display_name = "Moon Star Fox"


class MoonOwlsRequired(Range):
    """Set the amount of owl statues required to reach the Moon after playing Oath to Order."""
    display_name = "Moon Owls Required"
    range_start = 0
    range_end = 10
    default = 0


class MoonScarecrowsRequired(Range):
    """Set the amount of scarecrows required to reach the Moon after playing Oath to Order."""
    display_name = "Moon Scarecrows Required"
    range_start = 0
    range_end = 17
    default = 0


class MoonFrogsRequired(Range):
    """Set the amount of frogs required to reach the Moon after playing Oath to Order."""
    display_name = "Moon Frogs Required"
    range_start = 0
    range_end = 5
    default = 0


class MoonItemsRequired(Range):
    """Set the amount of trade items required to reach the Moon after playing Oath to Order."""
    display_name = "Moon Items Required"
    range_start = 0
    range_end = 29
    default = 0


# Majora Goal Options
class MajoraRemainsRequired(Range):
    """Set the amount of boss remains required to fight Majora."""
    display_name = "Majora Boss Remains Required"
    range_start = 0
    range_end = 4
    default = 4


class MajoraMasksRequired(Range):
    """Set the amount of masks required to fight Majora."""
    display_name = "Majora Masks Required"
    range_start = 0
    range_end = 24
    default = 0


class MajoraStarFox(Toggle):
    """Set whether you require Keaton, Scents, Bremen, Bunny & Gero masks to fight Majora."""
    display_name = "Majora Star Fox"


class MajoraOwlsRequired(Range):
    """Set the amount of owl statues required to fight Majora."""
    display_name = "Majora Owls Required"
    range_start = 0
    range_end = 10
    default = 0


class MajoraScarecrowsRequired(Range):
    """Set the amount of scarecrows required to fight Majora."""
    display_name = "Majora Scarecrows Required"
    range_start = 0
    range_end = 17
    default = 0


class MajoraFrogsRequired(Range):
    """Set the amount of frogs required to fight Majora."""
    display_name = "Majora Frogs Required"
    range_start = 0
    range_end = 5
    default = 0


class MajoraItemsRequired(Range):
    """Set the amount of trade items required to fight Majora."""
    display_name = "Majora Items Required"
    range_start = 0
    range_end = 29
    default = 0


class CompletionGoal(Toggle):
    """Enable the completion goal which requires 100% of all collectibles and has a special flag assosciated with it."""
    display_name = "Completion Goal"


class CAMC(DefaultOnToggle):
    """Set whether chest appearance matches contents."""
    display_name = "CAMC"


# TODO: separate dungeons and bosses into more groups
class DungeonEntranceRando(Toggle):
    """Set whether dungeons are randomized amongst themselves."""
    display_name = "Dungeon Entrance Rando"


class BossEntranceRando(Toggle):
    """Set whether bosses are randomized amongst themselves."""


class DungeonChaining(Toggle):
    """When Dungeon Entrance Rando and Boss Entrance Rando are enabled, set whether dungeons can lead into each other/only have bosses."""
    display_name = "Dungeon Chaining"


class Ocarinaless(Toggle):
    """Start the game without an Ocarina shuffling it into the pool. This has the effect of sped up time"""
    display_name = "Ocarinaless"


class Timeless(Toggle):
    """Start the game without Song of Time shuffling it into the pool."""
    display_name = "Timeless"


class Swordless(Toggle):
    """Start the game without a sword, and shuffle an extra Progressive Sword into the pool."""
    display_name = "Swordless"


class Shieldless(Toggle):
    """Start the game without a shield, and shuffle an extra Progressive Shield into the pool."""
    display_name = "Shieldless"


class StartWithSoaring(DefaultOnToggle):
    """Start the game with Song of Soaring."""
    display_name = "Start With Soaring"


class StartingHeartQuarters(Range):
    """The number of heart quarters Link starts with.
    If less than 12, extra heart items will be shuffled into the pool to accommodate."""
    display_name = "Starting Hearts"
    range_start = 4
    range_end = 12
    default = 12


class StartingHeartsAreContainersOrPieces(Choice):
    """Choose whether Link's starting hearts are shuffled into the pool as Heart Containers (plus the remainder as Heart Pieces) or as all Heart Pieces."""
    display_name = "Starting Hearts are Containers or Pieces"
    option_containers = 0
    option_pieces = 1
    default = 0


class ShuffleRegionalMaps(Choice):
    """Choose whether to shuffle every regional map from Tingle."""
    display_name = "Shuffle Regional Maps"
    option_vanilla = 0
    option_starting = 1
    option_anywhere = 2
    default = 1


class ChildWallet(Toggle):
    """Start the game without the initial 99 Rupee Wallet, and shuffle an extra Progressive Wallet into the pool."""
    display_name = "Shuffle Child Wallet"


class DeityWallet(Toggle):
    """Add an extra Progressive Wallet into the pool that holds up to 999 rupees.
    
    Offsensive Shop Prices will compensate for this increased maximum capacity."""
    display_name = "Add Deity Wallet"


class ShuffleBossRemains(Choice):
    """Choose whether to shuffle the Boss Remains received after beating a boss at the end of a dungeon.
    
    vanilla: Boss Remains are placed in their vanilla locations.
    anything: Any item can be given by any of the Boss Remains, and Boss Remains can be found anywhere in any world.
    bosses: Boss Remains are shuffled amongst themselves as the rewards for defeating bosses."""
    display_name = "Shuffle Boss Remains"
    option_vanilla = 0
    option_anywhere = 1
    option_bosses = 2
    default = 0


class BossWarpsWithRemains(DefaultOnToggle):
    """Choose whether to retain the vanilla ability to warp the boss of dungeons by having their vanilla remains.
    Getting the remains check for a dungeon will open its warp regardless."""
    display_name = "Warp to Bosses Using Remains"


class ShuffleSpiderHouseReward(Toggle):
    """Choose whether to shuffle the Mask of Truth given at the end of the Southern Spider House and the Wallet Upgrade at the end of the Ocean Spider House."""
    display_name = "Shuffle Swamphouse Reward"


class RequiredSkullTokens(Range):
    """The number of Gold Skulltula Tokens needed to get the reward from their respective Spider House.
    All 30 Tokens from each Spider House are still shuffled into the item pool regardless of the selection.
    Valid amounts are within the range 0-30."""
    display_name = "Required Skulltula Tokens"
    range_start = 0
    range_end = 30
    default = 30


class Skullsanity(Choice):
    """Choose what items gold skulltulas can give.
    
    vanilla: Keep the swamphouse in generation, but only place Skulltula tokens there.
    anything: Any item can be given by any Skulltula, and tokens can be found anywhere in any world.
    ignore: Remove the swamphouse from generation entirely, lowering the hint percentage."""
    display_name = "Skullsanity"
    option_vanilla = 0
    option_anything = 1
    option_ignore = 2
    default = 0


class Shopsanity(Choice):
    """Choose whether shops and their items are shuffled into the pool.
    This includes Trading Post, Bomb Shop, Goron Shop, and Zora Shop, along with the Gorman Ranch and Milk Bar purchases.
    
    vanilla: Shop items are not shuffled.
    enabled: Every item in shops are shuffled, with alternate shops sharing the same items.
    advanced: Every single item in shops are shuffled, including the alternate Night Trading Post and Spring Goron Shop."""
    display_name = "Shopsanity"
    option_vanilla = 0
    option_enabled = 1
    option_advanced = 2
    default = 0

class Scrubsanity(Toggle):
    """Choose whether to shuffle Business Scrub purchases."""
    display_name = "Shuffle Business Scrub Purchases"

class ShopPrices(Choice):
    """Choose how expensive shop items are.
    These only apply to the main shops of the game.
    This has no effect if shopsanity is disabled.
    
    vanilla: Shop items have their normal prices.
    free: All shop items are free and cost 0 Rupees.
    cheap: Shop items vary in price but can all be purchased with the starting wallet.
    expensive: Shop items vary in price but may require the Adult's Wallet. No shop items will require the Giant's Wallet.
    offensive: Shop items vary in price but may require the Adult's Wallet and sometimes even the Giant's Wallet."""
    display_name = "Shop Prices"
    option_vanilla = 0
    option_free = 1
    option_cheap = 2
    option_expensive = 3
    option_offensive = 4
    default = 0


class Cowsanity(Toggle):
    """Choose whether to shuffle Cows."""
    display_name = "Shuffle Cows"


class ShuffleGreatFairyRewards(Toggle):
    """Choose whether to shuffle Great Fairy rewards."""
    display_name = "Shuffle Great Fairy Rewards"


class RequiredStrayFairies(Range):
    """The number of Stray Fairies needed to get the reward from their respective Great Fairy (excluding North Clock Town's Great Fairy of Magic).
    All 15 Stray Fairies from each dungeon are still shuffled into the item pool regardless of the selection.
    Valid amounts are within the range 0-15."""
    display_name = "Required Stray Fairies"
    range_start = 0
    range_end = 15
    default = 15


class Fairysanity(Toggle):
    """Choose whether Stray Fairies are shuffled into the pool."""
    display_name = "Fairysanity"


class Keysanity(Toggle):
    """Choose whether Small Keys are shuffled into the pool or placed in their vanilla locations."""
    display_name = "Keysanity"


class BossKeysanity(Toggle):
    """Choose whether Boss Keys are shuffled into the pool or placed in their vanilla locations."""
    display_name = "BossKeysanity"    


class CuriosityShopTrades(Toggle):
    """Choose whether to shuffle the rupees given for trading bottled items at the Curiosty Shop."""
    display_name = "Curiosity Shop Trades"


class IntroChecks(Toggle):
    """Choose whether to shuffle the checks normally found before entering the Clock Tower.
    
    A way backwards through these areas has been added through the stone door at the bottom of the Clock Tower Interior."""
    display_name = "Enable Intro Checks"


class Grasssanity(Choice):
    """Choose how grass is shuffled into the pool.
    
    normal: All grass found throughout the game is shuffled.
    no_termina_field: All grass except Termina Field (including its grottos).
    grotto_and_cave_only: Only grass found in grottos and caves.
    dungeon_only: Only grass found in dungeons."""
    display_name = "Grasssanity"
    option_disabled = 0
    option_normal = 1
    option_no_termina_field = 2
    option_grotto_and_cave_only = 3
    option_dungeon_only = 4


class Potsanity(Choice):
    """Choose how pots are shuffled into the pool.
    
    all: All pots found throughout the game are shuffled.
    overworld_only: Only pots found in the overworld.
    dungeon_only: Only pots found in dungeons."""
    display_name = "Potsanity"
    option_disabled = 0
    option_all = 1
    option_overworld_only = 2
    option_dungeon_only = 3


class Hitsanity(Toggle):
    """Choose whether hit spot items are shuffled into the pool."""
    display_name = "Hitsanity"


class Rocksanity(Toggle):
    """Choose whether rock items are shuffled into the pool."""
    display_name = "Rocksanity"


class Soilsanity(Toggle):
    """Choose whether soil items are shuffled into the pool."""
    display_name = "Soilsanity"


class Rupeesanity(Toggle):
    """Choose whether freestanding rupees are shuffled into the pool."""
    display_name = "Rupeesanity"


class Invisisanity(Toggle):
    """Choose whether invisible rupees are shuffled into the pool."""
    display_name = "Invisisanity"    


class Snowsanity(Toggle):
    """Choose whether snowball items are shuffled into the pool."""
    display_name = "Snowsanity"


class Woodsanity(Toggle):
    """Choose whether wooden items are shuffled into the pool."""
    display_name = "Woodsanity"


class Realfairysanity(Toggle):
    """Choose whether gossip, butterfly and freestanding fairies are shuffled."""
    display_name = "Realfairysanity"


class Iciclesanity(Toggle):
    """Choose whether Icicle items are shuffled into the pool."""
    display_name = "Iciclesanity" 


class Scarecrowsanity(Toggle):
    """Choose whether the scarecrow rewards an item when he is spawned."""
    display_name = "Scarecrowsanity"


class Hivesanity(Toggle):
    """Choose whether Hive items are shuffled into the pool."""
    display_name = "Hivesanity"    


class Notebooksanity(Toggle):
    """Choose whether Notebook Entries are shuffled items."""
    display_name = "Notebooksanity"   


class Owlsanity(Toggle):
    """Choose whether Owl Statues are shuffled items."""
    display_name = "Owlsanity"  


class Frogsanity(Toggle):
    """Choose whether the four frogs are shuffled items."""
    display_name = "Frogsanity"       


class Treesanity(Toggle):
    """Choose whether the trees and bushes drop shuffled items."""
    display_name = "Treesanity"       


class Flowersanity(Toggle):
    """Choose whether the Deku Flowers give shuffled items when entered."""
    display_name = "Flowersanity" 


class Signsanity(Toggle):
    """Choose whether Square Signposts give shuffled items when cut."""
    display_name = "Signsanity"


class Websanity(Toggle):
    """Choose whether burning Spider Webs gives shuffled items."""
    display_name = "Websanity"


class Oneoffs(Toggle):
    """Choose whether one off locations like skullkid pictures and bombable walls give shuffled items."""
    display_name = "Oneoffs"


class BossSouls(Choice):
    """Add souls for the main bosses in the game.
    They will not spawn unless their soul is obtained.
    
    This includes Odolwa, Goht, Gyorg, Twinmold, and optionally Majora."""
    display_name = "Add Boss Souls"
    option_false = 0
    option_true = 1
    option_true_include_majora = 2
    default = 0


class NPCSouls(Toggle):
    """Add souls for npc characters such as Anju, Romani, Kamaro etc.
    They will not spawn unless their soul is obtained."""
    display_name = "Add NPC Souls"


class EnemySouls(Toggle):
    """Add souls for basic enemies such as wolfos, peahat etc.
    They will not spawn unless their soul is obtained."""
    display_name = "Add Enemy Souls"


class MiscSouls(Toggle):
    """Add souls for Cows, Gold Skulltulas, and Keaton.
    They will not spawn unless their soul is obtained."""
    display_name = "Add Misc Souls"


class UtilitySouls(Toggle):
    """Add souls for Utility Items like the Postboxes etc.
    They will not spawn unless their soul is obtained."""
    display_name = "Add Utility Souls"


class AbsurdSouls(Toggle):
    """Add souls for absurd things like rocks, songwall and grass etc.
    They will not spawn unless their soul is obtained."""
    display_name = "Add Absurd Souls"


class StartWithConsumables(DefaultOnToggle):
    """Choose whether to start with basic consumables (99 rupees, 10 deku sticks, 20 deku nuts)."""
    display_name = "Start With Consumables"


class PermanentChateauRomani(DefaultOnToggle):
    """Choose whether the Chateau Romani stays even after a reset."""
    display_name = "Permanent Chateau Romani"


class StartWithInvertedTime(Toggle):
    """Choose whether time starts out inverted at Day 1, even after a reset."""
    display_name = "Reset With Inverted Time"


class ReceiveFilledWallets(DefaultOnToggle):
    """Choose whether you receive wallets pre-filled (not including the starting wallet)."""
    display_name = "Receive Filled Wallets"


class MagicIsATrap(Toggle):
    """Set whether to preserve the vanilla bug where you are able to use certain magic items and abilities without magic.
    Once you receive magic, those items and abilities will begin to reduce magic normally.
    
    (No logical implications)"""
    display_name = "Magic Is a Trap"


class UsefulHints(Range):
    """The percentage of gossip stones which provide useful hints for items placed within the multiworld."""
    display_name = "Useful Hint Percentage"
    range_start = 0
    range_end = 100
    default = 70


class DamageMultiplier(Choice):
    """Adjust the amount of damage taken."""
    display_name = "Damage Multiplier"
    option_half = 0
    option_normal = 1
    option_double = 2
    option_quad = 3
    option_ohko = 4
    default = 1

class DeathBehavior(Choice):
    """Change what happens when you die.
    
    vanilla: The normal death cutscene plays when you die.
    fast: The death cutscene is massively sped up.
    moon_crash: Triggers a moon crash and restarts the current cycle."""
    display_name = "Death Behavior"
    option_vanilla = 0
    option_fast = 1
    option_instant = 2
    option_moon_crash = 3
    default = 0


class LinkTunicColor(OptionList):
    """Choose a color for Link's tunic."""
    display_name = "Link Tunic Color"
    default = [30, 105, 27]


mm_option_groups = [
    OptionGroup("Moon Requirements", [
        MoonRemainsRequired,
        MoonMasksRequired,
        MoonStarFox,
        MoonOwlsRequired,
        MoonScarecrowsRequired,
        MoonFrogsRequired,
        MoonItemsRequired,
    ]),
    OptionGroup("Majora Requirements", [
        MajoraRemainsRequired,
        MajoraMasksRequired,
        MajoraStarFox,
        MajoraOwlsRequired,
        MajoraScarecrowsRequired,
        MajoraFrogsRequired,
        MajoraItemsRequired,
    ]),
    OptionGroup("Starting Item Shuffle", [
        Swordless,
        Shieldless,
        StartingHeartQuarters, # change to just hearts
        StartingHeartsAreContainersOrPieces,
        Ocarinaless,
        Timeless
    ]),
    OptionGroup("Extensions", [
        ChildWallet,
        DeityWallet,
    ]),
    OptionGroup("Helpful Starting Items", [
        StartWithSoaring,
        StartWithInvertedTime, # remove option later
        StartWithConsumables, # adjust option once we get capacity shuffles
        PermanentChateauRomani,
        ReceiveFilledWallets,
        MagicIsATrap
    ]),
    OptionGroup("Dungeon Options", [
        ShuffleBossRemains,
        BossWarpsWithRemains,
        Keysanity,
        BossKeysanity,
    ]),
    OptionGroup("Sanities", [
        Skullsanity,
        RequiredSkullTokens,
        ShuffleSpiderHouseReward,
        Fairysanity,
        RequiredStrayFairies,
        ShuffleGreatFairyRewards,
        Shopsanity,
        ShopPrices,
        Scrubsanity,
        CuriosityShopTrades,
        Cowsanity,
        ShuffleRegionalMaps, # Tinglesanity
        IntroChecks,
        Grasssanity,
        Potsanity,
        Hitsanity,
        Rocksanity,
        Soilsanity,
        Rupeesanity,
        Invisisanity,
        Snowsanity,
        Woodsanity,
        Realfairysanity,
        Iciclesanity,
        Scarecrowsanity,
        Hivesanity,
        Notebooksanity,
        Owlsanity,
        Frogsanity,
        Treesanity,
        Flowersanity,
        Signsanity,
        Websanity,
        Oneoffs
    ]),
    OptionGroup("Souls", [
        BossSouls,
        NPCSouls,
        EnemySouls,
        MiscSouls,
        UtilitySouls,
        AbsurdSouls,
    ]),
    OptionGroup("Entrance Randomization", [
        DungeonEntranceRando,
        BossEntranceRando,
        DungeonChaining,
    ]),
    OptionGroup("options I want to remove in the future and are down here to get out of the way", [
        CompletionGoal,
        CAMC,
        DamageMultiplier,
        DeathBehavior,
        LinkTunicColor,
        DeathLink,
    ]),
]


@dataclass
class MMROptions(PerGameCommonOptions):
    start_inventory_from_pool: StartInventoryPool
    logic_difficulty: LogicDifficulty
    moon_remains_required: MoonRemainsRequired
    moon_masks_required: MoonMasksRequired
    moon_star_fox: MoonStarFox
    moon_owls_required: MoonOwlsRequired
    moon_scarecrows_required: MoonScarecrowsRequired
    moon_frogs_required: MoonFrogsRequired
    moon_items_required: MoonItemsRequired
    majora_remains_required: MajoraRemainsRequired
    majora_masks_required: MajoraMasksRequired
    majora_star_fox: MajoraStarFox
    majora_owls_required: MajoraOwlsRequired
    majora_scarecrows_required: MajoraScarecrowsRequired
    majora_frogs_required: MajoraFrogsRequired
    majora_items_required: MajoraItemsRequired
    completion_goal: CompletionGoal
    camc: CAMC
    dungeon_entrance_rando: DungeonEntranceRando
    boss_entrance_rando: BossEntranceRando
    dungeon_chaining: DungeonChaining
    ocarinaless: Ocarinaless
    timeless: Timeless
    swordless: Swordless
    shieldless: Shieldless
    start_with_soaring: StartWithSoaring
    starting_hearts: StartingHeartQuarters
    starting_hearts_are_containers_or_pieces: StartingHeartsAreContainersOrPieces
    shuffle_regional_maps: ShuffleRegionalMaps
    shuffle_boss_remains: ShuffleBossRemains
    remains_allow_boss_warps: BossWarpsWithRemains
    shuffle_spiderhouse_reward: ShuffleSpiderHouseReward
    required_skull_tokens: RequiredSkullTokens
    skullsanity: Skullsanity
    shopsanity: Shopsanity
    scrubsanity: Scrubsanity
    shop_prices: ShopPrices
    child_wallet: ChildWallet
    deity_wallet: DeityWallet
    cowsanity: Cowsanity
    shuffle_great_fairy_rewards: ShuffleGreatFairyRewards
    required_stray_fairies: RequiredStrayFairies
    fairysanity: Fairysanity
    keysanity: Keysanity
    bosskeysanity: BossKeysanity
    curiostity_shop_trades: CuriosityShopTrades
    intro_checks: IntroChecks
    grasssanity: Grasssanity
    potsanity: Potsanity
    hitsanity: Hitsanity
    rocksanity: Rocksanity
    soilsanity: Soilsanity
    rupeesanity: Rupeesanity
    invisisanity: Invisisanity
    snowsanity: Snowsanity
    woodsanity: Woodsanity
    realfairysanity: Realfairysanity
    iciclesanity: Iciclesanity
    scarecrowsanity: Scarecrowsanity
    hivesanity: Hivesanity
    notebooksanity: Notebooksanity
    owlsanity: Owlsanity
    frogsanity: Frogsanity
    treesanity: Treesanity
    flowersanity: Flowersanity
    signsanity: Signsanity
    websanity: Websanity
    oneoffs: Oneoffs
    boss_souls: BossSouls
    npc_souls: NPCSouls
    enemy_souls: EnemySouls
    misc_souls: MiscSouls
    utility_souls: UtilitySouls
    absurd_souls: AbsurdSouls
    start_with_consumables: StartWithConsumables
    permanent_chateau_romani: PermanentChateauRomani
    start_with_inverted_time: StartWithInvertedTime
    receive_filled_wallets: ReceiveFilledWallets
    magic_is_a_trap: MagicIsATrap
    hint_percentage: UsefulHints
    damage_multiplier: DamageMultiplier
    death_behavior: DeathBehavior
    death_link: DeathLink
    link_tunic_color: LinkTunicColor