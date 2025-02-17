#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnTimeTag;

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED)

#define THIS ((EnTimeTag*)thisx)

typedef void (*EnTimeTagActionFunc)(struct EnTimeTag*, PlayState*);

#define TIMETAG_GET_TYPE(thisx) (((thisx)->params & 0xE000) >> 0xD)

#define TIMETAG_KICKOUT_GET_TEXT(thisx) (((thisx)->params & 0x1FE0) >> 0x5)
#define TIMETAG_KICKOUT_GET_EXIT_INDEX(thisx) ((thisx)->params & 0x1F)
#define TIMETAG_KICKOUT_HOUR(thisx) ((thisx)->home.rot.x)
#define TIMETAG_KICKOUT_MINUTE(thisx) ((thisx)->home.rot.y)

#define TIMETAG_SOARING_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)

#define TIMETAG_DIARY_GET_TYPE(thisx) ((thisx)->params & 0x7F)
#define TIMETAG_DIARY_TIMER(thisx) ((thisx)->home.rot.x)
#define TIMETAG_DIARY_SONG(thisx) ((thisx)->home.rot.z)

typedef enum {
    /* 0 */ TIMETAG_KICKOUT_DOOR,
    /* 1 */ TIMETAG_SOARING_ENGRAVING,
    /* 2 */ TIMETAG_ROOFTOP_OATH,
    /* 3 */ TIMETAG_DIARY,
    /* 4 */ TIMETAG_KICKOUT_FINAL_HOURS,
    /* 5 */ TIMETAG_KICKOUT_NO_DOOR
} TimeTagType;

typedef enum {
    /* 0 */ TIMETAG_DIARY_MIKAU,
    /* 1 */ TIMETAG_DIARY_LULU
} TimeTagDiaryType;

typedef enum {
    /* 0 */ TIMETAG_DIARY_SONG_EVAN_PART1,
    /* 1 */ TIMETAG_DIARY_SONG_EVAN_PART2
} TimeTagDiarySong;

typedef enum {
    /*  0 */ TIMETAG_KICKOUT_TEXT_0, // I am closing the shop
    /*  1 */ TIMETAG_KICKOUT_TEXT_SWAMP_SHOOTING_GALLERY, // time to close shop
    /*  2 */ TIMETAG_KICKOUT_TEXT_STOCK_POT_INN, // Stock Pot Inn locking the door
    /*  3 */ TIMETAG_KICKOUT_TEXT_HONEY_AND_DARLING, // need some alone time
    /*  4 */ TIMETAG_KICKOUT_TEXT_LOTTERY, // business is finished
    /*  5 */ TIMETAG_KICKOUT_TEXT_BARN_DAY, // the lady needs a break
    /*  6 */ TIMETAG_KICKOUT_TEXT_GENERIC, // (Many scenes) place is about to close
    /*  7 */ TIMETAG_KICKOUT_TEXT_BARN_NIGHT, // they came
    /*  8 */ TIMETAG_KICKOUT_TEXT_TRADING_POST, // we have to close shop
    /*  9 */ TIMETAG_KICKOUT_TEXT_MILK_BAR, // I have to get ready for business
    /* 10 */ TIMETAG_KICKOUT_TEXT_CURIOSITY_SHOP_BACK_1, // shop is opening, you need to go
    /* 11 */ TIMETAG_KICKOUT_TEXT_CURIOSITY_SHOP_BACK_2, // shop is opening, we're in the way
    /* 12 */ TIMETAG_KICKOUT_TEXT_TOWN_SHOOTING_GALLERY_1, // it's time to close
    /* 13 */ TIMETAG_KICKOUT_TEXT_TOWN_SHOOTING_GALLERY_2, // quick, it's closing time
    /* 14 */ TIMETAG_KICKOUT_TEXT_SWORDSMANS_SCHOOL, // need to cut the moon, must leave
    /* 15 */ TIMETAG_KICKOUT_TEXT_TREASURE_CHEST_SHOP, // already time to close shop
    /* 16 */ TIMETAG_KICKOUT_TEXT_DOGGY_RACETRACK // no more for today
} TimeTagKickOutText;

typedef struct EnTimeTag {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnTimeTagActionFunc actionFunc;
} EnTimeTag; // size = 0x148

void EnTimeTag_RooftopOath_DoNothing(EnTimeTag* this, PlayState* play);
void EnTimeTag_RooftopOath_Cutscene(EnTimeTag* this, PlayState* play);

void EnTimeTag_SoaringEngraving_GetSong(EnTimeTag* this, PlayState* play);
void EnTimeTag_SoaringEngraving_StartCutscene(EnTimeTag* this, PlayState* play);
void EnTimeTag_SoaringEngraving_Wait(EnTimeTag* this, PlayState* play);

void EnTimeTag_Diary_AfterOcarina(EnTimeTag* this, PlayState* play);
void EnTimeTag_Diary_TeachEvanSongSnippets(EnTimeTag* this, PlayState* play);
void EnTimeTag_Diary_Cutscene(EnTimeTag* this, PlayState* play);
void EnTimeTag_Diary_Wait(EnTimeTag* this, PlayState* play);

void EnTimeTag_KickOut_DoNothing(EnTimeTag* this, PlayState* play);
void EnTimeTag_KickOut_Transition(EnTimeTag* this, PlayState* play);
void EnTimeTag_KickOut_WaitForTrigger(EnTimeTag* this, PlayState* play);
void EnTimeTag_KickOut_WaitForTime(EnTimeTag* this, PlayState* play);

RECOMP_PATCH void EnTimeTag_RooftopOath_Wait(EnTimeTag* this, PlayState* play) {
    Actor* thisx = &this->actor;

    if ((play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) && (play->msgCtx.lastPlayedSong == OCARINA_SONG_OATH)) {
        if (this->actor.csId != CS_ID_NONE) {
            this->actionFunc = EnTimeTag_RooftopOath_Cutscene;
        }
        play->msgCtx.ocarinaMode = OCARINA_MODE_END;
    }
}

RECOMP_PATCH void EnTimeTag_RooftopOath_Cutscene(EnTimeTag* this, PlayState* play) {
    //~ if (CutsceneManager_IsNext(this->actor.csId)) {
        //~ CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);

        //~ this->actionFunc = EnTimeTag_RooftopOath_DoNothing;
        //~ gSaveContext.timerStates[TIMER_ID_MOON_CRASH] = TIMER_STATE_OFF;

        //~ if (CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA) && CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT) &&
            //~ CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG) && CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD)) {
            //~ SET_WEEKEVENTREG(WEEKEVENTREG_25_02);
        //~ }
    //~ } else {
        //~ CutsceneManager_Queue(this->actor.csId);
    //~ }
    if (play->msgCtx.ocarinaMode == OCARINA_MODE_END) {
        this->actionFunc = EnTimeTag_RooftopOath_Wait;
    }
    if (CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA) && CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT) &&
        CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG) && CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD)) {
        gSaveContext.timerStates[TIMER_ID_MOON_CRASH] = TIMER_STATE_OFF;
        play->msgCtx.ocarinaMode = OCARINA_MODE_END;
        play->nextEntrance = ENTRANCE(THE_MOON, 0);
        play->transitionType = TRANS_TYPE_FADE_WHITE_FAST;
        gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE_FAST;
        play->transitionTrigger = TRANS_TRIGGER_START;
        Audio_StopSubBgm();
        Audio_StopSequenceAtDefaultPos();
        Audio_StopFanfare(0);
        play->msgCtx.ocarinaMode = OCARINA_MODE_END;
    }
}

RECOMP_PATCH void EnTimeTag_Init(Actor* thisx, PlayState* play) {
    EnTimeTag* this = THIS;

    this->actionFunc = EnTimeTag_KickOut_WaitForTime;

    switch (TIMETAG_GET_TYPE(&this->actor)) {
        case TIMETAG_KICKOUT_FINAL_HOURS:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_CLOCK_TOWER_OPENED) || (CURRENT_DAY != 3)) {
                Actor_Kill(&this->actor);
                return;
            }
            // Overwrite kickout time to midnight
            TIMETAG_KICKOUT_HOUR(&this->actor) = 0;
            TIMETAG_KICKOUT_MINUTE(&this->actor) = 0;
            break;

        case TIMETAG_ROOFTOP_OATH:
            this->actionFunc = EnTimeTag_RooftopOath_Wait;
            this->actor.flags |= ACTOR_FLAG_UPDATE_DURING_OCARINA;
            break;

        case TIMETAG_SOARING_ENGRAVING:
            //this->actionFunc = EnTimeTag_SoaringEngraving_Wait;
            this->actor.flags |= ACTOR_FLAG_UPDATE_DURING_OCARINA;
            //if (CHECK_QUEST_ITEM(QUEST_SONG_SOARING)) {
            if (rando_location_is_checked(0x04006A)) {
                this->actor.textId = 0;
            } else {
                this->actor.textId = 0xC02;
                this->actionFunc = EnTimeTag_SoaringEngraving_Wait;
            }
            break;

        case TIMETAG_DIARY:
            this->actionFunc = EnTimeTag_Diary_Wait;
            this->actor.textId = 0;
            TIMETAG_DIARY_TIMER(&this->actor) = 0;
            break;

        default:
            break;
    }
}

RECOMP_PATCH void EnTimeTag_SoaringEngraving_SubsequentInteraction(EnTimeTag* this, PlayState* play) {
    /*if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        Message_CloseTextbox(play);
        this->actionFunc = EnTimeTag_SoaringEngraving_Wait;
    }*/
}

RECOMP_PATCH void EnTimeTag_SoaringEngraving_Wait(EnTimeTag* this, PlayState* play) {
    /*if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        if (this->actor.textId == 0) {
            this->actionFunc = EnTimeTag_SoaringEngraving_StartCutscene;
        } else {
            this->actionFunc = EnTimeTag_SoaringEngraving_SubsequentInteraction;
        }
    } else if ((this->actor.xzDistToPlayer < 100.0f) && Player_IsFacingActor(&this->actor, 0x3000, play) &&
               (Flags_GetSwitch(play, TIMETAG_SOARING_GET_SWITCH_FLAG(&this->actor)) ||
                CHECK_QUEST_ITEM(QUEST_SONG_SOARING))) {
        this->actor.flags |= ACTOR_FLAG_ATTENTION_ENABLED;
        Actor_OfferTalk(&this->actor, play, 110.0f);
    }*/
    if (Actor_HasParent(&this->actor, play)) {
        this->actionFunc = EnTimeTag_SoaringEngraving_SubsequentInteraction;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(0x04006A), 0x04006A, 50.0f, 10.0f, true, true);
    }
}