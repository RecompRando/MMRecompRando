#include "ultra64.h"
#include "global.h"

extern Vtx object_spdwebVtx_000000[];
extern u64 object_spdweb_Tex_000138[];
extern u64 object_spdweb_Tex_000938[];

extern Vtx object_spdwebVtx_0011F0[];
extern u64 object_spdweb_Tex_001408[];

Gfx randoSpiderWeb1[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    // gsDPSetCombineLERP(TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, TEXEL0, 0, TEXEL0, 0, COMBINED, 0, SHADE, 0, COMBINED, 0,
    //                    PRIMITIVE, 0),
    // gsDPSetPrimColor(0, 0xFF, 255, 255, 255, 255),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(object_spdweb_Tex_000938, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP,
                         G_TX_MIRROR | G_TX_WRAP, 5, 6, 1, G_TX_NOLOD),
    gsDPLoadMultiBlock(object_spdweb_Tex_000138, 0x0100, 1, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR |
                       G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 5, 5, 2, 2),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(object_spdwebVtx_000000, 6, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 0, 2, 0),
    gsSP2Triangles(2, 1, 4, 0, 3, 2, 5, 0),
    gsSP1Triangle(5, 2, 4, 0),
    gsSPEndDisplayList(),
};

Gfx randoSpiderWeb2[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    // gsDPSetCombineLERP(TEXEL1, TEXEL0, PRIM_LOD_FRAC, TEXEL0, TEXEL1, 0, TEXEL1, 0, COMBINED, 0, SHADE, 0, COMBINED, 0,
    //                    PRIMITIVE, 0),
    // gsDPSetPrimColor(0, 0xFF, 255, 255, 255, 255),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, TEXEL0, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(object_spdweb_Tex_001408, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 64, 0, G_TX_MIRROR | G_TX_WRAP,
                         G_TX_MIRROR | G_TX_WRAP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadMultiBlock(object_spdweb_Tex_001408, 0x0000, 1, G_IM_FMT_IA, G_IM_SIZ_8b, 64, 64, 0, G_TX_MIRROR |
                       G_TX_WRAP, G_TX_MIRROR | G_TX_WRAP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_SHADING_SMOOTH),
    gsSPVertex(object_spdwebVtx_0011F0, 16, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 2, 0),
    gsSP2Triangles(6, 7, 2, 0, 5, 8, 6, 0),
    gsSP2Triangles(4, 9, 5, 0, 1, 4, 2, 0),
    gsSP2Triangles(10, 11, 6, 0, 10, 6, 8, 0),
    gsSP2Triangles(12, 10, 8, 0, 12, 8, 5, 0),
    gsSP2Triangles(13, 12, 5, 0, 13, 5, 9, 0),
    gsSP2Triangles(14, 13, 9, 0, 14, 9, 4, 0),
    gsSP2Triangles(6, 11, 15, 0, 6, 15, 7, 0),
    gsSP2Triangles(7, 15, 3, 0, 7, 3, 2, 0),
    gsSP2Triangles(0, 14, 4, 0, 0, 4, 1, 0),
    gsSPEndDisplayList(),
};