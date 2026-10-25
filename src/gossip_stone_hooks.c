#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"

#include "overlays/actors/ovl_En_Gs/z_en_gs.h"

// skip the mask of truth requirement by overriding what text is about to be displayed
RECOMP_HOOK("func_80997E4C")
void EnGs_HandleText(EnGs* this, PlayState* play) {
    switch (this->actor.params) {
        case ENGS_0:
            this->unk_210 = this->unk_195 + 0x20D3;
            if (this->unk_210 >= 0x20D4) {
                s32 temp_v1 = this->unk_210 - 0x20D4;

                // if ((this->unk_210 < 0x20E8) &&
                //     ((temp_v1 + ITEM_MASK_TRUTH) == INV_CONTENT(temp_v1 + ITEM_MASK_TRUTH))) {
                if (this->unk_210 < 0x20E8) {
                    this->unk_210 = temp_v1 + 0x2103;
                }
            }
            break;

        case ENGS_3:
            this->unk_210 = this->unk_195 + 0x20B0;
            break;

        case ENGS_1:
            this->unk_210 = this->unk_195 + 0x20F3;
            break;

        case ENGS_2:
            this->unk_210 = this->unk_195 + 0x20F7;
            break;

        default:
            break;
    }
}