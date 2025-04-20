#ifndef __APCONNECT_MENU_H__
#define __APCONNECT_MENU_H__

#include "recompui.h"

typedef struct {
    RecompuiContext context;
    RecompuiResource root;
    RecompuiResource container;

    RecompuiResource server_label;
    RecompuiResource slotname_label;
    RecompuiResource password_label;

    RecompuiResource server_textinput;
    RecompuiResource slot_textinput;
    RecompuiResource password_textinput;

    RecompuiResource connect_button;
} ApconnectMenu;

#endif
