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

// old defines that I didn't use
// #define RANDO_AP_ITEM "\xFF"
// #define RANDO_AP_PLAYER "\xFE"
// #define RANDO_AP_COLOR "\xFD"

// sets colours for AP item class. probably doesn't work
// u8 getAPItemColor(u32 location) {
//     switch (rando_get_location_type(location)) {
//         case 1:  return 0x05; // EZTR_CC_COLOR_LIGHTBLUE;  // progression - purple
//         case 2:  return 0x03; // EZTR_CC_COLOR_BLUE;  // useful - blue
//         case 3:  return 0x08; // EZTR_CC_COLOR_ORANGE;  // trap - orange
//         case 0:
//         default: return 0x07; // EZTR_CC_COLOR_SILVER;  // filler - grey
//     }
// }

void sanitizeRandoText(char* rando_string);

u8 getAPItemColor(u32 location) {
    u32 type = rando_get_location_type(location);
    if (type & 0b001) {
        return 0x05; // EZTR_CC_COLOR_LIGHTBLUE;  // progression - purple
    } else if (type & 0b010) {
        return 0x03; // EZTR_CC_COLOR_BLUE;  // useful - blue
    } else if (type & 0b100) {
        return 0x08; // EZTR_CC_COLOR_ORANGE;  // trap - orange
    } else {
        return 0x07; // EZTR_CC_COLOR_SILVER;  // filler - grey
    }
}

EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Send_Item); // "You sent [player] their [item]"
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Self_Item); // "You found your [item]"
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Kokiri_Sword);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Bombchu_Bag);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Magic);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SpinAttack);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_DoubleDefense);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Songs);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Souls);
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
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Scrub);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Milk_Bar);

// Get info from Rando to replace the text with item class colour, player name and item name
EZTR_MSG_CALLBACK(randoAPSend) {
    u32 location = rando_get_last_location_sent();
    char* player_name;
    char* item_name;
    
    rando_get_location_item_player(location, &player_name);
    rando_get_location_item_name(location, &item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You found " EZTR_CC_COLOR_RED "%s" EZTR_CC_COLOR_DEFAULT "'s" EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        player_name,
        getAPItemColor(location),
        item_name
    );
    
    recomp_free(player_name);
    recomp_free(item_name);
}

EZTR_MSG_CALLBACK(randoAPSelf) {
    u32 location = rando_get_last_location_sent();
    char* item_name;

    rando_get_location_item_name(location, &item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You found your" EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        getAPItemColor(location),
        item_name
    );
    
    recomp_free(item_name);
}
EZTR_MSG_CALLBACK(randoGIOwlStatue) {
    u32 location = rando_get_last_location_sent();
    char* item_name;

    rando_get_location_item_name(location, &item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You can now soar to the" EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        getAPItemColor(location),
        item_name
    );
    
    recomp_free(item_name);
}
EZTR_MSG_CALLBACK(randoGISongs) {
    u32 location = rando_get_last_location_sent();
    char* item_name;

    rando_get_location_item_name(location, &item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You learned the " EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        getAPItemColor(location),
        item_name
    );
    
    recomp_free(item_name);
}
EZTR_MSG_CALLBACK(randoGIMagic) {
    u8 magic_count = rando_has_item(AP_ITEM_ID_MAGIC);
    if (magic_count < 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got " EZTR_CC_COLOR_RED "Magic Power" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
            NULL
            );
            } else if (magic_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "Your " EZTR_CC_COLOR_RED "Magic Power" EZTR_CC_COLOR_DEFAULT " has been " EZTR_CC_COLOR_RED "doubled" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
            NULL
            );
            }
}
EZTR_MSG_CALLBACK(randoGISouls) {
    u32 location = rando_get_last_location_sent();
    char* item_name;

    rando_get_location_item_name(location, &item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You found the" EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        getAPItemColor(location),
        item_name
    );
    
    recomp_free(item_name);
}
EZTR_MSG_CALLBACK(randoGIFrogs) {
    u32 location = rando_get_last_location_sent();
    char* item_name;

    rando_get_location_item_name(location, &item_name);
    
    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "You found the" EZTR_CC_NEWLINE "%c%s" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        getAPItemColor(location),
        item_name
    );
    
    recomp_free(item_name);
}


EZTR_MSG_CALLBACK(randoTingleClockTown) {
    u32 tingleFirstItem = GI_TINGLE_MAP_CLOCK_TOWN;
    char* player_name;
    char* get_player_name;
    char* item_name;
    char* get_item_name;
    u32 tingleSecondItem = GI_TINGLE_MAP_WOODFALL;
    char* player_name2;
    char* get_player_name2;
    char* item_name2;
    char* get_item_name2;

    char* rupee_cost1;
    char* rupee_cost2;

    
    rando_get_location_item_player(tingleFirstItem, &get_player_name);
    rando_get_location_item_name(tingleFirstItem, &get_item_name);
    rando_get_location_item_player(tingleSecondItem, &get_player_name2);
    rando_get_location_item_name(tingleSecondItem, &get_item_name2);

    if (rando_location_is_checked(tingleFirstItem) == true) {player_name = "";
    } else if (rando_get_location_has_local_item(tingleFirstItem) == true) {player_name = "";
    } else {player_name = get_player_name;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {item_name = "SOLD OUT";
    } else {item_name = get_item_name;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {player_name2 = "";
    } else if (rando_get_location_has_local_item(tingleSecondItem) == true) {player_name2 = "";
    } else {player_name2 = get_player_name2;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {item_name2 = "SOLD OUT";
    } else {item_name2 = get_item_name2;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {rupee_cost1 = "";
    } else {rupee_cost1 = "5 Rupees";
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {rupee_cost2 = "";
    } else {rupee_cost2 = "40 Rupees";
    }



    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "" EZTR_CC_THREE_CHOICE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_GREEN "No thanks" EZTR_CC_END "",
        player_name,
        item_name,
        rupee_cost1,
        player_name2,
        item_name2,
        rupee_cost2
    );

    recomp_free(get_item_name);
    recomp_free(get_item_name2);
    recomp_free(get_player_name);
    recomp_free(get_player_name2);
}
EZTR_MSG_CALLBACK(randoTingleWoodfall) {
    u32 tingleFirstItem = GI_TINGLE_MAP_WOODFALL;
    char* player_name;
    char* get_player_name;
    char* item_name;
    char* get_item_name;
    u32 tingleSecondItem = GI_TINGLE_MAP_SNOWHEAD;
    char* player_name2;
    char* get_player_name2;
    char* item_name2;
    char* get_item_name2;

    char* rupee_cost1;
    char* rupee_cost2;

    
    rando_get_location_item_player(tingleFirstItem, &get_player_name);
    rando_get_location_item_name(tingleFirstItem, &get_item_name);
    rando_get_location_item_player(tingleSecondItem, &get_player_name2);
    rando_get_location_item_name(tingleSecondItem, &get_item_name2);

    if (rando_location_is_checked(tingleFirstItem) == true) {player_name = "";
    } else if (rando_get_location_has_local_item(tingleFirstItem) == true) {player_name = "";
    } else {player_name = get_player_name;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {item_name = "SOLD OUT";
    } else {item_name = get_item_name;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {player_name2 = "";
    } else if (rando_get_location_has_local_item(tingleSecondItem) == true) {player_name2 = "";
    } else {player_name2 = get_player_name2;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {item_name2 = "SOLD OUT";
    } else {item_name2 = get_item_name2;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {rupee_cost1 = "";
    } else {rupee_cost1 = "20 Rupees";
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {rupee_cost2 = "";
    } else {rupee_cost2 = "40 Rupees";
    }



    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "" EZTR_CC_THREE_CHOICE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_GREEN "No thanks" EZTR_CC_END "",        player_name,
        item_name,
        rupee_cost1,
        player_name2,
        item_name2,
        rupee_cost2
    );

    recomp_free(get_item_name);
    recomp_free(get_item_name2);
    recomp_free(get_player_name);
    recomp_free(get_player_name2);
}
EZTR_MSG_CALLBACK(randoTingleSnowhead) {
    u32 tingleFirstItem = GI_TINGLE_MAP_SNOWHEAD;
    char* player_name;
    char* get_player_name;
    char* item_name;
    char* get_item_name;
    u32 tingleSecondItem = GI_TINGLE_MAP_ROMANI_RANCH;
    char* player_name2;
    char* get_player_name2;
    char* item_name2;
    char* get_item_name2;

    char* rupee_cost1;
    char* rupee_cost2;

    
    rando_get_location_item_player(tingleFirstItem, &get_player_name);
    rando_get_location_item_name(tingleFirstItem, &get_item_name);
    rando_get_location_item_player(tingleSecondItem, &get_player_name2);
    rando_get_location_item_name(tingleSecondItem, &get_item_name2);

    if (rando_location_is_checked(tingleFirstItem) == true) {player_name = "";
    } else if (rando_get_location_has_local_item(tingleFirstItem) == true) {player_name = "";
    } else {player_name = get_player_name;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {item_name = "SOLD OUT";
    } else {item_name = get_item_name;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {player_name2 = "";
    } else if (rando_get_location_has_local_item(tingleSecondItem) == true) {player_name2 = "";
    } else {player_name2 = get_player_name2;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {item_name2 = "SOLD OUT";
    } else {item_name2 = get_item_name2;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {rupee_cost1 = "";
    } else {rupee_cost1 = "20 Rupees";
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {rupee_cost2 = "";
    } else {rupee_cost2 = "40 Rupees";
    }



    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "" EZTR_CC_THREE_CHOICE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_GREEN "No thanks" EZTR_CC_END "",        player_name,
        item_name,
        rupee_cost1,
        player_name2,
        item_name2,
        rupee_cost2
    );

    recomp_free(get_item_name);
    recomp_free(get_item_name2);
    recomp_free(get_player_name);
    recomp_free(get_player_name2);
}
EZTR_MSG_CALLBACK(randoTingleMilkRoad) {
    u32 tingleFirstItem = GI_TINGLE_MAP_ROMANI_RANCH;
    char* player_name;
    char* get_player_name;
    char* item_name;
    char* get_item_name;
    u32 tingleSecondItem = GI_TINGLE_MAP_GREAT_BAY;
    char* player_name2;
    char* get_player_name2;
    char* item_name2;
    char* get_item_name2;

    char* rupee_cost1;
    char* rupee_cost2;

    
    rando_get_location_item_player(tingleFirstItem, &get_player_name);
    rando_get_location_item_name(tingleFirstItem, &get_item_name);
    rando_get_location_item_player(tingleSecondItem, &get_player_name2);
    rando_get_location_item_name(tingleSecondItem, &get_item_name2);

    if (rando_location_is_checked(tingleFirstItem) == true) {player_name = "";
    } else if (rando_get_location_has_local_item(tingleFirstItem) == true) {player_name = "";
    } else {player_name = get_player_name;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {item_name = "SOLD OUT";
    } else {item_name = get_item_name;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {player_name2 = "";
    } else if (rando_get_location_has_local_item(tingleSecondItem) == true) {player_name2 = "";
    } else {player_name2 = get_player_name2;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {item_name2 = "SOLD OUT";
    } else {item_name2 = get_item_name2;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {rupee_cost1 = "";
    } else {rupee_cost1 = "20 Rupees";
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {rupee_cost2 = "";
    } else {rupee_cost2 = "40 Rupees";
    }



    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "" EZTR_CC_THREE_CHOICE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_GREEN "No thanks" EZTR_CC_END "",        player_name,
        item_name,
        rupee_cost1,
        player_name2,
        item_name2,
        rupee_cost2
    );

    recomp_free(get_item_name);
    recomp_free(get_item_name2);
    recomp_free(get_player_name);
    recomp_free(get_player_name2);
}
EZTR_MSG_CALLBACK(randoTingleGreatBay) {
    u32 tingleFirstItem = GI_TINGLE_MAP_GREAT_BAY;
    char* player_name;
    char* get_player_name;
    char* item_name;
    char* get_item_name;
    u32 tingleSecondItem = GI_TINGLE_MAP_STONE_TOWER;
    char* player_name2;
    char* get_player_name2;
    char* item_name2;
    char* get_item_name2;

    char* rupee_cost1;
    char* rupee_cost2;

    
    rando_get_location_item_player(tingleFirstItem, &get_player_name);
    rando_get_location_item_name(tingleFirstItem, &get_item_name);
    rando_get_location_item_player(tingleSecondItem, &get_player_name2);
    rando_get_location_item_name(tingleSecondItem, &get_item_name2);

    if (rando_location_is_checked(tingleFirstItem) == true) {player_name = "";
    } else if (rando_get_location_has_local_item(tingleFirstItem) == true) {player_name = "";
    } else {player_name = get_player_name;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {item_name = "SOLD OUT";
    } else {item_name = get_item_name;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {player_name2 = "";
    } else if (rando_get_location_has_local_item(tingleSecondItem) == true) {player_name2 = "";
    } else {player_name2 = get_player_name2;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {item_name2 = "SOLD OUT";
    } else {item_name2 = get_item_name2;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {rupee_cost1 = "";
    } else {rupee_cost1 = "20 Rupees";
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {rupee_cost2 = "";
    } else {rupee_cost2 = "40 Rupees";
    }



    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "" EZTR_CC_THREE_CHOICE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_GREEN "No thanks" EZTR_CC_END "",        player_name,
        item_name,
        rupee_cost1,
        player_name2,
        item_name2,
        rupee_cost2
    );

    recomp_free(get_item_name);
    recomp_free(get_item_name2);
    recomp_free(get_player_name);
    recomp_free(get_player_name2);
}
EZTR_MSG_CALLBACK(randoTingleStoneTower) {
    u32 tingleFirstItem = GI_TINGLE_MAP_STONE_TOWER;
    char* player_name;
    char* get_player_name;
    char* item_name;
    char* get_item_name;
    u32 tingleSecondItem = GI_TINGLE_MAP_CLOCK_TOWN;
    char* player_name2;
    char* get_player_name2;
    char* item_name2;
    char* get_item_name2;

    char* rupee_cost1;
    char* rupee_cost2;

    
    rando_get_location_item_player(tingleFirstItem, &get_player_name);
    rando_get_location_item_name(tingleFirstItem, &get_item_name);
    rando_get_location_item_player(tingleSecondItem, &get_player_name2);
    rando_get_location_item_name(tingleSecondItem, &get_item_name2);

    if (rando_location_is_checked(tingleFirstItem) == true) {player_name = "";
    } else if (rando_get_location_has_local_item(tingleFirstItem) == true) {player_name = "";
    } else {player_name = get_player_name;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {item_name = "SOLD OUT";
    } else {item_name = get_item_name;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {player_name2 = "";
    } else if (rando_get_location_has_local_item(tingleSecondItem) == true) {player_name2 = "";
    } else {player_name2 = get_player_name2;
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {item_name2 = "SOLD OUT";
    } else {item_name2 = get_item_name2;
    }
    if (rando_location_is_checked(tingleFirstItem) == true) {rupee_cost1 = "";
    } else {rupee_cost1 = "20 Rupees";
    }
    if (rando_location_is_checked(tingleSecondItem) == true) {rupee_cost2 = "";
    } else {rupee_cost2 = "40 Rupees";
    }



    EZTR_MsgSContent_Sprintf(
        buf->data.content,
        "" EZTR_CC_THREE_CHOICE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_LIGHTBLUE "%s " EZTR_CC_COLOR_GREEN "%s" EZTR_CC_COLOR_RED " %s" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_GREEN "No thanks" EZTR_CC_END "",        player_name,
        item_name,
        rupee_cost1,
        player_name2,
        item_name2,
        rupee_cost2
    );

    recomp_free(get_item_name);
    recomp_free(get_item_name2);
    recomp_free(get_player_name);
    recomp_free(get_player_name2);
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
    if (shopItemId == SI_POTION_BLUE) {
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

EZTR_MSG_CALLBACK(randoScrub) {
    
}

EZTR_MSG_CALLBACK(randoMilkBar) {
    
}

EZTR_MSG_CALLBACK(WoodfallStrayFairyCount) {
    u8 fairy_count = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_WOODFALL);
    u8 fairy_required = rando_get_slotdata_u32("required_stray_fairies");
    u8 fairy_remaining = fairy_required - fairy_count;
        if (fairy_count >= fairy_required) {
            if (fairy_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_PINK "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else if (fairy_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_PINK "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else if (fairy_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_PINK "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_PINK "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            }
        } else if (fairy_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else if (fairy_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else if (fairy_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        }
}
EZTR_MSG_CALLBACK(SnowheadStrayFairyCount) {
    u8 fairy_count = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_SNOWHEAD);
    u8 fairy_required = rando_get_slotdata_u32("required_stray_fairies");
    u8 fairy_remaining = fairy_required - fairy_count;
        if (fairy_count >= fairy_required) {
            if (fairy_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else if (fairy_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else if (fairy_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            }
        } else if (fairy_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else if (fairy_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else if (fairy_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        }
}
EZTR_MSG_CALLBACK(GreatBayStrayFairyCount) {
    u8 fairy_count = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_GREATBAY);
    u8 fairy_required = rando_get_slotdata_u32("required_stray_fairies");
    u8 fairy_remaining = fairy_required - fairy_count;
        if (fairy_count >= fairy_required) {
            if (fairy_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else if (fairy_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else if (fairy_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            }
        } else if (fairy_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else if (fairy_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else if (fairy_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        }
}
EZTR_MSG_CALLBACK(StoneTowerStrayFairyCount) {
    u8 fairy_count = rando_has_item(AP_ITEM_ID_STRAY_FAIRY_STONETOWER);
    u8 fairy_required = rando_get_slotdata_u32("required_stray_fairies");
    u8 fairy_remaining = fairy_required - fairy_count;
        if (fairy_count >= fairy_required) {
            if (fairy_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_YELLOW "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else if (fairy_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_YELLOW "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else if (fairy_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_YELLOW "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_YELLOW "Great Fairy " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            fairy_count
            );
            }
        } else if (fairy_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else if (fairy_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else if (fairy_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            fairy_count,
            fairy_remaining
            );
        }
}
EZTR_MSG_CALLBACK(SnowheadSmallKeyCount) {
    u8 key_count = rando_has_item(AP_ITEM_ID_KEY_SMALL_SNOWHEAD);
    if (key_count == 1) {
        EZTR_MsgSContent_Sprintf(buf->data.content,
        "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        key_count);
    } else if (key_count == 2) {
        EZTR_MsgSContent_Sprintf(buf->data.content,
        "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        key_count);
    } else if (key_count == 3) {
        EZTR_MsgSContent_Sprintf(buf->data.content,
        "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        key_count);
    }
}
EZTR_MSG_CALLBACK(StoneTowerSmallKeyCount) {
    u8 key_count = rando_has_item(AP_ITEM_ID_KEY_SMALL_STONETOWER);
    if (key_count == 1) {
        EZTR_MsgSContent_Sprintf(buf->data.content,
        "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        key_count);
    } else if (key_count == 2) {
        EZTR_MsgSContent_Sprintf(buf->data.content,
        "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        key_count);
    } else if (key_count == 3) {
        EZTR_MsgSContent_Sprintf(buf->data.content,
        "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        key_count);
    } else {
        EZTR_MsgSContent_Sprintf(buf->data.content,
        "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        key_count);
    }
}
EZTR_MSG_CALLBACK(SwampSpiderTokenCount) {
    u8 token_count = rando_has_item(GI_TRUE_SKULL_TOKEN);
    u8 token_required = rando_get_slotdata_u32("required_skull_tokens");
    u8 token_remaining = token_required - token_count;
    if (token_count >= token_required) {
        if (token_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else if (token_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else if (token_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else if (token_count == 21) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else if (token_count == 22) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else if (token_count == 23) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_GREEN "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        }
        } else if (token_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else if (token_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else if (token_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else if (token_count == 21) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else if (token_count == 22) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else if (token_count == 23) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_GREEN "Swamp " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        }
}
EZTR_MSG_CALLBACK(OceanSpiderTokenCount) {
    u8 token_count = rando_has_item(GI_OCEAN_SKULL_TOKEN);
    u8 token_required = rando_get_slotdata_u32("required_skull_tokens");
    u8 token_remaining = token_required - token_count;
    if (token_count >= token_required) {
        if (token_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else if (token_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else if (token_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else if (token_count == 21) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else if (token_count == 22) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else if (token_count == 23) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Head to the " EZTR_CC_COLOR_BLUE "Spider House " EZTR_CC_COLOR_DEFAULT "for your" EZTR_CC_NEWLINE "reward if you haven't already." EZTR_CC_END "",
            token_count);
        }
        } else if (token_count == 1) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else if (token_count == 2) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else if (token_count == 3) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else if (token_count == 21) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dst" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else if (token_count == 22) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dnd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else if (token_count == 23) {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%drd" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        } else {
            EZTR_MsgSContent_Sprintf(buf->data.content,
            "You got a " EZTR_CC_COLOR_BLUE "Ocean " EZTR_CC_COLOR_RED "Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED "%dth" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_NEWLINE "Only " EZTR_CC_COLOR_RED "%d" EZTR_CC_COLOR_DEFAULT " more left to find!" EZTR_CC_END "",
            token_count,
            token_remaining
            );
        }
}

// honestly don't know, but seems important
// char* item_str;
// char* player_str;

// rando_get_location_item_name(rando_get_last_location_sent(), &item_str);
// rando_get_location_item_player(rando_get_last_location_sent(), &player_str);
// sanitizeRandoText(item_str);
// sanitizeRandoText(player_str);


// text sanitize stolen from old file. Not sure what role this plays exactly, but I know it fixes crashes on funky letters
// commented out because this function is already in use elsewhere
// void sanitizeRandoText(char* rando_string) {
//     u8 c = rando_string[0];
//     u8 next = 0;
//     u8 i = 0;
//     bool shift_string = false;

//     while (c != 0) {
//         if (c <= 0x08 || (c >= 0x0A && c <= 0x1F) || (c >= 0xB0 && c <= 0xBB) || (c >= 0xBF && c <= 0xE8) || (c >= 0xF0 && c <= 0xFF)) {
//             next = rando_string[i+1];
//             if (c == 0xC3 && next == 0xA1) { // á
//                 rando_string[i] = 0x98;
//                 shift_string = true;
//             } else {
//                 rando_string[i] = 0xAE; // replace all invalid bytes with ¿
//             }
//         }

//         if (shift_string) {
//             u8 new_i = i + 1;
//             u8 new_c = rando_string[new_i];
//             u8 new_next = rando_string[new_i + 1];
//             while (new_c != 0) {
//                 rando_string[new_i] = new_next;
//                 new_i++;
//                 new_c = rando_string[new_i];
//                 new_next = rando_string[new_i + 1];
//             }
//             shift_string = false;
//         }

//         i++;
//         c = rando_string[i];
//     }
// }





// text replacements for AP items. Not yet set up. Probably won't use but keeping here just in case.
// EZTR_MSG_CALLBACK(randoAPSend) {
//     buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
//     EZTR_MsgSContent_Sprintf(buf->data.content, "You found " EZTR_CC_COLOR_RED "\xFE" EZTR_CC_COLOR_DEFAULT "'s" EZTR_CC_NEWLINE "" RANDO_AP_COLOR "\xFF" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END ""),
        
            
// }

// text replacements for pictograph box
EZTR_MSG_CALLBACK(randoPictograph) {
    if (!CHECK_QUEST_ITEM(QUEST_PICTOGRAPH)) {
        Snap_RecordPictographedActors(play);
    }

    if (Snap_CheckFlag(PICTO_VALID_MONKEY)) {
        buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of a monkey" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
    } else if (Snap_CheckFlag(PICTO_VALID_BIG_OCTO)) {
        buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of a Big Octo" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
    } else if (Snap_CheckFlag(PICTO_VALID_SCARECROW)) {
        buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of a scarecrow" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
    } else if (Snap_CheckFlag(PICTO_VALID_TINGLE)) {
        buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of Tingle" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
    } else if (Snap_CheckFlag(PICTO_VALID_DEKU_KING)) {
        buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of the Deku King" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
    } else if (Snap_CheckFlag(PICTO_VALID_PIRATE_GOOD)) {
        buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "good picture of a pirate" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
    } else if (Snap_CheckFlag(PICTO_VALID_PIRATE_TOO_FAR)) {
        buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "bad picture of a pirate" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
    } else if (Snap_CheckFlag(PICTO_VALID_LULU_HEAD)) {
        if (Snap_CheckFlag(PICTO_VALID_LULU_RIGHT_ARM) && Snap_CheckFlag(PICTO_VALID_LULU_LEFT_ARM)) {
            buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
            EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "good picture of Lulu" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
        } else {
            buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
            EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "bad picture of Lulu" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
        }
    } else if (Snap_CheckFlag(PICTO_VALID_IN_SWAMP)) {
        buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of the swamp" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
    }
    else {
        buf->data.text_box_type = EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
    }
}

// Replacements of existing IDs
EZTR_ON_INIT void init_text() {
    EZTR_Basic_ReplaceText(
        0x353C, // Fast Dog
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "..." EZTR_CC_END "",
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
        "" EZTR_CC_SFX "|29|13Hoo-whine." EZTR_CC_NEWLINE "How can any of us win against..." EZTR_CC_NEWLINE "" EZTR_CC_COLOR_BLUE "blue dog" EZTR_CC_COLOR_DEFAULT "..." EZTR_CC_END "",
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
    //     "Use the " EZTR_CC_COLOR_RED "!hint" EZTR_CC_COLOR_DEFAULT " command to hint" EZTR_CC_NEWLINE "for an item!" EZTR_CC_END "",
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
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoPictograph
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
        randoTingleClockTown
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
        randoTingleWoodfall
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
        randoTingleSnowhead
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
        randoTingleMilkRoad
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
        randoTingleGreatBay
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
        randoTingleStoneTower
    );


    // Custom Text IDs
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_Send_Item),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_Self_Item),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Songs),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoGISongs
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Magic),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_SpinAttack),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You mastered the " EZTR_CC_COLOR_RED "Spin Attack" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_DoubleDefense),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_HEART_CONTAINER,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "Your " EZTR_CC_COLOR_RED "defense" EZTR_CC_COLOR_DEFAULT " has been" EZTR_CC_NEWLINE "strengthened!" EZTR_CC_NEWLINE "Enemies now do half as much" EZTR_CC_NEWLINE "damage as before!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Souls),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Frogs),
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

    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Kokiri_Sword),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_KOKIRI_SWORD,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_RED "Kokiri Sword" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Bombchu_Bag),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_BOMBCHU,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a " EZTR_CC_COLOR_RED "Bombchu Bag" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_CTSF),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_STRAY_FAIRY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_ORANGE "Clock Town" EZTR_CC_NEWLINE "" EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_WFSF),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_SHSF),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBSF),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_STSF),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_WFBK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_BOSS_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Boss Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_SHBK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_BOSS_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Boss Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBBK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_BOSS_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Boss Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_STBK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_BOSS_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Boss Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_WFSK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_SMALL_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_SHSK),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBSK),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_SMALL_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_STSK),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_WFMap),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_DUNGEON_MAP,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Map" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_SHMap),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_DUNGEON_MAP,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Map" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBMap),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_DUNGEON_MAP,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Map" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBMap),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_DUNGEON_MAP,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Map" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_WFCompass),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_COMPASS,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Compass" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_SHCompass),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_COMPASS,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Compass" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBCompass),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_COMPASS,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Compass" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBCompass),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_COMPASS,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Compass" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Swamp_Token),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Ocean_Token),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_FOOL),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You are a" EZTR_CC_COLOR_RED "FOOL" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_Moon_Child_Return),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        1,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "...But you are not strong enough..." EZTR_CC_CARRIAGE_RETURN "" EZTR_CC_BOX_BREAK2 "Shall... I send you back?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "",
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_Scrub),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoScrub
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_Milk_Bar),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoMilkBar
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_OwlStatue),
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Owl_Hidden),
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the" EZTR_CC_COLOR_RED " Hidden Owl Statue" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "You can now " EZTR_CC_NEWLINE "" EZTR_CC_COLOR_RED "Index Warp" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
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
    sGetItemTable_ap[GI_SONG_TIME - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Songs)); // Songs
    sGetItemTable_ap[GI_SONG_HEALING - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Songs));
    sGetItemTable_ap[GI_SONG_EPONA - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Songs));
    sGetItemTable_ap[GI_SONG_SOARING - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Songs));
    sGetItemTable_ap[GI_SONG_STORMS - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Songs));
    sGetItemTable_ap[GI_SONG_SONATA - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Songs));
    sGetItemTable_ap[GI_SONG_LULLABY - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Songs));
    sGetItemTable_ap[GI_SONG_NOVA - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Songs));
    sGetItemTable_ap[GI_SONG_ELEGY - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Songs));
    sGetItemTable_ap[GI_SONG_OATH - 1].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Songs));
}
