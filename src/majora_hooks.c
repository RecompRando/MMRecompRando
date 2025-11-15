#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"

struct Boss07;

typedef void (*Boss07ActionFunc)(struct Boss07*, struct PlayState*);

typedef struct Boss07 {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ Boss07ActionFunc actionFunc;
    /* 0x0148 */ char filler_148[0x18];
    /* 0x0160 */ s16 motionBlurAlpha;
    /* 0x0164 */ char filler_164[0xAABC]; // hopefully i did my math right lol
} Boss07; // size = 0xAC20

// Boss07_Mask_IntroCutscene
void func_80A01750(Boss07* this, PlayState* play);

// Boss07_Mask_SetupIntroCutscene
RECOMP_PATCH void func_80A016E4(Boss07* this, PlayState* play) {
    // prevent the original function from running when "completion_goal" is enabled and the player doesn't have the soul
    if (rando_get_slotdata_u32("completion_goal") && !rando_has_item(AP_ITEM_ID_SOUL_BOSS_MAJORA)) {
        this->actionFunc = func_80A016E4; // set actionFunc to self

        // setup talking when targetted to reset the cycle when talked to
        if (this->actor.isLockedOn) {
            Actor_OfferTalkExchangeEquiCylinder(&this->actor, play, 1000.0f, PLAYER_IA_MINUS1);
            if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
                play->nextEntrance = ENTRANCE(SOUTH_CLOCK_TOWN, 0);
                gSaveContext.save.entrance = play->nextEntrance;
                play->state.running = false;
                play->state.init = gGameStateOverlayTable[GAMESTATE_PLAY].init;
                play->state.size = gGameStateOverlayTable[GAMESTATE_PLAY].instanceSize;
                gSaveContext.save.day = 0;
                gSaveContext.save.time = CLOCK_TIME(6, 0);
                Sram_SaveEndOfCycle(play);
            }
        }
    } else {
        // original setup (+ motion blur)
        this->actionFunc = func_80A01750;
        this->motionBlurAlpha = KREG(75) + 150;
        Play_EnableMotionBlur(this->motionBlurAlpha);
    }

    // original positioning (otherwise majora would be in the center of the arena lol)
    this->actor.world.pos.x = 0.0f;
    this->actor.world.pos.y = 300.0f;
    this->actor.world.pos.z = -922.5f;
}