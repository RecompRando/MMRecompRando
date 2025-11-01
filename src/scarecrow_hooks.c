#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define ACTOR_FLAG_LOCK_ON_DISABLED  (1 << 27)
#include "overlays/actors/ovl_En_Kakasi/z_en_kakasi.h"

#define LOCATION_SCARECROW (AP_PREFIX_SCARECROW | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->picto.actor))

EnKakasi* savedEnKakasi;
EnKakasiActionFunc savedEnKakasiActionFunc;

void EnKakasi_Draw(Actor* thisx, PlayState* play);
void EnKakasi_RisingOutOfGround(EnKakasi* this, PlayState* play);
void EnKakasi_SetupRiseOutOfGround(EnKakasi* this, PlayState* play);

RECOMP_HOOK("EnKakasi_Init")
void OnEnKakasi_Init(Actor* thisx, PlayState* play) {
    savedEnKakasi = ((EnKakasi*)thisx);
}

RECOMP_HOOK_RETURN("EnKakasi_Init")
void AfterEnKakasi_Init() {
    EnKakasi* this = savedEnKakasi;
    PlayState* play = gPlay;
    // there's some weird bug that seems to not know if the player has this item?
    if (rando_has_item(LOCATION_SCARECROW)) { // locations and items share ids
        this->picto.actor.draw = EnKakasi_Draw;
        this->unkState196 = 6;
        this->actionFunc = EnKakasi_RisingOutOfGround;
    }
}

void EnKakasi_GiveRandoItem(EnKakasi* this, PlayState* play) {
    if (Actor_HasParent(&this->picto.actor, play)) {
        this->picto.actor.parent = NULL;
        this->actionFunc = savedEnKakasiActionFunc;
    } else {
        Actor_OfferGetItemHook(&this->picto.actor, play, rando_get_item_id(LOCATION_SCARECROW), LOCATION_SCARECROW, 1000.0f, 1000.0f, true, true);
    }
}

// patched to prevent scarecrow spawns when location is checked
RECOMP_PATCH void EnKakasi_IdleUnderground(EnKakasi* this, PlayState* play) {
    if (rando_get_slotdata_u32("scarecrowsanity") && rando_location_is_checked(LOCATION_SCARECROW)) {
        return;
    }
    
    // if (CHECK_WEEKEVENTREG(WEEKEVENTREG_79_08) && (this->picto.actor.xzDistToPlayer < this->songSummonDist) &&
    //     ((BREG(1) != 0) || (play->msgCtx.ocarinaMode == OCARINA_MODE_PLAYED_SCARECROW_SPAWN))) {
    if ((this->picto.actor.xzDistToPlayer < this->songSummonDist) && ((BREG(1) != 0) || (play->msgCtx.ocarinaMode == OCARINA_MODE_ACTIVE))) {
        this->picto.actor.flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
        play->msgCtx.ocarinaMode = OCARINA_MODE_END;
        AudioOcarina_SetOcarinaDisableTimer(0, 1);
        Message_CloseTextbox(play);
        if (rando_get_slotdata_u32("scarecrowsanity") && !rando_location_is_checked(LOCATION_SCARECROW)) {
            recomp_printf("scarecrow location: 0x%06X\n", LOCATION_SCARECROW);
            savedEnKakasiActionFunc = this->actionFunc;
            this->actionFunc = EnKakasi_GiveRandoItem;
        } else {
            this->actionFunc = EnKakasi_SetupRiseOutOfGround;
        }
    }
}

RECOMP_HOOK("EnKakasi_IdleRisen")
void EnKakasi_IdleRisenCheckForOcarina(EnKakasi* this, PlayState* play) {
    if (!rando_get_slotdata_u32("scarecrowsanity") || rando_location_is_checked(LOCATION_SCARECROW)) {
        return;
    }

    if ((this->picto.actor.xzDistToPlayer < this->songSummonDist) && ((BREG(1) != 0) || (play->msgCtx.ocarinaMode == OCARINA_MODE_ACTIVE))) {
        // this->picto.actor.flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
        play->msgCtx.ocarinaMode = OCARINA_MODE_END;
        AudioOcarina_SetOcarinaDisableTimer(0, 1);
        Message_CloseTextbox(play);
        
        recomp_printf("scarecrow location: 0x%06X\n", LOCATION_SCARECROW);
        savedEnKakasiActionFunc = this->actionFunc;
        this->actionFunc = EnKakasi_GiveRandoItem;
    }
}