#include "modding.h"
#include "global.h"
#include "overlays/actors/ovl_Obj_Moon_Stone/z_obj_moon_stone.h"

#include "apcommon.h"

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

#define FLAGS (ACTOR_FLAG_10)

#define THIS ((EnElforg*)thisx)

#define STRAY_FAIRY_TYPE(thisx) ((thisx)->params & 0xF)
#define STRAY_FAIRY_GET_NON_DUNGEON_AREA(thisx) (((thisx)->params & 0x1C0) >> 6)
#define STRAY_FAIRY_GET_FLAG(thisx) (((thisx)->params & 0xFE00) >> 9)
#define STRAY_FAIRY_SPARKLE_COUNT(thisx) ((thisx)->home.rot.x)

//! @note `nonDungeonArea` does not always use the enum
#define STRAY_FAIRY_PARAMS(flag, nonDungeonArea, type) ((((flag) & 0x7F) << 9) | (((nonDungeonArea) & 7) << 6) | ((type) & 0xF))

#define STRAY_FAIRY_FLAG_MOVES_QUICKLY_TO_HOME (1 << 0)
#define STRAY_FAIRY_FLAG_SPARKLES_AND_SHRINKS (1 << 1)
#define STRAY_FAIRY_FLAG_CIRCLES_QUICKLY_IN_FOUNTAIN (1 << 2)
#define STRAY_FAIRY_FLAG_GREAT_FAIRYS_MASK_EQUIPPED (1 << 3)

#define LOCATION_STRAY_FAIRY (0x010000 | (play->sceneId << 8) | STRAY_FAIRY_GET_FLAG(&this->actor))
#define LOCATION_CLOCK_TOWN_STRAY_FAIRY 0x01007F

extern FlexSkeletonHeader gStrayFairySkel;
extern AnimationHeader gStrayFairyFlyingAnim;

typedef enum StrayFairyType {
    /* 0 */ STRAY_FAIRY_TYPE_FREE_FLOATING,        // The ones just floating around
    /* 1 */ STRAY_FAIRY_TYPE_FAIRY_FOUNTAIN,       // The ones already present when you enter a Fairy Fountain
    /* 2 */ STRAY_FAIRY_TYPE_BUBBLE,               // The ones trapped in bubbles
    /* 3 */ STRAY_FAIRY_TYPE_CLOCK_TOWN,           // The free-floating Stray Fairies in Clock Town
    /* 4 */ STRAY_FAIRY_TYPE_ENEMY,                // The ones trapped inside enemies
    /* 5 */ STRAY_FAIRY_TYPE_COLLIDER,             // Unused in retail. The fairy is hidden until the collider is hit
    /* 6 */ STRAY_FAIRY_TYPE_CHEST,                // The ones in treasure chests
    /* 7 */ STRAY_FAIRY_TYPE_COLLECTIBLE,          // The ones in boxes, pots, beehives, etc.
    /* 8 */ STRAY_FAIRY_TYPE_RETURNING_TO_FOUNTAIN // The ones you "turn in" by walking into a Fairy Fountain
} StrayFairyType;

typedef enum GreatFairyType {
    /* 0 */ GREAT_FAIRY_TYPE_MAGIC,
    /* 1 */ GREAT_FAIRY_TYPE_POWER,
    /* 2 */ GREAT_FAIRY_TYPE_WISDOM,
    /* 3 */ GREAT_FAIRY_TYPE_COURAGE,
    /* 4 */ GREAT_FAIRY_TYPE_KINDNESS
} GreatFairyType;

// Corresponds to the Great Fairy types
typedef enum StrayFairyArea {
    /* 0 */ STRAY_FAIRY_AREA_CLOCK_TOWN = GREAT_FAIRY_TYPE_MAGIC,
    /* 1 */ STRAY_FAIRY_AREA_WOODFALL = GREAT_FAIRY_TYPE_POWER,
    /* 2 */ STRAY_FAIRY_AREA_SNOWHEAD = GREAT_FAIRY_TYPE_WISDOM,
    /* 3 */ STRAY_FAIRY_AREA_GREAT_BAY = GREAT_FAIRY_TYPE_COURAGE,
    /* 4 */ STRAY_FAIRY_AREA_STONE_TOWER = GREAT_FAIRY_TYPE_KINDNESS,
    /* 5 */ STRAY_FAIRY_AREA_MAX
} StrayFairyArea;

typedef enum StrayFairyLimb {
    /* 0x00 */ STRAY_FAIRY_LIMB_NONE,
    /* 0x01 */ STRAY_FAIRY_LIMB_RIGHT_FACING_HEAD,
    /* 0x02 */ STRAY_FAIRY_LIMB_LEFT_WING,
    /* 0x03 */ STRAY_FAIRY_LIMB_RIGHT_WING,
    /* 0x04 */ STRAY_FAIRY_LIMB_GLOW,
    /* 0x05 */ STRAY_FAIRY_LIMB_TORSO,
    /* 0x06 */ STRAY_FAIRY_LIMB_RIGHT_ARM,
    /* 0x07 */ STRAY_FAIRY_LIMB_PELVIS_AND_LEGS,
    /* 0x08 */ STRAY_FAIRY_LIMB_LEFT_ARM,
    /* 0x09 */ STRAY_FAIRY_LIMB_LEFT_FACING_HEAD,
    /* 0x0A */ STRAY_FAIRY_LIMB_MAX
} StrayFairyLimb;

struct EnElforg;

typedef void (*EnElforgActionFunc)(struct EnElforg*, PlayState*);

typedef struct EnElforg {
    /* 0x000 */ Actor actor;
    /* 0x144 */ SkelAnime skelAnime;
    /* 0x188 */ Vec3s jointTable[STRAY_FAIRY_LIMB_MAX];
    /* 0x1C4 */ ColliderCylinder collider;
    /* 0x210 */ Actor* enemy;
    /* 0x214 */ u16 strayFairyFlags;
    /* 0x216 */ s16 direction; // negative when facing right, positive when facing left
    /* 0x218 */ s16 area;
    /* 0x21C */ s32 timer;
    /* 0x220 */ s32 secondaryTimer;
    /* 0x224 */ f32 targetSpeedXZ;
    /* 0x228 */ f32 targetDistanceFromHome;
    /* 0x22C */ EnElforgActionFunc actionFunc;
} EnElforg; // size = 0x230

static bool gotLaundryPool = false;

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_PLAYER,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 16, 32, 0, { 0, 0, 0 } },
};

void EnElforg_Draw(Actor* thisx, PlayState* play);

void EnElforg_FreeFloatingFairyFountain(EnElforg* this, PlayState* play);
void EnElforg_FreeFloating(EnElforg* this, PlayState* play);
void EnElforg_TurnInFairy(EnElforg* this, PlayState* play);
void EnElforg_TrappedByBubble(EnElforg* this, PlayState* play);
Actor* EnElforg_GetHoldingEnemy(EnElforg* this, PlayState* play);

void EnElforg_FairyCollected(EnElforg* this, PlayState* play);
void EnElforg_SetupFairyCollected(EnElforg* this, PlayState* play);
void EnElforg_SetupTrappedByEnemy(EnElforg* this, PlayState* play);
void EnElforg_TrappedByEnemy(EnElforg* this, PlayState* play);
void EnElforg_CirclePlayer(EnElforg* this, PlayState* play);
void EnElforg_MoveToTarget(EnElforg* this, Vec3f* targetPos);
void EnElforg_HiddenByCollider(EnElforg* this, PlayState* play);
void func_80ACCBB8(EnElforg* this, PlayState* play);
void EnElforg_InitializeParams(EnElforg* this);

static u32 fairyGI[15];
static bool townFairyObjectStatic[15];
static bool townFairyObjectLoading[15];
static bool townFairyObjectLoaded[15];
static bool townFairyRenderNormal[15];
static OSMesgQueue townFairyObjectLoadQueue[15];
static void* townFairyObjectSegment[15];

RECOMP_PATCH void EnElforg_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnElforg* this = THIS;

    Actor_SetScale(thisx, 0.01f);
    this->strayFairyFlags = 0;
    this->direction = 0;
    SkelAnime_InitFlex(play, &this->skelAnime, &gStrayFairySkel, &gStrayFairyFlyingAnim, this->jointTable,
                       this->jointTable, STRAY_FAIRY_LIMB_MAX);
    this->skelAnime.playSpeed = 1.0f;
    ActorShape_Init(&thisx->shape, 0.0f, NULL, 0.0f);
    thisx->shape.shadowAlpha = 255;

    s16 getItemId;
    switch (this->area) {
        case STRAY_FAIRY_AREA_WOODFALL:
        case STRAY_FAIRY_AREA_SNOWHEAD:
        case STRAY_FAIRY_AREA_GREAT_BAY:
        case STRAY_FAIRY_AREA_STONE_TOWER:
            getItemId = rando_get_item_id(LOCATION_STRAY_FAIRY);
            break;
        default:
            getItemId = rando_get_item_id(LOCATION_CLOCK_TOWN_STRAY_FAIRY);
    }

    fairyGI[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = getItemId;
    townFairyObjectStatic[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = false;
    townFairyObjectLoading[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = false;
    townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = false;
    townFairyRenderNormal[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = false;

    if (rando_location_is_checked(LOCATION_STRAY_FAIRY)) {
        if (Actor_HasParent(thisx, play)) {
            Actor_Kill(thisx->parent);
        }
        Actor_Kill(thisx);
        return;
    }

    switch (STRAY_FAIRY_TYPE(thisx)) {
        case STRAY_FAIRY_TYPE_CLOCK_TOWN:
            /*if (CHECK_WEEKEVENTREG(WEEKEVENTREG_08_80)) {
                Actor_Kill(thisx);
                return;
            }*/
            if (rando_location_is_checked(LOCATION_CLOCK_TOWN_STRAY_FAIRY)) {
                Actor_Kill(thisx);
                return;
            }
            break;

        case STRAY_FAIRY_TYPE_COLLECTIBLE:
            if (Flags_GetCollectible(play, STRAY_FAIRY_GET_FLAG(thisx))) {
                Actor_Kill(thisx);
                return;
            }
            break;

        case STRAY_FAIRY_TYPE_FAIRY_FOUNTAIN:
        case STRAY_FAIRY_TYPE_BUBBLE:
        case STRAY_FAIRY_TYPE_CHEST:
        case STRAY_FAIRY_TYPE_RETURNING_TO_FOUNTAIN:
            break;

        default:
            if (Flags_GetSwitch(play, STRAY_FAIRY_GET_FLAG(thisx))) {
                Actor_Kill(thisx);
                return;
            }
            break;
    }

    if (Map_IsInDungeonOrBossArea(play)) {
        this->area = gSaveContext.dungeonIndex + STRAY_FAIRY_AREA_WOODFALL;
    } else {
        this->area = STRAY_FAIRY_GET_NON_DUNGEON_AREA(thisx);
    }

    switch (STRAY_FAIRY_TYPE(thisx)) {
        case STRAY_FAIRY_TYPE_FAIRY_FOUNTAIN:
            EnElforg_InitializeParams(this);
            this->actionFunc = EnElforg_FreeFloatingFairyFountain;
            this->targetSpeedXZ = Rand_ZeroFloat(2.0f) + 1.0f;
            this->targetDistanceFromHome = Rand_ZeroFloat(100.0f) + 50.0f;
            break;

        case STRAY_FAIRY_TYPE_RETURNING_TO_FOUNTAIN:
            EnElforg_InitializeParams(this);
            this->actionFunc = EnElforg_TurnInFairy;
            this->secondaryTimer = 60;
            break;

        case STRAY_FAIRY_TYPE_BUBBLE:
            this->timer = 0;
            this->actionFunc = EnElforg_TrappedByBubble;
            break;

        case STRAY_FAIRY_TYPE_ENEMY:
            this->actionFunc = EnElforg_SetupTrappedByEnemy;
            EnElforg_SetupTrappedByEnemy(this, play);
            thisx->draw = NULL;
            break;

        case STRAY_FAIRY_TYPE_COLLIDER:
            this->actionFunc = EnElforg_HiddenByCollider;
            thisx->draw = NULL;
            Collider_InitAndSetCylinder(play, &this->collider, thisx, &sCylinderInit);
            Collider_UpdateCylinder(thisx, &this->collider);
            break;

        default:
            EnElforg_InitializeParams(this);
            this->actionFunc = EnElforg_FreeFloating;
            break;
    }

    thisx->shape.rot.y = 0;
}

RECOMP_PATCH void EnElforg_ClockTownFairyCollected(EnElforg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    EnElforg_CirclePlayer(this, play);

    player->actor.freezeTimer = 100;
    player->stateFlags1 |= PLAYER_STATE1_20000000;

    if (Actor_TextboxIsClosing(&this->actor, play)) {
        player->actor.freezeTimer = 0;
        player->stateFlags1 &= ~PLAYER_STATE1_20000000;
        Actor_Kill(&this->actor);
        //SET_WEEKEVENTREG(WEEKEVENTREG_08_80);
        CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
        Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
        return;
    }

    Actor_PlaySfx_Flagged(&this->actor, NA_SE_PL_CHIBI_FAIRY_HEAL - SFX_FLAG);
    if (CutsceneManager_GetCurrentCsId() != CS_ID_GLOBAL_TALK) {
        if (CutsceneManager_IsNext(CS_ID_GLOBAL_TALK)) {
            CutsceneManager_Start(CS_ID_GLOBAL_TALK, &this->actor);
        } else {
            CutsceneManager_Queue(CS_ID_GLOBAL_TALK);
        }
    }
}

void EnElforg_Freezing(EnElforg* this, PlayState* play);

RECOMP_PATCH void EnElforg_FreeFloating(EnElforg* this, PlayState* play) {
    Vec3f pos;
    f32 scaledYDistance;
    Player* player = GET_PLAYER(play);

    SkelAnime_Update(&this->skelAnime);

    if (Player_GetMask(play) == PLAYER_MASK_GREAT_FAIRY) {
        pos = player->bodyPartsPos[PLAYER_BODYPART_WAIST];
        this->targetSpeedXZ = 5.0f;
        EnElforg_MoveToTarget(this, &pos);
    } else {
        this->targetSpeedXZ = 1.0f;
        EnElforg_MoveToTarget(this, &this->actor.home.pos);
    }

    scaledYDistance = this->actor.playerHeightRel - (this->actor.shape.yOffset * this->actor.scale.y);

    if (!Player_InCsMode(play)) {
        if ((this->actor.xzDistToPlayer < 30.0f) && (scaledYDistance < 12.0f) && (scaledYDistance > -68.0f)) {
            EnElforg_SetupFairyCollected(this, play);
            Health_ChangeBy(play, 0x30);

            switch (STRAY_FAIRY_TYPE(&this->actor)) {
                case STRAY_FAIRY_TYPE_COLLECTIBLE:
                    Flags_SetCollectible(play, STRAY_FAIRY_GET_FLAG(&this->actor));
                    break;

                case STRAY_FAIRY_TYPE_CHEST:
                    Flags_SetTreasure(play, STRAY_FAIRY_GET_FLAG(&this->actor));
                    break;

                default:
                    Flags_SetSwitch(play, STRAY_FAIRY_GET_FLAG(&this->actor));
                    break;
            }

            if (STRAY_FAIRY_TYPE(&this->actor) == STRAY_FAIRY_TYPE_CLOCK_TOWN) {
                player->actor.freezeTimer = 100;
                player->stateFlags1 |= PLAYER_STATE1_20000000;
                rando_send_location(LOCATION_CLOCK_TOWN_STRAY_FAIRY);
                // Bring me back to North Clock Town!
                Message_StartTextbox(play, rando_get_item_id(LOCATION_CLOCK_TOWN_STRAY_FAIRY), NULL);
                this->actionFunc = EnElforg_ClockTownFairyCollected;
                CutsceneManager_Queue(CS_ID_GLOBAL_TALK);
                return;
            }

            if (Map_IsInDungeonOrBossArea(play)) {
                //gSaveContext.save.saveInfo.inventory.strayFairies[gSaveContext.dungeonIndex]++;
                recomp_printf("stray fairy location: 0x%06X\n", LOCATION_STRAY_FAIRY);
                rando_send_location(LOCATION_STRAY_FAIRY);
                player->actor.freezeTimer = 10;
                player->stateFlags1 |= PLAYER_STATE1_20000000;
                // You found a Stray Fairy!
                Message_StartTextbox(play, rando_get_item_id(LOCATION_STRAY_FAIRY), NULL);
                if (gSaveContext.save.saveInfo.inventory.strayFairies[(void)0, gSaveContext.dungeonIndex] >=
                    STRAY_FAIRY_SCATTERED_TOTAL) {
                    Audio_PlayFanfare(NA_BGM_GET_ITEM | 0x900);
                }
            }
        }

        Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 20.0f, 20.0f,
                                UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_2 | UPDBGCHECKINFO_FLAG_4);
        func_80ACCBB8(this, play);

        if (Player_GetMask(play) == PLAYER_MASK_GREAT_FAIRY) {
            if (!(this->strayFairyFlags & STRAY_FAIRY_FLAG_GREAT_FAIRYS_MASK_EQUIPPED)) {
                Audio_PlaySfx(NA_SE_SY_FAIRY_MASK_SUCCESS);
            }

            this->strayFairyFlags |= STRAY_FAIRY_FLAG_GREAT_FAIRYS_MASK_EQUIPPED;
        } else {
            this->strayFairyFlags &= ~STRAY_FAIRY_FLAG_GREAT_FAIRYS_MASK_EQUIPPED;
        }
    }
}

RECOMP_PATCH void EnElforg_FairyCollected(EnElforg* this, PlayState* play) {
    Player* player = GET_PLAYER(play);
    EnElforg_CirclePlayer(this, play);

    if (Actor_TextboxIsClosing(&this->actor, play)) {
        player->actor.freezeTimer = 0;
        player->stateFlags1 &= ~PLAYER_STATE1_20000000;
        Actor_Kill(&this->actor);
        CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
        Interface_SetHudVisibility(HUD_VISIBILITY_ALL);
        return;
    } else {
        player->actor.freezeTimer = 10;
        player->stateFlags1 |= PLAYER_STATE1_20000000;
    }

    Actor_PlaySfx_Flagged(&this->actor, NA_SE_PL_CHIBI_FAIRY_HEAL - SFX_FLAG);
}


void EnElforg_WaitForObject(EnElforg* this, PlayState* play) {
    s16 getItemId;
    switch (this->area) {
        case STRAY_FAIRY_AREA_WOODFALL:
        case STRAY_FAIRY_AREA_SNOWHEAD:
        case STRAY_FAIRY_AREA_GREAT_BAY:
        case STRAY_FAIRY_AREA_STONE_TOWER:
            getItemId = rando_get_item_id(LOCATION_STRAY_FAIRY);
            break;
        default:
            getItemId = rando_get_item_id(LOCATION_CLOCK_TOWN_STRAY_FAIRY);
    }

    if (play->sceneId == SCENE_YOUSEI_IZUMI) {
        townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
        return;
    }

    switch (getItemId) {
        case GI_B2:
            this->area = STRAY_FAIRY_AREA_CLOCK_TOWN;
            townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
            townFairyRenderNormal[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
            break;
        case GI_46:
            this->area = STRAY_FAIRY_AREA_WOODFALL;
            townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
            townFairyRenderNormal[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
            break;
        case GI_47:
            this->area = STRAY_FAIRY_AREA_SNOWHEAD;
            townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
            townFairyRenderNormal[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
        case GI_48:
            this->area = STRAY_FAIRY_AREA_GREAT_BAY;
            townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
            townFairyRenderNormal[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
            break;
        case GI_49:
            this->area = STRAY_FAIRY_AREA_STONE_TOWER;
            townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
            townFairyRenderNormal[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
            break;
        
        default:
            break;
    }

    s16 objectSlot = Object_GetSlot(&play->objectCtx, getObjectId(getItemId));

    if (isAP(getItemId)) {
        townFairyObjectStatic[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
        townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
        fairyGI[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = getItemId;
    } else if (!townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] && !townFairyObjectLoading[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] && Object_IsLoaded(&play->objectCtx, objectSlot)) {
        this->actor.objectSlot = objectSlot;
        Actor_SetObjectDependency(play, &this->actor);
        townFairyObjectStatic[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
        townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
        fairyGI[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = getItemId;
    } else if (!townFairyObjectLoading[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] && !townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15]) {
        loadObject(play, &townFairyObjectSegment[STRAY_FAIRY_GET_FLAG(&this->actor) % 15], &townFairyObjectLoadQueue[STRAY_FAIRY_GET_FLAG(&this->actor) % 15], getObjectId(getItemId));
        townFairyObjectLoading[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
    } else if (osRecvMesg(&townFairyObjectLoadQueue[STRAY_FAIRY_GET_FLAG(&this->actor) % 15], NULL, OS_MESG_NOBLOCK) == 0) {
        townFairyObjectLoading[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = false;
        townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = true;
        fairyGI[STRAY_FAIRY_GET_FLAG(&this->actor) % 15] = getItemId;
    }
}

RECOMP_HOOK("EnElforg_Update")
void EnElforg_CheckLoaded(Actor* thisx, PlayState* play) {
    EnElforg* this = THIS;

    if (!townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15]) {
        EnElforg_WaitForObject(this, play);
    }
}

RECOMP_PATCH s32 EnElforg_OverrideLimbDraw(PlayState* play, s32 limbIndex, Gfx** dList, Vec3f* pos, Vec3s* rot, Actor* thisx,
                              Gfx** gfx) {
    EnElforg* this = THIS;

    if (play->sceneId == SCENE_YOUSEI_IZUMI || townFairyRenderNormal[STRAY_FAIRY_GET_FLAG(&this->actor) % 15]) {
        if (this->direction < 0) {
            if (limbIndex == STRAY_FAIRY_LIMB_LEFT_FACING_HEAD) {
                *dList = NULL;
            }
        } else if (limbIndex == STRAY_FAIRY_LIMB_RIGHT_FACING_HEAD) {
            *dList = NULL;
        }

        return false;
    }

    // sometime draws with the wrong wing color?
    switch (limbIndex) {
        case STRAY_FAIRY_LIMB_LEFT_WING:
        case STRAY_FAIRY_LIMB_RIGHT_WING:
        case STRAY_FAIRY_LIMB_GLOW:
            return false;
        
        case STRAY_FAIRY_LIMB_TORSO:
            if (townFairyObjectLoaded[STRAY_FAIRY_GET_FLAG(&this->actor) % 15]) {
                Matrix_Translate(this->actor.world.pos.x,
                                this->actor.world.pos.y + (this->actor.shape.yOffset * this->actor.scale.y),
                                this->actor.world.pos.z, MTXMODE_NEW);
                Matrix_Scale(0.5f, 0.5f, 0.5f, MTXMODE_APPLY);
                Matrix_RotateZYX(0, play->gameplayFrames * 0x3E8, 0, MTXMODE_APPLY);

                if (townFairyObjectStatic[STRAY_FAIRY_GET_FLAG(&this->actor) % 15]) {
                    GetItem_Draw(play, getGid(fairyGI[STRAY_FAIRY_GET_FLAG(&this->actor) % 15]));
                } else {
                    GetItem_DrawDynamic(play, townFairyObjectSegment[STRAY_FAIRY_GET_FLAG(&this->actor) % 15], getGid(fairyGI[STRAY_FAIRY_GET_FLAG(&this->actor) % 15]));
                }
            }

        default:
            *dList = NULL;
            break;
    }

    return true;
}