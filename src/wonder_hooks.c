#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_En_Hit_Tag/z_en_hit_tag.h"
#include "overlays/actors/ovl_En_Invisible_Ruppe/z_en_invisible_ruppe.h"
#include "overlays/actors/ovl_Obj_Dora/z_obj_dora.h"

#define LOCATION_WONDER_HIT (0x150000 | ((play->sceneId + play->roomCtx.curRoom.num) << 8) \
                            | (randoGetLoadedActorNumInSameRoom(play, thisx)) << 4)
#define LOCATION_WONDER_RUPEE (0x160000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))
#define LOCATION_WONDER_HIT_GONG (0x160000 | (play->sceneId << 8))

ActorExtensionId wonderHitExtension;
u32* extendedWonderHitData;
ActorExtensionId wonderRupeeExtension;
u32* extendedWonderRupeeData;

static Vec3f sEffectVelocity = { 0.0f, 0.1f, 0.0f };
static Vec3f sEffectAccel = { 0.0f, 0.01f, 0.0f };
static Color_RGBA8 sEffectPrimColor = { 255, 255, 127, 255 };
static Color_RGBA8 sEffectPrimColor2 = { 127, 255, 255, 255 };
static Color_RGBA8 sEffectEnvColor = { 255, 255, 255, 255 };

// Hit Tag (Invisible hitbox that can spawn rupees)
RECOMP_HOOK("EnHitTag_Init")
void OnEnHitTag_Init(Actor* thisx, PlayState* play) {
    extendedWonderHitData = z64recomp_get_extended_actor_data(thisx, wonderHitExtension);
    *extendedWonderHitData = LOCATION_WONDER_HIT;
}

RECOMP_HOOK("EnHitTag_Update")
void OnEnHitTag_Update(Actor* thisx, PlayState* play) {
    Vec3f pos;

    // TODO: make this a lot nicer
    pos.x = thisx->world.pos.x + ((Rand_ZeroOne() - 0.5f) * 20.0f);
    pos.y = thisx->world.pos.y + ((Rand_ZeroOne() - 0.5f) * 20.0f);
    pos.z = thisx->world.pos.z + ((Rand_ZeroOne() - 0.5f) * 20.0f);
    EffectSsKirakira_SpawnDispersed(play, &pos, &sEffectVelocity, &sEffectAccel, &sEffectPrimColor, &sEffectEnvColor, 4000, 20);
}

// could change to a non-patch by changing actionfunc in init
RECOMP_PATCH void EnHitTag_WaitForHit(EnHitTag* this, PlayState* play) {
    Vec3f dropLocation;
    s32 i;

    extendedWonderHitData = z64recomp_get_extended_actor_data(&this->actor, wonderHitExtension);

    if (this->collider.base.acFlags & AC_HIT) {
        Audio_PlaySfx(NA_SE_SY_GET_RUPY);
        Actor_Kill(&this->actor);
        dropLocation.x = this->actor.world.pos.x;
        dropLocation.y = this->actor.world.pos.y;
        dropLocation.z = this->actor.world.pos.z;

        for (i = 0; i < 3; i++) {
            u32 wonderHitRupeeLocation = *extendedWonderHitData | i;
            if (!rando_location_is_checked(wonderHitRupeeLocation)) {
                Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, wonderHitRupeeLocation);
            } else {
                Item_DropCollectible(play, &dropLocation, ITEM00_RUPEE_GREEN);
            }
        }
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);
    }
}

// Invisible Rupees
RECOMP_HOOK("EnInvisibleRuppe_Init")
void OnEnInvisibleRuppe_Init(Actor* thisx, PlayState* play) {
    extendedWonderRupeeData = z64recomp_get_extended_actor_data(thisx, wonderRupeeExtension);
    *extendedWonderRupeeData = LOCATION_WONDER_RUPEE;
}

RECOMP_HOOK("EnInvisibleRuppe_Update")
void OnEnInvisibleRuppe_Update(Actor* thisx, PlayState* play) {
    Vec3f pos;

    // TODO: make this a lot nicer
    pos.x = thisx->world.pos.x + ((Rand_ZeroOne() - 0.5f) * 20.0f);
    pos.y = thisx->world.pos.y + ((Rand_ZeroOne() - 0.5f) * 20.0f);
    pos.z = thisx->world.pos.z + ((Rand_ZeroOne() - 0.5f) * 20.0f);
    EffectSsKirakira_SpawnDispersed(play, &pos, &sEffectVelocity, &sEffectAccel, &sEffectPrimColor2, &sEffectEnvColor, 4000, 20);
}

void func_80C259E8(EnInvisibleRuppe* this, PlayState* play); // literally just Actor_Kill(&this->actor);

void EnItem00_RandoTextAndFreeze(EnItem00* this, PlayState* play);

RECOMP_PATCH void func_80C2590C(EnInvisibleRuppe* this, PlayState* play) {
    Actor* item;
    if (this->collider.base.ocFlags1 & OC1_HIT) {
        extendedWonderRupeeData = z64recomp_get_extended_actor_data(&this->actor, wonderRupeeExtension);
        if (!rando_location_is_checked(*extendedWonderRupeeData)) {
            item = Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedWonderRupeeData);
            ((EnItem00*)item)->actionFunc = EnItem00_RandoTextAndFreeze;
        } else {
            switch (INVISIBLERUPPE_GET_3(&this->actor)) {
                case 0:
                    Audio_PlaySfx(NA_SE_SY_GET_RUPY);
                    Item_DropCollectible(play, &this->actor.world.pos, 0x8000 | ITEM00_RUPEE_GREEN);
                    break;

                case 1:
                    Audio_PlaySfx(NA_SE_SY_GET_RUPY);
                    Item_DropCollectible(play, &this->actor.world.pos, 0x8000 | ITEM00_RUPEE_BLUE);
                    break;

                case 2:
                    Audio_PlaySfx(NA_SE_SY_GET_RUPY);
                    Item_DropCollectible(play, &this->actor.world.pos, 0x8000 | ITEM00_RUPEE_RED);
                    break;

                default:
                    break;
            }
        }

        if (this->switchFlag > SWITCH_FLAG_NONE) {
            Flags_SetSwitch(play, this->switchFlag);
        }

        this->actionFunc = func_80C259E8;
    }
}

// swordsman school gong
typedef enum {
    /* 0x0 */ DORA_HIT_NONE,
    /* 0x1 */ DORA_HIT_LIGHT,
    /* 0x2 */ DORA_HIT_STRONG
} ObjDoraHitStrength;

typedef enum {
    /* 0x0 */ DORA_DMGEFF_NONE,
    /* 0xE */ DORA_DMGEFF_STRONG = 0xE,
    /* 0xF */ DORA_DMGEFF_LIGHT
} ObjDoraDamageEffect;

void ObjDora_SetupMoveGong(ObjDora* this);
s32 ObjDora_IsHalfHour(u16 time);

RECOMP_PATCH void ObjDora_UpdateCollision(ObjDora* this, PlayState* play) {
    Actor* itemDrop;
    u16 time;

    if (this->colliderTris.base.acFlags & AC_HIT) {
        time = gSaveContext.save.time;
        this->colliderTris.base.acFlags &= ~AC_HIT;
        this->collisionCooldownTimer = 5;

        switch (this->actor.colChkInfo.damageEffect) {
            case DORA_DMGEFF_STRONG:
            case DORA_DMGEFF_LIGHT:
                if (this->actor.colChkInfo.damageEffect == DORA_DMGEFF_LIGHT) {
                    Actor_PlaySfx(&this->actor, NA_SE_EV_DORA_S);
                    this->lastGongHitType = DORA_HIT_LIGHT;
                } else {
                    Actor_PlaySfx(&this->actor, NA_SE_EV_DORA_L);
                    this->lastGongHitType = DORA_HIT_STRONG;
                }

                Actor_RequestQuakeAndRumble(&this->actor, play, 5, 10);
                ObjDora_SetupMoveGong(this);

                if ((ObjDora_IsHalfHour(time) == true) && (this->rupeeDropTimer == 0)) {
                    Actor_PlaySfx(&this->actor, NA_SE_SY_TRE_BOX_APPEAR);
                    if (!rando_location_is_checked(LOCATION_WONDER_HIT_GONG)) {
                        itemDrop = Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, LOCATION_WONDER_HIT_GONG);
                    } else {
                        itemDrop = Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RUPEE_BLUE);
                    }
                    itemDrop->world.rot.y = this->actor.world.rot.y;
                    itemDrop->world.rot.y += (s32)DEG_TO_BINANG_ALT3(Rand_Centered() * 90.0f);
                    itemDrop->velocity.y = 5.0f;
                    itemDrop->gravity = -1.0f;
                    this->rupeeDropTimer = 40;
                }
                break;
        }
    }

    if (this->rupeeDropTimer > 0) {
        this->rupeeDropTimer--;
    }

    if (this->collisionCooldownTimer > 0) {
        this->collisionCooldownTimer--;
    } else {
        CollisionCheck_SetAC(play, &play->colChkCtx, &this->colliderTris.base);
    }
}