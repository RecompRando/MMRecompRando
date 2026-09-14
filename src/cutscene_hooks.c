#include "modding.h"
#include "global.h"
#include "z64shrink_window.h"

#include "apcommon.h"

#define SHRT_MAX 32767.0f

extern ActorCutscene sGlobalCutsceneList[];

typedef enum {
    /* 0 */ CS_START_0,
    /* 1 */ CS_START_1,
    /* 2 */ CS_START_2
} ActorCutsceneStartMethod;

typedef struct {
    /* 0x00 */ s16 csId;
    /* 0x02 */ s16 length;
    /* 0x04 */ s16 endCsId;
    /* 0x06 */ s16 subCamId;
    /* 0x08 */ Actor* targetActor;
    /* 0x0C */ s32 startMethod;
    /* 0x10 */ PlayState* play;
    /* 0x14 */ s16 retCamId;
    /* 0x16 */ s16 isCameraStored;
} CutsceneManager; // size = 0x18

extern CutsceneManager sCutsceneMgr;

extern ActorCutscene* sSceneCutsceneList;
extern s16 sSceneCutsceneCount;
extern u8 sWaitingCutsceneList[16];
extern u8 sNextCutsceneList[16];

ActorCutscene* CutsceneManager_GetCutsceneEntryImpl(s16 csId);
s16 CutsceneManager_SetHudVisibility(s16 csHudVisibility);

// this sucks
void memcpy(u8* __dest, u8* __src, size_t __n) {
    size_t i;
    for (i = 0; i < __n; ++i) {
        __dest[i] = __src[i];
    }
}

// remind me to completely gut this for a better goal system
RECOMP_HOOK("CutsceneManager_Start")
void completeGoal(s16 csId, Actor* actor) {
    ActorCutscene* csEntry;
    s32 csType = 0;
    
    if ((csId <= CS_ID_NONE) || (sCutsceneMgr.csId != CS_ID_NONE)) {
        return;
    }

    csEntry = CutsceneManager_GetCutsceneEntryImpl(csId);

    if (csId == CS_ID_GLOBAL_END) {
        csType = 1;
    } else if (csEntry->scriptIndex != CS_SCRIPT_ID_NONE) {
        // scripted cutscene
        csType = 1;
    } else if ((csId != CS_ID_GLOBAL_DOOR) && (csId != CS_ID_GLOBAL_TALK)) {
        csType = 2;
    }
/*
    if (actor != NULL) {
        actor_id = actor->id;
    }

    // 0x80723BC4 is Oath to Order's csEntry address
    recomp_printf("starting cs with id 0x%04X, actor id 0x%04X, csType %d, csEntry address 0x%08X, scriptIndex 0x%04X, and csCamId 0x%04X\n", csId, actor_id, csType, csEntry, csEntry->scriptIndex, csEntry->csCamId);
*/
    // makeshift goal completion
    if (csId == 9 && csType == 1 && csEntry->scriptIndex == 0 && ((u32) csEntry) == 0x807137D4) {
        rando_complete_goal();
    }
}

RECOMP_PATCH s16 CutsceneManager_MarkNextCutscenes(void) {
    s16 bit;
    s32 i;
    s32 j;
    s32 count = 0;
    s16 csIdMax = CS_ID_NONE;
    s16 priorityMax = SHRT_MAX; // lower number means higher priority
    s16 csId;
    s16 priority;

    for (i = 0; i < ARRAY_COUNT(sNextCutsceneList); i++) {
        for (bit = 1, j = 0; j < 8; j++) {
            if (sWaitingCutsceneList[i] & bit) {
                csId = (i << 3) | j;
                priority = CutsceneManager_GetCutsceneEntryImpl(csId)->priority;

                if ((priority ^ 0) == -1) {
                    sNextCutsceneList[i] |= bit;
                } else if ((priority < priorityMax) && (priority > 0)) {
                    csIdMax = csId;
                    priorityMax = priority;
                }
                count++;
            }
            bit <<= 1;
        }
    }
    if (csIdMax != CS_ID_NONE) {
        //recomp_printf("marking csIdMax 0x%04X\n", csIdMax);
        sNextCutsceneList[csIdMax >> 3] |= 1 << (csIdMax & 7);
    }
    return count;
}