// most of this is temporary and grabbed directly from danielryb's BetterDoubleSoT mod

#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"

#include "z64environment.h"

#define CURRENT_TIME ((void)0, gSaveContext.save.time)
#define WEATHER_MODE_RAIN WEATHER_MODE_1

typedef enum ThreeDayDaytime {
    /* 0 */ THREEDAY_DAYTIME_NIGHT,
    /* 1 */ THREEDAY_DAYTIME_DAY,
    /* 2 */ THREEDAY_DAYTIME_MAX
} ThreeDayDaytime;

void dsot_rain_fix(PlayState* play);
void dsot_bgm_fix(PlayState* play);
void dsot_actor_fixes(PlayState* play);

void rando_advance_hour(PlayState* play, u8 choiceHour) {
    gSaveContext.save.time = CLOCK_TIME(choiceHour, 0);
    gSaveContext.skyboxTime = CURRENT_TIME;

    dsot_rain_fix(play);
    dsot_bgm_fix(play);
    dsot_actor_fixes(play);
}

void dsot_rain_fix(PlayState* play) {
    if ((CURRENT_DAY == 2) && (Environment_GetStormState(play) != STORM_STATE_OFF)) {
        if ((CURRENT_TIME >= CLOCK_TIME(8, 0)) && (CURRENT_TIME < CLOCK_TIME(18, 00))) {
            gWeatherMode = WEATHER_MODE_RAIN;
            play->envCtx.lightningState = LIGHTNING_ON;
            play->envCtx.precipitation[PRECIP_RAIN_MAX] = 60;
            play->envCtx.precipitation[PRECIP_RAIN_CUR] = 60;
            Environment_PlayStormNatureAmbience(play);
        } else {
            gWeatherMode = WEATHER_MODE_CLEAR;
            play->envCtx.lightningState = LIGHTNING_OFF;
            play->envCtx.precipitation[PRECIP_RAIN_MAX] = 0;
            play->envCtx.precipitation[PRECIP_RAIN_CUR] = 0;
            Environment_StopStormNatureAmbience(play);
        }
    }
}

void dsot_bgm_fix(PlayState* play) {
    play->envCtx.timeSeqState = TIMESEQ_FADE_DAY_BGM;

    if ((CURRENT_TIME >= CLOCK_TIME(18, 0)) || (CURRENT_TIME <= CLOCK_TIME(6,0))) {
        SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_BGM_MAIN, 0);
        play->envCtx.timeSeqState = TIMESEQ_NIGHT_BEGIN_SFX;
    } else if (CURRENT_TIME > CLOCK_TIME(17, 10)) {
        SEQCMD_STOP_SEQUENCE(SEQ_PLAYER_BGM_MAIN, 240);
        play->envCtx.timeSeqState = TIMESEQ_NIGHT_BEGIN_SFX;
    }

    if ((CURRENT_TIME >= CLOCK_TIME(18, 0)) || (CURRENT_TIME <= CLOCK_TIME(6, 0))) {
        // Audio_PlayAmbience(play->sceneSequences.ambienceId);
        Audio_PlayAmbience(play->sequenceCtx.ambienceId);
        Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_0, 1, 1);
        play->envCtx.timeSeqState = TIMESEQ_NIGHT_DELAY;
    }

    if ((CURRENT_TIME >= CLOCK_TIME(19, 0)) || (CURRENT_TIME <= CLOCK_TIME(6, 0))) {
        Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_0, 1, 0);
        Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_1 << 4 | AMBIENCE_CHANNEL_CRITTER_3, 1, 1);
        play->envCtx.timeSeqState = TIMESEQ_DAY_BEGIN_SFX;
    }

    if ((CURRENT_TIME >= CLOCK_TIME(5, 0)) && (CURRENT_TIME <= CLOCK_TIME(6, 0))) {
        Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_1 << 4 | AMBIENCE_CHANNEL_CRITTER_3, 1, 0);
        Audio_SetAmbienceChannelIO(AMBIENCE_CHANNEL_CRITTER_4 << 4 | AMBIENCE_CHANNEL_CRITTER_5, 1, 1);
        play->envCtx.timeSeqState = TIMESEQ_DAY_DELAY;
    }
}

#include "overlays/actors/ovl_En_Test4/z_en_test4.h"
#include "overlays/actors/ovl_Obj_Tokei_Step/z_obj_tokei_step.h"
#include "overlays/actors/ovl_Obj_Tokeidai/z_obj_tokeidai.h"

void dsot_ObjEnTest4_fix(EnTest4* this, PlayState* play);
void dsot_ObjTokeiStep_fix(ObjTokeiStep* this, PlayState* play);
void dsot_ObjTokeidai_fix(ObjTokeidai* this, PlayState* play);

// Calls actor specific fixes.
void dsot_actor_fixes(PlayState* play) {
    s32 category;
    Actor* actor;
    Player* player = GET_PLAYER(play);
    ActorListEntry* entry;

    ActorContext* actorCtx = &play->actorCtx;

    for (category = 0, entry = actorCtx->actorLists; category < ACTORCAT_MAX;
         entry++, category++) {
        actor = entry->first;

        for (actor = entry->first; actor != NULL; actor = actor->next) {
            switch(actor->id) {
                case ACTOR_EN_TEST4:
                    dsot_ObjEnTest4_fix((EnTest4*)actor, play);
                    break;
                case ACTOR_OBJ_TOKEI_STEP:
                    dsot_ObjTokeiStep_fix((ObjTokeiStep*)actor, play);
                    break;
                case ACTOR_OBJ_TOKEIDAI:
                    dsot_ObjTokeidai_fix((ObjTokeidai*)actor, play);
                    break;
            }
        }
    }
}

#define PAST_MIDNIGHT ((CURRENT_DAY == 3) && (CURRENT_TIME < CLOCK_TIME(6, 0)) && (CURRENT_TIME > CLOCK_TIME(0, 0)))

// z_obj_en_test_4

// void EnTest4_GetBellTimeOnDay3(EnTest4* this);
void func_80A42198(EnTest4* this);

void dsot_ObjEnTest4_fix(EnTest4* this, PlayState* play) {
    // this->prevTime = CURRENT_TIME;
    // this->prevBellTime = CURRENT_TIME;
    this->unk_146 = CURRENT_TIME;
    this->lastBellTime = CURRENT_TIME;

    // Change daytime to night manually if necessary.
    // if (((this->daytimeIndex = THREEDAY_DAYTIME_DAY) && (CURRENT_TIME > CLOCK_TIME(18, 0))) || (CURRENT_TIME <= CLOCK_TIME(6, 0))) {
    //     this->daytimeIndex = THREEDAY_DAYTIME_NIGHT;
    if (((this->csIdIndex = THREEDAY_DAYTIME_DAY) && (CURRENT_TIME > CLOCK_TIME(18, 0))) || (CURRENT_TIME <= CLOCK_TIME(6, 0))) {
        this->csIdIndex = THREEDAY_DAYTIME_NIGHT;
        // Re-spawn the setup actors.
        play->numSetupActors = -play->numSetupActors;
    }

    // Setup next bell time.
    if (CURRENT_DAY == 3) {
        gSaveContext.save.time--;
        // EnTest4_GetBellTimeOnDay3(this);
        func_80A42198(this);
        gSaveContext.save.time++;
    }
}

// z_obj_tokei_step

void ObjTokeiStep_InitStepsOpen(ObjTokeiStep* this);
void ObjTokeiStep_SetupDoNothingOpen(ObjTokeiStep* this);
void ObjTokeiStep_DrawOpen(Actor* thisx, PlayState* play);

void dsot_ObjTokeiStep_fix(ObjTokeiStep* this, PlayState* play) {
    if (PAST_MIDNIGHT) {
        this->dyna.actor.draw = ObjTokeiStep_DrawOpen;
        ObjTokeiStep_InitStepsOpen(this);
        ObjTokeiStep_SetupDoNothingOpen(this);
        DynaPoly_DisableCollision(play, &play->colCtx.dyna, this->dyna.bgId);
    }
}

// z_obj_tokeidai

#define GET_CLOCK_FACE_ROTATION(currentClockHour) (TRUNCF_BINANG(currentClockHour * (0x10000 / 24.0f)))
#define GET_MINUTE_RING_OR_EXTERIOR_GEAR_ROTATION(currentClockMinute) \
    (TRUNCF_BINANG(currentClockMinute * (0x10000 * 12.0f / 360)))

s32 ObjTokeidai_GetTargetSunMoonPanelRotation(void);
void ObjTokeidai_Init(Actor* thisx, PlayState* play);
void ObjTokeidai_Draw(Actor* thisx, PlayState* play);
void ObjTokeidai_ExteriorGear_Draw(Actor* thisx, PlayState* play);
void ObjTokeidai_Clock_Draw(Actor* thisx, PlayState* play);

void ObjTokeidai_Counterweight_Idle(ObjTokeidai* this, PlayState* play);
void ObjTokeidai_ExteriorGear_Idle(ObjTokeidai* this, PlayState* play);
void ObjTokeidai_TowerClock_Idle(ObjTokeidai* this, PlayState* play);

s32 dsot_ObjTokeidai_get_target_sun_moon_panel_rotation(void) {
    if (CURRENT_TIME >= CLOCK_TIME(18, 0) || CURRENT_TIME < CLOCK_TIME(6, 0)) {
        return 0x8000;
    }
    return 0;
}

void dsot_ObjTokeidai_update_clock(ObjTokeidai* this, u16 currentHour, u16 currentMinute) {
    this->clockTime = CURRENT_TIME;

    // Instantly switch to desired hour.
    u16 clockHour = currentHour;
    if (currentMinute == 0) {
        clockHour--;
        if (clockHour > 23) {
            clockHour = 23;
        }
    }

    this->clockFaceRotation = GET_CLOCK_FACE_ROTATION(clockHour);
    this->clockHour = clockHour;
    this->clockFaceAngularVelocity = 0;
    this->clockFaceRotationTimer = 0;

    // Instantly switch to desired minute.
    u16 clockMinute = currentMinute - 1;
    if (clockMinute > 59) {
        clockMinute = 59;
    }

    this->minuteRingOrExteriorGearRotation = GET_MINUTE_RING_OR_EXTERIOR_GEAR_ROTATION(clockMinute);
    this->clockMinute = clockMinute;
    this->minuteRingOrExteriorGearAngularVelocity = 0x5A;
    this->minuteRingOrExteriorGearRotationTimer = 0;

    // Switch the medalion rotation immediately.
    if (((currentHour != 6) && (currentHour != 18)) || (currentMinute != 0)) {
        this->sunMoonPanelRotation = dsot_ObjTokeidai_get_target_sun_moon_panel_rotation();
        this->sunMoonPanelAngularVelocity = 0;
    }
}

void dsot_ObjTokeidai_fix(ObjTokeidai* this, PlayState* play) {
    s32 type = OBJ_TOKEIDAI_TYPE(&this->actor);
    u16 currentHour = TIME_TO_HOURS_F(CURRENT_TIME);
    u16 currentMinute = TIME_TO_MINUTES_F(CURRENT_TIME);

    switch(type) {
        case OBJ_TOKEIDAI_TYPE_COUNTERWEIGHT_TERMINA_FIELD:
        case OBJ_TOKEIDAI_TYPE_COUNTERWEIGHT_CLOCK_TOWN:
            if (PAST_MIDNIGHT && (this->actionFunc == ObjTokeidai_Counterweight_Idle)) {
                this->actor.shape.rot.y = 0;
                ObjTokeidai_Init((Actor*)this, play);
            }
            break;

        case OBJ_TOKEIDAI_TYPE_EXTERIOR_GEAR_TERMINA_FIELD:
        case OBJ_TOKEIDAI_TYPE_EXTERIOR_GEAR_CLOCK_TOWN:
            if (PAST_MIDNIGHT && (this->actionFunc == ObjTokeidai_ExteriorGear_Idle)) {
                dsot_ObjTokeidai_update_clock(this, 0, 0);
                this->actor.draw = ObjTokeidai_ExteriorGear_Draw;
                ObjTokeidai_Init((Actor*)this, play);
            } else {
                dsot_ObjTokeidai_update_clock(this, currentHour, currentMinute);
            }
            break;

        case OBJ_TOKEIDAI_TYPE_TOWER_CLOCK_TERMINA_FIELD:
        case OBJ_TOKEIDAI_TYPE_TOWER_CLOCK_CLOCK_TOWN:
            if (PAST_MIDNIGHT && (this->actionFunc == ObjTokeidai_TowerClock_Idle)) {
                dsot_ObjTokeidai_update_clock(this, 0, 0);
                this->actor.draw = ObjTokeidai_Clock_Draw;
                ObjTokeidai_Init((Actor*)this, play);
            } else {
                dsot_ObjTokeidai_update_clock(this, currentHour, currentMinute);
            }
            break;
        case OBJ_TOKEIDAI_TYPE_STAIRCASE_TO_ROOFTOP:
            if (PAST_MIDNIGHT) {
                this->actor.draw = ObjTokeidai_Draw;
            }
            break;
        case OBJ_TOKEIDAI_TYPE_WALL_CLOCK:
        case OBJ_TOKEIDAI_TYPE_SMALL_WALL_CLOCK:
            dsot_ObjTokeidai_update_clock(this, currentHour, currentMinute);
            break;
    }
}