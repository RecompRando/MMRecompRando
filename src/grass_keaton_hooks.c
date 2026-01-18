#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_En_Kusa2/z_en_kusa2.h"

#define LOCATION_KEATON_GRASS (AP_PREFIX_GRASS_KEATON | (play->sceneId << 8) | randoGetLoadedActorNumInSameRoom(play, thisx))
#define LOCATION_KEATON_GRASS_INCREMENTAL (AP_PREFIX_GRASS_KEATON | (play->sceneId << 8) | kusa2->unk_1BC)

EnKusa2* keatonGrass;

void grab_grass_texture();

RECOMP_HOOK("EnKusa2_Init")
void OnEnKusa2_Init(Actor* thisx, PlayState* play) {
    u32* keatonGrassLocation = z64recomp_get_extended_actor_data(thisx, actorLocationExtension);
    *keatonGrassLocation = LOCATION_KEATON_GRASS;
}

RECOMP_HOOK("func_80A5D7A4")
void OnEnKusa2_SetupDrawFuncs(EnKusa2* this) {
    grab_grass_texture();
}

// this could possibly be a patch, but seriously why would someone else edit this
RECOMP_PATCH void func_80A5BD14(EnKusa2* this, PlayState* play, s32 arg2) {
    static s32 D_80A5EB24[] = {
        ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN,
        ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_RED,
    };
    EnKusa2* kusa2 = this->unk_1C0;

    u32* keatonGrassLocation;
    if (kusa2 != NULL) {
        if (kusa2->unk_1BC > 8) {
            kusa2->unk_1BC = 8;
        }

        keatonGrassLocation = z64recomp_get_extended_actor_data(&this->actor, actorLocationExtension);
        u32 currentGrassLocation = *keatonGrassLocation;
        // if (incremental_keaton_grass_option) { // TODO: make an option
        //     currentGrassLocation = LOCATION_KEATON_GRASS_INCREMENTAL;
        // }

        if (rando_get_slotdata_u32("grasssanity") && !rando_location_is_checked(currentGrassLocation)) {
            Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, currentGrassLocation);
        } else {
            Item_DropCollectible(play, &this->actor.world.pos, D_80A5EB24[kusa2->unk_1BC]);
        }

        kusa2->unk_1BC += arg2;
    }
}