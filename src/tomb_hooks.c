#include "modding.h"
#include "global.h"

#include "apcommon.h"

#include "overlays/actors/ovl_Bg_Haka_Tomb/z_bg_haka_tomb.h"

#define GI_STORMS GI_A2

s32 func_80BD6638(s16* csId, s16* csIdList, s32 numCutscenes);
void func_80BD6754(BgHakaTomb* this);

RECOMP_PATCH void func_80BD66AC(BgHakaTomb* this, PlayState* play) {
    s16 csId;

    if (Flags_GetClear(play, this->dyna.actor.room)) {
        this->dyna.actor.flags |= (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY);
    }
    if (!func_80BD6638(&csId, this->csIdList, ARRAY_COUNT(this->csIdList)) && (csId <= CS_ID_NONE) &&
        Flags_GetClear(play, this->dyna.actor.room) && !(rando_location_is_checked(GI_STORMS))) {
        this->dyna.actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        if (this->dyna.actor.isLockedOn) {
            // func_80BD6754(this);
			Actor_OfferGetItem(&this->dyna.actor, play, GI_STORMS, 300.0f, 300.0f);
        }
    } else {
        this->dyna.actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
    }
}