#include "ultra64.h"
#include "global.h"

extern u16 pinkFlowerTex[];
extern u16 goldFlowerTex[];

typedef enum PinkDekuFlowerLimb {
    /* 0x00 */ PINK_DEKU_FLOWER_LIMB_NONE,
    /* 0x01 */ PINK_DEKU_FLOWER_LIMB_BASE,
    /* 0x02 */ PINK_DEKU_FLOWER_LIMB_CENTER,
    /* 0x03 */ PINK_DEKU_FLOWER_LIMB_BACK_LEAF,
    /* 0x04 */ PINK_DEKU_FLOWER_LIMB_FRONT_LEAF,
    /* 0x05 */ PINK_DEKU_FLOWER_LIMB_FRONT_RIGHT_PETAL,
    /* 0x06 */ PINK_DEKU_FLOWER_LIMB_BACK_RIGHT_PETAL,
    /* 0x07 */ PINK_DEKU_FLOWER_LIMB_RIGHT_LEAF,
    /* 0x08 */ PINK_DEKU_FLOWER_LIMB_FRONT_LEFT_PETAL,
    /* 0x09 */ PINK_DEKU_FLOWER_LIMB_LEFT_LEAF,
    /* 0x0A */ PINK_DEKU_FLOWER_LIMB_BACK_LEFT_PETAL,
    /* 0x0B */ PINK_DEKU_FLOWER_LIMB_MAX
} PinkDekuFlowerLimb;

typedef enum GoldDekuFlowerLimb {
    /* 0x00 */ GOLD_DEKU_FLOWER_LIMB_NONE,
    /* 0x01 */ GOLD_DEKU_FLOWER_LIMB_BASE,
    /* 0x02 */ GOLD_DEKU_FLOWER_LIMB_CENTER,
    /* 0x03 */ GOLD_DEKU_FLOWER_LIMB_BACK_PETAL,
    /* 0x04 */ GOLD_DEKU_FLOWER_LIMB_FRONT_PETAL,
    /* 0x05 */ GOLD_DEKU_FLOWER_LIMB_FRONT_RIGHT_PETAL,
    /* 0x06 */ GOLD_DEKU_FLOWER_LIMB_BACK_RIGHT_PETAL,
    /* 0x07 */ GOLD_DEKU_FLOWER_LIMB_RIGHT_PETAL,
    /* 0x08 */ GOLD_DEKU_FLOWER_LIMB_FRONT_LEFT_PETAL,
    /* 0x09 */ GOLD_DEKU_FLOWER_LIMB_LEFT_PETAL,
    /* 0x0A */ GOLD_DEKU_FLOWER_LIMB_BACK_LEFT_PETAL,
    /* 0x0B */ GOLD_DEKU_FLOWER_LIMB_MAX
} GoldDekuFlowerLimb;

// pink
extern Gfx randoPinkDekuFlowerIdleDL[];
extern Gfx randoPinkDekuFlowerBackLeftPetalDL[];
extern Gfx randoPinkDekuFlowerFrontLeftPetalDL[];
extern Gfx randoPinkDekuFlowerBackRightPetalDL[];
extern Gfx randoPinkDekuFlowerFrontRightPetalDL[];

// gold
extern Gfx randoGoldDekuFlowerIdleDL[];
extern Gfx randoGoldDekuFlowerBackLeftPetalDL[];
extern Gfx randoGoldDekuFlowerLeftPetalDL[];
extern Gfx randoGoldDekuFlowerFrontLeftPetalDL[];
extern Gfx randoGoldDekuFlowerRightPetalDL[];
extern Gfx randoGoldDekuFlowerBackRightPetalDL[];
extern Gfx randoGoldDekuFlowerFrontRightPetalDL[];
extern Gfx randoGoldDekuFlowerFrontPetalDL[];
extern Gfx randoGoldDekuFlowerBackPetalDL[];