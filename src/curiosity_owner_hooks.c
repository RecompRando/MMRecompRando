#include "modding.h"
#include "global.h"
#include "overlays/actors/ovl_En_GirlA/z_en_girla.h"

#include "apcommon.h"

#define FSN_LIMB_MAX 0x12

#define ENFSN_IS_BACKROOM(thisx) ((thisx)->params & 1)
#define ENFSN_IS_SHOP(thisx) (!((thisx)->params & 1))

#define ENFSN_OFFER_FINAL_PRICE (1 << 6)
#define ENFSN_HAGGLE (1 << 7)
#define ENFSN_ANGRY (1 << 8)
#define ENFSN_CALM_DOWN (1 << 9)

#define ENFSN_LIMB_MAX FSN_LIMB_MAX + 1 // Note: adding 1 to FSN_LIMB_MAX due to bug in the skeleton, see bug in object_fsn.xml

#define LOCATION_CURIOSITY_OWNER_1 GI_MASK_KEATON

struct EnFsn;

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

#define THIS ((EnFsn*)thisx)

#define SI_NONE 0

#define ENFSN_END_CONVERSATION (1 << 0)
#define ENFSN_GIVE_ITEM (1 << 1)
#define ENFSN_GAVE_KEATONS_MASK (1 << 2)
#define ENFSN_GAVE_LETTER_TO_MAMA (1 << 3)

typedef void (*EnFsnActionFunc)(struct EnFsn*, PlayState*);

typedef struct EnFsn {
    /* 0x000 */ Actor actor;
    /* 0x144 */ UNK_TYPE1 pad144[0x4C];
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnFsnActionFunc actionFunc;
    /* 0x1D8 */ EnFsnActionFunc prevActionFunc; // Used to return to correct browsing function
    /* 0x1DC */ ColliderCylinder collider;
    /* 0x228 */ s16 fidgetTableY[ENFSN_LIMB_MAX];
    /* 0x24E */ s16 fidgetTableZ[ENFSN_LIMB_MAX];
    /* 0x274 */ Vec3s headRot;
    /* 0x27A */ Vec3s torsoRot; // Set but never used
    /* 0x280 */ Vec3s jointTable[ENFSN_LIMB_MAX];
    /* 0x2F2 */ Vec3s morphTable[ENFSN_LIMB_MAX];
    /* 0x364 */ s16 eyeTexIndex;
    /* 0x366 */ s16 blinkTimer;
    /* 0x368 */ s16 cutsceneState;
    /* 0x36A */ s16 csId;
    /* 0x36C */ s16 lookToShopkeeperCsId;
    /* 0x36E */ s16 lookToShelfCsId;
    /* 0x370 */ s16 lookToShopkeeperFromShelfCsId;
    /* 0x372 */ s16 lookToShopkeeperBuyingCsId;
    /* 0x374 */ s16 price;
    /* 0x376 */ u16 textId;
    /* 0x378 */ u8 isSelling;
    /* 0x379 */ u8 cursorIndex;
    /* 0x37C */ s32 getItemId;
    /* 0x380 */ s16 stolenItem1;
    /* 0x382 */ s16 stolenItem2;
    /* 0x384 */ s16 itemIds[3];
    /* 0x38A */ s16 totalSellingItems;
    /* 0x38C */ s16 numSellingItems;
    /* 0x390 */ EnGirlA* items[3];
    /* 0x39C */ s16 delayTimer;
    /* 0x3A0 */ s32 stickAccumX;
    /* 0x3A4 */ s32 stickAccumY;
    /* 0x3A8 */ Vec3f cursorPos;
    /* 0x3B4 */ Color_RGBAu32 cursorColor;
    /* 0x3C4 */ f32 cursorAnimTween;
    /* 0x3C8 */ u8 cursorAnimState;
    /* 0x3C9 */ u8 drawCursor;
    /* 0x3CC */ StickDirectionPrompt stickLeftPrompt;
    /* 0x404 */ StickDirectionPrompt stickRightPrompt;
    /* 0x43C */ f32 arrowAnimTween;
    /* 0x440 */ f32 stickAnimTween;
    /* 0x444 */ u8 arrowAnimState;
    /* 0x445 */ u8 stickAnimState;
    /* 0x448 */ f32 shopItemSelectedTween;
    /* 0x44C */ s16 animIndex;
    /* 0x44E */ u16 flags;
} EnFsn; // size = 0x450

RECOMP_PATCH void EnFsn_HandleConversationBackroom(EnFsn* this, PlayState* play) {
    switch (this->textId) {
        case 0:
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_PRIORITY_MAIL)) {
                this->textId = 0x29E0;
                break;
            } else {
                this->textId = 0x29E4;
                this->flags |= ENFSN_END_CONVERSATION;
            }
            break;

        case 0x29E0:
            //if (INV_CONTENT(ITEM_MASK_KEATON) == ITEM_MASK_KEATON) {
            if (rando_location_is_checked(LOCATION_CURIOSITY_OWNER_1)) {
                this->flags |= ENFSN_GIVE_ITEM;
                this->flags |= ENFSN_GAVE_LETTER_TO_MAMA;
                this->getItemId = GI_LETTER_TO_MAMA;
                SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_PRIORITY_MAIL);
                this->textId = 0x29F1;
                break;
            } else {
                this->textId = 0x29E1;
            }
            break;

        case 0x29E1:
            this->textId = 0x29E2;
            break;

        case 0x29E2:
            this->flags |= ENFSN_GIVE_ITEM;
            this->flags |= ENFSN_GAVE_KEATONS_MASK;
            this->getItemId = GI_MASK_KEATON;
            this->textId = 0x29E3;
            break;

        case 0x29E3:
            this->flags |= ENFSN_GIVE_ITEM;
            this->flags |= ENFSN_GAVE_LETTER_TO_MAMA;
            this->getItemId = GI_LETTER_TO_MAMA;
            SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_PRIORITY_MAIL);
            this->textId = 0x29F1;
            break;

        case 0x29F1:
            this->textId = 0x29E4;
            this->flags |= ENFSN_END_CONVERSATION;
            break;

        default:
            break;
    }
    Message_StartTextbox(play, this->textId, &this->actor);
    if (this->flags & ENFSN_END_CONVERSATION) {
        if (this->flags & ENFSN_GAVE_LETTER_TO_MAMA) {
            this->flags &= ~ENFSN_GAVE_LETTER_TO_MAMA;
            Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_PRIORITY_MAIL);
        }
        if (this->flags & ENFSN_GAVE_KEATONS_MASK) {
            this->flags &= ~ENFSN_GAVE_KEATONS_MASK;
            Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_KEATON_MASK);
        }
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_CURIOSITY_SHOP_MAN);
    }
}