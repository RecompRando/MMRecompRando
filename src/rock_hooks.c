#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_En_Ishi/z_en_ishi.h"
#include "overlays/actors/ovl_Obj_Hamishi/z_obj_hamishi.h"

#define LOCATION_ROCK (0x180000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                        | randoGetLoadedActorNumInSameRoomExtra(play, actor, ACTOR_OBJ_HAMISHI))
#define LOCATION_BOULDER2 (0x180000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                        | randoGetLoadedActorNumInSameRoomExtra(play, actor, ACTOR_EN_ISHI))

ActorExtensionId rockExtension;
u32* extendedRockData;
ActorExtensionId boulder2Extension;
u32* extendedBoulder2Data;

// TODO: add these to symbols
static s16 D_8095F76C[] = { -1, 1, 2, 20, 8, 0 };
static Vec3f D_8095F778 = { 0.0f, 1.0f, 0.0f };

// RECOMP_HOOK("EnIshi_Init")
// void OnEnIshi_Init(Actor* thisx, PlayState* play) {
//     extendedRockData = z64recomp_get_extended_actor_data(thisx, rockExtension);
//     *extendedRockData = LOCATION_ROCK;
// }

// note: this *should* only run when an actor is spawned on scene/room load
RECOMP_HOOK_RETURN("Actor_SpawnEntry")
void add_rock_and_boulder_locations() {
    Actor* actor = recomphook_get_return_ptr();
    if (actor == NULL) {
        return;
    }

    PlayState* play = gPlay;

    // rocks/silver boulders
    if (actor->id == ACTOR_EN_ISHI) {
        extendedRockData = z64recomp_get_extended_actor_data(actor, rockExtension);
        *extendedRockData = LOCATION_ROCK;
    // bronze boulders
    } else if (actor->id == ACTOR_OBJ_HAMISHI) {
        extendedBoulder2Data = z64recomp_get_extended_actor_data(actor, boulder2Extension);
        *extendedBoulder2Data = LOCATION_BOULDER2;
    }
}

RECOMP_PATCH void func_8095DF90(EnIshi* this, PlayState* play) {
    extendedRockData = z64recomp_get_extended_actor_data(&this->actor, rockExtension);
    if (!rando_location_is_checked(*extendedRockData)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedRockData);
        return;
    }

    // note: putting the item drop outside of this makes some normally impossible drops happen
    if (!ENISHI_GET_1(&this->actor) && !ENISHI_GET_100(&this->actor)) {
        Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos, ENISHI_GET_F0(&this->actor) * 0x10);
    }
}

RECOMP_PATCH void func_8095DFF0(EnIshi* this, PlayState* play) {
    s32 pad;
    s32 temp = D_8095F76C[ENISHI_GET_70(&this->actor)];
    Actor* sp3C;
    Vec3f sp30;
    f32 sp2C;
    f32 temp_f2;
    s16 temp_v1_2;

    if (temp >= 0) {
        extendedRockData = z64recomp_get_extended_actor_data(&this->actor, rockExtension);
        if (!rando_location_is_checked(*extendedRockData)) {
            sp3C = Item_RandoDropCollectible(play, &this->actor.world.pos, temp | (ENISHI_GET_FLAG(&this->actor) << 8), *extendedRockData);
        } else {
            sp3C = Item_DropCollectible(play, &this->actor.world.pos, temp | (ENISHI_GET_FLAG(&this->actor) << 8));
        }

        if (sp3C != NULL) {
            Matrix_Push();
            Matrix_RotateYS(this->actor.shape.rot.y, MTXMODE_NEW);
            Matrix_RotateXS(this->actor.shape.rot.x, MTXMODE_APPLY);
            Matrix_RotateZS(this->actor.shape.rot.z, MTXMODE_APPLY);
            Matrix_MultVecY(1.0f, &sp30);
            sp2C = Math3D_Parallel(&sp30, &D_8095F778);
            if (sp2C < 0.707f) {
                temp_v1_2 = Math_Atan2S_XY(sp30.z, sp30.x) - sp3C->world.rot.y;
                if (ABS_ALT(temp_v1_2) > 0x4000) {
                    sp3C->world.rot.y = BINANG_ROT180(sp3C->world.rot.y);
                }
                temp_f2 = sp2C + 0.5f;
                if (temp_f2 < 0.5f) {
                    temp_f2 = 0.5f;
                }
                sp3C->velocity.y *= temp_f2;
            }
            Matrix_Pop();
        }
    }
}

// bronze boulders
RECOMP_HOOK("func_809A10F4")
void ObjHamishi_Broken(ObjHamishi* this, PlayState* play) {
    extendedBoulder2Data = z64recomp_get_extended_actor_data(&this->actor, boulder2Extension);
    if (!rando_location_is_checked(*extendedBoulder2Data)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedBoulder2Data);
    }
}