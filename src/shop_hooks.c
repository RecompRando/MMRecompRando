#include "modding.h"
#include "global.h"

#include "apcommon.h"
#include "overlays/actors/ovl_En_GirlA/z_en_girla.h"

#define LOCATION_SHOP_ITEM (0x090000 | this->items[this->cursorIndex]->itemParams)
#define LOCATION_FSN_RUPEE (0x070000 | (this->actor.id) << 8 | this->getItemId)

#define FSN_LIMB_MAX 0x12
#define ENFSN_LIMB_MAX FSN_LIMB_MAX + 1 // Note: adding 1 to FSN_LIMB_MAX due to bug in the skeleton, see bug in object_fsn.xml
#include "overlays/actors/ovl_En_Fsn/z_en_fsn.h"

typedef enum {
    /* 0 */ ENFSN_CUTSCENESTATE_STOPPED,
    /* 1 */ ENFSN_CUTSCENESTATE_WAITING,
    /* 2 */ ENFSN_CUTSCENESTATE_PLAYING
} EnFsnCutsceneState;

#define ANI_LIMB_MAX 0x10
#define ENOSSAN_LIMB_MAX MAX((s32)FSN_LIMB_MAX + 1, (s32)ANI_LIMB_MAX)
#include "overlays/actors/ovl_En_Ossan/z_en_ossan.h"

#define ZORA_LIMB_MAX 0x14
#define BOMB_SHOPKEEPER_LIMB_MAX 0x10
#define GORON_LIMB_MAX 0x12
#define ENSOB1_LIMB_MAX MAX(MAX((s32)ZORA_LIMB_MAX, (s32)BOMB_SHOPKEEPER_LIMB_MAX), (s32)GORON_LIMB_MAX)
#include "overlays/actors/ovl_En_Sob1/z_en_sob1.h"

#include "overlays/actors/ovl_En_Trt/z_en_trt.h"

// Curiosity Shop
void EnFsn_SetupResumeInteraction(EnFsn* this, PlayState* play);
void EnFsn_PlayerCannotBuy(EnFsn* this, PlayState* play);

RECOMP_PATCH void EnFsn_GiveItem(EnFsn* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        if ((this->isSelling == true) &&
            (this->items[this->cursorIndex]->getItemId == GI_MASK_ALL_NIGHT || this->items[this->cursorIndex]->actor.params == SI_MASK_ALL_NIGHT)) {
            Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_ALL_NIGHT_MASK);
            Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_CURIOSITY_SHOP_MAN);
        }
        this->actor.parent = NULL;
        if (ENFSN_IS_SHOP(&this->actor) && !this->isSelling) {
            Player_UpdateBottleHeld(play, GET_PLAYER(play), ITEM_BOTTLE, PLAYER_IA_BOTTLE_EMPTY);
        }
        this->actionFunc = EnFsn_SetupResumeInteraction;
        this->getItemId = 0;
    } else if (this->isSelling == true) {
        if (rando_location_is_checked(LOCATION_SHOP_ITEM) || !rando_shopsanity_enabled()) {
            Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
        } else {
            Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
        }
    } else {
        if (this->getItemId == GI_MASK_KEATON || this->getItemId == GI_LETTER_TO_MAMA) {
            Actor_OfferGetItem(&this->actor, play, this->getItemId, 300.0f, 300.0f);
        } else {
            if (!rando_get_curiostity_shop_trades() || rando_location_is_checked(LOCATION_FSN_RUPEE)) {
                Actor_OfferGetItem(&this->actor, play, this->getItemId, 300.0f, 300.0f);
            } else {
                Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_FSN_RUPEE), LOCATION_FSN_RUPEE, 300.0f, 300.0f, true, true);
            }
        }
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
            if (rando_location_is_checked(LOCATION_SHOP_ITEM) || !rando_shopsanity_enabled()) {
                Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
            } else {
                Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
            }
            Rupees_ChangeBy(-play->msgCtx.unk1206C);
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

extern ShopItem sShops_ovl_En_Ossan[2][8];

RECOMP_HOOK("EnOssan_SpawnShopItems")
void EnOssan_FixNightShops(EnOssan* this, PlayState* play, ShopItem* shop) {
    // set trading post shop to use day items if advanced shopsanity is disabled
    if (this->actor.params == 1 && rando_get_slotdata_u32("shopsanity") == 1) {
        Lib_MemCpy(shop, sShops_ovl_En_Ossan[0], sizeof(sShops_ovl_En_Ossan[0]));
    }
}

RECOMP_PATCH void EnOssan_BuyItemWithFanfare(EnOssan* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        EnOssan_SetupAction(this, EnOssan_SetupItemPurchased);
    } else {
        if (rando_location_is_checked(LOCATION_SHOP_ITEM) || !rando_shopsanity_enabled()) {
            Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
        } else {
            Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
        }
    }
}

RECOMP_PATCH void EnOssan_SetupBuyItemWithFanfare(PlayState* play, EnOssan* this) {
    Player* player = GET_PLAYER(play);

    if (rando_location_is_checked(LOCATION_SHOP_ITEM) || !rando_shopsanity_enabled()) {
        Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
    } else {
        Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
    }
    Rupees_ChangeBy(-play->msgCtx.unk1206C);
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

extern ShopItem sShops_ovl_En_Sob1[4][3];

RECOMP_HOOK("EnSob1_SpawnShopItems")
void EnSob1_FixGoronSpringShop(EnSob1* this, PlayState* play, ShopItem* shopItem) {
    // set spring goron shop to use normal shop items if advanced shopsanity is disabled
    if (this->shopType == GORON_SHOP_SPRING && rando_get_slotdata_u32("shopsanity") == 1) {
        Lib_MemCpy(shopItem, sShops_ovl_En_Sob1[GORON_SHOP], sizeof(sShops_ovl_En_Sob1[GORON_SHOP]));
    }
}

RECOMP_PATCH void EnSob1_BuyItemWithFanfare(EnSob1* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        EnSob1_SetupAction(this, EnSob1_SetupItemPurchased);
    } else {
        if (rando_location_is_checked(LOCATION_SHOP_ITEM) || !rando_shopsanity_enabled()) {
            Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
        } else {
            Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
        }
    }
}

RECOMP_PATCH void EnSob1_SetupBuyItemWithFanfare(PlayState* play, EnSob1* this) {
    Player* player = GET_PLAYER(play);

    if (rando_location_is_checked(LOCATION_SHOP_ITEM) || !rando_shopsanity_enabled()) {
        Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
    } else {
        Actor_OfferGetItemHook(&this->actor, play, this->items[this->cursorIndex]->getItemId, LOCATION_SHOP_ITEM, 300.0f, 300.0f, true, true);
    }
    Rupees_ChangeBy(-play->msgCtx.unk1206C);
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

u32 location_to_buy;

void EnTrt_ShopsanityBuyItemWithFanfare(EnTrt* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnTrt_SetupItemGiven;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(location_to_buy), location_to_buy, 300.0f, 300.0f, true, true);
    }
}

void EnTrt_VanillaBuyItemWithFanfare(EnTrt* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnTrt_SetupItemGiven;
    } else {
        Actor_OfferGetItem(&this->actor, play, this->items[this->cursorIndex]->getItemId, 300.0f, 300.0f);
    }
}

RECOMP_PATCH void EnTrt_BuyItemWithFanfare(EnTrt* this, PlayState* play) {
    if (!rando_shopsanity_enabled() || rando_location_is_checked(location_to_buy)) {
        EnTrt_VanillaBuyItemWithFanfare(this, play);
    } else {
        EnTrt_ShopsanityBuyItemWithFanfare(this, play);
    }
}

RECOMP_PATCH void EnTrt_SetupBuyItemWithFanfare(PlayState* play, EnTrt* this) {
    Player* player = GET_PLAYER(play);

    Rupees_ChangeBy(-play->msgCtx.unk1206C);
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
void EnTrt_SetupCanBuy(PlayState* play, EnTrt* this, u16 textId);
void EnTrt_SetupCannotBuy(PlayState *play, EnTrt *this, u16 textId);

void EnTrt_ShopsanityHandleCanBuyItem(PlayState* play, EnTrt* this) {
    EnGirlA* item = this->items[this->cursorIndex];
    EnGirlA* item2;

    switch (item->canBuyFunc(play, item)) {
        case CANBUY_RESULT_SUCCESS_2:
        case CANBUY_RESULT_NO_ROOM:
        case CANBUY_RESULT_NEED_EMPTY_BOTTLE:
        case CANBUY_RESULT_SUCCESS_1:
            if (this->cutsceneState == ENTRT_CUTSCENESTATE_PLAYING) {
                CutsceneManager_Stop(this->csId);
                this->cutsceneState = ENTRT_CUTSCENESTATE_STOPPED;
            }
            Audio_PlaySfx_MessageDecide();
            item2 = this->items[this->cursorIndex];
            item2->buyFanfareFunc(play, item2);
            EnTrt_SetupBuyItemWithFanfare(play, this);
            this->drawCursor = 0;
            this->shopItemSelectedTween = 0.0f;
            item->boughtFunc(play, item);
            break;

        case CANBUY_RESULT_NEED_RUPEES:
            Audio_PlaySfx(NA_SE_SY_ERROR);
            EnTrt_SetupCannotBuy(play, this, 0x847);
            break;

        case CANBUY_RESULT_CANNOT_GET_NOW:
            Audio_PlaySfx(NA_SE_SY_ERROR);
            EnTrt_SetupCannotBuy(play, this, 0x643);
            break;

        default:
            break;
    }
}

void EnTrt_VanillaHandleCanBuyItem(PlayState* play, EnTrt* this) {
    EnGirlA* item = this->items[this->cursorIndex];
    EnGirlA* item2;

    switch (item->canBuyFunc(play, item)) {
        case CANBUY_RESULT_SUCCESS_1:
            if (this->cutsceneState == ENTRT_CUTSCENESTATE_PLAYING) {
                CutsceneManager_Stop(this->csId);
                this->cutsceneState = ENTRT_CUTSCENESTATE_STOPPED;
            }
            Audio_PlaySfx_MessageDecide();
            item2 = this->items[this->cursorIndex];
            item2->buyFanfareFunc(play, item2);
            EnTrt_SetupBuyItemWithFanfare(play, this);
            this->drawCursor = 0;
            this->shopItemSelectedTween = 0.0f;
            item->boughtFunc(play, item);
            break;

        case CANBUY_RESULT_SUCCESS_2:
            Audio_PlaySfx_MessageDecide();
            item->buyFunc(play, item);
            EnTrt_SetupCanBuy(play, this, 0x848);
            this->drawCursor = 0;
            this->shopItemSelectedTween = 0.0f;
            item->boughtFunc(play, item);
            break;

        case CANBUY_RESULT_NO_ROOM:
            Audio_PlaySfx(NA_SE_SY_ERROR);
            EnTrt_SetupCannotBuy(play, this, 0x641);
            break;

        case CANBUY_RESULT_NEED_EMPTY_BOTTLE:
            Audio_PlaySfx(NA_SE_SY_ERROR);
            EnTrt_SetupCannotBuy(play, this, 0x846);
            break;

        case CANBUY_RESULT_NEED_RUPEES:
            Audio_PlaySfx(NA_SE_SY_ERROR);
            EnTrt_SetupCannotBuy(play, this, 0x847);
            break;

        case CANBUY_RESULT_CANNOT_GET_NOW:
            Audio_PlaySfx(NA_SE_SY_ERROR);
            EnTrt_SetupCannotBuy(play, this, 0x643);
            break;

        default:
            break;
    }
}

bool kotake_is_weird = false;

RECOMP_PATCH void EnTrt_HandleCanBuyItem(PlayState* play, EnTrt* this) {
    if (kotake_is_weird) {
        EnTrt_ShopsanityHandleCanBuyItem(play, this);
    } else {
        EnTrt_VanillaHandleCanBuyItem(play, this);
    }
}

bool shopItemIsChecked(EnGirlA* item, PlayState* play) {
    return rando_location_is_checked_async(0x090000 | item->actor.params);
}

extern bool kotake_is_weird;

RECOMP_PATCH void EnTrt_SelectItem(EnTrt* this, PlayState* play) {
    EnGirlA* item = this->items[this->cursorIndex];
    u8 talkState = Message_GetState(&play->msgCtx);

    location_to_buy = (0x090000 | item->actor.params) & 0xFFFFFF;

    if (EnTrt_TakeItemOffShelf(this)) {
        if (talkState == TEXT_STATE_CHOICE) {
            func_8011552C(play, DO_ACTION_DECIDE);
            if (!EnTrt_TestCancelOption(this, play, CONTROLLER1(&play->state)) && Message_ShouldAdvance(play)) {
                switch (play->msgCtx.choiceIndex) {
                    case 0:
                        kotake_is_weird = rando_shopsanity_enabled() && !shopItemIsChecked(item, play);
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
        } else if ((talkState == TEXT_STATE_5) && Message_ShouldAdvance(play)) {
            // if (!Inventory_HasEmptyBottle()) {
            if (!Inventory_HasEmptyBottle() && (rando_location_is_checked(location_to_buy) || !rando_shopsanity_enabled())) {
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