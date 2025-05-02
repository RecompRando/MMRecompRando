#include "recomputils.h"
#include "recompconfig.h"

#include "apcommon.h"
#include "solo_menu.h"

RECOMP_IMPORT(".", void rando_scan_solo_seeds(const unsigned char* save_filename));
RECOMP_IMPORT(".", u32 rando_solo_count());
// Returns the actual string length
RECOMP_IMPORT(".", u32 rando_solo_get_seed_name(u32 seed_index, char* out, u32 max_length));
// Returns the actual string length
RECOMP_IMPORT(".", u32 rando_solo_get_generation_date(u32 seed_index, char* out, u32 max_length));

RECOMP_IMPORT(".", u32 rando_init_solo(u32 seed_index));

RandoSoloMenu solo_menu;

void clearSoloList() {
    for (u32 i = 0; i < solo_menu.entry_list_size; i++) {
        SeedEntry* cur_entry = &solo_menu.entry_list[i];
        recompui_destroy_element(solo_menu.list_container, cur_entry->entry_container);
    }
    recomp_free(solo_menu.entry_list);
    solo_menu.entry_list = NULL;
    solo_menu.entry_list_size = 0;
    solo_menu.selected_entry = 0;
}

static const RecompuiColor EntryBorderColorDefault = { 242, 242, 242, 12 };
static const RecompuiColor EntryBorderColorHovered = { 242, 242, 242, 160 };
static const RecompuiColor EntryBorderColorSelected = { 242, 242, 242, 64 };
static const RecompuiColor EntryBorderColorHoveredSelected = { 242, 242, 242, 255 };

static const RecompuiColor EntryBackgroundColorDefault = { 242, 242, 242, 0 };
static const RecompuiColor EntryBackgroundColorSelected = { 26, 24, 32, 255 };

static const RecompuiColor DividerColor = { 255, 255, 255, 25 };

void updateEntryStyle(SeedEntry* entry, u32 index) {
    bool hovered = entry->hovered;
    bool selected = solo_menu.selected_entry == index;
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

void selectEntry(u32 index) {
    u32 old_index = solo_menu.selected_entry;
    solo_menu.selected_entry = index;

    if (old_index < solo_menu.entry_list_size) {
        updateEntryStyle(&solo_menu.entry_list[old_index], old_index);
    }

    if (index < solo_menu.entry_list_size) {
        char label_buffer[128];
        char text_buffer[64];
        
        rando_solo_get_generation_date(index, text_buffer, sizeof(text_buffer));
        sprintf(label_buffer, "Generated: %s\n", text_buffer);
        recompui_set_text(solo_menu.details_time, label_buffer);

        rando_solo_get_seed_name(index, text_buffer, sizeof(text_buffer));
        sprintf(label_buffer, "Seed: %s\n", text_buffer);
        recompui_set_text(solo_menu.details_seedname, label_buffer);
        
        recompui_set_nav(solo_menu.start_button, NAVDIRECTION_LEFT, solo_menu.entry_list[index].entry_button);
    }
    else {
        recompui_set_nav(solo_menu.start_button, NAVDIRECTION_LEFT, solo_menu.back_button);
    }
}

void entrySelectedHandler(RecompuiResource resource, const RecompuiEventData* event, void* userdata) {
    u32 index = (u32)userdata;
    SeedEntry* entry = &solo_menu.entry_list[index];
    RecompuiResource entry_container = entry->entry_container; 

    switch (event->type) {
        // case UI_EVENT_CLICK:
        //     selectEntry(index);
        //     break;
        case UI_EVENT_FOCUS:
            if (event->data.focus.active) {
                selectEntry(index);
            }
            break;
        case UI_EVENT_HOVER:
            entry->hovered = event->data.hover.active;
            break;
        default:
            break;
    }

    updateEntryStyle(entry, index);
}

void createSoloListEntry(SeedEntry* entry, u32 index) {
    static const RecompuiColor SoloEntryBgColor = { 26, 24, 32, 255 };
    // static const RecompuiColor SoloEntryBorderColorDefault = { 242, 242, 242, 12 };
    static const RecompuiColor SoloEntryColorTextDefault = { 242, 242, 242, 255 };
    
    RecompuiResource cur_container = recompui_create_element(solo_menu.context, solo_menu.list_container);
    entry->entry_container = cur_container;
    
    recompui_set_position(cur_container, POSITION_RELATIVE);
    recompui_set_width(cur_container, 100, UNIT_PERCENT);
    recompui_set_height(cur_container, 30, UNIT_DP);
    recompui_set_padding(cur_container, 4.0f, UNIT_DP);
    recompui_set_border_left_width(cur_container, 2.0f, UNIT_DP);
    recompui_set_cursor(cur_container, CURSOR_POINTER);
    recompui_set_color(cur_container, &SoloEntryColorTextDefault);
    recompui_set_display(cur_container, DISPLAY_FLEX);
    recompui_set_flex_direction(cur_container, FLEX_DIRECTION_ROW);
    recompui_set_align_items(cur_container, ALIGN_ITEMS_CENTER);

    // Create an invisible button on top of the container.
    // Use absolute positioning and make it take up the entire size of the container.
    RecompuiResource cur_button = recompui_create_button(solo_menu.context, cur_container, "", BUTTONSTYLE_PRIMARY);
    entry->entry_button = cur_button;
    recompui_set_position(cur_button, POSITION_ABSOLUTE);
    recompui_set_left(cur_button, 0.0f, UNIT_DP);
    recompui_set_right(cur_button, 0.0f, UNIT_DP);
    recompui_set_top(cur_button, 0.0f, UNIT_DP);
    recompui_set_bottom(cur_button, 0.0f, UNIT_DP);
    recompui_set_opacity(cur_button, 0.0f);
    recompui_register_callback(cur_button, entrySelectedHandler, (void*)index);

    char datestr[64];
    rando_solo_get_generation_date(index, datestr, sizeof(datestr));
    RecompuiResource cur_label = recompui_create_label(solo_menu.context, cur_container, datestr, LABELSTYLE_NORMAL);
    entry->entry_label = cur_label;
    recompui_set_font_size(cur_label, 20.0f, UNIT_DP);

    recompui_set_nav(cur_button, NAVDIRECTION_RIGHT, solo_menu.start_button);

    updateEntryStyle(entry, index);
}

void createSoloList() {
    if (solo_menu.entry_list != NULL) {
        clearSoloList();
    }

    u8* save_path = recomp_get_save_file_path();
    rando_scan_solo_seeds(save_path);
    recomp_free(save_path);
    solo_menu.entry_list_size = rando_solo_count();
    if (solo_menu.entry_list_size != 0) {
        solo_menu.entry_list = (SeedEntry*)recomp_alloc(sizeof(solo_menu.entry_list[0]) * solo_menu.entry_list_size);

        for (u32 i = 0; i < solo_menu.entry_list_size; i++) {
            SeedEntry* cur_entry = &solo_menu.entry_list[i];
            createSoloListEntry(cur_entry, i);
            
            if (i == 0) {
                recompui_set_nav(cur_entry->entry_button, NAVDIRECTION_UP, solo_menu.back_button);
                recompui_set_nav(solo_menu.back_button, NAVDIRECTION_DOWN, cur_entry->entry_button);
            }
            else {
                recompui_set_nav(cur_entry->entry_button, NAVDIRECTION_UP, solo_menu.entry_list[i - 1].entry_button);
                recompui_set_nav(solo_menu.entry_list[i - 1].entry_button, NAVDIRECTION_DOWN, cur_entry->entry_button);
            }
        }

        u32 last_entry_index = solo_menu.entry_list_size - 1;
        recompui_set_nav(solo_menu.new_seed_button, NAVDIRECTION_UP, solo_menu.entry_list[last_entry_index].entry_button);
        recompui_set_nav(solo_menu.entry_list[last_entry_index].entry_button, NAVDIRECTION_DOWN, solo_menu.new_seed_button);
        recompui_set_display(solo_menu.start_button, DISPLAY_BLOCK);

        selectEntry(0);
    }
    else {
        recompui_set_nav(solo_menu.new_seed_button, NAVDIRECTION_UP, solo_menu.back_button);
        recompui_set_nav(solo_menu.back_button, NAVDIRECTION_DOWN, solo_menu.new_seed_button);
        recompui_set_display(solo_menu.start_button, DISPLAY_NONE);
    }
}

static void newSeedPressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        recompui_hide_context(solo_menu.context);
        randoShowYamlConfigMenu();
    }
}

static void backPressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        recompui_hide_context(solo_menu.context);
        randoShowStartMenu();
    }
}

static void startPressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        if (rando_init_solo(solo_menu.selected_entry)) {
            recomp_printf("Started successfully\n");
            recompui_hide_context(solo_menu.context);
            randoStart(false);
        }
        else {
            recomp_printf("Failed to start solo\n");
            recompui_close_context(solo_menu.context);
            randoEmitErrorNotification("Failed to load seed, file may be corrupted");
            recompui_open_context(solo_menu.context);
        }
    }
}

void randoCreateSoloMenu() {

    solo_menu.context = recompui_create_context();
    recompui_open_context(solo_menu.context);

    createUiFrame(solo_menu.context, &solo_menu.frame);
    
    // Adjust the container's properties.
    recompui_set_width(solo_menu.frame.container, 1200.0f, UNIT_DP);
    recompui_set_height(solo_menu.frame.container, 800.0f, UNIT_DP);
    recompui_set_display(solo_menu.frame.container, DISPLAY_FLEX);
    recompui_set_flex_direction(solo_menu.frame.container, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(solo_menu.frame.container, ALIGN_ITEMS_BASELINE);
    recompui_set_justify_content(solo_menu.frame.container, JUSTIFY_CONTENT_FLEX_START);

    // Remove the padding on the frame's container so that the divider line has the full width of the container.
    recompui_set_padding(solo_menu.frame.container, 0.0f, UNIT_DP);
    
    // Create the header.
    solo_menu.header = recompui_create_element(solo_menu.context, solo_menu.frame.container);
    recompui_set_flex_grow(solo_menu.header, 0.0f);
    recompui_set_flex_shrink(solo_menu.header, 1.0f);
    recompui_set_height_auto(solo_menu.header);
    recompui_set_width(solo_menu.header, 100.0f, UNIT_PERCENT);
    recompui_set_padding_top(solo_menu.header, 16.0f, UNIT_DP);
    recompui_set_padding_bottom(solo_menu.header, 16.0f, UNIT_DP);
    recompui_set_border_bottom_width(solo_menu.header, 1.1f, UNIT_DP);
    recompui_set_border_bottom_color(solo_menu.header, &DividerColor);

    // Set the header to be a row flexbox with center alignment in both directions.
    recompui_set_align_items(solo_menu.header, ALIGN_ITEMS_CENTER);
    recompui_set_display(solo_menu.header, DISPLAY_FLEX);
    recompui_set_flex_direction(solo_menu.header, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(solo_menu.header, JUSTIFY_CONTENT_CENTER);
    recompui_set_text_align(solo_menu.header, TEXT_ALIGN_CENTER);

    // Create the title.
    solo_menu.title = recompui_create_label(solo_menu.context, solo_menu.header, "Previous Sessions", LABELSTYLE_LARGE);

    // Create the body.
    solo_menu.body = recompui_create_element(solo_menu.context, solo_menu.frame.container);

    // Set the body to take up the remaining height.
    recompui_set_flex_grow(solo_menu.body, 1.0f);
    recompui_set_flex_shrink(solo_menu.body, 1.0f);
    recompui_set_width(solo_menu.body, 100.0f, UNIT_PERCENT);

    // Set the body to be a horizontal flexbox so the list can be on the left and the details can be on the right.
    recompui_set_display(solo_menu.body, DISPLAY_FLEX);
    recompui_set_justify_content(solo_menu.body, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_flex_direction(solo_menu.body, FLEX_DIRECTION_ROW);

    // Create the list container.
    solo_menu.list_container = recompui_create_element(solo_menu.context, solo_menu.body);

    // Set the list container to use flex basis to determine the size.
    recompui_set_flex_basis(solo_menu.list_container, 100.0f, UNIT_PERCENT);
    
    // Set the list container's properties.
    static const RecompuiColor ListContainerBackgroundColor = { 0, 0, 0, 89 };
    recompui_set_display(solo_menu.list_container, DISPLAY_BLOCK);
    recompui_set_overflow_y(solo_menu.list_container, OVERFLOW_AUTO);
    recompui_set_height(solo_menu.list_container, 100.0f, UNIT_PERCENT);
    recompui_set_max_height(solo_menu.list_container, 100.0f, UNIT_PERCENT);
    recompui_set_background_color(solo_menu.list_container, &ListContainerBackgroundColor);

    // Create the details container.
    solo_menu.details_container = recompui_create_element(solo_menu.context, solo_menu.body);

    // Set the details container to use flex basis to determine the size.
    recompui_set_flex_basis(solo_menu.details_container, 200.0f, UNIT_PERCENT);
    
    // Set the details container's properties.
    recompui_set_display(solo_menu.details_container, DISPLAY_FLEX);
    recompui_set_flex_direction(solo_menu.details_container, FLEX_DIRECTION_COLUMN);
    recompui_set_height(solo_menu.details_container, 300.0f, UNIT_PERCENT);
    recompui_set_max_height(solo_menu.details_container, 100.0f, UNIT_PERCENT);
    
    // Create the divisions of the details container.
    solo_menu.details_header = recompui_create_element(solo_menu.context, solo_menu.details_container);
    recompui_set_flex_grow(solo_menu.details_header, 0.0f);
    recompui_set_flex_shrink(solo_menu.details_header, 0.0f);
    recompui_set_padding(solo_menu.details_header, 8.0f, UNIT_DP);

    solo_menu.details_body = recompui_create_element(solo_menu.context, solo_menu.details_container);
    recompui_set_flex_grow(solo_menu.details_body, 1.0f);
    recompui_set_flex_shrink(solo_menu.details_body, 0.0f);

    solo_menu.details_footer = recompui_create_element(solo_menu.context, solo_menu.details_container);
    recompui_set_flex_grow(solo_menu.details_footer, 0.0f);
    recompui_set_flex_shrink(solo_menu.details_footer, 0.0f);

    recompui_set_display(solo_menu.details_footer, DISPLAY_FLEX);
    recompui_set_flex_direction(solo_menu.details_footer, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(solo_menu.details_footer, JUSTIFY_CONTENT_CENTER);
    recompui_set_padding(solo_menu.details_footer, 8.0f, UNIT_DP);

    // Create the labels in the details header.
    solo_menu.details_time = recompui_create_label(solo_menu.context, solo_menu.details_header, "", LABELSTYLE_NORMAL);
    recompui_set_margin_bottom(solo_menu.details_time, 8.0f, UNIT_DP);
    solo_menu.details_seedname = recompui_create_label(solo_menu.context, solo_menu.details_header, "", LABELSTYLE_NORMAL);

    // Create the start button.
    solo_menu.start_button = recompui_create_button(solo_menu.context, solo_menu.details_footer, "Start", BUTTONSTYLE_SECONDARY);
    recompui_register_callback(solo_menu.start_button, startPressed, NULL);
    recompui_set_width(solo_menu.start_button, 300.0f, UNIT_DP);
    recompui_set_text_align(solo_menu.start_button, TEXT_ALIGN_CENTER);
    
    // Create the footer.
    solo_menu.footer = recompui_create_element(solo_menu.context, solo_menu.frame.container);
    recompui_set_flex_grow(solo_menu.footer, 0.0f);
    recompui_set_flex_shrink(solo_menu.footer, 1.0f);
    recompui_set_width(solo_menu.footer, 100.0f, UNIT_PERCENT);
    recompui_set_padding_top(solo_menu.footer, 16.0f, UNIT_DP);
    recompui_set_padding_bottom(solo_menu.footer, 16.0f, UNIT_DP);
    recompui_set_border_top_width(solo_menu.footer, 1.1f, UNIT_DP);
    recompui_set_border_top_color(solo_menu.footer, &DividerColor);

    // Set the footer to be a row flexbox with center alignment in both directions.
    recompui_set_align_items(solo_menu.footer, ALIGN_ITEMS_CENTER);
    recompui_set_display(solo_menu.footer, DISPLAY_FLEX);
    recompui_set_flex_direction(solo_menu.footer, FLEX_DIRECTION_ROW);
    recompui_set_justify_content(solo_menu.footer, JUSTIFY_CONTENT_CENTER);
    recompui_set_text_align(solo_menu.footer, TEXT_ALIGN_CENTER);

    // Create the new seed button.
    solo_menu.new_seed_button = recompui_create_button(solo_menu.context, solo_menu.footer, "New Session", BUTTONSTYLE_SECONDARY);
    recompui_set_width(solo_menu.new_seed_button, 300.0f, UNIT_DP);
    recompui_register_callback(solo_menu.new_seed_button, newSeedPressed, NULL);

    // Create the back button, parenting it to the root with absolute positioning.
    solo_menu.back_button = recompui_create_button(solo_menu.context, solo_menu.frame.root, "Back", BUTTONSTYLE_SECONDARY);
    recompui_set_position(solo_menu.back_button, POSITION_ABSOLUTE);
    recompui_set_left(solo_menu.back_button, 64.0f, UNIT_DP);
    recompui_set_top(solo_menu.back_button, 32.0f, UNIT_DP);
    recompui_register_callback(solo_menu.back_button, backPressed, NULL);

    recompui_close_context(solo_menu.context);
}

void randoShowSoloMenu() {
    recompui_open_context(solo_menu.context);
    createSoloList();
    recompui_close_context(solo_menu.context);

    recompui_show_context(solo_menu.context);
}
