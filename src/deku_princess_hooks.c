#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnDnp;

typedef void (*EnDnpActionFunc)(struct EnDnp*, PlayState*);

#define DEKU_PRINCESS_LIMB_MAX 0x1A

typedef struct EnDnp {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnDnpActionFunc actionFunc;
    /* 0x18C */ ColliderCylinder collider;
    /* 0x1D8 */ Vec3f unk_1D8;
    /* 0x1E4 */ Vec3s unk_1E4;
    /* 0x1EA */ Vec3s jointTable[DEKU_PRINCESS_LIMB_MAX];
    /* 0x286 */ Vec3s morphTable[DEKU_PRINCESS_LIMB_MAX];
    /* 0x322 */ u16 unk_322;
    /* 0x324 */ u8 cueId;
    /* 0x328 */ s32 unk_328;
    /* 0x32C */ UNK_TYPE1 unk_32C[0x2];
    /* 0x32E */ s16 unk_32E;
    /* 0x330 */ s16 unk_330;
    /* 0x332 */ s16 unk_332;
    /* 0x334 */ s16 blinkTimer;
    /* 0x336 */ s16 eyeIndex;
    /* 0x338 */ s16 unk_338;
    /* 0x33A */ UNK_TYPE1 unk_33A[0x6];
    /* 0x340 */ s32 animIndex;
} EnDnp; // size = 0x344

RECOMP_PATCH void func_80B3D558(EnDnp* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->actor.csId)) {
        SET_WEEKEVENTREG(WEEKEVENTREG_23_20);
        play->nextEntrance = ENTRANCE(DEKU_KINGS_CHAMBER, 3);
        play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
        gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK_FAST;
        play->transitionTrigger = TRANS_TRIGGER_START;
        // CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
    } else {
        CutsceneManager_Queue(this->actor.csId);
    }
}

struct BgDkjailIvy;

typedef void (*BgDkjailIvyActionFunc)(struct BgDkjailIvy*, PlayState*);

typedef struct BgDkjailIvy {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ ColliderCylinder collider;
    /* 0x1A8 */ BgDkjailIvyActionFunc actionFunc;
    /* 0x1AC */ s8 fadeOutTimer;
    /* 0x1AD */ u8 alpha;
} BgDkjailIvy; // size = 0x1B0

#define BG_DKJAIL_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)

void BgDkjailIvy_IvyCutEffects(BgDkjailIvy* this, PlayState* play);
void BgDkjailIvy_SetupFadeOut(BgDkjailIvy* this);

RECOMP_PATCH void BgDkjailIvy_BeginCutscene(BgDkjailIvy* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->dyna.actor.csId)) {
        // CutsceneManager_StartWithPlayerCs(this->dyna.actor.csId, &this->dyna.actor);
        Player_SetCsActionWithHaltedActors(play, NULL, PLAYER_CSACTION_END);
        this->fadeOutTimer = 50;
        DynaPoly_DisableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
        Flags_SetSwitch(play, BG_DKJAIL_GET_SWITCH_FLAG(&this->dyna.actor));
        BgDkjailIvy_IvyCutEffects(this, play);
        Actor_PlaySfx(&this->dyna.actor, NA_SE_EV_GRASS_WALL_BROKEN);
        BgDkjailIvy_SetupFadeOut(this);
    } else {
        CutsceneManager_Queue(this->dyna.actor.csId);
    }
}