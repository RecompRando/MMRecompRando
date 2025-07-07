#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Tsubo/z_obj_tsubo.h"
#include "overlays/actors/ovl_En_Tubo_Trap/z_en_tubo_trap.h"
#include "overlays/actors/ovl_Obj_Flowerpot/z_obj_flowerpot.h"

#define LOCATION_POT (0x200000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_EN_TUBO_TRAP))
#define LOCATION_POT_FLYING (0x200000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_TSUBO))
#define LOCATION_POT_FLOWER (0x200000 | (play->sceneId << 8) | (0xF << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

ActorExtensionId potExtension;
u32* extendedPotData;
ActorExtensionId potFlyingExtension;
u32* extendedPotFlyingData;
ActorExtensionId potFlowerExtension;
u32* extendedPotFlowerData;

// note: different pot behaviors aren't accounted for yet

// Normal Pots (including green pots)
RECOMP_HOOK("ObjTsubo_Init")
void OnObjTsubo_Init(Actor* thisx, PlayState* play) {
    extendedPotData = z64recomp_get_extended_actor_data(thisx, potExtension);
    *extendedPotData = LOCATION_POT;
}

RECOMP_PATCH void func_8092762C(ObjTsubo* this, PlayState* play) {
    extendedPotData = z64recomp_get_extended_actor_data(&this->actor, potExtension);
    if (!rando_location_is_checked(*extendedPotData)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedPotData);
        return;
    } 

    if (!OBJ_TSUBO_P0010(&this->actor) && (OBJ_TSUBO_ZROT(&this->actor) != 2)) {
        Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, OBJ_TSUBO_P000F(&this->actor) << 4);
    }
}

// TODO: turn into hook after confirming this works as intended
RECOMP_PATCH void func_80927690(ObjTsubo* this, PlayState* play) {
    if (!rando_location_is_checked(*extendedPotData)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedPotData);
        this->unk_197 = true;
    }

    s32 itemDrop;
    extendedPotData = z64recomp_get_extended_actor_data(&this->actor, potExtension);
    if (!this->unk_197 && (OBJ_TSUBO_ZROT(&this->actor) != 2)) {
        itemDrop = func_800A8150(OBJ_TSUBO_P003F(&this->actor));
        if (itemDrop > ITEM00_NO_DROP) {
            Item_DropCollectible(play, &this->actor.world.pos, (OBJ_TSUBO_PFE00(&this->actor) << 8) | itemDrop);
            this->unk_197 = true;
        }
    }
}

// Flying Pots
RECOMP_HOOK("EnTuboTrap_Init")
void OnEnTuboTrap_Init(Actor* thisx, PlayState* play) {
    extendedPotFlyingData = z64recomp_get_extended_actor_data(thisx, potFlyingExtension);
    *extendedPotFlyingData = LOCATION_POT_FLYING;
}

RECOMP_PATCH void EnTuboTrap_DropCollectible(EnTuboTrap* this, PlayState* play) {
    extendedPotFlyingData = z64recomp_get_extended_actor_data(&this->actor, potFlyingExtension);
    if (!rando_location_is_checked(*extendedPotFlyingData)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedPotFlyingData);
        return;
    }
    
    s32 itemParam = ((this->actor.params >> 8) & 0x3F);
    s32 dropItem00Id = func_800A8150(itemParam);

    if (dropItem00Id > ITEM00_NO_DROP) {
        Item_DropCollectible(play, &this->actor.world.pos, ((this->actor.params & 0x7F) << 8) | dropItem00Id);
    }
}

// Flower Pots
RECOMP_HOOK("ObjFlowerpot_Init")
void OnObjFlowerpot_Init(Actor* thisx, PlayState* play) {
    extendedPotFlowerData = z64recomp_get_extended_actor_data(thisx, potFlowerExtension);
    *extendedPotFlowerData = LOCATION_POT_FLOWER;
}

// can technically be made into a hook
RECOMP_PATCH void func_80A1B914(ObjFlowerpot* this, PlayState* play) {
    extendedPotFlowerData = z64recomp_get_extended_actor_data(&this->actor, potFlowerExtension);
    if (!rando_location_is_checked(*extendedPotFlowerData)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedPotFlowerData);
        return;
    }
    
    if (!(this->unk_1EA & 4)) {
        s32 temp_v0 = func_800A8150(ENOBJFLOWERPOT_GET_3F(&this->actor));

        if (temp_v0 >= 0) {
            s32 params = ENOBJFLOWERPOT_GET_7F00(&this->actor);

            Item_DropCollectible(play, &this->actor.world.pos, temp_v0 | (params << 8));
            this->unk_1EA |= 4;
        }
    }
}