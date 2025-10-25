#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define FAIRY_LIMB_MAX 0x7
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"

ActorExtensionId fairyLocationExtension;
u32* fairyLocation;

#define LOCATION_FAIRY (0x0F0000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->actor))

extern u32* item00Location;

void Item_RandoCollectibleDraw(Actor* thisx, PlayState* play);
void EnItem00_RandoGive(EnItem00* this, PlayState* play, s32 getItemId, u32 location);

void func_8088F5F4(EnElf* this, PlayState* play, s32 sparkleLife);

void EnElf_RandoDraw(Actor* thisx, PlayState* play) {
    EnElf* this = ((EnElf*)thisx);
    func_8088F5F4(this, play, 32);
    
    fairyLocation = z64recomp_get_extended_actor_data(thisx, fairyLocationExtension);
    u32 getItemId = rando_get_item_id(*fairyLocation);
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

RECOMP_HOOK("EnElf_Init")
void OnEnElf_Init(Actor* thisx, PlayState* play2) {
    fairyLocation = z64recomp_get_extended_actor_data(thisx, fairyLocationExtension);
    *fairyLocation = 0;
}

RECOMP_HOOK("func_8088CC48")
void EnElf_RandoFairyReplace(EnElf* this, PlayState* play) {
    fairyLocation = z64recomp_get_extended_actor_data(&this->actor, fairyLocationExtension);
    if (this->actor.params == FAIRY_TYPE_6) {
        *fairyLocation = LOCATION_FAIRY;
        this->actor.draw = EnElf_RandoDraw;
    }
}

RECOMP_HOOK("func_8088E0F0")
void EnElf_RandoFairyTouched(EnElf* this, PlayState* play) {
    fairyLocation = z64recomp_get_extended_actor_data(&this->actor, fairyLocationExtension);
    
    if (!*fairyLocation) {
        return;
    }

    if (!rando_location_is_checked(*fairyLocation) && !this->unk_246) { // trying to make it print less
        Actor* item00 = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ITEM00, this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, 0,
                                0, 0, ITEM00_APITEM);
        item00Location = z64recomp_get_extended_actor_data(item00, item00LocationExtension);
        *item00Location = *fairyLocation;

        item00->draw = Item_RandoCollectibleDraw;
        Actor_SetScale(item00, 0.0f);
        ((EnItem00*)item00)->getItemId = rando_get_item_id(*fairyLocation);
        EnItem00_RandoGive(((EnItem00*)item00), play, ((EnItem00*)item00)->getItemId, *fairyLocation);
        recomp_printf("fairy: 0x%06X\n", *fairyLocation);
    }
}