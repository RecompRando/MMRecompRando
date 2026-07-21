#include "modding.h"
#include "global.h"
#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

#include "apcommon.h"

extern u8 sPlayerFormItems[];
extern s16 sEquipState;
extern s16 sEquipMagicArrowSlotHoldTimer;
extern s16 sEquipAnimTimer;

RECOMP_HOOK("KaleidoScope_UpdateItemCursor")
void KaleidoScope_CycleItems(PlayState* play) {
    PauseContext* pauseCtx = &play->pauseCtx;
    MessageContext* msgCtx = &play->msgCtx;

    if ((pauseCtx->state == PAUSE_STATE_MAIN) && (pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) &&
        (pauseCtx->pageIndex == PAUSE_ITEM) && !pauseCtx->itemDescriptionOn) {
        if ((pauseCtx->debugEditor == DEBUG_EDITOR_NONE) && (pauseCtx->state == PAUSE_STATE_MAIN) &&
            (pauseCtx->mainState == PAUSE_MAIN_STATE_IDLE) &&
            (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_L) || CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_A))) {
            // remove the A button press to prevent the item description from showing up
            CONTROLLER1(&play->state)->press.button &= ~BTN_A;
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