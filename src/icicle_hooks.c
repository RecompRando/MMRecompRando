#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "rando_colors.h"

#include "overlays/actors/ovl_Bg_Icicle/z_bg_icicle.h"

#define LOCATION_ICICLE (AP_PREFIX_ICICLES | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

ActorExtensionId icicleLocationExtension;
u32* icicleLocation;
ActorExtensionId icicleDroppedExtension;
bool* icicleDropped;

RECOMP_HOOK("BgIcicle_Init")
void OnBgIcicle_Init(Actor* thisx, PlayState* play) {
    icicleLocation = z64recomp_get_extended_actor_data(thisx, icicleLocationExtension);
    *icicleLocation = LOCATION_ICICLE;
    icicleDropped = z64recomp_get_extended_actor_data(thisx, icicleDroppedExtension);
    *icicleDropped = false;
}

void BgIcicle_Regrow(BgIcicle* this, PlayState* play);
void BgIcicle_Break(BgIcicle* this, PlayState* play, f32 arg2);

RECOMP_PATCH void BgIcicle_UpdateAttacked(BgIcicle* this, PlayState* play) {
    s32 dropItem00Id;
    icicleLocation = z64recomp_get_extended_actor_data(&this->dyna.actor, icicleLocationExtension);
    icicleDropped = z64recomp_get_extended_actor_data(&this->dyna.actor, icicleDroppedExtension);

    if (this->collider.base.acFlags & AC_HIT) {
        this->collider.base.acFlags &= ~AC_HIT;

        if (this->dyna.actor.params == ICICLE_STALAGMITE_RANDOM_DROP) {
            BgIcicle_Break(this, play, 50.0f);
            if (rando_get_slotdata_u32("iciclesanity") && !rando_location_is_checked(*icicleLocation)) {
                Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, *icicleLocation);
                *icicleDropped = true;
            } else if (this->unk_160 != 0xFF) {
                Item_DropCollectibleRandom(play, NULL, &this->dyna.actor.world.pos, this->unk_160 << 4);
            }
        } else if (this->dyna.actor.params == ICICLE_STALAGMITE_FIXED_DROP) {
            dropItem00Id = func_800A8150(this->unk_160);
            BgIcicle_Break(this, play, 50.0f);
            if (rando_get_slotdata_u32("iciclesanity") && !rando_location_is_checked(*icicleLocation)) {
                Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, (this->unk_161 << 8) | dropItem00Id, *icicleLocation);
                *icicleDropped = true;
            } else {
                Item_DropCollectible(play, &this->dyna.actor.world.pos, (this->unk_161 << 8) | dropItem00Id);
            }
        } else {
            if (rando_get_slotdata_u32("iciclesanity") && !rando_location_is_checked(*icicleLocation)) {
                Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, (this->unk_161 << 8), *icicleLocation);
                *icicleDropped = true;
            }

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

extern u64 gIcicleTex[];
extern Vtx object_icicleVtx_000000[];

Gfx randoIcicleDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(gIcicleTex, G_IM_FMT_I, G_IM_SIZ_8b, 64, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                         G_TX_WRAP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK | G_FOG),
    // gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(object_icicleVtx_000000, 13, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP1Triangle(10, 12, 11, 0),
    gsSPEndDisplayList(),
};

RECOMP_PATCH void BgIcicle_Draw(Actor* thisx, PlayState* play) {
    Color_RGB8 color = {255, 255, 255};
    BgIcicle* this = ((BgIcicle*)thisx);
    icicleLocation = z64recomp_get_extended_actor_data(&this->dyna.actor, icicleLocationExtension);
    icicleDropped = z64recomp_get_extended_actor_data(thisx, icicleDroppedExtension);
    
    if (rando_get_slotdata_u32("iciclesanity") && !rando_location_is_checked(*icicleLocation) && !*icicleDropped) {
        get_rando_color(&color, *icicleLocation);
    }

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color.r, color.g, color.b, 255);
    gSPDisplayList(POLY_OPA_DISP++, randoIcicleDL);

    CLOSE_DISPS(play->state.gfxCtx);

    // Gfx_DrawDListOpa(play, gIcicleDL);
}