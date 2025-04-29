#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define LOCATION_SHOP_ITEM (0x090000 | this->actor.params)
#define SHOP_ITEM_TEXT (0x3600 | this->actor.params)
#define SHOP_ITEM_BUY_TEXT (0x3700 | this->actor.params)

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

#include "overlays/actors/ovl_En_GirlA/z_en_girla.h"

#define THIS ((EnGirlA*)thisx)

void EnGirlA_Init(Actor* thisx, PlayState* play);
void EnGirlA_Destroy(Actor* thisx, PlayState* play);
void EnGirlA_Update(Actor* thisx, PlayState* play);
void EnGirlA_Draw(Actor* thisx, PlayState* play);

void EnGirlA_InitalUpdate(EnGirlA* this, PlayState* play);
void EnGirlA_Update2(EnGirlA* this, PlayState* play);

s32 EnGirlA_TrySetMaskItemDescription(EnGirlA* this, PlayState* play);
void EnGirlA_InitItem(PlayState* play, EnGirlA* this);
void EnGirlA_Bought(PlayState* play, EnGirlA* this);
void EnGirlA_Restock(PlayState* play, EnGirlA* this);
void EnGirlA_DoNothing(EnGirlA* this, PlayState* play);
void EnGirlA_SetupAction(EnGirlA* this, EnGirlAActionFunc action);

extern ShopItemEntry sShopItemEntries[];

// temporarily set to SI_MAX (43) slots to hold the items for every shop
static bool shopObjectStatic[SI_MAX];
static bool shopObjectLoading[SI_MAX];
static bool shopObjectLoaded[SI_MAX];
static OSMesgQueue shopObjectLoadQueue[SI_MAX];
static void* shopObjectSegments[SI_MAX];

void EnGirlA_WaitForObject(EnGirlA* this, PlayState* play) {
    s16 getItemId = rando_get_item_id(LOCATION_SHOP_ITEM);
    s16 objectSlot = Object_GetSlot(&play->objectCtx, getObjectId(getItemId));

    if (isAP(getItemId)) {
        shopObjectStatic[this->actor.params] = true;
        shopObjectLoaded[this->actor.params] = true;
    } else if (!shopObjectLoaded[this->actor.params] && !shopObjectLoading[this->actor.params] && Object_IsLoaded(&play->objectCtx, objectSlot)) {
        this->actor.objectSlot = objectSlot;
        Actor_SetObjectDependency(play, &this->actor);
        shopObjectStatic[this->actor.params] = true;
        shopObjectLoaded[this->actor.params] = true;
    } else if (!shopObjectLoading[this->actor.params] && !shopObjectLoaded[this->actor.params]) {
        loadObject(play, &shopObjectSegments[this->actor.params], &shopObjectLoadQueue[this->actor.params], getObjectId(getItemId));
        shopObjectLoading[this->actor.params] = true;
    } else if (osRecvMesg(&shopObjectLoadQueue[this->actor.params], NULL, OS_MESG_NOBLOCK) == 0) {
        shopObjectLoading[this->actor.params] = false;
        shopObjectLoaded[this->actor.params] = true;
    }
}

RECOMP_PATCH void EnGirlA_Update(Actor* thisx, PlayState* play) {
    EnGirlA* this = THIS;

    if (!shopObjectLoaded[this->actor.params] && rando_shopsanity_enabled() && !rando_location_is_checked(LOCATION_SHOP_ITEM)) {
        EnGirlA_WaitForObject(this, play);
    }

    this->mainActionFunc(this, play);
}

RECOMP_PATCH void EnGirlA_Draw(Actor* thisx, PlayState* play) {
    EnGirlA* this = THIS;

    Matrix_RotateYS(this->rotY, MTXMODE_APPLY);
    if (this->drawFunc != NULL) {
        this->drawFunc(&this->actor, play, 0);
    }

    if (rando_shopsanity_enabled() && !rando_location_is_checked(LOCATION_SHOP_ITEM)) {
        s16 getItemId = rando_get_item_id(LOCATION_SHOP_ITEM);
        if (shopObjectLoaded[this->actor.params]) {
            // fix rotation of some items
            switch(getItemId) {
                case GI_AP_PROG:
                case GI_AP_USEFUL:
                case GI_AP_FILLER:
                case GI_ROOM_KEY:
                    Matrix_RotateYS(DEG_TO_BINANG(90), MTXMODE_APPLY);
                    break;
                default:
                    break;
            }

            if (shopObjectStatic[this->actor.params]) {
                GetItem_Draw(play, getGid(getItemId));
            } else {
                GetItem_DrawDynamic(play, shopObjectSegments[this->actor.params], getGid(getItemId));
            }
        }
    } else {
        GetItem_Draw(play, this->getItemDrawId);
    }
}

void EnGirlA_RandoBought(PlayState* play, EnGirlA* this) {
    this->isOutOfStock = true;
    this->actor.draw = NULL;
}

// @rando restock with vanilla items by reinitializing w/ default info
void EnGirlA_RandoRestock(PlayState* play, EnGirlA* this) {
    s16 params = this->actor.params;
    ShopItemEntry* shopItem = &sShopItemEntries[params];

    if (Object_IsLoaded(&play->objectCtx, this->objectSlot)) {
        this->actor.flags &= ~ACTOR_FLAG_10;
        this->actor.objectSlot = this->objectSlot;
        this->actor.textId = shopItem->descriptionTextId;
        this->choiceTextId = shopItem->choiceTextId;

        // EnGirlA_TrySetMaskItemDescription always returns false
        if (!EnGirlA_TrySetMaskItemDescription(this, play)) {
            EnGirlA_InitItem(play, this);
        }

        this->boughtFunc = EnGirlA_Bought;
        this->restockFunc = EnGirlA_Restock;
        this->getItemId = shopItem->getItemId;
        this->canBuyFunc = shopItem->canBuyFunc;
        this->buyFunc = shopItem->buyFunc;
        this->buyFanfareFunc = shopItem->buyFanfareFunc;
        this->unk1C0 = 0;
        this->itemParams = shopItem->params;
        this->drawFunc = shopItem->drawFunc;
        this->getItemDrawId = shopItem->getItemDrawId;
        this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
        Actor_SetScale(&this->actor, 0.25f);
        this->actor.shape.yOffset = 24.0f;
        this->actor.shape.shadowScale = 4.0f;
        this->actor.floorHeight = this->actor.home.pos.y;
        this->actor.gravity = 0.0f;
        EnGirlA_SetupAction(this, EnGirlA_DoNothing);
        this->isInitialized = true;
        this->mainActionFunc = EnGirlA_Update2;
        this->isSelected = false;
        this->rotY = 0;
        this->initialRotY = this->actor.shape.rot.y;
    }
}

s32 EnGirlA_RandoCanBuyFunc(PlayState* play, EnGirlA* this) {
    if (gSaveContext.save.saveInfo.playerData.rupees < play->msgCtx.unk1206C) {
        return CANBUY_RESULT_NEED_RUPEES;
    }

    return CANBUY_RESULT_SUCCESS_1;
}

// not used?
void EnGirlA_RandoBuyFunc(PlayState* play, EnGirlA* this) {
    Rupees_ChangeBy(-play->msgCtx.unk1206C);
}

// Fanfare is handled by the shopkeeper
void EnGirlA_RandoBuyFanfare(PlayState* play, EnGirlA* this) {
}

RECOMP_PATCH void EnGirlA_InitItem(PlayState* play, EnGirlA* this) {
    if (!rando_shopsanity_enabled() || rando_location_is_checked(LOCATION_SHOP_ITEM)) {
        ShopItemEntry* shopItem = &sShopItemEntries[this->actor.params];

        this->actor.textId = shopItem->descriptionTextId;
        this->isOutOfStock = false;
        this->actor.draw = EnGirlA_Draw;
        return;
    }
    
    shopObjectStatic[this->actor.params] = false;
    shopObjectLoading[this->actor.params] = false;
    shopObjectLoaded[this->actor.params] = false;

    this->actor.textId = SHOP_ITEM_TEXT;
    this->isOutOfStock = false;
    this->actor.draw = EnGirlA_Draw;
}

RECOMP_PATCH void EnGirlA_InitalUpdate(EnGirlA* this, PlayState* play) {
    s16 params = this->actor.params;
    if (rando_shopsanity_enabled() && !rando_location_is_checked(LOCATION_SHOP_ITEM)
        && !(this->actor.params == SI_BOTTLE ||
            this->actor.params == SI_SWORD_GREAT_FAIRY ||
            this->actor.params == SI_SWORD_KOKIRI ||
            this->actor.params == SI_SWORD_RAZOR ||
            this->actor.params == SI_SWORD_GILDED
            )) {
        s16 trueGI = rando_get_item_id(LOCATION_SHOP_ITEM);
        ShopItemEntry item = { getObjectId(trueGI), getGid(trueGI), NULL, 1, SHOP_ITEM_TEXT, SHOP_ITEM_BUY_TEXT, trueGI, EnGirlA_RandoCanBuyFunc,
            EnGirlA_RandoBuyFunc, EnGirlA_RandoBuyFanfare };
        ShopItemEntry* shopItem = &item;

        if (Object_IsLoaded(&play->objectCtx, this->objectSlot)) {
            this->actor.flags &= ~ACTOR_FLAG_10;
            this->actor.objectSlot = this->objectSlot;
            this->actor.textId = shopItem->descriptionTextId;
            this->choiceTextId = shopItem->choiceTextId;

            // EnGirlA_TrySetMaskItemDescription always returns false
            if (!EnGirlA_TrySetMaskItemDescription(this, play)) {
                EnGirlA_InitItem(play, this);
            }

            this->boughtFunc = EnGirlA_RandoBought;
            this->restockFunc = EnGirlA_RandoRestock;
            this->getItemId = shopItem->getItemId;
            this->canBuyFunc = shopItem->canBuyFunc;
            this->buyFunc = shopItem->buyFunc;
            this->buyFanfareFunc = shopItem->buyFanfareFunc;
            this->unk1C0 = 0;
            // this->itemParams = shopItem->params;
            this->itemParams = this->actor.params;
            this->drawFunc = shopItem->drawFunc;
            this->getItemDrawId = shopItem->getItemDrawId;
            this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
            Actor_SetScale(&this->actor, 0.25f);
            this->actor.shape.yOffset = 24.0f;
            this->actor.shape.shadowScale = 4.0f;
            this->actor.floorHeight = this->actor.home.pos.y;
            this->actor.gravity = 0.0f;
            EnGirlA_SetupAction(this, EnGirlA_DoNothing);
            this->isInitialized = true;
            this->mainActionFunc = EnGirlA_Update2;
            this->isSelected = false;
            this->rotY = 0;
            this->initialRotY = this->actor.shape.rot.y;
        }
    } else {
        ShopItemEntry* shopItem = &sShopItemEntries[params];

        if (Object_IsLoaded(&play->objectCtx, this->objectSlot)) {
            this->actor.flags &= ~ACTOR_FLAG_10;
            this->actor.objectSlot = this->objectSlot;
            this->actor.textId = shopItem->descriptionTextId;
            this->choiceTextId = shopItem->choiceTextId;

            // EnGirlA_TrySetMaskItemDescription always returns false
            if (!EnGirlA_TrySetMaskItemDescription(this, play)) {
                EnGirlA_InitItem(play, this);
            }

            this->boughtFunc = EnGirlA_Bought;
            this->restockFunc = EnGirlA_Restock;
            this->getItemId = shopItem->getItemId;
            this->canBuyFunc = shopItem->canBuyFunc;
            this->buyFunc = shopItem->buyFunc;
            this->buyFanfareFunc = shopItem->buyFanfareFunc;
            this->unk1C0 = 0;
            this->itemParams = shopItem->params;
            this->drawFunc = shopItem->drawFunc;
            this->getItemDrawId = shopItem->getItemDrawId;
            this->actor.flags &= ~ACTOR_FLAG_TARGETABLE;
            Actor_SetScale(&this->actor, 0.25f);
            this->actor.shape.yOffset = 24.0f;
            this->actor.shape.shadowScale = 4.0f;
            this->actor.floorHeight = this->actor.home.pos.y;
            this->actor.gravity = 0.0f;
            EnGirlA_SetupAction(this, EnGirlA_DoNothing);
            this->isInitialized = true;
            this->mainActionFunc = EnGirlA_Update2;
            this->isSelected = false;
            this->rotY = 0;
            this->initialRotY = this->actor.shape.rot.y;
        }
    }
}

// @rando prevent vanilla shield from being bought without already having a progressive shield
RECOMP_PATCH s32 EnGirlA_CanBuyShieldHero(PlayState* play, EnGirlA* this) {
    if (!rando_has_item(GI_SHIELD_HERO)) {
        return CANBUY_RESULT_CANNOT_GET_NOW;
    }
    if (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SHIELD) != EQUIP_VALUE_SHIELD_NONE) {
        return CANBUY_RESULT_NO_ROOM;
    }
    if (gSaveContext.save.saveInfo.playerData.rupees < play->msgCtx.unk1206C) {
        return CANBUY_RESULT_NEED_RUPEES;
    }
    return CANBUY_RESULT_SUCCESS_1;
}