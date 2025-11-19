#ifndef __REPY_API__
#define __REPY_API__

#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"

/*! \file repy_api.h
    \version 1.0.0
    \brief The main header for Recomp External Python. Header version 2.
    \
 */

/**
 * @brief The mod id string for REPY.
 * 
 * The `recomp_py.h` imports all the functions and events needed for REPY, 
 * so you probably won't need to use this directly.
 */
#define REPY_MOD_ID_STR "MM_RecompExternalPython_API"

#ifdef DOXYGEN
#define REPY_IMPORT(func) func

#elif RECOMP_PY_BUILD_MODE
#define REPY_IMPORT(func) func

#else 
#define REPY_IMPORT(func) RECOMP_IMPORT(REPY_MOD_ID_STR, func)

#ifndef __FILE_NAME__
#define __FILE_NAME__ "Unknown Source File"
#endif

#endif
/** \defgroup repy_types Types
 * \brief All of the C types that REPY defines.
 *  @{
 */

/**
 * @brief Represents a Python object in REPY API functions. 
 * 
 * Technically, the handle is a unsigned integer value between 0x00000001 and 0xFFFFFFFF, inclusive. The REPY external library
 * will look up the corresponding Python object from an internally managed slot map. A handle is considered 'valid'
 * if the value is assigned to a Python object. 
 * 
 * In terms of Python's reference-count-based garbage collection, each valid handle counts as a single reference 
 * to an object, and multiple handles can be mapped to a single Python object. Once created, a handle must be released
 * to remove that reference, either by using `REPY_Release` or flagging the handle as Single-Use (more on that below). 
 * If you need multiple references to a Python object, you can get a new handle to the same object using `REPY_CopyHandle`.
 * With the exception of `REPY_MakeSUH`, any REPY API function that returns a REPY_Handle will be creating a new handle. 
 * Failure to release handles will result in resource/memory leaks. 
 * 
 * The handle value of `REPY_NO_OBJECT` (the numerical value 0) is a special case, and represents the absense of any Python object. 
 * Note that this is different from Python's `None`, which is itself a Python object. If an API function with `REPY_Handle`
 * as the return type returns `REPY_NO_OBJECT`, that will mean a Python error has occured (unless otherwise specified in the function's 
 * documentation).
 * 
 * There are a few cases where a REPY_Handle of `REPY_NO_OBJECT` is acceptable as a function argument, indicating that the argument
 * is not used. These arguments are named with the `_nullable` suffix.
 * 
 * Handles can be flagged as Single-Use, which means that the handle will be immediately released after it's next use in a REPY
 * API function. This is primarily to allow nesting calls to REPY API functions without incurring resource/memory leaks. Any handle
 * can be flagged as Single-Use using `REPY_MakeSUH` or `REPY_SetSUH`, and the Single-Use status can be checked with `REPY_GetSUH`.
 * Several common REPY API functions have alternative versions that automatically return a Single Use handle. These functions will
 * have the suffix `_SUH` in their names.
 * 
 * Not that the only functions that won't release a Single-Use handle are ones meant to manipulate the handles themselves:
 * `REPY_MakeSUH`, `REPY_IsValidHandle`, `REPY_GetSUH`, and `REPY_SetSUH`. Their REPY_Handle argument names have the suffix `_no_release`
 * to reflect this.
 * 
 * If at any point this documentation refers to a `REPY_Handle` as a Python object, understand that it is referring to the Python object that
 * the handle represents. The mechanism for mapping `REPY_Handle` values to Python objects is written entirely in C++, and the Python interpreter 
 * does not have access to it.
 */
typedef unsigned int REPY_Handle;

/**
 * @brief Represents the absence of a Python object in REPY API functions.
 * 
 * A more readable alternative to simply entering 0.
 */
#define REPY_NO_OBJECT 0

/**
 * @brief Used to set the type of code-string being compiled, in line with how Python's
 * built-in `compile` function operates.
 * 
 * Used with `REPY_CompileCStr` and `REPY_CompileCStr`. `REPY_Compile` accepts a REPY_Handle string argument instead.
 * 
 */
typedef enum REPY_CodeMode {
    REPY_CODE_EXEC = 0,
    REPY_CODE_EVAL = 1,
    REPY_CODE_SINGLE = 2
} REPY_CodeMode;

/**
 * @brief Helper object used to when iterating through Python objects in loops in C code.
 * 
 * These objects are primarily used as part of the the `REPY_FOREACH` and `REPY_FN_FOREACH_CACHE` macros,
 * which simulate the behavior of Python's own `for` loops. You can use them manually as well.
 * 
 * The lifetime of each REPY_Handle member is managed by IteratorHelper. Do not release them manually.
 * 
 */
typedef struct REPY_IteratorHelper {
    REPY_Handle iter; ///< Handle for the Python iterator object. 
    u32 index; ///< The index of the current object from the iterator.
    REPY_Handle curr; ///< Handle of the current object from the iterator. If you need to access this object outside of the current iteration, use `REPY_CopyHandle` to get a new handle.
    REPY_Handle py_scope; ///< If this handle != 0, then the `curr` object will be added to this scope with a variable name set by `var_name`.
    REPY_Handle var_name; ///< The variable name that will be used for `curr` when added to `py_scope`, if `py_scope` is not 0.
    bool _first_update; ///< Internal flag used to determine if the iterator has been updated for the first time.
} REPY_IteratorHelper;

/**
 * @brief Helper object used to cache Python expressions as bytecode, so that they don't need to be re-parsed and compiled every time they're run.
 * 
 * Used as part of the macro `REPY_FN_IF_CACHE_INIT`, and is used with `REPY_FN_IF_CACHE_STMT`, `REPY_FN_IF_CACHE` and `REPY_FN_ELIF_CACHE`.
 * Generally initialized as a `static` variable, so that that the compiled bytecode is is preserverd between uses.
 * 
 * The chain is a singly-linked list with the bytecode for each Python expression from a `REPY_IfStmtHelper_Step` call. Each link is constructed the 
 * that step is called.
 * 
 */
typedef struct REPY_IfStmtChain {
    REPY_Handle eval_expression_bytecode; ///< The bytecode for the Python expression to evaluate.
    struct REPY_IfStmtChain* next; ///< Pointer to the next link in the chain.
} REPY_IfStmtChain;

/**
 * @brief Helper used to step through a `REPY_IfStmtChain` while it's being evaluated.
 * 
 */
typedef struct REPY_IfStmtHelper {
    u32 index; ///< The number of links down the chain we've gone.
    REPY_IfStmtChain** root; ///< The start of the chain. A double pointer is used so that, the the chain doesn't exist yet, it can be initialized on the first call of `REPY_IfStmtHelper_Step`.
    REPY_IfStmtChain* curr; ///< The most recently evaluated link in the chain.
    bool _first_step; ///< ///< Internal flag used to determine if the helper has been stepped for the first time.
} REPY_IfStmtHelper;

/** @}*/

/** \defgroup repy_events Events
 *  @{
 */

/**
 * @brief Runs before the Python interpreter is initialized.
 * 
 * Primarily used to flag add NRM files to the Python module search path on startup.
 * 
 * Takes no arguments, returns void.
 */
#define REPY_ON_PRE_INIT RECOMP_CALLBACK(REPY_MOD_ID_STR, REPY_OnPreInit) 

/**
 * @brief Event used for initializing Python modules from code strings on startup.
 * 
 * Runs immediately after the interpreter is initialized.
 * 
 * Takes a single int `success` argument, indicating whether the interpreter was started correctly. Return should void.
 */
#define REPY_ON_LOAD_MODULES RECOMP_CALLBACK(REPY_MOD_ID_STR, REPY_OnLoadModules)

/**
 * @brief Event used for compiling Python bytecode from code strings on startup.
 * 
 * Runs immediately after `REPY_ON_LOAD_MODULES`.
 * 
 * Takes a single int `success` argument, indicating whether the interpreter was started correctly. Should return void.
 */
#define REPY_ON_MAKE_GLOBAL_CACHES RECOMP_CALLBACK(REPY_MOD_ID_STR, REPY_OnMakeGlobalCaches)

/**
 * @brief Generic initialization event. Use for your own mod's initialization code.
 * 
 * Runs immediately after `REPY_ON_MAKE_GLOBAL_CACHES`.
 * 
 * Takes a single int `success` argument, indicating whether the interpreter was started correctly. Should return void.
 */
#define REPY_ON_INIT RECOMP_CALLBACK(REPY_MOD_ID_STR, REPY_OnInit)

/** @}*/

/** \defgroup repy_init_macros Initialization Macros
 *  @{
 */

/**
 * @brief Adds this .nrm file to Python's module search path.
 * 
 * This will allow you to add Python modules (both single files and module folders) to your mod by
 * including them under the `additional_files` section of your mod.toml
 * 
 */
#define PRE_INIT_ADD_NRM_TO_MODULE_PATH \
REPY_ON_PRE_INIT void _repy_register_nrm () { \
    const unsigned char* nrm_file_path = recomp_get_mod_file_path(); \
    REPY_PreInitAddToModuleSearchPath(nrm_file_path); \
    recomp_free((void*)nrm_file_path); \
};

#ifdef REPY_SILENCE_INCBIN_SQUIGGLES
#define REPY_INCBIN(identifier, filename)      \
    extern u8 identifier[];                    \
    extern u8 identifier##_end[]

#define REPY_INCBIN_TEXT(identifier, filename) \
    extern u8 identifier[];                    \
    extern u8 identifier##_end[]               
#else
/**
 * @brief General INCBIN macro used by several other initialization macros to include external Python code.
 * 
 * The data included by this macro is not NULL-terminated. You'll need to use `identifier_end` to find the
 * end of the code-block and use `identifier_end - identifier` to find the length.
 * 
 * @param identifier The variable name for the start of the data. `identifier_end` will indicate the end of the data.
 * @param filename The path to the included file. The file needs to be in your include path.
 */
#define REPY_INCBIN(identifier, filename)      \
    asm(".pushsection .rodata\n"                      \
        "\t.globl " #identifier "\n"                  \
        "\t.type " #identifier ", @object\n"          \
        "\t.balign 8\n"                               \
        #identifier ":\n"                             \
        "\t.incbin \"" filename "\"\n"                \
        "\t.globl " #identifier "_end\n"              \
        #identifier "_end:\n"                         \
        "\t.popsection\n");                           \
    extern u8 identifier[];                           \
    extern u8 identifier##_end[]

    /**
 * @brief General INCBIN macro used by several other initialization macros to include external Python code.
 * 
 * The data included by this macro is NULL-terminated, with `identifier_end` pointing to the termination character.
 * Therefore, you'll need to use `identifier_end - identifier - 1` to find the length of the text.
 * 
 * @param identifier the variable name for the start of the data. `identifier_end` will indicate the end of the data.
 * @param filename the path to the included file. The file needs to be in your include path.
 */
#define REPY_INCBIN_TEXT(identifier, filename)      \
    asm(".pushsection .rodata\n"                      \
        "\t.globl " #identifier "\n"                  \
        "\t.type " #identifier ", @object\n"          \
        "\t.balign 8\n"                               \
        #identifier ":\n"                             \
        "\t.incbin \"" filename "\"\n"                \
        "\t.space 1"                                  \
        "\t.globl " #identifier "_end\n"              \
        #identifier "_end:\n"                         \
        "\t.popsection\n");                           \
    extern u8 identifier[];                           \
    extern u8 identifier##_end[]
#endif

/**
 * @brief On startup, construct a Python module from an INCBINed file. Use outside of any functions.
 * 
 * Note that in general, the preferred method of including Python modules in your mod is to include them in your
 * .nrm under the `additional files` section. This interface will likely be deprecated in a future update.
 * 
 * @param module_name The name for your module as it would be used in import statements. Remember that modules are shared across 
 * all mods using REPY, so be sure to make the name unique.
 * @param filename The path to the module file to INCBIN. The file needs to be in your include path. 
 */
#define REPY_INCBIN_MODULE(module_name, filename) \
REPY_INCBIN(module_name ## _code, filename); \
REPY_ON_LOAD_MODULES void _construct_module_ ## module_name (int success) { \
    if (success) { \
        REPY_LoadModuleN(#module_name, (const char*)module_name ## _code, (u32) (module_name ## _code_end - module_name ## _code)); \
    } \
}

/**
 * @brief On startup, compiles a Python code string into bytecode with a global handle. Use outside of any functions.
 * 
 * To access the the bytecode from another source file, use `extern REPY_Handle bytecode_identifier;`,
 * where `bytecode_identifier` is the variable name you entered in the `bytecode_identifier` parameter.
 * 
 * @param bytecode_identifier The variable name for the resultant `REPY_Handle` bytecode handle.
 * @param code_mode The type of code being compiled. See `REPY_CodeMode` for valid modes.
 * @param code_str The Python code string to compile. Must be NULL-terminated.
 */
#define REPY_GLOBAL_COMPILE_CACHE(bytecode_identifier, code_mode, code_str) \
REPY_Handle bytecode_identifier = 0; \
REPY_ON_MAKE_GLOBAL_CACHES void _cache_code_ ## bytecode_identifier (int success) { \
    if (success && bytecode_identifier == 0) { \
        char* iden_str = REPY_InlineCodeSourceStrHelper("REPY_GLOBAL_COMPILE_CACHE", __FILE_NAME__, (char*) __func__, __LINE__, #bytecode_identifier); \
        bytecode_identifier = REPY_CompileCStr(code_str, (const char*)iden_str, code_mode); \
        recomp_free(iden_str); \
    } \
}

/**
 * @brief On startup, compiles a Python code string into bytecode with a static handle. Use outside of any functions.
 * 
 * Because the `REPY_Handle` variable is marked as `static`, there will be no accessing it from other source files.
 * 
 * @param bytecode_identifier The variable name for the resultant `static REPY_Handle` bytecode handle.
 * @param code_mode The type of code being compiled. See `REPY_CodeMode` for valid modes.
 * @param code_str The Python code string to compile. Must be NULL-terminated.
 */
#define REPY_STATIC_COMPILE_CACHE(bytecode_identifier, code_mode, code_str) \
static REPY_Handle bytecode_identifier = 0; \
REPY_ON_MAKE_GLOBAL_CACHES void _cache_code_ ## bytecode_identifier (int success) { \
    if (success && bytecode_identifier == 0) { \
        char* iden_str = REPY_InlineCodeSourceStrHelper("REPY_STATIC_COMPILE_CACHE", __FILE_NAME__, (char*) __func__, __LINE__, #bytecode_identifier); \
        bytecode_identifier = REPY_CompileCStr(code_str, (const char*)iden_str, code_mode); \
        recomp_free(iden_str); \
    } \
}

/**
 * @brief On startup, compiles a INCBINed Python code file into bytecode with a global handle.
 * 
 * To access the the bytecode from another source file, use `extern REPY_Handle bytecode_identifier;`,
 * where `bytecode_identifier` is the variable name you entered in the `bytecode_identifier` parameter.
 * 
 * Unlike `REPY_GLOBAL_COMPILE_CACHE`, this macro assumes a REPY_CodeMode of `REPY_CODE_EXEC`.
 * 
 * @param bytecode_identifier The variable name for the resultant `REPY_Handle` bytecode handle.
 * @param filename The path to the module file to INCBIN. The file needs to be in your include path. 
 */
#define REPY_GLOBAL_COMPILE_INCBIN_CACHE(bytecode_identifier, filename) \
REPY_INCBIN(bytecode_identifier ## _code_str, filename); \
REPY_Handle bytecode_identifier = 0; \
REPY_ON_MAKE_GLOBAL_CACHES void _cache_code_ ## bytecode_identifier (int success) { \
    if (success && bytecode_identifier == 0) { \
        char* iden_str = REPY_InlineCodeSourceStrHelper("REPY_GLOBAL_COMPILE_INCBIN_CACHE: " filename, __FILE_NAME__, (char*) __func__, __LINE__, #bytecode_identifier); \
        bytecode_identifier = REPY_CompileCStrN(bytecode_identifier ## _code_str, bytecode_identifier ## _code_str_end - bytecode_identifier ## _code_str, \
            (const char*)iden_str, REPY_CODE_EXEC); \
        recomp_free(iden_str); \
    } \
}

/**
 * @brief On startup, compiles a INCBINed Python code file into bytecode with a global handle. Use outside of any functions.
 * 
 * Because the `REPY_Handle` variable is marked as `static`, there will be no accessing it from other source files.
 * 
 * Unlike `REPY_GLOBAL_COMPILE_CACHE`, this macro assumes a REPY_CodeMode of `REPY_CODE_EXEC`.
 * 
 * @param bytecode_identifier The variable name for the resultant `static REPY_Handle` bytecode handle.
 * @param filename The path to the module file to INCBIN. The file needs to be in your include path. 
 */
#define REPY_STATIC_COMPILE_INCBIN_CACHE(bytecode_identifier, filename) \
REPY_INCBIN(bytecode_identifier ## _code_str, filename); \
REPY_Handle bytecode_identifier = 0; \
REPY_ON_MAKE_GLOBAL_CACHES void _cache_code_ ## bytecode_identifier (int success) { \
    if (success && bytecode_identifier == 0) { \
        char* iden_str = REPY_InlineCodeSourceStrHelper("REPY_STATIC_COMPILE_INCBIN_CACHE: " filename, __FILE_NAME__, (char*) __func__, __LINE__, #bytecode_identifier); \
        bytecode_identifier = REPY_CompileCStrN(bytecode_identifier ## _code_str, bytecode_identifier ## _code_str_end - bytecode_identifier ## _code_str, \
            (const char*)iden_str, REPY_CODE_EXEC); \
        recomp_free(iden_str); \
    } \
}
/** @}*/

/** \defgroup repy_inline_cache_macros Inline Code Caching Macros
 *  @{
 */

/**
 * @brief Inside a function, construct a code block that compiles to bytecode a Python code string the first time it's run,
 * storing the handle for that bytecode in a static variable. Allows setting a category name.
 * 
 * This macro forms the backbone of many of the other macros in this header, expecially those in the `REPY_FN` section.
 * By only parsing and compiling the Python code string once, we can dramatically improve the performance of any function
 * that needs to execute inline Python code.
 * 
 * The category argument becomes part of the resultant bytecode object's `identifier` string, which is displayed in Python error messages.
 * In this header, macros that depend this one will use their own name for this argument. That way, if you get a error message from running
 * inline Python code, you can see which macro caused the error along with the other source information.
 * 
 * @param category A category name used as part of the bytecode's identifying string. 
 * @param bytecode_identifier The name of the static variable that the bytecode handle will be assigned to.
 * @param code_mode The type of code being compiled. See `REPY_CodeMode` for valid modes.
 * @param code_str The Python code string to compile. Should be NULL-terminated.
 */
#define REPY_INLINE_COMPILE_CACHE_BLOCK(category, bytecode_identifier, code_mode, code_str) \
static REPY_Handle bytecode_identifier = 0; \
if (bytecode_identifier == 0) { \
    char* iden_str = REPY_InlineCodeSourceStrHelper(category, __FILE_NAME__, (char*) __func__, __LINE__, #bytecode_identifier); \
    bytecode_identifier = REPY_CompileCStr(code_str, (const char*)iden_str, code_mode); \
    recomp_free(iden_str); \
}

/**
 * @brief Inside a function, construct a code block that compiles to bytecode a Python code string the first time it's run,
 * storing the handle for that bytecode in a static variable.
 * 
 * By only parsing and compiling the Python code string once, we can dramatically improve the performance of any function
 * that needs to execute inline Python code.
 * 
 * This macro is identical to `REPY_INLINE_COMPILE_CACHE_BLOCK`, except it uses REPY_INLINE_COMPILE_CACHE as the category.
 * 
 * @param bytecode_identifier The name of the static variable that the bytecode handle will be assigned to.
 * @param code_mode The type of code being compiled. See `REPY_CodeMode` for valid modes.
 * @param code_str The Python code string to compile. Should be NULL-terminated.
 */
#define REPY_INLINE_COMPILE_CACHE(bytecode_identifier, code_mode, code_str) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_INLINE_COMPILE_CACHE", bytecode_identifier, code_mode, code_str)

/** @}*/

/** \defgroup repy_foreach_macros REPY_FOREACH - Python Object Iteration Macros
 *  @{
 */

/**
 * @brief General macro to construct a `for` loop that uses `REPY_IteratorHelper`.
 * 
 * Serves as the basis for the `REPY_FOREACH` and `REPY_FN_FOREACH_CACHE` macros.
 * 
 * The arguments `py_scope_nullable` and `var_name` are used specifically by `REPY_FN_FOREACH_CACHE` for
 * scope management. Each call of `REPY_IteratorHelper_Update` the Python object for `iter_identifier->curr` will
 * be added to `py_scope_nullable` under a name set by the `var_name` argument. This behavior is disabled if 
 * `py_scope_nullable` is set to `REPY_NO_OBJECT`. Also, the `py_scope_nullable` handle is copied on initialization, 
 * meaning that it's safe to release the original. 
 * 
 * REPY_IteratorHelper doesn't hold onto the `py_object` handle you pass in, so it's safe to release it.
 * 
 * In this macro, `REPY_IteratorHelper_Update` is set to clean up the `REPY_IteratorHelper` automatically. once the iteration ends.
 * Ergo, you only have to clean up if you end the loop early, such as through a break or a return. The macros `REPY_FOREACH_CLEANUP_NOW`,
 * `REPY_FOREACH_BREAK`, and `REPY_FOREACH_RETURN` are provided to facillitate that. Failure to manually clean up the `REPY_IteratorHelper`
 * when exiting the loop early will result in a memory leak.
 * 
 * @param iter_identifier the variable name for the `REPY_IteratorHelper` pointer.
 * @param py_object a `REPY_Handle` for the Python object to iterate through.
 * @param py_scope_nullable a `REPY_Handle` to a Python `dict` being used a local scope. Can be `REPY_NO_OBJECT`.
 * @param var_name the variable name for the `REPY_IteratorHelper` pointer. If `py_scope_nullable` is set to `REPY_NO_OBJECT`,
 * this does nothing and should be set to NULL.
 */
#define REPY_FOREACH_BLOCK(iter_identifier, py_object, py_scope_nullable, var_name) \
for (REPY_IteratorHelper* iter_identifier = REPY_IteratorHelper_Create(py_object, py_scope_nullable, var_name); REPY_IteratorHelper_Update(iter_identifier, true);)

/**
 * @brief Iterate through a Python object, the way Python's `for` loops do.
 * 
 * A `REPY_IteratorHelper` object is created to manage the iteration process. The current object of the loop can be
 * accessed via `iter_identifier->curr`, and the index of that object can be accessed via `iter_identifier->index`.
 * See the `REPY_IteratorHelper` documentation for more information.
 * 
 * REPY_IteratorHelper doesn't hold onto the `py_object` handle you pass in, so it's safe to release it.
 * 
 * In this macro, `REPY_IteratorHelper_Update` is set to clean up the `REPY_IteratorHelper` automatically. once the iteration ends.
 * Ergo, you only have to clean up if you end the loop early, such as through a break or a return. The macros `REPY_FOREACH_CLEANUP_NOW`,
 * `REPY_FOREACH_BREAK`, and `REPY_FOREACH_RETURN` are provided to facillitate that. ailure to manually clean up the `REPY_IteratorHelper`
 * when exiting the loop early will result in a memory leak.
 * 
 * @param iter_identifier the variable name for the `REPY_IteratorHelper` pointer.
 * @param py_object a `REPY_Handle` for the Python object to iterate through.
 */
#define REPY_FOREACH(iter_identifier, py_object) \
REPY_FOREACH_BLOCK(iter_identifier, py_object, 0, NULL)

/**
 * @brief Manually clean up the `REPY_IteratorHelper` for a `REPY_FOREACH` loop.
 * 
 * Once this had been called, you'll have a crash if you attempt another pass of the loop.
 * 
 * @param iter_identifier the `REPY_IteratorHelper` pointer.
 */
#define REPY_FOREACH_CLEANUP_NOW(iter_identifier) \
REPY_IteratorHelper_Destroy(iter_identifier)

/**
 * @brief Manually clean up the `REPY_IteratorHelper` for a `REPY_FOREACH` loop, and immediately break.
 * 
 * @param iter_identifier the `REPY_IteratorHelper` pointer.
 */
#define REPY_FOREACH_BREAK(iter_identifier) \
REPY_FOREACH_CLEANUP_NOW(iter_identifier); break

/**
 * @brief Manually clean up the `REPY_IteratorHelper` for a `REPY_FOREACH` loop, and immediately return.
 * 
 * Supports returning a value.
 * 
 * @param iter_identifier the `REPY_IteratorHelper` pointer.
 */
#define REPY_FOREACH_RETURN(iter_identifier) \
REPY_FOREACH_CLEANUP_NOW(iter_identifier); return

/** @}*/

/** \defgroup repy_fn REPY_FN - Python Scoped Inline Code Execution.
 *  @{
 */

/**
 * @brief The variable name for inline execution global scopes.
 * 
 */
#define REPY_FN_GLOBAL_SCOPE __repy_globals

/**
 * @brief The variable name for inline execution local scopes.
 * 
 */
#define REPY_FN_LOCAL_SCOPE __repy_locals

/**
 * @brief Create an inline execution scope for your function without any globals.
 * 
 * The global and local scope `dict` objects will the same. Python's built-ins will be added
 * whenever Python code is first executed.
 */
#define REPY_FN_SETUP \
REPY_Handle REPY_FN_GLOBAL_SCOPE = REPY_CreateDict(0); \
REPY_Handle REPY_FN_LOCAL_SCOPE = REPY_FN_GLOBAL_SCOPE \

/**
 * @brief Create an inline execution scope for your function, using a pre-defined Python
 * `dict` as your global scope object, and creating a new Python `dict` for the local scope.
 * Note that, unless otherwise specified, executing Python code stores variables on the local scope.
 * 
 * If the global scope `dict` doesn't have Python's builtins predefined, they will be added to the `dict` whenever
 * Python code is first executed.
 * 
 * @param globals The Python `dict` to use as a global scope.
 */
#define REPY_FN_SETUP_WITH_GLOBALS(globals) \
REPY_Handle REPY_FN_GLOBAL_SCOPE = globals; \
REPY_Handle REPY_FN_LOCAL_SCOPE = REPY_CreateDict(0) \

/**
 * @brief Create an inline execution scope for your function, using a pre-defined Python
 * `dict` as your global scope and local scope. Useful for initializing globals to use across multiple functions.
 * 
 * If the global scope `dict` doesn't have Python's built-ins predefined, they will be added to the `dict` whenever
 * Python code is first executed.
 * 
 * You should copy the `globals` handle with `REPY_CopyHandle` (or forgo cleaning up entirely) if you intend to use this
 * global scope dict elsewhere, since the clean up macros will release the scope `dict`.
 * 
 * @param globals The Python `dict` to use as a global and local scope.
 */
#define REPY_FN_SETUP_GLOBALS_ONLY(globals) \
REPY_Handle REPY_FN_GLOBAL_SCOPE = globals; \
REPY_Handle REPY_FN_LOCAL_SCOPE = REPY_FN_GLOBAL_SCOPE; \

/**
 * @brief Clean up a inline execution scope by releasing the local scope.
 * 
 * The global scope is only released if the global and local scopes are the same.
 */
#define REPY_FN_CLEANUP \
REPY_Release(REPY_FN_LOCAL_SCOPE)

/**
 * @brief Clean up a inline scope by releasing the local scope, and return.
 * 
 * Supports returning a value.
 * 
 * The global scope is only released if the global and local scopes are the same.
 */
#define REPY_FN_RETURN \
REPY_Release(REPY_FN_LOCAL_SCOPE); return


/**
 * @brief Executes Python code object within the current inline execution scope.
 * 
 * The code to execute should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python code to execute. Should be a `REPY_Handle` to a valid code object.
 * @return 1 if execution was successful, 0 if there was an error.
 */
#define REPY_FN_EXEC(code_handle) \
REPY_Exec(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE) 

/**
 * @brief Executes Python code string string within the current inline execution scope.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run.
 * It is technically faster than `REPY_FN_EXEC_CACHE` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python code to execute. Should be a NULL-terminated C-string such as a string literal or a `const char*`.
 * @return 1 if execution was successful, 0 if there was an error.
 */
#define REPY_FN_EXEC_CSTR(code_str) \
REPY_ExecCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE) 

/**
 * @brief Executes a Python code string string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses.
 * 
 * This is the recommended method of inlining executable blocks of Python code inside of functions. The performance difference
 * made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the success of the execution is stored in a
 * variable named `identifier_success`. This variable will be 1 if execution was successful, 0 if there was an error.
 * 
 * @param identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_handle The Python code to execute. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 */
#define REPY_FN_EXEC_CACHE(identifier, code_str) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EXEC_CACHE", identifier, REPY_CODE_EXEC, code_str) \
u32 identifier ## _success = REPY_FN_EXEC(identifier) 

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return a `REPY_Handle` for the resultant Python object. Will be `REPY_NO_OBJECT` if an error has occured.
 */
#define REPY_FN_EVAL(code_handle) \
REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE) 

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `bool`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `bool`.
 */
#define REPY_FN_EVAL_BOOL(code_handle) \
REPY_CastBool(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `u8`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `u8`.
 */
#define REPY_FN_EVAL_U8(code_handle) \
REPY_CastU8(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `s8`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `s8`.
 */
#define REPY_FN_EVAL_S8(code_handle) \
REPY_CastS8(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `u16`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `u16`.
 */
#define REPY_FN_EVAL_U16(code_handle) \
REPY_CastU16(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `s16`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `s16`.
 */
#define REPY_FN_EVAL_S16(code_handle) \
REPY_CastS16(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `u32`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `u32`.
 */
#define REPY_FN_EVAL_U32(code_handle) \
REPY_CastU32(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `s32`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `s32`.
 */
#define REPY_FN_EVAL_S32(code_handle) \
REPY_CastS32(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `f32`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `f32`.
 */
#define REPY_FN_EVAL_F32(code_handle) \
REPY_CastF32(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `u64`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `u64`.
 */
#define REPY_FN_EVAL_U64(code_handle) \
REPY_CastU64(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `s64`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `s64`.
 */
#define REPY_FN_EVAL_S64(code_handle) \
REPY_CastS64(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `f64`.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `f64`.
 */
#define REPY_FN_EVAL_F64(code_handle) \
REPY_CastF64(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `char*` C string.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * Intended to be used when the result of the evaluation is a Python `str`. 
 * 
 * The C string returned by this macro will need to be freed with `recomp_free`. Failure to do so will result in a memory leak.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `char*`.
 */
#define REPY_FN_EVAL_STR(code_handle) \
REPY_CastStr(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))


/**
 * @brief Evaluates a Python expression code object within the current inline execution scope, and returns the result as a `char*` C string.
 * 
 * The expression to evaluate should already be a Python object. A precompiled bytecode object is recommended for performance
 * reasons, but a Python `str` object will also work.
 * 
 * Intended to be used when the result of the evaluation is a Python `bytes` object. 
 * 
 * The C string returned by this macro will need to be freed with `recomp_free`. Failure to do so will result in a memory leak.
 * 
 * @param code_handle The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `char*`.
 */
#define REPY_FN_EVAL_BYTESTR(code_handle) \
REPY_CastBytes(REPY_MakeSUH(REPY_Eval(code_handle, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return a `REPY_Handle` for the resultant Python object. Will be `REPY_NO_OBJECT` if an error has occured.
 */
#define REPY_FN_EVAL_CSTR(code_str) \
REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE) 

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `bool`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_BOOL` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `bool`.
 */
#define REPY_FN_EVAL_CSTR_BOOL(code_str) \
REPY_CastBool(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `u8`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_U8` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `u8`.
 */
#define REPY_FN_EVAL_CSTR_U8(code_str) \
REPY_CastU8(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `s8`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_S8` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `s8`.
 */
#define REPY_FN_EVAL_CSTR_S8(code_str) \
REPY_CastS8(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `u16`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_U16` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `u16`.
 */
#define REPY_FN_EVAL_CSTR_U16(code_str) \
REPY_CastU16(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `s16`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_S16` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `s16`.
 */
#define REPY_FN_EVAL_CSTR_S16(code_str) \
REPY_CastS16(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `u32`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_U32` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `u32`.
 */
#define REPY_FN_EVAL_CSTR_U32(code_str) \
REPY_CastU32(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `s32`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_S32` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `s32`.
 */
#define REPY_FN_EVAL_CSTR_S32(code_str) \
REPY_CastS32(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `f32`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_F32` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `f32`.
 */
#define REPY_FN_EVAL_CSTR_F32(code_str) \
REPY_CastF32(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `u64`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_U64` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `u64`.
 */
#define REPY_FN_EVAL_CSTR_U64(code_str) \
REPY_CastU64(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `s64`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_S64` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `s64`.
 */
#define REPY_FN_EVAL_CSTR_S64(code_str) \
REPY_CastS64(REPY_MakeSUH(REPY_EvalCStr(code,code_strREPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `f64`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_F64` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `f64`.
 */
#define REPY_FN_EVAL_CSTR_F64(code_str) \
REPY_CastF64(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `char*`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_STR` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * Intended to be used when the result of the evaluation is a Python `str`. 
 * 
 * The C string returned by this macro will need to be freed with `recomp_free`. Failure to do so will result in a memory leak.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `bool`.
 */
#define REPY_FN_EVAL_CSTR_STR(code_str) \
REPY_CastStr(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluates a Python expression code string within the current inline execution scope, and returns the result as a `char*`.
 * 
 * Not generally recommended, since this will require recompiling the Python code string every time it's run, which will result in significant slowdown.
 * It is technically faster than `REPY_FN_EVAL_CACHE_BYTESTR` for code that is only used once (since one fewer handle lookup is involved),
 * but the difference is so marginal that it really doesn't matter.
 * 
 * Intended to be used when the result of the evaluation is a Python `bytes` object. 
 * 
 * The C string returned by this macro will need to be freed with `recomp_free`. Failure to do so will result in a memory leak.
 * 
 * @param code_str The Python expression to evaluate. Should be a `REPY_Handle` to a valid code object.
 * @return The resultant Python object, cast to `char*`.
 */
#define REPY_FN_EVAL_CSTR_BYTESTR(code_str) \
REPY_CastBytes(REPY_MakeSUH(REPY_EvalCStr(code_str, REPY_FN_GLOBAL_SCOPE, REPY_FN_LOCAL_SCOPE)))

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `REPY_Handle`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `REPY_Handle`
 * variable named using the `out_var` argument. If `out_var` is `REPY_NO_OBJECT`, a Python error has occured.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `REPY_Handle` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE", bytecode_identifier, REPY_CODE_EVAL, code_str) \
REPY_Handle out_var = REPY_FN_EVAL(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `bool`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `bool`
 * variable named using the `out_var` argument.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `bool` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_BOOL(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_BOOL", bytecode_identifier, REPY_CODE_EVAL, code_str) \
bool out_var = REPY_FN_EVAL_BOOL(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `u8`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `u8`
 * variable named using the `out_var` argument.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `u8` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_U8(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_U8", bytecode_identifier, REPY_CODE_EVAL, code_str) \
u32 out_var = REPY_FN_EVAL_U8(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `s8`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `s8`
 * variable named using the `out_var` argument.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `s8` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_S8(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_S8", bytecode_identifier, REPY_CODE_EVAL, code_str) \
s32 out_var = REPY_FN_EVAL_S8(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `u16`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `u16`
 * variable named using the `out_var` argument.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `u16` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_U16(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_U16", bytecode_identifier, REPY_CODE_EVAL, code_str) \
u32 out_var = REPY_FN_EVAL_U16(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `s16`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `s16`
 * variable named using the `out_var` argument.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `s16` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_S16(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_S16", bytecode_identifier, REPY_CODE_EVAL, code_str) \
s32 out_var = REPY_FN_EVAL_S16(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `u32`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `u32`
 * variable named using the `out_var` argument.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `u32` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_U32(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_U32", bytecode_identifier, REPY_CODE_EVAL, code_str) \
u32 out_var = REPY_FN_EVAL_U32(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `s32`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `s32`
 * variable named using the `out_var` argument.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `s32` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_S32(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_S32", bytecode_identifier, REPY_CODE_EVAL, code_str) \
s32 out_var = REPY_FN_EVAL_S32(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `f32`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `f32`
 * variable named using the `out_var` argument.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `f32` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_F32(identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_F32", bytecode_identifier, REPY_CODE_EVAL, code_str) \
f32 out_var = REPY_FN_EVAL_F32(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `u64`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `u64`
 * variable named using the `out_var` argument.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `u64` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_U64(identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_U64", bytecode_identifier, REPY_CODE_EVAL, code_str) \
u64 out_var = REPY_FN_EVAL_U64(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `s64`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `s64`
 * variable named using the `out_var` argument.
 * 
 * @param identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `s64` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_S64(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_S64", bytecode_identifier, REPY_CODE_EVAL, code_str) \
s64 out_var = REPY_FN_EVAL_S64(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `f64`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `f64`
 * variable named using the `out_var` argument.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `f64` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_F64(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_F64", bytecode_identifier, REPY_CODE_EVAL, code_str) \
f64 out_var = REPY_FN_EVAL_F64(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `char*`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `char*`
 * variable named using the `out_var` argument. The C string stored in `out_var` will need to be freed with `recomp_free`.
 * Failure to do so will result in a memory leak.
 * 
 * Intended to be used when the result of the evaluation is a Python `str` object. 
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `char*` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_STR(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_STR", bytecode_identifier, REPY_CODE_EVAL, code_str) \
char* out_var = REPY_FN_EVAL_STR(bytecode_identifier)

/**
 * @brief Evaluate a Python expression code string within the current inline execution scope, compiling it
 * the first time it's run and caching the bytecode for subsequent uses, and storing its result as a `char*`.
 * 
 * The various `REPY_FN_EVAL_CACHE` macros are the recommended method of inlining Python expression evaluations inside of functions.
 * The performance difference made by not having to recompile the code strings into bytecode for every run is substantial.
 * 
 * Because this macro expands to a block of code, rather than a single funtion call, the evaluation is stored in a `char*`
 * variable named using the `out_var` argument. The C string stored in `out_var` will need to be freed with `recomp_free`.
 * Failure to do so will result in a memory leak.
 * 
 * Intended to be used when the result of the evaluation is a Python `bytes` object. 
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param code_str The Python expression to evaluate. Should be a NULL-terminated C-string such as a string literal.
 * This code string will only be parsed and compiled once.
 * @param out_var The name of a `char*` argument that will hold the expression result.
 */
#define REPY_FN_EVAL_CACHE_BYTESTR(bytecode_identifier, code_str, out_var) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_EVAL_CACHE_BYTESTR", bytecode_identifier, REPY_CODE_EVAL, code_str) \
char* out_var = REPY_FN_EVAL_BYTESTR(bytecode_identifier)

/**
 * @brief Inserts a Python module into the local scope. 
 * 
 * The variable name for the module will be the same as the module name.
 * 
 * @param module_name The name of the Python module as a C string.
 */
#define REPY_FN_IMPORT(module_name) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, module_name, REPY_MakeSUH(REPY_ImportModule(module_name)))

/**
 * @brief Gets a variable from the local scope.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `REPY_Handle`.
 */
#define REPY_FN_GET(var_name) \
REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name);

/**
 * @brief Sets a variable in the the local scope, using a `REPY_Handle` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param py_object The python object to insert. Should be a `REPY_Handle`.
 */
#define REPY_FN_SET(var_name, py_object) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, py_object)

/**
 * @brief Gets a variable from the local scope and casts it to a `bool`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `bool`.
 */
#define REPY_FN_GET_BOOL(var_name) \
REPY_CastBool(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `bool` in the the local scope, using a `bool` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The python object to insert. Should be a `bool`.
 */
#define REPY_FN_SET_BOOL(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateBool(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `u8`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `u8`.
 */
#define REPY_FN_GET_U8(var_name) \
REPY_CastU8(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `int` in the the local scope, using a `u8` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `int`. Should be a `u8`.
 */
#define REPY_FN_SET_U8(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateU8(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `s8`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `s8`.
 */
#define REPY_FN_GET_S8(var_name) \
REPY_CastS8(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `int` in the the local scope, using a `s8` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `int`. Should be a `s8`.
 */
#define REPY_FN_SET_S8(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateS8(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `u16`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `u16`.
 */
#define REPY_FN_GET_U16(var_name) \
REPY_CastU16(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `int` in the the local scope, using a `u16` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `int`. Should be a `u16`.
 */
#define REPY_FN_SET_U16(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateU16(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `s16`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `s16`.
 */
#define REPY_FN_GET_S16(var_name) \
REPY_CastS16(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `int` in the the local scope, using a `s16` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `int`. Should be a `s16`.
 */
#define REPY_FN_SET_S16(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateS16(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `u32`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `u32`.
 */
#define REPY_FN_GET_U32(var_name) \
REPY_CastU32(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `int` in the the local scope, using a `u32` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `int`. Should be a `u32`.
 */
#define REPY_FN_SET_U32(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateU32(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `s32`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `s32`.
 */
#define REPY_FN_GET_S32(var_name) \
REPY_CastS32(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `int` in the the local scope, using a `s32` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `int`. Should be a `s32`.
 */
#define REPY_FN_SET_S32(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateS32(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `f32`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `f32`.
 */
#define REPY_FN_GET_F32(var_name) \
REPY_CastF32(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `float` in the the local scope, using a `f32` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `float`. Should be a `f32`.
 */
#define REPY_FN_SET_F32(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateF32(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `u64`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `u64`.
 */
#define REPY_FN_GET_U64(var_name) \
REPY_CastU64(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `int` in the the local scope, using a `u64` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `int`. Should be a `u64`.
 */
#define REPY_FN_SET_U64(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateU64(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `s64`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `s64`.
 */
#define REPY_FN_GET_S64(var_name) \
REPY_CastS64(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `int` in the the local scope, using a `s64` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `int`. Should be a `s64`.
 */
#define REPY_FN_SET_S64(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateS64(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `f64`.
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `f64`.
 */
#define REPY_FN_GET_F64(var_name) \
REPY_CastF64(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `float` in the the local scope, using a `f64` for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `float`. Should be a `f64`.
 */
#define REPY_FN_SET_F64(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateF64(value)))

/**
 * @brief Gets a variable from the local scope and casts it to a `char*`. Intended to be used when the variable object is a Python `str`. 
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * The C string returned by this macro will need to be freed with `recomp_free`. Failure to do so will result in a memory leak.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `char*`.
 */
#define REPY_FN_GET_STR(var_name) \
REPY_CastStr(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, var_name)))

/**
 * @brief Sets a variable of the Python type `str` in the the local scope, using a  NULL-terminated C string.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `str`. Should be a NULL-terminated C string.
 */
#define REPY_FN_SET_STR(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateStr(value)))

/**
 * @brief Sets a variable of the Python type `str` in the the local scope, using `char` array of `N` length for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The content of the Python `str`. Should be a `char*`.
 * @param len The length of `value` in bytes.
 */
#define REPY_FN_SET_STR_N(var_name, value, len) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateStr(value, len)))

/**
 * @brief Gets a variable from the local scope and casts it to a `char*`. Intended to be used when the variable object is a Python `bytes`. 
 * 
 * Note that this macro will not retrieve global variables.
 * 
 * The C string returned by this macro will need to be freed with `recomp_free`. Failure to do so will result in a memory leak.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @return The value of the variable as a `char*`.
 */
#define REPY_FN_GET_BYTESTR(var_name) \
REPY_CastBytes(REPY_MakeSUH(REPY_DictGetCStr(REPY_FN_LOCAL_SCOPE, REPY_MakeSUH(REPY_CreateBytes(var_name)))))

/**
 * @brief Sets a variable of the Python type `str` in the the local scope, using a  NULL-terminated C string.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The value of the Python `str`. Should be a NULL-terminated C string.
 */
#define REPY_FN_SET_BYTESTR(var_name, value) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateBytes(value)))

/**
 * @brief Sets a variable of the Python type `bytes` in the the local scope, using `char` array of `N` length for the value.
 * 
 * Note that the global scope will be unaffected.
 * 
 * @param var_name The name of the variable. Should be a NULL-terminated C string.
 * @param value The content of the Python `bytes`. Should be a `char*`.
 * @param len The length of `value` in bytes.
 */
#define REPY_FN_SET_BYTESTR_N(var_name, value, len) \
REPY_DictSetCStr(REPY_FN_LOCAL_SCOPE, var_name, REPY_MakeSUH(REPY_CreateBytes(value, len)))

/**
 * @brief Initializes the helpers for cached Pythonic `if` statements in the current scope.
 * 
 * This macro is used as part of `REPY_FN_IF_CACHE`, but you can invoke it manually if needed
 * (for example, if you're mixing Python and C `if` statements in one if/else block).
 * 
 * See the `REPY_IfStmtHelper` section for more information.
 * 
 * @param helper_identifier The variable name of the `REPY_IfStmtHelper` object, as well as the prefix for the
 * `static REPY_IfStmtChain*` object. 
 */
#define REPY_FN_IF_CACHE_INIT(helper_identifier) \
static REPY_IfStmtChain* helper_identifier ## _chain_root = NULL; \
REPY_IfStmtHelper helper_identifier; \
REPY_IfStmtHelper_InitInPlace(&helper_identifier, &helper_identifier ## _chain_root); 

/**
 * @brief Constructs a `if` statement that uses a cached Python expression executed in the current scope.
 * 
 * This macro is used as part of `REPY_FN_IF_CACHE` and `REPY_FN_ELIF_CACHE`, but you can invoke it manually if needed
 * (for example, if you're mixing Python and C `if` statements in one if/else block).
 * 
 * See the `REPY_IfStmtHelper` section for more information.
 * 
 * @param helper_identifier The variable name of the `REPY_IfStmtHelper` object. Needs to be the same as the value used 
 * when initializing this if/else block (using either `REPY_FN_IF_CACHE_INIT` or `REPY_FN_IF_CACHE`).
 * @param py_expression The Python expression to evaluate within the scope. Should be a NULL-terminated C string.
 */
#define REPY_FN_IF_STMT_CACHE(helper_identifier, py_expression) \
if ( \
    REPY_IfStmtHelper_Step( \
        &helper_identifier, \
        REPY_FN_GLOBAL_SCOPE, \
        REPY_FN_LOCAL_SCOPE, \
        py_expression, \
        __FILE_NAME__, \
        (char*) __func__, \
        __LINE__, \
        #helper_identifier \
    ) \
) 

/**
 * @brief Initializes the helpers for a cached Pythonic `if/else` block, and constructs the first `if` statement,
 * using a cached Python expression executed in the local scope.
 * 
 * This is the simplest way to handle Pythonic if/else blocks in C code, provided there are no additional C expressions
 * that need to be evaluated as well.
 * 
 * See the `REPY_IfStmtHelper` section for more information.
 * 
 * @param helper_identifier The variable name of the `REPY_IfStmtHelper` object, as well as the prefix for the
 * `static REPY_IfStmtChain*` object. 
 * @param py_expression The Python expression to evaluate within the scope. Should be a NULL-terminated C string.
 */
#define REPY_FN_IF_CACHE(helper_identifier, py_expression) \
REPY_FN_IF_CACHE_INIT(helper_identifier) \
REPY_FN_IF_CACHE_STMT(helper_identifier, py_expression) 

/**
 * @brief Constructs a `else if` statement that uses a cached Python expression executed in the current scope.
 * 
 * Requires using the helpers initialized by `REPY_FN_IF_CACHE_INIT` or `REPY_FN_IF_CACHE`.
 * 
 * See the `REPY_IfStmtHelper` section for more information.
 * 
 * @param helper_identifier The variable name of the `REPY_IfStmtHelper` object. Needs to be the same as the value used 
 * when initializing this if/else block (using either `REPY_FN_IF_CACHE_INIT` or `REPY_FN_IF_CACHE`).
 * @param py_expression The Python expression to evaluate within the scope. Should be a NULL-terminated C string.
 */
#define REPY_FN_ELIF_CACHE(helper_identifier, py_expression) \
else REPY_FN_IF_CACHE_STMT(helper_identifier, py_expression)

/**
 * @brief Constructs a `while` loop that uses a cached Python expression executed in the current scope.
 * 
 * The Python expression is cast to `bool` by the API after evaluation.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param py_expression The Python code to evaluate as the loop condition. Should be a NULL-terminated C-string such
 * as a string literal. This code string will only be parsed and compiled once.
 */
#define REPY_FN_WHILE_CACHE(bytecode_identifier, py_expression) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_WHILE_CACHE", identifier, REPY_CODE_EVAL, py_expression); \
while (REPY_FN_EVAL_BOOL(bytecode_identifier))

/**
 * @brief Constructs a `for` loop that iterates through a Python object in the current scope, similar to how a Pythonic `for` loop works.
 * 
 * A Python expression is evaluated in the object to iterate through. This can be a literal, a variable, function call, or any other
 * valid Python expression, so long as the result is iterable. Each iteration, the current object will be added to the local scope
 * using the variable name of `var_name`.
 * 
 * Much like `REPY_FOREACH`, A `REPY_IteratorHelper` object is created to manage the iteration process. The variable name for this helper
 * in the format of `bytecode_handle ## _iter). So if your `bytecode_handle` is `a`, the iterator will be called `a_iter`. In addition to
 * being added to the scope, the current object of the loop can be accessed via `iter_identifier->curr`, and the index of that object 
 * can be accessed via `iter_identifier->index`. See the `REPY_IteratorHelper` documentation for more information.
 * 
 * Also like `REPY_FOREACH`, this macro is set to clean up the `REPY_IteratorHelper` automatically,m once the iteration ends.
 * Ergo, you only have to clean up if you end the loop early, such as through a break or a return. The macros `REPY_FN_FOREACH_CACHE_CLEANUP_NOW`,
 * `REPY_FN_FOREACH_CACHE_RETURN`, and `REPY_FN_FOREACH_CACHE_RETURN` are provided to facillitate that. Failure to manually clean up the 
 * `REPY_IteratorHelper` when exiting the loop early will result in a memory leak.
 * 
 * @param bytecode_identifier The name for a static variable that will hold the Python bytecode handle once created.
 * @param var_name Each iteration of the loop, the current loop object will be added to the local scope under this variable name.
 * Should be a NULL-terminated C-string such as a string literal.
 * @param py_expression A Pythonic expression that evaluates to the iterable object. Should be a NULL-terminated C-string
 * such as a string literal. This code string will only be parsed and compiled once.
 */
#define REPY_FN_FOREACH_CACHE(bytecode_identifier, var_name, py_expression) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_FOREACH_CACHE", bytecode_identifier, REPY_CODE_EVAL, py_expression); \
REPY_FOREACH_BLOCK(bytecode_identifier ## _iter, REPY_MakeSUH(REPY_FN_EVAL(bytecode_identifier)), REPY_FN_LOCAL_SCOPE, var_name)

/**
 * @brief Manually clean up the `REPY_IteratorHelper` for a `REPY_FN_FOREACH_CACHE` loop.
 * 
 * Once this had been called, you'll have a crash if you attempt another pass of the loop. Either call `break` or `return` to exit.
 * 
 * @param bytecode_identifier The name for a static Python bytecode variable. Used to get the the `REPY_IteratorHelper` variable name.
 */
#define REPY_FN_FOREACH_CACHE_CLEANUP_NOW(bytecode_identifier) \
REPY_IteratorHelper_Destroy(bytecode_identifier ## _iter)

/**
 * @brief Manually clean up the `REPY_IteratorHelper` for a `REPY_FN_FOREACH_CACHE` loop, and immediately break.
 * 
 * Looks cleaner than calling both.
 * 
 * @param bytecode_identifier The name for a static Python bytecode variable. Used to get the the `REPY_IteratorHelper` variable name.
 */
#define REPY_FN_FOREACH_CACHE_BREAK(bytecode_identifier) \
REPY_FN_FOREACH_CACHE_CLEANUP_NOW(bytecode_identifier); break

/**
 * @brief Manually clean up the `REPY_IteratorHelper` for a `REPY_FN_FOREACH_CACHE` loop, and immediately return.
 * 
 * Supports returning a value.
 * 
 * @param bytecode_identifier The name for a static Python bytecode variable. Used to get the the `REPY_IteratorHelper` variable name.
 */
#define REPY_FN_FOREACH_CACHE_RETURN(bytecode_identifier) \
REPY_FN_FOREACH_CACHE_CLEANUP_NOW(bytecode_identifier); return

/**
 * @brief Constructs a C style for loop, using cached Python code.
 * 
 * While the various `FOREACH` macros iterate through a Python object in the way that a `for` loop does in Python,
 * this macro iterates in the way that a `for` loop in C does, with an initialization statement, a continuation condition expression,
 * and an update statement. The key is that, with this macro, each of these statements/expressions are Python code, evaluated in the
 * current scope.
 * 
 * @param bytecode_identifier Used to construct the names for 3 static Python bytecode variable, needed by the three statements/expressions.
 * @param py_init_statement The Python code to use as the initialization statement. Should be a NULL-terminated C-string such as a string literal.
 * @param py_init_statement The Python code to use as the continuation condition expression. Should be a NULL-terminated C-string such as a string literal.
 * @param py_init_statement The Python code to use as the update statement. Should be a NULL-terminated C-string such as a string literal.
 */
#define REPY_FN_FOR_CACHE(bytecode_identifier, py_init_statement, py_eval_expression, py_update_statement) \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_FOR_CACHE", bytecode_identifier ## _init_statement, REPY_CODE_EXEC, py_init_statement); \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_FOR_CACHE", bytecode_identifier ## _eval_expression, REPY_CODE_EVAL, py_eval_expression); \
REPY_INLINE_COMPILE_CACHE_BLOCK("REPY_FN_FOR_CACHE", bytecode_identifier ## _update_statement, REPY_CODE_EXEC, py_update_statement); \
for ( \
    REPY_FN_EXEC(bytecode_identifier ## _init_statement); \
    REPY_FN_EVAL_BOOL(bytecode_identifier ## _eval_expression); \
    REPY_FN_EXEC(bytecode_identifier ## _update_statement) \
) 

/** @}*/

/** \defgroup repy_preinit_funcs PreInit Functions
 * \brief Functions that affect Python Initialization.
 *  @{
 */

 /**
  * @brief Instruct REPY to add a path to Python's module search path during initialization.
  * 
  * If you pass the path of an NRM (such as the path to the NRM for this mod obtained via `recomp_get_mod_file_path`),
  * REPY treat the NRM as a .zip file and will be able to search it for Python modules. 
  * 
  * Python module files and folders can be added to the NRM by including them under the `additional_files` section of the `mod.toml`
  * 
  * @param REPY_PreInitAddToModuleSearchPath 
  */
REPY_IMPORT(void REPY_PreInitAddToModuleSearchPath(const unsigned char* nrm_file_path));


/** \defgroup repy_handle_funcs Handle Functions
 * \brief Functions that operate on `REPY_Handle` values directly, rather than the Python objects they represent.
 *  @{
 */

/**
 * @brief Releases a `REPY_Handle`, removing the reference to the Python object and rendering this handle invalid.
 * 
 * It is safe to use this function with a Single-Use handle, since it will not try to release the handle twice.
 * 
 * @param REPY_Release The handle to release.
 */
REPY_IMPORT(void REPY_Release(REPY_Handle py_handle));

/**
 * @brief Convienience function that marks a `REPY_Handle` as Single-Use and then returns the value of the provided handle.
 * 
 * This function primarily exists to allow you to nest calls to API functions without causing memory/resource leaks.
 * Consider the following code:
 * 
 * ```C
 * REPY_CastBool(REPY_CreateBool(true));
 * ```
 * 
 * This is will result in the `REPY_Handle` created by `REPY_CreateBool` not getting released. and since the handle was never 
 * captured in a variable, We can't make a call to release it. In effect, this is a memory leak.
 * 
 *  ```C
 * REPY_CastBool(REPY_MakeSUH(REPY_CreateBool(true)));
 * ```
 * 
 * By marking the `REPY_Handle` from `REPY_CreateBool` as Single-Use, it will be released as soon as REPY_CastBool is done with it.
 * Thus, no memory leak.
 * 
 * @param py_handle_no_release The handle to make Single-Use.
 * @return The same handle as was passed in via `py_handle_no_release`..
 */
REPY_IMPORT(REPY_Handle REPY_MakeSUH(REPY_Handle py_handle_no_release));

/**
 * @brief Returns whether the a handle value is mapped to a Python object.
 * 
 * A handle of 0 (`REPY_NO_OBJECT`) will always return false.
 * 
 * This function is special in that it will not release a Single-Use `REPY_Handle` handle.
 * 
 * @param py_handle_no_release The handle in question.
 * @return True if the handle is valid. False otherwise.
 */
REPY_IMPORT(bool REPY_IsValidHandle(REPY_Handle py_handle_no_release));

/**
 * @brief Gets whether or not a `REPY_Handle` is Single-Use.
 * 
 * This function is special in that it will not release a Single-Use `REPY_Handle` handle.
 * 
 * @param py_handle_no_release The handle in question.
 * @return True if the handle is Single-Use. False otherwise.
 */
REPY_IMPORT(bool REPY_GetSUH(REPY_Handle py_handle_no_release));

/**
 * @brief Sets whether or not a `REPY_Handle` is Single-Use.
 * 
 * This function is special in that it will not release a Single-Use `REPY_Handle` handle.
 * 
 * @param py_handle_no_release The handle in question.
 * @param value True will make the handle Single-Use. False will make it not Single-Use.
 */
REPY_IMPORT(void REPY_SetSUH(REPY_Handle py_handle_no_release, bool value));

/**
 * @brief Creates a new handle to the same Python object as another handle.
 * 
 * Note that, unlike the other functions in this category, this function WILL release Single-Use handles.
 * In the future, a `_no_release` version of this function may be added.
 * 
 * @param py_handle_no_release A handle for an object you need another handle to.
 * @return A new handle to the same object.
 */
REPY_IMPORT(REPY_Handle REPY_CopyHandle(REPY_Handle py_handle));


/** \defgroup repy_handle_funcs Module Functions
 * \brief Functions Used for Python module operations.
 *  @{
 */

/**
 * @brief Construct a new Python module from a NULL-terminated code string, importable by name.
 * 
 * @deprecated This is an artifact of the old pre-releases, and will be removed in the next update. 
 * To include custom modules, include them as additional files in your NRM.
 * 
 * The Python code of the module is run immediately, rather than on first import.
 * 
 * @param identifier The name of the new module. Should be NULL-terminated.
 * @param code The Python code for the module. Should be NULL-terminated.
 */
REPY_IMPORT(void REPY_LoadModule(const char* identifier, const char* code));

/**
 * @brief Construct a new Python module from a `char` array, importable by name.
 * 
 * @deprecated This is an artifact of the old pre-releases, and will be removed in the next update. 
 * To include custom modules, include them as additional files in your NRM.
 * 
 * The Python code of the module is run immediately, rather than on first import.
 * 
 * @param identifier The name of the new module. Should be NULL-terminated.
 * @param code The Python code for the module. Does not need to be null terminated.
 * @param len The length of the Python code, in bytes.
 */
REPY_IMPORT(void REPY_LoadModuleN(const char* identifier, const char* code, u32 len));

/**
 * @brief Imports a Python module by name and returns a handle to it.
 * 
 * Members of the module can be accessed with the various attribute access functions.
 * 
 * @param identifier The name of the Python module. Should be NULL-terminated.
 */
REPY_IMPORT(REPY_Handle REPY_ImportModule(const char* identifier));
/** @}*/

/** \defgroup repy_primative_funcs Primative Operations
 * \brief Functions that deal with converting between C and Python primatives.
 *  @{
 */

/**
 * @brief Returns a `REPY_Handle` for a Python `bool` object, based on a C `bool`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Even though both Python's `True` and `False` objects are immortal objects, will still take up space in the handle management system.
 * Ergo, failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `bool`.
 * @return A new handle for your Python `bool`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateBool(bool value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `bool` object, based on a C `bool`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateBool(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `bool`.
 * @return A new Single-Use handle for your Python `bool`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateBool_SUH(bool value));

/** @brief Casts a Python object to a C `bool`. 
 * 
 * Intended to be used with a Python `bool`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(bool REPY_CastBool(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `int` object, based on a C `u8`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `int`.
 * @return A new handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateU8(u8 value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `int` object, based on a C `u8`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateU8(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `int`.
 * @return A new Single-Use handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateU8_SUH(u8 value));

/** @brief Casts a Python object to a C `u8`. 
 * 
 * Intended to be used with a Python `int`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(u8 REPY_CastU8(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `int` object, based on a C `s8`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `int`.
 * @return A new handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateS8(s8 value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `int` object, based on a C `s8`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateS8(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `int`.
 * @return A new Single-Use handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateS8_SUH(s8 value));

/** @brief Casts a Python object to a C `s8`. 
 * 
 * Intended to be used with a Python `int`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(s8 REPY_CastS8(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `int` object, based on a C `u16`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `int`.
 * @return A new handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateU16(u16 value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `u16` object, based on a C `int`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateU16(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `int`.
 * @return A new Single-Use handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateU16_SUH(u16 value));

/** @brief Casts a Python object to a C `u16`. 
 * 
 * Intended to be used with a Python `int`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(u16 REPY_CastU16(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `int` object, based on a C `s16`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `int`.
 * @return A new handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateS16(s16 value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `int` object, based on a C `s16`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateS16(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `int`.
 * @return A new Single-Use handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateS16_SUH(s16 value));

/** @brief Casts a Python object to a C `s16`. 
 * 
 * Intended to be used with a Python `int`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(s16 REPY_CastS16(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `int` object, based on a C `u32`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `int`.
 * @return A new handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateU32(u32 value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `int` object, based on a C `u32`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateU32(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `int`.
 * @return A new Single-Use handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateU32_SUH(u32 value));

/** @brief Casts a Python object to a C `u32`. 
 * 
 * Intended to be used with a Python `int`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(u32 REPY_CastU32(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `int` object, based on a C `s32`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `int`.
 * @return A new handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateS32(s32 value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `int` object, based on a C `s32`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateS32(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `int`.
 * @return A new Single-Use handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateS32_SUH(s32 value));

/** @brief Casts a Python object to a C `s32`. 
 * 
 * Intended to be used with a Python `int`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(s32 REPY_CastS32(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `float` object, based on a C `f32`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `float`.
 * @return A new handle for your Python `float`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateF32(f32 value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `float` object, based on a C `f32`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateBool(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `float`.
 * @return A new Single-Use handle for your Python `float`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateF32_SUH(f32 value));

/** @brief Casts a Python object to a C `f32`. 
 * 
 * Intended to be used with a Python `float`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(f32 REPY_CastF32(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `int` object, based on a C `void*`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `int`.
 * @return A new handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreatePtr(void* value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `int` object, based on a C `void*`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreatePtr(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `int`.
 * @return A new Single-Use handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreatePtr_SUH(void* value));

/** @brief Casts a Python object to a C `void*`. 
 * 
 * Intended to be used with a Python `int`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(void* REPY_CastPtr(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `int` object, based on a C `u64`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `int`.
 * @return A new handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateU64(u64 value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `int` object, based on a C `u64`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateU64(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `int`.
 * @return A new Single-Use handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateU64_SUH(u64 value));

/** @brief Casts a Python object to a C `u64`. 
 * 
 * Intended to be used with a Python `int`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(u64 REPY_CastU64(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `int` object, based on a C `s64`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `int`.
 * @return A new handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateS64(s64 value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `int` object, based on a C `s64`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateS64(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `int`.
 * @return A new Single-Use handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateS64_SUH(s64 value)); 

/** @brief Casts a Python object to a C `s64`. 
 * 
 * Intended to be used with a Python `int`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(s64 REPY_CastS64(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `int` object, based on a C `u8`. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `int`.
 * @return A new handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateF64(f64 value));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `int` object, based on a C `bool`. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateBool(value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `int`.
 * @return A new Single-Use handle for your Python `int`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateF64_SUH(f64 value));

/** @brief Casts a Python object to a C `u8`. 
 * 
 * Intended to be used with a Python `float`. Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(f64 REPY_CastF64(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `str` object, based on a NULL-terminated C string. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `str`.
 * @return A new handle for your Python `str`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateStr(const char* string));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `str` object, based on a NULL-terminated C string. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateStr(string))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param string The value for the Python `str`.
 * @return A new Single-Use handle for your Python `str`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateStr_SUH(const char* string));

/**
 * @brief Returns a `REPY_Handle` for a Python `str` object, based on `char` array.
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param string The beginning of the `char` array to create a Python `str` from.
 * @param len The length of the `string` array.
 * @return A new Single-Use handle for your Python `str`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateStrN(const char* string, u32 len) );

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `str` object, based on `char` array. 
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateStrN_SUH(string, len))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param string The beginning of the `char` array to create a Python `str` from.
 * @param len The length of the `string` array.
 * @return A new Single-Use handle for your Python `str`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateStrN_SUH(const char* string, u32 len) );

/** @brief Casts a Python object to NULL-terminated C string. Intended to be used with a Python `str`.
 * 
 * Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(char* REPY_CastStr(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a Python `bytes` object, based on a NULL-terminated C string. 
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param value The value for the Python `bytes`.
 * @return A new handle for your Python `bytes`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateByteStr(const char* string));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `bytes` object, based on a NULL-terminated C string.
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateByteStr(string))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param value The value for the Python `bytes`.
 * @return A new Single-Use handle for your Python `bytes`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateByteStr_SUH(const char* string));

/**
 * @brief Returns a `REPY_Handle` for a Python `bytes` object, based on a `char` array.
 * 
 * This function will stop reading from `string` if it encounters a NULL-terminator. If you need to copy a region of memory
 * regardless of it's contents, use `REPY_MemcpyToBytes`.
 * 
 * The handle returned by this function will need to be released, either by making is Single-Use or by calling `REPY_Release`.
 * Failure to release this handle will result in a memory leak. 
 * 
 * @param string The beginning of the `char` array to create a Python `bytes` from.
 * @param len The maximum length of the `string` array.
 * @return A new Single-Use handle for your Python `bytes`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateByteStrN(const char* string, u32 len));

/**
 * @brief Returns a Single-Use `REPY_Handle` for a Python `bytes` object, based on a `char` array.
 * 
 * This function will stop reading from `string` if it encounters a NULL-terminator. If you need to copy a region of memory
 * regardless of it's contents, use `REPY_MemcpyToBytes`.
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateByteStrN(string))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param string The beginning of the `char` array to create a Python `bytes` from.
 * @param len The length of the `string` array.
 * @return A new Single-Use handle for your Python `bytes`.
 */
REPY_IMPORT(REPY_Handle REPY_CreateByteStrN_SUH(const char* string, u32 len));

/** @brief Casts a Python object to a NULL-terminated C string. Intended to be used with a Python `bytes`.
 * 
 * Behavior with other Python types may change between versions.
 * 
 * @param object The handle for the Python object in question.
 */
REPY_IMPORT(char* REPY_CastByteStr(REPY_Handle object));

/** @}*/

/** \defgroup repy_memcpy_funcs Memcpy Functions
 * \brief Functins used to directly transfer data between mod memory and the Python interpreter, using appropriate Python types.
 *  @{
 */

/**
 * @brief Create a Python `bytes` object from a chunk of mod memory.
 * 
 * This function copies the entire `src` region exactly as is, NULL-terminators and all.
 * 
 * @param src The beginning of the memory region to copy. 
 * @param len The size of the region to copy, in bytes. 
 * @param reverse Set to `false` to copy normally, or `true` to reverse the byte order of the memory being copied.
 * @return A `REPY_Handle` to the `bytes` obect created.
 */
REPY_IMPORT(REPY_Handle REPY_MemcpyToBytes(void* src, u32 len, u32 reverse));

/**
 * @brief Copy the content of a Python `bytes` object into mod memory.
 * 
 * The `bytes` data will be copied exactly as is, NULL-terminators and all. No additional NULL-terminator will be
 * added to the end of the data.
 * 
 * If the Python `bytes` object is smaller than `len`, then the remainder of destination will be untouched. If the
 * `bytes` object is larger than len, then copying will terminate when `len` is reached. Use the return value to get
 * how many bytes were actually copied.
 * 
 * Behavior with Python types other than `bytes` is undefined, and may change between versions.
 * 
 * @param dst The beginning of the memory region to write to. 
 * @param len The size of the destination region to copy, in bytes. 
 * @param reverse Set to `false` to copy normally, or `true` to reverse the byte order of the data being copied.
 * @param bytes_obj The Python `bytes` object to copy from.
 * @return The number of bytes actually copied.
 */
REPY_IMPORT(u32 REPY_MemcpyFromBytes(void* dst, u32 len, u32 reverse, REPY_Handle bytes_obj));


/**
 * @brief Copy the content of a Python `bytes` object into mod memory, automatically allocating the space for it
 * with `recomp_alloc`.
 * 
 * The `bytes` data will be copied exactly as is, NULL-terminators and all. No additional NULL-terminator will be
 * added to the end of the data.
 * 
 * Failure to free the `void*` returned by this function with `recomp_free` will result in a memory leak.
 * 
 * Behavior with Python types other than `bytes` is undefined, and may change between versions.
 * 
 * @param reverse Set to `false` to copy normally, or `true` to reverse the byte order of the data being copied.
 * @param bytes_obj The Python `bytes` object to copy from.
 * @param write_size A pointer to a `u32`, where the number of bytes copied can be written to.
 * @return A `void*` to the data copied into mod memory.
 */
REPY_IMPORT(void* REPY_AllocAndCopyBytes(u32 reverse, REPY_Handle bytes_obj, u32* write_size));

/**
 * @brief Create a Python `bytearray` object from a chunk of mod memory.
 * 
 * This function copies the entire `src` region exactly as is, NULL-terminators and all.
 * 
 * @param src The beginning of the memory region to copy. 
 * @param len The size of the region to copy, in bytes. 
 * @param reverse Set to `false` to copy normally, or `true` to reverse the byte order of the memory being copied.
 * @return A `REPY_Handle` to the `bytearray` obect created.
 */
REPY_IMPORT(REPY_Handle REPY_MemcpyToByteArray(void* src, u32 len, u32 reverse));

/**
 * @brief Copy the content of a Python `bytearray` object into mod memory.
 * 
 * The `bytearray` data will be copied exactly as is, NULL-terminators and all. No additional NULL-terminator will be
 * added to the end of the data.
 * 
 * If the Python `bytearray` object is smaller than `len`, then the remainder of destination will be untouched. If the
 * `bytearray` object is larger than len, then copying will terminate when `len` is reached. Use the return value to get
 * how many bytes were actually copied.
 * 
 * Behavior with Python types other than `bytearray` is undefined, and may change between versions.
 * 
 * @param dst The beginning of the memory region to write to. 
 * @param len The size of the destination region to copy, in bytes. 
 * @param reverse Set to `false` to copy normally, or `true` to reverse the byte order of the data being copied.
 * @param bytes_obj The Python `bytearray` object to copy from.
 * @return The number of bytes actually copied.
 */
REPY_IMPORT(u32 REPY_MemcpyFromByteArray(void* dst, u32 len, u32 reverse, REPY_Handle bytes_obj));

/**
 * @brief Copy the content of a Python `bytearray` object into mod memory, automatically allocating the space for it
 * with `recomp_alloc`.
 * 
 * The `bytes` data will be copied exactly as is, NULL-terminators and all. No additional NULL-terminator will be
 * added to the end of the data.
 * 
 * Failure to free the `void*` returned by this function with `recomp_free` will result in a memory leak.
 * 
 * Behavior with Python types other than `bytearray` is undefined, and may change between versions.
 * 
 * @param reverse Set to `false` to copy normally, or `true` to reverse the byte order of the data being copied.
 * @param bytes_obj The Python `bytearray` object to copy from.
 * @param write_size A pointer to a `u32`, where the number of bytes copied can be written to.
 * @return A `void*` to the data copied into mod memory.
 */
REPY_IMPORT(void* REPY_AllocAndCopyByteArray(u32 reverse, REPY_Handle bytes_obj, u32* write_size));

/** @}*/

/** \defgroup repy_general_funcs General Object Functions
 * \brief Functions related to accessing the members and propertied of objects.
 *  @{
 */

/**
 * @brief Gets the length of an appropriate Python object as a `u32`.
 * 
 * This function is analogous to Python's own `len` function, and should work in all the same circumstances.
 * 
 * @param object The Python object to get the length of.
 */
REPY_IMPORT(u32 REPY_Len(REPY_Handle object));

/**
 * @brief Returns a `REPY_Handle` for a named attribute of a Python object, using a `REPY_Handle` for the attribute name.
 * 
 * This function is analogous to Python's own `getattr` function, and should work in all the same circumstances.
 * 
 * @param object The parent Python object.
 * @param key The name of attribute you're trying to access. The Python object referenced should generally be a `str`, as is standard with `getattr`.
 * @param default_value_nullable A default Python object to return if the attribute isn't found. Use `REPY_NO_OBJECT` to ignore this argument.
 * Note that if this default object is returned, the return value will be a new handle to the same object.
 * @return A handle to the desired attribute. Not that `REPY_NO_OBJECT` being returned doesn't necessarily mean that the object doesn't exist, 
 * only that an error has occured.
 */
REPY_IMPORT(REPY_Handle REPY_GetAttr(REPY_Handle object, REPY_Handle key, REPY_Handle default_value_nullable));

/**
 * @brief Returns a `REPY_Handle` for a named attribute of a Python object, using a NULL-terminated C string for the name.
 * 
 * This function is analogous to Python's own `getattr` function, and should work in all the same circumstances.
 * 
 * @param object The parent Python object.
 * @param key The name of attribute you're trying to access. Should be a NULL-terminated C string.
 * @param default_value_nullable A default Python object to return if the attribute isn't found. Use `REPY_NO_OBJECT` to ignore this argument.
 * Note that if this default object is returned, the return value will be a new handle to the same object.
 * @return A handle to the desired attribute. Not that `REPY_NO_OBJECT` being returned doesn't necessarily mean that the object doesn't exist, 
 * only that an error has occured.
 */
REPY_IMPORT(REPY_Handle REPY_GetAttrCStr(REPY_Handle object, char* key, REPY_Handle default_value_nullable));

/**
 * @brief Assigned a named attribute of a Python object, using a `REPY_Handle` for the attribute name.
 * 
 * This function is analogous to Python's own `setattr` function, and should work in all the same circumstances.
 * 
 * @param object The parent Python object.
 * @param key The name of attribute you're trying to assign to. The Python object referenced should generally be a `str`, as is standard with `setattr`.
 * @param value The Python object to assign to the attribute.
 */
REPY_IMPORT(void REPY_SetAttr(REPY_Handle object, REPY_Handle key, REPY_Handle value));

/**
 * @brief Assigned a named attribute of a Python object, using a NULL-terminated C string for the name.
 * 
 * This function is analogous to Python's own `setattr` function, and should work in all the same circumstances.
 * 
 * @param object The parent Python object.
 * @param key The name of attribute you're trying to assign to. The Python object referenced should generally be a `str`, as is standard with `setattr`.
 * @param value The Python object to assign to the attribute.
 */
REPY_IMPORT(void REPY_SetAttrCStr(REPY_Handle object, char* key, REPY_Handle value));

/**
 * @brief Checks if a Python object has an attribute with a specific name, using a `REPY_Handle` for the attribute name.
 * 
 * This function is analogous to Python's own `hasattr` function, and should work in all the same circumstances.
 * 
 * @param object The parent Python object.
 * @param key The name of attribute to check. The Python object referenced should generally be a `str`, as is standard with `hasattr`.
 * @return `true` if the attribute exists, `false` otherwise.
 */
REPY_IMPORT(bool REPY_HasAttr(REPY_Handle object, REPY_Handle key));

/**
 * @brief Checks if a Python object has an attribute with a specific name, using a NULL-terminated C string for the name.
 * 
 * This function is analogous to Python's own `hasattr` function, and should work in all the same circumstances.
 * 
 * @param object The parent Python object.
 * @param key The name of attribute to check. Should be a NULL-terminated C string.
 * @return `true` if the attribute exists, `false` otherwise.
 */
REPY_IMPORT(bool REPY_HasAttrCStr(REPY_Handle object, char* key));

/**
 * @brief Deletes an attribute from a Python object by name, using a `REPY_Handle` for the attribute name.
 * 
 * This function is analogous to Python's own `delattr`, and should work all the same circumstances.
 * 
 * @param object The parent Python object.
 * @param key The name of attribute to delete. The Python object referenced should generally be a `str`, as is standard with `delattr`.
 */
REPY_IMPORT(void REPY_DelAttr(REPY_Handle object, REPY_Handle key));

/**
 * @brief Deletes an attribute from a Python object by name, using a NULL-terminated C string for the name.
 * 
 * This function is analogous to Python's own `delattr`, and should work all the same circumstances.
 * 
 * @param object The parent Python object.
 * @param key The name of attribute to delete. Should be a NULL-terminated C string. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(void REPY_DelAttrCStr(REPY_Handle object, char* key));
/** @}*/

/** \defgroup repy_iteration_funcs Module Functions
 * \brief Functions that operate on `REPY_Handle` values directly, rather than the Python objects they represent.
 * 
 * The same DLL mechanisms used by these functions are used internally by `REPY_IteratorHelper`.
 *  @{
 */

/**
 * @brief Gets an iterator for a given Python object.
 * 
 * This function is analogous to Python's own `iter` function, and should work in all the same circumstances.
 * 
 * @param object The Python object to get an iterator for. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_Iter(REPY_Handle object));

/**
 * @brief Gets the next Python object from a Python iterator.
 * 
 * This function is mostly analogous to Python's own `next` function, and should work in all the same circumstances.
 * 
 * Generally, this function returning `REPY_NO_OBJECT` indicates that an error has occured, but there is a special case. 
 * In Python, the end of iteraton is indicated by the raising of a `StopIteration` exception. Instead of requiring the user
 * to handle this exception manually handle this exception, the argument `process_stop_iteration` enables this functin 
 * silently and automatically handle the `StopIteration` exception while still returning `REPY_NO_OBJECT`. This way,
 * you can easily check whether iteration ended successfully by simply calling `REPY_IsErrorSet`, instead of having to
 * write more elaborate exception handling code.
 * 
 * @param iterator The Python iterator being used.
 * @param default_obj_nullable A default object to return if iteration is complete. Use `REPY_NO_OBJECT` to ignore this argument.
 * @param process_stop_iteration Set to `true` to enable automatic handling of the `StopIteration` exception. Set to `false` otherwise.
 * @return The next object from the iterator. Will be `REPY_NO_HANDLE` an error occured, or if `process_stop_iteration` is `true` and
 * iteration has ended
 */
REPY_IMPORT(REPY_Handle REPY_Next(REPY_Handle iterator, REPY_Handle default_obj_nullable, u32 process_stop_iteration));

/** @}*/

/** \defgroup repy_tuple_funcs Module Functions
 * \brief Functions that operate on Python `tuple` objects.
 *  @{
 */

/**
 * @brief Create a Python `tuple` and return a `REPY_Handle` for it.
 * 
 * Because the `REPY_Handle` created by this function requires manual release, it's not recommended to use this function nested
 * inside `REPY_Call` (or any of it's sister functions) to pass positional arguments. Use `REPY_CreateTuple_SUH` instead.
 * 
 * @param size The number of entries in the tuple. Use 0 for an empty tuple.
 * @param ... `REPY_Handle` arguments for each entry in the tuple. Leave out if `size` is 0.
 * @return A `REPY_Handle` for the new tuple. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_CreateTuple(u32 size, ...));

/**
 * @brief Create a Python `tuple` and return a Single-Use `REPY_Handle` for it.
 * 
 * This is the recommended function for constructing a positional arguments `tuple` nested inside a call 
 * to `REPY_Call` (or any of it's sister functions).
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateTuple(size, ...))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param size The number of entries in the tuple. Use 0 for an empty tuple.
 * @param ... `REPY_Handle` arguments for each entry in the tuple. Leave out if `size` is 0.
 * @return A Single-Use `REPY_Handle` for the new tuple. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_CreateTuple_SUH(u32 size, ...));

/**
 * @brief Create a Python `tuple` with exactly 2 entries and return a `REPY_Handle` for it.
 * 
 * Used primarily for constucting key/value pairs when calling `REPY_CreateDict`. However, because the `REPY_Handle` 
 * created by this function requires manual release, it's not recommended to use this nest a call to this function 
 * inside another.
 * 
 * @param key `REPY_Handle` argument for the first entry in the tuple.
 * @param value `REPY_Handle` argument for the second entry in the tuple.
 * @return A `REPY_Handle` for the new tuple. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_CreatePair(REPY_Handle key, REPY_Handle value));

/**
 * @brief Create a Python `tuple` with exactly 2 entries and return a Single-Use `REPY_Handle` for it.
 * 
 * Used primarily for constucting key/value pairs when calling `REPY_CreateDict`. Because the returned handle is 
 * Single-Use, this is the recommended function for nesting inside `REPY_CreateDict` calls.
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreatePair(key, value))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param key `REPY_Handle` argument for the first entry in the `tuple`.
 * @param value `REPY_Handle` argument for the second entry in the `tuple`.
 * @return A `REPY_Handle` for the new tuple. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_CreatePair_SUH(REPY_Handle key, REPY_Handle value));

/**
 * @brief Returns the Python object at a specific index from a `tuple`
 * 
 * Currently, is a known issue with this interface: Negative index values (which represent the position from the end of 
 * the `tuple` in Python code) do not currently work. This will be fixed in a future update.
 * 
 * Only works with Python `tuple` objects.
 * 
 * @param tuple The `tuple` to get an object from.
 * @param index The index of the desired object.
 * @return The Python object at `index` in `tuple`. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_TupleGetIndexS32(REPY_Handle tuple, int index));
/** @}*/

/** \defgroup repy_dict_funcs Module Functions
 * \brief Functions that operate on Python `dict` objects.
 *  @{
 */

// REPY_IMPORT(REPY_Handle REPY_CreateEmptyDict());
// REPY_IMPORT(REPY_Handle REPY_CreateEmptyDict_SUH());

/**
 * @brief Create a Python `dict` and return a `REPY_Handle` for it.
 * 
 * Because the `REPY_Handle` created by this function requires manual release, it's not recommended to use this function nested
 * inside `REPY_Call` (or any of it's sister functions) to pass keyword arguments. Use `REPY_CreateDict_SUH` instead.
 * 
 * @param size The number of entries in the `dict`. Use 0 for an empty `dict`.
 * @param ... `REPY_Handle` arguments for each entry in the `dict`. Each argument should be a two-entry tuple representing a 
 * key/value pair (using `REPY_CreatePair` or `REPY_CreatePair_SUH` makes this easy). Leave out if `size` is 0. 
 * @return A `REPY_Handle` for the new `dict`. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_CreateDict(u32 size, ...));

/**
 * @brief Create a Python `dict` and return a `REPY_Handle` for it.
 * 
 * This is the recommended function for constructing a keyword arguments `tuple` nested inside a call  to `REPY_Call` 
 * (or any of it's sister functions).
 * 
 * At this time, this function is just shorthand for `REPY_MakeSUH(REPY_CreateDict(size, ...))`, and thus will perform similarly.
 * However, internal performance improvements may make this function more performant in the future.
 * 
 * @param size The number of entries in the `dict`. Use 0 for an empty `dict`.
 * @param ... `REPY_Handle` arguments for each entry in the `dict`. Each argument should be a two-entry tuple representing a 
 * key/value pair (using `REPY_CreatePair` or `REPY_CreatePair_SUH` makes this easy). Leave out if `size` is 0. 
 * @return A `REPY_Handle` for the new `dict`. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_CreateDict_SUH(u32 size, ...));

/**
 * @brief Get an entry from a Python `dict`, using a REPY handle for the key.
 * 
 * Note that any hashable Python type can be used as a dict key, not just `str` objects.
 * 
 * @param dict The `dict` to get an entry from.
 * @param dict The key for the entry. Should be a hashable Python type.
 * @return The retrieved object. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_DictGet(REPY_Handle dict, REPY_Handle key));

/**
 * @brief Get an entry from a Python `dict`, using a NULL-terminated C string for the key.
 * 
 * The `key` variable will treated as a `str` object, as that is the most common use-case, as well as the use-case for
 * code execution scopes.
 * 
 * @param dict The `dict` to get an entry from.
 * @param dict The key for the entry. Should be a NULL-terminated C string.
 * @return The retrieved object. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_DictGetCStr(REPY_Handle dict, char* key));

/**
 * @brief Sets/adds a Python object to a Python `dict`, using a REPY handle for the key.
 * 
 * Note that any hashable Python type can be used as a dict key, not just `str` objects.
 * 
 * @param dict The `dict` to insert into.
 * @param dict The key for the entry. Should be a hashable Python type.
 * @param value The object to add.
 */
REPY_IMPORT(void REPY_DictSet(REPY_Handle dict, REPY_Handle key, REPY_Handle value));


/**
 * @brief Sets/adds a Python object to a Python `dict`, using a NULL-terminated C string for the key.
 * 
 * The `key` variable will treated as a `str` object, as that is the most common use-case, as well as the use-case for
 * code execution scopes.
 * 
 * @param dict The `dict` to insert into.
 * @param dict The key for the entry. Should be a NULL-terminated C string.
 * @param value The object to add.
 */
REPY_IMPORT(void REPY_DictSetCStr(REPY_Handle dict, char* key, REPY_Handle value));

/**
 * @brief Checks if a Python `dict` has an entry with a specific key, using a REPY handle for the key.
 * 
 * Note that any hashable Python type can be used as a dict key, not just `str` objects.
 * 
 * @param dict The `dict` to check for a key in.
 * @param dict The key for the entry. Should be a hashable Python type.
 * @return `true` if the entry exists, `false` otherwise.
 */
REPY_IMPORT(bool REPY_DictHas(REPY_Handle dict, REPY_Handle key));

/**
 * @brief Checks if a Python `dict` has an entry with a specific key, using a NULL-terminated C string for the key.
 * 
 * The `key` variable will treated as a `str` object, as that is the most common use-case, as well as the use-case for
 * code execution scopes.
 * 
 * @param dict The `dict` to check for a key in.
 * @param dict The key for the entry. Should be a NULL-terminated C string.
 * @return `true` if the entry exists, `false` otherwise.
 */
REPY_IMPORT(bool REPY_DictHasCStr(REPY_Handle dict, char* key));

/**
 * @brief Remove an entry from a `dict` via it's key, using a REPY handle for the key.
 * 
 * Note that any hashable Python type can be used as a dict key, not just `str` objects.
 * 
 * @param dict The `dict` to remove an entry from.
 * @param dict The key for the entry. Should be a hashable Python type.
 */
REPY_IMPORT(void REPY_DictDel(REPY_Handle dict, REPY_Handle key));

/**
 * @brief Remove an entry from a `dict` via it's key, using a NULL-terminated C string for the key.
 * 
 * The `key` variable will treated as a `str` object, as that is the most common use-case, as well as the use-case for
 * code execution scopes.
 * 
 * @param dict The `dict` to remove an entry from.
 * @param dict The key for the entry. Should be a NULL-terminated C string.
 */
REPY_IMPORT(void REPY_DictDelCStr(REPY_Handle dict, char* key));
/** @}*/

/** \defgroup repy_code_exec_funcs Code Compilation and Execution Functions
 * \brief Functions that deal with the compiling and executing Python code from inside mod code.
 *  @{
 */

/**
 * @brief Compile a Python code string into Python bytecode, using a `REPY_Handle` for the text.
 * 
 * This function is generally analogous to Python's own `compile` function, and should work in all the same circumstances.
 * 
 * @param code The Python text object containing the code to compile. Python's `compile` function works accepts `str`, `bytes`, and `bytearray`.
 * @param identifier A Python text object identifying where the code came from. 
 * @param mode A Python `str` identifying the type of code this is. Use "exec" for an executable statement or series of statements, "eval"
 * for an expression, or "single" for a interactive REPL statement.
 * @return A handle to the resulting Python bytecode object. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_Compile(REPY_Handle code, REPY_Handle identifier, REPY_Handle mode));

/**
 * @brief Compile a Python code string into Python bytecode, using a NULL-terminated C string for the code.
 * 
 * This function is generally analogous to Python's own `compile` function, and should work in all the same circumstances.
 * 
 * @param code A NULL-terminated C string of Python code to compile.
 * @param identifier A NULL-terminated C string identifying where the code came from. 
 * @param mode The type of code this is. Use `REPY_CODE_EXEC` for an executable statement or series of statements, `REPY_CODE_EVAL`
 * for an expression, or `REPY_CODE_SINGLE` for a interactive REPL statement.
 * @return A handle to the resulting Python bytecode object. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_CompileCStr(const char* code, const char* identifier, REPY_CodeMode mode));

/**
 * @brief Compile a Python code string into Python bytecode, using a C `char` array for the code.
 * 
 * This function is generally analogous to Python's own `compile` function, and should work in all the same circumstances.
 * 
 * @param code A pointer to the beginning of the `char` array to compile.
 * @param len The length of the `char` array to compile.
 * @param identifier A NULL-terminated C string identifying where the code came from. 
 * @param mode The type of code this is. Use `REPY_CODE_EXEC` for an executable statement or series of statements, `REPY_CODE_EVAL`
 * for an expression, or `REPY_CODE_SINGLE` for a interactive REPL statement.
 * @return A handle to the resulting Python bytecode object. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_CompileCStrN(const char* code, u32 len, const char* identifier, REPY_CodeMode mode));

/**
 * @brief Execute Python code statements from a `REPY_Handle`. Optionally provide `dict` objects to serve as a scope.
 * 
 * This function is generally analogous to Python's `exec` function, with the exception that calling directly from the API
 * means that there is no Python stack frame to inherit a scope from when executing. Hence, this function will create it's own
 * if no scope `dict` objects are provided.
 * 
 * As this function can execute precompiled Python bytecode, this is the recommended function for executing Python statements from the API.
 * 
 * @param code The Python code to execute. A precompiled bytecode object is recommended for performance reasons, but a Python `str`,
 * `bytes`, or `bytearray` object will also work.
 * @param global_scope_nullable The global scope to execute code in. Use `REPY_NO_OBJECT` to execute in a new, temporary scope.
 * @param local_scope_nullable The global scope to execute code in. Using `REPY_NO_OBJECT` will make the global and local scopes
 * be the same.
 * @return `true` if the code executed without error, `false` otherwise.
 */
REPY_IMPORT(bool REPY_Exec(REPY_Handle code, REPY_Handle global_scope_nullable, REPY_Handle local_scope_nullable));

/**
 * @brief Execute Python code from a NULL-terminated C string. Optionally provide `dict` objects to serve as a scope.
 * 
 * This function is generally analogous to Python's `exec` function, with the exception that calling directly from the API
 * means that there is no Python stack frame to inherit a scope from when executing. Hence, this function will create it's own
 * if no scope `dict` objects are provided.
 * 
 * This function is not recommended for most use-cases, since it must recompile the Python bytecode from the `code` string 
 * with each run.
 * 
 * @param code The Python code to execute. Should be a NULL-terminated C string.
 * @param global_scope_nullable The global scope to execute code in. Use `REPY_NO_OBJECT` to execute in a new, temporary scope.
 * @param local_scope_nullable The global scope to execute code in. Using `REPY_NO_OBJECT` will make the global and local scopes
 * be the same.
 * @return `true` if the code executed without error, `false` otherwise.
 */
REPY_IMPORT(bool REPY_ExecCStr(const char* code, REPY_Handle global_scope_nullable, REPY_Handle local_scope_nullable));

/**
 * @brief Execute Python code from a C `char` array. Optionally provide `dict` objects to serve as a scope.
 * 
 * This function is generally analogous to Python's `exec` function, with the exception that calling directly from the API
 * means that there is no Python stack frame to inherit a scope from when executing. Hence, this function will create it's own
 * if no scope `dict` objects are provided.
 * 
 * This function is not recommended for most use-cases, since it must recompile the Python bytecode from the `code` string 
 * with each run.
 * 
 * @param code A pointer to the beginning of the `char` array code to execute.
 * @param len The length of the `char` array in bytes.
 * @param global_scope_nullable The global scope to execute code in. Use `REPY_NO_OBJECT` to execute in a new, temporary scope.
 * @param local_scope_nullable The global scope to execute code in. Using `REPY_NO_OBJECT` will make the global and local scopes
 * be the same.
 * @return `true` if the code executed without error, `false` otherwise.
 */
REPY_IMPORT(bool REPY_ExecCStrN(const char* code, u32 len, REPY_Handle global_scope_nullable, REPY_Handle local_scope_nullable));

/**
 * @brief Evaluate a Python code expression from a `REPY_Handle`. Optionally provide `dict` objects to serve as a scope.
 * 
 * This function is generally analogous to Python's `eval` function, with the exception that calling directly from the API
 * means that there is no Python stack frame to inherit a scope from when executing. Hence, this function will create it's own
 * if no scope `dict` objects are provided.
 * 
 * As this function can execute precompiled Python bytecode, this is the recommended function for evaluating Python expressions from the API.
 * 
 * @param code The Python expression to evaluate. A precompiled bytecode object is recommended for performance reasons, but a Python `str`,
 * `bytes`, or `bytearray` object will also work.
 * @param global_scope_nullable The global scope to execute code in. Use `REPY_NO_OBJECT` to execute in a new, temporary scope.
 * @param local_scope_nullable The global scope to execute code in. Using `REPY_NO_OBJECT` will make the global and local scopes
 * be the same.
 * @return A handle for the resulting object. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_Eval(REPY_Handle code, REPY_Handle global_scope_nullable, REPY_Handle local_scope_nullable));

/**
 * @brief Evaluate a Python code expression from a NULL-terminated C string. Optionally provide `dict` objects to serve as a scope.
 * 
 * This function is generally analogous to Python's `eval` function, with the exception that calling directly from the API
 * means that there is no Python stack frame to inherit a scope from when executing. Hence, this function will create it's own
 * if no scope `dict` objects are provided.
 * 
 * This function is not recommended for most use-cases, since it must recompile the Python bytecode from the `code` string 
 * with each run.
 * 
 * @param code The Python expression to evaluate. Should be a NULL-terminated C string.
 * @param global_scope_nullable The global scope to execute code in. Use `REPY_NO_OBJECT` to execute in a new, temporary scope.
 * @param local_scope_nullable The global scope to execute code in. Using `REPY_NO_OBJECT` will make the global and local scopes
 * be the same.
* @return A handle for the resulting object. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_EvalCStr(const char* code, REPY_Handle global_scope_nullable, REPY_Handle local_scope_nullable));

/**
 * @brief Evaluate a Python code expression from a C `char` array. Optionally provide `dict` objects to serve as a scope.
 * 
 * This function is generally analogous to Python's `eval` function, with the exception that calling directly from the API
 * means that there is no Python stack frame to inherit a scope from when executing. Hence, this function will create it's own
 * if no scope `dict` objects are provided.
 * 
 * This function is not recommended for most use-cases, since it must recompile the Python bytecode from the `code` string 
 * with each run.
 * 
 * @param code A pointer to the beginning of the `char` array expression to evaluate.
 * @param len The length of the `char` array in bytes.
 * @param global_scope_nullable The global scope to execute code in. Use `REPY_NO_OBJECT` to execute in a new, temporary scope.
 * @param local_scope_nullable The global scope to execute code in. Using `REPY_NO_OBJECT` will make the global and local scopes
 * be the same.
 * @return A handle for the resulting object. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_EvalCStrN(const char* code, u32 len, REPY_Handle global_scope_nullable, REPY_Handle local_scope_nullable));

/** @}*/

/** \defgroup repy_fn_call_funcs Python Function Calling
 * \brief Functions that call Python functions and other callables.
 *  @{
 */

/**
 * @brief Call a Python function (or another callable object) by handle, and discard the return value.
 * 
 * `REPY_CreateTuple_SUH` and `REPY_CreateDict_SUH` serve as easy methods of nesting argument construction into a call to this function.
 * 
 * @param func The Python function or callable to call. Should be the actual function object itself.
 * @param args_nullable A `tuple` of the positional arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo positional arguments.
 * @param kwargs_nullable A `dict` of the keyword arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo keyword arguments.
 * @return `true` if the call executed without error, `false` otherwise.
 */
REPY_IMPORT(bool REPY_Call(REPY_Handle func, REPY_Handle args_nullable, REPY_Handle kwargs_nullable));

/**
 * @brief Call a Python function (or another callable object) by handle, and get the return value as a `REPY_Handle`.
 * 
 * Note that technically all Python functions and callables return a value. Functions that are typically thought of not having a return
 * will actually return Python's `None` object. This mean that, unless an error occurs, this API function will always return a `REPY_Handle`
 * that will need to be released.
 * 
 * `REPY_CreateTuple_SUH` and `REPY_CreateDict_SUH` serve as easy methods of nesting argument construction into a call to this function.
 * 
 * @param func The Python function or callable to call. Should be the actual function object itself.
 * @param args_nullable A `tuple` of the positional arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo positional arguments.
 * @param kwargs_nullable A `dict` of the keyword arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo keyword arguments.
 * @return `true` if the call executed without error, `false` otherwise.
 */
REPY_IMPORT(REPY_Handle REPY_CallReturn(REPY_Handle func, REPY_Handle args_nullable, REPY_Handle kwargs_nullable));

/**
 * @brief Call a Python object member (such as a module function, object method, or another callable) by parent object and attribute
 * name, and discard the return value. The attribute name should be a Python `str`.
 * 
 * `REPY_CreateTuple_SUH` and `REPY_CreateDict_SUH` serve as easy methods of nesting argument construction into a call to this function.
 * 
 * @param object The Python object to call a member of.
 * @param func The name of the object attribute to call. Should be a Python `str`.
 * @param args_nullable A `tuple` of the positional arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo positional arguments.
 * @param kwargs_nullable A `dict` of the keyword arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo keyword arguments.
 * @return `true` if the call executed without error, `false` otherwise.
 */
REPY_IMPORT(bool REPY_CallAttr(REPY_Handle object, REPY_Handle name, REPY_Handle args_nullable, REPY_Handle kwargs_nullable));

/**
 * @brief Call a Python object member (such as a module function, object method, or another callable) by parent object and attribute
 * name, and discard the return value. The attribute name should be a NULL-terminated C string.
 * 
 * `REPY_CreateTuple_SUH` and `REPY_CreateDict_SUH` serve as easy methods of nesting argument construction into a call to this function.
 * 
 * @param object The Python object to call a member of.
 * @param func The name of the object attribute to call. Should be a NULL-terminated C string.
 * @param args_nullable A `tuple` of the positional arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo positional arguments.
 * @param kwargs_nullable A `dict` of the keyword arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo keyword arguments.
 * @return `true` if the call executed without error, `false` otherwise.
 */
REPY_IMPORT(bool REPY_CallAttrCStr(REPY_Handle object, char* name, REPY_Handle args_nullable, REPY_Handle kwargs_nullable));

/**
 * @brief Call a Python object member (such as a module function, object method, or another callable) by parent object and attribute
 * name, and discard the return value. The attribute name should be a NULL-terminated C string.
 * 
 * Note that technically all Python functions and callables return a value. Functions that are typically thought of not having a return
 * will actually return Python's `None` object. This mean that, unless an error occurs, this API function will always return a `REPY_Handle`
 * that will need to be released.
 * 
 * `REPY_CreateTuple_SUH` and `REPY_CreateDict_SUH` serve as easy methods of nesting argument construction into a call to this function.
 * 
 * @param object The Python object to call a member of.
 * @param func The name of the object attribute to call. Should be a NULL-terminated C string.
 * @param args_nullable A `tuple` of the positional arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo positional arguments.
 * @param kwargs_nullable A `dict` of the keyword arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo keyword arguments.
 * @return A handle for the resulting object. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_CallAttrReturn(REPY_Handle object, REPY_Handle name, REPY_Handle args_nullable, REPY_Handle kwargs_nullable));

/**
 * @brief Call a Python object member (such as a module function, object method, or another callable) by parent object and attribute
 * name, and discard the return value. The attribute name should be a NULL-terminated C string.
 * 
 * Note that technically all Python functions and callables return a value. Functions that are typically thought of not having a return
 * will actually return Python's `None` object. This mean that, unless an error occurs, this API function will always return a `REPY_Handle`
 * that will need to be released.
 * 
 * `REPY_CreateTuple_SUH` and `REPY_CreateDict_SUH` serve as easy methods of nesting argument construction into a call to this function.
 * 
 * @param object The Python object to call a member of.
 * @param func The name of the object attribute to call. Should be a NULL-terminated C string.
 * @param args_nullable A `tuple` of the positional arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo positional arguments.
 * @param kwargs_nullable A `dict` of the keyword arguments to pass to the function. Use `REPY_NO_OBJECT` to forgo keyword arguments.
 * @return A handle for the resulting object. Will be `REPY_NO_HANDLE` if an error occured.
 */
REPY_IMPORT(REPY_Handle REPY_CallAttrCStrReturn(REPY_Handle object, char* name, REPY_Handle args_nullable, REPY_Handle kwargs_nullable));

/** @}*/

/** \defgroup repy_error_funcs Error Handling
 * \brief Functions used for Python error handling.
 * 
 * REPY API functions will try to catch any unhandled Python errors they raise, print them to the console, and make the exception objects available
 * through this interface. Note that passing bad/invalid handles (or using `REPY_NO_OBJECT` when not allowed) will not result in Python 
 * errors to be raised, because looking up the Python object for a given handle happens before the interpreter is invoked. As a result,
 * such issues will likely result in a crash instead.
 *  @{
 */

/**
 * @brief Checks if a Python exception was raised and caught by the REPY API.
 * 
 * If an exception was handled in Python code, then it will not be captured by REPY.
 * 
 * @return `true` if the REPY API currently has a Python exception captured, `false` otherwise.
 */
REPY_IMPORT(bool REPY_IsErrorSet());

/**
 * @brief Get the type of the current Python exception.
 * 
 * Note that each invokation of this method will result in a new handle being created to the appropriate type object.
 * 
 * @return The type for the currently captured Python exception. Returns `None` if no exception is currently captured.
 */
REPY_IMPORT(REPY_Handle REPY_GetErrorType());

/**
 * @brief Get the trace object for the Python exception.
 * 
 * Note that each invokation of this method will result in a new handle being created to the trace object.
 * 
 * @return The trace object for the currently captured Python exception. Returns `None` if no exception is currently captured.
 */
REPY_IMPORT(REPY_Handle REPY_GetErrorTrace());

/**
 * @brief Get the current Python exception.
 * 
 * Note that each invokation of this method will result in a new handle being created to the exception.
 * 
 * @return The currently captured Python exception. Returns `None` if no exception is currently captured.
 */
REPY_IMPORT(REPY_Handle REPY_GetErrorValue());

/**
 * @brief Releases the currently captured exception.
 * 
 * Call once you're done with your error handling, so that `REPY_IsErrorSet` doesn't return `true` for an error already dealt with.
 */
REPY_IMPORT(void REPY_ClearError());

/** @}*/

/** \defgroup repy_helper_funcs Helper Functions
 * \brief Various helper functions, primarily used by API macros.
 * 
 * You don't usually need to invoke these directly.
 *  @{
 */

/**
 * @brief Constructs the `filename` strings used by most macros that enable inlining Python code into C files.
 * 
 * The pointer returned by this function must be freed with `recomp_free`. Failure to do so will result in a memory leak.
 * 
 * @param category A category prefix for the filename string. Usually the name of the invoking macro.
 * @param filename A C filename to associate with a piece of Python code. Usually `_FILE_NAME__.
 * @param function_name The name of C function to associate with a piece of Python code. Usually `__func__`.
 * @param line_number A line number in a C file to associate with a piece of Python code. Usually `__LINE__`.
 * @param identifier An identifiying string for a piece of Python code. Usually the bytecode identifier from the C file.
 * @return The constructed source name as a NULL-terminated C string.
 */
REPY_IMPORT(char* REPY_InlineCodeSourceStrHelper(char* category, char* filename, char* function_name, u32 line_number, char* identifier));

/**
 * @brief Create a `REPY_IteratorHelper` object on the heap.
 * 
 * Used by the `REPY_FOREACH` and `REPY_FN_FOREACH_CACHE` macros.
 * 
 * @param py_object a `REPY_Handle` for the Python object to iterate through.
 * @param py_scope_nullable a `REPY_Handle` to a Python `dict` being used a local scope. Can be `REPY_NO_OBJECT`.
 * @param var_name the variable name for the `REPY_IteratorHelper` pointer. If `py_scope_nullable` is set to `REPY_NO_OBJECT`,
 * @return A pointer to the new `REPY_IteratorHelper` on the heap.
 */
REPY_IMPORT(REPY_IteratorHelper* REPY_IteratorHelper_Create(REPY_Handle py_object, REPY_Handle py_scope_nullable, const char* var_name));

/**
 * @brief Destructs a `REPY_IteratorHelper` object from the heap.
 * 
 * Used by the various cleanup macros for `REPY_FOREACH` and `REPY_FN_FOREACH_CACHE`.
 * 
 * `REPY_IteratorHelper_Update` can also destroy a `REPY_IteratorHelper` when it's finished, if `auto_destroy` is set to true.
 * 
 * @param helper A pointer to the `REPY_IteratorHelper` to destroy.
 */
REPY_IMPORT(void REPY_IteratorHelper_Destroy(REPY_IteratorHelper* helper));

/**
 * @brief Makes the `REPY_IteratorHelper` move on to the next object in the iteration.
 * 
 * Used by the `REPY_FOREACH` and `REPY_FN_FOREACH_CACHE` macros.
 * 
 * @param helper A pointer to the `REPY_IteratorHelper` to update.
 * @param auto_destroy If true, the `REPY_IteratorHelper` will automatically be destroyed once the loop ends.
 * @return `true` if the iteration/loop should continue. `false` once it's time to end.
 */
REPY_IMPORT(bool REPY_IteratorHelper_Update(REPY_IteratorHelper* helper, bool auto_destroy));

/**
 * @brief Creates a new link in a `REPY_IfStmtChain` if statement chain.
 * 
 * Invoked as part of `REPY_IfStmtHelper` operations, but exposed for manual use here.
 * 
 * @param expr_string The Python expression to evaluate. Should be a NULL-terminated C string. Will be compiled into Python bytecode immediately.
 * @param filename A C filename to associate with the Python expression. Usually `_FILE_NAME__.
 * @param function_name The name of C function to associate with the Python expression. Usually `__func__`.
 * @param line_number A line number in a C file to associate with the Python expression. Usually `__LINE__`.
 * @param identifier An identifiying string for the Python expression. Usually the bytecode identifier from the C file.
 * @return A pointer to the new `REPY_IfStmtChain` object.
 */
REPY_IMPORT(REPY_IfStmtChain* REPY_IfStmtChain_Create(char* expr_string, char* filename, char* function_name, u32 line_number, char* identifier));

/**
 * @brief Destructs a `REPY_IfStmtChain` object from the heap, recursively destructs and all additional links down the chain.
 * 
 * Exists for completeness sake. Since `REPY_IfStmtChain` pointers should usualy be `static` in their own functions, this doesn't really get much use.
 * 
 * @param helper A pointer to the `REPY_IfStmtChain` to recursively destroy.
 */
REPY_IMPORT(void REPY_IfStmtChain_Destroy(REPY_IfStmtChain* chain));

/**
 * @brief Initializes a pre-allocated `REPY_IfStmtHelper` for controlling managing a Pythonic if/else block.
 * 
 * Used by several of the various `REPY_FN_IF_CACHE` macros.
 * 
 * @param helper The a pointer to the `REPY_IfStmtHelper` to initialize.
 * @param root A pointer to the `REPY_IfStmtChain*` (ergo, a douple-pointer) variable for the first link in the if/else chain. For caching purposes, this
 * will usually be a `static` variable. If value of the variable at `root` us NULL, that will be taken to mean that the chain has not been created yet
 * (IE, this is the first run of this if/else block).
 */
REPY_IMPORT(void REPY_IfStmtHelper_InitInPlace(REPY_IfStmtHelper* helper, REPY_IfStmtChain** root));

/**
 * @brief Steps through and evaluate the next link in the `REPY_IfStmtChain` chain provided to the `REPY_IfStmtHelper`, or creates a new link if one
 * doesn't exist.
 * 
 * Used by several of the various `REPY_FN_IF_CACHE` macros. Can be used within the actual C `if` statements themselves.
 * 
 * @param helper A pointer to the `REPY_IfStmtHelper` controlling this if/else block.
 * @param global_scope The global scope `dict` to evaluate the expression in.
 * @param local_scope The local scope `dict` to evaluate the expression in.
 * @param expr_string The Python expression to evaluate. Should be a NULL-terminated C string. Will be compiled into Python bytecode immediately.
 * This argument is only used if there is no next link in the `REPY_IfStmtChain` chain (meaning the link needs to be created).
 * @param filename A C filename to associate with the Python expression. Usually `_FILE_NAME__.
 * This argument is only used if there is no next link in the `REPY_IfStmtChain` chain (meaning the link needs to be created).
 * @param function_name The name of C function to associate with the Python expression. Usually `__func__`.
 * This argument is only used if there is no next link in the `REPY_IfStmtChain` chain (meaning the link needs to be created).
 * @param line_number A line number in a C file to associate with the Python expression. Usually `__LINE__`.
 * This argument is only used if there is no next link in the `REPY_IfStmtChain` chain (meaning the link needs to be created).
 * @param identifier An identifiying string for the Python expression. Usually the bytecode identifier from the C file.
 * This argument is only used if there is no next link in the `REPY_IfStmtChain` chain (meaning the link needs to be created).
 */
REPY_IMPORT(bool REPY_IfStmtHelper_Step(REPY_IfStmtHelper* helper, REPY_Handle global_scope, REPY_Handle local_scope, char* expr_string, char* filename, char* function_name, u32 line_number, char* identifier));
/** @}*/
#endif
