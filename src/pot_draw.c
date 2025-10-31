#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "rando_colors.h"
#include "models/custom_pots.h"

#include "overlays/actors/ovl_Obj_Tsubo/z_obj_tsubo.h"
#include "overlays/actors/ovl_En_Tubo_Trap/z_en_tubo_trap.h"
#include "overlays/actors/ovl_Obj_Flowerpot/z_obj_flowerpot.h"

extern ActorExtensionId potLocationExtension;
extern u32* potLocation;
extern ActorExtensionId potDroppedExtension;
extern bool* potDropped;
extern ActorExtensionId potFlyingLocationExtension;
extern u32* potFlyingLocation;
extern ActorExtensionId potFlowerLocationExtension;
extern u32* potFlowerLocation;

typedef struct {
    /* 0x00 */ s16 objectId;
    /* 0x04 */ f32 scale;
    /* 0x08 */ Gfx* modelDL;
    /* 0x0C */ Gfx* shardDL;
    /* 0x10 */ s16 radius;
    /* 0x12 */ s16 height;
    /* 0x14 */ ObjTsuboUnkFunc breakPot1;
    /* 0x18 */ ObjTsuboUnkFunc breakPot2;
    /* 0x1C */ ObjTsuboUnkFunc breakPot3;
} ObjTsuboData; // size = 0x20

extern ObjTsuboData sPotTypeData[4];

void GenericPot_DrawRando(PlayState* play, u32 location, u8 potType) {
    TexturePtr pot_side_tex;
    TexturePtr pot_top_tex;

    s16 getItem = rando_get_item_id(location);
    bool drawOriginal = false;

    pot_side_tex = pot_prog_side;
    pot_top_tex = pot_prog_top;

    switch (getItem) {
        case GI_AP_PROG:
            pot_side_tex = pot_prog_side;
            pot_top_tex = pot_prog_top;
            break;
        case GI_AP_USEFUL:
            pot_side_tex = pot_useful_side;
            pot_top_tex = pot_useful_top;
            break;
        case GI_AP_FILLER:
            pot_side_tex = pot_filler_side;
            pot_top_tex = pot_filler_top;
            break;
        case GI_KEY_BOSS:
        case GI_KEY_BOSS_WOODFALL:
        case GI_KEY_BOSS_SNOWHEAD:
        case GI_KEY_BOSS_GREATBAY:
        case GI_KEY_BOSS_STONETOWER:
            pot_side_tex = pot_bosskey_side;
            pot_top_tex = pot_bosskey_top;
            break;
        case GI_KEY_SMALL:
        case GI_KEY_SMALL_WOODFALL:
        case GI_KEY_SMALL_SNOWHEAD:
        case GI_KEY_SMALL_GREATBAY:
        case GI_KEY_SMALL_STONETOWER:
            pot_side_tex = pot_key_side;
            pot_top_tex = pot_spider_top; // there isn't a key top for some reason
            break;
        case GI_B2:
        case GI_46:
        case GI_47:
        case GI_48:
        case GI_49:
            pot_side_tex = pot_fairy_side;
            pot_top_tex = pot_fairy_top;
            break;
        case GI_HEART_PIECE:
        case GI_HEART_CONTAINER:
        case GI_DEFENSE_DOUBLE:
            pot_side_tex = pot_heart_side;
            pot_top_tex = pot_heart_top;
            break;
        case GI_TRUE_SKULL_TOKEN:
        case GI_OCEAN_SKULL_TOKEN:
            pot_side_tex = pot_spider_side;
            pot_top_tex = pot_spider_top;
            break;
        default:
            if (!rando_get_location_has_local_item(location)) {
                switch (rando_get_location_type(location)) {
                    case RANDO_ITEM_CLASS_PROGRESSION:
                    case RANDO_ITEM_CLASS_TRAP:
                        pot_side_tex = pot_prog_side;
                        pot_top_tex = pot_prog_top;
                        break;
                    case RANDO_ITEM_CLASS_USEFUL:
                        pot_side_tex = pot_useful_side;
                        pot_top_tex = pot_useful_top;
                        break;
                    case RANDO_ITEM_CLASS_JUNK:
                    default:
                        pot_side_tex = pot_filler_side;
                        pot_top_tex = pot_filler_top;
                        break;
                }
            } else {
                switch (rando_get_location_type(location)) {
                    case RANDO_ITEM_CLASS_PROGRESSION:
                    case RANDO_ITEM_CLASS_TRAP:
                        pot_side_tex = pot_major_side;
                        pot_top_tex = pot_major_top;
                        break;
                    case RANDO_ITEM_CLASS_USEFUL:
                    case RANDO_ITEM_CLASS_JUNK:
                    default:
                        drawOriginal = true;
                        break;
                }
            }
    }

    if (getItem >= GI_BOSS_SOUL_ODOLWA && getItem <= GI_ABSURD_GENERIC) {
        pot_side_tex = pot_soul_side;
        pot_top_tex = pot_soul_top;
        drawOriginal = false;
    }

    if (drawOriginal) {
        Gfx_DrawDListOpa(play, sPotTypeData[potType].modelDL);
        return;
    }

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, 0x08, pot_side_tex);
    gSPSegment(POLY_OPA_DISP++, 0x09, pot_top_tex);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    
    switch (potType) {
        case 1:
            gSPDisplayList(POLY_OPA_DISP++, randoMagicPotDL);
            break;
        case 2:
            gSPDisplayList(POLY_OPA_DISP++, randoPotDL);
            break;
        case 0:
        case 3:
        default:
            gSPDisplayList(POLY_OPA_DISP++, randoDangeonPotDL);
            break;
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

// normal pot
RECOMP_PATCH void ObjTsubo_Draw(Actor* thisx, PlayState* play2) {
    PlayState* play = (PlayState*)play2;
    ObjTsubo* this = ((ObjTsubo*)thisx);

    potLocation = z64recomp_get_extended_actor_data(&this->actor, potLocationExtension);
    potDropped = z64recomp_get_extended_actor_data(&this->actor, potDroppedExtension);

    // if (!rando_get_slotdata_u32("potsanity") || (rando_location_is_checked(*potLocation) && !(*potDropped)) ||
    //     (OBJ_TSUBO_PFE00(thisx) && ((func_800A8150(OBJ_TSUBO_P003F(thisx)) != ITEM00_FLEXIBLE)) && !rando_get_slotdata_u32("fairysanity"))) { // stray fairies
    if (!rando_get_slotdata_u32("potsanity") || (rando_location_is_checked(*potLocation) && !(*potDropped))) {
        Gfx_DrawDListOpa(play, sPotTypeData[OBJ_TSUBO_GET_TYPE(thisx)].modelDL);
        return;
    }

    GenericPot_DrawRando(play, *potLocation, OBJ_TSUBO_GET_TYPE(thisx));
}

// flying pot
RECOMP_PATCH void EnTuboTrap_Draw(Actor* thisx, PlayState* play) {
    potFlyingLocation = z64recomp_get_extended_actor_data(thisx, potFlyingLocationExtension);
    
    if (!rando_get_slotdata_u32("potsanity") || rando_location_is_checked(*potFlyingLocation)) {
        Gfx_DrawDListOpa(play, gameplay_dangeon_keep_DL_017EA0);
        return;
    }

    GenericPot_DrawRando(play, *potFlyingLocation, 0);
}

// flower pot
u16 flowerPotLeavesTex[1024];
u16 originalFlowerPotLeavesTex[1024];
extern u64 object_flowerpot_Tex_000800[];

RECOMP_HOOK("ObjFlowerpot_Init")
void grab_pot_assets(Actor* thisx, PlayState* play) {
    Lib_MemCpy(originalFlowerPotLeavesTex, SEGMENTED_TO_K0(object_flowerpot_Tex_000800), sizeof(originalFlowerPotLeavesTex));
    RGBA16toIA16_Texture(originalFlowerPotLeavesTex, flowerPotLeavesTex, ARRAY_COUNT(originalFlowerPotLeavesTex), GRAYSCALE_OOTMM);
}

extern Gfx object_flowerpot_DL_0012E0[];
extern Gfx object_flowerpot_DL_001408[];

void func_80A1C838(ObjFlowerpot* this, PlayState* play);
void func_80A1B840(MtxF* matrix);
extern MtxF D_80A1D838[8];

RECOMP_PATCH void ObjFlowerpot_Draw(Actor* thisx, PlayState* play) {
    s32 pad;
    ObjFlowerpot* this = ((ObjFlowerpot*)thisx);

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, object_flowerpot_DL_0012E0);

    if ((this->actionFunc != func_80A1C838) || (this->unk_1EA & 1)) {
        Collider_UpdateSpheres(0, &this->collider);
        if (!(this->unk_1EA & 2)) {
            Collider_UpdateSpheres(1, &this->collider);
        }
    }

    if (!(this->unk_1EA & 2)) {
        if ((play->roomCtx.curRoom.behaviorType1 == ROOM_BEHAVIOR_TYPE1_0) && (this->actionFunc == func_80A1C838)) {
            if ((this->actor.projectedPos.z > -150.0f) && (this->actor.projectedPos.z < 400.0f)) {
                func_80A1B840(&D_80A1D838[this->unk_1EB]);

                gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx),
                          G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
            }
        }

        Color_RGB8 color;
        potFlowerLocation = z64recomp_get_extended_actor_data(&this->actor, potFlowerLocationExtension);
        if (rando_get_slotdata_u32("potsanity") && !rando_location_is_checked(*potFlowerLocation) && get_rando_color(&color, *potFlowerLocation)) {
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color.r, color.g, color.b, 255);
            gSPDisplayList(POLY_OPA_DISP++, randoFlowerPotLeavesDL);
        } else {
            gSPDisplayList(POLY_OPA_DISP++, object_flowerpot_DL_001408);
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}