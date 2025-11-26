#include "modding.h"
#include "global.h"

// from assets/objects/object_rz/object_rz.h
typedef enum ObjectRzLimb {
    /* 0x00 */ OBJECT_RZ_LIMB_NONE,
    /* 0x01 */ OBJECT_RZ_LIMB_01,
    /* 0x02 */ OBJECT_RZ_LIMB_02,
    /* 0x03 */ OBJECT_RZ_LIMB_03,
    /* 0x04 */ OBJECT_RZ_LIMB_04,
    /* 0x05 */ OBJECT_RZ_LIMB_05,
    /* 0x06 */ OBJECT_RZ_LIMB_06,
    /* 0x07 */ OBJECT_RZ_LIMB_07,
    /* 0x08 */ OBJECT_RZ_LIMB_08,
    /* 0x09 */ OBJECT_RZ_LIMB_09,
    /* 0x0A */ OBJECT_RZ_LIMB_0A,
    /* 0x0B */ OBJECT_RZ_LIMB_0B,
    /* 0x0C */ OBJECT_RZ_LIMB_0C,
    /* 0x0D */ OBJECT_RZ_LIMB_0D,
    /* 0x0E */ OBJECT_RZ_LIMB_0E,
    /* 0x0F */ OBJECT_RZ_LIMB_0F,
    /* 0x10 */ OBJECT_RZ_LIMB_10,
    /* 0x11 */ OBJECT_RZ_LIMB_11,
    /* 0x12 */ OBJECT_RZ_LIMB_12,
    /* 0x13 */ OBJECT_RZ_LIMB_13,
    /* 0x14 */ OBJECT_RZ_LIMB_14,
    /* 0x15 */ OBJECT_RZ_LIMB_15,
    /* 0x16 */ OBJECT_RZ_LIMB_MAX
} ObjectRzLimb;


// from overlays/actors/ovl_En_Rz/z_en_rz.h
#define EN_RZ_GET_SISTER(thisx) ((thisx)->params & 0x8000)
#define EN_RZ_GET_TYPE(thisx) ((thisx)->params & 0xF)
#define EN_RZ_GET_PATH_INDEX(thisx) (((thisx)->params & 0x7E00) >> 9)

#define EN_RZ_PATH_INDEX_NONE 0x3F

typedef enum {
    /* 0 */ EN_RZ_JUDO, // in red
    /* 1 */ EN_RZ_MARILLA // in blue
} EnRzSister;

typedef enum {
    /* 0 */ EN_RZ_TYPE_0,
    /* 1 */ EN_RZ_TYPE_1,
    /* 2 */ EN_RZ_TYPE_2
} EnRzType;

struct EnRz;

typedef void (*EnRzActionFunc)(struct EnRz*, PlayState*);

typedef struct EnRz {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[OBJECT_RZ_LIMB_MAX];
    /* 0x20C */ UNK_TYPE1 pad20C[0x94];
    /* 0x2A0 */ Vec3s morphTable[OBJECT_RZ_LIMB_MAX];
    /* 0x324 */ UNK_TYPE1 pad324[0x94];
    /* 0x3B8 */ Path* path;
    /* 0x3BC */ s32 curPointIndex;
    /* 0x3C0 */ ColliderCylinder collider;
    /* 0x40C */ struct EnRz* sister;
    /* 0x410 */ s16 eyeIndex;
    /* 0x412 */ s16 blinkTimer;
    /* 0x414 */ Vec3f shadowPos;
    /* 0x420 */ u16 stateFlags;
    /* 0x422 */ s16 animIndex;
    /* 0x424 */ s16 timer;
    /* 0x426 */ u16 cueType;
    /* 0x428 */ u16 cueId;
    /* 0x42A */ s16 csIdList[2];
    /* 0x430 */ EnRzActionFunc actionFunc;
} EnRz; // size = 0x434

// patches

RECOMP_PATCH s32 func_80BFBFAC(EnRz* this, PlayState* play) {
    Message_StartTextbox(play, 0x1000, NULL);
    Message_CloseTextbox(play);
    return true;
}