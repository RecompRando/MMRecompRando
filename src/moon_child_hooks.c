#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnJs;

typedef void (*EnJsActionFunc)(struct EnJs*, PlayState*);

#define MOONCHILD_LIMB_MAX 0x12

typedef struct EnJs {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ ColliderCylinder collider;
    /* 0x1D4 */ Vec3s jointTable[MOONCHILD_LIMB_MAX];
    /* 0x240 */ Vec3s morphTable[MOONCHILD_LIMB_MAX];
    /* 0x2AC */ Path* path;
    /* 0x2B0 */ s32 unk_2B0;
    /* 0x2B4 */ f32 unk_2B4;
    /* 0x2B8 */ u16 unk_2B8;
    /* 0x2BA */ s16 maskType;
    /* 0x2BC */ s16 unk_2BC;
    /* 0x2BE */ s16 csIdList[2];
    /* 0x2C2 */ s16 csIdIndex;
    /* 0x2C4 */ EnJsActionFunc actionFunc;
} EnJs; // size = 0x2C8

bool rando_met_majora_condition() {
    return ((CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA) > 0) +
            (CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT) > 0) +
            (CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG) > 0) +
            (CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD) > 0)) >= rando_get_majora_remains_required();
}

extern AnimationHeader gMoonChildStandingAnim;
extern AnimationHeader gMoonChildGettingUpAnim;

void func_809696EC(EnJs* this, s16 csIdIndex);
void func_8096A2C0(EnJs* this, PlayState* play);
s32 func_809695FC(EnJs* this, PlayState* play);
void func_80969530(EnJs* this, PlayState* play);

RECOMP_PATCH void func_8096A38C(EnJs* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        Animation_MorphToLoop(&this->skelAnime, &gMoonChildStandingAnim, 0.0f);
        this->unk_2B8 |= 2;
    }
    switch (Message_GetState(&play->msgCtx)) {
        case TEXT_STATE_CHOICE:
            if (Message_ShouldAdvance(play)) {
                switch (play->msgCtx.choiceIndex) {
                    case 0:
                        Audio_PlaySfx_MessageDecide();
                        break;

                    case 1:
                        Audio_PlaySfx_MessageCancel();
                        break;

                    default:
                        break;
                }

                switch (play->msgCtx.currentTextId) {
                    case 0x21FC:
                        switch (play->msgCtx.choiceIndex) {
                            case 0:
                                // @rando send player back to first day if they don't meet the victory condition
                                if (!rando_met_majora_condition()) {
                                    Message_ContinueTextbox(play, 0x3550);
                                    Animation_MorphToPlayOnce(&this->skelAnime, &gMoonChildGettingUpAnim, -5.0f);
                                    this->unk_2B8 |= 0x10;
                                    break;
                                }
                                Message_ContinueTextbox(play, 0x21FE);
                                Animation_MorphToPlayOnce(&this->skelAnime, &gMoonChildGettingUpAnim, -5.0f);
                                this->unk_2B8 |= 0x10;
                                break;

                            case 1:
                                Message_ContinueTextbox(play, 0x21FD);
                                break;

                            default:
                                break;
                        }
                        break;

                    case 0x21FE:
                        switch (play->msgCtx.choiceIndex) {
                            case 0:
                                Message_ContinueTextbox(play, 0x2200);
                                func_809696EC(this, 0);
                                break;

                            case 1:
                                Message_ContinueTextbox(play, 0x21FF);
                                break;

                            default:
                                break;
                        }
                        break;

                    case 0x2203:
                        switch (play->msgCtx.choiceIndex) {
                            case 0:
                                // @rando send player back to first day if they don't meet the victory condition
                                if (rando_location_is_checked(GI_MASK_FIERCE_DEITY) && !rando_met_majora_condition()) {
                                    Message_ContinueTextbox(play, 0x3550);
                                    Animation_MorphToPlayOnce(&this->skelAnime, &gMoonChildGettingUpAnim, -5.0f);
                                    this->unk_2B8 |= 0x10;
                                    break;
                                }
                                Message_ContinueTextbox(play, 0x2205);
                                Animation_MorphToPlayOnce(&this->skelAnime, &gMoonChildGettingUpAnim, -5.0f);
                                this->unk_2B8 |= 0x10;
                                break;

                            case 1:
                                Message_ContinueTextbox(play, 0x2204);
                                break;

                            default:
                                break;
                        }
                        break;

                    // @rando send player back to first day if they don't meet the victory condition
                    case 0x3550:
                        switch (play->msgCtx.choiceIndex) {
                            case 0:
                                Message_CloseTextbox(play);
                                SET_EVENTINF(EVENTINF_TRIGGER_DAYTELOP);
                                gSaveContext.save.day = 0;
                                gSaveContext.respawnFlag = -4;
                                play->nextEntrance = ENTRANCE(SOUTH_CLOCK_TOWN, 0);
                                gSaveContext.save.entrance = play->nextEntrance;
                                play->transitionTrigger = TRANS_TRIGGER_START;
                                play->transitionType = TRANS_TYPE_FADE_BLACK;
                                Sram_SaveEndOfCycle(play);
                                break;

                            case 1:
                                Message_ContinueTextbox(play, 0x21FD);
                                break;

                            default:
                                break;
                        }
                        break;

                    default:
                        break;
                }
            }
            break;

        case TEXT_STATE_5:
            if (Message_ShouldAdvance(play)) {
                switch (play->msgCtx.currentTextId) {
                    case 0x2202:
                    case 0x2205:
                    case 0x2206:
                    case 0x2209:
                        Message_ContinueTextbox(play, play->msgCtx.currentTextId + 1);
                        break;

                    case 0x2200:
                    case 0x2208:
                        func_809696EC(this, 1);
                        Message_ContinueTextbox(play, play->msgCtx.currentTextId + 1);
                        break;

                    case 0x2207:
                        // if (INV_CONTENT(ITEM_MASK_FIERCE_DEITY) == ITEM_MASK_FIERCE_DEITY) {
                        //     Message_ContinueTextbox(play, 0x2208);
                        //     func_809696EC(this, 0);
                        // } else {
                        Message_CloseTextbox(play);
                        this->actionFunc = func_8096A2C0;
                        func_8096A2C0(this, play);
                        // }
                        break;

                    case 0x2201:
                    case 0x220A:
                        if (!func_809695FC(this, play)) {
                            func_80969530(this, play);
                        }
                        break;

                    default:
                        func_80969530(this, play);
                        break;
                }
            }
            break;

        default:
            break;
    }
}

void func_8096A1E8(EnJs* this, PlayState* play);

void EnJs_PreventFightAfterFDOffer(EnJs* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        Animation_MorphToLoop(&this->skelAnime, &gMoonChildStandingAnim, 0.0f);
    }
    if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
        this->actor.flags &= ~ACTOR_FLAG_10000;
        this->actionFunc = func_8096A38C;
        Message_StartTextbox(play, 0x3550, &this->actor);
    } else {
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
    }
}

RECOMP_PATCH void func_8096A2C0(EnJs* this, PlayState* play) {
    if (SkelAnime_Update(&this->skelAnime)) {
        Animation_MorphToLoop(&this->skelAnime, &gMoonChildStandingAnim, 0.0f);
    }
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actor.flags |= ACTOR_FLAG_10000;
        if (rando_met_majora_condition()) {
            this->actionFunc = func_8096A1E8;
        } else {
            this->actionFunc = EnJs_PreventFightAfterFDOffer;
        }
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_MASK_FIERCE_DEITY, 10000.0f, 1000.0f);
    }
}