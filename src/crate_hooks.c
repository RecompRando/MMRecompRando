#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Kibako/z_obj_kibako.h"
#include "overlays/actors/ovl_Obj_Kibako2/z_obj_kibako2.h"

#define LOCATION_CRATE_SMALL (0x250000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))
#define LOCATION_CRATE_BIG (0x270000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

ActorExtensionId crateSmallExtension;
u32* extendedCrateSmallData;
ActorExtensionId crateBigExtension;
u32* extendedCrateBigData;

// small crate
RECOMP_HOOK("ObjKibako_Init")
void OnObjKibako_Init(Actor* thisx, PlayState* play) {
    extendedCrateSmallData = z64recomp_get_extended_actor_data(thisx, crateSmallExtension);
    *extendedCrateSmallData = LOCATION_CRATE_SMALL;
}

RECOMP_HOOK("ObjKibako_SpawnCollectible")
void OnObjKibako_SpawnCollectible(ObjKibako* this, PlayState* play) {
    extendedCrateSmallData = z64recomp_get_extended_actor_data(&this->actor, crateSmallExtension);
    if (!rando_location_is_checked(*extendedCrateSmallData)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedCrateSmallData);
        this->isDropCollected = 1;
    }
}

// big crate
RECOMP_HOOK("ObjKibako2_Init")
void OnObjKibako2_Init(Actor* thisx, PlayState* play) {
    extendedCrateBigData = z64recomp_get_extended_actor_data(thisx, crateBigExtension);
    *extendedCrateBigData = LOCATION_CRATE_BIG;
}

RECOMP_PATCH void ObjKibako2_SpawnCollectible(ObjKibako2* this, PlayState* play) {
    s32 dropItem00Id = func_800A8150(KIBAKO2_COLLECTIBLE_ID(&this->dyna.actor));

    extendedCrateBigData = z64recomp_get_extended_actor_data(&this->dyna.actor, crateBigExtension);
    if (!rando_location_is_checked(*extendedCrateBigData)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos,
            dropItem00Id | KIBAKO2_COLLECTIBLE_FLAG(&this->dyna.actor) << 8, *extendedCrateBigData);
        return;
    }

    if (dropItem00Id > ITEM00_NO_DROP) {
        Item_DropCollectible(play, &this->dyna.actor.world.pos,
                             dropItem00Id | KIBAKO2_COLLECTIBLE_FLAG(&this->dyna.actor) << 8);
    }
}