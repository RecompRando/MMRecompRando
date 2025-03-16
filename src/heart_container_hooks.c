#include "modding.h"
#include "global.h"

#include "apcommon.h"

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

typedef enum {
    /*  0 */ BHEART_PARAM_NORMAL,    // Spawn Parameter for Normal Scale.
    /* 35 */ BHEART_PARAM_SMALL = 35 // Spawn Parameter for Small Scale.
} ItemBHeartParam;

#define BHEART_SCALE_SMALL (0.1f)           // Scale value for Small Mode
#define BHEART_SCALE_NORMAL (1.0f)          // Scale value for Normal Mode
#define BHEART_SCALE_MIN_COLLECTIBLE (0.5f) // baseScale <50% will disable collection

typedef struct ItemBHeart {
    /* 0x000 */ Actor actor;
    /* 0x144 */ UNK_TYPE1 unk_144[0x20];
    /* 0x164 */ f32 variableScale;
    /* 0x168 */ f32 baseScale;
} ItemBHeart; // size = 0x16C

#define FLAGS 0x00000000

#define THIS ((ItemBHeart*)thisx)

#define LOCATION_HEART_CONTAINER ((0x050000) | (play->sceneId << 8))

void ItemBHeart_UpdateModel(ItemBHeart* this, PlayState* play);

s16 hcGetItemId;
bool hcObjectStatic;
bool hcObjectLoading;
bool hcObjectLoaded;
OSMesgQueue hcObjectLoadQueue;
void* hcObjectSegment;

static InitChainEntry sInitChain[] = {
    ICHAIN_VEC3F_DIV1000(scale, 0, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
};

RECOMP_PATCH void ItemBHeart_Init(Actor* thisx, PlayState* play) {
    ItemBHeart* this = THIS;

    if (rando_location_is_checked(LOCATION_HEART_CONTAINER)) {
        Actor_Kill(&this->actor);
        return;
    }
    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.8f);
    if (this->actor.params == BHEART_PARAM_SMALL) {
        this->baseScale = BHEART_SCALE_SMALL;
    } else {
        this->baseScale = BHEART_SCALE_NORMAL;
    }
    this->actor.world.pos.y += 20.0f * this->baseScale;
    hcGetItemId = rando_get_item_id(LOCATION_HEART_CONTAINER);
    hcObjectStatic = false;
    hcObjectLoading = false;
    hcObjectLoaded = false;
    hcObjectSegment = NULL;
}

void ItemBHeart_WaitForObject(ItemBHeart* this, PlayState* play) {
    s16 getItemId = rando_get_item_id(LOCATION_HEART_CONTAINER);
    u16 objectId = getObjectId(getItemId);
    s16 objectSlot = Object_GetSlot(&play->objectCtx, objectId);

    if (isAP(getItemId)) {
        hcGetItemId = getItemId;
        hcObjectStatic = true;
        hcObjectLoaded = true;
    } else if (!hcObjectLoaded && !hcObjectLoading && Object_IsLoaded(&play->objectCtx, objectSlot)) {
        this->actor.objectSlot = objectSlot;
        Actor_SetObjectDependency(play, &this->actor);
        hcObjectStatic = true;
        hcObjectLoaded = true;
        hcGetItemId = getItemId;
    } else if (!hcObjectLoading && !hcObjectLoaded) {
        loadObject(play, &hcObjectSegment, &hcObjectLoadQueue, objectId);
        hcObjectLoading = true;
    } else if (osRecvMesg(&hcObjectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
        hcObjectLoading = false;
        hcGetItemId = getItemId;
        hcObjectLoaded = true;
    }
}

/**
 * Adjusts size and handles collection (if of proper baseScale)
 */
RECOMP_PATCH void ItemBHeart_Update(Actor* thisx, PlayState* play) {
    ItemBHeart* this = THIS;

    if (!hcObjectLoaded) {
        ItemBHeart_WaitForObject(this, play);
        return;
    }

    ItemBHeart_UpdateModel(this, play);

    if (!(this->baseScale < BHEART_SCALE_MIN_COLLECTIBLE)) {
        if (Actor_HasParent(&this->actor, play)) {
            recomp_printf("Heart Container location: 0x%06X\n", LOCATION_HEART_CONTAINER);
            rando_send_location(LOCATION_HEART_CONTAINER);
            Flags_SetCollectible(play, 0x1F);
            Actor_Kill(&this->actor);
            return;
        }
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_HEART_CONTAINER), 0, 30.0f, 80.0f, false, true);
    }
}

//~ void ItemBHeart_Destroy(Actor* thisx, PlayState* play) {
    //~ if (!bossWorkaround && objectSegment != NULL) {
        //~ ZeldaArena_Free(objectSegment);
        //~ objectSegment = NULL;
    //~ }
//~ }

extern Gfx gGiHeartBorderDL[];
extern Gfx gGiHeartContainerDL[];

/**
 * Draw translucently when in front of a boss warp portal
 */
RECOMP_PATCH void ItemBHeart_Draw(Actor* thisx, PlayState* play) {
    ItemBHeart* this = THIS;
    Actor* actorIt;
    u8 drawTranslucent = false;

    OPEN_DISPS(play->state.gfxCtx);

    //~ actorIt = play->actorCtx.actorLists[ACTORCAT_ITEMACTION].first;

    //~ while (actorIt != NULL) {
        //~ if ((actorIt->id == ACTOR_DOOR_WARP1) && (actorIt->projectedPos.z > this->actor.projectedPos.z)) {
            //~ drawTranslucent = true;
            //~ break;
        //~ }
        //~ actorIt = actorIt->next;
    //~ }

    //~ if (drawTranslucent || (this->actor.world.rot.y != 0)) {
        //~ Gfx_SetupDL25_Xlu(play->state.gfxCtx);
        //~ gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        //~ gSPDisplayList(POLY_XLU_DISP++, &gGiHeartBorderDL);
        //~ gSPDisplayList(POLY_XLU_DISP++, &gGiHeartContainerDL);
    //~ } else {
        //~ Gfx_SetupDL25_Opa(play->state.gfxCtx);
        //~ gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
        //~ gSPDisplayList(POLY_OPA_DISP++, &gGiHeartBorderDL);
        //~ gSPDisplayList(POLY_OPA_DISP++, &gGiHeartContainerDL);
    //~ }

    if (hcObjectLoaded) {
        if (((u32) hcGetItemId) != rando_get_item_id(LOCATION_HEART_CONTAINER)) {
            hcObjectLoading = false;
            hcObjectLoaded = false;
            hcObjectStatic = false;
            hcGetItemId = rando_get_item_id(LOCATION_HEART_CONTAINER);
            return;
        }
        if (hcObjectStatic) {
            GetItem_Draw(play, getGid(hcGetItemId));
        } else {
            GetItem_DrawDynamic(play, hcObjectSegment, getGid(hcGetItemId));
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}