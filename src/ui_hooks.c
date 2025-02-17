#include "modding.h"
#include "global.h"
#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

#include "apcommon.h"

extern u8 sPlayerFormItems[];
extern s16 sEquipState;
extern s16 sEquipMagicArrowSlotHoldTimer;
extern s16 sEquipAnimTimer;

RECOMP_PATCH void KaleidoScope_UpdateItemCursor(PlayState* play) {
    s32 pad1;
    PauseContext* pauseCtx = &play->pauseCtx;
    MessageContext* msgCtx = &play->msgCtx;
    u16 vtxIndex;
    u16 cursorItem;
    u16 cursorSlot;
    u8 magicArrowIndex;
    s16 cursorPoint;
    s16 cursorXIndex;
    s16 cursorYIndex;
    s16 oldCursorPoint;
    s16 moveCursorResult;
    s16 pad2;

    pauseCtx->cursorColorSet = PAUSE_CURSOR_COLOR_SET_WHITE;
    pauseCtx->nameColorSet = PAUSE_NAME_COLOR_SET_WHITE;

    if ((pauseCtx->state == PAUSE_STATE_MAIN) && (pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) &&
        (pauseCtx->pageIndex == PAUSE_ITEM) && !pauseCtx->itemDescriptionOn) {
        moveCursorResult = PAUSE_CURSOR_RESULT_NONE;
        oldCursorPoint = pauseCtx->cursorPoint[PAUSE_ITEM];

        cursorItem = pauseCtx->cursorItem[PAUSE_ITEM];

        // Move cursor left/right
        if (pauseCtx->cursorSpecialPos == 0) {
            // cursor is currently on a slot
            pauseCtx->cursorColorSet = PAUSE_CURSOR_COLOR_SET_YELLOW;

            if (ABS_ALT(pauseCtx->stickAdjX) > 30) {
                cursorPoint = pauseCtx->cursorPoint[PAUSE_ITEM];
                cursorXIndex = pauseCtx->cursorXIndex[PAUSE_ITEM];
                cursorYIndex = pauseCtx->cursorYIndex[PAUSE_ITEM];

                // Search for slot to move to
                while (moveCursorResult == PAUSE_CURSOR_RESULT_NONE) {
                    if (pauseCtx->stickAdjX < -30) {
                        // move cursor left
                        pauseCtx->cursorShrinkRate = 4.0f;
                        if (pauseCtx->cursorXIndex[PAUSE_ITEM] != 0) {
                            pauseCtx->cursorXIndex[PAUSE_ITEM]--;
                            pauseCtx->cursorPoint[PAUSE_ITEM]--;
                            moveCursorResult = PAUSE_CURSOR_RESULT_SLOT;
                        } else {
                            pauseCtx->cursorXIndex[PAUSE_ITEM] = cursorXIndex;
                            pauseCtx->cursorYIndex[PAUSE_ITEM]++;

                            if (pauseCtx->cursorYIndex[PAUSE_ITEM] >= 4) {
                                pauseCtx->cursorYIndex[PAUSE_ITEM] = 0;
                            }

                            pauseCtx->cursorPoint[PAUSE_ITEM] =
                                pauseCtx->cursorXIndex[PAUSE_ITEM] + (pauseCtx->cursorYIndex[PAUSE_ITEM] * 6);

                            if (pauseCtx->cursorPoint[PAUSE_ITEM] >= ITEM_NUM_SLOTS) {
                                pauseCtx->cursorPoint[PAUSE_ITEM] = pauseCtx->cursorXIndex[PAUSE_ITEM];
                            }

                            if (cursorYIndex == pauseCtx->cursorYIndex[PAUSE_ITEM]) {
                                pauseCtx->cursorXIndex[PAUSE_ITEM] = cursorXIndex;
                                pauseCtx->cursorPoint[PAUSE_ITEM] = cursorPoint;

                                KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_LEFT);

                                moveCursorResult = PAUSE_CURSOR_RESULT_SPECIAL_POS;
                            }
                        }
                    } else if (pauseCtx->stickAdjX > 30) {
                        // move cursor right
                        pauseCtx->cursorShrinkRate = 4.0f;
                        if (pauseCtx->cursorXIndex[PAUSE_ITEM] <= 4) {
                            pauseCtx->cursorXIndex[PAUSE_ITEM]++;
                            pauseCtx->cursorPoint[PAUSE_ITEM]++;
                            moveCursorResult = PAUSE_CURSOR_RESULT_SLOT;
                        } else {
                            pauseCtx->cursorXIndex[PAUSE_ITEM] = cursorXIndex;
                            pauseCtx->cursorYIndex[PAUSE_ITEM]++;

                            if (pauseCtx->cursorYIndex[PAUSE_ITEM] >= 4) {
                                pauseCtx->cursorYIndex[PAUSE_ITEM] = 0;
                            }

                            pauseCtx->cursorPoint[PAUSE_ITEM] =
                                pauseCtx->cursorXIndex[PAUSE_ITEM] + (pauseCtx->cursorYIndex[PAUSE_ITEM] * 6);

                            if (pauseCtx->cursorPoint[PAUSE_ITEM] >= ITEM_NUM_SLOTS) {
                                pauseCtx->cursorPoint[PAUSE_ITEM] = pauseCtx->cursorXIndex[PAUSE_ITEM];
                            }

                            if (cursorYIndex == pauseCtx->cursorYIndex[PAUSE_ITEM]) {
                                pauseCtx->cursorXIndex[PAUSE_ITEM] = cursorXIndex;
                                pauseCtx->cursorPoint[PAUSE_ITEM] = cursorPoint;

                                KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_RIGHT);

                                moveCursorResult = PAUSE_CURSOR_RESULT_SPECIAL_POS;
                            }
                        }
                    }
                }

                if (moveCursorResult == PAUSE_CURSOR_RESULT_SLOT) {
                    cursorItem = gSaveContext.save.saveInfo.inventory.items[pauseCtx->cursorPoint[PAUSE_ITEM]];
                }
            }
        } else if (pauseCtx->cursorSpecialPos == PAUSE_CURSOR_PAGE_LEFT) {
            if (pauseCtx->stickAdjX > 30) {
                KaleidoScope_MoveCursorFromSpecialPos(play);
                cursorYIndex = 0;
                cursorXIndex = 0;
                cursorPoint = 0; // top row, left column (SLOT_OCARINA)

                // Search for slot to move to
                while (true) {
                    // Check if current cursor has an item in its slot
                    if (gSaveContext.save.saveInfo.inventory.items[cursorPoint] != ITEM_NONE) {
                        pauseCtx->cursorPoint[PAUSE_ITEM] = cursorPoint;
                        pauseCtx->cursorXIndex[PAUSE_ITEM] = cursorXIndex;
                        pauseCtx->cursorYIndex[PAUSE_ITEM] = cursorYIndex;
                        moveCursorResult = PAUSE_CURSOR_RESULT_SLOT;
                        break;
                    }

                    // move 1 row down and retry
                    cursorYIndex++;
                    cursorPoint += 6;
                    if (cursorYIndex < 4) {
                        continue;
                    }

                    // move 1 column right and retry
                    cursorYIndex = 0;
                    cursorPoint = cursorXIndex + 1;
                    cursorXIndex = cursorPoint;
                    if (cursorXIndex < 6) {
                        continue;
                    }

                    // No item available
                    KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_RIGHT);
                    break;
                }
            }
        } else { // PAUSE_CURSOR_PAGE_RIGHT
            if (pauseCtx->stickAdjX < -30) {
                KaleidoScope_MoveCursorFromSpecialPos(play);
                cursorXIndex = 5;
                cursorPoint = 5; // top row, right columne (SLOT_TRADE_DEED)
                cursorYIndex = 0;

                // Search for slot to move to
                while (true) {
                    // Check if current cursor has an item in its slot
                    if (gSaveContext.save.saveInfo.inventory.items[cursorPoint] != ITEM_NONE) {
                        pauseCtx->cursorPoint[PAUSE_ITEM] = cursorPoint;
                        pauseCtx->cursorXIndex[PAUSE_ITEM] = cursorXIndex;
                        pauseCtx->cursorYIndex[PAUSE_ITEM] = cursorYIndex;
                        moveCursorResult = PAUSE_CURSOR_RESULT_SLOT;
                        break;
                    }

                    // move 1 row down and retry
                    cursorYIndex++;
                    cursorPoint += 6;
                    if (cursorYIndex < 4) {
                        continue;
                    }

                    // move 1 column left and retry
                    cursorYIndex = 0;
                    cursorPoint = cursorXIndex - 1;
                    cursorXIndex = cursorPoint;
                    if (cursorXIndex >= 0) {
                        continue;
                    }

                    // No item available
                    KaleidoScope_MoveCursorToSpecialPos(play, PAUSE_CURSOR_PAGE_LEFT);
                    break;
                }
            }
        }

        if (pauseCtx->cursorSpecialPos == 0) {
            // move cursor up/down
            if (ABS_ALT(pauseCtx->stickAdjY) > 30) {
                moveCursorResult = PAUSE_CURSOR_RESULT_NONE;

                cursorPoint = pauseCtx->cursorPoint[PAUSE_ITEM];
                cursorYIndex = pauseCtx->cursorYIndex[PAUSE_ITEM];

                while (moveCursorResult == PAUSE_CURSOR_RESULT_NONE) {
                    if (pauseCtx->stickAdjY > 30) {
                        // move cursor up
                        moveCursorResult = PAUSE_CURSOR_RESULT_SPECIAL_POS;
                        if (pauseCtx->cursorYIndex[PAUSE_ITEM] != 0) {
                            pauseCtx->cursorYIndex[PAUSE_ITEM]--;
                            pauseCtx->cursorShrinkRate = 4.0f;
                            pauseCtx->cursorPoint[PAUSE_ITEM] -= 6;
                            moveCursorResult = PAUSE_CURSOR_RESULT_SLOT;
                        } else {
                            pauseCtx->cursorYIndex[PAUSE_ITEM] = cursorYIndex;
                            pauseCtx->cursorPoint[PAUSE_ITEM] = cursorPoint;
                        }
                    } else if (pauseCtx->stickAdjY < -30) {
                        // move cursor down
                        moveCursorResult = PAUSE_CURSOR_RESULT_SPECIAL_POS;
                        if (pauseCtx->cursorYIndex[PAUSE_ITEM] < 3) {
                            pauseCtx->cursorYIndex[PAUSE_ITEM]++;
                            pauseCtx->cursorShrinkRate = 4.0f;
                            pauseCtx->cursorPoint[PAUSE_ITEM] += 6;
                            moveCursorResult = PAUSE_CURSOR_RESULT_SLOT;
                        } else {
                            pauseCtx->cursorYIndex[PAUSE_ITEM] = cursorYIndex;
                            pauseCtx->cursorPoint[PAUSE_ITEM] = cursorPoint;
                        }
                    }
                }
            }

            cursorSlot = pauseCtx->cursorPoint[PAUSE_ITEM];
            pauseCtx->cursorColorSet = PAUSE_CURSOR_COLOR_SET_YELLOW;

            if (moveCursorResult == PAUSE_CURSOR_RESULT_SLOT) {
                cursorItem = gSaveContext.save.saveInfo.inventory.items[pauseCtx->cursorPoint[PAUSE_ITEM]];
            } else if (moveCursorResult != PAUSE_CURSOR_RESULT_SPECIAL_POS) {
                cursorItem = gSaveContext.save.saveInfo.inventory.items[pauseCtx->cursorPoint[PAUSE_ITEM]];
            }

            if (cursorItem == ITEM_NONE) {
                cursorItem = PAUSE_ITEM_NONE;
                pauseCtx->cursorColorSet = PAUSE_CURSOR_COLOR_SET_WHITE;
            }

            if ((cursorItem != (u32)PAUSE_ITEM_NONE) && (msgCtx->msgLength == 0)) {
                if (gSaveContext.buttonStatus[EQUIP_SLOT_A] == BTN_DISABLED) {
                    gSaveContext.buttonStatus[EQUIP_SLOT_A] = BTN_ENABLED;
                    gSaveContext.hudVisibility = HUD_VISIBILITY_IDLE;
                    Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
                }
            } else if (gSaveContext.buttonStatus[EQUIP_SLOT_A] != BTN_DISABLED) {
                gSaveContext.buttonStatus[EQUIP_SLOT_A] = BTN_DISABLED;
                gSaveContext.hudVisibility = HUD_VISIBILITY_IDLE;
                Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
            }

            pauseCtx->cursorItem[PAUSE_ITEM] = cursorItem;
            pauseCtx->cursorSlot[PAUSE_ITEM] = cursorSlot;

            if ((pauseCtx->debugEditor == DEBUG_EDITOR_NONE) && (pauseCtx->state == PAUSE_STATE_MAIN) &&
                (pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) &&
                CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_L)) {
                int i;
                int first_i;
                if (pauseCtx->cursorSlot[PAUSE_ITEM] == SLOT(ITEM_MOONS_TEAR)) {
                    if (INV_CONTENT(ITEM_MOONS_TEAR) == ITEM_NONE) {
                        u32 i;
                        for (i = GI_MOONS_TEAR; i <= GI_DEED_OCEAN; ++i) {
                            if (rando_has_item(i)) {
                                INV_CONTENT(ITEM_MOONS_TEAR) = i - 0x6E;
                                Audio_PlaySfx(NA_SE_SY_CURSOR);
                            }
                        }
                    } else {
                        first_i = i = INV_CONTENT(ITEM_MOONS_TEAR) + 0x6E;  // convert to GI
                        // cycle through the player's moon's tear slot items
                        while (true) {
                            ++i;
                            if (i == (GI_DEED_OCEAN + 1)) {
                                i = GI_MOONS_TEAR;
                            }
                            if (i == first_i) {
                                // we've done a full cycle,
                                // the player has no other items
                                break;
                            }
                            if (rando_has_item(i)) {
                                // the player has it, give it to them
                                INV_CONTENT(ITEM_MOONS_TEAR) = i - 0x6E;
                                Audio_PlaySfx(NA_SE_SY_CURSOR);
                                break;
                            }
                        }
                    }
                } else if (pauseCtx->cursorSlot[PAUSE_ITEM] == SLOT(ITEM_ROOM_KEY)) {
                    if (INV_CONTENT(ITEM_ROOM_KEY) == ITEM_NONE) {
                        u32 i;
                        for (i = GI_ROOM_KEY; i <= GI_LETTER_TO_MAMA; ++i) {
                            if (rando_has_item(i)) {
                                INV_CONTENT(ITEM_ROOM_KEY) = i - 0x73;
                                Audio_PlaySfx(NA_SE_SY_CURSOR);
                            }
                        }
                    } else {
                        first_i = i = INV_CONTENT(ITEM_ROOM_KEY) + 0x73;  // convert to GI
                        // cycle through the player's moon's tear slot items
                        while (true) {
                            ++i;
                            if (i == (GI_LETTER_TO_MAMA + 1)) {
                                i = GI_ROOM_KEY;
                            }
                            if (i == first_i) {
                                // we've done a full cycle,
                                // the player has no other items
                                break;
                            }
                            if (rando_has_item(i)) {
                                // the player has it, give it to them
                                INV_CONTENT(ITEM_ROOM_KEY) = i - 0x73;
                                Audio_PlaySfx(NA_SE_SY_CURSOR);
                                break;
                            }
                        }
                    }
                } else if (pauseCtx->cursorSlot[PAUSE_ITEM] == SLOT(ITEM_LETTER_TO_KAFEI)) {
                    if (INV_CONTENT(ITEM_LETTER_TO_KAFEI) == ITEM_NONE) {
                        u32 i;
                        for (i = GI_LETTER_TO_KAFEI; i <= GI_PENDANT_OF_MEMORIES; ++i) {
                            if (rando_has_item(i)) {
                                INV_CONTENT(ITEM_LETTER_TO_KAFEI) = i - 0x7B;
                                Audio_PlaySfx(NA_SE_SY_CURSOR);
                            }
                        }
                    } else {
                        first_i = i = INV_CONTENT(ITEM_LETTER_TO_KAFEI) + 0x7B;  // convert to GI
                        // cycle through the player's moon's tear slot items
                        while (true) {
                            ++i;
                            if (i == (GI_PENDANT_OF_MEMORIES + 1)) {
                                i = GI_LETTER_TO_KAFEI;
                            }
                            if (i == first_i) {
                                // we've done a full cycle,
                                // the player has no other items
                                break;
                            }
                            if (rando_has_item(i)) {
                                // the player has it, give it to them
                                INV_CONTENT(ITEM_LETTER_TO_KAFEI) = i - 0x7B;
                                Audio_PlaySfx(NA_SE_SY_CURSOR);
                                break;
                            }
                        }
                    }
                }
            }

            if (cursorItem != PAUSE_ITEM_NONE) {
                // Equip item to the C buttons
                if ((pauseCtx->debugEditor == DEBUG_EDITOR_NONE) && !pauseCtx->itemDescriptionOn &&
                    (pauseCtx->state == PAUSE_STATE_MAIN) && (pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) &&
                    CHECK_BTN_ANY(CONTROLLER1(&play->state)->press.button, BTN_CLEFT | BTN_CDOWN | BTN_CRIGHT)) {

                    // Ensure that a transformation mask can not be unequipped while being used
                    if (GET_PLAYER_FORM != PLAYER_FORM_HUMAN) {
                        if (1) {}
                        if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_CLEFT)) {
                            if (sPlayerFormItems[GET_PLAYER_FORM] == BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_C_LEFT)) {
                                Audio_PlaySfx(NA_SE_SY_ERROR);
                                return;
                            }
                        } else if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_CDOWN)) {
                            if (sPlayerFormItems[GET_PLAYER_FORM] == BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_C_DOWN)) {
                                Audio_PlaySfx(NA_SE_SY_ERROR);
                                return;
                            }
                        } else if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_CRIGHT)) {
                            if (sPlayerFormItems[GET_PLAYER_FORM] == BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_C_RIGHT)) {
                                Audio_PlaySfx(NA_SE_SY_ERROR);
                                return;
                            }
                        }
                    }

                    // Ensure that a non-transformation mask can not be unequipped while being used
                    if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_CLEFT)) {
                        if ((Player_GetCurMaskItemId(play) != ITEM_NONE) &&
                            (Player_GetCurMaskItemId(play) == BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_C_LEFT))) {
                            Audio_PlaySfx(NA_SE_SY_ERROR);
                            return;
                        }
                        pauseCtx->equipTargetCBtn = PAUSE_EQUIP_C_LEFT;
                    } else if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_CDOWN)) {
                        if ((Player_GetCurMaskItemId(play) != ITEM_NONE) &&
                            (Player_GetCurMaskItemId(play) == BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_C_DOWN))) {
                            Audio_PlaySfx(NA_SE_SY_ERROR);
                            return;
                        }
                        pauseCtx->equipTargetCBtn = PAUSE_EQUIP_C_DOWN;
                    } else if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_CRIGHT)) {
                        if ((Player_GetCurMaskItemId(play) != ITEM_NONE) &&
                            (Player_GetCurMaskItemId(play) == BUTTON_ITEM_EQUIP(0, EQUIP_SLOT_C_RIGHT))) {
                            Audio_PlaySfx(NA_SE_SY_ERROR);
                            return;
                        }
                        pauseCtx->equipTargetCBtn = PAUSE_EQUIP_C_RIGHT;
                    }

                    // Equip item to the C buttons
                    pauseCtx->equipTargetItem = cursorItem;
                    pauseCtx->equipTargetSlot = cursorSlot;
                    pauseCtx->mainState = PAUSE_MAIN_STATE_EQUIP_ITEM;
                    vtxIndex = cursorSlot * 4;
                    pauseCtx->equipAnimX = pauseCtx->itemVtx[vtxIndex].v.ob[0] * 10;
                    pauseCtx->equipAnimY = pauseCtx->itemVtx[vtxIndex].v.ob[1] * 10;
                    pauseCtx->equipAnimAlpha = 255;
                    sEquipMagicArrowSlotHoldTimer = 0;
                    sEquipState = EQUIP_STATE_MOVE_TO_C_BTN;
                    sEquipAnimTimer = 10;

                    if ((pauseCtx->equipTargetItem == ITEM_ARROW_FIRE) ||
                        (pauseCtx->equipTargetItem == ITEM_ARROW_ICE) ||
                        (pauseCtx->equipTargetItem == ITEM_ARROW_LIGHT)) {
                        magicArrowIndex = 0;
                        if (pauseCtx->equipTargetItem == ITEM_ARROW_ICE) {
                            magicArrowIndex = 1;
                        }
                        if (pauseCtx->equipTargetItem == ITEM_ARROW_LIGHT) {
                            magicArrowIndex = 2;
                        }
                        Audio_PlaySfx(NA_SE_SY_SET_FIRE_ARROW + magicArrowIndex);
                        pauseCtx->equipTargetItem = 0xB5 + magicArrowIndex;
                        pauseCtx->equipAnimAlpha = sEquipState = 0; // EQUIP_STATE_MAGIC_ARROW_GROW_ORB
                        sEquipAnimTimer = 6;
                    } else {
                        Audio_PlaySfx(NA_SE_SY_DECIDE);
                    }
                } else if ((pauseCtx->debugEditor == DEBUG_EDITOR_NONE) && (pauseCtx->state == PAUSE_STATE_MAIN) &&
                           (pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) &&
                           CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_A) && (msgCtx->msgLength == 0)) {
                    pauseCtx->itemDescriptionOn = true;
                    if (pauseCtx->cursorYIndex[PAUSE_ITEM] < 2) {
                        func_801514B0(play, 0x1700 + pauseCtx->cursorItem[PAUSE_ITEM], 3);
                    } else {
                        func_801514B0(play, 0x1700 + pauseCtx->cursorItem[PAUSE_ITEM], 1);
                    }
                }
            }
        } else {
            pauseCtx->cursorItem[PAUSE_ITEM] = PAUSE_ITEM_NONE;
        }

        if (oldCursorPoint != pauseCtx->cursorPoint[PAUSE_ITEM]) {
            Audio_PlaySfx(NA_SE_SY_CURSOR);
        }
    } else if ((pauseCtx->mainState == PAUSE_MAIN_STATE_EQUIP_ITEM) && (pauseCtx->pageIndex == PAUSE_ITEM)) {
        pauseCtx->cursorColorSet = PAUSE_CURSOR_COLOR_SET_YELLOW;
    }
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
                   // @ap Draw green 10 Bombchu ammo count.
                   ((item == ITEM_BOMBCHU) && (AMMO(item) == MAX(CUR_CAPACITY(UPG_BOMB_BAG), 10))) ||
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

// @ap Draw green 10 Bombchu ammo count if no bomb bag.
RECOMP_PATCH void Interface_DrawAmmoCount(PlayState* play, s16 button, s16 alpha) {
    static s16 sAmmoDigitsXPositions[] = { 162, 228, 250, 272 };
    static s16 sAmmoDigitsYPositions[] = { 35, 35, 51, 35 };
    u8 i;
    u16 ammo;

    OPEN_DISPS(play->state.gfxCtx);

    i = ((void)0, GET_CUR_FORM_BTN_ITEM(button));

    if ((i == ITEM_DEKU_STICK) || (i == ITEM_DEKU_NUT) || (i == ITEM_BOMB) || (i == ITEM_BOW) ||
        ((i >= ITEM_BOW_FIRE) && (i <= ITEM_BOW_LIGHT)) || (i == ITEM_BOMBCHU) || (i == ITEM_POWDER_KEG) ||
        (i == ITEM_MAGIC_BEANS) || (i == ITEM_PICTOGRAPH_BOX)) {

        if ((i >= ITEM_BOW_FIRE) && (i <= ITEM_BOW_LIGHT)) {
            i = ITEM_BOW;
        }

        ammo = AMMO(i);

        if (i == ITEM_PICTOGRAPH_BOX) {
            if (!CHECK_QUEST_ITEM(QUEST_PICTOGRAPH)) {
                ammo = 0;
            } else {
                ammo = 1;
            }
        }

        gDPPipeSync(OVERLAY_DISP++);
        //! @bug Missing a gDPSetEnvColor here, which means the ammo count will be drawn with the last env color set.
        //! Once you have the magic meter, this becomes a non issue, as the magic meter will set the color to black,
        //! but prior to that, when certain conditions are met, the color will have last been set by the wallet icon
        //! causing the ammo count to be drawn incorrectly. This is most obvious when you get deku nuts early on, and
        //! the ammo count is drawn with a shade of green.

        if ((button == EQUIP_SLOT_B) && (gSaveContext.minigameStatus == MINIGAME_STATUS_ACTIVE)) {
            ammo = play->interfaceCtx.minigameAmmo;
        } else if ((button == EQUIP_SLOT_B) && (play->bButtonAmmoPlusOne > 1)) {
            ammo = play->bButtonAmmoPlusOne - 1;
        } else if (((i == ITEM_BOW) && (AMMO(i) == CUR_CAPACITY(UPG_QUIVER))) ||
                   ((i == ITEM_BOMB) && (AMMO(i) == CUR_CAPACITY(UPG_BOMB_BAG))) ||
                   ((i == ITEM_DEKU_STICK) && (AMMO(i) == CUR_CAPACITY(UPG_DEKU_STICKS))) ||
                   ((i == ITEM_DEKU_NUT) && (AMMO(i) == CUR_CAPACITY(UPG_DEKU_NUTS))) ||
                   // @ap Draw green 10 Bombchu ammo count if no bomb bag.
                   ((i == ITEM_BOMBCHU) && (AMMO(i) == MAX(CUR_CAPACITY(UPG_BOMB_BAG), 10))) ||
                   ((i == ITEM_POWDER_KEG) && (ammo == 1)) || ((i == ITEM_PICTOGRAPH_BOX) && (ammo == 1)) ||
                   ((i == ITEM_MAGIC_BEANS) && (ammo == 20))) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 120, 255, 0, alpha);
        }

        if ((u32)ammo == 0) {
            gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 100, 100, 100, alpha);
        }

        for (i = 0; ammo >= 10; i++) {
            ammo -= 10;
        }

        // Draw upper digit (tens)
        if ((u32)i != 0) {
            OVERLAY_DISP =
                Gfx_DrawTexRectIA8(OVERLAY_DISP, (u8*)gAmmoDigit0Tex + i * AMMO_DIGIT_TEX_SIZE, AMMO_DIGIT_TEX_WIDTH,
                                   AMMO_DIGIT_TEX_HEIGHT, sAmmoDigitsXPositions[button], sAmmoDigitsYPositions[button],
                                   AMMO_DIGIT_TEX_WIDTH, AMMO_DIGIT_TEX_HEIGHT, 1 << 10, 1 << 10);
        }

        // Draw lower digit (ones)
        OVERLAY_DISP =
            Gfx_DrawTexRectIA8(OVERLAY_DISP, (u8*)gAmmoDigit0Tex + ammo * AMMO_DIGIT_TEX_SIZE, AMMO_DIGIT_TEX_WIDTH,
                               AMMO_DIGIT_TEX_HEIGHT, sAmmoDigitsXPositions[button] + 6, sAmmoDigitsYPositions[button],
                               AMMO_DIGIT_TEX_WIDTH, AMMO_DIGIT_TEX_HEIGHT, 1 << 10, 1 << 10);
    }

    CLOSE_DISPS(play->state.gfxCtx);
}