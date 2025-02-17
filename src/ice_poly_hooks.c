#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct ObjIcePoly;

typedef void (*ObjIcePolyActionFunc)(struct ObjIcePoly*, PlayState*);

typedef struct ObjIcePoly {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ObjIcePolyActionFunc actionFunc;
    /* 0x148 */ u8 unk_148;
    /* 0x149 */ u8 switchFlag;
    /* 0x14A */ s16 unk_14A;
    /* 0x14C */ ColliderCylinder colliders1[2];
    /* 0x1E4 */ ColliderCylinder colliders2[2];
} ObjIcePoly; // size = 0x27C

void func_80931828(ObjIcePoly* this, PlayState* play);
void func_80931EEC(ObjIcePoly* this, PlayState* play);

RECOMP_PATCH void func_80931E58(ObjIcePoly* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->actor.csId)) {
        // CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
        Player_SetCsActionWithHaltedActors(play, NULL, PLAYER_CSACTION_END);
        if (this->unk_14A == 1) {
            func_80931828(this, play);
            Actor_Kill(&this->actor);
            return;
        }

        this->unk_14A = 40;
        Actor_PlaySfx(&this->actor, NA_SE_EV_ICE_MELT);
        this->actionFunc = func_80931EEC;
    } else {
        CutsceneManager_Queue(this->actor.csId);
    }
}