#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_extensions.h"

#include "overlays/actors/ovl_En_Kusa2/z_en_kusa2.h"

#define LOCATION_KEATON_GRASS (0x120000 | (play->sceneId << 8) | D_80A5EAF0)

EnKusa2* keatonGrass;
ActorExtensionId keatonGrassExtension;
u32* extendedKeatonGrassData;

extern s16 D_80A5EAF0; // cur grass

RECOMP_HOOK("EnKusa2_Init")
void OnEnKusa2_Init(Actor* thisx, PlayState* play) {
    EnKusa2* this = (EnKusa2*)thisx;

    if (ENKUSA2_GET_1(&this->actor)) { // ignore the spawner
        extendedKeatonGrassData = z64recomp_get_extended_actor_data(thisx, keatonGrassExtension);
        *extendedKeatonGrassData = LOCATION_KEATON_GRASS;
        recomp_printf("keaton grass: 0x%06X\n", *extendedKeatonGrassData);
    }
}