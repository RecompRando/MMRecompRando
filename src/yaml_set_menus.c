#include "apcommon.h"
#include "z_title_setup.h"
#include "sys_cfb.h"
#include "idle.h"
#include "recomputils.h"
#include "yaml_generation.h"
#include "recompconfig.h"
#include "libc/string.h"

int rando_yaml_printf(const char* fmt, ...);

RandoYamlSetMenu yaml_tricks_menu;
RandoYamlSetMenu yaml_glitches_menu;

static void backPressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    RandoYamlSetMenu* menu = (RandoYamlSetMenu*)userdata;

    if (data->type == UI_EVENT_CLICK) {
        recompui_hide_context(menu->context);
        randoShowYamlConfigMenu();
    }
}

RandoOptionData* randoAllocateSetOption(RandoYamlSetMenu* menu, const char* option_id) {
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

RecompuiResource randoYAMLSetCreateMenuEntryArea (RecompuiContext context, RecompuiResource parent) {
    RecompuiResource retVal = recompui_create_element(context, parent);

    recompui_set_display(retVal, DISPLAY_BLOCK);
    recompui_set_justify_content(retVal, JUSTIFY_CONTENT_SPACE_EVENLY);
    recompui_set_flex_direction(retVal, FLEX_DIRECTION_ROW);
    recompui_set_padding(retVal, 12.0f, UNIT_DP);
    recompui_set_gap(retVal, 20.0f, UNIT_DP);
    recompui_set_align_items(retVal, ALIGN_ITEMS_BASELINE);

    return retVal;
}

RecompuiResource randoYAMLSetCreateSettingLabel(RecompuiContext context, RecompuiResource parent, const char* display_name) {
    RecompuiResource label = recompui_create_label(context, parent, display_name, LABELSTYLE_NORMAL);
    recompui_set_display(label, DISPLAY_BLOCK);
    recompui_set_margin_bottom(label, 12.0f, UNIT_DP);
    return label;
}

static char* rando_bool_prop_names[] = {"Off", "On"};
RandoOptionData* randoCreateSetItem(RandoYamlSetMenu* menu, const char* option_id, const char* display_name) {
    RecompuiResource radio_area = randoYAMLSetCreateMenuEntryArea(menu->context, menu->body);

    randoYAMLSetCreateSettingLabel(menu->context, radio_area, display_name);

    // RandoOptionData* option_data = randoAllocateSetOption(menu, option_id);
    RandoOptionData* option_data = randoAllocateSetOption(menu, display_name); // use display name instead for nicer yamls

    RecompuiResource radio = recompui_create_labelradio(menu->context, radio_area, (const char**) rando_bool_prop_names, 2);
    recompui_set_input_value_u32(radio, (u32)false);

    option_data->type = OPTION_BOOL;
    option_data->root_element = radio_area;
    option_data->input_element = radio;
    return option_data;
}

// tricks

void randoYamlTricksCallback() {
    RandoYamlSetMenu* menu = &yaml_tricks_menu;
    if (menu->num_options == 0) {
        return;
    }

    rando_yaml_printf("  logic_tricks:\n");

    for (u32 i = 0; i < menu->num_options; i++) {
        RandoOptionData* option = &menu->all_options[i];
        rando_yaml_printf("    - %s\n", option->option_id);
    }
}

void randoCreateYamlTricksMenu() {
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

    yaml_tricks_menu.context = recompui_create_context();
    recompui_open_context(yaml_tricks_menu.context);

    createUiFrame(yaml_tricks_menu.context, &yaml_tricks_menu.frame);

    // Adjust the container's properties.
    recompui_set_width(yaml_tricks_menu.frame.container, 1200.0f, UNIT_DP);
    recompui_set_height(yaml_tricks_menu.frame.container, 800.0f, UNIT_DP);
    recompui_set_display(yaml_tricks_menu.frame.container, DISPLAY_FLEX);
    recompui_set_flex_direction(yaml_tricks_menu.frame.container, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(yaml_tricks_menu.frame.container, ALIGN_ITEMS_STRETCH);
    recompui_set_justify_content(yaml_tricks_menu.frame.container, JUSTIFY_CONTENT_FLEX_START);

    // Remove the padding on the frame's container so that the divider line has the full width of the container.
    recompui_set_padding(yaml_tricks_menu.frame.container, 0.0f, UNIT_DP);

    // Create the header.
    yaml_tricks_menu.header = recompui_create_element(yaml_tricks_menu.context, yaml_tricks_menu.frame.container);
    recompui_set_flex_grow(yaml_tricks_menu.header, 0.0f);
    recompui_set_flex_shrink(yaml_tricks_menu.header, 0.0f);
    recompui_set_display(yaml_tricks_menu.header, DISPLAY_FLEX);
    recompui_set_flex_direction(yaml_tricks_menu.header, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(yaml_tricks_menu.header, JUSTIFY_CONTENT_SPACE_BETWEEN);
    recompui_set_border_bottom_width(yaml_tricks_menu.header, 1.1f, UNIT_DP);
    recompui_set_height_auto(yaml_tricks_menu.header);
    recompui_set_padding(yaml_tricks_menu.header, 16.0f, UNIT_DP);
    recompui_set_align_items(yaml_tricks_menu.header, ALIGN_ITEMS_CENTER);

    RecompuiColor divider_color;
    divider_color.r = 255;
    divider_color.g = 255;
    divider_color.b = 255;
    divider_color.a = 25;
    recompui_set_border_bottom_color(yaml_tricks_menu.header, &divider_color);

    yaml_tricks_menu.header_label = recompui_create_label(yaml_tricks_menu.context, yaml_tricks_menu.header, "Randomizer Tricks Menu", LABELSTYLE_LARGE);
    recompui_set_text_align(yaml_tricks_menu.header_label, TEXT_ALIGN_CENTER);
    recompui_set_margin_left(yaml_tricks_menu.header_label, 32.0f, UNIT_DP);

    // Create the body.
    yaml_tricks_menu.body = recompui_create_element(yaml_tricks_menu.context, yaml_tricks_menu.frame.container);
    recompui_set_display(yaml_tricks_menu.body, DISPLAY_BLOCK);
    recompui_set_overflow_y(yaml_tricks_menu.body, OVERFLOW_SCROLL);

    yaml_tricks_menu.num_options = 0;
    randoCreateSetItem(&yaml_tricks_menu, "logic_hard_combat", "Earlygame Hard Combat");
    randoCreateSetItem(&yaml_tricks_menu, "logic_stockpot_night", "Stock Pot Inn at Night with Nothing");
    randoCreateSetItem(&yaml_tricks_menu, "logic_keg_everywhere", "Powder Keg as Explosive");
    randoCreateSetItem(&yaml_tricks_menu, "logic_postman_nobunny", "Postman Game with Nothing");
    randoCreateSetItem(&yaml_tricks_menu, "logic_early_bank", "Bank Checks without Wallets");
    randoCreateSetItem(&yaml_tricks_menu, "logic_goron_swamp_roof", "Goron Pound to Swamp Tour Roof");
    randoCreateSetItem(&yaml_tricks_menu, "logic_talltransform_swamp", "Cross Poisoned Swamp as Goron or Zora");
    randoCreateSetItem(&yaml_tricks_menu, "logic_blind_climb", "Climb Mountain Village Wall with Nothing");
    // randoCreateSetItem(&yaml_tricks_menu, "", "");

    // Create the back button, parenting it to the root with absolute positioning.
    yaml_tricks_menu.back_button = recompui_create_button(yaml_tricks_menu.context, yaml_tricks_menu.frame.root, "Back", BUTTONSTYLE_SECONDARY);
    recompui_set_position(yaml_tricks_menu.back_button, POSITION_ABSOLUTE);
    recompui_set_left(yaml_tricks_menu.back_button, 64.0f, UNIT_DP);
    recompui_set_top(yaml_tricks_menu.back_button, 32.0f, UNIT_DP);
    recompui_register_callback(yaml_tricks_menu.back_button, backPressed, &yaml_tricks_menu);

    recompui_set_nav(yaml_tricks_menu.back_button, NAVDIRECTION_DOWN, yaml_tricks_menu.all_options[0].input_element);
    recompui_set_nav(yaml_tricks_menu.all_options[0].input_element, NAVDIRECTION_UP, yaml_tricks_menu.back_button);

    recompui_close_context(yaml_tricks_menu.context);
}

void randoShowYamlTricksMenu() {
    recompui_show_context(yaml_tricks_menu.context);
}

// glitches

void randoYamlGlitchesCallback() {
    RandoYamlSetMenu* menu = &yaml_glitches_menu;
    if (menu->num_options == 0) {
        return;
    }

    rando_yaml_printf("  logic_glitches:\n");

    for (u32 i = 0; i < menu->num_options; i++) {
        RandoOptionData* option = &menu->all_options[i];
        rando_yaml_printf("    - %s\n", option->option_id);
    }
}

void randoCreateYamlGlitchesMenu() {
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

    yaml_glitches_menu.context = recompui_create_context();
    recompui_open_context(yaml_glitches_menu.context);

    createUiFrame(yaml_glitches_menu.context, &yaml_glitches_menu.frame);

    // Adjust the container's properties.
    recompui_set_width(yaml_glitches_menu.frame.container, 1200.0f, UNIT_DP);
    recompui_set_height(yaml_glitches_menu.frame.container, 800.0f, UNIT_DP);
    recompui_set_display(yaml_glitches_menu.frame.container, DISPLAY_FLEX);
    recompui_set_flex_direction(yaml_glitches_menu.frame.container, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(yaml_glitches_menu.frame.container, ALIGN_ITEMS_STRETCH);
    recompui_set_justify_content(yaml_glitches_menu.frame.container, JUSTIFY_CONTENT_FLEX_START);

    // Remove the padding on the frame's container so that the divider line has the full width of the container.
    recompui_set_padding(yaml_glitches_menu.frame.container, 0.0f, UNIT_DP);

    // Create the header.
    yaml_glitches_menu.header = recompui_create_element(yaml_glitches_menu.context, yaml_glitches_menu.frame.container);
    recompui_set_flex_grow(yaml_glitches_menu.header, 0.0f);
    recompui_set_flex_shrink(yaml_glitches_menu.header, 0.0f);
    recompui_set_display(yaml_glitches_menu.header, DISPLAY_FLEX);
    recompui_set_flex_direction(yaml_glitches_menu.header, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(yaml_glitches_menu.header, JUSTIFY_CONTENT_SPACE_BETWEEN);
    recompui_set_border_bottom_width(yaml_glitches_menu.header, 1.1f, UNIT_DP);
    recompui_set_height_auto(yaml_glitches_menu.header);
    recompui_set_padding(yaml_glitches_menu.header, 16.0f, UNIT_DP);
    recompui_set_align_items(yaml_glitches_menu.header, ALIGN_ITEMS_CENTER);

    RecompuiColor divider_color;
    divider_color.r = 255;
    divider_color.g = 255;
    divider_color.b = 255;
    divider_color.a = 25;
    recompui_set_border_bottom_color(yaml_glitches_menu.header, &divider_color);

    yaml_glitches_menu.header_label = recompui_create_label(yaml_glitches_menu.context, yaml_glitches_menu.header, "Randomizer Glitches Menu", LABELSTYLE_LARGE);
    recompui_set_text_align(yaml_glitches_menu.header_label, TEXT_ALIGN_CENTER);
    recompui_set_margin_left(yaml_glitches_menu.header_label, 32.0f, UNIT_DP);

    // Create the body.
    yaml_glitches_menu.body = recompui_create_element(yaml_glitches_menu.context, yaml_glitches_menu.frame.container);
    recompui_set_display(yaml_glitches_menu.body, DISPLAY_BLOCK);
    recompui_set_overflow_y(yaml_glitches_menu.body, OVERFLOW_SCROLL);

    yaml_glitches_menu.num_options = 0;
    randoCreateSetItem(&yaml_glitches_menu, "", "Fierce Deity Out Of Bounds");
    randoCreateSetItem(&yaml_glitches_menu, "", "Bomb Hover");
    randoCreateSetItem(&yaml_glitches_menu, "", "Long Bomb Hover");
    randoCreateSetItem(&yaml_glitches_menu, "", "Ocarina Items");
    randoCreateSetItem(&yaml_glitches_menu, "", "Action Swap");
    randoCreateSetItem(&yaml_glitches_menu, "", "Seamwalk");
    randoCreateSetItem(&yaml_glitches_menu, "", "Hard Seamwalk");
    randoCreateSetItem(&yaml_glitches_menu, "", "Weirdshot");
    randoCreateSetItem(&yaml_glitches_menu, "", "Time Stop");
    randoCreateSetItem(&yaml_glitches_menu, "", "Bow Extension");
    randoCreateSetItem(&yaml_glitches_menu, "", "Remote Hookshot");
    randoCreateSetItem(&yaml_glitches_menu, "", "Invert Stone Tower without Light Arrow");
    randoCreateSetItem(&yaml_glitches_menu, "", "Equip Swap");
    // randoCreateSetItem(&yaml_glitches_menu, "", "");

    // Create the back button, parenting it to the root with absolute positioning.
    yaml_glitches_menu.back_button = recompui_create_button(yaml_glitches_menu.context, yaml_glitches_menu.frame.root, "Back", BUTTONSTYLE_SECONDARY);
    recompui_set_position(yaml_glitches_menu.back_button, POSITION_ABSOLUTE);
    recompui_set_left(yaml_glitches_menu.back_button, 64.0f, UNIT_DP);
    recompui_set_top(yaml_glitches_menu.back_button, 32.0f, UNIT_DP);
    recompui_register_callback(yaml_glitches_menu.back_button, backPressed, &yaml_glitches_menu);

    recompui_set_nav(yaml_glitches_menu.back_button, NAVDIRECTION_DOWN, yaml_glitches_menu.all_options[0].input_element);
    recompui_set_nav(yaml_glitches_menu.all_options[0].input_element, NAVDIRECTION_UP, yaml_glitches_menu.back_button);

    recompui_close_context(yaml_glitches_menu.context);
}

void randoShowYamlGlitchesMenu() {
    recompui_show_context(yaml_glitches_menu.context);
}