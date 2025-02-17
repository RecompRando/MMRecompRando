#include "modding.h"
#include "global.h"
#include "z64snap.h"

#include "apcommon.h"

#define OBJECT_MNK_1_LIMB_MAX 0x05
#define OBJECT_MNK_2_LIMB_MAX 0x17
#define OBJECT_MNK_3_LIMB_MAX 0x04

struct EnMnk;

typedef void (*EnMnkActionFunc)(struct EnMnk*, PlayState*);
typedef void (*EnMnkFunc)(PlayState*, struct EnMnk*);

#define FLAGS (ACTOR_FLAG_ATTENTION_ENABLED | ACTOR_FLAG_FRIENDLY | ACTOR_FLAG_UPDATE_CULLING_DISABLED)

#define THIS ((EnMnk*)thisx)

#define MONKEY_GET_TYPE(thisx) (((thisx)->params & 0x780) >> 7)
#define MONKEY_GET_SWITCH_FLAG(thisx) ((thisx)->params & 0x7F)
#define MONKEY_GET_PATH_INDEX(thisx) (((thisx)->params & 0xF800) >> 11)

#define MONKEY_SWITCH_FLAG_NONE 0x7F
#define MONKEY_PATH_INDEX_NONE 0x1F

typedef enum {
    /*  0 */ MONKEY_0,
    /*  1 */ MONKEY_OUTSIDEWOODS,
    /*  2 */ MONKEY_BY_WITCH,
    /*  3 */ MONKEY_OUTSIDECHAMBER,
    /*  4 */ MONKEY_OUTSIDEPALACE,
    /*  5 */ MONKEY_TIED_UP,
    /*  6 */ MONKEY_HANGING,
    /*  7 */ MONKEY_WOODS_GUIDE,
    /*  8 */ MONKEY_8,
    /*  9 */ MONKEY_AFTER_SAVED,
    /* 10 */ MONKEY_10
} MonkeyType;

#define MONKEY_FLAGS_1 (1 << 0)
#define MONKEY_FLAGS_2 (1 << 1)
#define MONKEY_FLAGS_4 (1 << 2)
#define MONKEY_FLAGS_8 (1 << 3)
#define MONKEY_FLAGS_10 (1 << 4)
#define MONKEY_FLAGS_20 (1 << 5)
#define MONKEY_FLAGS_40 (1 << 6)
#define MONKEY_FLAGS_80 (1 << 7)
#define MONKEY_FLAGS_100 (1 << 8)
#define MONKEY_FLAGS_200 (1 << 9)
#define MONKEY_FLAGS_400 (1 << 10)
#define MONKEY_FLAGS_800 (1 << 11)
#define MONKEY_FLAGS_1000 (1 << 12)
#define MONKEY_FLAGS_2000 (1 << 13)
#define MONKEY_FLAGS_4000 (1 << 14)
#define MONKEY_FLAGS_8000 (1 << 15)

#define MONKEY_PROP_LIMB_MAX MAX((s32)OBJECT_MNK_1_LIMB_MAX, (s32)OBJECT_MNK_3_LIMB_MAX)

typedef struct EnMnk {
    /* 0x000 */ PictoActor picto;
    /* 0x148 */ SkelAnime skelAnime;
    /* 0x18C */ Vec3s jointTable[OBJECT_MNK_2_LIMB_MAX];
    /* 0x216 */ Vec3s morphTable[OBJECT_MNK_2_LIMB_MAX];
    /* 0x2A0 */ SkelAnime propSkelAnime;
    /* 0x2E4 */ Vec3s propJointTable[MONKEY_PROP_LIMB_MAX];
    /* 0x302 */ Vec3s propMorphTable[MONKEY_PROP_LIMB_MAX];
    /* 0x320 */ ColliderCylinder collider;
    /* 0x36C */ MtxF unk_36C;
    /* 0x3AC */ Path* path;
    /* 0x3B0 */ s16 csIdList[6];
    /* 0x3BC */ s16 csId;
    /* 0x3BE */ s16 animIndex;
    /* 0x3C0 */ s32 destPointIndex;
    /* 0x3C4 */ s32 pathIndex;
    /* 0x3C8 */ s32 unk_3C8;
    /* 0x3CC */ s16 unk_3CC; // beard x rot?
    /* 0x3CE */ s16 unk_3CE; // beard y rot?
    /* 0x3D0 */ union {
                    s16 cueId; // Tied monkey
                    s16 unk_3D0; // Hanging monkey
                };
    /* 0x3D4 */ u32 unk_3D4;
    /* 0x3D8 */ f32 approachPlayerRadius;
    /* 0x3DC */ s16 blinkFrame;
    /* 0x3DE */ s16 blinkTimer;
    /* 0x3E0 */ s32 unk_3E0;
    /* 0x3E4 */ u16 flags;
    /* 0x3E8 */ EnMnkActionFunc actionFunc;
} EnMnk; // size = 0x3EC

static ColliderCylinderInit sCylinderInit = {
    {
        COL_MATERIAL_NONE,
        AT_NONE,
        AC_ON | AC_TYPE_PLAYER,
        OC1_ON | OC1_TYPE_ALL,
        OC2_TYPE_1,
        COLSHAPE_CYLINDER,
    },
    {
        ELEM_MATERIAL_UNK0,
        { 0x00000000, 0x00, 0x00 },
        { 0x01000200, 0x00, 0x00 },
        ATELEM_NONE | ATELEM_SFX_NORMAL,
        ACELEM_ON,
        OCELEM_ON,
    },
    { 15, 30, 0, { 0, 0, 0 } },
};

typedef enum {
    /* 0 */ MONKEY_TIEDUP_ANIM_NONE = -1,
    /* 0 */ MONKEY_TIEDUP_ANIM_KICKAROUND,
    /* 1 */ MONKEY_TIEDUP_ANIM_KICKUPANDDOWN,
    /* 2 */ MONKEY_TIEDUP_ANIM_SHH,
    /* 3 */ MONKEY_TIEDUP_ANIM_SING,
    /* 4 */ MONKEY_TIEDUP_ANIM_SMALLCLAPFEET,
    /* 5 */ MONKEY_TIEDUP_ANIM_LARGECLAPFEET,
    /* 6 */ MONKEY_TIEDUP_ANIM_SHAKEHEAD,
    /* 7 */ MONKEY_TIEDUP_ANIM_WAIT,
    /* 8 */ MONKEY_TIEDUP_ANIM_CONFUSED
} MonkeyTiedUpAnim;

typedef enum {
    /* 0 */ MONKEY_HANGING_ANIM_FREEZE,
    /* 1 */ MONKEY_HANGING_ANIM_RISE,
    /* 2 */ MONKEY_HANGING_ANIM_STRUGGLE
} MonkeyHangingAnim;

typedef enum {
    /* 0 */ MONKEY_FOLLOWPATH_RESULT_FOLLOWING,
    /* 1 */ MONKEY_FOLLOWPATH_RESULT_SHORTRANGE_REACHEDPOINT,
    /* 2 */ MONKEY_FOLLOWPATH_RESULT_SHORTRANGE_FINISHEDPATH,
    /* 3 */ MONKEY_FOLLOWPATH_RESULT_LONGRANGE_REACHEDPOINT,
    /* 4 */ MONKEY_FOLLOWPATH_RESULT_LONGRANGE_FINISHEDPATH
} MonkeyFollowPathResult;

extern FlexSkeletonHeader gMonkeySkel;
extern AnimationHeader object_mnk_Anim_0105DC;

void EnMnk_Monkey_SetupDrop(EnMnk* this);
void EnMnk_Monkey_Run(EnMnk* this, PlayState* play);
void EnMnk_Monkey_SetupRunAfterTalk(EnMnk* this, PlayState* play);
void EnMnk_StartNextPath(EnMnk* this, PlayState* play);
void EnMnk_Monkey_ChangeAnim(EnMnk* this, s32 animIndex, u8 animMode, f32 morphFrames);
void EnMnk_Monkey_SetAnim(EnMnk* this, s32 animIndex);
s32 EnMnk_FollowPath(EnMnk* this, u16 flags);
void EnMnk_SpawnWaterEffects(EnMnk* this, PlayState* play);
void EnMnk_PlayWaterWalkSfx(EnMnk* this);
void EnMnk_PlayWalkSfx(EnMnk* this);
void EnMnk_Monkey_SetupWaitToRunAndWaitAtEachPoint(EnMnk* this, PlayState* play);
void EnMnk_Monkey_StartInvisible(EnMnk* this, PlayState* play);
void EnMnk_MonkeyTiedUp_Init(Actor* thisx, PlayState* play);
void EnMnk_MonkeyHanging_Init(Actor* thisx, PlayState* play);

void EnMnk_Monkey_WaitToRunAndWaitAtEachPoint(EnMnk* this, PlayState* play);
void EnMnk_MonkeyTiedUp_Wait(EnMnk* this, PlayState* play);
void EnMnk_MonkeyHanging_StruggleBeforeDunk(EnMnk* this, PlayState* play);
void EnMnk_Monkey_WaitToTalkAfterRun(EnMnk* this, PlayState* play);
void EnMnk_Monkey_WaitForPlayerApproach(EnMnk* this, PlayState* play);
void EnMnk_Monkey_WaitToFollowPath(EnMnk* this, PlayState* play);
void EnMnk_MonkeyTiedUp_WaitUnused(EnMnk* this, PlayState* play);
void EnMnk_MonkeyTiedUp_WaitForInstrument(EnMnk* this, PlayState* play);
void EnMnk_MonkeyTiedUp_WaitForCutRope(EnMnk* this, PlayState* play);
void EnMnk_MonkeyHanging_WaitAfterDunk(EnMnk* this, PlayState* play);
void EnMnk_Monkey_WaitToTalkAfterSaved(EnMnk* this, PlayState* play);

void EnMnk_DoNothing(EnMnk* this, PlayState* play);
void EnMnk_Monkey_WaitOutsideWoods(EnMnk* this, PlayState* play);
void EnMnk_Monkey_WaitOutsideChamber(EnMnk* this, PlayState* play);
void EnMnk_Monkey_WaitToGuideThroughWoods(EnMnk* this, PlayState* play);
void func_80AB9084(EnMnk* this, PlayState* play);
void func_80AB92CC(EnMnk* this, PlayState* play);

s32 EnMnk_ValidatePictograph(PlayState* play, Actor* thisx);
s32 EnMnk_AlreadyExists(EnMnk* this, PlayState* play);

bool monkeySongChecked;
bool monkeyOffered;

void EnMnk_MonkeyTiedUp_OfferAPItem(EnMnk* this, PlayState* play) {
    if (monkeySongChecked || (monkeyOffered && Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING)) {
        play->nextEntrance = ENTRANCE(DEKU_PALACE, 1);
        play->transitionType = TRANS_TYPE_FADE_WHITE_FAST;
        gSaveContext.nextTransitionType = TRANS_TYPE_FADE_WHITE_FAST;
        play->transitionTrigger = TRANS_TRIGGER_START;
    } else if (Actor_HasParent(&this->picto.actor, play)) {
        play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;
        SET_WEEKEVENTREG(WEEKEVENTREG_09_80);
    } else if (play->msgCtx.ocarinaMode == OCARINA_MODE_END) {
        Actor_OfferGetItemHook(&this->picto.actor, play, rando_get_item_id(0x040061), 0x040061, 1000.0f, 1000.0f, true, true);
        monkeyOffered = true;
    } else {
        this->picto.actor.textId = 0;
        CutsceneManager_Stop(this->csId);
        this->picto.actor.csId = this->csIdList[0];
        this->csId = CS_ID_NONE;
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_OFF);
        play->msgCtx.ocarinaMode = OCARINA_MODE_END;
        Message_CloseTextbox(play);
        monkeySongChecked = rando_location_is_checked(0x040061);
    }
}

extern FlexSkeletonHeader gMonkeyTiedUpPoleSkel;
extern AnimationHeader object_mnk_Anim_003584;

void EnMnk_MonkeyTiedUp_ChangeAnim(EnMnk* this, s32 animIndex, u8 animMode, f32 morphFrames);
void EnMnk_MonkeyTiedUp_Draw(Actor* thisx, PlayState* play);

void EnMnk_MonkeyTiedUp_SetAnim(EnMnk* this, s32 animIndex);
s32 EnMnk_PlayerIsInTalkRange(EnMnk* this, PlayState* play);

void EnMnk_MonkeyTiedUp_TransitionAfterTalk(EnMnk* this, PlayState* play);
void EnMnk_MonkeyTiedUp_ReactToWrongInstrument(EnMnk* this, PlayState* play);

s32 Actor_TalkOfferAccepted(Actor* this, GameState* play);

RECOMP_PATCH void EnMnk_MonkeyTiedUp_Wait(EnMnk* this, PlayState* play) {
    s32 pad;

    SkelAnime_Update(&this->skelAnime);
    if (SkelAnime_Update(&this->propSkelAnime)) {
        this->cueId--;
        if (this->cueId < 0) {
            this->cueId = 4;
            EnMnk_MonkeyTiedUp_ChangeAnim(this, MONKEY_TIEDUP_ANIM_KICKAROUND, ANIMMODE_ONCE, -5.0f);
        } else if (this->cueId == 0) {
            EnMnk_MonkeyTiedUp_ChangeAnim(this, MONKEY_TIEDUP_ANIM_SHAKEHEAD, ANIMMODE_ONCE, -5.0f);
        } else {
            EnMnk_MonkeyTiedUp_ChangeAnim(this, MONKEY_TIEDUP_ANIM_KICKAROUND, ANIMMODE_ONCE, 0.0f);
            SkelAnime_Update(&this->skelAnime);
            SkelAnime_Update(&this->propSkelAnime);
        }
    }
}

RECOMP_PATCH void EnMnk_MonkeyTiedUp_WaitForInstrument(EnMnk* this, PlayState* play) {
    SkelAnime_Update(&this->skelAnime);
    SkelAnime_Update(&this->propSkelAnime);

    if (Actor_OcarinaInteractionAccepted(&this->picto.actor, &play->state)) {
        switch (gSaveContext.save.playerForm) {
            case PLAYER_FORM_HUMAN:
            case PLAYER_FORM_FIERCE_DEITY:
                this->picto.actor.textId = 0x8D4;
                EnMnk_MonkeyTiedUp_SetAnim(this, MONKEY_TIEDUP_ANIM_SHAKEHEAD);
                break;

            case PLAYER_FORM_GORON:
                this->picto.actor.textId = 0x8D5;
                EnMnk_MonkeyTiedUp_SetAnim(this, MONKEY_TIEDUP_ANIM_SHAKEHEAD);
                break;

            case PLAYER_FORM_ZORA:
                this->picto.actor.textId = 0x8D6;
                EnMnk_MonkeyTiedUp_SetAnim(this, MONKEY_TIEDUP_ANIM_SHAKEHEAD);
                break;

            case PLAYER_FORM_DEKU:
                monkeySongChecked = false;
                monkeyOffered = false;
                this->actionFunc = EnMnk_MonkeyTiedUp_OfferAPItem;
                return;

            default:
                this->picto.actor.textId = 0x8D4;
                EnMnk_MonkeyTiedUp_SetAnim(this, MONKEY_TIEDUP_ANIM_SHAKEHEAD);
                break;
        }
        this->actionFunc = EnMnk_MonkeyTiedUp_ReactToWrongInstrument;
        this->picto.actor.csId = this->csIdList[0];
        Message_StartTextbox(play, this->picto.actor.textId, NULL);
        CutsceneManager_Queue(this->picto.actor.csId);
    } else if (Actor_TalkOfferAccepted(&this->picto.actor, &play->state)) {
        this->actionFunc = EnMnk_MonkeyTiedUp_TransitionAfterTalk;
        EnMnk_MonkeyTiedUp_SetAnim(this, MONKEY_TIEDUP_ANIM_KICKAROUND);
    } else if (EnMnk_PlayerIsInTalkRange(this, play)) {
        this->picto.actor.textId = 0x8D3;
        Actor_OfferTalk(&this->picto.actor, play, 100.0f);
        Actor_OfferOcarinaInteraction(&this->picto.actor, play, 100.0f, 100.0f);
    }
}

RECOMP_PATCH void EnMnk_MonkeyTiedUp_TransitionAfterTalk(EnMnk* this, PlayState* play) {
    s32 pad;

    SkelAnime_Update(&this->skelAnime);
    SkelAnime_Update(&this->propSkelAnime);
    if (play->msgCtx.currentTextId == 0x8CD) {
        if (this->skelAnime.curFrame <= 17.0f) {
            this->unk_3E0 = 0;
        } else {
            this->unk_3E0 = 2;
        }
    } else if (play->msgCtx.msgBufPos == 0 && play->msgCtx.currentTextId == 0x8D3) {
        play->msgCtx.font.msgBuf.schar[0x90] = 0xBF;
    } else if (play->msgCtx.msgBufPos == 0x90 && play->msgCtx.currentTextId == 0x8D3 && Message_GetState(&play->msgCtx) == TEXT_STATE_CLOSING) {
        Message_CloseTextbox(play);
        this->actionFunc = EnMnk_MonkeyTiedUp_WaitForInstrument;
        EnMnk_MonkeyTiedUp_SetAnim(this, MONKEY_TIEDUP_ANIM_WAIT);
    }
}

RECOMP_PATCH void EnMnk_MonkeyTiedUp_Init(Actor* thisx, PlayState* play) {
    EnMnk* this = THIS;
    s16 csId;
    s32 i;

    this->actionFunc = EnMnk_MonkeyTiedUp_WaitForInstrument;
    this->picto.actor.flags |= ACTOR_FLAG_UPDATE_DURING_OCARINA;
    SkelAnime_InitFlex(play, &this->propSkelAnime, &gMonkeyTiedUpPoleSkel, &object_mnk_Anim_003584,
                       this->propJointTable, this->propMorphTable, OBJECT_MNK_1_LIMB_MAX);
    this->cueId = 4;
    this->animIndex = MONKEY_TIEDUP_ANIM_NONE;
    EnMnk_MonkeyTiedUp_ChangeAnim(this, MONKEY_TIEDUP_ANIM_WAIT, ANIMMODE_LOOP, -5.0f);
    this->picto.actor.draw = EnMnk_MonkeyTiedUp_Draw;
    this->picto.actor.shape.shadowDraw = NULL;
    this->flags |= MONKEY_FLAGS_1 | MONKEY_FLAGS_2 | MONKEY_FLAGS_20 | MONKEY_FLAGS_200;
    csId = this->picto.actor.csId;

    for (i = 0; i < ARRAY_COUNT(this->csIdList); i++) {
        this->csIdList[i] = csId;
        if (csId != CS_ID_NONE) {
            this->picto.actor.csId = csId;
            csId = CutsceneManager_GetAdditionalCsId(this->picto.actor.csId);
        }
    }

    this->csId = CS_ID_NONE;
    this->picto.actor.csId = this->csIdList[0];
}

RECOMP_PATCH void EnMnk_Init(Actor* thisx, PlayState* play) {
    EnMnk* this = THIS;
    s32 pad;

    Actor_SetScale(&this->picto.actor, 0.012f);
    ActorShape_Init(&this->picto.actor.shape, 0.0f, ActorShadow_DrawCircle, 12.0f);
    this->actionFunc = EnMnk_DoNothing;
    SkelAnime_InitFlex(play, &this->skelAnime, &gMonkeySkel, &object_mnk_Anim_0105DC, this->jointTable,
                       this->morphTable, OBJECT_MNK_2_LIMB_MAX);
    Animation_PlayLoop(&this->skelAnime, &object_mnk_Anim_0105DC);
    this->flags = 0;
    this->unk_3D4 = 0;
    this->unk_3E0 = 0;
    this->unk_3CC = 0;
    this->unk_3CE = 0;
    this->cueId = 0;
    this->picto.actor.colChkInfo.mass = MASS_IMMOVABLE;
    this->picto.actor.colChkInfo.cylRadius = 40;
    this->picto.actor.velocity.y = -9.0f;
    this->picto.actor.terminalVelocity = -9.0f;
    this->picto.actor.gravity = -1.0f;
    Collider_InitAndSetCylinder(play, &this->collider, &this->picto.actor, &sCylinderInit);
    Collider_UpdateCylinder(&this->picto.actor, &this->collider);
    this->pathIndex = MONKEY_GET_PATH_INDEX(thisx);
    this->approachPlayerRadius = 0.0f;
    if (this->pathIndex == MONKEY_PATH_INDEX_NONE) {
        this->pathIndex = PATH_INDEX_NONE;
    }

    switch (MONKEY_GET_TYPE(thisx)) {
        case MONKEY_0:
            if (!Flags_GetSwitch(play, MONKEY_GET_SWITCH_FLAG(&this->picto.actor)) ||
                Flags_GetSwitch(play, MONKEY_GET_SWITCH_FLAG(&this->picto.actor) + 1) ||
                CHECK_WEEKEVENTREG(WEEKEVENTREG_09_80)) {
                Actor_Kill(&this->picto.actor);
                return;
            }
            break;

        case MONKEY_OUTSIDEWOODS:
            Actor_Kill(thisx);
            return;

        case MONKEY_OUTSIDEPALACE:
            if (Flags_GetSwitch(play, MONKEY_GET_SWITCH_FLAG(&this->picto.actor))) {
                Actor_Kill(thisx);
                return;
            }
            break;

        case MONKEY_OUTSIDECHAMBER:
            if (Flags_GetSwitch(play, MONKEY_GET_SWITCH_FLAG(&this->picto.actor)) ||
                CHECK_WEEKEVENTREG(WEEKEVENTREG_29_80)) {
                Actor_Kill(thisx);
                return;
            }
            break;

        case MONKEY_TIED_UP:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_09_80) || CHECK_WEEKEVENTREG(WEEKEVENTREG_23_20)) {
                Actor_Kill(thisx);
                return;
            }
            break;

        case MONKEY_HANGING:
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_09_80) || CHECK_WEEKEVENTREG(WEEKEVENTREG_23_20)) {
                Actor_Kill(thisx);
                return;
            }
            break;

        case MONKEY_8:
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_23_20) || CHECK_WEEKEVENTREG(WEEKEVENTREG_29_80)) {
                Actor_Kill(thisx);
                return;
            }
            break;

        case MONKEY_AFTER_SAVED:
            if (!CHECK_WEEKEVENTREG(WEEKEVENTREG_29_80)) {
                Actor_Kill(thisx);
                return;
            }
            break;

        case MONKEY_BY_WITCH:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_79_02) || CHECK_WEEKEVENTREG(WEEKEVENTREG_SAVED_KOUME) ||
                Flags_GetSwitch(play, MONKEY_GET_SWITCH_FLAG(&this->picto.actor))) {
                Actor_Kill(thisx);
                return;
            }
            break;

        case MONKEY_WOODS_GUIDE:
            if (CHECK_WEEKEVENTREG(WEEKEVENTREG_79_02) || CHECK_WEEKEVENTREG(WEEKEVENTREG_SAVED_KOUME)) {
                Actor_Kill(thisx);
                return;
            }
            if (EnMnk_AlreadyExists(this, play)) {
                Actor_Kill(thisx);
                return;
            }
            break;

        default:
            break;
    }

    switch (MONKEY_GET_TYPE(thisx)) {
        case MONKEY_0:
            EnMnk_Monkey_SetupWaitToRunAndWaitAtEachPoint(this, play);
            break;

        case MONKEY_OUTSIDEWOODS:
            this->actionFunc = EnMnk_Monkey_WaitOutsideWoods;
            this->picto.actor.textId = 0x7D0;
            CLEAR_EVENTINF(EVENTINF_25);
            CLEAR_EVENTINF(EVENTINF_26);
            EnMnk_Monkey_StartInvisible(this, play);
            break;

        case MONKEY_OUTSIDECHAMBER:
            this->picto.actor.textId = 0x7D3;
            this->actionFunc = EnMnk_Monkey_WaitOutsideChamber;
            EnMnk_Monkey_StartInvisible(this, play);
            this->collider.dim.radius = 120;
            break;

        case MONKEY_BY_WITCH:
            EnMnk_Monkey_SetAnim(this, 0);
            this->actionFunc = EnMnk_Monkey_WaitToFollowPath;
            this->unk_3C8 = 0;
            this->flags |= MONKEY_FLAGS_2;
            this->picto.actor.flags &= ~ACTOR_FLAG_ATTENTION_ENABLED;
            this->picto.actor.velocity.y = 0.0f;
            this->picto.actor.terminalVelocity = 0.0f;
            this->picto.actor.gravity = 0.0f;
            break;

        case MONKEY_OUTSIDEPALACE:
            EnMnk_Monkey_SetAnim(this, 0);
            this->actionFunc = EnMnk_Monkey_WaitToFollowPath;
            this->unk_3C8 = 0;
            break;

        case MONKEY_TIED_UP:
            EnMnk_MonkeyTiedUp_Init(thisx, play);
            break;

        case MONKEY_HANGING:
            EnMnk_MonkeyHanging_Init(thisx, play);
            break;

        case MONKEY_WOODS_GUIDE:
            this->picto.actor.room = -1;
            EnMnk_Monkey_SetAnim(this, 0);
            this->actionFunc = EnMnk_Monkey_WaitToGuideThroughWoods;
            this->unk_3C8 = 0;
            this->destPointIndex = 0;

            switch (this->picto.actor.home.rot.x) {
                case 1:
                    this->picto.actor.textId = 0x7DD;
                    break;

                case 2:
                    this->picto.actor.textId = 0x7DE;
                    break;

                default:
                    this->picto.actor.textId = 0x7DC;
                    break;
            }
            break;

        case MONKEY_8:
            this->actionFunc = func_80AB9084;
            this->cueId = 0;
            break;

        case MONKEY_AFTER_SAVED:
            EnMnk_Monkey_SetAnim(this, 0);
            this->unk_3C8 = 0;
            this->actionFunc = EnMnk_Monkey_WaitToTalkAfterSaved;
            this->picto.actor.textId = 0x8E5;
            break;

        case MONKEY_10:
            this->actionFunc = func_80AB92CC;
            this->cueId = 0;
            break;

        default:
            this->actionFunc = EnMnk_DoNothing;
            break;
    }

    this->picto.actor.world.rot.x = this->picto.actor.world.rot.z = 0;
    this->picto.actor.shape.rot.x = this->picto.actor.shape.rot.z = 0;
    this->picto.validationFunc = EnMnk_ValidatePictograph;
}

RECOMP_PATCH void EnMnk_Destroy(Actor* thisx, PlayState* play) {
    EnMnk* this = THIS;

    Collider_DestroyCylinder(play, &this->collider);
    if ((MONKEY_GET_TYPE(&this->picto.actor) == MONKEY_TIED_UP) && (this->flags & MONKEY_FLAGS_2000)) {
        //Item_Give(play, ITEM_SONG_SONATA);
        //recomp_send_location(0x040061);
        CLEAR_EVENTINF(EVENTINF_24);
    }
}