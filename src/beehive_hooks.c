#include "modding.h"
#include "global.h"

#include "apcommon.h"

#include "overlays/actors/ovl_Obj_Comb/z_obj_comb.h"

#include "actor_helpers.h"

#define LOCATION_SKULL_TOKEN (0x060000 | (play->sceneId << 8) | OBJCOMB_GET_1F(&this->actor))
#define LOCATION_BEEHIVE (0x240000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

ActorExtensionId beehiveExtension;
u32* extendedBeehiveData;

bool func_8098CE40(ObjComb* this, PlayState* play);
void func_8098CEAC(ObjComb* this, PlayState* play);
void func_8098DA74(ObjComb* this, PlayState* play);

RECOMP_PATCH void func_8098DC60(ObjComb* this, PlayState* play) {
    s32 temp_a0;
    s8 temp_v0;
    u32 temp_v1;

    this->unk_1AA += this->unk_1AC;
    if (this->unk_1B3) {
        this->collider.base.acFlags &= ~AC_HIT;
    }

    this->unk_1A8 -= 0x32;
    if (this->unk_1A8 < 0) {
        this->unk_1A8 = 0;
    }

    if (this->unk_1B3) {
        if (this->collider.elements->info.acHitInfo->toucher.dmgFlags & 0x0182C29C) {
            func_8098CEAC(this, play);
            func_8098DA74(this, play);
            Actor_Kill(&this->actor);
        } else {
            s32 dmgFlags = this->collider.elements->info.acHitInfo->toucher.dmgFlags;

            if (dmgFlags & 0x13820) {
                this->unk_1A8 = 0xDAC;
                this->unk_1AC = 0x36B0;
            } else {
                this->unk_1A8 = 0x5DC;
                this->unk_1AC = 0x2EE0;
            }

            if ((this->unk_1B2 <= 0) && (dmgFlags & 0x13820)) {
                if (this->unk_1B5 == 0) {
                    this->unk_1B5 = 1;
                    this->actor.flags |= ACTOR_FLAG_10;
                }
                this->unk_1B2 = 20;
            }
        }
    } else {
        if (this->unk_1B8 >= 0) {
            if (this->unk_1B8 == 0) {
                if (!rando_location_is_checked(LOCATION_SKULL_TOKEN)) {
                    Actor_PlaySfx(&this->actor, NA_SE_EN_STALGOLD_ROLL);
                }
                if (Rand_ZeroOne() < 0.1f) {
                    this->unk_1B8 = Rand_S16Offset(40, 80);
                } else {
                    this->unk_1B8 = 8;
                }
            } else {
                this->unk_1B8--;
            }
        }
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }

    if (this->actor.update != NULL) {
        CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
    }

    this->actor.shape.rot.x = (s32)(Math_SinS(this->unk_1AA) * this->unk_1A8) + this->actor.home.rot.x;
}

RECOMP_PATCH void func_8098D99C(ObjComb* this, PlayState* play) {
    s32 params;
    Actor* temp_v0;

    if (func_8098CE40(this, play)) {
        if (rando_location_is_checked(LOCATION_SKULL_TOKEN)) {
            return;
        }
        params = (OBJCOMB_GET_1F(&this->actor) << 2) | 0xFF01;
        temp_v0 = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_SW, this->actor.world.pos.x, this->actor.world.pos.y,
                              this->actor.world.pos.z, 0, this->actor.home.rot.y, 0, params);

        if (temp_v0 != NULL) {
            temp_v0->parent = &this->actor;
            if (this->actionFunc == func_8098DC60) {
                temp_v0->velocity.y = 8.0f;
                temp_v0->speed = 2.0f;
            } else {
                temp_v0->velocity.y = 10.0f;
                temp_v0->speed = 2.0f;
            }
            this->unk_1B6 = 1;
            Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
        }
    }
}

RECOMP_HOOK("ObjComb_Init")
void OnObjComb_Init(Actor* thisx, PlayState* play) {
    extendedBeehiveData = z64recomp_get_extended_actor_data(thisx, beehiveExtension);
    *extendedBeehiveData = LOCATION_BEEHIVE;
}

RECOMP_HOOK("func_8098D8C8")
void ObjComb_SpawnItemWithBees(ObjComb* this, PlayState* play) {
    extendedBeehiveData = z64recomp_get_extended_actor_data(&this->actor, beehiveExtension);
    if (!rando_location_is_checked(*extendedBeehiveData)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedBeehiveData);
        return;
    }
}

RECOMP_PATCH void func_8098D870(ObjComb* this, PlayState* play) {
    s32 temp_v0 = func_800A8150(OBJCOMB_GET_3F(&this->actor));

    extendedBeehiveData = z64recomp_get_extended_actor_data(&this->actor, beehiveExtension);
    if (temp_v0 != ITEM00_HEART_PIECE && !rando_location_is_checked(*extendedBeehiveData)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ((OBJCOMB_GET_7F00(&this->actor)) << 8) | temp_v0, *extendedBeehiveData);
        return;
    }

    if (temp_v0 >= 0) {
        Item_DropCollectible(play, &this->actor.world.pos, ((OBJCOMB_GET_7F00(&this->actor)) << 8) | temp_v0);
    }
}