#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "overlays/actors/ovl_En_GirlA/z_en_girla.h"

#define LOCATION_SHOP_ITEM (0x090000 | this->items[this->cursorIndex]->itemParams)
#define LOCATION_FSN_RUPEE (0x070000 | (this->actor.id) << 8 | this->getItemId)

#define FSN_LIMB_MAX 0x12
#define ENFSN_LIMB_MAX FSN_LIMB_MAX + 1 // Note: adding 1 to FSN_LIMB_MAX due to bug in the skeleton, see bug in object_fsn.xml
#define ENFSN_IS_SHOP(thisx) (!((thisx)->params & 1))

struct EnFsn;

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

typedef enum {
    /* 0 */ ENFSN_CUTSCENESTATE_STOPPED,
    /* 1 */ ENFSN_CUTSCENESTATE_WAITING,
    /* 2 */ ENFSN_CUTSCENESTATE_PLAYING
} EnFsnCutsceneState;

#define ANI_LIMB_MAX 0x10
#define ENOSSAN_LIMB_MAX MAX((s32)FSN_LIMB_MAX + 1, (s32)ANI_LIMB_MAX) // Note: adding 1 to FSN_LIMB_MAX due to bug in the skeleton, see bug in object_fsn.xml

struct EnOssan;

typedef void (*EnOssanActionFunc)(struct EnOssan*, PlayState*);
typedef void (*EnOssanBlinkFunc)(struct EnOssan*);

typedef struct EnOssan {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnOssanActionFunc actionFunc;
    /* 0x18C */ EnOssanActionFunc prevActionFunc; // Used to restore back to correct browsing function
    /* 0x190 */ ColliderCylinder collider;
    /* 0x1DC */ s16 delayTimer;
    /* 0x1DE */ s8 objectSlot;
    /* 0x1E0 */ s16 eyeTexIndex;
    /* 0x1E2 */ s16 blinkTimer;
    /* 0x1E4 */ EnOssanBlinkFunc blinkFunc;
    /* 0x1E8 */ EnGirlA* items[8];
    /* 0x208 */ s32 stickAccumX;
    /* 0x20C */ s32 stickAccumY;
    /* 0x210 */ u8 moveHorizontal;
    /* 0x211 */ u8 moveVertical;
    /* 0x214 */ Vec3f cursorPos;
    /* 0x220 */ Color_RGBAu32 cursorColor;
    /* 0x230 */ f32 cursorAnimTween;
    /* 0x234 */ u8 cursorAnimState;
    /* 0x235 */ u8 drawCursor;
    /* 0x236 */ u8 cursorIndex;
    /* 0x238 */ StickDirectionPrompt stickLeftPrompt;
    /* 0x270 */ StickDirectionPrompt stickRightPrompt;
    /* 0x2A8 */ f32 arrowAnimTween;
    /* 0x2AC */ f32 stickAnimTween;
    /* 0x2B0 */ u8 arrowAnimState;
    /* 0x2B1 */ u8 stickAnimState;
    /* 0x2B4 */ f32 shopItemSelectedTween;
    /* 0x2B8 */ s16 lookToShopkeeperCsId;
    /* 0x2BA */ s16 lookToLeftShelfCsId;
    /* 0x2BC */ s16 lookToRightShelfCsId;
    /* 0x2BE */ s16 lookToShopKeeperFromShelfCsId;
    /* 0x2C0 */ s16 csId;
    /* 0x2C2 */ s16 cutsceneState;
    /* 0x2C4 */ u16 textId;
    /* 0x2C6 */ Vec3s headRot;
    /* 0x2CC */ Vec3s unk2CC; // Set but never used
    /* 0x2D2 */ s16 fidgetTableY[ENOSSAN_LIMB_MAX];
    /* 0x2F8 */ s16 fidgetTableZ[ENOSSAN_LIMB_MAX];
    /* 0x31E */ Vec3s jointTable[ENOSSAN_LIMB_MAX];
    /* 0x390 */ Vec3s morphTable[ENOSSAN_LIMB_MAX];
    /* 0x402 */ s16 animIndex;
    /* 0x404 */ Vec3s partTimerHeadRot;
    /* 0x40A */ u16 flags;
} EnOssan; // size = 0x40C

#define ZORA_LIMB_MAX 0x14
#define BOMB_SHOPKEEPER_LIMB_MAX 0x10
#define GORON_LIMB_MAX 0x12
#define ENSOB1_LIMB_MAX MAX(MAX((s32)ZORA_LIMB_MAX, (s32)BOMB_SHOPKEEPER_LIMB_MAX), (s32)GORON_LIMB_MAX)

struct EnSob1;

typedef void (*EnSob1ActionFunc)(struct EnSob1*, PlayState*);
typedef void (*EnSob1BlinkFunc)(struct EnSob1*);

#define ENSOB1_GET_SHOPTYPE(thisx) ((thisx)->params & 0x1F)
#define ENSOB1_GET_PATH_INDEX(thisx) (((thisx)->params & 0x3E0) >> 5)

#define ENSOB1_PATH_INDEX_NONE 0x1F

typedef struct EnSob1XZRange {
    /* 0x0 */ f32 xMin;
    /* 0x4 */ f32 xMax;
    /* 0x8 */ f32 zMin;
    /* 0xC */ f32 zMax;
} EnSob1XZRange; // size = 0x10

typedef struct EnSob1 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnSob1ActionFunc actionFunc;
    /* 0x18C */ EnSob1ActionFunc prevActionFunc; // Used to restore back to correct browsing function
    /* 0x190 */ EnSob1ActionFunc changeObjectFunc;
    /* 0x194 */ ColliderCylinder collider;
    /* 0x1E0 */ Path* path;
    /* 0x1E4 */ s32 waypoint;
    /* 0x1E8 */ s16 delayTimer;
    /* 0x1EA */ s8 mainObjectSlot;
    /* 0x1EB */ s8 unusedObjectSlot;
    /* 0x1EC */ s8 shopkeeperAnimObjectSlot;
    /* 0x1EE */ s16 headRot;
    /* 0x1F0 */ s16 headRotTarget;
    /* 0x1F2 */ Vec3s jointTable[ENSOB1_LIMB_MAX];
    /* 0x26A */ Vec3s morphTable[ENSOB1_LIMB_MAX];
    /* 0x2E2 */ s16 eyeTexIndex;
    /* 0x2E4 */ s16 blinkTimer;
    /* 0x2E8 */ EnSob1BlinkFunc blinkFunc;
    /* 0x2EC */ EnGirlA* items[3]; // Items on shelf are indexed as: /* 2 1 0 */
    /* 0x2F8 */ s32 stickAccumX;
    /* 0x2FC */ s32 stickAccumY;
    /* 0x300 */ Vec3f cursorPos;
    /* 0x30C */ Color_RGBAu32 cursorColor;
    /* 0x31C */ f32 cursorAnimTween;
    /* 0x320 */ u8 cursorAnimState;
    /* 0x321 */ u8 drawCursor;
    /* 0x322 */ u8 cursorIndex;
    /* 0x324 */ StickDirectionPrompt stickLeftPrompt;
    /* 0x35C */ StickDirectionPrompt stickRightPrompt;
    /* 0x394 */ f32 arrowAnimTween;
    /* 0x398 */ f32 stickAnimTween;
    /* 0x39C */ u8 arrowAnimState;
    /* 0x39D */ u8 stickAnimState;
    /* 0x39E */ s16 cutsceneState;
    /* 0x3A0 */ s16 csId;
    /* 0x3A2 */ s16 lookFowardCsId;
    /* 0x3A4 */ s16 lookToShelfCsId;
    /* 0x3A6 */ s16 lookToShopkeeperCsId;
    /* 0x3A8 */ UNK_TYPE1 pad3A8[0x4];
    /* 0x3AC */ f32 shopItemSelectedTween;
    /* 0x3B0 */ UNK_TYPE1 pad3B0[0x4];
    /* 0x3B4 */ u16 welcomeTextId;
    /* 0x3B6 */ u16 talkOptionTextId;
    /* 0x3B8 */ u16 goodbyeTextId;
    /* 0x3BA */ u8 wasTalkedToWhileWalking;
    /* 0x3BC */ EnSob1XZRange posXZRange;
    /* 0x3CC */ s16 shopType;
} EnSob1; // size = 0x3D0

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

// Curiosity Shop
void EnFsn_SetupResumeInteraction(EnFsn* this, PlayState* play);
void EnFsn_PlayerCannotBuy(EnFsn* this, PlayState* play);

RECOMP_PATCH void EnFsn_GiveItem(EnFsn* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        if ((this->isSelling == true) && (this->items[this->cursorIndex]->getItemId == GI_MASK_ALL_NIGHT)) {
            Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_ALL_NIGHT_MASK);
            Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_CURIOSITY_SHOP_MAN);
        }
        this->actor.parent = NULL;
        if (ENFSN_IS_SHOP(&this->actor) && !this->isSelling) {
            Player_UpdateBottleHeld(play, GET_PLAYER(play), ITEM_BOTTLE, PLAYER_IA_BOTTLE_EMPTY);
        }
        this->actionFunc = EnFsn_SetupResumeInteraction;
    } else if (this->isSelling == true) {
        // Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
        Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
    } else {
        // note: this gets hit 4 times so i don't think vanilla rewards can work here
        // Actor_OfferGetItem(&this->actor, play, this->getItemId, 300.0f, 300.0f);
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_FSN_RUPEE), LOCATION_FSN_RUPEE, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void EnFsn_HandleCanPlayerBuyItem(EnFsn* this, PlayState* play) {
    EnGirlA* item = this->items[this->cursorIndex];

    switch (item->canBuyFunc(play, item)) {
        case CANBUY_RESULT_SUCCESS_2:
            Audio_PlaySfx_MessageDecide();
            SET_WEEKEVENTREG(WEEKEVENTREG_BOUGHT_CURIOSITY_SHOP_SPECIAL_ITEM);
            // fallthrough
        case CANBUY_RESULT_SUCCESS_1:
            if (this->cutsceneState == ENFSN_CUTSCENESTATE_PLAYING) {
                CutsceneManager_Stop(this->csId);
                this->cutsceneState = ENFSN_CUTSCENESTATE_STOPPED;
            }
            Audio_PlaySfx_MessageDecide();
            item = this->items[this->cursorIndex];
            item->buyFanfareFunc(play, item);
            // Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
            Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
            Rupees_ChangeBy(-20);
            play->msgCtx.msgMode = MSGMODE_TEXT_CLOSING;
            play->msgCtx.stateTimer = 4;
            Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
            this->drawCursor = 0;
            this->shopItemSelectedTween = 0.0f;
            item = this->items[this->cursorIndex];
            item->boughtFunc(play, item);
            if (this->stolenItem1 == this->cursorIndex) {
                SET_STOLEN_ITEM_1(STOLEN_ITEM_NONE);
            } else if (this->stolenItem2 == this->cursorIndex) {
                SET_STOLEN_ITEM_2(STOLEN_ITEM_NONE);
            }
            this->numSellingItems--;
            this->itemIds[this->cursorIndex] = -1;
            this->actionFunc = EnFsn_GiveItem;
            break;

        case CANBUY_RESULT_NEED_RUPEES:
            Audio_PlaySfx(NA_SE_SY_ERROR);
            Message_ContinueTextbox(play, 0x29F0);
            this->actionFunc = EnFsn_PlayerCannotBuy;
            break;

        case CANBUY_RESULT_CANNOT_GET_NOW:
            Audio_PlaySfx(NA_SE_SY_ERROR);
            Message_ContinueTextbox(play, 0x29DD);
            this->actionFunc = EnFsn_PlayerCannotBuy;
            break;

        default:
            break;
    }
}

// Trading Post
void EnOssan_SetupAction(EnOssan* this, EnOssanActionFunc action);
void EnOssan_SetupItemPurchased(EnOssan* this, PlayState* play);

RECOMP_PATCH void EnOssan_BuyItemWithFanfare(EnOssan* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        EnOssan_SetupAction(this, EnOssan_SetupItemPurchased);
    } else {
        // Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
        Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void EnOssan_SetupBuyItemWithFanfare(PlayState* play, EnOssan* this) {
    Player* player = GET_PLAYER(play);

    // Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
    Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
    Rupees_ChangeBy(-20);
    play->msgCtx.msgMode = MSGMODE_TEXT_CLOSING;
    play->msgCtx.stateTimer = 4;
    player->stateFlags2 &= ~PLAYER_STATE2_20000000;
    Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
    this->drawCursor = 0;
    EnOssan_SetupAction(this, EnOssan_BuyItemWithFanfare);
}

// Zora, Goron, and Bomb
void EnSob1_SetupAction(EnSob1* this, EnSob1ActionFunc action);
void EnSob1_SetupItemPurchased(EnSob1* this, PlayState* play);

RECOMP_PATCH void EnSob1_BuyItemWithFanfare(EnSob1* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        EnSob1_SetupAction(this, EnSob1_SetupItemPurchased);
    } else {
        // Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
        Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void EnSob1_SetupBuyItemWithFanfare(PlayState* play, EnSob1* this) {
    Player* player = GET_PLAYER(play);

    // Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
    Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
    Rupees_ChangeBy(-20);
    play->msgCtx.msgMode = MSGMODE_TEXT_CLOSING;
    play->msgCtx.stateTimer = 4;
    player->stateFlags2 &= ~PLAYER_STATE2_20000000;
    Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
    this->drawCursor = 0;
    EnSob1_SetupAction(this, EnSob1_BuyItemWithFanfare);
}

// Kotake
void EnTrt_SetupItemGiven(EnTrt* this, PlayState* play);

#define ENTRT_CUTSCENESTATE_STOPPED 0x0
#define ENTRT_CUTSCENESTATE_PLAYING 0x3

RECOMP_PATCH void EnTrt_BuyItemWithFanfare(EnTrt* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnTrt_SetupItemGiven;
    } else {
        // Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
        Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void EnTrt_SetupBuyItemWithFanfare(PlayState* play, EnTrt* this) {
    Player* player = GET_PLAYER(play);

    // Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
    Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
    Rupees_ChangeBy(-20);
    play->msgCtx.msgMode = MSGMODE_TEXT_CLOSING;
    play->msgCtx.stateTimer = 4;
    player->stateFlags2 &= ~PLAYER_STATE2_20000000;
    Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
    this->drawCursor = 0;
    this->actionFunc = EnTrt_BuyItemWithFanfare;
}

s32 EnTrt_TakeItemOffShelf(EnTrt *this);
u16 EnTrt_GetItemTextId(EnTrt *this);
s32 EnTrt_TestCancelOption(EnTrt *this, PlayState *play, Input *input);
void EnTrt_SetupCannotBuy(PlayState *play, EnTrt *this, u16 textId);
void EnTrt_HandleCanBuyItem(PlayState *play, EnTrt *this);

RECOMP_PATCH void EnTrt_SelectItem(EnTrt* this, PlayState* play) {
    EnGirlA* item = this->items[this->cursorIndex];
    u8 talkState = Message_GetState(&play->msgCtx);

    if (EnTrt_TakeItemOffShelf(this)) {
        if (talkState == TEXT_STATE_CHOICE) {
            Interface_SetAButtonDoAction(play, DO_ACTION_DECIDE);
            if (!EnTrt_TestCancelOption(this, play, CONTROLLER1(&play->state)) && Message_ShouldAdvance(play)) {
                switch (play->msgCtx.choiceIndex) {
                    case 0:
                        EnTrt_HandleCanBuyItem(play, this);
                        break;

                    case 1:
                        Audio_PlaySfx_MessageCancel();
                        this->actionFunc = this->prevActionFunc;
                        Message_ContinueTextbox(play, EnTrt_GetItemTextId(this));
                        break;

                    default:
                        break;
                }
            }
        } else if ((talkState == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
            // if (!Inventory_HasEmptyBottle()) {
            if (!Inventory_HasEmptyBottle() && !rando_shopsanity_enabled()) {
                Audio_PlaySfx(NA_SE_SY_ERROR);
                EnTrt_SetupCannotBuy(play, this, 0x846);
            } else {
                if (this->cutsceneState == ENTRT_CUTSCENESTATE_PLAYING) {
                    CutsceneManager_Stop(this->csId);
                    this->cutsceneState = ENTRT_CUTSCENESTATE_STOPPED;
                }
                EnTrt_SetupBuyItemWithFanfare(play, this);
                this->drawCursor = 0;
                this->shopItemSelectedTween = 0.0f;
                item->boughtFunc(play, item);
                SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_FREE_BLUE_POTION);
            }
        }
    }
}