#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_En_Hit_Tag/z_en_hit_tag.h"
#include "overlays/actors/ovl_En_Invisible_Ruppe/z_en_invisible_ruppe.h"

#define LOCATION_WONDER_HIT (0x150000 | (play->sceneId << 8) \
                            | (randoGetLoadedActorNumInSameRoom(play, thisx) << 4))
#define LOCATION_WONDER_RUPEE (0x160000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

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

// TODO: just give the items instead of spawning the actor
RECOMP_PATCH void func_80C2590C(EnInvisibleRuppe* this, PlayState* play) {
    if (this->collider.base.ocFlags1 & OC1_HIT) {
        extendedWonderRupeeData = z64recomp_get_extended_actor_data(&this->actor, wonderRupeeExtension);
        recomp_printf("wonder invisible rupee: 0x%06X\n", *extendedWonderRupeeData);
        switch (INVISIBLERUPPE_GET_3(&this->actor)) {
            case 0:
                Audio_PlaySfx(NA_SE_SY_GET_RUPY);
                Item_DropCollectible(play, &this->actor.world.pos, 0x8000 | ITEM00_RUPEE_GREEN);
                // Item_RandoDropCollectible(play, &this->actor.world.pos, 0x8000 | ITEM00_APITEM, LOCATION_WONDER_RUPEE);
                break;

            case 1:
                Audio_PlaySfx(NA_SE_SY_GET_RUPY);
                Item_DropCollectible(play, &this->actor.world.pos, 0x8000 | ITEM00_RUPEE_BLUE);
                // Item_RandoDropCollectible(play, &this->actor.world.pos, 0x8000 | ITEM00_APITEM, LOCATION_WONDER_RUPEE);
                break;

            case 2:
                Audio_PlaySfx(NA_SE_SY_GET_RUPY);
                Item_DropCollectible(play, &this->actor.world.pos, 0x8000 | ITEM00_RUPEE_RED);
                // Item_RandoDropCollectible(play, &this->actor.world.pos, 0x8000 | ITEM00_APITEM, LOCATION_WONDER_RUPEE);
                break;

            default:
                break;
        }

        if (this->switchFlag > SWITCH_FLAG_NONE) {
            Flags_SetSwitch(play, this->switchFlag);
        }

        this->actionFunc = func_80C259E8;
    }
}