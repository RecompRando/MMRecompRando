#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_En_Kusa2/z_en_kusa2.h"

#define LOCATION_KEATON_GRASS (0x130000 | (play->sceneId << 8) | curGrass)
#define LOCATION_KEATON_GRASS_INCREMENTAL (0x130000 | (play->sceneId << 8) | kusa2->unk_1BC)

EnKusa2* keatonGrass;
ActorExtensionId keatonGrassExtension;
u32* extendedKeatonGrassData;

RECOMP_HOOK("EnKusa2_Init")
void OnEnKusa2_Init(Actor* thisx, PlayState* play) {
    EnKusa2* this = (EnKusa2*)thisx;

    static u8 curGrass;

    if (!ENKUSA2_GET_1(&this->actor)) { // spawner
        curGrass = 0;
    } else {
        extendedKeatonGrassData = z64recomp_get_extended_actor_data(thisx, keatonGrassExtension);
        *extendedKeatonGrassData = LOCATION_KEATON_GRASS;
        curGrass++;
    }
}

// this could possibly be a patch, but seriously why would someone else edit this
RECOMP_PATCH void func_80A5BD14(EnKusa2* this, PlayState* play, s32 arg2) {
    static s32 D_80A5EB24[] = {
        ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN,
        ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_RED,
    };
    EnKusa2* kusa2 = this->unk_1C0;

    if (kusa2 != NULL) {
        if (kusa2->unk_1BC > 8) {
            kusa2->unk_1BC = 8;
        }

        extendedKeatonGrassData = z64recomp_get_extended_actor_data(&this->actor, keatonGrassExtension);
        u32 currentGrassLocation = *extendedKeatonGrassData;
        // if (incremental_keaton_grass_option) { // TODO: make an option
        //     currentGrassLocation = LOCATION_KEATON_GRASS_INCREMENTAL;
        // }

        if (!rando_location_is_checked(currentGrassLocation)) {
            Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, currentGrassLocation);
        } else {
            Item_DropCollectible(play, &this->actor.world.pos, D_80A5EB24[kusa2->unk_1BC]);
        }

        kusa2->unk_1BC += arg2;
    }
}