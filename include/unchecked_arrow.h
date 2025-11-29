#include "modding.h"
#include "global.h"

typedef enum {
    ARROW_SCARECROW,
    ARROW_OWL_CLOSED,
    ARROW_OWL_OPEN
} UncheckedArrowType;

void Draw_UncheckedArrow(PlayState* play, Vec3f pos, f32 scale, u32 location, UncheckedArrowType type);