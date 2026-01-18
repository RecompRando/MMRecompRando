#include "z64recomp_api.h"

extern ActorExtensionId actorLocationExtension;
extern ActorExtensionId actorDroppedExtension;

extern ActorExtensionId mureIndexExtension;
extern ActorExtensionId mure2IndexExtension;
extern ActorExtensionId wonderHitTimerExtension;

s32 randoGetLoadedActorNumInSameRoom(PlayState* play, Actor* actorIn);
s32 randoGetLoadedActorNumInSameRoomExtra(PlayState* play, Actor* actorIn, s16 extraId);
u16 getCurrentGrotto(PlayState* play);