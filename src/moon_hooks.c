#include "modding.h"
#include "global.h"
#include "recompconfig.h"
#include "recomputils.h"

#include "apcommon.h"

#include "overlays/actors/ovl_En_Fall/z_en_fall.h"
#include "overlays/actors/ovl_En_Clear_Tag/z_en_clear_tag.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)

#define THIS ((EnFall*)thisx)

#define LOCATION_OATH 0x040065
#define LOCATION_MOON_ALL_MASK_TRADE 0x00007B

s16 fallTrueGI;
extern s16 moonLiveGI;

static bool fallObjectStatic;
static bool fallObjectLoading;
static bool fallObjectLoaded;
static OSMesgQueue fallObjectLoadQueue;
static void* fallObjectSegment;

RECOMP_HOOK("EnFall_Init")
void EnFall_RandoInit(Actor* thisx, PlayState* play) {
    EnFall* this = THIS;

    fallTrueGI = moonLiveGI;
    fallObjectSegment = ZeldaArena_Malloc(0x2000);
    fallObjectStatic = false;
    fallObjectLoading = false;
    fallObjectLoaded = false;
}

void EnFall_WaitForObject(EnFall* this, PlayState* play) {
    s16 getItemId = moonLiveGI;
    s16 objectSlot = Object_GetSlot(&play->objectCtx, getObjectId(getItemId));

    if (isAP(getItemId)) {
        fallObjectStatic = true;
        fallObjectLoaded = true;
        fallTrueGI = getItemId;
    } else if (!fallObjectLoaded && !fallObjectLoading && Object_IsLoaded(&play->objectCtx, objectSlot)) {
        this->actor.objectSlot = objectSlot;
        Actor_SetObjectDependency(play, &this->actor);
        fallObjectStatic = true;
        fallObjectLoaded = true;
        fallTrueGI = getItemId;
    } else if (!fallObjectLoading && !fallObjectLoaded) {
        loadObject(play, &fallObjectSegment, &fallObjectLoadQueue, getObjectId(getItemId));
        fallObjectLoading = true;
    } else if (osRecvMesg(&fallObjectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
        fallObjectLoading = false;
        fallObjectLoaded = true;
        fallTrueGI = getItemId;
    }
}

void EnFall_LoadObjectFast(EnFall* this, PlayState* play) {
    fallTrueGI = moonLiveGI;

    if (isAP(fallTrueGI)) {
        fallObjectStatic = true;
        fallObjectLoaded = true;
        return;
    }

    loadObject(play, &fallObjectSegment, &fallObjectLoadQueue, getObjectId(fallTrueGI));
    osRecvMesg(&fallObjectLoadQueue, NULL, OS_MESG_BLOCK);
    fallObjectLoaded = true;
}

RECOMP_PATCH void EnFall_Update(Actor* thisx, PlayState* play) {
    EnFall* this = THIS;

    if (!fallObjectLoaded) {
        EnFall_WaitForObject(this, play);
        return;
    }

    this->actionFunc(this, play);
}

void EnFall_RandoDraw(EnFall* this, PlayState* play) {
    s16 getItemId = moonLiveGI;

    Matrix_Translate(this->actor.world.pos.x + 500.0f, this->actor.world.pos.y - 6000.0f, this->actor.world.pos.z -3000.0f, MTXMODE_APPLY);
    Matrix_Scale(200.0f, 200.0f, 200.0f, MTXMODE_APPLY);

    switch(getItemId) {
        case GI_AP_PROG:
        case GI_AP_USEFUL:
        case GI_AP_FILLER:
            Matrix_RotateYS(DEG_TO_BINANG(-90), MTXMODE_APPLY);
            break;
        case GI_ROOM_KEY:
            Matrix_RotateYS(DEG_TO_BINANG(90), MTXMODE_APPLY);
            break;
        case GI_FROG_BLUE:
        case GI_FROG_CYAN:
        case GI_FROG_PINK:
        case GI_FROG_WHITE:
        case GI_FROG_YELLOW:
            Matrix_RotateYS(DEG_TO_BINANG(-90), MTXMODE_APPLY);
            break;
        default:
            break;
    }

    if (fallObjectLoaded) {
        if (fallTrueGI != getItemId) {
            EnFall_LoadObjectFast(this, play);
        }
        if (fallObjectStatic) {
            GetItem_Draw(play, getGid(getItemId));
        } else {
            GetItem_DrawDynamic(play, fallObjectSegment, getGid(getItemId));
        }
    }
}

/**
 * Used for all closed-mouth high-detail moons, including
 * StoppedClosedMouthMoon and CrashingMoon.
 */
RECOMP_PATCH void EnFall_Moon_Draw(Actor* thisx, PlayState* play) {
    // This offsets the moon's focus so that the Moon's Tear actually falls
    // out of its eye when looking at it through the telescope.
    static Vec3f sFocusOffset[] = { 1800.0f, 1000.0f, 4250.0f };
    EnFall* this = THIS;
    s32 primColor;

    OPEN_DISPS(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    Matrix_MultVec3f(sFocusOffset, &this->actor.focus.pos);

    EnFall_RandoDraw(this, play);

    CLOSE_DISPS(play->state.gfxCtx);
}

RECOMP_PATCH void EnFall_LodMoon_Draw(Actor* thisx, PlayState* play) {
    s32 pad;
    EnFall* this = THIS;
    s32 primColor;

    OPEN_DISPS(play->state.gfxCtx);

    EnFall_RandoDraw(this, play);

    CLOSE_DISPS(play->state.gfxCtx);
}

RECOMP_PATCH void EnFall_OpenMouthMoon_Draw(Actor* thisx, PlayState* play) {
    EnFall* this = THIS;
    s32 primColor;

    OPEN_DISPS(play->state.gfxCtx);

    EnFall_RandoDraw(this, play);

    CLOSE_DISPS(play->state.gfxCtx);
}