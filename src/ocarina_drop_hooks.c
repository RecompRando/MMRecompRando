#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct DmChar02;

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED)

#define THIS ((DmChar02*)thisx)

#define LOCATION_SOT 0x040067

typedef void (*DmChar02ActionFunc)(struct DmChar02*, PlayState*);

typedef struct DmChar02 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ UNK_TYPE1 pad_188[0x108];
    /* 0x290 */ DmChar02ActionFunc actionFunc;
    /* 0x294 */ UNK_TYPE1 pad_294[0x4C];
    /* 0x2E0 */ s16 animIndex;
    /* 0x2E2 */ UNK_TYPE1 pad_2E2[0xE];
    /* 0x2F0 */ u32 unk_2F0;
} DmChar02; // size = 0x2F4

void* ZeldaArena_Malloc(size_t size);
void DmChar02_HandleCutscene(DmChar02* this, PlayState* play);
s32 DmChar02_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx);
void DmChar02_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx);
void DmChar02_TransformLimbDraw(PlayState* play, s32 limbIndex, Actor* thisx);
void DmChar02_ChangeAnim(SkelAnime* skelAnime, AnimationInfo* animInfo, u16 animIndex);
void DmChar02_PlaySfxForCutscenes(DmChar02* this, PlayState* play);

typedef enum {
    /* 0 */ DMCHAR02_ANIM_HIT_GROUND,
    /* 1 */ DMCHAR02_ANIM_TURN_AROUND,
    /* 2 */ DMCHAR02_ANIM_JUGGLE,
    /* 3 */ DMCHAR02_ANIM_FALL,
    /* 4 */ DMCHAR02_ANIM_MAX
} DmChar02Animation;

extern FlexSkeletonHeader gClockTowerOcarinaOfTimeSkel;

extern AnimationInfo sAnimationInfo[DMCHAR02_ANIM_MAX];

static int ocarinaCheckState;
static bool objectStatic;
static bool objectLoading;
static bool objectLoaded;
static OSMesgQueue objectLoadQueue;
static void* objectSegment;

RECOMP_PATCH void DmChar02_Init(Actor* thisx, PlayState* play) {
    DmChar02* this = THIS;

    ocarinaCheckState = 0;
    objectSegment = ZeldaArena_Malloc(0x2000);

    //if (gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] == ITEM_NONE) {
    if (!rando_location_is_checked(GI_OCARINA_OF_TIME) || !rando_location_is_checked(LOCATION_SOT)) {
        this->animIndex = DMCHAR02_ANIM_HIT_GROUND;
        this->actor.lockOnArrowOffset = 3000.0f;
        ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 24.0f);
        SkelAnime_InitFlex(play, &this->skelAnime, &gClockTowerOcarinaOfTimeSkel, NULL, NULL, NULL, 0);
        DmChar02_ChangeAnim(&this->skelAnime, &sAnimationInfo[DMCHAR02_ANIM_HIT_GROUND], 0);
        Actor_SetScale(&this->actor, 0.01f);
        this->actionFunc = DmChar02_HandleCutscene;
    } else {
        Actor_Kill(&this->actor);
    }
}

RECOMP_PATCH void DmChar02_Update(Actor* thisx, PlayState* play) {
    DmChar02* this = THIS;

    /*s16 objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_GI_RESERVE_C_00);

    if (!objectLoaded && !objectLoading && Object_IsLoaded(&play->objectCtx, objectSlot)) {
        this->actor.objectSlot = objectSlot;
        Actor_SetObjectDependency(play, &this->actor);
        objectStatic = true;
        objectLoaded = true;
    } else if (!objectLoading && !objectLoaded) {
        //loadObject(play, objectSegment, &objectLoadQueue, getObjectId(moonsTearTrueGI));
        loadObject(play, objectSegment, &objectLoadQueue, OBJECT_GI_RESERVE_C_00);
        objectLoading = true;
    } else if (osRecvMesg(&objectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
        objectLoading = false;
        objectLoaded = true;
    }*/

    SkelAnime_Update(&this->skelAnime);
    this->unk_2F0 = this->unk_2F0;
    this->actionFunc(this, play);
    if ((this->actor.xzDistToPlayer <= 30.0f) && (fabsf(this->actor.playerHeightRel) <= fabsf(80.0f))) {
        // Audio_PlayFanfare(NA_BGM_GET_SMALL_ITEM);
        if (!rando_location_is_checked(GI_OCARINA_OF_TIME)) {
            Actor_OfferGetItem(&this->actor, play, GI_OCARINA_OF_TIME, 300.0f, 300.0f);
        } else if (!rando_location_is_checked(LOCATION_SOT)) {
            Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_SOT), LOCATION_SOT, 300.0f, 300.0f, true, true);
        } else {
            Actor_Kill(thisx);
        }
    }
    /*if (ocarinaCheckState == 0 && !Actor_HasParent(&this->actor, play)) {
        //Actor_OfferGetItem(&this->actor, play, GI_OCARINA_OF_TIME, 30.0f, 80.0f);
        Actor_OfferGetItemHook(&this->actor, play, apGetItemId(GI_OCARINA_OF_TIME), GI_OCARINA_OF_TIME, 30.0f, 80.0f);
    } else if (ocarinaCheckState == 0 && Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        ocarinaCheckState = 1;
    } else if (ocarinaCheckState == 1 && !Actor_HasParent(&this->actor, play)) {
        Actor_OfferGetItemHook(&this->actor, play, apGetItemId(0x040067), 0x040067, 30.0f, 80.0f);
    } else if (Actor_HasParent(&this->actor, play)) {
        //gSaveContext.save.playerForm = PLAYER_FORM_HUMAN;
        Actor_Kill(&this->actor);
    }*/

    DmChar02_PlaySfxForCutscenes(this, play);
}

RECOMP_PATCH void DmChar02_Draw(Actor* thisx, PlayState* play) {
    DmChar02* this = THIS;
    s32 shouldDraw = false;

    if ((play->csCtx.state == CS_STATE_IDLE) && (this->actor.world.pos.y < 100.0f)) {
        shouldDraw = true;
    } else if (Cutscene_IsCueInChannel(play, CS_CMD_ACTOR_CUE_107)) {
        switch (play->csCtx.actorCues[Cutscene_GetCueChannel(play, CS_CMD_ACTOR_CUE_107)]->id) {
            case 0x17:
            case 0x1C:
            case 0x26:
                shouldDraw = true;
                break;
        }
    }

    if (shouldDraw) {
    //if (shouldDraw && objectLoaded) {
        Gfx_SetupDL25_Opa(play->state.gfxCtx);
        SkelAnime_DrawTransformFlexOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable,
                                       this->skelAnime.dListCount, DmChar02_OverrideLimbDraw, DmChar02_PostLimbDraw,
                                       DmChar02_TransformLimbDraw, &this->actor);
        /*if (objectStatic) {
            GetItem_Draw(play, OBJECT_GI_RESERVE_C_00);
        } else {
            // TODO: use segment 0xD instead of 0x6?
            GetItem_DrawDynamic(play, objectSegment, OBJECT_GI_RESERVE_C_00);
        }*/
    }
}

RECOMP_PATCH void DmChar02_ChangeAnim(SkelAnime* skelAnime, AnimationInfo* animInfo, u16 animIndex) {
    f32 endFrame;

    animInfo += animIndex;

    if (animInfo->frameCount < 0.0f) {
        endFrame = Animation_GetLastFrame(animInfo->animation);
    } else {
        endFrame = animInfo->frameCount;
    }

    Animation_Change(skelAnime, animInfo->animation, animInfo->playSpeed, animInfo->startFrame, endFrame,
                     animInfo->mode, animInfo->morphFrames);
}