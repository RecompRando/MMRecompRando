#include "apcommon.h"
#include "z_title_setup.h"
#include "sys_cfb.h"
#include "idle.h"
#include "recomputils.h"
#include "yaml_generation.h"
#include "recompconfig.h"
#include "libc/string.h"

bool is_generate_menu_shown = false;

bool randoGenerateMenuOpen() {
    return is_generate_menu_shown;
}

RandoYamlConfigMenu yaml_config_menu;

static void backPressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        recompui_hide_context(yaml_config_menu.context);
        is_generate_menu_shown = false;
        // Close the start menu context temporarily so that the solo context can be opened.
        recompui_close_context(yaml_config_menu.context);
        randoShowSoloMenu();
        // Reopen the start menu context.
        recompui_open_context(yaml_config_menu.context);
    }
}

RandoOptionData* randoAllocateOption(RandoYamlConfigMenu* menu, const char* option_id) {
    // Get a new option data element and increate the option count.
    RandoOptionData* ret = &menu->all_options[menu->num_options++];

    // Check if the max number of options has been reached and error out if so.
    if (menu->num_options >= MAX_OPTIONS) {
        recomp_printf("Max rando options reached, increase the MAX_OPTIONS field in \"yaml_generation.h\" to use more options\n");
        *(volatile int*)0 = 0;
        return NULL;
    }

    // Copy the option_id into the returned option.
    size_t option_id_len = strlen(option_id);
    ret->option_id = recomp_alloc(option_id_len + 1);
    ret->is_callback = false;
    Lib_MemCpy(ret->option_id, (void*)option_id, option_id_len + 1);

    return ret;
}

void* rando_yaml_printf_pfn(void* dst, const char* fmt, size_t size) {
    (void)dst;
    rando_yaml_puts(fmt, size);
    return (void*)1;
}

RECOMP_EXPORT int rando_yaml_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int ret = _Printf(&rando_yaml_printf_pfn, NULL, fmt, args);

    va_end(args);

    return ret;
}


void randoYAMLGenerateCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    RandoYamlConfigMenu* menu = (RandoYamlConfigMenu*)userdata;
    if (data->type == UI_EVENT_CLICK) {
        rando_yaml_init();
        rando_yaml_printf("name: Player\n");
        rando_yaml_printf("game: Majora's Mask Recompiled\n");
        rando_yaml_printf("requires:\n  version: %s\n", "0.4.5"); // TODO hook this up to the mod version?
        rando_yaml_printf("Majora's Mask Recompiled:\n");

        // Iterate over the options and write their values into the yaml.
        for (u32 i = 0; i < menu->num_options; i++) {
            RandoOptionData* option = &menu->all_options[i];
            if (!option->is_callback) {
                switch (option->type) {
                    case OPTION_BOOL:
                        rando_yaml_printf("  %s:\n    '%s': 1\n", option->option_id, recompui_get_input_value_u32(option->input_element) ? "true" : "false");
                        break;
                    case OPTION_RADIO:
                        {
                            EnumOptionValue* enum_options = (EnumOptionValue*)option->data;
                            rando_yaml_printf("  %s:\n    %s: 1\n", option->option_id, enum_options[recompui_get_input_value_u32(option->input_element)].id);
                        }
                        break;
                    case OPTION_INT_SLIDER:
                        // TODO replace this with recompui_get_input_value_s32 when it gets added
                        rando_yaml_printf("  %s:\n    %d: 1\n", option->option_id, (s32)recompui_get_input_value_float(option->input_element));
                        break;
                    case OPTION_FLOAT_SLIDER:
                        rando_yaml_printf("  %s:\n    %f: 1\n", option->option_id, recompui_get_input_value_float(option->input_element));
                        break;
                }
            }
            else {
                switch (option->type) {
                    case OPTION_BOOL:
                        option->bool_callback((bool)recompui_get_input_value_u32(option->input_element));
                        break;
                    case OPTION_RADIO:
                        option->bool_callback(recompui_get_input_value_u32(option->input_element));
                        break;
                    case OPTION_INT_SLIDER:
                        // TODO replace this with recompui_get_input_value_s32 when it gets added
                        option->int_callback((s32)recompui_get_input_value_float(option->input_element));
                        break;
                    case OPTION_FLOAT_SLIDER:
                        option->float_callback(recompui_get_input_value_float(option->input_element));
                        break;
                }
            }
        }

        unsigned char* save_path = recomp_get_save_file_path();
        rando_yaml_finalize(save_path);
        recomp_free(save_path);

        if (rando_solo_generate()) {
            recompui_hide_context(yaml_config_menu.context);
            is_generate_menu_shown = false;
            // Close the start menu context temporarily so that the solo context can be opened.
            recompui_close_context(yaml_config_menu.context);
            randoShowSoloMenu();
            // Reopen the start menu context.
            recompui_open_context(yaml_config_menu.context);
        }
        else {
            recompui_close_context(yaml_config_menu.context);
            randoEmitErrorNotification("Failed to generate. Please report the settings you used to the developers.");
            recompui_open_context(yaml_config_menu.context);
        }
    }
}

RecompuiResource randoYAMLCreateMenuEntryArea (RecompuiContext context, RecompuiResource parent) {
    RecompuiResource retVal = recompui_create_element(context, parent);

    recompui_set_display(retVal, DISPLAY_BLOCK);
    recompui_set_justify_content(retVal, JUSTIFY_CONTENT_SPACE_EVENLY);
    recompui_set_flex_direction(retVal, FLEX_DIRECTION_ROW);
    recompui_set_padding(retVal, 12.0f, UNIT_DP);
    recompui_set_gap(retVal, 20.0f, UNIT_DP);
    recompui_set_align_items(retVal, ALIGN_ITEMS_BASELINE);

    return retVal;
}

RecompuiResource randoYAMLCreateSettingLabel(RecompuiContext context, RecompuiResource parent, const char* display_name) {
    RecompuiResource label = recompui_create_label(context, parent, display_name, LABELSTYLE_NORMAL);
    recompui_set_display(label, DISPLAY_BLOCK);
    recompui_set_margin_bottom(label, 12.0f, UNIT_DP);
    return label;
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
    RandoOptionData* option_data = userdata;
    if (data->type == UI_EVENT_CLICK) {
        option_data->data = (void*)(!(u32)option_data->data);
        randoSetToggleButton(button, (u32)option_data->data);
    }
}

// Not currently implemented
// RandoOptionData* randoCreateToggleButtonOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name, u32* config_option) {
//     RecompuiResource button_area = randoYAMLCreateMenuEntryArea(menu->context, menu->body);

//     randoYAMLCreateSettingLabel(menu->context, button_area, display_name);

//     RandoOptionData* option_data = randoAllocateOption(menu, option_id);

//     RecompuiResource button = recompui_create_button(menu->context, button_area, display_name, BUTTONSTYLE_SECONDARY);
//     recompui_set_display(button, DISPLAY_BLOCK);
//     // recompui_set_text_align(button, TEXT_ALIGN_CENTER);
//     recompui_register_callback(button, randoYAMLToggleButtonCallback, option_data);
//     recompui_set_padding(button, 8.0f, UNIT_DP);
//     recompui_set_max_width(button, 600.0f, UNIT_DP);
//     randoSetToggleButton(button, *config_option);

//     recomp_printf("%s button created.\n", display_name);
    
//     return option_data;
// }

static int toupper(int ch) {
    if (ch >= 'a' && ch <= 'z') {
        return ch - 'a' + 'A';
    }
    return ch;
}

char* randoFormatOptionName(EnumOptionValue* option) {
    // If the option has an explicit name, copy it as-is.
    if (option->name != NULL) {
        u32 len = strlen(option->name) + 1;
        char* ret = recomp_alloc(len);
        Lib_MemCpy(ret, (void*)option->name, len);
        return ret;
    }
    // Otherwise, create a name based on the formatted id.
    else {
        u32 len = strlen(option->id) + 1;
        char* ret = recomp_alloc(len);
        // Format the id, capitalizing the first letter of each word and replacing underscores with spaces.
        bool capitalize_next = true;
        for (u32 i = 0; i < len; i++) {
            char cur = option->id[i];
            // Capitalize the letter if needed.
            if (capitalize_next) {
                cur = toupper(cur);
                capitalize_next = false;
            }
            // Convert underscores to spaces and mark the next letter for capitalization.
            if (option->id[i] == '_') {
                cur = ' ';
                capitalize_next = true;
            }
            ret[i] = cur;
        }
        return ret;
    }
}

RandoOptionData* randoCreateRadioOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name,
    EnumOptionValue* options, unsigned long num_options, u32 default_value) {
    RecompuiResource radio_area = randoYAMLCreateMenuEntryArea(menu->context, menu->body);

    randoYAMLCreateSettingLabel(menu->context, radio_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id);

    // Copy the and format the options into an array to pass as the option list for the radio.
    char** option_names = recomp_alloc(sizeof(char*) * num_options);
    for (u32 i = 0; i < num_options; i++) {
        option_names[i] = randoFormatOptionName(&options[i]);
    }

    RecompuiResource radio = recompui_create_labelradio(menu->context, radio_area, (const char**) option_names, num_options);

    // Free the option list.
    for (u32 i = 0; i < num_options; i++) {
        recomp_free(option_names[i]);
    }
    recomp_free(option_names);

    recompui_set_input_value_u32(radio, default_value);

    option_data->type = OPTION_RADIO;
    option_data->root_element = radio_area;
    option_data->input_element = radio;
    option_data->data = options;
    return option_data;
}

static char* rando_bool_prop_names[] = {"Off", "On"};
RandoOptionData* randoCreateBoolPropOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name, bool default_value) {
    RecompuiResource radio_area = randoYAMLCreateMenuEntryArea(menu->context, menu->body);

    randoYAMLCreateSettingLabel(menu->context, radio_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id);

    RecompuiResource radio = recompui_create_labelradio(menu->context, radio_area, (const char**) rando_bool_prop_names, 2);
    recompui_set_input_value_u32(radio, (u32)default_value);

    option_data->type = OPTION_BOOL;
    option_data->root_element = radio_area;
    option_data->input_element = radio;
    return option_data;
}

RandoOptionData* randoCreateCallbackBoolPropOption(RandoYamlConfigMenu* menu, const char* display_name, bool default_value, bool_callback_t* callback) {
    RandoOptionData* option = randoCreateBoolPropOption(menu, "", display_name, default_value);
    option->is_callback = true;
    option->bool_callback = callback;
    return option;
}

// Slider
void randoYAMLSliderCallback(RecompuiResource labelenum, const RecompuiEventData* data, void* userdata) {
    u32* config_option = userdata;
    if (data->type == UI_EVENT_CLICK) {
        *config_option = (u32)recompui_get_input_value_float(labelenum);
    }
}

RandoOptionData* randoCreateIntSliderOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name,
    s32 min, s32 max, s32 step, s32 default_value) {
    RecompuiResource slider_area = randoYAMLCreateMenuEntryArea(menu->context, menu->body);

    randoYAMLCreateSettingLabel(menu->context, slider_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id);

    RecompuiResource slider = recompui_create_slider(menu->context, slider_area, SLIDERTYPE_INTEGER, (float)min, (float)max, (float)step, (float)default_value);
    recompui_set_max_width(slider, 600.0f, UNIT_DP);

    option_data->type = OPTION_INT_SLIDER;
    option_data->root_element = slider_area;
    option_data->input_element = slider;
    return option_data;
}

RandoOptionData* randoCreateFloatSliderOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name,
    float min, float max, float step, float default_value) {
    RecompuiResource slider_area = randoYAMLCreateMenuEntryArea(menu->context, menu->body);

    randoYAMLCreateSettingLabel(menu->context, slider_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id);

    RecompuiResource slider = recompui_create_slider(menu->context, slider_area, SLIDERTYPE_NUMBER, min, max, step, default_value);
    recompui_set_max_width(slider, 600.0f, UNIT_DP);

    option_data->type = OPTION_FLOAT_SLIDER;
    option_data->root_element = slider_area;
    option_data->input_element = slider;
    return option_data;
}

void tunicColorCallback(bool enabled) {
    if (enabled) {
        u32 link_color = Rand_Next();
        u32 link_red = (link_color >> 24) & 0xFF;
        u32 link_green = (link_color >> 16) & 0xFF;
        u32 link_blue = (link_color >> 8) & 0xFF;
        rando_yaml_printf("  link_tunic_color:\n    [%d, %d, %d]\n", link_red, link_green, link_blue);
    }
}

static EnumOptionValue rando_accessibility_options[] = {
    { "locations", "Full" },
    { "minimal", NULL }
};

static EnumOptionValue rando_logic_difficulty_options[] = {
    // { "easy", NULL },
    { "normal", NULL },
    // { "obscure_glitchless", NULL },
    // { "glitched", NULL },
    { "no_logic", NULL },
};

static EnumOptionValue rando_starting_hearts_type_options[] = {
    { "containers", NULL },
    { "pieces", NULL },
};

static EnumOptionValue rando_shuffle_regional_maps_options[] = {
    { "vanilla", NULL },
    { "starting", NULL },
    { "anywhere", NULL },
};

static EnumOptionValue rando_shuffle_boss_remains_options[] = {
    { "vanilla", NULL },
    { "anywhere", NULL },
    { "bosses", NULL },
};

static EnumOptionValue shop_prices_options[] = {
    { "vanilla", NULL },
    { "free", NULL },
    { "cheap", NULL },
    { "expensive", NULL },
    { "offensive", NULL },
};

static EnumOptionValue rando_shuffle_spiderhouse_reward_options[] ={
    { "disabled", NULL },
    { "vanilla", NULL },
    { "enabled", NULL},
};

static EnumOptionValue rando_skullsanity_options[] = {
    { "vanilla", NULL },
    { "anything", NULL },
    { "ignore", NULL },
};

static EnumOptionValue rando_shuffle_great_fairy_rewards_options[] ={
    { "disabled", NULL },
    { "vanilla", NULL },
    { "enabled", NULL},
};

static EnumOptionValue rando_shopsanity_options[] = {
    { "vanilla", NULL },
    { "enabled", NULL },
    { "advanced", NULL },
};

static EnumOptionValue treasure_chest_game_shuffle_options[] = {
    { "disabled", NULL },
    { "goron_only", NULL },
    { "everything", NULL },
};

static EnumOptionValue rando_damage_multiplier_options[] = {
    { "half", NULL },
    { "normal", NULL },
    { "double", NULL },
    { "quad", NULL },
    { "ohko", "One-Hit KO" },
};

static EnumOptionValue rando_death_behavior_options[] = {
    { "vanilla", NULL }, 
    { "fast", NULL },
    { "instant", NULL },
    { "moon_crash", NULL },
};

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

    yaml_config_menu.context = recompui_create_context();
    recompui_open_context(yaml_config_menu.context);

    createUiFrame(yaml_config_menu.context, &yaml_config_menu.frame);

    // Adjust the container's properties.
    recompui_set_width(yaml_config_menu.frame.container, 1200.0f, UNIT_DP);
    recompui_set_height(yaml_config_menu.frame.container, 800.0f, UNIT_DP);
    recompui_set_display(yaml_config_menu.frame.container, DISPLAY_FLEX);
    recompui_set_flex_direction(yaml_config_menu.frame.container, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(yaml_config_menu.frame.container, ALIGN_ITEMS_STRETCH);
    recompui_set_justify_content(yaml_config_menu.frame.container, JUSTIFY_CONTENT_FLEX_START);

    // Remove the padding on the frame's container so that the divider line has the full width of the container.
    recompui_set_padding(yaml_config_menu.frame.container, 0.0f, UNIT_DP);

    // Create the header.
    yaml_config_menu.header = recompui_create_element(yaml_config_menu.context, yaml_config_menu.frame.container);
    recompui_set_flex_grow(yaml_config_menu.header, 0.0f);
    recompui_set_flex_shrink(yaml_config_menu.header, 0.0f);
    recompui_set_display(yaml_config_menu.header, DISPLAY_FLEX);
    recompui_set_flex_direction(yaml_config_menu.header, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(yaml_config_menu.header, JUSTIFY_CONTENT_SPACE_BETWEEN);
    recompui_set_border_bottom_width(yaml_config_menu.header, 1.1f, UNIT_DP);
    recompui_set_height_auto(yaml_config_menu.header);
    recompui_set_padding(yaml_config_menu.header, 16.0f, UNIT_DP);
    recompui_set_align_items(yaml_config_menu.header, ALIGN_ITEMS_CENTER);

    RecompuiColor divider_color;
    divider_color.r = 255;
    divider_color.g = 255;
    divider_color.b = 255;
    divider_color.a = 25;
    recompui_set_border_bottom_color(yaml_config_menu.header, &divider_color);

    yaml_config_menu.header_label = recompui_create_label(yaml_config_menu.context, yaml_config_menu.header, "Randomizer Settings", LABELSTYLE_LARGE);
    recompui_set_text_align(yaml_config_menu.header_label, TEXT_ALIGN_CENTER);
    recompui_set_margin_left(yaml_config_menu.header_label, 32.0f, UNIT_DP);

    yaml_config_menu.generate_button = recompui_create_button(yaml_config_menu.context, yaml_config_menu.header, "Generate", BUTTONSTYLE_PRIMARY);
    recompui_register_callback(yaml_config_menu.generate_button, randoYAMLGenerateCallback, &yaml_config_menu);

    // Create the body.
    yaml_config_menu.body = recompui_create_element(yaml_config_menu.context, yaml_config_menu.frame.container);
    recompui_set_display(yaml_config_menu.body, DISPLAY_BLOCK);
    recompui_set_overflow_y(yaml_config_menu.body, OVERFLOW_SCROLL);

    yaml_config_menu.num_options = 0;
    randoCreateRadioOption(&yaml_config_menu, "accessibility", "Accessibility:", rando_accessibility_options, ARRAY_COUNT(rando_accessibility_options), RANDO_ACCESSABILITY_FULL);
    randoCreateRadioOption(&yaml_config_menu, "logic_difficulty", "Logic Difficulty:", rando_logic_difficulty_options, ARRAY_COUNT(rando_logic_difficulty_options), RANDO_LOGIC_DIFFICULTY_NORMAL);
    randoCreateIntSliderOption(&yaml_config_menu, "moon_remains_required", "Moon Boss Remains Required:", 0, 4, 1, 4);
    randoCreateIntSliderOption(&yaml_config_menu, "majora_remains_required", "Majora Boss Remains Required:", 0, 4, 1, 4);
    randoCreateBoolPropOption(&yaml_config_menu, "camc", "Chests Match Contents:", true);
    randoCreateBoolPropOption(&yaml_config_menu, "swordless", "Start Swordless:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "shieldless", "Start Shieldless:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "start_with_soaring", "Start with Song of Soaring:", true);
    randoCreateIntSliderOption(&yaml_config_menu, "starting_hearts", "Starting Heart Segments:", 4, 12, 1, 12);
    randoCreateRadioOption(&yaml_config_menu, "starting_hearts_are_containers_or_pieces", "Unused Starting Hearts are Distributed as:", rando_starting_hearts_type_options, ARRAY_COUNT(rando_starting_hearts_type_options), RANDO_STARTING_HEARTS_ARE_CONTAINERS);
    randoCreateRadioOption(&yaml_config_menu, "shuffle_regional_maps", "Shuffle Regional Maps:", rando_shuffle_regional_maps_options, ARRAY_COUNT(rando_shuffle_regional_maps_options), RANDO_SHUFFLE_REGIONAL_MAPS_VANILLA);
    randoCreateRadioOption(&yaml_config_menu, "shuffle_boss_remains", "Shuffle Boss Remains:", rando_shuffle_boss_remains_options, ARRAY_COUNT(rando_shuffle_boss_remains_options), RANDO_SHUFFLE_BOSS_REMAINS_VANILLA);
    randoCreateRadioOption(&yaml_config_menu, "shuffle_spiderhouse_reward", "Shuffle Spiderhouse Rewards:", rando_shuffle_spiderhouse_reward_options, ARRAY_COUNT(rando_shuffle_spiderhouse_reward_options), 0);
    randoCreateIntSliderOption(&yaml_config_menu, "required_skull_tokens", "Required Skulltula Tokens:", 0, 30, 1, 30);
    randoCreateRadioOption(&yaml_config_menu, "skullsanity", "Skull-Sanity Mode:", rando_skullsanity_options, ARRAY_COUNT(rando_skullsanity_options), RANDO_SKULLSANITY_VANILLA);
    randoCreateRadioOption(&yaml_config_menu, "shuffle_great_fairy_rewards", "Shuffle Great Fairy Rewards:", rando_shuffle_great_fairy_rewards_options, ARRAY_COUNT(rando_shuffle_great_fairy_rewards_options), 0);
    randoCreateIntSliderOption(&yaml_config_menu, "required_stray_fairies", "Required Stray Fairies:", 0, 15, 1, 15);
    randoCreateBoolPropOption(&yaml_config_menu, "fairysanity", "Fairy-Sanity:", false);
    randoCreateRadioOption(&yaml_config_menu, "shop_prices", "Shop Prices:", shop_prices_options, ARRAY_COUNT(shop_prices_options), RANDO_SHOP_PRICES_VANILLA);
    randoCreateRadioOption(&yaml_config_menu, "shopsanity", "Shop-Sanity Mode:", rando_shopsanity_options, ARRAY_COUNT(rando_shopsanity_options), RANDO_SHOPSANITY_VANILLA);
    randoCreateBoolPropOption(&yaml_config_menu, "scrubsanity", "Scrub-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "cowsanity", "Cow-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "keysanity", "Key-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "bosskeysanity", "Boss-Key-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "curiostity_shop_trades", "Curiosity Shop Trades:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "intro_checks", "Intro Checks:", false);
    randoCreateRadioOption(&yaml_config_menu, "shuffle_treasure_chest_game", "Treasure Chest Game Shuffle:", treasure_chest_game_shuffle_options, ARRAY_COUNT(treasure_chest_game_shuffle_options), 1);
    randoCreateBoolPropOption(&yaml_config_menu, "start_with_consumables", "Start With Consumables:", true);
    randoCreateBoolPropOption(&yaml_config_menu, "permanent_chateau_romani", "Permanent Chateau Romani:", true);
    randoCreateBoolPropOption(&yaml_config_menu, "start_with_inverted_time", "Start With Inverted Time:", true);
    randoCreateBoolPropOption(&yaml_config_menu, "receive_filled_wallets", "Receive Filled Wallets:", true);
    randoCreateRadioOption(&yaml_config_menu, "damage_multiplier", "Damage Multiplier:", rando_damage_multiplier_options, ARRAY_COUNT(rando_damage_multiplier_options), RANDO_DAMAGE_MULITPLIER_NORMAL);
    randoCreateRadioOption(&yaml_config_menu, "death_behavior", "Death Behavior:", rando_death_behavior_options, ARRAY_COUNT(rando_death_behavior_options), RANDO_DEATH_BEHAVIOR_VANILLA);
    randoCreateBoolPropOption(&yaml_config_menu, "magic_is_a_trap", "Magic is a Trap:", false);
    randoCreateCallbackBoolPropOption(&yaml_config_menu, "Randomize Tunic Color:", true, tunicColorCallback);

    // Create the back button, parenting it to the root with absolute positioning.
    yaml_config_menu.back_button = recompui_create_button(yaml_config_menu.context, yaml_config_menu.frame.root, "Back", BUTTONSTYLE_SECONDARY);
    recompui_set_position(yaml_config_menu.back_button, POSITION_ABSOLUTE);
    recompui_set_left(yaml_config_menu.back_button, 64.0f, UNIT_DP);
    recompui_set_top(yaml_config_menu.back_button, 32.0f, UNIT_DP);
    recompui_register_callback(yaml_config_menu.back_button, backPressed, NULL);

    recompui_set_nav(yaml_config_menu.generate_button, NAVDIRECTION_DOWN, yaml_config_menu.all_options[0].input_element);
    recompui_set_nav(yaml_config_menu.all_options[0].input_element, NAVDIRECTION_UP, yaml_config_menu.generate_button);

    recompui_close_context(yaml_config_menu.context);
}

void randoShowYamlConfigMenu() {
    recompui_show_context(yaml_config_menu.context);
    is_generate_menu_shown = true;
}