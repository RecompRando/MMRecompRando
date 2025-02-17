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
#define ENGK_ANIM_3 0x3

typedef enum {
    /* 0 */ ENGK_F_0,
    /* 1 */ ENGK_F_1,
    /* 2 */ ENGK_F_2,
    /* 3 */ ENGK_F_3,
    /* 4 */ ENGK_F_4,
    /* 5 */ ENGK_F_5
} EnGkParam;

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

#define ENGK_ANIM_MAX 0xD

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
        if (player->stateFlags2 & ACTOR_FLAG_LOCK_ON_DISABLED) {
            this->unk_1E4 |= 0x40;
            Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
        }
    } else if (!(player->stateFlags2 & ACTOR_FLAG_LOCK_ON_DISABLED)) {
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