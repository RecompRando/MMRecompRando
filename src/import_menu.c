#include "recomputils.h"
#include "recompconfig.h"

#include "apcommon.h"
#include "import_menu.h"

RandoImportMenu import_menu;

static const RecompuiColor DividerColor = { 255, 255, 255, 25 };

static void clearImportList() {
    for (u32 i = 0; i < import_menu.entry_list_size; i++) {
        ImportEntry* entry = &import_menu.entry_list[i];
        recompui_destroy_element(import_menu.list_container, entry->entry_container);
        if (entry->filename != NULL) {
            recomp_free(entry->filename);
            entry->filename = NULL;
        }
    }
    import_menu.entry_list_size = 0;
    import_menu.selected_entry = 0;
}

static const RecompuiColor EntryBorderColorDefault = { 242, 242, 242, 12 };
static const RecompuiColor EntryBorderColorHovered = { 242, 242, 242, 160 };
static const RecompuiColor EntryBorderColorSelected = { 242, 242, 242, 64 };
static const RecompuiColor EntryBorderColorHoveredSelected = { 242, 242, 242, 255 };
static const RecompuiColor EntryBackgroundColorDefault = { 242, 242, 242, 0 };
static const RecompuiColor EntryBackgroundColorSelected = { 26, 24, 32, 255 };

static void updateImportEntryStyle(ImportEntry* entry, u32 index) {
    bool hovered = entry->hovered;
    bool selected = (import_menu.selected_entry == index);
    RecompuiResource res = entry->entry_container;
    if (hovered && selected) {
        recompui_set_border_color(res, &EntryBorderColorHoveredSelected);
    }
    else if (selected) {
        recompui_set_border_color(res, &EntryBorderColorSelected);
    }
    else if (hovered) {
        recompui_set_border_color(res, &EntryBorderColorHovered);
    }
    else {
        recompui_set_border_color(res, &EntryBorderColorDefault);
    }

    if (selected) {
        recompui_set_background_color(res, &EntryBackgroundColorSelected);
    }
    else {
        recompui_set_background_color(res, &EntryBackgroundColorDefault);
    }
}

static void selectImportEntry(u32 index) {
    u32 old_index = import_menu.selected_entry;
    import_menu.selected_entry = index;
    if (old_index < import_menu.entry_list_size) {
        updateImportEntryStyle(&import_menu.entry_list[old_index], old_index);
    }
    if (index < import_menu.entry_list_size) {
        updateImportEntryStyle(&import_menu.entry_list[index], index);
    }
}

static void importEntryHandler(RecompuiResource resource, const RecompuiEventData* event, void* userdata) {
    u32 index = (u32)userdata;
    ImportEntry* entry = &import_menu.entry_list[index];
    switch (event->type) {
        case UI_EVENT_FOCUS:
            if (event->data.focus.active) {
                selectImportEntry(index);
            }
            break;
        case UI_EVENT_HOVER:
            entry->hovered = event->data.hover.active;
            break;
        default:
            break;
    }

    updateImportEntryStyle(entry, index);
}

static void createImportEntry(ImportEntry* entry, u32 index) {
    RecompuiResource container = recompui_create_element(import_menu.context, import_menu.list_container);
    entry->entry_container = container;
    recompui_set_position(container, POSITION_RELATIVE);
    recompui_set_width(container, 100, UNIT_PERCENT);
    recompui_set_padding(container, 6.0f, UNIT_DP);
    recompui_set_border_left_width(container, 2.0f, UNIT_DP);
    recompui_set_cursor(container, CURSOR_POINTER);
    recompui_set_display(container, DISPLAY_FLEX);
    recompui_set_flex_direction(container, FLEX_DIRECTION_ROW);
    recompui_set_align_items(container, ALIGN_ITEMS_CENTER);

    RecompuiResource button = recompui_create_button(import_menu.context, container, "", BUTTONSTYLE_PRIMARY);
    entry->entry_button = button;
    recompui_set_position(button, POSITION_ABSOLUTE);
    recompui_set_left(button, 0.0f, UNIT_DP);
    recompui_set_right(button, 0.0f, UNIT_DP);
    recompui_set_top(button, 0.0f, UNIT_DP);
    recompui_set_bottom(button, 0.0f, UNIT_DP);
    recompui_set_opacity(button, 0.0f);
    recompui_register_callback(button, importEntryHandler, (void*)index);

    RecompuiResource label = recompui_create_label(import_menu.context, container, entry->filename, LABELSTYLE_NORMAL);
    entry->entry_label = label;
    recompui_set_font_size(label, 22.0f, UNIT_DP);

    updateImportEntryStyle(entry, index);
}

static void buildImportList() {
    if (import_menu.entry_list_size != 0) {
        clearImportList();
    }

    rando_solo_scan_yamls();
    u32 count = rando_solo_yaml_count();
    if (count > MAX_IMPORT_ENTRIES) {
        count = MAX_IMPORT_ENTRIES;
    }
    import_menu.entry_list_size = count;

    for (u32 i = 0; i < count; i++) {
        ImportEntry* entry = &import_menu.entry_list[i];
        rando_solo_yaml_get_name(i, &entry->filename);
        entry->hovered = false;
        createImportEntry(entry, i);

        if (i == 0) {
            recompui_set_nav(entry->entry_button, NAVDIRECTION_UP, import_menu.back_button);
        }
        else {
            recompui_set_nav(entry->entry_button, NAVDIRECTION_UP, import_menu.entry_list[i - 1].entry_button);
            recompui_set_nav(import_menu.entry_list[i - 1].entry_button, NAVDIRECTION_DOWN, entry->entry_button);
        }
    }

    if (count != 0) {
        recompui_set_display(import_menu.generate_button, DISPLAY_BLOCK);
        selectImportEntry(0);
    }
    else {
        recompui_set_display(import_menu.generate_button, DISPLAY_NONE);
    }
}

static void generatePressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        if (import_menu.selected_entry >= import_menu.entry_list_size) {
            return;
        }

        REPY_FN_SETUP_RANDO;

        REPY_FN_IMPORT("rando_solo");
        REPY_FN_SET_U32("selection", import_menu.selected_entry);
        
        REPY_FN_EXEC_CACHE(
            py_rando_write_selected_yaml_to_players,
            "import shutil\n"
            "rando_solo.clear_players_folder()\n"
            "selected_file = recomp_data.solo_yamls[selection]\n"
            "print(selected_file)\n"
            "output_dir = recomp_data.mod_data_path.joinpath('Archipelago', 'local', 'Players')\n"
            "shutil.copy2(selected_file, output_dir)\n"
        );

        if (rando_solo_generate()) {
            recompui_hide_context(import_menu.context);
            recompui_close_context(import_menu.context);
            randoShowSoloMenu();
            recompui_open_context(import_menu.context);
        }
        else {
            recompui_close_context(import_menu.context);
            randoEmitErrorNotification("Failed to generate. Please report the settings you used to the developers.");
            recompui_open_context(import_menu.context);
        }

        REPY_FN_CLEANUP;
    }
}

static void refreshPressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        buildImportList();
    }
}

static void backPressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        recompui_hide_context(import_menu.context);
        // Close this context so the solo context can be opened.
        recompui_close_context(import_menu.context);
        randoShowSoloMenu();
        recompui_open_context(import_menu.context);
    }
}

void randoCreateImportMenu() {
    import_menu.context = recompui_create_context();
    recompui_open_context(import_menu.context);

    createUiFrame(import_menu.context, &import_menu.frame);

    recompui_set_width(import_menu.frame.container, 1200.0f, UNIT_DP);
    recompui_set_height(import_menu.frame.container, 800.0f, UNIT_DP);
    recompui_set_display(import_menu.frame.container, DISPLAY_FLEX);
    recompui_set_flex_direction(import_menu.frame.container, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(import_menu.frame.container, ALIGN_ITEMS_STRETCH);
    recompui_set_justify_content(import_menu.frame.container, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_padding(import_menu.frame.container, 0.0f, UNIT_DP);

    import_menu.header = recompui_create_element(import_menu.context, import_menu.frame.container);
    recompui_set_flex_grow(import_menu.header, 0.0f);
    recompui_set_flex_shrink(import_menu.header, 0.0f);
    recompui_set_display(import_menu.header, DISPLAY_FLEX);
    recompui_set_flex_direction(import_menu.header, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(import_menu.header, JUSTIFY_CONTENT_SPACE_BETWEEN);
    recompui_set_align_items(import_menu.header, ALIGN_ITEMS_CENTER);
    recompui_set_height_auto(import_menu.header);
    recompui_set_padding(import_menu.header, 16.0f, UNIT_DP);
    recompui_set_border_bottom_width(import_menu.header, 1.1f, UNIT_DP);
    recompui_set_border_bottom_color(import_menu.header, &DividerColor);

    REPY_FN_SETUP_RANDO;

    import_menu.header_label = recompui_create_label(import_menu.context, import_menu.header, "Import Settings", LABELSTYLE_LARGE);
    recompui_set_text_align(import_menu.header_label, TEXT_ALIGN_CENTER);

    import_menu.generate_button = recompui_create_button(import_menu.context, import_menu.header, "Generate", BUTTONSTYLE_PRIMARY);
    recompui_register_callback(import_menu.generate_button, generatePressed, NULL);

    import_menu.body = recompui_create_element(import_menu.context, import_menu.frame.container);
    recompui_set_flex_grow(import_menu.body, 1.0f);
    recompui_set_flex_shrink(import_menu.body, 1.0f);
    recompui_set_width(import_menu.body, 100.0f, UNIT_PERCENT);
    recompui_set_display(import_menu.body, DISPLAY_FLEX);
    recompui_set_flex_direction(import_menu.body, FLEX_DIRECTION_COLUMN);

    import_menu.list_container = recompui_create_element(import_menu.context, import_menu.body);
    recompui_set_display(import_menu.list_container, DISPLAY_BLOCK);
    recompui_set_overflow_y(import_menu.list_container, OVERFLOW_AUTO);
    recompui_set_flex_grow(import_menu.list_container, 1.0f);
    recompui_set_width(import_menu.list_container, 100.0f, UNIT_PERCENT);
    recompui_set_padding(import_menu.list_container, 16.0f, UNIT_DP);

    import_menu.footer = recompui_create_element(import_menu.context, import_menu.frame.container);
    recompui_set_flex_grow(import_menu.footer, 0.0f);
    recompui_set_flex_shrink(import_menu.footer, 0.0f);
    recompui_set_width(import_menu.footer, 100.0f, UNIT_PERCENT);
    recompui_set_display(import_menu.footer, DISPLAY_FLEX);
    recompui_set_flex_direction(import_menu.footer, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(import_menu.footer, ALIGN_ITEMS_CENTER);
    recompui_set_padding(import_menu.footer, 12.0f, UNIT_DP);
    recompui_set_border_top_width(import_menu.footer, 1.1f, UNIT_DP);
    recompui_set_border_top_color(import_menu.footer, &DividerColor);

    REPY_FN_EVAL_CACHE_STR(
        py_rando_create_yaml_import_hint,
        // showing the whole folder path would overflow, so we shorten it here
        // though, this may cause confusion if the user doesn't know this folder is in their OS's AppData equivalent (or portable location)
        "f'To import settings, place .yaml files in:\\n~/mod_data/{recomp_data.recomp_mod_id}/solo_yamls'",
        import_folder_hint
    );

    import_menu.path_label = recompui_create_label(import_menu.context, import_menu.footer, import_folder_hint, LABELSTYLE_NORMAL);
    recompui_set_text_align(import_menu.path_label, TEXT_ALIGN_CENTER);
    recompui_set_margin_bottom(import_menu.path_label, 8.0f, UNIT_DP);

    import_menu.refresh_button = recompui_create_button(import_menu.context, import_menu.footer, "Refresh", BUTTONSTYLE_SECONDARY);
    recompui_set_width(import_menu.refresh_button, 240.0f, UNIT_DP);
    recompui_set_text_align(import_menu.refresh_button, TEXT_ALIGN_CENTER);
    recompui_register_callback(import_menu.refresh_button, refreshPressed, NULL);

    import_menu.back_button = recompui_create_button(import_menu.context, import_menu.frame.root, "Back", BUTTONSTYLE_SECONDARY);
    recompui_set_position(import_menu.back_button, POSITION_ABSOLUTE);
    recompui_set_left(import_menu.back_button, 64.0f, UNIT_DP);
    recompui_set_top(import_menu.back_button, 32.0f, UNIT_DP);
    recompui_set_text_align(import_menu.back_button, TEXT_ALIGN_CENTER);
    recompui_register_callback(import_menu.back_button, backPressed, NULL);

    import_menu.entry_list_size = 0;
    import_menu.selected_entry = 0;

    recompui_close_context(import_menu.context);
    REPY_FN_CLEANUP;
}

void randoShowImportMenu() {
    recompui_open_context(import_menu.context);
    buildImportList();
    recompui_close_context(import_menu.context);

    recompui_show_context(import_menu.context);
}