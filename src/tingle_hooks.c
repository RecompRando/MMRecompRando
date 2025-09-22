#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define TINGLE_LIMB_MAX 0x21
#include "overlays/actors/ovl_En_Bal/z_en_bal.h"

// @ap always start in the air
RECOMP_HOOK("EnBal_Init")
void OnEnBal_Init(Actor* thisx, PlayState* play) {
    gSaveContext.save.saveInfo.playerData.isMagicAcquired = true;
}

// reset magic back
RECOMP_HOOK_RETURN("EnBal_Init")
void AfterEnBal_Init(Actor* thisx, PlayState* play) {
    gSaveContext.save.saveInfo.playerData.isMagicAcquired = rando_has_item(0x020000); // 0x020000 = magic
}

extern s16 sBuyMapOptions[TINGLE_MAP_MAX][2];

RECOMP_PATCH s32 EnBal_CheckIfMapUnlocked(EnBal* this, PlayState* play) {
    this->purchaseMapId = sBuyMapOptions[this->locationMapId][play->msgCtx.choiceIndex];
    return rando_location_is_checked(this->purchaseMapId + 0xB4);
}

RECOMP_PATCH void EnBal_UnlockSelectedAreaMap(EnBal* this) {
}