#include "global.h"
#include "modding.h"

#include "apcommon.h"

#include "overlays/actors/ovl_En_Sekihi/z_en_sekihi.h"

bool healedMikau;

void func_80A450B0(EnSekihi* this, PlayState* play);

void EnSekihi_GiveMask(EnSekihi* this, PlayState* play) {
    if (!Actor_HasParent(&this->dyna.actor, play)) {
        Actor_OfferGetItem(&this->dyna.actor, play, GI_MASK_ZORA, 1000.0f, 1000.0f);
    } else {
        healedMikau = false;
        this->dyna.actor.parent = NULL;
        this->actionFunc = func_80A450B0;
    }
}

RECOMP_HOOK("func_80A450B0") void On_Guitar_Update(EnSekihi* this, PlayState* play) {
    if (healedMikau) {
        this->actionFunc = EnSekihi_GiveMask;
    }
}