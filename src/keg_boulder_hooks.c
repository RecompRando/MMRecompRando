#include "modding.h"
#include "global.h"

#include "apcommon.h"

struct ObjHugebombiwa;

#define THIS ((ObjHugebombiwa*)thisx)

typedef void (*ObjHugebombiwaActionFunc)(struct ObjHugebombiwa*, PlayState*);

#define ENHUGEBOMBIWA_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)
#define ENHUGEBOMBIWA_GET_100(thisx) (((thisx)->params >> 8) & 1)

typedef struct {
    /* 0x00 */ Vec3f unk_00;
    /* 0x0C */ Vec3f unk_0C;
    /* 0x18 */ f32 unk_18;
    /* 0x1C */ Vec3s unk_1C;
    /* 0x22 */ s16 unk_22;
    /* 0x24 */ s16 unk_24;
} EnHugebombiwaStruct; // size = 0x28

typedef struct ObjHugebombiwa {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ EnHugebombiwaStruct unk_190[20];
    /* 0x4B0 */ s16 unk_4B0;
    /* 0x4B2 */ s8 unk_4B2;
    /* 0x4B3 */ s8 unk_4B3;
    /* 0x4B4 */ ObjHugebombiwaActionFunc actionFunc;
} ObjHugebombiwa; // size = 0x4B8

RECOMP_HOOK("ObjHugebombiwa_Init")
void ObjHugebombiwa_KillIfHasKegPrivileges(Actor* thisx, PlayState* play) {
    ObjHugebombiwa* this = THIS;
    if (rando_location_is_checked(GI_POWDER_KEG)) {
        SET_WEEKEVENTREG(WEEKEVENTREG_19_02);
        Actor_Kill(&this->actor);
    }
}

void func_80A53BE0(PlayState* play, Vec3f* arg1);
void func_80A54E10(ObjHugebombiwa* this);

void func_80A541F4(ObjHugebombiwa* this, PlayState* play);
void func_80A55310(ObjHugebombiwa* this);

RECOMP_PATCH void func_80A54CEC(ObjHugebombiwa* this, PlayState* play) {
    s32 pad;

    if (CutsceneManager_IsNext(this->actor.csId)) {
        // CutsceneManager_StartWithPlayerCs(this->actor.csId, &this->actor);
        Flags_SetSwitch(play, ENHUGEBOMBIWA_GET_SWITCH_FLAG(&this->actor));
        if (!ENHUGEBOMBIWA_GET_100(&this->actor) &&
            ((play->sceneId == SCENE_17SETUGEN) || (play->sceneId == SCENE_17SETUGEN2))) {
            SET_WEEKEVENTREG(WEEKEVENTREG_19_02);
        }

        if (!ENHUGEBOMBIWA_GET_100(&this->actor)) {
            SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 80, NA_SE_EV_WALL_BROKEN);
        } else {
            SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 80, NA_SE_EV_SNOWBALL_BROKEN);
        }

        if (!ENHUGEBOMBIWA_GET_100(&this->actor)) {
            func_80A53BE0(play, &this->actor.world.pos);
            func_80A54E10(this);
        } else {
            func_80A541F4(this, play);
            func_80A55310(this);
        }
    } else {
        CutsceneManager_Queue(this->actor.csId);
    }
}