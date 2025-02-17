#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct ObjComb;

typedef void (*ObjCombActionFunc)(struct ObjComb*, PlayState*);

#define OBJCOMB_GET_F(thisx) ((thisx)->params & 0xF)
#define OBJCOMB_GET_10(thisx) (((thisx)->params >> 4) & 0x1)
#define OBJCOMB_GET_1F(thisx) ((thisx)->params & 0x1F)
#define OBJCOMB_GET_3F(thisx) ((thisx)->params & 0x3F)
#define OBJCOMB_GET_80(thisx) (((thisx)->params >> 7) & 0x1)
#define OBJCOMB_GET_7F00(thisx) (((thisx)->params >> 0x8) & 0x7F)
#define OBJCOMB_GET_8000(thisx) (((thisx)->params >> 0xE) & 2)

#define LOCATION_SKULL_TOKEN (0x060000 | (play->sceneId << 8) | OBJCOMB_GET_1F(&this->actor))

typedef struct ObjComb {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ObjCombActionFunc actionFunc;
    /* 0x148 */ ColliderJntSph collider;
    /* 0x168 */ ColliderJntSphElement colliderElements[1];
    /* 0x1A8 */ s16 unk_1A8;
    /* 0x1AA */ s16 unk_1AA;
    /* 0x1AC */ s16 unk_1AC;
    /* 0x1AE */ s16 unk_1AE;
    /* 0x1B0 */ s16 unk_1B0;
    /* 0x1B2 */ s8 unk_1B2;
    /* 0x1B3 */ s8 unk_1B3;
    /* 0x1B4 */ s8 unk_1B4;
    /* 0x1B5 */ s8 unk_1B5;
    /* 0x1B6 */ s8 unk_1B6;
    /* 0x1B7 */ s8 unk_1B7;
    /* 0x1B8 */ s8 unk_1B8;
} ObjComb; // size = 0x1BC

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
        if (this->collider.elements[0].base.acHitElem->atDmgInfo.dmgFlags & 0x0182C29C) {
            func_8098CEAC(this, play);
            func_8098DA74(this, play);
            Actor_Kill(&this->actor);
        } else {
            s32 dmgFlags = this->collider.elements[0].base.acHitElem->atDmgInfo.dmgFlags;

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
                    this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
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