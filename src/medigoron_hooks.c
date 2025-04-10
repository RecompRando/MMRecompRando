#include "modding.h"
#include "global.h"

#include "apcommon.h"

#include "z64msgevent.h"

#define WEEKEVENTREG_HAS_POWDERKEG_PRIVILEGES PACK_WEEKEVENTREG_FLAG(18, 0x80)

struct EnGo;

typedef void (*EnGoActionFunc)(struct EnGo*, PlayState*);
typedef s32 (*MsgEventFunc)(Actor*, PlayState*);

#define ENGO_GET_TYPE(thisx) (((thisx)->params & 0xF) & 0xFF)
#define ENGO_GET_SUBTYPE(thisx) ((((thisx)->params & 0x70) >> 4) & 0xFF)

#define GORON_LIMB_MAX 0x12
#define ENGO_FIDGET_TABLE_LEN 3
#define ENGO_SNOW_EFFECT_COUNT 16
#define ENGO_OTHER_EFFECT_COUNT 16
#define ENGO_EFFECT_COUNT (ENGO_SNOW_EFFECT_COUNT + ENGO_OTHER_EFFECT_COUNT)

typedef struct EnGoEffect {
    /* 0x00 */ u8 type;
    /* 0x01 */ u8 alphaDenom;
    /* 0x02 */ u8 alphaNumer;
    /* 0x04 */ Vec3s rotVelocity;
    /* 0x0A */ Vec3s rotAngle;
    /* 0x10 */ Vec3f pos;
    /* 0x1C */ Vec3f accel;
    /* 0x28 */ Vec3f velocity;
    /* 0x34 */ f32 scaleXY;
    /* 0x38 */ f32 scaleXYDelta;
} EnGoEffect; // size = 0x3C;

typedef struct EnGo {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnGoActionFunc actionFunc;
    /* 0x18C */ EnGoActionFunc interruptedActionFunc;
    /* 0x190 */ EnGoActionFunc graveyardDialogActionFunc;
    /* 0x194 */ ColliderCylinder colliderCylinder;
    /* 0x1E0 */ ColliderCylinder unusedCylinder;
    /* 0x22C */ ColliderSphere colliderSphere;
    /* 0x284 */ Path* gatekeeperPath;
    /* 0x288 */ s8 taisouObjectSlot;
    /* 0x289 */ s8 hakuginDemoObjectSlot;
    /* 0x28C */ s32 msgScriptResumePos;
    /* 0x290 */ Vec3f headPos;
    /* 0x29C */ Vec3f bodyPos;
    /* 0x2A8 */ Vec3s headRot;
    /* 0x2AE */ Vec3s bodyRot;
    /* 0x2B4 */ Vec3s jointTable[GORON_LIMB_MAX];
    /* 0x320 */ Vec3s morphTable[GORON_LIMB_MAX];
    /* 0x38C */ Actor* attentionTarget;
    /* 0x390 */ u16 actionFlags;
    /* 0x392 */ u16 lastTextId;
    /* 0x394 */ u8 springArrivalCueId;
    /* 0x398 */ f32 animPlaySpeed;
    /* 0x39C */ f32 iceBlockScale;
    /* 0x3A0 */ f32 iceBlockAlpha;
    /* 0x3A4 */ f32 scaleFactor;
    /* 0x3A8 */ f32 scaleFactorPoundDistortion;
    /* 0x3AC */ UNK_TYPE1 unk3AC[0x2];
    /* 0x3AE */ union {
        s16 snorePhase;      // Used whilst a Goron is sleeping, +0x400 at a time
        s16 elapsedHangtime; // Hangtime at the top of a pound.
    };
    /* 0x3B0 */ s16 headRotZ;
    /* 0x3B2 */ s16 headRotY;
    /* 0x3B4 */ s16 bodyRotZ;
    /* 0x3B6 */ s16 bodyRotY;
    /* 0x3B8 */ s16 csId;
    /* 0x3BA */ s16 loseAttentionTimer;
    /* 0x3BC */ s16 blinkTimer;
    /* 0x3BE */ s16 eyeTexIndex;
    /* 0x3C0 */ s16 cutsceneState;
    /* 0x3C2 */ s16 cutsceneDelayTimer;
    /* 0x3C4 */ s16 gatekeeperAnimState;
    /* 0x3C6 */ s16 harmlessTimer;
    /* 0x3C8 */ s16 fidgetTableZ[ENGO_FIDGET_TABLE_LEN];
    /* 0x3CE */ s16 fidgetTableY[ENGO_FIDGET_TABLE_LEN];
    /* 0x3D4 */ s16 surprisePhase;
    /* 0x3D8 */ MsgEventFunc msgEventFunc;
    /* 0x3DC */ s32 animIndex;
    /* 0x3E0 */ UNK_TYPE1 unk3E0[0x4];
    /* 0x3E4 */ s32 indexPathPoint;
    /* 0x3E8 */ s32 indexEffect;
    /* 0x3EC */ s32 sleepState;
    /* 0x3F0 */ s32 springArrivalCutsceneActive;
    /* 0x3F4 */ s32 changedText;
    /* 0x3F8 */ EnGoEffect effectTable[ENGO_EFFECT_COUNT];
} EnGo; // size = 0xB78

typedef enum EnGoType {
    /* 0 */ ENGO_F_0,
    /* 1 */ ENGO_ATHLETIC,         // Racers stretching before the race
    /* 2 */ ENGO_SPECTATOR,        // Spectators to the race
    /* 3 */ ENGO_GATEKEEPER,       // Gatekeeper to the Goron Shrine
    /* 4 */ ENGO_GRAVEYARD,        // Gravemaker and other
    /* 5 */ ENGO_ASIDE_STORE,      // Shrine Goron outside store
    /* 6 */ ENGO_ASIDE_ELDERSROOM, // Shrine Goron outside Elder's room
    /* 7 */ ENGO_ASIDE_ELDERSSON,  // Shrine Goron aside the Elder's Son
    /* 8 */ ENGO_MEDIGORON         // Medigoron selling Powder Kegs
} EnGoType;

typedef enum EnGoRacetrackSubtype {
    /* 0 */ ENGO_ATHLETIC_A,
    /* 1 */ ENGO_ATHLETIC_B,
    /* 2 */ ENGO_ATHLETIC_C,
    /* 3 */ ENGO_ATHLETIC_D,
    /* 4 */ ENGO_ATHLETIC_HAMSTRINGSIT,
    /* 5 */ ENGO_ATHLETIC_HAMSTRINGSTAND,
    /* 6 */ ENGO_SPECTATOR_CHEER,
    /* 7 */ ENGO_SPECTATOR_SHOUTING
} EnGoRacetrackSubtype;

#define ENGO_AWAKE 0x0

// MsgEvent script for the Goron who made Darmani's grave in the mountain village.
static MsgScript sMsgScriptGoronGravemaker[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_THAWED_GRAVEYARD_GORON, 0x0045 - 0x0005),
    /* 0x0005 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x002A - 0x0008),
    /* 0x0008 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_21_02, 0x0025 - 0x000D),
    /* 0x000D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E10),
    /* 0x0010 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0011 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E11),
    /* 0x0014 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0015 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E12),
    /* 0x0018 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0019 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E13),
    /* 0x001C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001D 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E14),
    /* 0x0020 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0021 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_21_02),
    /* 0x0024 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0025 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E15),
    /* 0x0028 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0029 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x002A 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_GORON_GRAVEMAKER_AS_GORON, 0x0040 - 0x002F),
    /* 0x002F 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E16),
    /* 0x0032 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0033 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E17),
    /* 0x0036 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0037 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E18),
    /* 0x003A 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x003B 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_GORON_GRAVEMAKER_AS_GORON),
    /* 0x003E 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x003F 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0040 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E19),
    /* 0x0043 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0044 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0045 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_GORON_GRAVEMAKER_AS_GORON, 0x0057 - 0x004A),
    /* 0x004A 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0052 - 0x004D),
    /* 0x004D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E31),
    /* 0x0050 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0051 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0052 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E2F),
    /* 0x0055 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0056 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0057 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x005F - 0x005A),
    /* 0x005A 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E2D),
    /* 0x005D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x005E 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x005F 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E2B),
    /* 0x0062 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0063 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for the frozen Goron aside the gravemaker in the mountain village.
static MsgScript sMsgScriptGoronFrozen[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_THAWED_GRAVEYARD_GORON, 0x0083 - 0x0005),
    /* 0x0005 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0055 - 0x0008),
    /* 0x0008 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_GORON_GRAVEMAKER_AS_GORON, 0x0025 - 0x000D),
    /* 0x000D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E1A),
    /* 0x0010 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0011 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0012 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E23),
    /* 0x0015 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0016 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0017 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E24),
    /* 0x001A 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001B 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E25),
    /* 0x001E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001F 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0020 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x0021 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_THAWED_GRAVEYARD_GORON),
    /* 0x0024 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0025 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E1A),
    /* 0x0028 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0029 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x002A 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E23),
    /* 0x002D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002E 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x002F 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E24),
    /* 0x0032 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0033 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E25),
    /* 0x0036 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0037 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0038 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E26),
    /* 0x003B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x003C 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x003D 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E27),
    /* 0x0040 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0041 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0042 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E28),
    /* 0x0045 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0046 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0047 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E29),
    /* 0x004A 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x004B 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x004C 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E2A),
    /* 0x004F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0050 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x0051 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_THAWED_GRAVEYARD_GORON),
    /* 0x0054 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0055 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E1A),
    /* 0x0058 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0059 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x005A 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E1B),
    /* 0x005D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x005E 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x005F 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E1C),
    /* 0x0062 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0063 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E1D),
    /* 0x0066 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0067 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E1E),
    /* 0x006A 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x006B 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x006C 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E1F),
    /* 0x006F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0070 0x01 */ MSCRIPT_CMD_FOCUS_TO_SELF(),
    /* 0x0071 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E20),
    /* 0x0074 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0075 0x01 */ MSCRIPT_CMD_FOCUS_TO_CHILD(),
    /* 0x0076 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E21),
    /* 0x0079 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x007A 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E22),
    /* 0x007D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x007E 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x007F 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_THAWED_GRAVEYARD_GORON),
    /* 0x0082 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0083 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_TALKED_GORON_GRAVEMAKER_AS_GORON, 0x0095 - 0x0088),
    /* 0x0088 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0090 - 0x008B),
    /* 0x008B 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E32),
    /* 0x008E 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x008F 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0090 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E30),
    /* 0x0093 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0094 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0095 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x009D - 0x0098),
    /* 0x0098 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E2E),
    /* 0x009B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x009C 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x009D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E2C),
    /* 0x00A0 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00A1 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for the shrine Goron standing aside the store.
static MsgScript sMsgScriptGoronAsideStore[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E52),
    /* 0x0003 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0004 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for the shrine Goron standing aside the Elder's son.
static MsgScript sMsgScriptGoronAsideEldersSon[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_CALMED_GORON_ELDERS_SON, 0x003D - 0x0005),
    /* 0x0005 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0018 - 0x0008),
    /* 0x0008 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E43),
    /* 0x000B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000C 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E44),
    /* 0x000F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0010 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E45),
    /* 0x0013 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0014 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_24_80),
    /* 0x0017 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0018 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_22_08, 0x0038 - 0x001D),
    /* 0x001D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E46),
    /* 0x0020 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0021 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E47),
    /* 0x0024 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0025 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E48),
    /* 0x0028 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0029 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E49),
    /* 0x002C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002D 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E4A),
    /* 0x0030 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0031 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_22_08),
    /* 0x0034 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_24_80),
    /* 0x0037 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0038 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E4B),
    /* 0x003B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x003C 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x003D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E42),
    /* 0x0040 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0041 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for the shrine Goron standing aside the Elder's room.
static MsgScript sMsgScriptGoronAsideEldersRoom[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_CALMED_GORON_ELDERS_SON, 0x0027 - 0x0005),
    /* 0x0005 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0011 - 0x0008),
    /* 0x0008 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E4D),
    /* 0x000B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000C 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E4E),
    /* 0x000F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0010 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0011 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_23_01, 0x0022 - 0x0016),
    /* 0x0016 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E4F),
    /* 0x0019 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001A 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0E50),
    /* 0x001D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001E 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_23_01),
    /* 0x0021 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0022 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E51),
    /* 0x0025 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0026 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0027 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E4C),
    /* 0x002A 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x002B 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for the Goron in Goron Village who sells Powder Kegs.
static MsgScript sMsgScriptGoronMedigoron[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0015 - 0x0003),
    /* 0x0003 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_18_20, 0x0010 - 0x0008),
    /* 0x0008 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C8E),
    /* 0x000B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000C 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_18_20),
    /* 0x000F 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0010 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C8F),
    /* 0x0013 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0014 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0015 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_18_40, 0x0037 - 0x001A),
    //! @bug Incorrect sfx
    //! The text associated with 0x0C80 makes use of an incorrect pop sound (`NA_SE_EN_SLIME_BREAK`)
    /* 0x001A 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C80),
    /* 0x001D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x001E 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_18_40),
    /* 0x0021 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_HAS_POWDERKEG_PRIVILEGES, 0x009A - 0x0026),
    /* 0x0026 0x05 */ // MSCRIPT_CMD_CHECK_ITEM(ITEM_POWDER_KEG, 0x009A - 0x002B),
    /* 0x0026 0x02 */ MSCRIPT_CMD_JUMP(0x2),
    /* 0x0027 0x01 */ 0x00,
    /* 0x0028 0x01 */ 0x00,
    /* 0x002B 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_19_01, 0x0075 - 0x0030),
    /* 0x0030 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0C81),
    /* 0x0033 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0034 0x03 */ MSCRIPT_CMD_JUMP(0x004A - 0x0037),
    /* 0x0037 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_HAS_POWDERKEG_PRIVILEGES, 0x009A - 0x003C),
    /* 0x003C 0x05 */ // MSCRIPT_CMD_CHECK_ITEM(ITEM_POWDER_KEG, 0x009A - 0x0041),
    /* 0x003C 0x02 */ MSCRIPT_CMD_JUMP(0x2),
    /* 0x003D 0x01 */ 0x00,
    /* 0x003E 0x01 */ 0x00,
    /* 0x0041 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_19_01, 0x0075 - 0x0046),
    /* 0x0046 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C81),
    /* 0x0049 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x004A 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0C82),
    /* 0x004D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x004E 0x07 */ MSCRIPT_CMD_CHECK_TEXT_CHOICE(0x0, 0x006F - 0x0055, 0x006F - 0x0055),
    /* 0x0055 0x01 */ MSCRIPT_CMD_PLAY_DECIDE(),
    /* 0x0056 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C83),
    /* 0x0059 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x005A 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x005B 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x005E 0x03 */ MSCRIPT_CMD_AUTOTALK(0x0),
    /* 0x0061 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C84),
    /* 0x0064 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x0065 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0066 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_19_01),
    /* 0x0069 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x006A 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C84),
    /* 0x006D 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x006E 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x006F 0x01 */ MSCRIPT_CMD_PLAY_CANCEL(),
    /* 0x0070 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C85),
    /* 0x0073 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0074 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0075 0x03 */ MSCRIPT_CMD_CHECK_HAS_POWDER_KEG(0x006A - 0x0078),

    /* 0x0078 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_19_02, 0x008C - 0x007D),
    /* 0x007D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C88),
    /* 0x0080 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0081 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0C89),
    /* 0x0084 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0085 0x07 */ MSCRIPT_CMD_CHECK_TEXT_CHOICE(0x0096 - 0x008C, 0x006F - 0x008C, 0x006F - 0x008C),

    /* 0x008C 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C86),
    /* 0x008F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0090 0x03 */ MSCRIPT_CMD_SET_WEEK_EVENT_REG(WEEKEVENTREG_HAS_POWDERKEG_PRIVILEGES),
    /* 0x0093 0x03 */ MSCRIPT_CMD_JUMP(0x00B7 - 0x0096),
    /* 0x0096 0x01 */ MSCRIPT_CMD_PLAY_DECIDE(),
    /* 0x0097 0x03 */ MSCRIPT_CMD_JUMP(0x005A - 0x009A),

    /* 0x009A 0x03 */ MSCRIPT_CMD_CHECK_HAS_POWDER_KEG(0x00C9 - 0x009D),
    /* 0x009D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C8C),
    /* 0x00A0 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00A1 0x07 */ MSCRIPT_CMD_CHECK_TEXT_CHOICE(0x0, 0x00AD - 0x00A8, 0x0),
    /* 0x00A8 0x05 */ MSCRIPT_CMD_CHECK_RUPEES(100, 0x00B3 - 0x00AD),
    /* 0x00AD 0x01 */ MSCRIPT_CMD_PLAY_ERROR(),
    /* 0x00AE 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C8D),
    /* 0x00B1 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00B2 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x00B3 0x01 */ MSCRIPT_CMD_PLAY_DECIDE(),
    /* 0x00B4 0x03 */ MSCRIPT_CMD_CHANGE_RUPEES(-100),
    /* 0x00B7 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x00B8 0x05 */ MSCRIPT_CMD_OFFER_ITEM(GI_POWDER_KEG, 0x0),
    /* 0x00BD 0x03 */ MSCRIPT_CMD_SET_COLLECTIBLE(0x0034),
    /* 0x00C0 0x03 */ MSCRIPT_CMD_AUTOTALK(0x0),
    /* 0x00C3 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C87),
    /* 0x00C6 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00C7 0x01 */ MSCRIPT_CMD_UNSET_AUTOTALK(),
    /* 0x00C8 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x00C9 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0C8B),
    /* 0x00CC 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x00CD 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for the gatekeeper Goron in Goron Village who opens the shrine gate.
static MsgScript sMsgScriptGoronGatekeeper[] = {
    /* 0x0000 0x05 */ MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_GATEKEEPER_OPENED_GORON_SHRINE, 0x002E - 0x0005),
    /* 0x0005 0x03 */ MSCRIPT_CMD_CHECK_DEKU(0x0016 - 0x0008),
    /* 0x0008 0x03 */ MSCRIPT_CMD_CHECK_ZORA(0x001E - 0x000B),
    /* 0x000B 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0026 - 0x000E),
    /* 0x000E 0x05 */
    MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_GATEKEEPER_OPENED_GORON_SHRINE_FOR_HUMAN, 0x0064 - 0x0013),
    /* 0x0013 0x03 */ MSCRIPT_CMD_JUMP(0x0034 - 0x0016),
    /* 0x0016 0x05 */
    MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_GATEKEEPER_OPENED_GORON_SHRINE_FOR_DEKU, 0x0064 - 0x001B),
    /* 0x001B 0x03 */ MSCRIPT_CMD_JUMP(0x0034 - 0x001E),
    /* 0x001E 0x05 */
    MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_GATEKEEPER_OPENED_GORON_SHRINE_FOR_ZORA, 0x0064 - 0x0023),
    /* 0x0023 0x03 */ MSCRIPT_CMD_JUMP(0x0034 - 0x0026),
    /* 0x0026 0x05 */
    MSCRIPT_CMD_CHECK_WEEK_EVENT_REG(WEEKEVENTREG_GATEKEEPER_OPENED_GORON_SHRINE_FOR_GORON, 0x0064 - 0x002B),
    /* 0x002B 0x03 */ MSCRIPT_CMD_JUMP(0x005D - 0x002E),
    /* 0x002E 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0D4D),
    /* 0x0031 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0032 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0033 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0034 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0D48),
    /* 0x0037 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0038 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0D49),
    /* 0x003B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x003C 0x03 */ MSCRIPT_CMD_CONTINUE_TEXT(0x0D4A),
    /* 0x003F 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0040 0x07 */ MSCRIPT_CMD_CHECK_TEXT_CHOICE(0x0, 0x0056 - 0x0047, 0x0056 - 0x0047),
    /* 0x0047 0x01 */ MSCRIPT_CMD_PLAY_DECIDE(),
    /* 0x0048 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0D4B),
    /* 0x004B 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x004C 0x01 */ MSCRIPT_CMD_PAUSE(),
    /* 0x004D 0x03 */ MSCRIPT_CMD_CHECK_CALLBACK(0x0),
    /* 0x0050 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0D4D),
    /* 0x0053 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0054 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0055 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0056 0x01 */ MSCRIPT_CMD_PLAY_CANCEL(),
    /* 0x0057 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0D4C),
    /* 0x005A 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x005B 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x005C 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x005D 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0D4E),
    /* 0x0060 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0061 0x03 */ MSCRIPT_CMD_JUMP(0x003C - 0x0064),

    /* 0x0064 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0D4F),
    /* 0x0067 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0068 0x03 */ MSCRIPT_CMD_JUMP(0x0040 - 0x006B),
};

// MsgEvent script for one of the Goron stretchers at the racetrack (Initially stretching side to side while squatting).
static MsgScript sMsgScriptGoronAthleticA[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0009 - 0x0003),
    /* 0x0003 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0DFE),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0008 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0009 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0DFF),
    /* 0x000C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000D 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x000E 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for one of the Goron stretchers at the racetrack (Initially doing sidebend stretches with one arm).
static MsgScript sMsgScriptGoronAthleticB[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0009 - 0x0003),
    /* 0x0003 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E00),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0008 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0009 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E01),
    /* 0x000C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000D 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x000E 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for one of the Goron stretchers at the racetrack (Initially shaking out their limbs).
static MsgScript sMsgScriptGoronAthleticC[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0009 - 0x0003),
    /* 0x0003 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E02),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0008 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0009 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E03),
    /* 0x000C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000D 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x000E 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for one of the Goron stretchers (Initially doing sidebend stretches with both arms).
static MsgScript sMsgScriptGoronAthleticD[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0009 - 0x0003),
    /* 0x0003 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E04),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0008 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0009 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E05),
    /* 0x000C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000D 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x000E 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for the pair of Gorons stretching one Goron's hamstrings.
static MsgScript sMsgScriptGoronAthleticHamstring[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_CHECK_GORON(0x0009 - 0x0003),
    /* 0x0003 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E06),
    /* 0x0006 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0007 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0008 0x01 */ MSCRIPT_CMD_DONE(),

    /* 0x0009 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x0E07),
    /* 0x000C 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x000D 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x000E 0x01 */ MSCRIPT_CMD_DONE(),
};

// MsgEvent script for any sleeping Goron.
static MsgScript sMsgScriptGoronSleeping[] = {
    /* 0x0000 0x03 */ MSCRIPT_CMD_BEGIN_TEXT(0x023A),
    /* 0x0003 0x01 */ MSCRIPT_CMD_AWAIT_TEXT(),
    /* 0x0004 0x01 */ MSCRIPT_CMD_CLOSE_TEXT(),
    /* 0x0005 0x01 */ MSCRIPT_CMD_DONE(),
};

/**
 * Return the MsgEvent script appropriate for the actor.
 */
// RECOMP_PATCH MsgScript* EnGo_GetMsgScript(EnGo* this, PlayState* play) {
RECOMP_PATCH MsgScript* EnGo_GetMsgEventScript(EnGo* this, PlayState* play) {
    static MsgScript* sMsgScriptGraveyard[] = {
        sMsgScriptGoronGravemaker,
        sMsgScriptGoronFrozen,
    };

    if (this->sleepState != ENGO_AWAKE) {
        return sMsgScriptGoronSleeping; // Lullaby hint
    }

    if (ENGO_GET_TYPE(&this->actor) == ENGO_ATHLETIC) {
        switch (ENGO_GET_SUBTYPE(&this->actor) % 6) {
            case ENGO_ATHLETIC_A:
                return sMsgScriptGoronAthleticA;

            case ENGO_ATHLETIC_B: // Unused!
                return sMsgScriptGoronAthleticB;

            case ENGO_ATHLETIC_C:
                return sMsgScriptGoronAthleticC;

            case ENGO_ATHLETIC_D:
                return sMsgScriptGoronAthleticD;

            case ENGO_ATHLETIC_HAMSTRINGSIT:
                return sMsgScriptGoronAthleticHamstring;

            case ENGO_ATHLETIC_HAMSTRINGSTAND:
                return sMsgScriptGoronAthleticHamstring;

            default:
                break;
        }
    }

    switch (ENGO_GET_TYPE(&this->actor)) {
        case ENGO_GATEKEEPER:
            return sMsgScriptGoronGatekeeper;

        case ENGO_GRAVEYARD:
            return sMsgScriptGraveyard[ENGO_GET_SUBTYPE(&this->actor)];

        case ENGO_ASIDE_STORE:
            return sMsgScriptGoronAsideStore;

        case ENGO_ASIDE_ELDERSROOM:
            return sMsgScriptGoronAsideEldersRoom;

        case ENGO_ASIDE_ELDERSSON:
            return sMsgScriptGoronAsideEldersSon;

        case ENGO_MEDIGORON:
            if (rando_location_is_checked(GI_POWDER_KEG)) {
                SET_WEEKEVENTREG(WEEKEVENTREG_HAS_POWDERKEG_PRIVILEGES);
            } else {
                CLEAR_WEEKEVENTREG(WEEKEVENTREG_HAS_POWDERKEG_PRIVILEGES);
            }
            return sMsgScriptGoronMedigoron;

        default:
            return sMsgScriptGoronAsideStore;
    }
}

// MsgEvent_CheckHasPowderKeg
RECOMP_PATCH s32 func_8010BBCC(Actor* actor, PlayState* play, u8** script, MsgScriptCallback callback, s32* endScript) {
    MsgScriptCmdCheckHasPowderKeg* cmd = (MsgScriptCmdCheckHasPowderKeg*)*script;
    s16 skip = SCRIPT_PACK_16(cmd->offsetH, cmd->offsetL);

    if ((CHECK_WEEKEVENTREG(WEEKEVENTREG_HAS_POWDERKEG_PRIVILEGES) && (AMMO(ITEM_POWDER_KEG) != 0 || INV_CONTENT(ITEM_POWDER_KEG) != ITEM_POWDER_KEG))
        || (play->actorCtx.flags & ACTORCTX_FLAG_0)) {
        *script += skip;
    }
    return false;
}