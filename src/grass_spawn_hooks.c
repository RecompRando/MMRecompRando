#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"

#include "overlays/actors/ovl_Obj_Grass/z_obj_grass.h"
#include "overlays/actors/ovl_Obj_Grass_Unit/z_obj_grass_unit.h"

#define LOCATION_GRASS(grassElem) ObjGrass_GetLocation(grassElem)

#define THIS ((ObjGrass*)thisx)
ObjGrass* gObjGrass;

// modified ootmm implementation
u32 ObjGrass_GetLocation(ObjGrassElement* grass) {
    u32 offset;
    u32 offsetPack;
    u32 offsetBush;
    int groupId; // ootmm packId
    int bushId;

    /* Extract the pack ID */
    offset = (u32)grass - (u32)gObjGrass;
    offsetPack = (offset - offsetof(ObjGrassGroup, elements) - offsetof(ObjGrass, grassGroups));
    groupId = offsetPack / sizeof(ObjGrassGroup);

    /* Extract the bush ID */
    offset = (u32)grass - ((u32)&gObjGrass->grassGroups[groupId]);
    offsetBush = offset - offsetof(ObjGrassGroup, elements);
    bushId = offsetBush / sizeof(ObjGrassElement);

    /* Adjust the current scene */
    PlayState* play = gPlay;
    s16 sceneId = play->sceneId;
    // do stuff if in grotto

    // recomp_printf("pack: %d, bush: %d\n", groupId, bushId);
    return 0x100000 | (groupId << 12) | (sceneId << 4) | bushId;
}

RECOMP_HOOK("ObjGrass_Init")
void OnObjGrass_Init(Actor* thisx, PlayState* play) {
    gObjGrass = THIS;
}

RECOMP_HOOK("ObjGrass_DropCollectible")
void ObjGrass_ReplaceCollectible(ObjGrassElement* grassElem, PlayState* play) {
    if (!rando_location_is_checked(LOCATION_GRASS(grassElem))) {
        Item_DropCollectible(play, &grassElem->pos, ITEM00_HEART_PIECE);
        grassElem->dropTable = 0x10; // disable further drops
    }
}