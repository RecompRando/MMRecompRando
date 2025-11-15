#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_En_Kanban/z_en_kanban.h"

#define LOCATION_SIGN (AP_PREFIX_SIGNS | (signId << 12) | this->actor.textId)

ActorExtensionId signLocationExtension;
ActorExtensionId signDropExtension;

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