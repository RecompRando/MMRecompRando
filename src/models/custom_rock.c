#include "ultra64.h"
#include "global.h"

extern u16 rockTex[];
extern Vtx gameplay_field_keepVtx_006500[];

Gfx randoFieldSmallRockOpaDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    // gsDPLoadTextureBlock(gameplay_field_keep_Tex_006810, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
    //                      G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    // gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock(rockTex, G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING),
    gsSPVertex(gameplay_field_keepVtx_006500, 27, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 4, 0),
    gsSP2Triangles(5, 6, 7, 0, 0, 8, 9, 0),
    gsSP2Triangles(10, 11, 6, 0, 12, 13, 14, 0),
    gsSP2Triangles(15, 7, 6, 0, 16, 1, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 16, 0),
    gsSP2Triangles(23, 24, 16, 0, 25, 26, 13, 0),
    gsSPEndDisplayList(),
};

Gfx randoFieldSmallRockXluDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    // gsDPLoadTextureBlock(gameplay_field_keep_Tex_006810, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
    //                      G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    // gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPLoadTextureBlock(rockTex, G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING),
    gsSPVertex(gameplay_field_keepVtx_006500, 27, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 4, 0),
    gsSP2Triangles(5, 6, 7, 0, 0, 8, 9, 0),
    gsSP2Triangles(10, 11, 6, 0, 12, 13, 14, 0),
    gsSP2Triangles(15, 7, 6, 0, 16, 1, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 16, 0),
    gsSP2Triangles(23, 24, 16, 0, 25, 26, 13, 0),
    gsSPEndDisplayList(),
};

// probably redundant
extern u16 rockTex2[];
extern Vtx object_ishiVtx_000000[];

Gfx randoSmallRockDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    // gsDPLoadTextureBlock(gSmallRockTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR
    //                      | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    // gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsDPLoadTextureBlock(rockTex2, G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR
                         | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING),
    gsSPVertex(object_ishiVtx_000000, 27, 0),
    gsSP2Triangles(0, 1, 2, 0, 2, 3, 4, 0),
    gsSP2Triangles(5, 6, 7, 0, 0, 8, 9, 0),
    gsSP2Triangles(10, 11, 6, 0, 12, 13, 14, 0),
    gsSP2Triangles(15, 7, 6, 0, 16, 1, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 16, 0),
    gsSP2Triangles(23, 24, 16, 0, 25, 26, 13, 0),
    gsSPEndDisplayList(),
};