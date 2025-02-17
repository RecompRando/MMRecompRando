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

void ItemBHeart_UpdateModel(ItemBHeart* this, PlayState* play);

#define LOCATION_HEART_CONTAINER ((0x050000) | (play->sceneId << 8))

//~ s16 hcGetItemId;
//~ bool hcObjectStatic;
//~ bool hcObjectLoading;
//~ bool hcObjectLoaded;
//~ OSMesgQueue hcObjectLoadQueue;
//~ void* hcObjectSegment;

//~ static InitChainEntry sInitChain[] = {
    //~ ICHAIN_VEC3F_DIV1000(scale, 0, ICHAIN_CONTINUE),
    //~ ICHAIN_F32(uncullZoneForward, 4000, ICHAIN_CONTINUE),
    //~ ICHAIN_F32(uncullZoneScale, 800, ICHAIN_CONTINUE),
    //~ ICHAIN_F32(uncullZoneDownward, 800, ICHAIN_STOP),
//~ };

//~ void ItemBHeart_Init(Actor* thisx, PlayState* play) {
    //~ ItemBHeart* this = THIS;

    //~ if (recomp_location_is_checked(LOCATION_HEART_CONTAINER)) {
        //~ Flags_SetCollectible(play, 0x1F);
    //~ }

    //~ if (Flags_GetCollectible(play, 0x1F)) {
        //~ Actor_Kill(&this->actor);
        //~ return;
    //~ }
    //~ Actor_ProcessInitChain(&this->actor, sInitChain);
    //~ ActorShape_Init(&this->actor.shape, 0.0f, NULL, 0.8f);
    //~ if (this->actor.params == BHEART_PARAM_SMALL) {
        //~ this->baseScale = BHEART_SCALE_SMALL;
    //~ } else {
        //~ this->baseScale = BHEART_SCALE_NORMAL;
    //~ }
    //~ this->actor.world.pos.y += 20.0f * this->baseScale;
    //~ hcGetItemId = apGetItemId(LOCATION_HEART_CONTAINER);
    //~ hcObjectStatic = false;
    //~ hcObjectLoading = false;
    //~ hcObjectLoaded = false;
    //~ hcObjectSegment = NULL;
//~ }

/**
 * Adjusts size and handles collection (if of proper baseScale)
 */
RECOMP_PATCH void ItemBHeart_Update(Actor* thisx, PlayState* play) {
    ItemBHeart* this = THIS;

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

//~ extern Gfx gGiHeartBorderDL[];
//~ extern Gfx gGiHeartContainerDL[];

//~ /**
 //~ * Draw translucently when in front of a boss warp portal
 //~ */
//~ void ItemBHeart_Draw(Actor* thisx, PlayState* play) {
    //~ ItemBHeart* this = THIS;
    //~ Actor* actorIt;
    //~ u8 drawTranslucent = false;

    //~ OPEN_DISPS(play->state.gfxCtx);

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
        //~ MATRIX_FINALIZE_AND_LOAD(POLY_XLU_DISP++, play->state.gfxCtx);
        //~ gSPDisplayList(POLY_XLU_DISP++, &gGiHeartBorderDL);
        //~ gSPDisplayList(POLY_XLU_DISP++, &gGiHeartContainerDL);
    //~ } else {
        //~ Gfx_SetupDL25_Opa(play->state.gfxCtx);
        //~ MATRIX_FINALIZE_AND_LOAD(POLY_OPA_DISP++, play->state.gfxCtx);
        //~ gSPDisplayList(POLY_OPA_DISP++, &gGiHeartBorderDL);
        //~ gSPDisplayList(POLY_OPA_DISP++, &gGiHeartContainerDL);
    //~ }

    //~ CLOSE_DISPS(play->state.gfxCtx);
//~ }