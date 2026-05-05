#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"
#include "eztr_api.h"
#include "attributes.h"

#include "apcommon.h"

#include "z64snap.h"

//old defines that I didn't use
// #define RANDO_AP_ITEM "\xFF"
// #define RANDO_AP_PLAYER "\xFE"
// #define RANDO_AP_COLOR "\xFD"

//sets colours for AP item class. probably doesn't work
// u8 getAPItemColor(u32 location) {
//     switch (rando_get_location_type(location)) {
//         case 1:  return 0x05; // EZTR_CC_COLOR_LIGHTBLUE;  // progression - purple
//         case 2:  return 0x03; // EZTR_CC_COLOR_BLUE;  // useful - blue
//         case 3:  return 0x08; // EZTR_CC_COLOR_ORANGE;  // trap - orange
//         case 0:
//         default: return 0x07; // EZTR_CC_COLOR_SILVER;  // filler - grey
//     }
// }
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
//Get info from Rando to replace the text with item class colour, player name and item name
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
EZTR_MSG_CALLBACK(randoTingle) {
    
}
EZTR_MSG_CALLBACK(randoShop) {
    
}
EZTR_MSG_CALLBACK(randoScrub) {
    
}
EZTR_MSG_CALLBACK(randoMilkBar) {
    
}
//honestly don't know, but seems important
// char* item_str;
// char* player_str;

// rando_get_location_item_name(rando_get_last_location_sent(), &item_str);
// rando_get_location_item_player(rando_get_last_location_sent(), &player_str);
// sanitizeRandoText(item_str);
// sanitizeRandoText(player_str);


//text sanitize stolen from old file. Not sure what role this plays exactly, but I know it fixes crashes on funky letters
//commented out because this function is already in use elsewhere
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

EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Send_Item);//"You sent [player] their [item]"
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Self_Item);//"You found your [item]"
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Kokiri_Sword);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Bombchu_Bag);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_CTSF);//stray fairies
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_WFSF);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SHSF);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_GBSF);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_STSF);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_WFBK);//boss keys
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SHBK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_GBBK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_STBK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_WFSK);//small keys
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SHSK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_GBSK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_STSK);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_WFMap);//maps
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SHMap);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_GBMap);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_STMap);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_WFCompass);//compasses
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_SHCompass);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_GBCompass);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_STCompass);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Swamp_Token);//skull tokens
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_Ocean_Token);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_GI_FOOL);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Moon_Child_Return);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Tingle);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Shop);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Scrub);
EZTR_DEFINE_CUSTOM_MSG_HANDLE(Rando_Milk_Bar);


//text replacements for AP items. Not yet set up. Probably won't use but keeping here just in case.
// EZTR_MSG_CALLBACK(randoAPSend) {
//     buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
//     EZTR_MsgSContent_Sprintf(buf->data.content, "You found " EZTR_CC_COLOR_RED "\xFE" EZTR_CC_COLOR_DEFAULT "'s" EZTR_CC_NEWLINE "" RANDO_AP_COLOR "\xFF" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END ""),
        
            
// }

//text replacements for pictograph box
EZTR_MSG_CALLBACK(randoPictograph) {
            if (!CHECK_QUEST_ITEM(QUEST_PICTOGRAPH)) {
                Snap_RecordPictographedActors(play);
            }

            if (Snap_CheckFlag(PICTO_VALID_MONKEY)) {
                buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of a monkey" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            } else if (Snap_CheckFlag(PICTO_VALID_BIG_OCTO)) {
                buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of a Big Octo" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            } else if (Snap_CheckFlag(PICTO_VALID_SCARECROW)) {
                buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of a scarecrow" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            } else if (Snap_CheckFlag(PICTO_VALID_TINGLE)) {
                buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of Tingle" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            } else if (Snap_CheckFlag(PICTO_VALID_DEKU_KING)) {
                buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of the Deku King" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            } else if (Snap_CheckFlag(PICTO_VALID_PIRATE_GOOD)) {
                buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "good picture of a pirate" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            } else if (Snap_CheckFlag(PICTO_VALID_PIRATE_TOO_FAR)) {
                buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "bad picture of a pirate" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            } else if (Snap_CheckFlag(PICTO_VALID_LULU_HEAD)) {
                if (Snap_CheckFlag(PICTO_VALID_LULU_RIGHT_ARM) && Snap_CheckFlag(PICTO_VALID_LULU_LEFT_ARM)) {
                    buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "good picture of Lulu" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            } else {
                    buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "bad picture of Lulu" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            }
            } else if (Snap_CheckFlag(PICTO_VALID_IN_SWAMP)) {
                buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture of the swamp" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            }
            else {
                buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II,
                EZTR_MsgSContent_Sprintf(buf->data.content, "Keep this " EZTR_CC_COLOR_RED "picture" EZTR_CC_COLOR_DEFAULT "?" EZTR_CC_COLOR_GREEN "" EZTR_CC_NEWLINE "" EZTR_CC_NEWLINE "" EZTR_CC_TWO_CHOICE "Yes" EZTR_CC_NEWLINE "No" EZTR_CC_END "");
            }
}
extern GetItemEntryAP sGetItemTable_ap[];
//Replacements of existing IDs
EZTR_ON_INIT void init_text() {
    EZTR_Basic_ReplaceText(
        0x1B9E,//Sonata of Awakening
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_GREEN "Sonata of Awakening" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x1B9F,//Goron's Lullaby
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_RED "Goron's Lullaby" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x1BA0,//New Wave Bossa Nova
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_BLUE "New Wave Bossa Nova" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x1BA1,//Elegy of Emptiness
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_ORANGE "Elegy of Emptiness" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x1BA2,//Oath to Order
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_PINK "Oath to Order" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x1BA4,//Song of Time
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_LIGHTBLUE "Song of Time" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x1BA5,//Song of Healing
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_PINK "Song of Healing" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x1BA6,//Epona's Song
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got " EZTR_CC_COLOR_ORANGE "Epona's Song" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x1BA7,//Song of Soaring
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_RED "Song of Soaring" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x1BA6,//Song of Storms
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_RED "Song of Storms" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x00C8,//Magic Power 1
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_SMALL_MAGIC_JAR,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You've been granted " EZTR_CC_COLOR_GREEN "Magic Power" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x00CA,//Spin Attack Upgrade
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You mastered the " EZTR_CC_COLOR_RED "Spin Attack" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x353C,//Fast Dog
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
        0x3545,//Slow Dog
        EZTR_TRANSLUSCENT_BLUE_TEXT_BOX,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "" EZTR_CC_SFX "|29|13Hoo-whine." EZTR_CC_NEWLINE "How can any of us win against..." EZTR_CC_NEWLINE "" EZTR_CC_COLOR_BLUE "blue god" EZTR_CC_COLOR_DEFAULT "..." EZTR_CC_END "",
        NULL
    );
    
    // EZTR_Basic_ReplaceText(
    //     0x20D0,//Gossip Hint joke
    //     EZTR_STANDARD_TEXT_BOX_II,
    //     0,
    //     EZTR_ICON_NO_ICON,
    //     EZTR_NO_VALUE,
    //     EZTR_NO_VALUE,
    //     EZTR_NO_VALUE,
    //     false,
    //     "Use the " EZTR_CC_COLOR_RED "!hint" EZTR_CC_COLOR_DEFAULT " command to hint" EZTR_CC_NEWLINE "for an item!" EZTR_CC_END "",
    //     NULL
    // );
    EZTR_Basic_ReplaceText(
        0x20D0,//Replaces the Gossip Stone Tatl text to test if it can read the AP items correctly without fully implimenting it on get item.
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoAPSend
    );
    EZTR_Basic_ReplaceText(
        0x0090,//Replaces rando text for filler AP items.
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "No, that didn't work. Try again.",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x00B3,//Replaces rando text for useful AP items.
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "No, that didn't work. Try again.",
        NULL
    );
    EZTR_Basic_ReplaceText(
        0x0077,//Replaces rando text for progression AP items.
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "No, that didn't work. Try again.",
        randoAPSend
    );
    //Pictograph Box text
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


    //Custom IDs for items not normally obtained
    //AP send item text. Probably doesn't work.
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_Send_Item),
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoAPSelf
    );

    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Kokiri_Sword),
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_STRAY_FAIRY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED EZTR_CC_STRAY_FAIRIES "" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_SHSF),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_STRAY_FAIRY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED EZTR_CC_STRAY_FAIRIES "" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBSF),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_STRAY_FAIRY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED EZTR_CC_STRAY_FAIRIES "" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBSF),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_STRAY_FAIRY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Stray Fairy" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE "This is your " EZTR_CC_COLOR_RED EZTR_CC_STRAY_FAIRIES "" EZTR_CC_COLOR_DEFAULT " one." EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_WFBK),
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_BOSS_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got the " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Boss Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBBK),
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_SMALL_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a " EZTR_CC_COLOR_PINK "Woodfall " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_SHSK),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_SMALL_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a " EZTR_CC_COLOR_GREEN "Snowhead " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBSK),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_SMALL_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a " EZTR_CC_COLOR_BLUE "Great Bay " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_GBSK),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_SMALL_KEY,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a " EZTR_CC_COLOR_YELLOW "Stone Tower " EZTR_CC_COLOR_RED "Small Key" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_WFMap),
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
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
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_GOLD_SKULLTULA_TOKEN,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got a "EZTR_CC_COLOR_RED "Swamp Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_Ocean_Token),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_GOLD_SKULLTULA_TOKEN,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "You got an "EZTR_CC_COLOR_RED "Ocean Skulltula Token" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_END "",
        NULL
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_GI_FOOL),
        EZTR_STANDARD_TEXT_BOX_II,
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
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_Tingle),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoTingle
    );
    EZTR_Basic_AddCustomText(EZTR_HNAME(Rando_Shop),
        EZTR_STANDARD_TEXT_BOX_II,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        randoShop
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
    sGetItemTable_ap[GI_SWORD_KOKIRI].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Kokiri_Sword));
    sGetItemTable_ap[GI_BAG_BOMBCHU].textId = EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(Rando_GI_Bombchu_Bag));
    // sGetItemTable_ap[GI_APLOGO_USEFUL].textId = EZTR_GET_ID_H(Rando_Send_Item);
    // sGetItemTable_ap[GI_APLOGO_FILLER].textId = EZTR_GET_ID_H(Rando_Send_Item);
    // sGetItemTable_ap[GI_APLOGO_PROG].textId = EZTR_GET_ID_H(Rando_Send_Item);
}
