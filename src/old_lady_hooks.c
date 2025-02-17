#include "modding.h"
#include "global.h"
#include "overlays/actors/ovl_En_GirlA/z_en_girla.h"

#include "apcommon.h"

#define ZORA_LIMB_MAX 0x14
#define BOMB_SHOPKEEPER_LIMB_MAX 0x10
#define GORON_LIMB_MAX 0x12

#define ENSOB1_LIMB_MAX MAX(MAX((s32)ZORA_LIMB_MAX, (s32)BOMB_SHOPKEEPER_LIMB_MAX), (s32)GORON_LIMB_MAX)

struct EnSob1;

typedef void (*EnSob1ActionFunc)(struct EnSob1*, PlayState*);
typedef void (*EnSob1BlinkFunc)(struct EnSob1*);

typedef struct EnSob1XZRange {
    /* 0x0 */ f32 xMin;
    /* 0x4 */ f32 xMax;
    /* 0x8 */ f32 zMin;
    /* 0xC */ f32 zMax;
} EnSob1XZRange; // size = 0x10

typedef struct EnSob1 {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnSob1ActionFunc actionFunc;
    /* 0x18C */ EnSob1ActionFunc prevActionFunc; // Used to restore back to correct browsing function
    /* 0x190 */ EnSob1ActionFunc changeObjectFunc;
    /* 0x194 */ ColliderCylinder collider;
    /* 0x1E0 */ Path* path;
    /* 0x1E4 */ s32 waypoint;
    /* 0x1E8 */ s16 delayTimer;
    /* 0x1EA */ s8 mainObjectSlot;
    /* 0x1EB */ s8 unusedObjectSlot;
    /* 0x1EC */ s8 shopkeeperAnimObjectSlot;
    /* 0x1EE */ s16 headRot;
    /* 0x1F0 */ s16 headRotTarget;
    /* 0x1F2 */ Vec3s jointTable[ENSOB1_LIMB_MAX];
    /* 0x26A */ Vec3s morphTable[ENSOB1_LIMB_MAX];
    /* 0x2E2 */ s16 eyeTexIndex;
    /* 0x2E4 */ s16 blinkTimer;
    /* 0x2E8 */ EnSob1BlinkFunc blinkFunc;
    /* 0x2EC */ EnGirlA* items[3]; // Items on shelf are indexed as: /* 2 1 0 */
    /* 0x2F8 */ s32 stickAccumX;
    /* 0x2FC */ s32 stickAccumY;
    /* 0x300 */ Vec3f cursorPos;
    /* 0x30C */ Color_RGBAu32 cursorColor;
    /* 0x31C */ f32 cursorAnimTween;
    /* 0x320 */ u8 cursorAnimState;
    /* 0x321 */ u8 drawCursor;
    /* 0x322 */ u8 cursorIndex;
    /* 0x324 */ StickDirectionPrompt stickLeftPrompt;
    /* 0x35C */ StickDirectionPrompt stickRightPrompt;
    /* 0x394 */ f32 arrowAnimTween;
    /* 0x398 */ f32 stickAnimTween;
    /* 0x39C */ u8 arrowAnimState;
    /* 0x39D */ u8 stickAnimState;
    /* 0x39E */ s16 cutsceneState;
    /* 0x3A0 */ s16 csId;
    /* 0x3A2 */ s16 lookFowardCsId;
    /* 0x3A4 */ s16 lookToShelfCsId;
    /* 0x3A6 */ s16 lookToShopkeeperCsId;
    /* 0x3A8 */ UNK_TYPE1 pad3A8[0x4];
    /* 0x3AC */ f32 shopItemSelectedTween;
    /* 0x3B0 */ UNK_TYPE1 pad3B0[0x4];
    /* 0x3B4 */ u16 welcomeTextId;
    /* 0x3B6 */ u16 talkOptionTextId;
    /* 0x3B8 */ u16 goodbyeTextId;
    /* 0x3BA */ u8 wasTalkedToWhileWalking;
    /* 0x3BC */ EnSob1XZRange posXZRange;
    /* 0x3CC */ s16 shopType;
} EnSob1; // size = 0x3D0

struct EnBaba;

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

#define THIS ((EnBaba*)thisx)

#define BOMB_SHOP_LADY_LIMB_MAX 0x12

#define BOMB_SHOP_LADY_STATE_END_CONVERSATION (1 << 0)
#define BOMB_SHOP_LADY_STATE_VISIBLE (1 << 1)
#define BOMB_SHOP_LADY_STATE_KNOCKED_OVER (1 << 2) // Don't track player
#define BOMB_SHOP_LADY_STATE_AUTOTALK (1 << 3)
#define BOMB_SHOP_LADY_STATE_GIVE_BLAST_MASK (1 << 5)
#define BOMB_SHOP_LADY_STATE_GAVE_BLAST_MASK (1 << 6)
#define BOMB_SHOP_LADY_STATE_DRAW_SHADOW (1 << 7)

typedef void (*EnBabaActionFunc)(struct EnBaba*, PlayState*);

#define BOMB_SHOP_LADY_GET_PATH_INDEX(thisx) (((thisx)->params & 0x3F00) >> 8)
#define BOMB_SHOP_LADY_GET_TYPE(thisx) (((thisx)->params & 0xC000) >> 0xE)

#define BOMB_SHOP_LADY_PATH_INDEX_NONE 0x3F

#define LOCATION_OLD_LADY GI_MASK_BLAST

// Types BOMB_SHOP_LADY_TYPE_FOLLOW_SCHEDULE and BOMB_SHOP_LADY_TYPE_IDLE can only be used in SCENE_BACKTOWN
// Type BOMB_SHOP_LADY_TYPE_SWAY can be used anywhere except SCENE_BACKTOWN and SCENE_BOMYA
typedef enum {
    /* 0 */ BOMB_SHOP_LADY_TYPE_FOLLOW_SCHEDULE,
    /* 1 */ BOMB_SHOP_LADY_TYPE_IDLE,
    /* 2 */ BOMB_SHOP_LADY_TYPE_SWAY
} BombShopLadyType;

typedef struct EnBaba {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnSob1* bombShopkeeper;
    /* 0x148 */ EnBabaActionFunc actionFunc;
    /* 0x14C */ UNK_TYPE1 unk14C[4];
    /* 0x150 */ SkelAnime skelAnime;
    /* 0x194 */ ColliderCylinder collider;
    /* 0x1E0 */ u16 textId;
    /* 0x1E2 */ u8 msgFading;
    /* 0x1E4 */ Path* path;
    /* 0x1E8 */ s16 waypoint;
    /* 0x1EA */ UNK_TYPE1 unk1EA[0x1C];
    /* 0x206 */ Vec3s jointTable[BOMB_SHOP_LADY_LIMB_MAX];
    /* 0x272 */ Vec3s morphTable[BOMB_SHOP_LADY_LIMB_MAX];
    /* 0x2DE */ Vec3s trackTarget;
    /* 0x2E4 */ Vec3s headRot;
    /* 0x2EA */ Vec3s torsoRot;
    /* 0x2F0 */ UNK_TYPE1 unk2F0[0x12];
    /* 0x302 */ s16 fidgetTableY[BOMB_SHOP_LADY_LIMB_MAX];
    /* 0x326 */ s16 fidgetTableZ[BOMB_SHOP_LADY_LIMB_MAX];
    /* 0x34A */ UNK_TYPE1 unk34A[0xBA];
    /* 0x404 */ s16 sakonDeadTimer;
    /* 0x406 */ UNK_TYPE1 unk406[4];
    /* 0x40A */ u16 stateFlags;
    /* 0x40C */ s32 animIndex;
    /* 0x410 */ Path* timePath;
    /* 0x414 */ Vec3f timePathPoint;
    /* 0x420 */ f32 timePathProgress;
    /* 0x424 */ s32 timePathTotalTime;
    /* 0x428 */ s32 timePathWaypointTime;
    /* 0x42C */ s32 timePathWaypoint;
    /* 0x430 */ s32 timePathElapsedTime;
    /* 0x434 */ u8 scheduleResult;
    /* 0x436 */ s16 timePathTimeSpeed;
    /* 0x438 */ s32 timePathIsSetup;
    /* 0x43C */ s32 timePathHasReachedEnd;
} EnBaba; // size = 0x440

RECOMP_PATCH void EnBaba_HandleConversation(EnBaba* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    switch (this->textId) {
        case 0:
            if (this->stateFlags & BOMB_SHOP_LADY_STATE_AUTOTALK) {
                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_RECOVERED_STOLEN_BOMB_BAG)) {
                    // Thanks. Can stock Bomb Bags tomorrow
                    this->textId = 0x2A34;
                    break;
                }

                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_SAKON_DEAD)) {
                    this->stateFlags |= BOMB_SHOP_LADY_STATE_END_CONVERSATION;
                    // Oh my, learned my lesson. Can't stock Bomb Bags tomorrow
                    this->textId = 0x2A33;
                    break;
                }

                this->stateFlags |= BOMB_SHOP_LADY_STATE_END_CONVERSATION;
                // Can't stock Bomb Bags tomorrow
                this->textId = 0x2A32;
                break;
            }

            if (player->transformation == PLAYER_FORM_DEKU) {
                if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_79_20)) {
                    SET_WEEKEVENTREG(WEEKEVENTREG_79_20);
                    this->stateFlags |= BOMB_SHOP_LADY_STATE_END_CONVERSATION;
                    // Small customer, use bombs as adult
                    this->textId = 0x2A37;
                    break;
                } else {
                    this->stateFlags |= BOMB_SHOP_LADY_STATE_END_CONVERSATION;
                    // use bombs as adult
                    this->textId = 0x2A38;
                }
                break;
            }

            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECOVERED_STOLEN_BOMB_BAG)) {
                if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_73_01)) {
                    // Thought could sell Big Bomb Bags
                    this->textId = 0x660;
                    break;
                }
                // Can't judge people
                this->textId = 0x662;
                break;
            }

            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_73_02)) {
                // Someone helped me out
                this->textId = 0x65A;
                break;
            }

            // Buy Big Bomb Bag
            this->textId = 0x65E;
            break;

        case 0x660:
            Actor_ChangeFocus(&this->actor, play, &this->bombShopkeeper->actor);
            // Don't go by yourself
            this->textId = 0x661;
            break;

        case 0x661:
            Actor_ChangeFocus(&this->bombShopkeeper->actor, play, &this->actor);
            // Can't judge people
            this->textId = 0x662;
            break;

        case 0x662:
            Actor_ChangeFocus(&this->actor, play, &this->bombShopkeeper->actor);
            // I'll go next time
            this->textId = 0x663;
            SET_WEEKEVENTREG(WEEKEVENTREG_73_01);
            this->stateFlags |= BOMB_SHOP_LADY_STATE_END_CONVERSATION;
            break;

        case 0x65A:
            Actor_ChangeFocus(&this->actor, play, &this->bombShopkeeper->actor);
            // Don't pick up Bomb bags at night
            this->textId = 0x65B;
            break;

        case 0x65B:
            Actor_ChangeFocus(&this->bombShopkeeper->actor, play, &this->actor);
            // Lifelong dream to sell Big Bomb Bags
            this->textId = 0x65C;
            break;

        case 0x65C:
            Actor_ChangeFocus(&this->actor, play, &this->bombShopkeeper->actor);
            // I worry about you
            this->textId = 0x65D;
            SET_WEEKEVENTREG(WEEKEVENTREG_73_02);
            this->stateFlags |= BOMB_SHOP_LADY_STATE_END_CONVERSATION;
            break;

        case 0x65E:
            Actor_ChangeFocus(&this->actor, play, &this->bombShopkeeper->actor);
            // I worry about you
            this->textId = 0x65F;
            this->stateFlags |= BOMB_SHOP_LADY_STATE_END_CONVERSATION;
            break;

        case 0x2A34:
            //if (INV_CONTENT(ITEM_MASK_BLAST) == ITEM_MASK_BLAST) {
            if (rando_location_is_checked(LOCATION_OLD_LADY)) {
                this->stateFlags |= BOMB_SHOP_LADY_STATE_END_CONVERSATION;
                // Thank you
                this->textId = 0x2A36;
                break;
            }
            this->stateFlags |= BOMB_SHOP_LADY_STATE_GIVE_BLAST_MASK;
            // It's a dangerous mask
            this->textId = 0x2A35;
            break;

        case 0x2A35:
            this->stateFlags |= BOMB_SHOP_LADY_STATE_END_CONVERSATION;
            // Thank you
            this->textId = 0x2A36;
            break;

        case 0x2A30:
        case 0x2A31:
            this->stateFlags |= BOMB_SHOP_LADY_STATE_END_CONVERSATION;
            break;

        default:
            break;
    }

    Message_StartTextbox(play, this->textId, &this->actor);
    if (this->stateFlags & BOMB_SHOP_LADY_STATE_END_CONVERSATION) {
        if (this->stateFlags & BOMB_SHOP_LADY_STATE_GAVE_BLAST_MASK) {
            this->stateFlags &= ~BOMB_SHOP_LADY_STATE_GAVE_BLAST_MASK;
            Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_RECEIVED_BLAST_MASK);
        }
        Message_BombersNotebookQueueEvent(play, BOMBERS_NOTEBOOK_EVENT_MET_BOMB_SHOP_LADY);
    }
}