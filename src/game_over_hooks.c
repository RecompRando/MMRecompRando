#include "modding.h"
#include "global.h"

#include "apcommon.h"

void ShrinkWindow_Letterbox_SetSizeTarget(s32 target);

static s16 sGameOverTimer = 0;

RECOMP_PATCH void GameOver_Update(PlayState* play) {
    GameOverContext* gameOverCtx = &play->gameOverCtx;
    s16 timerId;

    switch (gameOverCtx->state) {
        case GAMEOVER_DEATH_START:
            Message_CloseTextbox(play);

            for (timerId = 0; timerId < TIMER_ID_MAX; timerId++) {
                gSaveContext.timerStates[timerId] = TIMER_STATE_OFF;
            }

            CLEAR_EVENTINF_ALT(EVENTINF_10);

            if (CUR_FORM == 0) {
                if (CUR_FORM_EQUIP(EQUIP_SLOT_B) != ITEM_SWORD_KOKIRI &&
                    CUR_FORM_EQUIP(EQUIP_SLOT_B) != ITEM_SWORD_RAZOR &&
                    CUR_FORM_EQUIP(EQUIP_SLOT_B) != ITEM_SWORD_GILDED &&
                    CUR_FORM_EQUIP(EQUIP_SLOT_B) != ITEM_SWORD_DEITY) {

                    if (gSaveContext.buttonStatus[EQUIP_SLOT_B] != BTN_ENABLED) {
                        CUR_FORM_EQUIP(EQUIP_SLOT_B) = gSaveContext.buttonStatus[EQUIP_SLOT_B];
                    } else {
                        CUR_FORM_EQUIP(EQUIP_SLOT_B) = ITEM_NONE;
                    }
                }
            }

            gSaveContext.nayrusLoveTimer = 2000;
            gSaveContext.save.saveInfo.playerData.tatlTimer = 0;
            gSaveContext.seqId = (u8)NA_BGM_DISABLED;
            gSaveContext.ambienceId = AMBIENCE_ID_DISABLED;
            gSaveContext.eventInf[0] = 0;
            gSaveContext.eventInf[1] = 0;
            gSaveContext.eventInf[2] = 0;
            gSaveContext.eventInf[3] = 0;
            gSaveContext.buttonStatus[EQUIP_SLOT_B] = BTN_ENABLED;
            gSaveContext.buttonStatus[EQUIP_SLOT_C_LEFT] = BTN_ENABLED;
            gSaveContext.buttonStatus[EQUIP_SLOT_C_DOWN] = BTN_ENABLED;
            gSaveContext.buttonStatus[EQUIP_SLOT_C_RIGHT] = BTN_ENABLED;
            gSaveContext.buttonStatus[EQUIP_SLOT_A] = BTN_ENABLED;
            gSaveContext.hudVisibilityForceButtonAlphasByStatus = false;
            gSaveContext.nextHudVisibility = HUD_VISIBILITY_IDLE;
            gSaveContext.hudVisibility = HUD_VISIBILITY_IDLE;
            gSaveContext.hudVisibilityTimer = 0;
            Environment_InitGameOverLights(play);
            sGameOverTimer = 20;
            Rumble_Request(0.0f, 126, 124, 63);
            gameOverCtx->state = GAMEOVER_DEATH_WAIT_GROUND;
            break;

        case GAMEOVER_DEATH_FADE_OUT:
            // if (AudioSeq_GetActiveSeqId(SEQ_PLAYER_FANFARE) != NA_BGM_GAME_OVER) {
                func_80169F78(play);
                if (gSaveContext.respawnFlag != -7) {
                    gSaveContext.respawnFlag = -6;
                }
                gSaveContext.nextTransitionType = TRANS_TYPE_FADE_BLACK;
                gSaveContext.save.saveInfo.playerData.health = 0x30;
                gameOverCtx->state++;
                if (INV_CONTENT(ITEM_MASK_DEKU) == ITEM_MASK_DEKU) {
                    gSaveContext.save.playerForm = PLAYER_FORM_HUMAN;
                    gSaveContext.save.equippedMask = PLAYER_MASK_NONE;
                }
                Rumble_StateReset();
            // }
            break;

        case GAMEOVER_REVIVE_START:
            gameOverCtx->state++; // GAMEOVER_REVIVE_RUMBLE
            sGameOverTimer = 0;
            Environment_InitGameOverLights(play);
            ShrinkWindow_Letterbox_SetSizeTarget(32);
            break;

        case GAMEOVER_REVIVE_RUMBLE:
            sGameOverTimer = 50;
            gameOverCtx->state++; // GAMEOVER_REVIVE_WAIT_GROUND
            Rumble_Request(0.0f, 126, 124, 63);
            break;

        case GAMEOVER_REVIVE_WAIT_GROUND:
            sGameOverTimer--;
            if (sGameOverTimer == 0) {
                sGameOverTimer = 64;
                gameOverCtx->state++; // GAMEOVER_REVIVE_WAIT_FAIRY
            }
            break;

        case GAMEOVER_REVIVE_WAIT_FAIRY:
            sGameOverTimer--;
            if (sGameOverTimer == 0) {
                sGameOverTimer = 50;
                gameOverCtx->state++; // GAMEOVER_REVIVE_FADE_OUT
            }
            break;

        case GAMEOVER_REVIVE_FADE_OUT:
            Environment_FadeOutGameOverLights(play);
            sGameOverTimer--;
            if (sGameOverTimer == 0) {
                gameOverCtx->state = GAMEOVER_INACTIVE;
            }
            break;
    }
}

// In z_player.c
s32 Player_SetAction(PlayState *play, Player *this, PlayerActionFunc actionFunc, s32 arg3);
void Player_Anim_PlayOnce(PlayState* play, Player* this, PlayerAnimationHeader* anim);
void Player_AnimSfx_PlayVoice(Player* this, u16 sfxId);
void Player_Action_62(Player* this, PlayState* play);
void Player_Action_24(Player* this, PlayState* play);
void func_8082DE50(PlayState *play, Player *this);
void func_8082DAD4(Player *this);


RECOMP_PATCH void func_80831F34(PlayState* play, Player* this, PlayerAnimationHeader* anim) {
    s32 sp24 = func_801242B4(this);

    func_8082DE50(play, this);
    Player_SetAction(play, this, sp24 ? Player_Action_62 : Player_Action_24, 0);
    Player_Anim_PlayOnce(play, this, anim);

    // if (anim == &gPlayerAnim_link_derth_rebirth) {
    if (rando_death_behavior() == 1) {
        this->skelAnime.endFrame = 0.0f;
    }

    this->stateFlags1 |= PLAYER_STATE1_DEAD;

    func_8082DAD4(this);
    Player_AnimSfx_PlayVoice(this, NA_SE_VO_LI_DOWN);

    if (this == GET_PLAYER(play)) {
        this->csId = play->playerCsIds[PLAYER_CS_ID_DEATH];
        Audio_SetBgmVolumeOff();
        gSaveContext.powderKegTimer = 0;
        gSaveContext.unk_1014 = 0;
        gSaveContext.jinxTimer = 0;

        if (Inventory_ConsumeFairy(play)) {
            play->gameOverCtx.state = GAMEOVER_REVIVE_START;
            this->av1.actionVar1 = 1;
        } else {
            play->gameOverCtx.state = GAMEOVER_DEATH_START;
            Audio_StopFanfare(0);
            Audio_PlayFanfare(NA_BGM_GAME_OVER);
            gSaveContext.seqId = (u8)NA_BGM_DISABLED;
            gSaveContext.ambienceId = AMBIENCE_ID_DISABLED;
        }

        ShrinkWindow_Letterbox_SetSizeTarget(32);
    }
}
