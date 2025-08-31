#include "modding.h"
#include "global.h"

#include "apcommon.h"

// Sending Checks
RECOMP_EXPORT s32 Actor_OfferGetItemHookExternal(Actor* actor, PlayState* play, GetItemId getItemId, u32 location, f32 xzRange, f32 yRange, bool use_workaround, bool item_is_shuffled) {
    return Actor_OfferGetItemHook(actor, play, getItemId, location, xzRange, yRange, use_workaround, item_is_shuffled);
}

RECOMP_EXPORT void rando_send_location_external(u32 location_id) {
    return rando_send_location(location_id);
}

// Check if a Location is Checked
RECOMP_EXPORT bool rando_location_is_checked_external(u32 location_id) {
    return rando_location_is_checked(location_id);
}

RECOMP_EXPORT bool rando_location_is_checked_async_external(u32 location_id) {
    return rando_location_is_checked_async(location_id);
}

// Get Simple Data From Rando
RECOMP_EXPORT u32 rando_get_random_seed_external() {
    return rando_get_random_seed();
}

RECOMP_EXPORT u32 rando_get_moon_remains_required_external() {
    return rando_get_moon_remains_required();
}

RECOMP_EXPORT u32 rando_get_majora_remains_required_external() {
    return rando_get_majora_remains_required();
}

RECOMP_EXPORT bool rando_is_magic_trap_external() {
    return rando_is_magic_trap();
}

RECOMP_EXPORT s16 rando_get_shop_price_external(u32 shop_item_id) {
    return rando_get_shop_price(shop_item_id);
}

RECOMP_EXPORT u32 rando_get_location_type_external(u32 location_id) {
    return rando_get_location_type(location_id);
}

// Get Randomizer/Archipelago Specific Data
RECOMP_EXPORT u32 rando_get_item_id_external(u32 location_id) {
    return rando_get_item_id(location_id);
}

RECOMP_EXPORT void rando_broadcast_location_hint_external(u32 location_id) {
    return rando_broadcast_location_hint(location_id);
}

RECOMP_EXPORT u32 rando_has_item_external(u32 item_id) {
    return rando_has_item(item_id);
}

RECOMP_EXPORT u32 rando_has_item_async_external(u32 item_id) {
    return rando_has_item_async(item_id);
}

RECOMP_EXPORT u32 rando_get_own_slot_id_external() {
    return rando_get_own_slot_id();
}

RECOMP_EXPORT void rando_get_own_slot_name_external(char* out_str) {
    rando_get_own_slot_name(out_str);
}

RECOMP_EXPORT u32 rando_get_sending_player_external(u32 items_i) {
    return rando_get_sending_player(items_i);
}

RECOMP_EXPORT void rando_get_item_name_from_id_external(u32 item_id, char* out_str) {
    rando_get_item_name_from_id(item_id, out_str);
}

RECOMP_EXPORT void rando_get_sending_player_name_external(u32 items_i, char* out_str) {
    rando_get_sending_player_name(items_i, out_str);
}

RECOMP_EXPORT void rando_get_location_item_player_external(u32 location_id, char* out_str) {
    rando_get_location_item_player(location_id, out_str);
}

RECOMP_EXPORT void rando_get_location_item_name_external(u32 location_id, char* out_str) {
    rando_get_location_item_name(location_id, out_str);
}

RECOMP_EXPORT u32 rando_get_seed_name_external(char* seed_name_out, u32 buffer_size) {
    return rando_get_seed_name(seed_name_out, buffer_size);
}