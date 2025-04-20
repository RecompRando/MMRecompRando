#include "apcommon.h"
#include "apconnect_menu.h"
#include "recomputils.h"

RECOMP_IMPORT(".", bool rando_init(char* address, char* player_name, char* password));

ApconnectMenu connect_menu;

void connect_pressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        char* server_text = recompui_get_input_text(connect_menu.server_textinput);
        char* slot_text = recompui_get_input_text(connect_menu.slot_textinput);
        char* password_text = recompui_get_input_text(connect_menu.password_textinput);
        bool success = rando_init(server_text, slot_text, password_text);
        recomp_free(server_text);
        recomp_free(slot_text);
        recomp_free(password_text);

        if (success) {
            randoStart();
            recompui_hide_context(connect_menu.context);
        }
        else {
            recomp_printf("Failed to connect\n");
            // TODO show UI context for connection error
        }
    }
}

RecompuiResource create_spacer(RecompuiContext context, RecompuiResource parent, float flex_grow, float flex_shrink, float flex_basis) {
    RecompuiResource ret = recompui_create_element(context, parent);

    recompui_set_flex_basis(ret, flex_basis, UNIT_DP);
    recompui_set_flex_grow(ret, flex_grow);
    recompui_set_flex_shrink(ret, flex_shrink);

    return ret;
}

void randoCreateAPConnectMenu() {
    RecompuiColor bg_color;
    bg_color.r = 255;
    bg_color.g = 255;
    bg_color.b = 255;
    bg_color.a = 0.1f * 255;

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

    connect_menu.context = recompui_create_context();
    recompui_open_context(connect_menu.context);

    connect_menu.root = recompui_context_root(connect_menu.context);

    // Split the root into 3 columns.
    RecompuiResource outer_spacer1 = create_spacer(connect_menu.context, connect_menu.root, 1.0f, 0.0f, 0.0f);

    RecompuiResource inner_container = recompui_create_element(connect_menu.context, connect_menu.root);
    recompui_set_display(inner_container, DISPLAY_FLEX);
    recompui_set_flex_direction(inner_container, FLEX_DIRECTION_COLUMN);

    RecompuiResource outer_spacer2 = create_spacer(connect_menu.context, connect_menu.root, 1.0f, 0.0f, 0.0f);

    // Split the middle column up into 3 rows.
    RecompuiResource inner_spacer1 = create_spacer(connect_menu.context, inner_container, 1.0f, 0.0f, 0.0f);

    // Create a container to act as the modal background and hold the elements in the modal.
    connect_menu.container = recompui_create_element(connect_menu.context, inner_container);

    // Set up the flexbox properties of the container.
    recompui_set_display(connect_menu.container, DISPLAY_INLINE);
    recompui_set_justify_content(connect_menu.container, JUSTIFY_CONTENT_CENTER);
    recompui_set_padding(connect_menu.container, 16.0f, UNIT_DP);
    recompui_set_gap(connect_menu.container, 16.0f, UNIT_DP);
    recompui_set_align_items(connect_menu.container, ALIGN_ITEMS_CENTER);
    recompui_set_width(connect_menu.container, 400, UNIT_DP);
    recompui_set_height(connect_menu.container, 400, UNIT_DP);
    
    // Set up the container to be the modal's background.
    recompui_set_border_width(connect_menu.container, modal_border_width, UNIT_DP);
    recompui_set_border_radius(connect_menu.container, modal_border_radius, UNIT_DP);
    recompui_set_border_color(connect_menu.container, &border_color);
    recompui_set_background_color(connect_menu.container, &modal_color);

    // Create a label for the server address.
    connect_menu.server_label = recompui_create_label(connect_menu.context, connect_menu.container, "Server Address:Port", LABELSTYLE_NORMAL);
    recompui_set_font_size(connect_menu.server_label, 30.0f, UNIT_DP);

    // Create a text input for the server address.
    connect_menu.server_textinput = recompui_create_textinput(connect_menu.context, connect_menu.container);
    recompui_set_font_size(connect_menu.server_textinput, 30.0f, UNIT_DP);
    recompui_set_input_text(connect_menu.server_textinput, "archipelago.gg:38281");
    recompui_set_flex_basis(connect_menu.server_textinput, 150.0f, UNIT_DP);
    recompui_set_flex_grow(connect_menu.server_textinput, 0.0f);
    recompui_set_flex_shrink(connect_menu.server_textinput, 0.0f);
    recompui_set_margin_top(connect_menu.server_textinput, 10.0f, UNIT_DP);
    recompui_set_margin_bottom(connect_menu.server_textinput, 20.0f, UNIT_DP);

    // Create a label for the slotname.
    connect_menu.slotname_label = recompui_create_label(connect_menu.context, connect_menu.container, "Slotname", LABELSTYLE_NORMAL);
    recompui_set_font_size(connect_menu.slotname_label, 30.0f, UNIT_DP);

    // Create a text input for the slotname.
    connect_menu.slot_textinput = recompui_create_textinput(connect_menu.context, connect_menu.container);
    recompui_set_font_size(connect_menu.slot_textinput, 30.0f, UNIT_DP);
    recompui_set_input_text(connect_menu.slot_textinput, "Player");
    recompui_set_flex_basis(connect_menu.slot_textinput, 150.0f, UNIT_DP);
    recompui_set_flex_grow(connect_menu.slot_textinput, 0.0f);
    recompui_set_flex_shrink(connect_menu.slot_textinput, 0.0f);
    recompui_set_margin_top(connect_menu.slot_textinput, 10.0f, UNIT_DP);
    recompui_set_margin_bottom(connect_menu.slot_textinput, 20.0f, UNIT_DP);

    // Create a label for the password.
    connect_menu.password_label = recompui_create_label(connect_menu.context, connect_menu.container, "Password", LABELSTYLE_NORMAL);
    recompui_set_font_size(connect_menu.password_label, 30.0f, UNIT_DP);

    // Create a text input for the password.
    connect_menu.password_textinput = recompui_create_textinput(connect_menu.context, connect_menu.container);
    recompui_set_font_size(connect_menu.password_textinput, 30.0f, UNIT_DP);
    recompui_set_input_text(connect_menu.password_textinput, "");
    recompui_set_flex_basis(connect_menu.password_textinput, 150.0f, UNIT_DP);
    recompui_set_flex_grow(connect_menu.password_textinput, 0.0f);
    recompui_set_flex_shrink(connect_menu.password_textinput, 0.0f);
    recompui_set_margin_top(connect_menu.password_textinput, 20.0f, UNIT_DP);

    // Create the connect button.
    connect_menu.connect_button = recompui_create_button(connect_menu.context, connect_menu.container, "Connect", BUTTONSTYLE_SECONDARY);
    recompui_set_text_align(connect_menu.connect_button, TEXT_ALIGN_CENTER);
    recompui_set_flex_basis(connect_menu.connect_button, 150.0f, UNIT_DP);
    recompui_set_flex_grow(connect_menu.connect_button, 0.0f);
    recompui_set_flex_shrink(connect_menu.connect_button, 0.0f);
    recompui_set_margin_top(connect_menu.connect_button, 70.0f, UNIT_DP);

    // Bind the callback for the text input button.
    recompui_register_callback(connect_menu.connect_button, connect_pressed, NULL);

    RecompuiResource inner_spacer2 = create_spacer(connect_menu.context, inner_container, 1.0f, 0.0f, 0.0f);
    
    recompui_close_context(connect_menu.context);
}

void randoShowAPConnectMenu() {
    recompui_show_context(connect_menu.context);
}
