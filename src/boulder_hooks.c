#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Bombiwa/z_obj_bombiwa.h"
#include "overlays/actors/ovl_Obj_Hamishi/z_obj_hamishi.h"

#define LOCATION_BOULDER (0x1B0000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                        | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_HAMISHI))
#define LOCATION_BOULDER2 (0x1B0000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                        | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_BOMBIWA))

ActorExtensionId boulderExtension;
u32* extendedBoulderData;
ActorExtensionId boulder2Extension;
u32* extendedBoulder2Data;

// normal boulders
RECOMP_HOOK("ObjBombiwa_Init")
void OnObjBombiwa_Init(Actor* thisx, PlayState* play) {
    extendedBoulderData = z64recomp_get_extended_actor_data(thisx, boulderExtension);
    *extendedBoulderData = LOCATION_BOULDER;
}

ObjBombiwa* sObjBombiwa;

RECOMP_HOOK("func_80939EF4")
void OnObjBombiwa_CheckBroken(ObjBombiwa* this, PlayState* play) {
    sObjBombiwa = this;
}

RECOMP_HOOK_RETURN("func_80939EF4")
void AfterObjBombiwa_CheckBroken() {
    ObjBombiwa* this = sObjBombiwa;
    PlayState* play = gPlay;

    if (Flags_GetSwitch(play, OBJBOMBIWA_GET_SWITCH_FLAG(&this->actor))) {
        extendedBoulderData = z64recomp_get_extended_actor_data(&this->actor, boulderExtension);
        if (!rando_location_is_checked(*extendedBoulderData)) {
            Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedBoulderData);
        }
    }
}

// bronze boulders
RECOMP_HOOK("ObjHamishi_Init")
void OnObjHamishi_Init(Actor* thisx, PlayState* play) {
    extendedBoulder2Data = z64recomp_get_extended_actor_data(thisx, boulder2Extension);
    *extendedBoulder2Data = LOCATION_BOULDER2;
}

RECOMP_HOOK("func_809A10F4")
void ObjHamishi_Broken(ObjHamishi* this, PlayState* play) {
    extendedBoulder2Data = z64recomp_get_extended_actor_data(&this->actor, boulder2Extension);
    if (!rando_location_is_checked(*extendedBoulder2Data)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedBoulder2Data);
    }
}