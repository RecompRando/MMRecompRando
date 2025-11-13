#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"
#include "rando_colors.h"
#include "models/custom_butterflies.h"

#define FAIRY_LIMB_MAX 0x7
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "overlays/actors/ovl_En_Butte/z_en_butte.h"

#define LOCATION_FAIRY_BUTTERFLY (AP_PREFIX_BUTTERFLIES | (sceneId << 8) | (play->roomCtx.curRoom.num << 4) \
                                    | randoGetLoadedActorNumInSameRoom(play, thisx))

ActorExtensionId butterflyLocationExtension;
u32* butterflyLocation;
extern ActorExtensionId fairyLocationExtension;
extern u32* fairyLocation;

EnButte* sEnButte;

void func_8091C140(void);
void func_8091D070(EnButte* this);
void func_8091CF64(EnButte* this);

void EnElf_RandoDraw(Actor* thisx, PlayState* play);

u16 butterflyTex[2048];
u16 originalButterflyTex[2048];
extern u64 gameplay_field_keep_Tex_001F30[];

RECOMP_HOOK("EnButte_Init")
void OnEnButte_Init(Actor* thisx, PlayState* play) {
    EnButte* this = ((EnButte*)thisx);
    this->actor.params = BUTTERFLY_1;

    s16 sceneId = play->sceneId;

    // handle grottos
    if (sceneId == SCENE_KAKUSIANA) {
        sceneId = getCurrentGrotto(play);
    }

    butterflyLocation = z64recomp_get_extended_actor_data(thisx, butterflyLocationExtension);
    *butterflyLocation = LOCATION_FAIRY_BUTTERFLY;

    // grab textures
    Lib_MemCpy(originalButterflyTex, SEGMENTED_TO_K0(gameplay_field_keep_Tex_001F30), sizeof(originalButterflyTex));
    RGBA16toIA16_Texture(originalButterflyTex, butterflyTex, ARRAY_COUNT(originalButterflyTex), GRAYSCALE_MAX);
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
        
        butterflyLocation = z64recomp_get_extended_actor_data(&this->actor, butterflyLocationExtension);
        if (rando_get_slotdata_u32("realfairysanity") && !rando_location_is_checked(*butterflyLocation)) {
            fairyLocation = z64recomp_get_extended_actor_data(fairy, fairyLocationExtension);
            *fairyLocation = *butterflyLocation;
            fairy->draw = EnElf_RandoDraw;
        }
    } else if (this->unk_24C <= 0) {
        func_8091D070(this);
    }
}

RECOMP_HOOK("func_8091CBB4")
void EnButte_SetupNotHorribleButterflies(EnButte* this, PlayState* play) {
    sEnButte = this;
}

RECOMP_HOOK_RETURN("func_8091CBB4")
void EnButte_NotHorribleButterflies() {
    EnButte* this = sEnButte;
    PlayState* play = gPlay;
    Player* player = GET_PLAYER(play);

    // if ((player->heldItemAction != PLAYER_IA_DEKU_STICK) || !(fabsf(player->actor.speed) < 1.8f) ||
    //     (this->unk_252 > 0) || !(distSq < SQ(320.0f))) {
        // func_8091C748(this);
    // } else if ((distSq > SQ(240.0f)) &&
                                        //  this->actor.world.pos.x, this->actor.world.pos.z) < SQ(60.0f))) {
    if ((player->heldItemAction == PLAYER_IA_DEKU_STICK) && 
               (Math3D_XZDistanceSquared(player->meleeWeaponInfo[0].tip.x, player->meleeWeaponInfo[0].tip.z,
                                         this->actor.world.pos.x, this->actor.world.pos.z) < SQ(240.0f))) {
        func_8091CF64(this);
    }
}

void func_8091C178(EnButte* this, PlayState* play);

s32 EnButte_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx) {
    butterflyLocation = z64recomp_get_extended_actor_data(thisx, butterflyLocationExtension);
    Color_RGB8 color;

    if (get_rando_color(&color, *butterflyLocation) && rando_get_slotdata_u32("realfairysanity")) {
        OPEN_DISPS(play->state.gfxCtx);

        gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x01, color.r, color.g, color.b, 255);

        CLOSE_DISPS(play->state.gfxCtx);
        
        if (limbIndex == BUTTERFLY_LIMB_04) {
            *dList = randoButterflyLimb04DL;
        }

        if (limbIndex == BUTTERFLY_LIMB_07) {
            *dList = randoButterflyLimb07DL;
        }
    }

    return false;
}

RECOMP_PATCH void EnButte_Draw(Actor* thisx, PlayState* play) {
    EnButte* this = ((EnButte*)thisx);

    if (this->unk_250 != 0) {
        Gfx_SetupDL25_Opa(play->state.gfxCtx);
        SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, EnButte_OverrideLimbDraw, NULL, thisx);
        // SkelAnime_DrawOpa(play, this->skelAnime.skeleton, this->skelAnime.jointTable, NULL, NULL, NULL);
    }

    if ((BUTTERFLY_GET_1(&this->actor) == BUTTERFLY_1) && (this->actionFunc == func_8091CFB4)) {
        func_8091C178(this, play);
    }
}