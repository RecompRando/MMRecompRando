#include "modding.h"
#include "global.h"

#define BG_UMAJUMP_GET_PATH_INDEX(thisx) (((thisx)->params >> 8) & 0xFF)
#define BG_UMAJUMP_GET_TYPE(thisx) ((thisx)->params & 0xFF)

typedef enum {
    /* 1 */ BG_UMAJUMP_TYPE_1 = 1,
    /* 2 */ BG_UMAJUMP_TYPE_2,
    /* 3 */ BG_UMAJUMP_TYPE_3,
    /* 4 */ BG_UMAJUMP_TYPE_4,
    /* 5 */ BG_UMAJUMP_TYPE_5,
    /* 6 */ BG_UMAJUMP_TYPE_6
} BgUmaJumpType;

struct BgUmajump;

typedef void (*BgUmajumpActionFunc)(struct BgUmajump*, PlayState*);

typedef struct BgUmajump {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ BgUmajumpActionFunc actionFunc;
    /* 0x160 */ union {
                    s32 objectSlot;
                    s32 rotationTimer; // y rotation
                    s32 pathIndex;
                };
    /* 0x164 */ s32 hasSoundPlayed;
    /* 0x168 */ Actor* horse;
} BgUmajump; // size = 0x16C

#define THIS ((BgUmajump*)thisx)

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(cullingVolumeScale, 1200, ICHAIN_CONTINUE),
    ICHAIN_F32(cullingVolumeDownward, 300, ICHAIN_CONTINUE),
    ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP),
};

RECOMP_PATCH void BgUmajump_Init(Actor* thisx, PlayState* play) {
    BgUmajump* this = THIS;

    Actor_ProcessInitChain(thisx, sInitChain);

    this->actionFunc = NULL;
    this->horse = NULL;
    this->hasSoundPlayed = false;

    DynaPolyActor_Init(&this->dyna, 0);

    this->pathIndex = BG_UMAJUMP_GET_PATH_INDEX(thisx);
    thisx->params = BG_UMAJUMP_GET_TYPE(thisx);

    if (thisx->params == BG_UMAJUMP_TYPE_2) {
        // if ((play->sceneId == SCENE_F01) && !CHECK_WEEKEVENTREG(WEEKEVENTREG_89_20) &&
        //     !CHECK_QUEST_ITEM(QUEST_SONG_EPONA) && (thisx->csId != CS_ID_NONE)) {
        //     this->actionFunc = BgUmajump_CheckDistance;
        //     thisx->update = func_8091A5A0;
        //     thisx->flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        //     this->horse = SubS_FindActor(play, this->horse, ACTORCAT_BG, ACTOR_EN_HORSE);
        // } else {
        thisx->update = Actor_Noop;
        // }
    } else {
        this->objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_UMAJUMP);

        if (this->objectSlot <= OBJECT_SLOT_NONE) {
            Actor_Kill(thisx);
        }

        if ((thisx->params == BG_UMAJUMP_TYPE_3) && CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
            Actor_Spawn(&play->actorCtx, play, ACTOR_EN_KANBAN, thisx->world.pos.x, thisx->world.pos.y,
                        thisx->world.pos.z, thisx->shape.rot.x, thisx->shape.rot.y, thisx->shape.rot.z, 0x3E);
            Actor_Kill(thisx);
        }
    }
}