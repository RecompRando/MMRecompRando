#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define LOCATION_SCOPECROW_RUPEE (AP_PREFIX_FREESTANDING | (play->sceneId << 8) | (0xC << 4) | this->ruppeIndex)

#define OBJECT_CROW_LIMB_MAX 0x09
#include "overlays/actors/ovl_En_Scopecrow/z_en_scopecrow.h"
#include "overlays/actors/ovl_En_Sc_Ruppe/z_en_sc_ruppe.h"

RECOMP_HOOK("EnScopecrow_Init")
void OnEnScopecrow_Init(Actor* thisx, PlayState* play) {
    // fix oversight where both telescope guays drop the same item (offset to drop a different rupee)
    thisx->params += randoGetLoadedActorNumInSameRoom(play, thisx);
}

void EnItem00_RandoGive(EnItem00* this, PlayState* play, s32 getItemId, u32 location);

void EnScRuppe_RandoDraw(Actor* thisx, PlayState* play) {
    EnScRuppe* this = ((EnScRuppe*)thisx);
    
    u32 getItemId = rando_get_item_id(LOCATION_SCOPECROW_RUPEE);
    u16 objectId = getObjectId(getItemId);

    Matrix_Scale(20.0f, 20.0f, 20.0f, MTXMODE_APPLY);
    if (isAP(getItemId)) {
        GetItem_Draw(play, getGid(getItemId));
    } else if (ObjLoad(play, 0x06, objectId)) {
        GetItem_Draw(play, getGid(getItemId));
        ObjUnload(play, 0x06, objectId);
    // } else {
    //     GetItem_Draw(play, GID_APLOGO_USEFUL);
    }
}

void func_80BD6B18(EnScRuppe* this, PlayState* play);

void EnScRuppe_RandoIdle(EnScRuppe* this, PlayState* play) {
    if (this->collider.base.ocFlags1 & OC1_HIT) {
        this->ruppeDisplayTime = 0;
        this->actor.gravity = 0.0f;
        Actor_PlaySfx(&this->actor, NA_SE_SY_GET_RUPY);
        // Rupees_ChangeBy(sRupeeInfo[this->ruppeIndex].amount);
        Message_StartTextbox(play, getTextId(rando_get_item_id(LOCATION_SCOPECROW_RUPEE)), NULL);
        recomp_printf("scopecrow location: 0x%06X\n", LOCATION_SCOPECROW_RUPEE);
        this->actionFunc = func_80BD6B18;
    }
    this->actor.shape.rot.y += 0x1F4;
    Actor_MoveWithGravity(&this->actor);
}

EnScRuppe* savedEnScRuppe;

RECOMP_HOOK("EnScRuppe_Init")
void OnEnScRuppe_Init(Actor* thisx, PlayState* play) {
    savedEnScRuppe = ((EnScRuppe*)thisx);
}

RECOMP_HOOK_RETURN("EnScRuppe_Init")
void AfterEnScRuppe_Init() {
    EnScRuppe* this = savedEnScRuppe;
    PlayState* play = gPlay;

    if (rando_get_slotdata_u32("rupeesanity") && !rando_location_is_checked(LOCATION_SCOPECROW_RUPEE)) {
        savedEnScRuppe->actor.draw = EnScRuppe_RandoDraw;
        savedEnScRuppe->actionFunc = EnScRuppe_RandoIdle;
    }
}