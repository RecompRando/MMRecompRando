#include "global.h"
#include "modding.h"

#include "apcommon.h"

struct EnBomBowlMan;

typedef void (*EnBomBowlManActionFunc)(struct EnBomBowlMan*, PlayState*);

#define ENBOMBOWLMAN_GET_F(thisx) ((thisx)->params & 0xF)
#define ENBOMBOWLMAN_GET_F0(thisx) (((thisx)->params >> 4) & 0xF)
#define ENBOMBOWLMAN_GET_PATH_INDEX(thisx) (((thisx)->params >> 8) & 0xFF)

#define ENBOMBOWLMAN_PATH_INDEX_NONE 0x3F

#define ENBOMBOWLMAN_F0_0 0
#define ENBOMBOWLMAN_F0_2 2

#define OBJECT_CS_LIMB_MAX 0x15

typedef struct EnBomBowlMan {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[OBJECT_CS_LIMB_MAX];
    /* 0x206 */ Vec3s morphTable[OBJECT_CS_LIMB_MAX];
    /* 0x284 */ EnBomBowlManActionFunc actionFunc;
    /* 0x288 */ s16 unk_288;
    /* 0x28A */ s16 unk_28A;
    /* 0x28C */ UNK_TYPE1 unk28C[2];
    /* 0x28E */ s16 unk_28E;
    /* 0x290 */ s16 unk_290;
    /* 0x292 */ UNK_TYPE1 unk292[2];
    /* 0x294 */ Path* path;
    /* 0x298 */ s16 unk_298;
    /* 0x29A */ s16 pathIndex;
    /* 0x29C */ s16 unk_29C;
    /* 0x29E */ s16 unk_29E;
    /* 0x2A0 */ Vec3f unk_2A0;
    /* 0x2AC */ UNK_TYPE1 unk2AC[0xC];
    /* 0x2B8 */ s16 unk_2B8;
    /* 0x2BA */ s16 unk_2BA;
    /* 0x2BC */ s16 unk_2BC;
    /* 0x2BE */ UNK_TYPE1 unk2BE[2];
    /* 0x2C0 */ s16 unk_2C0;
    /* 0x2C2 */ s16 unk_2C2;
    /* 0x2C4 */ f32 animEndFrame;
    /* 0x2C8 */ f32 unk_2C8;
    /* 0x2CC */ s16 csIdList[2];
    /* 0x2D0 */ s16 csId1;
    /* 0x2D2 */ s16 csId2;
    /* 0x2D4 */ s16 unk_2D4;
    /* 0x2D6 */ s16 csId3;
    /* 0x2D8 */ struct EnBomBowlMan* unk_2D8[5];
    /* 0x2EC */ UNK_TYPE1 unk2EC[0x4];
    /* 0x2F0 */ s16 unk_2F0;
    /* 0x2F2 */ s16 unk_2F2;
    /* 0x2F4 */ s16 unk_2F4;
    /* 0x2F6 */ s16 unk_2F6;
    /* 0x2F8 */ s32 animIndex;
} EnBomBowlMan; // size = 0x2FC

#define ENBOMBOWLMAN_ANIM_1 0x01
#define ENBOMBOWLMAN_ANIM_3 0x03
#define ENBOMBOWLMAN_ANIM_17 0x11

extern u16 D_809C618C[];
extern u16 D_809C6198[];
extern s32 D_809C6100;
extern s32 D_809C6104;

void func_809C59A4(EnBomBowlMan* this, PlayState* play);
void func_809C59F0(EnBomBowlMan* this, PlayState* play);
void func_809C5AA4(EnBomBowlMan* this, PlayState* play);
void func_809C5B1C(EnBomBowlMan* this, PlayState* play);
void func_809C51B4(EnBomBowlMan* this, PlayState* play);
void EnBomBowlMan_ChangeAnim(EnBomBowlMan* this, s32 animIndex, f32 playSpeed);

RECOMP_PATCH void func_809C4DA4(EnBomBowlMan* this, PlayState* play) {
    Math_SmoothStepToS(&this->unk_290, this->unk_2D4, 1, 5000, 0);

    if (this->unk_2B8 == 0) {
        Player* player = GET_PLAYER(play);

        if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
            CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
            CutsceneManager_Queue(this->csId1);
            return;
        }

        if (!CutsceneManager_IsNext(this->csId1)) {
            CutsceneManager_Queue(this->csId1);
            return;
        }

        CutsceneManager_StartWithPlayerCs(this->csId1, &this->actor);
        this->unk_2B8 = 1;
        this->unk_2C0 = 0;
        this->unk_2D4 = this->actor.yawTowardsPlayer;
        this->unk_290 = this->actor.yawTowardsPlayer;
        if (player->transformation == PLAYER_FORM_HUMAN) {
            Message_StartTextbox(play, D_809C618C[0], &this->actor);
        } else {
            Message_StartTextbox(play, D_809C6198[0], &this->actor);
        }
    }

    if ((this->unk_2BC == 0) && (Message_GetState(&play->msgCtx) == TEXT_STATE_5) && Message_ShouldAdvance(play)) {
        Player* player = GET_PLAYER(play);
        s32 pad;
        s32 sp28 = false;

        Message_CloseTextbox(play);

        switch (this->unk_2C0) {
            case 0:
                this->unk_2C0 = 1;
                D_809C6104 = 1;
                Camera_SetTargetActor(Play_GetCamera(play, CutsceneManager_GetCurrentSubCamId(this->csId1)),
                                      &this->unk_2D8[0]->actor);
                this->unk_2D4 = 0;
                this->unk_2BC = 10;
                EnBomBowlMan_ChangeAnim(this, ENBOMBOWLMAN_ANIM_17, 1.0f);
                break;

            case 1:
                D_809C6104 = 0;
                EnBomBowlMan_ChangeAnim(this, ENBOMBOWLMAN_ANIM_3, 1.0f);
                this->unk_2D4 = this->actor.yawTowardsPlayer;
                this->unk_2C0 = 2;
                // if ((player->transformation == PLAYER_FORM_HUMAN) && CHECK_QUEST_ITEM(QUEST_BOMBERS_NOTEBOOK)) {
                if ((player->transformation == PLAYER_FORM_HUMAN) && rando_location_is_checked(GI_BOMBERS_NOTEBOOK)) {
                    this->unk_2C0 = 4;
                }
                break;

            case 2:
                if (player->transformation == PLAYER_FORM_HUMAN) {
                    this->unk_2B8 = 2;
                    CutsceneManager_Stop(this->csId1);
                    func_809C59A4(this, play);
                    sp28 = true;
                } else {
                    this->unk_2C0 = 3;
                    play->msgCtx.msgLength = 0;
                    EnBomBowlMan_ChangeAnim(this, ENBOMBOWLMAN_ANIM_1, 1.0f);
                    D_809C6100 = 1;
                    if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
                        CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
                        CutsceneManager_Queue(this->csId2);
                    } else if (!CutsceneManager_IsNext(this->csId2)) {
                        CutsceneManager_Queue(this->csId2);
                    }
                    CutsceneManager_Stop(this->csId1);
                    this->actionFunc = func_809C5B1C;
                    sp28 = true;
                }
                break;

            case 3:
                if (player->transformation == PLAYER_FORM_HUMAN) {
                    this->unk_2C0 = 4;
                } else {
                    this->actionFunc = func_809C51B4;
                    return;
                }
                break;

            case 4:
                if (this->unk_2B8 != 2) {
                    CutsceneManager_Stop(this->csId1);
                }
                play->msgCtx.msgLength = 0;
                EnBomBowlMan_ChangeAnim(this, ENBOMBOWLMAN_ANIM_1, 1.0f);
                D_809C6100 = 1;
                this->actionFunc = func_809C5B1C;
                sp28 = true;
                break;

            case 5:
                Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_LEARNED_SECRET_CODE);
                Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_BOMBERS_NOTEBOOK);
                Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_BOMBERS);
                this->actionFunc = func_809C51B4;
                sp28 = true;
                break;

            default:
                break;
        }

        if (!sp28) {
            if (player->transformation == PLAYER_FORM_HUMAN) {
                Message_ContinueTextbox(play, D_809C618C[this->unk_2C0]);
            } else {
                Message_ContinueTextbox(play, D_809C6198[this->unk_2C0]);
            }
        }
    }
}