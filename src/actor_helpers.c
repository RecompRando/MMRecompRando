#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "actor_helpers.h"
#include "apcommon.h"

void registerActorExtensions() {
    item00LocationExtension = z64recomp_extend_actor(ACTOR_EN_ITEM00, sizeof(u32));
    mureIndexExtension = z64recomp_extend_actor(ACTOR_OBJ_MURE, sizeof(u32));
    mure2IndexExtension = z64recomp_extend_actor(ACTOR_OBJ_MURE2, sizeof(u32));
    kusaGrassLocationExtension = z64recomp_extend_actor(ACTOR_EN_KUSA, sizeof(u32));
    keatonGrassLocationExtension = z64recomp_extend_actor(ACTOR_EN_KUSA2, sizeof(u32));
    potLocationExtension = z64recomp_extend_actor(ACTOR_OBJ_TSUBO, sizeof(u32));
    potDroppedExtension = z64recomp_extend_actor(ACTOR_OBJ_TSUBO, sizeof(bool));
    potFlyingLocationExtension = z64recomp_extend_actor(ACTOR_EN_TUBO_TRAP, sizeof(u32));
    potFlowerLocationExtension = z64recomp_extend_actor(ACTOR_OBJ_FLOWERPOT, sizeof(u32));
    wonderHitLocationExtension = z64recomp_extend_actor(ACTOR_EN_HIT_TAG, sizeof(u32));
    wonderHitTimerExtension = z64recomp_extend_actor(ACTOR_EN_HIT_TAG, sizeof(u32));
    wonderRupeeLocationExtension = z64recomp_extend_actor(ACTOR_EN_INVISIBLE_RUPPE, sizeof(u32));
    rockLocationExtension = z64recomp_extend_actor(ACTOR_EN_ISHI, sizeof(u32));
    boulderLocationExtension = z64recomp_extend_actor(ACTOR_OBJ_BOMBIWA, sizeof(u32));
    boulder2LocationExtension = z64recomp_extend_actor(ACTOR_OBJ_HAMISHI, sizeof(u32));
    snowballLocationExtension = z64recomp_extend_actor(ACTOR_OBJ_SNOWBALL2, sizeof(u32));
    bigSnowballLocationExtension = z64recomp_extend_actor(ACTOR_OBJ_SNOWBALL, sizeof(u32));
    gamelupyLocationExtension = z64recomp_extend_actor(ACTOR_EN_GAMELUPY, sizeof(u32));
    crateSmallLocationExtension = z64recomp_extend_actor(ACTOR_OBJ_KIBAKO, sizeof(u32));
    crateBigLocationExtension = z64recomp_extend_actor(ACTOR_OBJ_KIBAKO2, sizeof(u32));
    barrelLocationExtension = z64recomp_extend_actor(ACTOR_OBJ_TARU, sizeof(u32));
    icicleLocationExtension = z64recomp_extend_actor(ACTOR_BG_ICICLE, sizeof(u32));
    icicleDroppedExtension = z64recomp_extend_actor(ACTOR_BG_ICICLE, sizeof(bool));
    beehiveLocationExtension = z64recomp_extend_actor(ACTOR_OBJ_COMB, sizeof(u32));
    fairyLocationExtension = z64recomp_extend_actor(ACTOR_EN_ELF, sizeof(u32));
    butterflyLocationExtension = z64recomp_extend_actor(ACTOR_EN_BUTTE, sizeof(u32));
    palmTreeDropExtension = z64recomp_extend_actor(ACTOR_OBJ_YASI, sizeof(bool));
    // snowTreeDropExtension = z64recomp_extend_actor(ACTOR_EN_SNOWWD, sizeof(bool));
    woodTreeDropExtension = z64recomp_extend_actor(ACTOR_EN_WOOD02, sizeof(bool));
    bigTreeDropExtension = z64recomp_extend_actor(ACTOR_OBJ_TREE, sizeof(bool));
    signLocationExtension = z64recomp_extend_actor(ACTOR_EN_KANBAN, sizeof(u32));
    signDropExtension = z64recomp_extend_actor(ACTOR_EN_KANBAN, sizeof(bool));
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
        case ACTOR_EN_DAIKU: //Carpenters
        case ACTOR_EN_DAIKU2: //Milk road Carpenter
        case ACTOR_EN_GURUGURU:
        case ACTOR_EN_YB:
        case ACTOR_EN_NB:
        case ACTOR_EN_AN: //Anju
        case ACTOR_EN_PM: //Postman
        case ACTOR_EN_OSSAN: //EVERY SHOP EVER
        case ACTOR_EN_HG:
        case ACTOR_EN_SHN:
        case ACTOR_EN_STONE_HEISHI: //Town Exit Guards
        case ACTOR_EN_PO_COMPOSER: //Poe Composer
        case ACTOR_EN_S_GORO:
        case ACTOR_EN_FU:
        case ACTOR_EN_IG:
        case ACTOR_EN_JG:
        case ACTOR_EN_TRT:
        case ACTOR_EN_KENDO_JS:
        case ACTOR_EN_GINKO_MAN:
        case ACTOR_EN_RZ:
        case ACTOR_EN_HS:
        case ACTOR_EN_AL:
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
        case ACTOR_EN_MS: //Bean Daddy
        case ACTOR_EN_MNK:
        case ACTOR_EN_TRU:
        case ACTOR_EN_IN: //Gorman Bros
        case ACTOR_EN_ZOW:
        case ACTOR_EN_ZOT:
        case ACTOR_EN_ZOV:
        case ACTOR_EN_KUJIYA:
        case ACTOR_EN_LIFT_NUTS:
        case ACTOR_EN_GO:
        case ACTOR_EN_STOP_HEISHI: //Town Guards
        case ACTOR_DM_STK:
        case ACTOR_EN_TK: //Dampe
        case ACTOR_EN_GB2:
        case ACTOR_EN_GEG: // Hungry Goron
        case ACTOR_EN_JS: //Moon Kids
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | actor->id);
                if (!rando_has_item(0x0C0000 | actor->id)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_OSN: // Happy Mask Salesman
            if (rando_get_slotdata_u32("npc_souls")) {
                // don't prevent happy mask salesman from spawning on moon crash
                if (justDied) {
                    return;
                }
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
        case ACTOR_EN_SSH: // Spider Man
        case ACTOR_EN_STH:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_SSH);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_SSH)) {
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
        case ACTOR_EN_AOB_01: //Doggy Race Lady
        case ACTOR_EN_DG: //Dogs
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_AOB_01);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_AOB_01)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_EN_MA_YTO: //(Cremia) Romani and Cremia
        case ACTOR_EN_MA_YTS: //(Romani)
        case ACTOR_EN_MA4:
        case ACTOR_EN_INVADEPOH:
        case ACTOR_OBJ_DINNER: //(I wonder what's for dinner?)
        case ACTOR_OBJ_UM: // Cremia Escort Milk Bottles
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_MA_YTS);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_MA_YTS)) {
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
        case ACTOR_EN_RAIL_SKB: 
        case ACTOR_EN_HINT_SKB: // Graveyard and OSH Stalchildren
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
        case ACTOR_EN_ELFGRP: // Great Fairy Soul
        case ACTOR_BG_DY_YOSEIZO:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(0x0C0000 | ACTOR_EN_ELFGRP);
                if (!rando_has_item(0x0C0000 | ACTOR_EN_ELFGRP)) {
                    *should = false;
                }
                return;
            }
            break;
    }
    switch (actor->id) {
        case ACTOR_EN_GAKUFU:
        case ACTOR_OBJ_ETCETERA: // Deku Flowers
        case ACTOR_DOOR_ANA: // Grottos
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
        case ACTOR_OBJ_GRASS: //Grass
        case ACTOR_OBJ_GRASS_UNIT:
        case ACTOR_OBJ_GRASS_CARRY:
        case ACTOR_EN_KUSA:
        case ACTOR_EN_KUSA2:
            if (rando_get_slotdata_u32("absurd_souls")) {
                // *should = rando_has_item(0x0E0000 | ACTOR_EN_WOOD2);
                if (!rando_has_item(0x0E0000 | ACTOR_OBJ_GRASS)) {
                    *should = false;
                }
                return;
            }
            break;
        case ACTOR_OBJ_TARU: // Barrels
            if (rando_get_slotdata_u32("absurd_souls")) {
                // ignore pirates' fortress bariers
                if (actor->params & 0x80) {
                    return;
                }
                // *should = rando_has_item(0x0E0000 | actor->id);
                if (!rando_has_item(0x0E0000 | actor->id)) {
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
        case ACTOR_OBJ_KEPN_KOYA: //Gorman Racetrack Buildings
        case ACTOR_BOSS_01:
        // case ACTOR_EN_PAMETFROG:
        // case ACTOR_EN_KAME:
            // recomp_printf("actor soul id: 0x%06X\n", 0x0C0000 | actor->id);
            *should = false;
            break;
    }
    // recomp_printf("actor soul id: 0x%06X\n", 0x0C0000 | actor->id);
}