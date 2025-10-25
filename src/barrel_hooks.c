#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Taru/z_obj_taru.h"

#define LOCATION_BARREL (0x220000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

ActorExtensionId barrelLocationExtension;
u32* barrelLocation;

RECOMP_HOOK("ObjTaru_Init")
void OnObjTaru_Init(Actor* thisx, PlayState* play) {
    barrelLocation = z64recomp_get_extended_actor_data(thisx, barrelLocationExtension);
    *barrelLocation = LOCATION_BARREL;
}

RECOMP_PATCH void func_80B9BC64(ObjTaru* this, PlayState* play) {
    s32 item = func_800A8150(OBJ_TARU_GET_3F(&this->dyna.actor));

    barrelLocation = z64recomp_get_extended_actor_data(&this->dyna.actor, barrelLocationExtension);
    if (!rando_location_is_checked(*barrelLocation)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, (OBJ_TARU_GET_7F00(&this->dyna.actor) << 8) | item, *barrelLocation);
        return;
    }
    
    if (item >= 0) {
        Item_DropCollectible(play, &this->dyna.actor.world.pos, (OBJ_TARU_GET_7F00(&this->dyna.actor) << 8) | item);
    }
}

// pirate's fortress barricades?
RECOMP_HOOK("func_80B9B9C8")
void ObjTaru_DropBarricadeItem(ObjTaru* this, PlayState* play) {
    barrelLocation = z64recomp_get_extended_actor_data(&this->dyna.actor, barrelLocationExtension);
    if (!rando_location_is_checked(*barrelLocation)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, *barrelLocation);
    }
}