#include "ultra64.h"
#include "global.h"

// palm tree

extern Vtx object_obj_yasiVtx_000000[];
// extern Gfx gPalmTreeDL[];
extern u64 gPalmTreeTLUT[];
extern u64 gPalmTreeFrondTex[];
extern u64 gPalmTreeDekuNutTex[];
extern u64 gPalmTreeScarredWoodTex[];
extern u64 gPalmTreeWoodTex[];

Gfx randoPalmTreeDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 3, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL1, TEXEL0, LOD_FRACTION, TEXEL0, TEXEL1, TEXEL0, LOD_FRACTION, TEXEL0, COMBINED, 0, SHADE,
                       0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetTextureLOD(G_TL_LOD),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(gPalmTreeScarredWoodTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadMultiBlock(gPalmTreeScarredWoodTex, 0x0000, 1, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_NOMIRROR |
                       G_TX_WRAP, G_TX_NOMIRROR | G_TX_WRAP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsDPLoadMultiBlock(gPalmTreeWoodTex, 0x0040, 2, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 8, 0, G_TX_NOMIRROR | G_TX_WRAP,
                       G_TX_NOMIRROR | G_TX_WRAP, 3, 3, 1, 1),
    gsDPLoadMultiBlock(gPalmTreeWoodTex, 0x0040, 3, G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 8, 0, G_TX_NOMIRROR | G_TX_WRAP,
                       G_TX_NOMIRROR | G_TX_WRAP, 3, 3, 1, 1),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(object_obj_yasiVtx_000000, 15, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(2, 4, 5, 0, 2, 5, 3, 0),
    gsSP2Triangles(1, 6, 7, 0, 1, 7, 2, 0),
    gsSP2Triangles(3, 5, 8, 0, 3, 8, 9, 0),
    gsSP2Triangles(10, 0, 3, 0, 10, 3, 9, 0),
    gsSP2Triangles(11, 12, 13, 0, 11, 13, 7, 0),
    gsSP2Triangles(6, 14, 11, 0, 6, 11, 7, 0),
    gsSP2Triangles(7, 13, 4, 0, 7, 4, 2, 0),
    gsDPPipeSync(),
    gsDPSetTextureLOD(G_TL_TILE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPPipeSync(),
    gsDPLoadTextureBlock(gPalmTreeDekuNutTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 16, 16, 0, G_TX_MIRROR | G_TX_CLAMP,
                         G_TX_MIRROR | G_TX_CLAMP, 4, 4, G_TX_NOLOD, G_TX_NOLOD),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&object_obj_yasiVtx_000000[15], 9, 0),
    // gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    // gsSP1Triangle(6, 7, 8, 0),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_RGBA16),
    gsDPLoadTLUT_pal16(15, gPalmTreeTLUT),
    gsDPLoadTextureBlock_4b(gPalmTreeFrondTex, G_IM_FMT_CI, 32, 128, 15, G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR |
                            G_TX_CLAMP, 5, 7, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&object_obj_yasiVtx_000000[24], 30, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(4, 6, 5, 0, 6, 7, 8, 0),
    gsSP2Triangles(7, 3, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(12, 9, 11, 0, 13, 12, 14, 0),
    gsSP2Triangles(10, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(16, 18, 17, 0, 18, 19, 20, 0),
    gsSP2Triangles(19, 15, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 21, 23, 0, 25, 24, 26, 0),
    gsSP2Triangles(22, 25, 26, 0, 27, 0, 28, 0),
    gsSP2Triangles(29, 27, 28, 0, 1, 29, 2, 0),
    gsSPEndDisplayList(),
};

// snow trees
extern Vtx object_snowwdVtx_000000[];
extern Gfx gSnowTreeEmptyDL[];
extern Gfx gSnowTreeDL[];
extern u64 gSnowTreeLeavesTex[];
extern u64 gSnowTreeBodyTex[];
extern u64 gSnowTreeSnowLeavesTex[];

Gfx randoSnowTreeDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    // gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    // gsDPSetPrimColor(0, 0xFF, 255, 255, 255, 255),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(0x08000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                         G_TX_CLAMP, 5, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&object_snowwdVtx_000000[13], 12, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPPipeSync(),
    
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetPrimColor(0, 0xFF, 255, 255, 255, 255),

    gsDPLoadTextureBlock(gSnowTreeBodyTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BACK | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(object_snowwdVtx_000000, 13, 0),
    gsSP2Triangles(0, 1, 2, 0, 1, 3, 4, 0),
    gsSP2Triangles(3, 5, 6, 0, 5, 7, 8, 0),
    gsSP2Triangles(9, 10, 11, 0, 10, 0, 12, 0),
    gsSPEndDisplayList(),
};

// wood trees + bushes
extern u16 woodTreeLeavesTex[]; // WOOD_DRAW_TREE_CONICAL
extern u16 woodBushTex[]; // WOOD_DRAW_BUSH_GREEN

extern u64 object_wood02_Tex_000F90[];
extern Vtx object_wood02Vtx_007790[];

extern u64 object_wood02_Tex_004790[];
extern Vtx object_wood02Vtx_000000[];

Gfx randoWood02TreeLeaves[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock_4b(object_wood02_Tex_000F90, G_IM_FMT_IA, 32, 128, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_NOMIRROR |
                            G_TX_CLAMP, 5, 7, G_TX_NOLOD, G_TX_NOLOD),
    // gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, ENVIRONMENT, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    // gsDPSetPrimColor(0, 0, 28, 88, 13, 255),
    gsSPVertex(&object_wood02Vtx_007790[11], 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP1Triangle(6, 7, 8, 0),
    gsSPEndDisplayList(),
};

Gfx randoWood02Bush[] = {
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPLoadTextureBlock(object_wood02_Tex_004790, G_IM_FMT_IA, G_IM_SIZ_8b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP,
                         G_TX_NOMIRROR | G_TX_CLAMP, 5, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_ZB_XLU_SURF2),
    gsSPClearGeometryMode(G_CULL_BACK | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR),
    gsSPSetGeometryMode(G_FOG | G_LIGHTING),
    // gsDPSetPrimColor(0, 0, 102, 152, 112, 255),
    gsSPVertex(object_wood02Vtx_000000, 9, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP1Triangle(6, 7, 8, 0),
    gsSPEndDisplayList(),
};

// normal tree
extern u16 ObjTreeLeavesTex[];
extern Vtx object_treeVtx_000000[];

Gfx randoTreeLeavesDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    // gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, COMBINED, 0, PRIMITIVE, 0),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
    gsDPPipeSync(),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(ObjTreeLeavesTex, G_IM_FMT_IA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR |
                         G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    // gsDPLoadTextureBlock(gTreeLeavesTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_CLAMP, G_TX_MIRROR |
    //                      G_TX_CLAMP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    // gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&object_treeVtx_000000[68], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(2, 4, 5, 0, 2, 5, 3, 0),
    gsSP2Triangles(6, 7, 8, 0, 6, 8, 9, 0),
    gsSP2Triangles(10, 11, 6, 0, 10, 6, 9, 0),
    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    gsSP2Triangles(14, 16, 17, 0, 14, 17, 15, 0),
    gsSP2Triangles(18, 19, 20, 0, 18, 20, 21, 0),
    gsSP2Triangles(20, 22, 23, 0, 20, 23, 21, 0),
    gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0),
    gsSP2Triangles(28, 29, 24, 0, 28, 24, 27, 0),
    gsSPVertex(&object_treeVtx_000000[98], 6, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 0, 0, 4, 0, 3, 0),
    gsSPEndDisplayList(),
};