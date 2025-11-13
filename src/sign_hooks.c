#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_En_Kanban/z_en_kanban.h"

#define LOCATION_SIGN (AP_PREFIX_SIGNS | this->actor.textId)

ActorExtensionId signDropExtension;

RECOMP_HOOK("EnKanban_Init")
void EnKanban_Init(Actor* thisx, PlayState* play) {
    bool* dropped = z64recomp_get_extended_actor_data(thisx, signDropExtension);
    *dropped = false;
}

RECOMP_HOOK("EnKanban_Update")
void EnKanban_DropItem(Actor* thisx, PlayState* play) {
    EnKanban* this = ((EnKanban*)thisx);
    bool* dropped = z64recomp_get_extended_actor_data(thisx, signDropExtension);

    if (this->actionState == ENKANBAN_SIGN) {
        if ((this->invincibilityTimer == 0) && (this->collider.base.acFlags & AC_HIT)) {
            if (this->unk_19C != this->collider.base.ac) { // i don't know what this line does
                if (rando_get_slotdata_u32("signsanity") && !rando_location_is_checked(LOCATION_SIGN) && !(*dropped)) {
                    Item_RandoDropCollectible(play, &thisx->world.pos, 0x60, LOCATION_SIGN);
                    *dropped = true;
                }
            }
        }
    }
}