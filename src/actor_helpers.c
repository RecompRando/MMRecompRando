#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "actor_helpers.h"
#include "apcommon.h"

ActorExtensionId actorLocationExtension;
ActorExtensionId actorDroppedExtension;

void registerActorExtensions() {
    actorLocationExtension = z64recomp_extend_actor_all(sizeof(u32));
    actorDroppedExtension = z64recomp_extend_actor_all(sizeof(u32));
    
    mureIndexExtension = z64recomp_extend_actor(ACTOR_OBJ_MURE, sizeof(u32));
    mure2IndexExtension = z64recomp_extend_actor(ACTOR_OBJ_MURE2, sizeof(u32));
    wonderHitTimerExtension = z64recomp_extend_actor(ACTOR_EN_HIT_TAG, sizeof(u32));
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
            actor->id == ACTOR_OBJ_LUPYGAMELIFT ||  // deku playground elevator
            actor->id == ACTOR_EN_DNO) {            // deku butler
            // PLAYER_STATE1_20000000 is used when grabbing items
            params->canFreezeCategory |= PLAYER_STATE1_20000000 & player->stateFlags1;
        }
    }
}

// @rando insert a generic location into the actor's extension
void Actor_InitHalfDaysBit(ActorContext* actorCtx);
void Actor_KillAllOnHalfDayChange(PlayState* play, ActorContext* actorCtx);
Actor* Actor_SpawnEntry(ActorContext* actorCtx, ActorEntry* actorEntry, PlayState* play);

#define LOCATION_ACTOR (AP_PREFIX_ENEMY_DROP + (actor->id << 16) + (play->sceneId << 8) + (play->roomCtx.curRoom.num << 4) + i) /* uses addition due to overlaps when or'ing */

RECOMP_PATCH void Actor_SpawnSetupActors(PlayState* play, ActorContext* actorCtx) {
    if (play->numSetupActors > 0) {
        ActorEntry* actorEntry = play->setupActorList;
        s32 prevHalfDaysBitValue = actorCtx->halfDaysBit;
        s32 shiftedHalfDaysBit;
        s32 actorEntryHalfDayBit;
        s32 i;

        Actor* actor;
        u32* actorLocation;
        bool* actorDropped;

        Actor_InitHalfDaysBit(actorCtx);
        Actor_KillAllOnHalfDayChange(play, &play->actorCtx);

        // Shift to previous halfDay bit, but ignoring DAY0_NIGHT.
        // In other words, if the current halfDay is DAY1_DAY then this logic is ignored and this variable is zero
        shiftedHalfDaysBit = (actorCtx->halfDaysBit << 1) & (HALFDAYBIT_ALL & ~HALFDAYBIT_DAY0_NIGHT);

        for (i = 0; i < play->numSetupActors; i++) {
            actorEntryHalfDayBit = ((actorEntry->rot.x & 7) << 7) | (actorEntry->rot.z & 0x7F);
            if (actorEntryHalfDayBit == 0) {
                actorEntryHalfDayBit = HALFDAYBIT_ALL;
            }

            if (!(actorEntryHalfDayBit & prevHalfDaysBitValue) && (actorEntryHalfDayBit & actorCtx->halfDaysBit) &&
                (!CHECK_EVENTINF(EVENTINF_17) || !(actorEntryHalfDayBit & shiftedHalfDaysBit) ||
                 !(actorEntry->id & 0x800))) {
                actor = Actor_SpawnEntry(&play->actorCtx, actorEntry, play);

                // @rando assign a generic location to every valid actor
                if (actor != NULL) {
                    actorLocation = z64recomp_get_extended_actor_data(actor, actorLocationExtension);
                    // ignore actors that already had a location assigned to them
                    if (!(*actorLocation)) {
                        *actorLocation = LOCATION_ACTOR;
                        actorDropped = z64recomp_get_extended_actor_data(actor, actorDroppedExtension);
                        *actorDropped = false;
                    }
                }
            }
            actorEntry++;
        }

        // Prevents re-spawning the setup actors
        play->numSetupActors = -play->numSetupActors;
    }
}

RECOMP_CALLBACK("*", recomp_should_actor_init)
void Rando_ShouldActorInit(PlayState* play, Actor* actor, bool* should) {
    if(actor == NULL) return;
    if(gSaveContext.gameMode != GAMEMODE_NORMAL) return;

    // recomp_printf("actor id: 0x%02X 0x%06X %d\n", actor->id, AP_ITEM_PREFIX_SOUL_BOSS | actor->id, rando_has_item(AP_ITEM_PREFIX_SOUL_BOSS | actor->id));
    switch (actor->id) {
        case ACTOR_BOSS_01: // odolwa
        case ACTOR_BOSS_HAKUGIN: // goht
        case ACTOR_BOSS_03: // gyorg
        case ACTOR_BOSS_02: // twinmold
        case ACTOR_BOSS_07: // majora
            if (rando_get_slotdata_u32("boss_souls")) {
                if (actor->id == ACTOR_BOSS_07 &&
                    (rando_get_slotdata_u32("boss_souls") != 2 || rando_get_slotdata_u32("completion_goal"))) return;
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_BOSS | actor->id);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_BOSS | actor->id)) {
                    *should = false;
                    actor->destroy = NULL;
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
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_MISC | actor->id);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_MISC | actor->id)) {
                    *should = false;
                    actor->destroy = NULL;
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
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | actor->id);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | actor->id)) {
                    *should = false;
                    actor->destroy = NULL;
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
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | actor->id);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | actor->id)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_DT: // Council Soul
        case ACTOR_EN_MUTO:
        case ACTOR_EN_BAISEN:
        case ACTOR_EN_HEISHI: 
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_DT);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_DT)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_SSH: // Spider Man
        case ACTOR_EN_STH:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_SSH);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_SSH)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_SYATEKI_MAN: // Archery Guy
        case ACTOR_EN_SYATEKI_OKUTA:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_SYATEKI_MAN);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_SYATEKI_MAN)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_AKINDONUTS: // Business Scrub Soul
        case ACTOR_EN_SELLNUTS:
        case ACTOR_EN_SCOPENUTS:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_AKINDONUTS);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_AKINDONUTS)) {
                    *should = false;
                    actor->destroy = NULL;
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
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_BOMJIMA);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_BOMJIMA)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_TOTO: //Toto and Gorman (To prevent Gorman Softlock)
        case ACTOR_EN_GM:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_TOTO);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_TOTO)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
             break;
        case ACTOR_EN_AOB_01: //Doggy Race Lady
        case ACTOR_EN_DG: //Dogs
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_AOB_01);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_AOB_01)) {
                    *should = false;
                    actor->destroy = NULL;
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
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_MA_YTS);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_MA_YTS)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_KGY: //Smithy
        case ACTOR_EN_KBT:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_KGY);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_KGY)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_JGAME_TSN: //Fisherman
        case ACTOR_EN_TSN:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_TSN);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_TSN)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_SKB:
        case ACTOR_EN_RAIL_SKB: 
        case ACTOR_EN_HINT_SKB: // Graveyard and OSH Stalchildren
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_SKB);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_SKB)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_RAILGIBUD: //Gibdo Soul
        case ACTOR_EN_TALK_GIBUD:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_TALK_GIBUD);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_EN_TALK_GIBUD)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_ELFGRP: // Great Fairy Soul
        case ACTOR_BG_DY_YOSEIZO:
            if (rando_get_slotdata_u32("npc_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_BG_DY_YOSEIZO);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_NPC | ACTOR_BG_DY_YOSEIZO)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
    }
    switch (actor->id) {
        case ACTOR_EN_GAKUFU:
        case ACTOR_OBJ_ETCETERA: // Deku Flowers
        case ACTOR_DOOR_ANA: // Grottos
        case ACTOR_EN_ISHI: // Rocks
        case ACTOR_EN_KANBAN: // Signs
            if (rando_get_slotdata_u32("absurd_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | actor->id);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | actor->id)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_EN_WOOD02: //Trees/Shrubs
        case ACTOR_EN_SNOWWD: //
        case ACTOR_OBJ_TREE:
        case ACTOR_OBJ_YASI:
            if (rando_get_slotdata_u32("absurd_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | ACTOR_EN_WOOD2);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | ACTOR_EN_WOOD02)) {
                    *should = false;
                    actor->destroy = NULL;
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
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | ACTOR_EN_WOOD2);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | ACTOR_OBJ_GRASS)) {
                    *should = false;
                    actor->destroy = NULL;
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
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | actor->id);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | actor->id)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_OBJ_KEPN_KOYA: //Gorman Stables
            if (rando_get_slotdata_u32("absurd_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | actor->id);
                if (rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | actor->id)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
        case ACTOR_OBJ_TSUBO: //Pots
        case ACTOR_EN_TUBO_TRAP:
        case ACTOR_OBJ_FLOWERPOT:
            if (rando_get_slotdata_u32("absurd_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | ACTOR_OBJ_TSUBO);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ABSURD | ACTOR_OBJ_TSUBO)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
    }
    switch (actor->id) {
        case ACTOR_EN_PST:    // Postboxes
            if (rando_get_slotdata_u32("utility_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_UTILITY | actor->id);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_UTILITY | actor->id)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
    }
    switch (actor->id) {
        case ACTOR_EN_PAMETFROG: // Gekko
        case ACTOR_EN_BIGPAMET:
            if (rando_get_slotdata_u32("enemy_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_ENEMY| ACTOR_EN_PAMETFROG);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ENEMY | ACTOR_EN_PAMETFROG)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
    }
    switch (actor->id) {
        case ACTOR_EN_WF:          // Wolfos & White Wolfos
        case ACTOR_EN_CROW:        // Guay
        case ACTOR_EN_SLIME:       // ChuChus
        case ACTOR_EN_THIEFBIRD:   // Takkuri
        case ACTOR_EN_PR:          // Giant Bonefish
        case ACTOR_EN_GRASSHOPPER: // Dragonflies
        case ACTOR_EN_MKK:         // Black and White Boes
        case ACTOR_EN_DINOFOS:     // Dinolfos
        case ACTOR_EN_KAME:        // Snappers
        case ACTOR_EN_ST:          // Hanging Skulltulas
        case ACTOR_EN_RAF:         // Carnivorous Lilypads
        case ACTOR_EN_FZ:          // Freezard
        case ACTOR_EN_WIZ:         // Wizrobe 
        case ACTOR_EN_RAT:         // Real Bombchu
        case ACTOR_EN_PEEHAT:      // Peehat
        case ACTOR_EN_DODONGO:     // Dodongos
        case ACTOR_EN_NEO_REEBA:   // Leevers
        case ACTOR_EN_SNOWMAN:     // Eenos
        case ACTOR_EN_TITE:        // Tektite
        case ACTOR_EN_KAIZOKU:     // Coloured Pirate Minibosses
        case ACTOR_EN_BIGSLIME:    // GBT Jelly Gekko Miniboss
        case ACTOR_BOSS_04:        // Wart
        case ACTOR_EN_OKUTA:       // Octoroks
        case ACTOR_EN_IK:          // Iron Knuckle
        case ACTOR_EN_BAT:         // Bad Bats
        case ACTOR_EN_FIREFLY:     // Keese
        case ACTOR_EN_JSO2:        // Garo Master
        case ACTOR_EN_JSO:         // Garo 
        case ACTOR_EN_KNIGHT:      // Igos
        case ACTOR_EN_DEATH:       // Gomess
        case ACTOR_EN_EGOL:        // Eyegore
        case ACTOR_EN_AM:          // Armos
        case ACTOR_EN_FAMOS:       // Death Armos Statues
        case ACTOR_EN_RD:          // Redead
            if (rando_get_slotdata_u32("enemy_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_ENEMY| actor->id);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ENEMY | actor->id)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
    }
    switch (actor->id) {
        case ACTOR_EN_DEKUBABA: // Dekubaba
        case ACTOR_EN_KAREBABA: // Wilted/Minibaba
        case ACTOR_BOSS_05:  // Bio Baba
            if (rando_get_slotdata_u32("enemy_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_ENEMY| ACTOR_EN_DEKUBABA);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ENEMY | ACTOR_EN_DEKUBABA)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
    }
    switch (actor->id) {
        case ACTOR_EN_GE2:         // Purple Pirate Guards
        case ACTOR_EN_GE1:         // White Pirate Guards
            if (rando_get_slotdata_u32("enemy_souls")) {
                // *should = rando_has_item(AP_ITEM_PREFIX_SOUL_ENEMY| ACTOR_EN_GE2);
                if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ENEMY | ACTOR_EN_GE2)) {
                    *should = false;
                    actor->destroy = NULL;
                }
                return;
            }
            break;
    }

    // prevent chests from spawning when you don't have their accompanying enemy soul
    if (actor->id == ACTOR_EN_BOX && rando_get_slotdata_u32("enemy_souls")) {
        switch (play->sceneId) {
            // grottos
            case SCENE_KAKUSIANA:
                recomp_printf("box room num %d\n", play->roomCtx.curRoom.num);
                switch (play->roomCtx.curRoom.num) {
                    case 7: // dodongo grotto
                        if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ENEMY | ACTOR_EN_DODONGO)) {
                            *should = false;
                            actor->destroy = NULL;
                        }
                        return;
                    case 13: // peahat grotto
                        if (!rando_has_item(AP_ITEM_PREFIX_SOUL_ENEMY | ACTOR_EN_PEEHAT)) {
                            *should = false;
                            actor->destroy = NULL;
                        }
                        return;
                }
            default:
                break;
        }
    }
    
    // switch (actor->id) {
    //     case ACTOR_EN_KANBAN:
    //     // case ACTOR_EN_KAME:
    //         // recomp_printf("actor soul id: 0x%06X\n", AP_ITEM_PREFIX_SOUL_NPC | actor->id);
    //         *should = false;
    //                 actor->destroy = NULL;
    //         break;
    // }
    // // recomp_printf("actor soul id: 0x%06X\n", AP_ITEM_PREFIX_SOUL_NPC | actor->id);
}

// this is likely temporary as we'd want to PR most if not all of these (unless its a rando only issue)
RECOMP_IMPORT("mm_recomp_better_double_sot", void dsot_set_whitelist_true(void));

// anything that should actually respawn on time changes should go in here
RECOMP_CALLBACK("mm_recomp_better_double_sot", dsot_on_whitelist_check)
void dsot_set_rando_actor_whitelists(s16 actor_id) {
    // switch (actor_id) {
    //     case ACTOR:
    //         dsot_set_whitelist_true();
    // }
}