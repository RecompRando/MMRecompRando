#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"

#include "overlays/actors/ovl_Door_Shutter/z_door_shutter.h"

// boss doors
DoorShutter* sDoorShutter;

RECOMP_HOOK("DoorShutter_Init")
void OnDoorShutter_Init(Actor* thisx, PlayState* play2) {
    sDoorShutter = ((DoorShutter*)thisx);
}

RECOMP_HOOK_RETURN("DoorShutter_Init")
void AfterDoorShutter_Init() {
    // only do things if entrance rando is enabled (bosses)
    if (!rando_get_slotdata_u32("entrance_rando")) return;
    
    DoorShutter* this = sDoorShutter;
    
    // boss door
    if (this->doorType == 5) {
        u32 dungeonNum = 0;
        switch (gPlay->sceneId) {
            case SCENE_MITURIN:
                dungeonNum = 0;
                break;
            case SCENE_HAKUGIN:
                dungeonNum = 1;
                break;
            case SCENE_SEA:
                dungeonNum = 2;
                break;
            case SCENE_INISIE_R:
                dungeonNum = 3;
                break;
            // skip boss rooms
            case SCENE_MITURIN_BS:
            case SCENE_HAKUGIN_BS:
            case SCENE_SEA_BS:
            case SCENE_INISIE_BS:
                return;
        }

        u32 er_placements[rando_get_slotdata_u32("er_placement_count")];
        u32 incomingPtr[2]; // not sure why this needs to be 2
        u32 er_result;
        
        rando_get_slotdata_raw_o32("er_placement_ids", er_placements);

        rando_access_slotdata_raw_array_o32(er_placements, dungeonNum + 4, incomingPtr);
        er_result = (rando_access_slotdata_raw_u32_o32(incomingPtr) - 4) & 0xF;

        this->slidingDoor.unk_15E = er_result + 1;
        if (er_result < 0 || er_result >= 4) { // failsafe if result is not a boss
            this->slidingDoor.unk_15E = 0;
        }
    }
}


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