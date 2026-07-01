#include "recomputils.h"
#include "apcommon.h"
#include "yaml_generation.h"
#include "trick_list.h"
#include "libc/string.h"

// A [X]/[ ] checklist with a search filter, for tabs whose entries come from
// the apworld (tricks, glitches).

static void trickSetButtonText(RandoTrick* trick) {
    char buf[128];
    buf[0] = '[';
    buf[1] = trick->checked ? 'X' : ' ';
    buf[2] = ']';
    buf[3] = ' ';
    u32 i = 0;
    while (trick->name[i] != '\0' && i < sizeof(buf) - 5) {
        buf[4 + i] = trick->name[i];
        i++;
    }
    buf[4 + i] = '\0';
    recompui_set_text(trick->button, buf);
}

static void trickToggleCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        RandoTrick* trick = (RandoTrick*)userdata;
        trick->checked = !trick->checked;
        trickSetButtonText(trick);
    }
}

static bool strContainsCI(const char* haystack, const char* needle) {
    if (needle[0] == '\0') {
        return true;
    }
    for (u32 i = 0; haystack[i] != '\0'; i++) {
        u32 j = 0;
        while (haystack[i + j] != '\0' && needle[j] != '\0') {
            char a = haystack[i + j];
            char b = needle[j];
            if (a >= 'A' && a <= 'Z') a += 32;
            if (b >= 'A' && b <= 'Z') b += 32;
            if (a != b) break;
            j++;
        }
        if (needle[j] == '\0') {
            return true;
        }
    }
    return false;
}

static void trickSearchCallback(RecompuiResource button, const RecompuiEventData* data, void* userdata) {
    if (data->type == UI_EVENT_CLICK) {
        RandoTrickList* list = (RandoTrickList*)userdata;
        char* query = recompui_get_input_text(list->search_input);
        for (u32 i = 0; i < list->num_tricks; i++) {
            bool match = strContainsCI(list->tricks[i].name, query);
            recompui_set_display(list->tricks[i].button, match ? DISPLAY_BLOCK : DISPLAY_NONE);
        }
        recomp_free(query);
    }
}

void randoTrickListBegin(RandoYamlConfigMenu* menu, RandoTrickList* list) {
    list->num_tricks = 0;
    list->context = menu->context;

    RecompuiResource search_row = recompui_create_element(menu->context, menu->current_body);
    recompui_set_display(search_row, DISPLAY_FLEX);
    recompui_set_flex_direction(search_row, FLEX_DIRECTION_ROW);
    recompui_set_align_items(search_row, ALIGN_ITEMS_CENTER);
    recompui_set_gap(search_row, 8.0f, UNIT_DP);
    recompui_set_padding(search_row, 8.0f, UNIT_DP);

    list->search_input = recompui_create_textinput(menu->context, search_row);
    recompui_set_flex_grow(list->search_input, 1.0f);

    RecompuiResource search_button = recompui_create_button(menu->context, search_row, "Search", BUTTONSTYLE_SECONDARY);
    recompui_register_callback(search_button, trickSearchCallback, list);

    list->list_container = recompui_create_element(menu->context, menu->current_body);
    recompui_set_display(list->list_container, DISPLAY_BLOCK);
    recompui_set_padding(list->list_container, 8.0f, UNIT_DP);
}

void randoTrickListAdd(RandoTrickList* list, const char* name) {
    if (list->num_tricks >= MAX_TRICKS) {
        return;
    }
    RandoTrick* trick = &list->tricks[list->num_tricks++];

    size_t len = strlen(name);
    trick->name = recomp_alloc(len + 1);
    Lib_MemCpy(trick->name, (void*)name, len + 1);
    trick->checked = false;

    trick->button = recompui_create_button(list->context, list->list_container, "", BUTTONSTYLE_SECONDARY);
    recompui_set_display(trick->button, DISPLAY_BLOCK);
    recompui_set_margin_bottom(trick->button, 2.0f, UNIT_DP);
    recompui_register_callback(trick->button, trickToggleCallback, trick);
    trickSetButtonText(trick);
}