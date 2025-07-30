#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"

#define LOCATION_FROG (0xFF0000 | this->frogIndex)

#define FROG_LIMB_MAX 0x18
#include "overlays/actors/ovl_En_Minifrog/z_en_minifrog.h"

EnMinifrog* sEnMinifrog;

static u16 sIsFrogReturnedFlags[] = {
    0,                  // FROG_YELLOW
    WEEKEVENTREG_32_40, // FROG_CYAN
    WEEKEVENTREG_32_80, // FROG_PINK
    WEEKEVENTREG_33_01, // FROG_BLUE
    WEEKEVENTREG_33_02, // FROG_WHITE
};

void EnMinifrog_SpawnDust(EnMinifrog* this, PlayState* play);
void EnMinifrog_ReturnFrogCutscene(EnMinifrog* this, PlayState* play);

RECOMP_HOOK("EnMinifrog_Init")
void OnEnMinifrog_Init(Actor* thisx, PlayState* play) {
    EnMinifrog* this = ((EnMinifrog*)thisx);
    sEnMinifrog = this;

    if (!EN_FROG_IS_RETURNED(&this->actor)) {
        if (!rando_location_is_checked(LOCATION_FROG)) {
            CLEAR_WEEKEVENTREG(sIsFrogReturnedFlags[this->frogIndex]);
        } else {
            SET_WEEKEVENTREG(sIsFrogReturnedFlags[this->frogIndex]);
        }
    }
}

// RECOMP_HOOK_RETURN("EnMinifrog_Init")
// void AfterEnMinifrog_Init(Actor* thisx, PlayState* play) {
//     EnMinifrog* this = sEnMinifrog;
//     PlayState* play = gPlay;
// 
//     if (!EN_FROG_IS_RETURNED(&this->actor)) {
//         if (rando_has_item(FROG_ITEM)) {
//             SET_WEEKEVENTREG(sIsFrogReturnedFlags[this->frogIndex]);
//         } else {
//             CLEAR_WEEKEVENTREG(sIsFrogReturnedFlags[this->frogIndex]);
//         }
//     }
// }

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