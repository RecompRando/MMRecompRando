#ifndef __IMPORT_MENU_H__
#define __IMPORT_MENU_H__

#include "apcommon.h"
#include "recompui.h"
#include "menu_helpers.h"

#define MAX_IMPORT_ENTRIES 256

typedef struct {
    RecompuiResource entry_container;
    RecompuiResource entry_button;
    RecompuiResource entry_label;
    char*            filename;   // owned; freed on list clear
    bool             hovered;
} ImportEntry;

typedef struct {
    RecompuiContext  context;
    UiFrame          frame;

    RecompuiResource header;
    RecompuiResource header_label;
    RecompuiResource generate_button;

    RecompuiResource body;
    RecompuiResource list_container;

    RecompuiResource footer;
    RecompuiResource path_label;
    RecompuiResource refresh_button;
    RecompuiResource back_button;

    ImportEntry      entry_list[MAX_IMPORT_ENTRIES];
    u32              entry_list_size;
    u32              selected_entry;
} RandoImportMenu;

extern RandoImportMenu import_menu;

void randoCreateImportMenu();
void randoShowImportMenu();

#endif