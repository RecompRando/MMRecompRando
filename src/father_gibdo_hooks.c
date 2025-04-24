#include "modding.h"
#include "global.h"

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

typedef enum {
    /* 0 */ HG_CS_FIRST_ENCOUNTER,
    /* 1 */ HG_CS_GET_MASK,
    /* 2 */ HG_CS_SUBSEQUENT_ENCOUNTER,
    /* 3 */ HG_CS_SONG_OF_HEALING
} HgCsIndex;

RECOMP_HOOK_RETURN("EnHg_WaitForPlayerAction")
void EnHg_ForceGibdoMaskDrop(EnHg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    // If the player has gone upstairs this interaction will not trigger
    if ((this->actor.colChkInfo.health != 1) || (fabsf(this->actor.playerHeightRel) >= 80.0f)) {
        return;
    }
    
    if (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) {
        if ((play->msgCtx.lastPlayedSong == OCARINA_SONG_HEALING) && (GET_PLAYER_FORM == PLAYER_FORM_HUMAN)) {
            this->csIdIndex = HG_CS_GET_MASK;
        }
    }
}