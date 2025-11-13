#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "models/custom_barrels.h"

#include "overlays/actors/ovl_Obj_Taru/z_obj_taru.h"

#define LOCATION_BARREL (AP_PREFIX_BARRELS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))

ActorExtensionId barrelLocationExtension;
u32* barrelLocation;

RECOMP_HOOK("ObjTaru_Init")
void OnObjTaru_Init(Actor* thisx, PlayState* play) {
    barrelLocation = z64recomp_get_extended_actor_data(thisx, barrelLocationExtension);
    *barrelLocation = LOCATION_BARREL;

    // replace known stray fairy locations
    switch (LOCATION_BARREL) {
        case 0x224900:
            *barrelLocation = 0x01491C;
            break;
        case 0x2249A0:
            *barrelLocation = 0x01491A;
            break;
    }
}

RECOMP_PATCH void func_80B9BC64(ObjTaru* this, PlayState* play) {
    s32 item = func_800A8150(OBJ_TARU_GET_3F(&this->dyna.actor));

    barrelLocation = z64recomp_get_extended_actor_data(&this->dyna.actor, barrelLocationExtension);
    if (rando_get_slotdata_u32("woodsanity") && !rando_location_is_checked(*barrelLocation)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, (OBJ_TARU_GET_7F00(&this->dyna.actor) << 8) | item, *barrelLocation);
        return;
    }
    
    if (item >= 0) {
        Item_DropCollectible(play, &this->dyna.actor.world.pos, (OBJ_TARU_GET_7F00(&this->dyna.actor) << 8) | item);
    }
}

// pirate's fortress barricades?
RECOMP_HOOK("func_80B9B9C8")
void ObjTaru_DropBarricadeItem(ObjTaru* this, PlayState* play) {
    barrelLocation = z64recomp_get_extended_actor_data(&this->dyna.actor, barrelLocationExtension);
    if (rando_get_slotdata_u32("woodsanity") && !rando_location_is_checked(*barrelLocation)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, *barrelLocation);
    }
}

extern Gfx gObjTaruBreakablePiratePanelDL[];
extern Gfx gObjTaruBarrelDL[];

Gfx* GenericContainer_SetTextures(PlayState* play, Gfx* gfx, u8* customDraw, u32 location);

RECOMP_PATCH void ObjTaru_Draw(Actor* thisx, PlayState* play) {
    barrelLocation = z64recomp_get_extended_actor_data(thisx, barrelLocationExtension);
    u8 customDraw;
    
    OPEN_DISPS(play->state.gfxCtx);

    gSPDisplayList(POLY_OPA_DISP++, gSetupDLs[SETUPDL_25]);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    POLY_OPA_DISP = GenericContainer_SetTextures(play, POLY_OPA_DISP, &customDraw, *barrelLocation);
    if (rando_get_slotdata_u32("woodsanity") && customDraw) {
        if (OBJ_TARU_GET_80(thisx)) {
            gSPDisplayList(POLY_OPA_DISP++, randoBreakablePiratePanelDL);
        } else {
            gSPDisplayList(POLY_OPA_DISP++, randoBarrelDL);
        }
    } else {
        if (OBJ_TARU_GET_80(thisx)) {
            gSPDisplayList(POLY_OPA_DISP++, gObjTaruBreakablePiratePanelDL);
        } else {
            gSPDisplayList(POLY_OPA_DISP++, gObjTaruBarrelDL);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}