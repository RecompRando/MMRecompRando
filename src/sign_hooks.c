#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "rando_colors.h"
#include "models/custom_sign.h"

#include "overlays/actors/ovl_En_Kanban/z_en_kanban.h"

#define LOCATION_SIGN (AP_PREFIX_SIGNS | (signId << 12) | this->actor.textId)

ActorExtensionId signLocationExtension;
ActorExtensionId signDropExtension;

u16 signFrontTex[1024];
u16 signSideTex[1024];
u16 signPostTex[1024];
u16 originalSignTexTemp[1024];
extern u64 gSignRectangularFrontTex[];
extern u64 gSignRectangularSideTex[];
extern u64 gSignPostWoodTex[];

RECOMP_HOOK("EnKanban_Init")
void EnKanban_Init(Actor* thisx, PlayState* play) {
    EnKanban* this = ((EnKanban*)thisx);

    bool* dropped = z64recomp_get_extended_actor_data(thisx, signDropExtension);
    *dropped = false;

    if (this->actor.params != ENKANBAN_PIECE) {
        if (this->actor.params == ENKANBAN_FISHING) {
            if (LINK_IS_CHILD) { // lol
                this->actor.textId = 0x409D;
            } else {
                this->actor.textId = 0x4090;
            }
        } else {
            this->actor.textId = this->actor.params | ENKANBAN_FISHING;
        }
    }

    // duplicate sign fix
    u32 signId = 0;
    switch (this->actor.textId) {
        // basic dupes next to each other
        case 0x338: // deku palace
            signId = randoGetLoadedActorNumInSameRoom(play, thisx);
            break;
        // special cases
        case 0x33D: // woods of mystery (unique days)
            signId = (((CURRENT_DAY - 1) * 3) + (randoGetLoadedActorNumInSameRoom(play, thisx)));
            break;
        case 0x314: // mountain village
            signId = (play->sceneId == SCENE_10YUKIYAMANOMURA && randoGetLoadedActorNumInSameRoom(play, thisx) == 2);
            break;
        case 0x30C: // swamp spring water
            switch (play->sceneId) {
                case SCENE_KINSTA1: // swamp spider house
                    signId = 1;
                    break;
                case SCENE_KAKUSIANA: // bean daddy (grotto)
                    signId = 2;
                    break;
                case SCENE_20SICHITAI:
                case SCENE_20SICHITAI2:
                default:
                    signId = 0;
                    break;
            }
            break;
        default:
            break;
    }

    u32* location = z64recomp_get_extended_actor_data(thisx, signLocationExtension);
    *location = LOCATION_SIGN;

    // if (this->actor.params != ENKANBAN_PIECE) {
    //     recomp_printf("sign id: 0x%06X\n", *location);
    // }

    // CAMC Texture Grabbing
    Lib_MemCpy(originalSignTexTemp, SEGMENTED_TO_K0(gSignRectangularFrontTex), sizeof(originalSignTexTemp));
    RGBA16toIA16_Texture(originalSignTexTemp, signFrontTex, ARRAY_COUNT(originalSignTexTemp), GRAYSCALE_OOTMM);
    Lib_MemCpy(originalSignTexTemp, SEGMENTED_TO_K0(gSignRectangularSideTex), sizeof(originalSignTexTemp));
    RGBA16toIA16_Texture(originalSignTexTemp, signSideTex, ARRAY_COUNT(originalSignTexTemp), GRAYSCALE_OOTMM);
    Lib_MemCpy(originalSignTexTemp, SEGMENTED_TO_K0(gSignPostWoodTex), sizeof(originalSignTexTemp));
    RGBA16toIA16_Texture(originalSignTexTemp, signPostTex, ARRAY_COUNT(originalSignTexTemp), GRAYSCALE_OOTMM);
}

RECOMP_HOOK("EnKanban_Update")
void EnKanban_DropItem(Actor* thisx, PlayState* play) {
    EnKanban* this = ((EnKanban*)thisx);
    u32* location = z64recomp_get_extended_actor_data(thisx, signLocationExtension);
    bool* dropped = z64recomp_get_extended_actor_data(thisx, signDropExtension);

    if (this->actionState == ENKANBAN_SIGN) {
        if ((this->invincibilityTimer == 0) && (this->collider.base.acFlags & AC_HIT)) {
            if (this->unk_19C != this->collider.base.ac) { // i don't know what this line does
                if (rando_get_slotdata_u32("signsanity") && !rando_location_is_checked(*location) && !(*dropped)) {
                    Item_RandoDropCollectible(play, &thisx->world.pos, 0x60, *location);
                    *dropped = true;
                }
            }
        }
    }
}

#include "overlays/actors/ovl_En_Kanban/z_en_kanban_gfx.c"

extern u16 sPartFlags[11];
extern Gfx* sDisplayLists_ovl_En_Kanban[];
extern f32 sCutAngles[];

extern Gfx gSignMaterialDL[];
extern Gfx gSignRectangularDL[];
extern Gfx gSignParticleDL[];
extern Gfx gEnKanban_D_80957DE0[];

RECOMP_PATCH void EnKanban_Draw(Actor* thisx, PlayState* play) {
    EnKanban* this = ((EnKanban*)thisx);
    f32 zShift;
    f32 zShift2;
    s32 i;
    Player* player = GET_PLAYER(play);
    u8* shadowTex = GRAPH_ALLOC(play->state.gfxCtx, ARRAY_COUNT(sShadowTexFlags));

    u32* location = z64recomp_get_extended_actor_data(thisx, signLocationExtension);
    bool* dropped = z64recomp_get_extended_actor_data(thisx, signDropExtension);

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    gSPDisplayList(POLY_OPA_DISP++, gSignMaterialDL);

    if (this->actionState != ENKANBAN_SIGN) {
        Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, MTXMODE_NEW);
        Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);
        Matrix_RotateXFApply(this->floorRot.x);
        Matrix_RotateZF(this->floorRot.z, MTXMODE_APPLY);
        Matrix_Translate(0.0f, this->actor.shape.yOffset, 0.0f, MTXMODE_APPLY);
        Matrix_RotateYS(this->actor.shape.rot.y, MTXMODE_APPLY);
        Matrix_RotateXS(this->actor.shape.rot.x, MTXMODE_APPLY);

        zShift = fabsf(Math_SinS(this->spinRot.x) * this->pieceHeight);
        zShift2 = fabsf(Math_SinS(this->spinRot.z) * this->pieceWidth);
        zShift = CLAMP_MIN(zShift, zShift2);
        zShift *= -(f32)this->direction;

        Matrix_Translate(0.0f, 0.0f, zShift, MTXMODE_APPLY);
        Matrix_RotateXS(this->spinRot.x, MTXMODE_APPLY);
        Matrix_RotateYS(this->spinRot.z, MTXMODE_APPLY);
        Matrix_Translate(this->offset.x, this->offset.y, this->offset.z - 100.0f, MTXMODE_APPLY);

        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        for (i = 0; i < ARRAY_COUNT(sPartFlags); i++) {
            if (sPartFlags[i] & this->partFlags) {
                gSPDisplayList(POLY_OPA_DISP++, sDisplayLists_ovl_En_Kanban[i]);
            }
        }
    } else {
        f32 phi_f0 = 0.0f;

        if ((player->transformation == PLAYER_FORM_HUMAN) || (player->transformation == PLAYER_FORM_DEKU)) {
            phi_f0 = -15.0f;
        }
        this->actor.world.pos.y = this->actor.home.pos.y + phi_f0;
        Matrix_Translate(0.0f, 0.0f, -100.0f, MTXMODE_APPLY);
        gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        if (this->partFlags == 0xFFFF) {
            // @rando CAMC signs
            Color_RGB8 color;
            if (rando_get_slotdata_u32("signsanity") && get_rando_color(&color, *location) && !(*dropped)) {
                gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color.r, color.g, color.b, 255);
                gSPDisplayList(POLY_OPA_DISP++, randoSignRectangularDL);
            } else {
                gSPDisplayList(POLY_OPA_DISP++, gSignRectangularDL);
            }
        } else {
            for (i = 0; i < ARRAY_COUNT(sPartFlags); i++) {
                if (sPartFlags[i] & this->partFlags) {
                    gSPDisplayList(POLY_OPA_DISP++, sDisplayLists_ovl_En_Kanban[i]);
                }
            }
        }

        if (this->cutMarkAlpha != 0) {
            f32 cutOffset = (this->cutType == CUT_POST) ? -1200.0f : 0.0f;

            Matrix_Translate(0.0f, 4400.0f + cutOffset, 200.0f, MTXMODE_APPLY);
            Matrix_RotateZF(sCutAngles[this->cutType], MTXMODE_APPLY);
            Matrix_Scale(0.0f, 10.0f, 2.0f, MTXMODE_APPLY);

            gDPPipeSync(POLY_XLU_DISP++);
            gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, 255, 255, 255, this->cutMarkAlpha);
            gDPSetEnvColor(POLY_XLU_DISP++, 255, 255, 150, 0);
            gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            gSPDisplayList(POLY_XLU_DISP++, gSignParticleDL);
        }
    }

    if ((this->actor.projectedPos.z <= 400.0f) && (this->actor.projectedPos.z > 0.0f) &&
        (this->actor.floorHeight > -3000.0f) && ((this->bounceX != 0) || (this->bounceZ != 0))) {
        u16 dayTime = gSaveContext.save.time;
        f32 shadowAlpha;

        if (dayTime >= CLOCK_TIME(12, 0)) {
            dayTime = (DAY_LENGTH - 1) - dayTime;
        }

        shadowAlpha = (dayTime * 0.00275f) + 10.0f;
        if (this->actor.projectedPos.z > 300.0f) {
            shadowAlpha *= (400.0f - this->actor.projectedPos.z) * 0.01f;
        }

        gDPSetPrimColor(POLY_XLU_DISP++, 0x00, 0x00, 0, 0, 0, (s8)shadowAlpha);

        if ((player->transformation == PLAYER_FORM_HUMAN) || (player->transformation == PLAYER_FORM_DEKU)) {
            zShift = 0.0f;
        } else {
            zShift = ((this->actor.world.pos.y - this->actor.floorHeight) * -50.0f) / 100.0f;
        }

        Matrix_Translate(this->actor.world.pos.x, this->actor.floorHeight, this->actor.world.pos.z + zShift,
                         MTXMODE_NEW);
        Matrix_RotateXFApply(this->floorRot.x);
        Matrix_RotateZF(this->floorRot.z, MTXMODE_APPLY);
        Matrix_Scale(this->actor.scale.x, 0.0f, this->actor.scale.z, MTXMODE_APPLY);

        if (this->actionState == ENKANBAN_SIGN) {
            Matrix_RotateXFApply(-M_PI / 5);
        }

        Matrix_RotateYS(this->actor.shape.rot.y, MTXMODE_APPLY);
        Matrix_RotateXS(this->actor.shape.rot.x, MTXMODE_APPLY);
        Matrix_RotateXS(this->spinRot.x, MTXMODE_APPLY);
        Matrix_RotateYS(this->spinRot.z, MTXMODE_APPLY);
        Matrix_Translate(this->offset.x, this->offset.y, this->offset.z, MTXMODE_APPLY);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

        for (i = 0; i < ARRAY_COUNT(sShadowTexFlags); i++) {
            if (sShadowTexFlags[i] & this->partFlags) {
                shadowTex[i] = 0xFF;
            } else {
                shadowTex[i] = 0;
            }
        }

        gSPSegment(POLY_XLU_DISP++, 0x08, Lib_SegmentedToVirtual(shadowTex));
        gSPDisplayList(POLY_XLU_DISP++, gEnKanban_D_80957DE0);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}
