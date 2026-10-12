//#include "apcommon.h"
//#include "eztr_api.h"
//#include "modding.h"

//EZTR_MSG_CALLBACK(cursed_spider_first_time_callback) {
//    if ((s16) rando_get_slotdata_u32("shuffle_spiderhouse_reward") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content, "I have nothing to give you." EZTR_CC_NEWLINE "Go away." EZTR_CC_EVENT2 EZTR_CC_END);
//    }
//}
//EZTR_ON_INIT void cursed_spider_first_time_text() {
//    EZTR_Basic_ReplaceText(
//        0x910,                              // Text ID
//        EZTR_STANDARD_TEXT_BOX_I,           // Text box type
//        0,                                  // Text box Y axis offset
//        EZTR_ICON_NO_ICON,                  // Icon display
//        0x911,                              // Next message text ID
//        EZTR_NO_VALUE,                      // First item rupee cost
//        EZTR_NO_VALUE,                      // Second item rupee cose
//        false,
//        "Ah! Help me!" EZTR_CC_NEWLINE "I am not a monster!" EZTR_CC_NEWLINE
//        "The spider's curse..." EZTR_CC_NEWLINE "It made me this way..." EZTR_CC_CONTINUE
//        /*"I beg of you..." EZTR_CC_COLOR_RED "In here" EZTR_CC_COLOR_DEFAULT "...Find them" EZTR_CC_NEWLINE
//        EZTR_CC_COLOR_RED "all" EZTR_CC_COLOR_DEFAULT "...The" EZTR_CC_COLOR_RED "gold" EZTR_CC_COLOR_DEFAULT "ones...The cursed" EZTR_CC_NEWLINE
//        EZTR_CC_COLOR_RED "spiders" EZTR_CC_COLOR_DEFAULT "...Defeat them...Quickly!" EZTR_CC_EVENT2 EZTR_CC_END*/,
//        cursed_spider_first_time_callback   // Callback
//    );
//}
//
//EZTR_MSG_CALLBACK(curse_spider_recurring_callback) {
//    if ((s16) rando_get_slotdata_u32("shuffle_spiderhouse_reward") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "Do not bother lifting the curse." EZTR_CC_NEWLINE
//            "The mask is mine!" EZTR_CC_EVENT2 EZTR_CC_END
//        );
//    }
//}
//EZTR_ON_INIT void cursed_spider_recurring_text() {
//    EZTR_Basic_ReplaceText(
//        0x914,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        0x915,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        EZTR_CC_COLOR_RED "In here" EZTR_CC_COLOR_DEFAULT "...The gold ones...The cursed" EZTR_CC_NEWLINE
//        EZTR_CC_COLOR_RED "spiders" EZTR_CC_COLOR_DEFAULT "...Defeat them " EZTR_CC_COLOR_RED "all" EZTR_CC_COLOR_DEFAULT "...Make me" EZTR_CC_NEWLINE
//        "normal...again..." EZTR_CC_EVENT2 EZTR_CC_END,
//        curse_spider_recurring_callback
//    );
//}
//
//EZTR_MSG_CALLBACK(ssh_guy_first_time_callback) {
//    if ((s16) rando_get_slotdata_u32("shuffle_spiderhouse_reward") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "I am not giving you my mask." EZTR_CC_NEWLINE
//            "It's mine and mine only!" EZTR_CC_EVENT2 EZTR_CC_END
//        );
//    }
//}
//EZTR_ON_INIT void ssh_guy_first_time_text() {
//    EZTR_Basic_ReplaceText(
//        0x91B,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        "As soon as I calm down, I'm going" EZTR_CC_NEWLINE "to get rid of it." EZTR_CC_EVENT2 EZTR_CC_END,
//        ssh_guy_first_time_callback
//    );
//}
//
//EZTR_MSG_CALLBACK(ssh_guy_recurring_callback) {
//    if ((s16) rando_get_slotdata_u32("shuffle_spiderhouse_reward") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "Great, now my mask is gone. That" EZTR_CC_NEWLINE
//            "mask was giving me infinite riches."
//            EZTR_CC_BOX_BREAK "Now what?" EZTR_CC_EVENT2 EZTR_CC_END);
//    }
//}
//EZTR_ON_INIT void ssh_guy_recurring_text() {
//    EZTR_Basic_ReplaceText(
//        0x918,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        "I've had enough of this..." EZTR_CC_BOX_BREAK
//        "As soon as I get it together," EZTR_CC_NEWLINE
//        "I'm going home." EZTR_CC_EVENT2 EZTR_CC_END,
//        ssh_guy_recurring_callback
//    );
//}

//EZTR_MSG_CALLBACK(chest_game_init_as_human_callback) {
//    if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content, "Oh! A customer! Unfortunately," EZTR_CC_NEWLINE "we are under construction." EZTR_CC_CONTINUE);
//    }
//    else if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 1) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "Sorry, but we only accept" EZTR_CC_NEWLINE EZTR_CC_COLOR_RED "strong and burly" EZTR_CC_COLOR_DEFAULT "customers." EZTR_CC_CONTINUE
//        );
//    }
//}
//EZTR_ON_INIT void chest_game_init_as_human_text() {
//    EZTR_Basic_ReplaceText(
//        0x76D,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        0x771,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        "Come in! Oh, is the kid collecting" EZTR_CC_NEWLINE
//        "masks? How about it? Wanna" EZTR_CC_NEWLINE
//        "play?" EZTR_CC_CONTINUE,
//        NULL
//    );
//}

//EZTR_MSG_CALLBACK(chest_game_init_as_deku_callback) {
//    if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content, "Oh! A customer! Unfortunately," EZTR_CC_NEWLINE "we are under construction." EZTR_CC_CONTINUE);
//    }
//    else if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 1) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "Sorry, but we only accept" EZTR_CC_NEWLINE EZTR_CC_COLOR_RED "strong and burly" EZTR_CC_COLOR_DEFAULT "customers." EZTR_CC_CONTINUE
//        );
//    }
//}
//EZTR_ON_INIT void chest_game_init_as_deku_text() {
//    EZTR_Basic_ReplaceText(
//        0x76C,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        EZTR_NO_VALUE,//0x770,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        "Oh, what a cute little customer!" EZTR_CC_NEWLINE
//        "So, do you want to play?" EZTR_CC_CONTINUE,
//        chest_game_init_as_deku_callback
//    );
//}

//EZTR_MSG_CALLBACK(chest_game_init_as_goron_callback) {
//    if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content, "Oh! A customer! Unfortunately," EZTR_CC_NEWLINE "we are under construction." EZTR_CC_CONTINUE);
//    }
//}
//EZTR_ON_INIT void chest_game_init_as_goron_text() {
//    EZTR_Basic_ReplaceText(
//        0x76E,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        EZTR_NO_VALUE,//0x772,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        "Well, aren't you a fit fellow!" EZTR_CC_NEWLINE
//        "Wanna play?" EZTR_CC_CONTINUE,
//        chest_game_init_as_goron_callback
//    );
//}

//EZTR_MSG_CALLBACK(chest_game_init_as_zora_callback) {
//    if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content, "Oh! A customer! Unfortunately," EZTR_CC_NEWLINE "we are under construction." EZTR_CC_CONTINUE);
//    }
//    else if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 1) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "Sorry, but we only accept" EZTR_CC_NEWLINE EZTR_CC_COLOR_RED "strong and burly" EZTR_CC_COLOR_DEFAULT "customers." EZTR_CC_CONTINUE
//        );
//    }
//}
//EZTR_ON_INIT void chest_game_init_as_zora_text() {
//    EZTR_Basic_ReplaceText(
//        0x76F,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        EZTR_NO_VALUE,//0x773,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        "Well, hello there, handsome!" EZTR_CC_NEWLINE
//        "Wanna play?" EZTR_CC_CONTINUE,
//        chest_game_init_as_zora_callback
//    );
//}

//EZTR_MSG_CALLBACK(chest_game_selection_as_human_callback) {
//    Player* player = GET_PLAYER(play);
//    if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "We'll be opening " EZTR_CC_COLOR_RED "after the carnival" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_NEWLINE
//            "We hope to see you!" EZTR_CC_EVENT2 EZTR_CC_END
//        );
//    }
//    else if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 1 && player->transformation == !PLAYER_FORM_GORON) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "Looks like you're collecting " EZTR_CC_COLOR_RED "masks" EZTR_CC_COLOR_DEFAULT "!" EZTR_CC_NEWLINE
//            "There's " EZTR_CC_COLOR_RED "one I like" EZTR_CC_COLOR_DEFAULT ", but I don't" EZTR_CC_NEWLINE
//            "know if you have it..." EZTR_CC_EVENT2 EZTR_CC_END
//        );
//    }
//}
//EZTR_ON_INIT void chest_game_selection_as_human_text() {
//    EZTR_Basic_ReplaceText(
//        0x771,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        "For you, it's  " EZTR_CC_COLOR_PINK "20 Rupees" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_NEWLINE
//        EZTR_CC_NEWLINE
//        EZTR_CC_COLOR_GREEN EZTR_CC_TWO_CHOICE "OK" EZTR_CC_NEWLINE
//        "No thanks" EZTR_CC_EVENT2 EZTR_CC_END,
//        chest_game_selection_as_human_callback
//    );
//}
//
//EZTR_MSG_CALLBACK(chest_game_selection_as_deku_callback) {
//    Player* player = GET_PLAYER(play);
//    if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "We'll be opening " EZTR_CC_COLOR_RED "after the carnival" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_NEWLINE
//            "We hope to see you!" EZTR_CC_EVENT2 EZTR_CC_END
//        );
//    }
//    else if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 1 && player->transformation == !PLAYER_FORM_GORON) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "Which is unfortunate because you" EZTR_CC_NEWLINE
//            "are so cute!!!" EZTR_CC_EVENT2 EZTR_CC_END
//        );
//    }
//}
//EZTR_ON_INIT void chest_game_selection_as_deku_text() {
//    EZTR_Basic_ReplaceText(
//        0x770,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        "Which is unfortunate because you" EZTR_CC_NEWLINE
//        "are so cute!!!" EZTR_CC_EVENT2 EZTR_CC_END,
//        chest_game_selection_as_deku_callback
//    );
//}
//
//EZTR_ON_INIT void chest_game_selection_as_goron_text() {
//    EZTR_Basic_ReplaceText(
//        0x772,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        "We'll be opening " EZTR_CC_COLOR_RED "after the carnival" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_NEWLINE
//        "We hope to see you!" EZTR_CC_EVENT2 EZTR_CC_END,
//        NULL
//    );
//}
//
//EZTR_MSG_CALLBACK(chest_game_selection_as_zora_callback) {
//    Player* player = GET_PLAYER(play);
//    if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 0) {
//        EZTR_MsgSContent_Sprintf(buf->data.content,
//            "We'll be opening " EZTR_CC_COLOR_RED "after the carnival" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_NEWLINE
//            "We hope to see you!" EZTR_CC_EVENT2 EZTR_CC_END
//        );
//    }
//    else if ((s16) rando_get_slotdata_u32("shuffle_treasure_chest_game") == 1 && player->transformation == !PLAYER_FORM_GORON) {
//        EZTR_MsgSContent_Sprintf(buf->data.content, "It's too bad... You look like my type..." EZTR_CC_EVENT2 EZTR_CC_END);
//    }
//}
//EZTR_ON_INIT void chest_game_selection_as_zora_text() {
//    EZTR_Basic_ReplaceText(
//        0x773,
//        EZTR_STANDARD_TEXT_BOX_I,
//        0,
//        EZTR_ICON_NO_ICON,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        EZTR_NO_VALUE,
//        false,
//        "It's too bad... You look like my type..." EZTR_CC_EVENT2 EZTR_CC_END,
//        NULL
//    );
//}