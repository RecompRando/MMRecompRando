#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define FAIRY_LIMB_MAX 0x7
#include "overlays/actors/ovl_En_Elf/z_en_elf.h"
#include "overlays/actors/ovl_En_Gs/z_en_gs.h"

#define LOCATION_FAIRY_GOSSIP (AP_PREFIX_GOSSIP | (play->sceneId << 8) | (this->actor.params << 4) | this->unk_195)

void func_80999AC0(EnGs* this);
void func_809984F4(EnGs* this, PlayState* play);
void func_80998300(EnGs* this, PlayState* play);
void func_80997D14(EnGs* this, PlayState* play);

extern u32* fairyLocation;
void EnElf_RandoDraw(Actor* thisx, PlayState* play);

// randomize fairy
RECOMP_PATCH void func_8099807C(EnGs* this, PlayState* play) {
    Actor* fairy = NULL;

    switch (play->msgCtx.ocarinaMode) {
        case OCARINA_MODE_EVENT:
            switch (play->msgCtx.lastPlayedSong) {
                case OCARINA_SONG_HEALING:
                case OCARINA_SONG_EPONAS:
                    if (!Flags_GetSwitch(play, this->switchFlag)) {
                        fairy = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, this->actor.world.pos.x,
                                    this->actor.world.pos.y + 40.0f, this->actor.world.pos.z, 0, 0, 0,
                                    FAIRY_PARAMS(FAIRY_TYPE_2, false, 0));
                        Actor_PlaySfx(&this->actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
                        Flags_SetSwitch(play, this->switchFlag);
                    }
                    break;

                case OCARINA_SONG_STORMS:
                    if (!Flags_GetSwitch(play, this->switchFlag)) {
                        fairy = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ELF, this->actor.world.pos.x,
                                    this->actor.world.pos.y + 40.0f, this->actor.world.pos.z, 0, 0, 0,
                                    FAIRY_PARAMS(FAIRY_TYPE_7, false, 0));
                        Actor_PlaySfx(&this->actor, NA_SE_EV_BUTTERFRY_TO_FAIRY);
                        Flags_SetSwitch(play, this->switchFlag);
                    }
                    break;

                case OCARINA_SONG_SONATA:
                    if ((this->actor.params == ENGS_1) && (GET_PLAYER_FORM == PLAYER_FORM_DEKU)) {
                        this->unk_194 = 1;
                        this->unk_19C = 5;
                        this->unk_19A |= 1;
                        func_80999AC0(this);
                        func_809984F4(this, play);
                    }
                    break;

                case OCARINA_SONG_NEW_WAVE:
                    if ((this->actor.params == ENGS_1) && (GET_PLAYER_FORM == PLAYER_FORM_ZORA)) {
                        this->unk_194 = 3;
                        this->unk_19C = 5;
                        this->unk_19A |= 1;
                        func_80999AC0(this);
                        func_809984F4(this, play);
                    }
                    break;

                case OCARINA_SONG_GORON_LULLABY:
                    if ((this->actor.params == ENGS_1) && (GET_PLAYER_FORM == PLAYER_FORM_GORON)) {
                        this->unk_194 = 2;
                        this->unk_19C = 5;
                        this->unk_19A |= 1;
                        func_80999AC0(this);
                        func_809984F4(this, play);
                    }
                    break;

                default:
                    break;
            }
            break;

        case OCARINA_MODE_NONE:
        case OCARINA_MODE_END:
            func_80998300(this, play);

        case OCARINA_MODE_APPLY_DOUBLE_SOT:
            func_80997D14(this, play);
            break;

        default:
            break;
    }

    if (fairy != NULL && rando_get_slotdata_u32("realfairysanity") && !rando_location_is_checked(LOCATION_FAIRY_GOSSIP)) {
        fairyLocation = z64recomp_get_extended_actor_data(fairy, fairyLocationExtension);
        *fairyLocation = LOCATION_FAIRY_GOSSIP;
        fairy->draw = EnElf_RandoDraw;
    }
}