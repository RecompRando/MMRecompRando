#include "modding.h"
#include "global.h"
#include "recomputils.h"

#include "apcommon.h"

#include "rando_colors.h"

Color_RGB8 rainbowColor = {255, 255, 255};
Color_RGB8 goldColor = {255, 176, 0};

Color_RGB8 randoUsefulColor = {64, 255, 255};
Color_RGB8 randoFillerColor = {128, 128, 128}; // might go unused in favor of vanilla textures

Color_RGB8 randoHeartColor = {255, 0, 0};
Color_RGB8 randoSpiderColor = {255, 255, 255};
Color_RGB8 randoFairyColor = {255, 128, 255};
Color_RGB8 randoKeyColor = {192, 192, 192};
Color_RGB8 randoSoulColor = {136, 0, 255};

u16 RGBA16toGrayscale_Color(u16 RGBA, IA_Grayscale_Type type) {
    // https://github.com/krimtonz/rgba5551topng/blob/master/rgba5551topng.c#L111-L114
    u8 red = (((RGBA >> 11) & 0x1F) * 255 + 15) / 31; 
    u8 green = (((RGBA >> 6) & 0x1F) * 255 + 15) / 31;
    u8 blue = (((RGBA >> 1) & 0x1F) * 255 + 15) / 31;
    u8 alpha = (RGBA & 0x0001) * 255;

    u8 grayscale;
    f32 luminosity;

    switch (type) {
        case GRAYSCALE_LUMINOSITY:
            // BT.601 luminosity grayscale
            grayscale = (red * 0.299) + (green * 0.587) + (blue * 0.114);
            break;
        case GRAYSCALE_MAX:
            // simplified version of below
            // guessing this is decomposition?
            grayscale = (MAX(MAX(red, blue), green));
            break;
        case GRAYSCALE_OOTMM:
            // combo's grayscale conversion
            luminosity = (MAX(MAX(red, blue), green)) / 255.0f;
            grayscale = CLAMP_MAX(sqrtf(sqrtf(luminosity)), 1.0) * 255;
            break;
    }

    return (grayscale << 8) | (alpha);
}

void RGBA16toIA16_Texture(u16* rgbaTex, u16* outTex, u32 arrayCount, IA_Grayscale_Type type) {
    for (u32 i = 0; i < arrayCount; i++) {
        outTex[i] = RGBA16toGrayscale_Color(rgbaTex[i], type);
    }
}

void hsv_to_rgb(float h, float s, float v, Color_RGB8* out) {
    float r = 0, g = 0, b = 0;
    if (s == 0) {
        r = g = b = v;
    } else {
        int i;
        float f, p, q, t;
        h = h / 60.0f;
        i = (int)(h);
        f = h - i;
        p = v * (1 - s);
        q = v * (1 - f * s);
        t = v * (1 - (1 - f) * s);
        switch (i) {
            case 0:
                r = v;
                g = t;
                b = p;
                break;
            case 1:
                r = q;
                g = v;
                b = p;
                break;
            case 2:
                r = p;
                g = v;
                b = t;
                break;
            case 3:
                r = p;
                g = q;
                b = v;
                break;
            case 4:
                r = t;
                g = p;
                b = v;
                break;
            default:
                r = v;
                g = p;
                b = q;
                break;
        }
    }
    out->r = r * 255;
    out->g = g * 255;
    out->b = b * 255;
}

// extern Color_RGBA8_u32 gPlayVisMonoColor;
extern Color_RGB16 D_801D07DC[];
extern Color_RGB16 D_801D080C[];
extern Color_RGB16 D_801D086C[];

// Color_RGB8 blueC = randoUsefulColor;
// Color_RGB8 pinkC = {245, 169, 184};
// Color_RGB8 whiteC = {255, 255, 255};

// void trans(Color_RGB8* result) {
//     static float lerp = 0.0f;
//     lerp += 0.05f;
//     float diff = 0.0f;

//     Color_RGB8 from = randoUsefulColor;
//     Color_RGB8 to = randoFairyColor;

//     // if (lerp > 4.0f) {
//     if (lerp > 3.0f) {
//         lerp = 0.0f;
//     } else if (lerp > 1.0f && lerp <= 2.0f) {
//         from = randoFairyColor;
//         to = whiteColor;
//         diff = 1.0f;
//     } else if (lerp > 2.0f && lerp <= 3.0f) {
//         from = whiteColor;
//         // to = randoFairyColor;
//         to = randoUsefulColor;
//         diff = 2.0f;
//     // } else if (lerp > 3.0f && lerp <= 4.0f) {
//     //     from = randoFairyColor;
//     //     to = randoUsefulColor;
//     //     diff = 3.0f;
//     }

//     Color_RGB8_Lerp(&from, &to, lerp - diff, &rainbowColor);
// }

RECOMP_CALLBACK("*", recomp_on_play_main)
void cycle_rainbow() {
    static float hue = 0.0f;
    hue += 5.0;
    if (hue >= 360.0f) {
        hue -= 360.0f;
    }
    hsv_to_rgb(hue, 1.0f, 1.0f, &rainbowColor);
    // trans(&rainbowColor);
    // Color_RGB8_Lerp(&blueC, &pinkC, 0.5f, &rainbowColor);

    D_801D080C[0].r = rainbowColor.r;
    D_801D080C[0].g = rainbowColor.g;
    D_801D080C[0].b = rainbowColor.b;

    // Play_FillScreen(gPlay, true, rainbowColor.r, rainbowColor.g, rainbowColor.b, 64);
    // gVisMonoColor.r = rainbowColor.r;
    // gVisMonoColor.g = rainbowColor.g;
    // gVisMonoColor.b = rainbowColor.b;
    // gVisMonoColor.a = 255;
    // gVisMonoColor.r = 255;
    // gVisMonoColor.g = 0;
    // gVisMonoColor.b = 0;
    // gVisMonoColor.a = 255;

    // func_80848640();
}

// void func_80848640(PlayState* play, Player* this);

#include "overlays/actors/ovl_En_Torch2/z_en_torch2.h"

void spawn_elegy_shell(PlayState* play, Player* this) {
    EnTorch2* torch2;
    Actor* effChange;

    torch2 = play->actorCtx.elegyShells[this->transformation];
    if (torch2 != NULL) {
        Math_Vec3f_Copy(&torch2->actor.home.pos, &this->actor.world.pos);
        torch2->actor.home.rot.y = this->actor.shape.rot.y;
        torch2->state = 0;
        torch2->framesUntilNextState = 20;
    } else {
        torch2 = (EnTorch2*)Actor_Spawn(&play->actorCtx, play, ACTOR_EN_TORCH2, this->actor.world.pos.x,
                                        this->actor.world.pos.y, this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0,
                                        this->transformation);
    }

    // if (torch2 != NULL) {
    //     play->actorCtx.elegyShells[this->transformation] = torch2;
    //     Play_SetupRespawnPoint(play, this->transformation + 3, PLAYER_PARAMS(0xFF, PLAYER_START_MODE_B));
    // }

    effChange = Actor_Spawn(&play->actorCtx, play, ACTOR_EFF_CHANGE, this->actor.world.pos.x, this->actor.world.pos.y,
                            this->actor.world.pos.z, 0, this->actor.shape.rot.y, 0,
                            (GET_PLAYER_FORM << 3) | this->transformation);
    if (effChange != NULL) {
        //! @bug: This function should only pass Player*: it uses *(this + 0x153), which is meant to be
        //! player->currentMask, but in this case is garbage in the skelAnime
        Player_PlaySfx((Player*)effChange, NA_SE_PL_TRANSFORM);
    }
}

// #define RANDOM_ENTRANCE(scene, spawn) (((randomEntrance & 0x7F) << 9) | (((spawn) & 0x1F) << 4))

s32 Room_StartRoomTransition(PlayState* play, RoomContext* roomCtx, s32 index);
void func_8012EBF8(PlayState* play, RoomContext* roomCtx);

void Player_Door_Knob(PlayState* play, Player* this, Actor* door);

RECOMP_HOOK("Player_Update")
void onPlayer_Update(Player* this, PlayState* play) {
    if (CHECK_BTN_ALL(CONTROLLER1(&play->state)->press.button, BTN_L)) {
        // recomp_printf("%f %f %f %f\n", this->actor.world.pos.x, this->actor.world.pos.y, this->actor.world.pos.z, this->actor.floorHeight);
        
        // Rupees_ChangeBy(0);

        // recomp_printf("button pressed\n");
        // spawn_elegy_shell(play, this);
        // u16 randomEntrance = Rand_ZeroOne() * ENTR_SCENE_MAX; // there are a few unset ones that crash
        // // play->nextEntrance = ENTRANCE(SOUTH_CLOCK_TOWN, 0); // warping to normal entrances
        // play->nextEntrance = (randomEntrance & 0x7F) << 9;
        // play->transitionType = TRANS_TYPE_INSTANT;
        // gSaveContext.nextTransitionType = TRANS_TYPE_INSTANT;
        // play->transitionTrigger = TRANS_TRIGGER_START;
        
        // gTransitionTileState = TRANS_TILE_PROCESS;
        // Room_StartRoomTransition(play, &play->roomCtx, 1);
        // func_8012EBF8(play, &play->roomCtx);

        // Actor* actor = play->actorCtx.actorLists[ACTORCAT_DOOR].first;
        // this->doorDirection = 1;
        // Player_Door_Knob(play, this, actor);
        // while (actor != NULL) {
        
        // }

        // Actor* actor = play->actorCtx.actorLists[ACTORCAT_BOSS].first;

        // while (actor != NULL) {
        //     actor->colChkInfo.health = 0;
        //     actor = actor->next;
        // }

        // gSaveContext.save.day = 2;
        // rando_advance_hour(play, 6);

        // recomp_printf("day %d hour %d\n", gSaveContext.save.day, gSaveContext.save.time / CLOCK_TIME_HOUR);

    }
    
    // play->actorCtx.freezeFlashTimer = 0;
    // recomp_printf("fov: %f\n", play->mainCamera.fov);
    // Play_SetCameraFov(play, play->activeCamId, 300.0f);
    // Play_SetCameraFov(play, play->activeCamId, 20.0f);
    // // Play_SetCameraRoll(play, play->activeCamId, this->actor.shape.rot.z);
    // Camera* camera = Play_GetCamera(play, CAM_ID_MAIN);
    // if (camera->focalActor != NULL) {
    //     camera->focalActorAtOffset.x = 0.0f;
    //     camera->focalActorAtOffset.y = 0.0f;
    //     camera->focalActorAtOffset.z = -200.0f;
    // }
    
    // View_ApplyOrtho(&play->view);
}

// sets rColor based on item/location type, returns false to draw original
bool get_rando_color(Color_RGB8* rColor, u32 location) {
    // show default DL if camc is off or location was checked
    if (!rando_get_camc_enabled() || rando_location_is_checked(location)) {
        return false;
    }

    // use yellow color if "show unchecked" in enabled
    if (rando_get_camc_enabled() == CAMC_SHOW_UNCHECKED && !rando_location_is_checked(location)) {
        *rColor = goldColor;
        return true;
    }

    s16 getItem = rando_get_item_id(location);

    // souls (bounds need to be extended for more souls)
    if (getItem >= GI_BOSS_SOUL_ODOLWA && getItem <= GI_ABSURD_GENERIC) {
        *rColor = randoSoulColor;
        return true;
    }

    switch (getItem) {
        case GI_AP_PROG:
            *rColor = rainbowColor;
            return true;
        case GI_AP_USEFUL:
            *rColor = randoUsefulColor;
            return true;
        case GI_AP_FILLER:
            *rColor = randoFillerColor;
            return true;
        // case GI_KEY_BOSS:
        // case GI_KEY_SMALL:
        // case GI_KEY_BOSS_WOODFALL:
        // case GI_KEY_SMALL_WOODFALL:
        // case GI_MAP_WOODFALL:
        // case GI_COMPASS_WOODFALL:
        // case GI_KEY_BOSS_SNOWHEAD:
        // case GI_KEY_SMALL_SNOWHEAD:
        // case GI_MAP_SNOWHEAD:
        // case GI_COMPASS_SNOWHEAD:
        // case GI_KEY_BOSS_GREATBAY:
        // case GI_KEY_SMALL_GREATBAY:
        // case GI_MAP_GREATBAY:
        // case GI_COMPASS_GREATBAY:
        // case GI_KEY_BOSS_STONETOWER:
        // case GI_KEY_SMALL_STONETOWER:
        // case GI_MAP_STONETOWER:
        // case GI_COMPASS_STONETOWER:
        //     *rColor = randoKeyColor;
        //     return true;
        case GI_B2:
        case GI_46:
        case GI_47:
        case GI_48:
        case GI_49:
            *rColor = randoFairyColor;
            return true;
        case GI_HEART_PIECE:
        case GI_HEART_CONTAINER:
        case GI_DEFENSE_DOUBLE:
            *rColor = randoHeartColor;
            return true;
        case GI_TRUE_SKULL_TOKEN:
        case GI_OCEAN_SKULL_TOKEN:
            *rColor = randoSpiderColor;
            return true;
        default:
            switch (rando_get_location_type(location)) {
                case RANDO_ITEM_CLASS_PROGRESSION:
                case RANDO_ITEM_CLASS_TRAP:
                    *rColor = rainbowColor;
                    return true;
                case RANDO_ITEM_CLASS_USEFUL:
                    *rColor = randoUsefulColor;
                    return true;
                case RANDO_ITEM_CLASS_JUNK:
                default:
                    return false;
            }
    }
}