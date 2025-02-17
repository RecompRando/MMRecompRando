#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define GI_EPONAS_SONG GI_A5

typedef enum RomaniLimb {
    /* 0x00 */ ROMANI_LIMB_NONE,
    /* 0x01 */ ROMANI_LIMB_ROOT,
    /* 0x02 */ ROMANI_LIMB_PELVIS,
    /* 0x03 */ ROMANI_LIMB_LEFT_THIGH,
    /* 0x04 */ ROMANI_LIMB_LEFT_SHIN,
    /* 0x05 */ ROMANI_LIMB_LEFT_FOOT,
    /* 0x06 */ ROMANI_LIMB_RIGHT_THIGH,
    /* 0x07 */ ROMANI_LIMB_RIGHT_SHIN,
    /* 0x08 */ ROMANI_LIMB_RIGHT_FOOT,
    /* 0x09 */ ROMANI_LIMB_DRESS_1,
    /* 0x0A */ ROMANI_LIMB_DRESS_2,
    /* 0x0B */ ROMANI_LIMB_DRESS_3,
    /* 0x0C */ ROMANI_LIMB_DRESS_4,
    /* 0x0D */ ROMANI_LIMB_TORSO,
    /* 0x0E */ ROMANI_LIMB_HEAD,
    /* 0x0F */ ROMANI_LIMB_UPPER_HAIR,
    /* 0x10 */ ROMANI_LIMB_LOWER_HAIR,
    /* 0x11 */ ROMANI_LIMB_LEFT_UPPER_ARM,
    /* 0x12 */ ROMANI_LIMB_LEFT_FOREARM,
    /* 0x13 */ ROMANI_LIMB_LEFT_HAND,
    /* 0x14 */ ROMANI_LIMB_RIGHT_UPPER_ARM,
    /* 0x15 */ ROMANI_LIMB_RIGHT_FOREARM,
    /* 0x16 */ ROMANI_LIMB_RIGHT_HAND,
    /* 0x17 */ ROMANI_LIMB_MAX
} RomaniLimb;

struct EnMa4;

typedef void (*EnMa4ActionFunc)(struct EnMa4*, PlayState*);

#define ENMA_GET_PATH_INDEX(thisx) (((thisx)->params & 0xFF00) >> 8)

typedef struct EnMa4 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnMa4ActionFunc actionFunc;
    /* 0x18C */ ColliderCylinder collider;
    /* 0x1D8 */ NpcInteractInfo interactInfo;
    /* 0x200 */ Vec3s* pathPoints;
    /* 0x204 */ Vec3s jointTable[ROMANI_LIMB_MAX];
    /* 0x28E */ UNK_TYPE1 unk28E[0x6];
    /* 0x294 */ Vec3s morphTable[ROMANI_LIMB_MAX];
    /* 0x31E */ UNK_TYPE1 unk31E[0x6];
    /* 0x324 */ s16 waypointIndex;
    /* 0x326 */ s16 pathPointsCount;
    /* 0x328 */ UNK_TYPE1 unk_328[0x2];
    /* 0x32A */ s16 blinkTimer;
    /* 0x33C */ s16 overrideEyeTexIndex;
    /* 0x32E */ s16 eyeTexIndex;
    /* 0x330 */ s16 mouthTexIndex;
    /* 0x332 */ s16 type;
    /* 0x334 */ u16 unk_334; // set but not used
    /* 0x336 */ s16 state;
    /* 0x338 */ s16 poppedBalloonCounter; // modified by EnPoFusen
    /* 0x33A */ s16 hasBow;
    /* 0x33C */ s16 animTimer;
    /* 0x33E */ u16 textId;
} EnMa4; // size = 0x340

typedef enum {
    /* 0 */ MA4_TYPE_DAY1,
    /* 1 */ MA4_TYPE_ALIENS_DEFEATED,
    /* 2 */ MA4_TYPE_ALIENS_WON
} EnMa4Type;

typedef enum {
    /* 0 */ MA4_STATE_DEFAULT,
    /* 1 */ MA4_STATE_HORSEBACKGAME,
    /* 2 */ MA4_STATE_AFTERHORSEBACKGAME,
    /* 3 */ MA4_STATE_AFTERDESCRIBETHEMCS
} EnMa4State;

void EnMa4_SetFaceExpression(EnMa4* this, s16 overrideEyeTexIndex, s16 mouthTexIndex);
void EnMa4_SetupBeginEponasSongCs(EnMa4* this);
void EnMa4_BeginEponasSongCs(EnMa4* this, PlayState* play);
void EnMa4_SetupBeginHorsebackGame(EnMa4* this);
void EnMa4_SetupBeginDescribeThemCs(EnMa4* this);
void EnMa4_Wait(EnMa4* this, PlayState* play);

RECOMP_PATCH void EnMa4_StartDialogue(EnMa4* this, PlayState* play) {
    s32 pad;
    OSTime time;

    switch (this->type) {
        case MA4_TYPE_DAY1:
            if (GET_PLAYER_FORM != PLAYER_FORM_HUMAN) {
                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_21_80)) {
                    EnMa4_SetFaceExpression(this, 3, 3);
                    Message_StartTextbox(play, 0x3337, &this->actor);
                    this->textId = 0x3337;
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_ROMANI);
                } else {
                    Message_StartTextbox(play, 0x3335, &this->actor);
                    this->textId = 0x3335;
                    SET_WEEKEVENTREG(WEEKEVENTREG_21_80);
                }
            } else if (this->state == MA4_STATE_DEFAULT) {
                // if (CHECK_WEEKEVENTREG(WEEKEVENTREG_21_40)) {
                if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_PROMISED_TO_HELP_WITH_ALIENS)) {
                    Message_StartTextbox(play, 0x3346, &this->actor);
                    this->textId = 0x3346;
                } else {
                    Message_StartTextbox(play, 0x3347, &this->actor);
                    this->textId = 0x3347;
                }
                // } else {
                //     Message_StartTextbox(play, 0x3338, &this->actor);
                //     this->textId = 0x3338;
                //     SET_WEEKEVENTREG(WEEKEVENTREG_21_40);
                // }
            } else if (this->state == MA4_STATE_AFTERHORSEBACKGAME) {
                if (gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2] >= SECONDS_TO_TIMER(120)) {
                    // "Too bad Grasshopper"
                    EnMa4_SetFaceExpression(this, 0, 0);
                    Message_StartTextbox(play, 0x336D, &this->actor);
                    this->textId = 0x336D;
                } else {
                    time = gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2];
                    if ((s32)time < HS_GET_HORSE_BACK_BALLOON_TIME()) {
                        // [Score] New record!
                        HS_SET_HORSE_BACK_BALLOON_TIME(time);
                        EnMa4_SetFaceExpression(this, 0, 3);
                        Message_StartTextbox(play, 0x3350, &this->actor);
                        this->textId = 0x3350;
                    } else {
                        // [Score] Great.
                        Message_StartTextbox(play, 0x3351, &this->actor);
                        this->textId = 0x3351;
                    }
                }
                this->state = MA4_STATE_DEFAULT;
                this->actor.flags &= ~ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
            } else if (this->state == MA4_STATE_AFTERDESCRIBETHEMCS) {
                // "Cremia doesn't believe me..."
                Message_StartTextbox(play, 0x3340, &this->actor);
                this->textId = 0x3340;
                this->actor.flags &= ~ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
            }
            break;

        case MA4_TYPE_ALIENS_DEFEATED:
            if (GET_PLAYER_FORM != PLAYER_FORM_HUMAN) {
                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_21_80)) {
                    EnMa4_SetFaceExpression(this, 3, 3);
                    Message_StartTextbox(play, 0x3337, &this->actor);
                    this->textId = 0x3337;
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_ROMANI);
                } else {
                    Message_StartTextbox(play, 0x3335, &this->actor);
                    this->textId = 0x3335;
                    SET_WEEKEVENTREG(WEEKEVENTREG_21_80);
                }
            } else if (this->state == MA4_STATE_DEFAULT) {
                Message_StartTextbox(play, 0x3354, &this->actor);
                this->textId = 0x3354;
            } else if (this->state == MA4_STATE_AFTERHORSEBACKGAME) {
                if (gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2] >= SECONDS_TO_TIMER(120)) {
                    // "Try again?"
                    Message_StartTextbox(play, 0x3356, &this->actor);
                    this->textId = 0x3356;
                } else {
                    time = gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2];
                    if ((s32)time < HS_GET_HORSE_BACK_BALLOON_TIME()) {
                        HS_SET_HORSE_BACK_BALLOON_TIME(time);
                        EnMa4_SetFaceExpression(this, 0, 3);
                        Message_StartTextbox(play, 0x3350, &this->actor);
                        this->textId = 0x3350;
                    } else {
                        Message_StartTextbox(play, 0x3351, &this->actor);
                        this->textId = 0x3351;
                    }
                }
                this->state = MA4_STATE_DEFAULT;
                this->actor.flags &= ~ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
            }
            break;

        case MA4_TYPE_ALIENS_WON:
            if (this->state == MA4_STATE_DEFAULT) {
                // "Huh... You?"
                Message_StartTextbox(play, 0x3358, &this->actor);
                this->textId = 0x3358;
            } else if (this->state == MA4_STATE_AFTERHORSEBACKGAME) {
                if (gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2] >= SECONDS_TO_TIMER(120)) {
                    // "Try again?"
                    Message_StartTextbox(play, 0x3356, &this->actor);
                    this->textId = 0x3356;
                } else {
                    time = gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2];
                    if ((s32)time < HS_GET_HORSE_BACK_BALLOON_TIME()) {
                        // New record
                        HS_SET_HORSE_BACK_BALLOON_TIME(time);
                        Message_StartTextbox(play, 0x335D, &this->actor);
                        this->textId = 0x335D;
                    } else {
                        // "Old record was: [record]"
                        Message_StartTextbox(play, 0x335E, &this->actor);
                        this->textId = 0x335E;
                    }
                }
                this->state = MA4_STATE_DEFAULT;
                this->actor.flags &= ~ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
            }
            break;

        default:
            // Dead code
            Message_StartTextbox(play, 0x3335, &this->actor);
            this->textId = 0x3335;
            break;
    }
}

void EnMa4_OfferMilkItem(EnMa4* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnMa4_Wait;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(GI_EPONAS_SONG), GI_EPONAS_SONG, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void EnMa4_ChooseNextDialogue(EnMa4* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s32 aux;

    if (Message_ShouldAdvance(play)) {
        switch (this->textId) {
            case 0x2390:
                Message_CloseTextbox(play);
                EnMa4_SetupBeginHorsebackGame(this);
                break;

            case 0x3335:
                EnMa4_SetFaceExpression(this, 0, 3);
                Message_StartTextbox(play, 0x3336, &this->actor);
                this->textId = 0x3336;
                Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_ROMANI);
                break;

            case 0x3338:
                Message_StartTextbox(play, 0x3339, &this->actor);
                this->textId = 0x3339;
                break;

            case 0x333A:
            case 0x333B:
                EnMa4_SetFaceExpression(this, 0, 3);
                Message_StartTextbox(play, 0x333C, &this->actor);
                this->textId = 0x333C;
                break;

            case 0x333C:
                EnMa4_SetFaceExpression(this, 0, 2);
                Message_StartTextbox(play, 0x333D, &this->actor);
                this->textId = 0x333D;
                break;

            case 0x333D:
                Message_StartTextbox(play, 0x333E, &this->actor);
                this->textId = 0x333E;
                break;

            case 0x333E:
                Message_CloseTextbox(play);
                EnMa4_SetupBeginDescribeThemCs(this);
                break;

            case 0x333F:
                Message_StartTextbox(play, 0x3340, &this->actor);
                this->textId = 0x3340;
                break;

            case 0x3340:
                EnMa4_SetFaceExpression(this, 0, 3);
                Message_StartTextbox(play, 0x3341, &this->actor);
                this->textId = 0x3341;
                break;

            case 0x3343:
                EnMa4_SetFaceExpression(this, 0, 0);
                Message_StartTextbox(play, 0x3344, &this->actor);
                this->textId = 0x3344;
                break;

            case 0x3344:
                Message_StartTextbox(play, 0x3345, &this->actor);
                this->textId = 0x3345;
                break;

            case 0x3345:
                Message_StartTextbox(play, 0x3349, &this->actor);
                this->textId = 0x3349;
                break;

            case 0x3349:
                Message_StartTextbox(play, 0x334A, &this->actor);
                this->textId = 0x334A;
                break;

            case 0x334A:
                Message_CloseTextbox(play);
                EnMa4_SetupBeginHorsebackGame(this);
                break;

            case 0x334C:
                Message_StartTextbox(play, 0x334D, &this->actor);
                this->textId = 0x334D;
                break;

            case 0x3350:
            case 0x3351:
            case 0x335D:
            case 0x335E:
                if (this->type == MA4_TYPE_DAY1) {
                    Message_StartTextbox(play, 0x3352, &this->actor);
                    this->textId = 0x3352;
                } else {
                    Message_StartTextbox(play, 0x3356, &this->actor);
                    this->textId = 0x3356;
                }
                break;

            case 0x3352: // Win minigame
            case 0x336D: // Lose minigame
                EnMa4_SetFaceExpression(this, 0, 0);

                // TODO: handle check and cutscene skip

                // Check if player has Epona's song
                //~ if (CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
                if (rando_location_is_checked(GI_MILK_BOTTLE)) {
                    Message_StartTextbox(play, 0x334C, &this->actor);
                    this->textId = 0x334C;
                } else {
                    Message_CloseTextbox(play);
                    this->actionFunc = EnMa4_OfferMilkItem;
                    //~ player->stateFlags1 |= PLAYER_STATE1_20;
                    //~ EnMa4_SetupBeginEponasSongCs(this);
                    //~ EnMa4_BeginEponasSongCs(this, play);
                }
                break;

            case 0x3358:
                if ((GET_PLAYER_FORM != PLAYER_FORM_HUMAN) || !CHECK_QUEST_ITEM(QUEST_SONG_EPONA)) {
                    Message_StartTextbox(play, 0x335C, &this->actor);
                    this->textId = 0x335C;
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_ROMANI);
                } else {
                    Message_StartTextbox(play, 0x3359, &this->actor);
                    this->textId = 0x3359;
                }
                break;

            default:
                break;
        }
    }
}