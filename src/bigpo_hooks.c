#include "modding.h"
#include "global.h"

bool skipDigging = false;

#define BIG_POE_SET_SWITCH_FLAG(type) ((1 << 0x8) | type)

struct EnBigpo;

typedef void (*EnBigPoActionFunc)(struct EnBigpo*, PlayState*);

typedef struct EnBigpoFireEffect {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ LightNode* light;
    /* 0x10 */ LightInfo info; // size 0xE
} EnBigpoFireEffect; // size = 0x20

#define BIG_POE_LIMB_MAX 0xA
#define BIG_POE_BODYPART_MAX 0x9

typedef struct EnBigpo {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[BIG_POE_LIMB_MAX];
    /* 0x1C4 */ Vec3s morphTable[BIG_POE_LIMB_MAX];
    /* 0x200 */ EnBigPoActionFunc actionFunc;
    /* 0x204 */ u8 storePrevBgm;
    /* 0x206 */ s16 idleTimer; // frame counter
    /* 0x208 */ s16 unk208; // facing rotY?
    /* 0x20A */ s16 rotVelocity;
    /* 0x20C */ s16 unk20C; // is this counting the number of frames the player is ztargeting them?
    /* 0x20E */ s16 subCamId;
    /* 0x210 */ s16 switchFlag;
    /* 0x212 */ s16 hoverHeightCycleTimer; // sin wave up and down bobbing
    /* 0x214 */ f32 fireRadius; // distance from center during conjunction cutscene
    /* 0x218 */ f32 savedHeight; // actual height while flying moves as part of bobbing
    /* 0x21C */ f32 drawDmgEffAlpha;
    /* 0x220 */ f32 drawDmgEffScale;
    /* 0x224 */ Vec3f bodyPartsPos[BIG_POE_BODYPART_MAX];
    /* 0x290 */ Color_RGBA8 mainColor;
    /* 0x294 */ Color_RGBA8 lanternColor;
    /* 0x298 */ UNK_TYPE1 pad298[0x14];
    /* 0x2AC */ ColliderCylinder collider;
    /* 0x2F8 */ MtxF drawMtxF;
    // the three fires that merge to become big po
    //   also the fires dampe digs up under his house
    /* 0x338 */ EnBigpoFireEffect fires[3];
} EnBigpo; // size = 0x398

typedef enum EnBigpoType {
  /* 0 */ BIG_POE_TYPE_REGULAR,
  /* 1 */ BIG_POE_TYPE_SUMMONED,
  /* 2 */ BIG_POE_TYPE_POSSIBLE_FIRE,
  /* 3 */ BIG_POE_TYPE_CHOSEN_FIRE,
  /* 4 */ BIG_POE_TYPE_REVEALED_FIRE,
  /* 5 */ BIG_POE_TYPE_UNK5
} EnBigpoType;

void EnBigpo_UpdateFire(Actor *thisx, PlayState *play);
void EnBigpo_ChangeToFireCounting(EnBigpo *this);
void EnBigpo_Die(EnBigpo *this, PlayState *play);
void EnBigpo_DrawMainBigpo(Actor *thisx, PlayState *play);
void EnBigpo_SetupWarpIn(EnBigpo *this, PlayState *play);
void EnBigpo_SetupSpawnCutscene(EnBigpo* this);

// set every dig spot to have a fire
RECOMP_PATCH void EnBigpo_SelectRandomFireLocations(EnBigpo* this, PlayState* play) {
    Actor* enemyPtr;
    EnBigpo* randomFirePo;
    s32 fireIndex;
    s32 randomIndex;
    s32 fireCount = 0;

    // spawn missing fire actors
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BIGPO, 240, 8, 20, 0, 0, 0, BIG_POE_SET_SWITCH_FLAG(BIG_POE_TYPE_POSSIBLE_FIRE));
    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_BIGPO, -240, 8, 20, 0, 0, 0, BIG_POE_SET_SWITCH_FLAG(BIG_POE_TYPE_POSSIBLE_FIRE));

    // count the number of possible fires we can find (4 in vanilla)
    for (enemyPtr = GET_FIRST_ENEMY(play); enemyPtr != NULL; enemyPtr = enemyPtr->next) {
        if ((enemyPtr->id == ACTOR_EN_BIGPO) && (enemyPtr->params == BIG_POE_TYPE_POSSIBLE_FIRE)) {
            fireCount++;
        }
    }

    // massive speedup, could become an option
    if (skipDigging) {
        fireCount = 0;
        // need dampe to be under his bed somewhere here (currently completely removes him)
        Actor* npcPtr;
        for (npcPtr = ((Actor*)(play)->actorCtx.actorLists[ACTORCAT_NPC].first); npcPtr != NULL; npcPtr = npcPtr->next) {
            if (npcPtr->id == ACTOR_EN_TK) {
                Actor_Kill(npcPtr);
            }
        }
    }

    // if not enough fires exist, just starting fighting immediately
    if (fireCount < ARRAY_COUNT(this->fires)) {
        this->actor.draw = EnBigpo_DrawMainBigpo;
        Actor_SetScale(&this->actor, 0.014f);
        EnBigpo_SetupWarpIn(this, play);
        Math_Vec3f_Copy(&this->actor.world.pos, &this->actor.home.pos);
        this->actor.world.pos.y += 100.0f;
        return;
    }

    // for available possiblefires, pick three to be random fires
    for (fireIndex = 0; fireIndex < ARRAY_COUNT(this->fires); fireIndex++, fireCount--) {
        enemyPtr = GET_FIRST_ENEMY(play);
        // randomIndex = ((s32)Rand_ZeroFloat(fireCount)) % fireCount;

        while (enemyPtr != NULL) {
            if ((enemyPtr->id == ACTOR_EN_BIGPO) && (enemyPtr->params == BIG_POE_TYPE_POSSIBLE_FIRE)) {
                randomFirePo = (EnBigpo*)enemyPtr;
                randomFirePo->actor.params = BIG_POE_TYPE_CHOSEN_FIRE;
                randomFirePo->actor.parent = (Actor*)this;
                randomFirePo->actor.update = EnBigpo_UpdateFire;
                Actor_ChangeCategory(play, &play->actorCtx, &randomFirePo->actor, ACTORCAT_PROP);
                randomFirePo->unk20C = fireIndex;
                randomFirePo->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                // make invisible by size: 0
                Actor_SetScale(&randomFirePo->actor, 0);
                if (this->actor.child == NULL) {
                    this->actor.child = &randomFirePo->actor;
                } else {
                    randomFirePo->actor.child = this->actor.child;
                    this->actor.child = &randomFirePo->actor;
                }
                
                // if (randomIndex == 0) {
                //     randomFirePo = (EnBigpo*)enemyPtr;
                //     randomFirePo->actor.params = BIG_POE_TYPE_CHOSEN_FIRE;
                //     Math_Vec3f_Copy(&this->fires[fireIndex].pos, &randomFirePo->actor.world.pos);
                //     randomFirePo->actor.parent = (Actor*)this;
                //     randomFirePo->actor.update = EnBigpo_UpdateFire;
                //     Actor_ChangeCategory(play, &play->actorCtx, &randomFirePo->actor, ACTORCAT_PROP);
                //     randomFirePo->unk20C = fireIndex;
                //     randomFirePo->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
                //     // make invisible by size: 0
                //     Actor_SetScale(&randomFirePo->actor, 0);

                //     if (this->actor.child == NULL) {
                //         this->actor.child = &randomFirePo->actor;
                //     } else {
                //         randomFirePo->actor.child = this->actor.child;
                //         this->actor.child = &randomFirePo->actor;
                //     }
                //     break;
                // }

                // randomIndex--;
            }
            enemyPtr = enemyPtr->next;
        }
    }

    // // remove unused fires
    // for (enemyPtr = GET_FIRST_ENEMY(play); enemyPtr != NULL; enemyPtr = enemyPtr->next) {
    //     if ((enemyPtr->id == ACTOR_EN_BIGPO) && (enemyPtr->params == BIG_POE_TYPE_POSSIBLE_FIRE)) {
    //         randomFirePo = (EnBigpo*)enemyPtr;
    //         randomFirePo->actionFunc = EnBigpo_Die;
    //         randomFirePo->actor.update = EnBigpo_UpdateFire;
    //     }
    // }

    EnBigpo_ChangeToFireCounting(this);
}