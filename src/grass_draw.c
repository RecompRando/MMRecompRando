#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"
#include "rt64_extended_gbi.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "rando_colors.h"
#include "grass.h"

#include "overlays/actors/ovl_Obj_Grass/z_obj_grass.h"
#include "overlays/actors/ovl_Obj_Grass_Unit/z_obj_grass_unit.h"
#include "overlays/actors/ovl_Obj_Grass_Carry/z_obj_grass_carry.h"
#include "overlays/actors/ovl_En_Kusa/z_en_kusa.h"
#include "overlays/actors/ovl_En_Kusa2/z_en_kusa2.h"

RECOMP_IMPORT(".", bool rando_get_camc_enabled());

#define LOCATION_GRASS(grassElem) ObjGrass_GetLocation(grassElem)

u32 ObjGrass_GetLocation(ObjGrassElement* grass);

extern Gfx gObjGrass_D_809AA9F0[];
extern Gfx gObjGrass_D_809AAA68[];
extern Gfx gObjGrass_D_809AAAE0[];

extern Gfx gKusaBushType1DL[];
extern Gfx gKusaBushType2DL[];

extern Gfx gKusaSproutDL[];
extern Gfx gKusaStumpDL[];

u16 grassTex[1024];
u16 grassSproutTex[1024];
u16 originalGrassTex[1024];
extern u16 gameplay_field_keep_Tex_007010[];
extern u16 gameplay_keep_Tex_052940[];

void grab_grass_texture() {
    Lib_MemCpy(originalGrassTex, SEGMENTED_TO_K0(gameplay_field_keep_Tex_007010), sizeof(originalGrassTex));
    RGBA16toIA16_Texture(originalGrassTex, grassTex, ARRAY_COUNT(originalGrassTex));
}

void grab_sprout_texture() {
    Lib_MemCpy(originalGrassTex, SEGMENTED_TO_K0(gameplay_keep_Tex_052940), sizeof(originalGrassTex));
    RGBA16toIA16_Texture(originalGrassTex, grassSproutTex, ARRAY_COUNT(originalGrassTex));
}

RECOMP_IMPORT("*", ActorExtensionId actor_transform_id(Actor* actor));

#define gEXMatrixGroupDecomposedNormal(cmd, id, push, proj, edit) \
    gEXMatrixGroupDecomposed(cmd, id, push, proj, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_INTERPOLATE, G_EX_COMPONENT_SKIP, G_EX_COMPONENT_INTERPOLATE, G_EX_ORDER_LINEAR, edit)

Gfx* GenericGrass_DrawRandoColored(PlayState* play, Gfx* original, Gfx* gfx, u8 alpha, u32 location) {
    Color_RGB8 color;
    
    if (!get_rando_color(&color, location)) {
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, alpha);
        gSPDisplayList(gfx++, original);
        return gfx;
    }

    // choose Opa or Xlu DL based on type
    Gfx* customDL;
    customDL = gKusaBushType1GrayscaleDL; // failsafe
    
    if (original == gKusaBushType1DL) {
        customDL = gKusaBushType1GrayscaleDL;
    } else if (original == gKusaBushType2DL) {
        customDL = gKusaBushType2GrayscaleDL;
    } else if (original == gObjGrass_D_809AA9F0) {
        customDL = gObjGrass_GrayscaleOpaDL;
    } else if (original == gObjGrass_D_809AAA68) {
        customDL = gObjGrass_GrayscaleXluDL;
    } else if (original == gKusaSproutDL) {
        customDL = gKusaSproutGrayscaleDL;
    }
    
    gDPSetPrimColor(gfx++, 0, 0, color.r, color.g, color.b, alpha);
    gSPDisplayList(gfx++, customDL);
    return gfx;
}

void EnKusa_WaitForInteract(EnKusa* this, PlayState* play);
void EnKusa_ApplySway(MtxF* matrix);
extern MtxF D_80936AD8[8];

void EnKusa_DrawBushRando(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnKusa* this = ((EnKusa*)thisx);
    
    u32* kusaLocation = z64recomp_get_extended_actor_data(thisx, kusaGrassExtension);

    if ((this->actor.projectedPos.z <= 1200.0f) || ((this->isInWater & 1) && (this->actor.projectedPos.z < 1300.0f))) {

        if ((play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_0) &&
            (this->actionFunc == EnKusa_WaitForInteract) && (this->actor.projectedPos.z > -150.0f) &&
            (this->actor.projectedPos.z < 400.0f)) {
            EnKusa_ApplySway(&D_80936AD8[this->kusaMtxIdx]);
        }

        // Gfx_DrawDListOpa(play, gKusaBushType1DL);
        
        OPEN_DISPS(play->state.gfxCtx);
        Gfx_SetupDL25_Opa(play->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        
        POLY_OPA_DISP = GenericGrass_DrawRandoColored(play, gKusaBushType1DL, POLY_OPA_DISP, 255, *kusaLocation);
        // gSPDisplayList(POLY_OPA_DISP++, gKusaBushType1DL);

        CLOSE_DISPS(play->state.gfxCtx);

    } else if (this->actor.projectedPos.z < 1300.0f) {
        s32 alpha;

        OPEN_DISPS(play->state.gfxCtx);

        alpha = (1300.0f - this->actor.projectedPos.z) * 2.55f;
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        // gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, alpha);
        // gSPDisplayList(POLY_XLU_DISP++, gKusaBushType2DL);

        POLY_XLU_DISP = GenericGrass_DrawRandoColored(play, gKusaBushType2DL, POLY_XLU_DISP, alpha, *kusaLocation);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void EnKusa_DrawGrassRando(Actor* thisx, PlayState* play) {
    EnKusa* this = ((EnKusa*)thisx);

    u32* kusaLocation = z64recomp_get_extended_actor_data(thisx, kusaGrassExtension);

    if (this->isCut) {
        Gfx_DrawDListOpa(play, gKusaStumpDL);
    } else {
        if ((play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_0) &&
            (this->actionFunc == EnKusa_WaitForInteract)) {
            if ((this->actor.projectedPos.z > -150.0f) && (this->actor.projectedPos.z < 400.0f)) {
                EnKusa_ApplySway(&D_80936AD8[this->kusaMtxIdx]);
            }
        }
        // Gfx_DrawDListOpa(play, gKusaSproutDL);
        
        OPEN_DISPS(play->state.gfxCtx);
        Gfx_SetupDL25_Opa(play->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        
        POLY_OPA_DISP = GenericGrass_DrawRandoColored(play, gKusaSproutDL, POLY_OPA_DISP, 255, *kusaLocation);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void ObjGrass_InitDraw(ObjGrass* this, PlayState* play);
void ObjGrass_OverrideMatrixCurrent(MtxF* matrix);

// @recomp Patched to set matrix groups for grass.
void ObjGrass_DrawOpaRando(Actor* thisx, PlayState* play2) {
    ObjGrass* this = (ObjGrass*)thisx;
    PlayState* play = play2;
    Lights* lights;
    ObjGrassGroup* grassGroup;
    s32 i;
    s32 j;
    Vec3s rot = { 0, 0, 0 };
    ObjGrassElement* grassElem;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    // gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, 255);
    // gSPDisplayList(POLY_OPA_DISP++, gObjGrass_D_809AA9F0);
    
    // @recomp Extract this actor's ID.
    u32 actor_id = actor_transform_id(thisx);

    for (i = 0; i < this->activeGrassGroups; i++) {
        grassGroup = &this->grassGroups[i];

        if (grassGroup->flags & OBJ_GRASS_GROUP_DRAW) {
            lights = LightContext_NewLights(&play->lightCtx, play->state.gfxCtx);
            Lights_BindAll(lights, play->lightCtx.listHead, &grassGroup->homePos, play);
            Lights_Draw(lights, play->state.gfxCtx);

            for (j = 0; j < grassGroup->count; j++) {
                grassElem = &grassGroup->elements[j];

                if ((grassElem->flags & OBJ_GRASS_ELEM_DRAW) && (grassElem->alpha == 255)) {
                    // @rando Apply color based on location type/if not checked
                    POLY_OPA_DISP = GenericGrass_DrawRandoColored(play, gObjGrass_D_809AA9F0, POLY_OPA_DISP, grassElem->alpha, LOCATION_GRASS(grassElem));
                    
                    rot.y = grassElem->rotY;
                    Matrix_SetTranslateRotateYXZ(grassElem->pos.x, grassElem->pos.y, grassElem->pos.z, &rot);
                    Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
                    if (grassElem->flags & OBJ_GRASS_ELEM_ANIM) {
                        ObjGrass_OverrideMatrixCurrent(&this->distortionMtx[j]);
                    }

                    // @recomp Push a matrix group.
                    gEXMatrixGroupDecomposedNormal(POLY_OPA_DISP++, actor_id + i * OBJ_GRASS_GROUP_ELEM_COUNT_MAX + j, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);

                    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    gSPDisplayList(POLY_OPA_DISP++, gObjGrass_D_809AAAE0);

                    // @recomp Pop the matrix group.
                    gEXPopMatrixGroup(POLY_OPA_DISP++, G_MTX_MODELVIEW);
                }
            }
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

// @recomp Patched to set matrix groups for grass.
void ObjGrass_DrawXluRando(Actor* thisx, PlayState* play) {
    ObjGrass* this = (ObjGrass*)thisx;
    ObjGrassGroup* grassGroup;
    ObjGrassElement* grassElem;
    s32 i;
    s32 j;
    Vec3s rot = { 0, 0, 0 };

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    // gSPDisplayList(POLY_XLU_DISP++, gObjGrass_D_809AAA68);
    
    // @recomp Extract this actor's ID.
    u32 actor_id = actor_transform_id(thisx);

    for (i = 0; i < this->activeGrassGroups; i++) {
        grassGroup = &this->grassGroups[i];

        if (grassGroup->flags & OBJ_GRASS_GROUP_DRAW) {
            for (j = 0; j < grassGroup->count; j++) {
                grassElem = &grassGroup->elements[j];

                if ((grassElem->flags & OBJ_GRASS_ELEM_DRAW) && (grassElem->alpha > 0) && (grassElem->alpha < 255)) {
                    // @rando Apply color based on location type/if not checked
                    POLY_XLU_DISP = GenericGrass_DrawRandoColored(play, gObjGrass_D_809AAA68, POLY_XLU_DISP, grassElem->alpha, LOCATION_GRASS(grassElem));
                    
                    rot.y = grassElem->rotY;
                    Matrix_SetTranslateRotateYXZ(grassElem->pos.x, grassElem->pos.y, grassElem->pos.z, &rot);
                    Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);

                    // @recomp Push a matrix group.
                    gEXMatrixGroupDecomposedNormal(POLY_XLU_DISP++, actor_id + i * OBJ_GRASS_GROUP_ELEM_COUNT_MAX + j, G_EX_PUSH, G_MTX_MODELVIEW, G_EX_EDIT_NONE);

                    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                              G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                    // gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, grassElem->alpha);
                    gSPDisplayList(POLY_XLU_DISP++, gObjGrass_D_809AAAE0);

                    // @recomp Pop the matrix group.
                    gEXPopMatrixGroup(POLY_XLU_DISP++, G_MTX_MODELVIEW);
                }
            }
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

extern MtxF D_80A60908[8];

void func_80A5B954(MtxF* matrix, f32 arg1);
void func_80A5E80C(PlayState* play, s32 arg1);

RECOMP_PATCH void EnKusa2_Draw(Actor* thisx, PlayState* play) {
    EnKusa2* this = (EnKusa2*)thisx;
    s32 alpha;

    if (!Object_IsLoaded(&play->objectCtx, this->actor.objectSlot)) {
        return;
    }

    u32* keatonLocation = z64recomp_get_extended_actor_data(thisx, keatonGrassExtension);

    if (this->actor.projectedPos.z <= 1200.0f) {
        if ((play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_0) && (this->actor.projectedPos.z > -150.0f) &&
            (this->actor.projectedPos.z < 400.0f)) {
            func_80A5B954(&D_80A60908[this->unk_1CE], 0.0015f);
        }
        if (rando_get_slotdata_u32("grasssanity") == 0) {
            Gfx_DrawDListOpa(play, gKusaBushType1DL);
            return;
        }
        OPEN_DISPS(play->state.gfxCtx);
        Gfx_SetupDL25_Opa(play->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        POLY_OPA_DISP = GenericGrass_DrawRandoColored(play, gKusaBushType1DL, POLY_OPA_DISP, 255, *keatonLocation);
        // gSPDisplayList(POLY_OPA_DISP++, gKusaBushType1DL);

        CLOSE_DISPS(play->state.gfxCtx);
    } else if (this->actor.projectedPos.z < 1300.0f) {
        if (rando_get_slotdata_u32("grasssanity") == 0) {
            func_80A5E80C(play, (1300.0f - this->actor.projectedPos.z) * 2.55f);
            return;
        }
        OPEN_DISPS(play->state.gfxCtx);

        alpha = (1300.0f - this->actor.projectedPos.z) * 2.55f;
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        // gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, alpha);
        // gSPDisplayList(POLY_XLU_DISP++, gKusaBushType2DL);

        POLY_XLU_DISP = GenericGrass_DrawRandoColored(play, gKusaBushType2DL, POLY_XLU_DISP, alpha, *keatonLocation);
        CLOSE_DISPS(play->state.gfxCtx);
    }
}

RECOMP_PATCH void func_80A5EA48(Actor* thisx, PlayState* play) {
    EnKusa2* this = (EnKusa2*)thisx;
    s32 alpha;

    u32* keatonLocation = z64recomp_get_extended_actor_data(thisx, keatonGrassExtension);

    if (this->unk_1CF == 0xFF) {
        OPEN_DISPS(play->state.gfxCtx);
        Gfx_SetupDL25_Opa(play->state.gfxCtx);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        POLY_OPA_DISP = GenericGrass_DrawRandoColored(play, gKusaBushType1DL, POLY_OPA_DISP, 255, *keatonLocation);

        CLOSE_DISPS(play->state.gfxCtx);
    } else {
        OPEN_DISPS(play->state.gfxCtx);

        alpha = (1300.0f - this->actor.projectedPos.z) * 2.55f;
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        POLY_XLU_DISP = GenericGrass_DrawRandoColored(play, gKusaBushType2DL, POLY_XLU_DISP, alpha, *keatonLocation);
        CLOSE_DISPS(play->state.gfxCtx);
    }
}

void ObjGrass_DrawRando(Actor* thisx, PlayState* play) {
    ObjGrass* this = ((ObjGrass*)thisx);

    ObjGrass_InitDraw(this, play);
    ObjGrass_DrawOpaRando(thisx, play); // nearby
    ObjGrass_DrawXluRando(thisx, play); // draw distance
}

void ObjGrassCarry_DrawRando(Actor* this, PlayState* play) {
    ObjGrassElement* grassElem = ((ObjGrassCarry*)this)->grassElem;
    
    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    
    POLY_OPA_DISP = GenericGrass_DrawRandoColored(play, gObjGrass_D_809AA9F0, POLY_OPA_DISP, grassElem->alpha, LOCATION_GRASS(grassElem));

    CLOSE_DISPS(play->state.gfxCtx);
}