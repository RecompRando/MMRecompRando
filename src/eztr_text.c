#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "eztr_api.h"
#include "attributes.h"

#include "apcommon.h"
#include "shops.h"

#include "z64snap.h"

extern GetItemEntryAP sGetItemTable_ap[];

u8 getAPItemColor(u32 type) {
    if (type & 0b001) {
        return 0x05; // EZTR_CC_COLOR_LIGHTBLUE (progression - purple)
    } else if (type & 0b010) {
        return 0x03; // EZTR_CC_COLOR_BLUE (useful - blue)
    } else if (type & 0b100) {
        return 0x08; // EZTR_CC_COLOR_ORANGE (trap - orange)
    } else {
        return 0x07; // EZTR_CC_COLOR_SILVER (filler - grey)
    }
}

u8 getAPLocationItemColor(u32 location) {
    u32 type = rando_get_location_type(location);
    return getAPItemColor(type);
}

void sanitizeRandoText(char* rando_string) {
    u8 c = rando_string[0];
    u8 next = 0;
    u8 i = 0;
    bool shift_string = false;

    while (c != 0) {
        if (c <= 0x08 || (c >= 0x0A && c <= 0x1F) || (c >= 0xB0 && c <= 0xBB) || (c >= 0xBF && c <= 0xE8) || (c >= 0xF0 && c <= 0xFF)) {
            next = rando_string[i+1];
            if (c == 0xC3 && next == 0xA1) { // á
                rando_string[i] = 0x98;
                shift_string = true;
            } else {
                rando_string[i] = 0xAE; // replace all invalid bytes with ¿
            }
        }

        if (shift_string) {
            u8 new_i = i + 1;
            u8 new_c = rando_string[new_i];
            u8 new_next = rando_string[new_i + 1];
            while (new_c != 0) {
                rando_string[new_i] = new_next;
                new_i++;
                new_c = rando_string[new_i];
                new_next = rando_string[new_i + 1];
            }
            shift_string = false;
        }

        i++;
        c = rando_string[i];
    }
}

void numberSuffix(u32 num, char* out_str, size_t str_len) {
    u8 first_digit = num % 10;
    bool ignore = (num % 100) >= 11 && (num % 100) <= 13; // ignore 11st, 12nd, 13rd

    if (!ignore && first_digit == 1) {
        EZTR_MsgSContent_Snprintf(
            out_str,
            str_len,
            "%dst" EZTR_CC_END,
            num
        );
    } else if (!ignore && first_digit == 2) {
        EZTR_MsgSContent_Snprintf(
            out_str,
            str_len,
            "%dnd" EZTR_CC_END,
            num
        );
    } else if (!ignore && first_digit == 3) {
        EZTR_MsgSContent_Snprintf(
            out_str,
            str_len,
            "%drd" EZTR_CC_END,
            num
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            out_str,
            str_len,
            "%dth" EZTR_CC_END,
            num
        );
    }
}

RECOMP_HOOK_RETURN("Message_OpenText")
void Post_Message_OpenText() {
    PlayState* play = gPlay;
    MessageContext* msgCtx = &play->msgCtx;

    // QoL: let us skip all text
    msgCtx->textUnskippable = false;

    // below is if we specifically need to unset skippable text from the original function
    // if ((msgCtx->unk11F0C == 1) || (msgCtx->unk11F0C == 3)) {
    //     msgCtx->textUnskippable = false;
    // }
}

// u16 savedPress;

// RECOMP_HOOK("Message_Update")
// void skip_credits(PlayState* play) {
//     MessageContext* msgCtx = &play->msgCtx;
//     Input* controller = CONTROLLER1(&play->state);

//     savedPress = controller->press.button;
//     controller->press.button |= BTN_A;
// }

// RECOMP_HOOK_RETURN("Message_Update")
// void fix_inputs() {
//     PlayState* play = gPlay;
//     MessageContext* msgCtx = &play->msgCtx;
//     Input* controller = CONTROLLER1(&play->state);
//     controller->press.button = savedPress;
// }

EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Send_Item); // "You sent [player] their [item]"
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Self_Item); // "You found your [item]"
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Kokiri_Sword);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Bombchu_Bag);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Magic);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SpinAttack);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_DoubleDefense);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Time); // Songs
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Healing);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Epona);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Soaring);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Storms);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Sonata);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Lullaby);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Nova);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Elegy);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Oath);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Souls); // collectables
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Frogs);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_CTSF); // stray fairies
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_WFSF);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SHSF);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_GBSF);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_STSF);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_WFBK); // boss keys
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SHBK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_GBBK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_STBK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_WFSK); // small keys
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SHSK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_GBSK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_STSK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_WFMap); // maps
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SHMap);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_GBMap);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_STMap);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_WFCompass); // compasses
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SHCompass);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_GBCompass);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_STCompass);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Swamp_Token); // skull tokens
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Ocean_Token);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_OwlStatue); // Owl Statues
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Owl_Hidden);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_FOOL);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Moon_Child_Return);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Tingle); // Shops
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Shop);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Shop_Buying);

// Get info from Rando to replace the text with item class colour, player name and item name
EZTR_MSG_CALLBACK(randoAPSend) {
    u32 location = rando_get_last_location_sent();
    char* player_name;
    char* item_name;
    
    rando_get_location_item_player(location, &player_name);
    rando_get_location_item_name(location, &item_name);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You found " EZTR_CC_COLOR_RED "%s" EZTR_CC_COLOR_DEFAULT "'s" EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        player_name,
        getAPLocationItemColor(location),
        item_name
    );
    
    recomp_free(player_name);
    recomp_free(item_name);
}

EZTR_MSG_CALLBACK(randoAPSelf) {
    u32 location = rando_get_last_location_sent();
    char* item_name;

    rando_get_location_item_name(location, &item_name);
    sanitizeRandoText(item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You found your" EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        getAPLocationItemColor(location),
        item_name
    );
    
    recomp_free(item_name);
}

EZTR_MSG_CALLBACK(randoGIOwlStatue) {
    u32 location = rando_get_last_location_sent();
    char* item_name;

    rando_get_location_item_name(location, &item_name);
    sanitizeRandoText(item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You can now soar to the" EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        getAPLocationItemColor(location),
        item_name
    );
    
    recomp_free(item_name);
}

EZTR_MSG_CALLBACK(randoGISongs) {
    u32 location = rando_get_last_location_sent();
    char* item_name;

    rando_get_location_item_name(location, &item_name);
    sanitizeRandoText(item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You learned the " EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        getAPLocationItemColor(location),
        item_name
    );
    
    recomp_free(item_name);
}

EZTR_MSG_CALLBACK(randoGIMagic) {
    u8 magic_count = rando_has_item(AP_ITEM_ID_MAGIC);
    if (magic_count < 2) {
        EZTR_MsgSContent_Sprintf(
            buf->data.content,
            "You got " EZTR_CC_COLOR_RED "Magic Power" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
            NULL
        );
    } else if (magic_count == 2) {
        EZTR_MsgSContent_Sprintf(
            buf->data.content,
            "Your " EZTR_CC_COLOR_RED "Magic Power" EZTR_CC_COLOR_DEFAULT " has been " EZTR_CC_COLOR_RED "doubled" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
            NULL
        );
    }
}

EZTR_MSG_CALLBACK(randoGISouls) {
    u32 location = rando_get_last_location_sent();
    char* item_name;

    rando_get_location_item_name(location, &item_name);
    sanitizeRandoText(item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You found the" EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        getAPLocationItemColor(location),
        item_name
    );
    
    recomp_free(item_name);
}

EZTR_MSG_CALLBACK(randoGIFrogs) {
    u32 location = rando_get_last_location_sent();
    char* item_name;

    rando_get_location_item_name(location, &item_name);
    sanitizeRandoText(item_name);
    
    // TODO: set different colors for each frog
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You found the %c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        getAPLocationItemColor(location),
        item_name
    );
    
    recomp_free(item_name);
}

extern s16 currentTingle;
EZTR_MSG_CALLBACK(randoTingle) {
    u32 tingleFirstItem = GI_TINGLE_MAP_CLOCK_TOWN;
    char* player_name;
    char* item_name;
    u16 price1 = 5;
    
    u32 tingleSecondItem = GI_TINGLE_MAP_WOODFALL;
    char* player_name2;
    char* item_name2;
    u16 price2 = 40;

    switch (currentTingle) {
        case TINGLE_MAP_CLOCK_TOWN:
            tingleFirstItem = GI_TINGLE_MAP_CLOCK_TOWN;
            tingleSecondItem = GI_TINGLE_MAP_WOODFALL;
            price1 = 5;
            price2 = 40;
            break;

        case TINGLE_MAP_WOODFALL:
            tingleFirstItem = GI_TINGLE_MAP_WOODFALL;
            tingleSecondItem = GI_TINGLE_MAP_SNOWHEAD;
            price1 = 20;
            price2 = 40;
            break;

        case TINGLE_MAP_SNOWHEAD:
            tingleFirstItem = GI_TINGLE_MAP_SNOWHEAD;
            tingleSecondItem = GI_TINGLE_MAP_ROMANI_RANCH;
            price1 = 20;
            price2 = 40;
            break;

        case TINGLE_MAP_ROMANI_RANCH:
            tingleFirstItem = GI_TINGLE_MAP_ROMANI_RANCH;
            tingleSecondItem = GI_TINGLE_MAP_GREAT_BAY;
            price1 = 20;
            price2 = 40;
            break;

        case TINGLE_MAP_GREAT_BAY:
            tingleFirstItem = GI_TINGLE_MAP_GREAT_BAY;
            tingleSecondItem = GI_TINGLE_MAP_STONE_TOWER;
            price1 = 20;
            price2 = 40;
            break;

        case TINGLE_MAP_STONE_TOWER:
            tingleFirstItem = GI_TINGLE_MAP_STONE_TOWER;
            tingleSecondItem = GI_TINGLE_MAP_CLOCK_TOWN;
            price1 = 20;
            price2 = 40;
            break;
    }

    rando_get_location_item_player(tingleFirstItem, &player_name);
    rando_get_location_item_name(tingleFirstItem, &item_name);
    rando_get_location_item_player(tingleSecondItem, &player_name2);
    rando_get_location_item_name(tingleSecondItem, &item_name2);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);
    sanitizeRandoText(item_name2);
    sanitizeRandoText(player_name2);

    char formatted_player_name1[128];
    if (!rando_get_location_has_local_item(tingleFirstItem) && (!rando_location_is_checked(tingleFirstItem))) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name1,
            128,
            " (" EZTR_CC_COLOR_LIGHTBLUE "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name1,
            128,
            EZTR_CC_END
        );
    }

    char formatted_player_name2[128];
    if (!rando_get_location_has_local_item(tingleSecondItem) && (!rando_location_is_checked(tingleSecondItem))) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name2,
            128,
            " (" EZTR_CC_COLOR_LIGHTBLUE "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name2
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name2,
            128,
            EZTR_CC_END
        );
    }

    char formatted_item_name1[128];
    if (!rando_location_is_checked(tingleFirstItem)) {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name1,
            128,
            "%s" EZTR_CC_END,
            item_name
        );        
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name1,
            128,
            EZTR_CC_COLOR_SILVER "SOLD OUT" EZTR_CC_END
        );
    }

    char formatted_item_name2[128];
    if (!rando_location_is_checked(tingleSecondItem)) {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name2,
            128,
            "%s" EZTR_CC_END,
            item_name2
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name2,
            128,
            EZTR_CC_COLOR_SILVER "SOLD OUT" EZTR_CC_END
        );
    }

    // hide rupee count when "SOLD OUT" text appears
    char cost_str1[128];
    if (!rando_location_is_checked(tingleFirstItem)) {
        EZTR_MsgSContent_Snprintf(
            cost_str1,
            128,
            " %d Rupees" EZTR_CC_END,
            price1
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            cost_str1,
            128,
            EZTR_CC_END
        );
    }

    char cost_str2[128];
    if (!rando_location_is_checked(tingleSecondItem)) {
        EZTR_MsgSContent_Snprintf(
            cost_str2,
            128,
            " %d Rupees" EZTR_CC_END,
            price2
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            cost_str2,
            128,
            EZTR_CC_END
        );
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        EZTR_CC_THREE_CHOICE EZTR_CC_COLOR_GREEN "%m" EZTR_CC_COLOR_RED "%m" EZTR_CC_COLOR_DEFAULT "%m" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_GREEN "%m" EZTR_CC_COLOR_RED "%m" EZTR_CC_COLOR_DEFAULT "%m" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_GREEN "No thanks" EZTR_CC_END,
        formatted_item_name1,
        cost_str1,
        formatted_player_name1,
        formatted_item_name2,
        cost_str2,
        formatted_player_name2
    );
    
    recomp_free(item_name);
    recomp_free(item_name2);
    recomp_free(player_name);
    recomp_free(player_name2);
}

extern s16 shopItemId;
EZTR_MSG_CALLBACK(randoShop) {
    u32 shop_location = LOCATION_SHOP_ITEM_ID(shopItemId);
    s32 price = rando_get_shop_price(shopItemId);

    EZTR_MsgBuffer_SetFirstItemRupees(buf, price);

    char* item_name;
    char* player_name;
    rando_get_location_item_name(shop_location, &item_name);
    rando_get_location_item_player(shop_location, &player_name);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);

    char* classificationText;
    u32 type = rando_get_location_type(shop_location);
    if (type & 0b001) {
        classificationText = EZTR_CC_COLOR_LIGHTBLUE "Progression Item" EZTR_CC_COLOR_DEFAULT EZTR_CC_END;
    } else if (type & 0b010) {
        classificationText = EZTR_CC_COLOR_BLUE "Useful Item" EZTR_CC_COLOR_DEFAULT EZTR_CC_END;
    } else if (type & 0b100) {
        classificationText = EZTR_CC_COLOR_ORANGE "Trap" EZTR_CC_COLOR_DEFAULT EZTR_CC_END;
    } else {
        classificationText = EZTR_CC_COLOR_SILVER "Filler Item" EZTR_CC_COLOR_DEFAULT EZTR_CC_END;
    }

    char forText[128];
    if (!rando_get_location_has_local_item(shop_location)) {
        EZTR_MsgSContent_Snprintf(
            forText,
            128,
            " for" EZTR_CC_NEWLINE
            EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            forText,
            128,
            EZTR_CC_END
        );
    }

    char* extraText;
    if (shopItemId == SI_POTION_BLUE) { // TODO: also check if the mushroom was already given
        extraText = EZTR_CC_NEWLINE EZTR_CC_COLOR_LIGHTBLUE "Requires a Magic Mushroom" EZTR_CC_END;
    } else {
        extraText = EZTR_CC_END;
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        EZTR_CC_COLOR_RED "%s: %d Rupees" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_DEFAULT "This is a %m%m%m" EZTR_CC_PERSISTENT EZTR_CC_END,
        item_name,
        price,
        classificationText,
        forText,
        extraText
    );

    recomp_free(item_name);
    recomp_free(player_name);
}

EZTR_MSG_CALLBACK(randoShopBuy) {
    u32 shop_location = LOCATION_SHOP_ITEM_ID(shopItemId);
    s32 price = rando_get_shop_price(shopItemId);
    
    EZTR_MsgBuffer_SetFirstItemRupees(buf, price);

    char* item_name;
    rando_get_location_item_name(shop_location, &item_name);
    sanitizeRandoText(item_name);

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "%s: %d Rupees" EZTR_CC_NEWLINE
        EZTR_CC_NEWLINE
        EZTR_CC_COLOR_GREEN EZTR_CC_TWO_CHOICE
        "I'll buy it" EZTR_CC_NEWLINE
        "No thanks" EZTR_CC_PERSISTENT EZTR_CC_END,
        item_name,
        price
    );

    recomp_free(item_name);
}

#define LOCATION_SCRUB_SHOP(item) (0x090100 | item)

extern s16 currentScrub;
EZTR_MSG_CALLBACK(randoScrub) {
    u32 scrubLocation = LOCATION_SCRUB_SHOP(GI_MAGIC_BEANS);
    if (!rando_get_slotdata_u32("scrubsanity") || rando_location_is_checked(scrubLocation)) {
        return;
    }

    char* player_name;
    char* item_name;
    u16 price = 5;
    
    switch (currentScrub) {
        case 0:
            scrubLocation = LOCATION_SCRUB_SHOP(GI_MAGIC_BEANS);
            price = 10;
            break;
        case 1:
            scrubLocation = LOCATION_SCRUB_SHOP(GI_BOMB_BAG_40);
            price = 200;
            break;
        case 2:
            scrubLocation = LOCATION_SCRUB_SHOP(GI_POTION_GREEN);
            price = 40;
            break;
        case 3:
            scrubLocation = LOCATION_SCRUB_SHOP(GI_POTION_BLUE);
            price = 100;
            break;
    }

    rando_get_location_item_player(scrubLocation, &player_name);
    rando_get_location_item_name(scrubLocation, &item_name);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);

    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(scrubLocation) && (!rando_location_is_checked(scrubLocation))) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_NEWLINE "(" EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_END
        );
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "For " EZTR_CC_COLOR_PINK "%d Rupees" EZTR_CC_COLOR_DEFAULT ", I'm selling"
        EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "%m" EZTR_CC_EVENT EZTR_CC_END,
        price,
        getAPLocationItemColor(scrubLocation),
        item_name,
        formatted_player_name
    );

    recomp_free(item_name);
    recomp_free(player_name);
}
EZTR_MSG_CALLBACK(randoScrubNotSelling) {
    u32 scrubLocation = LOCATION_SCRUB_SHOP(GI_MAGIC_BEANS);
    if (!rando_get_slotdata_u32("scrubsanity") || rando_location_is_checked(scrubLocation)) {
        return;
    }

    char* player_name;
    char* item_name;
    u16 price = 5;
    u16 requiredForm = 1;
    switch (currentScrub) {
        case 0:
            scrubLocation = LOCATION_SCRUB_SHOP(GI_MAGIC_BEANS);
            price = 10;
            requiredForm = 1;
            break;
        case 1:
            scrubLocation = LOCATION_SCRUB_SHOP(GI_BOMB_BAG_40);
            price = 200;
            requiredForm = 2;
            break;
        case 2:
            scrubLocation = LOCATION_SCRUB_SHOP(GI_POTION_GREEN);
            price = 40;
            requiredForm = 3;
            break;
        case 3:
            scrubLocation = LOCATION_SCRUB_SHOP(GI_POTION_BLUE);
            price = 100;
            requiredForm = 4;
            break;
    }

    rando_get_location_item_player(scrubLocation, &player_name);
    rando_get_location_item_name(scrubLocation, &item_name);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);

    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(scrubLocation) && (!rando_location_is_checked(scrubLocation))) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_NEWLINE "(" EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_NEWLINE EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_NEWLINE EZTR_CC_END
        );
    }
    char extra_text[256];
    if (requiredForm == 1) {
        EZTR_MsgSContent_Snprintf(
            extra_text,
            128,
            EZTR_CC_BOX_BREAK2 "But only to Deku Scrubs. I'd really" EZTR_CC_NEWLINE
            "like to leave my hometown. I'm" EZTR_CC_NEWLINE
            "hoping to find some success in" EZTR_CC_NEWLINE
            "a livelier place!" EZTR_CC_END,
            NULL
            );
    } else if (requiredForm == 2){
        EZTR_MsgSContent_Snprintf(
            extra_text,
            128,
            EZTR_CC_BOX_BREAK2 "But only to Gorons." EZTR_CC_NEWLINE
            "What I'd really like to do is go" EZTR_CC_NEWLINE 
            "back home and do business where" EZTR_CC_NEWLINE
            "I'm surrounded by the trees and grass." EZTR_CC_END,
            NULL
            );
    } else if (requiredForm == 3) {
        EZTR_MsgSContent_Snprintf(
            extra_text,
            128,
            EZTR_CC_BOX_BREAK2 "But only to Zora." EZTR_CC_NEWLINE
            "Actually, I'd like to do business" EZTR_CC_NEWLINE 
            "someplace where it's cooler and" EZTR_CC_NEWLINE
            "the air is clean." EZTR_CC_END,
            NULL
            );
    } else if (requiredForm == 4) {
        EZTR_MsgSContent_Snprintf(
            extra_text,
            128,
            EZTR_CC_BOX_BREAK2 "Actually, I want to do business in" EZTR_CC_NEWLINE 
            "the sea breeze while listening to" EZTR_CC_NEWLINE
            "the sound of the waves." EZTR_CC_END,
            NULL
            );
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "For " EZTR_CC_COLOR_PINK "%d Rupees" EZTR_CC_COLOR_DEFAULT ", I'm selling"
        EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "%m%m" EZTR_CC_EVENT EZTR_CC_END,
        price,
        getAPLocationItemColor(scrubLocation),
        item_name,
        formatted_player_name,
        extra_text
    );

    recomp_free(item_name);
    recomp_free(player_name);
}

#define LOCATION_MILK (0x263 << 8 | GI_MILK) // (ACTOR_ID_BARTEN << 8 | GI_MILK)
#define MILK_BAR_LOCATION_CHATEAU GI_CHATEAU

EZTR_MSG_CALLBACK(randoMilkBar) {
    u32 milkBarFirstItem = LOCATION_MILK; // Regular Milk Purchase check ID
    char* player_name;
    char* item_name;
    u16 price1 = 20;
    
    u32 milkBarSecondItem = MILK_BAR_LOCATION_CHATEAU;
    char* player_name2;
    char* item_name2;
    u16 price2 = 200;

    rando_get_location_item_player(milkBarFirstItem, &player_name);
    rando_get_location_item_name(milkBarFirstItem, &item_name);
    rando_get_location_item_player(milkBarSecondItem, &player_name2);
    rando_get_location_item_name(milkBarSecondItem, &item_name2);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);
    sanitizeRandoText(item_name2);
    sanitizeRandoText(player_name2);

    char formatted_player_name1[128];
    if (!rando_get_location_has_local_item(milkBarFirstItem) && (!rando_location_is_checked(milkBarFirstItem))) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name1,
            128,
            " (" EZTR_CC_COLOR_LIGHTBLUE "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name1,
            128,
            EZTR_CC_END
        );
    }

    char formatted_player_name2[128];
    if (!rando_get_location_has_local_item(milkBarSecondItem) && (!rando_location_is_checked(milkBarSecondItem))) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name2,
            128,
            " (" EZTR_CC_COLOR_LIGHTBLUE "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name2
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name2,
            128,
            EZTR_CC_END
        );
    }

    char formatted_item_name1[128];
    if (!rando_location_is_checked(milkBarFirstItem)) {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name1,
            128,
            "%s" EZTR_CC_END,
            item_name
        );        
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name1,
            128,
            EZTR_CC_COLOR_GREEN "Regular Milk:" EZTR_CC_END
        );
    }

    char formatted_item_name2[128];
    if (!rando_location_is_checked(milkBarSecondItem)) {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name2,
            128,
            "%s" EZTR_CC_END,
            item_name2
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name2,
            128,
            EZTR_CC_COLOR_GREEN "Chateau:" EZTR_CC_END
        );
    }

    char rupee_cost1[128];
    if (!rando_location_is_checked(milkBarFirstItem)) {
        EZTR_MsgSContent_Snprintf(
            rupee_cost1,
            128,
            " %d Rupees" EZTR_CC_END,
            price1
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            rupee_cost1,
            128,
            " 20 Rupees" EZTR_CC_END
        );
    }

    char rupee_cost2[128];
    if (!rando_location_is_checked(milkBarSecondItem)) {
        EZTR_MsgSContent_Snprintf(
            rupee_cost2,
            128,
            " %d Rupees" EZTR_CC_END,
            price2
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            rupee_cost2,
            128,
            " 200 Rupees" EZTR_CC_END
        );
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "What'll it be?" 
        EZTR_CC_CARRIAGE_RETURN EZTR_CC_CARRIAGE_RETURN EZTR_CC_NEWLINE 
        EZTR_CC_BOX_BREAK2 EZTR_CC_THREE_CHOICE EZTR_CC_COLOR_GREEN "%m:" EZTR_CC_COLOR_PINK "%m" EZTR_CC_COLOR_DEFAULT "%m" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_GREEN "%m:" EZTR_CC_COLOR_PINK "%m" EZTR_CC_COLOR_DEFAULT "%m" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_GREEN "Nothing" EZTR_CC_END,
        formatted_item_name1,
        rupee_cost1,
        formatted_player_name1,
        formatted_item_name2,
        rupee_cost2,
        formatted_player_name2
    );
    
    recomp_free(item_name);
    recomp_free(item_name2);
    recomp_free(player_name);
    recomp_free(player_name2);
}

u16 savedDungeonIndex;
bool changedDungeonIndex;

// unsure if handling this dungeon change like this is a problem
RECOMP_HOOK_RETURN("Message_CloseTextbox")
void Message_RestoreDungeon() {
    if (changedDungeonIndex) {
        gSaveContext.dungeonIndex = savedDungeonIndex;
        changedDungeonIndex = false;
    }
}

#define LOCATION_GREAT_FAIRY(type) (0x030000 | type)
void strayFairyMsg(EZTR_MsgBuffer* buf, DungeonIndex type, u8 count, u8 required) {
    char* type_str;
    u32 reward_location = LOCATION_GREAT_FAIRY(type + 1);

    savedDungeonIndex = gSaveContext.dungeonIndex;
    gSaveContext.dungeonIndex = type;
    changedDungeonIndex = true;

    switch (type) {
        case DUNGEON_INDEX_WOODFALL_TEMPLE:
            type_str = EZTR_CC_COLOR_PINK "Woodfall" EZTR_CC_END;
            break;
        case DUNGEON_INDEX_SNOWHEAD_TEMPLE:
            type_str = EZTR_CC_COLOR_GREEN "Snowhead" EZTR_CC_END;
            break;
        case DUNGEON_INDEX_GREAT_BAY_TEMPLE:
            type_str = EZTR_CC_COLOR_BLUE "Great Bay" EZTR_CC_END;
            break;
        case DUNGEON_INDEX_STONE_TOWER_TEMPLE:
            type_str = EZTR_CC_COLOR_YELLOW "Stone Tower" EZTR_CC_END;
            break;
    }
    
    char flavor_text[128];
    if (rando_location_is_checked(reward_location)) {
        EZTR_MsgSContent_Snprintf(
            flavor_text,
            128,
            "You have already claimed their reward." EZTR_CC_END
        );
    } else if (count >= required) {
        EZTR_MsgSContent_Snprintf(
            flavor_text,
            128,
            "Head to the " EZTR_CC_COLOR_PINK "Great Fairy " EZTR_CC_NEWLINE
            EZTR_CC_COLOR_DEFAULT "for your reward." EZTR_CC_END
        );
    } else {
        u8 remaining = required - count;
        EZTR_MsgSContent_Snprintf(
            flavor_text,
            128,
            "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END,
            remaining
        );
    }

    char count_str[8];
    numberSuffix(count, count_str, 8);

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You got a %m Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE
        "This is your " EZTR_CC_COLOR_RED "%m" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE
        "%m" EZTR_CC_END,
        type_str,
        count_str,
        flavor_text
    );
}

EZTR_MSG_CALLBACK(WoodfallStrayFairyCount) {
    u8 fairy_count = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_WOODFALL);
    u8 fairy_required = rando_get_slotdata_u32("required_stray_fairies");
    
    strayFairyMsg(buf, DUNGEON_INDEX_WOODFALL_TEMPLE, fairy_count, fairy_required);
}

EZTR_MSG_CALLBACK(SnowheadStrayFairyCount) {
    u8 fairy_count = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_SNOWHEAD);
    u8 fairy_required = rando_get_slotdata_u32("required_stray_fairies");
        
    strayFairyMsg(buf, DUNGEON_INDEX_SNOWHEAD_TEMPLE, fairy_count, fairy_required);
}

EZTR_MSG_CALLBACK(GreatBayStrayFairyCount) {
    u8 fairy_count = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_GREATBAY);
    u8 fairy_required = rando_get_slotdata_u32("required_stray_fairies");
        
    strayFairyMsg(buf, DUNGEON_INDEX_GREAT_BAY_TEMPLE, fairy_count, fairy_required);
}

EZTR_MSG_CALLBACK(StoneTowerStrayFairyCount) {
    u8 fairy_count = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_STONETOWER);
    u8 fairy_required = rando_get_slotdata_u32("required_stray_fairies");
        
    strayFairyMsg(buf, DUNGEON_INDEX_STONE_TOWER_TEMPLE, fairy_count, fairy_required);
}

EZTR_MSG_CALLBACK(SnowheadSmallKeyCount) {
    u8 key_count = rando_has_item(AP_ITEM_ID_KEY_SMALL_SNOWHEAD);
    
    char count_str[8];
    numberSuffix(key_count, count_str, 8);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE
        "This is your " EZTR_CC_COLOR_RED "%m" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END,
        count_str
    );
}

EZTR_MSG_CALLBACK(StoneTowerSmallKeyCount) {
    u8 key_count = rando_has_item(AP_ITEM_ID_KEY_SMALL_STONETOWER);
        
    char count_str[8];
    numberSuffix(key_count, count_str, 8);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE
        "This is your " EZTR_CC_COLOR_RED "%m" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END,
        count_str
    );
}

#define SPIDER_SWAMP 0
#define SPIDER_OCEAN 1
void skullTokenMsg(EZTR_MsgBuffer* buf, u8 type, u8 count, u8 required) {
    char* type_str;
    u32 reward_location;

    switch (type) {
        case SPIDER_SWAMP:
            type_str = EZTR_CC_COLOR_GREEN "Swamp" EZTR_CC_END;
            reward_location = GI_MASK_TRUTH;
            break;
        case SPIDER_OCEAN:
            type_str = EZTR_CC_COLOR_BLUE "Ocean" EZTR_CC_END;
            reward_location = GI_WALLET_GIANT;
            break;
    }
    
    char flavor_text[128];
    if (rando_location_is_checked(reward_location)) {
        EZTR_MsgSContent_Snprintf(
            flavor_text,
            128,
            "You have already claimed your reward." EZTR_CC_END
        );
    } else if (count >= required) {
        EZTR_MsgSContent_Snprintf(
            flavor_text,
            128,
            "Head to the %m Spider House" EZTR_CC_COLOR_DEFAULT EZTR_CC_NEWLINE
            "for your reward!" EZTR_CC_END,
            type_str
        );
    } else {
        u8 remaining = required - count;
        EZTR_MsgSContent_Snprintf(
            flavor_text,
            128,
            "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END,
            remaining
        );
    }

    char count_str[8];
    numberSuffix(count, count_str, 8);

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You got a %m Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE
        "This is your " EZTR_CC_COLOR_RED "%m" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE
        "%m" EZTR_CC_END,
        type_str,
        count_str,
        flavor_text
    );
}

EZTR_MSG_CALLBACK(SwampSpiderTokenCount) {
    u8 token_count = rando_has_item(GI_TRUE_SKULL_TOKEN);
    u8 tokens_required = rando_get_slotdata_u32("required_skull_tokens");
    
    skullTokenMsg(buf, SPIDER_SWAMP, token_count, tokens_required);
}

EZTR_MSG_CALLBACK(OceanSpiderTokenCount) {
    u8 token_count = rando_has_item(GI_OCEAN_SKULL_TOKEN);
    u8 tokens_required = rando_get_slotdata_u32("required_skull_tokens");
    
    skullTokenMsg(buf, SPIDER_OCEAN, token_count, tokens_required);
}

// text replacements for pictograph box
EZTR_MSG_CALLBACK(randoPictograph) {
    // process pictograph image before determining text
    if (!CHECK_QUEST_ITEM(QUEST_PICTOGRAPH)) {
        Snap_RecordPictographedActors(play);
    }

    char* picture_type;

    if (Snap_CheckFlag(PICTO_VALID_MONKEY)) {
        picture_type = "picture of a monkey" EZTR_CC_END;
    } else if (Snap_CheckFlag(PICTO_VALID_BIG_OCTO)) {
        picture_type = "picture of a Big Octo" EZTR_CC_END;
    } else if (Snap_CheckFlag(PICTO_VALID_SCARECROW)) {
        picture_type = "picture of a scarecrow" EZTR_CC_END;
    } else if (Snap_CheckFlag(PICTO_VALID_TINGLE)) {
        picture_type = "picture of Tingle" EZTR_CC_END;
    } else if (Snap_CheckFlag(PICTO_VALID_DEKU_KING)) {
        picture_type = "picture of the Deku King" EZTR_CC_END;
    } else if (Snap_CheckFlag(PICTO_VALID_PIRATE_GOOD)) {
        picture_type = "good picture of a pirate" EZTR_CC_END;
    } else if (Snap_CheckFlag(PICTO_VALID_PIRATE_TOO_FAR)) {
        picture_type = "bad picture of a pirate" EZTR_CC_END;
    } else if (Snap_CheckFlag(PICTO_VALID_LULU_HEAD)) {
        if (Snap_CheckFlag(PICTO_VALID_LULU_RIGHT_ARM) && Snap_CheckFlag(PICTO_VALID_LULU_LEFT_ARM)) {
            picture_type = "good picture of Lulu" EZTR_CC_END;
        } else {
            picture_type = "bad picture of Lulu" EZTR_CC_END;
        }
    } else if (Snap_CheckFlag(PICTO_VALID_IN_SWAMP)) {
        picture_type = "picture of the swamp" EZTR_CC_END;
    } else {
        picture_type = "picture" EZTR_CC_END;
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "Keep this " EZTR_CC_COLOR_RED "%m" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN EZTR_CC_NEWLINE
        EZTR_CC_NEWLINE EZTR_CC_TWO_CHOICE
        "Yes" EZTR_CC_NEWLINE
        "No" EZTR_CC_END,
        picture_type
    );
}

EZTR_MSG_CALLBACK(randoBankHints) {
    u32 bankFirstItem = LOCATION_BANK_200_REWARD; // West Clock Town Bank 200 Rupees
    u32 bankSecondItem = LOCATION_BANK_500_REWARD; // West Clock Town Bank 500 Rupees
    u32 bankThirdItem = LOCATION_BANK_1000_REWARD; // West Clock Town Bank 1000 Rupees

    char* is_bank_important;
    u32 type = rando_get_location_type(bankFirstItem);
    u32 type2 = rando_get_location_type(bankSecondItem);
    u32 type3 = rando_get_location_type(bankThirdItem);
    if (type & 0b001 || type2 & 0b001 || type3 & 0b001) { // at least one is "progressive"
        is_bank_important = "." EZTR_CC_NEWLINE "At least one of them looks " EZTR_CC_COLOR_LIGHTBLUE "important" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END;
    } else if (type & 0b010 || type2 & 0b010 || type3 & 0b010) { // at least one is "useful"
        is_bank_important = "." EZTR_CC_NEWLINE "At least one of them looks " EZTR_CC_COLOR_BLUE "useful" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END;
    } else {
        is_bank_important = EZTR_CC_NEWLINE "that I " EZTR_CC_COLOR_SILVER "wanted to get rid of" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END;
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "For example, if you deposit enough" EZTR_CC_NEWLINE 
        EZTR_CC_COLOR_PINK "Rupees" EZTR_CC_COLOR_DEFAULT ", you'll get up to three items%m" EZTR_CC_EVENT EZTR_CC_END,
        is_bank_important
    );
}

EZTR_MSG_CALLBACK(randoLotterySignHint) {
    u32 lotteryItem = LOCATION_LOTTERY_SHOP;
    char* item_name;
    char* player_name;

    rando_get_location_item_name(lotteryItem, &item_name);
    rando_get_location_item_player(lotteryItem, &player_name);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);

    char formatted_item_name[128];
    if (!rando_location_is_checked(lotteryItem)) {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name,
            128,
            "%s" EZTR_CC_END,
            item_name
        );        
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name,
            128,
            EZTR_CC_COLOR_SILVER "50 Rupees" EZTR_CC_END
        );
    }

    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(lotteryItem) && (!rando_location_is_checked(lotteryItem))) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_NEWLINE "(" EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_END
        );
    }
        
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "          Lottery Shop" EZTR_CC_NEWLINE
        "Grand Prize:" EZTR_CC_NEWLINE
        "%c%m"
        EZTR_CC_COLOR_DEFAULT "%m" EZTR_CC_END,
        getAPLocationItemColor(lotteryItem),
        formatted_item_name,
        formatted_player_name
    );
    
    recomp_free(item_name);
    recomp_free(player_name);
}

EZTR_MSG_CALLBACK(randoLotteryNPCHint) {
    u32 lotteryItem = LOCATION_LOTTERY_SHOP;
    char* item_name;
    char* player_name;

    rando_get_location_item_name(lotteryItem, &item_name);
    rando_get_location_item_player(lotteryItem, &player_name);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);

    char formatted_item_name[128];
    if (!rando_location_is_checked(lotteryItem)) {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name,
            128,
            "%s" EZTR_CC_END,
            item_name
        );        
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name,
            128,
            EZTR_CC_COLOR_SILVER "50 Rupees" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END
        );
    }

    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(lotteryItem) && (!rando_location_is_checked(lotteryItem))) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_NEWLINE "(" EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_END
        );
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "Would you like the chance to buy" EZTR_CC_NEWLINE
        "your dreams for " EZTR_CC_COLOR_PINK "10 Rupees" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_NEWLINE
        EZTR_CC_CARRIAGE_RETURN EZTR_CC_BOX_BREAK2
        "Pick any three numbers, and if" EZTR_CC_NEWLINE
        "those are picked, you'll win" EZTR_CC_NEWLINE
        "%c%m"
        EZTR_CC_COLOR_DEFAULT "%m" EZTR_CC_EVENT EZTR_CC_END,
        getAPLocationItemColor(lotteryItem),
        formatted_item_name,
        formatted_player_name
    );
    
    recomp_free(item_name);
    recomp_free(player_name);
}

EZTR_MSG_CALLBACK(randoGrave1Hint) {
    u32 location1 = 0x0000A2; // Graveyard Day 1 Iron Knuckle Song
    char* item_name;
    char* player_name;
    u32 location2 = 0x060C03; // Graveyard Day 1 Bats Chest
    char* item_name2;
    char* player_name2;

    rando_get_location_item_name(location1, &item_name);
    rando_get_location_item_player(location1, &player_name);
    rando_get_location_item_name(location2, &item_name2);
    rando_get_location_item_player(location2, &player_name2);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);
    sanitizeRandoText(item_name2);
    sanitizeRandoText(player_name2);

    char formatted_item_name[128];
    if (!rando_location_is_checked(location1)) {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name,
            128,
            "%s" EZTR_CC_END,
            item_name
        );        
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name,
            128,
            EZTR_CC_COLOR_SILVER "nothing" EZTR_CC_END
        );
    }
    
    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(location1) && !rando_location_is_checked(location1)) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_NEWLINE EZTR_CC_COLOR_DEFAULT "(" EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_END
        );
    }

    char formatted_item_name2[128];
    if (!rando_location_is_checked(location2)) {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name2,
            128,
            "%s" EZTR_CC_END,
            item_name2
        );        
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name2,
            128,
            EZTR_CC_COLOR_SILVER "nothing" EZTR_CC_END
        );
    }

    char formatted_player_name2[128];
    if (!rando_get_location_has_local_item(location2) && !rando_location_is_checked(location2)) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name2,
            128,
            EZTR_CC_NEWLINE EZTR_CC_COLOR_DEFAULT "(" EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name2
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name2,
            128,
            EZTR_CC_END
        );
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "Hear lies the melody that" EZTR_CC_NEWLINE
        "summons the tears of" EZTR_CC_NEWLINE
        "%c%m%m" EZTR_CC_NEWLINE
        EZTR_CC_BOX_BREAK2
        "Those who defeat the" EZTR_CC_NEWLINE
        "evil will find" EZTR_CC_NEWLINE
        "%c%m%m" EZTR_CC_END,
        getAPLocationItemColor(location1),
        formatted_item_name,
        formatted_player_name,
        getAPLocationItemColor(location2),
        formatted_item_name2,
        formatted_player_name2
    );
    
    recomp_free(item_name);
    recomp_free(player_name);
    recomp_free(item_name2);
    recomp_free(player_name2);
}

EZTR_MSG_CALLBACK(randoGrave2Hint) {
    u32 location = 0x060C00; // Graveyard Day 2 Iron Knuckle Chest
    char* item_name;
    char* player_name;

    rando_get_location_item_name(location, &item_name);
    rando_get_location_item_player(location, &player_name);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);

    char formatted_item_name[128];
    if (!rando_location_is_checked(location)) {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name,
            128,
            "%s" EZTR_CC_END,
            item_name
        );        
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name,
            128,
            EZTR_CC_COLOR_SILVER "nothing" EZTR_CC_END
        );
    }

    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(location) && !rando_location_is_checked(location)) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_NEWLINE EZTR_CC_COLOR_DEFAULT "(" EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_END
        );
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "Those who possess eyes that can"
        EZTR_CC_NEWLINE "see the truth will find"
        EZTR_CC_NEWLINE "%c%m%m" EZTR_CC_END,
        getAPLocationItemColor(location),
        formatted_item_name,
        formatted_player_name
    );
    
    recomp_free(item_name);
    recomp_free(player_name);
}

EZTR_MSG_CALLBACK(randoGrave3Hint) {
    u32 location = 0x063000; // Graveyard Day 3 Dampe Big Poe Chest
    char* item_name;
    char* player_name;

    rando_get_location_item_name(location, &item_name);
    rando_get_location_item_player(location, &player_name);
    sanitizeRandoText(item_name);
    sanitizeRandoText(player_name);

    char formatted_item_name[128];
    if (!rando_location_is_checked(location)) {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name,
            128,
            "%s" EZTR_CC_END,
            item_name
        );        
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_item_name,
            128,
            EZTR_CC_COLOR_SILVER "nothing" EZTR_CC_END
        );
    }

    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(location) && !rando_location_is_checked(location)) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_NEWLINE EZTR_CC_COLOR_DEFAULT "(" EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT ")" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_END
        );
    }

    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "Bringing light to the darkness"
        EZTR_CC_NEWLINE "will reveal to you"
        EZTR_CC_NEWLINE "%c%m%m" EZTR_CC_END,
        getAPLocationItemColor(location),
        formatted_item_name,
        formatted_player_name
    );
    
    recomp_free(item_name);
    recomp_free(player_name);
}

// Bomb Shop Powder Keg Goron (he won't stop talking about Powder Kegs)
#define LOCATION_POWDER_KEG_GORON 0x024234
EZTR_MSG_CALLBACK(randoBombShopGoronNonGoronGreeting) {
    if (rando_location_is_checked(LOCATION_POWDER_KEG_GORON)) {
        return;
    }

    char* item_name;
    rando_get_location_item_name(LOCATION_POWDER_KEG_GORON, &item_name);
    sanitizeRandoText(item_name);

    char* player_name;
    rando_get_location_item_player(LOCATION_POWDER_KEG_GORON, &player_name);
    sanitizeRandoText(player_name);

    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(LOCATION_POWDER_KEG_GORON)) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_NEWLINE EZTR_CC_COLOR_DEFAULT "for %s" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_END
        );
    }
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        EZTR_CC_SFX "|38|FCHullo. Did ya come to buy a" EZTR_CC_NEWLINE
        "%c%s%m" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_CONTINUE EZTR_CC_END,
        getAPLocationItemColor(LOCATION_POWDER_KEG_GORON),
        item_name,
        formatted_player_name
    );
}

EZTR_MSG_CALLBACK(randoBombShopGoronNonGoronSecret) {
    if (rando_location_is_checked(LOCATION_POWDER_KEG_GORON)) {
        return;
    }

    char* classificationText;
    u32 type = rando_get_location_type(LOCATION_POWDER_KEG_GORON);
    if (type & 0b001) {
        classificationText = EZTR_CC_COLOR_LIGHTBLUE " Progression Item" EZTR_CC_COLOR_DEFAULT EZTR_CC_END;
    } else if (type & 0b010) {
        classificationText = EZTR_CC_COLOR_BLUE " Useful Item" EZTR_CC_COLOR_DEFAULT EZTR_CC_END;
    } else if (type & 0b100) {
        classificationText = EZTR_CC_COLOR_ORANGE " Trap" EZTR_CC_COLOR_DEFAULT EZTR_CC_END;
    } else {
        classificationText = "n " EZTR_CC_COLOR_SILVER "Item" EZTR_CC_COLOR_DEFAULT EZTR_CC_END;
    }
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "..This is just between us, but I" EZTR_CC_NEWLINE
        "have a%m that this shop" EZTR_CC_NEWLINE
        "doesn't even carry." EZTR_CC_CONTINUE EZTR_CC_END,
        classificationText
    );
}

EZTR_MSG_CALLBACK(randoBombShopGoronNonGoronHeavy) {
    if (rando_location_is_checked(LOCATION_POWDER_KEG_GORON)) {
        return;
    }
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        EZTR_CC_SFX "|3A|BBLook, I'd like you to buy my %citem," EZTR_CC_COLOR_DEFAULT EZTR_CC_NEWLINE
        "too, but it's just too heavy for" EZTR_CC_NEWLINE
        "you to carry." EZTR_CC_NEWLINE
        EZTR_CC_DELAY "|00|04" EZTR_CC_QUICKTEXT_ENABLE "Sorry." EZTR_CC_QUICKTEXT_DISABLE EZTR_CC_END,
        getAPLocationItemColor(LOCATION_POWDER_KEG_GORON)
    );
}

EZTR_MSG_CALLBACK(randoBombShopGoronGreeting) {
    if (rando_location_is_checked(LOCATION_POWDER_KEG_GORON)) {
        return;
    }

    char* item_name;
    rando_get_location_item_name(LOCATION_POWDER_KEG_GORON, &item_name);
    sanitizeRandoText(item_name);

    char* player_name;
    rando_get_location_item_player(LOCATION_POWDER_KEG_GORON, &player_name);
    sanitizeRandoText(player_name);

    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(LOCATION_POWDER_KEG_GORON)) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_COLOR_DEFAULT "%s's" EZTR_CC_NEWLINE EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_END
        );
    }
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        EZTR_CC_SFX "|38|FC" EZTR_CC_QUICKTEXT_ENABLE "Oh! Of all places!" EZTR_CC_QUICKTEXT_DISABLE EZTR_CC_NEWLINE
        EZTR_CC_CARRIAGE_RETURN EZTR_CC_CARRIAGE_RETURN
        EZTR_CC_BOX_BREAK2
        "You're a Goron, so you can carry" EZTR_CC_NEWLINE
        "%m%c%s" EZTR_CC_COLOR_DEFAULT "," EZTR_CC_NEWLINE
        "right?" EZTR_CC_CONTINUE EZTR_CC_END,
        formatted_player_name,
        getAPLocationItemColor(LOCATION_POWDER_KEG_GORON),
        item_name
    );
}

EZTR_MSG_CALLBACK(randoBombShopGoronWhatSell) {
    if (rando_location_is_checked(LOCATION_POWDER_KEG_GORON)) {
        return;
    }

    char* item_name;
    rando_get_location_item_name(LOCATION_POWDER_KEG_GORON, &item_name);
    sanitizeRandoText(item_name);

    char* player_name;
    rando_get_location_item_player(LOCATION_POWDER_KEG_GORON, &player_name);
    sanitizeRandoText(player_name);

    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(LOCATION_POWDER_KEG_GORON)) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_COLOR_GREEN "%s's" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            "a" EZTR_CC_END
        );
    }
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "Then I'll sell you %m" EZTR_CC_NEWLINE
        "%c%s" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END,
        formatted_player_name,
        getAPLocationItemColor(LOCATION_POWDER_KEG_GORON),
        item_name
    );
}

bool randomizedKegGoronItem;
EZTR_MSG_CALLBACK(randoBombShopGoronIsSell) {
    randomizedKegGoronItem = false;
    if (rando_location_is_checked(LOCATION_POWDER_KEG_GORON)) {
        return;
    }

    u32 price = 50; // TODO: randomize

    char* item_name;
    rando_get_location_item_name(LOCATION_POWDER_KEG_GORON, &item_name);
    sanitizeRandoText(item_name);

    char* player_name;
    rando_get_location_item_player(LOCATION_POWDER_KEG_GORON, &player_name);
    sanitizeRandoText(player_name);

    char formatted_player_name[128];
    if (!rando_get_location_has_local_item(LOCATION_POWDER_KEG_GORON)) {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            EZTR_CC_COLOR_GREEN "%s's" EZTR_CC_END,
            player_name
        );
    } else {
        EZTR_MsgSContent_Snprintf(
            formatted_player_name,
            128,
            "a" EZTR_CC_END
        );
    }
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "So, won't ya buy %m" EZTR_CC_NEWLINE
        "%c%s" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_DEFAULT "for " EZTR_CC_COLOR_PINK "%d Rupees" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_CONTINUE EZTR_CC_END,
        formatted_player_name,
        getAPLocationItemColor(LOCATION_POWDER_KEG_GORON),
        item_name,
        price
    );
    randomizedKegGoronItem = true;
}

EZTR_MSG_CALLBACK(randoBombShopGoronHasSell) {
    if (!randomizedKegGoronItem) {
        return;
    }

    char* item_name;
    rando_get_location_item_name(LOCATION_POWDER_KEG_GORON, &item_name);
    sanitizeRandoText(item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        EZTR_CC_SFX "|3A|BB" EZTR_CC_COLOR_RED "%s" EZTR_CC_COLOR_DEFAULT " made by my" EZTR_CC_NEWLINE
        "instructor are extremely powerful." EZTR_CC_NEWLINE
        EZTR_CC_CARRIAGE_RETURN EZTR_CC_BOX_BREAK2
        "Come back and see me if you run" EZTR_CC_NEWLINE
        "out." EZTR_CC_END,
        item_name
    );
    randomizedKegGoronItem = false;
}

#include "hints.h"

// Gossip Stones
EZTR_MSG_CALLBACK(randoGossips) {
    // recomp_printf("textId 0x%04X\n", textId);
    
    REPY_FN_SETUP_RANDO;

    REPY_FN_SET_U16("textId", textId);

    REPY_FN_EXEC_CACHE(
        rando_get_gossip_hint,
        "hint = recomp_data.ctx.slot_data['hints'][str(textId)]\n"
        "item_name = hint['item']\n"
        "item_type = hint['item_type']\n"
        "location_name = hint['location']\n"
        "location_id = hint['location_id']\n"
        "from_player = hint['from_player']\n"
        "to_player = hint['to_player']\n"
        "type = hint['type']\n"
        "filled = hint['filled']\n"
    );

    bool hint_exists = REPY_FN_GET_BOOL("filled");
    
    // place random junk hints when a hint isn't assigned
    if (!hint_exists) {
        u32 seed = rando_get_random_seed() * textId;
        u32 selection = Rand_ZeroOne_Variable(&seed) * HINT_NUM_JUNK;
        // u32 selection = Rand_ZeroOne() * HINT_NUM_JUNK; // true random rather than seeded random
        bool custom_text = false;
        char* text;
        switch (selection) {
            case 1: // frog is king
                text = "They say that " EZTR_CC_COLOR_GREEN "frog is king" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END;
                break;
            case 2: // player isn't doing enough
                REPY_FN_SET_U32("seed", seed);
                REPY_FN_EXEC_CACHE(
                    rando_get_random_player_for_junk_hint,
                    "import random\n"
                    "random.seed(seed + textId)\n"
                    "player_names = dict(recomp_data.ctx.player_names)\n"
                    "player_names.pop(0)\n" // remove the "Archipelago" player (even though its funny)
                    "player_choice = random.choice(list(player_names.items()))\n"
                    "player_slot = player_choice[0]\n"
                    "player_name = player_choice[1]\n"
                    "num_players = len(player_names)\n"
                );

                u32 random_slot = REPY_FN_GET_U32("player_slot");
                if (random_slot == rando_get_own_slot_id()) {
                    u32 num_players = REPY_FN_GET_U32("num_players");
                    if (num_players == 1) {
                        EZTR_MsgSContent_Sprintf(
                            buf->data.content,
                            "It seems that " EZTR_CC_COLOR_RED "you" EZTR_CC_COLOR_DEFAULT " aren't playing" EZTR_CC_NEWLINE
                            "through this rando well enough." EZTR_CC_END
                        );
                    } else {
                        EZTR_MsgSContent_Sprintf(
                            buf->data.content,
                            "It seems that " EZTR_CC_COLOR_RED "you" EZTR_CC_COLOR_DEFAULT " aren't pulling" EZTR_CC_NEWLINE
                            "your weight in this multiworld." EZTR_CC_END
                        );
                    }
                } else {
                    char* player_name = REPY_FN_GET_STR("player_name");
                    sanitizeRandoText(player_name);

                    EZTR_MsgSContent_Sprintf(
                        buf->data.content,
                        "It seems that " EZTR_CC_COLOR_RED "%s" EZTR_CC_COLOR_DEFAULT EZTR_CC_NEWLINE
                        "isn't pulling their weight in this" EZTR_CC_NEWLINE
                        "multiworld." EZTR_CC_END,
                        player_name
                    );

                    recomp_free(player_name);
                }

                custom_text = true;
                break;
            case 3: // loss
                // since %m doesn't accept pipe inputs, we need to use custom text for this
                EZTR_MsgSContent_NoPipe_Sprintf(
                    buf->data.content,
                    EZTR_CC_QUICKTEXT_ENABLE
                    "              |  ||" EZTR_CC_NEWLINE
                    "              ||  |_" EZTR_CC_END
                );
                custom_text = true;
                break;
            case 4: // display random fake(?) bomber's code
                EZTR_MsgBuffer_SetTextBoxDisplayIcon(buf, EZTR_ICON_BOMBERS_NOTEBOOK);
                custom_text = true;
                
                // convert bomber's code to 5 digit number
                u16 real_code = 0;
                for (int i = 0; i < ARRAY_COUNT(gSaveContext.save.saveInfo.bomberCode); i++) {
                    real_code = (real_code * 10) + gSaveContext.save.saveInfo.bomberCode[i];
                }

                // this is what the actual game does to generate the bomber's code
                s32 randBombers;
                bool digit_used;
                s16 digit_iter;
                s16 i = 1;
                
                s8 fakeBombersCode[5];

                do {
                    randBombers = Rand_S16Offset(0, 6);
                } while ((randBombers <= 0) || (randBombers >= 6));

                fakeBombersCode[0] = randBombers;

                while (i != 5) {
                    digit_used = false;

                    do {
                        randBombers = Rand_S16Offset(0, 6);
                    } while ((randBombers <= 0) || (randBombers >= 6));

                    digit_iter = 0;
                    do {
                        if (randBombers == fakeBombersCode[digit_iter]) {
                            digit_used = true;
                        }
                        digit_iter++;
                    } while (digit_iter < i);

                    if (digit_used == false) {
                        fakeBombersCode[i] = randBombers;
                        i++;
                    }
                }

                u16 fake_code = 0;
                for (int i = 0; i < ARRAY_COUNT(fakeBombersCode); i++) {
                    fake_code = (fake_code * 10) + fakeBombersCode[i];
                }

                char* random_connector;

                s16 random_connector_choice = Rand_S16Offset(0, 5);
                switch (random_connector_choice) {
                    default:
                    case 0:
                        random_connector = "might" EZTR_CC_NEWLINE "be " EZTR_CC_END;
                        break;
                    case 1:
                        random_connector = "is" EZTR_CC_NEWLINE "possibly " EZTR_CC_END;
                        break;
                    case 2:
                        random_connector = "could" EZTR_CC_NEWLINE "be " EZTR_CC_END;
                        break;
                    case 3:
                        random_connector = "probably" EZTR_CC_NEWLINE "isn't " EZTR_CC_END;
                        break;
                    case 4:
                        random_connector = "would" EZTR_CC_NEWLINE EZTR_CC_COLOR_RED "never" EZTR_CC_COLOR_DEFAULT " be " EZTR_CC_END;
                        break;
                }

                if (fake_code == real_code) {
                    EZTR_MsgBuffer_SetTextBoxDisplayIcon(buf, EZTR_ICON_EXCLAMATION_MARK);
                    random_connector = "is" EZTR_CC_NEWLINE "in fact " EZTR_CC_END;
                }

                EZTR_MsgSContent_Sprintf(
                    buf->data.content,
                    "The " EZTR_CC_COLOR_BLUE "Bomber's Secret Code" EZTR_CC_COLOR_DEFAULT " %m"
                    EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END,
                    random_connector,
                    fake_code
                );

                break;
            case 0: // use the !hint command!
            default:
                text = "Use the " EZTR_CC_COLOR_RED "!hint" EZTR_CC_COLOR_DEFAULT " command to hint" EZTR_CC_NEWLINE
                        "for an item!" EZTR_CC_END;
                break;
        }

        if (!custom_text) {
            EZTR_MsgSContent_Sprintf(
                buf->data.content,
                "%m" EZTR_CC_END,
                text
            );
        }

        REPY_FN_CLEANUP;

        return;
    }

    char* location_name = REPY_FN_GET_STR("location_name");
    sanitizeRandoText(location_name);

    char* item_name = REPY_FN_GET_STR("item_name");
    sanitizeRandoText(item_name);
    
    u32 item_type = REPY_FN_GET_U32("item_type");

    u32 from_player_id = REPY_FN_GET_U32("from_player");
    char* from_player;
    rando_get_player_name(from_player_id, &from_player);
    sanitizeRandoText(from_player);
    
    u32 to_player_id = REPY_FN_GET_U32("to_player");
    char* to_player;
    rando_get_player_name(to_player_id, &to_player);
    sanitizeRandoText(to_player);

    // Moon Gossip Stone Text
    if (textId >= 0x2103 && textId <= 0x2116) {
        if (from_player_id == to_player_id) {
            EZTR_MsgSContent_Sprintf(
                buf->data.content,
                "It seems " EZTR_CC_COLOR_RED "%s" EZTR_CC_COLOR_DEFAULT " was at" EZTR_CC_NEWLINE
                EZTR_CC_COLOR_LIGHTBLUE "%s" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END,
                item_name,
                location_name
            );
        } else {
            EZTR_MsgSContent_Sprintf(
                buf->data.content,
                "It seems " EZTR_CC_COLOR_BLUE "%s" EZTR_CC_COLOR_DEFAULT " had " EZTR_CC_COLOR_RED "%s" EZTR_CC_COLOR_DEFAULT EZTR_CC_NEWLINE
                "at " EZTR_CC_COLOR_LIGHTBLUE "%s" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END,
                from_player,
                item_name,
                location_name
            );
        }
    }

    u32 type = REPY_FN_GET_U32("type");
    char non_local_player[256];
    char non_local_world[256];

    // "initialize" with a blank/generic string (replace if needed for certain hints)
    EZTR_MsgSContent_Snprintf(
        non_local_player,
        256,
        EZTR_CC_END
    );
    EZTR_MsgSContent_Snprintf(
        non_local_world,
        256,
        EZTR_CC_END
    );
    if (from_player_id != to_player_id) { // non-local hint
        if (from_player_id == rando_get_own_slot_id()) { // item is in our world, hinting for someone else
            EZTR_MsgSContent_Snprintf(
                non_local_player,
                256,
                EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT "'s " EZTR_CC_END,
                to_player
            );
        } else { // our item is in someone else's world
            EZTR_MsgSContent_Snprintf(
                non_local_world,
                256,
                EZTR_CC_NEWLINE "in " EZTR_CC_COLOR_BLUE "%s" EZTR_CC_COLOR_DEFAULT "'s world" EZTR_CC_END,
                from_player
            );
        }
    }

    switch (type) {
        case HINT_TYPE_DEKU:
            // simple
            EZTR_MsgSContent_Sprintf(
                buf->data.content,
                "The " EZTR_CC_COLOR_RED "mask of scrubs" EZTR_CC_COLOR_DEFAULT " can be found%m at" EZTR_CC_NEWLINE
                EZTR_CC_COLOR_LIGHTBLUE "%s" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END,
                non_local_world,
                location_name
            );
            break;
        case HINT_TYPE_BEAVERS:
            // complex
            REPY_FN_EXEC_CACHE(
                rando_get_beaver_extra_hint,
                "extra_hint = recomp_data.ctx.slot_data['hints'][str(textId)]['extra']\n"
                "extra_item_name = extra_hint['item']\n"
                "extra_item_type = extra_hint['item_type']\n"
                "extra_item_player = extra_hint['player']\n"
                "extra_item_location_id = extra_hint['location_id']\n"
            );

            char* extra_item_name = REPY_FN_GET_STR("extra_item_name");
            u32 extra_item_type = REPY_FN_GET_U32("extra_item_type");
            
            u32 extra_item_player = REPY_FN_GET_U32("extra_item_player");
            char* extra_player_name;
            rando_get_player_name(extra_item_player, &extra_player_name);

            char non_local_player2[128];
            if (extra_item_player != rando_get_own_slot_id()) {
                EZTR_MsgSContent_Snprintf(
                    non_local_player2,
                    128,
                    EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT "'s " EZTR_CC_END,
                    extra_player_name
                );
            } else {
                EZTR_MsgSContent_Snprintf(
                    non_local_player2,
                    128,
                    EZTR_CC_END
                );
            }

            EZTR_MsgSContent_Sprintf(
                buf->data.content,
                "The " EZTR_CC_COLOR_RED "wretched aquatic mammals" EZTR_CC_COLOR_DEFAULT " offer" EZTR_CC_NEWLINE
                "%m%c%s" EZTR_CC_COLOR_DEFAULT " and" EZTR_CC_NEWLINE
                "%m%c%s" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END,
                non_local_player,
                getAPItemColor(item_type),
                item_name,
                non_local_player2,
                getAPItemColor(extra_item_type),
                extra_item_name
            );

            REPY_FN_EXEC_CACHE(
                py_rando_broadcast_beaver_gossip_hint,
                "msg_func = recomp_data.ctx.send_msgs([{\"cmd\": \"CreateHints\",\n"
                "                                       \"locations\": [location_id, extra_item_location_id],\n"
                "                                       \"player\": from_player}])\n"
                "RecompClient.run_async_task_once(msg_func)\n"
            );

            recomp_free(extra_item_name);
            recomp_free(extra_player_name);
            break;
        case HINT_TYPE_ANJU_KAFEI:
            EZTR_MsgSContent_Sprintf(
                buf->data.content,
                "A " EZTR_CC_COLOR_RED "couple's reunion" EZTR_CC_COLOR_DEFAULT " provides" EZTR_CC_NEWLINE
                "%m%c%s" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END,
                non_local_player,
                getAPItemColor(item_type),
                item_name
            );
            break;
        case HINT_TYPE_NONE:
        default:
            // generic (possibly placeholder) hint
            EZTR_MsgSContent_Sprintf(
                buf->data.content,
                "%m%c%s" EZTR_CC_COLOR_DEFAULT " can be found at" EZTR_CC_NEWLINE
                EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_DEFAULT "%m." EZTR_CC_END,
                non_local_player,
                getAPItemColor(item_type),
                item_name,
                location_name,
                non_local_world
            );
            break;
    }

    // this would be handled better in mod code using the glue function, but location_id may be far too large
    // exact copy of rando_broadcast_location_hint_player() from glue
    REPY_FN_EXEC_CACHE(
        py_rando_broadcast_gossip_hint,
        "msg_func = recomp_data.ctx.send_msgs([{\"cmd\": \"CreateHints\",\n"
        "                                       \"locations\": [location_id],\n"
        "                                       \"player\": from_player}])\n"
        "RecompClient.run_async_task_once(msg_func)\n"
    );

    recomp_free(location_name);
    recomp_free(item_name);
    recomp_free(from_player);
    recomp_free(to_player);

    REPY_FN_CLEANUP;
}

// Replacements of existing IDs
EZTR_ON_INIT void init_text() {
    EZTR_Basic_ReplaceText(
        0x13F9, // Night 1 Ikana Gravestone
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "\xBF",
        randoGrave1Hint
    );

    EZTR_Basic_ReplaceText(
        0x13FB, // Night 2 Ikana Gravestone
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "\xBF",
        randoGrave2Hint
    );

    EZTR_Basic_ReplaceText(
        0x13FA, // Night 3 Ikana Gravestone
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "\xBF",
        randoGrave3Hint
    );

    EZTR_Basic_ReplaceText(
        0x1C13, // Lottery Shop sign
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "\xBF",
        randoLotterySignHint
    );

    EZTR_Basic_ReplaceText(
        0x2B5C, // Lottery Shop NPC
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "\xBF",
        randoLotteryNPCHint
    );
    
    EZTR_Basic_ReplaceText(
        0x044D, // Bank hints
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "\xBF",
        randoBankHints
    );

    EZTR_Basic_ReplaceText(
        0x15E9, // Magic Bean Scrub
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|3A|D2Do you know what " EZTR_CC_COLOR_GREEN "magic beans" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_DEFAULT "are, sir?"
        EZTR_CC_NEWLINE "I'll sell you one for " EZTR_CC_COLOR_PINK "10 Rupees" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_EVENT EZTR_CC_END,
        randoScrub
    );
    EZTR_Basic_ReplaceText(
        0x15E1, // Magic Bean Scrub when talking as non-deku
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "" EZTR_CC_SFX "|39|A7I sell " EZTR_CC_COLOR_GREEN "Magic Beans " EZTR_CC_COLOR_DEFAULT "to Deku" EZTR_CC_NEWLINE 
        "Scrubs, but I'd really like to leave" EZTR_CC_NEWLINE 
        "my hometown." EZTR_CC_NEWLINE 
        EZTR_CC_BOX_BREAK2 "I'm hoping to find some success in" EZTR_CC_NEWLINE 
        "a livelier place!" EZTR_CC_EVENT "" EZTR_CC_END "",
        randoScrubNotSelling
    );
        
    EZTR_Basic_ReplaceText(
        0x15F3, // Magic Bean Scrub in new home
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|3A|D2Do you know what " EZTR_CC_COLOR_GREEN "magic beans"
        EZTR_CC_NEWLINE EZTR_CC_COLOR_DEFAULT "are?" EZTR_CC_NEWLINE
        "I'll sell you one for " EZTR_CC_COLOR_PINK "10 Rupees" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_EVENT EZTR_CC_END,
        randoScrub
    );
    
    EZTR_Basic_ReplaceText(
        0x1600, // Bomb Bag Scrub
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|38|81I'll give you my Biggest Bomb Bag," EZTR_CC_NEWLINE
        "regularly priced at " EZTR_CC_COLOR_PINK "1000 Rupees" EZTR_CC_COLOR_DEFAULT "..." EZTR_CC_NEWLINE
        EZTR_CC_CARRIAGE_RETURN EZTR_CC_BOX_BREAK2 "In return, you'll give me your " EZTR_CC_COLOR_RED "Big" EZTR_CC_NEWLINE
        "Bomb Bag " EZTR_CC_COLOR_DEFAULT "and just " EZTR_CC_COLOR_PINK "200 Rupees" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_EVENT EZTR_CC_END,
        randoScrub
    );
    EZTR_Basic_ReplaceText(
        0x15F5, // Bomb Bag Scrub when talking to Deku
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "I sell bomb bags, but I'm focusing" EZTR_CC_NEWLINE 
        "my marketing efforts on " EZTR_CC_COLOR_RED "Gorons." EZTR_CC_NEWLINE 
        "" EZTR_CC_COLOR_DEFAULT "" EZTR_CC_CARRIAGE_RETURN "" EZTR_CC_BOX_BREAK2 
        "What I'd really like to do is go" EZTR_CC_NEWLINE 
        "back home and do business where" EZTR_CC_NEWLINE 
        "I'm surrounded by trees and grass." EZTR_CC_EVENT "" EZTR_CC_END "",
        randoScrubNotSelling
    );
    
    EZTR_Basic_ReplaceText(
        0x1606, // Bomb Bag Scrub in new home
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|38|81I'll give you my Biggest Bomb Bag," EZTR_CC_NEWLINE
        "regularly priced at 1000 Rupees," EZTR_CC_NEWLINE 
        "for your " EZTR_CC_COLOR_RED "Big Bomb Bag " EZTR_CC_COLOR_DEFAULT "and just" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_PINK "200 Rupees" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_EVENT EZTR_CC_END,
        randoScrub
    );
    
    EZTR_Basic_ReplaceText(
        0x1612, // Green Potion Scrub
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|39|8CI'll sell you a " EZTR_CC_COLOR_GREEN "Green Potion" EZTR_CC_COLOR_DEFAULT " for" 
        EZTR_CC_NEWLINE EZTR_CC_COLOR_PINK "40 Rupees" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_EVENT2 EZTR_CC_END,
        randoScrub
    );
    EZTR_Basic_ReplaceText(
        0x1608, // Green Potion Scrub when talking to Goron
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|39|A7I'm selling " EZTR_CC_COLOR_GREEN "Green Potions" EZTR_CC_COLOR_DEFAULT ", but I'm" EZTR_CC_NEWLINE 
        "focusing my marketing efforts on" EZTR_CC_NEWLINE 
        "Zoras." EZTR_CC_NEWLINE "" EZTR_CC_BOX_BREAK2 
        "Actually, I'd like to do business" EZTR_CC_NEWLINE 
        "someplace where it's cooler and" EZTR_CC_NEWLINE 
        "the air is clean." EZTR_CC_EVENT "" EZTR_CC_END "",
        randoScrubNotSelling
    );
    
    EZTR_Basic_ReplaceText(
        0x1617, // Green Potion Scrub in new home
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|39|8CI'll sell you a " EZTR_CC_COLOR_GREEN "Green Potion" EZTR_CC_COLOR_DEFAULT " for" 
        EZTR_CC_NEWLINE EZTR_CC_COLOR_PINK "40 Rupees" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_EVENT2 EZTR_CC_END,
        randoScrub
    );
    
    EZTR_Basic_ReplaceText(
        0x1626, // Blue Potion Scrub
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|3A|D2Don't you need any " EZTR_CC_COLOR_BLUE "Blue Potion" EZTR_CC_COLOR_DEFAULT " in" EZTR_CC_NEWLINE
        "case you get cursed?" EZTR_CC_NEWLINE
        "One drink is " EZTR_CC_COLOR_PINK "100 Rupees" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_EVENT EZTR_CC_END,
        randoScrub
    );
    EZTR_Basic_ReplaceText(
        0x161C, // Blue Potion Scrub talking to Zora
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|39|A7I'm here to sell " EZTR_CC_COLOR_BLUE "Blue Potion" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_NEWLINE 
        EZTR_CC_CARRIAGE_RETURN "" EZTR_CC_CARRIAGE_RETURN "" EZTR_CC_BOX_BREAK2 "Actually, I want to do business in" EZTR_CC_NEWLINE 
        "the sea breeze while listening to" EZTR_CC_NEWLINE 
        "the sound of the waves." EZTR_CC_EVENT "" EZTR_CC_END "",
        randoScrubNotSelling
    );
    
    EZTR_Basic_ReplaceText(
        0x162D, // Blue Potion Scrub in new home
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|39|8CI'll sell you one " EZTR_CC_COLOR_BLUE "Blue Potion" EZTR_CC_COLOR_DEFAULT " for" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_PINK "100 Rupees" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_EVENT EZTR_CC_END,
        randoScrub
    );
    
    EZTR_Basic_ReplaceText(
        0x353C, // Fast Dog
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "..." EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_ReplaceText(
        0x3545, // Slow Dog
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
         EZTR_CC_SFX "|29|13Hoo-whine." EZTR_CC_NEWLINE "How can any of us win against..." EZTR_CC_NEWLINE EZTR_CC_COLOR_BLUE "blue dog" EZTR_CC_COLOR_DEFAULT "..." EZTR_CC_END,
        NULL
    );
    
    // EZTR_Basic_ReplaceText(
    //     0x20D0, // Gossip Hint joke
    //     EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
    //     0,
    //     EZTR_ICON_NO_ICON,
    //     EZTR_NO_VALUE,
    //     EZTR_NO_VALUE,
    //     EZTR_NO_VALUE,
    //     false,
    //     "Use the " EZTR_CC_COLOR_RED "!hint" EZTR_CC_COLOR_DEFAULT " command to hint" EZTR_CC_NEWLINE "for an item!" EZTR_CC_END,
    //     NULL
    // );
    
    // EZTR_Basic_ReplaceText(
    //     0x20D0, // Replaces the Gossip Stone Tatl text to test if it can read the AP items correctly without fully implimenting it on get item.
    //     EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
    //     0,
    //     EZTR_ICON_NO_ICON,
    //     EZTR_NO_VALUE,
    //     EZTR_NO_VALUE,
    //     EZTR_NO_VALUE,
    //     false,
    //     "\xBF",
    //     randoAPSend
    // );
    
    // Pictograph Box text
    EZTR_Basic_ReplaceText(
        0x00F8,
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoPictograph
    );
    
    EZTR_Basic_ReplaceText(
        0x2B0B,
        EZTR_STANDARD_TEXT_BOX_II,
        1,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        20,
        200,
        true,
        "\xBF",
        randoMilkBar
    );

    // Kotake Blue Potion Text
    EZTR_Basic_ReplaceText(
        0x0880,
        EZTR_STANDARD_TEXT_BOX_II,
        0x30,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoShop
    );

    // Tingle Text
    // North Clock Town Tingle
    EZTR_Basic_ReplaceText(
        0x1D11,
        EZTR_STANDARD_TEXT_BOX_II,
        1,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        5,
        40,
        true,
        "\xBF",
        randoTingle
    );

    // Path to Swamp Tingle (Woodfall)
    EZTR_Basic_ReplaceText(
        0x1D12,
        EZTR_STANDARD_TEXT_BOX_II,
        1,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        20,
        40,
        true,
        "\xBF",
        randoTingle
    );

    // Twin Islands Tingle (Snowhead)
    EZTR_Basic_ReplaceText(
        0x1D13,
        EZTR_STANDARD_TEXT_BOX_II,
        1,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        20,
        40,
        true,
        "\xBF",
        randoTingle
    );

    // Milk Road Tingle
    EZTR_Basic_ReplaceText(
        0x1D14,
        EZTR_STANDARD_TEXT_BOX_II,
        1,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        20,
        40,
        true,
        "\xBF",
        randoTingle
    );

    // Great Bay Coast Tingle
    EZTR_Basic_ReplaceText(
        0x1D15,
        EZTR_STANDARD_TEXT_BOX_II,
        1,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        20,
        40,
        true,
        "\xBF",
        randoTingle
    );

    // Ikana Canyon Tingle
    EZTR_Basic_ReplaceText(
        0x1D16,
        EZTR_STANDARD_TEXT_BOX_II,
        1,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        20,
        40,
        true,
        "\xBF",
        randoTingle
    );

    // Bomb Shop Powder Keg Goron (talks about the Powder Keg too much)
    EZTR_Basic_ReplaceText(
        0x0664,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|38|FCHullo. Did ya come to buy a" EZTR_CC_NEWLINE
        "Powder Keg?" EZTR_CC_CONTINUE EZTR_CC_END,
        randoBombShopGoronNonGoronGreeting
    );

    EZTR_Basic_ReplaceText(
        0x0665,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "..This is just between us, but I" EZTR_CC_NEWLINE
        "have a huge bomb that this shop" EZTR_CC_NEWLINE
        "doesn't even carry." EZTR_CC_CONTINUE EZTR_CC_END,
        randoBombShopGoronNonGoronSecret
    );

    EZTR_Basic_ReplaceText(
        0x066C,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|3A|BBLook, I'd like you to buy my bomb," EZTR_CC_NEWLINE
        "too, but it's just too heavy for" EZTR_CC_NEWLINE
        "you to carry." EZTR_CC_NEWLINE
        EZTR_CC_DELAY "|00|04" EZTR_CC_QUICKTEXT_ENABLE "Sorry." EZTR_CC_QUICKTEXT_DISABLE EZTR_CC_END,
        randoBombShopGoronNonGoronHeavy
    );

    EZTR_Basic_ReplaceText(
        0x067B,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|38|FC" EZTR_CC_QUICKTEXT_ENABLE "Oh! Of all places!" EZTR_CC_QUICKTEXT_DISABLE EZTR_CC_NEWLINE
        EZTR_CC_CARRIAGE_RETURN EZTR_CC_CARRIAGE_RETURN
        EZTR_CC_BOX_BREAK2
        "You're a Goron, so you can carry" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_RED "Powder Kegs" EZTR_CC_COLOR_DEFAULT ", right?" EZTR_CC_CONTINUE EZTR_CC_END,
        randoBombShopGoronGreeting
    );

    EZTR_Basic_ReplaceText(
        0x0682,
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "Then I'll sell you a " EZTR_CC_COLOR_RED "Powder Keg" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_END,
        randoBombShopGoronWhatSell
    );
    
    EZTR_Basic_ReplaceText(
        0x066E,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|38|FC" EZTR_CC_QUICKTEXT_ENABLE "Oh! Of all places!" EZTR_CC_QUICKTEXT_DISABLE EZTR_CC_NEWLINE
        EZTR_CC_CARRIAGE_RETURN EZTR_CC_CARRIAGE_RETURN
        EZTR_CC_BOX_BREAK2
        "You're a Goron, so you can carry" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_RED "Powder Kegs" EZTR_CC_COLOR_DEFAULT ", right?" EZTR_CC_CONTINUE EZTR_CC_END,
        randoBombShopGoronGreeting
    );

    EZTR_Basic_ReplaceText(
        0x066F,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "So, won't ya buy a " EZTR_CC_COLOR_RED "Powder Keg" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_DEFAULT "for " EZTR_CC_COLOR_PINK "50 Rupees" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_CONTINUE EZTR_CC_END,
        randoBombShopGoronIsSell
    );

    EZTR_Basic_ReplaceText(
        0x0675,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        EZTR_CC_SFX "|3A|BB" EZTR_CC_COLOR_RED "Powder Kegs" EZTR_CC_COLOR_DEFAULT " made by my" EZTR_CC_NEWLINE
        "instructor are extremely powerful." EZTR_CC_NEWLINE
        EZTR_CC_CARRIAGE_RETURN EZTR_CC_BOX_BREAK2
        "Come back and see me if you run" EZTR_CC_NEWLINE
        "out." EZTR_CC_END,
        randoBombShopGoronHasSell
    );
    
    
    // Gossip Stones
    // hopefully this covers all the gossip stones properly and doesn't crash (it'll probably crash on some)
    for (int gossip_index = 0x20B0; gossip_index <= 0x2116; gossip_index++) {
        // ignore "time remaining" message that appears when hitting gossip stones
        if (gossip_index == 0x20D2) {
            continue;
        }
        EZTR_Basic_ReplaceText(
            gossip_index,
            EZTR_STANDARD_TEXT_BOX_I, // normally EZTR_TRANSLUSCENT_BLUE_TEXT_BOX
            0,
            EZTR_ICON_NO_ICON,
            EZTR_NO_VALUE,
            EZTR_NO_VALUE,
            EZTR_NO_VALUE,
            false,
            EZTR_CC_END,
            randoGossips
        );
    }


    // Custom Text IDs
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_Send_Item),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoAPSend
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_Self_Item),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoAPSelf
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Magic),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_LARGE_MAGIC_JAR,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoGIMagic
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_SpinAttack),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You mastered the " EZTR_CC_COLOR_RED "Spin Attack" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_DoubleDefense),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_HEART_CONTAINER,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "Your " EZTR_CC_COLOR_RED "defense" EZTR_CC_COLOR_DEFAULT " has been" EZTR_CC_NEWLINE "strengthened!" EZTR_CC_NEWLINE
        "Enemies now do half as much" EZTR_CC_NEWLINE "damage as before!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Souls),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoGISouls
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Frogs),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_DON_GEROS_MASK,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoGIFrogs
    );

    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Kokiri_Sword),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_KOKIRI_SWORD,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_RED "Kokiri Sword" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Bombchu_Bag),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_BOMBCHU,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a " EZTR_CC_COLOR_RED "Bombchu Bag" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_CTSF),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_STRAY_FAIRY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_ORANGE "Clock Town" EZTR_CC_NEWLINE
        EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_WFSF),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_STRAY_FAIRY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        WoodfallStrayFairyCount
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_SHSF),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_STRAY_FAIRY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        SnowheadStrayFairyCount
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_GBSF),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_STRAY_FAIRY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        GreatBayStrayFairyCount
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_STSF),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_STRAY_FAIRY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        StoneTowerStrayFairyCount
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_WFBK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_BOSS_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Boss Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_SHBK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_BOSS_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Boss Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_GBBK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_BOSS_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Boss Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_STBK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_BOSS_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Boss Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_WFSK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_SMALL_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_SHSK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_SMALL_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        SnowheadSmallKeyCount
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_GBSK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_SMALL_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_STSK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_SMALL_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        StoneTowerSmallKeyCount
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_WFMap),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_DUNGEON_MAP,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Map" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_SHMap),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_DUNGEON_MAP,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Map" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_GBMap),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_DUNGEON_MAP,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Map" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_GBMap),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_DUNGEON_MAP,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Map" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_WFCompass),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_COMPASS,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Compass" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_SHCompass),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_COMPASS,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Compass" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_GBCompass),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_COMPASS,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Compass" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_GBCompass),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_COMPASS,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Compass" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Swamp_Token),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_GOLD_SKULLTULA_TOKEN,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        SwampSpiderTokenCount
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Ocean_Token),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_GOLD_SKULLTULA_TOKEN,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        OceanSpiderTokenCount
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_FOOL),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You are a" EZTR_CC_COLOR_RED "FOOL" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_Moon_Child_Return),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        1,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "...But you are not strong enough..." EZTR_CC_CARRIAGE_RETURN
        EZTR_CC_BOX_BREAK2
        "Shall... I send you back?" EZTR_CC_COLOR_GREEN EZTR_CC_NEWLINE
        EZTR_CC_NEWLINE
        EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE
        "No" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_Shop),
        EZTR_STANDARD_TEXT_BOX_II,
        0x30,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoShop
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_Shop_Buying),
        EZTR_STANDARD_TEXT_BOX_II,
        0x31,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoShopBuy
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_OwlStatue),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoGIOwlStatue
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Owl_Hidden),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the" EZTR_CC_COLOR_RED " Hidden Owl Statue" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE
        "You can now " EZTR_CC_NEWLINE EZTR_CC_COLOR_RED "Index Warp" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );

    // Songs
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Time),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You learned the " EZTR_CC_COLOR_LIGHTBLUE "Song of Time" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );

    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Healing),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You learned the " EZTR_CC_COLOR_PINK "Song of Healing" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );

    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Epona),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You learned the " EZTR_CC_COLOR_ORANGE "Epona's Song" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );

    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Soaring),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You learned the " EZTR_CC_COLOR_BLUE "Song of Soaring" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );

    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Storms),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You learned the " EZTR_CC_COLOR_SILVER "Song of Storms" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );

    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Sonata),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You learned the " EZTR_CC_COLOR_GREEN "Sonata of Awakening" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );

    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Lullaby),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You learned the " EZTR_CC_COLOR_RED "Goron Lullaby" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );

    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Nova),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You learned the " EZTR_CC_COLOR_BLUE "New Wave Bossa Nova" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );

    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Elegy),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You learned the " EZTR_CC_COLOR_ORANGE "Elegy of Emptiness" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    EZTR_Basic_AddCustomText(
        EZTR_HNAME(Rando_GI_Oath),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You learned the " EZTR_CC_COLOR_PINK "Oath to Order" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END,
        NULL
    );
    
    sGetItemTable_ap[GI_BOSS_SOUL_ODOLWA - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls)); // Souls
    sGetItemTable_ap[GI_BOSS_SOUL_GOHT - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_BOSS_SOUL_GYORG - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_BOSS_SOUL_TWINMOLD - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_BOSS_SOUL_MAJORA - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_MISC_SOUL_COW - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_MISC_SOUL_KEATON - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_MISC_SOUL_GOLD_SKULLTULAS - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_MISC_SOUL_BUTTERFLY - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_NPC_GENERIC - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_UTILITY_GENERIC - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_ENEMY_GENERIC - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_ABSURD_GENERIC - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Souls));
    sGetItemTable_ap[GI_FROG_BLUE - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Frogs)); // Frogs
    sGetItemTable_ap[GI_FROG_CYAN - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Frogs)); 
    sGetItemTable_ap[GI_FROG_PINK - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Frogs)); 
    sGetItemTable_ap[GI_FROG_YELLOW - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Frogs)); 
    sGetItemTable_ap[GI_FROG_WHITE - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Frogs)); 
    sGetItemTable_ap[GI_SWORD_KOKIRI - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Kokiri_Sword)); // Items
    sGetItemTable_ap[GI_BAG_BOMBCHU - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Bombchu_Bag));
    sGetItemTable_ap[GI_SCARECROW - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item)); 
    sGetItemTable_ap[GI_MAGIC_UPGRADE - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Magic));
    sGetItemTable_ap[GI_SPIN_ATTACK - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_SpinAttack));
    sGetItemTable_ap[GI_DEFENSE_DOUBLE - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_DoubleDefense));
    sGetItemTable_ap[GI_AP_PROG - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Send_Item)); // AP Non-local items
    sGetItemTable_ap[GI_AP_FILLER - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Send_Item));
    sGetItemTable_ap[GI_AP_USEFUL - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Send_Item));
    sGetItemTable_ap[GI_OOT_ITEM_PROG - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Send_Item)); // Custom OoT/WW items just in case we want them
    sGetItemTable_ap[GI_OOT_ITEM_FILLER - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Send_Item));
    sGetItemTable_ap[GI_OOT_ITEM_USEFUL - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Send_Item));
    sGetItemTable_ap[GI_WW_ITEM_PROG - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Send_Item)); 
    sGetItemTable_ap[GI_WW_ITEM_FILLER - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Send_Item));
    sGetItemTable_ap[GI_WW_ITEM_USEFUL - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Send_Item));
    sGetItemTable_ap[GI_STRAY_FAIRY_CLOCKTOWN - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_CTSF)); // Stray Fairies
    sGetItemTable_ap[GI_STRAY_FAIRY_WOODFALL - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_WFSF));
    sGetItemTable_ap[GI_STRAY_FAIRY_SNOWHEAD - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_SHSF));
    sGetItemTable_ap[GI_STRAY_FAIRY_GREATBAY - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_GBSF));
    sGetItemTable_ap[GI_STRAY_FAIRY_STONETOWER - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_STSF));
    sGetItemTable_ap[GI_KEY_BOSS_WOODFALL - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_WFBK)); // Boss Keys
    sGetItemTable_ap[GI_KEY_BOSS_SNOWHEAD - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_SHBK));
    sGetItemTable_ap[GI_KEY_BOSS_GREATBAY - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_GBBK));
    sGetItemTable_ap[GI_KEY_BOSS_STONETOWER - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_STBK));
    sGetItemTable_ap[GI_KEY_SMALL_WOODFALL - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_WFSK)); // Small Keys
    sGetItemTable_ap[GI_KEY_SMALL_SNOWHEAD - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_SHSK));
    sGetItemTable_ap[GI_KEY_SMALL_GREATBAY - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_GBSK));
    sGetItemTable_ap[GI_KEY_SMALL_STONETOWER - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_STSK));
    sGetItemTable_ap[GI_MAP_WOODFALL - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_WFMap)); // Maps
    sGetItemTable_ap[GI_MAP_SNOWHEAD - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_SHMap));
    sGetItemTable_ap[GI_MAP_GREATBAY - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_GBMap));
    sGetItemTable_ap[GI_MAP_STONETOWER - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_STMap));
    sGetItemTable_ap[GI_COMPASS_WOODFALL - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_WFCompass)); // Compasses
    sGetItemTable_ap[GI_COMPASS_SNOWHEAD - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_SHCompass));
    sGetItemTable_ap[GI_COMPASS_GREATBAY - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_GBCompass));
    sGetItemTable_ap[GI_COMPASS_STONETOWER - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_STCompass));
    sGetItemTable_ap[GI_TRUE_SKULL_TOKEN - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Swamp_Token)); // Skulltula Tokens
    sGetItemTable_ap[GI_OCEAN_SKULL_TOKEN - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Ocean_Token));
    sGetItemTable_ap[GI_OWL_GREAT_BAY_COAST - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item)); // Owl Statues
    sGetItemTable_ap[GI_OWL_ZORA_CAPE - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item));
    sGetItemTable_ap[GI_OWL_SNOWHEAD - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item));
    sGetItemTable_ap[GI_OWL_MOUNTAIN_VILLAGE - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item));
    sGetItemTable_ap[GI_OWL_CLOCK_TOWN - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item));
    sGetItemTable_ap[GI_OWL_MILK_ROAD - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item));
    sGetItemTable_ap[GI_OWL_WOODFALL - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item));
    sGetItemTable_ap[GI_OWL_SOUTHERN_SWAMP - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item));
    sGetItemTable_ap[GI_OWL_IKANA_CANYON - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item));
    sGetItemTable_ap[GI_OWL_STONE_TOWER - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item));
    sGetItemTable_ap[GI_OWL_HIDDEN - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_Self_Item));
    sGetItemTable_ap[GI_SONG_TIME - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Time)); // Songs
    sGetItemTable_ap[GI_SONG_HEALING - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Healing));
    sGetItemTable_ap[GI_SONG_EPONA - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Epona));
    sGetItemTable_ap[GI_SONG_SOARING - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Soaring));
    sGetItemTable_ap[GI_SONG_STORMS - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Storms));
    sGetItemTable_ap[GI_SONG_SONATA - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Sonata));
    sGetItemTable_ap[GI_SONG_LULLABY - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Lullaby));
    sGetItemTable_ap[GI_SONG_NOVA - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Nova));
    sGetItemTable_ap[GI_SONG_ELEGY - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Elegy));
    sGetItemTable_ap[GI_SONG_OATH - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Oath));
}
