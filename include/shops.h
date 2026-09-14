#define FSN_LIMB_MAX 0x12
#define ENFSN_LIMB_MAX FSN_LIMB_MAX + 1
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

extern EnFsn* sEnFsn;
extern EnOssan* sEnOssan;
extern EnSob1* sEnSob1;
extern EnTrt* sEnTrt;

#include "overlays/actors/ovl_En_GirlA/z_en_girla.h"

#define LOCATION_SHOP_ITEM (0x090000 | this->items[this->cursorIndex]->itemParams)
#define LOCATION_SHOP_ITEM_ID(shopId) (0x090000 | shopId)
#define LOCATION_FSN_RUPEE (0x070000 | (this->actor.id) << 8 | this->getItemId)

s32 rando_get_shop_price(u32 shop_item_id);