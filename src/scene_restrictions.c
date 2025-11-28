#include "modding.h"

#include "apcommon.h"

typedef struct {
    /* 0x0 */ u8 scene;
    /* 0x1 */ u8 flags1;
    /* 0x2 */ u8 flags2;
    /* 0x3 */ u8 flags3;
} RestrictionFlags; // size = 0x4

#define RESTRICTIONS_TABLE_END 0xFF

#define RESTRICTIONS_GET_BITS(flags, s) (((flags) & (3 << (s))) >> (s))

// does nothing
#define RESTRICTIONS_GET_HGAUGE(flags) RESTRICTIONS_GET_BITS((flags)->flags1, 6)
// 0 = usable, !0 = unusable
#define RESTRICTIONS_GET_B_BUTTON(flags) RESTRICTIONS_GET_BITS((flags)->flags1, 4)
// does nothing
#define RESTRICTIONS_GET_A_BUTTON(flags) RESTRICTIONS_GET_BITS((flags)->flags1, 2)
// 0 = usable, !0 = unusable
#define RESTRICTIONS_GET_TRADE_ITEMS(flags) RESTRICTIONS_GET_BITS((flags)->flags1, 0)
// 0 = usable, !0 = unusable
#define RESTRICTIONS_GET_SONG_OF_TIME(flags) RESTRICTIONS_GET_BITS((flags)->flags2, 6)
// 0 = usable, !0 = unusable
#define RESTRICTIONS_GET_SONG_OF_DOUBLE_TIME(flags) RESTRICTIONS_GET_BITS((flags)->flags2, 4)
// 0 = usable, !0 = unusable
#define RESTRICTIONS_GET_INV_SONG_OF_TIME(flags) RESTRICTIONS_GET_BITS((flags)->flags2, 2)
// 0 = usable, !0 = unusable
#define RESTRICTIONS_GET_SONG_OF_SOARING(flags) RESTRICTIONS_GET_BITS((flags)->flags2, 0)
// 0 = usable, !0 = unusable
#define RESTRICTIONS_GET_SONG_OF_STORMS(flags) RESTRICTIONS_GET_BITS((flags)->flags3, 6)
// 0 = usable, !0 = unusable
#define RESTRICTIONS_GET_MASKS(flags) RESTRICTIONS_GET_BITS((flags)->flags3, 4)
// 0 = usable, !0 = unusable
#define RESTRICTIONS_GET_PICTO_BOX(flags) RESTRICTIONS_GET_BITS((flags)->flags3, 2)
// 0 = usable, !0 = unusable
#define RESTRICTIONS_GET_ALL(flags) RESTRICTIONS_GET_BITS((flags)->flags3, 0)

#define RESTRICTIONS_SET(hGauge, bButton, aButton, tradeItems, songOfTime, songOfDoubleTime, invSongOfTime, \
                         songOfSoaring, songOfStorms, masks, pictoBox, all)                                 \
    ((((hGauge)&3) << 6) | (((bButton)&3) << 4) | (((aButton)&3) << 2) | (((tradeItems)&3) << 0)),          \
        ((((songOfTime)&3) << 6) | (((songOfDoubleTime)&3) << 4) | (((invSongOfTime)&3) << 2) |             \
         (((songOfSoaring)&3) << 0)),                                                                       \
        ((((songOfStorms)&3) << 6) | (((masks)&3) << 4) | (((pictoBox)&3) << 2) | (((all)&3) << 0))

// Common patterns
#define RESTRICTIONS_NONE RESTRICTIONS_SET(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#define RESTRICTIONS_INDOORS RESTRICTIONS_SET(0, 1, 0, 0, 0, 3, 0, 0, 3, 0, 0, 1)
#define RESTRICTIONS_MOON RESTRICTIONS_SET(0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0)
#define RESTRICTIONS_NO_DOUBLE_TIME RESTRICTIONS_SET(0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0)

#define DEFINE_SCENE(_name, enumValue, _textId, _drawConfig, restrictionFlags, _persistentCycleFlags) \
    { enumValue, restrictionFlags },

#define DEFINE_SCENE_UNSET(enumValue) { enumValue, RESTRICTIONS_NONE },

RestrictionFlags sRestrictionFlags[] = {
#include "tables/scene_table.h"
    // { RESTRICTIONS_TABLE_END, RESTRICTIONS_NONE }, // See note below
};

RECOMP_PATCH void Interface_SetSceneRestrictions(PlayState* play) {
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    s16 i = 0;
    u8 currentScene;
    Player* player = GET_PLAYER(play);

    interfaceCtx->restrictions.bButton = 0;
    interfaceCtx->restrictions.songOfDoubleTime = 0;

    do {
        currentScene = (u8)play->sceneId;
        switch (currentScene) {
            case SCENE_OKUJOU:
            case SCENE_TOUGITES:
                return;
        }
        if (currentScene == sRestrictionFlags[i].scene) {
            interfaceCtx->restrictions.hGauge = RESTRICTIONS_GET_HGAUGE(&sRestrictionFlags[i]);
            interfaceCtx->restrictions.all = RESTRICTIONS_GET_ALL(&sRestrictionFlags[i]);
            interfaceCtx->restrictions.masks = RESTRICTIONS_GET_MASKS(&sRestrictionFlags[i]);
            interfaceCtx->restrictions.aButton = RESTRICTIONS_GET_A_BUTTON(&sRestrictionFlags[i]);
            interfaceCtx->restrictions.tradeItems = RESTRICTIONS_GET_TRADE_ITEMS(&sRestrictionFlags[i]);
            interfaceCtx->restrictions.songOfTime = RESTRICTIONS_GET_SONG_OF_TIME(&sRestrictionFlags[i]);
            interfaceCtx->restrictions.invSongOfTime = RESTRICTIONS_GET_INV_SONG_OF_TIME(&sRestrictionFlags[i]);
            interfaceCtx->restrictions.songOfSoaring = RESTRICTIONS_GET_SONG_OF_SOARING(&sRestrictionFlags[i]);
            interfaceCtx->restrictions.songOfStorms = RESTRICTIONS_GET_SONG_OF_STORMS(&sRestrictionFlags[i]);
            interfaceCtx->restrictions.pictoBox = RESTRICTIONS_GET_PICTO_BOX(&sRestrictionFlags[i]);
            break;
        }
        i++;
    } while (sRestrictionFlags[i].scene != RESTRICTIONS_TABLE_END);

    // Enable Song of Soaring on moon scenes for randomizer
    switch (currentScene) {
        case SCENE_LAST_DEKU:   // 0x2A - Moon Deku Trial
        case SCENE_LAST_GORON:  // 0x3F - Moon Goron Trial
        case SCENE_LAST_ZORA:   // 0x47 - Moon Zora Trial
        case SCENE_LAST_LINK:   // 0x66 - Moon Link Trial
        case SCENE_SOUGEN:      // 0x67 - The Moon (field with tree)
        case SCENE_LAST_BS:     // 0x0B - Majora's Lair
            interfaceCtx->restrictions.songOfSoaring = 0;
            break;
    }
}