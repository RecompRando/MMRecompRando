#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define LOCATION_BOMBSHOP_KEG ((this->actor.id) << 8 | GI_POWDER_KEG)

struct EnSGoro;

typedef void (*EnSGoroActionFunc)(struct EnSGoro*, PlayState*);

#define GORON_LIMB_MAX 0x12

typedef struct EnSGoro {
    /* 0x000 */ Actor actor;
    /* 0x144 */ Actor* otherGoron;
    /* 0x148 */ SkelAnime skelAnime;
    /* 0x18C */ EnSGoroActionFunc actionFunc;
    /* 0x190 */ ColliderCylinder collider;
    /* 0x1DC */ u16 actionFlags;
    /* 0x1DE */ u16 bombbuyFlags;
    /* 0x1E0 */ UNK_TYPE1 unk_1E0[0xC];
    /* 0x1EC */ s16 eyeTexIndex;
    /* 0x1EE */ s16 eyeTimer;
    /* 0x1F0 */ s16 loseAttentionTimer;
    /* 0x1F4 */ s32 objectSlot;
    /* 0x1F8 */ Vec3s jointTable[GORON_LIMB_MAX];
    /* 0x264 */ Vec3s morphTable[GORON_LIMB_MAX];
    /* 0x2D0 */ f32 scaleFactor;
    /* 0x2D4 */ s16 headRotZ;
    /* 0x2D6 */ s16 headRotY;
    /* 0x2D8 */ s16 bodyRotZ;
    /* 0x2DA */ s16 bodyRotY;
    /* 0x2DC */ Vec3f headTranslate;
    /* 0x2E8 */ Vec3s headRotate;
    /* 0x2F0 */ Vec3f bodyTranslate;
    /* 0x2FC */ Vec3s bodyRotate;
    /* 0x302 */ UNK_TYPE1 unk_302[2];
    /* 0x304 */ u16 textId;
    /* 0x306 */ s16 snorePhase;
    /* 0x308 */ s32 animInfoIndex;
    /* 0x30C */ s16 powderKegPrice;
} EnSGoro; // size = 0x310

#define EN_S_GORO_ACTIONFLAG_ROLLEDUP (1 << 0)
#define EN_S_GORO_ACTIONFLAG_FACEPLAYER (1 << 1)
#define EN_S_GORO_ACTIONFLAG_EYESOPEN (1 << 2)
#define EN_S_GORO_ACTIONFLAG_EARSCOVERED (1 << 3)
#define EN_S_GORO_ACTIONFLAG_ENGAGED (1 << 4)
#define EN_S_GORO_ACTIONFLAG_LASTMESSAGE (1 << 5)
#define EN_S_GORO_ACTIONFLAG_GKQUIET_ACKNOWLEDGED (1 << 6)
#define EN_S_GORO_ACTIONFLAG_SNOREPHASE (1 << 7)
#define EN_S_GORO_ACTIONFLAG_UNK0100 (1 << 8)
#define EN_S_GORO_ACTIONFLAG_HANDTAP (1 << 9)
#define EN_S_GORO_ACTIONFLAG_TIRED (1 << 10)
#define EN_S_GORO_ACTIONFLAG_SUPPRESS_SNORE (1 << 11)

#define EN_S_GORO_BOMBBUYFLAG_TALKED_HUMAN (1 << 0)
#define EN_S_GORO_BOMBBUYFLAG_TALKED_GORONPK (1 << 1)
#define EN_S_GORO_BOMBBUYFLAG_TALKED_DEKU (1 << 2)
#define EN_S_GORO_BOMBBUYFLAG_TALKED_ZORA (1 << 3)
#define EN_S_GORO_BOMBBUYFLAG_TALKED_HUMAN_FINALNIGHT (1 << 4)
#define EN_S_GORO_BOMBBUYFLAG_TALKED_GOROKPK_FINALNIGHT (1 << 5)
#define EN_S_GORO_BOMBBUYFLAG_TALKED_DEKU_FINALNIGHT (1 << 6)
#define EN_S_GORO_BOMBBUYFLAG_TALKED_ZORA_FINALNIGHT (1 << 7)
#define EN_S_GORO_BOMBBUYFLAG_TALKED_GORON (1 << 8)
#define EN_S_GORO_BOMBBUYFLAG_TALKED_GORON_FINALNIGHT (1 << 9)
#define EN_S_GORO_BOMBBUYFLAG_YESBUY (1 << 10)

void EnSGoro_ShopGoron_Idle(EnSGoro* this, PlayState* play);
void EnSGoro_ShopGoron_FinishTransaction(EnSGoro* this, PlayState* play);
void EnSGoro_UpdateToHandtapAnimation(EnSGoro* this);
void EnSGoro_UpdateSleepyAnimation(EnSGoro* this);
void EnSGoro_UpdateToIdleAnimation(EnSGoro* this);

/**
 * Dialogue tree for bomb shop goron. Returns ID of next message to display.
 */
RECOMP_PATCH u16 EnSGoro_BombshopGoron_NextTextId(EnSGoro* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    switch (this->textId) {
        case 0x0:
            switch (player->transformation) {
                case PLAYER_FORM_GORON:
                    // Check if Powder Keg is in Powder Key slot.
                    if (INV_CONTENT(ITEM_POWDER_KEG) == ITEM_POWDER_KEG) {
                        if ((gSaveContext.save.day == 3) && gSaveContext.save.isNight) {
                            if (!(this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_TALKED_GOROKPK_FINALNIGHT)) {
                                this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_TALKED_GOROKPK_FINALNIGHT;
                                this->actionFlags |= EN_S_GORO_ACTIONFLAG_HANDTAP;
                                return 0x677;
                            }
                            return 0x67A;
                        }
                        if (!(this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_TALKED_GORONPK)) {
                            this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_TALKED_GORONPK;
                            this->actionFlags |= EN_S_GORO_ACTIONFLAG_HANDTAP;
                            return 0x66E;
                        }
                        return 0x679;
                    }

                    // No powder keg
                    if ((gSaveContext.save.day == 3) && gSaveContext.save.isNight) {
                        if (!(this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_TALKED_GORON_FINALNIGHT)) {
                            this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_TALKED_GORON_FINALNIGHT;
                            this->actionFlags |= EN_S_GORO_ACTIONFLAG_HANDTAP;
                            return 0x67E;
                        }
                        return 0x683;
                    }
                    if (!(this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_TALKED_GORON)) {
                        this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_TALKED_GORON;
                        this->actionFlags |= EN_S_GORO_ACTIONFLAG_HANDTAP;
                        return 0x67B;
                    }
                    return 0x681;

                case PLAYER_FORM_ZORA:
                    if ((gSaveContext.save.day == 3) && gSaveContext.save.isNight) {
                        if (!(this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_TALKED_ZORA_FINALNIGHT)) {
                            this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_TALKED_ZORA_FINALNIGHT;
                            return 0x668;
                        }
                        this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                        this->actionFlags |= EN_S_GORO_ACTIONFLAG_TIRED;
                        return 0x66D;
                    }
                    if (!(this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_TALKED_ZORA)) {
                        this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_TALKED_ZORA;
                        return 0x664;
                    }
                    this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                    this->actionFlags |= EN_S_GORO_ACTIONFLAG_TIRED;
                    return 0x66C;

                case PLAYER_FORM_DEKU:
                    if ((gSaveContext.save.day == 3) && gSaveContext.save.isNight) {
                        if (!(this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_TALKED_DEKU_FINALNIGHT)) {
                            this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_TALKED_DEKU_FINALNIGHT;
                            return 0x668;
                        }
                        this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                        return 0x66D;
                    }
                    if (!(this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_TALKED_DEKU)) {
                        this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_TALKED_DEKU;
                        return 0x664;
                    }
                    this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                    return 0x66C;

                case PLAYER_FORM_HUMAN:
                    if ((gSaveContext.save.day == 3) && gSaveContext.save.isNight) {
                        if (!(this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_TALKED_HUMAN_FINALNIGHT)) {
                            this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_TALKED_HUMAN_FINALNIGHT;
                            return 0x668;
                        }
                        this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                        return 0x66D;
                    }
                    if (!(this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_TALKED_HUMAN)) {
                        this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_TALKED_HUMAN;
                        return 0x664;
                    }
                    this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                    return 0x66C;
            }

        case 0x664:
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_HANDTAP;
            return 0x665;

        case 0x665:
            this->actionFlags &= ~EN_S_GORO_ACTIONFLAG_HANDTAP;
            return 0x666;

        case 0x666:
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_TIRED;
            return 0x667;

        case 0x668:
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_HANDTAP;
            return 0x669;

        case 0x669:
            this->actionFlags &= ~EN_S_GORO_ACTIONFLAG_HANDTAP;
            return 0x66A;

        case 0x66A:
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_TIRED;
            return 0x66B;

        case 0x677:
            this->actionFlags &= ~EN_S_GORO_ACTIONFLAG_HANDTAP;
            return 0x678;

        case 0x678:
            return 0x670;

        case 0x66E:
            this->actionFlags &= ~EN_S_GORO_ACTIONFLAG_HANDTAP;
            return 0x66F;

        case 0x66F:
            return 0x670;

        case 0x679:
            return 0x670;

        case 0x67A:
            return 0x670;

        case 0x67E:
            this->actionFlags &= ~EN_S_GORO_ACTIONFLAG_HANDTAP;
            return 0x67F;

        case 0x67F:
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
            return 0x680;

        case 0x67B:
            this->actionFlags &= ~EN_S_GORO_ACTIONFLAG_HANDTAP;
            return 0x67C;

        case 0x67C:
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
            return 0x67D;

        case 0x681:
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
            return 0x682;

        case 0x683:
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
            return 0x684;

        case 0x670:
            if (this->bombbuyFlags & EN_S_GORO_BOMBBUYFLAG_YESBUY) {
                if (AMMO(ITEM_POWDER_KEG) != 0 && (rando_location_is_checked(LOCATION_BOMBSHOP_KEG) || !rando_shopsanity_enabled())) {
                    this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                    Audio_PlaySfx(NA_SE_SY_ERROR);
                    return 0x673;
                }
                this->powderKegPrice = play->msgCtx.unk1206C;
                if (gSaveContext.save.saveInfo.playerData.rupees < this->powderKegPrice) {
                    this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                    this->actionFlags |= EN_S_GORO_ACTIONFLAG_TIRED;
                    Audio_PlaySfx(NA_SE_SY_ERROR);
                    return 0x674;
                }
                if ((gSaveContext.save.day == 3) && gSaveContext.save.isNight) {
                    this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                    Audio_PlaySfx_MessageDecide();
                    return 0x676;
                }
                this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                Audio_PlaySfx_MessageDecide();
                return 0x675;
            }
            if ((gSaveContext.save.day == 3) && gSaveContext.save.isNight) {
                this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                return 0x672;
            }
            this->actionFlags |= EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
            return 0x671;
    }
    return 0;
}

#include "recomputils.h"

void EnSGoro_ShopGoron_TakePayment(EnSGoro* this, PlayState* play);

void EnSGoro_ShopGoron_GiveRandoItem(EnSGoro* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        Rupees_ChangeBy(-this->powderKegPrice);
        this->actionFunc = EnSGoro_ShopGoron_FinishTransaction;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_BOMBSHOP_KEG), LOCATION_BOMBSHOP_KEG, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void EnSGoro_ShopGoron_Talk(EnSGoro* this, PlayState* play) {
    u8 talkState = Message_GetState(&play->msgCtx);

    if (this->actionFlags & EN_S_GORO_ACTIONFLAG_HANDTAP) {
        EnSGoro_UpdateToHandtapAnimation(this);
    } else if (this->actionFlags & EN_S_GORO_ACTIONFLAG_TIRED) {
        EnSGoro_UpdateSleepyAnimation(this);
    } else {
        EnSGoro_UpdateToIdleAnimation(this);
    }

    if (talkState == TEXT_STATE_DONE) {
        if (Message_ShouldAdvance(play)) {
            if (this->actionFlags & EN_S_GORO_ACTIONFLAG_LASTMESSAGE) {
                this->actionFlags &= ~EN_S_GORO_ACTIONFLAG_LASTMESSAGE;
                this->actionFlags &= ~EN_S_GORO_ACTIONFLAG_ENGAGED;
                this->textId = 0;
                this->actionFunc = EnSGoro_ShopGoron_Idle;
                return;
            }
            this->textId = EnSGoro_BombshopGoron_NextTextId(this, play);
            Message_StartTextbox(play, this->textId, &this->actor);
        }
    } else if ((talkState == TEXT_STATE_CHOICE) && Message_ShouldAdvance(play)) {
        switch (play->msgCtx.choiceIndex) {
            case 0:
                this->bombbuyFlags |= EN_S_GORO_BOMBBUYFLAG_YESBUY;
                break;
            case 1:
                Audio_PlaySfx_MessageCancel();
                this->bombbuyFlags &= ~EN_S_GORO_BOMBBUYFLAG_YESBUY;
                break;
        }

        this->textId = EnSGoro_BombshopGoron_NextTextId(this, play);
        if ((this->textId == 0x675) || (this->textId == 0x676)) {
            play->msgCtx.msgMode = MSGMODE_TEXT_CLOSING;
            play->msgCtx.stateTimer = 4;
            if (rando_location_is_checked(LOCATION_BOMBSHOP_KEG) || !rando_shopsanity_enabled()) {
                this->actionFunc = EnSGoro_ShopGoron_TakePayment;
            } else {
                this->actionFunc = EnSGoro_ShopGoron_GiveRandoItem;
            }
        } else {
            Message_StartTextbox(play, this->textId, &this->actor);
        }
    }

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 5, 0x1000, 0x100);
    this->actor.world.rot.y = this->actor.shape.rot.y;
}