#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define FAIRY_LIMB_MAX 0x7
#define BUTTERFLY_LIMB_MAX 0x8
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "overlays/actors/ovl_En_Butte/z_en_butte.h"

#define LOCATION_FAIRY_BUTTERFLY (0xBF0000 | (play->sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                                    | randoGetLoadedActorNumInSameRoom(play, thisx))

ActorExtensionId butterflyExtension;
u32* extendedButterflyData;
extern ActorExtensionId fairyExtension;
extern u32* extendedFairyData;

void func_8091C140(void);
void func_8091D070(EnButte* this);

void EnElf_RandoDraw(Actor* thisx, PlayState* play);

RECOMP_HOOK("EnButte_Init")
void OnEnButte_Init(Actor* thisx, PlayState* play) {
    extendedButterflyData = z64recomp_get_extended_actor_data(thisx, butterflyExtension);
    *extendedButterflyData = LOCATION_FAIRY_BUTTERFLY;
}

RECOMP_PATCH void func_8091CFB4(EnButte* this, PlayState* play) {
    Actor* fairy;
    SkelAnime_Update(&this->skelAnime);
    func_8091C140();

    if (this->unk_24C == 5) {
        SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 60, NA_SE_EV_BUTTERFRY_TO_FAIRY);
    } else if (this->unk_24C == 4) {
        fairy = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, this->actor.focus.pos.x, this->actor.focus.pos.y,
                    this->actor.focus.pos.z, 0, this->actor.shape.rot.y, 0, FAIRY_PARAMS(FAIRY_TYPE_2, false, 0));
        this->unk_250 = 0;
        
        extendedButterflyData = z64recomp_get_extended_actor_data(&this->actor, butterflyExtension);
        extendedFairyData = z64recomp_get_extended_actor_data(fairy, fairyExtension);
        *extendedFairyData = *extendedButterflyData;
        fairy->draw = EnElf_RandoDraw;
    } else if (this->unk_24C <= 0) {
        func_8091D070(this);
    }
}