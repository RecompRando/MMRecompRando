#include "apcommon.h"
#include "z_title_setup.h"
#include "recompui.h"
#include "sys_cfb.h"
#include "idle.h"
#include "apconnect_menu.h"
#include "yaml_generation.h"
#include "recomputils.h"

#define C_TO_PARAMS(c) (c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF

RECOMP_IMPORT(".", int rando_get_tunic_color());
RECOMP_IMPORT("mm_recomp_colors", void colors_set_human_tunic(u8 r, u8 g, u8 b));

bool rando_started = false;

void randoStart() {
    rando_started = true;
}

// Startup Menu
void Setup_InitImpl(SetupState* this);

void RandoMenu_Main(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    // Perform the original setup init after connection.
    if (rando_started) {
        colors_set_human_tunic(C_TO_PARAMS(rando_get_tunic_color()));
        recompui_hide_context(yaml_config_menu.context);
        Setup_InitImpl(this);
    }

    // Setup drawing just so that the renderer sees new frames coming in.
    func_8012CF0C(thisx->gfxCtx, true, false, 0, 0, 0);
}

RECOMP_PATCH void Setup_Init(GameState* thisx) {
    SetupState* this = (SetupState*)thisx;

    // recompui_show_context(context);
    // recompui_show_context(yaml_config_menu.context);
    randoShowYamlConfigMenu();
    recomp_printf("Context Shown\n");
    rando_started = false;

    this->state.main = RandoMenu_Main;
    this->state.destroy = Setup_Destroy;
    // Move init to RandoMenu_Main.
}
