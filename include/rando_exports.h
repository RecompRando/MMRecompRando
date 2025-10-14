#include "global.h"

// Sending Checks
RECOMP_IMPORT("mm_recomp_rando", s32 Actor_OfferGetItemHookExternal(Actor* actor, PlayState* play, GetItemId getItemId, u32 location, f32 xzRange, f32 yRange, bool use_workaround, bool item_is_shuffled));
RECOMP_IMPORT("mm_recomp_rando", void rando_send_location_external(u32 location_id));

// Check if a Location is Checked
RECOMP_IMPORT("mm_recomp_rando", bool rando_location_is_checked_external(u32 location_id));
RECOMP_IMPORT("mm_recomp_rando", bool rando_location_is_checked_async_external(u32 location_id));

// Get Simple Data From Rando
RECOMP_IMPORT("mm_recomp_rando", u32 rando_get_random_seed_external());
RECOMP_IMPORT("mm_recomp_rando", u32 rando_get_moon_remains_required_external());
RECOMP_IMPORT("mm_recomp_rando", u32 rando_get_majora_remains_required_external());
RECOMP_IMPORT("mm_recomp_rando", bool rando_is_magic_trap_external());
RECOMP_IMPORT("mm_recomp_rando", s16 rando_get_shop_price_external(u32 shop_item_id));
RECOMP_IMPORT("mm_recomp_rando", u32 rando_get_location_type_external(u32 location_id));

// Get Randomizer/Archipelago Specific Data
RECOMP_IMPORT("mm_recomp_rando", u32 rando_get_item_id_external(u32 location_id));
RECOMP_IMPORT("mm_recomp_rando", void rando_broadcast_location_hint_external(u32 location_id));
RECOMP_IMPORT("mm_recomp_rando", u32 rando_has_item_external(u32 item_id));
RECOMP_IMPORT("mm_recomp_rando", u32 rando_has_item_async_external(u32 item_id));
RECOMP_IMPORT("mm_recomp_rando", u32 rando_get_own_slot_id_external());
RECOMP_IMPORT("mm_recomp_rando", void rando_get_own_slot_name_external(char* out_str));
RECOMP_IMPORT("mm_recomp_rando", u32 rando_get_sending_player_external(u32 items_i));
RECOMP_IMPORT("mm_recomp_rando", void rando_get_item_name_from_id_external(u32 item_id, char* out_str));
RECOMP_IMPORT("mm_recomp_rando", void rando_get_sending_player_name_external(u32 items_i, char* out_str));
RECOMP_IMPORT("mm_recomp_rando", void rando_get_location_item_player_external(u32 location_id, char* out_str));
RECOMP_IMPORT("mm_recomp_rando", void rando_get_location_item_name_external(u32 location_id, char* out_str));
RECOMP_IMPORT("mm_recomp_rando", u32 rando_get_seed_name_external(char* seed_name_out, u32 buffer_size));