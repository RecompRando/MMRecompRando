#include "modding.h"
#include "global.h"
#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

#include "apcommon.h"

ItemId KaleidoScope_RandoGetNextTradeItem(ItemId slot, ItemId max) {
    u32 i, first_i, tradeGIOffset;

    // each trade item has a different offset to convert ItemIds to GetItemIds
    switch (slot) {
        case ITEM_MOONS_TEAR:
            tradeGIOffset = 0x6E;
            break;
        case ITEM_ROOM_KEY:
            tradeGIOffset = 0x73;
            break;
        case ITEM_LETTER_TO_KAFEI:
            tradeGIOffset = 0x7B;
            break;
        default:
            return INV_CONTENT(slot);
    }

    u32 slotAsGI = slot + tradeGIOffset;
    u32 maxAsGI = max + tradeGIOffset;
    
    // failsafe(?) if there's no item in the slot but we want to cycle to it
    if (INV_CONTENT(slot) == ITEM_NONE) {
        for (i = slotAsGI; i <= maxAsGI; ++i) {
            if (rando_has_item(i)) {
                return i - tradeGIOffset;
            }
        }
        return INV_CONTENT(slot);
    } else {
        first_i = i = INV_CONTENT(slot) + tradeGIOffset; // convert to GI (causes a bug, see rando_has_item later)
        while (true) { // scary while true loop
            i++;
            if (i >= (maxAsGI + 1)) {
                // if the index flies past the max, set it back to the intial item (original slot item)
                i = slotAsGI;
            }
            if (i == first_i) {
                // we've done a full cycle,
                // the player has no other items
                return i - tradeGIOffset;
            }
            // @bug: there is a bug here intentionally left in if you equip swap a bottle over the Moon's Tear slot
            // we converted ItemIds to GetItemIds as they are the item ids used for the randomizer,
            // the offset 0x6E used for this conversion works if we're looking at items that would normally be here,
            // however if a Bottle is here this logic goes awry as the offset does not convert
            // Bottle ItemIds to Bottle GetItemIds, so it checks against what's actually there, which are Mask GIs
            // so in practice, if you have an Empty Bottle and try to cycle, you'll get ITEM_BOTTLE + 0x6E + 1,
            // which ends up being equivalent to GI_MASK_GARO, meaning if you have received Garo Mask in the randomizer
            // then the bottle will end up cycling to Red Potion (GI_MASK_GARO - 0x6E = ITEM_POTION_RED) and so on
            if (rando_has_item(i)) {
                // the player has it, give it to them
                return i - tradeGIOffset;
            }
        }
    }
}

void Rando_CylceTradeItem(ItemId slot, ItemId max) {
    ItemId current_item = INV_CONTENT(slot);
    ItemId next_item = KaleidoScope_RandoGetNextTradeItem(slot, max);
    if (current_item != next_item) {
        INV_CONTENT(slot) = next_item;
        Audio_PlaySfx(NA_SE_SY_CURSOR);
    }
}

RECOMP_HOOK("KaleidoScope_UpdateItemCursor")
void KaleidoScope_CycleItems(PlayState* play) {
    PauseContext* pauseCtx = &play->pauseCtx;

    if (pauseCtx->state == PAUSE_STATE_MAIN && pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE &&
        pauseCtx->pageIndex == PAUSE_ITEM && !pauseCtx->itemDescriptionOn && pauseCtx->debugEditor == DEBUG_EDITOR_NONE &&
        (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_L) || CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_A))) {
        
        // cycle to the next item in the selected slot
        bool cycle_attempted = true;
        if (pauseCtx->cursorSlot[PAUSE_ITEM] == SLOT_TRADE_DEED) {
            Rando_CylceTradeItem(ITEM_MOONS_TEAR, ITEM_DEED_OCEAN);
        } else if (pauseCtx->cursorSlot[PAUSE_ITEM] == SLOT_TRADE_KEY_MAMA) {
            Rando_CylceTradeItem(ITEM_ROOM_KEY, ITEM_LETTER_MAMA);
        } else if (pauseCtx->cursorSlot[PAUSE_ITEM] == SLOT_TRADE_COUPLE) {
            Rando_CylceTradeItem(ITEM_LETTER_TO_KAFEI, ITEM_PENDANT_OF_MEMORIES);
        } else {
            cycle_attempted = false;
        }

        if (cycle_attempted) {
            // remove the A button press to prevent the item description from showing up
            CONTROLLER1(&play->state)->press.button &= ~BTN_A;
        }
    }
}

void KaleidoScope_ApplyVtxOffset(Vtx* itemVtx, PauseContext* pauseCtx, u16 slot) {
    // undo the original function making the selected item icon bigger
    if (pauseCtx->cursorSlot[PAUSE_ITEM] == slot) {
        itemVtx[0].v.ob[0] = itemVtx[2].v.ob[0] = itemVtx[0].v.ob[0] + 2;
        itemVtx[1].v.ob[0] = itemVtx[3].v.ob[0] = itemVtx[0].v.ob[0] + 32 - 2;
        itemVtx[0].v.ob[1] = itemVtx[1].v.ob[1] = itemVtx[0].v.ob[1] - 2;
        itemVtx[2].v.ob[1] = itemVtx[3].v.ob[1] = itemVtx[0].v.ob[1] - 32 + 2;
    }

    itemVtx[0].v.ob[0] = itemVtx[2].v.ob[0] = itemVtx[0].v.ob[0] + 16; // Top Left X Offset
    itemVtx[1].v.ob[0] = itemVtx[3].v.ob[0] = itemVtx[0].v.ob[0] + 16; // Image Width
    itemVtx[0].v.ob[1] = itemVtx[1].v.ob[1] = itemVtx[0].v.ob[1] - 16; // Top Left Y Offset
    itemVtx[2].v.ob[1] = itemVtx[3].v.ob[1] = itemVtx[0].v.ob[1] - 16; // Image Height
}

RECOMP_HOOK_RETURN("KaleidoScope_DrawItemSelect")
void KaleidoScope_DrawCycleItems() {
    PlayState* play = gPlay;
    PauseContext* pauseCtx = &play->pauseCtx;
    
    ItemId next_item;
    static Vtx tradeItem1CycleVtx[4];
    static Vtx tradeItem2CycleVtx[4];
    static Vtx tradeItem3CycleVtx[4];

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL42_Opa(play->state.gfxCtx);
    
    for (u8 vert = 0; vert < 4; vert++) {
        tradeItem1CycleVtx[vert] = pauseCtx->itemVtx[(SLOT_TRADE_DEED * 4) + vert];
        tradeItem2CycleVtx[vert] = pauseCtx->itemVtx[(SLOT_TRADE_KEY_MAMA * 4) + vert];
        tradeItem3CycleVtx[vert] = pauseCtx->itemVtx[(SLOT_TRADE_COUPLE * 4) + vert];
    }
    
    KaleidoScope_ApplyVtxOffset(tradeItem1CycleVtx, pauseCtx, SLOT_TRADE_DEED);
    KaleidoScope_ApplyVtxOffset(tradeItem2CycleVtx, pauseCtx, SLOT_TRADE_KEY_MAMA);
    KaleidoScope_ApplyVtxOffset(tradeItem3CycleVtx, pauseCtx, SLOT_TRADE_COUPLE);

    next_item = KaleidoScope_RandoGetNextTradeItem(ITEM_MOONS_TEAR, ITEM_DEED_OCEAN);
    if (INV_CONTENT(ITEM_MOONS_TEAR) != next_item) {
        gSPVertex(POLY_OPA_DISP++, &tradeItem1CycleVtx[0], 4, 0);
        KaleidoScope_DrawTexQuadRGBA32(play->state.gfxCtx, gItemIcons[next_item], 32, 32, 0);
    }

    next_item = KaleidoScope_RandoGetNextTradeItem(ITEM_ROOM_KEY, ITEM_LETTER_MAMA);
    if (INV_CONTENT(ITEM_ROOM_KEY) != next_item) {
        gSPVertex(POLY_OPA_DISP++, &tradeItem2CycleVtx[0], 4, 0);
        KaleidoScope_DrawTexQuadRGBA32(play->state.gfxCtx, gItemIcons[next_item], 32, 32, 0);
    }

    next_item = KaleidoScope_RandoGetNextTradeItem(ITEM_LETTER_TO_KAFEI, ITEM_PENDANT_OF_MEMORIES);
    if (INV_CONTENT(ITEM_LETTER_TO_KAFEI) != next_item) {
        gSPVertex(POLY_OPA_DISP++, &tradeItem3CycleVtx[0], 4, 0);
        KaleidoScope_DrawTexQuadRGBA32(play->state.gfxCtx, gItemIcons[next_item], 32, 32, 0);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}

u8 gPlayerFormSlotRestrictions[PLAYER_FORM_MAX][ITEM_NUM_SLOTS] = {
    // PLAYER_FORM_FIERCE_DEITY
    {
        false, // SLOT_OCARINA
        false, // SLOT_BOW
        false, // SLOT_ARROW_FIRE
        false, // SLOT_ARROW_ICE
        false, // SLOT_ARROW_LIGHT
        false, // SLOT_TRADE_DEED
        false, // SLOT_BOMB
        false, // SLOT_BOMBCHU
        false, // SLOT_DEKU_STICK
        false, // SLOT_DEKU_NUT
        false, // SLOT_MAGIC_BEANS
        false, // SLOT_TRADE_KEY_MAMA
        false, // SLOT_POWDER_KEG
        false, // SLOT_PICTOGRAPH_BOX
        false, // SLOT_LENS_OF_TRUTH
        false, // SLOT_HOOKSHOT
        false, // SLOT_SWORD_GREAT_FAIRY
        false, // SLOT_TRADE_COUPLE
        true,  // SLOT_BOTTLE_1
        true,  // SLOT_BOTTLE_2
        true,  // SLOT_BOTTLE_3
        true,  // SLOT_BOTTLE_4
        true,  // SLOT_BOTTLE_5
        true,  // SLOT_BOTTLE_6
    },
    // PLAYER_FORM_GORON
    {
        true,  // SLOT_OCARINA
        false, // SLOT_BOW
        false, // SLOT_ARROW_FIRE
        false, // SLOT_ARROW_ICE
        false, // SLOT_ARROW_LIGHT
        true,  // SLOT_TRADE_DEED
        false, // SLOT_BOMB
        false, // SLOT_BOMBCHU
        false, // SLOT_DEKU_STICK
        false, // SLOT_DEKU_NUT
        false, // SLOT_MAGIC_BEANS
        true,  // SLOT_TRADE_KEY_MAMA
        true,  // SLOT_POWDER_KEG
        true,  // SLOT_PICTOGRAPH_BOX
        true,  // SLOT_LENS_OF_TRUTH
        false, // SLOT_HOOKSHOT
        false, // SLOT_SWORD_GREAT_FAIRY
        true,  // SLOT_TRADE_COUPLE
        true,  // SLOT_BOTTLE_1
        true,  // SLOT_BOTTLE_2
        true,  // SLOT_BOTTLE_3
        true,  // SLOT_BOTTLE_4
        true,  // SLOT_BOTTLE_5
        true,  // SLOT_BOTTLE_6
    },
    // PLAYER_FORM_ZORA
    {
        true,  // SLOT_OCARINA
        false, // SLOT_BOW
        false, // SLOT_ARROW_FIRE
        false, // SLOT_ARROW_ICE
        false, // SLOT_ARROW_LIGHT
        true,  // SLOT_TRADE_DEED
        false, // SLOT_BOMB
        false, // SLOT_BOMBCHU
        false, // SLOT_DEKU_STICK
        false, // SLOT_DEKU_NUT
        false, // SLOT_MAGIC_BEANS
        true,  // SLOT_TRADE_KEY_MAMA
        false, // SLOT_POWDER_KEG
        true,  // SLOT_PICTOGRAPH_BOX
        true,  // SLOT_LENS_OF_TRUTH
        false, // SLOT_HOOKSHOT
        false, // SLOT_SWORD_GREAT_FAIRY
        true,  // SLOT_TRADE_COUPLE
        true,  // SLOT_BOTTLE_1
        true,  // SLOT_BOTTLE_2
        true,  // SLOT_BOTTLE_3
        true,  // SLOT_BOTTLE_4
        true,  // SLOT_BOTTLE_5
        true,  // SLOT_BOTTLE_6
    },
    // PLAYER_FORM_DEKU
    {
        true,  // SLOT_OCARINA
        false, // SLOT_BOW
        false, // SLOT_ARROW_FIRE
        false, // SLOT_ARROW_ICE
        false, // SLOT_ARROW_LIGHT
        true,  // SLOT_TRADE_DEED
        false, // SLOT_BOMB
        false, // SLOT_BOMBCHU
        false, // SLOT_DEKU_STICK
        true,  // SLOT_DEKU_NUT
        false, // SLOT_MAGIC_BEANS
        true,  // SLOT_TRADE_KEY_MAMA
        false, // SLOT_POWDER_KEG
        true,  // SLOT_PICTOGRAPH_BOX
        true,  // SLOT_LENS_OF_TRUTH
        false, // SLOT_HOOKSHOT
        false, // SLOT_SWORD_GREAT_FAIRY
        true,  // SLOT_TRADE_COUPLE
        true,  // SLOT_BOTTLE_1
        true,  // SLOT_BOTTLE_2
        true,  // SLOT_BOTTLE_3
        true,  // SLOT_BOTTLE_4
        true,  // SLOT_BOTTLE_5
        true,  // SLOT_BOTTLE_6
    },
    // PLAYER_FORM_HUMAN
    {
        true,  // SLOT_OCARINA
        true,  // SLOT_BOW
        true,  // SLOT_ARROW_FIRE
        true,  // SLOT_ARROW_ICE
        true,  // SLOT_ARROW_LIGHT
        true,  // SLOT_TRADE_DEED
        true,  // SLOT_BOMB
        true,  // SLOT_BOMBCHU
        true,  // SLOT_DEKU_STICK
        true,  // SLOT_DEKU_NUT
        true,  // SLOT_MAGIC_BEANS
        true,  // SLOT_TRADE_KEY_MAMA
        false, // SLOT_POWDER_KEG
        true,  // SLOT_PICTOGRAPH_BOX
        true,  // SLOT_LENS_OF_TRUTH
        true,  // SLOT_HOOKSHOT
        true,  // SLOT_SWORD_GREAT_FAIRY
        true,  // SLOT_TRADE_COUPLE
        true,  // SLOT_BOTTLE_1
        true,  // SLOT_BOTTLE_2
        true,  // SLOT_BOTTLE_3
        true,  // SLOT_BOTTLE_4
        true,  // SLOT_BOTTLE_5
        true,  // SLOT_BOTTLE_6
    },
};

s16 sAmmoRectLeft[] = {
    95,  // SLOT_BOW
    62,  // SLOT_BOMB
    95,  // SLOT_BOMBCHU
    128, // SLOT_DEKU_STICK
    161, // SLOT_DEKU_NUT
    194, // SLOT_MAGIC_BEANS
    62,  // SLOT_POWDER_KEG
    95,  // SLOT_PICTOGRAPH_BOX
};

s16 sAmmoRectHeight[] = {
    85,  // SLOT_BOW
    117, // SLOT_BOMB
    117, // SLOT_BOMBCHU
    117, // SLOT_DEKU_STICK
    117, // SLOT_DEKU_NUT
    117, // SLOT_MAGIC_BEANS
    150, // SLOT_POWDER_KEG
    150, // SLOT_PICTOGRAPH_BOX
};

extern u64 gAmmoDigit0Tex[];

// @ap Draw green 10 Bombchu ammo count if no bomb bag.
RECOMP_PATCH void KaleidoScope_DrawAmmoCount(PauseContext* pauseCtx, GraphicsContext* gfxCtx, s16 item, u16 ammoIndex) {
    s16 ammoUpperDigit;
    s16 ammo;

    OPEN_DISPS(gfxCtx);

    if (item == ITEM_PICTOGRAPH_BOX) {
        if (!CHECK_QUEST_ITEM(QUEST_PICTOGRAPH)) {
            ammo = 0;
        } else {
            ammo = 1;
        }
    } else {
        ammo = AMMO(item);
    }

    gDPPipeSync(POLY_OPA_DISP++);

    if (!gPlayerFormSlotRestrictions[GET_PLAYER_FORM][SLOT(item)]) {
        // Ammo item is restricted
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 100, 100, 100, pauseCtx->alpha);
    } else {
        // Default ammo
        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 255, 255, 255, pauseCtx->alpha);
        if (ammo == 0) {
            // Out of ammo
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 130, 130, 130, pauseCtx->alpha);
        } else if (((item == ITEM_BOMB) && (AMMO(item) == CUR_CAPACITY(UPG_BOMB_BAG))) ||
                   ((item == ITEM_BOW) && (AMMO(item) == CUR_CAPACITY(UPG_QUIVER))) ||
                   ((item == ITEM_DEKU_STICK) && (AMMO(item) == CUR_CAPACITY(UPG_DEKU_STICKS))) ||
                   ((item == ITEM_DEKU_NUT) && (AMMO(item) == CUR_CAPACITY(UPG_DEKU_NUTS))) ||
                   // @ap Draw green Bombchu ammo count.
                   ((item == ITEM_BOMBCHU) && (AMMO(item) == MAX_BOMBCHUS)) ||
                   ((item == ITEM_POWDER_KEG) && (ammo == 1)) || ((item == ITEM_PICTOGRAPH_BOX) && (ammo == 1)) ||
                   ((item == ITEM_MAGIC_BEANS) && (ammo == 20))) {
            // Ammo at capacity
            gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, 120, 255, 0, pauseCtx->alpha);
        }
    }

    // Separate ammo into upper and lower digits
    for (ammoUpperDigit = 0; ammo >= 10; ammoUpperDigit++) {
        ammo -= 10;
    }

    gDPPipeSync(POLY_OPA_DISP++);

    // Draw upper digit
    if (ammoUpperDigit != 0) {
        POLY_OPA_DISP =
            Gfx_DrawTexRectIA8(POLY_OPA_DISP, ((u8*)gAmmoDigit0Tex + (8 * 8 * ammoUpperDigit)), 8, 8,
                               sAmmoRectLeft[ammoIndex], sAmmoRectHeight[ammoIndex], 8, 8, 1 << 10, 1 << 10);
    }

    // Draw lower digit
    POLY_OPA_DISP =
        Gfx_DrawTexRectIA8(POLY_OPA_DISP, ((u8*)gAmmoDigit0Tex + (8 * 8 * ammo)), 8, 8, sAmmoRectLeft[ammoIndex] + 6,
                           sAmmoRectHeight[ammoIndex], 8, 8, 1 << 10, 1 << 10);

    CLOSE_DISPS(gfxCtx);
}

#define AMMO_DIGIT_TEX_WIDTH  8
#define AMMO_DIGIT_TEX_HEIGHT  8
#define AMMO_DIGIT_TEX_SIZE  (AMMO_DIGIT_TEX_WIDTH * AMMO_DIGIT_TEX_HEIGHT)

extern s16 sAmmoDigitsXPositions[4];
extern s16 sAmmoDigitsYPositions[4];

u8 currentI;
u16 oldBombCapacity;

// @ap Draw green 10 Bombchu ammo count if no bomb bag.
RECOMP_HOOK("Interface_DrawAmmoCount")
void green_chu_count(PlayState* play, s16 button, s16 alpha) {
    currentI = ((void) 0, GET_CUR_FORM_BTN_ITEM(button));

    OPEN_DISPS(play->state.gfxCtx);

    gDPSetEnvColor(OVERLAY_DISP++, 0, 0, 0, 255);

    CLOSE_DISPS(play->state.gfxCtx);

    if (currentI == ITEM_BOMBCHU) {
        oldBombCapacity = CUR_CAPACITY(UPG_BOMB_BAG);
        CUR_CAPACITY(UPG_BOMB_BAG) = MAX_BOMBCHUS;
    }
}

RECOMP_HOOK_RETURN("Interface_DrawAmmoCount")
void green_chu_count_return() {
    if (currentI == ITEM_BOMBCHU) {
        CUR_CAPACITY(UPG_BOMB_BAG) = oldBombCapacity;
    }
}