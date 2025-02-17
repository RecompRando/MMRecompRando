#include "modding.h"
#include "global.h"

#include "apcommon.h"

#define CURRENT_TIME ((void)0, gSaveContext.save.time)

#define OSN_STATE_SPECIAL_CONVERSTATION (1 << 0)
#define OSN_STATE_MET_HUMAN (1 << 1)
#define OSN_STATE_MET_DEKU (1 << 2)
#define OSN_STATE_MET_GORON (1 << 3)
#define OSN_STATE_MET_ZORA (1 << 4)
#define OSN_STATE_END_CONVERSATION (1 << 5)

#define OSN_MASK_TEXT_GREAT_FAIRY (1 << 0)
#define OSN_MASK_TEXT_GIBDO (1 << 1)
#define OSN_MASK_TEXT_TRUTH (1 << 2)
#define OSN_MASK_TEXT_GIANT (1 << 3)
#define OSN_MASK_TEXT_KAFEIS (1 << 4)
#define OSN_MASK_TEXT_DON_GERO (1 << 5)
#define OSN_MASK_TEXT_BLAST (1 << 6)
#define OSN_MASK_TEXT_COUPLE (1 << 7)
#define OSN_MASK_TEXT_SCENTS (1 << 8)
#define OSN_MASK_TEXT_KAMARO (1 << 9)
#define OSN_MASK_TEXT_STONE (1 << 10)
#define OSN_MASK_TEXT_POSTMAN (1 << 11)
#define OSN_MASK_TEXT_BUNNY (1 << 12)
#define OSN_MASK_TEXT_CAPTAIN (1 << 13)
#define OSN_MASK_TEXT_BREMEN (1 << 14)
#define OSN_MASK_TEXT_CIRCUS_LEADER (1 << 15)
#define OSN_MASK_TEXT_KEATON (1 << 16)
#define OSN_MASK_TEXT_GARO (1 << 17)
#define OSN_MASK_TEXT_ALL_NIGHT (1 << 18)
#define OSN_MASK_TEXT_ROMANI (1 << 19)

struct EnOsn;

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

#define LOCATION_HEALING 0x040068

#define THIS ((EnOsn*)thisx)

typedef void (*EnOsnActionFunc)(struct EnOsn*, PlayState*);

#define ENOSN_GET_TYPE(this) ((thisx)->params & 3)
#define OSN_STATE_SPECIAL_CONVERSTATION (1 << 0)

typedef enum {
    /* 0 */ OSN_TYPE_CHOOSE, // Will choose between Idle and Cutscene
    /* 1 */ OSN_TYPE_LIE_FACE_DOWN,
    /* 2 */ OSN_TYPE_LIE_FACE_UP,
    /* 3 */ OSN_TYPE_CUTSCENE
} OsnType;

typedef struct EnOsn {
    /* 0x000 */ Actor actor;
    /* 0x144 */ ColliderCylinder collider;
    /* 0x190 */ SkelAnime skelAnime;
    /* 0x1D4 */ EnOsnActionFunc actionFunc;
    /* 0x1D8 */ Vec3s headRot;
    /* 0x1DE */ Vec3s torsoRot;
    /* 0x1E4 */ UNK_TYPE1 unk_1E4[0x6];
    /* 0x1EA */ u16 stateFlags;
    /* 0x1EC */ u8 animIndex;
    /* 0x1ED */ u8 cueId;
    /* 0x1EE */ s16 csId;
    /* 0x1F0 */ u8 shouldRotateHead;
    /* 0x1F1 */ UNK_TYPE1 unk_1F1[0x3];
    /* 0x1F4 */ u16 textId;
    /* 0x1F6 */ s32 maskTextFlags;
    /* 0x1FA */ u8 alpha;
} EnOsn; // size = 0x1FB

typedef enum {
    /*  0 */ OSN_ANIM_IDLE,
    /*  1 */ OSN_ANIM_ARMS_OUT,
    /*  2 */ OSN_ANIM_BOWING,
    /*  3 */ OSN_ANIM_REMINISCE,
    /*  4 */ OSN_ANIM_HANDS_CLASPED,
    /*  5 */ OSN_ANIM_BELIEVE,
    /*  6 */ OSN_ANIM_THINK,
    /*  7 */ OSN_ANIM_SHAKE_HEAD,
    /*  8 */ OSN_ANIM_ORGAN_TALK,
    /*  9 */ OSN_ANIM_ORGAN_PLAY,
    /* 10 */ OSN_ANIM_SHAKE,
    /* 11 */ OSN_ANIM_CHOKE,
    /* 12 */ OSN_ANIM_DESPAIR,
    /* 13 */ OSN_ANIM_FAST_BOWS,
    /* 14 */ OSN_ANIM_HAND_OUT,
    /* 15 */ OSN_ANIM_LYING_DOWN_FACE_UP,
    /* 16 */ OSN_ANIM_LYING_DOWN_FACE_DOWN,
    /* 17 */ OSN_ANIM_MASK_LOOK_AT,
    /* 18 */ OSN_ANIM_TURN_AROUND_START,
    /* 19 */ OSN_ANIM_TURN_AROUND_LOOP,
    /* 20 */ OSN_ANIM_WALK_AWAY,
    /* 21 */ OSN_ANIM_MASK_LOOK_FROM_START,
    /* 22 */ OSN_ANIM_MASK_LOOK_FROM_LOOP,
    /* 23 */ OSN_ANIM_HAND_OUT_2,    // Exact same as OSN_ANIM_HAND_OUT
    /* 24 */ OSN_ANIM_WALK_AWAY_END, // Only the last frame of OSN_ANIM_WALK_AWAY
    /* 25 */ OSN_ANIM_MAX
} OsnAnimation;

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_NONE,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0xF7CFFFFF, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 30, 40, 0, { 0, 0, 0 } },
};

static CollisionCheckInfoInit2 sColChkInfoInit = { 0, 0, 0, 0, MASS_IMMOVABLE };

static DamageTable sDamageTable = {
    /* Deku Nut       */ DMG_ENTRY(0, 0x0),
    /* Deku Stick     */ DMG_ENTRY(0, 0x0),
    /* Horse trample  */ DMG_ENTRY(0, 0x0),
    /* Explosives     */ DMG_ENTRY(0, 0x0),
    /* Zora boomerang */ DMG_ENTRY(0, 0x0),
    /* Normal arrow   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x06   */ DMG_ENTRY(0, 0x0),
    /* Hookshot       */ DMG_ENTRY(0, 0x0),
    /* Goron punch    */ DMG_ENTRY(0, 0x0),
    /* Sword          */ DMG_ENTRY(0, 0x0),
    /* Goron pound    */ DMG_ENTRY(0, 0x0),
    /* Fire arrow     */ DMG_ENTRY(0, 0x0),
    /* Ice arrow      */ DMG_ENTRY(0, 0x0),
    /* Light arrow    */ DMG_ENTRY(0, 0x0),
    /* Goron spikes   */ DMG_ENTRY(0, 0x0),
    /* Deku spin      */ DMG_ENTRY(0, 0x0),
    /* Deku bubble    */ DMG_ENTRY(0, 0x0),
    /* Deku launch    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x12   */ DMG_ENTRY(0, 0x0),
    /* Zora barrier   */ DMG_ENTRY(0, 0x0),
    /* Normal shield  */ DMG_ENTRY(0, 0x0),
    /* Light ray      */ DMG_ENTRY(0, 0x0),
    /* Thrown object  */ DMG_ENTRY(0, 0x0),
    /* Zora punch     */ DMG_ENTRY(0, 0x0),
    /* Spin attack    */ DMG_ENTRY(0, 0x0),
    /* Sword beam     */ DMG_ENTRY(0, 0x0),
    /* Normal Roll    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1B   */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1C   */ DMG_ENTRY(0, 0x0),
    /* Unblockable    */ DMG_ENTRY(0, 0x0),
    /* UNK_DMG_0x1E   */ DMG_ENTRY(0, 0x0),
    /* Powder Keg     */ DMG_ENTRY(0, 0x0),
};

static InitChainEntry sInitChain[] = {
    ICHAIN_U8(attentionRangeType, ATTENTION_RANGE_0, ICHAIN_STOP),
};

extern AnimationInfo sHappyMaskSalesmanAnimationInfo[];
extern FlexSkeletonHeader gHappyMaskSalesmanSkel;
extern AnimationHeader gHappyMaskSalesmanIdleAnim;

s32 Actor_TalkOfferAccepted(Actor* actor, GameState* gameState);

void EnOsn_DoNothing(EnOsn* this, PlayState* play);
void EnOsn_StartCutscene(EnOsn* this, PlayState* play);
s32 EnOsn_GetInitialText(EnOsn* this, PlayState* play);
void EnOsn_Talk(EnOsn* this, PlayState* play);
void EnOsn_InitCutscene(EnOsn* this);
void EnOsn_HandleCutscene(EnOsn* this, PlayState* play);
void EnOsn_ChooseAction(EnOsn* this, PlayState* play);

static bool shouldSetForm = false;
static u32 prevForm;

void EnOsn_Idle(EnOsn* this, PlayState* play);

RECOMP_PATCH void EnOsn_Init(Actor* thisx, PlayState* play) {
    s32 pad;
    EnOsn* this = THIS;

    if (shouldSetForm) {
        gSaveContext.save.playerForm = prevForm;
    }

    Actor_ProcessInitChain(&this->actor, sInitChain);
    ActorShape_Init(&this->actor.shape, 0.0f, ActorShadow_DrawCircle, 20.0f);
    SkelAnime_InitFlex(play, &this->skelAnime, &gHappyMaskSalesmanSkel, &gHappyMaskSalesmanIdleAnim, NULL, NULL, 0);
    Collider_InitCylinder(play, &this->collider);
    Collider_SetCylinder(play, &this->collider, &this->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&this->actor.colChkInfo, &sDamageTable, &sColChkInfoInit);
    this->alpha = 255;

    switch (ENOSN_GET_TYPE(&this->actor)) {
        case OSN_TYPE_CHOOSE:
            if (((gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 2)) ||
                 (gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 3))) ||
                (gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 6))) {
                this->stateFlags |= OSN_STATE_SPECIAL_CONVERSTATION;
            }
            this->shouldRotateHead = true;
            if (play->sceneId == SCENE_INSIDETOWER) {
                if ((gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 2)) ||
                    (gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 6))) {
                    this->actionFunc = EnOsn_HandleCutscene;
                } else if (gSaveContext.save.entrance == ENTRANCE(CLOCK_TOWER_INTERIOR, 3)) {
                    //EnOsn_InitCutscene(this);
                    //this->actionFunc = EnOsn_StartCutscene;
                    if (justDied) {
                        int entrance = ENTRANCE(SOUTH_CLOCK_TOWN, 0);
                        justDied = false;
                        // warp to south clock town
                        play->nextEntrance = entrance;
                        gSaveContext.save.entrance = play->nextEntrance;
                        play->state.running = false;
                        play->state.init = gGameStateOverlayTable[GAMESTATE_PLAY].init;
                        play->state.size = gGameStateOverlayTable[GAMESTATE_PLAY].instanceSize;
                        gSaveContext.save.day = 0;
                        gSaveContext.save.time = CLOCK_TIME(6, 0);
                        Sram_SaveEndOfCycle(gPlay);
                    }
                    this->actionFunc = EnOsn_Idle;
                } else {
                    EnOsn_ChooseAction(this, play);
                }
            } else {
                EnOsn_ChooseAction(this, play);
            }
            break;

        case OSN_TYPE_LIE_FACE_DOWN:
            this->animIndex = OSN_ANIM_LYING_DOWN_FACE_UP;
            Actor_ChangeAnimationByInfo(&this->skelAnime, sHappyMaskSalesmanAnimationInfo, this->animIndex);
            this->actionFunc = EnOsn_DoNothing;
            break;

        case OSN_TYPE_LIE_FACE_UP:
            this->animIndex = OSN_ANIM_LYING_DOWN_FACE_DOWN;
            Actor_ChangeAnimationByInfo(&this->skelAnime, sHappyMaskSalesmanAnimationInfo, this->animIndex);
            this->actionFunc = EnOsn_DoNothing;
            break;

        case OSN_TYPE_CUTSCENE:
            this->actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            this->actionFunc = EnOsn_HandleCutscene;
            break;

        default:
            Actor_Kill(&this->actor);
            break;
    }
}

void EnOsn_GiveRandoItem(EnOsn* this, PlayState* play) {
    if (!rando_location_is_checked(LOCATION_HEALING)) {
        Actor_OfferGetItemHook(&this->actor, play, rando_get_item_id(LOCATION_HEALING), LOCATION_HEALING, 300.0f, 300.0f, true, true);
    } else {
        Actor_OfferGetItem(&this->actor, play, GI_MASK_DEKU, 300.0f, 300.0f);
    }
}

RECOMP_PATCH void EnOsn_Idle(EnOsn* this, PlayState* play) {
    s16 yaw = this->actor.yawTowardsPlayer - this->actor.shape.rot.y;

    //if ((gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] != ITEM_NONE) &&
    //    !CHECK_QUEST_ITEM(QUEST_SONG_HEALING)) {
    if ((gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] != ITEM_NONE) &&
        (!rando_location_is_checked(LOCATION_HEALING) || !rando_location_is_checked(GI_MASK_DEKU))) {
        if ((this->actor.xzDistToPlayer <= 60.0f) && (fabsf(this->actor.playerHeightRel) <= fabsf(80.0f))) {
            //shouldSetForm = true;
            //prevForm = gSaveContext.save.playerForm;
            //this->actionFunc = EnOsn_StartCutscene;
            EnOsn_GiveRandoItem(this, play);
        }
    }/* else if (Actor_TalkOfferAccepted(&this->actor, &play->state)) {
        this->textId = EnOsn_GetInitialText(this, play);
        Message_StartTextbox(play, this->textId, &this->actor);
        this->actionFunc = EnOsn_Talk;
    } else if (((this->actor.xzDistToPlayer < 100.0f) || this->actor.isLockedOn) && (yaw < 0x4000) && (yaw > -0x4000)) {
        Actor_OfferTalkNearColChkInfoCylinder(&this->actor, play);
    }*/
}

s32 EnOsn_GetInitialMaskText(EnOsn* this, PlayState* play);

RECOMP_PATCH s32 EnOsn_GetInitialText(EnOsn* this, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] != ITEM_NONE) &&
        CHECK_QUEST_ITEM(QUEST_SONG_HEALING)) {
        if (this->stateFlags & OSN_STATE_SPECIAL_CONVERSTATION) {
            this->stateFlags |= OSN_STATE_END_CONVERSATION;
            if ((gSaveContext.save.saveInfo.inventory.items[SLOT_OCARINA] != ITEM_NONE) &&
                (INV_CONTENT(ITEM_MASK_DEKU) == ITEM_MASK_DEKU)) {
                if ((gSaveContext.save.day == 3) && (CURRENT_TIME >= CLOCK_TIME(5, 0)) &&
                    (CURRENT_TIME < CLOCK_TIME(6, 0))) {
                    return 0x2006;
                }
                return 0x1FCD;
            }
            return 0x1FAF;
        }

        if (player->transformation == PLAYER_FORM_DEKU) {
            if (this->stateFlags & OSN_STATE_MET_DEKU) {
                this->stateFlags |= OSN_STATE_END_CONVERSATION;
                if ((gSaveContext.save.day == 3) && (CURRENT_TIME >= CLOCK_TIME(5, 0)) &&
                    (CURRENT_TIME < CLOCK_TIME(6, 0))) {
                    return 0x2006;
                }
                return 0x1FCD;
            }
            this->stateFlags |= OSN_STATE_MET_DEKU;
            return 0x1FC8;
        }

        if (player->transformation == PLAYER_FORM_GORON) {
            if (this->stateFlags & OSN_STATE_MET_GORON) {
                this->stateFlags |= OSN_STATE_END_CONVERSATION;
                if ((gSaveContext.save.day == 3) && (CURRENT_TIME >= CLOCK_TIME(5, 0)) &&
                    (CURRENT_TIME < CLOCK_TIME(6, 0))) {
                    return 0x2006;
                } else {
                    return 0x1FCD;
                }
            }
            this->stateFlags |= OSN_STATE_MET_GORON;
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_76_20)) {
                return 0x1FC8;
            } else {
                SET_WEEKEVENTREG(WEEKEVENTREG_76_20);
                return 0x1FCE;
            }
        }

        if (player->transformation == PLAYER_FORM_ZORA) {
            if (this->stateFlags & OSN_STATE_MET_ZORA) {
                this->stateFlags |= OSN_STATE_END_CONVERSATION;
                if ((gSaveContext.save.day == 3) && (CURRENT_TIME >= CLOCK_TIME(5, 0)) &&
                    (CURRENT_TIME < CLOCK_TIME(6, 0))) {
                    return 0x2006;
                }
                return 0x1FCD;
            }

            this->stateFlags |= OSN_STATE_MET_ZORA;
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_76_40)) {
                return 0x1FC8;
            }
            SET_WEEKEVENTREG(WEEKEVENTREG_76_40);
            return 0x1FD0;
        }

        if (Player_GetMask(play) == PLAYER_MASK_NONE) {
            if (this->stateFlags & OSN_STATE_MET_HUMAN) {
                this->stateFlags |= OSN_STATE_END_CONVERSATION;
                if ((gSaveContext.save.day == 3) && (CURRENT_TIME >= CLOCK_TIME(5, 0)) &&
                    (CURRENT_TIME < CLOCK_TIME(6, 0))) {
                    return 0x2006;
                }
                return 0x1FCD;
            }
            this->stateFlags |= OSN_STATE_MET_HUMAN;
            return 0x1FC8;
        }

        return EnOsn_GetInitialMaskText(this, play);
    }

    this->stateFlags |= OSN_STATE_END_CONVERSATION;
    if ((gSaveContext.save.day == 3) && (CURRENT_TIME >= CLOCK_TIME(5, 0)) && (CURRENT_TIME < CLOCK_TIME(6, 0))) {
        return 0x2004;
    }

    return 0x1FAE;
}
/*
void EnOsn_Destroy(Actor* thisx, PlayState* play) {
    EnOsn* this = THIS;

    shouldSetForm = false;

    SkelAnime_Free(&this->skelAnime, play);
    Collider_DestroyCylinder(play, &this->collider);
}*/