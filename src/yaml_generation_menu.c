#include "apcommon.h"
#include "z_title_setup.h"
#include "recompui.h"
#include "sys_cfb.h"
#include "idle.h"
#include "recomputils.h"
#include "yaml_generation.h"

RandoYamlConfig* RandoYamlConfig_Create() {
    RandoYamlConfig* retVal = recomp_alloc(sizeof(RandoYamlConfig));
    retVal->accessability = RANDO_ACCESSABILITY_FULL;
    retVal->logicDifficulty = RANDO_LOGIC_DIFFICULTY_NORMAL;
    retVal->chestsMatchContents = true;
    retVal->startSwordless = false;
    retVal->startShieldless = false;
    retVal->startWithSoaring = true;
    retVal->startingHeartsAreRandom = false;
    retVal->startingHeartsMin = 4;
    retVal->startingHeartsMax = 12;
    retVal->startingHearts = 12;
    retVal->startingHeartsAreContainersOrPieces = RANDO_STARTING_HEARTS_ARE_CONTAINERS;
    retVal->shuffleRegionalMaps = RANDO_SHUFFLE_REGIONAL_MAPS_VANILLA;
    retVal->shuffleBossRemains = RANDO_SHUFFLE_BOSS_REMAINS_VANILLA;
    retVal->shuffleSpiderHouseRewards = false;
    retVal->skullSanity = RANDO_SKULLSANITY_VANILLA;
    retVal->shopSanity = RANDO_SHOPSANITY_VANILLA;
    retVal->scrubSanity = false;
    retVal->cowSanity = false;
    retVal->shuffleCreatFairyRewards = false;
    retVal->fairySanity = false;
    retVal->startWithConsumables = false;
    retVal->permanentChateauRomani = true;
    retVal->startWithInvertedTime = true;
    retVal->recieveFilledWallets = true;
    retVal->damageMultiplier = RANDO_DAMAGE_MULITPLIER_NORMAL;
    retVal->deathBehavior = RANDO_DEATH_BEHAVIOR_VANILLA;

    return retVal;
}

RandoYamlConfig* RandoYamlConfig_Destroy(RandoYamlConfig* config) {
    recomp_free(config);
}