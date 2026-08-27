#include "modding.h"
#include "global.h"
#include "recompconfig.h"
#include "recomputils.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_En_Ishi/z_en_ishi.h"

static ColliderInfo* sBoostedToucher = NULL;
 // Add a mod config to enable zora fins to break rocks when underwater since
// small rocks normally only accept 0x508 (explosives, goron punch, goron pound) as destructive.
RECOMP_HOOK("func_8095E660")
void OnEnIshi_AllowFins(EnIshi* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    ColliderInfo* hit;

    sBoostedToucher = NULL;
    if (!recomp_get_config_u32("zora_fins_break_rocks")) {
        return;
    }
    if (ENISHI_GET_1(&this->actor) || !(this->collider.base.acFlags & AC_HIT)) {
        return;
    }
    if (!(player->actor.bgCheckFlags & BGCHECKFLAG_WATER)) {
        return;
    }
    hit = this->collider.info.acHitInfo;
    if (hit == NULL) {
        return;
    }
    if (!(hit->toucher.dmgFlags & DMG_ZORA_BOOMERANG) || (hit->toucher.dmgFlags & DMG_EXPLOSIVES)) {
        return;
    }
    hit->toucher.dmgFlags |= DMG_EXPLOSIVES;
    sBoostedToucher = hit;
}

RECOMP_HOOK_RETURN("func_8095E660")
void OnEnIshi_AllowFinsReturn() {
    if (sBoostedToucher != NULL) {
        sBoostedToucher->toucher.dmgFlags &= ~DMG_EXPLOSIVES;
        sBoostedToucher = NULL;
    }
}
