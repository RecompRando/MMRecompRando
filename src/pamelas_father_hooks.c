#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnHg;

typedef void (*EnHgActionFunc)(struct EnHg*, PlayState*);

#define PAMELAS_FATHER_GIBDO_LIMB_MAX 0x13

typedef struct EnHg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnHgActionFunc actionFunc;
    /* 0x1D8 */ MtxF mf;
    /* 0x218 */ s32 csIdIndex;
    /* 0x21C */ s32 animIndex;
    /* 0x220 */ Vec3s jointTable[PAMELAS_FATHER_GIBDO_LIMB_MAX];
    /* 0x292 */ Vec3s morphTable[PAMELAS_FATHER_GIBDO_LIMB_MAX];
    /* 0x304 */ UNK_TYPE1 pad304[0xC];
    /* 0x310 */ u16 csIdList[4];
} EnHg; // size = 0x318

struct EnHgo;

typedef void (*EnHgoActionFunc)(struct EnHgo*, PlayState*);

#define PAMELAS_FATHER_HUMAN_LIMB_MAX 0x13

typedef struct EnHgo {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnHgoActionFunc actionFunc;
    /* 0x1D8 */ MtxF mf;
    /* 0x218 */ s32 animIndex;
    /* 0x21C */ Vec3s jointTable[PAMELAS_FATHER_HUMAN_LIMB_MAX];
    /* 0x28E */ Vec3s morphTable[PAMELAS_FATHER_HUMAN_LIMB_MAX];
    /* 0x300 */ Vec3s headRot;
    /* 0x306 */ Vec3s torsoRot; // Set, but not used
    /* 0x30C */ s16 eyeIndex;
    /* 0x30E */ s16 blinkTimer;
    /* 0x310 */ s16 talkFlags;
    /* 0x312 */ s16 isInCutscene;
    /* 0x314 */ u16 textId;
    /* 0x316 */ u16 cueId;
} EnHgo; // size = 0x318

typedef enum {
    /* 0 */ HG_CS_FIRST_ENCOUNTER,
    /* 1 */ HG_CS_GET_MASK,
    /* 2 */ HG_CS_SUBSEQUENT_ENCOUNTER,
    /* 3 */ HG_CS_SONG_OF_HEALING
} HgCsIndex;

void EnHg_PlayCutscene(EnHg* this, PlayState* play);

void EnHg_OfferGibdoMask(EnHg* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnHg_PlayCutscene;
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_MASK_GIBDO, 1000.0f, 1000.0f);
    }
}

EnHg* gEnHg;

RECOMP_HOOK("EnHg_WaitForPlayerAction")
void EnHg_OnWaitForPlayerAction(EnHg* this, PlayState* play) {
    gEnHg = this;
}

RECOMP_HOOK_RETURN("EnHg_WaitForPlayerAction")
void EnHg_ForceGibdoMaskDrop() {
    PlayState* play = gPlay;
    Player* player = GET_PLAYER(play);
    EnHg* this = gEnHg;

    // If the player has gone upstairs this interaction will not trigger
    if ((this->actor.colChkInfo.health != 1) || (fabsf(this->actor.playerHeightRel) >= 80.0f)) {
        return;
    }
    
    if (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) {
        if ((play->msgCtx.lastPlayedSong == OCARINA_SONG_HEALING) && (GET_PLAYER_FORM == PLAYER_FORM_HUMAN)) {
            // @rando always force non-mask cutscene to fix offer
            this->csIdIndex = HG_CS_SONG_OF_HEALING;
            this->actionFunc = EnHg_OfferGibdoMask; // could choose to not show when already checked, but ¯\_(ツ)_/¯
        }
    }
}