#include "ultra64.h"
#include "global.h"

#include "container_textures.h"

extern Vtx object_boxVtx_0004B0[];
extern Vtx object_boxVtx_001998[];
extern Vtx object_boxVtx_000EC8[];

Gfx randoChestBaseDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(0x0A000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_WRAP,
                         G_TX_MIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPDisplayList(0x08000000),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(object_boxVtx_0004B0, 28, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
    gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0),
    gsDPPipeSync(),
    gsDPLoadTextureBlock(0x09000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR
                         | G_TX_WRAP, 5, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(&object_boxVtx_0004B0[28], 8, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSPEndDisplayList(),
};

Gfx randoChestLidDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(0x0A000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_WRAP,
                         G_TX_MIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPDisplayList(0x08000000),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(object_boxVtx_001998, 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
    gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0),
    gsSP1Triangle(28, 29, 30, 0),
    gsSPVertex(&object_boxVtx_001998[31], 29, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 2, 0),
    gsSP2Triangles(5, 6, 2, 0, 7, 2, 8, 0),
    gsSP2Triangles(9, 10, 11, 0, 9, 11, 12, 0),
    gsSP2Triangles(13, 14, 15, 0, 13, 15, 16, 0),
    gsSP2Triangles(17, 18, 19, 0, 17, 20, 21, 0),
    gsSP2Triangles(17, 22, 23, 0, 24, 25, 26, 0),
    gsSP1Triangle(25, 27, 28, 0),
    gsSPEndDisplayList(),
};

// could be changed if we need more wooden lids
Gfx randoChestLidJunkDL[] = {
    gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON),
    gsDPPipeSync(),
    gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_OPA_SURF2),
    gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, PRIMITIVE, 0, COMBINED, 0, 0, 0, 0, COMBINED),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPLoadTextureBlock(0x0A000000, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_MIRROR | G_TX_WRAP,
                         G_TX_MIRROR | G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD),
    gsDPSetPrimColor(0, 0x80, 255, 255, 255, 255),
    gsSPDisplayList(0x08000000),
    gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
    gsSPVertex(object_boxVtx_000EC8, 30, 0),
    gsSP2Triangles(0, 1, 2, 0, 3, 4, 5, 0),
    gsSP2Triangles(6, 7, 8, 0, 9, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 15, 16, 17, 0),
    gsSP2Triangles(18, 19, 20, 0, 21, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 27, 28, 29, 0),
    gsDPPipeSync(),
    gsDPLoadTextureBlock(apJunkFrontBlankTex, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 64, 0, G_TX_MIRROR | G_TX_WRAP, G_TX_MIRROR
                         | G_TX_WRAP, 5, 6, G_TX_NOLOD, G_TX_NOLOD),
    gsSPVertex(&object_boxVtx_000EC8[30], 32, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
    gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
    gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
    gsSP2Triangles(16, 17, 18, 0, 16, 18, 19, 0),
    gsSP2Triangles(20, 21, 22, 0, 20, 22, 23, 0),
    gsSP2Triangles(24, 25, 26, 0, 24, 26, 27, 0),
    gsSP2Triangles(28, 29, 30, 0, 28, 30, 31, 0),
    gsSPVertex(&object_boxVtx_000EC8[62], 4, 0),
    gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
    gsSPEndDisplayList(),
};

// fast64 export for unmirrored/assymetric chest face
Vtx randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_vtx_cull[8] = {
	{{ {-2525, 2, -1942}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-2525, 2, 1962}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-2525, 2733, 1962}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-2525, 2733, -1942}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {2525, 2, -1942}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {2525, 2, 1962}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {2525, 2733, 1962}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {2525, 2733, -1942}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_vtx_0[4] = {
	{{ {0, 2, -1942}, 0, {1009, 2116}, {0, 0, 129, 255} }},
	{{ {0, 2733, -1942}, 0, {1009, -95}, {0, 0, 129, 255} }},
	{{ {2525, 2733, -1942}, 0, {-68, -95}, {0, 0, 129, 255} }},
	{{ {2525, 2, -1942}, 0, {-68, 2116}, {0, 0, 129, 255} }},
};

Gfx randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_tri_0[] = {
	gsSPVertex(randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Vtx randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_vtx_1[4] = {
	{{ {-2525, 2, -1942}, 0, {1064, 2116}, {0, 0, 129, 255} }},
	{{ {-2525, 2733, -1942}, 0, {1064, -95}, {0, 0, 129, 255} }},
	{{ {0, 2733, -1942}, 0, {-13, -95}, {0, 0, 129, 255} }},
	{{ {0, 2, -1942}, 0, {-13, 2116}, {0, 0, 129, 255} }},
};

Gfx randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_tri_1[] = {
	gsSPVertex(randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_vtx_1 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Vtx randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_vtx_2[35] = {
	{{ {2525, 2733, 1962}, 0, {2084, -95}, {0, 0, 127, 255} }},
	{{ {-2525, 2733, 1962}, 0, {-36, -95}, {0, 0, 127, 255} }},
	{{ {-2525, 2, 1962}, 0, {-36, 2128}, {0, 0, 127, 255} }},
	{{ {2525, 2, 1962}, 0, {2084, 2128}, {0, 0, 127, 255} }},
	{{ {2525, 2733, 1962}, 0, {-57, -95}, {127, 0, 0, 255} }},
	{{ {2525, 2, 1962}, 0, {-57, 2143}, {127, 0, 0, 255} }},
	{{ {2525, 2, -1942}, 0, {2105, 2143}, {127, 0, 0, 255} }},
	{{ {2525, 2733, -1942}, 0, {2105, -95}, {127, 0, 0, 255} }},
	{{ {-2525, 2, -1942}, 0, {-57, 2143}, {129, 0, 0, 255} }},
	{{ {-2525, 2, 1962}, 0, {2105, 2143}, {129, 0, 0, 255} }},
	{{ {-2525, 2733, 1962}, 0, {2105, -95}, {129, 0, 0, 255} }},
	{{ {-2525, 2733, -1942}, 0, {-57, -95}, {129, 0, 0, 255} }},
	{{ {2525, 2733, -1942}, 0, {-53, -51}, {0, 127, 0, 255} }},
	{{ {0, 2733, -1942}, 0, {1024, -51}, {0, 127, 0, 255} }},
	{{ {-2525, 2733, -1942}, 0, {2101, -51}, {0, 127, 0, 255} }},
	{{ {-2225, 2733, -1642}, 0, {1973, 34}, {0, 127, 0, 255} }},
	{{ {2225, 2733, -1642}, 0, {75, 34}, {0, 127, 0, 255} }},
	{{ {-2225, 2733, 1662}, 0, {1938, -88}, {0, 127, 0, 255} }},
	{{ {-2225, 2733, -1638}, 0, {111, -88}, {0, 127, 0, 255} }},
	{{ {-2525, 2733, -1942}, 0, {-57, 88}, {0, 127, 0, 255} }},
	{{ {-2525, 2733, 1962}, 0, {2104, 88}, {0, 127, 0, 255} }},
	{{ {2525, 197, -1942}, 0, {-53, 2135}, {0, 127, 0, 255} }},
	{{ {-2525, 197, -1942}, 0, {2101, 2135}, {0, 127, 0, 255} }},
	{{ {-2525, 197, 1962}, 0, {2101, -87}, {0, 127, 0, 255} }},
	{{ {2525, 197, 1962}, 0, {-53, -87}, {0, 127, 0, 255} }},
	{{ {-2525, 2733, 1962}, 0, {-2, 82}, {0, 127, 0, 255} }},
	{{ {2525, 2733, 1962}, 0, {2050, 82}, {0, 127, 0, 255} }},
	{{ {2225, 2733, 1662}, 0, {1928, -82}, {0, 127, 0, 255} }},
	{{ {-2225, 2733, 1662}, 0, {120, -82}, {0, 127, 0, 255} }},
	{{ {2225, 2733, -1642}, 0, {1938, -88}, {0, 127, 0, 255} }},
	{{ {2225, 2733, 1658}, 0, {111, -88}, {0, 127, 0, 255} }},
	{{ {2525, 2733, 1962}, 0, {-57, 88}, {0, 127, 0, 255} }},
	{{ {2225, 2733, -1642}, 0, {1938, -88}, {0, 127, 0, 255} }},
	{{ {2525, 2733, 1962}, 0, {-57, 88}, {0, 127, 0, 255} }},
	{{ {2525, 2733, -1942}, 0, {2104, 88}, {0, 127, 0, 255} }},
};

Gfx randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_tri_2[] = {
	gsSPVertex(randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_vtx_2 + 0, 32, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSP2Triangles(4, 5, 6, 0, 4, 6, 7, 0),
	gsSP2Triangles(8, 9, 10, 0, 8, 10, 11, 0),
	gsSP2Triangles(12, 13, 14, 0, 12, 14, 15, 0),
	gsSP2Triangles(12, 15, 16, 0, 17, 18, 19, 0),
	gsSP2Triangles(17, 19, 20, 0, 21, 22, 23, 0),
	gsSP2Triangles(21, 23, 24, 0, 25, 26, 27, 0),
	gsSP2Triangles(25, 27, 28, 0, 29, 30, 31, 0),
	gsSPVertex(randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_vtx_2 + 32, 3, 0),
	gsSP1Triangle(0, 1, 2, 0),
	gsSPEndDisplayList(),
};

Gfx mat_randoChestBaseAsymmetricDL_f3dlite_material_005_layerOpaque[] = {
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, 0x09000000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_MIRROR, 6, 0, G_TX_WRAP | G_TX_MIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 252),
	gsSPDisplayList(0x8000000),
	gsSPEndDisplayList(),
};

Gfx mat_randoChestBaseAsymmetricDL_f3dlite_material_014_layerOpaque[] = {
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, 0x0B000000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 2047, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_MIRROR, 6, 0, G_TX_WRAP | G_TX_MIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 252),
	gsSPDisplayList(0x8000000),
	gsSPEndDisplayList(),
};

Gfx mat_randoChestBaseAsymmetricDL_f3dlite_material_008_layerOpaque[] = {
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, ENVIRONMENT, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_OPA_SURF2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, 0x0A000000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_WRAP | G_TX_MIRROR, 5, 0, G_TX_WRAP | G_TX_MIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPDisplayList(0x8000000),
	gsSPEndDisplayList(),
};

Gfx randoChestBaseAsymmetricDL[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_randoChestBaseAsymmetricDL_f3dlite_material_005_layerOpaque),
	gsSPDisplayList(randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_tri_0),
	gsSPDisplayList(mat_randoChestBaseAsymmetricDL_f3dlite_material_014_layerOpaque),
	gsSPDisplayList(randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_tri_1),
	gsSPDisplayList(mat_randoChestBaseAsymmetricDL_f3dlite_material_008_layerOpaque),
	gsSPDisplayList(randoChestBaseAsymmetricDL_randoChestBaseAsymmetricDL_mesh_layer_Opaque_tri_2),
	gsSPEndDisplayList(),
};