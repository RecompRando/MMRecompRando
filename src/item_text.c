#include "modding.h"

#include "apcommon.h"

#include "z64snap.h"

void Message_OpenText(PlayState* play, u16 textId);

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

RECOMP_PATCH void Message_StartTextbox(PlayState* play, u16 textId, Actor* actor) {
    MessageContext* msgCtx = &play->msgCtx;
    msgCtx->ocarinaAction = 0xFFFF;
    Message_OpenText(play, textId);
    msgCtx->talkActor = actor;
    msgCtx->msgMode = MSGMODE_TEXT_START;
    msgCtx->stateTimer = 0;
    msgCtx->textDelayTimer = 0;
    play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;
}

#undef SEGMENT_ROM_START
#define SEGMENT_ROM_START(segment) (_ ## segment ## SegmentRomStart)

extern u8 D_801C6A70;
extern s32 sCharTexSize;
extern s32 sCharTexScale;
extern s32 D_801F6B08;

static unsigned char ap_msg[128] = "You got an\x01 AP item\x00!\xbf";
static unsigned char ctsf_msg[128] = "You found\x08 Clock Town's\x06 Stray Fairy\x00!\xbf";
static unsigned char wfsf_msg[128] = "You found a\x02 Woodfall\x06 Stray Fairy\x00!\xbf";
static unsigned char shsf_msg[128] = "You found a\x05 Snowhead\x06 Stray Fairy\x00!\xbf";
static unsigned char gbsf_msg[128] = "You found a\x03 Great Bay\x06 Stray Fairy\x00!\xbf";
static unsigned char stsf_msg[128] = "You found a\x04 Stone Tower\x06 Stray Fairy\x00!\xbf";
static unsigned char sot_msg[128] = "You got the\x05 Song of Time\x00!\xbf";
static unsigned char soh_msg[128] = "You got the\x05 Song of Healing\x00!\xbf";
static unsigned char es_msg[128] = "You got\x05 Epona's Song\x00!\xbf";
static unsigned char sost_msg[128] = "You got the\x05 Song of Storms\x00!\xbf";
static unsigned char sos_msg[128] = "You got the\x05 Song of Soaring\x00!\xbf";
static unsigned char soa_msg[128] = "You got the\x05 Sonata of Awakening\x00!\xbf";
static unsigned char gl_msg[128] = "You got the\x05 Goron's Lullaby\x00!\xbf";
static unsigned char nwbn_msg[128] = "You got the\x05 New Wave Bossa Nova\x00!\xbf";
static unsigned char eoe_msg[128] = "You got the\x05 Elegy of Emptiness\x00!\xbf";
static unsigned char oto_msg[128] = "You got the\x05 Oath to Order\x00!\xbf";
static unsigned char ssht_msg[128] = "You got a\x02 Swamp Token\x00!\xbf";
static unsigned char osht_msg[128] = "You got an\x03 Ocean Token\x00!\xbf";
static unsigned char sword_msg[128] = "You got the\x01 Kokiri Sword\x00!\xbf";
static unsigned char spin_msg[128] = "You mastered the\x01 Spin Attack\x00!\xbf";
static unsigned char sk_msg[128] = "You got a\x01 Small Key\x00! \xbf";
static unsigned char bk_msg[128] = "You got the\x01 Boss Key\x00! \xbf";
static unsigned char map_msg[128] = "You found a\x01 Map\x00! \xbf";
static unsigned char compass_msg[128] = "You found a\x01 Compass\x00! \xbf";

static unsigned char p_monkey_msg[128] = "Keep this\x01 picture of a monkey\x00?\x02\x11\x11\xc2Yes\x11No\xbf";
static unsigned char p_big_octo_msg[128] = "Keep this\x01 picture of an Octorok\x00?\x02\x11\x11\xc2Yes\x11No\xbf";
static unsigned char p_lulu_good_msg[128] = "Keep this\x01 good picture of Lulu\x00?\x02\x11\x11\xc2Yes\x11No\xbf";
static unsigned char p_lulu_bad_msg[128] = "Keep this\x01 bad picture of Lulu\x00?\x02\x11\x11\xc2Yes\x11No\xbf";
static unsigned char p_scarecrow_msg[128] = "Keep this\x01 picture of a scarecrow\x00?\x02\x11\x11\xc2Yes\x11No\xbf";
static unsigned char p_tingle_msg[128] = "Keep this\x01 picture of Tingle\x00?\x02\x11\x11\xc2Yes\x11No\xbf";
static unsigned char p_deku_king_msg[128] = "Keep this\x01 picture of the Deku King\x00?\x02\x11\x11\xc2Yes\x11No\xbf";
static unsigned char p_swamp_msg[128] = "Keep this\x01 picture of the swamp\x00?\x02\x11\x11\xc2Yes\x11No\xbf";
static unsigned char p_pirate_good_msg[128] = "Keep this\x01 good picture of a pirate\x00?\x02\x11\x11\xc2Yes\x11No\xbf";
static unsigned char p_pirate_bad_msg[128] = "Keep this\x01 bad picture of a pirate\x00?\x02\x11\x11\xc2Yes\x11No\xbf";

static unsigned char slow_dog_msg[128] = "Hoo-whine.\x11How can any of us win against...\x10.\x0a.\x0a." "\x03" "blue dog" "\x00" "?\xbf";
static unsigned char fast_dog_msg[128] = "\x0a\x0a\x0a\x0a\x0a\x0a.\x0a.\x0a.\x0a\x0a\x0a\x0a\xbf";

void Message_FindMessage(PlayState* play, u16 textId);

RECOMP_PATCH void Message_OpenText(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;
    Player* player = GET_PLAYER(play);
    f32 var_fv0;
    uintptr_t i;
    u32 ffcount = 0;
    unsigned char* msg = NULL;

    if (textId == 0x52) {
        textId = 0x75;
    }

    if (play->msgCtx.msgMode == MSGMODE_NONE) {
        gSaveContext.prevHudVisibility = gSaveContext.hudVisibility;
    }

    if (textId == 0xFF) {
        func_80115844(play, DO_ACTION_STOP);
        play->msgCtx.hudVisibility = gSaveContext.hudVisibility;
        Interface_SetHudVisibility(HUD_VISIBILITY_A_B_C);
        gSaveContext.save.unk_06 = 20;
    } else if ((textId == 0x579) || (textId == 0x8D8)) {
        Interface_SetHudVisibility(HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE);
    } else if (((textId == 0x28) || (textId == 0x29) || (textId == 0x2A)) &&
               (player->transformation == PLAYER_FORM_DEKU)) {
        //! FAKE:
        if (msgCtx) {}
        textId = 0xC9;
    } else if (textId == 0x11) {
        if (gSaveContext.save.saveInfo.inventory.strayFairies[((void)0, gSaveContext.dungeonIndex)] == 0xF) {
            textId = 0xF3;
        }
    } else if ((textId == 0x92) && (play->sceneId == SCENE_KOEPONARACE)) {
        textId = 0xCD;
    } else if ((textId == 0xC) && ((GET_QUEST_HEART_PIECE_COUNT - 1) != 0)) {
        textId = GET_QUEST_HEART_PIECE_COUNT - 1;
        textId += 0xC4;
        if (textId == 0xC3) {
            textId = 0xC7;
        }
    } else if (textId == 0x37) {
        if (rando_get_unconverted_item_id(rando_get_last_location()) != 0x37) {
            textId = 0xCA;
        }
    }

    msgCtx->currentTextId = textId;

    if ((msgCtx->currentTextId == 0xC) || (msgCtx->currentTextId == 0xD) || (msgCtx->currentTextId == 0xC5) ||
        (msgCtx->currentTextId == 0xC6) || (msgCtx->currentTextId == 0xC7)) {
        Interface_SetHudVisibility(HUD_VISIBILITY_A_HEARTS_MAGIC_WITH_OVERWRITE);
    }

    msgCtx->messageHasSetSfx = false;
    D_801C6A70 = 0;
    msgCtx->textboxSkipped = false;
    msgCtx->textIsCredits = false;
    var_fv0 = 1.0f;

    if (play->pauseCtx.bombersNotebookOpen) {
        if (gSaveContext.options.language == LANGUAGE_JPN) {
            msgCtx->textCharScale = 1.4f;
            msgCtx->unk11FFC = 0x1E;
            msgCtx->unk11FF8 = 0x32;
            var_fv0 = 1.4;
        } else {
            msgCtx->textCharScale = 1.4f;
            msgCtx->unk11FFC = 0x16;
            msgCtx->unk11FF8 = 0x32;
            var_fv0 = 1.4;
        }
    } else if (textId >= 0x4E20) {
        msgCtx->textIsCredits = true;
        msgCtx->textCharScale = 0.85f;
        msgCtx->unk11FFC = 6;
        msgCtx->unk11FF8 = 0x14;
    } else if (gSaveContext.options.language == LANGUAGE_JPN) {
        msgCtx->textCharScale = 0.88f;
        msgCtx->unk11FFC = 0x12;
        msgCtx->unk11FF8 = 0x32;
    } else {
        msgCtx->textCharScale = 0.75f;
        msgCtx->unk11FFC = 0xC;
        msgCtx->unk11FF8 = 0x41;
    }

    sCharTexSize = msgCtx->textCharScale * 16.0f;
    sCharTexScale = 1024.0f / msgCtx->textCharScale;
    D_801F6B08 = 1024.0f / var_fv0;

    if (msgCtx->textIsCredits) {
        Message_FindCreditsMessage(play, textId);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, (uintptr_t) &SEGMENT_ROM_START(staff_message_data_static)[font->messageStart],
                            font->messageEnd);
    } else if (gSaveContext.options.language == LANGUAGE_JPN) {
        Message_FindMessage(play, textId);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, (uintptr_t) &SEGMENT_ROM_START(message_data_static)[font->messageStart],
                            font->messageEnd);
    } else {
        Message_FindMessageNES(play, textId);
        msgCtx->msgLength = font->messageEnd;
        DmaMgr_SendRequest0(&font->msgBuf, (uintptr_t) &SEGMENT_ROM_START(message_data_static)[font->messageStart],
                            font->messageEnd);
    }

    msgCtx->choiceNum = 0;
    msgCtx->textUnskippable = false;
    msgCtx->textboxEndType = TEXTBOX_ENDTYPE_00;
    msgCtx->textDrawPos = 0;
    msgCtx->msgBufPos = 0;
    msgCtx->decodedTextLen = 0;

    switch (textId) {
        case 0x37:
            msg = sword_msg;
            break;
        case 0xCA:
            msg = spin_msg;
            break;
        case 0x72:
            msg = osht_msg;
            break;
        case 0x75:
            msg = ssht_msg;
            break;
        case 0xA2:
            msg = sost_msg;
            break;
        case 0xA3:
            msg = sos_msg;
            break;
        case 0xA5:
            msg = es_msg;
            break;
        case 0xA7:
            msg = oto_msg;
            break;
        case 0xA8:
            msg = eoe_msg;
            break;
        case 0xAC:
            msg = nwbn_msg;
            break;
        case 0xAD:
            msg = gl_msg;
            break;
        case 0xAE:
            msg = soa_msg;
            break;
        case 0xAF:
            msg = soh_msg;
            break;
        case 0xB0:
            msg = sot_msg;
            break;
        case 0xB2:
            msg = ctsf_msg;
            break;
        case 0x46:
            msg = wfsf_msg;
            break;
        case 0x47:
            msg = shsf_msg;
            break;
        case 0x48:
            msg = gbsf_msg;
            break;
        case 0x49:
            msg = stsf_msg;
            break;
        case GI_AP_FILLER:
        case GI_AP_PROG:
        case GI_AP_USEFUL:
            msg = ap_msg;
            break;
        case 0xF8:
            Snap_RecordPictographedActors(play);
            if (Snap_CheckFlag(PICTO_VALID_MONKEY)) {
                msg = p_monkey_msg;
            } else if (Snap_CheckFlag(PICTO_VALID_BIG_OCTO)) {
                msg = p_big_octo_msg;
            } else if (Snap_CheckFlag(PICTO_VALID_SCARECROW)) {
                msg = p_scarecrow_msg;
            } else if (Snap_CheckFlag(PICTO_VALID_TINGLE)) {
                msg = p_tingle_msg;
            } else if (Snap_CheckFlag(PICTO_VALID_DEKU_KING)) {
                msg = p_deku_king_msg;
            } else if (Snap_CheckFlag(PICTO_VALID_PIRATE_GOOD)) {
                msg = p_pirate_good_msg;
            } else if (Snap_CheckFlag(PICTO_VALID_PIRATE_TOO_FAR)) {
                msg = p_pirate_bad_msg;
            } else if (Snap_CheckFlag(PICTO_VALID_LULU_HEAD)) {
                if (Snap_CheckFlag(PICTO_VALID_LULU_RIGHT_ARM) && Snap_CheckFlag(PICTO_VALID_LULU_LEFT_ARM)) {
                    msg = p_lulu_good_msg;
                } else {
                    msg = p_lulu_bad_msg;
                }
            } else if (Snap_CheckFlag(PICTO_VALID_IN_SWAMP)) {
                msg = p_swamp_msg;
            }
            break;
        case 0x353C:
            msg = fast_dog_msg;
            break;
        case 0x3545:
            msg = slow_dog_msg;
            break;
        case 0x3C:
            msg = sk_msg;
            break;
        case 0x3D:
            msg = bk_msg;
            break;
        case 0x3E:
            msg = map_msg;
            break;
        case 0x3F:
            msg = compass_msg;
            break;
    }

    if (msg != NULL) {
        font->msgBuf.schar[0] = 0x02;
        font->msgBuf.schar[1] = 0x00;
        font->msgBuf.schar[2] = 0xFE;
        for (i = 0; i < 128; ++i) {
            font->msgBuf.schar[i + 11] = msg[i];
            if (msg[i] == 0xBF) {
                break;
            }
        }
    }

    if (textId == 0xF8) {
        font->msgBuf.schar[0] = 0x06;
        font->msgBuf.schar[1] = 0x71;
    }

    if (msg == sk_msg || msg == bk_msg || msg == map_msg || msg == compass_msg) {
        u8* dungeon_msg;
        u8 wf_str[128] = "\x02(Woodfall)\x00\xbf";
        u8 sh_str[128] = "\x05(Snowhead)\x00\xbf";
        u8 gb_str[128] = "\x03(Great Bay)\x00\xbf";
        u8 st_str[128] = "\x04(Stone Tower)\x00\xbf";

        s16 dungeonId = rando_get_unconverted_item_id(rando_get_last_location()) & 0xF00;
        switch (dungeonId) {
            case 0x000:
                dungeon_msg = wf_str;
                break;
            case 0x100:
                dungeon_msg = sh_str;
                break;
            case 0x200:
                dungeon_msg = gb_str;
                break;
            case 0x300:
                dungeon_msg = st_str;
                break;
        }
        u8 end_i = i + 11;
        for (i = 0; i < 128; ++i) {
            font->msgBuf.schar[end_i + i] = dungeon_msg[i];
            if (dungeon_msg[i] == 0xBF) {
                break;
            }
        }

        // small key count
        if (msg == sk_msg) {
            u8 count_str[128] = "\x11This is your \xbf";
            u8* count_msg = count_str;
            u8 new_end_i = end_i + i;
            for (i = 0; i < 128; ++i) {
                font->msgBuf.schar[new_end_i + i] = count_msg[i];
                if (count_msg[i] == 0xBF) {
                    u8 key_count = DUNGEON_KEY_COUNT(dungeonId / 0x100);
                    u8 count_suffix[2] = "th";
                    if ((key_count % 10) == 1 && key_count != 11) {
                        count_suffix[0] = 's';
                        count_suffix[1] = 't';
                    } else if ((key_count % 10) == 2 && key_count != 12) {
                        count_suffix[0] = 'n';
                        count_suffix[1] = 'd';
                    } else if ((key_count % 10) == 3 && key_count != 13) {
                        count_suffix[0] = 'r';
                        count_suffix[1] = 'd';
                    }
                    font->msgBuf.schar[new_end_i + i] = 0x01;
                    i += 1;
                    if (key_count >= 10) {
                        font->msgBuf.schar[new_end_i + i] = (key_count / 10) + 0x30;
                        i += 1;
                    }
                    font->msgBuf.schar[new_end_i + i] = (key_count % 10) + 0x30;
                    font->msgBuf.schar[new_end_i + i + 1] = count_suffix[0];
                    font->msgBuf.schar[new_end_i + i + 2] = count_suffix[1];
                    font->msgBuf.schar[new_end_i + i + 3] = 0x00;
                    font->msgBuf.schar[new_end_i + i + 4] = '.';
                    font->msgBuf.schar[new_end_i + i + 5] = 0xBF;
                    break;
                }
            }
        }
    }
    

    if (msg == ssht_msg) {
        u8 count_str[128] = "\x11This is your \xbf";
        u8 count_done_str[128] = "\x11You've found all of them!\xbf";
        u8* count_msg = count_str;
        if (Inventory_GetSkullTokenCount(0x27) >= 30) {
            count_msg = count_done_str;
        }
        u8 end_i = i + 11;
        for (i = 0; i < 128; ++i) {
            font->msgBuf.schar[end_i + i] = count_msg[i];
            if (count_msg[i] == 0xBF) {
                if (count_msg == count_done_str) {
                    break;
                }
                u8 swamp_token_count = ((rando_skulltulas_enabled()) ? rando_has_item(GI_TRUE_SKULL_TOKEN) : Inventory_GetSkullTokenCount(0x27));
                u8 count_suffix[2] = "th";
                if ((swamp_token_count % 10) == 1 && swamp_token_count != 11) {
                    count_suffix[0] = 's';
                    count_suffix[1] = 't';
                } else if ((swamp_token_count % 10) == 2 && swamp_token_count != 12) {
                    count_suffix[0] = 'n';
                    count_suffix[1] = 'd';
                } else if ((swamp_token_count % 10) == 3 && swamp_token_count != 13) {
                    count_suffix[0] = 'r';
                    count_suffix[1] = 'd';
                }
                font->msgBuf.schar[end_i + i] = 0x01;
                i += 1;
                if (swamp_token_count >= 10) {
                    font->msgBuf.schar[end_i + i] = (swamp_token_count / 10) + 0x30;
                    i += 1;
                }
                font->msgBuf.schar[end_i + i] = (swamp_token_count % 10) + 0x30;
                font->msgBuf.schar[end_i + i + 1] = count_suffix[0];
                font->msgBuf.schar[end_i + i + 2] = count_suffix[1];
                font->msgBuf.schar[end_i + i + 3] = 0x00;
                font->msgBuf.schar[end_i + i + 4] = '.';
                font->msgBuf.schar[end_i + i + 5] = 0xBF;
                break;
            }
        }
    } else if (msg == osht_msg) {
        u8 count_str[128] = "\x11This is your \xbf";
        u8 count_done_str[128] = "\x11You've found all of them!\xbf";
        u8* count_msg = count_str;
        if (Inventory_GetSkullTokenCount(0x28) >= 30) {
            count_msg = count_done_str;
        }
        u8 end_i = i + 11;
        for (i = 0; i < 128; ++i) {
            font->msgBuf.schar[end_i + i] = count_msg[i];
            if (count_msg[i] == 0xBF) {
                if (count_msg == count_done_str) {
                    break;
                }
                u8 ocean_token_count = ((rando_skulltulas_enabled()) ? rando_has_item(GI_OCEAN_SKULL_TOKEN) : Inventory_GetSkullTokenCount(0x28));
                u8 count_suffix[2] = "th";
                if ((ocean_token_count % 10) == 1 && ocean_token_count != 11) {
                    count_suffix[0] = 's';
                    count_suffix[1] = 't';
                } else if ((ocean_token_count % 10) == 2 && ocean_token_count != 12) {
                    count_suffix[0] = 'n';
                    count_suffix[1] = 'd';
                } else if ((ocean_token_count % 10) == 3 && ocean_token_count != 13) {
                    count_suffix[0] = 'r';
                    count_suffix[1] = 'd';
                }
                font->msgBuf.schar[end_i + i] = 0x01;
                i += 1;
                if (ocean_token_count >= 10) {
                    font->msgBuf.schar[end_i + i] = (ocean_token_count / 10) + 0x30;
                    i += 1;
                }
                font->msgBuf.schar[end_i + i] = (ocean_token_count % 10) + 0x30;
                font->msgBuf.schar[end_i + i + 1] = count_suffix[0];
                font->msgBuf.schar[end_i + i + 2] = count_suffix[1];
                font->msgBuf.schar[end_i + i + 3] = 0x00;
                font->msgBuf.schar[end_i + i + 4] = '.';
                font->msgBuf.schar[end_i + i + 5] = 0xBF;
                break;
            }
        }
    } else if (msg == wfsf_msg || msg == shsf_msg || msg == gbsf_msg || msg == stsf_msg) {
        u8 count_str[128] = "\x11This is your \xbf";
        u8 count_done_str[128] = "\x11You've found all of them!\xbf";
        u8* count_msg = count_str;
        if (gSaveContext.save.saveInfo.inventory.strayFairies[textId - 0x46] >= 0xF) {
            count_msg = count_done_str;
        }
        u8 end_i = i + 11;
        for (i = 0; i < 128; ++i) {
            font->msgBuf.schar[end_i + i] = count_msg[i];
            if (count_msg[i] == 0xBF) {
                if (count_msg == count_done_str) {
                    break;
                }
                u8 fairy_count = gSaveContext.save.saveInfo.inventory.strayFairies[textId - 0x46];
                u8 count_suffix[2] = "th";
                if ((fairy_count % 10) == 1 && fairy_count != 11) {
                    count_suffix[0] = 's';
                    count_suffix[1] = 't';
                } else if ((fairy_count % 10) == 2 && fairy_count != 12) {
                    count_suffix[0] = 'n';
                    count_suffix[1] = 'd';
                } else if ((fairy_count % 10) == 3 && fairy_count != 13) {
                    count_suffix[0] = 'r';
                    count_suffix[1] = 'd';
                }
                font->msgBuf.schar[end_i + i] = 0x01;
                i += 1;
                if (fairy_count >= 10) {
                    font->msgBuf.schar[end_i + i] = (fairy_count / 10) + 0x30;
                    i += 1;
                }
                font->msgBuf.schar[end_i + i] = (fairy_count % 10) + 0x30;
                font->msgBuf.schar[end_i + i + 1] = count_suffix[0];
                font->msgBuf.schar[end_i + i + 2] = count_suffix[1];
                font->msgBuf.schar[end_i + i + 3] = 0x00;
                font->msgBuf.schar[end_i + i + 4] = '.';
                font->msgBuf.schar[end_i + i + 5] = 0xBF;
                break;
            }
        }
    }

    // for reverse-engineering text
    //~ size_t text_i = msgCtx->msgBufPos;
    //~ char c = font->msgBuf.schar[text_i];
    //~ while (c != '\xbf') {
        //~ recomp_printf("%c", c);
        //~ text_i += 1;
        //~ c = font->msgBuf.schar[text_i];
    //~ }

    //~ text_i = msgCtx->msgBufPos;
    //~ c = font->msgBuf.schar[text_i];
    //~ while (c != '\xbf') {
        //~ recomp_printf("0x%02X ", (u8) c);
        //~ text_i += 1;
        //~ c = font->msgBuf.schar[text_i];
    //~ }

    msgCtx->unk11F08 = font->msgBuf.schar[msgCtx->msgBufPos] << 8;
    msgCtx->unk11F08 |= font->msgBuf.schar[msgCtx->msgBufPos + 1];

    msgCtx->unk11F18 = (msgCtx->unk11F08 & 0xF000) >> 0xC;
    msgCtx->textBoxType = (msgCtx->unk11F08 & 0xF00) >> 8;
    msgCtx->textBoxPos = (msgCtx->unk11F08 & 0xF0) >> 4;
    msgCtx->unk11F0C = msgCtx->unk11F08 & 0xF;

    if ((msgCtx->unk11F0C == 1) || (msgCtx->unk11F0C == 3)) {
        //msgCtx->textUnskippable = true;
    }
    msgCtx->itemId = 0xFE;

    if ((msgCtx->textBoxType == TEXTBOX_TYPE_5) || (msgCtx->textBoxType == TEXTBOX_TYPE_D) ||
        (play->pauseCtx.bombersNotebookOpen)) {
        msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2 = 0;
    } else {
        msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2 = 0xFF;
    }

    msgCtx->choiceIndex = 0;

    if (msgCtx->unk11F0C != 3) {
        msgCtx->textColorAlpha = 0xFF;
    } else {
        msgCtx->textColorAlpha = 0;
    }

    if (textId == 0x1467) {
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_KAFEI);
    }

    if (textId == 0x2955) {
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_ANJU);
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_KAFEI);
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_COUPLES_MASK);
    }
}