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

ActorExtensionId boulderLocationExtension;
u32* boulderLocation;
ActorExtensionId boulder2LocationExtension;
u32* boulder2Location;

// normal boulders
RECOMP_HOOK("ObjBombiwa_Init")
void OnObjBombiwa_Init(Actor* thisx, PlayState* play) {
    boulderLocation = z64recomp_get_extended_actor_data(thisx, boulderLocationExtension);
    *boulderLocation = LOCATION_BOULDER;
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
        boulderLocation = z64recomp_get_extended_actor_data(&this->actor, boulderLocationExtension);
        if (!rando_location_is_checked(*boulderLocation)) {
            Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *boulderLocation);
        }
    }
}

// bronze boulders
RECOMP_HOOK("ObjHamishi_Init")
void OnObjHamishi_Init(Actor* thisx, PlayState* play) {
    boulder2Location = z64recomp_get_extended_actor_data(thisx, boulder2LocationExtension);
    *boulder2Location = LOCATION_BOULDER2;
}

RECOMP_HOOK("func_809A10F4")
void ObjHamishi_Broken(ObjHamishi* this, PlayState* play) {
    boulder2Location = z64recomp_get_extended_actor_data(&this->actor, boulder2LocationExtension);
    if (!rando_location_is_checked(*boulder2Location)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *boulder2Location);
    }
}