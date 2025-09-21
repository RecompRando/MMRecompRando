#include "ultra64.h"
#include "z64.h"
#include "macros.h"

extern u16 grassTex[];
extern u16 grassSproutTex[];
extern Vtx gameplay_field_keepVtx_007810[];
extern Vtx object_kusaVtx_000000[];

Gfx gKusaBushType1GrayscaleDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(grassTex, G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    // gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(gameplay_field_keepVtx_007810, 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP1Triangle(6, 7, 8, 0),
    gsSPEndDisplayList(),
};

Gfx gKusaBushType2GrayscaleDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(grassTex, G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    // gsDPSetCombineMode(G_CC_MODULATEIDECALA, G_CC_MODULATEIA_PRIM2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsSPVertex(gameplay_field_keepVtx_007810, 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP1Triangle(6, 7, 8, 0),
    gsSPEndDisplayList(),
};

Gfx gObjGrass_GrayscaleOpaDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(grassTex, G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsSPEndDisplayList(),
};

Gfx gObjGrass_GrayscaleXluDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(grassTex, G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    gsSPEndDisplayList(),
};

Gfx gKusaSproutGrayscaleDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(grassSproutTex, G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR |
                         G_TX_CLAMP, G_TX_NOMIRROR | G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    // gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPVertex(object_kusaVtx_000000, 20, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 3, 1, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 7, 5, 0),
    gsSP2Triangles(8, 9, 10, 0, 8, 11, 9, 0),
    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
    gsSPEndDisplayList(),
};