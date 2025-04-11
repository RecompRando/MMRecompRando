#ifndef __YAML_GEN__
#define __YAML_GEN__

#include "apcommon.h"
#include "recompui.h"

typedef enum {
    RANDO_ACCESSABILITY_FULL = 0,
    RANDO_ACCESSABILITY_MINIMAL = 1,
    RANDO_ACCESSABILITY_MAX = 0xFFFFFFFF,
} RandoAccessability;

typedef enum {
    RANDO_LOGIC_DIFFICULTY_EASY = 0,
    RANDO_LOGIC_DIFFICULTY_NORMAL = 1,
    RANDO_LOGIC_DIFFICULTY_NO_LOGIC = 2,
    RANDO_LOGIC_DIFFICULTY_MAX = 0xFFFFFFFF
} RandoLogicDifficulty;

typedef enum {
    RANDO_STARTING_HEARTS_ARE_CONTAINERS = 0,
    RANDO_STARTING_HEARTS_ARE_PIECES = 1,
    RANDO_STARTING_HEARTS_ARE_MAX = 0xFFFFFFFF
} RandoStartingHeartsAreContainersOrPieces;

typedef enum {
    RANDO_SHUFFLE_REGIONAL_MAPS_VANILLA = 0,
    RANDO_SHUFFLE_REGIONAL_MAPS_STARTING = 1,
    RANDO_SHUFFLE_REGIONAL_MAPS_ANYWHERE = 2,
    RANDO_SHUFFLE_REGIONAL_MAPS_MAX = 0xFFFFFFFF
} RandoShuffleRegionalMaps;

typedef enum {
    RANDO_SHUFFLE_BOSS_REMAINS_VANILLA = 0,
    RANDO_SHUFFLE_BOSS_REMAINS_ANYWHERE = 1,
    RANDO_SHUFFLE_BOSS_REMAINS_BOSSES = 2,
    RANDO_SHUFFLE_BOSS_REMAINS_MAX = 0xFFFFFFFF
} RandoShuffleBossRemains;

typedef enum {
    RANDO_SKULLSANITY_VANILLA = 0,
    RANDO_SKULLSANITY_ANYTHING = 1,
    RANDO_SKULLSANITY_IGNORE = 2,
    RANDO_SKULLSANITY_MAX = 0xFFFFFFFF
} RandoSkullSanity;

typedef enum {
    RANDO_SHOPSANITY_VANILLA = 0,
    RANDO_SHOPSANITY_ENABLED = 1,
    RANDO_SHOPSANITY_ADVANCED = 2,
    RANDO_SHOPSANITY_MAX = 0xFFFFFFFF
} RandoShopSanity;

typedef enum {
    RANDO_DAMAGE_MULITPLIER_HALF = 0,
    RANDO_DAMAGE_MULITPLIER_NORMAL = 1,
    RANDO_DAMAGE_MULITPLIER_DOUBLE = 2,
    RANDO_DAMAGE_MULITPLIER_QUAD = 3,
    RANDO_DAMAGE_MULITPLIER_ONE_HIT_KO = 4,
    RANDO_DAMAGE_MULITPLIER_MAX = 0xFFFFFFFF
} RandoDamageMultiplier;

typedef enum {
    RANDO_DEATH_BEHAVIOR_VANILLA = 0,
    RANDO_DEATH_BEHAVIOR_FAST = 1,
    RANDO_DEATH_BEHAVIOR_MOON_CRASH = 2,
    RANDO_DEATH_BEHAVIOR_MAX = 0xFFFFFFFF
} RandoDeathBehavior;

typedef struct {
    /* RandoAccessability */ 
    u32 accessability;
    /* RandoLogicDifficulty */ 
    u32 logicDifficulty;
    bool chestsMatchContents;
    bool startSwordless;
    bool startShieldless;
    bool startWithSoaring;
    bool startingHeartsAreRandom;
    u32 startingHeartsMin;
    u32 startingHeartsMax;
    u32 startingHearts;
    /* RandoStartingHeartsAreContainersOrPieces */ 
    u32 startingHeartsAreContainersOrPieces;
    /*RandoShuffleRegionalMaps */ 
    u32 shuffleRegionalMaps;
    /* RandoShuffleBossRemains */ 
    u32 shuffleBossRemains;
    bool shuffleSpiderHouseRewards;
    /* RandoSkullSanity */ 
    u32 skullSanity;
    /* RandoShopSanity */
    u32 shopSanity;
    bool scrubSanity;
    bool cowSanity;
    bool shuffleCreatFairyRewards;
    bool fairySanity;
    bool startWithConsumables;
    bool permanentChateauRomani;
    bool startWithInvertedTime;
    bool recieveFilledWallets;
    /* RandoDamageMultiplier */ 
    u32 damageMultiplier;
    /* RandoDeathBehavior */ 
    u32 deathBehavior;

} RandoYamlConfig;

typedef struct {
    RecompuiContext context;
    RecompuiResource root;
    RecompuiResource frame;
    RecompuiResource header;
    RecompuiResource body;
} RandoYamlConfigMenu;

extern RandoYamlConfig yaml_config;
extern RandoYamlConfigMenu yaml_config_menu;


void RandoYamlConfig_Init(RandoYamlConfig* config);
RandoYamlConfig* RandoYamlConfig_Create();
void RandoYamlConfig_Destroy(RandoYamlConfig* config);
void randoCreateYamlConfigMenu();

#endif