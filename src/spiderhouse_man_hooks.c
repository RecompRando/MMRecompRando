#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define STH_LIMB_MAX 0x10
#include "overlays/actors/ovl_En_Sth/z_en_sth.h"
#include "overlays/actors/ovl_En_Ssh/z_en_ssh.h"

typedef enum EnSthAnimation {
    /* -1 */ STH_ANIM_NONE = -1,
    /*  0 */ STH_ANIM_SIGNALLING,   // default, waving arms at you from telescope, OOT: cured happy animation
    /*  1 */ STH_ANIM_BENDING_DOWN, // default anim of cured spider house, but never seen before wait overrides it
    /*  2 */ STH_ANIM_TALK,
    /*  3 */ STH_ANIM_WAIT,
    /*  4 */ STH_ANIM_LOOK_UP,     // South Clock Town, looking at moon
    /*  5 */ STH_ANIM_LOOK_AROUND, // checking out Oceanside Spider House
    /*  6 */ STH_ANIM_PLEAD,       // wants to buy Oceanside Spider House
    /*  7 */ STH_ANIM_PANIC,       // after buying Oceanside Spider House, can be found at bottom of slide,
    /*  8 */ STH_ANIM_MAX          // set in init, not an actual index to the array
} EnSthAnimation;

u32 savedTokenCount;

// sets current skull token count to 30 if it exceeds set requirement
void Inventory_FakeSkullTokenCount(s16 sceneIndex) {
    s16 currentTokens;
    savedTokenCount = gSaveContext.save.saveInfo.skullTokenCount;
    
    if (sceneIndex == SCENE_KINSTA1) { // Swamp Spider House
        currentTokens = (gSaveContext.save.saveInfo.skullTokenCount & 0xFFFF0000) >> 0x10;
        if (currentTokens >= (s16) rando_get_slotdata_u32("required_skull_tokens")) {
            gSaveContext.save.saveInfo.skullTokenCount |= 30 << 0x10;
        }
    } else { // Ocean Spider House
        currentTokens = gSaveContext.save.saveInfo.skullTokenCount & 0xFFFF;
        if (currentTokens >= (s16) rando_get_slotdata_u32("required_skull_tokens")) {
            gSaveContext.save.saveInfo.skullTokenCount |= 30;
        }
    }
}

// cursed swamp spider house guy
RECOMP_HOOK("EnSsh_Init")
void OnEnSsh_Init(Actor* thisx, PlayState* play) {
    // TODO: replace swamp guy's mask with its randomzied reward
    // fake skull token count for lower requirements
    Inventory_FakeSkullTokenCount(play->sceneId);
}

RECOMP_HOOK_RETURN("EnSsh_Init")
void AfterEnSsh_Init() {
    gSaveContext.save.saveInfo.skullTokenCount = savedTokenCount; // revert skull token count
}

// standard/cured spider house guy
RECOMP_HOOK("EnSth_Init")
void OnEnSth_Init(Actor* thisx, PlayState* play) {
    // TODO: replace swamp guy's mask with its randomzied reward (in a return hook?)
    
    // unset flag whenever the location isn't checked (because its set by mikau's grave???)
    if (!rando_location_is_checked(GI_WALLET_GIANT)) {
        CLEAR_WEEKEVENTREG(WEEKEVENTREG_OCEANSIDE_SPIDER_HOUSE_BUYER_MOVED_IN);
    }

    // fake skull token count for lower requirements
    Inventory_FakeSkullTokenCount(play->sceneId);

    // If ShuffleSpiderhouseReward is disabled, set the flag that prevent giving the SSH reward
    if ((s16) rando_get_slotdata_u32("shuffle_spiderhouse_reward") == 0) {
        SET_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_MASK_OF_TRUTH);
    }
}

RECOMP_HOOK_RETURN("EnSth_Init")
void AfterEnSth_Init() {
    gSaveContext.save.saveInfo.skullTokenCount = savedTokenCount; // revert skull token count
}

void EnSth_ChangeAnim(EnSth* this, s16 animIndex);

RECOMP_PATCH void EnSth_GetInitialSwampSpiderHouseText(EnSth* this, PlayState* play) {
    u16 nextTextId;

    if (CHECK_WEEKEVENTREG(WEEKEVENTREG_34_10)) {
        nextTextId = 0x903; // (does not exist)
        EnSth_ChangeAnim(this, STH_ANIM_TALK);
    } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_34_20)) {
        nextTextId = 0x90F; // (does not exist)
        EnSth_ChangeAnim(this, STH_ANIM_TALK);
    } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_RECEIVED_MASK_OF_TRUTH)) {
        if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_TALKED_SWAMP_SPIDER_HOUSE_MAN)) {
            nextTextId = 0x91B; // As soon as I calm down, getting rid of it
        } else {
            nextTextId = 0x918; // I've had enough of this, going home
        }
        EnSth_ChangeAnim(this, STH_ANIM_TALK);
    } else if (Inventory_GetSkullTokenCount(SCENE_KINSTA1) >= (s16) rando_get_slotdata_u32("required_skull_tokens")) {
        if (rando_location_is_checked(GI_MASK_TRUTH)) {
            this->sthFlags |= STH_FLAG_SWAMP_SPIDER_HOUSE_SAVED;
            nextTextId = 0x919; // I've been saved!
        } else {
            nextTextId = 0x916; // I've been saved! (Duplicate)
        }
    } else if (CHECK_WEEKEVENTREG(WEEKEVENTREG_34_02)) {
        nextTextId = 0x8FF; // (does not exist)
    } else {
        nextTextId = 0x8FC; // (does not exist)
        SET_WEEKEVENTREG(WEEKEVENTREG_34_02);
    }

    Message_StartTextbox(play, nextTextId, &this->actor);
}

void EnSth_PostOceanspiderhouseReward(EnSth* this, PlayState* play);

RECOMP_PATCH void EnSth_GiveOceansideSpiderHouseReward(EnSth* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);

    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnSth_PostOceanspiderhouseReward;
        this->actor.flags |= ACTOR_FLAG_10000;
        Actor_OfferTalkExchange(&this->actor, play, 1000.0f, 1000.0f, PLAYER_IA_MINUS1);
        if (CURRENT_DAY == 3) {
            EnSth_ChangeAnim(this, STH_ANIM_PLEAD);
        } else {
            EnSth_ChangeAnim(this, STH_ANIM_WAIT);
        }
    } else {
        // this could all be moved into a hook?
        // handle all 3 day checks here as well
        if (rando_location_is_checked(GI_WALLET_GIANT) || (s16) rando_get_slotdata_u32("shuffle_spiderhouse_reward") == 0) {
            STH_GI_ID(&this->actor) = GI_RUPEE_SILVER;
        } else {
            STH_GI_ID(&this->actor) = GI_WALLET_GIANT;
        }
        Actor_OfferGetItem(&this->actor, play, STH_GI_ID(&this->actor), 10000.0f, 500.0f);
    }
}

void EnSth_Update(Actor* thisx, PlayState* play);
void EnSth_Draw(Actor* thisx, PlayState* play);

// could honestly also work as a patch
RECOMP_HOOK("EnSth_UpdateOceansideSpiderHouseWaitForTokens")
void EnSth_UpdateOceansideSpiderHouseWaitForTokensRando(Actor* thisx, PlayState* play) {
    EnSth* this = ((EnSth*)thisx);

    if (Inventory_GetSkullTokenCount(play->sceneId) >= (s16) rando_get_slotdata_u32("required_skull_tokens")) {
        this->actor.update = EnSth_Update;
        this->actor.draw = EnSth_Draw;
        this->actor.flags |= ACTOR_FLAG_TARGETABLE;
    }
}

RECOMP_HOOK("EnSth_UpdateWaitForObject")
void OnEnSth_UpdateWaitForObject(Actor* thisx, PlayState* play) {
    // fake skull token count for lower requirements
    Inventory_FakeSkullTokenCount(play->sceneId);
}

RECOMP_HOOK_RETURN("EnSth_UpdateWaitForObject")
void AfterEnSth_UpdateWaitForObject() {
    gSaveContext.save.saveInfo.skullTokenCount = savedTokenCount; // revert skull token count
}