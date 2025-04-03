#include "global.h"
#include "modding.h"

#include "apcommon.h"

#define LOCATION_BEAVER_BOTTLE 0x09018D
#define LOCATION_BEAVER_HP 0x07018D

// #include "overlays/actors/ovl_En_Fish/z_en_fish.h"
#include "overlays/actors/ovl_En_Twig/z_en_twig.h"

#define FLAGS (ACTOR_FLAG_10 | ACTOR_FLAG_80000000)
#define THIS ((EnAz*)thisx)

struct EnAz;

typedef void (*EnAzActionFunc)(struct EnAz*, PlayState*);

#define BEAVER_GET_PARAM_F00(thisx) (((thisx)->params >> 8) & 0xF)
#define BEAVER_GET_PARAM_FF(thisx) ((thisx)->params & 0xFF)

#define BEAVER_OLDER_BROTHER_LIMB_MAX 0x18

typedef struct EnAz {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnAzActionFunc actionFunc;
    /* 0x148 */ SkelAnime skelAnime;
    /* 0x18C */ ColliderCylinder collider;
    /* 0x1D8 */ Vec3s jointTable[BEAVER_OLDER_BROTHER_LIMB_MAX]; // Also BEAVER_YOUNGER_BROTHER_LIMB_MAX
    /* 0x268 */ Vec3s morphTable[BEAVER_OLDER_BROTHER_LIMB_MAX]; // Also BEAVER_YOUNGER_BROTHER_LIMB_MAX
    /* 0x2F8 */ s16 unk_2F8;
    /* 0x2FA */ s16 unk_2FA; // cutscene state?
    /* 0x2FC */ s32 animIndex;
    /* 0x300 */ ActorPathing unk_300;
    /* 0x36C */ f32 unk_36C;
    /* 0x370 */ UNK_TYPE1 unk370[4];
    /* 0x374 */ u16 unk_374; // flags of some sort
    /* 0x376 */ u16 unk_376; // flags of some sort
    /* 0x378 */ u8 unk_378; // cutscene state?
    /* 0x37A */ s16 unk_37A;
    /* 0x37C */ s16 unk_37C;
    /* 0x37E */ s16 unk_37E;
    /* 0x380 */ s16 unk_380;
    /* 0x382 */ s16 unk_382;
    /* 0x384 */ s16 unk_384;
    /* 0x388 */ struct EnAz* brother;
    /* 0x38C */ UNK_TYPE1 unk38C[0x10];
    /* 0x39C */ s16 unk_39C;
    /* 0x39E */ s16 unk_39E; // some sort of rotation
    /* 0x3A0 */ UNK_TYPE1 unk3A0[4];
    /* 0x3A4 */ f32 unk_3A4;
    /* 0x3A8 */ Vec3f unk_3A8;
    /* 0x3B4 */ Vec3f unk_3B4; // translation 
    /* 0x3C0 */ s16 unk_3C0; // seems to do nothing
    /* 0x3C2 */ s16 unk_3C2;
    /* 0x3C4 */ s16 unk_3C4;
    /* 0x3C6 */ UNK_TYPE1 unk3C6[6];
    /* 0x3CC */ s32 getItemId;
    /* 0x3D0 */ s16 csIdList[1];
    /* 0x3D2 */ u16 unk_3D2;
    /* 0x3D4 */ s16 unk_3D4;
    /* 0x3D6 */ s16 unk_3D6;
} EnAz; // size = 0x3D8

struct EnFish;

typedef void (*EnFishUnkFunc)(Actor*, PlayState*);

typedef enum {
    /* -1 */ ENFISH_MINUS1 = -1,
    /*  0 */ ENFISH_0,
    /*  1 */ ENFISH_1,
    /*  2 */ ENFISH_2
} EnFishParam;

#define FISH_PARAMS(param) (param)

#define FISH_LIMB_MAX 0x07

typedef struct EnFish {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderJntSph collider;
    /* 0x164 */ ColliderJntSphElement colliderElements[1];
    /* 0x1A4 */ SkelAnime skelAnime;
    /* 0x1E8 */ Vec3s jointTable[FISH_LIMB_MAX];
    /* 0x212 */ Vec3s morphTable[FISH_LIMB_MAX];
    /* 0x23C */ EnFishUnkFunc unkFunc;
    /* 0x240 */ s16 unk_240;
    /* 0x242 */ s16 unk_242;
    /* 0x244 */ s16 unk_244;
    /* 0x246 */ s16 unk_246;
    /* 0x248 */ s32 updBgCheckInfoFlags;
    /* 0x24C */ f32 unk_24C;
    /* 0x250 */ f32 unk_250;
    /* 0x254 */ f32 unk_254;
    /* 0x258 */ f32 unk_258;
    /* 0x25C */ f32 unk_25C;
    /* 0x260 */ f32 unk_260;
    /* 0x264 */ s16 unk_264;
    /* 0x266 */ s16 unk_266;
    /* 0x268 */ s16 unk_268;
    /* 0x26A */ s16 unk_26A;
    /* 0x26C */ s16 unk_26C;
    /* 0x26E */ s16 unk_26E;
    /* 0x270 */ s16 unk_270;
    /* 0x272 */ s16 unk_272;
    /* 0x274 */ s16 unk_274;
    /* 0x276 */ s8 unk_276;
    /* 0x277 */ u8 unk_277;
    /* 0x278 */ s8 unk_278;
    /* 0x279 */ s8 unk_279;
} EnFish; // size = 0x27C

typedef enum {
    BEAVER_ANIM_NONE = -1,
    BEAVER_ANIM_IDLE,
    BEAVER_ANIM_WALK,
    BEAVER_ANIM_SWIM_WITH_SPINNING_TAIL,
    BEAVER_ANIM_SWIM_WITH_RAISED_TAIL,
    BEAVER_ANIM_TALK,
    BEAVER_ANIM_TALK_WAVE_ARMS,
    BEAVER_ANIM_LAUGH_RIGHT,
    BEAVER_ANIM_LAUGH_LEFT,
    BEAVER_ANIM_SWIM,
    BEAVER_ANIM_TALK_TO_LEFT,
    BEAVER_ANIM_TALK_TO_RIGHT,
    BEAVER_ANIM_BOW,
    BEAVER_ANIM_IDLE_FACE_LEFT,
    BEAVER_ANIM_IDLE_FACE_RIGHT,
    BEAVER_ANIM_IDLE_FACE_MAX
} BeaverAnimation;

extern Vec3f D_80A99E80;
extern f32 D_80A99E8C;
// extern AnimationSpeedInfo sAnimationSpeedInfo[BEAVER_ANIM_IDLE_FACE_MAX];
extern AnimationSpeedInfo D_80AEF868[BEAVER_ANIM_IDLE_FACE_MAX];

void func_80A982E0(PlayState *play, ActorPathing *actorPathing);
void func_80A98414(EnAz *this, PlayState *play);
void func_80A98EFC(EnAz *this, PlayState *play, u16 textId, s32 animIndex, s32 brotherAnimIndex);
void func_80A94AB8(EnAz *this, PlayState *play, s32 spawnIndex);
s32 func_80A94B98(EnAz *this, PlayState *play);
s32 func_80A95B34(PlayState *play, ActorPathing *actorPathing);
void func_80A97410(EnAz *this, PlayState *play);
void func_80A979DC(EnAz *this, PlayState *play);
void func_80A97A28(EnAz *this, PlayState *play);

RECOMP_PATCH s32 func_80A9617C(EnAz* this, PlayState* play) {
    s32 pad[2];
    s32 ret = 2;
    EnAz* brother = this->brother;

    switch (Message_GetState(&play->msgCtx)) {
        case TEXT_STATE_CHOICE:
        case TEXT_STATE_5:
        case TEXT_STATE_DONE:
            if ((play->msgCtx.currentTextId == 0x10DD) && (this->unk_374 & 0x8000)) {
                if (SubS_StartCutscene(&brother->actor, brother->csIdList[0], CS_ID_GLOBAL_TALK,
                                       SUBS_CUTSCENE_NORMAL)) {
                    brother->unk_374 |= 0x8000;
                    play->msgCtx.msgMode = MSGMODE_PAUSED;
                    ret = 0;
                }
            } else if (Message_ShouldAdvance(play)) {
                ret = 3;
                switch (play->msgCtx.currentTextId) {
                    case 0x70:
                    case 0xCD:
                        CLEAR_WEEKEVENTREG(WEEKEVENTREG_24_01);
                        this->actor.textId = 0x10F2;
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_BOW,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_BOW,
                                                        &brother->animIndex);
                        break;

                    case 0x10CE:
                        this->actor.textId = 0x10CF;
                        SET_WEEKEVENTREG(WEEKEVENTREG_16_40);
                        break;

                    case 0x10CF:
                        this->actor.textId = 0x10D0;
                        break;

                    case 0x10D0:
                        this->actor.textId = 0x10D1;
                        ret = 3;
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868,
                                                        BEAVER_ANIM_TALK_WAVE_ARMS, &this->animIndex);
                        break;

                    case 0x10D1:
                        this->actor.textId = 0x10D2;
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_TALK,
                                                        &this->animIndex);
                        break;

                    case 0x10D2:
                        if (play->msgCtx.choiceIndex == 0) {
                            Audio_PlaySfx_MessageDecide();
                            this->actor.textId = 0x10D6;
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868,
                                                            BEAVER_ANIM_TALK_WAVE_ARMS, &this->animIndex);
                        } else {
                            Audio_PlaySfx_MessageCancel();
                            this->actor.textId = 0x10D3;
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_BOW,
                                                            &this->animIndex);
                        }
                        break;

                    case 0x10D3:
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        this->unk_374 |= 0x20;
                        ret = 0;
                        break;

                    case 0x10D4:
                        this->actor.textId = 0x10D2;
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_TALK,
                                                        &this->animIndex);
                        break;

                    case 0x10D6:
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        func_80A979DC(this, play);
                        this->unk_2FA = 1;
                        ret = 0;
                        break;

                    case 0x10D7:
                        this->actor.textId = 0x10D8;
                        break;

                    case 0x10D8:
                        if (play->msgCtx.choiceIndex == 0) {
                            Audio_PlaySfx_MessageDecide();
                            switch (this->unk_2FA) {
                                case 2:
                                    this->unk_2FA = 1;
                                    break;

                                case 4:
                                    this->unk_2FA = 3;
                                    break;

                                case 7:
                                    this->unk_2FA = 6;
                                    break;

                                case 9:
                                    this->unk_2FA = 8;
                                    break;

                                default:
                                    break;
                            }
                            ret = 0;
                        } else {
                            Audio_PlaySfx_MessageCancel();
                            this->actor.textId = 0x10D9;
                        }
                        break;

                    case 0x10D9:
                        if ((this->unk_2FA == 3) || (this->unk_2FA == 8)) {
                            // CLEAR_WEEKEVENTREG(WEEKEVENTREG_24_04);
                        }
                        func_80A94AB8(this, play, 0);
                        func_80A979DC(this, play);
                        ret = 0;
                        break;

                    case 0x10DA:
                        this->actor.textId = 0x10DB;
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_TALK,
                                                        &this->animIndex);
                        break;

                    case 0x10DB:
                        if (play->msgCtx.choiceIndex == 0) {
                            Audio_PlaySfx_MessageDecide();
                            play->msgCtx.msgMode = MSGMODE_PAUSED;
                            this->unk_2FA = 1;
                            ret = 0;
                        } else {
                            Audio_PlaySfx_MessageCancel();
                            this->actor.textId = 0x10DC;
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_BOW,
                                                            &this->animIndex);
                        }
                        break;

                    case 0x10DC:
                        func_80A94AB8(this, play, 0);
                        func_80A979DC(this, play);
                        this->unk_374 |= 0x20;
                        ret = 0;
                        break;

                    case 0x10DD:
                        func_80A98EFC(this, play, 0x10DE, BEAVER_ANIM_IDLE_FACE_RIGHT, BEAVER_ANIM_TALK_TO_LEFT);
                        this->unk_374 |= 0x8000;
                        ret = 2;
                        break;

                    case 0x10DE:
                        func_80A98EFC(this, play, 0x10DF, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x10DF:
                        func_80A98EFC(this, play, 0x10E0, BEAVER_ANIM_IDLE_FACE_RIGHT, BEAVER_ANIM_TALK_TO_LEFT);
                        ret = 0;
                        break;

                    case 0x10E0:
                        func_80A98EFC(this, play, 0x10E1, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x10E1:
                        func_80A98EFC(this, play, 0x10E2, BEAVER_ANIM_IDLE_FACE_RIGHT, BEAVER_ANIM_TALK_TO_LEFT);
                        ret = 0;
                        break;

                    case 0x10E2:
                        this->actor.textId = 0x10E3;
                        ret = 3;
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_TALK,
                                                        &this->animIndex);
                        break;

                    case 0x10E3:
                        func_80A98EFC(this, play, 0x10E4, BEAVER_ANIM_IDLE, BEAVER_ANIM_TALK);
                        ret = 0;
                        break;

                    case 0x10E4:
                        func_80A98EFC(this, play, 0x10E5, BEAVER_ANIM_IDLE, BEAVER_ANIM_TALK);
                        ret = 0;
                        break;

                    case 0x10E5:
                        if (play->msgCtx.choiceIndex == 0) {
                            Audio_PlaySfx_MessageDecide();
                            this->actor.textId = 0x10E8;
                        } else {
                            Audio_PlaySfx_MessageCancel();
                            this->actor.textId = 0x10E6;
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868,
                                                            BEAVER_ANIM_TALK_TO_LEFT, &this->animIndex);
                            SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868,
                                                            BEAVER_ANIM_IDLE_FACE_RIGHT, &brother->animIndex);
                        }
                        break;

                    case 0x10E6:
                        this->actor.textId = 0x10E7;
                        func_80A98EFC(this, play, 0x10E7, BEAVER_ANIM_IDLE, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x10E7:
                        // CLEAR_WEEKEVENTREG(WEEKEVENTREG_24_04);
                        func_80A94AB8(this, play, 0);
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        func_80A979DC(this, play);
                        ret = 0;
                        break;

                    case 0x10E8:
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        this->unk_2FA = 3;
                        ret = 0;
                        break;

                    case 0x10E9:
                        func_80A98EFC(this, play, 0x10EA, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK);
                        ret = 0;
                        break;

                    case 0x10EA:
                        func_80A98EFC(this, play, 0x10EB, BEAVER_ANIM_IDLE, BEAVER_ANIM_TALK);
                        ret = 0;
                        break;

                    case 0x10EB:
                        if (play->msgCtx.choiceIndex == 0) {
                            play->msgCtx.msgMode = MSGMODE_PAUSED;
                            Audio_PlaySfx_MessageDecide();
                            switch (this->unk_2FA) {
                                case 4:
                                    this->unk_2FA = 3;
                                    break;

                                case 7:
                                    this->unk_2FA = 6;
                                    break;

                                case 9:
                                default:
                                    this->unk_2FA = 8;

                                    break;
                            }
                            ret = 0;
                        } else {
                            Audio_PlaySfx_MessageCancel();
                            this->actor.textId = 0x10EC;
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868,
                                                            BEAVER_ANIM_LAUGH_LEFT, &this->animIndex);
                            SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868,
                                                            BEAVER_ANIM_IDLE_FACE_RIGHT, &brother->animIndex);
                        }
                        break;

                    case 0x10EC:
                        func_80A98EFC(this, play, 0x10ED, BEAVER_ANIM_NONE, BEAVER_ANIM_LAUGH_RIGHT);
                        ret = 0;
                        break;

                    case 0x10ED:
                        if ((this->unk_2FA == 4) || (this->unk_2FA == 9)) {
                            // CLEAR_WEEKEVENTREG(WEEKEVENTREG_24_04);
                        }
                        func_80A94AB8(this, play, 0);
                        func_80A979DC(this, play);
                        ret = 0;
                        break;

                    case 0x10EE:
                        func_80A98EFC(this, play, 0x10EF, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x10EF:
                        func_80A98EFC(this, play, 0x10F0, BEAVER_ANIM_IDLE_FACE_RIGHT, BEAVER_ANIM_TALK_TO_LEFT);
                        ret = 0;
                        break;

                    case 0x10F0:
                        func_80A98EFC(this, play, 0x10F1, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x10F1:
                        SET_WEEKEVENTREG(WEEKEVENTREG_93_01);
                        // if (CHECK_WEEKEVENTREG(WEEKEVENTREG_23_80)) {
                        if (rando_location_is_checked(LOCATION_BEAVER_BOTTLE)) {
                            this->getItemId = GI_RUPEE_RED;
                        } else {
                            this->getItemId = GI_BOTTLE;
                            // SET_WEEKEVENTREG(WEEKEVENTREG_23_80);
                        }
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        ret = 7;
                        break;

                    case 0x10F2:
                    case 0x1109:
                        // CLEAR_WEEKEVENTREG(WEEKEVENTREG_24_04);
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        func_80A94AB8(this, play, 0);
                        func_80A979DC(this, play);
                        ret = 0;
                        break;

                    case 0x10F3:
                        func_80A98EFC(this, play, 0x10F4, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x10F4:
                        func_80A98EFC(this, play, 0x10F5, BEAVER_ANIM_IDLE_FACE_RIGHT, BEAVER_ANIM_TALK_TO_LEFT);
                        ret = 0;
                        break;

                    case 0x10F5:
                        func_80A98EFC(this, play, 0x10F6, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x10F6:
                        func_80A98EFC(this, play, 0x10F7, BEAVER_ANIM_IDLE_FACE_RIGHT, BEAVER_ANIM_TALK_WAVE_ARMS);
                        ret = 0;
                        break;

                    case 0x10F7:
                        this->actor.textId = 0x10F8;
                        ret = 3;
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_TALK,
                                                        &this->animIndex);
                        break;

                    case 0x10F8:
                        if (play->msgCtx.choiceIndex == 0) {
                            Audio_PlaySfx_MessageDecide();
                            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_25_01)) {
                                this->actor.textId = 0x1107;
                            } else {
                                this->actor.textId = 0x10FA;
                            }
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868,
                                                            BEAVER_ANIM_TALK_WAVE_ARMS, &this->animIndex);
                        } else {
                            Audio_PlaySfx_MessageCancel();
                            this->actor.textId = 0x10F9;
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_BOW,
                                                            &this->animIndex);
                            SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_BOW,
                                                            &brother->animIndex);
                        }
                        break;

                    case 0x10F9:
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        this->unk_374 |= 0x20;
                        ret = 0;
                        break;

                    case 0x10FA:
                    case 0x1107:
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        this->unk_2FA = 6;
                        ret = 0;
                        break;

                    case 0x10FB:
                        func_80A98EFC(this, play, 0x10FC, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x10FC:
                        func_80A98EFC(this, play, 0x10FD, BEAVER_ANIM_IDLE_FACE_RIGHT, BEAVER_ANIM_TALK);
                        ret = 0;
                        break;

                    case 0x10FD:
                        this->actor.textId = 0x10FE;
                        ret = 3;
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_TALK,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        break;

                    case 0x10FE:
                        if (play->msgCtx.choiceIndex == 0) {
                            Audio_PlaySfx_MessageDecide();
                            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_25_01)) {
                                this->actor.textId = 0x1108;
                            } else {
                                this->actor.textId = 0x1101;
                            }
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_TALK,
                                                            &this->animIndex);
                        } else {
                            Audio_PlaySfx_MessageCancel();
                            this->actor.textId = 0x10FF;
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868,
                                                            BEAVER_ANIM_TALK_TO_LEFT, &this->animIndex);
                            SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868,
                                                            BEAVER_ANIM_IDLE_FACE_RIGHT, &brother->animIndex);
                        }
                        break;

                    case 0x10FF:
                        func_80A98EFC(this, play, 0x1100, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x1100:
                        // CLEAR_WEEKEVENTREG(WEEKEVENTREG_24_04);
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        func_80A94AB8(this, play, 0);
                        func_80A979DC(this, play);
                        ret = 0;
                        break;

                    case 0x1101:
                    case 0x1108:
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        this->unk_2FA = 8;
                        ret = 0;
                        break;

                    case 0x1102:
                        func_80A98EFC(this, play, 0x1103, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x1103:
                        func_80A98EFC(this, play, 0x1104, BEAVER_ANIM_IDLE_FACE_RIGHT, BEAVER_ANIM_TALK_TO_LEFT);
                        ret = 0;
                        break;

                    case 0x1104:
                        func_80A98EFC(this, play, 0x1105, BEAVER_ANIM_IDLE_FACE_LEFT, BEAVER_ANIM_TALK_TO_RIGHT);
                        ret = 0;
                        break;

                    case 0x1105:
                        // if (CHECK_WEEKEVENTREG(WEEKEVENTREG_25_01)) {
                        if (rando_location_is_checked(LOCATION_BEAVER_HP)) {
                            this->getItemId = GI_RUPEE_PURPLE;
                        } else {
                            this->getItemId = GI_HEART_PIECE;
                            // SET_WEEKEVENTREG(WEEKEVENTREG_25_01);
                        }
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        ret = 7;
                        break;

                    case 0x1106:
                        // CLEAR_WEEKEVENTREG(WEEKEVENTREG_24_04);
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        SubS_ChangeAnimationBySpeedInfo(&brother->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &brother->animIndex);
                        func_80A94AB8(this, play, 0);
                        func_80A979DC(this, play);
                        ret = 0;
                        break;

                    case 0x10D5:
                    default:
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, BEAVER_ANIM_IDLE,
                                                        &this->animIndex);
                        this->unk_374 |= 0x20;
                        ret = 0;
                        break;
                }
            }
            break;

        case TEXT_STATE_NONE:
        case TEXT_STATE_1:
        case TEXT_STATE_CLOSING:
        case TEXT_STATE_3:
        default:
            break;
    }
    return ret;
}

RECOMP_PATCH s32 func_80A97274(EnAz* this, PlayState* play) {
    s32 textId;

    if (this->unk_374 & 0x4000) {
        return 0x10D7;
    }
    switch (this->unk_2FA) {
        case 0:
        default:
            // CLEAR_WEEKEVENTREG(WEEKEVENTREG_24_04);
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_16_40)) {
                textId = 0x10CE;
            } else {
                textId = 0x10D4;
            }
            break;

        case 2:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_24_01)) {
                SET_WEEKEVENTREG(WEEKEVENTREG_24_04);
                this->unk_3C0 = 0;
                textId = 0x10DD;
            } else {
                textId = 0x10DA;
            }
            break;

        case 4:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_24_01)) {
                textId = 0x10EE;
            } else {
                textId = 0x10E9;
            }
            break;

        case 5:
            // CLEAR_WEEKEVENTREG(WEEKEVENTREG_24_04);
            textId = 0x10F3;
            break;

        case 7:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_24_01)) {
                SET_WEEKEVENTREG(WEEKEVENTREG_24_04);
                textId = 0x10FB;
            } else {
                textId = 0x10E9;
            }
            break;

        case 9:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_24_01)) {
                textId = 0x1102;
            } else {
                textId = 0x10E9;
            }
            break;
    }
    return textId;
}

RECOMP_PATCH void func_80A97AB4(EnAz* this, PlayState* play) {
    switch (Message_GetState(&play->msgCtx)) {
        case TEXT_STATE_NONE:
            Message_StartTextbox(play, 0x10D7, NULL);
            break;

        case TEXT_STATE_CHOICE:
        case TEXT_STATE_5:
        case TEXT_STATE_DONE:
            if (Message_ShouldAdvance(play)) {
                switch (play->msgCtx.currentTextId) {
                    case 0x10D7:
                        Message_ContinueTextbox(play, 0x10D8);
                        break;

                    case 0x10D8:
                        if (play->msgCtx.choiceIndex == 0) {
                            Audio_PlaySfx_MessageDecide();
                            play->msgCtx.msgMode = MSGMODE_PAUSED;
                            Environment_ForcePlaySequence(NA_BGM_TIMED_MINI_GAME);
                            func_80A94AB8(this, play, 1);
                            func_80A979DC(this, play);
                        } else {
                            Audio_PlaySfx_MessageCancel();
                            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_24_04)) {
                                // CLEAR_WEEKEVENTREG(WEEKEVENTREG_24_04);
                            }
                            Message_ContinueTextbox(play, 0x10D9);
                        }
                        break;

                    case 0x10D9:
                        func_80A94AB8(this, play, 0);
                        func_80A979DC(this, play);
                        break;

                    default:
                        break;
                }
            }
        case TEXT_STATE_1:
        case TEXT_STATE_CLOSING:
        default:
            break;
    }
}