#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct EnJgameTsn;

typedef void (*EnJgameTsnActionFunc)(struct EnJgameTsn*, PlayState*);
RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

typedef struct {
    /* 0x00 */ Vec3s* points;
    /* 0x04 */ s32 count;
} EnJgameTsnStruct; // size = 0x8

#define OBJECT_TSN_LIMB_MAX 0x11

typedef struct EnJgameTsn {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnJgameTsnActionFunc actionFunc;
    /* 0x1D8 */ EnJgameTsnStruct unk_1D8[4];
    /* 0x1F8 */ EnJgameTsnStruct unk_1F8;
    /* 0x200 */ EnJgameTsnStruct unk_200;
    /* 0x208 */ u8* unk_208[4];
    /* 0x218 */ s32 unk_218;
    /* 0x21C */ s32 unk_21C;
    /* 0x220 */ Vec3s jointTable[OBJECT_TSN_LIMB_MAX];
    /* 0x286 */ Vec3s morphTable[OBJECT_TSN_LIMB_MAX];
    /* 0x2EC */ Vec3s unk_2EC;
    /* 0x2F2 */ Vec3s unk_2F2;
    /* 0x2F8 */ s16 unk_2F8;
    /* 0x2FA */ s16 unk_2FA;
    /* 0x2FC */ s16 unk_2FC;
    /* 0x2FE */ s16 unk_2FE;
    /* 0x300 */ u16 unk_300;
} EnJgameTsn; // size = 0x304

#define OBJLUPYGAMELIFT_IGNITE_FIRE (1 << 0)

void func_80C14030(EnJgameTsn* this);
void func_80C145FC(EnJgameTsn* this);

RECOMP_HOOK("func_80C14230") void on_fisherman_game() {
	if (gSaveContext.minigameScore >= 20) {
		gSaveContext.timerCurTimes[TIMER_ID_MINIGAME_2] = SECONDS_TO_TIMER(0);
    }
}

RECOMP_PATCH void func_80C14554(EnJgameTsn* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_FISHERMANS_JUMPING_GAME_HEART_PIECE)) {
            SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_FISHERMANS_JUMPING_GAME_HEART_PIECE);
        }
        func_80C145FC(this);
    // } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_FISHERMANS_JUMPING_GAME_HEART_PIECE)) {
    //     Actor_OfferGetItem(&this->actor, play, GI_RUPEE_PURPLE, 500.0f, 100.0f);
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_HEART_PIECE, 500.0f, 100.0f);
    }
}