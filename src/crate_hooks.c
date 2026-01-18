#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "models/custom_crates.h"

#include "overlays/actors/ovl_Obj_Kibako/z_obj_kibako.h"
#include "overlays/actors/ovl_Obj_Kibako2/z_obj_kibako2.h"

#define LOCATION_CRATE_SMALL (AP_PREFIX_SMALL_CRATES | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))
#define LOCATION_CRATE_BIG (AP_PREFIX_BIG_CRATES | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

Gfx* GenericContainer_SetTextures(PlayState* play, Gfx* gfx, u8* customDraw, u32 location);

// small crate
RECOMP_HOOK("ObjKibako_Init")
void OnObjKibako_Init(Actor* thisx, PlayState* play) {
    u32* crateSmallLocation = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    *crateSmallLocation = LOCATION_CRATE_SMALL;
}

RECOMP_HOOK("ObjKibako_SpawnCollectible")
void OnObjKibako_SpawnCollectible(ObjKibako* this, PlayState* play) {
    u32* crateSmallLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);
    if (rando_get_slotdata_u32("woodsanity") && !rando_location_is_checked(*crateSmallLocation)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *crateSmallLocation);
        this->isDropCollected = 1;
    }
}

extern Gfx* sDisplayLists_ovl_Obj_Kibako[];

RECOMP_PATCH void ObjKibako_Draw(Actor* thisx, PlayState* play) {
    u32* crateSmallLocation = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    u8 customDraw;
    
    OPEN_DISPS(play->state.gfxCtx);

    gSPDisplayList(POLY_OPA_DISP++, gSetupDLs[SETUPDL_25]);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    POLY_OPA_DISP = GenericContainer_SetTextures(play, POLY_OPA_DISP, &customDraw, *crateSmallLocation);
    if (rando_get_slotdata_u32("woodsanity") && customDraw) {
        if (KIBAKO_BANK_INDEX(thisx) == 0) {
            gSPDisplayList(POLY_OPA_DISP++, randoSmallCrateDangeonDL);
        } else {
            gSPDisplayList(POLY_OPA_DISP++, randoSmallCrateDL);
        }
    } else {
        gSPDisplayList(POLY_OPA_DISP++, sDisplayLists_ovl_Obj_Kibako[KIBAKO_BANK_INDEX(thisx)]);
    }

    CLOSE_DISPS(play->state.gfxCtx);
    // Gfx_DrawDListOpa(play, sDisplayLists_ovl_Obj_Kibako[KIBAKO_BANK_INDEX(thisx)]);
}

// big crate
RECOMP_HOOK("ObjKibako2_Init")
void OnObjKibako2_Init(Actor* thisx, PlayState* play) {
    u32* crateBigLocation = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    *crateBigLocation = LOCATION_CRATE_BIG;

    // replace known stray fairy location
    switch (LOCATION_CRATE_BIG) {
        case 0x272110:
            *crateBigLocation = 0x01211E;
            break;
    }
}

RECOMP_PATCH void ObjKibako2_SpawnCollectible(ObjKibako2* this, PlayState* play) {
    s32 dropItem00Id = func_800A8150(KIBAKO2_COLLECTIBLE_ID(&this->dyna.actor));

    u32* crateBigLocation = z64recomp_get_extended_actor_data(&this->dyna.actor, actorLocationExtension);
    if (rando_get_slotdata_u32("woodsanity") && !rando_location_is_checked(*crateBigLocation)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos,
            dropItem00Id | KIBAKO2_COLLECTIBLE_FLAG(&this->dyna.actor) << 8, *crateBigLocation);
        return;
    }

    if (dropItem00Id > ITEM00_NO_DROP) {
        Item_DropCollectible(play, &this->dyna.actor.world.pos,
                             dropItem00Id | KIBAKO2_COLLECTIBLE_FLAG(&this->dyna.actor) << 8);
    }
}

extern Gfx gLargeCrateDL[];

RECOMP_PATCH void ObjKibako2_Draw(Actor* thisx, PlayState* play) {
    u32* crateBigLocation = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    u8 customDraw;
    
    OPEN_DISPS(play->state.gfxCtx);

    gSPDisplayList(POLY_OPA_DISP++, gSetupDLs[SETUPDL_25]);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    POLY_OPA_DISP = GenericContainer_SetTextures(play, POLY_OPA_DISP, &customDraw, *crateBigLocation);
    if (rando_get_slotdata_u32("woodsanity") && customDraw) {
        gSPDisplayList(POLY_OPA_DISP++, randoLargeCrateDL);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, gLargeCrateDL);
    }

    CLOSE_DISPS(play->state.gfxCtx);
    // Gfx_DrawDListOpa(play, gLargeCrateDL);
}