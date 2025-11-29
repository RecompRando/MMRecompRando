#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "rando_colors.h"

#include "unchecked_arrow.h"
#include "models/unchecked_arrow.h"

extern u64 scarecrow_img[];
extern u64 owl_closed_img[];
extern u64 owl_open_img[];

void Draw_UncheckedArrow(PlayState* play, Vec3f pos, f32 scale, u32 location, UncheckedArrowType type) {
    int speed = 1000;
    f32 amplitude = 5.0f;

    // set image based on type
    TexturePtr image;
    switch (type) {
        case ARROW_SCARECROW:
            image = scarecrow_img;
            break;
        case ARROW_OWL_CLOSED:
            image = owl_closed_img;
            break;
        case ARROW_OWL_OPEN:
            image = owl_open_img;
            break;
    }

    // set color based on location
    Color_RGB8 color = {255, 255, 0}; // yellow default color
    get_rando_color(&color, location);
    
    // set y pos based on sinewave
    pos.y += (Math_SinS((s16)play->gameplayFrames * speed) * amplitude) + (amplitude / 2.0f);
    Matrix_Translate(pos.x, pos.y, pos.z, MTXMODE_NEW);

    // scale
    Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);

    // billboarding
    Matrix_Mult(&play->billboardMtxF, MTXMODE_APPLY);
    Matrix_ReplaceRotation(&play->billboardMtxF);

    // normal draw code
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    // set color + image
    gDPSetPrimColor(POLY_OPA_DISP++, 0x80, 0x80, color.r, color.g, color.b, 255);
    gSPSegment(POLY_OPA_DISP++, 0x08, image);

    gSPDisplayList(POLY_OPA_DISP++, unchecked_arrow);

    CLOSE_DISPS(play->state.gfxCtx);
}