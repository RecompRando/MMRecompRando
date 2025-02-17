#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnZos;

typedef void (*EnZosActionFunc)(struct EnZos*, PlayState*);

#define EVAN_LIMB_MAX 0x12

typedef struct EnZos {
    /* 0x000 */ Actor actor;
    /* 0x144 */ Vec3s jointTable[EVAN_LIMB_MAX];
    /* 0x1B0 */ Vec3s morphTable[EVAN_LIMB_MAX];
    /* 0x21C */ SkelAnime skelAnime;
    /* 0x260 */ ColliderCylinder collider;
    /* 0x2AC */ s16 eyeIndex;
    /* 0x2AE */ s16 blinkTimer;
    /* 0x2B0 */ UNK_TYPE1 unk2B0[6];
    /* 0x2B6 */ u16 unk_2B6;
    /* 0x2B8 */ s16 animIndex;
    /* 0x2BA */ s16 cueId;
    /* 0x2BC */ s16 unk_2BC;
    /* 0x2C0 */ EnZosActionFunc actionFunc;
} EnZos; // size = 0x2C4

#define EN_ZOS_ANIM_LEAN_ON_KEYBOARD 0x00
#define EN_ZOS_ANIM_TALK_HANDS_ON_HIPS 0x06

void EnZos_ChangeAnim(EnZos* this, s16 animIndex, u8 animMode);
void func_80BBBB84(EnZos* this, PlayState* play);
void func_80BBB8AC(EnZos* this, PlayState* play);
void func_80BBBD5C(EnZos* this, PlayState* play);
void func_80BBB0D4(EnZos* this, PlayState* play);
void func_80BBB15C(EnZos* this, PlayState* play);
s32 func_80BBAF5C(EnZos* this, PlayState* play);
void func_80BBB2C4(EnZos* this, PlayState* play);

void func_80BBB354(EnZos* this, PlayState* play) {
    s32 getItemId;

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = func_80BBB2C4;
        SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_EVAN_HEART_PIECE);
        this->actor.flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
    } else {
        // if (CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_EVAN_HEART_PIECE)) {
        if (rando_location_is_checked(LOCATION_EVAN_SONG)) {
            getItemId = GI_RUPEE_PURPLE;
        } else {
            getItemId = GI_HEART_PIECE;
        }
        Actor_OfferGetItem(&this->actor, play, getItemId, 10000.0f, 50.0f);
    }
}

RECOMP_PATCH void func_80BBBDE0(EnZos* this, PlayState* play) {
    Actor* thisx = &this->actor;
    Vec3f seqPos;

    if (this->unk_2B6 & 1) {
        Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.home.rot.y, 2, 0x1000, 0x200);
        this->actor.world.rot.y = thisx->shape.rot.y;
        if (this->actor.home.rot.y == thisx->shape.rot.y) {
            EnZos_ChangeAnim(this, EN_ZOS_ANIM_LEAN_ON_KEYBOARD, ANIMMODE_ONCE);
            this->unk_2B6 &= ~1;
        }
    }

    func_80BBB0D4(this, play);

    if (play->msgCtx.ocarinaMode == OCARINA_MODE_PLAYED_FULL_EVAN_SONG) {
        play->msgCtx.ocarinaMode = OCARINA_MODE_END;
        this->actionFunc = func_80BBB354;
        EnZos_ChangeAnim(this, EN_ZOS_ANIM_TALK_HANDS_ON_HIPS, ANIMMODE_LOOP);
        // this->actionFunc = func_80BBBB84;
        // this->actor.flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
        // Actor_OfferTalk(&this->actor, play, 120.0f);
        return;
    }

    if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        this->actionFunc = func_80BBB8AC;
        func_80BBB15C(this, play);
    } else if (Cutscene_IsCueInChannel(play, CS_CMD_ACTOR_CUE_501)) {
        this->actionFunc = func_80BBBD5C;
    } else if (func_80BBAF5C(this, play)) {
        Actor_OfferTalk(&this->actor, play, 120.0f);
    }

    if (!Actor_IsFacingPlayer(&this->actor, 0x4000) && (this->actor.xzDistToPlayer < 100.0f)) {
        SET_WEEKEVENTREG(WEEKEVENTREG_52_10);
    } else {
        CLEAR_WEEKEVENTREG(WEEKEVENTREG_52_10);
    }

    seqPos.x = this->actor.projectedPos.x;
    seqPos.y = this->actor.projectedPos.y;
    seqPos.z = this->actor.projectedPos.z;
    Audio_PlaySequenceAtPos(SEQ_PLAYER_BGM_SUB, &seqPos, NA_BGM_PIANO_PLAY, 1000.0f);
}