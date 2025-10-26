#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

#include "overlays/actors/ovl_Obj_Mure3/z_obj_mure3.h"
#include "overlays/actors/ovl_En_Scopecoin/z_en_scopecoin.h"
#include "overlays/actors/ovl_En_Gamelupy/z_en_gamelupy.h"

#define LOCATION_RUPEE (AP_PREFIX_FREESTANDING | (gPlay->sceneId << 8) | (gPlay->roomCtx.curRoom.num << 4) \
                        | randoGetLoadedActorNumInSameRoom(gPlay, actor))
#define LOCATION_MURE3_RUPEE(index) (AP_PREFIX_FREESTANDING | (play->sceneId << 8) | (0xF << 4) | index)
#define LOCATION_TERMINATREE_RUPEE(index) (AP_PREFIX_FREESTANDING | (play->sceneId << 8) | (0xE << 4) | index)
#define LOCATION_PLAYGROUND_RUPEE (AP_PREFIX_FREESTANDING | (play->sceneId << 8) | (CURRENT_DAY << 4) | randoGetLoadedActorNumInSameRoom(play, thisx))

void Item_RandoCollectibleDraw(Actor* thisx, PlayState* play);
void Item_RandoCollectibleGround(EnItem00* this, PlayState* play);

// freestanding items
// note: this *should* only run when an actor is spawned on scene/room load
RECOMP_HOOK_RETURN("Actor_SpawnEntry")
void freestanding_item_replacement() {
    Actor* actor = recomphook_get_return_ptr();
    if (actor == NULL) {
        return;
    }

    if (actor->id == ACTOR_EN_ITEM00) {
        if (actor->params == ITEM00_RUPEE_GREEN ||
            actor->params == ITEM00_RUPEE_BLUE ||
            actor->params == ITEM00_RUPEE_RED ||
            actor->params == ITEM00_RUPEE_PURPLE ||
            actor->params == ITEM00_RUPEE_HUGE ||
            actor->params == ITEM00_RECOVERY_HEART
            ) {
            if (rando_get_slotdata_u32("rupeesanity") && !rando_location_is_checked(LOCATION_RUPEE)) {
                u32* item00Location;
                item00Location = z64recomp_get_extended_actor_data(actor, item00LocationExtension);
                *item00Location = LOCATION_RUPEE;
                actor->params = ITEM00_APITEM;
                actor->draw = Item_RandoCollectibleDraw;
                ((EnItem00*)actor)->getItemId = rando_get_item_id(LOCATION_RUPEE);
                ((EnItem00*)actor)->actionFunc = Item_RandoCollectibleGround;
            }
        }
    }
}

// Group of Rupees (spawned by Mure3)
// note: only func_8098F220 (seven spawn) is used in the actual game?
ObjMure3* savedObjMure3;

RECOMP_HOOK("func_8098F040")
void OnMure3_FiveRupeeSpawn(ObjMure3* this, PlayState* play) {
    savedObjMure3 = this;
}

RECOMP_HOOK_RETURN("func_8098F040")
void AfterMure3_FiveRupeeSpawn() {
    ObjMure3* this = savedObjMure3;
    PlayState* play = gPlay;
    for (s32 i = 0; i < 5; i++) {
        if (this->unk148[i] != NULL) {
            if (rando_get_slotdata_u32("rupeesanity") && !rando_location_is_checked(LOCATION_MURE3_RUPEE(i))) {
                Actor* actor = &this->unk148[i]->actor;
                u32* item00Location;
                item00Location = z64recomp_get_extended_actor_data(actor, item00LocationExtension);
                *item00Location = LOCATION_MURE3_RUPEE(i);
                actor->params = ITEM00_APITEM;
                actor->draw = Item_RandoCollectibleDraw;
                ((EnItem00*)actor)->getItemId = rando_get_item_id(LOCATION_MURE3_RUPEE(i));
                ((EnItem00*)actor)->actionFunc = Item_RandoCollectibleGround;
            }
        }
    }
}

RECOMP_HOOK("func_8098F110")
void OnMure3_FiveRupeeSpawn2(ObjMure3* this, PlayState* play) {
    savedObjMure3 = this;
}

RECOMP_HOOK_RETURN("func_8098F110")
void AfterMure3_FiveRupeeSpawn2() {
    ObjMure3* this = savedObjMure3;
    PlayState* play = gPlay;
    for (s32 i = 0; i < 5; i++) {
        if (this->unk148[i] != NULL) {
            if (rando_get_slotdata_u32("rupeesanity") && !rando_location_is_checked(LOCATION_MURE3_RUPEE(i))) {
                Actor* actor = &this->unk148[i]->actor;
                u32* item00Location;
                item00Location = z64recomp_get_extended_actor_data(actor, item00LocationExtension);
                *item00Location = LOCATION_MURE3_RUPEE(i);
                actor->params = ITEM00_APITEM;
                actor->draw = Item_RandoCollectibleDraw;
                ((EnItem00*)actor)->getItemId = rando_get_item_id(LOCATION_MURE3_RUPEE(i));
                ((EnItem00*)actor)->actionFunc = Item_RandoCollectibleGround;
            }
        }
    }
}

RECOMP_HOOK("func_8098F220")
void OnMure3_SevenRupeeSpawn(ObjMure3* this, PlayState* play) {
    savedObjMure3 = this;
}

RECOMP_HOOK_RETURN("func_8098F220")
void AfterMure3_SevenRupeeSpawn() {
    ObjMure3* this = savedObjMure3;
    PlayState* play = gPlay;
    for (s32 i = 0; i < 7; i++) {
        if (this->unk148[i] != NULL) {
            if (rando_get_slotdata_u32("rupeesanity") && !rando_location_is_checked(LOCATION_MURE3_RUPEE(i))) {
                Actor* actor = &this->unk148[i]->actor;
                u32* item00Location;
                item00Location = z64recomp_get_extended_actor_data(actor, item00LocationExtension);
                *item00Location = LOCATION_MURE3_RUPEE(i);
                actor->params = ITEM00_APITEM;
                actor->draw = Item_RandoCollectibleDraw;
                ((EnItem00*)actor)->getItemId = rando_get_item_id(LOCATION_MURE3_RUPEE(i));
                ((EnItem00*)actor)->actionFunc = Item_RandoCollectibleGround;
            }
        }
    }
}

// termina field tree where the parttimer is climbing
void EnScopecoin_RandoDraw(Actor* thisx, PlayState* play) {
    s32 actorIndex = randoGetLoadedActorNumInSameRoom(play, thisx);
    u32 getItemId = rando_get_item_id(LOCATION_TERMINATREE_RUPEE(actorIndex));
    u16 objectId = getObjectId(getItemId);

    if (actorIndex == 0) { // hide extra rupee on pillar
        return;
    }

    Matrix_Scale(40.0f, 40.0f, 40.0f, MTXMODE_APPLY);
    if (isAP(getItemId)) {
        GetItem_Draw(play, getGid(getItemId));
    } else if (ObjLoad(play, 0x06, objectId)) {
        GetItem_Draw(play, getGid(getItemId));
        ObjUnload(play, 0x06, objectId);
    // } else {
    //     GetItem_Draw(play, GID_APLOGO_USEFUL);
    }
}

Actor* sEnScopecoinActor;

RECOMP_HOOK("EnScopecoin_Init")
void OnEnScopecoin_Init(Actor* thisx, PlayState* play) {
    sEnScopecoinActor = thisx;
}

RECOMP_HOOK_RETURN("EnScopecoin_Init")
void AfterEnScopecoin_Init() {
    PlayState* play = gPlay;
    s32 actorIndex = randoGetLoadedActorNumInSameRoom(play, sEnScopecoinActor);
    if (rando_get_slotdata_u32("rupeesanity") && !rando_location_is_checked(LOCATION_TERMINATREE_RUPEE(actorIndex))) {
        sEnScopecoinActor->draw = EnScopecoin_RandoDraw;
    }
}

RECOMP_PATCH void EnScopecoin_CheckCollectible(EnScopecoin* this, PlayState* play) {
    s32 actorIndex = randoGetLoadedActorNumInSameRoom(play, &this->actor);

    this->actor.shape.rot.y += 0x1F4; // spin
    if (Flags_GetCollectible(play, OBJMUPICT_GET_RUPEE_FLAG(&this->actor))) {
        if (rando_get_slotdata_u32("rupeesanity") && !rando_location_is_checked(LOCATION_TERMINATREE_RUPEE(actorIndex))) {
            Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, LOCATION_TERMINATREE_RUPEE(actorIndex));
        } else {
            Item_DropCollectible(play, &this->actor.world.pos, ITEM00_RUPEE_RED);
        }
        Actor_Kill(&this->actor);
    }
}

// deku playground
ActorExtensionId gamelupyLocationExtension;
u32* gamelupyLocation;
Actor* EnGamelupyActor;

void EnGamelupy_RandoDraw(Actor* thisx, PlayState* play) {
    gamelupyLocation = z64recomp_get_extended_actor_data(thisx, gamelupyLocationExtension);
    u32 getItemId = rando_get_item_id(*gamelupyLocation);
    u16 objectId = getObjectId(getItemId);

    Matrix_Scale(20.0f, 20.0f, 20.0f, MTXMODE_APPLY);
    if (isAP(getItemId)) {
        GetItem_Draw(play, getGid(getItemId));
    } else if (ObjLoad(play, 0x06, objectId)) {
        GetItem_Draw(play, getGid(getItemId));
        ObjUnload(play, 0x06, objectId);
    // } else {
    //     GetItem_Draw(play, GID_APLOGO_USEFUL);
    }
}

RECOMP_HOOK("EnGamelupy_Init")
void OnEnGamelupy_Init(Actor* thisx, PlayState* play) {
    gamelupyLocation = z64recomp_get_extended_actor_data(thisx, gamelupyLocationExtension);
    *gamelupyLocation = LOCATION_PLAYGROUND_RUPEE;
    EnGamelupyActor = thisx;
}

RECOMP_HOOK_RETURN("EnGamelupy_Init")
void AfterEnGamelupy_Init() {
    PlayState* play = gPlay;
    gamelupyLocation = z64recomp_get_extended_actor_data(EnGamelupyActor, gamelupyLocationExtension);
    if (rando_get_slotdata_u32("rupeesanity") && !rando_location_is_checked(*gamelupyLocation)) {
        EnGamelupyActor->draw = EnGamelupy_RandoDraw;
    }
}

void EnGamelupy_SetupCollected(EnGamelupy* this);
void EnGamelupy_Collected(EnGamelupy* this, PlayState* play);

void EnItem00_RandoTextAndFreeze(EnItem00* this, PlayState* play);

RECOMP_PATCH void EnGamelupy_Idle(EnGamelupy* this, PlayState* play) {
    if (this->collider.base.ocFlags1 & OC1_HIT) {
        *this->minigameScore += ENGAMELUPY_POINTS;

        gamelupyLocation = z64recomp_get_extended_actor_data(&this->actor, gamelupyLocationExtension);
        if (rando_get_slotdata_u32("rupeesanity") && !rando_location_is_checked(*gamelupyLocation)) {
            this->collectedTimer = 0;
            this->actor.gravity = 0.0f;
            Actor* item = Item_RandoDropCollectible(play, &this->actor.world.pos, ITEM00_APITEM, *gamelupyLocation);
            ((EnItem00*)item)->actionFunc = EnItem00_RandoTextAndFreeze;
            // this->actionFunc = EnGamelupy_Collected;
            Actor_Kill(&this->actor);
            return;
        } else {
            if (this->type == ENGAMELUPY_TYPE_BLUE) {
                Rupees_ChangeBy(5);
            } else {
                Rupees_ChangeBy(1);
            }
            EnGamelupy_SetupCollected(this);
        }
    }
    this->actor.shape.rot.y += 0x1F4;
}