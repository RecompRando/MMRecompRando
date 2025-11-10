#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Mure/z_obj_mure.h"

#define LOCATION_MURE(i) (AP_PREFIX_MURE_GRASS | (play->sceneId << 8) | ((play->roomCtx.curRoom.num + *mureIndex) << 4) | i)
#define LOCATION_MURE2(i) (AP_PREFIX_MURE_GRASS | (play->sceneId << 8) | ((play->roomCtx.curRoom.num + *mure2Index) << 4) | i)
#define LOCATION_MURE2_ROCK(i) (AP_PREFIX_MURE_ROCKS | (play->sceneId << 8) | ((play->roomCtx.curRoom.num + *mure2Index) << 4) | i)

ActorExtensionId mureIndexExtension;
u32* mureIndex;
ActorExtensionId mure2IndexExtension;
u32* mure2Index;

extern ActorExtensionId kusaGrassLocationExtension;
extern u32* kusaLocation;
extern ActorExtensionId rockLocationExtension;
extern u32* rockLocation;

u32 EnKusa_CreateLocation(PlayState* play, Actor* actor);

// probably not the best way to do this
RECOMP_HOOK("ObjMure_Init")
void OnObjMure_Init(Actor* thisx, PlayState* play) {
    mureIndex = z64recomp_get_extended_actor_data(thisx, mureIndexExtension);
    *mureIndex = randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_MURE2);
}

s32 ObjMure_GetMaxChildSpawns(ObjMure* this);

ObjMure* sObjMure;

RECOMP_HOOK("ObjMure_SpawnActors")
void OnObjMure_SpawnActors(ObjMure* this, PlayState* play) {
    sObjMure = this;
}

RECOMP_HOOK_RETURN("ObjMure_SpawnActors")
void AfterObjMure_SpawnActors() {
    ObjMure* this = sObjMure;
    PlayState* play = gPlay;

    if (this->svNum != 0 && this->svNum != 1) {
        return;
    }

    s32 maxChildren = ObjMure_GetMaxChildSpawns(this);
    mureIndex = z64recomp_get_extended_actor_data(&this->actor, mureIndexExtension);

    for (s32 i = 0; i < maxChildren; i++) {
        if (this->children[i] == NULL) {
            continue;
        }
        if (this->children[i]->id == ACTOR_EN_KUSA) {
            kusaLocation = z64recomp_get_extended_actor_data(this->children[i], kusaGrassLocationExtension);
            *kusaLocation = LOCATION_MURE(i);
        }
    }
}

// again, probably not the best way to do this
RECOMP_HOOK("ObjMure2_Init")
void OnObjMure2_Init(Actor* thisx, PlayState* play) {
    mure2Index = z64recomp_get_extended_actor_data(thisx, mure2IndexExtension);
    *mure2Index = randoGetLoadedActorNumInSameRoomExtra(play, thisx, ACTOR_OBJ_MURE);
}

#define OBJ_MURE2_CHILD_COUNT_BUSH_RING 9
#define OBJ_MURE2_CHILD_COUNT_BUSH_SCATTERED 12
#define OBJ_MURE2_CHILD_COUNT_ROCK_RING 8

typedef enum Mure2ChildType {
    /* 0 */ OBJMURE2_CHILDTYPE_BUSH_RING,
    /* 1 */ OBJMURE2_CHILDTYPE_BUSH_SCATTERED,
    /* 2 */ OBJMURE2_CHILDTYPE_ROCK_RING,
    /* 3 */ OBJMURE2_CHILDTYPE_MAX
} Mure2ChildType;

static s16 sChildCounts[OBJMURE2_CHILDTYPE_MAX] = {
    OBJ_MURE2_CHILD_COUNT_BUSH_RING,      // OBJMURE2_CHILDTYPE_BUSH_RING
    OBJ_MURE2_CHILD_COUNT_BUSH_SCATTERED, // OBJMURE2_CHILDTYPE_BUSH_SCATTERED
    OBJ_MURE2_CHILD_COUNT_ROCK_RING,      // OBJMURE2_CHILDTYPE_ROCK_RING
};

struct ObjMure2;

typedef void (*ObjMure2ActionFunc)(struct ObjMure2*, PlayState*);

#define OBJ_MURE2_CHILD_MAX 12
#define OBJ_MURE2_GET_CHILD_TYPE(thisx) ((thisx)->params & 3)
#define OBJ_MURE2_GET_CHILD_UPPER_PARAMS(thisx) ((((thisx)->params) >> 8) & 0x1F)

typedef struct ObjMure2 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ObjMure2ActionFunc actionFunc;
    /* 0x148 */ Actor* actors[OBJ_MURE2_CHILD_MAX];
    /* 0x178 */ u16 spawnFlags;
    /* 0x17C */ f32 rangeMultiplier;
} ObjMure2; // size = 0x180

ObjMure2* sObjMure2;

// ObjMure2_SpawnChildren
RECOMP_HOOK("func_8096104C")
void OnObjMure2_SpawnChildren(ObjMure2* this, PlayState* play) {
    sObjMure2 = this;
}

RECOMP_HOOK_RETURN("func_8096104C")
void AfterObjMure2_SpawnChildren() {
    ObjMure2* this = sObjMure2;
    PlayState* play = gPlay;
    Mure2ChildType childType = OBJ_MURE2_GET_CHILD_TYPE(&this->actor);
    mure2Index = z64recomp_get_extended_actor_data(&this->actor, mure2IndexExtension);

    for (s32 i = 0; i < sChildCounts[childType]; i++) {
        if (this->actors[i] == NULL) {
            continue;
        }
        switch (childType) {
            case OBJMURE2_CHILDTYPE_BUSH_RING:
            case OBJMURE2_CHILDTYPE_BUSH_SCATTERED:
                kusaLocation = z64recomp_get_extended_actor_data(this->actors[i], kusaGrassLocationExtension);
                *kusaLocation = LOCATION_MURE2(i);
                break;
            case OBJMURE2_CHILDTYPE_ROCK_RING:
                rockLocation = z64recomp_get_extended_actor_data(this->actors[i], rockLocationExtension);
                *rockLocation = LOCATION_MURE2_ROCK(i);
                break;
            default:
                break;
        }
    }
}