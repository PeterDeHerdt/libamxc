/****************************************************************************
**
** Copyright (c) 2020 SoftAtHome
**
** Redistribution and use in source and binary forms, with or
** without modification, are permitted provided that the following
** conditions are met:
**
** 1. Redistributions of source code must retain the above copyright
** notice, this list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above
** copyright notice, this list of conditions and the following
** disclaimer in the documentation and/or other materials provided
** with the distribution.
**
** Subject to the terms and conditions of this license, each
** copyright holder and contributor hereby grants to those receiving
** rights under this license a perpetual, worldwide, non-exclusive,
** no-charge, royalty-free, irrevocable (except for failure to
** satisfy the conditions of this license) patent license to make,
** have made, use, offer to sell, sell, import, and otherwise
** transfer this software, where such license applies only to those
** patent claims, already acquired or hereafter acquired, licensable
** by such copyright holder or contributor that are necessarily
** infringed by:
**
** (a) their Contribution(s) (the licensed copyrights of copyright
** holders and non-copyrightable additions of contributors, in
** source or binary form) alone; or
**
** (b) combination of their Contribution(s) with the work of
** authorship to which such Contribution(s) was added by such
** copyright holder or contributor, if, at the time the Contribution
** is added, such addition causes such combination to be necessarily
** infringed. The patent license shall not apply to any other
** combinations which include the Contribution.
**
** Except as expressly stated above, no rights or licenses from any
** copyright holder or contributor is granted under this license,
** whether expressly, by implication, estoppel or otherwise.
**
** DISCLAIMER
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
** CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
** INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
** MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR
** CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
** USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
** AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
** LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
** ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************/

#if !defined(__AMXC_VARIANT_H__)
#define __AMXC_VARIANT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <inttypes.h>

#include <amxc/amxc_htable.h>
#include <amxc/amxc_llist.h>
#include <amxc/amxc_string.h>
#include <amxc/amxc_timestamp.h>
#include <amxc/amxc_common.h>

/**
   @file
   @brief
   Ambiorix variant API header file
 */

/**
   @ingroup amxc_containers
   @defgroup amxc_variant variant

   A variant is a generic data container and can contain primitive types
   or composite types.

   @section amxc_var_primitive Variant Primitive Types
   - null
   - cstring_t (char *)
   - int64_t
   - uint64_t
   - double
   - bool

   @section amxc_var_composite Variant Composite Types
   - amxc_llist_t - contains a list of variants
   - amxc_htable_t - contains a hash table with key value pairs where the key is
                     a string and the value is a variant

 */

#define AMXC_VAR_ID_INVALID      UINT32_MAX      /**< Invalid variant type identifier */
#define AMXC_VAR_ID_NULL         0               /**< the null variant id */
#define AMXC_VAR_ID_CSTRING      1               /**< the c-string variant id, null terminated string */
#define AMXC_VAR_ID_INT8         2               /**< signed 8 bit integer variant id*/
#define AMXC_VAR_ID_INT16        3               /**< signed 16 bit integer variant id*/
#define AMXC_VAR_ID_INT32        4               /**< signed 32 bit integer variant id*/
#define AMXC_VAR_ID_INT64        5               /**< signed 64 bit integer variant id*/
#define AMXC_VAR_ID_UINT8        6               /**< unsigned 8 bit integer variant id*/
#define AMXC_VAR_ID_UINT16       7               /**< unsigned 16 bit integer variant id*/
#define AMXC_VAR_ID_UINT32       8               /**< unsigned 32 bit integer variant id*/
#define AMXC_VAR_ID_UINT64       9               /**< unsigned 64 bit integer variant id*/
#define AMXC_VAR_ID_FLOAT        10              /**< float variant id*/
#define AMXC_VAR_ID_DOUBLE       11              /**< double variant id*/
#define AMXC_VAR_ID_BOOL         12              /**< boolean variant id*/
#define AMXC_VAR_ID_LIST         13              /**< amxc linked list variant id*/
#define AMXC_VAR_ID_HTABLE       14              /**< amxc htable variant id*/
#define AMXC_VAR_ID_FD           15              /**< file descriptor variant id*/
#define AMXC_VAR_ID_TIMESTAMP    16              /**< timestamp variant id*/
#define AMXC_VAR_ID_CSV_STRING   17              /**< comma separated values string id*/
#define AMXC_VAR_ID_SSV_STRING   18              /**< space separated values string id*/

#define AMXC_VAR_ID_ANY          19              /**< Can be used in coversion function */

#define AMXC_VAR_ID_CUSTOM_BASE  20              /**< start id for custom variants, id is assigned when registering */
#define AMXC_VAR_ID_MAX          UINT32_MAX      /**< Invalid variant type id */

#define AMXC_VAR_NAME_NULL       "null"          /**< the null variant name */
#define AMXC_VAR_NAME_CSTRING    "cstring_t"     /**< the c-string variant name*/
#define AMXC_VAR_NAME_INT8       "int8_t"        /**< the int8_t variant name*/
#define AMXC_VAR_NAME_INT16      "int16_t"       /**< the int16_t variant name*/
#define AMXC_VAR_NAME_INT32      "int32_t"       /**< the int32_t variant name*/
#define AMXC_VAR_NAME_INT64      "int64_t"       /**< the int64_t variant name*/
#define AMXC_VAR_NAME_UINT8      "uint8_t"       /**< the uint8_t variant name*/
#define AMXC_VAR_NAME_UINT16     "uint16_t"      /**< the uint16_t variant name*/
#define AMXC_VAR_NAME_UINT32     "uint32_t"      /**< the uint32_t variant name*/
#define AMXC_VAR_NAME_UINT64     "uint64_t"      /**< the uint64_t variant name*/
#define AMXC_VAR_NAME_FLOAT      "float"         /**< the float variant name*/
#define AMXC_VAR_NAME_DOUBLE     "double"        /**< the double variant name*/
#define AMXC_VAR_NAME_BOOL       "bool"          /**< the bool variant name*/
#define AMXC_VAR_NAME_LIST       "amxc_llist_t"  /**< the amxc linked list variant name*/
#define AMXC_VAR_NAME_HTABLE     "amxc_htable_t" /**< the amxc htable variant name*/
#define AMXC_VAR_NAME_FD         "fd_t"          /**< the file descriptor variant name*/
#define AMXC_VAR_NAME_TIMESTAMP  "amxc_ts_t"     /**< the time stamp variant name*/
#define AMXC_VAR_NAME_CSV_STRING "csv_string_t"  /**< the time stamp variant name*/
#define AMXC_VAR_NAME_SSV_STRING "ssv_string_t"  /**< the time stamp variant name*/

#define AMXC_VAR_FLAG_DEFAULT   0x00             /**< Default behaviour */
#define AMXC_VAR_FLAG_COPY      0x01             /**< Copy the variant & variant data */
#define AMXC_VAR_FLAG_UPDATE    0x02             /**< Update if already exists */

/**
   @brief
   Convenience macro
 */
#define cstring_t char *
#define csv_string_t char *
#define ssv_string_t char *

/**
   @brief
   Convenience macro
 */
#define fd_t int

/**
   @ingroup amxc_variant
   @brief
   Convenience macro for setting a value in a variant.

   Macro expands to amxc_var_set_<TYPE>>(var, data)
 */
#define amxc_var_set(type, var, data) amxc_var_set_ ## type(var, data)

/**
   @ingroup amxc_variant
   @brief
   Convenience macro for adding a variant to composite variant type.

   Macro expands to amxc_var_add_new_<TYPE>>(var, data)
 */
#define amxc_var_add(type, var, data) amxc_var_add_new_ ## type(var, data)

/**
   @ingroup amxc_variant
   @brief
   Convenience macro for adding a variant to composite variant type.

   Macro expands to amxc_var_add_new_key_<TYPE>>(var, key, data)
 */
#define amxc_var_add_key(type, var, key, data) amxc_var_add_new_key_ ## type(var, key, data)

/**
   @ingroup amxc_variant
   @brief
   Dynamic cast a variant to a certain type.

   Conversion is applied and a copy is given.
   If a pointer is returned, the allocated memory must be freed.

   Macro expands to amxc_var_get_<TYPE>(var)
 */
#define amxc_var_dyncast(type, var) amxc_var_get_ ## type(var)

/**
   @ingroup amxc_variant
   @brief
   Const cast a variant to a certain type.

   If the given variant is not of the type specified, it will return NULL.
   Only works on variants containing pointer types.

   No conversions are applied, no copies are done. The pointer returned is
   the pointer in the variant.

   Macro expands to amxc_var_get_const_<TYPE>(var)
 */
#define amxc_var_constcast(type, var) amxc_var_get_const_ ## type(var)

/**
   @ingroup amxc_variant
   @brief
   Takes the content from a variant.

   If the given variant is not of the type specified, it will return NULL.
   Only works on variants containing pointer types.

   No conversions are applied, no copies are done. The pointer returned is
   the pointer in the variant. The variant is reset to the "null" variant.

   Ownership of the pointer is transferd to the caller.

   Macro expands to amxc_var_take_<TYPE>(var)
 */
#define amxc_var_take(type, var) amxc_var_take_ ## type(var)

#define amxc_var_push(type, var, val) amxc_var_push_ ## type(var, val)

/**
   @brief
   Get the variant pointer from an amxc htable iterator.

   The htable iterator given, must be an amxc htable iterator of a variant.

   Gives the pointer to the variant.
 */
#define amxc_var_from_htable_it(ht_it) \
    ((amxc_var_t *) (((char *) ht_it) - offsetof(amxc_var_t, hit)))

/**
   @brief
   Get the variant pointer from an amxc linked list iterator.

   The linked list iterator given, must be a linked list iterator of a variant.

   Gives the pointer to the variant.
 */
#define amxc_var_from_llist_it(ll_it) \
    ((amxc_var_t *) (((char *) ll_it) - offsetof(amxc_var_t, lit)))

typedef struct _amxc_var {
    amxc_llist_it_t lit;        /**< Linked list iterator, can be used to store the variant in a linked list */
    amxc_htable_it_t hit;       /**< Hash table iterator, can be used to store the variant in a hash table */
    uint32_t type_id;           /**< Variant type */
    union
    {
        char *s;                /**< String */
        int8_t i8;              /**< 8 bit signed integer */
        int16_t i16;            /**< 16 bit signed integer */
        int32_t i32;            /**< 32 bit signed integer */
        int64_t i64;            /**< 64 bit signed integer */
        uint8_t ui8;            /**< 8 bit unsigned integer */
        uint16_t ui16;          /**< 16 bit unsigned integer */
        uint32_t ui32;          /**< 32 bit unsigned integer */
        uint64_t ui64;          /**< 64 bit unsigned integer */
        float f;                /**< float */
        double d;               /**< double */
        bool b;                 /**< boolean */
        amxc_llist_t vl;        /**< ambiorix linked list of variants */
        amxc_htable_t vm;       /**< ambiorix hash table (key - value pair) of variants */
        int fd;                 /**< file descriptor */
        amxc_ts_t ts;           /**< time stamp */
        void *data;             /**< pointer to hold custom data types */
    } data;                     /**< Variant data */
} amxc_var_t;

/**
   @ingroup amxc_variant
   @brief
   Allocates a variant and initializes it to the null variant type.

   A variant is a container that can hold any type.

   @note
   The allocated memory must be freed when not used anymore,
   use @ref amxc_var_delete to free the memory

   @param var pointer to a pointer that points to the new allocated variant

   @return
   When allocation is successfull, this functions returns 0.
 */
int amxc_var_new(amxc_var_t **var);

/**
   @ingroup amxc_variant
   @brief
   Frees the previously allocated variant.

   Frees the allocated memory and sets the pointer to NULL.

   @note
   Only call this function for variants that are allocated on the heap using
   @ref amxc_var_new

   @param var a pointer to the location where the pointer to the variant is
              stored
 */
void amxc_var_delete(amxc_var_t **var);

/**
   @ingroup amxc_variant
   @brief
   Initializes a variant.

   Call this to initialize variants structures (amxc_var_t) allocated on the stack.

   When the variant is not needed anymore call amxc_var_clean to make sure
   that all allocated memory to store the data is freed. Failing to do so could
   lead to a memory leak.

   The variant will be initialized as a null variant. To change the variant type
   use @ref amxc_var_set_type.

   Do not re-initialize an already initialized or used variant. The data contained
   in the variant is lost, no memory is freed if needed. To reset the variant back to
   a null variant use amxc_var_clean.

   @param var pointer to a variant structure.

   @return
   When initialization is successful, this functions returns 0.
 */
int amxc_var_init(amxc_var_t * const var);

/**
   @ingroup amxc_variant
   @brief
   Clean-up and reset variant.

   If memory was allocated to contain the data for the variant, it will be freed.
   The variant is reset to the null variant.

   @param var pointer to a variant structure.
 */
void amxc_var_clean(amxc_var_t * const var);

/**
   @ingroup amxc_variant
   @brief
   Change the variant data type.

   If the variant was already containing data, this will be cleaned up.
   The type is changed and the data part reset to default value. (0, NULL, false, ...)

   @param var pointer to variant struct
   @param type the variant type id

   @return
   When the type is set, this functions returns 0, any other value in case of failure.
 */
int amxc_var_set_type(amxc_var_t * const var, const uint32_t type);

/**
   @ingroup amxc_variant
   @brief
   Copy the type and data from one variant (source) in another variant (destination).

   The destination variant is first set to the same type as the source variant using
   @ref amxc_var_set_type and then the data is copied.

   The content of both variants after a copy is identical.
   If the copy fails, the destination variant is a null variant.

   The list and htable iterators are not copied.

   @param dest pointer to variant struct, the destination
   @param src pointer to variant struct, the source

   @return
   When the copy fails, this functions returns a none 0 value and the destination
   variant is reset to a null variant.
 */
int amxc_var_copy(amxc_var_t * const dest, const amxc_var_t * const src);

/**
   @ingroup amxc_variant
   @brief
   Converts one variant (source) to another variant(destination) using the specified
   variant type id.

   If no type is found with the geven type id, the converson fails.
   If no conversion methods are available the conversion fails.

   If the source variant type has a convert to method, this is called first.
   If that function fails to convert, the convert from method of the destination
   variant type is called.

   If conversion fails or is not supported the destination variant is reset to
   the null variant.

   @param dest pointer to a variant struct, the destination
   @param src pointer to a variant struct, the source
   @param type_id the variant type id to which the source data needs to be converted to

   @return
   When the conversion fails, this functions returns a none 0 value and
   the destination variant is reset to a null variant.
 */
int amxc_var_convert(amxc_var_t * const dest,
                     const amxc_var_t *src,
                     int type_id);
/**
   @ingroup amxc_variant
   @brief
   Compares two variants.

   Tries to compare the data of two variants.
   If it is not possible to compare the two variants, the function fails and
   returns a non 0 value.

   The comparison result is put in result:
   - when 0: var1 == var2
   - when > 0: var1 > var2
   - when < 0: var1 < var2

   @param var1 pointer to variant structure, the left value
   @param var2 pointer to variant structure, the right value
   @param result pointer to integer, will contain comparison result

   @return
   if it is not possible to compare the variants (depends on the types),
   this function returns none 0.
 */
int amxc_var_compare(const amxc_var_t * const var1,
                     const amxc_var_t * const var2,
                     int * const result);

/**
   @ingroup amxc_variant
   @brief
   Get a reference to a part of composed variant using a key.

   If the data contained in the variant is composed of different parts
   and some or all parts can be identified with a key, a reference to such part
   can be retrieved with this function.

   @param var pointer to variant structure, containing the composed data
   @param key string containing the key
   @param flags bitmap field. Bitwise ORed of the AMXC_VAR_FLAG_XXXX

   @return
   When no part is found with the given key a null pointer is returned.
   The pointer returned is pointing to a real part and should not be freed unless
   the variant part must be removed from the composed variant.
 */
amxc_var_t *amxc_var_get_key(const amxc_var_t * const var,
                             const char * const key,
                             const int flags);
/**
   @ingroup amxc_variant
   @brief
   Sets a part of composed variant using a key.

   If the data contained in the variant is composed of different parts
   and some or all parts can be identified with a key, the data of such a part
   can be changed with this function.

   The default behaviour is not to copy anything, just store the given variant.
   Using AMXC_VAR_FLAG_COPY this behavior can be changed into copy the variant
   and the variant data.

   @note
   The behaviour depends on the variant type of the destination variant (var parameter).
   Please read the documentation of the destination variant type.

   @param var pointer to variant structure, containing the composed data
   @param key string containing the key
   @param data a variant containing the data the will be set
   @param flags bitmap, see AMXC_VAR_FLAG_DEFAULT, AMXC_VAR_FLAG_COPY,
                AMXC_VAR_FLAG_UPDATE

   @return
   When the data has been set this function returns 0, when failed, the function
   returns a non-zero value.
 */
int amxc_var_set_key(amxc_var_t * const var,
                     const char * const key,
                     amxc_var_t *data,
                     const int flags);

/**
   @ingroup amxc_variant
   @brief
   Get a reference to a part of composed variant using an index.

   If the data contained in the variant is composed of different parts
   and some or all parts can be identified with an index, a reference to such part
   can be retrieved with this function.

   @param var pointer to variant structure, containing the composed data
   @param index the index
   @param flags bitmap field. Bitwise ORed of the AMXC_VAR_FLAG_XXXX

   @return
   When no part is found with the given key a null pointer is returned.
   The pointer returned is pointing to a real part.
 */
amxc_var_t *amxc_var_get_index(const amxc_var_t * const var,
                               const int64_t index,
                               const int flags);
/**
   @ingroup amxc_variant
   @brief
   Set a part of composed variant using an index.

   If the data contained in the variant is composed of different parts
   and some or all parts can be identified with an index, the data of such a
   part can be changed with this function.

   @param var pointer to variant structure, containing the composed data
   @param index the index
   @param data a variant containing the data the will be set
   @param flags bitmap, see AMXC_VAR_FLAG_DEFAULT, AMXC_VAR_FLAG_COPY,
                AMXC_VAR_FLAG_UPDATE

   @return
   When the data has been set this function returns 0, when failed, the function
   returns a non-zero value.
 */
int amxc_var_set_index(amxc_var_t * const var,
                       const int64_t index,
                       amxc_var_t *data,
                       const int flags);

/**
   @ingroup amxc_variant
   @brief
   Adds a new variant with a key to a composite variant.

   The type of the given variant must be of a composite type and must at least
   support the get_key and set_key functions.
   A new variant object is added to the composite variant. The new variant is of
   the null-type.

   @param var pointer to a variant struct to wich a new variant will be added.
   @param key the new key that needs to be added

   @return
   Pointer to the new added variant or NULL when it was not possible to add a new
   key to the variant.
 */
amxc_var_t *amxc_var_add_new_key(amxc_var_t * const var,
                                 const char *key);

/**
   @ingroup amxc_variant
   @brief
   Adds a new variant to a composite variant.

   The type of the given variant must be of a composite type and must at least
   support the get_index and set_index functions.
   A new variant object is added to the composite variant. The new variant is of
   the null-type.

   @param var pointer to a variant struct

   @return
   Pointer to the new added variant or NULL when it was not possible to add a new
   key to the variant.
 */
amxc_var_t *amxc_var_add_new(amxc_var_t * const var);


/**
   @ingroup amxc_variant
   @brief
   Retrieves the variant at the given path of a composite variant.

   The type of the given variant is a composite type and some of the parts are
   composite variants as well, this function makes it easy to retrieve a variant
   deep down in the composite variant structure.
   The path consists of a sequence of keys and/or indexes seprated by a '.'.
   If the path exists, the pointer to the variant at that position is returned.

   @param var pointer to a variant struct, the variant type should be a composite type
   @param path path to a variant in the composite variant structure, a path is
               a sequence of indexes and/or keys seperated by a '.'
   @param flags bitmap, see AMXC_VAR_FLAG_DEFAULT, AMXC_VAR_FLAG_COPY,
                AMXC_VAR_FLAG_UPDATE

   @return
   Pointer to the variant at the given path or NULL if the path does not exist.
 */
amxc_var_t *amxc_var_get_path(const amxc_var_t * const var,
                              const char * const path,
                              const int flags);

/**
   @ingroup amxc_variant
   @brief
   Retrieves the variant at the given path of a composite variant.

   The type of the given variant is a composite type and some of the parts are
   composite variants as well, this function makes it easy to retrieve a variant
   deep down in the composite variant structure.
   The path consists of a sequence of keys and/or indexes seprated by a '.'.
   If the path exists, the pointer to the variant at that position is returned.

   @param var pointer to a variant struct, the variant type should be a composite type
   @param flags bitmap, see AMXC_VAR_FLAG_DEFAULT, AMXC_VAR_FLAG_COPY,
                AMXC_VAR_FLAG_UPDATE
   @param fmt path to a variant in the composite variant structure, a path is
              a sequence of indexes and/or keys seperated by a '.'. Does support
              printf format.

   @return
   Pointer to the variant at the given path or NULL if the path does not exist.
 */
amxc_var_t *amxc_var_get_pathf(const amxc_var_t * const var,
                               const int flags,
                               const char * const fmt,
                               ...
                               ) __attribute__ ((format(printf, 3, 4)));

/**
   @ingroup amxc_variant
   @brief
   Gets the variant type id of a variant.

   @param var pointer to a variant struct

   @return
   The variant type id, or -1 if the type does not exists or if it is
   an invalid variant.
 */
uint32_t amxc_var_type_of(const amxc_var_t * const var);

/**
   @ingroup amxc_variant
   @brief
   Gets the variant type name of a variant.

   @param var pointer to a variant struct

   @return
   A constant string containing the name of the variant type.
   Or a null pointer when the type does not exists or it is an invalid variant.
 */
const char *amxc_var_type_name_of(const amxc_var_t * const var);

/**
   @ingroup amxc_variant
   @brief
   Checks if the given variant is of the "null" type.

   @param var pointer to a variant struct

   @return
   true when variant pointer is NULL or the variant is of the "null" type.
 */
AMXC_INLINE
bool amxc_var_is_null(const amxc_var_t * const var) {
    return var == NULL ? true : (var->type_id == AMXC_VAR_ID_NULL);
}

void variant_list_it_free(amxc_llist_it_t *it);
void variant_htable_it_free(const char *key, amxc_htable_it_t *it);
int amxc_var_dump(const amxc_var_t * const var, int fd);

/*
   Set of wrapper functions to easier access variants
 */

/*
   Use amxc_var_set(<type>, <var>, <value>) to call one of the following functions
 */
int amxc_var_set_bool(amxc_var_t * const var, bool boolean);
int amxc_var_set_int32_t(amxc_var_t * const var, int32_t val);
int amxc_var_set_uint32_t(amxc_var_t * const var, uint32_t val);
int amxc_var_set_int64_t(amxc_var_t * const var, int64_t val);
int amxc_var_set_uint64_t(amxc_var_t * const var, uint64_t val);
int amxc_var_set_cstring_t(amxc_var_t * const var, const char * const val);
int amxc_var_set_csv_string_t(amxc_var_t * const var, const char * const val);
int amxc_var_set_ssv_string_t(amxc_var_t * const var, const char * const val);
int amxc_var_set_double(amxc_var_t *var, double val);
int amxc_var_set_fd_t(amxc_var_t *var, int val);
int amxc_var_set_amxc_ts_t(amxc_var_t *var, amxc_ts_t *val);

/*
   Use amxc_var_dyncast(<type>, <var>) to call one of the following functions.
   These function will apply conversion if needed and provde a copy of the
   data. If a pointer is returned, call the correct function to free the
   allocated memory if no longer needed.
 */
bool amxc_var_get_bool(const amxc_var_t * const var);
int32_t amxc_var_get_int32_t(const amxc_var_t * const var);
uint32_t amxc_var_get_uint32_t(const amxc_var_t *var);
int64_t amxc_var_get_int64_t(const amxc_var_t * const var);
uint64_t amxc_var_get_uint64_t(const amxc_var_t * const var);
cstring_t amxc_var_get_cstring_t(const amxc_var_t * const var);
amxc_htable_t *amxc_var_get_amxc_htable_t(const amxc_var_t * const var);
amxc_llist_t *amxc_var_get_amxc_llist_t(const amxc_var_t * const var);
double amxc_var_get_double(const amxc_var_t *var);
fd_t amxc_var_get_fd_t(const amxc_var_t *var);
amxc_ts_t *amxc_var_get_amxc_ts_t(const amxc_var_t *var);

AMXC_INLINE
cstring_t amxc_var_get_csv_string_t(const amxc_var_t * const var) {
    return amxc_var_get_cstring_t(var);
}

AMXC_INLINE
cstring_t amxc_var_get_ssv_string_t(const amxc_var_t * const var) {
    return amxc_var_get_cstring_t(var);
}

/*
   Use amxc_var_constcast(<type>, <var>) to call one of the following functions
 */
const char *amxc_var_get_const_cstring_t(const amxc_var_t * const var);
const amxc_htable_t *amxc_var_get_const_amxc_htable_t(const amxc_var_t * const var);
const amxc_llist_t *amxc_var_get_const_amxc_llist_t(const amxc_var_t * const var);
bool amxc_var_get_const_bool(const amxc_var_t * const var);
int32_t amxc_var_get_const_int32_t(const amxc_var_t * const var);
int64_t amxc_var_get_const_int64_t(const amxc_var_t * const var);
uint32_t amxc_var_get_const_uint32_t(const amxc_var_t * const var);
uint64_t amxc_var_get_const_uint64_t(const amxc_var_t * const var);
double amxc_var_get_const_double(const amxc_var_t * const var);
fd_t amxc_var_get_const_fd_t(const amxc_var_t * const var);
const amxc_ts_t *amxc_var_get_const_amxc_ts_t(const amxc_var_t * const var);

AMXC_INLINE
const char *amxc_var_get_const_csv_string_t(const amxc_var_t * const var) {
    return amxc_var_get_const_cstring_t(var);
}

AMXC_INLINE
const char *amxc_var_get_const_ssv_string_t(const amxc_var_t * const var) {
    return amxc_var_get_const_cstring_t(var);
}

/*

 */
amxc_var_t *amxc_var_add_new_bool(amxc_var_t * const var, bool boolean);
amxc_var_t *amxc_var_add_new_int32_t(amxc_var_t * const var, int32_t val);
amxc_var_t *amxc_var_add_new_uint32_t(amxc_var_t * const var, uint32_t val);
amxc_var_t *amxc_var_add_new_int64_t(amxc_var_t * const var, int64_t val);
amxc_var_t *amxc_var_add_new_uint64_t(amxc_var_t * const var, uint64_t val);
amxc_var_t *amxc_var_add_new_cstring_t(amxc_var_t * const var, const char * const val);
amxc_var_t *amxc_var_add_new_csv_string_t(amxc_var_t * const var, const char * const val);
amxc_var_t *amxc_var_add_new_ssv_string_t(amxc_var_t * const var, const char * const val);
amxc_var_t *amxc_var_add_new_double(amxc_var_t * const var, double val);
amxc_var_t *amxc_var_add_new_fd_t(amxc_var_t * const var, int val);
amxc_var_t *amxc_var_add_new_amxc_ts_t(amxc_var_t * const var, amxc_ts_t *ts);

/* when list is NULL, empty list is added */
amxc_var_t *amxc_var_add_new_amxc_llist_t(amxc_var_t * const var,
                                          const amxc_llist_t *list);

/* when htable is NULL, empty htable is added */
amxc_var_t *amxc_var_add_new_amxc_htable_t(amxc_var_t * const var,
                                           const amxc_htable_t *htable);

amxc_var_t *amxc_var_add_new_key_bool(amxc_var_t * const var, const char *key, bool boolean);
amxc_var_t *amxc_var_add_new_key_int32_t(amxc_var_t * const var, const char *key, int32_t val);
amxc_var_t *amxc_var_add_new_key_uint32_t(amxc_var_t * const var, const char *key, uint32_t val);
amxc_var_t *amxc_var_add_new_key_int64_t(amxc_var_t * const var, const char *key, int64_t val);
amxc_var_t *amxc_var_add_new_key_uint64_t(amxc_var_t * const var, const char *key, uint64_t val);
amxc_var_t *amxc_var_add_new_key_cstring_t(amxc_var_t * const var, const char *key, const char * const val);
amxc_var_t *amxc_var_add_new_key_csv_string_t(amxc_var_t * const var, const char *key, const char * const val);
amxc_var_t *amxc_var_add_new_key_ssv_string_t(amxc_var_t * const var, const char *key, const char * const val);
amxc_var_t *amxc_var_add_new_key_double(amxc_var_t * const var, const char *key, double val);
amxc_var_t *amxc_var_add_new_key_fd_t(amxc_var_t * const var, const char *key, int val);
amxc_var_t *amxc_var_add_new_key_amxc_ts_t(amxc_var_t * const var, const char *key, amxc_ts_t *ts);

/* when list is NULL, empty list is added */
amxc_var_t *amxc_var_add_new_key_amxc_llist_t(amxc_var_t * const var,
                                              const char *key,
                                              const amxc_llist_t *list);

/* when htable is NULL, empty htable is added */
amxc_var_t *amxc_var_add_new_key_amxc_htable_t(amxc_var_t * const var,
                                               const char *key,
                                               const amxc_htable_t *htable);

/*

 */
cstring_t amxc_var_take_cstring_t(amxc_var_t * const var);
amxc_string_t *amxc_var_take_amxc_string_t(amxc_var_t * const var);

AMXC_INLINE
cstring_t amxc_var_take_csv_string_t(amxc_var_t * const var) {
    return amxc_var_take_cstring_t(var);
}

AMXC_INLINE
cstring_t amxc_var_take_ssv_string_t(amxc_var_t * const var) {
    return amxc_var_take_cstring_t(var);
}

/*

 */
int amxc_var_push_cstring_t(amxc_var_t * const var, char *val);
int amxc_var_push_csv_string_t(amxc_var_t * const var, char *val);
int amxc_var_push_ssv_string_t(amxc_var_t * const var, char *val);
int amxc_var_push_amxc_string_t(amxc_var_t * const var, amxc_string_t *val);

#ifdef __cplusplus
}
#endif

#endif // __AMXC_VARIANT_H__
