#include "modding.h"
#include "global.h"

#include "../mm-decomp/src/overlays/actors/ovl_En_Horse/z_en_horse.h"
#include "z64horse.h"

#include "apcommon.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_20 | ACTOR_FLAG_100000)
#define THIS ((EnTest4*)thisx)

struct ObjTokeiStep;

typedef void (*ObjTokeiStepFunc)(struct ObjTokeiStep*, struct PlayState*);

typedef struct ObjTokeiStepPanel {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 posChangeY;
    /* 0x10 */ s16 startFallingTimer; // Timer for when a step starts falling after the previous step has started
                                      // bouncing during opening cutscene
    /* 0x12 */ s8 numBounces; // Counter for times the step falls below its final Y pos, and thus will bounce back up
                              // during opening cutscene
    /* 0x13 */ s8 hasSoundPlayed;
} ObjTokeiStepPanel; // size = 0x14

typedef struct ObjTokeiStep {
    /* 0x000 */ DynaPolyActor dyna;
    /* 0x15C */ ObjTokeiStepFunc actionFunc;
    /* 0x160 */ ObjTokeiStepPanel panels[7];
} ObjTokeiStep; // size = 0x1EC

ObjTokeiStep* tokei_steps = NULL;

ItemId ocarina_item;
s16 test4_scene_id;

struct EnTest4;

typedef void (*EnTest4ActionFunc)(struct EnTest4*, PlayState*);

typedef struct EnTest4 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ s8 csIdIndex; // 0 on night, 1 on day
    /* 0x145 */ u8 transitionCsTimer;
    /* 0x146 */ u16 unk_146;
    /* 0x148 */ u16 nextBellTime; // Next time the bell will sound
    /* 0x14A */ u16 lastBellTime; // Last time the bell sounded
    /* 0x14C */ u8 state;
    /* 0x150 */ EnTest4ActionFunc actionFunc;
} EnTest4; // size = 0x154

RECOMP_HOOK("ObjTokeiStep_Init")
void ObjTokeiStep_Init(Actor* thisx, PlayState* play) {
    tokei_steps = (ObjTokeiStep*) thisx;
}

RECOMP_HOOK("ObjTokeiStep_Destroy")
void ObjTokeiStep_Destroy(Actor* thisx, PlayState* play) {
    tokei_steps = NULL;
}

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

extern u16 D_80A43364[];
extern s16 sCsIdList[2];

void func_80A41FA4(EnTest4 *this, PlayState *play);
void func_80A42198(EnTest4 *this);
void func_80A425E4(EnTest4 *this, PlayState *play);

void ObjTokeiStep_SetupOpen(ObjTokeiStep* this);
void ObjTokeiStep_DrawOpen(Actor* thisx, PlayState* play);

RECOMP_PATCH void func_80A42AB8(EnTest4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((play->transitionMode == TRANS_MODE_OFF) && !Play_InCsMode(play) && (play->numSetupActors <= 0) &&
        (play->roomCtx.status == 0) && !Play_IsDebugCamEnabled()) {
        s16 temp_a2;
        u16 temp_a0 = D_80A43364[this->csIdIndex];
        s16 temp_a3;
        s16 bellDiff;
        s16 new_var;

        temp_a3 = gSaveContext.save.time - temp_a0;
        temp_a2 = this->unk_146 - temp_a0;
        bellDiff = this->lastBellTime - this->nextBellTime;
        new_var = gSaveContext.save.time - this->nextBellTime;

        if ((temp_a3 * temp_a2) <= 0) {
            gSaveContext.unk_3CA7 = 1;
            if (play->actorCtx.flags & ACTORCTX_FLAG_PICTO_BOX_ON) {
                play->actorCtx.flags &= ~ACTORCTX_FLAG_PICTO_BOX_ON;
            }

            if (temp_a0 != CLOCK_TIME(6, 0)) {
                func_80A41FA4(this, play);
            } else if (temp_a0 == CLOCK_TIME(6, 0)) {
                if (CURRENT_DAY == 3) {
                    Interface_StartMoonCrash(play);
                    Actor_Kill(&this->actor);
                    SET_EVENTINF(EVENTINF_17);
                } else if (((sCsIdList[this->csIdIndex] < 0) || (play->actorCtx.flags & ACTORCTX_FLAG_1)) &&
                           (CURRENT_DAY != 3)) {
                    func_80A41FA4(this, play);
                } else {
                    gSaveContext.screenScale = 0.0f;
                    Play_SetRespawnData(&play->state, RESPAWN_MODE_DOWN, Entrance_CreateFromSpawn(0), player->unk_3CE,
                                        PLAYER_PARAMS(0xFF, PLAYER_INITMODE_B), &player->unk_3C0, player->unk_3CC);
                    func_80169EFC(&play->state);
                    if (player->stateFlags1 & PLAYER_STATE1_800000) {
                        EnHorse* rideActor = (EnHorse*)player->rideActor;

                        if ((rideActor->type == HORSE_TYPE_EPONA) || (rideActor->type == HORSE_TYPE_2)) {
                            if (CURRENT_DAY < 3) {
                                gHorseIsMounted = true;
                            } else {
                                gHorseIsMounted = false;
                            }
                        }
                    }

                    gSaveContext.respawnFlag = -4;
                    SET_EVENTINF(EVENTINF_TRIGGER_DAYTELOP);
                    Actor_Kill(&this->actor);
                }
            }

            if ((sCsIdList[this->csIdIndex] >= 0) && !(play->actorCtx.flags & ACTORCTX_FLAG_1)) {
                player->stateFlags1 |= PLAYER_STATE1_200;
                this->unk_146 = gSaveContext.save.time;
            } else {
                if (this->csIdIndex == 0) {
                    this->csIdIndex = 1;
                } else {
                    this->csIdIndex = 0;
                }

                this->unk_146 = gSaveContext.save.time += CLOCK_TIME_MINUTE;
            }
        } else if ((new_var * bellDiff) <= 0) {
            Audio_PlaySfx_BigBells(&this->actor.projectedPos, (this->actor.params >> 5) & 0xF);
            this->lastBellTime = gSaveContext.save.time;

            if (CURRENT_DAY == 3) {
                // @rando replace clock tower cutscene with open animation
                if (gSaveContext.save.time < CLOCK_TIME(0, 1) && tokei_steps != NULL && tokei_steps->dyna.actor.draw != ObjTokeiStep_DrawOpen) {
                    ObjTokeiStep_SetupOpen(tokei_steps);
                    tokei_steps->dyna.actor.draw = ObjTokeiStep_DrawOpen;
                }
                func_80A42198(this);
            } else {
                func_80A425E4(this, play);
            }
        }
    }
}