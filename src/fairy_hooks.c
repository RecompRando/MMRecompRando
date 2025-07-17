#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define FAIRY_LIMB_MAX 0x7
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"

ActorExtensionId fairyExtension;
u32* extendedFairyData;

#define LOCATION_FAIRY (0x0F0000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->actor))

void func_8088F5F4(EnElf* this, PlayState* play, s32 sparkleLife);

void EnElf_RandoDraw(Actor* thisx, PlayState* play) {
    EnElf* this = ((EnElf*)thisx);
    func_8088F5F4(this, play, 32);
    
    extendedFairyData = z64recomp_get_extended_actor_data(thisx, fairyExtension);
    u32 getItemId = rando_get_item_id(*extendedFairyData);
    u16 objectId = getObjectId(getItemId);

    Matrix_Scale(20.0f, 20.0f, 20.0f, MTXMODE_APPLY);
    if (isAP(getItemId)) {
        GetItem_Draw(play, getGid(getItemId));
    // } else if (ObjLoad(play, 0x06, objectId)) {
    //     GetItem_Draw(play, getGid(getItemId));
    //     ObjUnload(play, 0x06, objectId);
    } else {
        GetItem_Draw(play, GID_APLOGO_USEFUL);
    }
}

RECOMP_HOOK("func_8088CC48")
void EnElf_RandoFairyReplace(EnElf* this, PlayState* play) {
    extendedFairyData = z64recomp_get_extended_actor_data(&this->actor, fairyExtension);
    *extendedFairyData = LOCATION_FAIRY;
    this->actor.draw = EnElf_RandoDraw;
    recomp_printf("fairy: 0x%06X\n", *extendedFairyData);
}

// TODO: in-game text boxes
RECOMP_HOOK("func_8088E0F0")
void EnElf_RandoFairyTouched(EnElf* this, PlayState* play) {
    extendedFairyData = z64recomp_get_extended_actor_data(&this->actor, fairyExtension);
    if (!rando_location_is_checked_async(*extendedFairyData)) {
        rando_send_location(*extendedFairyData);
    }
}