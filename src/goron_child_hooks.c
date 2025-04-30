#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define LOCATION_LULLABY GI_AD

struct EnGk;

typedef void (*EnGkActionFunc)(struct EnGk*, PlayState*);

#define ENGK_GET_F(thisx) ((thisx)->params & 0xF)
#define ENGK_GET_PATH_INDEX(thisx) (((thisx)->params & 0xF0) >> 4)
#define ENGK_GET_SWITCH_FLAG(thisx) (((thisx)->params & 0x3F00) >> 8)

#define ENGK_PATH_INDEX_NONE 0xF

#define OBJECT_GK_LIMB_MAX 0x14

typedef enum {
    /* 0 */ ENGK_F_0,
    /* 1 */ ENGK_F_1,
    /* 2 */ ENGK_F_2,
    /* 3 */ ENGK_F_3,
    /* 4 */ ENGK_F_4,
    /* 5 */ ENGK_F_5
} EnGkParam;

typedef enum {
    /*  -1 */ ENGK_ANIM_NONE = -1,
    /* 0x0 */ ENGK_ANIM_0,
    /* 0x1 */ ENGK_ANIM_1,
    /* 0x2 */ ENGK_ANIM_2,
    /* 0x3 */ ENGK_ANIM_3,
    /* 0x4 */ ENGK_ANIM_4,
    /* 0x5 */ ENGK_ANIM_5,
    /* 0x6 */ ENGK_ANIM_6,
    /* 0x7 */ ENGK_ANIM_7,
    /* 0x8 */ ENGK_ANIM_8,
    /* 0x9 */ ENGK_ANIM_9,
    /* 0xA */ ENGK_ANIM_10,
    /* 0xB */ ENGK_ANIM_11,
    /* 0xC */ ENGK_ANIM_12,
    /* 0xD */ ENGK_ANIM_MAX
} EnGkAnimation;

typedef struct EnGk {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnGkActionFunc actionFunc;
    /* 0x1D8 */ Vec3s unk_1D8;
    /* 0x1DE */ Vec3s unk_1DE;
    /* 0x1E4 */ u16 unk_1E4;
    /* 0x1E8 */ Path* path;
    /* 0x1EC */ s32 unk_1EC;
    /* 0x1F0 */ Vec3s jointTable[OBJECT_GK_LIMB_MAX];
    /* 0x1F0 */ Vec3s morphTable[OBJECT_GK_LIMB_MAX];
    /* 0x2E0 */ s16 unk_2E0;
    /* 0x2E2 */ s16 unk_2E2;
    /* 0x2E4 */ s16 animIndex;
    /* 0x2E8 */ Vec3f unk_2E8;
    /* 0x2F4 */ Vec3f unk_2F4;
    /* 0x300 */ Vec3f unk_300;
    /* 0x30C */ Vec3f unk_30C;
    /* 0x318 */ s16 csId;
    /* 0x31A */ u8 csAnimIndex;
    /* 0x31B */ u8 cueId;
    /* 0x31C */ u16 unk_31C;
    /* 0x31E */ s16 unk_31E;
    /* 0x320 */ s16 unk_320;
    /* 0x322 */ s16 unk_322;
    /* 0x324 */ s16 unk_324;
    /* 0x328 */ Vec3f unk_328;
    /* 0x334 */ Vec3s unk_334;
    /* 0x33C */ Vec3f unk_33C;
    /* 0x348 */ Vec3s unk_348;
    /* 0x34E */ s16 unk_34E;
    /* 0x350 */ s16 unk_350;
    /* 0x354 */ f32 unk_354;
} EnGk; // size = 0x358

extern AnimationHeader object_gk_Anim_00787C;
extern AnimationHeader object_gk_Anim_007DC4;
extern AnimationHeader object_gk_Anim_0092C0;
extern AnimationHeader object_gk_Anim_005EDC;
extern AnimationHeader object_gk_Anim_009638;
extern AnimationHeader object_gk_Anim_008774;
extern AnimationHeader object_gk_Anim_00AE34;
extern AnimationHeader object_gk_Anim_00BD90;
extern AnimationHeader object_gk_Anim_00C308;
extern AnimationHeader object_gk_Anim_009858;
extern AnimationHeader object_gk_Anim_009D88;
extern AnimationHeader object_gk_Anim_00A21C;
extern AnimationHeader object_gk_Anim_00AAEC;

static AnimationInfo sAnimationInfo[ENGK_ANIM_MAX] = {
    { &object_gk_Anim_00787C, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, 0.0f }, // ENGK_ANIM_0
    { &object_gk_Anim_007DC4, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, 0.0f }, // ENGK_ANIM_1
    { &object_gk_Anim_0092C0, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, 0.0f }, // ENGK_ANIM_2
    { &object_gk_Anim_005EDC, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, 0.0f }, // ENGK_ANIM_3
    { &object_gk_Anim_009638, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, 0.0f }, // ENGK_ANIM_4
    { &object_gk_Anim_008774, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, 0.0f }, // ENGK_ANIM_5
    { &object_gk_Anim_00AE34, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, 0.0f }, // ENGK_ANIM_6
    { &object_gk_Anim_00BD90, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, 0.0f }, // ENGK_ANIM_7
    { &object_gk_Anim_00C308, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, 0.0f }, // ENGK_ANIM_8
    { &object_gk_Anim_009858, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, 0.0f }, // ENGK_ANIM_9
    { &object_gk_Anim_009D88, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, 0.0f }, // ENGK_ANIM_10
    { &object_gk_Anim_00A21C, 1.0f, 0.0f, 0.0f, ANIMMODE_ONCE, 0.0f }, // ENGK_ANIM_11
    { &object_gk_Anim_00AAEC, 1.0f, 0.0f, 0.0f, ANIMMODE_LOOP, 0.0f }, // ENGK_ANIM_12
};

void func_80B521E8(EnGk* this, PlayState* play);

void EnGk_AfterLullaby(EnGk* this, PlayState* play) {
    Flags_SetSwitch(play, ENGK_GET_SWITCH_FLAG(&this->actor));
    this->animIndex = ENGK_ANIM_3;
    Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, ENGK_ANIM_3);
    // @ap set goron child to be talked to after playing lullaby just in case it isn't done earlier
    SET_WEEKEVENTREG(WEEKEVENTREG_24_80);
    this->actionFunc = func_80B521E8;
}

void EnGk_OfferFullLullaby(EnGk* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnGk_AfterLullaby;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_LULLABY), LOCATION_LULLABY, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH s32 func_80B50854(EnGk* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (!(this->unk_1E4 & 0x40)) {
        if (player->stateFlags2 & PLAYER_STATE2_8000000) {
            this->unk_1E4 |= 0x40;
            Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
        }
    } else if (!(player->stateFlags2 & PLAYER_STATE2_8000000)) {
        this->unk_1E4 &= ~0x40;
    }

    // TODO: have this activate with lullaby intro as well (disable lullaby intro check?)
    if ((player->transformation == PLAYER_FORM_GORON) && (play->msgCtx.ocarinaMode == OCARINA_MODE_EVENT) &&
        (play->msgCtx.lastPlayedSong == OCARINA_SONG_GORON_LULLABY)) {
        
        if (!rando_location_is_checked(LOCATION_LULLABY)) {
            this->actionFunc = EnGk_OfferFullLullaby;
            return true;
        }

        Flags_SetSwitch(play, ENGK_GET_SWITCH_FLAG(&this->actor));
        this->animIndex = ENGK_ANIM_3;
        Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, ENGK_ANIM_3);
        this->actionFunc = func_80B521E8;
        return true;
    }
    return false;
}

void func_80B51D9C(EnGk* this, PlayState* play);
void func_80B51970(EnGk* this, PlayState* play);
void func_80B51B40(EnGk* this, PlayState* play);
void func_80B51760(EnGk* this, PlayState* play);
void func_80B525E0(EnGk* this, PlayState* play);
u16 func_80B50410(EnGk* this, PlayState* play);

void EnGk_GiveGoldDust(EnGk* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = func_80B51760;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, GI_GOLD_DUST_2, 0, 300.0f, 300.0f, true, false);
    }
}

RECOMP_PATCH void func_80B5253C(EnGk* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = func_80B525E0;
    } else {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(GI_GOLD_DUST), GI_GOLD_DUST, 300.0f, 300.0f, true, true);
    }
}

RECOMP_PATCH void func_80B51760(EnGk* this, PlayState* play) {
    s16 curFrame = this->skelAnime.curFrame;
    s16 endFrame;

    if (this->animIndex == ENGK_ANIM_11) {
        endFrame = Animation_GetLastFrame(sAnimationInfo[this->animIndex].animation);
        if (curFrame == endFrame) {
            this->animIndex = ENGK_ANIM_5;
            Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, this->animIndex);
        }
    } else if (this->animIndex == ENGK_ANIM_10) {
        endFrame = Animation_GetLastFrame(sAnimationInfo[this->animIndex].animation);
        if (curFrame == endFrame) {
            this->animIndex = ENGK_ANIM_11;
            Actor_ChangeAnimationByInfo(&this->skelAnime, sAnimationInfo, this->animIndex);
        }
    } else {
        if (Flags_GetSwitch(play, ENGK_GET_SWITCH_FLAG(&this->actor))) {
            SET_WEEKEVENTREG(WEEKEVENTREG_40_40);
            this->actionFunc = func_80B51D9C;
            return;
        }

        if (Actor_ProcessTalkRequest(&this->actor, &play->state)) {
            this->unk_1E4 |= 4;
            this->unk_31C = func_80B50410(this, play);
            Message_StartTextbox(play, this->unk_31C, &this->actor);
            this->actionFunc = func_80B51970;
            if (this->unk_31C == 0xE81) {
                this->animIndex = ENGK_ANIM_0;
                this->unk_1E4 |= 2;
            } else if (this->unk_31C == 0xE92) {
                this->actionFunc = EnGk_GiveGoldDust;
            }
        } else if (((this->actor.xzDistToPlayer < 100.0f) || this->actor.isLockedOn) &&
                   (gSaveContext.save.entrance != ENTRANCE(GORON_RACETRACK, 1))) {
            Actor_OfferTalkNearColChkInfoCylinder(&this->actor, play);
        }

        if (this->unk_1E4 & 4) {
            Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 5, 0x1000, 0x100);
            this->actor.world.rot.y = this->actor.shape.rot.y;
        }
    }
}

RECOMP_PATCH u16 func_80B50410(EnGk* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (play->sceneId == SCENE_17SETUGEN2) {
        if (player->transformation == PLAYER_FORM_GORON) {
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_40_80)) {
                switch (this->unk_31C) {
                    case 0xE7A:
                        return 0xE7B;

                    case 0xE7B:
                        return 0xE7C;

                    case 0xE7C:
                        return 0xE7D;

                    case 0xE7D:
                        return 0xE7E;

                    case 0xE7E:
                        return 0xE7F;

                    case 0xE7F:
                        SET_WEEKEVENTREG(WEEKEVENTREG_40_80);
                        this->unk_1E4 |= 1;
                        return 0xE80;

                    default:
                        return 0xE7A;
                }
            } else {
                this->unk_1E4 |= 1;
                return 0xE81;
            }
        } else if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_41_01)) {
            switch (this->unk_31C) {
                case 0xE82:
                    return 0xE83;

                case 0xE83:
                    return 0xE7D;

                case 0xE7D:
                    return 0xE7E;

                case 0xE7E:
                    return 0xE7F;

                case 0xE7F:
                    SET_WEEKEVENTREG(WEEKEVENTREG_41_01);
                    this->unk_1E4 |= 1;
                    return 0xE80;

                default:
                    return 0xE82;
            }
        } else {
            this->unk_1E4 |= 1;
            return 0xE81;
        }
    } else if (play->sceneId == SCENE_GORONRACE) {
        if (player->transformation == PLAYER_FORM_GORON) {
            if (rando_location_is_checked(GI_GOLD_DUST)) {
                this->unk_1E4 |= 1;
                return 0xE92;
            }

            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_41_04)) {
                if (this->unk_31C == 0xE88) {
                    // if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_41_08) || Inventory_HasEmptyBottle()) {
                    if (!rando_location_is_checked(GI_GOLD_DUST)) {
                        return 0xE89;
                    }
                    SET_WEEKEVENTREG(WEEKEVENTREG_41_04);
                    this->unk_1E4 |= 1;
                    return 0xE94;
                }
                return 0xE88;
            }

            if ((this->unk_31C == 0xE8D) || (this->unk_31C == 0xE98)) {
                // if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_41_08) || Inventory_HasEmptyBottle()) {
                if (!rando_location_is_checked(GI_GOLD_DUST)) {
                    return 0xE89;
                }
                SET_WEEKEVENTREG(WEEKEVENTREG_41_04);
                this->unk_1E4 |= 1;
                return 0xE94;
            }

            if (this->unk_1E4 & 0x10) {
                return 0xE8D;
            }
            return 0xE98;
        }

        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_41_02)) {
            switch (this->unk_31C) {
                case 0xE85:
                    return 0xE86;

                case 0xE86:
                    SET_WEEKEVENTREG(WEEKEVENTREG_41_02);
                    this->unk_1E4 |= 1;
                    return 0xE87;

                default:
                    return 0xE85;
            }
        } else {
            this->unk_1E4 |= 1;
            return 0xE87;
        }
    }

    return 0;
}

RECOMP_PATCH void func_80B52430(EnGk* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_DONE) && Message_ShouldAdvance(play)) {
        switch (this->unk_31C) {
            case 0xE93:
                this->unk_31C = 0xE89;
                Message_StartTextbox(play, this->unk_31C, &this->actor);
                this->actionFunc = func_80B51B40;
                return;

            case 0xE90:
                this->unk_31C = 0xE91;
                Message_StartTextbox(play, this->unk_31C, &this->actor);
                return;

            case 0xE91:
                this->actionFunc = func_80B5253C;
                return;

            case 0xE92:
                this->unk_1E4 &= ~4;
                // this->actionFunc = func_80B51760;
                this->actionFunc = EnGk_GiveGoldDust;
                return;

            default:
                break;
        }
    }

    Math_SmoothStepToS(&this->actor.shape.rot.y, this->actor.yawTowardsPlayer, 5, 0x1000, 0x100);
    this->actor.world.rot.y = this->actor.shape.rot.y;
}