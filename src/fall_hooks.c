#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define EN_FALL_SCALE(thisx) ((thisx)->params & 0x7F)
#define EN_FALL_TYPE(thisx) (((thisx)->params & 0xF80) >> 7)

typedef enum {
    /*  0 */ EN_FALL_TYPE_TERMINA_FIELD_MOON,
    /*  1 */ EN_FALL_TYPE_TITLE_SCREEN_MOON,
    /*  2 */ EN_FALL_TYPE_CRASHING_MOON,
    /*  3 */ EN_FALL_TYPE_CRASH_FIRE_BALL,
    /*  4 */ EN_FALL_TYPE_CRASH_RISING_DEBRIS,
    /*  5 */ EN_FALL_TYPE_LODMOON_NO_LERP, // unused in final game
    /*  6 */ EN_FALL_TYPE_LODMOON,
    /*  7 */ EN_FALL_TYPE_STOPPED_MOON_CLOSED_MOUTH,
    /*  8 */ EN_FALL_TYPE_MOONS_TEAR,
    /*  9 */ EN_FALL_TYPE_CLOCK_TOWER_MOON,
    /* 10 */ EN_FALL_TYPE_STOPPED_MOON_OPEN_MOUTH,
    /* 11 */ EN_FALL_TYPE_CRASH_FIRE_RING,
    /* 12 */ EN_FALL_TYPE_LODMOON_INVERTED_STONE_TOWER
} EnFallType;

struct EnFall;

typedef void (*EnFallActionFunc)(struct EnFall*, PlayState*);

typedef struct EnFall {
    /* 0x000 */ Actor actor;
    /* 0x144 */ s16 dayStartTime;
    /* 0x146 */ s16 currentDay;
    /* 0x148 */ u8 objectSlot;
    /* 0x14C */ f32 scale;
    /* 0x150 */ union {
                    f32 eyeGlowIntensity;
                    f32 fireballIntensity;
                    f32 fireRingAlpha;
                };
    /* 0x154 */ u16 flags;
    /* 0x158 */ union {
                    s32 fireballAlpha;
                    s32 activeDebrisEffectCount;
                };
    /* 0x15C */ union {
                    s16 fireballYTexScroll1;
                    s16 fireWallYScale;
                };
    /* 0x15E */ s16 fireballYTexScroll2;
    /* 0x160 */ EnFallActionFunc actionFunc;
} EnFall; // size = 0x164

/**
 * Sets the scale of the moon depending on the current day. On the Final Day,
 * it also moves the moon closer to the ground depending on the current time.
 */
RECOMP_PATCH void EnFall_Moon_AdjustScaleAndPosition(EnFall* this, PlayState* play) {
    u16 currentTime = gSaveContext.save.time;
    u16 dayStartTime = this->dayStartTime;
    f32 finalDayRelativeHeight;

    if (currentTime < dayStartTime) {
        finalDayRelativeHeight = 1.0f - (((f32)dayStartTime - (f32)currentTime) * (1.0f / 0x10000));
    } else {
        finalDayRelativeHeight = ((f32)currentTime - (f32)dayStartTime) * (1.0f / 0x10000);
    }
    switch (CURRENT_DAY) {
        case 0:
            Actor_SetScale(&this->actor, this->scale * 1.2f);
            this->actor.world.pos.y = this->actor.home.pos.y;
            break;

        case 1:
            Actor_SetScale(&this->actor, this->scale * 2.4f);
            this->actor.world.pos.y = this->actor.home.pos.y;
            break;

        case 2:
            Actor_SetScale(&this->actor, this->scale * 3.6f);
            this->actor.world.pos.y = this->actor.home.pos.y;
            break;

        case 3:
            Actor_SetScale(&this->actor, this->scale * 3.6f);
            if (EN_FALL_TYPE(&this->actor) == EN_FALL_TYPE_LODMOON_INVERTED_STONE_TOWER) {
                this->actor.world.pos.y =
                    this->actor.home.pos.y + (finalDayRelativeHeight * 6700.0f * (this->scale * 6.25f));
            } else {
                this->actor.world.pos.y =
                    this->actor.home.pos.y - (finalDayRelativeHeight * 6700.0f * (this->scale * 6.25f));
            }
            break;

        default:
            break;
    }
}