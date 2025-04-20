#ifndef __START_MENU_H__
#define __START_MENU_H__

#include "recompui.h"
#include "menu_helpers.h"

typedef struct {
    RecompuiContext context;
    UiFrame frame;
    RecompuiResource title_label;
    RecompuiResource button_container;
    RecompuiResource solo_button;
    RecompuiResource multiworld_button;
} RandoStartMenu;

#endif
