#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnDno;

typedef void (*EnDnoActionFunc)(struct EnDno*, PlayState*);

#define DEKU_BUTLER_LIMB_MAX 0x1C

#define EN_DNO_GET_7F(thisx) ((thisx)->params & 0x7F)
#define EN_DNO_GET_RACE_STARTED_SWITCH_FLAG(thisx) (((thisx)->params >> 7) & 0x7F)
#define EN_DNO_GET_C000(thisx) (((thisx)->params >> 0xE) & 0x3)

#define LOCATION_BUTLER GI_MASK_SCENTS

typedef enum {
    /* 0 */ EN_DNO_GET_C000_0,
    /* 1 */ EN_DNO_GET_C000_1
} EnDnoUnkC000;

typedef struct EnDno {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnDnoActionFunc actionFunc;
    /* 0x148 */ SkelAnime skelAnime;
    /* 0x18C */ ColliderCylinder collider;
    /* 0x1D8 */ Vec3s jointTable[DEKU_BUTLER_LIMB_MAX];
    /* 0x280 */ Vec3s morphTable[DEKU_BUTLER_LIMB_MAX];
    /* 0x328 */ s32 unk_328;
    /* 0x32C */ s32 animIndex;
    /* 0x330 */ UNK_TYPE1 unk_330[0x4];
    /* 0x334 */ Vec3f unk_334;
    /* 0x340 */ ActorPathing actorPath;
    /* 0x3AC */ s16 unk_3AC;
    /* 0x3AE */ s16 unk_3AE;
    /* 0x3B0 */ u16 unk_3B0;
    /* 0x3B2 */ UNK_TYPE1 unk_3B2[0xC];
    /* 0x3BE */ s16 unk_3BE;
    /* 0x3C0 */ f32 unk_3C0;
    /* 0x3C4 */ UNK_TYPE1 unk_3C4[0x78];
    /* 0x43C */ LightNode* lightNode;
    /* 0x440 */ LightInfo lightInfo;
    /* 0x44E */ u8 unk_44E;
    /* 0x44F */ UNK_TYPE1 unk_44F[0x3];
    /* 0x452 */ s16 unk_452;
    /* 0x454 */ f32 unk_454;
    /* 0x458 */ s32 getItemId;
    /* 0x45C */ s16 unk_45C;
    /* 0x460 */ Actor* unk_460;
    /* 0x464 */ u16 textId;
    /* 0x466 */ s16 unk_466;
    /* 0x468 */ u8 cueId;
} EnDno; // size = 0x46C

void func_80A71B04(EnDno* this, PlayState* play);
void EnDno_DoNothing(EnDno* this, PlayState* play);
void func_80A71B68(EnDno* this, PlayState* play);
void func_80A71C3C(EnDno* this, PlayState* play);
void func_80A71E54(EnDno* this, PlayState* play);
void func_80A71F18(EnDno* this, PlayState* play);
void func_80A72438(EnDno* this, PlayState* play);
void func_80A724B8(EnDno* this, PlayState* play);
void func_80A725E0(EnDno* this, PlayState* play);
void func_80A725F8(EnDno* this, PlayState* play);
void func_80A72AE4(EnDno* this, PlayState* play);
void func_80A72B3C(EnDno* this, PlayState* play);
void func_80A72B84(EnDno* this, PlayState* play);
void func_80A72BA4(EnDno* this, PlayState* play);
void func_80A72C04(EnDno* this, PlayState* play);
void func_80A72CF8(EnDno* this, PlayState* play);
void func_80A730A0(EnDno* this, PlayState* play);
void func_80A73244(EnDno* this, PlayState* play);
void func_80A732C8(EnDno* this, PlayState* play);
s32 EnDno_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx);
void EnDno_PostLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3s* rot, Actor* thisx);

typedef enum {
    /* -1 */ EN_DNO_ANIM_NONE = -1,
    /*  0 */ EN_DNO_ANIM_START_RACE_START,
    /*  1 */ EN_DNO_ANIM_START_RACE_END,
    /*  2 */ EN_DNO_ANIM_FLY,
    /*  3 */ EN_DNO_ANIM_FAREWELL,
    /*  4 */ EN_DNO_ANIM_GREETING,
    /*  5 */ EN_DNO_ANIM_GREETING_WITH_CANDLE,
    /*  6 */ EN_DNO_ANIM_PRAYER_LOOP,
    /*  7 */ EN_DNO_ANIM_CLOSE_PARASOL,
    /*  8 */ EN_DNO_ANIM_OPEN_PARASOL,
    /*  9 */ EN_DNO_ANIM_IMPLORE_LOOP,
    /* 10 */ EN_DNO_ANIM_IMPLORE_END,
    /* 11 */ EN_DNO_ANIM_TALK,
    /* 12 */ EN_DNO_ANIM_TALK_WITH_PARSOL_AND_CANDLE,
    /* 13 */ EN_DNO_ANIM_IDLE,
    /* 14 */ EN_DNO_ANIM_IDLE_WITH_CANDLE,
    /* 15 */ EN_DNO_ANIM_PRAYER_START,
    /* 16 */ EN_DNO_ANIM_IMPLORE_START,
    /* 17 */ EN_DNO_ANIM_SHOCK_START,
    /* 18 */ EN_DNO_ANIM_SHOCK_LOOP,
    /* 19 */ EN_DNO_ANIM_GRIEVE,
    /* 20 */ EN_DNO_ANIM_MAX
} EnDnoAnimation;

extern AnimationHeader gDekuButlerStartRaceStartAnim;
extern AnimationHeader gDekuButlerStartRaceEndAnim;
extern AnimationHeader gDekuButlerFlyAnim;
extern AnimationHeader gDekuButlerFarewellAnim;
extern AnimationHeader gDekuButlerGreetingAnim;
extern AnimationHeader gDekuButlerGreetingWithCandleAnim;
extern AnimationHeader gDekuButlerPrayerLoopAnim;
extern AnimationHeader gDekuButlerCloseParasolAnim;
extern AnimationHeader gDekuButlerOpenParasolAnim;
extern AnimationHeader gDekuButlerImploreLoopAnim;
extern AnimationHeader gDekuButlerImploreEndAnim;
extern AnimationHeader gDekuButlerTalkAnim;
extern AnimationHeader gDekuButlerTalkWithParasolAndCandleAnim;
extern AnimationHeader gDekuButlerIdleAnim;
extern AnimationHeader gDekuButlerIdleWithCandleAnim;
extern AnimationHeader gDekuButlerPrayerStartAnim;
extern AnimationHeader gDekuButlerImploreStartAnim;
extern AnimationHeader gDekuButlerShockStartAnim;
extern AnimationHeader gDekuButlerShockLoopAnim;
extern AnimationHeader gDekuButlerGrieveAnim;

static AnimationSpeedInfo sAnimationSpeedInfo[EN_DNO_ANIM_MAX] = {
    { &gDekuButlerStartRaceStartAnim, 1.0f, ANIMMODE_ONCE, 0.0f },           // EN_DNO_ANIM_START_RACE_START
    { &gDekuButlerStartRaceEndAnim, 1.0f, ANIMMODE_ONCE, 0.0f },             // EN_DNO_ANIM_START_RACE_END
    { &gDekuButlerFlyAnim, 1.0f, ANIMMODE_LOOP, 0.0f },                      // EN_DNO_ANIM_FLY
    { &gDekuButlerFarewellAnim, 1.0f, ANIMMODE_ONCE, 0.0f },                 // EN_DNO_ANIM_FAREWELL
    { &gDekuButlerGreetingAnim, 1.0f, ANIMMODE_ONCE, 0.0f },                 // EN_DNO_ANIM_GREETING
    { &gDekuButlerGreetingWithCandleAnim, 1.0f, ANIMMODE_ONCE, 0.0f },       // EN_DNO_ANIM_GREETING_WITH_CANDLE
    { &gDekuButlerPrayerLoopAnim, 1.0f, ANIMMODE_LOOP, 0.0f },               // EN_DNO_ANIM_PRAYER_LOOP
    { &gDekuButlerCloseParasolAnim, 1.0f, ANIMMODE_ONCE, 0.0f },             // EN_DNO_ANIM_CLOSE_PARASOL
    { &gDekuButlerOpenParasolAnim, 1.0f, ANIMMODE_ONCE, 0.0f },              // EN_DNO_ANIM_OPEN_PARASOL
    { &gDekuButlerImploreLoopAnim, 1.0f, ANIMMODE_LOOP, 0.0f },              // EN_DNO_ANIM_IMPLORE_LOOP
    { &gDekuButlerImploreEndAnim, 1.0f, ANIMMODE_ONCE, 0.0f },               // EN_DNO_ANIM_IMPLORE_END
    { &gDekuButlerTalkAnim, 1.0f, ANIMMODE_LOOP, 0.0f },                     // EN_DNO_ANIM_TALK
    { &gDekuButlerTalkWithParasolAndCandleAnim, 1.0f, ANIMMODE_LOOP, 0.0f }, // EN_DNO_ANIM_TALK_WITH_PARSOL_AND_CANDLE
    { &gDekuButlerIdleAnim, 1.0f, ANIMMODE_LOOP, 0.0f },                     // EN_DNO_ANIM_IDLE
    { &gDekuButlerIdleWithCandleAnim, 1.0f, ANIMMODE_LOOP, 0.0f },           // EN_DNO_ANIM_IDLE_WITH_CANDLE
    { &gDekuButlerPrayerStartAnim, 1.0f, ANIMMODE_ONCE, 0.0f },              // EN_DNO_ANIM_PRAYER_START
    { &gDekuButlerImploreStartAnim, 1.0f, ANIMMODE_ONCE, 0.0f },             // EN_DNO_ANIM_IMPLORE_START
    { &gDekuButlerShockStartAnim, 1.0f, ANIMMODE_ONCE, 0.0f },               // EN_DNO_ANIM_SHOCK_START
    { &gDekuButlerShockLoopAnim, 1.0f, ANIMMODE_LOOP, 0.0f },                // EN_DNO_ANIM_SHOCK_LOOP
    { &gDekuButlerGrieveAnim, 1.0f, ANIMMODE_LOOP, 0.0f },                   // EN_DNO_ANIM_GRIEVE
};

s32 func_80A71424(s16* arg0, s16 arg1, s16 yawToPlayer, s16 rotY, s16 arg4, s16 arg5);

RECOMP_PATCH void func_80A725F8(EnDno* this, PlayState* play) {
    s32 pad[2];

    func_80A71424(&this->unk_466, 0, 0, 0, 0x2000, 0x16C);
    switch (Message_GetState(&play->msgCtx)) {
        case TEXT_STATE_NONE:
            switch (this->unk_328) {
                case 0:
                    if (this->animIndex == EN_DNO_ANIM_IDLE_WITH_CANDLE) {
                        if (Math_ScaledStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0x2D8)) {
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, sAnimationSpeedInfo,
                                                            EN_DNO_ANIM_GREETING_WITH_CANDLE, &this->animIndex);
                        }
                    } else if ((this->animIndex == EN_DNO_ANIM_GREETING_WITH_CANDLE) &&
                               Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                        if (Flags_GetSwitch(play, EN_DNO_GET_RACE_STARTED_SWITCH_FLAG(&this->actor))) {
                            Message_StartTextbox(play, 0x801, &this->actor);
                        } else if (Player_GetMask(play) == PLAYER_MASK_SCENTS) {
                            Message_StartTextbox(play, 0x806, &this->actor);
                        } else {
                            Message_StartTextbox(play, 0x800, &this->actor);
                        }
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, sAnimationSpeedInfo,
                                                        EN_DNO_ANIM_IDLE_WITH_CANDLE, &this->animIndex);
                    }
                    break;

                case 2:
                    if (Math_ScaledStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 0x2D8)) {
                        SET_WEEKEVENTREG(WEEKEVENTREG_93_02);
                        Message_StartTextbox(play, 0x802, &this->actor);
                    }
                    break;

                case 3:
                    Message_StartTextbox(play, 0x804, &this->actor);
                    break;

                default:
                    break;
            }
            break;

        case TEXT_STATE_NEXT:
        case TEXT_STATE_CLOSING:
        case TEXT_STATE_FADING:
            if (((play->msgCtx.currentTextId == 0x800) || (play->msgCtx.currentTextId == 0x801)) &&
                (this->animIndex == EN_DNO_ANIM_OPEN_PARASOL)) {
                Math_SmoothStepToF(&this->unk_454, 1.0f, 1.0f, 0.1f, 0.01f);
                if (this->skelAnime.curFrame <= 23.0f) {
                    this->unk_452 = 3;
                    if (Animation_OnFrame(&this->skelAnime, 23.0f)) {
                        Actor_PlaySfx(&this->actor, NA_SE_EV_OPEN_AMBRELLA);
                    }
                } else if (this->skelAnime.curFrame <= 24.0f) {
                    this->unk_452 = 4;
                } else if (this->skelAnime.curFrame >= 25.0f) {
                    this->unk_452 = 2;
                }

                if (Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
                    SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, sAnimationSpeedInfo,
                                                    EN_DNO_ANIM_TALK_WITH_PARSOL_AND_CANDLE, &this->animIndex);
                    Message_StartTextbox(play, 0x803, &this->actor);
                }
            }
            break;

        case TEXT_STATE_CHOICE:
        case TEXT_STATE_EVENT:
        case TEXT_STATE_DONE:
            switch (play->msgCtx.currentTextId) {
                case 0x800:
                case 0x801:
                    if (Message_ShouldAdvance(play)) {
                        play->msgCtx.msgMode = MSGMODE_PAUSED;
                        this->unk_452 = 1;
                        this->unk_454 = 0.0f;
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, sAnimationSpeedInfo, EN_DNO_ANIM_OPEN_PARASOL,
                                                        &this->animIndex);
                    }
                    break;

                case 0x802:
                    if (Message_ShouldAdvance(play)) {
                        //if (INV_CONTENT(ITEM_MASK_SCENTS) == ITEM_MASK_SCENTS) {
                        if (rando_location_is_checked(LOCATION_BUTLER)) {
                            this->getItemId = GI_RUPEE_RED;
                        } else {
                            this->getItemId = GI_MASK_SCENTS;
                        }
                        Actor_OfferGetItem(&this->actor, play, this->getItemId, 60.0f, 60.0f);
                        Message_CloseTextbox(play);
                        func_80A72B84(this, play);
                    }
                    break;

                case 0x803:
                    if (Message_ShouldAdvance(play)) {
                        func_80A72AE4(this, play);
                    }
                    break;

                case 0x804:
                    if (this->animIndex == EN_DNO_ANIM_IDLE_WITH_CANDLE) {
                        if (Message_ShouldAdvance(play)) {
                            SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, sAnimationSpeedInfo,
                                                            EN_DNO_ANIM_GREETING_WITH_CANDLE, &this->animIndex);
                            if (!(this->unk_3B0 & 0x40)) {
                                func_80A72CF8(this, play);
                                this->unk_3B0 |= 0x40;
                            }
                        }
                    } else if ((this->animIndex == EN_DNO_ANIM_GREETING_WITH_CANDLE) &&
                               (this->skelAnime.curFrame == this->skelAnime.endFrame)) {
                        Message_CloseTextbox(play);
                        func_80A72438(this, play);
                    }
                    break;

                case 0x806:
                    if (Message_ShouldAdvance(play)) {
                        Message_ContinueTextbox(play, 0x800);
                    }
                    break;

                default:
                    func_80A72438(this, play);
                    break;
            }
            break;
    }
}