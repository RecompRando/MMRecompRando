#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Snowball/z_obj_snowball.h"
#include "overlays/actors/ovl_Obj_Snowball2/z_obj_snowball2.h"

#define LOCATION_SNOWBALL (0x210000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_SNOWBALL))
#define LOCATION_SNOWBALL_BIG (0x210000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_SNOWBALL2))

ActorExtensionId snowballLocationExtension;
u32* snowballLocation;
ActorExtensionId bigSnowballLocationExtension;
u32* bigSnowballLocation;

void grab_snowball_texture();

// small snowball
RECOMP_HOOK("ObjSnowball2_Init")
void OnObjSnowball2_Init(Actor* thisx, PlayState* play) {
    snowballLocation = z64recomp_get_extended_actor_data(thisx, snowballLocationExtension);
    *snowballLocation = LOCATION_SNOWBALL;
    grab_snowball_texture();
}

RECOMP_PATCH void func_80B38E88(ObjSnowball2* this, PlayState* play) {
    s32 temp_v0;

    if (this->unk_1AE == 0) {
        temp_v0 = func_800A8150(ENOBJSNOWBALL2_GET_3F(&this->actor));
        
        snowballLocation = z64recomp_get_extended_actor_data(&this->actor, snowballLocationExtension);
        if (!rando_location_is_checked(*snowballLocation)) {
            Item_RandoDropCollectible(play, &this->actor.world.pos, (ENOBJSNOWBALL2_GET_7F00(&this->actor) << 8) | temp_v0, *snowballLocation);
            this->unk_1AE = 1;
            return;
        }

        if (temp_v0 >= 0) {
            Item_DropCollectible(play, &this->actor.world.pos, (ENOBJSNOWBALL2_GET_7F00(&this->actor) << 8) | temp_v0);
            this->unk_1AE = 1;
        }
    }
}

// large snowball
RECOMP_HOOK("ObjSnowball_Init")
void OnObjSnowball_Init(Actor* thisx, PlayState* play) {
    bigSnowballLocation = z64recomp_get_extended_actor_data(thisx, bigSnowballLocationExtension);
    *bigSnowballLocation = LOCATION_SNOWBALL_BIG;
    grab_snowball_texture();
}

s32 func_800A8150(s32 index);

RECOMP_HOOK("func_80B03FF8")
void on_func_80B03FF8(ObjSnowball* this, PlayState* play) {
    // s16 rotY = this->actor.home.rot.y; // snowball type (only 0 normally drops items)
    s32 temp_v0 = func_800A8150(OBJSNOWBALL_GET_SWITCH_FLAG(&this->actor));
    bigSnowballLocation = z64recomp_get_extended_actor_data(&this->actor, bigSnowballLocationExtension);
    if (!rando_location_is_checked(*bigSnowballLocation)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, (OBJSNOWBALL_GET_7F00(&this->actor) << 8) | temp_v0, *bigSnowballLocation);
        return;
    }
}

RECOMP_PATCH void func_80B02D58(ObjSnowball* this, PlayState* play) {
    s32 temp_v0 = func_800A8150(OBJSNOWBALL_GET_SWITCH_FLAG(&this->actor));

    bigSnowballLocation = z64recomp_get_extended_actor_data(&this->actor, bigSnowballLocationExtension);
    if (rando_location_is_checked(*bigSnowballLocation)) {
        if (temp_v0 >= 0) {
            Item_DropCollectible(play, &this->actor.home.pos, (OBJSNOWBALL_GET_7F00(&this->actor) << 8) | temp_v0);
        }
    }
}

void func_80B03FF8(ObjSnowball* this, PlayState* play);
void func_80B04608(ObjSnowball* this, PlayState* play);
void func_80B046E4(ObjSnowball* this, PlayState* play);

// cutscene skip
RECOMP_PATCH void func_80B0457C(ObjSnowball* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->actor.csId)) {
        // CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
        func_80B03FF8(this, play);
        this->unk_20B = 1;
        if (this->unk_20A == 0) {
            func_80B04608(this, play);
        } else {
            func_80B046E4(this, play);
        }
    } else {
        CutsceneManager_Queue(this->actor.csId);
    }
}

// custom draw
#include "snowball.h"
#include "rando_colors.h"

u16 snowballTex[2048];
u16 originalSnowballTex[2048];
extern u16 object_goroiwa_Tex_008DA0[];
extern Gfx object_goroiwa_DL_008B90[];

void grab_snowball_texture() {
    Lib_MemCpy(originalSnowballTex, SEGMENTED_TO_K0(object_goroiwa_Tex_008DA0), sizeof(originalSnowballTex));
    RGBA16toIA16_Texture(originalSnowballTex, snowballTex, ARRAY_COUNT(originalSnowballTex)); // why was it rgba16 lmao
}

Gfx* GenericSnowball_DrawRandoColored(PlayState* play, u32 location, Gfx* gfx) {
    Color_RGB8 color;
    
    if (!get_rando_color(&color, location)) {
        gDPSetPrimColor(gfx++, 0, 0, 255, 255, 255, 255);
        gSPDisplayList(gfx++, object_goroiwa_DL_008B90);
        return gfx;
    }
    
    gDPSetPrimColor(gfx++, 0, 0, color.r, color.g, color.b, 255);
    gSPDisplayList(gfx++, randoSnowballDL);
    return gfx;
}

RECOMP_PATCH void ObjSnowball_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    bigSnowballLocation = z64recomp_get_extended_actor_data(thisx, bigSnowballLocationExtension);
    POLY_OPA_DISP = GenericSnowball_DrawRandoColored(play, *bigSnowballLocation, POLY_OPA_DISP);

    CLOSE_DISPS(play->state.gfxCtx);
}

RECOMP_PATCH void ObjSnowball2_Draw(Actor* thisx, PlayState* play) {
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);

    snowballLocation = z64recomp_get_extended_actor_data(thisx, snowballLocationExtension);
    POLY_OPA_DISP = GenericSnowball_DrawRandoColored(play, *snowballLocation, POLY_OPA_DISP);

    CLOSE_DISPS(play->state.gfxCtx);
}