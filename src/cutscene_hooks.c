#include "modding.h"
#include "global.h"
#include "z64shrink_window.h"

#include "apcommon.h"

#define SHRT_MAX 32767.0f

extern CutsceneEntry sGlobalCutsceneList[];

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

extern CutsceneEntry* sSceneCutsceneList;
extern s16 sSceneCutsceneCount;
extern u8 sWaitingCutsceneList[16];
extern u8 sNextCutsceneList[16];

CutsceneEntry* CutsceneManager_GetCutsceneEntryImpl(s16 csId);
s16 CutsceneManager_SetHudVisibility(s16 csHudVisibility);

// this sucks
void memcpy(u8* __dest, u8* __src, size_t __n) {
    size_t i;
    for (i = 0; i < __n; ++i) {
        __dest[i] = __src[i];
    }
}

RECOMP_PATCH s16 CutsceneManager_Start(s16 csId, Actor* actor) {
    CutsceneEntry* csEntry;
    Camera* subCam;
    Camera* retCam;
    s32 csType = 0;
    s16 actor_id = -1;

    if ((csId <= CS_ID_NONE) || (sCutsceneMgr.csId != CS_ID_NONE)) {
        return csId;
    }

    sCutsceneMgr.startMethod = CS_START_0;
    csEntry = CutsceneManager_GetCutsceneEntryImpl(csId);

    ShrinkWindow_Letterbox_SetSizeTarget(csEntry->letterboxSize);
    CutsceneManager_SetHudVisibility(csEntry->hudVisibility);

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

    if (csType != 0) {
        sCutsceneMgr.retCamId = Play_GetActiveCamId(sCutsceneMgr.play);
        sCutsceneMgr.subCamId = Play_CreateSubCamera(sCutsceneMgr.play);

        subCam = Play_GetCamera(sCutsceneMgr.play, sCutsceneMgr.subCamId);
        retCam = Play_GetCamera(sCutsceneMgr.play, sCutsceneMgr.retCamId);

        if ((retCam->setting == CAM_SET_START0) || (retCam->setting == CAM_SET_START2) ||
            (retCam->setting == CAM_SET_START1)) {
            if (CutsceneManager_FindEntranceCsId() != csId) {
                func_800E0348(retCam);
            } else {
                Camera_UnsetStateFlag(retCam, CAM_STATE_2);
            }
        }

        memcpy((u8*) subCam, (u8*) retCam, sizeof(Camera));
        subCam->camId = sCutsceneMgr.subCamId;
        Camera_UnsetStateFlag(subCam, CAM_STATE_6 | CAM_STATE_0);

        Play_ChangeCameraStatus(sCutsceneMgr.play, sCutsceneMgr.retCamId, CAM_STATUS_WAIT);
        Play_ChangeCameraStatus(sCutsceneMgr.play, sCutsceneMgr.subCamId, CAM_STATUS_ACTIVE);

        subCam->target = sCutsceneMgr.targetActor = actor;
        subCam->behaviorFlags = 0;

        if (csType == 1) {
            Camera_ChangeSetting(subCam, CAM_SET_FREE0);
            Cutscene_StartScripted(sCutsceneMgr.play, csEntry->scriptIndex);
            sCutsceneMgr.length = csEntry->length;
        } else {
            if (csEntry->csCamId != CS_CAM_ID_NONE) {
                Camera_ChangeActorCsCamIndex(subCam, csEntry->csCamId);
            } else {
                Camera_ChangeSetting(subCam, CAM_SET_FREE0);
            }
            sCutsceneMgr.length = csEntry->length;
        }
    }
    sCutsceneMgr.csId = csId;

    return csId;
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

RECOMP_PATCH void CutsceneManager_Queue(s16 csId) {
    //recomp_printf("queueing csId: 0x%04X\n", csId);
    if (csId > CS_ID_NONE) {
        sWaitingCutsceneList[csId >> 3] |= 1 << (csId & 7);
    }
}