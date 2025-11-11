#include "modding.h"
#include "global.h"

#include "apcommon.h"

bool rando_met_remains_condition(u32 required_amount) {
    return ((CHECK_QUEST_ITEM(QUEST_REMAINS_ODOLWA) > 0) +
            (CHECK_QUEST_ITEM(QUEST_REMAINS_GOHT) > 0) +
            (CHECK_QUEST_ITEM(QUEST_REMAINS_GYORG) > 0) +
            (CHECK_QUEST_ITEM(QUEST_REMAINS_TWINMOLD) > 0)) >= required_amount;
}

bool rando_met_moon_condition() {
    return rando_met_remains_condition(rando_get_moon_remains_required());
}

bool rando_met_majora_condition() {
    return rando_met_remains_condition(rando_get_majora_remains_required());
}