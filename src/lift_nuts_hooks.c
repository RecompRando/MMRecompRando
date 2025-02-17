#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define BUSINESS_SCRUB_LIMB_MAX 0x1C

struct EnLiftNuts;

#define FLAGS                                                                                  \
    (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED | \
     ACTOR_FLAG_UPDATE_DURING_OCARINA)

#define THIS ((EnLiftNuts*)thisx)

typedef void (*EnLiftNutsActionFunc)(struct EnLiftNuts*, PlayState*);

#define ENLIFTNUTS_GET_PATH_INDEX(thisx) (((thisx)->params & 0xFF00) >> 8)

typedef struct EnLiftNuts {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnLiftNutsActionFunc actionFunc;
    /* 0x1D8 */ Vec3f waypointPos;
    /* 0x1E4 */ s32 eyeTexIndex;
    /* 0x1E8 */ s32 unk1E8; // Set but never used
    /* 0x1EC */ s16* minigameScore; // Pointer to shared memory location with actor EnGamelupy
    /* 0x1F0 */ Vec3s jointTable[BUSINESS_SCRUB_LIMB_MAX];
    /* 0x298 */ Vec3s morphTable[BUSINESS_SCRUB_LIMB_MAX];
    /* 0x340 */ UNK_TYPE1 unk_340[0xC];
    /* 0x34C */ s16 textId;
    /* 0x34E */ s16 autotalk;
    /* 0x350 */ UNK_TYPE1 unk_350[0x4];
    /* 0x354 */ s16 timer; // Frame counter used for various different things
    /* 0x356 */ s16 isFirstTimeHiding;
} EnLiftNuts; // size = 0x358

void EnLiftNuts_HandleConversationEvent(EnLiftNuts* this, PlayState* play);

void EnLiftNuts_SetupIdleHidden(EnLiftNuts* this);
void EnLiftNuts_IdleHidden(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_Burrow(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupIdle(EnLiftNuts* this);
void EnLiftNuts_RiseUp(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_Idle(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupStartConversation(EnLiftNuts* this);
void EnLiftNuts_StartConversation(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_HandleConversation(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupMove(EnLiftNuts* this);
void EnLiftNuts_Move(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupMovePlayerToActor(EnLiftNuts* this);
void EnLiftNuts_MovePlayerToActor(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupStartGame(EnLiftNuts* this);
void EnLiftNuts_StartGame(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupStartGameImmediately(EnLiftNuts* this);
void EnLiftNuts_StartGameImmediately(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupRunGame(EnLiftNuts* this);
void EnLiftNuts_RunGame(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupEndGame(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_EndGame(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupGiveReward(EnLiftNuts* this);
void EnLiftNuts_GiveReward(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupResumeConversation(EnLiftNuts* this);
void EnLiftNuts_ResumeConversation(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_SetupStartHiding(EnLiftNuts* this);
void EnLiftNuts_StartHiding(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_Hide(EnLiftNuts* this, PlayState* play);
void EnLiftNuts_UpdateEyes(EnLiftNuts* this);
void EnLiftNuts_SpawnDust(EnLiftNuts* this, PlayState* play);

RECOMP_PATCH void EnLiftNuts_GiveReward(EnLiftNuts* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        if (CHECK_WEEKEVENTREG(WEEKEVENTREG_WON_DEKU_PLAYGROUND_DAY_1) &&
            CHECK_WEEKEVENTREG(WEEKEVENTREG_WON_DEKU_PLAYGROUND_DAY_2) && (CURRENT_DAY == 3) &&
            !CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_DEKU_PLAYGROUND_HEART_PIECE)) {
            SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_DEKU_PLAYGROUND_HEART_PIECE);
        }
        EnLiftNuts_SetupResumeConversation(this);
    //} else if ((this->textId == 0x27F4) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_DEKU_PLAYGROUND_HEART_PIECE)) {
    } else if ((this->textId == 0x27F4) && !rando_location_is_checked(LOCATION_PLAYGROUND_ALL_DAYS)) {
        Actor_OfferGetItem(&this->actor, play, GI_HEART_PIECE, 500.0f, 100.0f);
    //} else {
    } else if (!rando_location_is_checked(LOCATION_PLAYGROUND_ANY_DAY)) {
        Actor_OfferGetItem(&this->actor, play, GI_RUPEE_PURPLE, 500.0f, 100.0f);
    } else {
        Actor_OfferGetItemHook(&this->actor, play, GI_RUPEE_PURPLE, 0, 500.0f, 100.0f, false, false);
    }
}

void EnLiftNuts_SetupResumeConversation(EnLiftNuts* this) {
    this->actionFunc = EnLiftNuts_ResumeConversation;
}