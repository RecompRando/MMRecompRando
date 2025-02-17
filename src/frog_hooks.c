// #include "modding.h"
// #include "global.h"

// #include "apcommon.h"

// struct EnMinifrog;

// typedef void (*EnMinifrogActionFunc)(struct EnMinifrog*, PlayState*);

// #define EN_FROG_IS_RETURNED(thisx) ((((thisx)->params) & 0xF0) >> 4)

// typedef enum {
//     /* 0x00 */ FROG_YELLOW, // Mountain Village
//     /* 0x01 */ FROG_CYAN, // Woodfall Temple
//     /* 0x02 */ FROG_PINK, // Great Bay Temple
//     /* 0x03 */ FROG_BLUE, // Southern Swamp
//     /* 0x04 */ FROG_WHITE, // Laundry Pool
//     /* 0xFF */ FROG_NONE = 0xFF // -1
// } FrogType;

// typedef enum {
//     /* 0 */ FROG_STATE_JUMP,
//     /* 1 */ FROG_STATE_AIR,
//     /* 2 */ FROG_STATE_GROUND
// } FrogJumpState;

// #define FROG_LIMB_MAX 0x18

// typedef struct EnMinifrog {
//     /* 0x000 */ Actor actor;
//     /* 0x144 */ SkelAnime skelAnime;
//     /* 0x188 */ Vec3s jointTable[FROG_LIMB_MAX];
//     /* 0x218 */ Vec3s morphTable[FROG_LIMB_MAX];
//     /* 0x2A8 */ EnMinifrogActionFunc actionFunc;
//     /* 0x2AC */ struct EnMinifrog* frog;
//     /* 0x2B0 */ s16 frogIndex;
//     /* 0x2B2 */ s16 jumpState;
//     /* 0x2B4 */ s16 timer;
//     /* 0x2B6 */ u16 flags;
//     /* 0x2B8 */ ColliderCylinder collider;
// } EnMinifrog; // size = 0x304

// void EnMinifrog_YellowFrogDialog(EnMinifrog* this, PlayState* play) {
//     EnMinifrog_TurnToPlayer(this);
//     EnMinifrog_Jump(this);
//     switch (Message_GetState(&play->msgCtx)) {
//         case TEXT_STATE_CHOICE:
//             if (Message_ShouldAdvance(play)) {
//                 switch (play->msgCtx.choiceIndex) {
//                     case 0: // Yes
//                         Audio_PlaySfx_MessageDecide();
//                         this->actionFunc = EnMinifrog_BeginChoirCutscene;
//                         play->msgCtx.msgLength = 0;
//                         break;

//                     case 1: // No
//                         Audio_PlaySfx_MessageCancel();
//                         Message_ContinueTextbox(play, 0xD7E);
//                         break;

//                     default:
//                         break;
//                 }
//             }
//             break;

//         case TEXT_STATE_EVENT:
//             if (Message_ShouldAdvance(play)) {
//                 EnMinifrog_SetJumpState(this);
//                 switch (play->msgCtx.currentTextId) {
//                     case 0xD76:
//                         Message_ContinueTextbox(play, play->msgCtx.currentTextId + 1);
//                         this->actor.flags &= ~ACTOR_FLAG_10000;
//                         SET_WEEKEVENTREG(WEEKEVENTREG_34_01);
//                         break;

//                     case 0xD78:
//                     case 0xD79:
//                     case 0xD7A:
//                     case 0xD7F:
//                         Message_ContinueTextbox(play, play->msgCtx.currentTextId + 1);
//                         break;

//                     case 0xD77:
//                         this->actionFunc = EnMinifrog_BeginChoirCutscene;
//                         play->msgCtx.msgLength = 0;
//                         break;

//                     case 0xD7C:
//                         if (CHECK_WEEKEVENTREG(WEEKEVENTREG_35_80)) { // Obtained Heart Piece
//                             Message_ContinueTextbox(play, 0xD7E);
//                         } else {
//                             Message_ContinueTextbox(play, 0xD7D); // Get Heart Piece
//                             SET_WEEKEVENTREG(WEEKEVENTREG_35_80);
//                         }
//                         break;

//                     case 0xD7D:
//                         Message_CloseTextbox(play);
//                         this->actionFunc = EnMinifrog_GetFrogHP;
//                         EnMinifrog_GetFrogHP(this, play);
//                         break;

//                     case 0xD7B:
//                     case 0xD7E:
//                     default:
//                         Message_CloseTextbox(play);
//                         this->actionFunc = EnMinifrog_SetupYellowFrogDialog;
//                         this->actor.flags &= ~ACTOR_FLAG_10000;
//                         break;
//                 }
//             }
//             break;

//         default:
//             break;
//     }
// }