#include "apcommon.h"
#include "z_title_setup.h"
#include "recompui.h"
#include "sys_cfb.h"
#include "idle.h"
#include "apconnect_menu.h"
#include "yaml_generation.h"
#include "menu_helpers.h"
#include "recomputils.h"
#include "recompconfig.h"

void createUiFrame(RecompuiContext context, UiFrame* frame) {
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

    const float modal_border_width = 5.1f;
    const float modal_border_radius = 16.0f;

    frame->root = recompui_context_root(context);
    // Set up the root element so it takes up the full screen.
    recompui_set_position(frame->root, POSITION_ABSOLUTE);
    recompui_set_top(frame->root, 0, UNIT_DP);
    recompui_set_right(frame->root, 0, UNIT_DP);
    recompui_set_bottom(frame->root, 0, UNIT_DP);
    recompui_set_left(frame->root, 0, UNIT_DP);
    recompui_set_width_auto(frame->root);
    recompui_set_height_auto(frame->root);

    // Set up the root element's background color so the modal contents don't touch the screen edges.
    recompui_set_background_color(frame->root, &bg_color);

    // Set up the flexbox properties of the root element.
    recompui_set_flex_direction(frame->root, FLEX_DIRECTION_COLUMN);
    recompui_set_justify_content(frame->root, JUSTIFY_CONTENT_CENTER);
    recompui_set_align_items(frame->root, ALIGN_ITEMS_CENTER);

    // Create a container to act as the modal background and hold the elements in the modal.
    frame->container = recompui_create_element(context, frame->root);

    // Set the container's size to grow based on the child elements.
    recompui_set_flex_grow(frame->container, 0.0f);
    recompui_set_flex_shrink(frame->container, 0.0f);
    recompui_set_width_auto(frame->container);
    recompui_set_height_auto(frame->container);

    // Set up the properties of the container.
    recompui_set_display(frame->container, DISPLAY_BLOCK);
    recompui_set_padding(frame->container, 16.0f, UNIT_DP);
    recompui_set_align_items(frame->container, ALIGN_ITEMS_STRETCH);
    
    // Set up the container to be the modal's background.
    recompui_set_border_width(frame->container, modal_border_width, UNIT_DP);
    recompui_set_border_radius(frame->container, modal_border_radius, UNIT_DP);
    recompui_set_border_color(frame->container, &border_color);
    recompui_set_background_color(frame->container, &modal_color);
}

#define C_TO_PARAMS(c) (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF

RECOMP_IMPORT(".", int rando_get_tunic_color());
RECOMP_IMPORT("mm_recomp_colors", void colors_set_human_tunic(u8 r, u8 g, u8 b));

RECOMP_DECLARE_EVENT(rando_on_connect());

bool rando_started = false;
bool is_multiworld = false;

void randoStart(bool multiworld) {
    rando_started = true;
    is_multiworld = multiworld;
    rando_on_connect();
    randoScout();
}

// Startup Menu
void Setup_InitImpl(SetupState* this);

void RandoMenu_Main(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    notificationUpdateCycle();

    // Perform the original setup init after connection.
    if (rando_started) {
        // Set the filename based on the seed and session type.
        char seed_name[64];
        char file_name[72];
        char slot_name[20];
        rando_get_seed_name(seed_name, sizeof(seed_name));
        rando_get_own_slot_name(slot_name);
        if (is_multiworld) {
            sprintf(file_name, "multi_%s_%s", slot_name, seed_name);
        }
        else {
            sprintf(file_name, "solo_%s", seed_name);
        }
        recomp_change_save_file(file_name);

        colors_set_human_tunic(C_TO_PARAMS(rando_get_tunic_color()));
        Setup_InitImpl(this);
    }

    // Setup drawing just so that the renderer sees new frames coming in.
    func_8012CF0C(thisx->gfxCtx, true, false, 0, 0, 0);

    // Advance RNG (used for some solo menu options).
    Rand_Next();
}

RECOMP_PATCH void Setup_Init(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    // Seed the game's RNG.
    Rand_Seed(osGetTime());

    randoShowStartMenu();
    recomp_printf("Context Shown\n");
    rando_started = false;

    this->state.main = RandoMenu_Main;
    this->state.destroy = Setup_Destroy;
    // Move init to RandoMenu_Main.
}
