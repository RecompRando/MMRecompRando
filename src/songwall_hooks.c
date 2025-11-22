#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"

#include "overlays/actors/ovl_En_Gakufu/z_en_gakufu.h"

// these share a location prefix with wonder items, but they won't collide so we're fine
#define LOCATION_SONGWALL(index, i) (AP_PREFIX_SONGWALL | (index << 4) | i)
#define LOCATION_SONGWALL_HOUR(hour, i) (AP_PREFIX_SONGWALL | (hour << 4) | i)

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
        u32 location = LOCATION_SONGWALL((i + sRewardDropsIndex[hour]), i);
        // if (every_hour_option) { // this ends up actually adding more checks than in the game
        //     location = LOCATION_SONGWALL_HOUR(hour, i);
        // }
        
        if (!rando_location_is_checked(location)) {
            Item_RandoDropCollectible(play, &sRewardDropsSpawnTerminaFieldPos, ITEM00_APITEM, location);
        } else {
            Item_DropCollectible(play, &sRewardDropsSpawnTerminaFieldPos, sRewardDrops[i + sRewardDropsIndex[hour]]);
        }
    }

    this->actionFunc = EnGakufu_DoNothing;
}