#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Yasi/z_obj_yasi.h" // palm trees
#include "overlays/actors/ovl_En_Snowwd/z_en_snowwd.h" // snow covered trees
#include "overlays/actors/ovl_En_Wood02/z_en_wood02.h" // generic trees?
#include "overlays/actors/ovl_Obj_Tree/z_obj_tree.h" // beefier tree

#define LOCATION_PALM_TREE (0x2A0000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))
#define LOCATION_SNOW_TREE (0x2B0000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))
#define LOCATION_WOOD_TREE (0x2C0000 | (play->sceneId << 8) | EnWood02_GetIDHash(this))
#define LOCATION_THIC_TREE (0x2D0000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->dyna.actor))

ActorExtensionId palmTreeDropExtension;
// ActorExtensionId snowTreeDropExtension; // already handled by actor
ActorExtensionId woodTreeDropExtension;
ActorExtensionId bigTreeDropExtension;

// palm trees
RECOMP_HOOK("ObjYasi_Update")
void OnObjYasi_Update(Actor* thisx, PlayState* play) {
    ObjYasi* this = (ObjYasi*)thisx;
    Vec3f dropPos;

    bool* dropped = z64recomp_get_extended_actor_data(thisx, palmTreeDropExtension);

    if (this->dyna.actor.home.rot.z != 0 && rando_get_slotdata_u32("treesanity") && !rando_location_is_checked(LOCATION_PALM_TREE) && !(*dropped)) {
        // if (CAN_DROP_NUT(thisx)) {
        //     if (Rand_ZeroOne() < 0.5f) {
                dropPos.x = this->dyna.actor.world.pos.x;
                dropPos.y = this->dyna.actor.world.pos.y + 280.0f;
                dropPos.z = this->dyna.actor.world.pos.z;
                // Item_DropCollectible(play, &dropPos, ITEM00_DEKU_NUTS_1);
                Item_RandoDropCollectible(play, &dropPos, ITEM00_DEKU_NUTS_1, LOCATION_PALM_TREE);
        //     }
        // }
        this->dyna.actor.home.rot.y = GET_PLAYER(play)->actor.shape.rot.y;
        this->dyna.actor.home.rot.x = 400;
        this->dyna.actor.home.rot.z = 0;
        *dropped = true;
    }
}

// snow covered trees
RECOMP_HOOK("EnSnowwd_Idle")
void OnEnSnowwd_Idle(EnSnowwd* this, PlayState* play) {
    Actor* thisx = &this->actor;
    Vec3f pos;

    if (thisx->home.rot.y != 0) {
        this->timer = 21;
        thisx->home.rot.y = 0; // should prevent normal drops from happening

        if (!SNOWWD_DROPPED_COLLECTIBLE(&this->actor) && rando_get_slotdata_u32("treesanity") && !rando_location_is_checked(LOCATION_SNOW_TREE)) {
            pos = thisx->world.pos;
            pos.y += 200.0f;

            Item_RandoDropCollectible(play, &pos, SNOWWD_GET_DROP_TABLE(&this->actor) * DROP_TABLE_SIZE, LOCATION_SNOW_TREE);
            SNOWWD_DROPPED_COLLECTIBLE(&this->actor) = true;
        }
    }
}

// generic trees/bushes?
u16 EnWood02_GetIDHash(EnWood02* this) {
    // tree/bush indexes aren't constant so a hashing approach is used instead
    u16 xPos = (u16)(this->actor.home.pos.x);
    u16 zPos = (u16)(this->actor.home.pos.z);
    return (xPos ^ zPos) & 0xFF;
}

RECOMP_HOOK("func_808C4458")
void EnWood02_DropRandoItem(EnWood02* this, PlayState* play, Vec3f* dropsSpawnPt, u16 arg3) {
    bool* dropped = z64recomp_get_extended_actor_data(&this->actor, woodTreeDropExtension);
    recomp_printf("draw type %d\n", this->drawType);
    
    // no idea how this is meant to work, could be simplified further with better understanding
    if (rando_get_slotdata_u32("treesanity") && !rando_location_is_checked(LOCATION_WOOD_TREE) && !(*dropped)) {
        // if ((this->unk_144 == -1) || !Flags_GetCollectible(play, this->unk_144)) {
            if ((this->unk_148 < 0) && (this->unk_148 >= -0x10)) {
                if (arg3 & 1) {
                    Item_RandoDropCollectible(play, dropsSpawnPt, ITEM00_APITEM | 0x8000, LOCATION_WOOD_TREE);
                } else {
                    Item_RandoDropCollectible(play, dropsSpawnPt, ITEM00_APITEM, LOCATION_WOOD_TREE);
                }
            } else {
                Item_RandoDropCollectible(play, dropsSpawnPt, ITEM00_APITEM, LOCATION_WOOD_TREE);
            }
        // }

        this->unk_148 = 0;
        *dropped = true;
    }
}

// beefier trees
RECOMP_HOOK("ObjTree_SetupSway")
void OnObjTree_SetupSway(ObjTree* this) {
    PlayState* play = gPlay;
    bool* dropped = z64recomp_get_extended_actor_data(&this->dyna.actor, bigTreeDropExtension);

    if (rando_get_slotdata_u32("treesanity") && !rando_location_is_checked(LOCATION_THIC_TREE) && !(*dropped)) {
        Vec3f dropPos;

        dropPos.x = this->dyna.actor.world.pos.x;
        dropPos.y = this->dyna.actor.world.pos.y + 280.0f;
        dropPos.z = this->dyna.actor.world.pos.z;
        
        // TODO: add way to prevent duplicate items from falling after being hit once
        Item_RandoDropCollectible(play, &dropPos, ITEM00_APITEM, LOCATION_THIC_TREE);
        *dropped = true;
    }
}