#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "actor_helpers.h"

void registerActorExtensions() {
    item00Extension = z64recomp_extend_actor(ACTOR_EN_ITEM00, sizeof(u32));
    kusaGrassExtension = z64recomp_extend_actor(ACTOR_EN_KUSA, sizeof(u32));
    keatonGrassExtension = z64recomp_extend_actor(ACTOR_EN_KUSA2, sizeof(u32));
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