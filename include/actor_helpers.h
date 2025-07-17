#include "z64recomp_api.h"

extern ActorExtensionId item00Extension;
extern ActorExtensionId mureExtension;
extern ActorExtensionId mure2Extension;
extern ActorExtensionId kusaGrassExtension;
extern ActorExtensionId keatonGrassExtension;
extern ActorExtensionId potExtension;
extern ActorExtensionId potFlyingExtension;
extern ActorExtensionId potFlowerExtension;
extern ActorExtensionId wonderHitExtension;
extern ActorExtensionId wonderRupeeExtension;
extern ActorExtensionId rockExtension;
extern ActorExtensionId boulder2Extension;
extern ActorExtensionId snowballExtension;
extern ActorExtensionId bigSnowballExtension;
extern ActorExtensionId gamelupyExtension;
extern ActorExtensionId crateSmallExtension;
extern ActorExtensionId crateBigExtension;
extern ActorExtensionId barrelExtension;
extern ActorExtensionId icicleExtension;
extern ActorExtensionId beehiveExtension;
extern ActorExtensionId fairyExtension;

s32 randoGetLoadedActorNumInSameRoom(PlayState* play, Actor* actorIn);
s32 randoGetLoadedActorNumInSameRoomExtra(PlayState* play, Actor* actorIn, s16 extraId);
u16 getCurrentGrotto(PlayState* play);