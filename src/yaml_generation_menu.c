#include "apcommon.h"
#include "z_title_setup.h"
#include "sys_cfb.h"
#include "idle.h"
#include "recomputils.h"
#include "yaml_generation.h"

RandoYamlConfig* RandoYamlConfig_Create() {
    RandoYamlConfig* retVal = recomp_alloc(sizeof(RandoYamlConfig));
    retVal->accessability = RANDO_ACCESSABILITY_FULL;
    retVal->logicDifficulty = RANDO_LOGIC_DIFFICULTY_NORMAL;
    retVal->chestsMatchContents = true;
    retVal->startSwordless = false;
    retVal->startShieldless = false;
    retVal->startWithSoaring = true;
    retVal->startingHeartsAreRandom = false;
    retVal->startingHeartsMin = 4;
    retVal->startingHeartsMax = 12;
    retVal->startingHearts = 12;
    retVal->startingHeartsAreContainersOrPieces = RANDO_STARTING_HEARTS_ARE_CONTAINERS;
    retVal->shuffleRegionalMaps = RANDO_SHUFFLE_REGIONAL_MAPS_VANILLA;
    retVal->shuffleBossRemains = RANDO_SHUFFLE_BOSS_REMAINS_VANILLA;
    retVal->shuffleSpiderHouseRewards = false;
    retVal->skullSanity = RANDO_SKULLSANITY_VANILLA;
    retVal->shopSanity = RANDO_SHOPSANITY_VANILLA;
    retVal->scrubSanity = false;
    retVal->cowSanity = false;
    retVal->shuffleCreatFairyRewards = false;
    retVal->fairySanity = false;
    retVal->startWithConsumables = false;
    retVal->permanentChateauRomani = true;
    retVal->startWithInvertedTime = true;
    retVal->recieveFilledWallets = true;
    retVal->damageMultiplier = RANDO_DAMAGE_MULITPLIER_NORMAL;
    retVal->deathBehavior = RANDO_DEATH_BEHAVIOR_VANILLA;

    return retVal;
}

void RandoYamlConfig_Destroy(RandoYamlConfig* config) {
    recomp_free(config);
}

RecompuiContext yaml_menu_context;
RecompuiResource yaml_menu_root;
RecompuiResource yaml_menu_container;

void randoCreateYamlConfigMenu() {
    RecompuiColor bg_color;
    bg_color.r = 255;
    bg_color.g = 255;
    bg_color.b = 255;
    bg_color.a = 0.5f * 255;

    RecompuiColor border_color;
    border_color.r = 255;
    border_color.g = 255;
    border_color.b = 255;
    border_color.a = 0.7f * 255;

    RecompuiColor modal_color;
    modal_color.r = 8;
    modal_color.g = 7;
    modal_color.b = 13;
    modal_color.a = 0.9f * 255;

    const float body_padding = 64.0f;
    const float modal_height = RECOMPUI_TOTAL_HEIGHT - (2 * body_padding);
    const float modal_max_width = modal_height * (16.0f / 9.0f);
    const float modal_border_width = 5.1f;
    const float modal_border_radius = 16.0f;

    yaml_menu_context = recompui_create_context();
    recompui_open_context(yaml_menu_context);

    yaml_menu_root = recompui_context_root(yaml_menu_context);
    yaml_menu_container = recompui_create_element(yaml_menu_context, yaml_menu_root);

    // Take up the full height and full width, up to a maximum width.
    recompui_set_height(yaml_menu_container, 96.5f, UNIT_PERCENT);
    recompui_set_width(yaml_menu_container, 90.0f, UNIT_PERCENT);
    recompui_set_flex_grow(yaml_menu_container, 1.0f);
    // recompui_set_max_width(container, modal_max_width, UNIT_DP);
    // Set up the flexbox properties of the container.
    recompui_set_display(yaml_menu_container, DISPLAY_FLEX);
    recompui_set_justify_content(yaml_menu_container, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_flex_direction(yaml_menu_container, FLEX_DIRECTION_ROW);
    recompui_set_padding(yaml_menu_container, 16.0f, UNIT_DP);
    recompui_set_gap(yaml_menu_container, 16.0f, UNIT_DP);
    recompui_set_align_items(yaml_menu_container, ALIGN_ITEMS_BASELINE);

    // Set up the container to be the modal's background.
    recompui_set_border_width(yaml_menu_container, modal_border_width, UNIT_DP);
    recompui_set_border_radius(yaml_menu_container, modal_border_radius, UNIT_DP);
    recompui_set_border_color(yaml_menu_container, &border_color);
    recompui_set_background_color(yaml_menu_container, &modal_color);

    recompui_close_context(yaml_menu_context);
}