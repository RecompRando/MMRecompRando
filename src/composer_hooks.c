#include "modding.h"
#include "global.h"

#include "z64player.h"

typedef enum {
    /*  1 */ POE_COMPOSER_CUEID_IDLE = 1,
    /*  2 */ POE_COMPOSER_CUEID_APPEAR_SPIN,
    /*  3 */ POE_COMPOSER_CUEID_IDLE2,
    /*  4 */ POE_COMPOSER_CUEID_RAISE_ARMS,
    /*  5 */ POE_COMPOSER_CUEID_LOWER_ARMS,
    /*  6 */ POE_COMPOSER_CUEID_PLAY_CURSE,
    /*  7 */ POE_COMPOSER_CUEID_ROLL,
    /*  8 */ POE_COMPOSER_CUEID_APPEAR,
    /*  9 */ POE_COMPOSER_CUEID_DISAPPEAR,
    /* 10 */ POE_COMPOSER_CUEID_DONE,
    /* 99 */ POE_COMPOSER_CUEID_NONE = 99
} EnPoComposerCueId;

typedef enum {
    /* 0 */ SHARP_CS_ENCOUNTER_FIRST,
    /* 1 */ SHARP_CS_ENCOUNTER_OTHER,
    /* 2 */ SHARP_CS_SONG_HEALING,
    /* 3 */ SHARP_CS_SONG_STORMS,
    /* 4 */ SHARP_CS_MAX
} SharpCsIdIndex;

struct EnPoComposer;

typedef void (*EnPoComposerActionFunc)(struct EnPoComposer*, PlayState*);

 #define POE_COMPOSER_LIMB_MAX 0xC

typedef struct EnPoComposer {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder bodyCollider;
    /* 0x190 */ ColliderJntSph lanternCollider;
    /* 0x1B0 */ ColliderJntSphElement lanternColliderElements[1];
    /* 0x1F0 */ SkelAnime skelAnime;
    /* 0x234 */ EnPoComposerActionFunc actionFunc;
    /* 0x238 */ MtxF lanternMtxF;
    /* 0x278 */ LightNode* lightNode;
    /* 0x27C */ LightInfo lightInfo;
    /* 0x28A */ UNK_TYPE1 unk_28A[0x6];
    /* 0x290 */ s32 sharpCsNum;
    /* 0x294 */ Vec3s jointTable[POE_COMPOSER_LIMB_MAX];
    /* 0x2DC */ Vec3s morphTable[POE_COMPOSER_LIMB_MAX];
    /* 0x324 */ s16 csIds[SHARP_CS_MAX];
    /* 0x32C */ s16 actionTimer;
    /* 0x32E */ s16 csCueTimer;
    /* 0x330 */ s16 inCutscene;
    /* 0x332 */ s16 visible;
    /* 0x334 */ u16 cueId;
    /* 0x336 */ Color_RGBA8 lightColor;
    /* 0x33A */ Color_RGBA8 envColor;
} EnPoComposer; // size = 0x340

static s32 sPlayerIsPlayingOcarina = false;

void EnPoComposer_StartCutscene(EnPoComposer* this, PlayState* play);

void EnPoComposer_SetupStartCutscene(EnPoComposer* this) {
    this->inCutscene = true;
    // this->actionFunc = EnPoComposer_StartCutscene;
    this->actionFunc = EnPoComposer_StartCutscene;
}

void EnPoComposer_SharpCutsceneDone(EnPoComposer* this) {
    SET_WEEKEVENTREG(WEEKEVENTREG_14_04);
    Actor_Kill(&this->actor);
}

RECOMP_PATCH void EnPoComposer_PlayCurse(EnPoComposer* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    // Damage the player every second
    if (this->actionTimer == 20) {
        func_800B8D10(play, &this->actor, 0.0f, 0, 0.0f, 1, 4);
        this->actionTimer = 0;
    } else {
        this->actionTimer++;
    }

    // Ocarina check
    if (player->stateFlags2 & PLAYER_STATE2_USING_OCARINA) {
        if (!sPlayerIsPlayingOcarina) {
            // Play sound whenever the player begins playing the Ocarina
            Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
        }
        sPlayerIsPlayingOcarina = true;
    } else {
        sPlayerIsPlayingOcarina = false;
    }

    // Song check, check if player just played song of storms or song of healing
    if (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) {
        switch (play->msgCtx.lastPlayedSong) {
            case OCARINA_SONG_STORMS:
                this->sharpCsNum = SHARP_CS_SONG_STORMS;
                // EnPoComposer_SetupStartCutscene(this);
                EnPoComposer_SharpCutsceneDone(this);
                this->inCutscene = false;

                play->nextEntrance = ENTRANCE(IKANA_CANYON, 13);
                play->transitionType = TRANS_TYPE_FADE_BLACK_FAST;
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK_FAST;
                play->transitionTrigger = TRANS_TRIGGER_START;
                return;

            case OCARINA_SONG_HEALING:
                this->sharpCsNum = SHARP_CS_SONG_HEALING;
                EnPoComposer_SetupStartCutscene(this);
                break;

            default:
                break;
        }
    }

    // Play sharp's curse bgm
    Audio_PlaySequenceAtPos(SEQ_PLAYER_BGM_SUB, &this->actor.projectedPos, NA_BGM_SHARPS_CURSE, 2000.0f);
}