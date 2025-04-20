#ifndef __MENU_HELPERS_H__
#define __MENU_HELPERS_H__

#include "recompui.h"

typedef struct {
    RecompuiResource root;
    RecompuiResource container;
} UiFrame;

void createUiFrame(RecompuiContext context, UiFrame* frame);

#endif
