#ifndef __YAML_GEN__
#define __YAML_GEN__

#include "apcommon.h"
#include "recompui.h"

typedef enum {
    RANDO_ACCESSABILITY_FULL = 0,
    RANDO_ACCESSABILITY_MINIMAL = 1,
} RandoAccessability;

typedef enum {
    RANDO_LOGIC_DIFFICULTY_EASY = 0,
    RANDO_LOGIC_DIFFICULTY_NORMAL = 1,
    RANDO_LOGIC_DIFFICULTY_NO_LOGIC = 2
} RandoLogicDifficulty;

typedef enum {
    RANDO_STARTING_HEARTS_ARE_CONTAINERS = 0,
    RANDO_STARTING_HEARTS_ARE_PIECES = 1
} RandoStartingHeartsAreContainersOrPieces;

typedef enum {
    RANDO_SHUFFLE_REGIONAL_MAPS_VANILLA = 0,
    RANDO_SHUFFLE_REGIONAL_MAPS_STARTING = 1,
    RANDO_SHUFFLE_REGIONAL_MAPS_ANYWHERE = 2
} RandoShuffleRegionalMaps;

typedef enum {
    RANDO_SHUFFLE_BOSS_REMAINS_VANILLA = 0,
    RANDO_SHUFFLE_BOSS_REMAINS_ANYWHERE = 1,
    RANDO_SHUFFLE_BOSS_REMAINS_BOSSES = 2
} RandoShuffleBossRemains;

typedef enum {
    RANDO_SKULLSANITY_VANILLA = 0,
    RANDO_SKULLSANITY_ANYTHING = 1,
    RANDO_SKULLSANITY_IGNORE = 2
} RandoSkullSanity;

typedef enum {
    RANDO_SHOPSANITY_VANILLA = 0,
    RANDO_SHOPSANITY_ENABLED = 1,
    RANDO_SHOPSANITY_ADVANCED = 2
} RandoShopSanity;

typedef enum {
    RANDO_DAMAGE_MULITPLIER_HALF = 0,
    RANDO_DAMAGE_MULITPLIER_NORMAL = 1,
    RANDO_DAMAGE_MULITPLIER_DOUBLE = 2,
    RANDO_DAMAGE_MULITPLIER_QUAD = 3,
    RANDO_DAMAGE_MULITPLIER_ONE_HOT_KO = 4
} RandoDamageMultiplier;

typedef enum {
    RANDO_DEATH_BEHAVIOR_VANILLA = 0,
    RANDO_DEATH_BEHAVIOR_FAST = 1,
    RANDO_DEATH_BEHAVIOR_MOON_CRASH = 2
} RandoDeathBehavior;

typedef struct {
    RandoAccessability accessability;
    RandoLogicDifficulty logicDifficulty;
    bool chestsMatchContents;
    bool startSwordless;
    bool startShieldless;
    bool startWithSoaring;
    bool startingHeartsAreRandom;
    int startingHeartsMin;
    int startingHeartsMax;
    int startingHearts;
    RandoStartingHeartsAreContainersOrPieces startingHeartsAreContainersOrPieces;
    RandoShuffleRegionalMaps shuffleRegionalMaps;
    RandoShuffleBossRemains shuffleBossRemains;
    bool shuffleSpiderHouseRewards;
    RandoSkullSanity skullSanity;
    RandoShopSanity shopSanity;
    bool scrubSanity;
    bool cowSanity;
    bool shuffleCreatFairyRewards;
    bool fairySanity;
    bool startWithConsumables;
    bool permanentChateauRomani;
    bool startWithInvertedTime;
    bool recieveFilledWallets;
    RandoDamageMultiplier damageMultiplier;
    RandoDeathBehavior deathBehavior;

} RandoYamlConfig;

extern RecompuiContext yaml_menu_context;
extern RecompuiResource yaml_menu_root;
extern RecompuiResource yaml_menu_container;

RandoYamlConfig* RandoYamlConfig_Create();
void RandoYamlConfig_Destroy(RandoYamlConfig* config);
void randoCreateYamlConfigMenu();

#endif