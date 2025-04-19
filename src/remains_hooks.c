#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define LOCATION_REMAINS(i) (GI_REMAINS_ODOLWA + i)
#define LOCATION_SONG_OATH 0x040065

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20)
#define THIS ((DmHina*)thisx)

struct DmHina;

typedef void (*DmHinaActionFunc)(struct DmHina*, PlayState*);

typedef struct DmHina {
    /* 0x000 */ Actor actor;
    /* 0x144 */ DmHinaActionFunc actionFunc;
    /* 0x148 */ f32 unk148;
    /* 0x14C */ f32 unk14C;
    /* 0x150 */ f32 unk150;
    /* 0x154 */ f32 unk154;
    /* 0x158 */ f32 unk158;
    /* 0x15C */ f32 unk15C;
    /* 0x160 */ Vec3f subCamEye;
    /* 0x16C */ Vec3f subCamAt;
    /* 0x178 */ UNK_TYPE1 pad178[2];
    /* 0x17A */ s16 subCamId;
    /* 0x17C */ s16 unk17C;
    /* 0x17E */ u8 unk17E;
    /* 0x17F */ u8 unk17F;
    /* 0x180 */ u8 isDrawn;
} DmHina; // size = 0x184

bool rChecked[] = { false, false, false, false };
s16 rGetItemId;
bool rObjectStatic;
bool rObjectLoading;
bool rObjectLoaded;
OSMesgQueue rObjectLoadQueue;
void* rObjectSegment;

void func_80A1F470(DmHina* this, PlayState* play);

RECOMP_PATCH void DmHina_Init(Actor* thisx, PlayState* play) {
    DmHina* this = THIS;

    rGetItemId = rando_get_item_id(LOCATION_REMAINS(this->actor.params));
    rObjectStatic = false;
    rObjectLoading = false;
    rObjectLoaded = false;
    rObjectSegment = NULL;

    this->isDrawn = true;
    this->actionFunc = func_80A1F470;
    this->unk158 = this->actor.world.pos.y;
    this->unk148 = 0.0f;
    this->unk15C = 1.0f;
    this->actor.focus.pos.x = this->actor.world.pos.x;
    this->actor.focus.pos.y = this->actor.world.pos.y;
    this->actor.focus.pos.z = this->actor.world.pos.z;
}

void DmHina_WaitForObject(DmHina* this, PlayState* play) {
    s16 getItemId = rando_get_item_id(LOCATION_REMAINS(this->actor.params));
    u16 objectId = getObjectId(getItemId);
    s16 objectSlot = Object_GetSlot(&play->objectCtx, objectId);

    if (isAP(getItemId)) {
        rGetItemId = getItemId;
        rObjectStatic = true;
        rObjectLoaded = true;
    } else if (!rObjectLoaded && !rObjectLoading && Object_IsLoaded(&play->objectCtx, objectSlot)) {
        this->actor.objectSlot = objectSlot;
        Actor_SetObjectDependency(play, &this->actor);
        rObjectStatic = true;
        rObjectLoaded = true;
        rGetItemId = getItemId;
    } else if (!rObjectLoading && !rObjectLoaded) {
        loadObject(play, &rObjectSegment, &rObjectLoadQueue, objectId);
        rObjectLoading = true;
    } else if (osRecvMesg(&rObjectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
        rObjectLoading = false;
        rGetItemId = getItemId;
        rObjectLoaded = true;
    }
}

void func_80A1F75C(DmHina* this, PlayState* play);

RECOMP_PATCH void DmHina_Update(Actor* thisx, PlayState* play) {
    DmHina* this = THIS;

    if (!rObjectLoaded) {
        DmHina_WaitForObject(this, play);
    }

    this->actionFunc(this, play);
    func_80A1F75C(this, play);
}

void func_80A1F9AC(DmHina* this, PlayState* play);

RECOMP_PATCH void DmHina_Draw(Actor* thisx, PlayState* play) {
    DmHina* this = THIS;
    f32 scale;

    if (this->isDrawn) {
        Matrix_Translate(this->actor.world.pos.x,
                         this->actor.world.pos.y + (this->unk154 * this->unk15C) + (40.0f * this->unk15C),
                         this->actor.world.pos.z, MTXMODE_NEW);
        Matrix_RotateZYX(0, play->gameplayFrames * 0x3E8, 0, MTXMODE_APPLY);
        scale = this->unk148 * (1.0f - this->unk14C) * this->unk15C;
        Matrix_Scale(scale, scale, scale, MTXMODE_APPLY);

        if (rObjectLoaded) {
            if (!rChecked[this->actor.params] && ((u32) rGetItemId) != rando_get_item_id(LOCATION_REMAINS(this->actor.params))) {
                rObjectLoading = false;
                rObjectLoaded = false;
                rObjectStatic = false;
                rGetItemId = rando_get_item_id(LOCATION_REMAINS(this->actor.params));
                return;
            }
            if (rObjectStatic) {
                GetItem_Draw(play, getGid(rGetItemId));
            } else {
                GetItem_DrawDynamic(play, rObjectSegment, getGid(rGetItemId));
            }
        }

        func_80A1F9AC(this, play);
    }
}

void func_80A1F56C(DmHina* this, PlayState* play);
void func_80A1F5AC(DmHina* this, PlayState* play);

void DmHina_OfferOath(DmHina* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = func_80A1F56C;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_SONG_OATH), LOCATION_SONG_OATH, 30.0f, 80.0f, true, true);
    }
}

RECOMP_PATCH void func_80A1F56C(DmHina* this, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        // @rando override warp animation
        if (!rando_location_is_checked(LOCATION_SONG_OATH)) {
            this->actor.parent = NULL;
            this->actionFunc = DmHina_OfferOath;
            return;
        }
        this->unk17C = 2;
        this->actionFunc = func_80A1F5AC;
    }
}