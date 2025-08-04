#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

// "scene" 0xFF lol
#define LOCATION_RUPEE_CROW(index) (0x17FF00 | index)

#define OBJECT_CROW_LIMB_MAX 0x09
#include "overlays/actors/ovl_En_Ruppecrow/z_en_ruppecrow.h"
#include "overlays/actors/ovl_En_Sc_Ruppe/z_en_sc_ruppe.h"

EnRuppecrow* sEnRuppecrow;

s32 EnRuppecrow_CanSpawnBlueRupees(PlayState*);

void Item_RandoCollectibleDraw(Actor* thisx, PlayState* play);
void Item_RandoCollectibleActionFunc(EnItem00* this, PlayState* play);

RECOMP_HOOK("EnRuppecrow_SpawnRupee")
void OnEnRuppecrow_SpawnRupee(EnRuppecrow* this, PlayState* play) {
    sEnRuppecrow = this;
}

RECOMP_HOOK_RETURN("EnRuppecrow_SpawnRupee")
void AfterEnRuppecrow_SpawnRupee() {
    EnRuppecrow* this = sEnRuppecrow;
    PlayState* play = gPlay;

    Player* player = GET_PLAYER(play);
    f32 xOffset;
    s16 rupeeIndex = this->rupeeIndex - 1;
    EnItem00* rupee = this->rupees[rupeeIndex];

    u32* extendedItem00Data;
    extendedItem00Data = z64recomp_get_extended_actor_data(&rupee->actor, item00Extension);
    *extendedItem00Data = LOCATION_RUPEE_CROW(rupeeIndex);

    if (EnRuppecrow_CanSpawnBlueRupees(play) && (rupeeIndex % 5) == 4 && rupeeIndex != 19) {
        *extendedItem00Data = LOCATION_RUPEE_CROW(rupeeIndex + 20); // arbitrary +20
    }

    rupee->actor.params = ITEM00_APITEM;
    rupee->actor.draw = Item_RandoCollectibleDraw;
    rupee->getItemId = rando_get_item_id(*extendedItem00Data);
    rupee->actionFunc = Item_RandoCollectibleActionFunc;
}