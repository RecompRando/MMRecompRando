#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "rando_colors.h"
#include "models/custom_boulders.h"

#include "overlays/actors/ovl_Obj_Bombiwa/z_obj_bombiwa.h"
#include "overlays/actors/ovl_Obj_Hamishi/z_obj_hamishi.h"

#define LOCATION_BOULDER (AP_PREFIX_BOULDERS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                        | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_HAMISHI))
#define LOCATION_BOULDER2 (AP_PREFIX_BOULDERS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                        | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_BOMBIWA))

// normal boulders
// need to figure out how to handle CI textures for actual normal boulder (maybe just change the TLUT lol)
u16 tallBombiwaTex[1024];
u16 tempBoulderTex[1024];
extern u16 object_bombiwa_Tex_004830[];

RECOMP_HOOK("ObjBombiwa_Init")
void ObjBombiwa_GrabTextures(Actor* thisx, PlayState* play) {
    Lib_MemCpy(tempBoulderTex, SEGMENTED_TO_K0(object_bombiwa_Tex_004830), sizeof(tempBoulderTex));
    RGBA16toIA16_Texture(tempBoulderTex, tallBombiwaTex, ARRAY_COUNT(tempBoulderTex), GRAYSCALE_OOTMM);
}

extern Gfx object_bombiwa_DL_0009E0[];
extern Gfx object_bombiwa_DL_000AF0[];
extern Gfx object_bombiwa_DL_004560[];
extern Gfx object_bombiwa_DL_004688[];
extern Gfx object_bombiwa_DL_005990[];

void func_80939EF4(ObjBombiwa* this, PlayState* play);

RECOMP_PATCH void func_8093A418(Actor* thisx, PlayState* play) {
    ObjBombiwa* this = ((ObjBombiwa*)thisx);
    f32 sp28;

    u32* boulderLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);

    Color_RGB8 color;

    if ((this->actor.projectedPos.z <= 2200.0f) || ((this->unk_203 & 1) && (this->actor.projectedPos.z < 2300.0f))) {
        this->actor.shape.shadowAlpha = 160;

        if (!rando_get_slotdata_u32("rocksanity") || !get_rando_color(&color, *boulderLocation)) {
            Gfx_DrawDListOpa(play, object_bombiwa_DL_0009E0);
            return;
        }

        OPEN_DISPS(play->state.gfxCtx);

        Gfx_SetupDL25_Opa(play->state.gfxCtx);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color.r, color.g, color.b, 255);
        gSPDisplayList(POLY_OPA_DISP++, randoBombiwaOpaDL);

        CLOSE_DISPS(play->state.gfxCtx);
        return;
    }

    if (this->actor.projectedPos.z < 2300.0f) {
        OPEN_DISPS(play->state.gfxCtx);

        sp28 = (2300.0f - this->actor.projectedPos.z) * 2.55f;

        this->actor.shape.shadowAlpha = sp28 * (32.0f / 51);
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        if (rando_get_slotdata_u32("rocksanity") && get_rando_color(&color, *boulderLocation)) {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, color.r, color.g, color.b, (s32)sp28);
            gSPDisplayList(POLY_XLU_DISP++, randoBombiwaXluDL);
        } else {
            gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 255, 255, (s32)sp28);
            gSPDisplayList(POLY_XLU_DISP++, object_bombiwa_DL_000AF0);
        }

        CLOSE_DISPS(play->state.gfxCtx);
    } else {
        this->actor.shape.shadowAlpha = 0;
    }
}

// tall boulders
RECOMP_PATCH void func_8093A608(Actor* thisx, PlayState* play) {
    s32 pad[8];
    ObjBombiwa* this = ((ObjBombiwa*)thisx);
    f32 sp38;
    s32 i;
    ObjBombiwaStruct* ptr;

    u32* boulderLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);

    Color_RGB8 color;

    OPEN_DISPS(play->state.gfxCtx);

    if (this->actionFunc == func_80939EF4) {
        if ((this->actor.projectedPos.z <= 2200.0f) ||
            ((this->unk_203 & 1) && (this->actor.projectedPos.z < 2300.0f))) {
            Gfx_SetupDL25_Opa(play->state.gfxCtx);

            gSPSegment(POLY_OPA_DISP++, 0x08, D_801AEFA0);
            gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            
            if (rando_get_slotdata_u32("rocksanity") && get_rando_color(&color, *boulderLocation)) {
                gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x9B, color.r, color.g, color.b, 255);
                gSPDisplayList(POLY_OPA_DISP++, randoTallBombiwaDL);
            } else {
                gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x9B, 255, 255, 255, 255);
                gSPDisplayList(POLY_OPA_DISP++, object_bombiwa_DL_004560);
            }

            Gfx_SetupDL25_Xlu(play->state.gfxCtx);

            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, object_bombiwa_DL_004688);
        } else if (this->actor.projectedPos.z < 2300.0f) {
            sp38 = (2300.0f - this->actor.projectedPos.z) * 2.55f;
            Gfx_SetupDL25_Xlu(play->state.gfxCtx);

            gSPSegment(POLY_XLU_DISP++, 0x08, D_801AEF88);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            
            if (rando_get_slotdata_u32("rocksanity") && get_rando_color(&color, *boulderLocation)) {
                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x9B, color.r, color.g, color.b, (s32)sp38);
                gSPDisplayList(POLY_XLU_DISP++, randoTallBombiwaDL);
            } else {
                gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x9B, 255, 255, 255, (s32)sp38);
                gSPDisplayList(POLY_XLU_DISP++, object_bombiwa_DL_004560);
            }
        }
    } else {
        Gfx_SetupDL25_Opa(play->state.gfxCtx);

        for (i = 0; i < ARRAY_COUNT(this->unk_190); i++) {
            ptr = &this->unk_190[i];

            if (ptr->unk_1A == 0) {
                Matrix_SetTranslateRotateYXZ(ptr->unk_04.x, ptr->unk_04.y, ptr->unk_04.z, &ptr->unk_14);
                Matrix_Scale(ptr->unk_00, ptr->unk_00, ptr->unk_00, MTXMODE_APPLY);

                gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
                gSPDisplayList(POLY_OPA_DISP++, object_bombiwa_DL_005990);
            }
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

// bronze boulders
extern Gfx gameplay_field_keep_DL_0061E8[];

RECOMP_PATCH void ObjHamishi_Draw(Actor* thisx, PlayState* play) {
    ObjHamishi* this = ((ObjHamishi*)thisx);
    u32* boulder2Location = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);

    Color_RGB8 color = {255, 170, 130};
    get_rando_color(&color, *boulder2Location);

    OPEN_DISPS(play->state.gfxCtx);

    if ((thisx->projectedPos.z <= 2150.0f) || ((this->unk_1A2 & 1) && (thisx->projectedPos.z < 2250.0f))) {
        thisx->shape.shadowAlpha = 160;
        Gfx_SetupDL25_Opa(play->state.gfxCtx);

        gSPSegment(POLY_OPA_DISP++, 0x08, D_801AEFA0);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        // gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 170, 130, 255);
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color.r, color.g, color.b, 255);
        gSPDisplayList(POLY_OPA_DISP++, gameplay_field_keep_DL_0061E8);
    } else if (thisx->projectedPos.z < 2250.0f) {
        f32 sp20 = (2250.0f - thisx->projectedPos.z) * 2.55f;

        thisx->shape.shadowAlpha = sp20 * 0.627451f;
        Gfx_SetupDL25_Xlu(play->state.gfxCtx);

        gSPSegment(POLY_XLU_DISP++, 0x08, D_801AEF88);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        // gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, 255, 170, 130, (s32)sp20);
        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, color.r, color.g, color.b, (s32)sp20);
        gSPDisplayList(POLY_XLU_DISP++, gameplay_field_keep_DL_0061E8);
    } else {
        thisx->shape.shadowAlpha = 0;
    }

    CLOSE_DISPS(play->state.gfxCtx);
}