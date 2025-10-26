#include "modding.h"
#include "global.h"

#include "apcommon.h"
#define LOCATION_TAKKURI_HUGO (0x170000 | (play->sceneId << 8) | (0xD << 4) | ITEM00_RUPEE_HUGE)

#define TAKKURI_LIMB_MAX 0x11
#include "overlays/actors/ovl_En_Thiefbird/z_en_thiefbird.h"

// todo: add to static symbols
static s16 D_80C13664[] = {
    ITEM00_ARROWS_10, ITEM00_BOMBS_B, ITEM00_RUPEE_GREEN, ITEM00_RUPEE_BLUE, ITEM00_RUPEE_RED, ITEM00_RUPEE_PURPLE,
};

// disable takkuri item stealing
RECOMP_PATCH s32 func_80C10B0C(EnThiefbird* this, PlayState* play) {
    return false;
}

// for quick testing
// RECOMP_HOOK("func_80C12B1C")
// void EnThiefbird_OneShot(EnThiefbird* this, PlayState* play) {
//     if (this->collider.base.acFlags & AC_HIT) {
//         this->actor.colChkInfo.health = 0;
//     }
// }

RECOMP_PATCH void func_80C11DF0(EnThiefbird* this, PlayState* play) {
    s32 i;
    s32 j;

    if (Math_ScaledStepToS(&this->actor.shape.rot.x, -0x8000, 0x800)) {
        Math_ScaledStepToS(&this->unk_192, 0x1C00, 0x200);
        this->actor.shape.rot.y += this->unk_192;
    }

    if ((this->actor.bgCheckFlags & BGCHECKFLAG_GROUND) || (this->actor.floorHeight == BGCHECK_Y_MIN)) {
        for (i = 0; i < EN_THIEFBIRD_BODYPART_MAX; i++) {
            func_800B3030(play, &this->bodyPartsPos[i], &gZeroVec3f, &gZeroVec3f, 0x8C, 0, 0);
        }

        SoundSource_PlaySfxAtFixedWorldPos(play, &this->actor.world.pos, 11, NA_SE_EN_EXTINCT);
        
        // @rando replace takkuri huge rupee drop
        if (rando_get_slotdata_u32("rupeesanity") && !rando_location_is_checked(LOCATION_TAKKURI_HUGO)) {
            Actor* item = Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, LOCATION_TAKKURI_HUGO);
            Actor_SetScale(item, 0.035f); // make item slightly larger to mimic huge rupee scale (0.045f)
        } else {
            Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RUPEE_HUGE);
        }

        for (i = 0; i < ARRAY_COUNT(D_80C13664); i++) {
            for (j = 0; j < this->unk_196[i]; j++) {
                Item_DropCollectible(play, &this->actor.world.pos, D_80C13664[i]);
            }
        }

        Actor_Kill(&this->actor);
    }
}