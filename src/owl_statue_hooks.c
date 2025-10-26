#include "global.h"
#include "modding.h"
#include "recomputils.h"

#include "apcommon.h"

#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"
#include "overlays/actors/ovl_Obj_Warpstone/z_obj_warpstone.h"

#define OWL_WARP_HIDDEN_OWL 0xF

extern s16 sKaleidoSetupRightPageIndex[];
extern f32 sKaleidoSetupRightPageEyeX[];
extern f32 sKaleidoSetupRightPageEyeZ[];

// TODO: move to a different mod
RECOMP_PATCH void func_800F4A10(PlayState* play) {
    PauseContext* pauseCtx = &play->pauseCtx;
    s16 i;

    Rumble_StateReset();

    pauseCtx->switchPageTimer = 0;
    pauseCtx->mainState = PAUSE_MAIN_STATE_SWITCHING_PAGE;

    // Set eye position and pageIndex such that scrolling left brings to the desired page
    pauseCtx->eye.x = sKaleidoSetupRightPageEyeX[pauseCtx->pageIndex];
    pauseCtx->eye.z = sKaleidoSetupRightPageEyeZ[pauseCtx->pageIndex];
    pauseCtx->pageIndex = sKaleidoSetupRightPageIndex[pauseCtx->pageIndex];
    pauseCtx->infoPanelOffsetY = -40;

    for (i = 0; i < ARRAY_COUNT(pauseCtx->worldMapPoints); i++) {
        pauseCtx->worldMapPoints[i] = false;
    }

    if (pauseCtx->state == PAUSE_STATE_OPENING_0) {
        for (i = 0; i < REGION_MAX; i++) {
            if ((gSaveContext.save.saveInfo.regionsVisited >> i) & 1) {
                pauseCtx->worldMapPoints[i] = true;
            }
        }
    } else {
        // skip other warps if hidden owl is hit
        if (!((gSaveContext.save.saveInfo.playerData.owlActivationFlags >> OWL_WARP_HIDDEN_OWL) & 1)) {
            for (i = OWL_WARP_STONE_TOWER; i >= OWL_WARP_GREAT_BAY_COAST; i--) {
                if ((gSaveContext.save.saveInfo.playerData.owlActivationFlags >> i) & 1) {
                    pauseCtx->worldMapPoints[i] = true;
                    pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = i;
                }
            }

            if ((gSaveContext.save.saveInfo.playerData.owlActivationFlags >> 4) & 1) {
                pauseCtx->cursorPoint[PAUSE_WORLD_MAP] = 4;
            }
        }
    }

    YREG(11) = -0x00C8;
    YREG(12) = -0x3840;
    YREG(13) = 0x2710;
    YREG(14) = 0x2710;
    YREG(15) = 0x2710;
    YREG(16) = -0x00BE;
    YREG(17) = -0x06D6;
    YREG(18) = -0x0B90;
    YREG(19) = 0x06E0;
    YREG(20) = 0;
    YREG(21) = -0x0622;
    YREG(22) = -0x0C44;
    YREG(23) = 0x0622;
    R_PAUSE_WORLD_MAP_YAW = -0x622;
    R_PAUSE_WORLD_MAP_Y_OFFSET = -90;
    R_PAUSE_WORLD_MAP_DEPTH = -14400;
}

// ford f150
#define LOCATION_OWL_STATUE (AP_PREFIX_OWLS | OBJ_WARPSTONE_GET_ID(&this->dyna.actor))

s32 ObjWarpstone_OpenedIdle(ObjWarpstone* this, PlayState* play);
s32 ObjWarpstone_ClosedIdle(ObjWarpstone* this, PlayState* play);
s32 ObjWarpstone_BeginOpeningCutscene(ObjWarpstone* this, PlayState* play);
void ObjWarpstone_SetupAction(ObjWarpstone* this, ObjWarpstoneActionFunc actionFunc);

ObjWarpstone* sObjWarpstone;

RECOMP_HOOK("ObjWarpstone_Init")
void OnObjWarpstone_Init(Actor* thisx, PlayState* play) {
    sObjWarpstone = ((ObjWarpstone*)thisx);
}

RECOMP_HOOK_RETURN("ObjWarpstone_Init")
void AfterObjWarpstone_Init(Actor* thisx, PlayState* play) {
    ObjWarpstone* this = sObjWarpstone;
    if (rando_get_slotdata_u32("owlsanity")) {
        if (!rando_location_is_checked(LOCATION_OWL_STATUE)) {
            ObjWarpstone_SetupAction(this, ObjWarpstone_ClosedIdle);
        } else {
            ObjWarpstone_SetupAction(this, ObjWarpstone_OpenedIdle);
        }

        // if we want to be really fancy put this in both idles
        // if (rando_has_item(LOCATION_OWL_STATUE)) {
        //     this->modelIndex = SEK_MODEL_OPENED;
        // } else {
        //     this->modelIndex = SEK_MODEL_CLOSED;
        // }
    }
}

s32 ObjWarpstone_OfferItem(ObjWarpstone* this, PlayState* play) {
    if (Actor_HasParent(&this->dyna.actor, play)) {
        this->dyna.actor.parent = NULL;
        this->actionFunc = ObjWarpstone_OpenedIdle;
    } else {
        Actor_OfferGetItemHook(&this->dyna.actor, play, rando_get_item_id(LOCATION_OWL_STATUE), LOCATION_OWL_STATUE, 1000.0f, 1000.0f, true, true);
    }
    return true;
}

RECOMP_PATCH s32 ObjWarpstone_ClosedIdle(ObjWarpstone* this, PlayState* play) {
    if (this->collider.base.acFlags & AC_HIT) {
        if (rando_get_slotdata_u32("owlsanity")) {
            ObjWarpstone_SetupAction(this, ObjWarpstone_OfferItem);
            recomp_printf("owl location: 0x%06X\n", LOCATION_OWL_STATUE);
        } else {
            ObjWarpstone_SetupAction(this, ObjWarpstone_BeginOpeningCutscene);
        }
        return true;
    } else {
        /*Ye who hold the sacred sword, leave proof of our encounter.*/
        this->dyna.actor.textId = 0xC00;
        return false;
    }
}