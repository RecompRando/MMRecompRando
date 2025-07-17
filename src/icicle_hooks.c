#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Bg_Icicle/z_bg_icicle.h"

#define LOCATION_ICICLE (0x230000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

ActorExtensionId icicleExtension;
u32* extendedIcicleData;

RECOMP_HOOK("BgIcicle_Init")
void OnBgIcicle_Init(Actor* thisx, PlayState* play) {
    extendedIcicleData = z64recomp_get_extended_actor_data(thisx, icicleExtension);
    *extendedIcicleData = LOCATION_ICICLE;
}

void BgIcicle_Regrow(BgIcicle* this, PlayState* play);
void BgIcicle_Break(BgIcicle* this, PlayState* play, f32 arg2);

RECOMP_PATCH void BgIcicle_UpdateAttacked(BgIcicle* this, PlayState* play) {
    s32 dropItem00Id;
    extendedIcicleData = z64recomp_get_extended_actor_data(&this->dyna.actor, icicleExtension);

    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;

        if (this->dyna.actor.params == ICICLE_STALAGMITE_RANDOM_DROP) {
            BgIcicle_Break(this, play, 50.0f);
            if (!rando_location_is_checked(*extendedIcicleData)) {
                Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, *extendedIcicleData);
            } else if (this->unk_160 != 0xFF) {
                Item_DropCollectibleRandom(play, NULL, &this->dyna.actor.world.pos, this->unk_160 << 4);
            }
        } else if (this->dyna.actor.params == ICICLE_STALAGMITE_FIXED_DROP) {
            dropItem00Id = func_800A8150(this->unk_160);
            BgIcicle_Break(this, play, 50.0f);
            if (!rando_location_is_checked(*extendedIcicleData)) {
                Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, (this->unk_161 << 8) | dropItem00Id, *extendedIcicleData);
            } else {
                Item_DropCollectible(play, &this->dyna.actor.world.pos, (this->unk_161 << 8) | dropItem00Id);
            }
        } else {
            if (this->dyna.actor.params == ICICLE_STALACTITE_REGROW) {
                BgIcicle_Break(this, play, 40.0f);
                this->dyna.actor.world.pos.y = this->dyna.actor.home.pos.y + 120.0f;
                DynaPoly_EnableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
                this->actionFunc = BgIcicle_Regrow;
                return;
            }

            BgIcicle_Break(this, play, 40.0f);
        }

        Actor_Kill(&this->dyna.actor);
    }
}