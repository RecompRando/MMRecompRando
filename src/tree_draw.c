#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "rando_colors.h"
#include "models/custom_trees.h"

#include "overlays/actors/ovl_Obj_Yasi/z_obj_yasi.h" // palm trees
#include "overlays/actors/ovl_En_Snowwd/z_en_snowwd.h" // snow covered trees
#include "overlays/actors/ovl_En_Wood02/z_en_wood02.h" // generic trees?
#include "overlays/actors/ovl_Obj_Tree/z_obj_tree.h" // beefier tree

#define LOCATION_PALM_TREE (AP_PREFIX_PALM_TREES | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))
#define LOCATION_SNOW_TREE (AP_PREFIX_SNOW_TREES | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, thisx))
#define LOCATION_WOOD_TREE (AP_PREFIX_WOOD_TREES | (play->sceneId << 8) | EnWood02_GetIDHash(this))
#define LOCATION_THIC_TREE (AP_PREFIX_TREE_TREES | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->dyna.actor))

// palm trees
extern Gfx gPalmTreeDL[];

Vec3f palmTreeItemPos[] = { // i didn't feel like doing this programmatically lmao
    {-50.0f, 0.0f, 0.0f},
    {25.0f, 0.0f, 43.3f},
    {25.0f, 0.0f, -43.3f},
};

RECOMP_PATCH void ObjYasi_Draw(Actor* thisx, PlayState* play) {
    ObjYasi* this = (ObjYasi*)thisx;

    Matrix_Translate(this->dyna.actor.world.pos.x, this->dyna.actor.world.pos.y, this->dyna.actor.world.pos.z,
                     MTXMODE_NEW);

    if (this->dyna.actor.shape.rot.x != 0) {
        Matrix_RotateYS(this->dyna.actor.home.rot.y, MTXMODE_APPLY);
        Matrix_RotateXS(this->dyna.actor.shape.rot.x, MTXMODE_APPLY);
        Matrix_RotateYS(BINANG_SUB(this->dyna.actor.shape.rot.y, this->dyna.actor.home.rot.y), MTXMODE_APPLY);
    } else {
        Matrix_RotateYS(this->dyna.actor.shape.rot.y, MTXMODE_APPLY);
    }

    Matrix_Scale(0.1f, 0.1f, 0.1f, MTXMODE_APPLY);

    bool* dropped = z64recomp_get_extended_actor_data(thisx, palmTreeDropExtension);
    if (!rando_location_is_checked(LOCATION_PALM_TREE) && !(*dropped) && rando_get_camc_enabled()) {
        Gfx_DrawDListOpa(play, randoPalmTreeDL);
        
        u32 getItemId = rando_get_item_id(LOCATION_PALM_TREE);
        u16 objectId = getObjectId(getItemId);

        Matrix_Scale(6.0f, 6.0f, 6.0f, MTXMODE_APPLY);
        Matrix_Translate(0.0f, 475.0f, 0.0f, MTXMODE_APPLY);
        
        Vec3f itemPos;

        // TODO: make all of this better lmao
        // skelanime seems to only spawn one and some items are randomly offcenter and change in size (some rupees)
        for (int i = 0; i < 3; i++) {
            itemPos = palmTreeItemPos[i];
            Matrix_Translate(itemPos.x, itemPos.y, itemPos.z, MTXMODE_APPLY);
            Matrix_RotateYS(DEG_TO_BINANG((play->state.frames) % 360), MTXMODE_APPLY);
            
            if (isAP(getItemId)) {
                GetItem_Draw(play, getGid(getItemId));
            } else if (ObjLoad(play, 0x06, objectId)) {
                GetItem_Draw(play, getGid(getItemId));
                ObjUnload(play, 0x06, objectId);
            }

            Matrix_RotateYS(-DEG_TO_BINANG((play->state.frames) % 360), MTXMODE_APPLY);
            Matrix_Translate(-itemPos.x, -itemPos.y, -itemPos.z, MTXMODE_APPLY);
        }
    } else {
        Gfx_DrawDListOpa(play, gPalmTreeDL);
    }
}

// snow covered trees
extern Gfx gSnowTreeDL[];
extern u64 gSnowTreeSnowLeavesTex[];

RECOMP_PATCH void EnSnowwd_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPSegment(POLY_OPA_DISP++, 0x08, Lib_SegmentedToVirtual(gSnowTreeSnowLeavesTex));
    
    Color_RGB8 color;
    if (rando_get_slotdata_u32("treesanity") && !SNOWWD_DROPPED_COLLECTIBLE(thisx) && get_rando_color(&color, LOCATION_SNOW_TREE)) {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, color.r, color.g, color.b, 255);
        gSPDisplayList(POLY_OPA_DISP++, randoSnowTreeDL);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, gSnowTreeDL);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

// wood trees/bushes
extern Gfx* D_808C4D54[];
extern Gfx* D_808C4D70[];

extern Gfx object_wood02_DL_0078D0[];
extern Gfx object_wood02_DL_007CA0[];
extern Gfx object_wood02_DL_008160[];
extern Gfx object_wood02_DL_000090[];
extern Gfx object_wood02_DL_000340[];
extern Gfx object_wood02_DL_000340[];
extern Gfx object_wood02_DL_000700[];

extern Gfx object_wood02_DL_007968[];
extern Gfx object_wood02_DL_007D38[];
extern Gfx object_wood02_DL_0080D0[];
extern Gfx object_wood02_DL_007AD0[];
extern Gfx object_wood02_DL_007E20[];
extern Gfx object_wood02_DL_009340[];
extern Gfx object_wood02_DL_000160[];
extern Gfx object_wood02_DL_000440[];
extern Gfx object_wood02_DL_000700[];

typedef enum {
    /* 0 */ WOOD_DRAW_TREE_CONICAL,
    /* 1 */ WOOD_DRAW_TREE_OVAL,
    /* 2 */ WOOD_DRAW_TREE_KAKARIKO_ADULT,
    /* 3 */ WOOD_DRAW_BUSH_GREEN,
    /* 4 */ WOOD_DRAW_4, // Used for black bushes and green leaves
    /* 5 */ WOOD_DRAW_LEAF_YELLOW
} WoodDrawType;

Gfx* woodBodyRandoDLs[] = {
    object_wood02_DL_0078D0, object_wood02_DL_007CA0, object_wood02_DL_008160, randoWood02Bush,
    object_wood02_DL_000340, object_wood02_DL_000340, object_wood02_DL_000700,
};

Gfx* woodLeavesRandoDLs[] = {
    // object_wood02_DL_007968,
    randoWood02TreeLeaves,
    object_wood02_DL_007D38,
    object_wood02_DL_0080D0,
    NULL,
    NULL,
    NULL,
    object_wood02_DL_007AD0,
    object_wood02_DL_007E20,
    object_wood02_DL_009340,
    object_wood02_DL_000160,
    object_wood02_DL_000440,
    object_wood02_DL_000700,
};

u16 EnWood02_GetIDHash(EnWood02* this);

RECOMP_PATCH void EnWood02_Draw(Actor* thisx, PlayState* play) {
    EnWood02* this = ((EnWood02*)thisx);
    GraphicsContext* gfxCtx = play->state.gfxCtx;
    u8 red;
    u8 green;
    u8 blue;

    Gfx** woodBodyDLs = D_808C4D54;
    Gfx** woodLeavesDLs = D_808C4D70;

    OPEN_DISPS(gfxCtx);

    if ((thisx->params == WOOD_TREE_OVAL_GREEN_SPAWNER) || (thisx->params == WOOD_TREE_OVAL_GREEN_SPAWNED) ||
        (thisx->params == WOOD_TREE_OVAL_GREEN) || (thisx->params == WOOD_LEAF_GREEN)) {
        red = 50;
        green = 170;
        blue = 70;
    } else if ((thisx->params == WOOD_TREE_OVAL_YELLOW_SPAWNER) || (thisx->params == WOOD_TREE_OVAL_YELLOW_SPAWNED) ||
               (thisx->params == WOOD_LEAF_YELLOW)) {
        red = 180;
        green = 155;
        blue = 0;
    } else {
        red = green = blue = 255;
    }

    Color_RGB8 color;
    bool* dropped = z64recomp_get_extended_actor_data(&this->actor, woodTreeDropExtension);
    if (rando_get_slotdata_u32("treesanity") && !(*dropped) && get_rando_color(&color, LOCATION_WOOD_TREE)) {
        woodBodyDLs = woodBodyRandoDLs;
        woodLeavesDLs = woodLeavesRandoDLs;
        red = color.r;
        green = color.g;
        blue = color.b;
    }

    Gfx_SetupDL25_Xlu(gfxCtx);

    if ((thisx->params == WOOD_LEAF_GREEN) || (thisx->params == WOOD_LEAF_YELLOW)) {
        Gfx_SetupDL25_Opa(gfxCtx);

        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, red, green, blue, 127);

        Gfx_DrawDListOpa(play, object_wood02_DL_000700);
    } else if (woodLeavesDLs[this->drawType & 0xF] != NULL) {
        Gfx_DrawDListOpa(play, woodBodyDLs[this->drawType & 0xF]);

        gDPSetEnvColor(POLY_XLU_DISP++, red, green, blue, 0);
        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, woodLeavesDLs[this->drawType & 0xF]);
    } else {
        Gfx_SetupDL25_Xlu(gfxCtx);

        gDPSetPrimColor(POLY_XLU_DISP++, 0, 0, red, green, blue, 127);
        gDPSetEnvColor(POLY_XLU_DISP++, red, green, blue, 0);

        gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        gSPDisplayList(POLY_XLU_DISP++, woodBodyDLs[this->drawType & 0xF]);
    }

    CLOSE_DISPS(gfxCtx);
}

// big tree
extern Gfx gTreeBodyDL[];
extern Gfx gTreeLeavesDL[];

u16 ObjTreeLeavesTex[1024];
u16 originalTreeTexTemp[1024];
extern u64 gTreeLeavesTex[];

RECOMP_HOOK("ObjTree_Init")
void ObjTree_GrabTextures(Actor* thisx, PlayState* play) {
    Lib_MemCpy(originalTreeTexTemp, SEGMENTED_TO_K0(gTreeLeavesTex), sizeof(originalTreeTexTemp));
    RGBA16toIA16_Texture(originalTreeTexTemp, ObjTreeLeavesTex, ARRAY_COUNT(originalTreeTexTemp), GRAYSCALE_OOTMM);
}

RECOMP_PATCH void ObjTree_Draw(Actor* thisx, PlayState* play) {
    ObjTree* this = ((ObjTree*)thisx);
    
    s16 xRot = (f32)thisx->shape.rot.x;
    s16 zRot = (f32)thisx->shape.rot.z;

    bool* dropped = z64recomp_get_extended_actor_data(&this->dyna.actor, bigTreeDropExtension);

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, gTreeBodyDL);

    Matrix_RotateZYX(xRot, 0, zRot, MTXMODE_APPLY);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    
    Color_RGB8 color;
    if (rando_get_slotdata_u32("treesanity") && get_rando_color(&color, LOCATION_THIC_TREE) && !(*dropped)) {
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, color.r, color.g, color.b, 255);
        gSPDisplayList(POLY_OPA_DISP++, randoTreeLeavesDL);
    } else {
        gSPDisplayList(POLY_OPA_DISP++, gTreeLeavesDL);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}