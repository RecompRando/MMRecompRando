#include "apcommon.h"
#include "menu_helpers.h"
#include "start_menu.h"

RandoStartMenu start_menu;

#define RANDO_TITLE "Majora's Mask Recompiled Randomizer"

static void soloButtonClicked(RecompuiResource resource, const RecompuiEventData* event, void* userdata) {
    (void)resource;
    (void)userdata;

    if (event->type == UI_EVENT_CLICK) {
        recompui_hide_context(start_menu.context);
        // Close the start menu context temporarily so that the solo context can be opened.
        recompui_close_context(start_menu.context);
        randoShowSoloMenu();
        // Reopen the start menu context.
        recompui_open_context(start_menu.context);
    }
}

static void multiworldButtonClicked(RecompuiResource resource, const RecompuiEventData* event, void* userdata) {
    (void)resource;
    (void)userdata;

    if (event->type == UI_EVENT_CLICK) {
        recompui_hide_context(start_menu.context);
        randoShowAPConnectMenu();
    }
}

void randoCreateStartMenu() {
    start_menu.context = recompui_create_context();
    recompui_open_context(start_menu.context);

    createUiFrame(start_menu.context, &start_menu.frame);

    // Remove the left/right padding on the frame's container so that the divider line has the full width of the container.
    recompui_set_padding_left(start_menu.frame.container, 0.0f, UNIT_DP);
    recompui_set_padding_right(start_menu.frame.container, 0.0f, UNIT_DP);

    // Create the title.
    start_menu.title_label = recompui_create_label(start_menu.context, start_menu.frame.container, RANDO_TITLE, LABELSTYLE_LARGE);
    // Set the title to have left/right margins to account for having no left/right padding on the container itself.
    recompui_set_margin_left(start_menu.title_label, 16.0f, UNIT_DP);
    recompui_set_margin_right(start_menu.title_label, 16.0f, UNIT_DP);
 
    // Create the button container (row where the two buttons sit).
    start_menu.button_container = recompui_create_element(start_menu.context, start_menu.frame.container);
    recompui_set_display(start_menu.button_container, DISPLAY_FLEX);
    recompui_set_flex_direction(start_menu.button_container, FLEX_DIRECTION_ROW);
    recompui_set_gap(start_menu.button_container, 16.0f, UNIT_DP);
    recompui_set_justify_content(start_menu.button_container, JUSTIFY_CONTENT_CENTER);
    recompui_set_text_align(start_menu.button_container, TEXT_ALIGN_CENTER);
    recompui_set_border_top_width(start_menu.button_container, 1.1f, UNIT_DP);
    recompui_set_padding_top(start_menu.button_container, 16.0f, UNIT_DP);
    recompui_set_margin_top(start_menu.button_container, 16.0f, UNIT_DP);

    RecompuiColor divider_color;
    divider_color.r = 255;
    divider_color.g = 255;
    divider_color.b = 255;
    divider_color.a = 25;
    recompui_set_border_top_color(start_menu.button_container, &divider_color);

    // Create the two buttons.
    const float button_width = 200.0f;
    start_menu.solo_button = recompui_create_button(start_menu.context, start_menu.button_container, "Solo", BUTTONSTYLE_SECONDARY);
    recompui_set_flex_grow(start_menu.solo_button, 0.0f);
    recompui_set_flex_shrink(start_menu.solo_button, 0.0f);
    recompui_set_flex_basis(start_menu.solo_button, button_width, UNIT_DP);
    recompui_register_callback(start_menu.solo_button, soloButtonClicked, NULL);

    start_menu.multiworld_button = recompui_create_button(start_menu.context, start_menu.button_container, "Multiworld", BUTTONSTYLE_SECONDARY);
    recompui_set_flex_grow(start_menu.multiworld_button, 0.0f);
    recompui_set_flex_shrink(start_menu.multiworld_button, 0.0f);
    recompui_set_flex_basis(start_menu.multiworld_button, button_width, UNIT_DP);
    recompui_register_callback(start_menu.multiworld_button, multiworldButtonClicked, NULL);

    recompui_close_context(start_menu.context);
}

void randoShowStartMenu() {
    recompui_show_context(start_menu.context);
}
