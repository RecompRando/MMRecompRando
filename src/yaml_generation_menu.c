#include "apcommon.h"
#include "z_title_setup.h"
#include "sys_cfb.h"
#include "idle.h"
#include "recomputils.h"
#include "yaml_generation.h"
#include "recompconfig.h"

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

void randoYAMLGenerateCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    u32* config_option = userdata;
    if (data->type == UI_EVENT_CLICK) {
        char* yaml_str = recomp_get_save_file_path();
        char yaml_text[14] = "HELLO WORLD!\n";
        recomp_printf("GENERATE: %s\n", yaml_str);
        rando_save_run_yaml(yaml_str, 1, yaml_text);
        recomp_free(yaml_str);
    }
}

RecompuiResource randoYAMLCreateMenuEntryArea (RecompuiContext context, RecompuiResource parent) {
    RecompuiResource retVal = recompui_create_element(context, parent);

    recompui_set_display(retVal, DISPLAY_BLOCK);
    recompui_set_justify_content(retVal, JUSTIFY_CONTENT_SPACE_EVENLY);
    recompui_set_flex_direction(retVal, FLEX_DIRECTION_ROW);
    recompui_set_padding(retVal, 20.0f, UNIT_DP);
    recompui_set_gap(retVal, 20.0f, UNIT_DP);
    recompui_set_align_items(retVal, ALIGN_ITEMS_BASELINE);

    return retVal;
}

// Toggle Button Helpers:
static RecompuiColor on_color = {0, 255, 0, 255};
static RecompuiColor off_color = {255, 0, 0, 255};
void randoSetToggleButton(RecompuiResource button, u32 value) {
    if (value) {
        recompui_set_text(button, "On");
        recompui_set_color(button, &on_color);
    } else {
        recompui_set_text(button, "Off");
        recompui_set_color(button, &off_color);
    }
}

void randoYAMLToggleButtonCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    u32* config_option = userdata;
    if (data->type == UI_EVENT_CLICK) {
        *config_option = !*config_option;
        randoSetToggleButton(button, *config_option);
    }
}

RecompuiResource randoCreateToggleButtonOption(RecompuiContext context, RecompuiResource parent, char* display_name, u32* config_option) {
    RecompuiResource button_area = randoYAMLCreateMenuEntryArea(context, parent);

    RecompuiResource label = recompui_create_label(context, button_area, display_name, LABELSTYLE_NORMAL);
    recompui_set_display(label, DISPLAY_BLOCK);
    recompui_set_padding(label, 16.0f, UNIT_DP);

    RecompuiResource button = recompui_create_button(context, button_area, display_name, BUTTONSTYLE_SECONDARY);
    recompui_set_display(button, DISPLAY_BLOCK);
    // recompui_set_text_align(button, TEXT_ALIGN_CENTER);
    recompui_register_callback(button, randoYAMLToggleButtonCallback, config_option);
    recompui_set_padding(button, 8.0f, UNIT_DP);
    recompui_set_max_width(button, 600.0f, UNIT_DP);
    randoSetToggleButton(button, *config_option);

    recomp_printf("%s button created.\n", display_name);
    return button;
}

// Radio Helpers:
void randoYAMLRadioCallback(RecompuiResource labelenum, const RecompuiEventData* data, void* userdata) {
    u32* config_option = userdata;
    if (data->type == UI_EVENT_CLICK) {
        *config_option = recompui_get_input_value_u32(labelenum);
    }
}

RecompuiResource randoCreateRadioOption(RecompuiContext context, RecompuiResource parent, char* display_name,
    char** options, unsigned long num_options, u32* config_option) {
    RecompuiResource radio_area = randoYAMLCreateMenuEntryArea(context, parent);

    RecompuiResource label = recompui_create_label(context, radio_area, display_name, LABELSTYLE_NORMAL);
    recompui_set_display(label, DISPLAY_BLOCK);
    recompui_set_padding_top(label, 16.0f, UNIT_DP);
    recompui_set_padding_bottom(label, 8.0f, UNIT_DP);

    RecompuiResource radio = recompui_create_labelradio(context, radio_area, options, num_options);
    recompui_set_display(radio, DISPLAY_BLOCK);
    // recompui_set_text_align(radio, TEXT_ALIGN_CENTER);
    recompui_register_callback(radio, randoYAMLRadioCallback, config_option);
    recompui_set_padding_left(radio, 60.0f, UNIT_DP);
    recompui_set_padding_bottom(radio, 16.0f, UNIT_DP);
    recompui_set_max_width(radio, 600.0f, UNIT_DP);
    recompui_set_input_value_u32(radio, *config_option);

    recomp_printf("%s radio created.\n", display_name);
    return 0;
}

// Slider
void randoYAMLSliderCallback(RecompuiResource labelenum, const RecompuiEventData* data, void* userdata) {
    u32* config_option = userdata;
    if (data->type == UI_EVENT_CLICK) {
        *config_option = (u32)recompui_get_input_value_float(labelenum);
    }
}

RecompuiResource randoCreateSliderOption(RecompuiContext context, RecompuiResource parent, char* display_name,
    float min, float max, float step, u32* config_option) {
    RecompuiResource slider_area = randoYAMLCreateMenuEntryArea(context, parent);

    RecompuiResource label = recompui_create_label(context, slider_area, display_name, LABELSTYLE_NORMAL);
    recompui_set_display(label, DISPLAY_BLOCK);
    recompui_set_padding_top(label, 16.0f, UNIT_DP);
    recompui_set_padding_bottom(label, 8.0f, UNIT_DP);

    RecompuiResource slider = recompui_create_slider(context, slider_area, SLIDERTYPE_NUMBER, min, max, step, (float)*config_option);

    recompui_set_display(slider, DISPLAY_BLOCK);
    // recompui_set_text_align(radio, TEXT_ALIGN_CENTER);
    recompui_register_callback(slider, randoYAMLSliderCallback, config_option);
    recompui_set_padding_left(slider, 60.0f, UNIT_DP);
    recompui_set_padding_bottom(slider, 16.0f, UNIT_DP);
    recompui_set_max_width(slider, 600.0f, UNIT_DP);


    recomp_printf("%s slider created.\n", display_name);
    return 0;
}


static char* rando_accessability_names[] = {"Full", "Minimal"};
static char* rando_logic_difficulty_names[] = {"Easy", "Normal", "No Logic"};
static char* rando_starting_hearts_type_names[] = {"Containers", "Pieces"};
static char* rando_shuffle_regional_maps_names[] = {"Vanilla", "Starting", "Anywhere"};
static char* rando_shuffle_boss_remains_names[] = {"Vanilla", "Anywhere", "Bosses"};
static char* rando_skullsanity_names[] = {"Off", "Anything", "Ignore"};
static char* rando_shopsanity_names[] = {"Off", "Enabled", "Advanced"};
static char* rando_damage_multiplier_names[] = {"Half", "Normal", "Double", "Quad", "One Hit KO"};
static char* rando_death_behavior_names[] = {"Vanilla", "Fast", "Moon Crash"};

// Adding the sliders to the menu messes with the border and the static header.
// TWe can keep that code, but make it toggleable here, until that gets fixed.
// #define STATIC_MENU_HEADER 

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
    yaml_config_menu.frame = recompui_create_element(yaml_config_menu.context, yaml_config_menu.root);
    recompui_set_display(yaml_config_menu.frame, DISPLAY_BLOCK);
    recompui_set_height(yaml_config_menu.frame, 100.0f, UNIT_PERCENT);
    recompui_set_width(yaml_config_menu.frame, 100.0f, UNIT_PERCENT);

#ifdef STATIC_MENU_HEADER
    yaml_config_menu.header = recompui_create_element(yaml_config_menu.context, yaml_config_menu.frame);
    recompui_set_display(yaml_config_menu.header, DISPLAY_BLOCK);
    recompui_set_overflow(yaml_config_menu.header, OVERFLOW_VISIBLE);
    recompui_set_height(yaml_config_menu.header, 10.0f, UNIT_PERCENT);
    recompui_set_width(yaml_config_menu.header, 100.0f, UNIT_PERCENT);
#endif

    yaml_config_menu.body = recompui_create_element(yaml_config_menu.context, yaml_config_menu.frame);
    recompui_set_display(yaml_config_menu.body, DISPLAY_BLOCK);
    recompui_set_overflow_y(yaml_config_menu.body, OVERFLOW_SCROLL);
    // Take up the full height and full width, up to a maximum width.
    recompui_set_width(yaml_config_menu.body, 100.0f, UNIT_PERCENT);

#ifdef STATIC_MENU_HEADER
    recompui_set_height(yaml_config_menu.body, 90.0f, UNIT_PERCENT);
    RecompuiResource header_label = recompui_create_label(yaml_config_menu.context, yaml_config_menu.header, "Randomizer: Configure", LABELSTYLE_LARGE);
#else
    recompui_set_height(yaml_config_menu.body, 100.0f, UNIT_PERCENT);
    RecompuiResource header_label = recompui_create_label(yaml_config_menu.context, yaml_config_menu.body, "Randomizer: Configure", LABELSTYLE_LARGE);
#endif

    recompui_set_display(header_label, DISPLAY_BLOCK);
    recompui_set_padding(header_label, 40.0f, UNIT_DP);
    recompui_set_gap(header_label, 40.0f, UNIT_DP);
    recompui_set_align_items(header_label, ALIGN_ITEMS_BASELINE);

    RecompuiResource submit_button = recompui_create_button(yaml_config_menu.context, yaml_config_menu.body, "Generate", BUTTONSTYLE_PRIMARY);
    recompui_set_display(submit_button, DISPLAY_BLOCK);
    recompui_set_height(submit_button, 80.0f, UNIT_DP);
    recompui_set_gap(submit_button, 40.0f, UNIT_DP);
    recompui_set_align_items(submit_button, ALIGN_ITEMS_BASELINE);
    recompui_register_callback(submit_button, randoYAMLGenerateCallback, NULL);

    // Set up the container to be the modal's background.
    recompui_set_border_width(yaml_config_menu.frame, modal_border_width, UNIT_DP);
    recompui_set_border_radius(yaml_config_menu.frame, modal_border_radius, UNIT_DP);
    recompui_set_border_color(yaml_config_menu.frame, &border_color);
    recompui_set_background_color(yaml_config_menu.frame, &modal_color);
    randoCreateRadioOption(yaml_config_menu.context, yaml_config_menu.body, "Accessability:", rando_accessability_names, 2, &yaml_config.accessability);
    randoCreateRadioOption(yaml_config_menu.context, yaml_config_menu.body, "Logic Difficulty:", rando_logic_difficulty_names, 3, &yaml_config.logicDifficulty);
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Chests Match Contents:", &(yaml_config.chestsMatchContents));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Start Swordless:", &(yaml_config.startSwordless));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Start Shieldless:", &(yaml_config.startShieldless));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Start with Song of Soaring:", &(yaml_config.startWithSoaring));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Starting Hearts are Random:", &(yaml_config.startingHeartsAreRandom));
    randoCreateSliderOption(yaml_config_menu.context, yaml_config_menu.body, "Random Starting Hearts Segments - Minimum:", 1, 12, 1, &(yaml_config.startingHeartsMin));
    randoCreateSliderOption(yaml_config_menu.context, yaml_config_menu.body, "Random Starting Hearts Segments - Maximum:", 1, 12, 1, &(yaml_config.startingHeartsMax));
    randoCreateSliderOption(yaml_config_menu.context, yaml_config_menu.body, "Non-Random Starting Heart Segments:", 1, 12, 1, &(yaml_config.startingHearts));
    randoCreateRadioOption(yaml_config_menu.context, yaml_config_menu.body, "Unused Starting Hearts are Distributed as:", rando_starting_hearts_type_names, 2, &yaml_config.startingHeartsAreContainersOrPieces);
    randoCreateRadioOption(yaml_config_menu.context, yaml_config_menu.body, "Shuffle Regional Maps:", rando_shuffle_regional_maps_names, 3, &yaml_config.shuffleRegionalMaps);
    randoCreateRadioOption(yaml_config_menu.context, yaml_config_menu.body, "Shuffle Boss Maps:", rando_shuffle_boss_remains_names, 3, &yaml_config.shuffleBossRemains);
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Shuffle Spiderhouse Rewards:", &(yaml_config.shuffleSpiderHouseRewards));
    randoCreateRadioOption(yaml_config_menu.context, yaml_config_menu.body, "Skull-Sanity Mode:", rando_skullsanity_names, 3, &yaml_config.skullSanity);
    randoCreateRadioOption(yaml_config_menu.context, yaml_config_menu.body, "Shop-Sanity Mode:", rando_shopsanity_names, 3, &yaml_config.shopSanity);
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Scrub-Sanity:", &(yaml_config.scrubSanity));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Cow-Sanity:", &(yaml_config.cowSanity));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Shuffle Great Fairy Rewards:", &(yaml_config.shuffleCreatFairyRewards));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "FairySanity:", &(yaml_config.fairySanity));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "startWithConsumables:", &(yaml_config.startWithConsumables));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Permanent Chateau Romani:", &(yaml_config.permanentChateauRomani));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Start With Inverted Time:", &(yaml_config.startWithInvertedTime));
    randoCreateToggleButtonOption(yaml_config_menu.context, yaml_config_menu.body, "Recieve Filled Wallets:", &(yaml_config.recieveFilledWallets));
    randoCreateRadioOption(yaml_config_menu.context, yaml_config_menu.body, "Damage Multiplier:", rando_damage_multiplier_names, 5, &yaml_config.damageMultiplier);
    randoCreateRadioOption(yaml_config_menu.context, yaml_config_menu.body, "Death Behavior:", rando_death_behavior_names, 3, &yaml_config.deathBehavior);
    
    recompui_close_context(yaml_config_menu.context);
}