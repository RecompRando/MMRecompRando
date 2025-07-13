#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Bean/z_obj_bean.h"
#include "overlays/actors/ovl_Obj_Swprize/z_obj_swprize.h"

// ignore how mashed together this is
#define LOCATION_SOFT_SOIL (0x190000 | (play->sceneId << 8) | (OBJ_SWPRIZE_GET_SWITCH_FLAG(&this->actor) + play->roomCtx.curRoom.num) << 4)

// @ap preplant magic beans when receiving the item
RECOMP_HOOK("ObjBean_Init")
void OnObjBean_Init(Actor* thisx, PlayState* play) {
    ObjBean* this = ((ObjBean*)thisx);
    if (OBJBEAN_GET_C000(&this->dyna.actor) == ENOBJBEAN_GET_C000_1) {
        if (rando_has_item(GI_MAGIC_BEANS)) {
            Flags_SetSwitch(play, OBJBEAN_GET_SWITCH_FLAG_1(&this->dyna.actor, 1));
        }
    }
}

extern s16 D_80C257F0[];
extern s16 D_80C257F8[];

void func_80C25360(ObjSwprize* this, Vec3f* vec);

RECOMP_PATCH void func_80C253D0(ObjSwprize* this, PlayState* play) {
    Actor* thisx = &this->actor;
    s32 i;
    Actor* collectible;
    Vec3f sp78;
    s32 type = OBJ_SWPRIZE_GET_TYPE(thisx);
    s32 temp_s0 = D_80C257F0[type];

    func_80C25360(this, &sp78);

    if (type == 2) {
        for (i = 0; i < 3; i++) {
            if (!rando_location_is_checked(LOCATION_SOFT_SOIL | i)) {
                collectible = Item_RandoDropCollectible(play, &thisx->world.pos, ITEM00_APITEM, LOCATION_SOFT_SOIL | i);
            } else {
                collectible = Item_DropCollectible(play, &thisx->world.pos, temp_s0);
            }
            if (collectible != NULL) {
                if (sp78.y < 0.98f) {
                    collectible->velocity.y = (sp78.y + 1.0f) * 4.0f;
                    collectible->speed = (2.0f * (1.0f - fabsf(sp78.y))) + 2.0f;
                    collectible->world.rot.y = Math_Atan2S_XY(sp78.z, sp78.x) + D_80C257F8[i];
                } else {
                    collectible->world.rot.y = i * (0x10000 / 3);
                }
            }
        }
    } else {
        if (!rando_location_is_checked(LOCATION_SOFT_SOIL)) {
            collectible = Item_RandoDropCollectible(play, &thisx->world.pos, ITEM00_APITEM, LOCATION_SOFT_SOIL | 3);
        } else {
            collectible = Item_DropCollectible(play, &thisx->world.pos, temp_s0);
        }
        if ((collectible != NULL) && (sp78.y < 0.98f)) {
            collectible->velocity.y = (sp78.y + 1.0f) * 4.0f;
            collectible->speed = (2.0f * (1.0f - fabsf(sp78.y))) + 2.0f;
            collectible->world.rot.y = Math_Atan2S_XY(sp78.z, sp78.x);
        }
    }
}