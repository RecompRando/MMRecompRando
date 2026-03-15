#include "rando_glue.h"

REPY_REGISTER_SUBINTERPRETER(rando_interp);

REPY_ON_POST_INIT void on_repy_post_init() {
    RandoGlue_Init("mm_recomp_rando", "Majora's Mask Recompiled");
}