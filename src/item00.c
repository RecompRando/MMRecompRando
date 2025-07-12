#include "modding.h"
#include "z64.h"
#include "z64actor.h"
#include "z64collision_check.h"
#include "ichain.h"
#include "ultra64.h"

#include "apcommon.h"
#include "actor_helpers.h"

#define THIS ((EnItem00*)thisx)

#define LOCATION_HEART_PIECE ((0x050000) | (sceneId << 8) | this->collectibleFlag)

#define ENITEM00_GET_8000(thisx) ((thisx)->params & 0x8000)
#define ENITEM00_GET_7F00(thisx) (((thisx)->params & 0x7F00) >> 8)

static Vec3f sEffectVelocity = { 0.0f, 0.1f, 0.0f };
static Vec3f sEffectAccel = { 0.0f, 0.01f, 0.0f };
static Color_RGBA8 sEffectPrimColor = { 255, 255, 127, 0 };
static Color_RGBA8 sEffectEnvColor = { 255, 255, 255, 0 };

static bool objectStatic;
static bool objectLoading;
static bool objectLoaded;
static OSMesgQueue objectLoadQueue;
static void* objectSegment;

ActorExtensionId item00Extension;
u32* extendedItem00Data;

void func_800A640C(EnItem00* this, PlayState* play);
void func_800A6A40(EnItem00* this, PlayState* play);

void* ZeldaArena_Malloc(size_t size);
void ZeldaArena_Free(void* ptr);
void Actor_ProcessInitChain(Actor* thisx, InitChainEntry* ichain);
s32 Collider_InitAndSetCylinder(struct PlayState* play, ColliderCylinder* collider, struct Actor* actor, ColliderCylinderInit* src);
void EnItem00_WaitForHeartObject(EnItem00* this, PlayState* play);
void EnItem00_WaitForObject(EnItem00* this, PlayState* play);
void EnItem00_SetObject(EnItem00* this, PlayState* play, f32* shadowOffset, f32* shadowScale);
void EnItem00_DrawRupee(EnItem00* this, PlayState* play);
void EnItem00_DrawSprite(EnItem00* this, PlayState* play);
void EnItem00_DrawHeartContainer(EnItem00* this, PlayState* play);
void EnItem00_DrawHeartPiece(EnItem00* this, PlayState* play);

static ColliderCylinderInit sCylinderInit = {
    {
        COLTYPE_NONE,
        AT_NONE,
        AC_ON | AT_TYPE_PLAYER,
        OC1_NONE,
        OC2_NONE,
        COLSHAPE_CYLINDER,
    },
    {
        ELEMTYPE_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x00000010, 0x00, 0x00 },
        TOUCH_NONE | TOUCH_SFX_NORMAL,
        BUMP_ON,
        OCELEM_NONE,
    },
    { 10, 30, 0, { 0, 0, 0 } },
};

static InitChainEntry sInitChain[] = {
    ICHAIN_F32(targetArrowOffset, 2000, ICHAIN_STOP),
};

static Item00Type item00ShuffledList[] = { ITEM00_HEART_PIECE };

RECOMP_IMPORT("*", int recomp_printf(const char* fmt, ...));

RECOMP_PATCH void EnItem00_Init(Actor* thisx, PlayState* play) {
    EnItem00* this = THIS;
    s32 pad;
    f32 shadowOffset = 980.0f;
    f32 shadowScale = 6.0f;
    GetItemId getItemId = GI_NONE;
    s32 sp30 = ENITEM00_GET_8000(thisx) ? 1 : 0;
    GetItemId i;
    bool shuffled = false;
    u8 sceneId = (play->sceneId == 0x00) ? 0x45 : play->sceneId;

    //objectSegment = ZeldaArena_Malloc(0x9000);
    this->collectibleFlag = ENITEM00_GET_7F00(thisx);

    thisx->params &= 0xFF;

    for (i = 0; i < sizeof(item00ShuffledList)/sizeof(item00ShuffledList[0]); ++i) {
        if (thisx->params == item00ShuffledList[i]) {
            shuffled = true;
            break;
        }
    }

    if (/*Flags_GetCollectible(play, this->collectibleFlag) || */(thisx->params == ITEM00_HEART_PIECE && rando_location_is_checked(LOCATION_HEART_PIECE))) {
        Actor_Kill(thisx);
        return;
    } else if (shuffled) {
        this->getItemId = rando_get_item_id(LOCATION_HEART_PIECE);
        objectStatic = false;
        objectLoading = false;
        objectLoaded = false;
        objectSegment = NULL;
        if (this->getItemId == GI_RECOVERY_HEART) {
            sp30 = 0;
        }
    }

    if (thisx->params == ITEM00_3_HEARTS) {
        thisx->params = ITEM00_RECOVERY_HEART;
    }

    Actor_ProcessInitChain(thisx, sInitChain);
    Collider_InitAndSetCylinder(play, &this->collider, thisx, &sCylinderInit);

    this->unk150 = 1;

    switch (thisx->params) {
        case ITEM00_RUPEE_GREEN:
        case ITEM00_RUPEE_BLUE:
        case ITEM00_RUPEE_RED:
            Actor_SetScale(thisx, 0.015f);
            this->unk154 = 0.015f;
            shadowOffset = 750.0f;
            break;

        case ITEM00_SMALL_KEY:
            this->unk150 = 0;
            Actor_SetScale(thisx, 0.03f);
            this->unk154 = 0.03f;
            shadowOffset = 350.0f;
            break;

        case ITEM00_HEART_PIECE:
        case ITEM00_HEART_CONTAINER:
            this->unk150 = 0;
            Actor_SetScale(thisx, 0.02f);
            this->unk154 = 0.02f;
            shadowOffset = 650.0f;
            if (thisx->params == ITEM00_HEART_CONTAINER) {
                sp30 = -1;
            }
            break;

        case ITEM00_RECOVERY_HEART:
            thisx->home.rot.z = Rand_CenteredFloat(0xFFFF);
            shadowOffset = 430.0f;
            Actor_SetScale(thisx, 0.02f);
            this->unk154 = 0.02f;
            break;

        case ITEM00_ARROWS_10:
        case ITEM00_ARROWS_30:
        case ITEM00_ARROWS_40:
        case ITEM00_ARROWS_50:
            Actor_SetScale(thisx, 0.035f);
            this->unk154 = 0.035f;
            shadowOffset = 250.0f;
            break;

        case ITEM00_BOMBS_A:
        case ITEM00_BOMBS_B:
        case ITEM00_DEKU_NUTS_1:
        case ITEM00_DEKU_STICK:
        case ITEM00_MAGIC_JAR_SMALL:
        case ITEM00_DEKU_NUTS_10:
        case ITEM00_BOMBS_0:
        case ITEM00_BOMBCHU:
            Actor_SetScale(thisx, 0.03f);
            this->unk154 = 0.03f;
            shadowOffset = 320.0f;
            break;

        case ITEM00_MAGIC_JAR_BIG:
            Actor_SetScale(thisx, 4.5f * 0.01f);
            this->unk154 = 4.5f * 0.01f;
            shadowOffset = 320.0f;
            break;

        case ITEM00_RUPEE_HUGE:
            Actor_SetScale(thisx, 4.5f * 0.01f);
            this->unk154 = 4.5f * 0.01f;
            shadowOffset = 750.0f;
            break;

        case ITEM00_RUPEE_PURPLE:
            Actor_SetScale(thisx, 0.03f);
            this->unk154 = 0.03f;
            shadowOffset = 750.0f;
            break;

        case ITEM00_FLEXIBLE:
        case ITEM00_BIG_FAIRY:
            shadowOffset = 500.0f;
            Actor_SetScale(thisx, 0.01f);
            this->unk154 = 0.01f;
            break;

        case ITEM00_SHIELD_HERO:
            thisx->objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_GI_SHIELD_2);
            EnItem00_SetObject(this, play, &shadowOffset, &shadowScale);
            break;

        case ITEM00_MAP:
            thisx->objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_GI_MAP);
            EnItem00_SetObject(this, play, &shadowOffset, &shadowScale);
            break;

        case ITEM00_COMPASS:
            thisx->objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_GI_COMPASS);
            EnItem00_SetObject(this, play, &shadowOffset, &shadowScale);
            break;

        default:
            break;
    }

    this->unk14E = 0;
    ActorShape_Init(&thisx->shape, shadowOffset, ActorShadow_DrawCircle, shadowScale);
    thisx->shape.shadowAlpha = 180;
    thisx->focus.pos = thisx->world.pos;

    if (sp30 < 0) {
        this->actionFunc = EnItem00_WaitForHeartObject;
        this->unk152 = -1;
        return;
    }

    if (sp30 == 0) {
        this->actionFunc = func_800A640C;
        this->unk152 = -1;
        return;
    }

    this->unk152 = 15;
    this->unk14C = 35;

    thisx->speed = 0.0f;
    thisx->velocity.y = 0.0f;
    thisx->gravity = 0.0f;

    switch (thisx->params) {
        case ITEM00_RUPEE_GREEN:
            Item_Give(play, ITEM_RUPEE_GREEN);
            break;

        case ITEM00_RUPEE_BLUE:
            Item_Give(play, ITEM_RUPEE_BLUE);
            break;

        case ITEM00_RUPEE_RED:
            Item_Give(play, ITEM_RUPEE_RED);
            break;

        case ITEM00_RUPEE_PURPLE:
            Item_Give(play, ITEM_RUPEE_PURPLE);
            break;

        case ITEM00_RUPEE_HUGE:
            Item_Give(play, ITEM_RUPEE_HUGE);
            break;

        case ITEM00_RECOVERY_HEART:
            Item_Give(play, ITEM_RECOVERY_HEART);
            break;

        case ITEM00_FLEXIBLE:
        case ITEM00_BIG_FAIRY:
            Health_ChangeBy(play, 0x70);
            break;

        case ITEM00_BOMBS_A:
        case ITEM00_BOMBS_B:
            Item_Give(play, ITEM_BOMBS_5);
            break;

        case ITEM00_BOMBCHU:
            Item_Give(play, ITEM_BOMBCHUS_5);
            break;

        case ITEM00_ARROWS_10:
            Item_Give(play, ITEM_ARROWS_10);
            break;

        case ITEM00_ARROWS_30:
            Item_Give(play, ITEM_ARROWS_30);
            break;

        case ITEM00_ARROWS_40:
            Item_Give(play, ITEM_ARROWS_40);
            break;

        case ITEM00_ARROWS_50:
            Item_Give(play, ITEM_ARROWS_50);
            break;

        case ITEM00_MAGIC_JAR_BIG:
            Item_Give(play, ITEM_MAGIC_JAR_BIG);
            break;

        case ITEM00_MAGIC_JAR_SMALL:
            Item_Give(play, ITEM_MAGIC_JAR_SMALL);
            break;

        case ITEM00_SMALL_KEY:
            Item_Give(play, ITEM_KEY_SMALL);
            break;

        case ITEM00_DEKU_NUTS_1:
            getItemId = GI_DEKU_NUTS_1;
            break;

        case ITEM00_DEKU_NUTS_10:
            getItemId = GI_DEKU_NUTS_10;
            break;

        default:
            break;
    }

    if ((getItemId != GI_NONE) && !Actor_HasParent(thisx, play)) {
        //~ Actor_OfferGetItemHook(thisx, play, getItemId, LOCATION_HEART_PIECE, 50.0f, 20.0f, false, true);
    }

    this->actionFunc = func_800A6A40;
    this->actionFunc(this, play);
}

void EnItem00_RandoGive(EnItem00* this, PlayState* play, s32 getItemId, u32 location);

RECOMP_PATCH void EnItem00_Update(Actor* thisx, PlayState* play) {
    EnItem00* this = THIS;
    s32 pad;
    Player* player = GET_PLAYER(play);
    s32 sp38 = player->stateFlags3 & PLAYER_STATE3_1000;
    s32 getItemId = GI_NONE;
    s32 params;
    u8 sceneId = (play->sceneId == 0x00) ? 0x45 : play->sceneId;

    if (this->unk152 > 0) {
        this->unk152--;
    }

    if ((this->unk152 > 0) && (this->unk152 <= 40) && (this->unk14C <= 0)) {
        this->unk14E = this->unk152;
    }

    params = this->actor.params;
    if (params == ITEM00_HEART_PIECE && !objectLoaded) {
        EnItem00_WaitForObject(this, play);
        return;
    }

    this->actionFunc(this, play);

    Math_SmoothStepToF(&this->actor.scale.x, this->unk154, 0.1f, this->unk154 * 0.1f, 0.0f);
    this->actor.scale.z = this->actor.scale.x;
    this->actor.scale.y = this->actor.scale.x;

    if (this->actor.gravity != 0.0f) {
        Actor_MoveWithGravity(&this->actor);
        Actor_UpdateBgCheckInfo(play, &this->actor, 20.0f, 15.0f, 15.0f,
                                UPDBGCHECKINFO_FLAG_1 | UPDBGCHECKINFO_FLAG_4 | UPDBGCHECKINFO_FLAG_8 |
                                    UPDBGCHECKINFO_FLAG_10);

        if (this->actor.floorHeight <= BGCHECK_Y_MIN) {
            Actor_Kill(&this->actor);
            return;
        }
    }

    Collider_UpdateCylinder(&this->actor, &this->collider);
    CollisionCheck_SetAC(play, &play->colChkCtx, &this->collider.base);

    if ((params == ITEM00_SHIELD_HERO) || (params == ITEM00_MAP) || (params == ITEM00_COMPASS)) {
        this->actor.shape.yOffset = fabsf(Math_CosS(this->actor.shape.rot.x) * 37.0f);
    }

    if (this->unk14C > 0) {
        return;
    }

    if (!((sp38 != 0) && (this->actor.xzDistToPlayer <= 60.0f) && (this->actor.playerHeightRel >= -100.0f) &&
          (this->actor.playerHeightRel <= 100.0f)) &&
        !((sp38 == 0) && (this->actor.xzDistToPlayer <= 30.0f) && (this->actor.playerHeightRel >= -50.0f) &&
          (this->actor.playerHeightRel <= 50.0f))) {
        if (!Actor_HasParent(&this->actor, play)) {
            return;
        }
    }

    if (play->gameOverCtx.state != GAMEOVER_INACTIVE) {
        return;
    }

    switch (this->actor.params) {
        case ITEM00_RUPEE_GREEN:
            this->unk1A4 = 1;
            Item_Give(play, ITEM_RUPEE_GREEN);
            break;

        case ITEM00_RUPEE_BLUE:
            this->unk1A4 = 1;
            Item_Give(play, ITEM_RUPEE_BLUE);
            break;

        case ITEM00_RUPEE_RED:
            this->unk1A4 = 1;
            Item_Give(play, ITEM_RUPEE_RED);
            break;

        case ITEM00_RUPEE_PURPLE:
            this->unk1A4 = 1;
            Item_Give(play, ITEM_RUPEE_PURPLE);
            break;

        case ITEM00_RUPEE_HUGE:
            this->unk1A4 = 1;
            Item_Give(play, ITEM_RUPEE_HUGE);
            break;

        case ITEM00_DEKU_STICK:
            getItemId = GI_DEKU_STICKS_1;
            break;

        case ITEM00_DEKU_NUTS_1:
            getItemId = GI_DEKU_NUTS_1;
            break;

        case ITEM00_DEKU_NUTS_10:
            getItemId = GI_DEKU_NUTS_10;
            break;

        case ITEM00_RECOVERY_HEART:
            Item_Give(play, ITEM_RECOVERY_HEART);
            break;

        case ITEM00_FLEXIBLE:
        case ITEM00_BIG_FAIRY:
            Health_ChangeBy(play, 0x70);
            break;

        case ITEM00_BOMBS_A:
        case ITEM00_BOMBS_B:
            Item_Give(play, ITEM_BOMBS_5);
            break;

        case ITEM00_BOMBCHU:
            Item_Give(play, ITEM_BOMBCHUS_5);
            break;

        case ITEM00_ARROWS_10:
            Item_Give(play, ITEM_ARROWS_10);
            break;

        case ITEM00_ARROWS_30:
            Item_Give(play, ITEM_ARROWS_30);
            break;

        case ITEM00_ARROWS_40:
            Item_Give(play, ITEM_ARROWS_40);
            break;

        case ITEM00_ARROWS_50:
            Item_Give(play, ITEM_ARROWS_50);
            break;

        case ITEM00_SMALL_KEY:
            getItemId = GI_KEY_SMALL;
            break;

        case ITEM00_HEART_PIECE:
            getItemId = rando_get_item_id(LOCATION_HEART_PIECE);
            break;

        case ITEM00_HEART_CONTAINER:
            getItemId = GI_HEART_CONTAINER;
            break;

        case ITEM00_MAGIC_JAR_BIG:
            Item_Give(play, ITEM_MAGIC_JAR_BIG);
            break;

        case ITEM00_MAGIC_JAR_SMALL:
            Item_Give(play, ITEM_MAGIC_JAR_SMALL);
            break;

        case ITEM00_SHIELD_HERO:
            getItemId = GI_SHIELD_HERO;
            break;

        case ITEM00_MAP:
            getItemId = GI_MAP;
            break;

        case ITEM00_COMPASS:
            getItemId = GI_COMPASS;
            break;

        case ITEM00_APITEM:
            extendedItem00Data = z64recomp_get_extended_actor_data(&this->actor, item00Extension);
            getItemId = rando_get_item_id(*extendedItem00Data);
            break;

        default:
            break;
    }

    if (getItemId != GI_NONE) {
        if (!Actor_HasParent(&this->actor, play)) {
            u32 location = 0;
            switch (this->actor.params) {
                case ITEM00_HEART_PIECE:
                    location = LOCATION_HEART_PIECE;
                    break;
                case ITEM00_APITEM:
                    extendedItem00Data = z64recomp_get_extended_actor_data(&this->actor, item00Extension);
                    location = *extendedItem00Data;
                    break;
                default:
                    break;
            }
            if (this->actor.params == ITEM00_HEART_PIECE && objectLoaded) {
                Flags_SetCollectible(play, this->collectibleFlag);
                Actor_OfferGetItemHook(&this->actor, play, getItemId, 0, 50.0f, 20.0f, true, true);
            } else if (this->actor.params == ITEM00_APITEM) {
                // if (this->actor.xzDistToPlayer <= 50.0f && fabsf(this->actor.playerHeightRel) <= fabsf(20.0f)) {
                if (this->actor.xzDistToPlayer <= 50.0f && fabsf(this->actor.playerHeightRel) <= fabsf(50.0f)) {
                    recomp_printf("Item00 location: 0x%06X\n", location);
                    EnItem00_RandoGive(this, play, getItemId, location);
                }
            } else if (this->actor.params != ITEM00_APITEM && this->actor.params != ITEM00_HEART_PIECE) {
                Actor_OfferGetItemHook(&this->actor, play, getItemId, 0, 50.0f, 20.0f, false, false);
            }
        }
    }

    if (this->actor.params == ITEM00_SHIELD_HERO) {
        Item_Give(play, ITEM_SHIELD_HERO);
    }

    switch (this->actor.params) {
        case ITEM00_HEART_PIECE:
        case ITEM00_HEART_CONTAINER:
        case ITEM00_SMALL_KEY:
        case ITEM00_SHIELD_HERO:
        case ITEM00_MAP:
        case ITEM00_COMPASS:
            if (objectLoaded && Actor_HasParent(&this->actor, play)) {
                Flags_SetCollectible(play, this->collectibleFlag);
                recomp_printf("Heart Piece location: 0x%06X\n", LOCATION_HEART_PIECE);
                objectLoading = false;
                objectLoaded = false;
                objectStatic = false;
                Actor_Kill(thisx);
            }
            return;
        case ITEM00_APITEM:
            if (Actor_HasParent(&this->actor, play)) {
                extendedItem00Data = z64recomp_get_extended_actor_data(&this->actor, item00Extension);
                // recomp_printf("Item00 location: 0x%06X\n", *extendedItem00Data);
                // Actor_Kill(thisx);
            }
            return;

        default:
            break;
    }

    if ((this->actor.params <= ITEM00_RUPEE_RED) || (this->actor.params == ITEM00_RUPEE_HUGE)) {
        Audio_PlaySfx(NA_SE_SY_GET_RUPY);
    } else if (getItemId != GI_NONE) {
        if (Actor_HasParent(&this->actor, play)) {
            Flags_SetCollectible(play, this->collectibleFlag);
            Actor_Kill(&this->actor);
        }
        return;
    } else {
        Audio_PlaySfx(NA_SE_SY_GET_ITEM);
    }

    Flags_SetCollectible(play, this->collectibleFlag);

    this->unk152 = 15;
    this->unk14C = 35;
    this->actor.shape.rot.z = 0;
    this->actor.speed = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.gravity = 0.0f;

    Actor_SetScale(&this->actor, this->unk154);

    this->getItemId = GI_NONE;
    this->actionFunc = func_800A6A40;
}

void EnItem00_WaitForObject(EnItem00* this, PlayState* play) {
    u8 sceneId = (play->sceneId == 0x00) ? 0x45 : play->sceneId;
    s16 getItemId = rando_get_item_id(LOCATION_HEART_PIECE);
    u16 objectId = getObjectId(getItemId);
    s16 objectSlot = Object_GetSlot(&play->objectCtx, objectId);

    if (isAP(getItemId)) {
        this->getItemId = getItemId;
        objectStatic = true;
        objectLoaded = true;
    } else if (!objectLoading && !objectLoaded) {
        loadObject(play, &objectSegment, &objectLoadQueue, objectId);
        objectLoading = true;
    } else if (osRecvMesg(&objectLoadQueue, NULL, OS_MESG_NOBLOCK) == 0) {
        objectLoading = false;
        this->getItemId = getItemId;
        objectLoaded = true;
    }
}

RECOMP_PATCH void EnItem00_Draw(Actor* thisx, PlayState* play) {
    s32 pad;
    EnItem00* this = THIS;
    u8 sceneId = (play->sceneId == 0x00) ? 0x45 : play->sceneId;

    if (!(this->unk14E & this->unk150)) {
        switch (this->actor.params) {
            case ITEM00_RUPEE_GREEN:
            case ITEM00_RUPEE_BLUE:
            case ITEM00_RUPEE_RED:
            case ITEM00_RUPEE_HUGE:
            case ITEM00_RUPEE_PURPLE:
                EnItem00_DrawRupee(this, play);
                break;

            case ITEM00_HEART_PIECE:
                Matrix_Scale(20.0f, 20.0f, 20.0f, MTXMODE_APPLY);
                if (objectLoaded) {
                    if (((u32) this->getItemId) != rando_get_item_id(LOCATION_HEART_PIECE)) {
                        objectLoading = false;
                        objectLoaded = false;
                        objectStatic = false;
                        this->getItemId = rando_get_item_id(LOCATION_HEART_PIECE);
                        this->actionFunc = EnItem00_WaitForObject;
                        return;
                    }
                    if (objectStatic) {
                        GetItem_Draw(play, getGid(this->getItemId));
                    } else {
                        GetItem_DrawDynamic(play, objectSegment, getGid(this->getItemId));
                    }
                }
                break;

            case ITEM00_HEART_CONTAINER:
                EnItem00_DrawHeartContainer(this, play);
                break;

            case ITEM00_RECOVERY_HEART:
                if (this->unk152 < 0) {
                    if (this->unk152 == -1) {
                        s8 objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_GI_HEART);

                        if (Object_IsLoaded(&play->objectCtx, objectSlot)) {
                            this->actor.objectSlot = objectSlot;
                            Actor_SetObjectDependency(play, &this->actor);
                            this->unk152 = -2;
                        }
                    } else {
                        Matrix_Scale(16.0f, 16.0f, 16.0f, MTXMODE_APPLY);
                        GetItem_Draw(play, GID_RECOVERY_HEART);
                    }
                    break;
                }
                // fallthrough
            case ITEM00_BOMBS_A:
            case ITEM00_ARROWS_10:
            case ITEM00_ARROWS_30:
            case ITEM00_ARROWS_40:
            case ITEM00_ARROWS_50:
            case ITEM00_BOMBS_B:
            case ITEM00_DEKU_NUTS_1:
            case ITEM00_DEKU_STICK:
            case ITEM00_MAGIC_JAR_BIG:
            case ITEM00_MAGIC_JAR_SMALL:
            case ITEM00_SMALL_KEY:
            case ITEM00_DEKU_NUTS_10:
            case ITEM00_BOMBS_0:
            case ITEM00_BOMBCHU:
                EnItem00_DrawSprite(this, play);
                break;

            case ITEM00_SHIELD_HERO:
                GetItem_Draw(play, GID_SHIELD_HERO);
                break;

            case ITEM00_MAP:
                GetItem_Draw(play, GID_DUNGEON_MAP);
                break;

            case ITEM00_COMPASS:
                GetItem_Draw(play, GID_COMPASS);
                break;

            case ITEM00_MASK:
            case ITEM00_3_HEARTS:
            case ITEM00_FLEXIBLE:
            case ITEM00_NOTHING:
            case ITEM00_BIG_FAIRY:
                break;
        }
    }
}

// TODO: change to return hook
RECOMP_PATCH void EnItem00_Destroy(Actor* thisx, PlayState* play) {
    EnItem00* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
    if (objectSegment != NULL && this->actor.params == ITEM00_HEART_PIECE) {
        ZeldaArena_Free(objectSegment);
        objectSegment = NULL;
    }
}

RECOMP_PATCH void func_800A6780(EnItem00* this, PlayState* play) {
    s32 pad;
    Vec3f pos;
    s32 var1;

    this->unk152++;

    if (this->actor.params == ITEM00_RECOVERY_HEART) {
        if (this->actor.velocity.y < 0.0f) {
            this->actor.speed = 0.0f;
            this->actor.gravity = -0.4f;
            if (this->actor.velocity.y < -1.5f) {
                this->actor.velocity.y = -1.5f;
            }
            this->actor.home.rot.z += (s16)((this->actor.velocity.y + 3.0f) * 1000.0f);
            this->actor.world.pos.x +=
                (Math_CosS(this->actor.yawTowardsPlayer) * (-3.0f * Math_CosS(this->actor.home.rot.z)));
            this->actor.world.pos.z +=
                (Math_SinS(this->actor.yawTowardsPlayer) * (-3.0f * Math_CosS(this->actor.home.rot.z)));
        }
    }

    if (this->actor.params <= ITEM00_RUPEE_RED) {
        this->actor.shape.rot.y += 0x3C0;
    } else if ((this->actor.params >= ITEM00_SHIELD_HERO) && (this->actor.params != ITEM00_DEKU_NUTS_10) &&
               (this->actor.params != ITEM00_BOMBS_0) && (this->actor.params != ITEM00_BOMBCHU)) {
        this->actor.world.rot.x -= 0x2BC;
        this->actor.shape.rot.y += 0x190;
        this->actor.shape.rot.x = this->actor.world.rot.x - 0x4000;
    }

    if (this->actor.velocity.y <= 2.0f) {
        var1 = (u16)this->actor.shape.rot.z + 0x2710;
        if (var1 < 0xFFFF) {
            this->actor.shape.rot.z += 0x2710;
        } else {
            this->actor.shape.rot.z = -1;
        }
    }

    if ((play->gameplayFrames & 1) == 0) {
        pos.x = this->actor.world.pos.x + ((Rand_ZeroOne() - 0.5f) * 10.0f);
        pos.y = this->actor.world.pos.y + ((Rand_ZeroOne() - 0.5f) * 10.0f);
        pos.z = this->actor.world.pos.z + ((Rand_ZeroOne() - 0.5f) * 10.0f);
        EffectSsKirakira_SpawnSmall(play, &pos, &sEffectVelocity, &sEffectAccel, &sEffectPrimColor, &sEffectEnvColor);
    }

    if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH)) {
        this->actionFunc = func_800A640C;
        this->actor.shape.rot.z = 0;
        this->actor.speed = 0.0f;
    }
}

// @ap custom item00 below

bool Item_RandoCollectibleShouldBlink(EnItem00* this) {
    extendedItem00Data = z64recomp_get_extended_actor_data(&this->actor, item00Extension);
    if ((*extendedItem00Data & 0xFFFF00) == 0x17FF00) { // rupee crow drops
        return true;
    }
    return false;
}

// could technically be used for heart pieces as well
void Item_RandoCollectibleDraw(Actor* thisx, PlayState* play) {
    EnItem00* this = THIS;

    extendedItem00Data = z64recomp_get_extended_actor_data(thisx, item00Extension);
    this->getItemId = rando_get_item_id(*extendedItem00Data);
    u16 objectId = getObjectId(this->getItemId);

    if (Item_RandoCollectibleShouldBlink(this) && (this->unk14E & this->unk150)) {
        return;
    }

    Matrix_Scale(20.0f, 20.0f, 20.0f, MTXMODE_APPLY);
    if (isAP(this->getItemId)) {
        GetItem_Draw(play, getGid(this->getItemId));
    // } else if (ObjLoad(play, 0x06, objectId)) {
    //     GetItem_Draw(play, getGid(this->getItemId));
    //     ObjUnload(play, 0x06, objectId);
    } else {
        GetItem_Draw(play, GID_APLOGO_USEFUL);
    }
}

void EnItem00_RandoItemAboveHead(EnItem00* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if (this->unk152 > 0) {
        this->unk152--;
    }

    this->actor.world.pos = player->actor.world.pos;

    this->actor.shape.rot.y += 0x3C0;

    this->actor.world.pos.y += (40.0f + (Math_SinS(this->unk152 * 7500) * (this->unk152 * 0.3f)));

    if (LINK_IS_ADULT) {
        this->actor.world.pos.y += 20.0f;
    }
}

void EnItem00_RandoTextAndFreeze(EnItem00* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    EnItem00_RandoItemAboveHead(this, play);

    extendedItem00Data = z64recomp_get_extended_actor_data(&this->actor, item00Extension);
    u32 locationType = rando_get_location_type(*extendedItem00Data);
    if (locationType != 0 && locationType != 2) {
        player->actor.freezeTimer = 10;
        player->stateFlags1 |= PLAYER_STATE1_20000000;
        
        if (Actor_TextboxIsClosing(&this->actor, play)) {
            player->actor.freezeTimer = 0;
            player->stateFlags1 &= ~PLAYER_STATE1_20000000;
            Actor_Kill(&this->actor);
            CutsceneManager_Stop(CS_ID_GLOBAL_TALK);
            return;
        }

        if (CutsceneManager_GetCurrentCsId() != CS_ID_GLOBAL_TALK) {
            if (CutsceneManager_IsNext(CS_ID_GLOBAL_TALK)) {
                CutsceneManager_Start(CS_ID_GLOBAL_TALK, &this->actor);
            } else {
                CutsceneManager_Queue(CS_ID_GLOBAL_TALK);
            }
        }
    } else {
        if (this->unk152 <= 0) {
            Actor_Kill(&this->actor);
            return;
        }
    }
}

void EnItem00_RandoGive(EnItem00* this, PlayState* play, s32 getItemId, u32 location) {
    Player* player = GET_PLAYER(play);

    rando_send_location(location);

    this->actor.gravity = 0.0f;
    this->actor.velocity.y = 0.0f;
    this->actor.speed = 0.0f;
    this->unk152 = 35;

    u32 locationType = rando_get_location_type(location);
    if (locationType != 0 && locationType != 2) {
        player->actor.freezeTimer = 10;
        player->stateFlags1 |= PLAYER_STATE1_20000000;

        Message_StartTextbox(play, getTextId(getItemId), NULL);
        Audio_PlayFanfare(NA_BGM_GET_ITEM | 0x900);
        CutsceneManager_Queue(CS_ID_GLOBAL_TALK);
    } else {
        Audio_PlayFanfare(NA_BGM_GET_SMALL_ITEM | 0x900);
        // TODO: have notification for junk/useful ap items
        // note: this works, but the image doesn't
        // if (isAP(rando_get_item_id(location))) {
        //     char item_name[33];
        //     char player_name[36];
        //     ItemId item_id = (locationType == 0) ? ITEM_AP_FILLER : ITEM_AP_USEFUL;
        //     item_id = ITEM_AP_PROG;
        //     rando_get_location_item_name(location, item_name);
        //     rando_get_location_item_player(location, player_name);
        //     randoEmitSendNotification(item_name, player_name, item_id, locationType);
        // }
    }

    this->actor.parent = &player->actor;

    this->actionFunc = EnItem00_RandoTextAndFreeze;
}

void Item_RandoCollectibleGround(EnItem00* this, PlayState* play);

// func_800A6650
void Item_RandoCollectibleNoLongerGround(EnItem00* this, PlayState* play) {
    s32 pad;
    Vec3f pos;

    if ((play->gameplayFrames & 1) != 0) {
        pos.x = this->actor.world.pos.x + Rand_CenteredFloat(10.0f);
        pos.y = this->actor.world.pos.y + Rand_CenteredFloat(10.0f);
        pos.z = this->actor.world.pos.z + Rand_CenteredFloat(10.0f);
        EffectSsKirakira_SpawnSmall(play, &pos, &sEffectVelocity, &sEffectAccel, &sEffectPrimColor, &sEffectEnvColor);
    }

    if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH)) {
        if (this->actor.velocity.y > -2.0f) {
            // this->actionFunc = func_800A640C;
            this->actionFunc = Item_RandoCollectibleGround;
        } else {
            this->actor.velocity.y = this->actor.velocity.y * -0.8f;
            this->actor.bgCheckFlags &= ~BGCHECKFLAG_GROUND;
        }
    }
}

// func_800A640C
void Item_RandoCollectibleGround(EnItem00* this, PlayState* play) {
    // heart piece/container rotation/pos
    this->actor.shape.rot.y = this->actor.shape.rot.y + 960;
    this->actor.shape.yOffset = (Math_SinS(this->actor.shape.rot.y) * 150.0f) + 850.0f;

    Math_SmoothStepToF(&this->actor.speed, 0.0f, 1.0f, 0.5f, 0.0f);

    if ((this->actor.gravity != 0.0f) && !(this->actor.bgCheckFlags & BGCHECKFLAG_GROUND)) {
        // this->actionFunc = func_800A6650;
        this->actionFunc = Item_RandoCollectibleNoLongerGround;
    }
}

// func_800A6780
void Item_RandoCollectibleActionFunc(EnItem00* this, PlayState* play) {
    s32 pad;
    Vec3f pos;
    s32 var1;

    // this->unk152++;

    if (this->actor.velocity.y <= 2.0f) {
        var1 = (u16)this->actor.shape.rot.z + 0x2710;
        if (var1 < 0xFFFF) {
            this->actor.shape.rot.z += 0x2710;
        } else {
            this->actor.shape.rot.z = -1;
        }
    }

    if ((play->gameplayFrames & 1) == 0) {
        pos.x = this->actor.world.pos.x + ((Rand_ZeroOne() - 0.5f) * 10.0f);
        pos.y = this->actor.world.pos.y + ((Rand_ZeroOne() - 0.5f) * 10.0f);
        pos.z = this->actor.world.pos.z + ((Rand_ZeroOne() - 0.5f) * 10.0f);
        EffectSsKirakira_SpawnSmall(play, &pos, &sEffectVelocity, &sEffectAccel, &sEffectPrimColor, &sEffectEnvColor);
    }

    if (this->actor.bgCheckFlags & (BGCHECKFLAG_GROUND | BGCHECKFLAG_GROUND_TOUCH)) {
        // this->actionFunc = func_800A640C;
        this->actionFunc = Item_RandoCollectibleGround;
        this->actor.shape.rot.z = 0;
        this->actor.speed = 0.0f;
    }
}

Actor* Item_RandoDropCollectible(PlayState* play, Vec3f* spawnPos, u32 params, u32 location) {
    s32 pad;
    Actor* spawnedActor = NULL;

    // i'm not sure if these params are useful for us
    s32 newParamFF;
    s32 param10000 = params & 0x10000;
    s16 param8000 = params & 0x8000;
    s16 param7F00 = params & 0x7F00;
    s32 param20000 = params & 0x20000;
    s32 paramFF = params & 0xFF;

    spawnedActor = Actor_Spawn(&play->actorCtx, play, ACTOR_EN_ITEM00, spawnPos->x, spawnPos->y, spawnPos->z, 0,
                                0, 0, ITEM00_APITEM);
    extendedItem00Data = z64recomp_get_extended_actor_data(spawnedActor, item00Extension);
    *extendedItem00Data = location;

    spawnedActor->draw = Item_RandoCollectibleDraw;
    ((EnItem00*)spawnedActor)->getItemId = rando_get_item_id(location);

    if ((spawnedActor != NULL)) {
        if (param8000) {
            // wonder item might fall through the world without this check
            spawnedActor->world.pos = GET_PLAYER(play)->actor.world.pos;
            return spawnedActor;
        }
        
        if (param10000 == 0) {
            spawnedActor->velocity.y = 8.0f;
        } else {
            spawnedActor->velocity.y = -2.0f;
        }
        spawnedActor->speed = 2.0f;
        spawnedActor->gravity = -0.9f;
        spawnedActor->world.rot.y = Rand_CenteredFloat(0x10000);
        Actor_SetScale(spawnedActor, 0.02f); // heart piece scale
        // ((EnItem00*)spawnedActor)->actionFunc = func_800A6780;
        ((EnItem00*)spawnedActor)->actionFunc = Item_RandoCollectibleActionFunc;
        ((EnItem00*)spawnedActor)->unk152 = 0xDC;
        // if ((spawnedActor->params != ITEM00_SMALL_KEY) && (spawnedActor->params != ITEM00_HEART_PIECE) &&
        //     (spawnedActor->params != ITEM00_HEART_CONTAINER)) {
        //     spawnedActor->room = -1;
        // }
        spawnedActor->flags |= 0x0010;
        // spawnedActor->flags |= ACTOR_FLAG_100000;
    }

    return spawnedActor;
}