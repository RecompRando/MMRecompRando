#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "rando_colors.h"
#include "models/custom_rock.h"

#include "overlays/actors/ovl_En_Ishi/z_en_ishi.h"

#define LOCATION_ROCK (0x180000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                        | randoGetLoadedActorNumInSameRoom(play, actor))

extern ActorExtensionId rockLocationExtension;
extern u32* rockLocation;

extern Gfx gameplay_field_keep_DL_0066B0[];
extern Gfx gameplay_field_keep_DL_006760[];
extern Gfx gameplay_field_keep_DL_0061E8[];
extern Gfx gSmallRockDL[];

u16 rockTex[1024];
u16 rockTex2[1024];
// u16 boulderTex[1024]; // can recolor without new texture
u16 tempRockTex[1024];
extern u16 gameplay_field_keep_Tex_006810[];
extern u16 gSmallRockTex[];

RECOMP_HOOK("EnIshi_Init")
void EnIshi_GrabTextures(Actor* thisx, PlayState* play) {
    Lib_MemCpy(tempRockTex, SEGMENTED_TO_K0(gameplay_field_keep_Tex_006810), sizeof(tempRockTex));
    RGBA16toIA16_Texture(tempRockTex, rockTex, ARRAY_COUNT(tempRockTex), GRAYSCALE_OOTMM);
    Lib_MemCpy(tempRockTex, SEGMENTED_TO_K0(gSmallRockTex), sizeof(tempRockTex));
    RGBA16toIA16_Texture(tempRockTex, rockTex2, ARRAY_COUNT(tempRockTex), GRAYSCALE_OOTMM);
}

// EnIshi_DrawGameplayKeepSmallRock
RECOMP_PATCH void func_8095F210(EnIshi* this, PlayState* play) {
    s32 pad;
    s32 sp28;

    Color_RGB8 color;
    rockLocation = z64recomp_get_extended_actor_data(&this->actor, rockLocationExtension);

    if ((this->actor.projectedPos.z <= 1200.0f) || ((this->unk_197 & 1) && (this->actor.projectedPos.z < 1300.0f))) {
        if (!rando_get_slotdata_u32("rocksanity") || !get_rando_color(&color, *rockLocation)) {
            Gfx_DrawDListOpa(play, gameplay_field_keep_DL_0066B0);
            return;
        }

        OPEN_DISPS(play->state.gfxCtx);

        Gfx_SetupDL25_Opa(play->state.gfxCtx);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color.r, color.g, color.b, 255);
        gSPDisplayList(POLY_OPA_DISP++, randoFieldSmallRockOpaDL);

        CLOSE_DISPS(play->state.gfxCtx);
        return;
    }

    if (this->actor.projectedPos.z < 1300.0f) {
        OPEN_DISPS(play->state.gfxCtx);

        Gfx_SetupDL25_Xlu(play->state.gfxCtx);

        sp28 = (1300.0f - this->actor.projectedPos.z) * 2.55f;

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        
        if (!rando_get_slotdata_u32("rocksanity") || !get_rando_color(&color, *rockLocation)) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (s32)sp28);
            gSPDisplayList(POLY_XLU_DISP++, gameplay_field_keep_DL_006760);
        } else {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, color.r, color.g, color.b, (s32)sp28);
            gSPDisplayList(POLY_XLU_DISP++, randoFieldSmallRockXluDL);
        }

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

// unused until mods like iron goron are supported, still turns them all gray but nobody will notice lol
// EnIshi_DrawGameplayKeepBoulder
RECOMP_PATCH void func_8095F36C(EnIshi* this, PlayState* play) {
    s32 pad;

    rockLocation = z64recomp_get_extended_actor_data(&this->actor, rockLocationExtension);

    Color_RGB8 color = {255, 255, 255};
    if (rando_get_slotdata_u32("rocksanity")) {
        get_rando_color(&color, *rockLocation);
    }

    OPEN_DISPS(play->state.gfxCtx);

    if ((this->actor.projectedPos.z <= 2150.0f) || ((this->unk_197 & 1) && (this->actor.projectedPos.z < 2250.0f))) {
        this->actor.shape.shadowAlpha = 160;

        Gfx_SetupDL25_Opa(play->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x08, D_801AEFA0);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        // gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color.r, color.g, color.b, 255);
        gSPDisplayList(POLY_OPA_DISP++, gameplay_field_keep_DL_0061E8);
    } else if (this->actor.projectedPos.z < 2250.0f) {
        f32 sp20 = (2250.0f - this->actor.projectedPos.z) * 2.55f;

        this->actor.shape.shadowAlpha = sp20 * 0.627451f;

        Gfx_SetupDL25_Xlu(play->state.gfxCtx);

        gSPSegment(POLY_XLU_DISP++, 0x08, D_801AEF88);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        // gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (s32)sp20);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, color.r, color.g, color.b, (s32)sp20);
        gSPDisplayList(POLY_XLU_DISP++, gameplay_field_keep_DL_0061E8);
    } else {
        this->actor.shape.shadowAlpha = 0;
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

// likely redundant
// EnIshi_DrawIshiObject
RECOMP_PATCH void func_8095F654(Actor* thisx, PlayState* play) {
    Color_RGB8 color;
    rockLocation = z64recomp_get_extended_actor_data(thisx, rockLocationExtension);

    if (!rando_get_slotdata_u32("rocksanity") || !get_rando_color(&color, *rockLocation)) {
        Gfx_DrawDListOpa(play, gSmallRockDL);
        return;
    }

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color.r, color.g, color.b, 255);
    gSPDisplayList(POLY_OPA_DISP++, randoSmallRockDL);

    CLOSE_DISPS(play->state.gfxCtx);
}