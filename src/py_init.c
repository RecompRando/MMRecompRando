#include "rando_glue.h"
#include "apcommon.h"

REPY_InterpreterIndex rando_interp = 0;

REPY_ON_POST_INIT void on_repy_post_init() {
    RandoGlue_Init("mm_recomp_rando", "Majora's Mask Recompiled");

    // these menus need to be set up after the python api is initialized as python functions are called within some of them
    randoCreateStartMenu();
    randoCreateSoloMenu();
    randoCreateYamlConfigMenu();
    randoCreateAPConnectMenu();
}