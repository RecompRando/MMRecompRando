#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnZot;

typedef void (*EnZotActionFunc)(struct EnZot*, PlayState*);

#define ZORA_LIMB_MAX 0x14

typedef struct EnZot {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ ColliderCylinder collider;
    /* 0x1D4 */ Vec3s jointTable[ZORA_LIMB_MAX];
    /* 0x24C */ Vec3s morphTable[ZORA_LIMB_MAX];
    /* 0x2C4 */ Vec3s headRot;
    /* 0x2CA */ Vec3s torsoRot;
    /* 0x2D0 */ Path* path;
    /* 0x2D4 */ s32 unk_2D4;
    /* 0x2D8 */ Actor* unk_2D8[5];
    /* 0x2EC */ s16 unk_2EC;
    /* 0x2EE */ s16 unk_2EE;
    /* 0x2F0 */ s16 animIndex;
    /* 0x2F2 */ u16 unk_2F2;
    /* 0x2F4 */ s16 unk_2F4;
    /* 0x2F8 */ EnZotActionFunc actionFunc;
} EnZot; // size = 0x2FC

#define LOCATION_ZORA_TORCHES (0x070000 | (this->actor.id) << 8 | GI_RUPEE_BLUE)
#define LOCATION_ZORA_JAR_GAME (0x070000 | (this->actor.id) << 8 | GI_RUPEE_SILVER)

void func_80B97708(EnZot *this, PlayState *play);
void func_80B965D0(EnZot *this, PlayState *play);
void func_80B96D4C(EnZot *this);

// TODO: continue text to get rid of 10 rupees
void EnZot_GiveRandoItem(EnZot* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = func_80B97708;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_ZORA_JAR_GAME), LOCATION_ZORA_JAR_GAME, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void func_80B973BC(EnZot* this, PlayState* play) {
    func_80B96D4C(this);
    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 2, 0x800, 0x100);
    this->actor.world.rot.y = this->actor.shape.rot.y;
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        switch (play->msgCtx.currentTextId) {
            case 0x126E:
            case 0x1270:
            case 0x1273:
            case 0x1274:
                Message_ContinueTextbox(play, play->msgCtx.currentTextId + 1);
                break;

            case 0x1272:
                Message_ContinueTextbox(play, 0x126F);
                break;

            // case 0x126F:
            //     Rupees_ChangeBy(90);
            //     Message_ContinueTextbox(play, 0x1270);
            //     break;

            case 0x1275:
                if (gSaveContext.save.saveInfo.playerData.rupees < 10) {
                    Message_ContinueTextbox(play, 0x1277);
                } else {
                    Message_ContinueTextbox(play, 0x1278);
                    Rupees_ChangeBy(-10);
                }
                break;

            case 0x1276:
                Message_ContinueTextbox(play, 0x1275);
                break;

            case 0x126F:

            case 0x1271:
            case 0x1277:
            case 0x1278:
            case 0x1279:
                Message_CloseTextbox(play);
                func_80B965D0(this, play);
                this->actor.flags &= ~ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
                this->actor.textId = 0;
                this->actionFunc = func_80B97708;
                if ((this->actor.csId != CS_ID_NONE) && !(this->unk_2F2 & 1)) {
                    CutsceneManager_Stop(this->actor.csId);
                }
                this->unk_2F2 &= ~1;
                if (play->msgCtx.currentTextId == 0x126F) {
                    this->actionFunc = EnZot_GiveRandoItem;
                }
                break;
        }
    }

    if (this->unk_2F2 & 1) {
        if (this->actor.csId == CS_ID_NONE) {
            this->unk_2F2 &= ~1;
        } else if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
            CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
            CutsceneManager_Queue(this->actor.csId);
        } else if (CutsceneManager_IsNext(this->actor.csId)) {
            CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
            this->unk_2F2 &= ~1;
        } else {
            CutsceneManager_Queue(this->actor.csId);
        }
    }
}

void func_80B990A4(EnZot* this, PlayState* play);

RECOMP_PATCH void func_80B98F30(EnZot* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = func_80B990A4;
    } else {
        // Actor_OfferGetItem(&this->actor, play, GI_RUPEE_BLUE, 10000.0f, 50.0f);
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_ZORA_TORCHES), LOCATION_ZORA_TORCHES, 10000.0f, 50.0f, true, true);
    }
}