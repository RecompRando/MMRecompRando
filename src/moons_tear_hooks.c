#include "modding.h"
#include "global.h"
#include "overlays/actors/ovl_Obj_Moon_Stone/z_obj_moon_stone.h"

#include "apcommon.h"

#define THIS ((ObjMoonStone*)thisx)

#define LOCATION_MOONS_TEAR GI_MOONS_TEAR

typedef void (*ActorFunc)(struct Actor* this, struct PlayState* play);

s16 tearTrueGI;

static bool tearObjectStatic;
static bool tearObjectLoading;
static bool tearObjectLoaded;
static OSMesgQueue tearObjectLoadQueue;
static void* tearObjectSegment;

extern AnimatedMaterial gGiMoonsTearTexAnim[];
extern Gfx gGiMoonsTearItemDL[];
extern Gfx gGiMoonsTearGlowDL[];

void* ZeldaArena_Malloc(size_t size);
void func_80C0662C(ObjMoonStone* this);
void func_80C0673C(ObjMoonStone* this);
void func_80C0685C(ObjMoonStone* this);

RECOMP_PATCH void ObjMoonStone_Init(Actor* thisx, PlayState* play) {
    ObjMoonStone* this = THIS;

    tearTrueGI = rando_get_item_id(GI_MOONS_TEAR);
    tearObjectSegment = ZeldaArena_Malloc(0x2000);
    tearObjectStatic = false;
    tearObjectLoading = false;
    tearObjectLoaded = false;

    Actor_SetScale(&this->actor, 0.3f);
    this->unk194 = (this->actor.params & 0xF000) >> 0xC;
    this->actor.attentionRangeType = ATTENTION_RANGE_0;
    this->actor.shape.yOffset = 25.0f;
    this->actor.focus.pos.y += 10.0f;
    if (this->unk194 == 0) {
        this->actor.colChkInfo.health = 0;
        this->actor.flags |= (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY);
        func_80C0662C(this);
    } else if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_74_40)) {
        if (CHECK_WEEKEVENTREG(WEEKEVENTREG_74_80)) {
            Actor_Spawn(&play->actorCtx, play, 1, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, 0, 0, -1);
        }
        this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
        func_80C0673C(this);
    } else {
        Actor_Kill(&this->actor);
    }
}

void ObjMoonStone_WaitForObject(ObjMoonStone* this, PlayState* play) {
    s16 getItemId = rando_get_item_id(LOCATION_MOONS_TEAR);
    s16 objectSlot = Object_GetSlot(&play->objectCtx, getObjectId(getItemId));

    if (isAP(getItemId)) {
        tearObjectStatic = true;
        tearObjectLoaded = true;
        tearTrueGI = getItemId;
    } else if (!tearObjectLoaded && !tearObjectLoading && Object_IsLoaded(&play->objectCtx, objectSlot)) {
        this->actor.objectSlot = objectSlot;
        Actor_SetObjectDependency(play, &this->actor);
        tearObjectStatic = true;
        tearObjectLoaded = true;
        tearTrueGI = getItemId;
    } else if (!tearObjectLoading && !tearObjectLoaded) {
        loadObject(play, &tearObjectSegment, &tearObjectLoadQueue, getObjectId(getItemId));
        tearObjectLoading = true;
    } else if (osRecvMesg(&tearObjectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
        tearObjectLoading = false;
        tearObjectLoaded = true;
        tearTrueGI = getItemId;
    }
}

RECOMP_PATCH void ObjMoonStone_Update(Actor* thisx, PlayState* play) {
    ObjMoonStone* this = THIS;
    Player* player = GET_PLAYER(play);

    if (!tearObjectLoaded) {
        ObjMoonStone_WaitForObject(this, play);
    }

    if (!(player->stateFlags1 & (PLAYER_STATE1_2 | PLAYER_STATE1_DEAD | PLAYER_STATE1_200 | PLAYER_STATE1_10000000))) {
        this->actionFunc(this, play);
    }
}

RECOMP_PATCH void ObjMoonStone_Draw(Actor* thisx, PlayState* play) {
    s16 getItemId = rando_get_item_id(LOCATION_MOONS_TEAR);
    GraphicsContext* gfxCtx = play->state.gfxCtx;

    OPEN_DISPS(play->state.gfxCtx);

    if (tearObjectLoaded) {
        if (((u32) tearTrueGI) != rando_get_item_id(LOCATION_MOONS_TEAR)) {
            tearObjectLoading = false;
            tearObjectLoaded = false;
            tearObjectStatic = false;
            tearTrueGI = rando_get_item_id(LOCATION_MOONS_TEAR);
            return;
        }
        if (tearObjectStatic) {
            GetItem_Draw(play, getGid(getItemId));
        } else {
            GetItem_DrawDynamic(play, tearObjectSegment, getGid(getItemId));
        }
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

RECOMP_PATCH void func_80C06768(ObjMoonStone* this, PlayState* play) {
    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_74_80)) {
        if (this->actor.draw == NULL) {
            this->actor.draw = ObjMoonStone_Draw;
            Actor_Spawn(&play->actorCtx, play, 1, this->actor.world.pos.x, this->actor.world.pos.y,
                        this->actor.world.pos.z, 0, 0, 0, -1);
        }
    }
    if (this->actor.draw) {
        if (Actor_HasParent(&this->actor, play)) {
            tearObjectLoading = false;
            tearObjectLoaded = false;
            tearObjectStatic = false;
            this->actor.parent = NULL;
            this->actor.draw = NULL;
            func_80C0685C(this);
        } else if (this->actor.xzDistToPlayer < 25.0f) {
            Actor_OfferGetItem(&this->actor, play, GI_MOONS_TEAR, 100.0f, 30.0f);
        }
    }
}