#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"

#include "overlays/actors/ovl_En_Gakufu/z_en_gakufu.h"

// these share a location prefix with wonder items, but they won't collide so we're fine
#define LOCATION_SONGWALL(hour, index) (0x150000 | (hour << 4) | index)

extern Vec3f sRewardDropsSpawnTerminaFieldPos;
extern u8 sRewardDropsIndex[];
extern u8 sRewardDrops[];

void EnGakufu_DoNothing(EnGakufu* this, PlayState* play);

RECOMP_PATCH void EnGakufu_GiveReward(EnGakufu* this, PlayState* play) {
    s32 hour;
    s32 i;

    Audio_PlaySfx(NA_SE_SY_CORRECT_CHIME);

    hour = TIME_TO_HOURS_F(gSaveContext.save.time);
    for (i = 0; i < 3; i++) {
        if (!rando_location_is_checked(LOCATION_SONGWALL(hour, i))) {
            Item_RandoDropCollectible(play, &sRewardDropsSpawnTerminaFieldPos, ITEM00_APITEM, LOCATION_SONGWALL(hour, i));
        } else {
            Item_DropCollectible(play, &sRewardDropsSpawnTerminaFieldPos, sRewardDrops[i + sRewardDropsIndex[hour]]);
        }
    }

    this->actionFunc = EnGakufu_DoNothing;
}