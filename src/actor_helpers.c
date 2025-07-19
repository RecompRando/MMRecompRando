#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "actor_helpers.h"

void registerActorExtensions() {
    item00Extension = z64recomp_extend_actor(ACTOR_EN_ITEM00, sizeof(u32));
    mureExtension = z64recomp_extend_actor(ACTOR_OBJ_MURE, sizeof(u32));
    mure2Extension = z64recomp_extend_actor(ACTOR_OBJ_MURE2, sizeof(u32));
    kusaGrassExtension = z64recomp_extend_actor(ACTOR_EN_KUSA, sizeof(u32));
    keatonGrassExtension = z64recomp_extend_actor(ACTOR_EN_KUSA2, sizeof(u32));
    potExtension = z64recomp_extend_actor(ACTOR_OBJ_TSUBO, sizeof(u32));
    potFlyingExtension = z64recomp_extend_actor(ACTOR_EN_TUBO_TRAP, sizeof(u32));
    potFlowerExtension = z64recomp_extend_actor(ACTOR_OBJ_FLOWERPOT, sizeof(u32));
    wonderHitExtension = z64recomp_extend_actor(ACTOR_EN_HIT_TAG, sizeof(u32));
    wonderRupeeExtension = z64recomp_extend_actor(ACTOR_EN_INVISIBLE_RUPPE, sizeof(u32));
    rockExtension = z64recomp_extend_actor(ACTOR_EN_ISHI, sizeof(u32));
    boulder2Extension = z64recomp_extend_actor(ACTOR_OBJ_HAMISHI, sizeof(u32));
    snowballExtension = z64recomp_extend_actor(ACTOR_OBJ_SNOWBALL2, sizeof(u32));
    bigSnowballExtension = z64recomp_extend_actor(ACTOR_OBJ_SNOWBALL, sizeof(u32));
    gamelupyExtension = z64recomp_extend_actor(ACTOR_EN_GAMELUPY, sizeof(u32));
    crateSmallExtension = z64recomp_extend_actor(ACTOR_OBJ_KIBAKO, sizeof(u32));
    crateBigExtension = z64recomp_extend_actor(ACTOR_OBJ_KIBAKO2, sizeof(u32));
    barrelExtension = z64recomp_extend_actor(ACTOR_OBJ_TARU, sizeof(u32));
    icicleExtension = z64recomp_extend_actor(ACTOR_BG_ICICLE, sizeof(u32));
    beehiveExtension = z64recomp_extend_actor(ACTOR_OBJ_COMB, sizeof(u32));
    fairyExtension = z64recomp_extend_actor(ACTOR_EN_ELF, sizeof(u32));
    butterflyExtension = z64recomp_extend_actor(ACTOR_EN_BUTTE, sizeof(u32));
}

s32 randoGetLoadedActorNumInSameRoom(PlayState* play, Actor* actorIn) {
    u8 actorCat = actorIn->category;
    s8 actorRoom = actorIn->room;
    s16 actorId = actorIn->id;

    Actor* actor = play->actorCtx.actorLists[actorCat].first;
    s32 actorIndex = 0;

    while (actor != NULL) {
        if (actor->id == actorId && actor->room == actorRoom) {
            if (actor == actorIn) {
                return actorIndex;
            }
            actorIndex++;
        }
        actor = actor->next;
    }

    return -1; // should never return -1 if used correctly
}

// note: this only works if both actors are in the same category
s32 randoGetLoadedActorNumInSameRoomExtra(PlayState* play, Actor* actorIn, s16 extraId) {
    u8 actorCat = actorIn->category;
    s8 actorRoom = actorIn->room;
    s16 actorId = actorIn->id;

    Actor* actor = play->actorCtx.actorLists[actorCat].first;
    s32 actorIndex = 0;

    while (actor != NULL) {
        if ((actor->id == actorId || actor->id == extraId) && actor->room == actorRoom) {
            if (actor == actorIn) {
                return actorIndex;
            }
            actorIndex++;
        }
        actor = actor->next;
    }

    return -1; // should never return -1 if used correctly
}

// returns a fake sceneId based on different parameters
u16 getCurrentGrotto(PlayState* play) {
    s8 entrance = (gSaveContext.save.entrance >> 4) & 0x1F;
    switch (entrance) {
        case 0x4: // chest grotto
            // 0x11 is max grotto entrance + 1, 0x1F is ENBOX_GET_CHEST_FLAG
            entrance = 0x11 + (gSaveContext.respawn[RESPAWN_MODE_UNK_3].data & 0x1F);
            break;
        case 0xA: // cows
            entrance = Entrance_GetSceneIdAbsolute(((void)0, gSaveContext.respawn[RESPAWN_MODE_UNK_3].entrance));
            break;
    }
    return 0x80 + entrance; // 0x80 is outside the range of real sceneIds
}

typedef struct {
    /* 0x00 */ PlayState* play;
    /* 0x04 */ Actor* actor;
    /* 0x08 */ u32 requiredActorFlag;
    /* 0x0C */ u32 canFreezeCategory;
    /* 0x10 */ Actor* talkActor;
    /* 0x14 */ Player* player;
    /* 0x18 */ u32 unk_18; // Bitmask of actor flags. The actor will only have main called if it has at least 1
                           // flag set that matches this bitmask
} UpdateActor_Params;      // size = 0x1C

RECOMP_HOOK("Actor_UpdateActor")
void FreezeActors(UpdateActor_Params* params) {
    PlayState* play = params->play;
    Actor* actor = params->actor;
    Player* player = GET_PLAYER(play);

    // force actors to freeze when grabbing items w/ text
    if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
        if (actor->id == ACTOR_OBJ_BEAN ||          // magic bean plant
            actor->id == ACTOR_EN_KUSA2 ||          // keaton grass
            actor->id == ACTOR_OBJ_LUPYGAMELIFT) {  // deku playground elevator
            // PLAYER_STATE1_20000000 is used when grabbing items
            params->canFreezeCategory |= PLAYER_STATE1_20000000 & player->stateFlags1;
        }
    }
}