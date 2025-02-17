#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define LOCATION_COW (0xBEEF00 | EnCow_GetCowID(&this->actor, play))

#include "z64voice.h"

struct EnCow;

#define COW_LIMB_MAX 0x06

typedef void (*EnCowActionFunc)(struct EnCow*, PlayState*);

typedef struct EnCow {
    /* 0x0000 */ Actor actor;
    /* 0x0144 */ ColliderCylinder colliders[2];
    /* 0x01DC */ SkelAnime skelAnime;
    /* 0x0220 */ Vec3s jointTable[COW_LIMB_MAX];
    /* 0x0244 */ Vec3s morphTable[COW_LIMB_MAX];
    /* 0x0268 */ Vec3s headTilt;
    /* 0x026E */ u16 flags;
    /* 0x0270 */ u16 animTimer;
    /* 0x0272 */ u16 animCycle;
    /* 0x0274 */ EnCowActionFunc actionFunc;
} EnCow; // size = 0x278

#define EN_COW_TYPE(thisx) ((thisx)->params & 0xF)

#define EN_COW_FLAG_PLAYER_HAS_APPROACHED (1 << 1)
#define EN_COW_FLAG_WONT_GIVE_MILK (1 << 2)

// OOTMM code
static Actor* EnCow_GetNearestCow(PlayState* play)
{
    Actor* cow;
    Actor* tmp;
    u32 count;
    float cowDist;

    cow = NULL;
    cowDist = 0.f;
    for (int i = 0; i < 12; ++i)
    {
        count = play->actorCtx.actorLists[i].length;
        tmp = play->actorCtx.actorLists[i].first;
        while (tmp && count)
        {
            if (tmp->id == ACTOR_EN_COW && tmp->params != 1 && (!cow || cowDist > tmp->xzDistToPlayer))
            {
                cow = tmp;
                cowDist = tmp->xzDistToPlayer;
            }
            tmp = tmp->next;
            count--;
        }
    }
    return cow;
}

static int EnCow_GetCowID(Actor* cow, PlayState* play)
{
    s32 sceneId;

    sceneId = play->sceneId;
    if (sceneId == SCENE_KAKUSIANA)
        sceneId = Entrance_GetSceneIdAbsolute(((void)0, gSaveContext.respawn[RESPAWN_MODE_UNK_3].entrance));
    switch (sceneId)
    {
    case SCENE_00KEIKOKU: // Termina Field
        return cow->world.pos.z > 930.f ? 0x13 : 0x14;
    case SCENE_30GYOSON: // Great Bay Coast
        return cow->world.pos.z > 930.f ? 0x15 : 0x16;
    case SCENE_OMOYA: // Ranch House
        return cow->world.pos.x < -100.f ? 0x10 : cow->world.pos.z < -100.f ? 0x12 : 0x11;
    case SCENE_REDEAD: // Beneath the Well
        return 0x17;
    }
    return -1;
}

extern s32 gHorsePlayedEponasSong;

void EnCow_Talk(EnCow* this, PlayState* play);
void EnCow_UpdateAnimation(EnCow* this, PlayState* play);

RECOMP_PATCH void EnCow_Idle(EnCow* this, PlayState* play) {
    if ((play->msgCtx.ocarinaMode == OCARINA_MODE_NONE) || (play->msgCtx.ocarinaMode == OCARINA_MODE_END)) {
        if (gHorsePlayedEponasSong) {
            Actor* nearestCow = EnCow_GetNearestCow(play);
            if (&this->actor != nearestCow) {
                return;
            }

            if (this->flags & EN_COW_FLAG_WONT_GIVE_MILK) {
                this->flags &= ~EN_COW_FLAG_WONT_GIVE_MILK;
                gHorsePlayedEponasSong = false;
            } else if ((this->actor.xzDistToPlayer < 150.0f) &&
                       ABS_ALT(BINANG_SUB(this->actor.yawTowardsPlayer, this->actor.shape.rot.y)) < 25000) {
                gHorsePlayedEponasSong = false;
                this->actionFunc = EnCow_Talk;
                this->actor.flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
                Actor_OfferTalk(&this->actor, play, 170.0f);
                this->actor.textId = 0x32C8; // Text to give milk after playing Epona's Song.

                EnCow_UpdateAnimation(this, play);
                return;
            } else {
                this->flags |= EN_COW_FLAG_WONT_GIVE_MILK;
            }
        } else {
            this->flags &= ~EN_COW_FLAG_WONT_GIVE_MILK;
        }
    }

    // "ミルク" activation
    if (this->actor.xzDistToPlayer < 150.0f &&
        ABS_ALT((s16)(this->actor.yawTowardsPlayer - this->actor.shape.rot.y)) < 25000) {
        if (AudioVoice_GetWord() == 4) {
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_TALKING_TO_COW_WITH_VOICE)) {
                SET_WEEKEVENTREG(WEEKEVENTREG_TALKING_TO_COW_WITH_VOICE);
                if (Inventory_HasEmptyBottle()) {
                    this->actor.textId = 0x32C9; // Text to give milk.
                } else {
                    this->actor.textId = 0x32CA; // Text if you don't have an empty bottle.
                }
                this->actor.flags |= ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
                Actor_OfferTalk(&this->actor, play, 170.0f);
                this->actionFunc = EnCow_Talk;
            }
        } else {
            CLEAR_WEEKEVENTREG(WEEKEVENTREG_TALKING_TO_COW_WITH_VOICE);
        }
    }

    EnCow_UpdateAnimation(this, play);
}

void EnCow_GiveMilkEnd(EnCow* this, PlayState* play);

RECOMP_PATCH void EnCow_GiveMilkWait(EnCow* this, PlayState* play) {
    if (Actor_HasParent(&this->actor, play)) {
        this->actor.parent = NULL;
        this->actionFunc = EnCow_GiveMilkEnd;
    } else {
        if (rando_location_is_checked(LOCATION_COW) && rando_cows_enabled()) {
            Actor_OfferGetItem(&this->actor, play, GI_MILK, 10000.0f, 100.0f);
        } else {
            Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_COW), LOCATION_COW, 300.0f, 300.0f, true, true);
        }
    }
}

RECOMP_PATCH void EnCow_GiveMilk(EnCow* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        this->actor.flags &= ~ACTOR_FLAG_TALK_OFFER_AUTO_ACCEPTED;
        Message_CloseTextbox(play);
        this->actionFunc = EnCow_GiveMilkWait;
        if (rando_location_is_checked(LOCATION_COW) && rando_cows_enabled()) {
            Actor_OfferGetItem(&this->actor, play, GI_MILK, 10000.0f, 100.0f);
        } else {
            Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_COW), LOCATION_COW, 300.0f, 300.0f, true, true);
        }
    }
}

void EnCow_TalkEnd(EnCow* this, PlayState* play);

RECOMP_PATCH void EnCow_CheckForEmptyBottle(EnCow* this, PlayState* play) {
    if ((Message_GetState(&play->msgCtx) == TEXT_STATE_EVENT) && Message_ShouldAdvance(play)) {
        if (Inventory_HasEmptyBottle() || rando_cows_enabled()) {
            Message_ContinueTextbox(play, 0x32C9); // Text to give milk.
            this->actionFunc = EnCow_GiveMilk;
        } else {
            Message_ContinueTextbox(play, 0x32CA); // Text if you don't have an empty bottle.
            this->actionFunc = EnCow_TalkEnd;
        }
    }
}