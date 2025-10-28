#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Tsubo/z_obj_tsubo.h"
#include "overlays/actors/ovl_En_Tubo_Trap/z_en_tubo_trap.h"
#include "overlays/actors/ovl_Obj_Flowerpot/z_obj_flowerpot.h"
#include "overlays/actors/ovl_En_Sw/z_en_sw.h"

#define LOCATION_POT (AP_PREFIX_POTS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_EN_TUBO_TRAP))
#define LOCATION_POT_FLYING (AP_PREFIX_POTS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_TSUBO))
#define LOCATION_POT_FLOWER (AP_PREFIX_POTS | (play->sceneId << 8) | (0xF << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

ActorExtensionId potLocationExtension;
u32* potLocation;
ActorExtensionId potDroppedExtension;
bool* potDropped;
ActorExtensionId potFlyingLocationExtension;
u32* potFlyingLocation;
ActorExtensionId potFlowerLocationExtension;
u32* potFlowerLocation;

// note: different pot behaviors aren't accounted for yet

// Normal Pots (including green pots)
RECOMP_HOOK("ObjTsubo_Init")
void OnObjTsubo_Init(Actor* thisx, PlayState* play) {
    potLocation = z64recomp_get_extended_actor_data(thisx, potLocationExtension);
    *potLocation = LOCATION_POT;
    potDropped = z64recomp_get_extended_actor_data(thisx, potDroppedExtension);
    *potDropped = false;

    // replace location for camc
    // skulltulas (only matters if we don't drop an item as well...)
    // if (OBJ_TSUBO_ZROT(thisx) == 2) {
    //     s32 skulltulaParams = (OBJ_TSUBO_P001F(thisx) << 2) | 0xFF01;
    //     s32 chestFlag = ENSW_GETS_3FC(skulltulaParams);
    //     *potLocation = (0x060000 | (play->sceneId << 8) | chestFlag);
    // }

    // should be stray fairies only
    // if (OBJ_TSUBO_PFE00(thisx) && (func_800A8150(OBJ_TSUBO_P003F(thisx)) != ITEM00_FLEXIBLE)) {
    //     *potLocation = (0x010000 | (play->sceneId << 8) | OBJ_TSUBO_PFE00(thisx));
    // }
}

RECOMP_PATCH void func_8092762C(ObjTsubo* this, PlayState* play) {
    potLocation = z64recomp_get_extended_actor_data(&this->actor, potLocationExtension);
    potDropped = z64recomp_get_extended_actor_data(&this->actor, potDroppedExtension);
    if (rando_get_slotdata_u32("potsanity") && !rando_location_is_checked(*potLocation) && !(*potDropped)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *potLocation);
        *potDropped = true;
        return;
    } 

    if (!OBJ_TSUBO_P0010(&this->actor) && (OBJ_TSUBO_ZROT(&this->actor) != 2)) {
        Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, OBJ_TSUBO_P000F(&this->actor) << 4);
    }
}

RECOMP_PATCH void func_80927690(ObjTsubo* this, PlayState* play) {
    s32 itemDrop;
    potLocation = z64recomp_get_extended_actor_data(&this->actor, potLocationExtension);
    potDropped = z64recomp_get_extended_actor_data(&this->actor, potDroppedExtension);
    if (rando_get_slotdata_u32("potsanity") && !rando_location_is_checked(*potLocation) && !(*potDropped)) {
        itemDrop = func_800A8150(OBJ_TSUBO_P003F(&this->actor));
        // recomp_printf("drop 0x%02X 0x%02X\n", OBJ_TSUBO_PFE00(&this->actor), itemDrop);
        Item_RandoDropCollectible(play, &this->actor.world.pos, (OBJ_TSUBO_PFE00(&this->actor) << 8) | itemDrop, *potLocation);
        this->unk_197 = true;
        *potDropped = true;
    }

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
    potFlyingLocation = z64recomp_get_extended_actor_data(thisx, potFlyingLocationExtension);
    *potFlyingLocation = LOCATION_POT_FLYING;
}

RECOMP_PATCH void EnTuboTrap_DropCollectible(EnTuboTrap* this, PlayState* play) {
    s32 itemParam = ((this->actor.params >> 8) & 0x3F);
    s32 dropItem00Id = func_800A8150(itemParam);
    
    potFlyingLocation = z64recomp_get_extended_actor_data(&this->actor, potFlyingLocationExtension);
    if (rando_get_slotdata_u32("potsanity") && !rando_location_is_checked(*potFlyingLocation)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ((this->actor.params & 0x7F) << 8) | dropItem00Id, *potFlyingLocation);
        return;
    }
    
    if (dropItem00Id > ITEM00_NO_DROP) {
        Item_DropCollectible(play, &this->actor.world.pos, ((this->actor.params & 0x7F) << 8) | dropItem00Id);
    }
}

// Flower Pots
RECOMP_HOOK("ObjFlowerpot_Init")
void OnObjFlowerpot_Init(Actor* thisx, PlayState* play) {
    potFlowerLocation = z64recomp_get_extended_actor_data(thisx, potFlowerLocationExtension);
    *potFlowerLocation = LOCATION_POT_FLOWER;
}

// can technically be made into a hook
RECOMP_PATCH void func_80A1B914(ObjFlowerpot* this, PlayState* play) {
    potFlowerLocation = z64recomp_get_extended_actor_data(&this->actor, potFlowerLocationExtension);
    if (rando_get_slotdata_u32("potsanity") && !rando_location_is_checked(*potFlowerLocation)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *potFlowerLocation);
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