#ifndef __RANDO_COLORS__
#define __RANDO_COLORS__

#include "modding.h"
#include "global.h"

extern Color_RGB8 rainbowColor;

extern Color_RGB8 randoProgColor;
extern Color_RGB8 randoUsefulColor;
extern Color_RGB8 randoFillerColor;

extern Color_RGB8 randoHeartColor;
extern Color_RGB8 randoSpiderColor;
extern Color_RGB8 randoFairyColor;
extern Color_RGB8 randoKeyColor;

u16 RGBA16toGrayscale_Color(u16 RGBA);
void RGBA16toIA16_Texture(u16* rgbaTex, u16* outTex, u32 arrayCount);
void hsv_to_rgb(float h, float s, float v, Color_RGB8* out);
bool get_rando_color(Color_RGB8* rColor, u32 location);

#endif