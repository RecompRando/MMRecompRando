#include "z64recomp_api.h"

extern ActorExtensionId item00Extension;
extern ActorExtensionId kusaGrassExtension;
extern ActorExtensionId keatonGrassExtension;
extern ActorExtensionId potExtension;
extern ActorExtensionId potFlyingExtension;
extern ActorExtensionId potFlowerExtension;
extern ActorExtensionId wonderHitExtension;
extern ActorExtensionId wonderRupeeExtension;

s32 randoGetLoadedActorNumInSameRoom(PlayState* play, Actor* actorIn);
s32 randoGetLoadedActorNumInSameRoomExtra(PlayState* play, Actor* actorIn, s16 extraId);
u16 getCurrentGrotto(PlayState* play);