#include "modding.h"
#include "apcommon.h"
#include "recomputils.h"

#include "rando_colors.h"

#include "aplogo_filler.h"
#include "aplogo_prog.h"
#include "aplogo.h"

#include "bombchu_bag.h"
#include "double_defense.h"
#include "skull_token.h"
#include "dungeon_items.h"
#include "rupoor.h"

// ty neirn
#define SEGMENTED_TO_GLOBAL_PTR(obj, segmentedPtr) ((void *)((uintptr_t)obj + SEGMENT_OFFSET(segmentedPtr)))

void GetItem_DrawBombchu(PlayState* play, s16 drawId);
void GetItem_DrawPoes(PlayState* play, s16 drawId);
void GetItem_DrawFairyBottle(PlayState* play, s16 drawId);
void GetItem_DrawSkullToken(PlayState* play, s16 drawId);
void GetItem_DrawCompass(PlayState* play, s16 drawId);
void GetItem_DrawPotion(PlayState* play, s16 drawId);
void GetItem_DrawGoronSword(PlayState* play, s16 drawId);
void GetItem_DrawDekuNuts(PlayState* play, s16 drawId);
void GetItem_DrawRecoveryHeart(PlayState* play, s16 drawId);
void GetItem_DrawFish(PlayState* play, s16 drawId);
void GetItem_DrawOpa0(PlayState* play, s16 drawId);
void GetItem_DrawOpa0Xlu1(PlayState* play, s16 drawId);
void GetItem_DrawOpa01(PlayState* play, s16 drawId);
void GetItem_DrawXlu01(PlayState* play, s16 drawId);
void GetItem_DrawSeahorse(PlayState* play, s16 drawId);
void GetItem_DrawFairyContainer(PlayState* play, s16 drawId);
void GetItem_DrawMoonsTear(PlayState* play, s16 drawId);
void GetItem_DrawMagicArrow(PlayState* play, s16 drawId);
void GetItem_DrawUpgrades(PlayState* play, s16 drawId);
void GetItem_DrawRupee(PlayState* play, s16 drawId);
void GetItem_DrawSmallRupee(PlayState* play, s16 drawId);
void GetItem_DrawWallet(PlayState* play, s16 drawId);
void GetItem_DrawRemains(PlayState* play, s16 drawId);

void GetItem_DrawRecompImport(PlayState* play, s16 drawId);
void GetItem_DrawBombchuBagDL(PlayState* play, void* dl0, void* dl1, void* dl2);
void GetItem_DrawSkullTokenDL(PlayState* play, void* dl0, TokenType type);
void GetItem_DrawRupeeDL(PlayState* play, void* dl0, void* dl1, void* dl2, void* dl3);
void GetItem_DrawDungeonOpa0(PlayState* play, void* dl0, s16 drawId);
void GetItem_DrawBossKeyRecolor(PlayState* play, void* dl0, void* dl1, s16 drawId);
void GetItem_DrawCompassRecolor(PlayState* play, void* dl0, void* dl1, s16 drawId);
void GetItem_DrawOpa0DL(PlayState* play, void* dl0);
void GetItem_DrawXlu01DL(PlayState* play, void* dl0, void* dl1);
void GetItem_DrawAPFiller(PlayState* play, s16 drawId);
void GetItem_DrawOpa0WithFlame(PlayState* play, void* dl0);
void GetItem_DrawOpa01WithFlame(PlayState* play, void* dl0, void* dl1);
void GetItem_DrawOwlStatue(PlayState* play);
void GetItem_DrawFrog(PlayState* play, s16 drawId);

extern Gfx gGiEmptyBottleCorkDL[];
extern Gfx gGiEmptyBottleGlassDL[];
extern Gfx gGiSmallKeyDL[];
extern Gfx gGiKamaroMaskDL[];
extern Gfx gGiKamaroMaskEmptyDL[];
extern Gfx gGiCouplesMaskFullDL[];
extern Gfx gGiCouplesMaskHalfDL[];
extern Gfx gGiSerenadeColorDL[];
extern Gfx gGiSongNoteDL[];
extern Gfx gGiRequiemColorDL[];
extern Gfx gGiNocturneColorDL[];
extern Gfx gGiPreludeColorDL[];
extern Gfx gGiRecoveryHeartDL[];
extern Gfx gGiBossKeyDL[];
extern Gfx gGiBossKeyGemDL[];
extern Gfx gGiCompassDL[];
extern Gfx gGiCompassGlassDL[];
extern Gfx gGiBombersNotebookEmptyDL[];
extern Gfx gGiBombersNotebookDL[];
extern Gfx gGiStoneMaskEmptyDL[];
extern Gfx gGiStoneMaskDL[];
extern Gfx gGiKafeiMaskDL[];
extern Gfx gGiKafeiMaskEmptyDL[];
extern Gfx gGiCircusLeaderMaskFaceDL[];
extern Gfx gGiCircusLeaderMaskEyebrowsDL[];
extern Gfx gGiBremenMaskDL[];
extern Gfx gGiBremenMaskEmptyDL[];
extern Gfx gGiAllNightMaskEyesDL[];
extern Gfx gGiAllNightMaskFaceDL[];
extern Gfx gGiNutDL[];
extern Gfx gGiHeartBorderDL[];
extern Gfx gGiHeartContainerDL[];
extern Gfx gGiHeartPieceDL[];
extern Gfx gGiQuiverInnerDL[];
extern Gfx gGiQuiverOuterDL[];
extern Gfx gGiQuiver30InnerColorDL[];
extern Gfx gGiQuiver30OuterColorDL[];
extern Gfx gGiQuiver40InnerColorDL[];
extern Gfx gGiQuiver40OuterColorDL[];
extern Gfx gGiQuiver50InnerColorDL[];
extern Gfx gGiQuiver50OuterColorDL[];
extern Gfx gGiBombBagDL[];
extern Gfx gGiBombBagRingDL[];
extern Gfx gGiBombBag20BagColorDL[];
extern Gfx gGiBombBag20RingColorDL[];
extern Gfx gGiBombBag30BagColorDL[];
extern Gfx gGiBombBag30RingColorDL[];
extern Gfx gGiBombBag40BagColorDL[];
extern Gfx gGiBombBag40RingColorDL[];
extern Gfx gGiStickDL[];
extern Gfx gGiDungeonMapDL[];
extern Gfx gGiMagicJarSmallDL[];
extern Gfx gGiMagicJarLargeDL[];
extern Gfx gGiBombDL[];
extern Gfx gGiStoneOfAgonyDL[];
extern Gfx gGiWalletDL[];
extern Gfx gGiAdultWalletColorDL[];
extern Gfx gGiAdultWalletRupeeOuterColorDL[];
extern Gfx gGiWalletRupeeOuterDL[];
extern Gfx gGiAdultWalletStringColorDL[];
extern Gfx gGiWalletStringDL[];
extern Gfx gGiAdultWalletRupeeInnerColorDL[];
extern Gfx gGiWalletRupeeInnerDL[];
extern Gfx gGiGiantsWalletColorDL[];
extern Gfx gGiGiantsWalletRupeeOuterColorDL[];
extern Gfx gGiGiantsWalletStringColorDL[];
extern Gfx gGiGiantsWalletRupeeInnerColorDL[];
extern Gfx gGiDonGeroMaskFaceDL[];
extern Gfx gGiDonGeroMaskBodyDL[];
extern Gfx gGiArrowSmallDL[];
extern Gfx gGiArrowMediumDL[];
extern Gfx gGiArrowLargeDL[];
extern Gfx gGiBombchuDL[];
extern Gfx gGiHerosShieldEmblemDL[];
extern Gfx gGiHerosShieldDL[];
extern Gfx gGiHookshotEmptyDL[];
extern Gfx gGiHookshotDL[];
extern Gfx gGiOcarinaOfTimeDL[];
extern Gfx gGiOcarinaOfTimeHolesDL[];
extern Gfx gGiMilkBottleContentsDL[];
extern Gfx gGiMilkBottleGlassDL[];
extern Gfx gGiKeatonMaskDL[];
extern Gfx gGiKeatonMaskEyesDL[];
extern Gfx gGiTingleMapDL[];
extern Gfx gGiTingleMapEmptyDL[];
extern Gfx gGiBowHandleDL[];
extern Gfx gGiBowStringDL[];
extern Gfx gGiLensDL[];
extern Gfx gGiLensGlassDL[];
extern Gfx gGiPotionContainerPotDL[];
extern Gfx gGiPotionContainerLiquidDL[];
extern Gfx gGiPotionContainerPatternDL[];
extern Gfx gGiPotionContainerGreenPotColorDL[];
extern Gfx gGiPotionContainerGreenLiquidColorDL[];
extern Gfx gGiPotionContainerGreenPatternColorDL[];
extern Gfx gGiPotionContainerRedPotColorDL[];
extern Gfx gGiPotionContainerRedLiquidColorDL[];
extern Gfx gGiPotionContainerRedPatternColorDL[];
extern Gfx gGiPotionContainerBluePotColorDL[];
extern Gfx gGiPotionContainerBlueLiquidColorDL[];
extern Gfx gGiPotionContainerBluePatternColorDL[];
extern Gfx gGiMirrorShieldEmptyDL[];
extern Gfx gGiMirrorShieldDL[];
extern Gfx gGiBeanDL[];
extern Gfx gGiFishContainerDL[];
extern Gfx gGiLetterToMamaEnvelopeLetterDL[];
extern Gfx gGiLetterToMamaInscriptionsDL[];
extern Gfx gGiBiggoronSwordDL[];
extern Gfx gGiSunMaskFaceDL[];
extern Gfx gGiSunMaskEyesSidePatternsDL[];
extern Gfx gGiBlastMaskEmptyDL[];
extern Gfx gGiBlastMaskDL[];
extern Gfx gGiFairyBottleEmptyDL[];
extern Gfx gGiFairyBottleGlassCorkDL[];
extern Gfx gGiFairyBottleContentsDL[];
extern Mtx gGiFairyBottleBillboardRotMtx;
extern Gfx gGiMaskOfScentsFaceDL[];
extern Gfx gGiMaskOfScentsTeethDL[];
extern Gfx gGiCaptainsHatBodyDL[];
extern Gfx gGiCaptainsHatFaceDL[];
extern Gfx gGiBunnyHoodDL[];
extern Gfx gGiBunnyHoodEyesDL[];
extern Gfx gGiMaskOfTruthDL[];
extern Gfx gGiMaskOfTruthAccentsDL[];
extern Gfx gGiTitleDeedEmptyDL[];
extern Gfx gGiTitleDeedSwampColorDL[];
extern Gfx gGiTitleDeedMountainColorDL[];
extern Gfx gGiGoldRupeeInnerColorDL[];
extern Gfx gGiGoldRupeeOuterColorDL[];
extern Gfx gGiTitleDeedOceanColorDL[];
extern Gfx gGiGoronMaskEmptyDL[];
extern Gfx gGiGoronMaskDL[];
extern Gfx gGiZoraMaskEmptyDL[];
extern Gfx gGiZoraMaskDL[];
extern Gfx gGiMagicArrowAmmoDL[];
extern Gfx gGiMagicArrowFireColorDL[];
extern Gfx gGiMagicArrowGlowDL[];
extern Gfx gGiMagicArrowIceColorDL[];
extern Gfx gGiMagicArrowLightColorDL[];
extern Gfx gGiSkulltulaTokenDL[];
extern Gfx gGiSkulltulaTokenFlameDL[];
extern Gfx gGiBugContainerContentsDL[];
extern Gfx gGiBugContainerGlassDL[];
extern Gfx gGiPoeContainerLidDL[];
extern Gfx gGiPoeContainerGlassDL[];
extern Gfx gGiPoeContainerContentsDL[];
extern Gfx gGiPoeContainerPoeColorDL[];
extern Gfx gGiFairyContainerBaseCapDL[];
extern Gfx gGiFairyContainerGlassDL[];
extern Gfx gGiFairyContainerContentsDL[];
extern Gfx gGiRupeeInnerDL[];
extern Gfx gGiGreenRupeeInnerColorDL[];
extern Gfx gGiRupeeOuterDL[];
extern Gfx gGiGreenRupeeOuterColorDL[];
extern Gfx gGiBlueRupeeInnerColorDL[];
extern Gfx gGiBlueRupeeOuterColorDL[];
extern Gfx gGiRedRupeeInnerColorDL[];
extern Gfx gGiRedRupeeOuterColorDL[];
extern Gfx gGiPoeContainerLidDL[];
extern Gfx gGiPoeContainerGlassDL[];
extern Gfx gGiPoeContainerContentsDL[];
extern Gfx gGiPoeContainerBigPoeColorDL[];
extern Gfx gGiPurpleRupeeInnerColorDL[];
extern Gfx gGiPurpleRupeeOuterColorDL[];
extern Gfx gGiSilverRupeeInnerColorDL[];
extern Gfx gGiSilverRupeeOuterColorDL[];
extern Gfx gGiKokiriSwordBladeHiltDL[];
extern Gfx gGiKokiriSwordGuardDL[];
extern Gfx gSkulltulaTokenDL[];
extern Gfx gSkulltulaTokenFlameDL[];
extern Gfx gGiRedPotionBottleEmptyDL[];
extern Gfx gGiRedPotionBottleDL[];
extern Gfx gGiZoraEggBottleContentsDL[];
extern Gfx gGiZoraEggBottleGlassAndCorkDL[];
extern Gfx gGiMoonsTearItemDL[];
extern Gfx gGiMoonsTearGlowDL[];
extern Gfx gGiTitleDeedLandColorDL[];
extern Gfx gGiDekuMaskEmptyDL[];
extern Gfx gGiDekuMaskDL[];
extern Gfx gRemainsOdolwaDL[];
extern Gfx gGiPowderKegBarrelDL[];
extern Gfx gGiPowderKegGoronSkullAndFuseDL[];
extern Gfx gGiGoldDustPowderDL[];
extern Gfx gGiGoldDustPowderEmptyDL[];
extern Gfx gGiSeahorseBottleEmptyDL[];
extern Gfx gGiSeahorseBottleGlassAndCorkDL[];
extern Gfx gGiHylianLoachBottleContentsDL[];
extern Gfx gGiHylianLoachBottleGlassCorkWaterDL[];
extern Gfx gGiHylianLoachEmptyDL[];
extern Gfx gGiHylianLoachDL[];
extern Gfx gGiSeahorseEmptyDL[];
extern Gfx gGiSeahorseBodyDL[];
extern Gfx gGiSeahorseGlowDL[];
extern Mtx gGiSeahorseBillboardRotMtx;
extern Gfx gRemainsGohtDL[];
extern Gfx gRemainsGyorgDL[];
extern Gfx gRemainsTwinmoldDL[];
extern Gfx gGiRazorSwordDL[];
extern Gfx gGiRazorSwordEmptyDL[];
extern Gfx gGiGildedSwordDL[];
extern Gfx gGiGildedSwordEmptyDL[];
extern Gfx gGiGreatFairysSwordBladeDL[];
extern Gfx gGiGreatFairysSwordHiltEmblemDL[];
extern Gfx gGiGarosMaskCloakDL[];
extern Gfx gGiGarosMaskFaceDL[];
extern Gfx gGiGreatFairyMaskFaceDL[];
extern Gfx gGiGreatFairyMaskLeavesDL[];
extern Gfx gGiGibdoMaskEmptyDL[];
extern Gfx gGiGibdoMaskDL[];
extern Gfx gGiRoomKeyEmptyDL[];
extern Gfx gGiRoomKeyDL[];
extern Gfx gGiLetterToKafeiEnvelopeLetterDL[];
extern Gfx gGiLetterToKafeiInscriptionsDL[];
extern Gfx gGiPendantOfMemoriesEmptyDL[];
extern Gfx gGiPendantOfMemoriesDL[];
extern Gfx gGiMagicMushroomEmptyDL[];
extern Gfx gGiMagicMushroomDL[];
extern Gfx gGiRomaniMaskCapDL[];
extern Gfx gGiRomaniMaskNoseEyeDL[];
extern Gfx gGiPostmanHatCapDL[];
extern Gfx gGiPostmanHatBunnyLogoDL[];
extern Gfx gGiGiantMaskEmptyDL[];
extern Gfx gGiGiantMaskDL[];
extern Gfx gGiChateauRomaniBottleEmptyDL[];
extern Gfx gGiChateauRomaniBottleDL[];
extern Gfx gGiPictoBoxFrameDL[];
extern Gfx gGiPictoBoxBodyAndLensDL[];
extern Gfx gGiFierceDeityMaskFaceDL[];
extern Gfx gGiFierceDeityMaskHairAndHatDL[];
extern Gfx gGiMinuetColorDL[];
extern Gfx gGiBoleroColorDL[];

extern Gfx archilogo_grayscale_archilogo_bw_mesh[];
extern Gfx archilogo_arrow_archilogo_mesh[];
extern Gfx archilogo_archilogo_mesh[];

extern Gfx gOwlStatueOpenedDL[];

Gfx gGiTimeColorDL[] = {
    gsDPSetEnvColor(50, 64, 168, 255),
    gsSPEndDisplayList(),
};

Gfx gGiHealingColorDL[] = {
    gsDPSetEnvColor(255, 0, 183, 255),
    gsSPEndDisplayList(),
};

Gfx gGiEponaColorDL[] = {
    gsDPSetEnvColor(140, 45, 10, 255),
    gsSPEndDisplayList(),
};

Gfx gGiSoaringColorDL[] = {
    gsDPSetEnvColor(255, 199, 179, 255),
    gsSPEndDisplayList(),
};

Gfx gGiStormsColorDL[] = {
    gsDPSetEnvColor(0, 1, 84, 255),
    gsSPEndDisplayList(),
};

typedef struct {
    /* 0x0 */ void (*drawFunc)(PlayState*, s16);
    /* 0x4 */ void* drawResources[8]; // Either display lists (Gfx*) or matrices (Mtx*)
} DrawItemTableEntry;                 // size = 0x24

static DrawItemTableEntry sDrawItemTable_new[] = {
    // GID_BOTTLE, OBJECT_GI_BOTTLE
    { GetItem_DrawOpa0Xlu1, { gGiEmptyBottleCorkDL, gGiEmptyBottleGlassDL } },
    // GID_KEY_SMALL, OBJECT_GI_KEY
    { GetItem_DrawOpa0, { gGiSmallKeyDL } },
    // GID_MASK_KAMARO, OBJECT_GI_MASK17
    { GetItem_DrawOpa01, { gGiKamaroMaskDL, gGiKamaroMaskEmptyDL } },
    // GID_MASK_COUPLE, OBJECT_GI_MASK13
    { GetItem_DrawOpa0Xlu1, { gGiCouplesMaskFullDL, gGiCouplesMaskHalfDL } },
    // GID_04, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiSerenadeColorDL, gGiSongNoteDL } },
    // GID_05, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiRequiemColorDL, gGiSongNoteDL } },
    // GID_06, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiNocturneColorDL, gGiSongNoteDL } },
    // GID_07, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiPreludeColorDL, gGiSongNoteDL } },
    // GID_RECOVERY_HEART, OBJECT_GI_HEART
    { GetItem_DrawRecoveryHeart, { gGiRecoveryHeartDL } },
    // GID_KEY_BOSS, OBJECT_GI_KEY_BOSS
    { GetItem_DrawOpa0Xlu1, { gGiBossKeyDL, gGiBossKeyGemDL } },
    // GID_COMPASS, OBJECT_GI_COMPASS
    { GetItem_DrawCompass, { gGiCompassDL, gGiCompassGlassDL } },
    // GID_BOMBERS_NOTEBOOK, OBJECT_GI_SCHEDULE
    { GetItem_DrawOpa0Xlu1, { gGiBombersNotebookEmptyDL, gGiBombersNotebookDL } },
    // GID_MASK_STONE, OBJECT_GI_STONEMASK
    { GetItem_DrawOpa0Xlu1, { gGiStoneMaskEmptyDL, gGiStoneMaskDL } },
    // GID_MASK_KAFEIS_MASK, OBJECT_GI_MASK05
    { GetItem_DrawOpa01, { gGiKafeiMaskDL, gGiKafeiMaskEmptyDL } },
    // GID_MASK_CIRCUS_LEADER, OBJECT_GI_MASK11
    { GetItem_DrawOpa0Xlu1, { gGiCircusLeaderMaskFaceDL, gGiCircusLeaderMaskEyebrowsDL } },
    // GID_MASK_BREMEN, OBJECT_GI_MASK20
    { GetItem_DrawOpa01, { gGiBremenMaskDL, gGiBremenMaskEmptyDL } },
    // GID_MASK_ALL_NIGHT, OBJECT_GI_MASK06
    { GetItem_DrawOpa0Xlu1, { gGiAllNightMaskEyesDL, gGiAllNightMaskFaceDL } },
    // GID_DEKU_NUTS, OBJECT_GI_NUTS
    { GetItem_DrawDekuNuts, { gGiNutDL } },
    // GID_HEART_CONTAINER, OBJECT_GI_HEARTS
    { GetItem_DrawXlu01, { gGiHeartBorderDL, gGiHeartContainerDL } },
    // GID_HEART_PIECE, OBJECT_GI_HEARTS
    { GetItem_DrawXlu01, { gGiHeartBorderDL, gGiHeartPieceDL } },
    // GID_QUIVER_30, OBJECT_GI_ARROWCASE
    { GetItem_DrawUpgrades, { gGiQuiverInnerDL, gGiQuiver30InnerColorDL, gGiQuiver30OuterColorDL, gGiQuiverOuterDL } },
    // GID_QUIVER_40, OBJECT_GI_ARROWCASE
    { GetItem_DrawUpgrades, { gGiQuiverInnerDL, gGiQuiver40InnerColorDL, gGiQuiver40OuterColorDL, gGiQuiverOuterDL } },
    // GID_QUIVER_50, OBJECT_GI_ARROWCASE
    { GetItem_DrawUpgrades, { gGiQuiverInnerDL, gGiQuiver50InnerColorDL, gGiQuiver50OuterColorDL, gGiQuiverOuterDL } },
    // GID_BOMB_BAG_20, OBJECT_GI_BOMBPOUCH
    { GetItem_DrawUpgrades, { gGiBombBagDL, gGiBombBag20BagColorDL, gGiBombBag20RingColorDL, gGiBombBagRingDL } },
    // GID_BOMB_BAG_30, OBJECT_GI_BOMBPOUCH
    { GetItem_DrawUpgrades, { gGiBombBagDL, gGiBombBag30BagColorDL, gGiBombBag30RingColorDL, gGiBombBagRingDL } },
    // GID_BOMB_BAG_40, OBJECT_GI_BOMBPOUCH
    { GetItem_DrawUpgrades, { gGiBombBagDL, gGiBombBag40BagColorDL, gGiBombBag40RingColorDL, gGiBombBagRingDL } },
    // GID_DEKU_STICK, OBJECT_GI_STICK
    { GetItem_DrawOpa0, { gGiStickDL } },
    // GID_DUNGEON_MAP, OBJECT_GI_MAP
    { GetItem_DrawOpa0, { gGiDungeonMapDL } },
    // GID_MAGIC_JAR_SMALL, OBJECT_GI_MAGICPOT
    { GetItem_DrawOpa0, { gGiMagicJarSmallDL } },
    // GID_MAGIC_JAR_BIG, OBJECT_GI_MAGICPOT
    { GetItem_DrawOpa0, { gGiMagicJarLargeDL } },
    // GID_BOMB, OBJECT_GI_BOMB_1
    { GetItem_DrawOpa0, { gGiBombDL } },
    // GID_STONE_OF_AGONY, OBJECT_GI_MAP
    { GetItem_DrawOpa0, { gGiStoneOfAgonyDL } },
    // GID_WALLET_ADULT, OBJECT_GI_PURSE
    { GetItem_DrawWallet,
      { gGiWalletDL, gGiAdultWalletColorDL, gGiAdultWalletRupeeOuterColorDL, gGiWalletRupeeOuterDL,
        gGiAdultWalletStringColorDL, gGiWalletStringDL, gGiAdultWalletRupeeInnerColorDL, gGiWalletRupeeInnerDL } },
    // GID_WALLET_GIANT, OBJECT_GI_PURSE
    { GetItem_DrawWallet,
      { gGiWalletDL, gGiGiantsWalletColorDL, gGiGiantsWalletRupeeOuterColorDL, gGiWalletRupeeOuterDL,
        gGiGiantsWalletStringColorDL, gGiWalletStringDL, gGiGiantsWalletRupeeInnerColorDL, gGiWalletRupeeInnerDL } },
    // GID_MASK_DON_GERO, OBJECT_GI_MASK16
    { GetItem_DrawOpa0Xlu1, { gGiDonGeroMaskFaceDL, gGiDonGeroMaskBodyDL } },
    // GID_ARROWS_SMALL, OBJECT_GI_ARROW
    { GetItem_DrawOpa0, { gGiArrowSmallDL } },
    // GID_ARROWS_MEDIUM, OBJECT_GI_ARROW
    { GetItem_DrawOpa0, { gGiArrowMediumDL } },
    // GID_ARROWS_LARGE, OBJECT_GI_ARROW
    { GetItem_DrawOpa0, { gGiArrowLargeDL } },
    // GID_BOMBCHU, OBJECT_GI_BOMB_2
    { GetItem_DrawBombchu, { gGiBombchuDL } },
    // GID_SHIELD_HERO, OBJECT_GI_SHIELD_2
    { GetItem_DrawOpa0Xlu1, { gGiHerosShieldEmblemDL, gGiHerosShieldDL } },
    // GID_HOOKSHOT, OBJECT_GI_HOOKSHOT
    { GetItem_DrawOpa01, { gGiHookshotEmptyDL, gGiHookshotDL } },
    // GID_29, OBJECT_GI_HOOKSHOT
    { GetItem_DrawOpa01, { gGiHookshotEmptyDL, gGiHookshotDL } },
    // GID_OCARINA, OBJECT_GI_OCARINA
    { GetItem_DrawOpa0Xlu1, { gGiOcarinaOfTimeDL, gGiOcarinaOfTimeHolesDL } },
    // GID_MILK, OBJECT_GI_MILK
    { GetItem_DrawOpa0Xlu1, { gGiMilkBottleContentsDL, gGiMilkBottleGlassDL } },
    // GID_MASK_KEATON, OBJECT_GI_KI_TAN_MASK
    { GetItem_DrawOpa0Xlu1, { gGiKeatonMaskDL, gGiKeatonMaskEyesDL } },
    // GID_TINGLE_MAP, OBJECT_GI_FIELDMAP
    { GetItem_DrawOpa01, { gGiTingleMapDL, gGiTingleMapEmptyDL } },
    // GID_BOW, OBJECT_GI_BOW
    { GetItem_DrawOpa01, { gGiBowHandleDL, gGiBowStringDL } },
    // GID_LENS, OBJECT_GI_GLASSES
    { GetItem_DrawOpa0Xlu1, { gGiLensDL, gGiLensGlassDL } },
    // GID_POTION_GREEN, OBJECT_GI_LIQUID
    { GetItem_DrawPotion,
      { gGiPotionContainerPotDL, gGiPotionContainerGreenPotColorDL, gGiPotionContainerGreenLiquidColorDL,
        gGiPotionContainerLiquidDL, gGiPotionContainerGreenPatternColorDL, gGiPotionContainerPatternDL } },
    // GID_POTION_RED, OBJECT_GI_LIQUID
    { GetItem_DrawPotion,
      { gGiPotionContainerPotDL, gGiPotionContainerRedPotColorDL, gGiPotionContainerRedLiquidColorDL,
        gGiPotionContainerLiquidDL, gGiPotionContainerRedPatternColorDL, gGiPotionContainerPatternDL } },
    // GID_POTION_BLUE, OBJECT_GI_LIQUID
    { GetItem_DrawPotion,
      { gGiPotionContainerPotDL, gGiPotionContainerBluePotColorDL, gGiPotionContainerBlueLiquidColorDL,
        gGiPotionContainerLiquidDL, gGiPotionContainerBluePatternColorDL, gGiPotionContainerPatternDL } },
    // GID_SHIELD_MIRROR, OBJECT_GI_SHIELD_3
    { GetItem_DrawOpa0Xlu1, { gGiMirrorShieldEmptyDL, gGiMirrorShieldDL } },
    // GID_MAGIC_BEANS, OBJECT_GI_BEAN
    { GetItem_DrawOpa0, { gGiBeanDL } },
    // GID_FISH, OBJECT_GI_FISH
    { GetItem_DrawFish, { gGiFishContainerDL } },
    // GID_LETTER_MAMA, OBJECT_GI_RESERVE_B_01
    { GetItem_DrawOpa0Xlu1, { gGiLetterToMamaEnvelopeLetterDL, gGiLetterToMamaInscriptionsDL } },
    // GID_37
    { GetItem_DrawAPFiller, { archilogo_grayscale_archilogo_bw_mesh } },
    // GID_SWORD_BGS, OBJECT_GI_LONGSWORD
    { GetItem_DrawGoronSword, { gGiBiggoronSwordDL } },
    // GID_MASK_SUN, OBJECT_GI_MSSA
    { GetItem_DrawOpa0Xlu1, { gGiSunMaskFaceDL, gGiSunMaskEyesSidePatternsDL } },
    // GID_MASK_BLAST, OBJECT_GI_MASK21
    { GetItem_DrawOpa0Xlu1, { gGiBlastMaskEmptyDL, gGiBlastMaskDL } },
    // GID_FAIRY, OBJECT_GI_BOTTLE_04
    { GetItem_DrawFairyContainer,
      { gGiFairyBottleEmptyDL, gGiFairyBottleGlassCorkDL, gGiFairyBottleContentsDL, &gGiFairyBottleBillboardRotMtx } },
    // GID_MASK_SCENTS, OBJECT_GI_MASK22
    { GetItem_DrawOpa01, { gGiMaskOfScentsFaceDL, gGiMaskOfScentsTeethDL } },
    // GID_MASK_CAPTAIN, OBJECT_GI_MASK18
    { GetItem_DrawOpa01, { gGiCaptainsHatBodyDL, gGiCaptainsHatFaceDL } },
    // GID_MASK_BUNNY, OBJECT_GI_RABIT_MASK
    { GetItem_DrawOpa0Xlu1, { gGiBunnyHoodDL, gGiBunnyHoodEyesDL } },
    // GID_MASK_TRUTH, OBJECT_GI_TRUTH_MASK
    { GetItem_DrawOpa0Xlu1, { gGiMaskOfTruthDL, gGiMaskOfTruthAccentsDL } },
    // GID_DEED_SWAMP, OBJECT_GI_RESERVE01
    { GetItem_DrawOpa01, { gGiTitleDeedEmptyDL, gGiTitleDeedSwampColorDL } },
    // GID_DEED_MOUNTAIN, OBJECT_GI_RESERVE01
    { GetItem_DrawOpa01, { gGiTitleDeedEmptyDL, gGiTitleDeedMountainColorDL } },
    // GID_RUPEE_HUGE, OBJECT_GI_RUPY
    { GetItem_DrawRupee, { gGiRupeeInnerDL, gGiGoldRupeeInnerColorDL, gGiRupeeOuterDL, gGiGoldRupeeOuterColorDL } },
    // GID_DEED_OCEAN, OBJECT_GI_RESERVE01
    { GetItem_DrawOpa01, { gGiTitleDeedEmptyDL, gGiTitleDeedOceanColorDL } },
    // GID_MASK_GORON, OBJECT_GI_GOLONMASK
    { GetItem_DrawOpa0Xlu1, { gGiGoronMaskEmptyDL, gGiGoronMaskDL } },
    // GID_MASK_ZORA, OBJECT_GI_ZORAMASK
    { GetItem_DrawOpa01, { gGiZoraMaskEmptyDL, gGiZoraMaskDL } },
    // GID_46
    { GetItem_DrawRecompImport, { archilogo_arrow_archilogo_mesh } },
    // GID_ARROW_FIRE, OBJECT_GI_M_ARROW
    { GetItem_DrawMagicArrow, { gGiMagicArrowAmmoDL, gGiMagicArrowFireColorDL, gGiMagicArrowGlowDL } },
    // GID_ARROW_ICE, OBJECT_GI_M_ARROW
    { GetItem_DrawMagicArrow, { gGiMagicArrowAmmoDL, gGiMagicArrowIceColorDL, gGiMagicArrowGlowDL } },
    // GID_ARROW_LIGHT, OBJECT_GI_M_ARROW
    { GetItem_DrawMagicArrow, { gGiMagicArrowAmmoDL, gGiMagicArrowLightColorDL, gGiMagicArrowGlowDL } },
    // GID_SKULL_TOKEN, OBJECT_GI_SUTARU
    { GetItem_DrawSkullToken, { gGiSkulltulaTokenDL, gGiSkulltulaTokenFlameDL } },
    // GID_BUG, OBJECT_GI_INSECT
    { GetItem_DrawOpa0Xlu1, { gGiBugContainerContentsDL, gGiBugContainerGlassDL } },
    // GID_4C
    { GetItem_DrawRecompImport, { archilogo_archilogo_mesh } },
    // GID_POE, OBJECT_GI_GHOST
    { GetItem_DrawPoes,
      { gGiPoeContainerLidDL, gGiPoeContainerGlassDL, gGiPoeContainerContentsDL, gGiPoeContainerPoeColorDL } },
    // GID_FAIRY_2, OBJECT_GI_SOUL
    { GetItem_DrawFairyBottle, { gGiFairyContainerBaseCapDL, gGiFairyContainerGlassDL, gGiFairyContainerContentsDL } },
    // GID_RUPEE_GREEN, OBJECT_GI_RUPY
    { GetItem_DrawSmallRupee,
      { gGiRupeeInnerDL, gGiGreenRupeeInnerColorDL, gGiRupeeOuterDL, gGiGreenRupeeOuterColorDL } },
    // GID_RUPEE_BLUE, OBJECT_GI_RUPY
    { GetItem_DrawSmallRupee,
      { gGiRupeeInnerDL, gGiBlueRupeeInnerColorDL, gGiRupeeOuterDL, gGiBlueRupeeOuterColorDL } },
    // GID_RUPEE_RED, OBJECT_GI_RUPY
    { GetItem_DrawSmallRupee, { gGiRupeeInnerDL, gGiRedRupeeInnerColorDL, gGiRupeeOuterDL, gGiRedRupeeOuterColorDL } },
    // GID_BIG_POE, OBJECT_GI_GHOST
    { GetItem_DrawPoes,
      { gGiPoeContainerLidDL, gGiPoeContainerGlassDL, gGiPoeContainerContentsDL, gGiPoeContainerBigPoeColorDL } },
    // GID_RUPEE_PURPLE, OBJECT_GI_RUPY
    { GetItem_DrawRupee, { gGiRupeeInnerDL, gGiPurpleRupeeInnerColorDL, gGiRupeeOuterDL, gGiPurpleRupeeOuterColorDL } },
    // GID_RUPEE_SILVER, OBJECT_GI_RUPY
    { GetItem_DrawRupee, { gGiRupeeInnerDL, gGiSilverRupeeInnerColorDL, gGiRupeeOuterDL, gGiSilverRupeeOuterColorDL } },
    // GID_SWORD_KOKIRI, OBJECT_GI_SWORD_1
    { GetItem_DrawOpa01, { gGiKokiriSwordBladeHiltDL, gGiKokiriSwordGuardDL } },
    // GID_SKULL_TOKEN_2, OBJECT_ST
    { GetItem_DrawSkullToken, { gSkulltulaTokenDL, gSkulltulaTokenFlameDL } },
    // GID_57, OBJECT_GI_BOTTLE_RED
    { GetItem_DrawOpa0Xlu1, { gGiRedPotionBottleEmptyDL, gGiRedPotionBottleDL } },
    // GID_ZORA_EGG, OBJECT_GI_BOTTLE_15
    { GetItem_DrawOpa0Xlu1, { gGiZoraEggBottleContentsDL, gGiZoraEggBottleGlassAndCorkDL } },
    // GID_MOONS_TEAR, OBJECT_GI_RESERVE00
    { GetItem_DrawMoonsTear, { gGiMoonsTearItemDL, gGiMoonsTearGlowDL } },
    // GID_DEED_LAND, OBJECT_GI_RESERVE01
    { GetItem_DrawOpa01, { gGiTitleDeedEmptyDL, gGiTitleDeedLandColorDL } },
    // GID_MASK_DEKU, OBJECT_GI_NUTSMASK
    { GetItem_DrawOpa0Xlu1, { gGiDekuMaskEmptyDL, gGiDekuMaskDL } },
    // GID_REMAINS_ODOLWA, OBJECT_BSMASK
    { GetItem_DrawRemains, { gRemainsOdolwaDL, gRemainsOdolwaDL } },
    // GID_POWDER_KEG, OBJECT_GI_BIGBOMB
    { GetItem_DrawOpa0Xlu1, { gGiPowderKegBarrelDL, gGiPowderKegGoronSkullAndFuseDL } },
    // GID_GOLD_DUST, OBJECT_GI_GOLD_DUST
    { GetItem_DrawOpa01, { gGiGoldDustPowderDL, gGiGoldDustPowderEmptyDL } },
    // GID_SEAHORSE, OBJECT_GI_BOTTLE_16
    { GetItem_DrawOpa0Xlu1, { gGiSeahorseBottleEmptyDL, gGiSeahorseBottleGlassAndCorkDL } },
    // GID_60, OBJECT_GI_BOTTLE_22
    { GetItem_DrawOpa0Xlu1, { gGiHylianLoachBottleContentsDL, gGiHylianLoachBottleGlassCorkWaterDL } },
    // GID_HYLIAN_LOACH, OBJECT_GI_LOACH
    { GetItem_DrawOpa0Xlu1, { gGiHylianLoachEmptyDL, gGiHylianLoachDL } },
    // GID_SEAHORSE_CAUGHT, OBJECT_GI_SEAHORSE
    { GetItem_DrawSeahorse, { gGiSeahorseEmptyDL, gGiSeahorseBodyDL, gGiSeahorseGlowDL, &gGiSeahorseBillboardRotMtx } },
    // GID_REMAINS_GOHT, OBJECT_BSMASK
    { GetItem_DrawRemains, { gRemainsGohtDL, gRemainsGohtDL } },
    // GID_REMAINS_GYORG, OBJECT_BSMASK
    { GetItem_DrawRemains, { gRemainsGyorgDL, gRemainsGyorgDL } },
    // GID_REMAINS_TWINMOLD, OBJECT_BSMASK
    { GetItem_DrawRemains, { gRemainsTwinmoldDL, gRemainsTwinmoldDL } },
    // GID_SWORD_RAZOR, OBJECT_GI_SWORD_2
    { GetItem_DrawOpa01, { gGiRazorSwordDL, gGiRazorSwordEmptyDL } },
    // GID_SWORD_GILDED, OBJECT_GI_SWORD_3
    { GetItem_DrawOpa01, { gGiGildedSwordDL, gGiGildedSwordEmptyDL } },
    // GID_SWORD_GREAT_FAIRY, OBJECT_GI_SWORD_4
    { GetItem_DrawOpa0Xlu1, { gGiGreatFairysSwordBladeDL, gGiGreatFairysSwordHiltEmblemDL } },
    // GID_MASK_GARO, OBJECT_GI_MASK09
    { GetItem_DrawOpa0Xlu1, { gGiGarosMaskCloakDL, gGiGarosMaskFaceDL } },
    // GID_MASK_GREAT_FAIRY, OBJECT_GI_MASK14
    { GetItem_DrawOpa0Xlu1, { gGiGreatFairyMaskFaceDL, gGiGreatFairyMaskLeavesDL } },
    // GID_MASK_GIBDO, OBJECT_GI_MASK15
    { GetItem_DrawOpa0Xlu1, { gGiGibdoMaskEmptyDL, gGiGibdoMaskDL } },
    // GID_ROOM_KEY, OBJECT_GI_RESERVE_B_00
    { GetItem_DrawOpa0Xlu1, { gGiRoomKeyEmptyDL, gGiRoomKeyDL } },
    // GID_LETTER_TO_KAFEI, OBJECT_GI_RESERVE_C_00
    { GetItem_DrawOpa0Xlu1, { gGiLetterToKafeiEnvelopeLetterDL, gGiLetterToKafeiInscriptionsDL } },
    // GID_PENDANT_OF_MEMORIES, OBJECT_GI_RESERVE_C_01
    { GetItem_DrawOpa0Xlu1, { gGiPendantOfMemoriesEmptyDL, gGiPendantOfMemoriesDL } },
    // GID_MUSHROOM, OBJECT_GI_MAGICMUSHROOM
    { GetItem_DrawOpa0Xlu1, { gGiMagicMushroomEmptyDL, gGiMagicMushroomDL } },
    // GID_MASK_ROMANI, OBJECT_GI_MASK10
    { GetItem_DrawOpa0Xlu1, { gGiRomaniMaskCapDL, gGiRomaniMaskNoseEyeDL } },
    // GID_MASK_POSTMAN, OBJECT_GI_MASK12
    { GetItem_DrawOpa0Xlu1, { gGiPostmanHatCapDL, gGiPostmanHatBunnyLogoDL } },
    // GID_MASK_GIANT, OBJECT_GI_MASK23
    { GetItem_DrawOpa01, { gGiGiantMaskEmptyDL, gGiGiantMaskDL } },
    // GID_CHATEAU, OBJECT_GI_BOTTLE_21
    { GetItem_DrawOpa0Xlu1, { gGiChateauRomaniBottleEmptyDL, gGiChateauRomaniBottleDL } },
    // GID_PICTOGRAPH_BOX, OBJECT_GI_CAMERA
    { GetItem_DrawOpa0Xlu1, { gGiPictoBoxFrameDL, gGiPictoBoxBodyAndLensDL } },
    // GID_MASK_FIERCE_DEITY, OBJECT_GI_MASK03
    { GetItem_DrawOpa01, { gGiFierceDeityMaskFaceDL, gGiFierceDeityMaskHairAndHatDL } },
    // GID_SONG_SONATA, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiMinuetColorDL, gGiSongNoteDL } },
    // GID_SONG_LULLABY, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiBoleroColorDL, gGiSongNoteDL } },
    // GID_SONG_NOVA, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiSerenadeColorDL, gGiSongNoteDL } },
    // GID_SONG_ELEGY, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiRequiemColorDL, gGiSongNoteDL } },
    // GID_SONG_OATH, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiNocturneColorDL, gGiSongNoteDL } },
    // GID_SONG_TIME, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiTimeColorDL, gGiSongNoteDL } },
    // GID_SONG_HEALING, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiHealingColorDL, gGiSongNoteDL } },
    // GID_SONG_EPONA, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiEponaColorDL, gGiSongNoteDL } },
    // GID_SONG_SOARING, OBJECT_GI_MELODY
    { GetItem_DrawXlu01, { gGiSoaringColorDL, gGiSongNoteDL } },
    //~ // GID_SONG_STORMS, OBJECT_GI_MELODY
    //~ { GetItem_DrawXlu01, { gGiStormsColorDL, gGiSongNoteDL } },
    //~ // GID_BAG_BOMBCHU, OBJECT_GI_BAG_BOMBCHU
    //~ { GetItem_DrawRecompImport, { archilogo_arrow_archilogo_mesh } },
};

typedef enum {
    OPA0,
    XLU0,
    OPA01,
    OPA0XLU1,
    XLU01
} ObjectType;

ObjectType sGetObjectType[] = {
    OPA0XLU1,
    OPA0,
    OPA01,
    OPA0XLU1,
    XLU01,
    XLU01,
    XLU01,
    XLU01,
    XLU0,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0,
    XLU01,
    XLU0,
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA01,  // ?
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA01,  // ?
    OPA01,  // ?
    OPA0XLU1,
    OPA0,
    OPA0,
    OPA0,
    OPA0,
    OPA0XLU1,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,  // ?
    OPA0XLU1,  // ?
    OPA0XLU1,  // ?
    OPA0XLU1,
    OPA0,
    XLU0,
    OPA0XLU1,
    OPA0,
    OPA0,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,  // ?
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA01,
    OPA0,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0,
    OPA0,
    OPA0,
    OPA01,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    OPA0XLU1,
    OPA0XLU1,
    OPA01,
    XLU01,
    XLU01,
    XLU01,
    XLU01,
    XLU01,
    XLU01,
    XLU01,
    XLU01,
    XLU01,
    XLU01
};

static DmaRequest objectDmaRequest;
static OSMesg objectLoadMsg;

void ZeldaArena_Free(void* ptr);
void* ZeldaArena_Malloc(size_t size);

bool loadObject(PlayState* play, void** objectSegment, OSMesgQueue* objectLoadQueue, s16 objectId) {
    s32 objectSlot = Object_GetSlot(&play->objectCtx, objectId);

    if (Object_IsLoaded(&play->objectCtx, objectSlot)) {
        return false;
    }

    if (objectId != OBJECT_UNSET_0) {
        size_t objectSize = gObjectTable[objectId].vromEnd - gObjectTable[objectId].vromStart;
        if (*objectSegment != NULL) {
            ZeldaArena_Free(*objectSegment);
        }
        *objectSegment = ZeldaArena_Malloc(objectSize);
        osCreateMesgQueue(objectLoadQueue, &objectLoadMsg, 1);
        DmaMgr_SendRequestImpl(&objectDmaRequest, *objectSegment, gObjectTable[objectId].vromStart,
                               objectSize, 0, objectLoadQueue, NULL);
        return true;
    }

    return false;
}

static bool objectLoaded[OBJECT_ID_MAX];
static void* objectSegments[OBJECT_ID_MAX];
static uintptr_t prevGSegments[NUM_SEGMENTS];

bool ObjLoad(PlayState* play, u8 segment, s16 objectId) {
    if (segment >= NUM_SEGMENTS) return false;

    if (!objectLoaded[objectId]) {
        size_t size = gObjectTable[objectId].vromEnd - gObjectTable[objectId].vromStart;

        objectSegments[objectId] = recomp_alloc(size);

        // At some point we may want to make this async.
        DmaMgr_SendRequest0(objectSegments[objectId], gObjectTable[objectId].vromStart, size);

        objectLoaded[objectId] = true;
    }

    prevGSegments[segment] = gSegments[segment];
    gSegments[segment] = OS_K0_TO_PHYSICAL(objectSegments[objectId]);

    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, segment, objectSegments[objectId]);
    gSPSegment(POLY_XLU_DISP++, segment, objectSegments[objectId]);

    CLOSE_DISPS(play->state.gfxCtx);

    return true;
}

void ObjUnload(PlayState* play, u8 segment, s16 objectId) {
    gSegments[segment] = prevGSegments[segment];

    OPEN_DISPS(play->state.gfxCtx);

    gSPSegment(POLY_OPA_DISP++, segment, gSegments[segment]);
    gSPSegment(POLY_XLU_DISP++, segment, gSegments[segment]);

    CLOSE_DISPS(play->state.gfxCtx);

    // At some point we may keep track of how many active loads there are and free the memory when it reaches 0.
    // For now, even with everything loaded, the memory usage is minimal so we aren't going to worry about it.
}

/**
 * Draw "Get Item" Model
 * Calls the corresponding draw function for the given draw ID
 */
RECOMP_PATCH void GetItem_Draw(PlayState* play, s16 drawId) {
    switch (drawId) {
        case GID_SONG_SOARING:
            GetItem_DrawXlu01DL(play, gGiSoaringColorDL, gGiSongNoteDL);
            return;
        case GID_SONG_STORMS:
            GetItem_DrawXlu01DL(play, gGiStormsColorDL, gGiSongNoteDL);
            return;
        case GID_BAG_BOMBCHU:
            GetItem_DrawBombchuBagDL(play, gGiBombchu0DL, gGiBombchu1DL, gGiBombchu2DL);
            return;
        case GID_DEFENSE_DOUBLE:
            GetItem_DrawXlu01DL(play, gGiDDHeartBorderDL, gGiDDHeartContainerDL);
            return;
        case GID_SWAMP_SKULL_TOKEN:
            GetItem_DrawSkullTokenDL(play, gGiSkulltulaTokenFullDL, TOKEN_SWAMP);
            return;
        case GID_OCEAN_SKULL_TOKEN:
            GetItem_DrawSkullTokenDL(play, gGiSkulltulaTokenFullDL, TOKEN_OCEAN);
            return;
        case GID_KEY_BOSS_WOODFALL:
        case GID_KEY_BOSS_SNOWHEAD:
        case GID_KEY_BOSS_GREATBAY:
        case GID_KEY_BOSS_STONETOWER:
            // remove "Mod" to revert back to vanilla (make this into an option?)
            GetItem_DrawBossKeyRecolor(play, gGiBossKeyDL, gGiBossKeyGemModDL, drawId);
            // GetItem_DrawBossKeyRecolor(play, gGiBossKeyModDL, gGiBossKeyGemModDL, drawId);
            return;
        case GID_KEY_SMALL_WOODFALL:
        case GID_KEY_SMALL_SNOWHEAD:
        case GID_KEY_SMALL_GREATBAY:
        case GID_KEY_SMALL_STONETOWER:
            GetItem_DrawDungeonOpa0(play, gGiSmallKeyModDL, drawId);
            return;
        case GID_MAP_WOODFALL:
        case GID_MAP_SNOWHEAD:
        case GID_MAP_GREATBAY:
        case GID_MAP_STONETOWER:
            GetItem_DrawDungeonOpa0(play, gGiDungeonMapModDL, drawId);
            return;
        case GID_COMPASS_WOODFALL:
        case GID_COMPASS_SNOWHEAD:
        case GID_COMPASS_GREATBAY:
        case GID_COMPASS_STONETOWER:
            GetItem_DrawCompassRecolor(play, gGiCompassModDL, gGiCompassGlassDL, drawId);
            return;
        case GID_RUPOOR:
            GetItem_DrawRupeeDL(play, gGiRupeeInnerDL, gGiRupoorInnerColorDL, gGiRupeeOuterDL, gGiRupoorOuterColorDL);
            return;
        case GID_SPIN_ATTACK:
            GetItem_DrawOpa01WithFlame(play, gGiKokiriSwordBladeHiltDL, gGiKokiriSwordGuardDL);
            return;
        case GID_MAGIC_UPGRADE:
            GetItem_DrawOpa0WithFlame(play, gGiMagicJarLargeDL);
            return;
        case GID_OWL_STATUE:
            GetItem_DrawOwlStatue(play);
            return;
        case GID_FROG_YELLOW:
        case GID_FROG_CYAN:
        case GID_FROG_PINK:
        case GID_FROG_BLUE:
        case GID_FROG_WHITE:
            GetItem_DrawFrog(play, drawId);
            return;
    }
    sDrawItemTable_new[drawId].drawFunc(play, drawId);
}

void GetItem_DrawDynamic(PlayState* play, void* objectSegment, s16 drawId) {
    OPEN_DISPS(play->state.gfxCtx);

    u32 prevSegment = gSegments[6];
    gSegments[6] = OS_K0_TO_PHYSICAL(objectSegment);

    if (drawId >= GID_SONG_SOARING) {
        switch(drawId) {
            case GID_SONG_SOARING:
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case GID_SONG_STORMS:
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case GID_DEFENSE_DOUBLE:
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case GID_SWAMP_SKULL_TOKEN:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case GID_OCEAN_SKULL_TOKEN:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case GID_KEY_BOSS_WOODFALL:
            case GID_KEY_BOSS_SNOWHEAD:
            case GID_KEY_BOSS_GREATBAY:
            case GID_KEY_BOSS_STONETOWER:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case GID_KEY_SMALL_WOODFALL:
            case GID_KEY_SMALL_SNOWHEAD:
            case GID_KEY_SMALL_GREATBAY:
            case GID_KEY_SMALL_STONETOWER:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                break;
            case GID_MAP_WOODFALL:
            case GID_MAP_SNOWHEAD:
            case GID_MAP_GREATBAY:
            case GID_MAP_STONETOWER:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                break;
            case GID_COMPASS_WOODFALL:
            case GID_COMPASS_SNOWHEAD:
            case GID_COMPASS_GREATBAY:
            case GID_COMPASS_STONETOWER:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case GID_RUPOOR:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case GID_SPIN_ATTACK:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case GID_MAGIC_UPGRADE:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case GID_OWL_STATUE:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                break;
            case GID_FROG_YELLOW:
            case GID_FROG_CYAN:
            case GID_FROG_PINK:
            case GID_FROG_BLUE:
            case GID_FROG_WHITE:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
        }
    } else {
        switch (sGetObjectType[drawId]) {
            case OPA0:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                break;
            case XLU0:
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case OPA01:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                break;
            case XLU01:
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
            case OPA0XLU1:
                gSPSegment(POLY_OPA_DISP++, 0x06, objectSegment);
                gSPSegment(POLY_XLU_DISP++, 0x06, objectSegment);
                break;
        }
    }

    GetItem_Draw(play, drawId);

    gSegments[6] = prevSegment;

    CLOSE_DISPS(play->state.gfxCtx);
}

/**
 * Draw `drawResources[0]` as Opa
 */
RECOMP_PATCH void GetItem_DrawOpa0(PlayState* play, s16 drawId) {
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, sDrawItemTable_new[drawId].drawResources[0]);

    CLOSE_DISPS(play->state.gfxCtx);
}

/**
 * Draw `drawResources[0]` as Opa and `drawResources[1]` as Xlu
 */
RECOMP_PATCH void GetItem_DrawOpa0Xlu1(PlayState* play, s16 drawId) {
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, sDrawItemTable_new[drawId].drawResources[0]);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, sDrawItemTable_new[drawId].drawResources[1]);

    CLOSE_DISPS(play->state.gfxCtx);
}

/**
 * Draw `drawResources[0]` and `drawResources[1]` as Opa
 */
RECOMP_PATCH void GetItem_DrawOpa01(PlayState* play, s16 drawId) {
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, sDrawItemTable_new[drawId].drawResources[0]);
    gSPDisplayList(POLY_OPA_DISP++, sDrawItemTable_new[drawId].drawResources[1]);

    CLOSE_DISPS(play->state.gfxCtx);
}

/**
 * Draw `drawResources[0]` and `drawResources[1]` as Xlu
 */
RECOMP_PATCH void GetItem_DrawXlu01(PlayState* play, s16 drawId) {
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, sDrawItemTable_new[drawId].drawResources[0]);
    gSPDisplayList(POLY_XLU_DISP++, sDrawItemTable_new[drawId].drawResources[1]);

    CLOSE_DISPS(play->state.gfxCtx);
}

RECOMP_PATCH void GetItem_DrawRemains(PlayState* play, s16 drawId) {
    s32 pad;
    s8 objectSlot;

    OPEN_DISPS(play->state.gfxCtx);

    //objectSlot = Object_GetSlot(&play->objectCtx, OBJECT_BSMASK);

    //gSPSegment(POLY_OPA_DISP++, 0x06, play->objectCtx.slots[objectSlot].segment);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Matrix_Scale(0.02f, 0.02f, 0.02f, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, sDrawItemTable_new[drawId].drawResources[0]);

    POLY_OPA_DISP = Play_SetFog(play, POLY_OPA_DISP);

    CLOSE_DISPS(play->state.gfxCtx);
}

void GetItem_DrawRecompImport(PlayState* play, s16 drawId) {
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Matrix_Scale(0.0375f, 0.0375f, 0.0375f, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, sDrawItemTable_new[drawId].drawResources[0]);

    CLOSE_DISPS(play->state.gfxCtx);
}

static void color4(u8* r, u8* g, u8* b, u8* a, u32 color)
{
    *r = (color >> 24) & 0xFF;
    *g = (color >> 16) & 0xFF;
    *b = (color >> 8) & 0xFF;
    *a = color & 0xFF;
}

void GetItem_DrawBombchuBagDL(PlayState* play, void* dl0, void* dl1, void* dl2) {
    static const u32 kPrimColor = 0xBA3F3AFF;
    static const u32 kEnvColor = 0xA5231EFF;
    static const u32 kPrimColor2 = 0x1695D2FF;
    static const u32 kEnvColor2 = 0x054C7FFF;

    u8 r, g, b, a;

    OPEN_DISPS(play->state.gfxCtx);
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    color4(&r, &g, &b, &a, kPrimColor2);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, kEnvColor2);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, dl0);
    color4(&r, &g, &b, &a, kPrimColor);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, r, g, b, a);
    color4(&r, &g, &b, &a, kEnvColor);
    gDPSetEnvColor(POLY_XLU_DISP++, r, g, b, a);
    gSPDisplayList(POLY_XLU_DISP++, dl1);
    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, dl2);

    CLOSE_DISPS();
}

void GetItem_DrawFireDL(PlayState* play, Color_RGB8 color) {
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    gSPSegment(POLY_XLU_DISP++, 0x08,
               Gfx_TwoTexScroll(play->state.gfxCtx, G_TX_RENDERTILE, play->state.frames * 0, -(play->state.frames * 5),
                                32, 32, 1, play->state.frames * 0, play->state.frames * 0, 32, 64));

    gDPSetPrimColor(POLY_XLU_DISP++, 0x80, 0x80, color.r, color.g, color.b, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, color.r, color.g, color.b, 0);

    Matrix_Push();
    Matrix_ReplaceRotation(&play->billboardMtxF);
    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    
    gSPDisplayList(POLY_XLU_DISP++, gGiFlameDL);

    Matrix_Pop();

    CLOSE_DISPS(play->state.gfxCtx);
}

void GetItem_DrawSkullTokenDL(PlayState* play, void* dl0, TokenType type) {
    Color_RGB8 flameColor;
    
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    if (type == TOKEN_SWAMP || type == TOKEN_OCEAN) {
        gDPSetPrimColor(POLY_OPA_DISP++, 0x80, 0x80, 255, 255, 170, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, 150, 120, 0, 0);
    } else {
        gDPSetPrimColor(POLY_OPA_DISP++, 0x80, 0x80, 255, 255, 255, 255);
        gDPSetEnvColor(POLY_OPA_DISP++, 196, 196, 196, 0);
        // gDPSetPrimColor(POLY_OPA_DISP++, 0x80, 0x80, rainbowColor.r, rainbowColor.g, rainbowColor.b, 255);
        // gDPSetEnvColor(POLY_OPA_DISP++, rainbowColor.r, rainbowColor.g, rainbowColor.b, 0);
    }

    switch (type) {
        case TOKEN_SWAMP:
            flameColor = gGiSwampFlameColor;
            break;
        case TOKEN_OCEAN:
            flameColor = gGiSwampFlameColor;
            break;
        default:
            flameColor = gGiDefaultFlameColor;
            // flameColor = rainbowColor;
            break;
    }

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, dl0);

    CLOSE_DISPS(play->state.gfxCtx);

    GetItem_DrawFireDL(play, flameColor);
}

void GetItem_DrawOpa0WithFlame(PlayState* play, void* dl0) {    
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, dl0);

    CLOSE_DISPS(play->state.gfxCtx);

    GetItem_DrawFireDL(play, gGiDefaultFlameColor);
}

void GetItem_DrawOpa01WithFlame(PlayState* play, void* dl0, void* dl1) {    
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, dl0);
    gSPDisplayList(POLY_OPA_DISP++, dl1);

    CLOSE_DISPS(play->state.gfxCtx);

    GetItem_DrawFireDL(play, gGiDefaultFlameColor);
}

void GetItem_DrawRupeeDL(PlayState* play, void* dl0, void* dl1, void* dl2, void* dl3) {
    s32 pad;

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, dl1);
    gSPDisplayList(POLY_OPA_DISP++, dl0);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, dl3);
    gSPDisplayList(POLY_XLU_DISP++, dl2);

    CLOSE_DISPS(play->state.gfxCtx);
}

void GetItem_DrawDungeonOpa0(PlayState* play, void* dl0, s16 drawId) {
    s32 pad;
    u8 dungeon = (drawId - GID_KEY_BOSS_WOODFALL) / 4;
    Color_RGB8 colorPrim = dungeonColorsPrim[dungeon];
    Color_RGB8 colorEnv = dungeonColorsEnv[dungeon];

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0, colorPrim.r, colorPrim.g, colorPrim.b, 255);
    gDPSetEnvColor(POLY_OPA_DISP++, colorEnv.r, colorEnv.g, colorEnv.b, 255);
    gSPDisplayList(POLY_OPA_DISP++, dl0);

    CLOSE_DISPS(play->state.gfxCtx);
}

void GetItem_DrawBossKeyRecolor(PlayState* play, void* dl0, void* dl1, s16 drawId) {
    s32 pad;
    u8 dungeon = (drawId - GID_KEY_BOSS_WOODFALL) / 4;
    Color_RGB8 colorPrim = dungeonColorsPrim[dungeon];
    Color_RGB8 colorEnv = dungeonColorsEnv[dungeon];

    OPEN_DISPS(play->state.gfxCtx);

    // main key recolor
    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, colorPrim.r, colorPrim.g, colorPrim.b, 255);
    gDPSetEnvColor(POLY_OPA_DISP++, colorEnv.r, colorEnv.g, colorEnv.b, 255);
    gSPDisplayList(POLY_OPA_DISP++, dl0);

    // gem recolor
    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, colorPrim.r, colorPrim.g, colorPrim.b, 255);
    gDPSetEnvColor(POLY_XLU_DISP++, colorEnv.r, colorEnv.g, colorEnv.b, 255);
    // gDPSetPrimColor(POLY_XLU_DISP++, 0, 0x80, colorPrim.r, colorPrim.g, colorPrim.b, 0x20); // used for alternate boss key gem combiner
    gSPDisplayList(POLY_XLU_DISP++, dl1);

    CLOSE_DISPS(play->state.gfxCtx);
}

void GetItem_DrawCompassRecolor(PlayState* play, void* dl0, void* dl1, s16 drawId) {
    s32 pad;
    u8 dungeon = ((drawId - GID_KEY_BOSS_WOODFALL) / 4);
    Color_RGB8 colorPrim = dungeonColorsPrim[dungeon];
    Color_RGB8 colorEnv = dungeonColorsEnv[dungeon];

    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gDPSetPrimColor(POLY_OPA_DISP++, 0, 0x80, colorPrim.r, colorPrim.g, colorPrim.b, 255);
    gDPSetEnvColor(POLY_OPA_DISP++, colorEnv.r, colorEnv.g, colorEnv.b, 255);
    gSPDisplayList(POLY_OPA_DISP++, dl0);

    POLY_XLU_DISP = Gfx_SetupDL(POLY_XLU_DISP, SETUPDL_5);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, dl1);

    CLOSE_DISPS(play->state.gfxCtx);
}

void GetItem_DrawOpa0DL(PlayState* play, void* dl0) {
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, dl0);

    CLOSE_DISPS(play->state.gfxCtx);
}

void GetItem_DrawXlu01DL(PlayState* play, void* dl0, void* dl1) {
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Xlu(play->state.gfxCtx);

    gSPMatrix(POLY_XLU_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_XLU_DISP++, dl0);
    gSPDisplayList(POLY_XLU_DISP++, dl1);

    CLOSE_DISPS(play->state.gfxCtx);
}

void GetItem_DrawOwlStatue(PlayState* play) {
    if (ObjLoad(play, 0x06, OBJECT_SEK)) {
        Matrix_Translate(0.0f, -34.0f, 0.0f, MTXMODE_APPLY);
        Matrix_Scale(0.01f, 0.01f, 0.01f, MTXMODE_APPLY);
        Gfx_DrawDListOpa(play, SEGMENTED_TO_GLOBAL_PTR(objectSegments[OBJECT_SEK], gOwlStatueOpenedDL));
        ObjUnload(play, 0x06, OBJECT_SEK);
    }
}

extern FlexSkeletonHeader gFrogSkel;
extern AnimationHeader gFrogIdleAnim;
extern Color_RGBA8 sFrogEnvColors[];
extern TexturePtr sEyeTextures_ovl_En_Minifrog[];

// note: the static colors/textures don't seem to work
void GetItem_DrawFrog(PlayState* play, s16 drawId) {
    static bool initialized = false;
    static SkelAnime skelAnime;
    static TexturePtr sEyeTextures[2];

    if (ObjLoad(play, 0x06, OBJECT_FR)) {
        if (!initialized) {
            initialized = true;
            for (u8 i = 0; i < ARRAY_COUNT(sEyeTextures); i++) {
                sEyeTextures[i] = Lib_SegmentedToVirtual(sEyeTextures_ovl_En_Minifrog[i]);
            }

            SkelAnime_InitFlex(play, &skelAnime, &gFrogSkel, &gFrogIdleAnim, NULL, NULL, 0);
        }
        
        // Color_RGBA8* envColor;

        OPEN_DISPS(play->state.gfxCtx);

        Matrix_Translate(0.0f, -25.0f, -10.0f, MTXMODE_APPLY);
        Matrix_Scale(0.03f, 0.03f, 0.03f, MTXMODE_APPLY);

        Gfx_SetupDL25_Opa(play->state.gfxCtx);
        // envColor = &sFrogEnvColors[drawId - GID_FROG_YELLOW];
        // recomp_printf("frog index: %d\n", drawId - GID_FROG_YELLOW);
        // recomp_printf("frog color: %d %d %d\n", envColor->r, envColor->g, envColor->b);
        gSPSegment(POLY_OPA_DISP++, 0x08, sEyeTextures_ovl_En_Minifrog[0]);
        gSPSegment(POLY_OPA_DISP++, 0x09, sEyeTextures_ovl_En_Minifrog[0]);
        // gDPSetEnvColor(POLY_OPA_DISP++, envColor->r, envColor->g, envColor->b, envColor->a);
        gDPSetEnvColor(POLY_OPA_DISP++, rainbowColor.r, rainbowColor.g, rainbowColor.b, 255);
        SkelAnime_DrawFlexOpa(play, skelAnime.skeleton, skelAnime.jointTable, skelAnime.dListCount, NULL, NULL, NULL);

        CLOSE_DISPS(play->state.gfxCtx);

        ObjUnload(play, 0x06, OBJECT_FR);
    }
}

void GetItem_DrawAPFiller(PlayState* play, s16 drawId) {
    OPEN_DISPS(play->state.gfxCtx);

    Gfx_SetupDL25_Opa(play->state.gfxCtx);
    Matrix_Scale(0.15f, 0.15f, 0.15f, MTXMODE_APPLY);

    gSPMatrix(POLY_OPA_DISP++, Matrix_NewMtx(play->state.gfxCtx), G_MTX_NOPUSH | G_MTX_LOAD | G_MTX_MODELVIEW);
    gSPDisplayList(POLY_OPA_DISP++, sDrawItemTable_new[drawId].drawResources[0]);

    CLOSE_DISPS(play->state.gfxCtx);
}