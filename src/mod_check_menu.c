#include "recomputils.h"
#include "recompconfig.h"

#include "apcommon.h"
#include "mod_check_menu.h"
#include "libc/string.h"

RandoModCheckMenu mod_check_menu;

static const RecompuiColor DividerColor = { 255, 255, 255, 25 };

static u32 randoModStubMissingCount() {
    return 0;
}

static void randoModStubMissingName(u32 index, char** out_str) {
    static const char* names[] = { "example_required_mod", "another_mod" };
    const char* src = (index < ARRAY_COUNT(names)) ? names[index] : "";
    u32 len = strlen(src) + 1;
    char* buf = recomp_alloc(len);
    Lib_MemCpy(buf, (void*)src, len);
    *out_str = buf;
}

static void continuePressed(RecompuiResource resource, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        recompui_hide_context(mod_check_menu.context);
        randoStart(true);
    }
}

static void clearModList() {
    recompui_destroy_element(mod_check_menu.frame.container, mod_check_menu.mod_list);
    mod_check_menu.mod_list = recompui_create_element(mod_check_menu.context, mod_check_menu.frame.container);
    recompui_set_display(mod_check_menu.mod_list, DISPLAY_FLEX);
    recompui_set_flex_direction(mod_check_menu.mod_list, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(mod_check_menu.mod_list, ALIGN_ITEMS_CENTER);
    recompui_set_flex_grow(mod_check_menu.mod_list, 1.0f);
    recompui_set_padding(mod_check_menu.mod_list, 16.0f, UNIT_DP);
}

void randoCreateModCheckMenu() {
    mod_check_menu.context = recompui_create_context();
    recompui_open_context(mod_check_menu.context);

    createUiFrame(mod_check_menu.context, &mod_check_menu.frame);

    recompui_set_width(mod_check_menu.frame.container, 1000.0f, UNIT_DP);
    recompui_set_height(mod_check_menu.frame.container, 600.0f, UNIT_DP);
    recompui_set_display(mod_check_menu.frame.container, DISPLAY_FLEX);
    recompui_set_flex_direction(mod_check_menu.frame.container, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(mod_check_menu.frame.container, ALIGN_ITEMS_CENTER);
    recompui_set_justify_content(mod_check_menu.frame.container, JUSTIFY_CONTENT_FLEX_START);
    recompui_set_padding(mod_check_menu.frame.container, 24.0f, UNIT_DP);

    mod_check_menu.title = recompui_create_label(mod_check_menu.context, mod_check_menu.frame.container, "Missing Required Mod(s)", LABELSTYLE_LARGE);
    recompui_set_text_align(mod_check_menu.title, TEXT_ALIGN_CENTER);
    recompui_set_margin_bottom(mod_check_menu.title, 16.0f, UNIT_DP);
    recompui_set_border_bottom_width(mod_check_menu.title, 1.1f, UNIT_DP);
    recompui_set_border_bottom_color(mod_check_menu.title, &DividerColor);

    mod_check_menu.mod_list = recompui_create_element(mod_check_menu.context, mod_check_menu.frame.container);
    recompui_set_display(mod_check_menu.mod_list, DISPLAY_FLEX);
    recompui_set_flex_direction(mod_check_menu.mod_list, FLEX_DIRECTION_COLUMN);
    recompui_set_align_items(mod_check_menu.mod_list, ALIGN_ITEMS_CENTER);
    recompui_set_flex_grow(mod_check_menu.mod_list, 1.0f);
    recompui_set_padding(mod_check_menu.mod_list, 16.0f, UNIT_DP);

    mod_check_menu.info_label = recompui_create_label(mod_check_menu.context, mod_check_menu.frame.container,
        "For the intended experience with the chosen settings, please install the listed mod(s), then restart the game.",
        LABELSTYLE_NORMAL);
    recompui_set_text_align(mod_check_menu.info_label, TEXT_ALIGN_CENTER);
    recompui_set_margin_bottom(mod_check_menu.info_label, 16.0f, UNIT_DP);

    mod_check_menu.continue_button = recompui_create_button(mod_check_menu.context, mod_check_menu.frame.container, "Continue without mods", BUTTONSTYLE_SECONDARY);
    recompui_set_width(mod_check_menu.continue_button, 360.0f, UNIT_DP);
    recompui_set_text_align(mod_check_menu.continue_button, TEXT_ALIGN_CENTER);
    recompui_register_callback(mod_check_menu.continue_button, continuePressed, NULL);

    mod_check_menu.created = true;

    recompui_close_context(mod_check_menu.context);
}

bool randoCheckRequiredMods() {
    if (!mod_check_menu.created) {
        randoCreateModCheckMenu();
    }

    u32 missing = randoModStubMissingCount();
    if (missing == 0) {
        return false;  // nothing missing; caller proceeds normally
    }

    recompui_open_context(mod_check_menu.context);
    clearModList();
    if (missing > MAX_MISSING_MODS) {
        missing = MAX_MISSING_MODS;
    }
    for (u32 i = 0; i < missing; i++) {
        char* name;
        randoModStubMissingName(i, &name);
        RecompuiResource label = recompui_create_label(mod_check_menu.context, mod_check_menu.mod_list, name, LABELSTYLE_NORMAL);
        recompui_set_font_size(label, 24.0f, UNIT_DP);
        recompui_set_margin_bottom(label, 4.0f, UNIT_DP);
        recomp_free(name);
    }
    recompui_close_context(mod_check_menu.context);

    recompui_show_context(mod_check_menu.context);
    return true;  // screen shown; caller should NOT proceed to start
}