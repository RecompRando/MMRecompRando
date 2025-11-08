#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"
#include "rt64_extended_gbi.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "models/container_textures.h"

Gfx* GenericContainer_SetTextures(PlayState* play, Gfx* gfx, u8* customDraw, u32 location) {
    if (!rando_get_camc_enabled()) {
        *customDraw = CAMC_DRAW_DISABLED;
        return gfx;
    }
    
    if (rando_get_camc_enabled() == CAMC_SHOW_UNCHECKED) {
        *customDraw = CAMC_DRAW_UNCHECKED;
        gSPSegment(gfx++, 0x09, unchecked_side);
        gSPSegment(gfx++, 0x0A, unchecked_corner);
        return gfx;
    }

    *customDraw = CAMC_DRAW_ENABLED;

    // set segments to textures based on item contents
    s16 getItem = rando_get_item_id(location);
    TexturePtr front_tex;
    TexturePtr side_tex;
    TexturePtr extra_tex; // only used for ap textures

    front_tex = major_front;
    side_tex = major_side;
    extra_tex = major_front;

    switch (getItem) {
        case GI_AP_PROG:
            *customDraw = CAMC_DRAW_CUSTOM;
            front_tex = apProgFrontLeftTex;
            side_tex = apProgCornerTex;
            extra_tex = apProgFrontRightTex;
            break;
        case GI_AP_USEFUL:
            *customDraw = CAMC_DRAW_CUSTOM;
            front_tex = apUsefulFrontLeftTex;
            side_tex = apUsefulCornerTex;
            extra_tex = apUsefulFrontRightTex;
            break;
        case GI_AP_FILLER:
            *customDraw = CAMC_DRAW_CUSTOM;
            front_tex = apJunkFrontLeftTex;
            side_tex = apJunkCornerTex;
            extra_tex = apJunkFrontRightTex;
            break;
        case GI_KEY_BOSS:
        case GI_KEY_BOSS_WOODFALL:
        case GI_KEY_BOSS_SNOWHEAD:
        case GI_KEY_BOSS_GREATBAY:
        case GI_KEY_BOSS_STONETOWER:
            front_tex = gBoxChestLockOrnateTex;
            side_tex = gBoxChestCornerOrnateTex;
            break;
        case GI_KEY_SMALL:
        case GI_KEY_SMALL_WOODFALL:
        case GI_KEY_SMALL_SNOWHEAD:
        case GI_KEY_SMALL_GREATBAY:
        case GI_KEY_SMALL_STONETOWER:
            front_tex = key_front;
            side_tex = key_side;
            break;
        case GI_B2:
        case GI_46:
        case GI_47:
        case GI_48:
        case GI_49:
            front_tex = fairy_front;
            side_tex = fairy_side;
            break;
        case GI_HEART_PIECE:
        case GI_HEART_CONTAINER:
        case GI_DEFENSE_DOUBLE:
            front_tex = heart_front;
            side_tex = heart_side;
            break;
        case GI_TRUE_SKULL_TOKEN:
        case GI_OCEAN_SKULL_TOKEN:
            front_tex = spider_front;
            side_tex = spider_side;
            break;
        default:
            // redundant due to GI models
            if (!rando_get_location_has_local_item(location)) {
                *customDraw = CAMC_DRAW_CUSTOM;
                switch (rando_get_location_type(location)) {
                    case RANDO_ITEM_CLASS_PROGRESSION:
                    case RANDO_ITEM_CLASS_TRAP:
                        front_tex = apProgFrontLeftTex;
                        side_tex = apProgCornerTex;
                        extra_tex = apProgFrontRightTex;
                        break;
                    case RANDO_ITEM_CLASS_USEFUL:
                        front_tex = apUsefulFrontLeftTex;
                        side_tex = apUsefulCornerTex;
                        extra_tex = apUsefulFrontRightTex;
                        break;
                    case RANDO_ITEM_CLASS_JUNK:
                    default:
                        front_tex = apJunkFrontLeftTex;
                        side_tex = apJunkCornerTex;
                        extra_tex = apJunkFrontRightTex;
                        break;
                }
            } else {
                // switch (rando_get_location_type(location)) {
                //     case RANDO_ITEM_CLASS_PROGRESSION:
                //     case RANDO_ITEM_CLASS_TRAP:
                //         front_tex = major_side;
                //         side_tex = major_front;
                //         break;
                //     case RANDO_ITEM_CLASS_USEFUL:
                //     case RANDO_ITEM_CLASS_JUNK:
                //     default:
                //         *customDraw = false;
                //         break;
                // }
                *customDraw = CAMC_DRAW_DISABLED;
            }
    }

    if (getItem >= GI_BOSS_SOUL_ODOLWA && getItem <= GI_ABSURD_GENERIC) {
        front_tex = soul_front;
        side_tex = soul_side;
        *customDraw = CAMC_DRAW_ENABLED;
    }

    if (!(*customDraw)) return gfx;

    gSPSegment(gfx++, 0x09, front_tex);
    gSPSegment(gfx++, 0x0A, side_tex);
    gSPSegment(gfx++, 0x0B, extra_tex);
    
    return gfx;
}