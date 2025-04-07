#include "global.h"
#include "modding.h"

#include "apcommon.h"

#include "recomputils.h"

#define LOCATION_LAB_HP 0x0701D9

#include "overlays/actors/ovl_En_Col_Man/z_en_col_man.h"

#define THIS ((EnColMan*)thisx)

s16 labHPTrueGI;

static bool labHPObjectStatic;
static bool labHPObjectLoading;
static bool labHPObjectLoaded;
static OSMesgQueue labHPObjectLoadQueue;
static void* labHPObjectSegment;

void EnColMan_WaitForObject(EnColMan* this, PlayState* play) {
    s16 getItemId = rando_get_item_id(LOCATION_LAB_HP);
    s16 objectSlot = Object_GetSlot(&play->objectCtx, getObjectId(getItemId));

    if (isAP(getItemId)) {
        labHPObjectStatic = true;
        labHPObjectLoaded = true;
        labHPTrueGI = getItemId;
    } else if (!labHPObjectLoaded && !labHPObjectLoading && Object_IsLoaded(&play->objectCtx, objectSlot)) {
        this->actor.objectSlot = objectSlot;
        Actor_SetObjectDependency(play, &this->actor);
        labHPObjectStatic = true;
        labHPObjectLoaded = true;
        labHPTrueGI = getItemId;
    } else if (!labHPObjectLoading && !labHPObjectLoaded) {
        loadObject(play, &labHPObjectSegment, &labHPObjectLoadQueue, getObjectId(getItemId));
        labHPObjectLoading = true;
    } else if (osRecvMesg(&labHPObjectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
        labHPObjectLoading = false;
        labHPObjectLoaded = true;
        labHPTrueGI = getItemId;
    }
}

RECOMP_PATCH void EnColMan_Update(Actor* thisx, PlayState* play) {
    s32 pad;
    EnColMan* this = THIS;

    if (this->actor.params == EN_COL_MAN_HEART_PIECE && !labHPObjectLoaded) {
        EnColMan_WaitForObject(this, play);
    }

    Actor_SetScale(&this->actor, this->scale);
    this->actionFunc(this, play);
    Actor_MoveWithGravity(&this->actor);
    Actor_UpdateBgCheckInfo(play, &this->actor, 30.0f, 30.0f, 30.0f,
                            UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4 |
                                UPDBGCHECKINFO_FLAG_8 | UPDBGCHECKINFO_FLAG_10);
    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetOC(play, &play->colChkCtx, &this->collider.base);
}

void func_80AFDE00(EnColMan* this, PlayState* play);

void EnColMan_DrawLabHP(Actor* thisx, PlayState* play) {
    s16 getItemId = rando_get_item_id(LOCATION_LAB_HP);

    Matrix_Scale(30.0f, 30.0f, 30.0f, MTXMODE_APPLY);

    if (labHPObjectLoaded) {
        if (labHPObjectStatic) {
            GetItem_Draw(play, getGid(getItemId));
        } else {
            GetItem_DrawDynamic(play, labHPObjectSegment, getGid(getItemId));
        }
    }
}

RECOMP_PATCH void func_80AFDD60(EnColMan* this) {
    this->actor.draw = EnColMan_DrawLabHP;
    this->actor.shape.yOffset = 700.0f;
    if (this->actor.params == EN_COL_MAN_HEART_PIECE) {
        this->actor.gravity = -2.0f;
    }
    this->type = EN_COL_MAN_HEART_PIECE;
    this->actionFunc = func_80AFDE00;
}

void EnColMan_SetHeartPieceCollectedAndKill(EnColMan* this, PlayState* play);

RECOMP_PATCH void func_80AFDE00(EnColMan* this, PlayState* play) {
    if (this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) {
        if (this->actor.params == EN_COL_MAN_HEART_PIECE) {
            this->actor.params = EN_COL_MAN_RECOVERY_HEART;
            this->actor.speed = 2.0f;
            this->actor.velocity.y = 8.0f;
        } else {
            this->actor.speed = 0.0f;
        }
    }
    if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_56_02)) {
        this->actor.shape.rot.y += 0x3E8;
    }
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actor.draw = NULL;
        this->actionFunc = EnColMan_SetHeartPieceCollectedAndKill;
    // } else if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_56_02)) {
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_HEART_PIECE, 40.0f, 40.0f);
    }
}