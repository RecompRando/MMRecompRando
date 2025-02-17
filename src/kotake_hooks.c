#include "modding.h"
#include "global.h"
#include "overlays/actors/ovl_En_GirlA/z_en_girla.h"
#include "assets/objects/gameplay_keep/gameplay_keep.h"

#include "apcommon.h"

struct EnTrt;

typedef void (*EnTrtActionFunc)(struct EnTrt*, PlayState*);
typedef void (*EnTrtBlinkFunc)(struct EnTrt*);

typedef struct EnTrt {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnTrtActionFunc actionFunc;
    /* 0x148 */ EnTrtActionFunc prevActionFunc;
    /* 0x14C */ SkelAnime skelAnime;
    /* 0x190 */ ColliderCylinder collider;
    /* 0x1DC */ UNK_TYPE1 pad1DC[0x154];
    /* 0x330 */ s16 delayTimer;
    /* 0x332 */ s16 sleepSoundTimer;
    /* 0x334 */ s16 cutsceneState;
    /* 0x338 */ s32 stickAccumX;
    /* 0x33C */ s32 stickAccumY;
    /* 0x340 */ EnGirlA* items[3];
    /* 0x34C */ u8 cursorIndex;
    /* 0x350 */ StickDirectionPrompt stickLeftPrompt;
    /* 0x388 */ StickDirectionPrompt stickRightPrompt;
    /* 0x3C0 */ f32 shopItemSelectedTween;
    /* 0x3C4 */ f32 cursorAnimTween;
    /* 0x3C8 */ u8 cursorAnimState;
    /* 0x3CC */ f32 arrowAnimTween;
    /* 0x3D0 */ f32 stickAnimTween;
    /* 0x3D4 */ u8 arrowAnimState;
    /* 0x3D5 */ u8 stickAnimState;
    /* 0x3D6 */ s16 blinkTimer;
    /* 0x3D8 */ s16 eyeTexIndex;
    /* 0x3DC */ EnTrtBlinkFunc blinkFunc;
    /* 0x3E0 */ s16 csId;
    /* 0x3E2 */ s16 tmpGetMushroomCsId;
    /* 0x3E4 */ Vec3f cursorPos;
    /* 0x3F0 */ Color_RGBAu32 cursorColor;
    /* 0x400 */ u8 drawCursor;
    /* 0x402 */ s16 timer;
    /* 0x404 */ s16 animIndex;
    /* 0x406 */ u16 textId;
    /* 0x408 */ u16 prevTextId;
    /* 0x40A */ u16 talkOptionTextId;
    /* 0x40C */ s16 lookForwardCutscene;
    /* 0x40E */ s16 lookToShelfCsId;
    /* 0x410 */ s16 lookToShopkeeperCsId;
    /* 0x412 */ s16 getMushroomCsId;
    /* 0x414 */ UNK_TYPE1 pad414[0x2];
    /* 0x416 */ Vec3s headRot;
    /* 0x41C */ Vec3f headPos;
    /* 0x428 */ s16 headPitch;
    /* 0x42A */ s16 headYaw;
    /* 0x42C */ u16 flags;
} EnTrt; // size = 0x430

typedef enum {
    /* 0 */ ENTRT_CUTSCENESTATE_STOPPED,
    /* 1 */ ENTRT_CUTSCENESTATE_WAITING,
    /* 2 */ ENTRT_CUTSCENESTATE_PLAYING_SPECIAL, // Used to do/set extra flags (such as waiting mid animation)
    /* 3 */ ENTRT_CUTSCENESTATE_PLAYING
} EnTrtCutsceneState;

void EnTrt_GivenRedPotionForKoume(EnTrt* this, PlayState* play);

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

void EnTrt_OfferItem(EnTrt* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE)) {
            SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE);
        }
        SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_RED_POTION_FOR_KOUME);
        player->stateFlags2 &= ~PLAYER_STATE2_20000000;
        this->actionFunc = EnTrt_GivenRedPotionForKoume;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(GI_POTION_RED_BOTTLE), GI_POTION_RED_BOTTLE, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void EnTrt_GiveRedPotionForKoume(EnTrt* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE)) {
            SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE);
        }
        SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_RED_POTION_FOR_KOUME);
        player->stateFlags2 &= ~PLAYER_STATE2_20000000;
        this->actionFunc = EnTrt_GivenRedPotionForKoume;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, GI_POTION_RED, 0, 300.0f, 300.0f, true, false);
    }
}

void EnTrt_SetupOffer(EnTrt* this) {
    if (rando_location_is_checked(GI_POTION_RED_BOTTLE)) {
        this->actionFunc = EnTrt_GiveRedPotionForKoume;
    } else {
        this->actionFunc = EnTrt_OfferItem;
    }
}

void EnTrt_Blink(EnTrt* this);
void EnTrt_EndConversation(EnTrt* this, PlayState* play);

RECOMP_PATCH void EnTrt_TryToGiveRedPotionAfterSurprised(EnTrt* this, PlayState* play) {
    u8 talkState = Message_GetState(&play->msgCtx);

    this->blinkFunc = EnTrt_Blink;
    if ((talkState == TEXT_STATE_DONE) && Message_ShouldAdvance(play)) {
        if (Inventory_HasEmptyBottle() || !CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE)) {
            if (this->cutsceneState == ENTRT_CUTSCENESTATE_PLAYING) {
                CutsceneManager_Stop(this->csId);
                this->cutsceneState = ENTRT_CUTSCENESTATE_STOPPED;
            }
            EnTrt_SetupOffer(this);
        } else {
            this->prevTextId = this->textId;
            this->textId = 0x88E;
            SET_WEEKEVENTREG(WEEKEVENTREG_FAILED_RECEIVED_RED_POTION_FOR_KOUME_SHOP);
            Message_StartTextbox(play, this->textId, &this->actor);
            this->actionFunc = EnTrt_EndConversation;
        }
    }
}

RECOMP_PATCH void EnTrt_TryToGiveRedPotion(EnTrt* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        if (this->textId == 0x83C) {
            if (Inventory_HasEmptyBottle()) {
                if (this->cutsceneState == ENTRT_CUTSCENESTATE_PLAYING) {
                    CutsceneManager_Stop(this->csId);
                    this->cutsceneState = ENTRT_CUTSCENESTATE_STOPPED;
                }
                play->msgCtx.msgMode = MSGMODE_TEXT_CLOSING;
                play->msgCtx.stateTimer = 4;
                EnTrt_SetupOffer(this);
            } else {
                this->prevTextId = this->textId;
                this->textId = 0x88E;
                SET_WEEKEVENTREG(WEEKEVENTREG_FAILED_RECEIVED_RED_POTION_FOR_KOUME_SHOP);
                Message_StartTextbox(play, this->textId, &this->actor);
                this->actionFunc = EnTrt_EndConversation;
            }
        } else {
            this->textId = 0x83C;
            Message_StartTextbox(play, this->textId, &this->actor);
        }
    }
}

void EnTrt_SetupStartShopping(PlayState* play, EnTrt* this, u8 skipHello);
void EnTrt_EndConversation(EnTrt* this, PlayState* play);
void EnTrt_TryToGiveRedPotion(EnTrt* this, PlayState* play);
void EnTrt_Surprised(EnTrt* this, PlayState* play);

RECOMP_PATCH void EnTrt_StartRedPotionConversation(EnTrt* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        if (this->textId == 0x88F) {
            //if (Inventory_HasEmptyBottle() || !CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE)) {
            if (!rando_location_is_checked(GI_POTION_RED_BOTTLE) || !CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE)) {
                if (this->cutsceneState == ENTRT_CUTSCENESTATE_PLAYING) {
                    CutsceneManager_Stop(this->csId);
                    this->cutsceneState = ENTRT_CUTSCENESTATE_STOPPED;
                }
                play->msgCtx.msgMode = MSGMODE_TEXT_CLOSING;
                play->msgCtx.stateTimer = 4;
                EnTrt_SetupOffer(this);
            } else {
                this->prevTextId = this->textId;
                this->textId = 0x88E;
                SET_WEEKEVENTREG(WEEKEVENTREG_FAILED_RECEIVED_RED_POTION_FOR_KOUME_SHOP);
                Message_StartTextbox(play, this->textId, &this->actor);
                this->actionFunc = EnTrt_EndConversation;
            }
        } else {
            //if (CHECK_WEEKEVENTREG(WEEKEVENTREG_TALKED_KOUME_INJURED)) {
            if (!rando_location_is_checked(GI_POTION_RED_BOTTLE) || CHECK_WEEKEVENTREG(WEEKEVENTREG_TALKED_KOUME_INJURED)) {
                this->timer = 30;
                this->textId = 0x838;
                this->cutsceneState = ENTRT_CUTSCENESTATE_PLAYING_SPECIAL;
                this->actionFunc = EnTrt_Surprised;
                return;
            } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_SAVED_KOUME)) {
                this->textId = 0x83D;
                EnTrt_SetupStartShopping(play, this, 0);
            } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_RED_POTION_FOR_KOUME)) {
                this->textId = 0x83B;
                if (Inventory_HasItemInBottle(ITEM_POTION_RED)) {
                    EnTrt_SetupStartShopping(play, this, false);
                } else {
                    this->actionFunc = EnTrt_TryToGiveRedPotion;
                }
            } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_TALKED_KOUME_KIOSK_EMPTY)) {
                this->textId = 0x835;
                EnTrt_SetupStartShopping(play, this, false);
            }
            Message_StartTextbox(play, this->textId, &this->actor);
        }
    }
}

typedef enum TrtAnimation {
    /*  0 */ TRT_ANIM_IDLE,
    /*  1 */ TRT_ANIM_HALF_AWAKE,
    /*  2 */ TRT_ANIM_SLEEPING,
    /*  3 */ TRT_ANIM_WAKE_UP,
    /*  4 */ TRT_ANIM_SURPRISED,
    /*  5 */ TRT_ANIM_HANDS_ON_COUNTER,
    /*  6 */ TRT_ANIM_HOVER,
    /*  7 */ TRT_ANIM_FLY_LOOK_AROUND,
    /*  8 */ TRT_ANIM_FLY_DOWN,
    /*  9 */ TRT_ANIM_FLY,
    /* 10 */ TRT_ANIM_MAX
} TrtAnimation;

extern AnimationHeader gKotakeIdleAnim;
extern AnimationHeader gKotakeHalfAwakeAnim;
extern AnimationHeader gKotakeSleepingAnim;
extern AnimationHeader gKotakeWakeUpAnim;
extern AnimationHeader gKotakeSurprisedAnim;
extern AnimationHeader gKotakeHandsOnCounterAnim;
extern AnimationHeader gKotakeHoverAnim;
extern AnimationHeader gKotakeFlyLookAroundAnim;
extern AnimationHeader gKotakeFlyDownAnim;
extern AnimationHeader gKotakeFlyAnim;

static AnimationInfoS sAnimationInfo[TRT_ANIM_MAX] = {
    { &gKotakeIdleAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },           // TRT_ANIM_IDLE
    { &gKotakeHalfAwakeAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },      // TRT_ANIM_HALF_AWAKE
    { &gKotakeSleepingAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },       // TRT_ANIM_SLEEPING
    { &gKotakeWakeUpAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },         // TRT_ANIM_WAKE_UP
    { &gKotakeSurprisedAnim, 1.0f, 0, -1, ANIMMODE_ONCE, 0 },      // TRT_ANIM_SURPRISED
    { &gKotakeHandsOnCounterAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 }, // TRT_ANIM_HANDS_ON_COUNTER
    { &gKotakeHoverAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },          // TRT_ANIM_HOVER
    { &gKotakeFlyLookAroundAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },  // TRT_ANIM_FLY_LOOK_AROUND
    { &gKotakeFlyDownAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },        // TRT_ANIM_FLY_DOWN
    { &gKotakeFlyAnim, 1.0f, 0, -1, ANIMMODE_LOOP, 0 },            // TRT_ANIM_FLY
};

void Interface_SetAButtonDoAction(PlayState* play, u16 aButtonDoAction);

void EnTrt_OpenEyesThenSetToBlink(EnTrt* this);
void EnTrt_ChangeAnim(SkelAnime* skelAnime, AnimationInfoS* animationInfo, s32 animIndex);
void EnTrt_Hello(EnTrt* this, PlayState* play);
void EnTrt_FaceShopkeeper(EnTrt* this, PlayState* play);
void EnTrt_SetupEndInteraction(EnTrt* this, PlayState* play);

RECOMP_PATCH void EnTrt_BeginInteraction(EnTrt* this, PlayState* play) {
    s16 curFrame = this->skelAnime.curFrame / this->skelAnime.playSpeed;
    s16 endFrame = Animation_GetLastFrame(&gKotakeWakeUpAnim) / TRUNCF_BINANG(this->skelAnime.playSpeed);

    if (this->cutsceneState == ENTRT_CUTSCENESTATE_WAITING) {
        if (CutsceneManager_IsNext(this->csId)) {
            CutsceneManager_StartWithPlayerCsAndSetFlag(this->csId, &this->actor);
            this->cutsceneState = ENTRT_CUTSCENESTATE_PLAYING_SPECIAL;
        } else {
            CutsceneManager_Queue(this->csId);
        }
    } else if (this->cutsceneState == ENTRT_CUTSCENESTATE_PLAYING_SPECIAL) {
        if (this->animIndex != TRT_ANIM_HANDS_ON_COUNTER) {
            if (curFrame == endFrame) {
                EnTrt_ChangeAnim(&this->skelAnime, sAnimationInfo, TRT_ANIM_WAKE_UP);
                this->animIndex = TRT_ANIM_WAKE_UP;
                this->blinkFunc = EnTrt_OpenEyesThenSetToBlink;
                this->timer = 10;
                this->cutsceneState = ENTRT_CUTSCENESTATE_PLAYING;
                Actor_PlaySfx(&this->actor, NA_SE_EN_KOTAKE_SURPRISED2);
            }
        } else {
            this->blinkFunc = EnTrt_OpenEyesThenSetToBlink;
            this->timer = 10;
            this->cutsceneState = ENTRT_CUTSCENESTATE_PLAYING;
        }
    } else if (DECR(this->timer) == 0) {
        this->timer = Rand_S16Offset(40, 20);
        EnTrt_ChangeAnim(&this->skelAnime, sAnimationInfo, TRT_ANIM_HANDS_ON_COUNTER);
        Message_StartTextbox(play, this->textId, &this->actor);
        this->animIndex = TRT_ANIM_HANDS_ON_COUNTER;
        switch (this->textId) {
            case 0x834:
                if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_SAVED_KOUME) &&
                    !CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_RED_POTION_FOR_KOUME) &&
                    !CHECK_WEEKEVENTREG(WEEKEVENTREG_TALKED_KOUME_INJURED) &&
                    !CHECK_WEEKEVENTREG(WEEKEVENTREG_TALKED_KOUME_KIOSK_EMPTY)) {
                    Interface_SetAButtonDoAction(play, DO_ACTION_DECIDE);
                    this->stickLeftPrompt.isEnabled = false;
                    this->stickRightPrompt.isEnabled = true;
                    this->actionFunc = EnTrt_Hello;
                } else {
                    this->actionFunc = EnTrt_StartRedPotionConversation;
                }
                break;

            case 0x83E:
                Interface_SetAButtonDoAction(play, DO_ACTION_DECIDE);
                this->stickLeftPrompt.isEnabled = false;
                this->stickRightPrompt.isEnabled = true;
                this->actionFunc = EnTrt_FaceShopkeeper;
                break;

            case 0x850:
            case 0x890:
                this->actionFunc = EnTrt_SetupEndInteraction;
                break;

            case 0x88F:
                this->actionFunc = EnTrt_StartRedPotionConversation;
                break;

            default:
                break;
        }
    }
}

// Below is for Kotake in Woods of Mystery:

struct EnTrt2;

typedef void (*EnTrt2ActionFunc)(struct EnTrt2*, PlayState*);
typedef void (*EnTrt2UnkFunc)(struct EnTrt2*);
typedef void (*EnTrt2UnkFunc2)(struct EnTrt2*, PlayState*);

#define ENTRT2_GET_PATH_INDEX(thisx) (((thisx)->params & 0xFC00) >> 0xA)

#define ENTRT2_PATH_INDEX_NONE 0x3F

typedef struct EnTrt2 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnTrt2ActionFunc actionFunc;
    /* 0x148 */ UNK_TYPE1 unk_148[8];
    /* 0x150 */ SkelAnime skelAnime;
    /* 0x194 */ ColliderCylinder collider;
    /* 0x1E0 */ Path* path;
    /* 0x1E4 */ s32 unk_1E4;
    /* 0x1E8 */ UNK_TYPE1 unk_1E8[0x154];
    /* 0x33C */ s16 unk_33C[27];
    /* 0x372 */ s16 unk_372[27];
    /* 0x3A8 */ u16 unk_3A8;
    /* 0x3AA */ UNK_TYPE1 unk_3AA[4];
    /* 0x3AE */ s16 unk_3AE;
    /* 0x3B0 */ s16 unk_3B0;
    /* 0x3B2 */ s16 unk_3B2;
    /* 0x3B4 */ s16 unk_3B4;
    /* 0x3B6 */ s16 unk_3B6;
    /* 0x3B8 */ s16 unk_3B8;
    /* 0x3BC */ EnTrt2UnkFunc unk_3BC;
    /* 0x3C0 */ s16 unk_3C0;
    /* 0x3C2 */ Vec3s unk_3C2;
    /* 0x3C8 */ Vec3f unk_3C8;
    /* 0x3D4 */ s16 unk_3D4;
    /* 0x3D6 */ s16 unk_3D6;
    /* 0x3D8 */ u8 unk_3D8;
    /* 0x3D9 */ u8 unk_3D9;
    /* 0x3DA */ s16 csId;
} EnTrt2; // size = 0x3DC

#undef FLAGS
#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY)

#undef THIS
#define THIS ((EnTrt2*)thisx)

RECOMP_PATCH void func_80AD3EF0(EnTrt2* this, PlayState* play) {
    u8 talkState = Message_GetState(&play->msgCtx);

    if (talkState == TEXT_STATE_DONE) {
        if (Message_ShouldAdvance(play)) {
            if ((Inventory_HasEmptyBottle() && !CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_RED_POTION_FOR_KOUME)) ||
                //~ !CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE)) {
                !rando_location_is_checked(GI_POTION_RED_BOTTLE)) {
                this->unk_3B2 = 12;
            } else {
                SET_WEEKEVENTREG(WEEKEVENTREG_FAILED_RECEIVED_RED_POTION_FOR_KOUME_WOODS);
                this->unk_3A8 = 0x88E;
                Message_StartTextbox(play, this->unk_3A8, &this->actor);
                this->unk_3B2 = 10;
            }
        }
    } else if ((talkState == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        play->msgCtx.msgMode = MSGMODE_TEXT_CLOSING;
        play->msgCtx.stateTimer = 4;
        this->unk_3B2 = 12;
    }
}

RECOMP_PATCH void func_80AD3FF4(EnTrt2* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE)) {
            SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE);
        }
        //~ SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_RED_POTION_FOR_KOUME);
        this->actor.parent = NULL;
        //~ this->unk_3B2 = 14;
        this->unk_3B2 = 15;
    //~ } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_KOTAKE_BOTTLE)) {
    } else if (rando_location_is_checked(GI_POTION_RED_BOTTLE)) {
        Actor_OfferGetItem(&this->actor, play, GI_POTION_RED, 300.0f, 300.0f);
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_POTION_RED_BOTTLE, 300.0f, 300.0f);
    }
}

RECOMP_PATCH void func_80AD3664(EnTrt2* this, PlayState* play) {
    if (CutsceneManager_IsNext(this->csId)) {
        //~ CutsceneManager_StartWithPlayerCs(this->csId, &this->actor);
        if (this->unk_3D9 == 0) {
            this->unk_3B2 = 1;
        } else {
            this->unk_3B2 = 2;
        }
    } else {
        //~ CutsceneManager_Queue(this->csId);
        return;
    }
    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_KOTAKE_FLY - SFX_FLAG);
}

RECOMP_PATCH void func_80AD4298(EnTrt2* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (CutsceneManager_IsNext(this->csId)) {
        CutsceneManager_StartWithPlayerCs(this->csId, &this->actor);
        player->stateFlags1 |= PLAYER_STATE1_20;
        this->unk_3B2 = 6;
    } else {
        if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
            CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
        }
        CutsceneManager_Queue(this->csId);
    }
}

f32 func_80AD49B8(Path* path, s32 arg1, Vec3f* arg2, Vec3s* arg3);

RECOMP_PATCH void func_80AD36EC(EnTrt2* this, PlayState* play) {
    Vec3s sp30;
    f32 sp2C;

    if (this->path != NULL) {
        sp2C = func_80AD49B8(this->path, this->unk_1E4, &this->actor.world.pos, &sp30);
        this->actor.shape.rot.y += 0x1000;
        Math_ApproachF(&this->actor.velocity.y, 0.5f, 0.2f, 1.0f);
        if (sp2C < 5.0f) {
            if (this->unk_1E4 >= (this->path->count - 1)) {
                this->unk_1E4 = 0;
                this->unk_3D9 = 1;
                this->actor.velocity.y = 0.0f;
                this->path = SubS_GetPathByIndex(play, this->path->additionalPathIndex, PATH_INDEX_NONE);
                CutsceneManager_Stop(this->csId);
                this->csId = CutsceneManager_GetAdditionalCsId(this->csId);
                CutsceneManager_Queue(this->csId);
                this->unk_3B2 = 0;
            } else {
                this->unk_1E4++;
            }
        }
    }
    Actor_MoveWithGravity(&this->actor);
    Actor_PlaySfx_Flagged(&this->actor, NA_SE_EN_KOTAKE_FLY - SFX_FLAG);
    if ((this->actor.shape.rot.y >= 0x2800) && (this->actor.shape.rot.y < 0x3800)) {
        Actor_PlaySfx(&this->actor, NA_SE_EN_KOTAKE_ROLL);
    }
}

s32 func_80AD4B08(PlayState* play);

RECOMP_PATCH void func_80AD381C(EnTrt2* this, PlayState* play) {
    if ((CURRENT_DAY == 2) || (CURRENT_DAY == 3)) {
        if (func_80AD4B08(play) == 1) {
            this->actor.world.pos.y -= 50.0f;
            this->unk_3D9 = 0;
            this->unk_3B2 = 0;
            this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            this->actor.flags |= ACTOR_FLAG_UPDATE_CULLING_DISABLED;
        }
    } else {
        Actor_Kill(&this->actor);
    }
}