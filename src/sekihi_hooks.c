#include "global.h"
#include "modding.h"

#include "apcommon.h"

#include "overlays/actors/ovl_En_Sekihi/z_en_sekihi.h"

bool healedMikau;

void func_80A450B0(EnSekihi* this, PlayState* play);

void EnSekihi_GiveMask(EnSekihi* this, PlayState* play) {
    if (!Actor_HasParent(&this->dyna.actor, play)) {
        Actor_OfferGetItem(&this->dyna.actor, play, GI_MASK_ZORA, 1000.0f, 1000.0f);
    } else {
        healedMikau = false;
        this->dyna.actor.parent = NULL;
        this->actionFunc = func_80A450B0;
    }
}

RECOMP_HOOK("func_80A450B0") void On_Guitar_Update(EnSekihi* this, PlayState* play) {
    if (healedMikau) {
        this->actionFunc = EnSekihi_GiveMask;
    }
}

extern CollisionHeader gSunsSongGraveTriforceCol;
extern CollisionHeader gSunsSongGraveGoronCol;
extern CollisionHeader gSunsSongGraveKokiriCol;
extern CollisionHeader gSongOfSoaringPedestalCol;
extern CollisionHeader gMikauGraveCol;

void EnSekihi_Draw(Actor* thisx, PlayState* play);
void EnSekihi_DoNothing(EnSekihi* this, PlayState* play);

RECOMP_PATCH void func_80A44DE8(EnSekihi* this, PlayState* play) {
    CollisionHeader* colHeader = NULL;
    s32 type;
    CollisionHeader* colHeaders[] = {
        &gSunsSongGraveTriforceCol, &gSunsSongGraveGoronCol, &gSunsSongGraveKokiriCol,
        &gSongOfSoaringPedestalCol, &gMikauGraveCol,
    };

    type = ENSIKIHI_GET_TYPE(&this->dyna.actor);
    if (Object_IsLoaded(&play->objectCtx, this->objectSlot)) {
        this->dyna.actor.objectSlot = this->objectSlot;
        this->dyna.actor.draw = EnSekihi_Draw;
        if (type == SEKIHI_TYPE_4) {
            this->actionFunc = func_80A450B0;
        } else {
            this->actionFunc = EnSekihi_DoNothing;
        }

        Actor_SetObjectDependency(play, &this->dyna.actor);
        DynaPolyActor_Init(&this->dyna, 0);
        if (colHeaders[type] != NULL) {
            CollisionHeader_GetVirtual(colHeaders[type], &colHeader);
        }

        this->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &this->dyna.actor, colHeader);
        // if ((type == SEKIHI_TYPE_4) && (INV_CONTENT(ITEM_MASK_ZORA) != ITEM_MASK_ZORA)) {
        //     Actor_Kill(&this->dyna.actor);
        // }
    }
}