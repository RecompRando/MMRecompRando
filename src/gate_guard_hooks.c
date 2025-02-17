#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnStopheishi;

#define FLAGS (ACTOR_FLAG_TARGETABLE | ACTOR_FLAG_FRIENDLY)

#define THIS ((EnStopheishi*)thisx)

typedef void (*EnStopheishiActionFunc)(struct EnStopheishi*, PlayState*);

#define ENSTOPHEISHI_GET_SWITCH_FLAG(thisx) (((thisx)->params) & 0x3F)
#define ENSTOPHEISHI_GET_F000(thisx) ((((thisx)->params) >> 0xC) & 0xF)

#define ENSTOPHEISHI_SWITCH_FLAG_NONE 0x7F

#define SOLDIER_LIMB_MAX 0x11

typedef struct EnStopheishi {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[SOLDIER_LIMB_MAX];
    /* 0x1EE */ Vec3s morphTable[SOLDIER_LIMB_MAX];
    /* 0x254 */ EnStopheishiActionFunc actionFunc;
    /* 0x258 */ s16 headRotZ;
    /* 0x25A */ s16 headRotX;
    /* ox25C */ UNK_TYPE1 unk_25C[2];
    /* 0x25E */ s16 pitchToPlayer;
    /* 0x260 */ s16 headRotXTarget;
    /* 0x262 */ s16 unk_262; // Unused
    /* 0x264 */ u8 unk_264; // Locking value at `0` prevents leaving the guard pose, however locking to 1 doesn't prevent guarding.
    /* 0x265 */ u8 unk_265;
    /* 0x266 */ s16 unk_266;
    /* 0x268 */ s32 animIndex;
    /* 0x26C */ f32 animEndFrame;
    /* 0x270 */ s16 timer;
    /* 0x272 */ s16 headTurnTimer1;
    /* 0x274 */ s16 unk_274;
    /* 0x276 */ s16 unk_276;
    /* 0x278 */ s16 disableCollider;
    /* 0x27A */ s16 switchFlag;
    /* 0x27C */ s16 messageIndex;
    /* 0x27E */ s16 unk_27E;
    /* 0x280 */ s16 rotYTarget;
    /* 0x284 */ s32 headTurnTimer2;
    /* 0x288 */ f32 unk_288;
    /* 0x28C */ f32 maxMoveStep;
    /* 0x28C */ Vec3f headWorldPos;
    /* 0x29C */ ColliderCylinder collider;
} EnStopheishi; // size = 0x2E8

static u16 sLeaveMessages[] = {
    0x0516, 0x0517, 0x051A, 0x0000, 0x051C, 0x0000, 0x051E, 0x0000, 0x0520, 0x0521, 0x0524, 0x0000, 0x0526, 0x0000,
    0x0528, 0x0000, 0x052A, 0x052B, 0x052E, 0x0000, 0x0530, 0x0000, 0x0532, 0x0000, 0x0534, 0x0535, 0x0538, 0x0000,
    0x053A, 0x0000, 0x053C, 0x0000, 0x0518, 0x0519, 0x051B, 0x0000, 0x051D, 0x0000, 0x051F, 0x0000, 0x0522, 0x0523,
    0x0525, 0x0000, 0x0527, 0x0000, 0x0529, 0x0000, 0x052C, 0x052D, 0x052F, 0x0000, 0x0531, 0x0000, 0x0533, 0x0000,
    0x0536, 0x0537, 0x0539, 0x0000, 0x053B, 0x0000, 0x053D, 0x0000, 0x0514, 0x0000, 0x0560, 0x0000, 0x0562, 0x0000,
    0x0564, 0x0000, 0x0515, 0x0000, 0x0561, 0x0000, 0x0563, 0x0000, 0x0565, 0x0000,
};

static u16 sThirdDayLeaveMessages[] = {
    0x0540, 0x0541, 0x0542, 0x0000, 0x0543, 0x0000, 0x0543, 0x0000, 0x0547, 0x0548, 0x0549, 0x0000, 0x054A, 0x0000,
    0x054B, 0x0000, 0x054F, 0x0550, 0x0551, 0x0000, 0x0552, 0x0000, 0x0553, 0x0000, 0x0557, 0x0558, 0x0559, 0x0000,
    0x055A, 0x0000, 0x055A, 0x0000, 0x0544, 0x0545, 0x0546, 0x0000, 0x0546, 0x0000, 0x0546, 0x0000, 0x054C, 0x054D,
    0x054E, 0x0000, 0x054E, 0x0000, 0x054E, 0x0000, 0x0554, 0x0555, 0x0556, 0x0000, 0x0556, 0x0000, 0x0556, 0x0000,
    0x055B, 0x055C, 0x055D, 0x0000, 0x055D, 0x0000, 0x055D, 0x0000, 0x053E, 0x0000, 0x053E, 0x0000, 0x053E, 0x0000,
    0x053E, 0x0000, 0x053F, 0x0000, 0x053F, 0x0000, 0x053F, 0x0000, 0x053F, 0x0000,
};

typedef enum {
    /* 0 */ SOLDIER_ANIM_LOOK_DOWN,
    /* 1 */ SOLDIER_ANIM_COME_UP_HERE,
    /* 2 */ SOLDIER_ANIM_STAND_HAND_ON_HIP,
    /* 3 */ SOLDIER_ANIM_STAND_LOOK_DOWN,
    /* 4 */ SOLDIER_ANIM_4,
    /* 5 */ SOLDIER_ANIM_5,
    /* 6 */ SOLDIER_ANIM_6,
    /* 7 */ SOLDIER_ANIM_STAND_HAND_ON_CHEST,
    /* 8 */ SOLDIER_ANIM_MAX
} SoldierAnimation;

s32 Actor_TalkOfferAccepted(Actor* actor, GameState* gameState);

void func_80AE77D4(EnStopheishi* this);
void func_80AE795C(EnStopheishi* this, PlayState* play);
void func_80AE854C(EnStopheishi* this, PlayState* play);

void EnStopheishi_UpdateHeadNormal(EnStopheishi* this, PlayState* play);
void EnStopHeishi_UpdateHeadThirdDay(EnStopheishi* this);
void EnStopHeishi_ChangeAnim(EnStopheishi* this, s32 animIndex);

RECOMP_PATCH void func_80AE7F34(EnStopheishi* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    s16 sp32;
    s16 phi_a2;
    s32 yawDiffAbs;
    s16 yawDiff;
    f32 xDiff;
    f32 zDiff;

    SkelAnime_Update(&this->skelAnime);
    if ((this->animIndex == SOLDIER_ANIM_5) && ((TRUNCF_BINANG(this->skelAnime.curFrame) % 2) != 0)) {
        Actor_PlaySfx(&this->actor, NA_SE_EV_SOLDIER_WALK);
    }
    if (gSaveContext.save.day != 3) {
        EnStopheishi_UpdateHeadNormal(this, play);
    } else {
        EnStopHeishi_UpdateHeadThirdDay(this);
    }
    Math_SmoothStepToS(&this->actor.world.rot.y, this->rotYTarget, 1, 0x1388, 0);

    sp32 = 0;
    if (gSaveContext.save.isNight) {
        sp32 = 0x20;
    }

    phi_a2 = 0;
    switch (player->transformation) {
        case PLAYER_FORM_HUMAN:
            if ((STOLEN_ITEM_1 == SLOT_SWORD_GREAT_FAIRY) || (STOLEN_ITEM_1 == ITEM_SWORD_KOKIRI) ||
                (STOLEN_ITEM_1 == ITEM_SWORD_RAZOR) || (STOLEN_ITEM_1 == ITEM_SWORD_GILDED) ||
                (STOLEN_ITEM_2 == SLOT_SWORD_GREAT_FAIRY) || (STOLEN_ITEM_2 == ITEM_SWORD_KOKIRI) ||
                (STOLEN_ITEM_2 == ITEM_SWORD_RAZOR) || (STOLEN_ITEM_2 == ITEM_SWORD_GILDED)) {
                if (this->unk_265 != 0) {
                    phi_a2 = 1;
                }
            } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_12_20)) {
                phi_a2 = 1;
            }
            break;

        case PLAYER_FORM_DEKU:
            phi_a2 = 4;
            break;

        case PLAYER_FORM_ZORA:
            phi_a2 = 3;
            break;

        case PLAYER_FORM_GORON:
            phi_a2 = 2;
            break;

        case PLAYER_FORM_FIERCE_DEITY:
            phi_a2 = 1;
            break;
    }

    if (((phi_a2 == 1) || (phi_a2 == 2) || (phi_a2 == 3)) &&
        (((this->animIndex == SOLDIER_ANIM_4)) || (this->animIndex == SOLDIER_ANIM_5) ||
         (this->animIndex == SOLDIER_ANIM_6))) {
        EnStopHeishi_ChangeAnim(this, SOLDIER_ANIM_STAND_HAND_ON_HIP);
        if ((gSaveContext.save.day != 3) && gSaveContext.save.isNight) {
            EnStopHeishi_ChangeAnim(this, SOLDIER_ANIM_STAND_LOOK_DOWN);
        }
        if (gSaveContext.save.day == 3) {
            EnStopHeishi_ChangeAnim(this, SOLDIER_ANIM_STAND_HAND_ON_CHEST);
        }
    }

    if ((phi_a2 == 0) || (phi_a2 == 4)) {
        xDiff = this->actor.home.pos.x - player->actor.world.pos.x;
        zDiff = this->actor.home.pos.z - player->actor.world.pos.z;
        this->collider.dim.radius = 50;
        this->collider.dim.height = 260;
        if (sqrtf(SQ(xDiff) + SQ(zDiff)) < 240.0f) {
            func_80AE795C(this, play);
        } else {
            func_80AE77D4(this);
        }
    } else {
        this->collider.dim.radius = 15;
        this->collider.dim.height = 60;
    }
    if (phi_a2 != 4) {
        this->messageIndex = this->unk_276 * 8;
        this->messageIndex += sp32;
        this->messageIndex += phi_a2 * 2;
    } else {
        this->messageIndex = 64;
        if (sp32 != 0) {
            this->messageIndex = 72;
        }
        this->messageIndex += this->unk_276 * 2;
    }
    if (gSaveContext.save.day != 3) {
        this->actor.textId = sLeaveMessages[this->messageIndex];
        this->unk_27E = sLeaveMessages[this->messageIndex + 1];
    } else {
        this->actor.textId = sThirdDayLeaveMessages[this->messageIndex];
        this->unk_27E = sThirdDayLeaveMessages[this->messageIndex + 1];
    }
    if (this->unk_27E != 0) {
        if ((STOLEN_ITEM_1 == ITEM_SWORD_GREAT_FAIRY) || (STOLEN_ITEM_1 == ITEM_SWORD_KOKIRI) ||
            (STOLEN_ITEM_1 == ITEM_SWORD_RAZOR) || (STOLEN_ITEM_1 == ITEM_SWORD_GILDED) ||
            (STOLEN_ITEM_2 == ITEM_SWORD_GREAT_FAIRY) || (STOLEN_ITEM_2 == ITEM_SWORD_KOKIRI) ||
            (STOLEN_ITEM_2 == ITEM_SWORD_RAZOR) || (STOLEN_ITEM_2 == ITEM_SWORD_GILDED)) {
            if (this->actor.textId == 0x516) {
                this->actor.textId = 0x56C;
            } else if (this->actor.textId == 0x520) {
                this->actor.textId = 0x56E;
            } else if (this->actor.textId == 0x52A) {
                this->actor.textId = 0x570;
            } else if (this->actor.textId == 0x534) {
                this->actor.textId = 0x572;
            } else if (this->actor.textId == 0x518) {
                this->actor.textId = 0x56D;
            } else if (this->actor.textId == 0x522) {
                this->actor.textId = 0x56F;
            } else if (this->actor.textId == 0x52C) {
                this->actor.textId = 0x571;
            } else if (this->actor.textId == 0x536) {
                this->actor.textId = 0x573;
            }
        } else if (GET_CUR_EQUIP_VALUE(EQUIP_TYPE_SWORD) == EQUIP_VALUE_SWORD_NONE) {
            if (this->actor.textId == 0x516) {
                this->actor.textId = 0x55E;
            } else if (this->actor.textId == 0x520) {
                this->actor.textId = 0x566;
            } else if (this->actor.textId == 0x52A) {
                this->actor.textId = 0x568;
            } else if (this->actor.textId == 0x534) {
                this->actor.textId = 0x56A;
            } else if (this->actor.textId == 0x518) {
                this->actor.textId = 0x55F;
            } else if (this->actor.textId == 0x522) {
                this->actor.textId = 0x567;
            } else if (this->actor.textId == 0x52C) {
                this->actor.textId = 0x569;
            } else if (this->actor.textId == 0x536) {
                this->actor.textId = 0x56B;
            }
        }
    }
    yawDiff = this->actor.yawTowardsPlayer - this->actor.world.rot.y;
    yawDiffAbs = ABS_ALT(yawDiff);

    if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        this->skelAnime.playSpeed = 1.0f;
        func_80AE854C(this, play);
    } else if (yawDiffAbs < 0x4BB9) {
        Actor_OfferTalk(&this->actor, play, 70.0f);
    }
}