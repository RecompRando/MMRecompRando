#ifndef __YAML_GEN__
#define __YAML_GEN__

#include "apcommon.h"
#include "recompui.h"
#include "menu_helpers.h"

#include "modding.h"

// Raised from 64 to fit the full MM option set since its such a chonker.
#define MAX_OPTIONS 96
#define MAX_TABS 16
#define MAX_SECTIONS 16

typedef enum {
    OPTION_BOOL,
    OPTION_RADIO,
    OPTION_INT_SLIDER,
    OPTION_FLOAT_SLIDER
} RandoOptionType;

typedef void (bool_callback_t)(bool value);
typedef void (radio_callback_t)(u32 value);
typedef void (int_callback_t)(int value);
typedef void (float_callback_t)(float value);

typedef struct {
    RandoOptionType type;
    RecompuiResource root_element;
    RecompuiResource input_element;
    void* data;
    char* option_id;
    char* description;
    bool is_callback;
    union {
        bool_callback_t* bool_callback;
        radio_callback_t* radio_callback;
        int_callback_t* int_callback;
        float_callback_t* float_callback;
    };
} RandoOptionData;

typedef struct {
    const char* id;
    const char* name;
} EnumOptionValue;

// Forward declaration so RandoTab can hold a back-pointer to its menu.
typedef struct RandoYamlConfigMenu RandoYamlConfigMenu;

typedef struct {
    RandoYamlConfigMenu* menu;  
    u32                  index;  
    RecompuiResource     button; 
    RecompuiResource     panel;  
} RandoTab;

typedef struct {
    RecompuiResource button;
    RecompuiResource wrapper;
    const char*      title;
    bool             open;
} RandoSection;

struct RandoYamlConfigMenu {
    RecompuiContext context;
    UiFrame frame;
    RecompuiResource header;
    RecompuiResource header_label;
    RecompuiResource generate_button;
    RecompuiResource export_button;
    RecompuiResource body;
    RecompuiResource option_column;
    RecompuiResource description_pane;
    RandoOptionData all_options[MAX_OPTIONS];
    u32 num_options;
    RecompuiResource back_button;

    // Tab support.
    RecompuiResource tab_bar;          // horizontal row of tab buttons in header
    RandoTab         tabs[MAX_TABS];
    u32              num_tabs;
    u32              active_tab;
    RecompuiResource current_body;     // panel currently receiving new options
};

extern RandoYamlConfigMenu yaml_config_menu;

#endif