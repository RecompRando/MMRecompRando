#ifndef __SOLO_MENU_H__
#define __SOLO_MENU_H__

#include "menu_helpers.h"

typedef struct {
    RecompuiResource entry_container;
    RecompuiResource entry_button;
    RecompuiResource entry_label;
    u8 hovered;
} SeedEntry;

typedef struct {
    RecompuiContext context;
    UiFrame frame;
    // Top row that contains the title. 
    RecompuiResource header;
    // Title in the header.
    RecompuiResource title;
    // Everything below the header, contains the list container and details container.
    RecompuiResource body;
    // The list of seeds on the left side.
    RecompuiResource list_container;
    // The details of the currently selected seed on the right side.
    RecompuiResource details_container;
    // The button to start.
    RecompuiResource start_button;
    // Bottom row that contains the new seed button. 
    RecompuiResource footer;
    // Button to return to the start menu.
    RecompuiResource new_seed_button;
    // Button to return to the start menu.
    RecompuiResource back_button;

    // The list of individual entry elements.
    SeedEntry* entry_list;
    // The number of entry elements.
    u32 entry_list_size;
    // The currently selected entry's index.
    u32 selected_entry;
} RandoSoloMenu;

#endif
