#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "actor_helpers.h"
#include "apcommon.h"

void registerActorExtensions() {
    item00Extension = z64recomp_extend_actor(ACTOR_EN_ITEM00, sizeof(u32));
    mureExtension = z64recomp_extend_actor(ACTOR_OBJ_MURE, sizeof(u32));
    mure2Extension = z64recomp_extend_actor(ACTOR_OBJ_MURE2, sizeof(u32));
    kusaGrassExtension = z64recomp_extend_actor(ACTOR_EN_KUSA, sizeof(u32));
    keatonGrassExtension = z64recomp_extend_actor(ACTOR_EN_KUSA2, sizeof(u32));
    potExtension = z64recomp_extend_actor(ACTOR_OBJ_TSUBO, sizeof(u32));
    potFlyingExtension = z64recomp_extend_actor(ACTOR_EN_TUBO_TRAP, sizeof(u32));
    potFlowerExtension = z64recomp_extend_actor(ACTOR_OBJ_FLOWERPOT, sizeof(u32));
    wonderHitExtension = z64recomp_extend_actor(ACTOR_EN_HIT_TAG, sizeof(u32));
    wonderRupeeExtension = z64recomp_extend_actor(ACTOR_EN_INVISIBLE_RUPPE, sizeof(u32));
    rockExtension = z64recomp_extend_actor(ACTOR_EN_ISHI, sizeof(u32));
    boulderExtension = z64recomp_extend_actor(ACTOR_OBJ_BOMBIWA, sizeof(u32));
    boulder2Extension = z64recomp_extend_actor(ACTOR_OBJ_HAMISHI, sizeof(u32));
    snowballExtension = z64recomp_extend_actor(ACTOR_OBJ_SNOWBALL2, sizeof(u32));
    bigSnowballExtension = z64recomp_extend_actor(ACTOR_OBJ_SNOWBALL, sizeof(u32));
    gamelupyExtension = z64recomp_extend_actor(ACTOR_EN_GAMELUPY, sizeof(u32));
    crateSmallExtension = z64recomp_extend_actor(ACTOR_OBJ_KIBAKO, sizeof(u32));
    crateBigExtension = z64recomp_extend_actor(ACTOR_OBJ_KIBAKO2, sizeof(u32));
    barrelExtension = z64recomp_extend_actor(ACTOR_OBJ_TARU, sizeof(u32));
    icicleExtension = z64recomp_extend_actor(ACTOR_BG_ICICLE, sizeof(u32));
    beehiveExtension = z64recomp_extend_actor(ACTOR_OBJ_COMB, sizeof(u32));
    fairyExtension = z64recomp_extend_actor(ACTOR_EN_ELF, sizeof(u32));
    butterflyExtension = z64recomp_extend_actor(ACTOR_EN_BUTTE, sizeof(u32));
}

s32 randoGetLoadedActorNumInSameRoom(PlayState* play, Actor* actorIn) {
    u8 actorCat = actorIn->category;
    s8 actorRoom = actorIn->room;
    s16 actorId = actorIn->id;

    Actor* actor = play->actorCtx.actorLists[actorCat].first;
    s32 actorIndex = 0;

    while (actor != NULL) {
        if (actor->id == actorId && actor->room == actorRoom) {
            if (actor == actorIn) {
                return actorIndex;
            }
            actorIndex++;
        }
        actor = actor->next;
    }

    return -1; // should never return -1 if used correctly
}

// note: this only works if both actors are in the same category
s32 randoGetLoadedActorNumInSameRoomExtra(PlayState* play, Actor* actorIn, s16 extraId) {
    u8 actorCat = actorIn->category;
    s8 actorRoom = actorIn->room;
    s16 actorId = actorIn->id;

    Actor* actor = play->actorCtx.actorLists[actorCat].first;
    s32 actorIndex = 0;

    while (actor != NULL) {
        if ((actor->id == actorId || actor->id == extraId) && actor->room == actorRoom) {
            if (actor == actorIn) {
                return actorIndex;
            }
            actorIndex++;
        }
        actor = actor->next;
    }

    return -1; // should never return -1 if used correctly
}

typedef struct {
    /* 0x00 */ PlayState* play;
    /* 0x04 */ Actor* actor;
    /* 0x08 */ u32 requiredActorFlag;
    /* 0x0C */ u32 canFreezeCategory;
    /* 0x10 */ Actor* talkActor;
    /* 0x14 */ Player* player;
    /* 0x18 */ u32 unk_18; // Bitmask of actor flags. The actor will only have main called if it has at least 1
                           // flag set that matches this bitmask
} UpdateActor_Params;      // size = 0x1C

RECOMP_HOOK("Actor_UpdateActor")
void FreezeActors(UpdateActor_Params* params) {
    PlayState* play = params->play;
    Actor* actor = params->actor;
    Player* player = GET_PLAYER(play);

    // force actors to freeze when grabbing items w/ text
    if (CutsceneManager_GetCurrentCsId() == CS_ID_GLOBAL_TALK) {
        if (actor->id == ACTOR_OBJ_BEAN ||          // magic bean plant
            actor->id == ACTOR_EN_KUSA2 ||          // keaton grass
            actor->id == ACTOR_OBJ_LUPYGAMELIFT) {  // deku playground elevator
            // PLAYER_STATE1_20000000 is used when grabbing items
            params->canFreezeCategory |= PLAYER_STATE1_20000000 & player->stateFlags1;
        }
    }
}

RECOMP_CALLBACK("*", recomp_should_actor_init)
void Rando_ShouldActorInit(PlayState* play, Actor* actor, bool* should) {
    if(actor == NULL) return;
    if(gSaveContext.gameMode != GAMEMODE_NORMAL) return;

    // recomp_printf("actor id: 0x%02X 0x%06X %d\n", actor->id, 0x0B0000 | actor->id, rando_has_item(0x0B0000 | actor->id));
    switch (actor->id) {
        case ACTOR_BOSS_01: // odolwa
        case ACTOR_BOSS_HAKUGIN: // goht
        case ACTOR_BOSS_03: // gyorg
        case ACTOR_BOSS_02: // twinmold
        case ACTOR_BOSS_07: // majora
            if (rando_get_slotdata_u32("boss_souls")) {
                if (actor->id == ACTOR_BOSS_07 && rando_get_slotdata_u32("boss_souls") != 2) return;
                // *should = rando_has_item(0x0B0000 | actor->id);
                if (!rando_has_item(0x0B0000 | actor->id)) {
                    *should = false;
                }
                return;
            }
    }

    switch (actor->id) {
        case ACTOR_EN_SW:
        case ACTOR_EN_COW:
        case ACTOR_EN_KITAN:
        case ACTOR_EN_BUTTE:
            if (rando_get_slotdata_u32("misc_souls")) {
                // *should = rando_has_item(0x0A0000 | actor->id);
                if (!rando_has_item(0x0A0000 | actor->id)) {
                    *should = false;
                }
                return;
            }
            break;
    }

    switch (actor->id) {
        case ACTOR_EN_TAKARAYA:
        case ACTOR_EN_JA:
        case ACTOR_EN_DAIKU:
        case ACTOR_EN_DAIKU2:
        case ACTOR_EN_GURUGURU:
        case ACTOR_EN_YB:
        case ACTOR_EN_NB:
        case ACTOR_EN_AN:
        case ACTOR_EN_PM:
        case ACTOR_EN_OSSAN:
        case ACTOR_EN_HG:
        case ACTOR_EN_SHN:
        case ACTOR_EN_STONE_HEISHI:
        case ACTOR_EN_PO_COMPOSER:
        case ACTOR_EN_S_GORO:
        case ACTOR_EN_FU:
        case ACTOR_EN_SSH:
        case ACTOR_EN_IG:
        case ACTOR_EN_AOB_01:
        case ACTOR_EN_JG:
        case ACTOR_EN_TRT:
        case ACTOR_EN_KENDO_JS:
        case ACTOR_EN_GINKO_MAN:
        case ACTOR_EN_RZ:
        case ACTOR_EN_HS:
        case ACTOR_EN_AL:
        case ACTOR_EN_OSN:
        case ACTOR_EN_ELFGRP:
        case ACTOR_EN_BAL:
        case ACTOR_EN_MK:
        case ACTOR_EN_BJT:
        case ACTOR_EN_AH:
        case ACTOR_EN_TAB:
        case ACTOR_EN_RECEPGIRL:
        case ACTOR_EN_FSN:
        case ACTOR_EN_BJI_01:
        case ACTOR_EN_TEST3:
        case ACTOR_EN_BABA:
        case ACTOR_EN_SUTTARI:
        case ACTOR_EN_KAKASI:
        case ACTOR_EN_GUARD_NUTS:
        case ACTOR_EN_LOOK_NUTS:
        case ACTOR_EN_TRU:
        case ACTOR_EN_ZOW:
        case ACTOR_EN_ZOT:
        case ACTOR_EN_ZOV:
        case ACTOR_EN_KUJIYA:
        case ACTOR_EN_LIFT_NUTS:
        case ACTOR_EN_GO:
        case ACTOR_EN_STH:
        case ACTOR_EN_DG: //Dogs
        case ACTOR_EN_STOP_HEISHI: //Town Guards
        case ACTOR_DM_STK:
        case ACTOR_EN_GB2:
        case ACTOR_EN_GEG: // Hungry Goron
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | actor->id);
                if (!rando_has_item(0x0C0000 | actor->id)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_DT: // Council Soul
        case ACTOR_EN_MUTO:
        case ACTOR_EN_BAISEN:
        case ACTOR_EN_HEISHI: 
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_DT);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_DT)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_AKINDONUTS: // Business Scrub Soul
        case ACTOR_EN_SELLNUTS:
        case ACTOR_EN_SCOPENUTS:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_AKINDONUTS);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_AKINDONUTS)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_BOMJIMA: //Bombers Soul
        case ACTOR_EN_BOMBERS:
        case ACTOR_EN_BOMBERS2:
        case ACTOR_EN_BOMJIMB:
        case ACTOR_EN_BOMBAL:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_BOMJIMA);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_BOMJIMA)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_TOTO: //Toto and Gorman (To prevent Gorman Softlock)
        case ACTOR_EN_GM:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_TOTO);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_TOTO)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_KGY: //Smithy
        case ACTOR_EN_KBT:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_KGY);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_KGY)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_JGAME_TSN: //Fisherman
        case ACTOR_EN_TSN:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_TSN);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_TSN)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_SKB:
        case ACTOR_EN_RAIL_SKB: // Graveyard Stalchildren
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_SKB);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_SKB)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_RAILGIBUD: //Gibdo Soul
        case ACTOR_EN_TALK_GIBUD:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_TALK_GIBUD);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_TALK_GIBUD)) {
                    *should = false;
                }
                return;
            }
            break;
}
    switch (actor->id) {
        case ACTOR_EN_GAKUFU:
        // case ACTOR_OBJ_ETCETERA:
            if (rando_get_slotdata_u32("absurd_souls")) {
                // *should = rando_has_item(0x0E0000 | actor->id);
                if (!rando_has_item(0x0E0000 | actor->id)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_WOOD02: //Trees/Shrubs
        case ACTOR_EN_SNOWWD: //
        case ACTOR_OBJ_TREE:
        case ACTOR_OBJ_YASI:
            if (rando_get_slotdata_u32("absurd_souls")) {
                // *should = rando_has_item(0x0E0000 | ACTOR_EN_WOOD2);
                if (!rando_has_item(0x0E0000 | ACTOR_EN_WOOD02)) {
                    *should = false;
                }
                return;
            }
            break;
    }
    switch (actor->id) {
        case ACTOR_EN_PST:    // Postboxes
            if (rando_get_slotdata_u32("utility_souls")) {
                // *should = rando_has_item(0x0D0000 | actor->id);
                if (!rando_has_item(0x0D0000 | actor->id)) {
                    *should = false;
                }
                return;
            }
            break;
    }
    switch (actor->id) {
        //case ACTOR_EN_GB2:
            recomp_printf("actor soul id: 0x%06X\n", 0x0C0000 | actor->id);
            *should = false;
            break;
    }
    recomp_printf("actor soul id: 0x%06X\n", 0x0C0000 | actor->id);
}