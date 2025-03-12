#include "apcommon.h"
#include "z_title_setup.h"
#include "recompui.h"
#include "sys_cfb.h"
#include "idle.h"

RECOMP_IMPORT("*", void recomp_free(void* mem));
RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

#define C_TO_PARAMS(c) (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF

RECOMP_IMPORT(".", bool rando_init(char* address, char* player_name, char* password));

RECOMP_IMPORT(".", int rando_get_tunic_color());
RECOMP_IMPORT("mm_recomp_colors", void colors_set_human_tunic(u8 r, u8 g, u8 b));

bool failed_to_connect = false;
bool connected = false;

RecompuiContext context;
RecompuiResource root;
RecompuiResource container;
RecompuiResource server_textinput;
RecompuiResource slot_textinput;
RecompuiResource connect_button;

void connect_pressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        char* server_text = recompui_get_input_text(server_textinput);
        char* slot_text = recompui_get_input_text(slot_textinput);
        recomp_printf("Connecting to %s as %s\n", server_text, slot_text);
        bool success = rando_init(server_text, slot_text, "");
        recomp_free(server_text);
        recomp_free(slot_text);

        if (success) {
            connected = true;
            colors_set_human_tunic(C_TO_PARAMS(rando_get_tunic_color()));
        }
        else {
            failed_to_connect = true;
        }
    }
}

void randoCreateMenus() {
    RecompuiColor bg_color;
    bg_color.r = 255;
    bg_color.g = 255;
    bg_color.b = 255;
    bg_color.a = 0.1f * 255;

    RecompuiColor border_color;
    border_color.r = 255;
    border_color.g = 255;
    border_color.b = 255;
    border_color.a = 0.2f * 255;

    RecompuiColor modal_color;
    modal_color.r = 8;
    modal_color.g = 7;
    modal_color.b = 13;
    modal_color.a = 0.9f * 255;

    const float body_padding = 64.0f;
    const float modal_height = RECOMPUI_TOTAL_HEIGHT - (2 * body_padding);
    const float modal_max_width = modal_height * (16.0f / 9.0f);
    const float modal_border_width = 1.1f;
    const float modal_border_radius = 16.0f;

    context = recompui_create_context();
    recompui_open_context(context);

    root = recompui_context_root(context);
    // Set up the root element so it takes up the full screen.
    recompui_set_position(root, POSITION_ABSOLUTE);
    recompui_set_top(root, 0, UNIT_DP);
    recompui_set_right(root, 0, UNIT_DP);
    recompui_set_bottom(root, 0, UNIT_DP);
    recompui_set_left(root, 0, UNIT_DP);
    recompui_set_width_auto(root);
    recompui_set_height_auto(root);

    // Set up the root element's padding so the modal contents don't touch the screen edges.
    recompui_set_padding(root, body_padding, UNIT_DP);
    recompui_set_background_color(root, &bg_color);

    // Set up the flexbox properties of the root element.
    recompui_set_flex_direction(root, FLEX_DIRECTION_COLUMN);
    recompui_set_justify_content(root, JUSTIFY_CONTENT_CENTER);

    // Create a container to act as the modal background and hold the elements in the modal.
    container = recompui_create_element(context, root);

    // Take up the full height and full width, up to a maximum width.
    recompui_set_height(container, 100.0f, UNIT_PERCENT);
    recompui_set_flex_grow(container, 1.0f);
    recompui_set_max_width(container, modal_max_width, UNIT_DP);

    // Set up the flexbox properties of the container.
    recompui_set_display(container, DISPLAY_FLEX);
    recompui_set_justify_content(container, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_flex_direction(container, FLEX_DIRECTION_ROW);
    recompui_set_padding(container, 16.0f, UNIT_DP);
    recompui_set_gap(container, 16.0f, UNIT_DP);
    recompui_set_align_items(container, ALIGN_ITEMS_BASELINE);
    
    // Set up the container to be the modal's background.
    recompui_set_border_width(container, modal_border_width, UNIT_DP);
    recompui_set_border_radius(container, modal_border_radius, UNIT_DP);
    recompui_set_border_color(container, &border_color);
    recompui_set_background_color(container, &modal_color);

    // Create a text input for the server address.
    server_textinput = recompui_create_textinput(context, container);
    recompui_set_flex_grow(server_textinput, 1.0f);
    recompui_set_font_size(server_textinput, 30.0f, UNIT_DP);
    recompui_set_input_text(server_textinput, "archipelago.gg:38281");
    
    // Create a text input for the slot.
    slot_textinput = recompui_create_textinput(context, container);
    recompui_set_flex_grow(slot_textinput, 1.0f);
    recompui_set_font_size(slot_textinput, 30.0f, UNIT_DP);
    recompui_set_input_text(slot_textinput, "Player");

    // Create the connect button.
    connect_button = recompui_create_button(context, container, "Connect", BUTTONSTYLE_SECONDARY);
    recompui_set_text_align(connect_button, TEXT_ALIGN_CENTER);
    recompui_set_flex_basis(connect_button, 150.0f, UNIT_DP);
    recompui_set_flex_grow(connect_button, 0.0f);
    recompui_set_flex_shrink(connect_button, 0.0f);

    // Bind the callback for the text input button.
    recompui_register_callback(connect_button, connect_pressed, NULL);
    
    recompui_close_context(context);
}

void Setup_InitImpl(SetupState* this);

void RandoMenu_Main(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    // Perform the original setup init after connection.
    if (connected) {
        recompui_hide_context(context);
        Setup_InitImpl(this);
    }
    // Show an error if connection failed.
    if (failed_to_connect) {
        recomp_printf("Failed to connect\n");
        // TODO show an error.
        failed_to_connect = false;
    }

    // Setup drawing just so that the renderer sees new frames coming in.
    func_8012CF0C(thisx->gfxCtx, true, false, 0, 0, 0);
}

RECOMP_PATCH void Setup_Init(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    recompui_show_context(context);
    connected = false;

    this->state.main = RandoMenu_Main;
    this->state.destroy = Setup_Destroy;
    // Move init to RandoMenu_Main.
}
