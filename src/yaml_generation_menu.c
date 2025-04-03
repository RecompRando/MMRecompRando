#include "apcommon.h"
#include "z_title_setup.h"
#include "sys_cfb.h"
#include "idle.h"
#include "recomputils.h"
#include "yaml_generation.h"

void RandoYamlConfig_Init(RandoYamlConfig* config) {
    config->accessability = RANDO_ACCESSABILITY_FULL;
    config->logicDifficulty = RANDO_LOGIC_DIFFICULTY_NORMAL;
    config->chestsMatchContents = true;
    config->startSwordless = false;
    config->startShieldless = false;
    config->startWithSoaring = true;
    config->startingHeartsAreRandom = false;
    config->startingHeartsMin = 4;
    config->startingHeartsMax = 12;
    config->startingHearts = 12;
    config->startingHeartsAreContainersOrPieces = RANDO_STARTING_HEARTS_ARE_CONTAINERS;
    config->shuffleRegionalMaps = RANDO_SHUFFLE_REGIONAL_MAPS_VANILLA;
    config->shuffleBossRemains = RANDO_SHUFFLE_BOSS_REMAINS_VANILLA;
    config->shuffleSpiderHouseRewards = false;
    config->skullSanity = RANDO_SKULLSANITY_VANILLA;
    config->shopSanity = RANDO_SHOPSANITY_VANILLA;
    config->scrubSanity = false;
    config->cowSanity = false;
    config->shuffleCreatFairyRewards = false;
    config->fairySanity = false;
    config->startWithConsumables = false;
    config->permanentChateauRomani = true;
    config->startWithInvertedTime = true;
    config->recieveFilledWallets = true;
    config->damageMultiplier = RANDO_DAMAGE_MULITPLIER_NORMAL;
    config->deathBehavior = RANDO_DEATH_BEHAVIOR_VANILLA;
}

RandoYamlConfig* RandoYamlConfig_Create() {
    RandoYamlConfig* retVal = recomp_alloc(sizeof(RandoYamlConfig));
    RandoYamlConfig_Init(retVal);

    return retVal;
}

void RandoYamlConfig_Destroy(RandoYamlConfig* config) {
    recomp_free(config);
}

RandoYamlConfig yaml_config;
RandoYamlConfigMenu yaml_config_menu;

static UIntRangeController startingHearts = {
    1,
    12,
    &yaml_config.accessability
};

static RecompuiColor test_color = {0, 255, 0, 255}; 

void randoSetToggleButton(RecompuiResource button, u32 value) {
    if (value) {
        recompui_set_text(button, "On");
    } else {
        recompui_set_text(button, "Off");
    }
}

void randoYAMLToggleButtonCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    u32* config_option = userdata;
    if (data->type == UI_EVENT_CLICK) {
        *config_option = !*config_option;
        randoSetToggleButton(button, *config_option);
        // recomp_printf("Toggle = %i\n", *config_option);
    }
}

RecompuiResource randoCreateToggleButtonOption(RecompuiContext context, RecompuiResource parent, char* display_name, u32* config_option) {
    RecompuiResource button_area = recompui_create_element(context, parent);

    // recompui_set_background_color(button_area, &test_color);
    recompui_set_width(button_area, 100.0f, UNIT_PERCENT);
    recompui_set_height(button_area, 60.0f, UNIT_PX);
    recompui_set_display(button_area, DISPLAY_BLOCK);
    // recompui_set_justify_content(button_area, JUSTIFY_CONTENT_SPACE_EVENLY);
    // recompui_set_flex_direction(button_area, FLEX_DIRECTION_ROW);
    // recompui_set_padding(button_area, 16.0f, UNIT_DP);
    // recompui_set_gap(button_area, 80.0f, UNIT_DP);
    recompui_set_align_items(button_area, ALIGN_ITEMS_BASELINE);

    RecompuiResource button = recompui_create_button(context, button_area, display_name, BUTTONSTYLE_PRIMARY);
    recompui_set_display(button, DISPLAY_INLINE);
    recompui_set_text_align(button, TEXT_ALIGN_CENTER);
    recompui_register_callback(button, randoYAMLToggleButtonCallback, config_option);
    randoSetToggleButton(button, *config_option);

    RecompuiResource label = recompui_create_label(context, button_area, display_name, LABELSTYLE_NORMAL);
    recompui_set_display(label, DISPLAY_INLINE);
    // recompui_set_text_align(label, TEXT_ALIGN_CENTER);

    recomp_printf("%s button created.\n", display_name);
    return button;
}



void randoCreateYamlConfigMenu() {
    RandoYamlConfig_Init(&yaml_config);

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

    yaml_config_menu.context = recompui_create_context();
    recompui_open_context(yaml_config_menu.context);

    yaml_config_menu.root = recompui_context_root(yaml_config_menu.context);
    yaml_config_menu.container = recompui_create_element(yaml_config_menu.context, yaml_config_menu.root);
    recompui_set_display(yaml_config_menu.container, DISPLAY_BLOCK);
    recompui_set_overflow_y(yaml_config_menu.container, OVERFLOW_AUTO);

    // Take up the full height and full width, up to a maximum width.
    recompui_set_height(yaml_config_menu.container, 100.0f, UNIT_PERCENT);
    recompui_set_width(yaml_config_menu.container, 100.0f, UNIT_PERCENT);

    RecompuiResource header = recompui_create_label(yaml_config_menu.context, yaml_config_menu.container, "Randomizer: Configure", LABELSTYLE_LARGE);
    recompui_set_padding(header, 40.0f, UNIT_DP);
    recompui_set_gap(header, 40.0f, UNIT_DP);
    recompui_set_align_items(header, ALIGN_ITEMS_BASELINE);

    // yaml_config_menu.wrapper = recompui_create_element(yaml_config_menu.context, yaml_config_menu.container);

    // Set up the container to be the modal's background.
    recompui_set_border_width(yaml_config_menu.container, modal_border_width, UNIT_DP);
    recompui_set_border_radius(yaml_config_menu.container, modal_border_radius, UNIT_DP);
    recompui_set_border_color(yaml_config_menu.container, &border_color);
    recompui_set_background_color(yaml_config_menu.container, &modal_color);

    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "Chests Match Contents", &(yaml_config.chestsMatchContents));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "Start Swordless", &(yaml_config.startSwordless));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "Start Shieldless", &(yaml_config.startShieldless));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "Start with Song of Soaring", &(yaml_config.startWithSoaring));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "Starting Hearts are Random", &(yaml_config.startingHeartsAreRandom));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "Shuffle Spiderhouse Rewards", &(yaml_config.shuffleSpiderHouseRewards));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "ScrubSanity", &(yaml_config.scrubSanity));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "CowSanity", &(yaml_config.cowSanity));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "Shuffle Great Fairy Rewards", &(yaml_config.shuffleCreatFairyRewards));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "FairySanity", &(yaml_config.fairySanity));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "startWithConsumables", &(yaml_config.startWithConsumables));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "Permanent Chateau Romani", &(yaml_config.permanentChateauRomani));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "Start With Inverted Time", &(yaml_config.startWithInvertedTime));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.container, "Recieve Filled Wallets", &(yaml_config.recieveFilledWallets));
    
    recompui_close_context(yaml_config_menu.context);
}