#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnMs;

typedef void (*EnMsActionFunc)(struct EnMs*, PlayState*);

typedef struct EnMs {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[9];
    /* 0x1BE */ Vec3s morphTable[9];
    /* 0x1F4 */ EnMsActionFunc actionFunc;
    /* 0x1F8 */ ColliderCylinder collider;
} EnMs; // size = 0x244

void EnMs_Talk(EnMs* this, PlayState* play);

RECOMP_PATCH void EnMs_Wait(EnMs* this, PlayState* play) {
    s16 yawDiff = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    if (rando_location_is_checked(LOCATION_BEAN_DADDY)) {
        this->actor.textId = 0x932; // "[...] So you liked my Magic Beans [...]"
    } else {
        this->actor.textId = 0x92E; // "[...] You're the first customer [...]"
    }

    if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        this->actionFunc = EnMs_Talk;
    } else if ((this->actor.xzDistToPlayer < 90.0f) && (ABS_ALT(yawDiff) < 0x2000)) {
        Actor_OfferTalk(&this->actor, play, 90.0f);
    }
}

void EnMs_Sell(EnMs* this, PlayState* play);

RECOMP_PATCH void EnMs_Talk(EnMs* this, PlayState* play) {
    switch (Message_GetState(&play->msgCtx)) {
        case TEXT_STATE_DONE:
            if (Message_ShouldAdvance(play)) {
                this->actionFunc = EnMs_Wait;
            }
            break;

        case TEXT_STATE_EVENT:
            if (Message_ShouldAdvance(play)) {
                Message_CloseTextbox(play);
                Actor_OfferGetItem(&this->actor, play, GI_MAGIC_BEANS, this->actor.xzDistToPlayer,
                                   this->actor.playerHeightRel);
                this->actionFunc = EnMs_Sell;
            }
            break;

        case TEXT_STATE_CHOICE:
            if (Message_ShouldAdvance(play)) {
                switch (play->msgCtx.choiceIndex) {
                    case 0: // yes
                        Message_CloseTextbox(play);
                        if (gSaveContext.save.saveInfo.playerData.rupees < 10) {
                            Audio_PlaySfx(NA_SE_SY_ERROR);
                            Message_ContinueTextbox(play, 0x935); // "[...] You don't have enough Rupees."
                        } else if (AMMO(ITEM_MAGIC_BEANS) >= 20) {
                            Audio_PlaySfx(NA_SE_SY_ERROR);
                            Message_ContinueTextbox(play, 0x937); // "[...] You can't carry anymore."
                        } else {
                            Audio_PlaySfx_MessageDecide();
                            Actor_OfferGetItemHook(&this->actor, play, GI_MAGIC_BEANS, 0, 90.0f, 10.0f, false, false);
                            Rupees_ChangeBy(-10);
                            this->actionFunc = EnMs_Sell;
                        }
                        break;

                    case 1: // no
                    default:
                        Audio_PlaySfx_MessageCancel();
                        Message_ContinueTextbox(play, 0x934); // "[...] Well, if your mood changes [...]"
                        break;
                }
            }
            break;

        default:
            break;
    }
}