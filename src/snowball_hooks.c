#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Snowball/z_obj_snowball.h"
#include "overlays/actors/ovl_Obj_Snowball2/z_obj_snowball2.h"

#define LOCATION_SNOWBALL (0x210000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_SNOWBALL))
#define LOCATION_SNOWBALL_BIG (0x210000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_SNOWBALL2))

ActorExtensionId snowballExtension;
u32* extendedSnowballData;
ActorExtensionId bigSnowballExtension;
u32* extendedBigSnowballData;

// small snowball
RECOMP_HOOK("ObjSnowball2_Init")
void OnObjSnowball2_Init(Actor* thisx, PlayState* play) {
    extendedSnowballData = z64recomp_get_extended_actor_data(thisx, snowballExtension);
    *extendedSnowballData = LOCATION_SNOWBALL;
}

RECOMP_PATCH void func_80B38E88(ObjSnowball2* this, PlayState* play) {
    s32 temp_v0;

    if (this->unk_1AE == 0) {
        temp_v0 = func_800A8150(ENOBJSNOWBALL2_GET_3F(&this->actor));
        
        extendedSnowballData = z64recomp_get_extended_actor_data(&this->actor, snowballExtension);
        if (!rando_location_is_checked(*extendedSnowballData)) {
            Item_RandoDropCollectible(play, &this->actor.world.pos, (ENOBJSNOWBALL2_GET_7F00(&this->actor) << 8) | temp_v0, *extendedSnowballData);
            this->unk_1AE = 1;
            return;
        }

        if (temp_v0 >= 0) {
            Item_DropCollectible(play, &this->actor.world.pos, (ENOBJSNOWBALL2_GET_7F00(&this->actor) << 8) | temp_v0);
            this->unk_1AE = 1;
        }
    }
}

// large snowball
RECOMP_HOOK("ObjSnowball_Init")
void OnObjSnowball_Init(Actor* thisx, PlayState* play) {
    extendedBigSnowballData = z64recomp_get_extended_actor_data(thisx, bigSnowballExtension);
    *extendedBigSnowballData = LOCATION_SNOWBALL_BIG;
}

s32 func_800A8150(s32 index);

RECOMP_HOOK("func_80B03FF8")
void on_func_80B03FF8(ObjSnowball* this, PlayState* play) {
    // s16 rotY = this->actor.home.rot.y; // snowball type (only 0 normally drops items)
    s32 temp_v0 = func_800A8150(OBJSNOWBALL_GET_SWITCH_FLAG(&this->actor));
    extendedBigSnowballData = z64recomp_get_extended_actor_data(&this->actor, bigSnowballExtension);
    if (!rando_location_is_checked(*extendedBigSnowballData)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, (OBJSNOWBALL_GET_7F00(&this->actor) << 8) | temp_v0, *extendedBigSnowballData);
        return;
    }
}

RECOMP_PATCH void func_80B02D58(ObjSnowball* this, PlayState* play) {
    s32 temp_v0 = func_800A8150(OBJSNOWBALL_GET_SWITCH_FLAG(&this->actor));

    extendedBigSnowballData = z64recomp_get_extended_actor_data(&this->actor, bigSnowballExtension);
    if (rando_location_is_checked(*extendedBigSnowballData)) {
        if (temp_v0 >= 0) {
            Item_DropCollectible(play, &this->actor.home.pos, (OBJSNOWBALL_GET_7F00(&this->actor) << 8) | temp_v0);
        }
    }
}

void func_80B03FF8(ObjSnowball* this, PlayState* play);
void func_80B04608(ObjSnowball* this, PlayState* play);
void func_80B046E4(ObjSnowball* this, PlayState* play);

// cutscene skip
RECOMP_PATCH void func_80B0457C(ObjSnowball* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->actor.csId)) {
        // CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
        func_80B03FF8(this, play);
        this->unk_20B = 1;
        if (this->unk_20A == 0) {
            func_80B04608(this, play);
        } else {
            func_80B046E4(this, play);
        }
    } else {
        CutsceneManager_Queue(this->actor.csId);
    }
}