#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define LOCATION_DAY2BATS ((play->sceneId << 8) | this->actor.id)

struct EnTk;

typedef void (*EnTkActionFunc)(struct EnTk*, PlayState*);
typedef void (*EnTkUnkFunc)(struct EnTk*, PlayState*);

#define ENTK_GET_F(thisx) ((thisx)->params & 0xF)
#define ENTK_GET_SWITCH_FLAG(thisx) (((thisx)->params >> 4) & 0x7F)
#define ENTK_GET_PATH_INDEX(thisx) (((thisx)->params >> 0xB) & 0x1F)

#define OBJECT_TK_LIMB_MAX 0x12

typedef struct EnTk {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ EnTkActionFunc actionFunc;
    /* 0x18C */ ColliderCylinder collider;
    /* 0x1D8 */ Vec3s jointTable[OBJECT_TK_LIMB_MAX];
    /* 0x244 */ Vec3s morphTable[OBJECT_TK_LIMB_MAX];
    /* 0x2B0 */ s8 unk_2B0;
    /* 0x2B1 */ s8 switchFlag;
    /* 0x2B4 */ Vec3f unk_2B4;
    /* 0x2C0 */ s16 unk_2C0;
    /* 0x2C2 */ s16 unk_2C2;
    /* 0x2C4 */ s16 unk_2C4;
    /* 0x2C6 */ s16 unk_2C6;
    /* 0x2C8 */ UNK_TYPE1 unk2C8[0x2];
    /* 0x2CA */ u16 unk_2CA;
    /* 0x2CC */ s16 unk_2CC;
    /* 0x2D0 */ s32 unk_2D0;
    /* 0x2D4 */ s32 animIndex;
    /* 0x2D8 */ f32 unk_2D8;
    /* 0x2DC */ f32 unk_2DC;
    /* 0x2E0 */ s32 unk_2E0;
    /* 0x2E4 */ s16 unk_2E4;
    /* 0x2E6 */ u16 unk_2E6;
    /* 0x2E8 */ s16 csLength;
    /* 0x2EC */ Vec3f unk_2EC;
    /* 0x2F8 */ Vec3s unk_2F8;
    /* 0x300 */ Vec3f unk_300;
    /* 0x30C */ EnTkUnkFunc unk_30C;
    /* 0x310 */ s16 unk_310;
    /* 0x312 */ s16 csIdList[2];
    /* 0x316 */ s16 unk_316;
    /* 0x318 */ s16 unk_318;
    /* 0x31A */ s16 unk_31A;
    /* 0x31C */ s16 unk_31C;
    /* 0x320 */ f32 animCurFrame;
    /* 0x324 */ Vec3f unk_324[6];
    /* 0x36C */ s32 unk_36C;
    /* 0x370 */ UNK_TYPE1 unk370[0x58];
    /* 0x3C8 */ Path* timePath;
    /* 0x3CC */ u8 scheduleResult;
    /* 0x3CE */ u16 unk_3CE;
    /* 0x3D0 */ s32 timePathTimeSpeed;
    /* 0x3D4 */ Vec3f timePathTargetPos;
    /* 0x3E0 */ f32 timePathProgress;
    /* 0x3E4 */ s32 timePathTotalTime;
    /* 0x3E8 */ s32 timePathWaypointTime;
    /* 0x3EC */ s32 timePathWaypoint;
    /* 0x3F0 */ s32 timePathElapsedTime;
} EnTk; // size = 0x3F4

extern AnimationHeader object_tk_Anim_001FA8;
extern AnimationHeader object_tk_Anim_0030A4;
extern AnimationHeader object_tk_Anim_001144;
extern AnimationHeader object_tk_Anim_003724;
extern AnimationHeader object_tk_Anim_003FB8;
extern AnimationHeader object_tk_Anim_0020C8;
extern AnimationHeader object_tk_Anim_003B10;

static AnimationSpeedInfo D_80AEF868[] = {
    { &object_tk_Anim_001FA8, 1.0f, ANIMMODE_LOOP, -10.0f }, { &object_tk_Anim_001FA8, 2.0f, ANIMMODE_LOOP, -10.0f },
    { &object_tk_Anim_0030A4, 1.0f, ANIMMODE_LOOP, -10.0f }, { &object_tk_Anim_001144, 1.0f, ANIMMODE_ONCE, -10.0f },
    { &object_tk_Anim_003724, 1.0f, ANIMMODE_ONCE, -10.0f }, { &object_tk_Anim_003FB8, 1.0f, ANIMMODE_LOOP, -10.0f },
    { &object_tk_Anim_0020C8, 1.0f, ANIMMODE_LOOP, -10.0f }, { &object_tk_Anim_003B10, 1.0f, ANIMMODE_LOOP, -10.0f },
};

u8 dampeSpeedMultiplier = 2;

// turning isn't sped up, so it may be hard to control dampé
RECOMP_HOOK_RETURN("func_80AEC658") void speedup_dampe_walk(SkelAnime* skelAnime, f32 arg1, f32 arg2, f32* arg3, f32* arg4) {
    *arg3 *= dampeSpeedMultiplier;
}

void func_80AECB6C(EnTk* this, PlayState* play);
void func_80AED544(EnTk* this, PlayState* play);

void EnTk_Day2BatsOffer(EnTk* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, 0, &this->animIndex);
        this->actionFunc = func_80AECB6C;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_DAY2BATS), LOCATION_DAY2BATS, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void func_80AED610(EnTk* this, PlayState* play) {
    if ((this->animIndex == 4) && Animation_OnFrame(&this->skelAnime, this->skelAnime.endFrame)) {
        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, 7, &this->animIndex);
    }

    switch (Message_GetState(&play->msgCtx)) {
        case TEXT_STATE_NONE:
            if (Math_ScaledStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer - 0x1555, 0x71C)) {
                if (Player_GetMask(play) == PLAYER_MASK_CAPTAIN) {
                    SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, 4, &this->animIndex);
                    Message_StartTextbox(play, 0x13FD, &this->actor);
                } else if (CURRENT_DAY != 2) {
                    func_80AED544(this, play);
                } else if (!Flags_GetSwitch(play, ENTK_GET_SWITCH_FLAG(&this->actor))) {
                    Message_StartTextbox(play, 0x1403, &this->actor);
                } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_60_02)) {
                    func_80AED544(this, play);
                } else {
                    Message_StartTextbox(play, 0x1413, &this->actor);
                }
                break;
            }

        case TEXT_STATE_1:
        case TEXT_STATE_CLOSING:
        case TEXT_STATE_3:
            break;

        case TEXT_STATE_CHOICE:
        case TEXT_STATE_5:
        case TEXT_STATE_DONE:
            if (Message_ShouldAdvance(play)) {
                switch (play->msgCtx.currentTextId) {
                    case 0x13FD:
                        this->unk_2CA |= 0x10;
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, 0, &this->animIndex);
                        this->skelAnime.playSpeed = 10.0f;
                        this->actionFunc = func_80AECB6C;
                        break;

                    case 0x13FE:
                        Message_ContinueTextbox(play, 0x13FF);
                        break;

                    case 0x1413:
                        // Rupees_ChangeBy(30);
                        SET_WEEKEVENTREG(WEEKEVENTREG_60_02);
                        Message_ContinueTextbox(play, 0x13FF);
                        this->actionFunc = EnTk_Day2BatsOffer; // this does give the item late, but it works
                        break;

                    case 0x13FF:
                    case 0x1400:
                    case 0x1401:
                    case 0x1402:
                    case 0x1403:
                    case 0x1404:
                    case 0x1405:
                    case 0x1406:
                    case 0x1407:
                    case 0x1408:
                    case 0x1409:
                    case 0x140A:
                    case 0x140B:
                    case 0x140C:
                    case 0x140D:
                    case 0x140E:
                    case 0x140F:
                    case 0x1410:
                    case 0x1411:
                    case 0x1412:
                    default:
                        SubS_ChangeAnimationBySpeedInfo(&this->skelAnime, D_80AEF868, 0, &this->animIndex);
                        this->actionFunc = func_80AECB6C;
                        break;
                }
            }
            break;
    }
}