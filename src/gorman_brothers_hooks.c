#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnIn;

typedef void (*EnInActionFunc)(struct EnIn*, PlayState*);

typedef enum {
    /* 0 */ ENIN_UNK_TYPE,
    /* 1 */ ENIN_HORSE_RIDER_YELLOW_SHIRT,
    /* 2 */ ENIN_HORSE_RIDER_BLUE_SHIRT,
    /* 3 */ ENIN_YELLOW_SHIRT,
    /* 4 */ ENIN_BLUE_SHIRT
} EnInType;

#define ENIN_GET_TYPE(thisx) ((thisx)->params & 0x1FF)
#define ENIN_GET_PATH_INDEX(thisx) (((thisx)->params & 0x7E00) >> 9) // Only used with ENIN_UNK_TYPE

#define ENIN_PATH_INDEX_NONE 0x3F

#define LOCATION_RACE_GORMAN GI_MASK_GARO

typedef enum EnInAnimation {
    /* -1 */ ENIN_ANIM_NONE = -1,
    /*  0 */ ENIN_ANIM_0,
    /*  1 */ ENIN_ANIM_1,
    /*  2 */ ENIN_ANIM_2,
    /*  3 */ ENIN_ANIM_3,
    /*  4 */ ENIN_ANIM_4,
    /*  5 */ ENIN_ANIM_5,
    /*  6 */ ENIN_ANIM_6,
    /*  7 */ ENIN_ANIM_7,
    /*  8 */ ENIN_ANIM_8,
    /*  9 */ ENIN_ANIM_9,
    /* 10 */ ENIN_ANIM_10,
    /* 11 */ ENIN_ANIM_11,
    /* 12 */ ENIN_ANIM_12,
    /* 13 */ ENIN_ANIM_13,
    /* 14 */ ENIN_ANIM_14,
    /* 15 */ ENIN_ANIM_15,
    /* 16 */ ENIN_ANIM_16,
    /* 17 */ ENIN_ANIM_17,
    /* 18 */ ENIN_ANIM_MAX
} EnInAnimation;

typedef enum EnInAnimation2 {
    /* -1 */ ENIN_ANIM2_NONE = -1,
    /*  0 */ ENIN_ANIM2_0,
    /*  1 */ ENIN_ANIM2_1,
    /*  2 */ ENIN_ANIM2_2,
    /*  3 */ ENIN_ANIM2_3,
    /*  4 */ ENIN_ANIM2_4,
    /*  5 */ ENIN_ANIM2_5,
    /*  6 */ ENIN_ANIM2_6,
    /*  7 */ ENIN_ANIM2_7,
    /*  8 */ ENIN_ANIM2_MAX
} EnInAnimation2;

#define OBJECT_IN_LIMB_MAX 0x14

typedef struct EnIn {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnInActionFunc actionFunc;
    /* 0x148 */ SkelAnime skelAnime;
    /* 0x18C */ ColliderJntSph colliderJntSph;
    /* 0x1AC */ ColliderJntSphElement colliderJntSphElement;
    /* 0x1EC */ ColliderCylinder colliderCylinder;
    /* 0x238 */ UNK_TYPE1 unk238[0x4];
    /* 0x23C */ u8 unk23C;
    /* 0x23D */ u8 unk23D;
    /* 0x240 */ Path* path;
    /* 0x244 */ s16 unk244;
    /* 0x248 */ Vec3f unk248;
    /* 0x254 */ Vec3f unk254;
    /* 0x260 */ u8 unk260;
    /* 0x261 */ u8 unk261;
    /* 0x262 */ Vec3s jointTable[OBJECT_IN_LIMB_MAX];
    /* 0x2DA */ Vec3s morphTable[OBJECT_IN_LIMB_MAX];
    /* 0x352 */ Vec3s trackTarget;
    /* 0x358 */ Vec3s headRot;
    /* 0x35E */ Vec3s torsoRot;
    /* 0x364 */ UNK_TYPE1 unk364[0x12];
    /* 0x376 */ s16 fidgetTableY[OBJECT_IN_LIMB_MAX];
    /* 0x39E */ s16 fidgetTableZ[OBJECT_IN_LIMB_MAX];
    /* 0x3C6 */ UNK_TYPE1 unk3C6[0xBC];
    /* 0x482 */ s16 unk482;
    /* 0x484 */ s16 unk484;
    /* 0x486 */ s16 unk486;
    /* 0x488 */ s16 animIndex2;
    /* 0x48A */ u16 unk48A;
    /* 0x48C */ s32 unk48C;
    /* 0x490 */ UNK_TYPE1 unk490[0x4];
    /* 0x494 */ s32 unk494;
    /* 0x498 */ UNK_TYPE1 unk498[0xC];
    /* 0x4A4 */ struct EnIn* unk4A4;
    /* 0x4A8 */ s32 unk4A8;
    /* 0x4AC */ s32 unk4AC;
    /* 0x4B0 */ s32 unk4B0;
    /* 0x4B4 */ Vec3f unk4B4;
    /* 0x4C0 */ f32 unk4C0;
    /* 0x4C0 */ f32 unk4C4;
    /* 0x4C0 */ s32 prevTalkState;
} EnIn; // size = 0x4CC

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

#define THIS ((EnIn*)thisx)

void func_808F5A94(EnIn* this, PlayState* play);
void func_808F3690(EnIn* this, PlayState* play);
void func_808F5A34(EnIn* this, PlayState* play);
s32 func_808F5994(EnIn* this, PlayState* play, Vec3f* arg2, s16 arg3);
s32 EnIn_ChangeAnim(SkelAnime* skelAnime, s16 animIndex);
void func_808F4108(EnIn* this, PlayState* play, u16 textId);
void func_808F3C40(EnIn* this, PlayState* play);
s32 func_808F4150(PlayState* play, EnIn* this, s32 arg2, MessageContext* msgCtx);
s32 func_808F43E0(EnIn* this);
void func_808F35D8(EnIn* this, PlayState* play);
void func_808F3B40(EnIn* this, PlayState* play);
void func_808F3D40(EnIn* this, PlayState* play);
s32 func_808F4270(PlayState* play, EnIn* this, s32 arg2, MessageContext* msgCtx, s32 arg4);

RECOMP_PATCH s32 func_808F4414(PlayState* play, EnIn* this, s32 arg2) {
    u16 textId = this->actor.textId;
    s32 ret = true;
    MessageContext* msgCtx = &play->msgCtx;
    s32 pad[2];

    switch (textId) {
        case 0x34AF:
            Actor_ContinueText(play, &this->actor, 0x34B0);
            ret = false;
            break;

        case 0x34A9:
            func_808F4108(this, play, 0x34AA);
            SET_WEEKEVENTREG(WEEKEVENTREG_63_40);
            ret = false;
            break;

        case 0x34AA:
            func_808F4108(this, play, 0x34AB);
            ret = false;
            break;

        case 0x34AB:
            func_808F4108(this, play, 0x34AC);
            ret = false;
            break;

        case 0x34AC:
            func_808F4108(this, play, 0x34AD);
            ret = false;
            break;

        case 0x34AD:
            Actor_ContinueText(play, &this->actor, 0x34AE);
            ret = false;
            break;

        case 0x34B1:
            Actor_ContinueText(play, &this->actor, 0x34B2);
            ret = false;
            break;

        default:
            break;
    }

    switch (arg2) {
        case 0:
            switch (textId) {
                case 0x3458:
                    SET_WEEKEVENTREG(WEEKEVENTREG_15_08);
                    Actor_ContinueText(play, &this->actor, 0x3459);
                    ret = false;
                    break;

                case 0x3459:
                    Actor_ContinueText(play, &this->actor, 0x345A);
                    ret = false;
                    break;

                case 0x345A:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x345B:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x345C:
                    Actor_ContinueText(play, &this->actor, 0x345D);
                    ret = false;
                    break;

                case 0x345D:
                    Actor_ContinueText(play, &this->actor, 0x345E);
                    ret = false;
                    break;

                case 0x345E:
                    Actor_ContinueText(play, &this->actor, 0x345F);
                    ret = false;
                    break;

                case 0x345F:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x3460:
                    Actor_ContinueText(play, &this->actor, 0x3461);
                    ret = false;
                    break;

                case 0x3461:
                    Actor_ContinueText(play, &this->actor, 0x3462);
                    ret = false;
                    break;

                case 0x3462:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                default:
                    break;
            }
            break;

        case 1:
            switch (textId) {
                case 0x3463:
                    SET_WEEKEVENTREG(WEEKEVENTREG_15_10);
                    Actor_ContinueText(play, &this->actor, 0x3464);
                    ret = false;
                    break;

                case 0x3464:
                    Actor_ContinueText(play, &this->actor, 0x3465);
                    ret = false;
                    break;

                case 0x3465:
                    Actor_ContinueText(play, &this->actor, 0x3466);
                    ret = false;
                    break;

                case 0x3466:
                    if (msgCtx->choiceIndex == 0) {
                        Audio_PlaySfx_MessageDecide();
                        if (gSaveContext.save.saveInfo.playerData.rupees >= play->msgCtx.unk1206C) {
                            // if (Inventory_HasEmptyBottle()) {
                            if (rando_shopsanity_enabled()) {
                                this->actionFunc = func_808F3C40;
                                Actor_OfferGetItem(&this->actor, play, GI_MILK, 500.0f, 100.0f);
                                Rupees_ChangeBy(-play->msgCtx.unk1206C);
                                ret = true;
                            } else {
                                Actor_ContinueText(play, &this->actor, 0x3469);
                                ret = false;
                            }
                        } else {
                            Audio_PlaySfx(NA_SE_SY_ERROR);
                            Actor_ContinueText(play, &this->actor, 0x3468);
                            ret = false;
                        }
                    } else {
                        Audio_PlaySfx_MessageCancel();
                        Actor_ContinueText(play, &this->actor, 0x3467);
                        ret = false;
                    }
                    break;

                case 0x3467:
                case 0x3468:
                case 0x3469:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x346B:
                    Actor_ContinueText(play, &this->actor, 0x346C);
                    ret = false;
                    break;

                case 0x346C:
                    Actor_ContinueText(play, &this->actor, 0x3466);
                    ret = false;
                    break;

                case 0x346A:
                    this->actionFunc = func_808F5A94;
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;
            }
            break;

        case 2:
            switch (textId) {
                case 0x346E:
                    func_808F4108(this, play, 0x346F);
                    ret = false;
                    break;

                case 0x3470:
                    Actor_ContinueText(play, &this->actor, 0x3471);
                    ret = false;
                    break;

                case 0x3483:
                    Actor_ContinueText(play, &this->actor, 0x3471);
                    ret = false;
                    break;

                case 0x3471:
                    func_808F4150(play, this, arg2, msgCtx);
                    ret = false;
                    break;

                case 0x3472:
                    func_808F43E0(this);
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x3473:
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;

                case 0x3475:
                    SET_WEEKEVENTREG_HORSE_RACE_STATE(WEEKEVENTREG_HORSE_RACE_STATE_START);
                    Environment_ForcePlaySequence(NA_BGM_HORSE);
                    play->nextEntrance = ENTRANCE(GORMAN_TRACK, 5);
                    play->transitionType = TRANS_TYPE_FADE_WHITE_FAST;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    SET_WEEKEVENTREG(WEEKEVENTREG_57_01);
                    break;

                case 0x3478:
                    if (msgCtx->choiceIndex == 0) {
                        func_808F4150(play, this, arg2, msgCtx);
                        ret = false;
                    } else {
                        Audio_PlaySfx_MessageCancel();
                        CLEAR_WEEKEVENTREG(WEEKEVENTREG_56_08);
                        func_808F4108(this, play, 0x3479);
                        ret = false;
                    }
                    break;

                case 0x347B:
                    func_808F4108(this, play, 0x347C);
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    ret = false;
                    break;

                default:
                    break;
            }
            break;

        case 3:
            switch (textId) {
                case 0x346D:
                    func_808F4108(this, play, 0x346E);
                    SET_WEEKEVENTREG(WEEKEVENTREG_56_04);
                    SET_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    ret = false;
                    break;

                case 0x346F:
                    func_808F4108(this, play, 0x3470);
                    ret = false;
                    break;

                case 0x3482:
                    func_808F4108(this, play, 0x3483);
                    SET_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    ret = false;
                    break;

                case 0x3484:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;

                case 0x3485:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;

                case 0x3474:
                    func_808F4108(this, play, 0x3475);
                    ret = false;
                    break;

                case 0x3476:
                    Actor_ContinueText(play, &this->actor, 0x3477);
                    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_1);
                    EnIn_ChangeAnim(&this->unk4A4->skelAnime, ENIN_ANIM_7);
                    ret = false;
                    break;

                case 0x3477:
                    SET_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    func_808F4108(this, play, 0x3478);
                    ret = false;
                    break;

                case 0x347A:
                    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_1);
                    EnIn_ChangeAnim(&this->unk4A4->skelAnime, ENIN_ANIM_7);
                    //if (INV_CONTENT(ITEM_MASK_GARO) == ITEM_MASK_GARO) {
                    if (rando_location_is_checked(LOCATION_RACE_GORMAN)) {
                        Actor_ContinueText(play, &this->actor, 0x347E);
                        ret = false;
                    } else {
                        SET_WEEKEVENTREG(WEEKEVENTREG_56_08);
                        func_808F4108(this, play, 0x347B);
                        ret = false;
                    }
                    break;

                case 0x347E:
                    func_808F35D8(this, play);
                    // if (Inventory_HasEmptyBottle()) {
                    if (rando_shopsanity_enabled()) {
                        this->actionFunc = func_808F3B40;
                        Actor_OfferGetItem(&this->actor, play, GI_MILK, 500.0f, 100.0f);
                        ret = true;
                    } else {
                        Actor_ContinueText(play, &this->actor, 0x347F);
                        ret = false;
                    }
                    break;

                case 0x347F:
                    Actor_ContinueText(play, &this->actor, 0x3480);
                    ret = false;
                    break;

                case 0x3480:
                    func_808F43E0(this);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x3479:
                    func_808F43E0(this);
                    func_808F35D8(this, play);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x347C:
                    this->actionFunc = func_808F3D40;
                    Actor_OfferGetItem(&this->actor, play, GI_MASK_GARO, 500.0f, 100.0f);
                    func_808F35D8(this, play);
                    ret = true;
                    break;

                case 0x3481:
                    this->actionFunc = func_808F5A34;
                    func_808F43E0(this);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x347D:
                    this->actionFunc = func_808F5A34;
                    func_808F43E0(this);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_GAROS_MASK);
                    ret = true;
                    break;

                default:
                    break;
            }
            break;

        case 4:
            switch (textId) {
                case 0x3486:
                    Actor_ContinueText(play, &this->actor, 0x3487);
                    SET_WEEKEVENTREG(WEEKEVENTREG_16_01);
                    ret = false;
                    break;

                case 0x3487:
                    Actor_ContinueText(play, &this->actor, 0x3488);
                    ret = false;
                    break;

                case 0x3488:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x3489:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;

                case 0x348A:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;

                case 0x348B:
                    Actor_ContinueText(play, &this->actor, 0x348C);
                    ret = false;
                    break;

                case 0x348C:
                    Actor_ContinueText(play, &this->actor, 0x348D);
                    ret = false;
                    break;

                case 0x348D:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                default:
                    break;
            }
            break;

        case 5:
            switch (textId) {
                case 0x3468:
                case 0x3469:
                case 0x3491:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;

                case 0x348E:
                case 0x34B3:
                    Actor_ContinueText(play, &this->actor, 0x348F);
                    SET_WEEKEVENTREG(WEEKEVENTREG_16_02);
                    ret = false;
                    break;

                case 0x3493:
                    Actor_ContinueText(play, &this->actor, 0x3494);
                    ret = false;
                    break;

                case 0x348F:
                case 0x3494:
                    Actor_ContinueText(play, &this->actor, 0x3490);
                    ret = false;
                    break;

                case 0x3490:
                    if (msgCtx->choiceIndex == 0) {
                        Audio_PlaySfx_MessageDecide();
                        if (gSaveContext.save.saveInfo.playerData.rupees >= play->msgCtx.unk1206C) {
                            // if (Inventory_HasEmptyBottle()) {
                            if (rando_shopsanity_enabled()) {
                                this->actionFunc = func_808F3C40;
                                Actor_OfferGetItem(&this->actor, play, GI_MILK, 500.0f, 100.0f);
                                Rupees_ChangeBy(-play->msgCtx.unk1206C);
                                ret = true;
                            } else {
                                Actor_ContinueText(play, &this->actor, 0x3469);
                                ret = false;
                            }
                        } else {
                            Audio_PlaySfx(NA_SE_SY_ERROR);
                            Actor_ContinueText(play, &this->actor, 0x3468);
                            ret = false;
                        }
                    } else {
                        Audio_PlaySfx_MessageCancel();
                        Actor_ContinueText(play, &this->actor, 0x3491);
                        ret = false;
                    }
                    break;

                case 0x3492:
                    this->actionFunc = func_808F5A94;
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;
            }
            break;

        case 7:
            switch (textId) {
                case 0x34A8:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;

                case 0x34A7:
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;

                case 0x3495:
                    func_808F4108(this, play, 0x3496);
                    SET_WEEKEVENTREG(WEEKEVENTREG_16_04);
                    SET_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    ret = false;
                    break;

                case 0x3497:
                    func_808F4108(this, play, 0x3498);
                    ret = false;
                    break;

                case 0x34A4:
                    this->actionFunc = func_808F5A34;
                    func_808F43E0(this);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x34A5:
                    func_808F4108(this, play, 0x34A6);
                    SET_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    ret = false;
                    break;

                case 0x3473:
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    break;

                case 0x3474:
                    Actor_ContinueText(play, &this->actor, 0x3475);
                    ret = false;
                    break;

                case 0x3475:
                    SET_WEEKEVENTREG_HORSE_RACE_STATE(WEEKEVENTREG_HORSE_RACE_STATE_START);
                    Environment_ForcePlaySequence(NA_BGM_HORSE);
                    play->nextEntrance = ENTRANCE(GORMAN_TRACK, 5);
                    play->transitionType = TRANS_TYPE_FADE_WHITE_FAST;
                    play->transitionTrigger = TRANS_TRIGGER_START;
                    SET_WEEKEVENTREG(WEEKEVENTREG_57_01);
                    break;

                case 0x349D:
                    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_1);
                    EnIn_ChangeAnim(&this->unk4A4->skelAnime, ENIN_ANIM_7);
                    //if (INV_CONTENT(ITEM_MASK_GARO) == ITEM_MASK_GARO) {
                    //if (recomp_location_is_checked(GI_MASK_GARO)) {
                    if (false) {
                        Actor_ContinueText(play, &this->actor, 0x34A1);
                        ret = false;
                    } else {
                        SET_WEEKEVENTREG(WEEKEVENTREG_56_08);
                        func_808F4108(this, play, 0x349E);
                        ret = false;
                    }
                    break;

                case 0x349F:
                    this->actionFunc = func_808F3D40;
                    Actor_OfferGetItem(&this->actor, play, GI_MASK_GARO, 500.0f, 100.0f);
                    func_808F35D8(this, play);
                    ret = true;
                    break;

                case 0x34A0:
                    this->actionFunc = func_808F5A34;
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_GAROS_MASK);
                    func_808F43E0(this);
                    ret = true;
                    break;

                case 0x34A1:
                    func_808F35D8(this, play);
                    // if (Inventory_HasEmptyBottle()) {
                    if (rando_shopsanity_enabled()) {
                        this->actionFunc = func_808F3B40;
                        Actor_OfferGetItem(&this->actor, play, GI_MILK, 500.0f, 100.0f);
                        ret = true;
                    } else {
                        Actor_ContinueText(play, &this->actor, 0x34A2);
                        ret = false;
                    }
                    break;

                case 0x34A2:
                    Actor_ContinueText(play, &this->actor, 0x34A3);
                    ret = false;
                    break;

                case 0x34A3:
                    func_808F43E0(this);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x3499:
                    Actor_ContinueText(play, &this->actor, 0x349A);
                    EnIn_ChangeAnim(&this->skelAnime, ENIN_ANIM_1);
                    EnIn_ChangeAnim(&this->unk4A4->skelAnime, ENIN_ANIM_7);
                    ret = false;
                    break;

                case 0x349A:
                    Actor_ContinueText(play, &this->actor, 0x349B);
                    ret = false;
                    break;

                case 0x349B:
                    if (msgCtx->choiceIndex == 0) {
                        func_808F4270(play, this, arg2, msgCtx, 1);
                        ret = false;
                    } else {
                        Audio_PlaySfx_MessageCancel();
                        Actor_ContinueText(play, &this->actor, 0x349C);
                        ret = false;
                    }
                    break;

                case 0x349C:
                    func_808F43E0(this);
                    func_808F35D8(this, play);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                default:
                    break;
            }
            break;

        case 6:
            switch (textId) {
                case 0x3496:
                    func_808F4108(this, play, 0x3497);
                    ret = false;
                    break;

                case 0x3498:
                case 0x34A6:
                    Actor_ContinueText(play, &this->actor, 0x3471);
                    ret = false;
                    break;

                case 0x3471:
                    func_808F4270(play, this, arg2, msgCtx, 0);
                    ret = false;
                    break;

                case 0x3472:
                    func_808F43E0(this);
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_GORMAN_BROTHERS);
                    ret = true;
                    break;

                case 0x349E:
                    func_808F4108(this, play, 0x349F);
                    CLEAR_WEEKEVENTREG(WEEKEVENTREG_56_08);
                    ret = false;
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }
    return ret;
}