#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnZoraegg;

typedef void (*EnZoraeggActionFunc)(struct EnZoraegg*, PlayState*);

#define ZORA_BABY_LIMB_MAX 0x07

typedef struct EnZoraegg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[ZORA_BABY_LIMB_MAX];
    /* 0x1B2 */ Vec3s morphTable[ZORA_BABY_LIMB_MAX];
    /* 0x1DC */ Actor* unk_1DC;
    /* 0x1E0 */ f32 unk_1E0;
    /* 0x1E4 */ f32 unk_1E4;
    /* 0x1E8 */ s16 unk_1E8;
    /* 0x1EA */ u16 unk_1EA;
    /* 0x1EC */ u8 unk_1EC;
    /* 0x1ED */ u8 unk_1ED;
    /* 0x1EE */ u8 unk_1EE;
    /* 0x1EF */ u8 unk_1EF;
    /* 0x1F0 */ u16 cueType;
    /* 0x1F2 */ s16 eyeIndex;
    /* 0x1F4 */ s16 blinkTimer;
    /* 0x1F8 */ EnZoraeggActionFunc actionFunc;
} EnZoraegg; // size = 0x1FC

void func_80B322BC(EnZoraegg* this, PlayState* play);
void func_80B319D0(PlayState* play, s32 arg1);

RECOMP_PATCH void func_80B321D0(EnZoraegg* this, PlayState* play) {
    if (CutsceneManager_GetCurrentCsId() != this->actor.csId) {
        this->actionFunc = func_80B322BC;
        // func_80B319D0(play, func_80B319A8(play) + 1);
        func_80B319D0(play, 7);
    }
}