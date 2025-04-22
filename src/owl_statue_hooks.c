#include "global.h"
#include "modding.h"

#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

#define OWL_WARP_HIDDEN_OWL 0xF

extern s16 sKaleidoSetupRightPageIndex[];
extern f32 sKaleidoSetupRightPageEyeX[];
extern f32 sKaleidoSetupRightPageEyeZ[];

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