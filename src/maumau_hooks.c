#include "modding.h"
#include "global.h"

#include "recomputils.h"

struct EnAob01;

typedef void (*EnAob01ActionFunc)(struct EnAob01*, PlayState*);

// This needs to be kept in sync with the number of weekEventRegs used to store the text ID offsets for
// the race dogs. In particular, there needs to be one weekEventReg for every two dogs. This needs to be
// an even number to ensure that all text ID offsets are created (see EnAob01_InitializeDogTextOffsets).
#define RACEDOG_COUNT 14

#define MAMAMU_YAN_LIMB_MAX 0x10

typedef struct EnAob01 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnAob01ActionFunc actionFunc;
    /* 0x148 */ SkelAnime skelAnime;
    /* 0x18C */ ColliderCylinder collider;
    /* 0x1D8 */ Path* dogPaths[RACEDOG_COUNT];
    /* 0x210 */ u16 textId;
    /* 0x212 */ Vec3s jointTable[MAMAMU_YAN_LIMB_MAX];
    /* 0x272 */ Vec3s morphTable[MAMAMU_YAN_LIMB_MAX];
    /* 0x2D2 */ u16 stateFlags;
    /* 0x2D4 */ Vec3s trackTarget;
    /* 0x2DA */ Vec3s headRot;
    /* 0x2E0 */ Vec3s torsoRot;
    /* 0x2E6 */ Vec3s prevTrackTarget;
    /* 0x2EC */ Vec3s prevHeadRot;
    /* 0x2F2 */ Vec3s prevTorsoRot;
    /* 0x2F8 */ s16 fidgetTableY[MAMAMU_YAN_LIMB_MAX];
    /* 0x318 */ s16 fidgetTableZ[MAMAMU_YAN_LIMB_MAX];
    /* 0x338 */ UNK_TYPE1 unk338[0xB6];
    /* 0x3EE */ s16 eyeIndex;
    /* 0x3F0 */ s16 blinkTimer;
    /* 0x3F4 */ Actor* dogToFollow;
    /* 0x3F8 */ Actor* dogs[RACEDOG_COUNT];
    /* 0x430 */ s16 csId;
    /* 0x432 */ s16 selectedDogIndex;
    /* 0x434 */ s32 rupeesBet; // Also used for determining how many rupees to give to the player if they place 1st-5th in the race.
    /* 0x438 */ UNK_TYPE1 unk438[4];
    /* 0x43C */ s32 animIndex;
    /* 0x440 */ s16 forceRaceEndTimer;
} EnAob01; // size = 0x444

#define DOG_LIMB_MAX 0x0D

struct EnRacedog;

typedef void (*EnRacedogActionFunc)(struct EnRacedog*, PlayState*);

typedef struct EnRacedog {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnRacedogActionFunc actionFunc;
    /* 0x148 */ UNK_TYPE1 unk_148[0x4];
    /* 0x14C */ SkelAnime skelAnime;
    /* 0x190 */ ColliderCylinder collider;
    /* 0x1DC */ UNK_TYPE1 unk_1DC[0x4];
    /* 0x1E0 */ Path* path;
    /* 0x1E4 */ UNK_TYPE1 unk_1E4[0x4];
    /* 0x1E8 */ s32 currentPoint;
    /* 0x1EC */ Vec3s jointTable[DOG_LIMB_MAX];
    /* 0x23A */ Vec3s morphTable[DOG_LIMB_MAX];
    /* 0x288 */ s16 selectionArrowTimer;
    /* 0x28A */ s16 raceStartTimer;
    /* 0x28C */ s16 extraTimeBeforeRaceStart;
    /* 0x28A */ UNK_TYPE1 unk_28E[0x2];
    /* 0x290 */ s16 index;
    /* 0x292 */ s16 selectedDogIndex;
    /* 0x294 */ UNK_TYPE1 unk_294[0x4];
    /* 0x298 */ f32 targetSpeed;
    /* 0x29C */ s16 raceStatus;
    /* 0x2A0 */ Vec3f prevRot;
    /* 0x2AC */ Vec3f curRot;
    /* 0x2B8 */ s32 pointForCurrentTargetSpeed;
    /* 0x2BC */ s32 selectionArrowGreenPrimColor;
    /* 0x2C0 */ s32 selectionArrowGreenEnvColor;
    /* 0x2C4 */ f32 selectionArrowScale;
} EnRacedog; // size = 0x2C8

typedef enum {
    /* 0 */ RACEDOG_RACE_STATUS_BEFORE_POINT_9,
    /* 1 */ RACEDOG_RACE_STATUS_BETWEEN_POINTS_9_AND_11,
    /* 2 */ RACEDOG_RACE_STATUS_AFTER_POINT_11,
    /* 3 */ RACEDOG_RACE_STATUS_FINISHED
} RacedogRaceStatus;

/**
 * Returns true if the race should be ended, which can happen in either of the following circumstances:
 * - All `RACEDOG_COUNT` dogs have finished the race.
 * - At least 10 dogs have finished the race, and at least 500 frames have passed since the 10th place
 *   dog finished. This is controlled by the forceRaceEndTimer, which is set in EnAob01_Init.
 */
RECOMP_PATCH s32 EnAob01_Race_IsRaceOver(EnAob01* this, PlayState* play) {
    Actor* npc = play->actorCtx.actorLists[ACTORCAT_NPC].first;
    s16 finishedCount = 0;

    while (npc != NULL) {
        if ((npc->id == ACTOR_EN_RACEDOG) && (((EnRacedog*)npc)->raceStatus == RACEDOG_RACE_STATUS_FINISHED)) {
            finishedCount++;
            // @rando finish dog race after selected dog wins
            if (((EnRacedog*)npc)->index == ((EnRacedog*)npc)->selectedDogIndex) {
                return true;
            }
        }
        npc = npc->next;
    }

    if (finishedCount >= RACEDOG_COUNT) {
        return true;
    }

    if ((finishedCount >= 10) && (DECR(this->forceRaceEndTimer) == 0)) {
        return true;
    }

    return false;
}