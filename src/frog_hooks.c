#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"

#define LOCATION_FROG (0xFF0000 | this->frogIndex)

#define FROG_LIMB_MAX 0x18
#include "overlays/actors/ovl_En_Minifrog/z_en_minifrog.h"

EnMinifrog* sEnMinifrog;

extern u16 sIsFrogReturnedFlags_ovl_En_Minifrog[];

void EnMinifrog_SpawnDust(EnMinifrog* this, PlayState* play);
void EnMinifrog_ReturnFrogCutscene(EnMinifrog* this, PlayState* play);

RECOMP_HOOK("EnMinifrog_Init")
void OnEnMinifrog_Init(Actor* thisx, PlayState* play) {
    EnMinifrog* this = ((EnMinifrog*)thisx);

    this->frogIndex = (this->actor.params & 0xF);
    if (this->frogIndex >= 5) {
        this->frogIndex = FROG_YELLOW;
    }
    
    if (!EN_FROG_IS_RETURNED(&this->actor)) {
        if (!rando_location_is_checked(LOCATION_FROG)) {
            CLEAR_WEEKEVENTREG(sIsFrogReturnedFlags_ovl_En_Minifrog[this->frogIndex]);
        } else {
            SET_WEEKEVENTREG(sIsFrogReturnedFlags_ovl_En_Minifrog[this->frogIndex]);
        }
    } else {
        if (this->frogIndex != FROG_YELLOW && rando_has_item(LOCATION_FROG)) { // location and item share ids
            SET_WEEKEVENTREG(sIsFrogReturnedFlags_ovl_En_Minifrog[this->frogIndex]);
        } else {
            CLEAR_WEEKEVENTREG(sIsFrogReturnedFlags_ovl_En_Minifrog[this->frogIndex]);
        }
    
        if (this->frogIndex == FROG_YELLOW && !rando_has_item(LOCATION_FROG)) {
            this->actor.params &= ~0xF0; // kill yellow frog
        }
    }
}

void EnMinifrog_Dissapear(EnMinifrog* this, PlayState* play) {
    EnMinifrog_SpawnDust(this, play);
    SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 30, NA_SE_EN_NPC_FADEAWAY);
    if (this->actor.csId != CS_ID_NONE) {
        if (CutsceneManager_GetCurrentCsId() == this->actor.csId) {
            CutsceneManager_Stop(this->actor.csId);
        }
    }

    Actor_Kill(&this->actor);
}

void EnMinifrog_OfferItem(EnMinifrog* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnMinifrog_Dissapear;
    } else {
        Message_CloseTextbox(play);
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_FROG), LOCATION_FROG, 1000.0f, 1000.0f, true, true);
    }
}

RECOMP_HOOK("EnMinifrog_Idle")
void OnEnMinifrog_Idle(EnMinifrog* this, PlayState* play) {
    sEnMinifrog = this;
}

RECOMP_HOOK_RETURN("EnMinifrog_Idle")
void AfterEnMinifrog_Idle() {
    EnMinifrog* this = sEnMinifrog;
    PlayState* play = gPlay;

    if (this->actionFunc == EnMinifrog_ReturnFrogCutscene) {
        this->actionFunc = EnMinifrog_OfferItem;
        recomp_printf("frog location: 0x%06X\n", LOCATION_FROG);
    }
}