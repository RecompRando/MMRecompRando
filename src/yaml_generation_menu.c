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

RandoOptionData* randoAllocateOption(RandoYamlConfigMenu* menu, const char* option_id, const char* option_description) {
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
    Lib_MemCpy(ret->option_id, (void*)option_id, option_id_len + 1);
    
    size_t option_description_len = strlen(option_description);
    ret->description = recomp_alloc(option_description_len + 1);
    Lib_MemCpy(ret->description, (void*)option_description, option_description_len + 1);
    
    ret->is_callback = false;

    return ret;
}

// probably delete these printfs ?
void* rando_yaml_printf_pfn(void* dst, const char* fmt, size_t size) {
    (void)dst;
    // rando_yaml_puts(fmt, size);
    return (void*)1;
}

RECOMP_EXPORT int rando_yaml_printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    int ret = _Printf(&rando_yaml_printf_pfn, NULL, fmt, args);

    va_end(args);

    return ret;
}

void randoYAMLCreate(RandoYamlConfigMenu* menu, bool place_in_archipelago) {
    REPY_FN_SETUP_RANDO;

    REPY_FN_IMPORT("rando_solo");

    REPY_FN_EXEC_CACHE(
        py_rando_setup_solo_yaml,
        "import yaml\n"
        "output_options = recomp_data.solo_yaml_options\n" // has to be in a module to make it usable by callbacks
    );

    // Iterate over the options and write their values into the yaml.
    for (u32 i = 0; i < menu->num_options; i++) {
        RandoOptionData* option = &menu->all_options[i];
        if (!option->is_callback) {
            REPY_FN_SET_STR("option_id", option->option_id);
            switch (option->type) {
                case OPTION_BOOL:
                    // rando_yaml_printf("  %s:\n    %s: 1\n", option->option_id, recompui_get_input_value_u32(option->input_element) ? "true" : "false");
                    REPY_FN_SET_BOOL("value", recompui_get_input_value_u32(option->input_element));
                    break;
                case OPTION_RADIO:
                    { // fixes warning
                        EnumOptionValue* enum_options = (EnumOptionValue*)option->data;
                        // rando_yaml_printf("  %s:\n    %s: 1\n", option->option_id, enum_options[recompui_get_input_value_u32(option->input_element)].id);
                        REPY_FN_SET_STR("value", enum_options[recompui_get_input_value_u32(option->input_element)].id);
                    }
                    break;
                case OPTION_INT_SLIDER:
                    // TODO replace this with recompui_get_input_value_s32 when it gets added
                    // rando_yaml_printf("  %s:\n    %d: 1\n", option->option_id, (s32)recompui_get_input_value_float(option->input_element));
                    REPY_FN_SET_S32("value", (s32)recompui_get_input_value_float(option->input_element));
                    break;
                case OPTION_FLOAT_SLIDER:
                    // rando_yaml_printf("  %s:\n    %f: 1\n", option->option_id, recompui_get_input_value_float(option->input_element));
                    REPY_FN_SET_F32("value", recompui_get_input_value_float(option->input_element)); // assuming f32 = float
                    break;
                case OPTION_LIST:
                    { // fixes warning
                        RandoListOption* list_data = (RandoListOption*)option->data;
                        switch (list_data->type) {
                            case LIST_STANDARD:
                                { // fixes warning
                                    REPY_FN_EXEC_CACHE(
                                        py_rando_setup_output_list,
                                        "value = []"
                                    );
                                    for (u32 i = 0; i < list_data->num_entries; i++) {
                                        if (list_data->entries[i].checked) {
                                            REPY_FN_SET_STR("entry_name", list_data->entries[i].name);
                                            REPY_FN_EXEC_CACHE(py_rando_output_list_append, "value.append(entry_name)");
                                        }
                                    }
                                }
                                break;
                            case LIST_DICT_ONES:
                                { // fixes warning
                                    REPY_FN_EXEC_CACHE(
                                        py_rando_setup_output_dict_ones,
                                        "value = {}"
                                    );
                                    for (u32 i = 0; i < list_data->num_entries; i++) {
                                        if (list_data->entries[i].checked) {
                                            REPY_FN_SET_STR("entry_name", list_data->entries[i].name);
                                            REPY_FN_EXEC_CACHE(py_rando_output_list_dict_one, "value[entry_name] = 1");
                                        }
                                    }
                                }
                                break;
                        }
                    }
                    break;
            }
            REPY_FN_EXEC_CACHE(
                py_rando_set_yaml_option,
                "output_options[option_id] = value\n"
            );
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
                case OPTION_LIST:
                    option->list_callback();
                    break;
            }
        }
    }

    recomp_printf("finished setting yaml options\n");

    REPY_FN_EXEC_CACHE(
        py_rando_write_yaml_to_mod_data,
        "from datetime import datetime\n"
        "current_datetime = datetime.now().strftime('%d-%m-%Y_%H-%M-%S')\n"
        "file_name = f'AP_Recomp_{current_datetime}.yaml'\n"
        "output_dir = recomp_data.mod_data_path.joinpath('solo_yamls', file_name)\n"
        "output_file = output_dir.open('w')\n"
        "rando_solo.populate_yaml_header(output_file)\n"
        "yaml.dump({recomp_data.game_name: output_options}, output_file)\n"
        "recomp_data.last_generated_yaml_location = output_dir\n"
    );

    if (place_in_archipelago) {
        REPY_FN_EXEC_CACHE(
            py_rando_write_solo_yaml_to_players,
            "rando_solo.clear_players_folder()\n"
            "output_dir = recomp_data.mod_data_path.joinpath('Archipelago', 'local', 'Players', 'solo.yaml')\n"
            "output_file = output_dir.open('w')\n"
            "rando_solo.populate_yaml_header(output_file)\n"
            "yaml.dump({recomp_data.game_name: output_options}, output_file)\n"
            "recomp_data.last_generated_yaml_location = output_dir\n"
        );
    }
    
    REPY_FN_CLEANUP;
}

// Stub: writes the configured YAML to disk without generating a seed.
// this still needs to be implemented once the export path is decided.
void randoYAMLExportCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    RandoYamlConfigMenu* menu = (RandoYamlConfigMenu*)userdata;
    if (data->type == UI_EVENT_CLICK) {
        REPY_FN_SETUP_RANDO;

        randoYAMLCreate(menu, false);

        REPY_FN_EVAL_CACHE_STR(
            py_rando_create_yaml_export_notification,
            "f'Wrote YAML file to {recomp_data.last_generated_yaml_location}.'",
            output_dir
        );
        
        // The notification opens its own UI context, so this one must be closed first.
        recompui_close_context(menu->context);
        randoCreateNormalNotification(output_dir);
        recompui_open_context(menu->context);
        
        // Return to start menu
        recompui_hide_context(yaml_config_menu.context);
        is_generate_menu_shown = false;
        // Close the start menu context temporarily so that the solo context can be opened.
        recompui_close_context(yaml_config_menu.context);
        randoShowSoloMenu();
        // Reopen the start menu context.
        recompui_open_context(yaml_config_menu.context);
        
        recomp_free(output_dir);
        REPY_FN_CLEANUP;
    }
}

void randoYAMLGenerateCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    RandoYamlConfigMenu* menu = (RandoYamlConfigMenu*)userdata;
    if (data->type == UI_EVENT_CLICK) {
        REPY_FN_SETUP_RANDO;
        
        randoYAMLCreate(menu, true);

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
            randoCreateErrorNotification("Failed to generate. Please report the settings you used to the developers.");
            recompui_open_context(yaml_config_menu.context);
        }

        REPY_FN_CLEANUP;
    }
}

RecompuiResource randoYAMLCreateMenuEntryArea (RecompuiContext context, RecompuiResource parent) {
    RecompuiResource retVal = recompui_create_element(context, parent);

    recompui_set_display(retVal, DISPLAY_FLEX);
    recompui_set_flex_direction(retVal, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(retVal, ALIGN_ITEMS_STRETCH);
    recompui_set_padding(retVal, 12.0f, UNIT_DP);
    recompui_set_gap(retVal, 4.0f, UNIT_DP);

    return retVal;
}

RecompuiResource randoYAMLCreateSettingLabel(RecompuiContext context, RecompuiResource parent, const char* display_name) {
    RecompuiResource label = recompui_create_label(context, parent, display_name, LABELSTYLE_NORMAL);
    recompui_set_display(label, DISPLAY_BLOCK);
    recompui_set_margin_bottom(label, 12.0f, UNIT_DP);
    return label;
}

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

// Sets the description pane to a given option's description on focus/hover.
void randoOptionFocusHandler(RecompuiResource resource, const RecompuiEventData* event, void* userdata) {
    RandoOptionData* option = (RandoOptionData*)userdata;
    bool show = false;
    switch (event->type) {
        case UI_EVENT_FOCUS: show = event->data.focus.active; break;
        case UI_EVENT_HOVER: show = event->data.hover.active; break;
        default: return;
    }
    if (show) {
        const char* text = option->description ? option->description : "";
        recompui_set_text(yaml_config_menu.description_pane, text);
    }
}

RandoOptionData* randoCreateRadioOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name, const char* option_description,
    EnumOptionValue* options, unsigned long num_options, u32 default_value) {
    RecompuiResource radio_area = randoYAMLCreateMenuEntryArea(menu->context, menu->current_body);

    randoYAMLCreateSettingLabel(menu->context, radio_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id, option_description);

    // Copy the and format the options into an array to pass as the option list for the radio.
    char** option_names = recomp_alloc(sizeof(char*) * num_options);
    for (u32 i = 0; i < num_options; i++) {
        option_names[i] = randoFormatOptionName(&options[i]);
    }

    RecompuiResource radio = recompui_create_labelradio(menu->context, radio_area, (const char**) option_names, num_options);
    recompui_set_width(radio, 100.0f, UNIT_PERCENT);

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
    recompui_register_callback(option_data->input_element, randoOptionFocusHandler, option_data);
    return option_data;
}

static char* rando_bool_prop_names[] = {"Off", "On"};
RandoOptionData* randoCreateBoolPropOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name, const char* option_description, bool default_value) {
    RecompuiResource radio_area = randoYAMLCreateMenuEntryArea(menu->context, menu->current_body);

    randoYAMLCreateSettingLabel(menu->context, radio_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id, option_description);

    RecompuiResource radio = recompui_create_labelradio(menu->context, radio_area, (const char**) rando_bool_prop_names, 2);
    recompui_set_input_value_u32(radio, (u32)default_value);

    option_data->type = OPTION_BOOL;
    option_data->root_element = radio_area;
    option_data->input_element = radio;
    recompui_register_callback(option_data->input_element, randoOptionFocusHandler, option_data);
    return option_data;
}

RandoOptionData* randoCreateCallbackBoolPropOption(RandoYamlConfigMenu* menu, const char* display_name, const char* option_description, bool default_value, bool_callback_t* callback) {
    RandoOptionData* option = randoCreateBoolPropOption(menu, "", display_name, option_description, default_value);
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

RandoOptionData* randoCreateIntSliderOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name, const char* option_description,
    s32 min, s32 max, s32 step, s32 default_value) {
    RecompuiResource slider_area = randoYAMLCreateMenuEntryArea(menu->context, menu->current_body);

    randoYAMLCreateSettingLabel(menu->context, slider_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id, option_description);

    RecompuiResource slider = recompui_create_slider(menu->context, slider_area, SLIDERTYPE_INTEGER, (float)min, (float)max, (float)step, (float)default_value);
    recompui_set_max_width(slider, 600.0f, UNIT_DP);

    option_data->type = OPTION_INT_SLIDER;
    option_data->root_element = slider_area;
    option_data->input_element = slider;
    recompui_register_callback(option_data->input_element, randoOptionFocusHandler, option_data);
    return option_data;
}

RandoOptionData* randoCreateFloatSliderOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name, const char* option_description,
    float min, float max, float step, float default_value) {
    RecompuiResource slider_area = randoYAMLCreateMenuEntryArea(menu->context, menu->current_body);

    randoYAMLCreateSettingLabel(menu->context, slider_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id, option_description);

    RecompuiResource slider = recompui_create_slider(menu->context, slider_area, SLIDERTYPE_NUMBER, min, max, step, default_value);
    recompui_set_max_width(slider, 600.0f, UNIT_DP);

    option_data->type = OPTION_FLOAT_SLIDER;
    option_data->root_element = slider_area;
    option_data->input_element = slider;
    recompui_register_callback(option_data->input_element, randoOptionFocusHandler, option_data);
    return option_data;
}

void randoCreateSearchSection(RandoYamlConfigMenu* menu, RandoListOption* list);
void randoListEntrySetText(RandoListEntry* entry);
void randoListEntryToggle(RecompuiResource button, const RecompuiEventData* data, void* userdata);

void randoListInitEntry(RandoYamlConfigMenu* menu, RandoListEntry* entry, const char* name) {
    RecompuiColor transparent_color = {0, 0, 0, 0};

    size_t len = strlen(name);
    entry->name = recomp_alloc(len + 1);
    Lib_MemCpy(entry->name, (void*)name, len + 1);

    entry->checked = false;
    entry->button = recompui_create_button(menu->context, menu->current_body, "", BUTTONSTYLE_SECONDARY);
    recompui_set_display(entry->button, DISPLAY_BLOCK);
    recompui_set_padding(entry->button, 4.0f, UNIT_DP);
    recompui_set_margin_bottom(entry->button, 2.0f, UNIT_DP);
    recompui_set_background_color(entry->button, &transparent_color);
    recompui_set_border_color(entry->button, &transparent_color);
    recompui_register_callback(entry->button, randoListEntryToggle, entry);
    randoListEntrySetText(entry);
}

RandoOptionData* randoCreateListOption(RandoYamlConfigMenu* menu, const char* option_id, const char* list_var, RandoListCategory type) {
    RandoOptionData* option_data = randoAllocateOption(menu, option_id, ""); // no need for option descriptions (they don't fit)

    option_data->type = OPTION_LIST;
    option_data->data = recomp_alloc(sizeof(RandoListOption));
    RandoListOption* list_data = (RandoListOption*)option_data->data;
    list_data->type = type;

    REPY_FN_SETUP_RANDO;
    
    REPY_FN_SET_STR("list_var", list_var);

    REPY_FN_EXEC_CACHE(
        py_rando_setup_list_element,
        "exec(f'list_elem = {list_var}')\n"
        "list_length = len(list_elem)\n"
    );

    u32 list_length = REPY_FN_GET_U32("list_length");

    list_data->entries = recomp_alloc(sizeof(RandoListEntry) * list_length);
    list_data->num_entries = list_length;

    randoCreateSearchSection(&yaml_config_menu, list_data);

    u32 item_index = 0;
    REPY_FN_FOREACH_CACHE(py_rando_fill_items_from_apworld, "entry", "list_elem") {
        char* entry = REPY_FN_GET_STR("entry");
        randoListInitEntry(&yaml_config_menu, &list_data->entries[item_index++], entry);
        recomp_free(entry);
    }

    REPY_FN_CLEANUP;

    return option_data;
}

// TODO: update colors mod to remove the need for this
void tunicColorCallback(bool enabled) {
    if (enabled) {
        REPY_FN_SETUP_RANDO;
        u32 link_color = Rand_Next();
        // u32 link_red = (link_color >> 24) & 0xFF;
        // u32 link_green = (link_color >> 16) & 0xFF;
        // u32 link_blue = (link_color >> 8) & 0xFF;
        // rando_yaml_printf("  link_tunic_color:\n    [%d, %d, %d]\n", link_red, link_green, link_blue);
        REPY_FN_SET_U32("link_red", (link_color >> 24) & 0xFF);
        REPY_FN_SET_U32("link_green", (link_color >> 16) & 0xFF);
        REPY_FN_SET_U32("link_blue", (link_color >> 8) & 0xFF);
        REPY_FN_EXEC_CACHE(
            py_rando_set_tunic_yaml,
            "recomp_data.solo_yaml_options['link_tunic_color'] = [link_red, link_green, link_blue]"
        );
        REPY_FN_CLEANUP;
    }
}

// Tabs: one button in the header bar, one body panel. Only the active panel.

RecompuiColor tab_active_color   = {255, 255, 255, 255};
RecompuiColor tab_inactive_color = {255, 255, 255, 110};

// Panel that section buttons parent into; set by randoBeginTab.
RecompuiResource rando_current_panel = 0;

// Per-tab flag: hide the description pane when this tab is active (for list
// tabs like Starting Items / Excluded Locations that don't use descriptions).
bool rando_tab_hide_description[MAX_TABS];

void randoSelectTab(RandoYamlConfigMenu* menu, u32 index) {
    for (u32 i = 0; i < menu->num_tabs; i++) {
        bool active = (i == index);
        recompui_set_display(menu->tabs[i].panel, active ? DISPLAY_BLOCK : DISPLAY_NONE);
        recompui_set_color(menu->tabs[i].button, active ? &tab_active_color : &tab_inactive_color);
    }
    bool hide_desc = rando_tab_hide_description[index];
    recompui_set_display(menu->description_pane, hide_desc ? DISPLAY_NONE : DISPLAY_BLOCK);
    recompui_set_flex_basis(menu->option_column, hide_desc ? 100.0f : 72.0f, UNIT_PERCENT);
    menu->active_tab = index;
}

void randoTabButtonCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        RandoTab* tab = (RandoTab*)userdata;
        randoSelectTab(tab->menu, tab->index);
    }
}

RecompuiResource randoBeginTab(RandoYamlConfigMenu* menu, const char* label) {
    if (menu->num_tabs >= MAX_TABS) {
        recomp_printf("Max rando tabs reached, increase MAX_TABS in \"yaml_generation.h\"\n");
        *(volatile int*)0 = 0;
        return 0;
    }
    RandoTab* tab = &menu->tabs[menu->num_tabs];
    tab->menu = menu;
    tab->index = menu->num_tabs++;

    tab->button = recompui_create_button(menu->context, menu->tab_bar, label, BUTTONSTYLE_SECONDARY);
    recompui_register_callback(tab->button, randoTabButtonCallback, tab);
    recompui_set_font_size(tab->button, 18.0f, UNIT_DP);
    recompui_set_padding(tab->button, 4.0f, UNIT_DP);
    recompui_set_flex_grow(tab->button, 1.0f);
    recompui_set_flex_shrink(tab->button, 0.0f);
    recompui_set_text_align(tab->button, TEXT_ALIGN_CENTER);

    tab->panel = recompui_create_element(menu->context, menu->option_column);
    recompui_set_display(tab->panel, DISPLAY_NONE);

    menu->current_body = tab->panel;
    rando_current_panel = tab->panel;
    return tab->panel;
}

void randoTabPlaceholder(RandoYamlConfigMenu* menu, const char* text) {
    RecompuiResource label = recompui_create_label(menu->context, menu->current_body, text, LABELSTYLE_NORMAL);
    recompui_set_padding(label, 24.0f, UNIT_DP);
}

// Collapsible section: a button toggling a wrapper of options. Options are built
// once at startup and shown/hidden, so YAML output is unaffected hopefully.
RandoSection rando_sections[MAX_SECTIONS];
u32 rando_num_sections = 0;

// Render an entry's button label as "[X] Name" / "[ ] Name".
void randoListEntrySetText(RandoListEntry* entry) {
    char buf[128];
    buf[0] = '[';
    buf[1] = entry->checked ? 'X' : ' ';
    buf[2] = ']';
    buf[3] = ' ';
    u32 i = 0;
    while (entry->name[i] != '\0' && i < sizeof(buf) - 5) {
        buf[4 + i] = entry->name[i];
        i++;
    }
    buf[4 + i] = '\0';
    recompui_set_text(entry->button, buf);
}

void randoListEntryToggle(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        RandoListEntry* entry = (RandoListEntry*)userdata;
        entry->checked = !entry->checked;
        randoListEntrySetText(entry);
    }
}

// Case-insensitive substring test for the search filters.
bool randoStrContainsCI(const char* haystack, const char* needle) {
    if (needle[0] == '\0') {
        return true;
    }
    for (u32 i = 0; haystack[i] != '\0'; i++) {
        u32 j = 0;
        while (haystack[i + j] != '\0' && needle[j] != '\0') {
            char a = haystack[i + j];
            char b = needle[j];
            if (a >= 'A' && a <= 'Z') a += 32;
            if (b >= 'A' && b <= 'Z') b += 32;
            if (a != b) break;
            j++;
        }
        if (needle[j] == '\0') {
            return true;
        }
    }
    return false;
}

// Single generic search callback. The list is passed via userdata.
void randoListSearchCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        RandoListOption* list = (RandoListOption*)userdata;
        char* query = recompui_get_input_text(list->search_input);
        for (u32 i = 0; i < list->num_entries; i++) {
            bool match = randoStrContainsCI(list->entries[i].name, query);
            recompui_set_display(list->entries[i].button, match ? DISPLAY_BLOCK : DISPLAY_NONE);
        }
        recomp_free(query);
    }
}

// Builds a pinned search row + an inner scrolling body. The search row stays
// fixed at the top of the tab while the returned body scrolls. 
void randoCreateSearchSection(RandoYamlConfigMenu* menu, RandoListOption* list) {
    // Make the tab panel a column so the search row and scroll body stack.
    recompui_set_display(menu->current_body, DISPLAY_FLEX);
    recompui_set_flex_direction(menu->current_body, FLEX_DIRECTION_COLUMN);
    recompui_set_height(menu->current_body, 100.0f, UNIT_PERCENT);

    // Pinned search row (does not scroll).
    RecompuiResource search_row = recompui_create_element(menu->context, menu->current_body);
    recompui_set_display(search_row, DISPLAY_FLEX);
    recompui_set_flex_direction(search_row, FLEX_DIRECTION_ROW);
    recompui_set_align_items(search_row, ALIGN_ITEMS_CENTER);
    recompui_set_gap(search_row, 8.0f, UNIT_DP);
    recompui_set_padding(search_row, 8.0f, UNIT_DP);
    recompui_set_flex_grow(search_row, 0.0f);
    recompui_set_flex_shrink(search_row, 0.0f);

    RecompuiResource input = recompui_create_textinput(menu->context, search_row);
    recompui_set_flex_grow(input, 1.0f);

    RecompuiResource search_button = recompui_create_button(menu->context, search_row, "Search", BUTTONSTYLE_SECONDARY);
    recompui_register_callback(search_button, randoListSearchCallback, list);

    // Inner scrolling body for the entries. Mirrors the solo menu's
    // list_container: 
    RecompuiResource scroll_body = recompui_create_element(menu->context, menu->current_body);
    recompui_set_display(scroll_body, DISPLAY_BLOCK);
    recompui_set_overflow_y(scroll_body, OVERFLOW_AUTO);
    recompui_set_flex_grow(scroll_body, 1.0f);
    recompui_set_flex_shrink(scroll_body, 1.0f);
    recompui_set_height(scroll_body, 100.0f, UNIT_PERCENT);
    recompui_set_max_height(scroll_body, 100.0f, UNIT_PERCENT);

    // Entries now parent into the scroll body.
    menu->current_body = scroll_body;

    list->search_input = input;
}

void randoSectionSetButtonText(RandoSection* section) {
    char buf[128];
    buf[0] = section->open ? '-' : '+';
    buf[1] = ' ';
    u32 i = 0;
    while (section->title[i] != '\0' && i < sizeof(buf) - 3) {
        buf[2 + i] = section->title[i];
        i++;
    }
    buf[2 + i] = '\0';
    recompui_set_text(section->button, buf);
}

void randoSectionToggleCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        RandoSection* section = (RandoSection*)userdata;
        section->open = !section->open;
        recompui_set_display(section->wrapper, section->open ? DISPLAY_BLOCK : DISPLAY_NONE);
        randoSectionSetButtonText(section);
    }
}

void randoBeginSection(RandoYamlConfigMenu* menu, const char* title) {
    RandoSection* section = &rando_sections[rando_num_sections++];
    
    size_t title_len = strlen(title);
    section->title = recomp_alloc(title_len + 1);
    Lib_MemCpy(section->title, (void*)title, title_len + 1);

    section->open = false;

    section->button = recompui_create_button(menu->context, rando_current_panel, title, BUTTONSTYLE_SECONDARY);
    recompui_set_display(section->button, DISPLAY_BLOCK);
    recompui_set_margin_top(section->button, 16.0f, UNIT_DP);
    recompui_set_margin_bottom(section->button, 4.0f, UNIT_DP);
    recompui_set_padding(section->button, 8.0f, UNIT_DP);
    recompui_register_callback(section->button, randoSectionToggleCallback, section);
    randoSectionSetButtonText(section);

    section->wrapper = recompui_create_element(menu->context, rando_current_panel);
    recompui_set_display(section->wrapper, DISPLAY_NONE);

    menu->current_body = section->wrapper;
}

void randoCreateYamlConfigMenu() {
    yaml_config_menu.context = recompui_create_context();
    recompui_open_context(yaml_config_menu.context);

    createUiFrame(yaml_config_menu.context, &yaml_config_menu.frame);

    recompui_set_width(yaml_config_menu.frame.container, 1200.0f, UNIT_DP);
    recompui_set_height(yaml_config_menu.frame.container, 800.0f, UNIT_DP);
    recompui_set_display(yaml_config_menu.frame.container, DISPLAY_FLEX);
    recompui_set_flex_direction(yaml_config_menu.frame.container, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(yaml_config_menu.frame.container, ALIGN_ITEMS_STRETCH);
    recompui_set_justify_content(yaml_config_menu.frame.container, JUSTIFY_CONTENT_FLEX_START);

    // No container padding so the header divider spans the full width.
    recompui_set_padding(yaml_config_menu.frame.container, 0.0f, UNIT_DP);

    // Header is a column: top row (title + Generate), then the tab bar.
    yaml_config_menu.header = recompui_create_element(yaml_config_menu.context, yaml_config_menu.frame.container);
    recompui_set_flex_grow(yaml_config_menu.header, 0.0f);
    recompui_set_flex_shrink(yaml_config_menu.header, 0.0f);
    recompui_set_display(yaml_config_menu.header, DISPLAY_FLEX);
    recompui_set_flex_direction(yaml_config_menu.header, FLEX_DIRECTION_COLUMN);
    recompui_set_border_bottom_width(yaml_config_menu.header, 1.1f, UNIT_DP);
    recompui_set_height_auto(yaml_config_menu.header);
    recompui_set_padding(yaml_config_menu.header, 16.0f, UNIT_DP);

    RecompuiColor divider_color;
    divider_color.r = 255;
    divider_color.g = 255;
    divider_color.b = 255;
    divider_color.a = 25;
    recompui_set_border_bottom_color(yaml_config_menu.header, &divider_color);

    // Top row: title on the left, Generate on the right.
    RecompuiResource header_top = recompui_create_element(yaml_config_menu.context, yaml_config_menu.header);
    recompui_set_display(header_top, DISPLAY_FLEX);
    recompui_set_flex_direction(header_top, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(header_top, JUSTIFY_CONTENT_SPACE_EVENLY);
    recompui_set_align_items(header_top, ALIGN_ITEMS_CENTER);

    yaml_config_menu.header_label = recompui_create_label(yaml_config_menu.context, header_top, "Randomizer Settings", LABELSTYLE_LARGE);
    recompui_set_text_align(yaml_config_menu.header_label, TEXT_ALIGN_CENTER);

    yaml_config_menu.export_button = recompui_create_button(yaml_config_menu.context, header_top, "Export Yaml", BUTTONSTYLE_PRIMARY);
    recompui_register_callback(yaml_config_menu.export_button, randoYAMLExportCallback, &yaml_config_menu);
    recompui_set_margin_right(yaml_config_menu.export_button, 12.0f, UNIT_DP);

    yaml_config_menu.generate_button = recompui_create_button(yaml_config_menu.context, header_top, "Generate", BUTTONSTYLE_PRIMARY);
    recompui_register_callback(yaml_config_menu.generate_button, randoYAMLGenerateCallback, &yaml_config_menu);

    // Second row: tab bar spanning the full header width.
    yaml_config_menu.tab_bar = recompui_create_element(yaml_config_menu.context, yaml_config_menu.header);
    recompui_set_display(yaml_config_menu.tab_bar, DISPLAY_FLEX);
    recompui_set_flex_direction(yaml_config_menu.tab_bar, FLEX_DIRECTION_ROW);
    recompui_set_align_items(yaml_config_menu.tab_bar, ALIGN_ITEMS_STRETCH);
    recompui_set_justify_content(yaml_config_menu.tab_bar, JUSTIFY_CONTENT_SPACE_BETWEEN);
    recompui_set_gap(yaml_config_menu.tab_bar, 4.0f, UNIT_DP);
    recompui_set_margin_top(yaml_config_menu.tab_bar, 12.0f, UNIT_DP);

    // Body: a row split into the scrollable option column (left) and the
    // description pane (right), divided by a border.
    yaml_config_menu.body = recompui_create_element(yaml_config_menu.context, yaml_config_menu.frame.container);
    recompui_set_display(yaml_config_menu.body, DISPLAY_FLEX);
    recompui_set_flex_direction(yaml_config_menu.body, FLEX_DIRECTION_ROW);
    recompui_set_flex_grow(yaml_config_menu.body, 1.0f);
    recompui_set_align_items(yaml_config_menu.body, ALIGN_ITEMS_STRETCH);

    yaml_config_menu.option_column = recompui_create_element(yaml_config_menu.context, yaml_config_menu.body);
    recompui_set_display(yaml_config_menu.option_column, DISPLAY_BLOCK);
    recompui_set_overflow_y(yaml_config_menu.option_column, OVERFLOW_SCROLL);
    recompui_set_flex_basis(yaml_config_menu.option_column, 72.0f, UNIT_PERCENT);
    recompui_set_flex_grow(yaml_config_menu.option_column, 0.0f);
    recompui_set_flex_shrink(yaml_config_menu.option_column, 0.0f);
    recompui_set_height(yaml_config_menu.option_column, 100.0f, UNIT_PERCENT);
    recompui_set_max_height(yaml_config_menu.option_column, 100.0f, UNIT_PERCENT);
    recompui_set_padding(yaml_config_menu.option_column, 16.0f, UNIT_DP);

    RecompuiColor pane_divider_color = {255, 255, 255, 40};
    yaml_config_menu.description_pane = recompui_create_label(yaml_config_menu.context, yaml_config_menu.body, "", LABELSTYLE_NORMAL);
    recompui_set_display(yaml_config_menu.description_pane, DISPLAY_BLOCK);
    recompui_set_flex_basis(yaml_config_menu.description_pane, 28.0f, UNIT_PERCENT);
    recompui_set_flex_grow(yaml_config_menu.description_pane, 1.0f);
    recompui_set_flex_shrink(yaml_config_menu.description_pane, 1.0f);
    recompui_set_padding(yaml_config_menu.description_pane, 16.0f, UNIT_DP);
    recompui_set_border_left_width(yaml_config_menu.description_pane, 1.1f, UNIT_DP);
    recompui_set_border_color(yaml_config_menu.description_pane, &pane_divider_color);
    recompui_set_font_size(yaml_config_menu.description_pane, 18.0f, UNIT_DP);

    yaml_config_menu.num_options = 0;
    yaml_config_menu.num_tabs = 0;
    rando_num_sections = 0;
    for (u32 i = 0; i < MAX_TABS; i++) {
        rando_tab_hide_description[i] = false;
    }

    REPY_FN_SETUP_RANDO;

    // General tab.
    randoBeginTab(&yaml_config_menu, "General");

    REPY_FN_FOREACH_CACHE(py_rando_fill_options_from_group, "option_group", "recomp_data.options.items()") {
        REPY_FN_EXEC_CACHE(
            py_rando_grab_option_groups,
            "option_group_name, options = option_group\n"
        );

        // only separate options into sections if there's more than the default "Game Options" and "Item & Location Options" group
        REPY_FN_IF_CACHE(py_rando_option_groups_actually_exist, "len(recomp_data.options) > 2 and option_group_name != 'Item & Location Options'") {
            char* option_group_name;
            option_group_name = REPY_FN_GET_STR("option_group_name");
            randoBeginSection(&yaml_config_menu, option_group_name);
            recomp_free(option_group_name);
        }

        REPY_FN_FOREACH_CACHE(py_rando_fill_options, "option", "options.items()") {
            REPY_FN_EXEC_CACHE(
                py_rando_grab_initial_option_info,
                "option_id, option_info = option\n"
                "option_type = option_info['type']\n"
                "option_name_colon = option_info['name'] + ':'\n" // yes this is dumb
                "option_description = option_info['description'].lstrip()\n" // lstrip just for the accessibility option having a leading newline
            );

            char* option_id;
            char* option_name;
            char* option_description;
            option_id = REPY_FN_GET_STR("option_id");
            option_name = REPY_FN_GET_STR("option_name_colon");
            option_description = REPY_FN_GET_STR("option_description");

            REPY_FN_IF_CACHE(py_rando_get_option_type, "option_type == 'Choice'") {
                REPY_FN_EXEC_CACHE(
                    py_rando_get_choice_option_info,
                    "num_choices = len(option_info['choices'])\n"
                    // the following is a mess because choices might be offset from what the ui interprets
                    // i.e. {'choices': {'normal': 1, 'no_logic': 4}, 'default': 1} choosing 'no_logic' rather than 'normal'
                    "default_choice = list(option_info['choices'].keys())[list(option_info['choices'].values()).index(option_info['default'])]\n"
                    "option_default = list(option_info['choices'].keys()).index(default_choice)\n"
                );

                u32 num_choices = REPY_FN_GET_U32("num_choices");
                u32 option_default = REPY_FN_GET_U32("option_default");

                EnumOptionValue* option_choices = recomp_alloc(sizeof(EnumOptionValue) * num_choices);
                int index = 0;
                
                REPY_FN_FOREACH_CACHE(py_rando_options_populate_choices, "choice", "option_info['choices'].items()") {
                    // we could get the actual numerical value of the choices here as well,
                    // but since the generator converts the string to a number anyways we let it handle that
                    REPY_FN_EXEC_CACHE(
                        py_rando_get_choice_option_names,
                        "choice_id = choice[0]\n"
                        "choice_name = choice_id.replace('_', ' ').title()\n" // the .title() part is a bit redundant since its displayed in all caps
                    );

                    // these get freed in the randoCreateRadioOption function (hopefully)
                    option_choices[index].id = REPY_FN_GET_STR("choice_id");
                    option_choices[index].name = REPY_FN_GET_STR("choice_name");

                    index++;
                }
                
                randoCreateRadioOption(
                    &yaml_config_menu,
                    option_id,
                    option_name,
                    option_description,
                    option_choices,
                    num_choices,
                    option_default
                );
            } REPY_FN_ELIF_CACHE(py_rando_get_option_type, "option_type == 'Range'") {
                REPY_FN_EXEC_CACHE(
                    py_rando_grab_range_option_info,
                    "range_start = option_info['range_start']\n"
                    "range_end = option_info['range_end']\n"
                    "option_default = option_info['default']\n"
                );

                u32 range_start = REPY_FN_GET_U32("range_start");
                u32 range_end = REPY_FN_GET_U32("range_end");
                u32 option_default = REPY_FN_GET_U32("option_default");
                
                randoCreateIntSliderOption(
                    &yaml_config_menu,
                    option_id,
                    option_name,
                    option_description,
                    range_start,
                    range_end,
                    1,
                    option_default
                );
            } REPY_FN_ELIF_CACHE(py_rando_get_option_type, "option_type == 'Toggle' or option_type == 'DefaultOnToggle'") {
                REPY_FN_EVAL_CACHE_U32(
                    py_rando_get_toggle_option_default,
                    "option_info['default']",
                    option_default
                );
                
                randoCreateBoolPropOption(
                    &yaml_config_menu,
                    option_id,
                    option_name,
                    option_description,
                    option_default
                );
            }

            recomp_free(option_id);
            recomp_free(option_name);
            recomp_free(option_description);
        }
    }

    // lol
    randoCreateCallbackBoolPropOption(&yaml_config_menu, "Randomize Tunic Color:", "Randomize the color of Link's tunic.", true, tunicColorCallback);

    // Starting Items tab.
    randoBeginTab(&yaml_config_menu, "Starting Items");
    rando_tab_hide_description[yaml_config_menu.num_tabs - 1] = true;
    
    randoCreateListOption(&yaml_config_menu, "start_inventory_from_pool", "recomp_data.item_names", LIST_DICT_ONES); // TODO: handle item groups correctly

    // Excluded Locations tab.
    randoBeginTab(&yaml_config_menu, "Excluded Locations");
    rando_tab_hide_description[yaml_config_menu.num_tabs - 1] = true;
    
    randoCreateListOption(&yaml_config_menu, "exclude_locations", "recomp_data.location_names", LIST_STANDARD); // TODO: handle location groups correctly

    // TODO: add everything needed to get these to work (elements are in option.valid_keys)
    // Tricks tab (filled by the apworld SoonTM; empty for now).
    // randoBeginTab(&yaml_config_menu, "Tricks");
    // rando_tab_hide_description[yaml_config_menu.num_tabs - 1] = true;
    // randoTabPlaceholder(&yaml_config_menu, "No tricks are available for this game yet.");

    // // Glitches tab (filled by the apworld; empty for now).
    // randoBeginTab(&yaml_config_menu, "Glitches");
    // randoTabPlaceholder(&yaml_config_menu, "No glitches are available for this game yet.");

    // Show the first tab by default.
    randoSelectTab(&yaml_config_menu, 0);

    // Create the back button, parenting it to the root with absolute positioning.
    yaml_config_menu.back_button = recompui_create_button(yaml_config_menu.context, yaml_config_menu.frame.root, "Back", BUTTONSTYLE_SECONDARY);
    recompui_set_position(yaml_config_menu.back_button, POSITION_ABSOLUTE);
    recompui_set_left(yaml_config_menu.back_button, 64.0f, UNIT_DP);
    recompui_set_top(yaml_config_menu.back_button, 32.0f, UNIT_DP);
    recompui_register_callback(yaml_config_menu.back_button, backPressed, NULL);

    recompui_set_nav(yaml_config_menu.generate_button, NAVDIRECTION_DOWN, yaml_config_menu.all_options[0].input_element);
    recompui_set_nav(yaml_config_menu.all_options[0].input_element, NAVDIRECTION_UP, yaml_config_menu.generate_button);

    recompui_close_context(yaml_config_menu.context);
    REPY_FN_CLEANUP;
}

void randoShowYamlConfigMenu() {
    recompui_show_context(yaml_config_menu.context);
    is_generate_menu_shown = true;
}