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


// Stub: writes the configured YAML to disk without generating a seed.
// this still needs to be implemented once the export path is decided.
void randoYAMLExportCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    RandoYamlConfigMenu* menu = (RandoYamlConfigMenu*)userdata;
    if (data->type == UI_EVENT_CLICK) {
        // The notification opens its own UI context, so this one must be closed first.
        recompui_close_context(menu->context);
        randoEmitNormalNotification("Export not yet implemented.");
        recompui_open_context(menu->context);
    }
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
                        rando_yaml_printf("  %s:\n    %s: 1\n", option->option_id, recompui_get_input_value_u32(option->input_element) ? "true" : "false");
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

// Option descriptions, keyed by option_id from the apworld's Options.py
// I think to move these to the apworld, we replace the body of
// randoLookupDescription with a glue call (e.g. rando_option_get_description) right Hyped?.
typedef struct {
    const char* id;
    const char* description;
} RandoOptionDescription;

static const RandoOptionDescription rando_option_descriptions[] = {
    // General
    { "accessibility", "Full requires every location be reachable. Minimal only requires the goal be reachable." },
    { "logic_difficulty", "Logic difficulty used when generating. No Logic places items with no reachability guarantees." },
    { "camc", "Chest appearance matches its contents." },
    { "magic_is_a_trap", "Preserves the vanilla bug where certain magic items work without magic, until you first receive magic. No logical implications." },
    { "hint_percentage", "Percentage of gossip stones that give useful hints for items in the multiworld." },
    { "damage_multiplier", "Adjusts the amount of damage taken. One-Hit KO kills on any hit." },
    { "death_behavior", "What happens on death. Fast speeds up the cutscene; Moon Crash restarts the current cycle." },
    { "death_link", "Share deaths with other players in the multiworld." },

    // Goals
    { "completion_goal", "Requires 100% of all collectibles, with a special flag associated with it." },
    { "moon_remains_required", "Boss Remains required to reach the Moon after playing Oath to Order." },
    { "moon_masks_required", "Masks required to reach the Moon after playing Oath to Order." },
    { "moon_star_fox", "Require Keaton, Scents, Bremen, Bunny and Gero masks to reach the Moon." },
    { "moon_owls_required", "Owl Statues required to reach the Moon after playing Oath to Order." },
    { "moon_scarecrows_required", "Scarecrows required to reach the Moon after playing Oath to Order." },
    { "moon_frogs_required", "Frogs required to reach the Moon after playing Oath to Order." },
    { "moon_items_required", "Trade items required to reach the Moon after playing Oath to Order." },
    { "majora_remains_required", "Boss Remains required to fight Majora." },
    { "majora_masks_required", "Masks required to fight Majora." },
    { "majora_star_fox", "Require Keaton, Scents, Bremen, Bunny and Gero masks to fight Majora." },
    { "majora_owls_required", "Owl Statues required to fight Majora." },
    { "majora_scarecrows_required", "Scarecrows required to fight Majora." },
    { "majora_frogs_required", "Frogs required to fight Majora." },
    { "majora_items_required", "Trade items required to fight Majora." },

    // Starting Items
    { "ocarinaless", "Start without an Ocarina, shuffling it into the pool. Has the effect of sped-up time." },
    { "timeless", "Start without the Song of Time, shuffling it into the pool." },
    { "swordless", "Start without a sword, and shuffle an extra Progressive Sword into the pool." },
    { "shieldless", "Start without a shield, and shuffle an extra Progressive Shield into the pool." },
    { "start_with_soaring", "Start with the Song of Soaring." },
    { "starting_hearts", "Heart quarters Link starts with. Below 12, extra heart items are shuffled into the pool." },
    { "starting_hearts_are_containers_or_pieces", "Whether starting hearts shuffle in as Heart Containers (plus remainder as Pieces) or all as Heart Pieces." },
    { "start_with_consumables", "Start with basic consumables (99 rupees, 10 deku sticks, 20 deku nuts)." },
    { "permanent_chateau_romani", "Chateau Romani stays even after a reset." },
    { "start_with_inverted_time", "Time starts inverted at Day 1, even after a reset." },
    { "receive_filled_wallets", "Receive wallets pre-filled (not including the starting wallet)." },

    // Shuffle
    { "dungeon_entrance_rando", "Randomize dungeons amongst themselves." },
    { "boss_entrance_rando", "Randomize bosses amongst themselves." },
    { "dungeon_chaining", "With dungeon and boss entrance rando on, allow dungeons to lead into each other or only have bosses." },
    { "shuffle_regional_maps", "Shuffle every regional map from Tingle: vanilla, in the starting inventory, or anywhere." },
    { "shuffle_boss_remains", "Where Boss Remains appear: vanilla, anywhere in any world, or shuffled among the bosses." },
    { "remains_allow_boss_warps", "Keep the vanilla ability to warp to a dungeon's boss by holding its remains. The remains check also opens its warp." },
    { "shuffle_spiderhouse_reward", "Shuffle the Mask of Truth from the Southern Spider House and the Wallet Upgrade from the Ocean Spider House." },
    { "required_skull_tokens", "Gold Skulltula Tokens needed for each Spider House reward. All 30 per house are shuffled in regardless." },
    { "skullsanity", "What gold skulltulas give: vanilla tokens, anything anywhere, or remove the swamphouse from generation." },
    { "shopsanity", "Whether shops and their items are shuffled. Advanced also shuffles night and spring variant shops." },
    { "scrubsanity", "Shuffle Business Scrub purchases." },
    { "shop_prices", "How expensive main-shop items are: vanilla, free, cheap, expensive, or offensive. No effect if shopsanity is off." },
    { "cowsanity", "Shuffle Cows." },
    { "shuffle_great_fairy_rewards", "Shuffle Great Fairy rewards." },
    { "required_stray_fairies", "Stray Fairies needed for each Great Fairy reward (excludes North Clock Town). All 15 per dungeon are shuffled in regardless." },
    { "fairysanity", "Shuffle Stray Fairies into the pool." },
    { "keysanity", "Shuffle Small Keys into the pool instead of their vanilla locations." },
    { "bosskeysanity", "Shuffle Boss Keys into the pool instead of their vanilla locations." },
    { "curiostity_shop_trades", "Shuffle the rupees given for trading bottled items at the Curiosity Shop." },
    { "intro_checks", "Shuffle the checks normally found before the Clock Tower. A way back is added via the stone door in the Clock Tower interior." },

    // Sanity
    { "grasssanity", "How grass is shuffled: all, all except Termina Field, grotto/cave only, or dungeon only." },
    { "potsanity", "Shuffle pots into the pool." },
    { "hitsanity", "Shuffle hit-spot items into the pool." },
    { "rocksanity", "Shuffle rock items into the pool." },
    { "soilsanity", "Shuffle soil items into the pool." },
    { "rupeesanity", "Shuffle freestanding rupees into the pool." },
    { "invisisanity", "Shuffle invisible rupees into the pool." },
    { "snowsanity", "Shuffle snowball items into the pool." },
    { "woodsanity", "Shuffle wooden items into the pool." },
    { "realfairysanity", "Shuffle gossip, butterfly, and freestanding fairies." },
    { "iciclesanity", "Shuffle icicle items into the pool." },
    { "scarecrowsanity", "The scarecrow rewards an item when spawned." },
    { "hivesanity", "Shuffle hive items into the pool." },
    { "notebooksanity", "Shuffle Notebook entries as items." },
    { "owlsanity", "Shuffle Owl Statues as items." },
    { "frogsanity", "Shuffle the four frogs as items." },
    { "treesanity", "Trees and bushes drop shuffled items." },
    { "flowersanity", "Deku Flowers give shuffled items when entered." },
    { "signsanity", "Square signposts give shuffled items when cut." },
    { "websanity", "Burning spider webs gives shuffled items." },
    { "oneoffs", "One-off locations like skull kid pictures and bombable walls give shuffled items." },

    // Souls
    { "boss_souls", "Add souls for main bosses (Odolwa, Goht, Gyorg, Twinmold, optionally Majora). They won't spawn without their soul." },
    { "npc_souls", "Add souls for NPCs such as Anju, Romani, and Kamaro. They won't spawn without their soul." },
    { "enemy_souls", "Add souls for basic enemies such as Wolfos and Peahat. They won't spawn without their soul." },
    { "misc_souls", "Add souls for Cows, Gold Skulltulas, and Keaton. They won't spawn without their soul." },
    { "utility_souls", "Add souls for utility items like Postboxes. They won't spawn without their soul." },
    { "absurd_souls", "Add souls for absurd things like rocks, songwall, and grass. They won't spawn without their soul." },
};

static bool randoStrEq(const char* a, const char* b) {
    u32 i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return false;
        }
        i++;
    }
    return a[i] == b[i];
}

static const char* randoLookupDescription(const char* option_id) {
    if (option_id == NULL || option_id[0] == '\0') {
        return "";
    }
    for (u32 i = 0; i < ARRAY_COUNT(rando_option_descriptions); i++) {
        if (randoStrEq(option_id, rando_option_descriptions[i].id)) {
            return rando_option_descriptions[i].description;
        }
    }
    return "";
}

// Sets the description pane to a given option's description on focus/hover.
static const char* rando_default_description = "";
static void randoOptionFocusHandler(RecompuiResource resource, const RecompuiEventData* event, void* userdata) {
    RandoOptionData* option = (RandoOptionData*)userdata;
    bool show = false;
    switch (event->type) {
        case UI_EVENT_FOCUS: show = event->data.focus.active; break;
        case UI_EVENT_HOVER: show = event->data.hover.active; break;
        default: return;
    }
    if (show) {
        const char* text = option->description ? option->description : rando_default_description;
        recompui_set_text(yaml_config_menu.description_pane, text);
    }
}

// Common tail for the create helpers: stores the description and wires the
// focus/hover handler that drives the description pane.
static void randoFinishOption(RandoOptionData* option, const char* description) {
    option->description = description;
    recompui_register_callback(option->input_element, randoOptionFocusHandler, option);
}

RandoOptionData* randoCreateRadioOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name,
    EnumOptionValue* options, unsigned long num_options, u32 default_value) {
    RecompuiResource radio_area = randoYAMLCreateMenuEntryArea(menu->context, menu->current_body);

    randoYAMLCreateSettingLabel(menu->context, radio_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id);

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
    randoFinishOption(option_data, randoLookupDescription(option_id));
    return option_data;
}

static char* rando_bool_prop_names[] = {"Off", "On"};
RandoOptionData* randoCreateBoolPropOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name, bool default_value) {
    RecompuiResource radio_area = randoYAMLCreateMenuEntryArea(menu->context, menu->current_body);

    randoYAMLCreateSettingLabel(menu->context, radio_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id);

    RecompuiResource radio = recompui_create_labelradio(menu->context, radio_area, (const char**) rando_bool_prop_names, 2);
    recompui_set_input_value_u32(radio, (u32)default_value);

    option_data->type = OPTION_BOOL;
    option_data->root_element = radio_area;
    option_data->input_element = radio;
    randoFinishOption(option_data, randoLookupDescription(option_id));
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
    RecompuiResource slider_area = randoYAMLCreateMenuEntryArea(menu->context, menu->current_body);

    randoYAMLCreateSettingLabel(menu->context, slider_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id);

    RecompuiResource slider = recompui_create_slider(menu->context, slider_area, SLIDERTYPE_INTEGER, (float)min, (float)max, (float)step, (float)default_value);
    recompui_set_max_width(slider, 600.0f, UNIT_DP);

    option_data->type = OPTION_INT_SLIDER;
    option_data->root_element = slider_area;
    option_data->input_element = slider;
    randoFinishOption(option_data, randoLookupDescription(option_id));
    return option_data;
}

RandoOptionData* randoCreateFloatSliderOption(RandoYamlConfigMenu* menu, const char* option_id, const char* display_name,
    float min, float max, float step, float default_value) {
    RecompuiResource slider_area = randoYAMLCreateMenuEntryArea(menu->context, menu->current_body);

    randoYAMLCreateSettingLabel(menu->context, slider_area, display_name);

    RandoOptionData* option_data = randoAllocateOption(menu, option_id);

    RecompuiResource slider = recompui_create_slider(menu->context, slider_area, SLIDERTYPE_NUMBER, min, max, step, default_value);
    recompui_set_max_width(slider, 600.0f, UNIT_DP);

    option_data->type = OPTION_FLOAT_SLIDER;
    option_data->root_element = slider_area;
    option_data->input_element = slider;
    randoFinishOption(option_data, randoLookupDescription(option_id));
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

static EnumOptionValue rando_skullsanity_options[] = {
    { "vanilla", NULL },
    { "anything", NULL },
    { "ignore", NULL },
};

static EnumOptionValue rando_shopsanity_options[] = {
    { "vanilla", NULL },
    { "enabled", NULL },
    { "advanced", NULL },
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

static EnumOptionValue rando_grasssanity_options[] = {
    { "off", NULL },
    { "normal", NULL },
    { "no_termina_field", "No Termina" },
    { "grotto_and_cave_only", "Grotto/Cave" },
    { "dungeon_only", "Dungeons" },
};

static EnumOptionValue rando_boss_souls_options[] = {
    { "false", "Off" },
    { "true", "On" },
    { "true_include_majora", "On (Include Majora)" },
};

// Tabs: one button in the header bar, one body panel. Only the active panel.

static RecompuiColor tab_active_color   = {255, 255, 255, 255};
static RecompuiColor tab_inactive_color = {255, 255, 255, 110};

// Panel that section buttons parent into; set by randoBeginTab.
static RecompuiResource rando_current_panel = 0;

static void randoSelectTab(RandoYamlConfigMenu* menu, u32 index) {
    for (u32 i = 0; i < menu->num_tabs; i++) {
        bool active = (i == index);
        recompui_set_display(menu->tabs[i].panel, active ? DISPLAY_BLOCK : DISPLAY_NONE);
        recompui_set_color(menu->tabs[i].button, active ? &tab_active_color : &tab_inactive_color);
    }
    menu->active_tab = index;
}

static void randoTabButtonCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        RandoTab* tab = (RandoTab*)userdata;
        randoSelectTab(tab->menu, tab->index);
    }
}

static RecompuiResource randoBeginTab(RandoYamlConfigMenu* menu, const char* label) {
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

static void randoTabPlaceholder(RandoYamlConfigMenu* menu, const char* text) {
    RecompuiResource label = recompui_create_label(menu->context, menu->current_body, text, LABELSTYLE_NORMAL);
    recompui_set_padding(label, 24.0f, UNIT_DP);
}

// Collapsible section: a button toggling a wrapper of options. Options are built
// once at startup and shown/hidden, so YAML output is unaffected hopefully.
#define MAX_SECTIONS 16
typedef struct {
    RecompuiResource button;
    RecompuiResource wrapper;
    const char*      title;
    bool             open;
} RandoSection;
static RandoSection rando_sections[MAX_SECTIONS];
static u32 rando_num_sections = 0;

static void randoSectionSetButtonText(RandoSection* section) {
    char buf[64];
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

static void randoSectionToggleCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        RandoSection* section = (RandoSection*)userdata;
        section->open = !section->open;
        recompui_set_display(section->wrapper, section->open ? DISPLAY_BLOCK : DISPLAY_NONE);
        randoSectionSetButtonText(section);
    }
}

static void randoBeginSection(RandoYamlConfigMenu* menu, const char* title) {
    RandoSection* section = &rando_sections[rando_num_sections++];
    section->title = title;
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
    recompui_set_justify_content(header_top, JUSTIFY_CONTENT_SPACE_BETWEEN);
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
    recompui_set_padding(yaml_config_menu.option_column, 16.0f, UNIT_DP);

    RecompuiColor pane_divider_color = {255, 255, 255, 40};
    yaml_config_menu.description_pane = recompui_create_label(yaml_config_menu.context, yaml_config_menu.body, "", LABELSTYLE_NORMAL);
    recompui_set_display(yaml_config_menu.description_pane, DISPLAY_BLOCK);
    recompui_set_flex_basis(yaml_config_menu.description_pane, 28.0f, UNIT_PERCENT);
    recompui_set_flex_grow(yaml_config_menu.description_pane, 1.0f);
    recompui_set_flex_shrink(yaml_config_menu.description_pane, 1.0f);
    recompui_set_padding(yaml_config_menu.description_pane, 20.0f, UNIT_DP);
    recompui_set_border_left_width(yaml_config_menu.description_pane, 1.1f, UNIT_DP);
    recompui_set_border_color(yaml_config_menu.description_pane, &pane_divider_color);

    yaml_config_menu.num_options = 0;
    yaml_config_menu.num_tabs = 0;
    rando_num_sections = 0;

    // General tab.
    randoBeginTab(&yaml_config_menu, "General");

    randoBeginSection(&yaml_config_menu, "General");
    randoCreateRadioOption(&yaml_config_menu, "accessibility", "Accessibility:", rando_accessibility_options, ARRAY_COUNT(rando_accessibility_options), RANDO_ACCESSABILITY_FULL);
    randoCreateRadioOption(&yaml_config_menu, "logic_difficulty", "Logic Difficulty:", rando_logic_difficulty_options, ARRAY_COUNT(rando_logic_difficulty_options), RANDO_LOGIC_DIFFICULTY_NORMAL);
    randoCreateBoolPropOption(&yaml_config_menu, "camc", "Chests Match Contents:", true);
    randoCreateBoolPropOption(&yaml_config_menu, "magic_is_a_trap", "Magic is a Trap:", false);
    randoCreateIntSliderOption(&yaml_config_menu, "hint_percentage", "Useful Hint Percentage:", 0, 100, 1, 70);
    randoCreateRadioOption(&yaml_config_menu, "damage_multiplier", "Damage Multiplier:", rando_damage_multiplier_options, ARRAY_COUNT(rando_damage_multiplier_options), RANDO_DAMAGE_MULITPLIER_NORMAL);
    randoCreateRadioOption(&yaml_config_menu, "death_behavior", "Death Behavior:", rando_death_behavior_options, ARRAY_COUNT(rando_death_behavior_options), RANDO_DEATH_BEHAVIOR_VANILLA);
    randoCreateBoolPropOption(&yaml_config_menu, "death_link", "Death Link:", false);
    randoCreateCallbackBoolPropOption(&yaml_config_menu, "Randomize Tunic Color:", true, tunicColorCallback)
        ->description = "Randomize the color of Link's tunic.";

    randoBeginSection(&yaml_config_menu, "Goals");
    randoCreateBoolPropOption(&yaml_config_menu, "completion_goal", "Completion Goal (100%):", false);
    randoCreateIntSliderOption(&yaml_config_menu, "moon_remains_required", "Moon: Boss Remains Required:", 0, 4, 1, 4);
    randoCreateIntSliderOption(&yaml_config_menu, "moon_masks_required", "Moon: Masks Required:", 0, 24, 1, 0);
    randoCreateBoolPropOption(&yaml_config_menu, "moon_star_fox", "Moon: Star Fox Masks:", false);
    randoCreateIntSliderOption(&yaml_config_menu, "moon_owls_required", "Moon: Owls Required:", 0, 10, 1, 0);
    randoCreateIntSliderOption(&yaml_config_menu, "moon_scarecrows_required", "Moon: Scarecrows Required:", 0, 14, 1, 0);
    randoCreateIntSliderOption(&yaml_config_menu, "moon_frogs_required", "Moon: Frogs Required:", 0, 5, 1, 0);
    randoCreateIntSliderOption(&yaml_config_menu, "moon_items_required", "Moon: Trade Items Required:", 0, 48, 1, 0);
    randoCreateIntSliderOption(&yaml_config_menu, "majora_remains_required", "Majora: Boss Remains Required:", 0, 4, 1, 4);
    randoCreateIntSliderOption(&yaml_config_menu, "majora_masks_required", "Majora: Masks Required:", 0, 24, 1, 0);
    randoCreateBoolPropOption(&yaml_config_menu, "majora_star_fox", "Majora: Star Fox Masks:", false);
    randoCreateIntSliderOption(&yaml_config_menu, "majora_owls_required", "Majora: Owls Required:", 0, 10, 1, 0);
    randoCreateIntSliderOption(&yaml_config_menu, "majora_scarecrows_required", "Majora: Scarecrows Required:", 0, 17, 1, 0);
    randoCreateIntSliderOption(&yaml_config_menu, "majora_frogs_required", "Majora: Frogs Required:", 0, 5, 1, 0);
    randoCreateIntSliderOption(&yaml_config_menu, "majora_items_required", "Majora: Trade Items Required:", 0, 29, 1, 0);

    randoBeginSection(&yaml_config_menu, "Shuffle");
    randoCreateBoolPropOption(&yaml_config_menu, "dungeon_entrance_rando", "Dungeon Entrance Rando:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "boss_entrance_rando", "Boss Entrance Rando:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "dungeon_chaining", "Dungeon Chaining:", false);
    randoCreateRadioOption(&yaml_config_menu, "shuffle_regional_maps", "Shuffle Regional Maps:", rando_shuffle_regional_maps_options, ARRAY_COUNT(rando_shuffle_regional_maps_options), RANDO_SHUFFLE_REGIONAL_MAPS_VANILLA);
    randoCreateRadioOption(&yaml_config_menu, "shuffle_boss_remains", "Shuffle Boss Remains:", rando_shuffle_boss_remains_options, ARRAY_COUNT(rando_shuffle_boss_remains_options), RANDO_SHUFFLE_BOSS_REMAINS_VANILLA);
    randoCreateBoolPropOption(&yaml_config_menu, "remains_allow_boss_warps", "Warp to Bosses Using Remains:", true);
    randoCreateBoolPropOption(&yaml_config_menu, "shuffle_spiderhouse_reward", "Shuffle Spiderhouse Rewards:", false);
    randoCreateIntSliderOption(&yaml_config_menu, "required_skull_tokens", "Required Skulltula Tokens:", 0, 30, 1, 30);
    randoCreateRadioOption(&yaml_config_menu, "skullsanity", "Skull-Sanity Mode:", rando_skullsanity_options, ARRAY_COUNT(rando_skullsanity_options), RANDO_SKULLSANITY_VANILLA);
    randoCreateRadioOption(&yaml_config_menu, "shopsanity", "Shop-Sanity Mode:", rando_shopsanity_options, ARRAY_COUNT(rando_shopsanity_options), RANDO_SHOPSANITY_VANILLA);
    randoCreateBoolPropOption(&yaml_config_menu, "scrubsanity", "Scrub-Sanity:", false);
    randoCreateRadioOption(&yaml_config_menu, "shop_prices", "Shop Prices:", shop_prices_options, ARRAY_COUNT(shop_prices_options), RANDO_SHOP_PRICES_VANILLA);
    randoCreateBoolPropOption(&yaml_config_menu, "cowsanity", "Cow-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "shuffle_great_fairy_rewards", "Shuffle Great Fairy Rewards:", false);
    randoCreateIntSliderOption(&yaml_config_menu, "required_stray_fairies", "Required Stray Fairies:", 0, 15, 1, 15);
    randoCreateBoolPropOption(&yaml_config_menu, "fairysanity", "Fairy-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "keysanity", "Key-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "bosskeysanity", "Boss-Key-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "curiostity_shop_trades", "Curiosity Shop Trades:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "intro_checks", "Intro Checks:", false);

    randoBeginSection(&yaml_config_menu, "Sanity");
    randoCreateRadioOption(&yaml_config_menu, "grasssanity", "Grass-Sanity:", rando_grasssanity_options, ARRAY_COUNT(rando_grasssanity_options), 0);
    randoCreateBoolPropOption(&yaml_config_menu, "potsanity", "Pot-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "hitsanity", "Hit-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "rocksanity", "Rock-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "soilsanity", "Soil-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "rupeesanity", "Rupee-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "invisisanity", "Invisi-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "snowsanity", "Snow-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "woodsanity", "Wood-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "realfairysanity", "Real-Fairy-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "iciclesanity", "Icicle-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "scarecrowsanity", "Scarecrow-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "hivesanity", "Hive-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "notebooksanity", "Notebook-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "owlsanity", "Owl-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "frogsanity", "Frog-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "treesanity", "Tree-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "flowersanity", "Flower-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "signsanity", "Sign-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "websanity", "Web-Sanity:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "oneoffs", "One-Off Locations:", false);

    randoBeginSection(&yaml_config_menu, "Souls");
    randoCreateRadioOption(&yaml_config_menu, "boss_souls", "Boss Souls:", rando_boss_souls_options, ARRAY_COUNT(rando_boss_souls_options), 0);
    randoCreateBoolPropOption(&yaml_config_menu, "npc_souls", "NPC Souls:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "enemy_souls", "Enemy Souls:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "misc_souls", "Misc Souls:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "utility_souls", "Utility Souls:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "absurd_souls", "Absurd Souls:", false);

    // Starting Items tab.
    randoBeginTab(&yaml_config_menu, "Starting Items");
    randoCreateBoolPropOption(&yaml_config_menu, "ocarinaless", "Start Ocarinaless:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "timeless", "Start Timeless:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "swordless", "Start Swordless:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "shieldless", "Start Shieldless:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "start_with_soaring", "Start with Song of Soaring:", true);
    randoCreateIntSliderOption(&yaml_config_menu, "starting_hearts", "Starting Heart Segments:", 4, 12, 1, 12);
    randoCreateRadioOption(&yaml_config_menu, "starting_hearts_are_containers_or_pieces", "Unused Starting Hearts Distributed as:", rando_starting_hearts_type_options, ARRAY_COUNT(rando_starting_hearts_type_options), RANDO_STARTING_HEARTS_ARE_CONTAINERS);
    randoCreateBoolPropOption(&yaml_config_menu, "start_with_consumables", "Start With Consumables:", true);
    randoCreateBoolPropOption(&yaml_config_menu, "permanent_chateau_romani", "Permanent Chateau Romani:", true);
    randoCreateBoolPropOption(&yaml_config_menu, "start_with_inverted_time", "Reset With Inverted Time:", false);
    randoCreateBoolPropOption(&yaml_config_menu, "receive_filled_wallets", "Receive Filled Wallets:", true);

    // Tricks tab (filled by the apworld SoonTM; empty for now).
    randoBeginTab(&yaml_config_menu, "Tricks");
    randoTabPlaceholder(&yaml_config_menu, "No tricks are available for this game yet.");

    // Glitches tab (filled by the apworld; empty for now).
    randoBeginTab(&yaml_config_menu, "Glitches");
    randoTabPlaceholder(&yaml_config_menu, "No glitches are available for this game yet.");

    // Excluded Locations tab (populated by the location-list glue).
    randoBeginTab(&yaml_config_menu, "Excluded Locations");
    randoTabPlaceholder(&yaml_config_menu, "Location list not yet loaded.");

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
}

void randoShowYamlConfigMenu() {
    recompui_show_context(yaml_config_menu.context);
    is_generate_menu_shown = true;
}