#ifndef __YAML_GEN__
#define __YAML_GEN__

#include "apcommon.h"
#include "recompui.h"
#include "menu_helpers.h"

#include "modding.h"
RECOMP_IMPORT(".", void rando_yaml_init());
RECOMP_IMPORT(".", void rando_yaml_puts(const char* text, u32 size));
RECOMP_IMPORT(".", void rando_yaml_finalize(const unsigned char* save_path));

#define MAX_OPTIONS 64

typedef enum {
    OPTION_BOOL,
    OPTION_RADIO,
    OPTION_INT_SLIDER,
    OPTION_FLOAT_SLIDER
} RandoOptionType;

typedef struct {
    RandoOptionType type;
    RecompuiResource root_element;
    RecompuiResource input_element;
    void* data;
    char* option_id;
} RandoOptionData;

typedef struct {
    const char* id;
    const char* name;
} EnumOptionValue;

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
    RecompuiContext context;
    UiFrame frame;
    RecompuiResource header;
    RecompuiResource header_label;
    RecompuiResource generate_button;
    RecompuiResource body;
    RandoOptionData all_options[MAX_OPTIONS];
    u32 num_options;
    RecompuiResource back_button;
} RandoYamlConfigMenu;

extern RandoYamlConfigMenu yaml_config_menu;

#endif
