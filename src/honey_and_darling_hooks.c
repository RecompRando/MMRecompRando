#include "modding.h"
#include "global.h"
#include "overlays/actors/ovl_Bg_Fu_Mizu/z_bg_fu_mizu.h"

#include "apcommon.h"

struct EnFu;

typedef void (*EnFuActionFunc)(struct EnFu*, PlayState*);

#define LOCATION_HONEY_AND_DARLING_ANY_DAY 0x0800B5
#define LOCATION_HONEY_AND_DARLING_ALL_DAYS 0x0700B5

#define HONEY_AND_DARLING_LIMB_MAX 0x15

#define ENFU_GET_PATH_INDEX(thisx) (((thisx)->params >> 8) & 0xFF)

typedef struct {
    /* 0x00 */ f32 unk_00;
    /* 0x04 */ UNK_TYPE1 unk_04[0x4];
    /* 0x08 */ Vec3f unk_08;
    /* 0x14 */ Vec3f unk_14;
    /* 0x20 */ Vec3f unk_20;
    /* 0x2C */ s16 unk_2C;
    /* 0x2E */ UNK_TYPE1 unk_2E[0x8];
    /* 0x36 */ u8 unk_36;
    /* 0x37 */ u8 unk_37;
} EnFuUnkStruct; // size = 0x38

typedef struct EnFu {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnFuActionFunc actionFunc;
    /* 0x1D8 */ Vec3s jointTable[HONEY_AND_DARLING_LIMB_MAX];
    /* 0x256 */ Vec3s morphTable[HONEY_AND_DARLING_LIMB_MAX];
    /* 0x2D4 */ BgFuMizu* unk_2D4;
    /* 0x2D8 */ EnFuUnkStruct unk_2D8[10];
    /* 0x508 */ Vec3f unk_508;
    /* 0x514 */ Vec3f unk_514;
    /* 0x520 */ s32 unk_520;
    /* 0x524 */ s16 unk_524[3];
    /* 0x52A */ s16 unk_52A[3];
    /* 0x530 */ UNK_TYPE1 unk530[0x8];
    /* 0x538 */ Vec3s* pathPoints;
    /* 0x53C */ s16 unk_53C;
    /* 0x53E */ s16 unk_53E;
    /* 0x540 */ s16 unk_540;
    /* 0x542 */ s16 unk_542;
    /* 0x544 */ s16 unk_544;
    /* 0x546 */ s16 unk_546;
    /* 0x548 */ s16 unk_548;
    /* 0x54A */ s16 unk_54A;
    /* 0x54C */ s16 unk_54C;
    /* 0x54E */ s16 unk_54E;
    /* 0x550 */ s16 unk_550;
    /* 0x552 */ u16 unk_552;
} EnFu; // size = 0x554

void func_80963610(EnFu* this);

RECOMP_PATCH void func_80963560(EnFu* this, PlayState* play) {
    if ((s16) rando_get_slotdata_u32("shuffle_minigames") == 0) {
        Actor_OfferGetItemHook(&this->actor, play, GI_RUPEE_PURPLE, 0, 500.0f, 100.0f, false, false);
    } else if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        func_80963610(this);
    } else if ((this->unk_552 == 0x2880) && !rando_location_is_checked(LOCATION_HONEY_AND_DARLING_ALL_DAYS)) {
        Actor_OfferGetItem(&this->actor, play, GI_HEART_PIECE, 500.0f, 100.0f);
    } else if ((s16) rando_get_slotdata_u32("shuffle_minigames") == 1) {
        Actor_OfferGetItemHook(&this->actor, play, GI_RUPEE_PURPLE, 0, 500.0f, 100.0f, false, false);
    } else if (!rando_location_is_checked(LOCATION_HONEY_AND_DARLING_ANY_DAY)) {
        Actor_OfferGetItem(&this->actor, play, GI_RUPEE_PURPLE, 500.0f, 100.0f);
    } else {
        Actor_OfferGetItemHook(&this->actor, play, GI_RUPEE_PURPLE, 0, 500.0f, 100.0f, false, false);
    }
    this->actor.child->freezeTimer = 10;
}

void func_80963EAC(EnFu* this, PlayState* play);

RECOMP_PATCH void func_809639D0(EnFu* this, PlayState* play) {
    switch (CURRENT_DAY) {
        case 1:
            if (GET_PLAYER_FORM == PLAYER_FORM_HUMAN) {
                if (!INV_HAS(ITEM_BOMBCHU) && CUR_UPG_VALUE(UPG_BOMB_BAG) == 0) {
                    Message_StartTextbox(play, 0x2853, &this->actor);
                    this->unk_552 = 0x2853;
                } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_22_10)) {
                    Message_StartTextbox(play, 0x284D, &this->actor);
                    this->unk_552 = 0x284D;
                } else if (this->unk_53E == 1) {
                    Message_StartTextbox(play, 0x284F, &this->actor);
                    this->unk_552 = 0x284F;
                } else {
                    this->unk_53E = 1;
                    Message_StartTextbox(play, 0x2851, &this->actor);
                    this->unk_552 = 0x2851;
                }
            } else {
                Message_StartTextbox(play, 0x286F, &this->actor);
                this->unk_552 = 0x286F;
            }
            break;

        case 2:
            if (GET_PLAYER_FORM != PLAYER_FORM_HUMAN) {
                Message_StartTextbox(play, 0x286F, &this->actor);
                this->unk_552 = 0x286F;
            } else if (CUR_UPG_VALUE(UPG_BOMB_BAG) == 0) {
                Message_StartTextbox(play, 0x2853, &this->actor);
                this->unk_552 = 0x2853;
            } else if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_22_10)) {
                if (this->unk_53E == 1) {
                    Message_StartTextbox(play, 0x285B, &this->actor);
                    this->unk_552 = 0x285B;
                } else {
                    this->unk_53E = 1;
                    Message_StartTextbox(play, 0x285D, &this->actor);
                    this->unk_552 = 0x285D;
                }
            } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_22_20)) {
                Message_StartTextbox(play, 0x2855, &this->actor);
                this->unk_552 = 0x2855;
            } else if (this->unk_53E == 1) {
                Message_StartTextbox(play, 0x2857, &this->actor);
                this->unk_552 = 0x2857;
            } else {
                this->unk_53E = 1;
                Message_StartTextbox(play, 0x2859, &this->actor);
                this->unk_552 = 0x2859;
            }
            break;

        case 3:
            if (GET_PLAYER_FORM != PLAYER_FORM_HUMAN) {
                if (GET_PLAYER_FORM == PLAYER_FORM_DEKU) {
                    func_80963EAC(this, play);
                } else {
                    Message_StartTextbox(play, 0x2841, &this->actor);
                    this->unk_552 = 0x2841;
                }
            } else if (CUR_UPG_VALUE(UPG_QUIVER) == 0) {
                Message_StartTextbox(play, 0x284B, &this->actor);
                this->unk_552 = 0x284B;
            } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_22_40)) {
                if (CHECK_WEEKEVENTREG(WEEKEVENTREG_22_10) && CHECK_WEEKEVENTREG(WEEKEVENTREG_22_20)) {
                    Message_StartTextbox(play, 0x285F, &this->actor);
                    this->unk_552 = 0x285F;
                } else {
                    Message_StartTextbox(play, 0x2861, &this->actor);
                    this->unk_552 = 0x2861;
                }
            } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_22_10) && CHECK_WEEKEVENTREG(WEEKEVENTREG_22_20)) {
                if (this->unk_53E == 1) {
                    Message_StartTextbox(play, 0x2863, &this->actor);
                    this->unk_552 = 0x2863;
                } else {
                    this->unk_53E = 1;
                    Message_StartTextbox(play, 0x2865, &this->actor);
                    this->unk_552 = 0x2865;
                }
            } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_22_10) || CHECK_WEEKEVENTREG(WEEKEVENTREG_22_20)) {
                if (this->unk_53E == 1) {
                    Message_StartTextbox(play, 0x2867, &this->actor);
                    this->unk_552 = 0x2867;
                } else {
                    this->unk_53E = 1;
                    Message_StartTextbox(play, 0x2869, &this->actor);
                    this->unk_552 = 0x2869;
                }
            } else if (this->unk_53E == 1) {
                Message_StartTextbox(play, 0x286B, &this->actor);
                this->unk_552 = 0x286B;
            } else {
                this->unk_53E = 1;
                Message_StartTextbox(play, 0x286D, &this->actor);
                this->unk_552 = 0x286D;
            }
            break;

        default:
            break;
    }
}