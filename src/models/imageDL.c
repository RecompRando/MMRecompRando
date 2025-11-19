#include "ultra64.h"
#include "global.h"

Vtx imageDL_imageDL_mesh_layer_Opaque_vtx_cull[8] = {
	{{ {-36, -36, 0}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-36, -36, 0}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-36, 36, 0}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {-36, 36, 0}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {36, -36, 0}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {36, -36, 0}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {36, 36, 0}, 0, {0, 0}, {0, 0, 0, 0} }},
	{{ {36, 36, 0}, 0, {0, 0}, {0, 0, 0, 0} }},
};

Vtx imageDL_imageDL_mesh_layer_Opaque_vtx_0[4] = {
	{{ {-36, -36, 0}, 0, {-16, 1008}, {0, 0, 127, 255} }},
	{{ {36, -36, 0}, 0, {1008, 1008}, {0, 0, 127, 255} }},
	{{ {36, 36, 0}, 0, {1008, -16}, {0, 0, 127, 255} }},
	{{ {-36, 36, 0}, 0, {-16, -16}, {0, 0, 127, 255} }},
};

Gfx imageDL_imageDL_mesh_layer_Opaque_tri_0[] = {
	gsSPVertex(imageDL_imageDL_mesh_layer_Opaque_vtx_0 + 0, 4, 0),
	gsSP2Triangles(0, 1, 2, 0, 0, 2, 3, 0),
	gsSPEndDisplayList(),
};

Gfx mat_imageDL_image_layerOpaque[] = {
	gsSPLoadGeometryMode(G_ZBUFFER | G_SHADE | G_FOG | G_LIGHTING | G_SHADING_SMOOTH),
	gsDPPipeSync(),
	gsDPSetCombineLERP(TEXEL0, 0, SHADE, 0, 0, 0, 0, TEXEL0, COMBINED, 0, PRIMITIVE, 0, 0, 0, 0, COMBINED),
	gsSPSetOtherMode(G_SETOTHERMODE_H, 4, 20, G_AD_NOISE | G_CD_MAGICSQ | G_CK_NONE | G_TC_FILT | G_TF_BILERP | G_TT_NONE | G_TL_TILE | G_TD_CLAMP | G_TP_PERSP | G_CYC_2CYCLE | G_PM_NPRIMITIVE),
	gsSPSetOtherMode(G_SETOTHERMODE_L, 0, 32, G_AC_NONE | G_ZS_PIXEL | G_RM_FOG_SHADE_A | G_RM_AA_ZB_TEX_EDGE2),
	gsSPTexture(65535, 65535, 0, 0, 1),
	gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
	gsDPSetTextureImage(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 1, 0x08000000),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b_LOAD_BLOCK, 0, 0, 7, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0, G_TX_WRAP | G_TX_NOMIRROR, 0, 0),
	gsDPLoadBlock(7, 0, 0, 1023, 256),
	gsDPSetTile(G_IM_FMT_RGBA, G_IM_SIZ_16b, 8, 0, 0, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0, G_TX_CLAMP | G_TX_NOMIRROR, 5, 0),
	gsDPSetTileSize(0, 0, 0, 124, 124),
	gsSPEndDisplayList(),
};

Gfx imageDL[] = {
	gsSPClearGeometryMode(G_LIGHTING),
	gsSPVertex(imageDL_imageDL_mesh_layer_Opaque_vtx_cull + 0, 8, 0),
	gsSPSetGeometryMode(G_LIGHTING),
	gsSPCullDisplayList(0, 7),
	gsSPDisplayList(mat_imageDL_image_layerOpaque),
	gsSPDisplayList(imageDL_imageDL_mesh_layer_Opaque_tri_0),
	gsSPEndDisplayList(),
};

