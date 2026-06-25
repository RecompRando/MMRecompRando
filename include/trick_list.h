#ifndef __TRICK_LIST_H__
#define __TRICK_LIST_H__

#include "apcommon.h"
#include "recompui.h"
#include "yaml_generation.h"

#define MAX_TRICKS 512

typedef struct {
    RecompuiResource button;
    char*            name;     // owned; allocated in randoTrickListAdd
    bool             checked;
} RandoTrick;

typedef struct {
    RecompuiContext  context;
    RecompuiResource search_input;
    RecompuiResource list_container;
    RandoTrick       tricks[MAX_TRICKS];
    u32              num_tricks;
} RandoTrickList;

// Creates the search box + list container in menu->current_body.
void randoTrickListBegin(RandoYamlConfigMenu* menu, RandoTrickList* list);

// Adds one toggleable [ ] entry. Call inside a REPY loop over apworld tricks.
void randoTrickListAdd(RandoTrickList* list, const char* name);

#endif