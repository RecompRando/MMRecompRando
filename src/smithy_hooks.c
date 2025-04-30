#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct ObjIcePoly;

typedef void (*ObjIcePolyActionFunc)(struct ObjIcePoly*, PlayState*);

typedef struct ObjIcePoly {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ObjIcePolyActionFunc actionFunc;
    /* 0x148 */ u8 unk_148;
    /* 0x149 */ u8 switchFlag;
    /* 0x14A */ s16 unk_14A;
    /* 0x14C */ ColliderCylinder colliders1[2];
    /* 0x1E4 */ ColliderCylinder colliders2[2];
} ObjIcePoly; // size = 0x27C

#define OBJECT_KBT_LIMB_MAX 0x14
#define OBJECT_KGY_LIMB_MAX 0x17

struct EnKbt;

typedef void (*EnKbtActionFunc)(struct EnKbt*, PlayState*);

typedef struct EnKbt {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[OBJECT_KBT_LIMB_MAX];
    /* 0x200 */ Vec3s morphTable[OBJECT_KBT_LIMB_MAX];
    /* 0x278 */ Actor* unk_278;
    /* 0x27C */ u16 unk_27C;
    /* 0x27E */ u8 unk_27E;
    /* 0x27F */ u8 unk_27F;
    /* 0x280 */ s16 animIndex;
    /* 0x282 */ s16 unk_282;
    /* 0x284 */ s16 unk_284;
    /* 0x288 */ EnKbtActionFunc actionFunc;
} EnKbt; // size = 0x28C

struct EnKgy;

typedef void (*EnKgyActionFunc)(struct EnKgy*, PlayState*);

#define ENKGY_GET_1F(thisx) ((thisx)->params & 0x1F)
#define ENKGY_GET_SWITCH_FLAG(thisx) (((thisx)->params & 0xFE00) >> 9)

typedef struct EnKgy {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[OBJECT_KGY_LIMB_MAX];
    /* 0x212 */ Vec3s morphTable[OBJECT_KGY_LIMB_MAX];
    /* 0x29C */ u16 unk_29C;
    /* 0x2A0 */ EnKbt* zubora;
    /* 0x2A4 */ ObjIcePoly* iceBlock;
    /* 0x2A8 */ Vec3f unk_2A8;
    /* 0x2B4 */ Vec3f unk_2B4;
    /* 0x2C0 */ Vec3f unk_2C0;
    /* 0x2CC */ Vec3s unk_2CC;
    /* 0x2D2 */ s16 animIndex;
    /* 0x2D4 */ s16 csIdList[6];
    /* 0x2E0 */ s16 csIdIndex;
    /* 0x2E2 */ s16 animIndex2;
    /* 0x2E4 */ s16 unk_2E4;
    /* 0x2E6 */ s16 unk_2E6;
    /* 0x2E8 */ s16 unk_2E8;
    /* 0x2EA */ s16 getItemId;
    /* 0x2EC */ LightInfo lightInfo;
    /* 0x2FC */ LightNode* lightNode;
    /* 0x300 */ s16 unk_300;
    /* 0x304 */ EnKgyActionFunc actionFunc;
} EnKgy; // size = 0x308

typedef enum EnKgyAnimation {
    /* -1 */ ENKGY_ANIM_NONE = -1,
    /*  0 */ ENKGY_ANIM_0,
    /*  1 */ ENKGY_ANIM_1,
    /*  2 */ ENKGY_ANIM_2,
    /*  3 */ ENKGY_ANIM_3,
    /*  4 */ ENKGY_ANIM_4,
    /*  5 */ ENKGY_ANIM_5,
    /*  6 */ ENKGY_ANIM_6,
    /*  7 */ ENKGY_ANIM_7,
    /*  8 */ ENKGY_ANIM_8,
    /*  9 */ ENKGY_ANIM_9,
    /* 10 */ ENKGY_ANIM_MAX
} EnKgyAnimation;

RECOMP_PATCH s32 func_80B41460(void) {
    // if ((GET_PLAYER_FORM != PLAYER_FORM_HUMAN) ||
    //     ((CUR_FORM_EQUIP(EQUIP_SLOT_B) != ITEM_SWORD_KOKIRI) && (CUR_FORM_EQUIP(EQUIP_SLOT_B) != ITEM_SWORD_RAZOR) &&
    //      (CUR_FORM_EQUIP(EQUIP_SLOT_B) != ITEM_SWORD_GILDED))) {
    if (GET_PLAYER_FORM != PLAYER_FORM_HUMAN) {
        return 0xC38;
    }

    if (CURRENT_DAY == 3) {
        return 0xC39;
    }

    return 0xC3A;
}

RECOMP_PATCH s32 func_80B41528(PlayState* play) {
    // if (CUR_FORM_EQUIP(EQUIP_SLOT_B) == ITEM_SWORD_GILDED) {
    if (rando_location_is_checked(GI_SWORD_GILDED) && rando_location_is_checked(GI_SWORD_RAZOR)) {
        return 0xC4C;
    }

    // if (CUR_FORM_EQUIP(EQUIP_SLOT_B) == ITEM_SWORD_RAZOR) {
    if (rando_location_is_checked(GI_SWORD_RAZOR)) {
        return 0xC45;
    }

    return 0xC3B;
}

void func_80B425A0(EnKgy *this, PlayState *play);
void EnKgy_ChangeAnimLoopMorph(EnKgy *this, s16 animIndex);
void func_80B40C74(PlayState *play);
void func_80B40D30(PlayState *play);
s32 func_80B40D8C(PlayState *play);
void func_80B40E18(EnKgy *this, u16 textId);
void func_80B40E38(EnKgy *this);
s32 func_80B40E54(EnKgy *this);
void func_80B40E74(EnKgy *this, PlayState *play, u16 textId);
void func_80B40EBC(EnKgy *this, PlayState *play, u16 arg2);
void func_80B40EE8(EnKgy *this, PlayState *play);
void func_80B41368(EnKgy *this, PlayState *play, s32 arg2);
void func_80B413C8(EnKgy *this);
s32 func_80B41460(void);
s32 func_80B41528(PlayState *play);
void func_80B41ACC(EnKgy *this, PlayState *play);
void func_80B41C54(EnKgy *this, PlayState *play);
void func_80B41D64(EnKgy *this, PlayState *play);

RECOMP_PATCH void func_80B41E18(EnKgy* this, PlayState* play) {
    u16 textId;
    s32 pad;
    Player* player = GET_PLAYER(play);

    if (SkelAnime_Update(&this->skelAnime) && (this->animIndex == ENKGY_ANIM_3)) {
        EnKgy_ChangeAnimLoopMorph(this, ENKGY_ANIM_4);
    }

    if ((this->unk_2E4 <= 0) && !(this->unk_29C & 2) && (func_80B40E54(this) == 0)) {
        switch (Message_GetState(&play->msgCtx)) {
            case TEXT_STATE_CHOICE:
                if (Message_ShouldAdvance(play)) {
                    textId = play->msgCtx.currentTextId;

                    switch (textId) {
                        case 0xC3B:
                            switch (play->msgCtx.choiceIndex) {
                                case 0:
                                    if (gSaveContext.save.saveInfo.playerData.rupees < play->msgCtx.unk1206C) {
                                        Audio_PlaySfx(NA_SE_SY_ERROR);
                                        func_80B40E74(this, play, 0xC3F);
                                    } else {
                                        Audio_PlaySfx_MessageDecide();
                                        func_80B40E74(this, play, 0xC42);
                                        Rupees_ChangeBy(-play->msgCtx.unk1206C);
                                    }
                                    break;

                                case 1:
                                    Audio_PlaySfx_MessageCancel();
                                    func_80B40EBC(this, play, textId);
                                    break;

                                default:
                                    break;
                            }
                            break;

                        case 0xC3E:
                            switch (play->msgCtx.choiceIndex) {
                                case 0:
                                    Audio_PlaySfx_MessageDecide();
                                    func_80B40E74(this, play, func_80B41460());
                                    break;

                                case 1:
                                    Audio_PlaySfx_MessageCancel();
                                    func_80B40E74(this, play, 0xC3C);
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
                    textId = play->msgCtx.currentTextId;

                    switch (textId) {
                        case 0xC35:
                            play->msgCtx.msgLength = 0;
                            this->actor.textId = textId;
                            this->unk_29C |= 4;
                            this->unk_2E8 = 3;
                            func_80B40E38(this);
                            break;

                        case 0xC36:
                            play->msgCtx.msgLength = 0;
                            func_80B41368(this, play, 4);
                            this->actor.textId = func_80B41460();
                            break;

                        case 0xC38:
                        case 0xC39:
                        case 0xC52:
                        case 0xC54:
                            Message_CloseTextbox(play);
                            this->actionFunc = func_80B425A0;
                            func_80B413C8(this);
                            func_80B40E18(this, 5);
                            EnKgy_ChangeAnimLoopMorph(this, ENKGY_ANIM_4);
                            this->actor.textId = 0xC35;
                            break;

                        case 0xC3A:
                            func_80B40E74(this, play, func_80B41528(play));
                            EnKgy_ChangeAnimLoopMorph(this, ENKGY_ANIM_4);
                            break;

                        case 0xC3C:
                        case 0xC3F:
                        case 0xC4C:
                            func_80B40EBC(this, play, textId);
                            break;

                        case 0xC3D:
                            Message_CloseTextbox(play);
                            this->actionFunc = func_80B41C54;
                            this->actor.textId = 0xC3E;
                            func_80B413C8(this);
                            func_80B40E18(this, 2);
                            break;

                        case 0xC40:
                            play->msgCtx.msgLength = 0;
                            func_80B41368(this, play, 0);
                            this->actor.textId = 0xC43;
                            break;

                        case 0xC42:
                            play->msgCtx.msgLength = 0;
                            func_80B41368(this, play, 0);
                            this->actor.textId = 0xC43;
                            // CUR_FORM_EQUIP(EQUIP_SLOT_B) = ITEM_NONE;
                            // SET_EQUIP_VALUE(EQUIP_TYPE_SWORD, EQUIP_VALUE_SWORD_NONE);
                            // Interface_LoadItemIconImpl(play, EQUIP_SLOT_B);
                            func_80B40C74(play);
                            break;

                        case 0xC4D:
                        case 0xC58:
                            this->unk_29C |= 0x10;

                        case 0xC45:
                            play->msgCtx.msgLength = 0;
                            func_80B41368(this, play, 3);
                            this->actor.textId = 0xFF;
                            this->actionFunc = func_80B41ACC;
                            break;

                        case 0xC57:
                            this->unk_29C &= ~0x8;
                            Message_CloseTextbox(play);
                            this->actionFunc = func_80B41C54;
                            this->actor.textId = 0xC58;
                            func_80B413C8(this);
                            func_80B40E18(this, 3);
                            break;

                        case 0xC46:
                        case 0xC55:
                            Player_UpdateBottleHeld(play, GET_PLAYER(play), ITEM_BOTTLE, PLAYER_IA_BOTTLE_EMPTY);
                            player->exchangeItemAction = PLAYER_IA_NONE;
                            this->unk_29C &= ~0x8;
                            play->msgCtx.msgLength = 0;
                            func_80B41368(this, play, 4);
                            if (this->unk_29C & 0x10) {
                                this->actor.textId = 0xC56;
                                Rupees_ChangeBy(play->msgCtx.unk1206C);
                            } else {
                                this->actor.textId = 0xC42;
                            }
                            break;

                        case 0xC47:
                            EnKgy_ChangeAnimLoopMorph(this, ENKGY_ANIM_1);
                            if (this->unk_29C & 8) {
                                player->exchangeItemAction = PLAYER_IA_NONE;
                                this->unk_29C &= ~8;
                            }
                            func_80B40EBC(this, play, textId);
                            break;

                        case 0xC48:
                            play->msgCtx.msgLength = 0;
                            func_80B41368(this, play, 0);
                            this->actor.textId = textId + 1;
                            break;

                        case 0xC49:
                            play->msgCtx.msgLength = 0;
                            func_80B41368(this, play, 4);
                            this->actor.textId = textId + 1;
                            break;

                        case 0xC4A:
                        case 0xC4B:
                            EnKgy_ChangeAnimLoopMorph(this, ENKGY_ANIM_4);
                            Message_CloseTextbox(play);
                            this->actionFunc = func_80B41C54;
                            this->actor.textId = 0xC4B;
                            func_80B413C8(this);
                            func_80B40E18(this, 4);
                            break;

                        case 0xC50:
                            if (func_80B40D8C(play)) {
                                this->getItemId = GI_SWORD_GILDED;
                                this->actor.textId = 0xC53;
                            } else {
                                this->getItemId = GI_SWORD_RAZOR;
                                this->actor.textId = 0xC51;
                            }
                            Message_CloseTextbox(play);
                            this->actionFunc = func_80B41D64;
                            func_80B413C8(this);
                            Actor_OfferGetItem(&this->actor, play, this->getItemId, 2000.0f, 1000.0f);
                            break;

                        case 0xC51:
                        case 0xC53:
                            play->msgCtx.msgLength = 0;
                            func_80B41368(this, play, 0);
                            this->actor.textId = textId + 1;
                            func_80B40D30(play);
                            break;

                        case 0xC56:
                            Message_CloseTextbox(play);
                            this->actionFunc = func_80B41C54;
                            this->actor.textId = 0xC56;
                            func_80B413C8(this);
                            func_80B40E18(this, 8);
                            break;

                        default:
                            break;
                    }
                }
                break;

            default:
                break;
        }
    }

    func_80B40EE8(this, play);
}