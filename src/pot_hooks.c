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

bool ObjTsubo_IsZoraPotGame(Actor* actor, PlayState* play) {
    // ignore if not in zora cape
    if (play->sceneId != SCENE_31MISAKI) return false;

    // rough positioning check
    if (actor->world.pos.x > 2000.0f || actor->world.pos.x < 0.0f) return false;

    return true;
}

// Normal Pots (including green pots)
RECOMP_HOOK("ObjTsubo_Init")
void OnObjTsubo_Init(Actor* thisx, PlayState* play) {
    u32* potLocation = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    *potLocation = LOCATION_POT;
    bool* potDropped = z64recomp_get_extended_actor_data(thisx, actorDroppedExtension);
    *potDropped = false;

    // replace location for camc
    // skulltulas (only matters if we don't drop an item as well...)
    // if (OBJ_TSUBO_ZROT(thisx) == 2) {
    //     s32 skulltulaParams = (OBJ_TSUBO_P001F(thisx) << 2) | 0xFF01;
    //     s32 chestFlag = ENSW_GETS_3FC(skulltulaParams);
    //     *potLocation = (0x060000 | (play->sceneId << 8) | chestFlag);
    // }

    // replace known stray fairy locations
    switch (LOCATION_POT) {
        case 0x201B16:
        case 0x204902:
        case 0x20491B:
        case 0x204947:
            *potLocation = (0x010000 | (play->sceneId << 8) | OBJ_TSUBO_PFE00(thisx));
            break;
    }

    // because new pot actors are spawned in for this game, it messes with our existing systems
    // these pots have to be locationally encoded (as seen below)
    // but new pots will show as being unchecked/dupe drops
    if (ObjTsubo_IsZoraPotGame(thisx, play)) {
        u8 potNum = 0;
        // using world x position to identify each pot
        switch ((s32)thisx->world.pos.x) {
            case 1232:
                potNum = 1;
                break;
            case 1376:
                potNum = 2;
                break;
            case 1347:
                potNum = 3;
                break;
            case 1469:
                potNum = 4;
                break;
            case 1391:
                potNum = 5;
                break;
        }
        // we pretend these pots are in room 1
        *potLocation = (AP_PREFIX_POTS | (play->sceneId << 8) | 0x10 | potNum);
    }
}

RECOMP_PATCH void func_8092762C(ObjTsubo* this, PlayState* play) {
    u32* potLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);
    bool* potDropped = z64recomp_get_extended_actor_data(&this->actor, actorDroppedExtension);
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
    u32* potLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);
    bool* potDropped = z64recomp_get_extended_actor_data(&this->actor, actorDroppedExtension);
    if (rando_get_slotdata_u32("potsanity") && !rando_location_is_checked(*potLocation) && !(*potDropped)) {
        itemDrop = func_800A8150(OBJ_TSUBO_P003F(&this->actor));
        // recomp_printf("pot location 0x%06X\n", *potLocation);
        // recomp_printf("drop 0x%02X 0x%02X\n", OBJ_TSUBO_PFE00(&this->actor), itemDrop);
        Actor* item = Item_RandoDropCollectible(play, &this->actor.world.pos, (OBJ_TSUBO_PFE00(&this->actor) << 8) | itemDrop, *potLocation);

        if (ObjTsubo_IsZoraPotGame(&this->actor, play)) {
            // make these items fly higher so they don't get stuck in the respawning pots
            item->velocity.y = 10.0f;
        }

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
    u32* potLocation = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    *potLocation = LOCATION_POT_FLYING;
}

RECOMP_PATCH void EnTuboTrap_DropCollectible(EnTuboTrap* this, PlayState* play) {
    s32 itemParam = ((this->actor.params >> 8) & 0x3F);
    s32 dropItem00Id = func_800A8150(itemParam);
    
    u32* potLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);
    if (rando_get_slotdata_u32("potsanity") && !rando_location_is_checked(*potLocation)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ((this->actor.params & 0x7F) << 8) | dropItem00Id, *potLocation);
        return;
    }
    
    if (dropItem00Id > ITEM00_NO_DROP) {
        Item_DropCollectible(play, &this->actor.world.pos, ((this->actor.params & 0x7F) << 8) | dropItem00Id);
    }
}

// Flower Pots
RECOMP_HOOK("ObjFlowerpot_Init")
void OnObjFlowerpot_Init(Actor* thisx, PlayState* play) {
    u32* potLocation = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    *potLocation = LOCATION_POT_FLOWER;
}

// can technically be made into a hook
RECOMP_PATCH void func_80A1B914(ObjFlowerpot* this, PlayState* play) {
    u32* potLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);
    if (rando_get_slotdata_u32("potsanity") && !rando_location_is_checked(*potLocation)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *potLocation);
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