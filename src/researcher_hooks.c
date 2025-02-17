#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define GI_SONG_NOVA GI_AC

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

struct EnMk;

typedef void (*EnMkActionFunc)(struct EnMk*, PlayState*);

#define MARINE_RESEARCHER_LIMB_MAX 0xD

typedef struct EnMk {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ Vec3s jointTable[MARINE_RESEARCHER_LIMB_MAX];
    /* 0x222 */ Vec3s morphTable[MARINE_RESEARCHER_LIMB_MAX];
    /* 0x270 */ Vec3s unk_270;
    /* 0x276 */ s16 csIdList[2];
    /* 0x27A */ u16 unk_27A;
    /* 0x27C */ s16 unk_27C;
    /* 0x280 */ EnMkActionFunc actionFunc;
} EnMk; // size = 0x284

s32 func_80959524(PlayState *play);
void func_8095954C(EnMk *this, PlayState *play);
void func_80959774(EnMk *this, PlayState *play);
void func_80959844(EnMk *this, PlayState *play);
void func_80959A24(EnMk *this, PlayState *play);
void func_80959C94(EnMk *this, PlayState *play);
void func_80959D28(EnMk *this, PlayState *play);

void EnMk_GiveRandoItem(EnMk* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actionFunc = func_8095954C;
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_SONG_NOVA, 300.0f, 300.0f);
    }
}

// TODO: make this better
RECOMP_PATCH void func_80959E18(EnMk* this, PlayState* play) {
    s32 pad;
    s16 sp22 = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    SkelAnime_Update(&this->skelAnime);

    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_20_40)) {
        this->unk_27A &= ~1;
        this->actionFunc = func_80959774;
        this->actor.home.rot.y += 0x4E20;
        return;
    }

    if (Actor_OcarinaInteractionAccepted(&this->actor, &play->state)) {
        play->msgCtx.ocarinaMode = OCARINA_MODE_END;
        // this->actionFunc = func_80959D28;
        if (GET_PLAYER_FORM == PLAYER_FORM_ZORA) {
            this->actor.csId = this->csIdList[0];
            SET_WEEKEVENTREG(WEEKEVENTREG_20_40);
            // Item_Give(play, ITEM_SONG_NOVA);
            this->actionFunc = EnMk_GiveRandoItem;
        } else {
            this->actor.csId = this->csIdList[1];
        }
        CutsceneManager_Queue(this->actor.csId);
    } else if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        func_80959844(this, play);
        this->actionFunc = func_80959A24;
        this->unk_27A |= 1;
    } else if ((this->unk_27A & 2) && (func_80959524(play) >= 7)) {
        this->actionFunc = func_80959C94;
    } else if ((this->actor.xzDistToPlayer < 120.0f) && (ABS_ALT(sp22) <= 0x4300)) {
        this->unk_27A |= 1;
        Actor_OfferTalk(&this->actor, play, 200.0f);
        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_20_40) && CHECK_WEEKEVENTREG(WEEKEVENTREG_19_40)) {
            Actor_OfferOcarinaInteraction(&this->actor, play, 200.0f, 100.0f);
        }
    } else {
        this->unk_27A &= ~1;
    }
    func_8095954C(this, play);
}