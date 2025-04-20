#include "apcommon.h"
#include "solo_menu.h"

RandoSoloMenu solo_menu;

static void backPressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        recompui_hide_context(solo_menu.context);
        randoShowStartMenu();
    }
}

void randoCreateSoloMenu() {
    solo_menu.context = recompui_create_context();
    recompui_open_context(solo_menu.context);

    createUiFrame(solo_menu.context, &solo_menu.frame);

    // Create the back button, parenting it to the root with absolute positioning.
    solo_menu.back_button = recompui_create_button(solo_menu.context, solo_menu.frame.root, "Back", BUTTONSTYLE_SECONDARY);
    recompui_set_position(solo_menu.back_button, POSITION_ABSOLUTE);
    recompui_set_left(solo_menu.back_button, 64.0f, UNIT_DP);
    recompui_set_top(solo_menu.back_button, 64.0f, UNIT_DP);
    recompui_register_callback(solo_menu.back_button, backPressed, NULL);

    recompui_close_context(solo_menu.context);
}

void randoShowSoloMenu() {
    recompui_show_context(solo_menu.context);
}
