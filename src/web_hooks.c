#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"
#include "recompconfig.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "rando_colors.h"
#include "models/custom_webs.h"

#include "overlays/actors/ovl_Bg_Spdweb/z_bg_spdweb.h"
#include "overlays/actors/ovl_Obj_Spidertent/z_obj_spidertent.h"

#define LOCATION_WEB (AP_PREFIX_WEBS | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->dyna.actor))
// these ids have to be different because of a single room in OSH where both appear (and they're in different categories)
#define LOCATION_WEB_TENT (AP_PREFIX_WEBS | ((play->sceneId + 1) << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->dyna.actor))

RECOMP_HOOK("BgSpdweb_Init")
void OnBgSpdweb_Init(Actor* thisx, PlayState* play) {
    BgSpdweb* this = ((BgSpdweb*)thisx);
    u32* location = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    *location = LOCATION_WEB;
}

RECOMP_HOOK("func_809CE234")
void BgSpdweb_Drop1(BgSpdweb* this, PlayState* play) {
    u32* location = z64recomp_get_extended_actor_data(&this->dyna.actor, actorLocationExtension);
    if (this->unk_162 == 1 && rando_get_slotdata_u32("websanity") && !rando_location_is_checked(*location)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, *location);
        Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
    }
}

RECOMP_HOOK("func_809CE830")
void BgSpdweb_Drop2(BgSpdweb* this, PlayState* play) {
    u32* location = z64recomp_get_extended_actor_data(&this->dyna.actor, actorLocationExtension);
    if (this->unk_162 == 1 && rando_get_slotdata_u32("websanity") && !rando_location_is_checked(*location)) {
        Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, *location);
        Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
    }
}

RECOMP_PATCH void BgSpdweb_Draw(Actor* thisx, PlayState* play) {
    u32* location = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);

    OPEN_DISPS(play->state.gfxCtx);

    gSPDisplayList(POLY_XLU_DISP++, gSetupDLs[SETUPDL_25]);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    Color_RGB8 color = {255, 255, 255};
    get_rando_color(&color, *location);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0xFF, color.r, color.g, color.b, 255);

    if (thisx->params == BGSPDWEB_FF_1) {
        gSPDisplayList(POLY_XLU_DISP++, randoSpiderWeb1);
    } else {
        Matrix_Translate(0.0f, (thisx->home.pos.y - thisx->world.pos.y) * 10.0f, 0.0f, MTXMODE_APPLY);
        Matrix_Scale(1.0f, ((thisx->home.pos.y - thisx->world.pos.y) + 10.0f) * 0.1f, 1.0f, MTXMODE_APPLY);

        gSPDisplayList(POLY_XLU_DISP++, randoSpiderWeb2);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

// tent
RECOMP_HOOK("ObjSpidertent_Init")
void OnObjSpidertent_Init(Actor* thisx, PlayState* play) {
    ObjSpidertent* this = ((ObjSpidertent*)thisx);
    u32* location = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    *location = LOCATION_WEB_TENT;
    bool* dropped = z64recomp_get_extended_actor_data(thisx, actorDroppedExtension);
    *dropped = false;
    // Flags_UnsetSwitch(play, OBJSPIDERTENT_GET_SWITCH_FLAG(&this->dyna.actor));
}

// this function does spawn it a bit early (setup burn func)
RECOMP_HOOK("func_80B30AD4")
void ObjSpidertent_DropOnSetupBurn(ObjSpidertent* this) {
    PlayState* play = gPlay;
    u32* location = z64recomp_get_extended_actor_data(&this->dyna.actor, actorLocationExtension);

    if (!rando_get_slotdata_u32("websanity") || rando_location_is_checked(*location)) return;

    Actor* item = Item_RandoDropCollectible(play, &this->dyna.actor.world.pos, ITEM00_APITEM, *location);
    item->velocity.y = 0.0f;

    // Set default angle from config
    // item->world.rot.y = DEG_TO_BINANG(recomp_get_config_double("item_angle"));

    // Override angle/position for specific problematic locations
    switch (*location) {
        // Ocean Spider House - Main Room (Room 1)
        case 0x2E2910: // Web Above Door 1st Floor Door
            item->world.rot.y = DEG_TO_BINANG(-170.0f);
            break;
            
        case 0x2E2911: // Web 1st Floor Near Staircase
            item->world.rot.y = DEG_TO_BINANG(Rand_ZeroFloat(60.0f) + 60.0f); // 60-120 degree range (safe zone)
            item->velocity.y = 6.0f; // Pop up
            break;
            
        case 0x2E2912: // Web Basement Covering Door
            item->world.pos.y += 30.0f; // Start higher up out of door frame
            item->world.rot.y = DEG_TO_BINANG(Rand_ZeroFloat(60.0f) + 60.0f); // 60-120 degree range (safe zone)
            item->velocity.y = 6.0f; // Pop up
            break;
            
        case 0x2E2913: // Library Web (tested - use safe angle)
            item->world.pos.y += 30.0f; // Start higher up out of door frame
            item->world.rot.y = DEG_TO_BINANG(Rand_ZeroFloat(60.0f) + 60.0f); // 60-120 degree range (safe zone)
            item->velocity.y = 6.0f; // Pop up
            break;

        case 0x2E2914: // Web Basement Covering Hole
            item->world.pos.y += 40.0f; // Start higher up out of hole
            item->world.rot.y = DEG_TO_BINANG(Rand_ZeroFloat(60.0f) + 60.0f); // 60-120 degree range (safe zone)
            item->velocity.y = 6.0f; // Pop up
            break;
            
        case 0x2E2915: // Web Over 1st Floor Pot
            item->world.rot.y = DEG_TO_BINANG(-170.0f);
            break;
            
        case 0x2E2916: // Web Basement Covering Crates
            item->world.rot.y = DEG_TO_BINANG(90.0f);
            break;
            
        case 0x2E2917: // Web Basement Near Staircase
            item->world.rot.y = DEG_TO_BINANG(-170.0f);
            break;

        // Ocean Spider House - Boat Room
        case 0x2E2950: // Boat Room Covering Crate
            item->world.rot.y = DEG_TO_BINANG(90.0f);
            break;
            
        case 0x2E2951: // Boat Room Ceiling Web
            item->world.pos.y -= 40.0f; // Start lower out of ceiling
            break;

        // Ocean Spider House - Coloured Mask Room (Room 3)
        case 0x2E2830: // Coloured Mask Ceiling Web (1)
            item->world.rot.y = DEG_TO_BINANG(90.0f);
            break;
            
        case 0x2E2930: // Coloured Mask Ceiling Web (2)
            item->world.rot.y = DEG_TO_BINANG(90.0f);
            break;

        default:
            // Keep the full random range of angles (or debug config angle) and original position
            break;
    }

    Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
    
    bool* dropped = z64recomp_get_extended_actor_data(&this->dyna.actor, actorDroppedExtension);
    *dropped = true;
}
typedef struct {
    /* 0x00 */ Gfx* unk_00;
    /* 0x04 */ CollisionHeader* unk_04;
    /* 0x08 */ ColliderTrisInit* unk_08;
    /* 0x0C */ f32 unk_0C;
    /* 0x10 */ f32 unk_10;
    /* 0x14 */ f32 unk_14;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ f32 unk_1C;
    /* 0x20 */ f32 unk_20;
    /* 0x24 */ f32 unk_24;
} ObjSpidertentStruct; // size = 0x28

extern ObjSpidertentStruct D_80B31350[];

RECOMP_PATCH void ObjSpidertent_Draw(Actor* thisx, PlayState* play) {
    ObjSpidertent* this = ((ObjSpidertent*)thisx);
    s32 params = OBJSPIDERTENT_GET_1(&this->dyna.actor);
    s32 temp_f18 = this->unk_3C5 * (29.0f / 51);
    Gfx* gfx;

    u32* location = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);

    OPEN_DISPS(play->state.gfxCtx);

    gfx = POLY_XLU_DISP;

    gSPDisplayList(gfx++, gSetupDLs[SETUPDL_25]);
    gSPMatrix(gfx++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    Color_RGB8 color = {this->unk_3C2, this->unk_3C3, this->unk_3C4};
    get_rando_color(&color, *location);
    
    // gDPSetPrimColor(gfx++, 0, 0xFF, this->unk_3C2, this->unk_3C3, this->unk_3C4, temp_f18);
    gDPSetPrimColor(gfx++, 0, 0xFF, color.r, color.g, color.b, temp_f18);
    gSPDisplayList(gfx++, D_80B31350[params].unk_00);

    POLY_XLU_DISP = gfx;
    CLOSE_DISPS(play->state.gfxCtx);
}