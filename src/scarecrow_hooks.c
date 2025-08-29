#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define ACTOR_FLAG_LOCK_ON_DISABLED  (1 << 27)
#include "overlays/actors/ovl_En_Kakasi/z_en_kakasi.h"

#define LOCATION_SCARECROW (0x300000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                            | randoGetLoadedActorNumInSameRoom(play, &this->picto.actor))

void EnKakasi_SetupRiseOutOfGround(EnKakasi* this, PlayState* play);
void EnKakasi_IdleUnderground(EnKakasi* this, PlayState* play);

void EnKakasi_GiveRandoItem(EnKakasi* this, PlayState* play) {
    if (Actor_HasParent(&this->picto.actor, play)) {
        this->picto.actor.parent = NULL;
        // this->actionFunc = EnKakasi_IdleUnderground;
        this->actionFunc = EnKakasi_SetupRiseOutOfGround;
    } else {
        Actor_OfferGetItemHook(&this->picto.actor, play, rando_get_item_id(LOCATION_SCARECROW), LOCATION_SCARECROW, 1000.0f, 1000.0f, true, true);
    }
}

RECOMP_HOOK("EnKakasi_IdleUnderground")
void EnKakasi_IdleUndergroundAndWaitForSpawn(EnKakasi* this, PlayState* play) {
    // if (CHECK_WEEKEVENTREG(WEEKEVENTREG_79_08) && (this->picto.actor.xzDistToPlayer < this->songSummonDist) &&
    //     ((BREG(1) != 0) || (play->msgCtx.ocarinaMode == OCARINA_MODE_PLAYED_SCARECROW_SPAWN))) {
    if ((this->picto.actor.xzDistToPlayer < this->songSummonDist) && ((BREG(1) != 0) || (play->msgCtx.ocarinaMode == OCARINA_MODE_ACTIVE))) {
        this->picto.actor.flags &= ~ACTOR_FLAG_LOCK_ON_DISABLED;
        play->msgCtx.ocarinaMode = OCARINA_MODE_END;
        AudioOcarina_SetOcarinaDisableTimer(0, 1);
        Message_CloseTextbox(play);
        if (!rando_location_is_checked(LOCATION_SCARECROW)) {
            recomp_printf("scarecrow location: 0x%06X\n", LOCATION_SCARECROW);
            this->actionFunc = EnKakasi_GiveRandoItem;
        } else {
            this->actionFunc = EnKakasi_SetupRiseOutOfGround;
        }
    }
}