#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "actor_extensions.h"

void registerActorExtensions() {
    keatonGrassExtension = z64recomp_extend_actor(ACTOR_EN_KUSA2, sizeof(u32));
    item00Extension = z64recomp_extend_actor(ACTOR_EN_ITEM00, sizeof(u32));
}