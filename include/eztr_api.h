#ifndef __EZTR_API__
#define __EZTR_API__

/*! \file eztr_api.h
    \version 2.3.0
    \brief The main header for EZTR
 */

#include "modding.h"
#include "global.h"

#ifdef DOXYGEN
#define EZTR_IMPORT(func) func
#define EZTR_PACK_STRUCT 

#else

#define EZTR_IMPORT(func) RECOMP_IMPORT(EZTR_MOD_ID_STR, func)
#define EZTR_PACK_STRUCT __attribute__((packed))

typedef struct {
    u16 new_id;
    u8 out_success;
} _EZTR_CustomMsgHandleSetter;

#define __EZTR_CUSTOM_MSG_HANDLE_BODY(name) { \
    static u16 id; \
    static u8 is_set = 0; \
    if (setter != NULL) { \
        if (is_set) { \
            _EZTR_ReportDebugMessage("EZTR_CustomMsgHandle '" #name "', Header Version 2.3.0: The textId has already been set and will not be updated."); \
            setter->out_success = 0; \
        } \
        else { \
            _EZTR_ReportDebugMessage("EZTR_CustomMsgHandle '" #name "', Header Version 2.3.0: Assignment of textId was successful."); \
            id = setter->new_id; \
            is_set = 1; \
            setter->out_success = 1; \
        } \
    } \
    return id; \
} \

#endif

/**
 * \defgroup General 
 * 
 * \brief General API information.
 * 
 * @{
 */


/**
 * @brief The mod id string for EZTR.
 * 
 * The `eztr_api.h` imports all the functions and events needed for EZTR, 
 * so you probably won't need to use this directly.
 */
#define EZTR_MOD_ID_STR "MM_EZ_Text_Replacer_API"

#ifndef DOXYGEN
EZTR_IMPORT( void _EZTR_ReportFatalMessage(char* msg));
EZTR_IMPORT( void _EZTR_ReportErrorMessage(char* msg));
EZTR_IMPORT( void _EZTR_ReportWarningMessage(char* msg));
EZTR_IMPORT( void _EZTR_ReportInfoMessage(char* msg));
EZTR_IMPORT( void _EZTR_ReportDebugMessage(char* msg));
EZTR_IMPORT( void _EZTR_ReportVerboseMessage(char* msg));

// Typo, Included for backwards compatibility:
EZTR_IMPORT( void _EXTR_ReportErrorMessage(char* msg));
#endif

#define EZTR_MSG_HIGHEST_ID 0x354C
/**
 * @brief The the full size of message buffer, in bytes.
 * 
 * Also equivalent to maximum length of buffer in single-byte characters (`char`).
 */
#define EZTR_MSG_BUFFER_SIZE 1280

/**
 * @brief The maximum length of buffer in two-byte characters (`wchar`).
 * 
 * Not really used, but included for parity with the base game.
 */
#define EZTR_MSG_BUFFER_WIDE_SIZE 640

/**
 * @brief The size of the message buffer's header region, in bytes.
 * 
 * Also servers as the starting index of the message buffer's content region.
 * 
 */
#define EZTR_MSG_HEADER_SIZE 11

/**
 * @brief The size of the message buffer's content region, in bytes.
 * 
 * Useful if you need to loop throught a buffer's content.
 */
 #define EZTR_MSG_CONTENT_SIZE 1269 // MESSAGE_BUFFER_SIZE - MESSAGE_HEADER_SIZE

#define EZTR_MSG_ENDING_CHAR '\xBF'
#define EZTR_PIPE_CHAR '|'

/** @}*/

/**
 * \defgroup CustomMsgHandle 
 * 
 * \brief Macros and types used for working with the handles for custom messages.
 * 
 * This page contains all of the information in custom message handles an how to use them.
 * 
 * It's worth noting that a custom message handle is actually a function, defined through macros.
 * In brief, here are the important things to know:
 * * The handler's only argument is a pointer that EZTR uses internally. Modders don't need to use it.
 * * You can retrieve the handle's textId by calling it with NULL as the argument (`handle(NULL)`)
 * * By default, the handle can only have its textId assigned once (See below on how to change this behavior). 
 * * Handles created using `EZTR_DEFINE_CUSTOM_MSG_HANDLE`, the recommended macro for creating custom message handles, are marked with `RECOMP_EXPORT`.
 *
 * This implementation was decided on to ensure that the textIds of custom messages are not lost through
 * variable reassignments, and to facilitate mods potentially needing to access/modify each other's messages.
 * 
 * For those interested in the technical details: The function's only argument is used by EZTR to set the textId variable 
 * when a custom message is assigned to it, and to check that the assignment was a success. 
 * 
 * The version of the handle function included in this header stores the textId in a static variable, and includes logic that
 * will only allow the textId to be set once. If a second attempt to set the ID is made, an error message is printed. Rather than
 * change a handle's textId, you should change the stored message at that id.
 * 
 * If you want to change the behavior of the handles, you can write your own custom message handle functions, but
 * it's not recommended.
 * 
 * @{
 */
 
/**
 * @brief Uses to set/get the actual symbol name for custom message handles
 * If you want to add a prefix/suffix to your custom message handles globally, it can be done here.
 */
#ifdef EZTR_NO_HANDLE_PREFIX
#define EZTR_CUSTOM_MSG_HANDLE_NAME(name) name
#else
#define EZTR_CUSTOM_MSG_HANDLE_NAME(name_suffix) EZTR_CustomMsgHandle_##name_suffix
#endif
/**
 * @brief Creates a non-exported CustomMsgHandle object
 * 
 * Most mods are not installed in a vacuum. You should consider exporting your custom message handles
 * so that other mods can access the messages you define if need be (compatibility patches, addons, etc.)
 * 
 */
#define EZTR_DEFINE_CUSTOM_MSG_HANDLE_NO_EXPORT(name) \
u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(_EZTR_CustomMsgHandleSetter* setter) \
__EZTR_CUSTOM_MSG_HANDLE_BODY(name)

/**
 * @brief Creates a CustomMsgHandle object
 * 
 * This is the primary method for creating handles for custom messages. You'll need to use this macro
 * inside one of your .c files (outside of any functions) to create the handle itself. Use 
 * `EZTR_DECLARE_CUSTOM_MSG_HANDLE` to access the handle from other files.
 * 
 * See the `EZTR_CustomMsgHandle` documentation for more information about handles and how they work.
 * 
 */
#define EZTR_DEFINE_CUSTOM_MSG_HANDLE(name) RECOMP_EXPORT \
u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(_EZTR_CustomMsgHandleSetter* setter) \
__EZTR_CUSTOM_MSG_HANDLE_BODY(name)

/**
 * @brief Creates a declaration for a CustomMsgHandle object. 
 * 
 * Use this reference handles created in other .c files. Also works in headers.
 * 
 */
#define EZTR_DECLARE_CUSTOM_MSG_HANDLE(name) u16 EZTR_CUSTOM_MSG_HANDLE_NAME(name)(_EZTR_CustomMsgHandleSetter* setter)

/**
 * @brief Creates a declaration for a CustomMsgHandle object. Alternative to `EZTR_DECLARE_CUSTOM_MSG_HANDLE`.
 * 
 * Use this reference handles created in other .c files. Also works in headers.
 * 
 */
#define EZTR_EXTERN_CUSTOM_MSG_HANDLE(name) EZTR_DECLARE_CUSTOM_MSG_HANDLE(name)


/**
 * @brief Declares and imports a CustomMsgHandle object from another mod.
 * 
 * This will allow you to use and interact with custom messages from other mods.
 */
#define EZTR_IMPORT_CUSTOM_MSG_HANDLE(mod_str, name_suffix) RECOMP_IMPORT(mod_str, u16 EZTR_CustomMsgHandle_##name_suffix(_EZTR_CustomMsgHandleSetter* setter))

/**
 * @brief Declares and imports a CustomMsgHandle object from another mod.
 * 
 * This will allow you to use and interact with custom messages from other mods.
 */
#define EZTR_IMPORT_CUSTOM_MSG_HANDLE_NO_PREFIX(mod_str, name) RECOMP_IMPORT(mod_str, u16 name(_EZTR_CustomMsgHandleSetter* setter))

/**
 * @brief Gets the textId from a custom message handle.
 * 
 * A more readable alternative to `handle(NULL)`
 * 
 */
#define EZTR_GET_CUSTOM_MSG_ID(handle) handle(NULL)

// Shorthand:
/**
 * @brief Shorthand for `EZTR_CUSTOM_MSG_HANDLE_NAME()`
 * 
 * Useful if you want to use global prefixes.
 * 
 */
#define EZTR_HNAME(name_suffix) EZTR_CUSTOM_MSG_HANDLE_NAME(name_suffix)

/**
 * @brief Gets the textId from a custom message handle.
 * 
 * A more readable alternative to `handle(NULL)`, and shorthand for `EZTR_GET_CUSTOM_MSG_ID()`
 * 
 * Note that this macro won't apply the global prefix if it's enabled. You'll need to use something like
 * `EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(my_handle))` or `EZTR_GET_ID(EZTR_HNAME(my_handle))`.
 */
#define EZTR_GET_ID(handle) EZTR_GET_CUSTOM_MSG_ID(handle)


/**
 * @brief Gets the textId from a custom message handle.
 * 
 * A more readable alternative to `EZTR_CustomMsgHandle_##handle(NULL)`.
 * 
 * This macro WILL apply the global prefix if it's enabled, and can be considered shorthand for
 * `EZTR_GET_CUSTOM_MSG_ID(EZTR_HNAME(my_handle))` or `EZTR_GET_ID(EZTR_HNAME(my_handle))`.
 */
#define EZTR_GET_ID_H(handle) EZTR_GET_CUSTOM_MSG_ID(EZTR_CUSTOM_MSG_HANDLE_NAME(handle))

/** @}*/
/**
 * @brief A macro to easily create message callback functions.
 * 
 * This macro can be used to create both the function definition and declaration (if one is needed);
 * 
 * * To create the definition, use `EZTR_MSG_CALLBACK(my_callback) {...}`
 * * To create the declaration, use `EZTR_MSG_CALLBACK(my_callback);`
 * 
 * `my_callback` can then be passed to any `EZTR_Basic_*` function as the `callback` argument. 
 * `my_callback` will be run immediately after EZTR loads the message from its internal
 * table, allowing you to make changes (or completely regenerate the message from scratch)
 * before the message is displayed.
 * 
 * The function generated by this macro has 3 arguments:
 * * `EZTR_MsgBuffer* buf` - A message buffer, prepopulated with a copy of whatever message you stored. Write to this buffer the change the message.
 * * `u16 textId` - The textId of the message. Useful if you want multiple messages to be controlled by one callback function.
 * * `PlayState* play` - The current PlayState for the game. This makes it easy to generate text based on the game's state.
 * 
 */
#define EZTR_MSG_CALLBACK(fname) void fname(EZTR_MsgBuffer* buf, u16 textId, PlayState* play)

/**
 * @brief Used to declare a function that should run after EZTR has finished initializing.
 * 
 * This is where you should declare all of your text replacements. You don't want to declare them during a 
 * `recomp_on_init` event, since EZTR may not have initialized itself yet, and attempting to declare text
 * replacements before that will cause a crash. Additionally, declaring messaged here will ensure that mod 
 * priority order is respected when declaring replacements.
 * 
 * As of version 2.1.0, EZTR now enforces that message all calls to `EZTR_Basic_*` functions must be made here.
 * 
 * Example: `EZTR_ON_INIT void declare_my_text() {...}`
 * 
 */
#define EZTR_ON_INIT RECOMP_CALLBACK("MM_EZ_Text_Replacer_API", EZTR_OnInit)

/**
 * @brief Used to declare a callback function for when EZTR dumps a message. Normally, EZTR only prints that message to the game console,
 * but this callback will allow you to do other things with that information. This `EZTR_ON_DUMP_BUFFER` event is only called when EZTR's 
 * text dump setting is set to `On`.
 * 
 * This is a feature intended for developers, for the creation of EZTR-related tools. It should not be used for gameplay.
 *
 * The functions parameters are  
 * * `const char* category`, currently only returns 'Game'.
 * * `u16 textId`, the ID of the message being dumped.
 * * ` s32 len`, the size of the message in bytes.
 * * `EZTR_MsgBuffer* buf`, the message buffer itself.
 * 
 */
#define EZTR_ON_DUMP_BUFFER(func_name) \
    RECOMP_CALLBACK("MM_EZ_Text_Replacer_API", EZTR_OnDumpBuffer) void func_name(const char* category, u16 textId, s32 len, EZTR_MsgBuffer* buf)

/**
 * @brief Used to declare a callback function for when EZTR dumps the entire message buffer. Normally, EZTR only prints that message to the game console,
 * but this callback will allow you to do other things with that information. This `EZTR_ON_DUMP_BUFFER` event is only called when EZTR's 
 * text dump setting is set to `Full`.
 * 
 * This is a feature intended for developers, for the creation of EZTR-related tools. It should not be used for gameplay.
 *
 * The functions parameters are  
 * * `const char* category`, currently only returns 'Game'.
 * * `u16 textId`, the ID of the message being dumped.
 * * ` s32 len`, the size of the message in bytes.
 * * `EZTR_MsgBuffer* buf`, the message buffer itself.
 * 
 */
#define EZTR_ON_DUMP_BUFFER_FULL(func_name) \
    RECOMP_CALLBACK("MM_EZ_Text_Replacer_API", EZTR_OnDumpBufferFull) void func_name(const char* category, u16 textId, s32 len, EZTR_MsgBuffer* buf)

/**
 * @brief Used by certain members of `EZTR_MsgData` (and the message header generally) to indicate that said member is not in use.
 * 
 * The header members in question are:
 * 
 * * `next_message_id`
 * * `first_item_rupees`
 * * `second_item_rupees`
 * 
 */
#define EZTR_NO_VALUE 0xffff

/**
 * \defgroup Types
 * 
 * \brief Type definitions that EZTR uses for Majora's Mask messages.
 * 
 * @{
 */

/**
 * @brief The message buffers type as defined in the Majora's Mask decompilation.
 * 
 * While you can edit messages by interacting with this struct directly, EZTR 
 * offers much better ways of editing messages.
 */
typedef union {
    char schar[EZTR_MSG_BUFFER_SIZE]; // msgBuf
    u16 wchar[EZTR_MSG_BUFFER_WIDE_SIZE];   // msgBufWide
    u64 force_structure_alignment_msg;
} EZTR_MsgBuffer_Raw;


/**
 * @brief The message buffer, but with the header and content regions defined as seperate arrays.
 * 
 * The `content` member can be passed to the `EZTR_MsgSContent_` functions for use in text operations.
 * 
 */
typedef struct {
    char header[EZTR_MSG_HEADER_SIZE];
    char content[EZTR_MSG_CONTENT_SIZE];
} EZTR_MsgBuffer_Partition;


/**
 * @brief The message buffer, with the header represented as its individual members.
 * 
 * When compiled, this struct is marked with the `packed` attribute to ensure that it's members align
 * to their proper locations within the buffer. 
 * 
 * The `padding` member represents a section of the buffer that is unused, and should thus be ignored.
 * 
 * The `content` member can be passed to the `EZTR_MsgSContent_` functions for use in text operations.
 * 
 */
typedef struct EZTR_PACK_STRUCT {
    u8 text_box_type; 
    u8 text_box_y_pos;
    u8 display_icon; 
    u16 next_message_id;
    u16 first_item_rupees;
    u16 second_item_rupees;
    u16 padding;
    char content[EZTR_MSG_CONTENT_SIZE];
} EZTR_MsgBuffer_Data;

/**
 * @brief A union of the three MsgBuffer structs, and the primary type for interacting with message data.
 * 
 * Each member of the union is a different way of representing the buffer in code.
 * 
 * * `raw` is the buffer as it exists in the Majora's Mask decomp.
 * * `partitions` is the buffer, seperated into it's two primary regions (header and content).
 * * `data` is the buffer, with it's header and content values listed as individual members.
 * 
 * Do not allocate this on the stack, as you'll likely end up with a StackOverflow. Instead, create a buffer using
 * `EZTR_MsgBuffer_Create()` of one of it's sister functions.
 */
typedef union {
        EZTR_MsgBuffer_Raw raw;
        EZTR_MsgBuffer_Partition partitions;
        EZTR_MsgBuffer_Data data;
} EZTR_MsgBuffer;

/**
 * @brief The type declaration for custom message handle.
 * 
 * These are created using either the `EZTR_DEFINE_CUSTOM_MSG_HANDLE()` macro
 * or the `EZTR_DEFINE_CUSTOM_MSG_HANDLE_NO_EXPORT()` macro in one of your .c files, 
 * outside of any functions.
 * 
 * 
 */
typedef u16 (*EZTR_CustomMsgHandle)(_EZTR_CustomMsgHandleSetter* setter);

/**
 * @brief The function pointer type for message callbacks. 
 * 
 * To easily create functions that match this type, see the `EZTR_MSG_CALLBACK()` macro.
 * 
 */
typedef void (*EZTR_MsgCallback)(EZTR_MsgBuffer* buf, u16 textId, PlayState* play);

/**
 * @brief Used in the message header to indicate the style of textbox used for the message.
 * 
 * You can set the text box type by assigning to the `text_box_type` member of EZTR_MsgData,
 * or by using `EZTR_MsgBuffer_SetTextBoxType()`.
 * 
 */
typedef enum {
    EZTR_STANDARD_TEXT_BOX_I = 0X00,
    EZTR_WOODEN_SIGN_BACKGROUND = 0X01,
    EZTR_TRANSLUSCENT_BLUE_TEXT_BOX = 0X02,
    EZTR_OCARINA_STAFF = 0X03,
    EZTR_INVISIBLE_TEXT_BOX_I = 0X04,
    EZTR_INVISIBLE_TEXT_BOX_II = 0X05,
    EZTR_STANDARD_TEXT_BOX_II = 0X06,
    EZTR_INVISIBLE_TEXT_BOX = 0X07,
    EZTR_BLUE_TEXT_BOX = 0X08,
    EZTR_RED_TEXT_BOX_I = 0X09,
    EZTR_INVISIBLE_TEXT_BOX_III = 0X0A,
    EZTR_INVISIBLE_TEXT_BOX_IV = 0X0B,
    EZTR_INVISIBLE_TEXT_BOX_V = 0X0C,
    EZTR_BOMBERS_NOTEBOOK = 0X0D,
    EZTR_INVISIBLE_TEXT_BOX_VI = 0X0E,
    EZTR_RED_TEXT_BOX_II = 0X0F
} EZTR_TextBoxType;

/**
 * @brief Used in the message header to indicate a display icon for the message.
 * 
 * You can set the display icon by assigning to the `display_icon` member of EZTR_MsgData,
 * or by using `EZTR_MsgBuffer_SetTextBoxDIsplayIcon()`.
 * 
 * Note that the value for not displaying an icon is `EZTR_ICON_NO_ICON`, and NOT 
 * 
 * `EZTR_ICON_NOTHING` or it's variants.
 */
typedef enum {
    EZTR_ICON_NOTHING = 0x00,
    EZTR_ICON_GREEN_RUPEE = 0x01,
    EZTR_ICON_BLUE_RUPEE = 0x02,
    EZTR_ICON_WHITE_RUPEE = 0x03,
    EZTR_ICON_RED_RUPEE = 0x04,
    EZTR_ICON_PURPLE_RUPEE = 0x05,
    EZTR_ICON_WHITE_RUPEE_1 = 0x06,
    EZTR_ICON_ORANGE_RUPEE = 0x07,
    EZTR_ICON_ADULT_WALLET = 0x08,
    EZTR_ICON_GIANTS_WALLET = 0x09,
    EZTR_ICON_RECOVERY_HEART = 0x0A,
    EZTR_ICON_RECOVERY_HEART_1 = 0x0B,
    EZTR_ICON_PIECE_OF_HEART = 0x0C,
    EZTR_ICON_HEART_CONTAINER = 0x0D,
    EZTR_ICON_SMALL_MAGIC_JAR = 0x0E,
    EZTR_ICON_LARGE_MAGIC_JAR = 0x0F,
    EZTR_ICON_RECOVERY_HEART_2 = 0x10,
    EZTR_ICON_STRAY_FAIRY = 0x11,
    EZTR_ICON_RECOVERY_HEART_3 = 0x12,
    EZTR_ICON_RECOVERY_HEART_4 = 0x13,
    EZTR_ICON_BOMB = 0x14,
    EZTR_ICON_BOMB_1 = 0x15,
    EZTR_ICON_BOMB_2 = 0x16,
    EZTR_ICON_BOMB_3 = 0x17,
    EZTR_ICON_BOMB_4 = 0x18,
    EZTR_ICON_DEKU_STICK = 0x19,
    EZTR_ICON_BOMBCHU = 0x1A,
    EZTR_ICON_BOMB_BAG = 0x1B,
    EZTR_ICON_BIG_BOMB_BAG = 0x1C,
    EZTR_ICON_BIGGER_BOMB_BAG = 0x1D,
    EZTR_ICON_HEROS_BOW = 0x1E,
    EZTR_ICON_HEROS_BOW_1 = 0x1F,
    EZTR_ICON_HEROS_BOW_2 = 0x20,
    EZTR_ICON_HEROS_BOW_3 = 0x21,
    EZTR_ICON_QUIVER = 0x22,
    EZTR_ICON_BIG_QUIVER = 0x23,
    EZTR_ICON_BIGGEST_QUIVER = 0x24,
    EZTR_ICON_FIRE_ARROW = 0x25,
    EZTR_ICON_ICE_ARROW = 0x26,
    EZTR_ICON_LIGHT_ARROW = 0x27,
    EZTR_ICON_DEKU_NUT = 0x28,
    EZTR_ICON_DEKU_NUT_1 = 0x29,
    EZTR_ICON_DEKU_NUT_2 = 0x2A,
    EZTR_ICON_NOTHING_1 = 0x2B,
    EZTR_ICON_NOTHING_2 = 0x2C,
    EZTR_ICON_NOTHING_3 = 0x2D,
    EZTR_ICON_NOTHING_4 = 0x2E,
    EZTR_ICON_NOTHING_5 = 0x2F,
    EZTR_ICON_NOTHING_6 = 0x30,
    EZTR_ICON_NOTHING_7 = 0x31,
    EZTR_ICON_HEROS_SHIELD = 0x32,
    EZTR_ICON_MIRROR_SHIELD = 0x33,
    EZTR_ICON_POWDER_KEG = 0x34,
    EZTR_ICON_MAGIC_BEAN = 0x35,
    EZTR_ICON_PICTOGRAPH_BOX = 0x36,
    EZTR_ICON_KOKIRI_SWORD = 0x37,
    EZTR_ICON_RAZOR_SWORD = 0x38,
    EZTR_ICON_GILDED_SWORD = 0x39,
    EZTR_ICON_FIERCE_DEITYS_SWORD = 0x3A,
    EZTR_ICON_GREAT_FAIRYS_SWORD = 0x3B,
    EZTR_ICON_SMALL_KEY = 0x3C,
    EZTR_ICON_BOSS_KEY = 0x3D,
    EZTR_ICON_DUNGEON_MAP = 0x3E,
    EZTR_ICON_COMPASS = 0x3F,
    EZTR_ICON_POWDER_KEG_1 = 0x40,
    EZTR_ICON_HOOKSHOT = 0x41,
    EZTR_ICON_LENS_OF_TRUTH = 0x42,
    EZTR_ICON_PICTOGRAPH_BOX_1 = 0x43,
    EZTR_ICON_FISHING_ROD = 0x44,
    EZTR_ICON_NOTHING_8 = 0x45,
    EZTR_ICON_NOTHING_9 = 0x46,
    EZTR_ICON_NOTHING_10 = 0x47,
    EZTR_ICON_NOTHING_11 = 0x48,
    EZTR_ICON_NOTHING_12 = 0x49,
    EZTR_ICON_NOTHING_13 = 0x4A,
    EZTR_ICON_NOTHING_14 = 0x4B,
    EZTR_ICON_OCARINA_OF_TIME = 0x4C,
    EZTR_ICON_NOTHING_15 = 0x4D,
    EZTR_ICON_NOTHING_16 = 0x4E,
    EZTR_ICON_NOTHING_17 = 0x4F,
    EZTR_ICON_BOMBERS_NOTEBOOK = 0x50,
    EZTR_ICON_NOTHING_18 = 0x51,
    EZTR_ICON_GOLD_SKULLTULA_TOKEN = 0x52,
    EZTR_ICON_NOTHING_19 = 0x53,
    EZTR_ICON_NOTHING_20 = 0x54,
    EZTR_ICON_ODOLWAS_REMAINS = 0x55,
    EZTR_ICON_GOHTS_REMAINS = 0x56,
    EZTR_ICON_GYORGS_REMAINS = 0x57,
    EZTR_ICON_TWINMOLDS_REMAINS = 0x58,
    EZTR_ICON_RED_POTION = 0x59,
    EZTR_ICON_EMPTY_BOTTLE = 0x5A,
    EZTR_ICON_RED_POTION_1 = 0x5B,
    EZTR_ICON_GREEN_POTION = 0x5C,
    EZTR_ICON_BLUE_POTION = 0x5D,
    EZTR_ICON_FAIRYS_SPIRIT = 0x5E,
    EZTR_ICON_DEKU_PRINCESS = 0x5F,
    EZTR_ICON_MILK = 0x60,
    EZTR_ICON_MILK_HALF = 0x61,
    EZTR_ICON_FISH = 0x62,
    EZTR_ICON_BUG = 0x63,
    EZTR_ICON_BLUE_FIRE = 0x64,
    EZTR_ICON_POE = 0x65,
    EZTR_ICON_BIG_POE = 0x66,
    EZTR_ICON_SPRING_WATER = 0x67,
    EZTR_ICON_HOT_SPRING_WATER = 0x68,
    EZTR_ICON_ZORA_EGG = 0x69,
    EZTR_ICON_GOLD_DUST = 0x6A,
    EZTR_ICON_MUSHROOM = 0x6B,
    EZTR_ICON_NOTHING_21 = 0x6C,
    EZTR_ICON_NOTHING_22 = 0x6D,
    EZTR_ICON_SEAHORSE = 0x6E,
    EZTR_ICON_CHATEAU_ROMANI = 0x6F,
    EZTR_ICON_HYLIAN_LOACH = 0x70,
    EZTR_ICON_NOTHING_23 = 0x71,
    EZTR_ICON_NOTHING_24 = 0x72,
    EZTR_ICON_NOTHING_25 = 0x73,
    EZTR_ICON_NOTHING_26 = 0x74,
    EZTR_ICON_NOTHING_27 = 0x75,
    EZTR_ICON_NOTHING_28 = 0x76,
    EZTR_ICON_NOTHING_29 = 0x77,
    EZTR_ICON_DEKU_MASK = 0x78,
    EZTR_ICON_GORON_MASK = 0x79,
    EZTR_ICON_ZORA_MASK = 0x7A,
    EZTR_ICON_FIERCE_DEITY_MASK = 0x7B,
    EZTR_ICON_MASK_OF_TRUTH = 0x7C,
    EZTR_ICON_KAFEIS_MASK = 0x7D,
    EZTR_ICON_ALL_NIGHT_MASK = 0x7E,
    EZTR_ICON_BUNNY_HOOD = 0x7F,
    EZTR_ICON_KEATON_MASK = 0x80,
    EZTR_ICON_GARO_MASK = 0x81,
    EZTR_ICON_ROMANI_MASK = 0x82,
    EZTR_ICON_CIRCUS_LEADERS_MASK = 0x83,
    EZTR_ICON_POSTMANS_HAT = 0x84,
    EZTR_ICON_COUPLES_MASK = 0x85,
    EZTR_ICON_GREAT_FAIRYS_MASK = 0x86,
    EZTR_ICON_GIBDO_MASK = 0x87,
    EZTR_ICON_DON_GEROS_MASK = 0x88,
    EZTR_ICON_KAMAROS_MASK = 0x89,
    EZTR_ICON_CAPTAINS_HAT = 0x8A,
    EZTR_ICON_STONE_MASK = 0x8B,
    EZTR_ICON_BREMEN_MASK = 0x8C,
    EZTR_ICON_BLAST_MASK = 0x8D,
    EZTR_ICON_MASK_OF_SCENTS = 0x8E,
    EZTR_ICON_GIANTS_MASK = 0x8F,
    EZTR_ICON_NOTHING_30 = 0x90,
    EZTR_ICON_CHATEAU_ROMANI_1 = 0x91,
    EZTR_ICON_MILK_1 = 0x92,
    EZTR_ICON_GOLD_DUST_1 = 0x93,
    EZTR_ICON_HYLIAN_LOACH_1 = 0x94,
    EZTR_ICON_SEAHORSE_1 = 0x95,
    EZTR_ICON_MOONS_TEAR = 0x96,
    EZTR_ICON_TOWN_TITLE_DEED = 0x97,
    EZTR_ICON_SWAMP_TITLE_DEED = 0x98,
    EZTR_ICON_MOUNTAIN_TITLE_DEED = 0x99,
    EZTR_ICON_OCEAN_TITLE_DEED = 0x9A,
    EZTR_ICON_NOTHING_31 = 0x9B,
    EZTR_ICON_NOTHING_32 = 0x9C,
    EZTR_ICON_NOTHING_33 = 0x9D,
    EZTR_ICON_NOTHING_34 = 0x9E,
    EZTR_ICON_NOTHING_35 = 0x9F,
    EZTR_ICON_ROOM_KEY = 0xA0,
    EZTR_ICON_SPECIAL_DELIVERY_TO_MAMA = 0xA1,
    EZTR_ICON_NOTHING_36 = 0xA2,
    EZTR_ICON_NOTHING_37 = 0xA3,
    EZTR_ICON_NOTHING_38 = 0xA4,
    EZTR_ICON_NOTHING_39 = 0xA5,
    EZTR_ICON_NOTHING_40 = 0xA6,
    EZTR_ICON_NOTHING_41 = 0xA7,
    EZTR_ICON_NOTHING_42 = 0xA8,
    EZTR_ICON_NOTHING_43 = 0xA9,
    EZTR_ICON_LETTER_TO_KAFEI = 0xAA,
    EZTR_ICON_PENDANT_OF_MEMORIES = 0xAB,
    EZTR_ICON_NOTHING_44 = 0xAC,
    EZTR_ICON_NOTHING_45 = 0xAD,
    EZTR_ICON_NOTHING_46 = 0xAE,
    EZTR_ICON_NOTHING_47 = 0xAF,
    EZTR_ICON_NOTHING_48 = 0xB0,
    EZTR_ICON_NOTHING_49 = 0xB1,
    EZTR_ICON_NOTHING_50 = 0xB2,
    EZTR_ICON_TINGLES_MAP = 0xB3,
    EZTR_ICON_TINGLES_MAP_1 = 0xB4,
    EZTR_ICON_TINGLES_MAP_2 = 0xB5,
    EZTR_ICON_TINGLES_MAP_3 = 0xB6,
    EZTR_ICON_TINGLES_MAP_4 = 0xB7,
    EZTR_ICON_TINGLES_MAP_5 = 0xB8,
    EZTR_ICON_TINGLES_MAP_6 = 0xB9,
    EZTR_ICON_NOTHING_51 = 0xBA,
    EZTR_ICON_NOTHING_52 = 0xBB,
    EZTR_ICON_NOTHING_53 = 0xBC,
    EZTR_ICON_NOTHING_54 = 0xBD,
    EZTR_ICON_NOTHING_55 = 0xBE,
    EZTR_ICON_NOTHING_56 = 0xBF,
    EZTR_ICON_NOTHING_57 = 0xC0,
    EZTR_ICON_NOTHING_58 = 0xC1,
    EZTR_ICON_NOTHING_59 = 0xC2,
    EZTR_ICON_NOTHING_60 = 0xC3,
    EZTR_ICON_NOTHING_61 = 0xC4,
    EZTR_ICON_NOTHING_62 = 0xC5,
    EZTR_ICON_NOTHING_63 = 0xC6,
    EZTR_ICON_NOTHING_64 = 0xC7,
    EZTR_ICON_NOTHING_65 = 0xC8,
    EZTR_ICON_NOTHING_66 = 0xC9,
    EZTR_ICON_NOTHING_67 = 0xCA,
    EZTR_ICON_NOTHING_68 = 0xCB,
    EZTR_ICON_NOTHING_69 = 0xCC,
    EZTR_ICON_NOTHING_70 = 0xCD,
    EZTR_ICON_NOTHING_71 = 0xCE,
    EZTR_ICON_NOTHING_72 = 0xCF,
    EZTR_ICON_NOTHING_73 = 0xD0,
    EZTR_ICON_NOTHING_74 = 0xD1,
    EZTR_ICON_NOTHING_75 = 0xD2,
    EZTR_ICON_NOTHING_76 = 0xD3,
    EZTR_ICON_NOTHING_77 = 0xD4,
    EZTR_ICON_NOTHING_78 = 0xD5,
    EZTR_ICON_NOTHING_79 = 0xD6,
    EZTR_ICON_NOTHING_80 = 0xD7,
    EZTR_ICON_SMALL_BLACK_LINE = 0xD8,
    EZTR_ICON_SMALL_BLACK_LINE_1 = 0xD9,
    EZTR_ICON_SMALL_BLACK_LINE_2 = 0xDA,
    EZTR_ICON_SMALL_BLACK_LINE_3 = 0xDB,
    EZTR_ICON_ANJU = 0xDC,
    EZTR_ICON_KAFEI = 0xDD,
    EZTR_ICON_CURIOSITY_SHOP_OWNER = 0xDE,
    EZTR_ICON_BOMB_SHOP_OWNERS_MOTHER = 0xDF,
    EZTR_ICON_ROMANI = 0xE0,
    EZTR_ICON_CREMIA = 0xE1,
    EZTR_ICON_MAYOR_DOTOUR = 0xE2,
    EZTR_ICON_MADAME_AROMA = 0xE3,
    EZTR_ICON_TOTO = 0xE4,
    EZTR_ICON_GORMAN = 0xE5,
    EZTR_ICON_POSTMAN = 0xE6,
    EZTR_ICON_ROSA_SISTERS = 0xE7,
    EZTR_ICON_TOILET_HAND = 0xE8,
    EZTR_ICON_GRANNY = 0xE9,
    EZTR_ICON_KAMARO = 0xEA,
    EZTR_ICON_GROG = 0xEB,
    EZTR_ICON_GORMAN_BROTHERS = 0xEC,
    EZTR_ICON_SHIRO = 0xED,
    EZTR_ICON_GURU_GURU = 0xEE,
    EZTR_ICON_BOMBERS = 0xEF,
    EZTR_ICON_EXCLAMATION_MARK = 0xF0,
    EZTR_ICON_NOTHING_81 = 0xF1,
    EZTR_ICON_NOTHING_82 = 0xF2,
    EZTR_ICON_NOTHING_83 = 0xF3,
    EZTR_ICON_NOTHING_84 = 0xF4,
    EZTR_ICON_NOTHING_85 = 0xF5,
    EZTR_ICON_NOTHING_86 = 0xF6,
    EZTR_ICON_NOTHING_87 = 0xF7,
    EZTR_ICON_NOTHING_88 = 0xF8,
    EZTR_ICON_NOTHING_89 = 0xF9,
    EZTR_ICON_NOTHING_90 = 0xFA,
    EZTR_ICON_NOTHING_91 = 0xFB,
    EZTR_ICON_NOTHING_92 = 0xFC,
    EZTR_ICON_NOTHING_93 = 0xFD,
    EZTR_ICON_NO_ICON = 0xFE,
    EZTR_ICON_MAX = 0xFE
} EZTR_TextBoxIcon;

/** @}*/


/**
 * \defgroup Control_Code_Macros 
 * \brief Macros for the various control codes and non-printable bytes used in the Majora's Mask text encoding.
 * 
 * Most of this information was pulled from [the CloudModding wiki](https://wiki.cloudmodding.com/mm/Text_Format).
 * 
 * @{
 */
/**
 * @brief Following Text Color: Default
 * 
 * Default is usually white, but may be black (ie: for notebook updates).
 * 
 */
#define EZTR_CC_COLOR_DEFAULT "\x00"

/**
 * @brief Following Text Color: Red
 */
#define EZTR_CC_COLOR_RED "\x01"

/**
 * @brief Following Text Color: Green
 */
#define EZTR_CC_COLOR_GREEN "\x02"

/**
 * @brief Following Text Color: Blue
 */
#define EZTR_CC_COLOR_BLUE "\x03"

/**
 * @brief Following Text Color: Yellow
 */
#define EZTR_CC_COLOR_YELLOW "\x04"

/**
 * @brief Following Text Color: Turquoise
 */
#define EZTR_CC_COLOR_LIGHTBLUE "\x05"

/**
 * @brief Following Text Color: Pink
 */
#define EZTR_CC_COLOR_PINK "\x06"

/**
 * @brief Following Text Color: Silver
 */
#define EZTR_CC_COLOR_SILVER "\x07"

/**
 * @brief Following Text Color: Orange
 */
#define EZTR_CC_COLOR_ORANGE "\x08"

/**
 * @brief Slows down text (not used)
 * 
 * Text normally prints 2 letters at a time. 0A acts as a null character. So 0A0A prints nothing not even a space when normally 2 letters are printed.
 * 
 */
#define EZTR_CC_TEXT_SPEED "\x0a"

/**
 * @brief Print: Hits Required to Win Jungle Cruise Reward
 */
#define EZTR_CC_HS_BOAT_ARCHERY "\x0b"

/**
 * @brief Print: Stray Fairies Collected in Current Dungeon
 * 
 */
#define EZTR_CC_STRAY_FAIRIES "\x0c"

/**
 * @brief Print: Gold Skulltulas Collected in Current Spider House
 */
#define EZTR_CC_TOKENS "\x0d"

/**
 * @brief Print: 0
 */
#define EZTR_CC_POINTS_TENS "\x0e"

/**
 * @brief Print: 0
 */
#define EZTR_CC_POINTS_THOUSANDS "\x0f"

/**
 * @brief Box Break I
 * 
 * Used when four lines of text have been printed, but can technically be used anywhere. More robust than 12/000B? [?]
 * 
 */
#define EZTR_CC_BOX_BREAK "\x10"

/**
 * @brief Line Break
 */
#define EZTR_CC_NEWLINE "\x11"

/**
 * @brief Box Break II
 * 
 * Used when three lines of text have been printed. Usually preceded by a 13/000C character.
 * 
 */
#define EZTR_CC_BOX_BREAK2 "\x12"

/**
 * @brief Reset Cursor Position to Start of Current Line
 * 
 * Used as a filler when there are fewer than four lines of text. Usually preceded by a newline when two lines of text have been printed.
 * 
 */
#define EZTR_CC_CARRIAGE_RETURN "\x13"

/**
 * @brief Print: xx Spaces
 * 
 * The value of the next byte will control how many spaces to print.
 * 
 */
#define EZTR_CC_SHIFT "\x14"

/**
 * @brief Print: %c Spaces
 * 
 * The value of the next byte will control how many spaces to print.
 * This macro adds a `%%c` flag after the control code, enabling you to set the argument byte using EZTR's printf handling.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 */
#define EZTR_CC_SHIFT_ARGC "\x14%c"

/**
 * @brief Disable Text Skip I
 * 
 * Triangle box. Does not play sound.
 * 
 */
#define EZTR_CC_CONTINUE "\x15"

/**
 * @brief Print: Player Name
 */
#define EZTR_CC_NAME "\x16"

/**
 * @brief Enable: Instantaneous Text
 */
#define EZTR_CC_QUICKTEXT_ENABLE "\x17"

/**
 * @brief Disable: Instantaneous Text
 */
#define EZTR_CC_QUICKTEXT_DISABLE "\x18"

/**
 * @brief Disable Text Skip II
 * 
 * Triangle box. Plays "Text Finished" sound.
 * 
 */
#define EZTR_CC_EVENT "\x19"

/**
 * @brief Disable Text Box Close
 * 
 * Used for shop item descriptions.
 * 
 */
#define EZTR_CC_PERSISTENT "\x1a"

/**
 * @brief Delay for xxxx Before Printing Remaining Text
 * 
 * The next two bytes determine the length of the delay.
 * 
 */
#define EZTR_CC_BOX_BREAK_DELAYED "\x1b"

/**
 * @brief Delay for xxxx Before Printing Remaining Text
 * 
 * The next two bytes determine the length of the delay.
 * This macro adds a `%%w` flag after the control code, enabling you to set the argument bytes using EZTR's printf handling.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 */
#define EZTR_CC_BOX_BREAK_DELAYED_ARGW "\x1b%w"


/**
 * @brief Keep Text on Screen for xxxx Before Closing
 * 
 * The next two bytes determine the length of the delay.
 * 
 * Player can move around while text is displayed.
 * 
 */
#define EZTR_CC_FADE "\x1c"

/**
 * @brief Keep Text on Screen for xxxx Before Closing
 * 
 * The next two bytes determine the length of the delay.
 * This macro adds a `%%w` flag after the control code, enabling you to set the argument bytes using EZTR's printf handling.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * Player can move around while text is displayed.
 * 
 */
#define EZTR_CC_FADE_ARGW "\x1c%w"

/**
 * @brief Delay for xxxx Before Ending Conversation
 * 
 * The next two bytes determine the length of the delay.
 * 
 */
#define EZTR_CC_FADE_SKIPPABLE "\x1d"

/**
 * @brief Delay for xxxx Before Ending Conversation
 *
 * The next two bytes determine the length of the delay.
 * This macro adds a `%%w` flag after the control code, enabling you to set the argument bytes using EZTR's printf handling.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 */
#define EZTR_CC_FADE_SKIPPABLE_ARGW "\x1d%w"

/**
 * @brief Play Sound Effect xxxx
 * 
 * The next two bytes will be the ID of the sound effect. The ID values are the same as the ones used with `Audio_PlaySfx`.
 * 
 */
#define EZTR_CC_SFX "\x1e"

/**
 * @brief Play Sound Effect xxxx
 * 
 * The next two bytes will be the ID of the sound effect. The ID values are the same as the ones used with `Audio_PlaySfx`.
 * This macro adds a `%%w` flag after the control code, enabling you to set the argument bytes using EZTR's printf handling.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 */
#define EZTR_CC_SFX_ARGW "\x1e%w"

/**
 * @brief Delay for xxxx Before Resuming Text Flow
 * 
 * The next two bytes determine the length of the delay.
 * 
 */
#define EZTR_CC_DELAY "\x1f"

/**
 * @brief Delay for xxxx Before Resuming Text Flow
 * 
 * The next two bytes determine the length of the delay.
 * This macro adds a `%%w` flag after the control code, enabling you to set the argument bytes using EZTR's printf handling.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 */
#define EZTR_CC_DELAY_ARGW "\x1f%w"

/**
 * @brief Displays the A Button icon
 */
#define EZTR_CC_BTN_A "\xb0"

/**
 * @brief Displays the B button icon
 */
#define EZTR_CC_BTN_B "\xb1"

/**
 * @brief Displays the C buttons icon
 */
#define EZTR_CC_BTN_C "\xb2"

/**
 * @brief Displays the L button icon
 */
#define EZTR_CC_BTN_L "\xb3"

/**
 * @brief Displays the R button icon
 */
#define EZTR_CC_BTN_R "\xb4"

/**
 * @brief Displays the Z button icon
 */
#define EZTR_CC_BTN_Z "\xb5"

/**
 * @brief Displays the C-up button icon
 */
#define EZTR_CC_BTN_CUP "\xb6"

/**
 * @brief Displays the C-down button icon
 */
#define EZTR_CC_BTN_CDOWN "\xb7"

/**
 * @brief Displays the C-left button icon
 */
#define EZTR_CC_BTN_CLEFT "\xb8"

/**
 * @brief Displays the C-right button icon
 */
#define EZTR_CC_BTN_CRIGHT "\xb9"

/**
 * @brief DDisplays the Z targeting icon
 */
#define EZTR_CC_Z_TARGET "\xba"

/**
 * @brief Displays the Control Pad icon
 */
#define EZTR_CC_CONTROL_PAD "\xbb"

/**
 * @brief Message End Marker
 * 
 * This is the termination character for message content.
 * 
 */
#define EZTR_CC_END "\xbf"

/**
 * @brief Ocarina Song Failure
 * 
 * Draws red X across the text box on the screen & centers text vertically/pushes it out.
 * 
 */
#define EZTR_CC_BACKGROUND "\xc1"

/**
 * @brief Initialize Selection: Two Choices
 */
#define EZTR_CC_TWO_CHOICE "\xc2"

/**
 * @brief Initialize Selection: Three Choices
 */
#define EZTR_CC_THREE_CHOICE "\xc3"

/**
 * @brief Print: Postman's Counting Game Results
 */
#define EZTR_CC_TIMER_POSTMAN "\xc4"

/**
 * @brief Timer Value
 */
#define EZTR_CC_TIMER_MINIGAME_1 "\xc5"

/**
 * @brief Timer Value
 */
#define EZTR_CC_TIMER_2 "\xc6"

/**
 * @brief Print: Remaining Time Till Moon Falls (Clock Tower Roof)
 */
#define EZTR_CC_TIMER_MOON_CRASH "\xc7"

/**
 * @brief Print: Deku Scrub Playground Results
 */
#define EZTR_CC_TIMER_MINIGAME_2 "\xc8"

/**
 * @brief Timer Value
 */
#define EZTR_CC_TIMER_ENV_HAZARD "\xc9"

/**
 * @brief Timer Value
 */
#define EZTR_CC_TIME "\xca"

/**
 * @brief Print: Shooting Gallery Minigame Results
 */
#define EZTR_CC_CHEST_FLAGS "\xcb"

/**
 * @brief Display Prompt: Withdraw or Deposit Rupees
 */
#define EZTR_CC_INPUT_BANK "\xcc"

/**
 * @brief Print: Number of Rupees Entered or Bet
 */
#define EZTR_CC_RUPEES_SELECTED "\xcd"

/**
 * @brief Print: Total Rupees in Bank or Won by Betting
 */
#define EZTR_CC_RUPEES_TOTAL "\xce"

/**
 * @brief Print: Time Remaining in Hours & Minutes
 */
#define EZTR_CC_TIME_UNTIL_MOON_CRASH "\xcf"

/**
 * @brief Display Prompt: Rupees to Bet
 */
#define EZTR_CC_INPUT_DOGGY_RACETRACK_BET "\xd0"

/**
 * @brief Display Prompt: Bombers' Code
 */
#define EZTR_CC_INPUT_BOMBER_CODE "\xd1"

/**
 * @brief Item Prompt
 * 
 * Delays closing the text box.
 * 
 */
#define EZTR_CC_PAUSE_MENU "\xd2"

/**
 * @brief [?]
 */
#define EZTR_CC_TIME_SPEED "\xd3"

/**
 * @brief Print: Song of Soaring Destination
 */
#define EZTR_CC_OWL_WARP "\xd4"

/**
 * @brief Display Prompt: Lottery Number
 */
#define EZTR_CC_INPUT_LOTTERY_CODE "\xd5"

/**
 * @brief Print: 123456
 * 
 * Each number's color is indicative of the Spider House mask code.
 * 
 */
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE "\xd6"

/**
 * @brief Print: Remaining Stray Fairies in Woodfall Temple
 */
#define EZTR_CC_STRAY_FAIRIES_LEFT_WOODFALL "\xd7"

/**
 * @brief Print: Remaining Stray Fairies in Snowhead Temple
 */
#define EZTR_CC_STRAY_FAIRIES_LEFT_SNOWHEAD "\xd8"

/**
 * @brief Print: Remaining Stray Fairies in Great Bay Temple
 */
#define EZTR_CC_STRAY_FAIRIES_LEFT_GREAT_BAY "\xd9"

/**
 * @brief Print: Remaining Stray Fairies in Stone Tower Temple
 */
#define EZTR_CC_STRAY_FAIRIES_LEFT_STONE_TOWER "\xda"

/**
 * @brief Print: Jungle Cruise Minigame Results
 */
#define EZTR_CC_POINTS_BOAT_ARCHERY "\xdb"

/**
 * @brief Print: Winning Lottery Numbers
 */
#define EZTR_CC_LOTTERY_CODE "\xdc"

/**
 * @brief Print: Player's Lottery Numbers
 */
#define EZTR_CC_LOTTERY_CODE_GUESS "\xdd"

/**
 * @brief Print: Item Value in Rupees
 * 
 * Default: 51 Rupees
 * 
 */
#define EZTR_CC_HELD_ITEM_PRICE "\xde"

/**
 * @brief Print: Bombers' Code
 */
#define EZTR_CC_BOMBER_CODE "\xdf"

/**
 * @brief End Conversation
 * 
 * Used exclusively for NPCs. Usually followed by a BF/0500 command.
 * 
 */
#define EZTR_CC_EVENT2 "\xe0"

/**
 * @brief Print: Color of Oceanside Spider House Mask 1
 */
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_1 "\xe1"

/**
 * @brief Print: Color of Oceanside Spider House Mask 2
 */
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_2 "\xe2"

/**
 * @brief Print: Color of Oceanside Spider House Mask 3
 */
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_3 "\xe3"

/**
 * @brief Print: Color of Oceanside Spider House Mask 4
 */
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_4 "\xe4"

/**
 * @brief Print: Color of Oceanside Spider House Mask 5
 */
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_5 "\xe5"

/**
 * @brief Print: Color of Oceanside Spider House Mask 6
 */
#define EZTR_CC_SPIDER_HOUSE_MASK_CODE_6 "\xe6"

/**
 * @brief Print: Remaining Time Till Moon Falls
 */
#define EZTR_CC_HOURS_UNTIL_MOON_CRASH "\xe7"

/**
 * @brief Print: Remaining Time Till Morning in Hours & Minutes
 */
#define EZTR_CC_TIME_UNTIL_NEW_DAY "\xe8"

/**
 * @brief Print: Total Rupees in Bank
 */
#define EZTR_CC_HS_POINTS_BANK_RUPEES "\xf0"

/**
 * @brief Print: 0
 */
#define EZTR_CC_HS_POINTS_UNK_1 "\xf1"

/**
 * @brief Print: 0
 */
#define EZTR_CC_HS_POINTS_FISHING "\xf2"

/**
 * @brief Print: 0"10'
 */
#define EZTR_CC_HS_TIME_BOAT_ARCHERY "\xf3"

/**
 * @brief Print: :0"00'
 */
#define EZTR_CC_HS_TIME_HORSE_BACK_BALLOON "\xf4"

/**
 * @brief Print: Timer or Highscore in 00"00' Format [?]
 * 
 * SRAM Offset: 0xEF2
 * 
 */
#define EZTR_CC_HS_TIME_LOTTERY_GUESS "\xf5"

/**
 * @brief Print: Town Shooting Gallery Highscore
 */
#define EZTR_CC_HS_TOWN_SHOOTING_GALLERY "\xf6"

/**
 * @brief Print: 00'00"00
 */
#define EZTR_CC_HS_UNK_1 "\xf7"

/**
 * @brief Print: Magic Bean Price
 */
#define EZTR_CC_HS_UNK_3_LOWER "\xf8"

/**
 * @brief Print: Epona Balloon Archery Highscore I
 */
#define EZTR_CC_HS_HORSE_BACK_BALLOON "\xf9"

/**
 * @brief Print: Deku Scrub Playground Highscore (Day 1)
 */
#define EZTR_CC_HS_DEKU_PLAYGROUND_DAY_1 "\xfa"

/**
 * @brief Print: Deku Scrub Playground Highscore (Day 2)
 */
#define EZTR_CC_HS_DEKU_PLAYGROUND_DAY_2 "\xfb"

/**
 * @brief Print: Deku Scrub Playground Highscore (Day 3)
 */
#define EZTR_CC_HS_DEKU_PLAYGROUND_DAY_3 "\xfc"

/**
 * @brief Print: Broken Character [?]
 */
#define EZTR_CC_DEKU_PLAYGROUND_NAME_DAY_1 "\xfd"

/**
 * @brief [?]
 */
#define EZTR_CC_DEKU_PLAYGROUND_NAME_DAY_2 "\xfe"

/**
 * @brief [?]
 */
#define EZTR_CC_DEKU_PLAYGROUND_NAME_DAY_3 "\xff"

/** @}*/

/**
 * \defgroup Basic_Replacement
 * \brief functions for replacing vanilla messages.
 * 
 * These functions are currently your primary means of declaring replacements, using Majora's Mask's built-in dialog handling.
 * 
 * @{
 */


/**
 * @brief Declare a replacement of a vanilla message by copying from a buffer defined by the user.
 * 
 * If you've defined a message using the MsgBuffer/MsgSContent functions, you can use this to set it (or, rather,
 * a copy of it) as a replacement for a vanilla message.
 * 
 * Note that this function is meant for replacing vanilla messages only. It will not allow you to use a textId greater 
 * than 0x354C, as that is the highest textId value found in the vanilla game.
 * If you wish to create/modify a custom message, see `EZTR_Basic_AddCustomBuffer()` and EZTR_Basic_ReplaceCustomBuffer()`
 * 
 * @param textId The id of the vanilla message you wish to replace.
 * @param buf The message buffer to copy from.
 * @param callback A function pointer to call right before this text is displayed. Useful for dynamic text. Set as NULL if
 * you don't want to use a callback. See `EZTR_MsgCallback` for more information.
 */
EZTR_IMPORT(void EZTR_Basic_ReplaceBuffer(u16 textId, EZTR_MsgBuffer* buf, EZTR_MsgCallback callback));

/**
 * @brief Declare a replacement of a vanilla message by defining the header attributes and message content.
 * 
 * This is probably the easiest method to declare replacement text. 
 * 
 * The `content` argument uses EZTR's printf formatting arguments, specified after `callback`. 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * Note that this function is meant for replacing vanilla messages only. It will not allow you to use a textId greater 
 * than 0x354C, as that is the highest textId value found in the vanilla game.
 * If you wish to create/modify a custom message, see `EZTR_Basic_AddCustomText()` and EZTR_Basic_ReplaceCustomText()`
 * 
 * @param textId The id of the vanilla message you wish to replace.
 * @param text_box_type The style of textbox to display. Use the `EZTR_TextBoxType` enum for more readable values.
 * @param text_box_y_pos The vertical position of the textbox on-screen.
 * @param display_icon Displays an icon in the textbox. Use the `EZTR_TextBoxIcon` enum for more readable values. 
 * Use `EZTR_ICON_NO_ICON` for no icon.
 * @param next_message_id The next message to display. If there is no next message, or the next message is determined by code,
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param first_item_rupees The price of the first item being offered for sale, if one exists. If there is no item, 
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param second_item_rupees The price of the second item being offered for sale, if one exists. If there is no item, 
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param pipe_escape_bytes If true, `content` is passed through `EZTR_MsgBuffer_Sprintf("%m", content)`. If false, 
 * then `content` is copied directly into storage.
 * @param content The new text content to display. Accepts valid EZTR printf type specifiers. If you want empty content (for use with dynamic messages), use "\xBF".
 * @param callback A function pointer to call right before this text is displayed. Useful for dynamic messages. 
 * Set as NULL if you don't want to use a callback. See `EZTR_MsgCallback` for more information.
 * @param ... variable arguments, using EZTR's printf implementation. 
 */
EZTR_IMPORT(void EZTR_Basic_ReplaceText(
    u16 textId, 
    u8 text_box_type, 
    u8 text_box_y_pos, 
    u8 display_icon, 
    u16 next_message_id, 
    u16 first_item_rupees, 
    u16 second_item_rupees, 
    bool pipe_escape_bytes, 
    char* content,
    EZTR_MsgCallback callback,
    ...
));

/**
 * @brief Declare a replacement of a vanilla message, where the replacement message is empty.
 * 
 * This is primarily used if you want the message to be completely dynamically generated.
 * 
 * Note that this function is meant for replacing vanilla messages only. It will not allow you to use a textId greater 
 * than 0x354C, as that is the highest textId value found in the vanilla game.
 * If you wish to create/modify a custom message, see `EZTR_Basic_AddCustomTextEmpty()` and EZTR_Basic_ReplaceCustomTextEmpty()`
 * 
 * @param textId The id of the vanilla message you wish to replace.
 * @param callback A function pointer to call right before this text is displayed, in which you will construct the 
 * complete message buffer dynamically. See `EZTR_MsgCallback` for more information.
 */
EZTR_IMPORT(void EZTR_Basic_ReplaceTextEmpty(u16 textId, EZTR_MsgCallback callback));
/** @} */

/** 
 * \defgroup Basic_CustomMessages 
 * \brief Functions for creating and replacing new messages.
 * @{
 * 
 */

/**
 * @brief Declare a brand new (i.e: custom) message by copying from a buffer defined by the user.
 * 
 * If you've defined a message using the MsgBuffer/MsgSContent functions, you can use this to set it (or, rather,
 * a copy of it) as the new message.
 * 
 * To avoid potential ID conflicts between mods, the u16 textId for this message will be assigned by EZTR and 
 * will be accessable via the handle. See `EZTR_CustomMsgHandle` for more information on how custom message handles work.
 * 
 * @param handle The handle for the new message. 
 * @param buf The message buffer to copy from.
 * @param callback A function pointer to call right before this text is displayed. Useful for dynamic text. Set as NULL if
 * you don't want to use a callback. See `EZTR_MsgCallback` for more information.
 */
EZTR_IMPORT(void EZTR_Basic_AddCustomBuffer(EZTR_CustomMsgHandle handle, EZTR_MsgBuffer* buf, EZTR_MsgCallback callback));

/**
 * @brief Declare a brand new (i.e: custom) message by defining the header attributes and message content.
 * 
 * This is probably the easiest method to declare new messages. 
 * 
 * The `content` argument uses EZTR's printf formatting arguments, specified after `callback`. 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior. 
 * 
 * To avoid potential ID conflicts between mods, the u16 textId for this message will be assigned by EZTR and 
 * will be accessable via the handle. See `EZTR_CustomMsgHandle` for more information on how custom message handles work.
 * 
 * @param handle The handle for the new message. 
 * @param text_box_type The style of textbox to display. Use the `EZTR_TextBoxType` enum for more readable values.
 * @param text_box_y_pos The vertical position of the textbox on-screen.
 * @param display_icon Displays an icon in the textbox. Use the `EZTR_TextBoxIcon` enum for more readable values. 
 * Use `EZTR_ICON_NO_ICON` for no icon.
 * @param next_message_id The next message to display. If there is no next message, or the next message is determined by code,
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param first_item_rupees The price of the first item being offered for sale, if one exists. If there is no item, 
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param second_item_rupees The price of the second item being offered for sale, if one exists. If there is no item, 
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param pipe_escape_bytes If true, `content` is passed through `EZTR_MsgBuffer_Sprintf("%m", content)`. If false, 
 * then `content` is copied directly into storage.
 * @param content The new text content to display. If you want empty content (for use with dynamic messages), use "\xBF".
 * @param callback A function pointer to call right before this text is displayed. Useful for dynamic messages. 
 * Set as NULL if you don't want to use a callback. See `EZTR_MsgCallback` for more information.
 * @param ... variable arguments, using EZTR's printf implementation. 
 */
EZTR_IMPORT(void EZTR_Basic_AddCustomText(EZTR_CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, EZTR_MsgCallback callback, ...));

/**
 * @brief Declare a brand new (i.e: custom) message, where the replacement message is empty.
 * 
 * This is primarily used if you want the message to be completely dynamically generated.
 * 
 * To avoid potential ID conflicts between mods, the u16 textId for this message will be assigned by EZTR and 
 * will be accessable via the handle. See `EZTR_CustomMsgHandle` for more information on how custom message handles work.
 * 
 * @param handle The handle for the new message.
 * @param callback A function pointer to call right before this text is displayed, in which you will construct the 
 * complete message buffer dynamically. See `EZTR_MsgCallback` for more information.
 */
EZTR_IMPORT(void EZTR_Basic_AddCustomTextEmpty(EZTR_CustomMsgHandle handle, EZTR_MsgCallback callback));

/**
 * @brief Declare a replacement for a custom message by copying from a buffer defined by the user.
 * 
 * If you've defined a message using the MsgBuffer/MsgSContent functions, you can use this to set it (or, rather,
 * a copy of it) as the new message.
 * 
 * @deprecated since 2.3.0. The new handling of message replacement order makes this unnecessary. 
 * As of this version, this function just wraps `EZTR_Basic_AddCustomBuffer` for compatability.
 * 
 * @param handle The handle for the message being replaced. 
 * @param buf The message buffer to copy from.
 * @param callback A function pointer to call right before this text is displayed. Useful for dynamic text. Set as NULL if
 * you don't want to use a callback. See `EZTR_MsgCallback` for more information.
 */
EZTR_IMPORT(void EZTR_Basic_ReplaceCustomBuffer(EZTR_CustomMsgHandle handle, EZTR_MsgBuffer* buf, EZTR_MsgCallback callback));

/**
 * @brief Declare a replacement of a custom message by defining the header attributes and message content.
 * 
 * This is probably the easiest method to replace custom messages, even those defined by other mods.
 * 
 * The `content` argument uses EZTR's printf formatting arguments, specified after `callback`.
 * See \ref prinf_functions for more information on EZTR's custom printf behavior. 
 * 
 * @deprecated since 2.3.0. The new handling of message replacement order makes this unnecessary. 
 * As of this version, this function just wraps `EZTR_Basic_AddCustomText` for compatability.
 * 
 * @param handle The handle for the message being replaced. 
 * @param text_box_type The style of textbox to display. Use the `EZTR_TextBoxType` enum for more readable values.
 * @param text_box_y_pos The vertical position of the textbox on-screen.
 * @param display_icon Displays an icon in the textbox. Use the `EZTR_TextBoxIcon` enum for more readable values. 
 * Use `EZTR_ICON_NO_ICON` for no icon.
 * @param next_message_id The next message to display. If there is no next message, or the next message is determined by code,
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param first_item_rupees The price of the first item being offered for sale, if one exists. If there is no item, 
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param second_item_rupees The price of the second item being offered for sale, if one exists. If there is no item, 
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param pipe_escape_bytes If true, `content` is passed through `EZTR_MsgBuffer_Sprintf("%m", content)`. If false, 
 * then `content` is copied directly into storage.
 * @param content The new text content to display. If you want empty content (for use with dynamic messages), use "\xBF".
 * @param callback A function pointer to call right before this text is displayed. Useful for dynamic messages. 
 * Set as NULL if you don't want to use a callback. See `EZTR_MsgCallback` for more information.
 * @param ... variable arguments, using EZTR's printf implementation. 
 */
EZTR_IMPORT(void EZTR_Basic_ReplaceCustomText(EZTR_CustomMsgHandle handle, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees, bool pipe_escape_bytes, char* content, EZTR_MsgCallback callback, ...));

/**
 * @brief Declare a replacement of a custom message, where the replacement message is empty.
 * 
 * This is primarily used if you want the message to be completely dynamically generated.
 * 
 * @deprecated since 2.3.0. The new handling of message replacement order makes this unnecessary. 
 * As of this version, this function just wraps `EZTR_Basic_AddCustomTextEmpty` for compatability.
 * 
 * @param handle The handle for the message being replaced.
 * @param callback A function pointer to call right before this text is displayed, in which you will construct the 
 * complete message buffer dynamically. See `EZTR_MsgCallback` for more information.
 */
EZTR_IMPORT(void EZTR_Basic_ReplaceCustomTextEmpty(EZTR_CustomMsgHandle handle, EZTR_MsgCallback callback));

/** @} */

/**
 * \defgroup MsgBuffer 
 * \brief Functions for high-level message buffer operations.
 * @{
 */


/**
 * @brief Creates a new message buffer object on the heap.
 * 
 * The created buffer will be have a default header and an empty content region.
 * You need to free any buffer you create using this function with `EZTR_MsgBuffer_Destroy()`, 
 * or else you will create a memory leak.
 * 
 * @return MsgBuffer* A pointer to the buffer you created.
 */
EZTR_IMPORT(EZTR_MsgBuffer* EZTR_MsgBuffer_Create());

/**
 * @brief Creates a new message buffer object on the heap, and copies `src` into it.
 * 
 * Equivalent to `buf = EZTR_MsgBuffer_Create(); EZTR_MsgBuffer_Copy(buf);`
 * 
 * You need to free any buffer you create using this function with `EZTR_MsgBuffer_Destroy()`, 
 * or else you will create a memory leak.
 * 
 * @param src The content to copy into the buffer. Expected to have a header region, and be terminated with '\xBF'.
 * @return MsgBuffer*  A pointer to the buffer you created.
 */
EZTR_IMPORT(EZTR_MsgBuffer* EZTR_MsgBuffer_CreateFromStr(char* src));

/**
 * @brief Creates a new message buffer object on the heap, and copies `src` into it for up to `len` bytes.
 * 
 * Equivalent to `buf = EZTR_MsgBuffer_Create(); EZTR_MsgBuffer_CopyN(buf, len);`
 * 
 * You need to free any buffer you create using this function with `EZTR_MsgBuffer_Destroy()`, 
 * or else you will create a memory leak.
 * 
 * @param src The content to copy into the buffer. Expected to have a header region, and be terminated with '\xBF'.
 * @param len The maximum number of bytes to copy. If a '\xBF` is encountered in the content region of `src`, the function will stop copying before `len` is reached.
 * @return MsgBuffer* A pointer to the buffer you created.
 */
EZTR_IMPORT(EZTR_MsgBuffer* EZTR_MsgBuffer_CreateFromStrN(char* src, size_t len));

/**
 * @brief Frees/destroys a message buffer.
 * 
 * EZTR only expects you to destroy buffers that you create youself using one of the above functions.
 * Buffers given to you by in `MsgCallback` functions are created and destroyed by EZTR.
 * 
 * @param buf A pointer to the message buffer to destroy.
 */
EZTR_IMPORT(void EZTR_MsgBuffer_Destroy(EZTR_MsgBuffer* buf));

// Copy:
/**
 * @brief Copies data from `src` into the message buffer `dst`.
 * 
 * Unlike something like `strcoy()`, this method is safe as long as dst is a full-sized message buffer,
 * as it will not copy beyond the message buffer size.
 * 
 * Because `src` is expected to have a header region, the message termination characters '\xBF' are ignored
 * for the first 11 bytes.
 * 
 * @param dst The message buffer to copy into.
 * @param src The data to copy. If you want to copy from another message buffer, use `src->raw.schar` or typecast src as `char*`.
 * @return u32 The number of bytes copied.
 */
EZTR_IMPORT(u32 EZTR_MsgBuffer_Copy(EZTR_MsgBuffer* dst, char* src));

 /**
 * @brief Copies data from `src` into the message buffer `dst`, up to `len` bytes.
 * 
 * Because `src` is expected to have a header region, the message termination characters '\xBF' are ignored
 * for the first 11 bytes.
 * 
 * @param dst The message buffer to copy into.
 * @param src The data to copy. If you want to copy from another message buffer, use `src->raw.schar` or typecast src as `char*`.
 * @param len The maximum number of bytes to copy. If a '\xBF` is encountered in the content region of `src`, the function 
 * will stop copying before `len` is reached.
 * @return u32 The number of bytes copied.
 */
EZTR_IMPORT(u32 EZTR_MsgBuffer_NCopy(EZTR_MsgBuffer* dst, char* src, size_t len));

/**
 * @brief Gets the size of the message buffer's stored data, in bytes.
 * 
 * Does not include the termination character '\xBF'.
 * 
 * @param buf The buffer to measure.
 * @return u32 The number of bytes the message buffer's data takes up.
 */
EZTR_IMPORT(u32 EZTR_MsgBuffer_Len(EZTR_MsgBuffer* buf));

/**
 * @brief Gets the size of the message buffer's content region, in bytes.
 * 
 * Effectively `EZTR_MsgBuffer_Len(buf) - 11` or `EZTR_MsgSContent_Len(buf->partition.content)`. 
 * Does not include the termination character '\xBF'.
 * 
 * @param buf The buffer to measure.
 * @return u32 The number of bytes the message buffer's content takes up.
 */
EZTR_IMPORT(u32 EZTR_MsgBuffer_ContentLen(EZTR_MsgBuffer* buf));

/**
 * @brief Sets a message buffer's header to default values.
 * 
 * The default values for a message buffer header are:
 * 
 * * text_box_type = EZTR_STANDARD_TEXT_BOX_I (0)
 * * text_box_y_pos = 0
 * * display_icon = EZTR_ICON_NO_ICON (0xFE)
 * * next_message_id = EZTR_NO_VALUE (0xFFFF)
 * * first_item_rupees = EZTR_NO_VALUE (0xFFFF)
 * * second_item_rupees = EZTR_NO_VALUE (0xFFFF)
 * 
 * @param buf the message buffer to write to.
 */
EZTR_IMPORT(void EZTR_MsgBuffer_WriteDefaultHeader(EZTR_MsgBuffer* buf));

/**
 * @brief 
 * 
 * @param buf the message buffer to write to. 
 * @param text_box_type The style of textbox to display. Use the `EZTR_TextBoxType` enum for more readable values.
 * @param text_box_y_pos The vertical position of the textbox on-screen.
 * @param display_icon Displays an icon in the textbox. Use the `EZTR_TextBoxIcon` enum for more readable values. 
 * Use `EZTR_ICON_NO_ICON` for no icon.
 * @param next_message_id The next message to display. If there is no next message, or the next message is determined by code,
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param first_item_rupees The price of the first item being offered for sale, if one exists. If there is no item, 
 * use 0xFFFF or `EZTR_NO_VALUE`.
 * @param second_item_rupees The price of the second item being offered for sale, if one exists. If there is no item, 
 * use 0xFFFF or `EZTR_NO_VALUE`.
 */
EZTR_IMPORT(void EZTR_MsgBuffer_WriteHeader(EZTR_MsgBuffer* buf, u8 text_box_type, u8 text_box_y_pos, u8 display_icon, 
    u16 next_message_id, u16 first_item_rupees, u16 second_item_rupees));

/**
 * @brief Retrieves the text_box_type from a message buffer's header.
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 * 
 * @param buf The message buffer to read from.
 * @return u8 The text_box_type of the message buffer. If `EZTR_MsgBufferData` is being packed correctly, this will be
 * equivalent to `buf->data.text_box_type`.
 */
EZTR_IMPORT(u8 EZTR_MsgBuffer_GetTextBoxType(EZTR_MsgBuffer* buf));

/**
 * @brief Sets the text_box_type of the message buffer's header. 
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 *  
 * @param buf The message buffer to write to.
 * @param type The new text_box_type value. If `EZTR_MsgBufferData` is being packed correctly, this will be equivalent 
 * to `buf->data.text_box_type = type`
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetTextBoxType(EZTR_MsgBuffer* buf, u8 type));

/**
 * @brief Retrieves the text_box_y_pos from a message buffer's header.
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 * 
 * @param buf The message buffer to read from.
 * @return u8 The text_box_y_pos of the message buffer. If `EZTR_MsgBufferData` is being packed correctly, this will be
 * equivalent to `buf->data.text_box_y_pos`.
 */
EZTR_IMPORT(u8 EZTR_MsgBuffer_GetTextBoxYPos(EZTR_MsgBuffer* buf));

/**
 * @brief Sets the text_box_y_pos of the message buffer's header. 
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 *  
 * @param buf The message buffer to write to.
 * @param pos The new text_box_y_pos value. If `EZTR_MsgBufferData` is being packed correctly, this will be equivalent 
 * to `buf->data.text_box_y_pos = pos`
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetTextBoxYPos(EZTR_MsgBuffer* buf, u8 pos));

/**
 * @brief Retrieves the display_icon from a message buffer's header.
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 * 
 * @param buf The message buffer to read from.
 * @return u8 The display_icon of the message buffer. If `EZTR_MsgBufferData` is being packed correctly, this will be
 * equivalent to `buf->data.display_icon`.
 */
EZTR_IMPORT(u8 EZTR_MsgBuffer_GetTextBoxDisplayIcon(EZTR_MsgBuffer* buf));

/**
 * @brief Sets the display_icon of the message buffer's header. 
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 *  
 * @param buf The message buffer to write to.
 * @param icon The new display_icon value. If `EZTR_MsgBufferData` is being packed correctly, this will be equivalent 
 * to `buf->data.display_icon = icon`
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetTextBoxDisplayIcon(EZTR_MsgBuffer* buf, u8 icon));

/**
 * @brief Retrieves the next_message_id from a message buffer's header.
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 * 
 * @param buf The message buffer to read from.
 * @return u16 The display_icon of the message buffer. If `EZTR_MsgBufferData` is being packed correctly, this will be
 * equivalent to `buf->data.next_message_id`.
 */
EZTR_IMPORT(u16 EZTR_MsgBuffer_GetNextMsg(EZTR_MsgBuffer* buf));

/**
 * @brief Sets the next_message_id of the message buffer's header. 
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 *  
 * @param buf The message buffer to write to.
 * @param textId The new display_icon value. If `EZTR_MsgBufferData` is being packed correctly, this will be equivalent 
 * to `buf->data.next_message_id = icon`.
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetNextMsg(EZTR_MsgBuffer* buf, u16 textId));

/**
 * @brief Retrieves the first_item_rupees from a message buffer's header.
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 * 
 * @param buf The message buffer to read from.
 * @return u16 The first_item_rupees of the message buffer. If `EZTR_MsgBufferData` is being packed correctly, this will be
 * equivalent to `buf->data.first_item_rupees`.
 */
EZTR_IMPORT(u16 EZTR_MsgBuffer_GetFirstItemRupees(EZTR_MsgBuffer* buf));

/**
 * @brief Sets the first_item_rupees of the message buffer's header. 
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 *  
 * @param buf The message buffer to write to.
 * @param value The new first_item_rupees value. If `EZTR_MsgBufferData` is being packed correctly, this will be equivalent 
 * to `buf->data.first_item_rupees = icon`.
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetFirstItemRupees(EZTR_MsgBuffer* buf, u16 val));

/**
 * @brief Retrieves the second_item_rupees from a message buffer's header.
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 * 
 * @param buf The message buffer to read from.
 * @return u16 The second_item_rupees of the message buffer. If `EZTR_MsgBufferData` is being packed correctly, this will be
 * equivalent to `buf->data.second_item_rupees`.
 */
EZTR_IMPORT(u16 EZTR_MsgBuffer_GetSecondItemRupees(EZTR_MsgBuffer* buf));

/**
 * @brief Sets the second_item_rupees of the message buffer's header. 
 * 
 * Useful if your compiler is having trouble with the `__attribute_((packed))` on `EZTR_MsgBufferData`, and the data isn't 
 * lining up correctly.
 *  
 * @param buf The message buffer to write to.
 * @param value The new second_item_rupees value. If `EZTR_MsgBufferData` is being packed correctly, this will be equivalent 
 * to `buf->data.second_item_rupees = icon`.
 */
EZTR_IMPORT(void EZTR_MsgBuffer_SetSecondItemRupees(EZTR_MsgBuffer* buf, u16 val));

/**
 * @brief Prints the contents of a message buffer to the console.
 * 
 * Each value in the header will be labeled. The content region will stop printing after the '\xBF' termination character.
 * 
 * @param buf The message buffer to print.
 */
EZTR_IMPORT(void EZTR_MsgBuffer_Print(EZTR_MsgBuffer* buf));

/**
 * @brief Prints the contents of a message buffer to the console, formatted as a `EZTR_Basic_ReplaceText` call.
 * 
 * The string literal for the content will end with the '\xBF' termination character.
 * 
 * @param buf The message buffer to print.
 */
EZTR_IMPORT(void EZTR_MsgBuffer_PrintCCode(EZTR_MsgBuffer* buf));

/**
 * @brief Prints the contents of a message buffer to the console.
 * 
 * Each value in the header will be labeled. The this function prints the entire 1279 bytes of the content region.
 * 
 * @param buf The message buffer to print.
 */
EZTR_IMPORT(void EZTR_MsgBuffer_PrintFull(EZTR_MsgBuffer* buf));

/**
 * @brief Prints the contents of a message buffer to the console, formatted as a `EZTR_Basic_ReplaceText` call.
 * 
 * The this function prints the entire 1279 bytes of the content region.
 * 
 * @param buf The message buffer to print.
 */
EZTR_IMPORT(void EZTR_MsgBuffer_PrintFullCCode(EZTR_MsgBuffer* buf));

/**
 * @brief Gets a pointer to the beginning of the content region for a desired message buffer.
 * 
 * Equivalent to `buf->partition.content`, or `buf->data.content` assuming EZTR_MsgBufferData is being packed correctly. 
 * 
 * @param buf The buffer to get the the content from.
 * @return char* The beginning of the message buffer's content region.
 */
EZTR_IMPORT(char* EZTR_MsgBuffer_GetContentPtr(EZTR_MsgBuffer* buf));

/** @}*/


/**
 * \defgroup MsgSContent 
 * \brief Functions for manipulating message text content.
 * @{
 */


/**
 * @brief Sets the message message content as empty.
 * 
 * This is accomplished by setting the first character in the message content to the '\xBF' termination character.
 * No other bytes are effected.
 * 
 * @param cont A pointer to message content string.
 */
EZTR_IMPORT(void EZTR_MsgSContent_SetEmpty(char* cont));

/**
 * @brief Gets the length of the a message content string in bytes, not counting the '\xBF' termination character
 * 
 * Similar to `strlen()` for null-terminated strings. 
 * 
 * This function is safe as long as `cont` points to the beginning of a message buffer's content region, since it 
 * will not count bytes beyond that size.
 * 
 * @param cont The message content you want to get the length of. 
 * @return u32 The length of the message content, in bytes.
 */
EZTR_IMPORT(u32 EZTR_MsgSContent_Len(char* cont));

/**
 * @brief Copies message content from `src` into the `dst`, up to `len` bytes.
 * 
 * Similar to `strncpy()` for null-terminated strings. 
 * 
 * @param dst The location to copy message content into.
 * @param src The message content to copy.
 * @param len The maximum number of bytes to copy. If a '\xBF' is encountered in `src`, the function  will stop copying 
 * before `len` is reached.
 * @return u32 The number of bytes copied.
 */
EZTR_IMPORT(u32 EZTR_MsgSContent_NCopy(char* dst, char* src, size_t len));

/**
 * @brief Copies message content from `src` into the `dst`.
 * 
 * Similar to `strcpy()` for null-terminated strings.
 * 
 * This function is safe as long as `dst` points to the beginning of a message buffer's content region, since it 
 * will not copy bytes beyond that size.
 * 
 * @param dst The location to copy message content into.
 * @param src The message content to copy. Should be '\xBF' terminated.
 * @return u32 The number of bytes copied.
 */
EZTR_IMPORT(u32 EZTR_MsgSContent_Copy(char* dst, char* src));

/**
 * @brief Copies the message content of `src` onto the end of `dst`, up to `len` bytes from `src`.
 * 
 * Similar to `strncat()` for null-terminated strings.
 * 
 * This function will not produce message content larger than a message buffer's content region. Copying will
 * stop once the maximum size is reached.
 * 
 * @param dst The location to append message content into.
 * @param src The message content to copy.
 * @param len The maximum number of bytes to copy. If a '\xBF' is encountered in `src`, the function  will stop copying 
 * before `len` is reached.
 * @return char* The pointer to the new string (i.e: to `dst`).
 */
EZTR_IMPORT(char* EZTR_MsgSContent_NCat(char* dst, char* src, size_t len));

/**
 * @brief Copies the message content of `src` onto the end of `dst`.
 * 
 * Similar to `strcat()` for null-terminated strings.
 * 
 * This function is safe as long as `dst` points to the beginning of a message buffer's content region, as it
 * will not produce message content larger than a message buffer's content region. Copying will
 * stop once the maximum size is reached.
 * 
 * @param dst The location to append message content into.
 * @param src The message content to copy. Should be '\xBF' terminated.
 * @return char* The pointer to the new string (i.e: to `dst`).
 */
EZTR_IMPORT(char* EZTR_MsgSContent_Cat(char* dst, char* src));

/**
 * @brief Compares up to `len` bytes of two message content strings.
 * 
 * Similar to `strncmp()` for null-terminated strings, but with one notable difference:
 * With `strncmp()`, if two strings are equal for the first `n` characters, but one string continues after `n` while  
 * the other doesn't, the shorter string is considered 'lesser' because the null terminator `'\x00'` is naturally
 * the lowest possible ASCII value. Because strings in MM are `'\xBF'` terminated, that behavior won't apply here.
 * 
 * In `EZTR_MsgSContent_NCmp()`, a special case is applied: If two strings are equal for `n` characters, but the 
 * next character after `n` is `'\xBF'` in one of the strings, it is considered the lesser string regardless of 
 * what the other character is.
 * 
 * @param str1 The first message content string to compare.
 * @param str2 The second message content string to compare.
 * @param len The maximum number of bytes to compare.
 * @return s32 Returns 0 is the strings are the same, 1 if the first string is greater than the second, and 
 * -1 if the second string is greater than the first.
 */
EZTR_IMPORT(s32 EZTR_MsgSContent_NCmp(char* str1, char* str2, size_t len));

/**
 * @brief Compares two message content strings.
 * 
 * This function is safe as long as both `str1` and `str2` point to the content region of message buffers, as the 
 * comparison will not continue beyond that size.
 * 
 * Similar to `strcmp()` for null-terminated strings, but with one notable difference:
 * With `strncmp()`, if two strings are equal for the first `n` characters, but one string continues after `n` while  
 * the other doesn't, the shorter string is considered 'lesser' because the null terminator `'\x00'` is naturally
 * the lowest possible ASCII value. Because strings in MM are `'\xBF'` terminated, that behavior won't apply here.
 * 
 * In `EZTR_MsgSContent_Cmp()`, a special case is applied: If two strings are equal for `n` characters, but the 
 * next character after `n` is `'\xBF'` in one of the strings, it is considered the lesser string regardless of 
 * what the other character is.
 * 
 * @param str1 The first message content string to compare.
 * @param str2 The second message content string to compare.
 * @return s32 Returns 0 is the strings are the same, 1 if the first string is greater than the second, and 
 * -1 if the second string is greater than the first.
 */
EZTR_IMPORT(s32 EZTR_MsgSContent_Cmp(char* str1, char* str2));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version uses pipe-escaped byte handling in the main formatting argument.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Printf(const char* format, ...));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version uses pipe-escaped byte handling in the main formatting argument.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_PrintfLine(const char* format, ...));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version uses pipe-escaped byte handling in the main formatting argument.
 * 
 * Unlike `EZTR_MsgSContent_Printf()`, this function will append a newline to the end of 
 * console output.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param buffer 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Sprintf(char* buffer, const char* format, ...));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version uses pipe-escaped byte handling in the main formatting argument.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param buffer 
 * @param count 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Snprintf(char* buffer, size_t count, const char* format, ...));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version uses pipe-escaped byte handling in the main formatting argument.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param buffer 
 * @param count 
 * @param format 
 * @param va 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Vsnprintf(char* buffer, size_t count, const char* format, va_list va));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version uses pipe-escaped byte handling in the main formatting argument.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param format 
 * @param va 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Vprintf(const char* format, va_list va));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param out 
 * @param arg 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_Fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version does not use pipe-escaped byte handling in the main formatting argument,
 * but are still used with the `%m` type specifier.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_NoPipe_Printf(const char* format, ...));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version does not use pipe-escaped byte handling in the main formatting argument,
 * but are still used with the `%m` type specifier.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_NoPipe_PrintfLine(const char* format, ...));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version does not use pipe-escaped byte handling in the main formatting argument,
 * but are still used with the `%m` type specifier.
 * 
 * Unlike `EZTR_MsgSContent_Printf()`, this function will append a newline to the end of 
 * console output.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param buffer 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_NoPipe_Sprintf(char* buffer, const char* format, ...));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version does not use pipe-escaped byte handling in the main formatting argument,
 * but are still used with the `%m` type specifier.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param buffer 
 * @param count 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_NoPipe_Snprintf(char* buffer, size_t count, const char* format, ...));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version does not use pipe-escaped byte handling in the main formatting argument,
 * but are still used with the `%m` type specifier.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param buffer 
 * @param count 
 * @param format 
 * @param va 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_NoPipe_Vsnprintf(char* buffer, size_t count, const char* format, va_list va));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version does not use pipe-escaped byte handling in the main formatting argument,
 * but are still used with the `%m` type specifier.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param format 
 * @param va 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_NoPipe_Vprintf(const char* format, va_list va));

/**
 * @brief A modified version of printf, specially designed to handle message content.
 * This version does not use pipe-escaped byte handling in the main formatting argument,
 * but are still used with the `%m` type specifier.
 * 
 * See \ref prinf_functions for more information on EZTR's custom printf behavior.
 * 
 * @param out 
 * @param arg 
 * @param format 
 * @param ... 
 * @return int 
 */
EZTR_IMPORT(int EZTR_MsgSContent_NoPipe_Fctprintf(void (*out)(char character, void* arg), void* arg, const char* format, ...));

/** @}*/

#endif