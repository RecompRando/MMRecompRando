#include "apcommon.h"
#include "recompui.h"
#include "recompconfig.h"
#include "incbin.h"

// used ProxySaw's notification mod as a base https://github.com/garrettjoecox/ProxyMM_RecompMods/blob/main/packages/Notifications/src/notifications.c

// note: there is a small amount of lag whenever a notification appears

#define MAX_NOTIFICATIONS 8
#define NOTIFICATION_TRUE_DURATION(seconds) (20 * seconds)
#define NOTIFICATION_DURATION NOTIFICATION_TRUE_DURATION(10)
#define NOTIFICATION_FADE_START NOTIFICATION_TRUE_DURATION(2)

#define NOTIFICATION_WIDTH 400.0f
#define NOTIFICATION_PADDING 12.0f
#define IMG_SIZE 75.0f

RecompuiContext notif_context;
RecompuiResource notif_root;
RecompuiResource notif_container;

RecompuiColor notifBg = { 0, 0, 0, 0.8f * 255 };
RecompuiColor notifBgError = { 128, 0, 0, 0.8f * 255 };
RecompuiColor msgTextColor = { 255, 255, 255, 255 };
RecompuiColor playerTextColor = { 0, 255, 127, 255 };

RecompuiColor progTextColor = { 175, 153, 239, 255 };
RecompuiColor usefulTextColor = { 109, 139, 232, 255 };
RecompuiColor junkTextColor = { 0, 238, 238, 255 };
RecompuiColor trapTextColor = { 255, 119, 0, 255 };

typedef struct {
    RecompuiResource notification;
    int duration;
} Notification;

static int notif_head = 0;
static int notif_count = 0;
Notification notifications[MAX_NOTIFICATIONS];

// cache icons (thanks wiseguy)
static RecompuiTextureHandle cached_texture_handles[256];

// load logo images
INCBIN(ap_logo_png, "textures/apLogoNormal.dds")
INCBIN(ap_logo_prog_png, "textures/apLogoProg.dds")
INCBIN(ap_logo_junk_png, "textures/apLogoFiller.dds")

#define ICON_ITEM_TEX_WIDTH  32
#define ICON_ITEM_TEX_HEIGHT 32
#define ICON_ITEM_TEX_SIZE ((ICON_ITEM_TEX_WIDTH * ICON_ITEM_TEX_HEIGHT) * 4)

void setNotificationPosition() {
    // Force notifications to the top right corner in the generate menu so they don't overlap the back button.
    if (randoGenerateMenuOpen()) {
        recompui_set_flex_direction(notif_container, FLEX_DIRECTION_COLUMN_REVERSE);
        recompui_set_align_items(notif_container, ALIGN_ITEMS_FLEX_END);
    }
    else {
        switch (recomp_get_config_u32("notification_position")) {
            case 0: // top left
                recompui_set_flex_direction(notif_container, FLEX_DIRECTION_COLUMN_REVERSE);
                recompui_set_align_items(notif_container, ALIGN_ITEMS_FLEX_START);
                break;
            case 1: // top right
                recompui_set_flex_direction(notif_container, FLEX_DIRECTION_COLUMN_REVERSE);
                recompui_set_align_items(notif_container, ALIGN_ITEMS_FLEX_END);
                break;
            case 2: // bottom left
                recompui_set_flex_direction(notif_container, FLEX_DIRECTION_COLUMN);
                recompui_set_align_items(notif_container, ALIGN_ITEMS_FLEX_START);
                break;
            case 3: // bottom right
                recompui_set_flex_direction(notif_container, FLEX_DIRECTION_COLUMN);
                recompui_set_align_items(notif_container, ALIGN_ITEMS_FLEX_END);
                break;
        }
    }
}

void notificationUpdateCycle() {
    // fade out + remove notifications
    int start_index = notif_head;
    for (int i = 0; i < notif_count; i++) {
        int index = (start_index + i) % MAX_NOTIFICATIONS;

        if (notifications[index].duration > 0) {
            notifications[index].duration--;

            if (notifications[index].duration <= 0) {
                recompui_open_context(notif_context);
                recompui_destroy_element(notif_container, notifications[index].notification);
                recompui_close_context(notif_context);

                // shift head
                if (index == notif_head) {
                    notif_head = (notif_head + 1) % MAX_NOTIFICATIONS;
                    notif_count--;
                }
            } else if (notifications[index].duration < NOTIFICATION_FADE_START) {
                // fade out
                recompui_open_context(notif_context);
                recompui_set_opacity(notifications[index].notification, (float) notifications[index].duration / NOTIFICATION_FADE_START);
                recompui_close_context(notif_context);
            }
        }
    }

    // change notification position
    recompui_open_context(notif_context);
    setNotificationPosition();
    recompui_close_context(notif_context);
}

void randoCreateNotificationContainer() {
    const float body_padding = 32.0f;

    // create notification context
    notif_context = recompui_create_context();
    recompui_set_context_captures_input(notif_context, 0);
    recompui_set_context_captures_mouse(notif_context, 0);

    recompui_open_context(notif_context);

    // create root covering the whole screen
    notif_root = recompui_context_root(notif_context);
    recompui_set_position(notif_root, POSITION_ABSOLUTE);
    recompui_set_top(notif_root, 0, UNIT_DP);
    recompui_set_right(notif_root, 0, UNIT_DP);
    recompui_set_bottom(notif_root, 0, UNIT_DP);
    recompui_set_left(notif_root, 0, UNIT_DP);
    recompui_set_width_auto(notif_root);
    recompui_set_height_auto(notif_root);
    recompui_set_padding(notif_root, body_padding, UNIT_DP);
    recompui_set_flex_direction(notif_root, FLEX_DIRECTION_COLUMN);
    recompui_set_justify_content(notif_root, JUSTIFY_CONTENT_CENTER);

    // create main notification container
    notif_container = recompui_create_element(notif_context, notif_root);
    recompui_set_height(notif_container, 100.0f, UNIT_PERCENT);
    recompui_set_flex_grow(notif_container, 1.0f);
    recompui_set_display(notif_container, DISPLAY_FLEX);
    recompui_set_gap(notif_container, 16.0f, UNIT_DP);

    // set notification order + position
    recompui_set_justify_content(notif_container, JUSTIFY_CONTENT_FLEX_END);
    setNotificationPosition();

    recompui_close_context(notif_context);
    recompui_show_context(notif_context);
}

RecompuiResource create_basic_notification_element() {
    if (notif_count == MAX_NOTIFICATIONS) {
        recompui_destroy_element(notif_container, notifications[notif_head].notification);
        notif_head = (notif_head + 1) % MAX_NOTIFICATIONS;
        notif_count--;
    }

    // const float min_height = IMG_SIZE + (NOTIFICATION_PADDING);

    int index = (notif_head + notif_count) % MAX_NOTIFICATIONS;

    notifications[index].notification = recompui_create_element(notif_context, notif_container);
    notifications[index].duration = NOTIFICATION_DURATION;

    RecompuiResource notification = notifications[index].notification;
    recompui_set_padding(notification, NOTIFICATION_PADDING, UNIT_DP);
    recompui_set_border_radius(notification, 12.0f, UNIT_DP);
    recompui_set_background_color(notification, &notifBg);
    recompui_set_display(notification, DISPLAY_FLEX);
    // recompui_set_gap(notification, 8.0f, UNIT_DP);

    // recompui_set_min_height(notification, min_height, UNIT_DP);
    recompui_set_max_width(notification, NOTIFICATION_WIDTH, UNIT_DP);
    recompui_set_width(notification, NOTIFICATION_WIDTH, UNIT_DP);
    recompui_set_justify_content(notification, JUSTIFY_CONTENT_CENTER);
    recompui_set_align_items(notification, ALIGN_ITEMS_CENTER);

    notif_count++;

    return notification;
}

extern u64 gArrowTex[];
extern u64 gRupeeCounterIconTex[];

RecompuiTextureHandle notification_get_item_image(const u8 item) {
    if (cached_texture_handles[item] != 0) {
        return cached_texture_handles[item];
    }

    u8 item_texture_data[ICON_ITEM_TEX_SIZE];
    RecompuiTextureHandle item_texture_handle;

    switch (item) {
        // use custom image
        case ITEM_AP_PROG:
            item_texture_handle = recompui_create_texture_image_bytes(ap_logo_prog_png, ap_logo_prog_png_end - ap_logo_prog_png);
            break;
        case ITEM_AP_USEFUL:
            item_texture_handle = recompui_create_texture_image_bytes(ap_logo_png, ap_logo_png_end - ap_logo_png);
            break;
        case ITEM_AP_FILLER:
            item_texture_handle = recompui_create_texture_image_bytes(ap_logo_junk_png, ap_logo_junk_png_end - ap_logo_junk_png);
            break;
        // use rom textures
        default:
            // icon_item_24 items
            if (item >= ITEM_SKULL_TOKEN && item <= ITEM_HEART_PIECE_2) {
                CmpDma_LoadFile((uintptr_t)SEGMENT_ROM_START(icon_item_24_static_yar), item - ITEM_SKULL_TOKEN, item_texture_data, 0x900);
                item_texture_handle = recompui_create_texture_rgba32(item_texture_data, 24, 24);
            // bomber's notebook (why is it so different)
            } else if (item == ITEM_BOMBERS_NOTEBOOK) {
                CmpDma_LoadFile((uintptr_t)SEGMENT_ROM_START(icon_item_static_yar), ITEM_SONG_SONATA, item_texture_data, 0x1000);
                item_texture_handle = recompui_create_texture_rgba32(item_texture_data, ICON_ITEM_TEX_WIDTH, ICON_ITEM_TEX_HEIGHT);
            // arrows
            } else if (item == ITEM_ARROWS_30) {
                // u16 array to make each index a single 2 byte pixel
                u16* arrow_data = Lib_SegmentedToVirtual(gArrowTex);
                for (int i = 0; i < 32 * 32; i++) {
                    item_texture_data[i * 4 + 0] = (((arrow_data[i] >> 11) & 0x1F) * 255 + 15) / 31; 
                    item_texture_data[i * 4 + 1] = (((arrow_data[i] >> 6) & 0x1F) * 255 + 15) / 31;
                    item_texture_data[i * 4 + 2] = (((arrow_data[i] >> 1) & 0x1F) * 255 + 15) / 31;
                    item_texture_data[i * 4 + 3] = (arrow_data[i] & 0x0001) * 255;
                }
                item_texture_handle = recompui_create_texture_rgba32(item_texture_data, ICON_ITEM_TEX_WIDTH, ICON_ITEM_TEX_HEIGHT);
            // rupees
            } else if (item == ITEM_RUPEE_GREEN ||
                       item == ITEM_RUPEE_BLUE ||
                       item == ITEM_RUPEE_10 ||
                       item == ITEM_RUPEE_RED ||
                       item == ITEM_RUPEE_PURPLE ||
                       item == ITEM_RUPEE_SILVER ||
                       item == ITEM_RUPEE_HUGE
                       ) {
                u32 color_r;
                u32 color_g;
                u32 color_b;

                switch (item) {
                    case ITEM_RUPEE_GREEN:
                        color_r = 200;
                        color_g = 0;
                        color_b = 255;
                        break;
                    case ITEM_RUPEE_BLUE:
                        color_r = 0;
                        color_g = 200;
                        color_b = 255;
                        break;
                    case ITEM_RUPEE_10:
                        color_r = 255;
                        color_g = 0;
                        color_b = 80;
                        break;
                    case ITEM_RUPEE_RED:
                        color_r = 255;
                        color_g = 100;
                        color_b = 100;
                        break;
                    case ITEM_RUPEE_PURPLE:
                        color_r = 255;
                        color_g = 80;
                        color_b = 255;
                        break;
                    case ITEM_RUPEE_SILVER:
                        color_r = 240;
                        color_g = 250;
                        color_b = 250;
                        break;
                    case ITEM_RUPEE_HUGE:
                        color_r = 255;
                        color_g = 240;
                        color_b = 0;
                        break;
                }

                // thanks to wiseguy for help with this

                // Make sure gSegments[2] is set to the pointer where parameter_static is loaded, since parameter_static is in RSP segment 2
                gSegments[2] = OS_K0_TO_PHYSICAL(gPlay->interfaceCtx.parameterSegment);
                u8* rupee_data = Lib_SegmentedToVirtual(gRupeeCounterIconTex);

                for (int i = 0; i < 16 * 16; i++) {
                    // Every pixel of an ia8 is 2 bytes (1 pixel per i)
                    // The first byte is the amount of color (intensity)
                    u8 rupee_intensity = (rupee_data[i] >> 4) * 0xF;
                    // The second byte is the alpha
                    u8 rupee_alpha = (rupee_data[i] & 0xF) * 0xF;
                    // RGBA32 is 4 bytes per pixel (so i * 4), in order of RGBA 
                    // Mix the rupee intensity with the target rupee color and divide by 255 to scale back down to the right magnitude.
                    item_texture_data[i * 4 + 0] = (rupee_intensity * color_r) / 255;
                    item_texture_data[i * 4 + 1] = (rupee_intensity * color_g) / 255;
                    item_texture_data[i * 4 + 2] = (rupee_intensity * color_b) / 255;
                    // Copy the input alpha to the output alpha directly
                    item_texture_data[i * 4 + 3] = rupee_alpha;
                }

                item_texture_handle = recompui_create_texture_rgba32(item_texture_data, 16, 16);
            // normal items
            } else {
                CmpDma_LoadFile((uintptr_t)SEGMENT_ROM_START(icon_item_static_yar), item, item_texture_data, sizeof(item_texture_data));
                item_texture_handle = recompui_create_texture_rgba32(item_texture_data, ICON_ITEM_TEX_WIDTH, ICON_ITEM_TEX_HEIGHT);
            }
    }

    cached_texture_handles[item] = item_texture_handle;
    return item_texture_handle;
}

RecompuiColor* notification_get_item_classification_color(RandoItemClassification item_class) {
    switch (item_class) {
        case RANDO_ITEM_CLASS_PROGRESSION:
            return &progTextColor;
        case RANDO_ITEM_CLASS_USEFUL:
            return &usefulTextColor;
        case RANDO_ITEM_CLASS_JUNK:
            return &junkTextColor;
        case RANDO_ITEM_CLASS_TRAP:
            return &trapTextColor;
    }

    return &msgTextColor;
}

void randoEmitRecieveNotification(const char* item_name, const char* from_name, const ItemId item, RandoItemClassification item_class) {
    const float max_text_width = NOTIFICATION_WIDTH - IMG_SIZE - (NOTIFICATION_PADDING * 3);

    recompui_open_context(notif_context);

    RecompuiResource notification = create_basic_notification_element();

    RecompuiTextureHandle item_texture_handle = notification_get_item_image(item);

    RecompuiResource item_view = recompui_create_imageview(notif_context, notification, item_texture_handle);
    recompui_set_height(item_view, IMG_SIZE, UNIT_DP);
    recompui_set_width(item_view, IMG_SIZE, UNIT_DP);

    RecompuiResource text_area = recompui_create_element(notif_context, notification);
    recompui_set_flex_grow(text_area, 1.0f);
    recompui_set_padding_left(text_area, NOTIFICATION_PADDING, UNIT_DP);
    recompui_set_width(text_area, max_text_width, UNIT_DP);
    recompui_set_display(text_area, DISPLAY_INLINE);

    RecompuiResource text_got = recompui_create_label(notif_context, text_area, "You got ", LABELSTYLE_SMALL);
    recompui_set_color(text_got, &msgTextColor);

    RecompuiResource text_item = recompui_create_label(notif_context, text_area, item_name, LABELSTYLE_SMALL);
    recompui_set_color(text_item, notification_get_item_classification_color(item_class));

    RecompuiResource text_from = recompui_create_label(notif_context, text_area, " from ", LABELSTYLE_SMALL);
    recompui_set_color(text_from, &msgTextColor);

    RecompuiResource text_player = recompui_create_label(notif_context, text_area, from_name, LABELSTYLE_SMALL);
    recompui_set_color(text_player, &playerTextColor);

    recompui_close_context(notif_context);
}

void randoEmitSendNotification(const char* item_name, const char* to_name, const ItemId item, RandoItemClassification item_class) {
    const float max_text_width = NOTIFICATION_WIDTH - IMG_SIZE - (NOTIFICATION_PADDING * 3);

    recompui_open_context(notif_context);

    RecompuiResource notification = create_basic_notification_element();

    RecompuiTextureHandle item_texture_handle = notification_get_item_image(item);

    RecompuiResource item_view = recompui_create_imageview(notif_context, notification, item_texture_handle);
    recompui_set_height(item_view, IMG_SIZE, UNIT_DP);
    recompui_set_width(item_view, IMG_SIZE, UNIT_DP);

    RecompuiResource text_area = recompui_create_element(notif_context, notification);
    recompui_set_flex_grow(text_area, 1.0f);
    recompui_set_padding_left(text_area, NOTIFICATION_PADDING, UNIT_DP);
    recompui_set_width(text_area, max_text_width, UNIT_DP);
    recompui_set_display(text_area, DISPLAY_INLINE);

    RecompuiResource text_sent = recompui_create_label(notif_context, text_area, "You sent ", LABELSTYLE_SMALL);
    recompui_set_color(text_sent, &msgTextColor);

    RecompuiResource text_item = recompui_create_label(notif_context, text_area, item_name, LABELSTYLE_SMALL);
    recompui_set_color(text_item, notification_get_item_classification_color(item_class));

    RecompuiResource text_to = recompui_create_label(notif_context, text_area, " to ", LABELSTYLE_SMALL);
    recompui_set_color(text_to, &msgTextColor);

    RecompuiResource text_player = recompui_create_label(notif_context, text_area, to_name, LABELSTYLE_SMALL);
    recompui_set_color(text_player, &playerTextColor);

    recompui_close_context(notif_context);
}

void randoEmitNormalNotification(const char* notif_text) {
    recompui_open_context(notif_context);

    RecompuiResource notification = create_basic_notification_element();
    recompui_set_width(notification, NOTIFICATION_WIDTH, UNIT_DP);

    RecompuiResource text_got = recompui_create_label(notif_context, notification, notif_text, LABELSTYLE_SMALL);
    recompui_set_color(text_got, &msgTextColor);

    recompui_close_context(notif_context);
}

void randoEmitErrorNotification(const char* error_text) {
    recompui_open_context(notif_context);

    RecompuiResource notification = create_basic_notification_element();
    recompui_set_background_color(notification, &notifBgError);
    recompui_set_width(notification, NOTIFICATION_WIDTH, UNIT_DP);

    RecompuiResource text_got = recompui_create_label(notif_context, notification, error_text, LABELSTYLE_SMALL);
    recompui_set_color(text_got, &msgTextColor);

    recompui_close_context(notif_context);
}