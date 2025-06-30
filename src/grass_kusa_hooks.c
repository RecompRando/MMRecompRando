#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_extensions.h"

#include "overlays/actors/ovl_En_Kusa/z_en_kusa.h"

ActorExtensionId kusaGrassExtension;
u32* extendedKusaGrassData;

extern s16 D_809366B0; // cur grass

u16 getCurrentGrotto(PlayState* play);

u32 EnKusa_CreateLocation(PlayState* play) {
    s16 sceneId = play->sceneId;
    
    // handle grottos
    if (sceneId == SCENE_KAKUSIANA) {
        sceneId = getCurrentGrotto(play);
    }

    return (0x120000 | (sceneId << 8) | D_809366B0);
}

// TODO: handle the different types of grass correctly (i.e. respawning)
RECOMP_HOOK("EnKusa_Init")
void OnEnKusa_Init(Actor* thisx, PlayState* play) {
    extendedKusaGrassData = z64recomp_get_extended_actor_data(thisx, kusaGrassExtension);
    *extendedKusaGrassData = EnKusa_CreateLocation(play);
    // recomp_printf("single grass: 0x%06X\n", *extendedKusaGrassData);
}

RECOMP_PATCH void EnKusa_DropCollectible(EnKusa* this, PlayState* play) {
    s32 collectible;
    s32 collectableParams;

    extendedKusaGrassData = z64recomp_get_extended_actor_data(&this->actor, kusaGrassExtension);
    if (!rando_location_is_checked(*extendedKusaGrassData)) {
        Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *extendedKusaGrassData);
        return;
    }

    if ((KUSA_GET_TYPE(&this->actor) == ENKUSA_TYPE_GRASS) || (KUSA_GET_TYPE(&this->actor) == ENKUSA_TYPE_BUSH)) {
        if (!KUSA_GET_PARAM_0C(&this->actor)) {
            Item_DropCollectibleRandom(play, NULL, &this->actor.world.pos,
                                       KUSA_GET_RAND_COLLECTIBLE_ID(&this->actor) * 0x10);
        }
    } else if (KUSA_GET_TYPE(&this->actor) == ENKUSA_TYPE_REGROWING_GRASS) {
        Item_DropCollectible(play, &this->actor.world.pos, 3);
    } else { // ENKUSA_TYPE_GRASS_2
        collectible = func_800A8150(KUSA_GET_PARAM_FC(&this->actor));
        if (collectible >= 0) {
            collectableParams = KUSA_GET_COLLECTIBLE_ID(&this->actor);
            Item_DropCollectible(play, &this->actor.world.pos, (collectableParams << 8) | collectible);
        }
    }
}