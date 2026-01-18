#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Bombiwa/z_obj_bombiwa.h"
#include "overlays/actors/ovl_Obj_Hamishi/z_obj_hamishi.h"

#define LOCATION_BOULDER (AP_PREFIX_BOULDERS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                        | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_HAMISHI))
#define LOCATION_BOULDER2 (AP_PREFIX_BOULDERS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                        | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_BOMBIWA))

// normal boulders
RECOMP_HOOK("ObjBombiwa_Init")
void OnObjBombiwa_Init(Actor* thisx, PlayState* play) {
    u32* boulderLocation = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
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
    u32* boulderLocation;

    if (Flags_GetSwitch(play, OBJBOMBIWA_GET_SWITCH_FLAG(&this->actor))) {
        boulderLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);
        if (rando_get_slotdata_u32("rocksanity") && !rando_location_is_checked(*boulderLocation)) {
            Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *boulderLocation);
        }
    }
}

// bronze boulders
RECOMP_HOOK("ObjHamishi_Init")
void OnObjHamishi_Init(Actor* thisx, PlayState* play) {
    u32* boulder2Location = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    *boulder2Location = LOCATION_BOULDER2;
}

RECOMP_HOOK("func_809A10F4")
void ObjHamishi_Broken(ObjHamishi* this, PlayState* play) {
    u32* boulder2Location = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);
    if (rando_get_slotdata_u32("rocksanity") && !rando_location_is_checked(*boulder2Location)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *boulder2Location);
    }
}