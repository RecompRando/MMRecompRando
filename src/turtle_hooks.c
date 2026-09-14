#include "modding.h"
#include "global.h"

#include "apcommon.h"

#include "overlays/actors/ovl_Dm_Char08/z_dm_char08.h"

DmChar08* savedDmChar08;

void func_80AAFB94(DmChar08* this, PlayState* play); // blank function

// TODO: skip the spawning cutscene(?) (maybe best left to a separate mod)

RECOMP_HOOK("func_80AAF8F4")
void DmChar08_SetupWarpToGBT(DmChar08* this, PlayState* play) {
    savedDmChar08 = this;
}

RECOMP_HOOK_RETURN("func_80AAF8F4")
void DmChar08_StartWarpToGBT() {
    DmChar08* this = savedDmChar08;
    PlayState* play = gPlay;

    if (fabsf(this->dyna.actor.xzDistToPlayer) < 200.0f) {
        // Audio_PlaySfx(NA_SE_OC_SECRET_WARP_OUT);
        play->nextEntrance = ENTRANCE(GREAT_BAY_TEMPLE, 0);
        play->transitionType = TRANS_TYPE_FADE_BLUE;
        gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLUE;
        play->transitionTrigger = TRANS_TRIGGER_START;
        this->actionFunc = func_80AAFB94;
    }
}

RECOMP_HOOK("func_80AB023C")
void DmChar08_MessageUpdate(DmChar08* this, PlayState* play) {
    // @rando add a way to get out of GBT by talking to the turtle rather than hookshot
    // original textbox replaced in eztr_text.c
    if (play->msgCtx.currentTextId == 0x102F && Message_GetState(&play->msgCtx) == TEXT_STATE_CHOICE && Message_ShouldAdvance(play)) {
        if (play->msgCtx.choiceIndex == 0) {
            play->nextEntrance = ENTRANCE(ZORA_CAPE, 7);
            play->transitionTrigger = TRANS_TRIGGER_START;
            Scene_SetExitFade(play);
            this->actionFunc = func_80AAFB94;
        } else {
            Audio_PlaySfx_MessageCancel();
            Message_CloseTextbox(play);
        }
    }
}