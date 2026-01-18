#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "rando_colors.h"

#include "overlays/actors/ovl_Obj_Comb/z_obj_comb.h"

#include "actor_helpers.h"

#define LOCATION_SKULL_TOKEN (0x060000 | (play->sceneId << 8) | OBJCOMB_GET_1F(&this->actor))
#define LOCATION_BEEHIVE (AP_PREFIX_BEEHIVES | (sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

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

u16 grayBeehiveTex[1024];
u16 originalBeehiveTex[1024];
extern u16 gBeehiveTex[];

RECOMP_HOOK("ObjComb_Init")
void OnObjComb_Init(Actor* thisx, PlayState* play) {
    u32* beehiveLocation = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    s16 sceneId = play->sceneId;
    
    // handle grottos
    if (sceneId == SCENE_KAKUSIANA) {
        sceneId = getCurrentGrotto(play);
    }

    *beehiveLocation = LOCATION_BEEHIVE;
    
    // grab texture for camc
    Lib_MemCpy(originalBeehiveTex, SEGMENTED_TO_K0(gBeehiveTex), sizeof(originalBeehiveTex));
    RGBA16toIA16_Texture(originalBeehiveTex, grayBeehiveTex, ARRAY_COUNT(originalBeehiveTex), GRAYSCALE_OOTMM);
}

RECOMP_HOOK("func_8098D8C8")
void ObjComb_SpawnItemWithBees(ObjComb* this, PlayState* play) {
    u32* beehiveLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);
    if (rando_get_slotdata_u32("hivesanity") && !rando_location_is_checked(*beehiveLocation)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *beehiveLocation);
        return;
    }
}

RECOMP_PATCH void func_8098D870(ObjComb* this, PlayState* play) {
    s32 temp_v0 = func_800A8150(OBJCOMB_GET_3F(&this->actor));

    u32* beehiveLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);
    if (rando_get_slotdata_u32("hivesanity") && temp_v0 != ITEM00_HEART_PIECE && !rando_location_is_checked(*beehiveLocation)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ((OBJCOMB_GET_7F00(&this->actor)) << 8) | temp_v0, *beehiveLocation);
        return;
    }

    if (temp_v0 >= 0) {
        Item_DropCollectible(play, &this->actor.world.pos, ((OBJCOMB_GET_7F00(&this->actor)) << 8) | temp_v0);
    }
}

extern Gfx gBeehiveDL[];
extern Gfx randoBeehiveDL[];

RECOMP_PATCH void ObjComb_Draw(Actor* thisx, PlayState* play) {
    ObjComb* this = ((ObjComb*)thisx);
    
    s32 itemDrop = func_800A8150(OBJCOMB_GET_3F(&this->actor));
    s32 combType = OBJCOMB_GET_8000(&this->actor) | OBJCOMB_GET_80(&this->actor);
    u32* beehiveLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Matrix_Translate(this->actor.world.pos.x, this->actor.world.pos.y + (118.0f * this->actor.scale.y),
                     this->actor.world.pos.z, MTXMODE_NEW);
    Matrix_RotateYS(this->actor.shape.rot.y, MTXMODE_APPLY);
    Matrix_RotateXS(this->actor.shape.rot.x, MTXMODE_APPLY);
    Matrix_RotateZS(this->actor.shape.rot.z, MTXMODE_APPLY);
    Matrix_Translate(0.0f, -(this->actor.scale.y * 118.0f), 0.0f, MTXMODE_APPLY);
    Matrix_Scale(this->actor.scale.x, this->actor.scale.y, this->actor.scale.z, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    
    Color_RGB8 color;
    if (rando_get_slotdata_u32("hivesanity") && get_rando_color(&color, *beehiveLocation)
        && itemDrop != ITEM00_HEART_PIECE && (combType == 0 || combType == 1)) {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color.r, color.g, color.b, 255);
        gSPDisplayList(POLY_OPA_DISP++, randoBeehiveDL);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, gBeehiveDL);
    }

    Collider_UpdateSpheres(0, &this->collider);

    CLOSE_DISPS(play->state.gfxCtx);
}