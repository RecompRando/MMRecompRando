#include "ultra64.h"
#include "global.h"

// standard boulders
extern u64 object_bombiwa_Tex_000020[];
extern u64 object_bombiwa_TLUT_000000[];
extern Vtx object_bombiwaVtx_000820[];

Gfx randoBombiwaOpaDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    // TODO: handle CI
    gsDPLoadTextureBlock_4b(object_bombiwa_Tex_000020, G_IM_FMT_CI, 64, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR
                            | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadTLUT_pal16(0, object_bombiwa_TLUT_000000),
    // gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, 0, 0, 0, COMBINED, 0, 0, 0, COMBINED),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING),
    gsSPVertex(object_bombiwaVtx_000820, 28, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 6, 4, 0, 5, 4, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 9, 11, 10, 0),
    gsSP2Triangles(10, 11, 12, 0, 11, 13, 12, 0),
    gsSP2Triangles(12, 14, 15, 0, 15, 6, 16, 0),
    gsSP2Triangles(15, 13, 17, 0, 18, 10, 19, 0),
    gsSP2Triangles(10, 12, 19, 0, 12, 15, 19, 0),
    gsSP2Triangles(19, 15, 20, 0, 3, 20, 15, 0),
    gsSP2Triangles(5, 21, 3, 0, 21, 20, 3, 0),
    gsSP2Triangles(10, 18, 8, 0, 22, 23, 24, 0),
    gsSP2Triangles(23, 25, 24, 0, 25, 26, 24, 0),
    gsSP2Triangles(26, 27, 24, 0, 27, 22, 24, 0),
    gsSPEndDisplayList(),
};

Gfx randoBombiwaXluDL[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    // TODO: handle CI
    gsDPLoadTextureBlock_4b(object_bombiwa_Tex_000020, G_IM_FMT_CI, 64, 64, 0, G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMIRROR
                            | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadTLUT_pal16(0, object_bombiwa_TLUT_000000),
    // gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, 1, 0, 0, 0, COMBINED, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_CULL_BACK | G_FOG | G_LIGHTING),
    gsSPVertex(object_bombiwaVtx_000820, 28, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(3, 6, 4, 0, 5, 4, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 9, 11, 10, 0),
    gsSP2Triangles(10, 11, 12, 0, 11, 13, 12, 0),
    gsSP2Triangles(12, 14, 15, 0, 15, 6, 16, 0),
    gsSP2Triangles(15, 13, 17, 0, 18, 10, 19, 0),
    gsSP2Triangles(10, 12, 19, 0, 12, 15, 19, 0),
    gsSP2Triangles(19, 15, 20, 0, 3, 20, 15, 0),
    gsSP2Triangles(5, 21, 3, 0, 21, 20, 3, 0),
    gsSP2Triangles(10, 18, 8, 0, 22, 23, 24, 0),
    gsSP2Triangles(23, 25, 24, 0, 25, 26, 24, 0),
    gsSP2Triangles(26, 27, 24, 0, 27, 22, 24, 0),
    gsSPEndDisplayList(),
};

// tall boulders
extern u16 tallBombiwaTex[];
extern u64 object_bombiwa_Tex_005030[];
extern Vtx object_bombiwaVtx_0042C0[];

Gfx randoTallBombiwaDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL1, 0, TEXEL0, 0, 0, 0, 0, 1, COMBINED, 0, SHADE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsSPDisplayList(0x08000000),
    gsDPSetTextureLUT(G_TT_NONE),
    // gsDPLoadTextureBlock(object_bombiwa_Tex_004830, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
    //                      G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadTextureBlock(tallBombiwaTex, G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadMultiBlock_4b(object_bombiwa_Tex_005030, 0x0100, 1, G_IM_FMT_I, 64, 64, 15, G_TX_NOMIRROR | G_TX_WRAP,
                          G_TX_MIRROR | G_TX_CLAMP, 6, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    // gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(object_bombiwaVtx_0042C0, 22, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 2, 0),
    gsSP2Triangles(4, 5, 6, 0, 5, 7, 6, 0),
    gsSP2Triangles(4, 8, 9, 0, 8, 10, 9, 0),
    gsSP2Triangles(11, 12, 13, 0, 12, 4, 13, 0),
    gsSP2Triangles(3, 4, 12, 0, 1, 5, 3, 0),
    gsSP2Triangles(4, 3, 5, 0, 8, 4, 6, 0),
    gsSP2Triangles(6, 10, 8, 0, 11, 2, 12, 0),
    gsSP2Triangles(12, 2, 3, 0, 7, 5, 14, 0),
    gsSP2Triangles(7, 10, 6, 0, 14, 15, 16, 0),
    gsSP2Triangles(16, 7, 14, 0, 0, 2, 17, 0),
    gsSP2Triangles(11, 17, 2, 0, 7, 16, 10, 0),
    gsSP2Triangles(11, 18, 19, 0, 20, 0, 17, 0),
    gsSP2Triangles(18, 11, 21, 0, 17, 11, 19, 0),
    gsSP1Triangle(17, 19, 20, 0),
    gsSPEndDisplayList(),
};