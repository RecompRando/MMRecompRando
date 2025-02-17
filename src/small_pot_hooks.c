#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct ObjTsubo;

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_THROW_ONLY | ACTOR_FLAG_CAN_PRESS_SWITCHES)

#define THIS ((ObjTsubo*)thisx)

typedef void (*ObjTsuboActionFunc)(struct ObjTsubo*, PlayState*);

typedef void (*ObjTsuboUnkFunc)(struct ObjTsubo*, PlayState*);

#define OBJ_TSUBO_P000F(thisx) ((thisx)->params & 0x0F)
#define OBJ_TSUBO_P001F(thisx) ((thisx)->params & 0x1F)
#define OBJ_TSUBO_P003F(thisx) ((thisx)->params & 0x3F)
#define OBJ_TSUBO_P0010(thisx) (((thisx)->params >> 4) & 1)
#define OBJ_TSUBO_GET_TYPE(thisx) (((thisx)->params >> 7) & 3)
#define OBJ_TSUBO_PFE00(thisx) (((thisx)->params >> 9) & 0x7F) //item collectable flag?
#define OBJ_TSUBO_ZROT(thisx) ((thisx)->home.rot.z)

#define LOCATION_SKULL_TOKEN (0x060000 | (play->sceneId << 8) | OBJ_TSUBO_P001F(&this->actor))

typedef struct ObjTsubo {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ObjTsuboActionFunc actionFunc;
    /* 0x148 */ ColliderCylinder cylinderCollider;
    /* 0x194 */ s8 unk_194;
    /* 0x195 */ s8 unk_195;
    /* 0x196 */ s8 homeRoom;
    /* 0x197 */ s8 unk_197;
    /* 0x198 */ s8 unk_198;
    /* 0x199 */ s8 objectSlot;
    /* 0x19A */ s8 unk_19A;
    /* 0x19B */ u8 unk_19B;
} ObjTsubo; // size = 0x19C

typedef enum {
    /* 0 */ OBJ_TSUBO_TYPE_0,
    /* 1 */ OBJ_TSUBO_TYPE_1,
    /* 2 */ OBJ_TSUBO_TYPE_2,
    /* 3 */ OBJ_TSUBO_TYPE_3
} ObjTsuboType;

#define ENSW_GETS_3(params) ((params & 3) & 0xFF)
#define ENSW_GET_3(thisx) (ENSW_GETS_3((thisx)->params))
#define ENSW_GETS_3FC(params) (((params & 0x3FC) >> 2) & 0xFF)

RECOMP_PATCH bool func_809275C0(ObjTsubo* this, PlayState* play) {
    s32 chestFlag = -1;
    s32 skulltulaParams = (OBJ_TSUBO_P001F(&this->actor) << 2) | 0xFF01;

    if (rando_location_is_checked(LOCATION_SKULL_TOKEN)) {
        return false;
    }

    if (ENSW_GETS_3(skulltulaParams)) {
        chestFlag = ENSW_GETS_3FC(skulltulaParams);
    }

    return (chestFlag < 0) == true || !Flags_GetTreasure(play, chestFlag);
}