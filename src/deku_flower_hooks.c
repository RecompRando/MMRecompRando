#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "rando_colors.h"

#include "models/custom_deku_flower.h"

#define PINK_DEKU_FLOWER_LIMB_MAX 0x0B
#include "overlays/actors/ovl_Obj_Etcetera/z_obj_etcetera.h"

#define LOCATION_FLOWER (0x3F0000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->dyna.actor))

static u32 flowerLocation;
static bool waitingForOffer;

void EnItem00_RandoTextAndFreeze(EnItem00* this, PlayState* play);

u16 pinkFlowerTex[1024];
u16 goldFlowerTex[1024];
u16 originalFlowerTex[1024];
extern u64 gPinkDekuFlowerPetalAndCenterTex[];
extern u64 gGoldDekuFlowerPetalAndCenterTex[];

RECOMP_HOOK("ObjEtcetera_Init")
void OnObjEtcetera_Init(Actor* thisx, PlayState* play) {
    waitingForOffer = false;
    // TODO: mask center + add alternate grayscale
    // copy pink flower texture
    Lib_MemCpy(originalFlowerTex, SEGMENTED_TO_K0(gPinkDekuFlowerPetalAndCenterTex), sizeof(originalFlowerTex));
    RGBA16toIA16_Texture(originalFlowerTex, pinkFlowerTex, ARRAY_COUNT(originalFlowerTex), GRAYSCALE_MAX);
    // copy gold flower texture (redundant?)
    Lib_MemCpy(originalFlowerTex, SEGMENTED_TO_K0(gGoldDekuFlowerPetalAndCenterTex), sizeof(originalFlowerTex));
    RGBA16toIA16_Texture(originalFlowerTex, goldFlowerTex, ARRAY_COUNT(originalFlowerTex), GRAYSCALE_MAX);
}

RECOMP_HOOK("ObjEtcetera_Idle")
void OnObjEtcetera_Idle(ObjEtcetera* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (!rando_get_slotdata_u32("flowersanity")) {
        return;
    }

    if (waitingForOffer && !(player->stateFlags3 & PLAYER_STATE3_100) && !rando_location_is_checked_async(flowerLocation)) {
        recomp_printf("flower location: 0x%06X\n", flowerLocation);
        Actor* item = Item_RandoDropCollectible(play, &player->actor.world.pos, ITEM00_APITEM, flowerLocation);
        ((EnItem00*)item)->actionFunc = EnItem00_RandoTextAndFreeze;
        flowerLocation = 0;
        waitingForOffer = false;
    }

    if ((player->stateFlags3 & PLAYER_STATE3_200) && (this->dyna.actor.xzDistToPlayer < 20.0f)) {
        if (!rando_location_is_checked_async(LOCATION_FLOWER)) {
            flowerLocation = LOCATION_FLOWER;
            waitingForOffer = true;
        }
    }
}

// draw
s32 ObjEtcetera_OverrideLimbDrawOnlyCenter(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx);
s32 ObjEtcetera_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx);

RECOMP_PATCH void ObjEtcetera_DrawIdle(Actor* thisx, PlayState* play) {
    ObjEtcetera* this = ((ObjEtcetera*)thisx);
    s32 type = DEKU_FLOWER_TYPE(&this->dyna.actor);
    Gfx* dList = this->dList;

    Color_RGB8 color = {255, 255, 255};
    if (rando_get_slotdata_u32("flowersanity") && get_rando_color(&color, LOCATION_FLOWER)) {
        if (type == DEKU_FLOWER_TYPE_PINK || type == DEKU_FLOWER_TYPE_PINK_WITH_INITIAL_BOUNCE) {      
            dList = randoPinkDekuFlowerIdleDL;
        } else {
            dList = randoGoldDekuFlowerIdleDL;
        }
    }

    OPEN_DISPS(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gDPSetEnvColor(POLY_OPA_DISP++, color.r, color.g, color.b, 255);
    gSPDisplayList(POLY_OPA_DISP++, dList);

    CLOSE_DISPS(play->state.gfxCtx);

    // temporarily draw just the center from the skelanime
    Gfx_SetupDL37_Opa(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, ObjEtcetera_OverrideLimbDrawOnlyCenter, NULL, &this->dyna.actor);
}

RECOMP_PATCH void ObjEtcetera_DrawAnimated(Actor* thisx, PlayState* play) {
    ObjEtcetera* this = ((ObjEtcetera*)thisx);

    Color_RGB8 color = {255, 255, 255};
    if (!rando_get_slotdata_u32("flowersanity") || !get_rando_color(&color, LOCATION_FLOWER)) {
        Gfx_SetupDL37_Opa(play->state.gfxCtx);
        SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, NULL, &this->dyna.actor);
        return;
    }

    OPEN_DISPS(play->state.gfxCtx);

    gDPSetEnvColor(POLY_OPA_DISP++, color.r, color.g, color.b, 255);

    CLOSE_DISPS(play->state.gfxCtx);

    Gfx_SetupDL37_Opa(play->state.gfxCtx);
    SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, ObjEtcetera_OverrideLimbDraw, NULL, &this->dyna.actor);
}

s32 ObjEtcetera_OverrideLimbDrawOnlyCenter(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    ObjEtcetera* this = ((ObjEtcetera*)thisx);
    s32 type = DEKU_FLOWER_TYPE(&this->dyna.actor);

    if (limbIndex != PINK_DEKU_FLOWER_LIMB_CENTER) { // equivalent to GOLD_DEKU_FLOWER_LIMB_CENTER
        return true;
    }
    
    return false;
}

s32 ObjEtcetera_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    ObjEtcetera* this = ((ObjEtcetera*)thisx);
    s32 type = DEKU_FLOWER_TYPE(&this->dyna.actor);

    if (type == DEKU_FLOWER_TYPE_PINK || type == DEKU_FLOWER_TYPE_PINK_WITH_INITIAL_BOUNCE) {
        switch (limbIndex) {
            case PINK_DEKU_FLOWER_LIMB_FRONT_RIGHT_PETAL:
                *dList = randoPinkDekuFlowerFrontRightPetalDL;
                break;
            case PINK_DEKU_FLOWER_LIMB_BACK_RIGHT_PETAL:
                *dList = randoPinkDekuFlowerBackRightPetalDL;
                break;
            case PINK_DEKU_FLOWER_LIMB_FRONT_LEFT_PETAL:
                *dList = randoPinkDekuFlowerFrontLeftPetalDL;
                break;
            case PINK_DEKU_FLOWER_LIMB_BACK_LEFT_PETAL:
                *dList = randoPinkDekuFlowerBackLeftPetalDL;
                break;
        }
    } else {
        switch (limbIndex) {
            case GOLD_DEKU_FLOWER_LIMB_BACK_PETAL:
                *dList = randoGoldDekuFlowerBackPetalDL;
                break;
            case GOLD_DEKU_FLOWER_LIMB_FRONT_PETAL:
                *dList = randoGoldDekuFlowerFrontPetalDL;
                break;
            case GOLD_DEKU_FLOWER_LIMB_FRONT_RIGHT_PETAL:
                *dList = randoGoldDekuFlowerFrontRightPetalDL;
                break;
            case GOLD_DEKU_FLOWER_LIMB_BACK_RIGHT_PETAL:
                *dList = randoGoldDekuFlowerBackRightPetalDL;
                break;
            case GOLD_DEKU_FLOWER_LIMB_RIGHT_PETAL:
                *dList = randoGoldDekuFlowerRightPetalDL;
                break;
            case GOLD_DEKU_FLOWER_LIMB_FRONT_LEFT_PETAL:
                *dList = randoGoldDekuFlowerFrontLeftPetalDL;
                break;
            case GOLD_DEKU_FLOWER_LIMB_LEFT_PETAL:
                *dList = randoGoldDekuFlowerLeftPetalDL;
                break;
            case GOLD_DEKU_FLOWER_LIMB_BACK_LEFT_PETAL:
                *dList = randoGoldDekuFlowerBackLeftPetalDL;
                break;
        }
    }
    
    return false;
}