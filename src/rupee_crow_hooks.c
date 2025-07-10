#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "z64recomp_api.h"

#include "apcommon.h"
#include "actor_helpers.h"

// "scene" 0xFF lol
#define LOCATION_RUPEE_CROW(index) (0x17FF00 | index)

#define ENRUPPECROW_GET_PATH_INDEX(thisx) (((thisx)->params & 0xFC00) >> 0xA)

#define ENRUPPECROW_PATH_INDEX_NONE 0x3F

#define ENRUPPECROW_RUPEE_COUNT 20

struct EnRuppecrow;

typedef void (*EnRuppecrowActionFunc)(struct EnRuppecrow*, PlayState*);

typedef enum EnRuppecrowBodyPart {
    /* 0 */ ENRUPPECROW_BODYPART_0,
    /* 1 */ ENRUPPECROW_BODYPART_1,
    /* 2 */ ENRUPPECROW_BODYPART_2,
    /* 3 */ ENRUPPECROW_BODYPART_3,
    /* 4 */ ENRUPPECROW_BODYPART_MAX
} EnRuppecrowBodyPart;

#define OBJECT_CROW_LIMB_MAX 0x09

typedef struct EnRuppecrow {
    /* 0x000 */ Actor actor;
    /* 0x144 */ EnItem00* rupees[ENRUPPECROW_RUPEE_COUNT];
    /* 0x194 */ SkelAnime skelAnime;
    /* 0x1D8 */ EnRuppecrowActionFunc actionFunc;
    /* 0x1DC */ UNK_TYPE4 unk_1DC; // unused
    /* 0x1E0 */ Vec3s jointTable[OBJECT_CROW_LIMB_MAX];
    /* 0x216 */ Vec3s morphTable[OBJECT_CROW_LIMB_MAX];
    /* 0x24C */ Path* path;
    /* 0x250 */ s32 currentPoint;
    /* 0x254 */ ColliderJntSph collider;
    /* 0x274 */ ColliderJntSphElement colliderElements[1];
    /* 0x2B4 */ u16 isGoingCounterClockwise;
    /* 0x2B8 */ f32 speedModifier;
    /* 0x2BC */ s16 rupeeIndex;
    /* 0x2BE */ s16 yOffset;
    /* 0x2C0 */ UNK_TYPE4 unk_2C0; // unused
    /* 0x2C4 */ u8 currentEffect; 
    /* 0x2C8 */ f32 unk_2C8; // set but not used
    /* 0x2CC */ f32 unk_2CC; // set but not used
    /* 0x2D0 */ f32 iceSfxTimer;
    /* 0x2D4 */ Vec3f bodyPartsPos[ENRUPPECROW_BODYPART_MAX];
} EnRuppecrow; // size = 0x304

EnRuppecrow* sEnRuppecrow;

s32 EnRuppecrow_CanSpawnBlueRupees(PlayState*);

void Item_RandoCollectibleDraw(Actor* thisx, PlayState* play);
void Item_RandoCollectibleActionFunc(EnItem00* this, PlayState* play);

RECOMP_HOOK("EnRuppecrow_SpawnRupee")
void OnEnRuppecrow_SpawnRupee(EnRuppecrow* this, PlayState* play) {
    sEnRuppecrow = this;
}

RECOMP_HOOK_RETURN("EnRuppecrow_SpawnRupee")
void AfterEnRuppecrow_SpawnRupee() {
    EnRuppecrow* this = sEnRuppecrow;
    PlayState* play = gPlay;

    Player* player = GET_PLAYER(play);
    f32 xOffset;
    s16 rupeeIndex = this->rupeeIndex - 1;
    EnItem00* rupee = this->rupees[rupeeIndex];

    u32* extendedItem00Data;
    extendedItem00Data = z64recomp_get_extended_actor_data(&rupee->actor, item00Extension);
    *extendedItem00Data = LOCATION_RUPEE_CROW(rupeeIndex);

    if (EnRuppecrow_CanSpawnBlueRupees(play) && (rupeeIndex % 5) == 4 && rupeeIndex != 19) {
        *extendedItem00Data = LOCATION_RUPEE_CROW(rupeeIndex + 20); // arbitrary +20
    }

    rupee->actor.params = ITEM00_APITEM;
    rupee->actor.draw = Item_RandoCollectibleDraw;
    rupee->getItemId = rando_get_item_id(*extendedItem00Data);
    rupee->actionFunc = Item_RandoCollectibleActionFunc;
}