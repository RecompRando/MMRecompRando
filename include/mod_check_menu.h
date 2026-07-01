#ifndef __MOD_CHECK_MENU_H__
#define __MOD_CHECK_MENU_H__

#include "apcommon.h"
#include "recompui.h"
#include "menu_helpers.h"

#define MAX_MISSING_MODS 64

typedef struct {
    RecompuiContext  context;
    UiFrame          frame;

    RecompuiResource title;
    RecompuiResource mod_list;
    RecompuiResource info_label;
    RecompuiResource continue_button;

    bool             created;
} RandoModCheckMenu;

extern RandoModCheckMenu mod_check_menu;

void randoCreateModCheckMenu();

bool randoCheckRequiredMods();

#endif