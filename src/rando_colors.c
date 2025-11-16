#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"

#include "rando_colors.h"

Color_RGB8 rainbowColor = {255, 255, 255};
Color_RGB8 goldColor = {255, 176, 0};

Color_RGB8 randoUsefulColor = {64, 255, 255};
Color_RGB8 randoFillerColor = {128, 128, 128}; // might go unused in favor of vanilla textures

Color_RGB8 randoHeartColor = {255, 0, 0};
Color_RGB8 randoSpiderColor = {255, 255, 255};
Color_RGB8 randoFairyColor = {255, 128, 255};
Color_RGB8 randoKeyColor = {192, 192, 192};
Color_RGB8 randoSoulColor = {136, 0, 255};

u16 RGBA16toGrayscale_Color(u16 RGBA, IA_Grayscale_Type type) {
    // https://github.com/krimtonz/rgba5551topng/blob/master/rgba5551topng.c#L111-L114
    u8 red = (((RGBA >> 11) & 0x1F) * 255 + 15) / 31; 
    u8 green = (((RGBA >> 6) & 0x1F) * 255 + 15) / 31;
    u8 blue = (((RGBA >> 1) & 0x1F) * 255 + 15) / 31;
    u8 alpha = (RGBA & 0x0001) * 255;

    u8 grayscale;
    f32 luminosity;

    switch (type) {
        case GRAYSCALE_LUMINOSITY:
            // BT.601 luminosity grayscale
            grayscale = (red * 0.299) + (green * 0.587) + (blue * 0.114);
            break;
        case GRAYSCALE_MAX:
            // simplified version of below
            // guessing this is decomposition?
            grayscale = (MAX(MAX(red, blue), green));
            break;
        case GRAYSCALE_OOTMM:
            // combo's grayscale conversion
            luminosity = (MAX(MAX(red, blue), green)) / 255.0f;
            grayscale = CLAMP_MAX(sqrtf(sqrtf(luminosity)), 1.0) * 255;
            break;
    }

    return (grayscale << 8) | (alpha);
}

void RGBA16toIA16_Texture(u16* rgbaTex, u16* outTex, u32 arrayCount, IA_Grayscale_Type type) {
    for (u32 i = 0; i < arrayCount; i++) {
        outTex[i] = RGBA16toGrayscale_Color(rgbaTex[i], type);
    }
}

void hsv_to_rgb(float h, float s, float v, Color_RGB8* out) {
    float r = 0, g = 0, b = 0;
    if (s == 0) {
        r = g = b = v;
    } else {
        int i;
        float f, p, q, t;
        h = h / 60.0f;
        i = (int)(h);
        f = h - i;
        p = v * (1 - s);
        q = v * (1 - f * s);
        t = v * (1 - (1 - f) * s);
        switch (i) {
            case 0:
                r = v;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = v;
                b = p;
                break;
            case 2:
                r = p;
                g = v;
                b = t;
                break;
            case 3:
                r = p;
                g = q;
                b = v;
                break;
            case 4:
                r = t;
                g = p;
                b = v;
                break;
            default:
                r = v;
                g = p;
                b = q;
                break;
        }
    }
    out->r = r * 255;
    out->g = g * 255;
    out->b = b * 255;
}

RECOMP_CALLBACK("*", recomp_on_play_main)
void cycle_rainbow() {
    static float hue = 0.0f;
    hue += 5.0;
    if (hue >= 360.0f) {
        hue -= 360.0f;
    }
    hsv_to_rgb(hue, 1.0f, 1.0f, &rainbowColor);
}

// sets rColor based on item/location type, returns false to draw original
bool get_rando_color(Color_RGB8* rColor, u32 location) {
    // show default DL if camc is off or location was checked
    if (!rando_get_camc_enabled() || rando_location_is_checked(location) || !rando_location_exists(location)) {
        return false;
    }

    // use yellow color if "show unchecked" in enabled
    if (rando_get_camc_enabled() == CAMC_SHOW_UNCHECKED && !rando_location_is_checked(location)) {
        *rColor = goldColor;
        return true;
    }

    s16 getItem = rando_get_item_id(location);

    // souls (bounds need to be extended for more souls)
    if (getItem >= GI_BOSS_SOUL_ODOLWA && getItem <= GI_ABSURD_GENERIC) {
        *rColor = randoSoulColor;
        return true;
    }

    switch (getItem) {
        case GI_AP_PROG:
            *rColor = rainbowColor;
            return true;
        case GI_AP_USEFUL:
            *rColor = randoUsefulColor;
            return true;
        case GI_AP_FILLER:
            *rColor = randoFillerColor;
            return true;
        // case GI_KEY_BOSS:
        // case GI_KEY_SMALL:
        // case GI_KEY_BOSS_WOODFALL:
        // case GI_KEY_SMALL_WOODFALL:
        // case GI_MAP_WOODFALL:
        // case GI_COMPASS_WOODFALL:
        // case GI_KEY_BOSS_SNOWHEAD:
        // case GI_KEY_SMALL_SNOWHEAD:
        // case GI_MAP_SNOWHEAD:
        // case GI_COMPASS_SNOWHEAD:
        // case GI_KEY_BOSS_GREATBAY:
        // case GI_KEY_SMALL_GREATBAY:
        // case GI_MAP_GREATBAY:
        // case GI_COMPASS_GREATBAY:
        // case GI_KEY_BOSS_STONETOWER:
        // case GI_KEY_SMALL_STONETOWER:
        // case GI_MAP_STONETOWER:
        // case GI_COMPASS_STONETOWER:
        //     *rColor = randoKeyColor;
        //     return true;
        case GI_B2:
        case GI_46:
        case GI_47:
        case GI_48:
        case GI_49:
            *rColor = randoFairyColor;
            return true;
        case GI_HEART_PIECE:
        case GI_HEART_CONTAINER:
        case GI_DEFENSE_DOUBLE:
            *rColor = randoHeartColor;
            return true;
        case GI_TRUE_SKULL_TOKEN:
        case GI_OCEAN_SKULL_TOKEN:
            *rColor = randoSpiderColor;
            return true;
        default:
            switch (rando_get_location_type(location)) {
                case RANDO_ITEM_CLASS_PROGRESSION:
                case RANDO_ITEM_CLASS_TRAP:
                    *rColor = rainbowColor;
                    return true;
                case RANDO_ITEM_CLASS_USEFUL:
                    *rColor = randoUsefulColor;
                    return true;
                case RANDO_ITEM_CLASS_JUNK:
                default:
                    return false;
            }
    }
}