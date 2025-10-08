#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"

#include "overlays/actors/ovl_Door_Shutter/z_door_shutter.h"

// secret shrine doors
s16 doorShutterOriginalHealthCapacity;

RECOMP_HOOK("func_808A1090")
void DoorShutter_SkipShrineDoor(DoorShutter* this, PlayState* play) {
    doorShutterOriginalHealthCapacity = gSaveContext.save.saveInfo.playerData.healthCapacity;
    
    if (this->doorType == 6) {
        gSaveContext.save.saveInfo.playerData.healthCapacity = 0x140;
    }
}

RECOMP_HOOK_RETURN("func_808A1090")
void DoorShutter_FixHealthAfterSkippingShrineDoor() {
    gSaveContext.save.saveInfo.playerData.healthCapacity = doorShutterOriginalHealthCapacity;
}