#include "modding.h"
#include "global.h"

s16 give_shield_back = false;

/*
 * The following two hooks are from the same function. That function in question triggers when the player is getting
 * spit out of a Like Like. It checks a few things, such as if Link is in human form and if he has Hero Shield. If
 * true, then it'll delete the shield from the inventory.
 */

/*
 * Check if Link is in human form and if he has Hero Shield equipped. If true, it'll remove it from the inventory. This
 * is to bypass the checks from the hooked function.
 */
RECOMP_HOOK("func_808FA4F4") void LikeLike_ShieldRemovalBypass_Hook() {
    if (GET_PLAYER_FORM == PLAYER_FORM_HUMAN && GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) == EQUIP_VALUE_SHIELD_HERO) {
        SET_EQUIP_VALUE(EQUIP_TYPE_SHIELD, EQUIP_VALUE_SHIELD_NONE);
        give_shield_back = true;
    }
}

/*
 * If the shield was removed from LikeLike_ShieldRemovalBypass_Hook, give the shield back to the player.
 */
RECOMP_HOOK_RETURN("func_808FA4F4") void LikeLike_ShieldRemovalBypass_Return() {
    if (give_shield_back == true) {
        SET_EQUIP_VALUE(EQUIP_TYPE_SHIELD, EQUIP_VALUE_SHIELD_HERO);
        give_shield_back = false;
    }
}