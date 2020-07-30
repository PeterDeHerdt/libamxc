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

#if !defined(__AMXC_STRING_H__)
#define __AMXC_STRING_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#include <amxc/amxc_common.h>
#include <amxc/amxc_llist.h>

#define AMXC_STRING_MAX SIZE_MAX


/**
   @file
   @brief
   Ambiorix string API header file
 */

/**
   @ingroup amxc_containers
   @defgroup amxc_string String
 */

/**
   @brief
   Get the pointer to a string structure from an amxc linked list iterator.

   The linked list iterator given, must be an linked list iterator of a
   string structure.

   Gives the pointer to the string structure.
 */
#define amxc_string_from_llist_it(ll_it) \
    ((amxc_string_t *) (((char *) ll_it) - offsetof(amxc_string_t, it)))

/**
   @ingroup amxc_string
   @brief
   The string structure.
 */
typedef struct _amxc_string {
    char *buffer;                  /**< Buffer containing the string */
    size_t length;                 /**< Length of the allocated buffer */
    size_t last_used;              /**< End of string, is alway smaller then
                                         the length of the buffer */
    amxc_llist_it_t it;            /**< Linked list iterator, can be used to add
                                        string to linked list */
} amxc_string_t;

typedef enum _amxc_string_flags {
    amxc_string_no_flags  = 0x00,
    amxc_string_insert = 0x00,
    amxc_string_overwrite = 0x01
} amxc_string_flags_t;

typedef int (*amxc_string_is_char_fn_t) (int c);

/**
   @ingroup amxc_string
   @brief
   Allocates a string.

   Allocates and initializes memory to store a string.
   This function allocates memory from the heap, if a string is on the stack,
   it can be initialized using function @ref amxc_string_init

   The size of the string buffer is not fixed and can be changed with the
   functions @ref amxc_string_grow or @ref amxc_string_shrink

   The string buffer will grow automatically when adding text to it.

   The size of the string is expressed in number of bytes that can be stored in
   the buffer.

   @note
   The allocated memory must be freed when not used anymore,
   use @ref amxc_string_delete to free the memory

   @param string a pointer to the location where the pointer
                to the new string can be stored
   @param length the size of the string in number of bytes

   @return
   -1 if an error occured. 0 on success
 */
int amxc_string_new(amxc_string_t **string, const size_t length);

/**
   @ingroup amxc_string
   @brief
   Frees the previously allocated string.

   Frees the allocated memory and sets the pointer to the string to NULL.

   @note
   Only call this function for string that are allocated on the heap using
   @ref amxc_string_new

   @param string a pointer to the location where the pointer to the string is
                 stored
 */
void amxc_string_delete(amxc_string_t **string);

/**
   @ingroup amxc_string
   @brief
   Initializes a string.

   Initializes the string structure.
   Memory is allocated from the heap to be able to store a string.

   This function is typically called for string objects that are on the stack.
   Allocating and initializing a string object on the heap can be done using
   @ref amxc_string_new

   @note
   When calling this function on an already initialized string objects,
   the string is reset and the previous content is gone. The string buffer resizes
   to the size given.

   Use @ref amxc_string_clean to clean the string buffer without resizing.

   @param string a pointer to the string structure.
   @param length the size of the string buffer in number of items

   @return
   0 on success.
   -1 if a NULL pointer is given.
 */
int amxc_string_init(amxc_string_t * const string, const size_t length);

/**
   @ingroup amxc_string
   @brief
   Frees the string buffer and reset length attributes

   @param string a pointer to the string structure
 */
void amxc_string_clean(amxc_string_t * const string);

/**
   @ingroup amxc_string
   @brief
   Resets the buffer, reset the content to all 0.

   @param string a pointer to the string structure
 */
void amxc_string_reset(amxc_string_t * const string);

/**
   @ingroup amxc_string
   @brief
   Copies the content

   @param dest a pointer to the destination string structure
   @param src a pointer to the source string structure

   @return
   0 on success.
   -1 if a NULL pointer is given.
 */
int amxc_string_copy(amxc_string_t * const dest,
                     const amxc_string_t * const src);

/**
   @ingroup amxc_string
   @brief
   Grows the string buffer.

   Grows the string buffer by the given number of bytes.
   Extra memory is allocated to be able to store the number of bytes
   requested.

   @param string a pointer to the string structure
   @param length the number of bytes the string buffer has to grow

   @return
   0 on success.
   -1 if an error has occured.
 */
int amxc_string_grow(amxc_string_t * const string, const size_t length);

/**
   @ingroup amxc_string
   @brief
   Shrinks the string buffer.

   Shrinks the string by the given number of bytes.

   @note
   Shrinking the string buffer can lead to data loss. Data loss will happen
   at the end of the string.

   @param string a pointer to the string structure
   @param length the number of bytes the string buffer has to shrink

   @return
   0 on success.
   -1 if an error has occured
 */
int amxc_string_shrink(amxc_string_t * const string,
                       const size_t length);

/**
   @ingroup amxc_string
   @brief
   Set text in the string buffer at a certain position

   A block of text can be inserted at a certain position, or can overwrite the
   current content of the string buffer at the given position.

   When the size of the current buffer is insufficient, the buffer will grow.
   It is not possible to insert a text block after the current last used byte
   in the buffer.

   @note
   The provided text block must at least contain as much data as specified in the
   length argument. If the provide text block is smaller, the function will go out
   of boundery.

   @param string a pointer to the string structure
   @param pos the position in the string buffer
   @param text the text block that needs to be inserted
   @param length the length of the text block
   @param flags TODO:not documented yet

   @return
   0 on success.
   -1 if an error has occured
 */
int amxc_string_set_at(amxc_string_t * const string,
                       const size_t pos,
                       const char * const text,
                       const size_t length,
                       const amxc_string_flags_t flags);

/**
   @ingroup amxc_string
   @brief
   Removes part of the text in the string buffer.

   This method removes 1 or more characters from the string buffer. The text
   right from the removed blocked is moved to the start of the removed block.

   It is not possible to remove blocks after the last used position.
   If the length provided (counting from the given position), is over the
   last used position, all text from the given position to the end of the used
   buffer is removed.

   @param string a pointer to the string structure
   @param pos the position in the string buffer
   @param length the length of the text block

   @return
   0 on success.
   -1 if an error has occured
 */
int amxc_string_remove_at(amxc_string_t * const string,
                          const size_t pos,
                          size_t length);

/**
   @ingroup amxc_string
   @brief
   Gets the content of the string buffer.

   Get the content from the string buffer. The pointer returned is the actual
   string buffer, or an offset of it.

   The offset can not be higher then the value return by @ref amxc_string_text_length.

   @param string a pointer to the string structure
   @param offset the offset

   @return
   Pointer to the string buffer, or a NULL pointer if no buffer is available
 */
const char *amxc_string_get(const amxc_string_t * const string,
                            const size_t offset);

/**
   @ingroup amxc_string
   @brief
   Takes the string buffer.

   Takes the string buffer (char *) from the string object.
   The string object is reset.

   @note
   When calling this function the ownership of the pointer is moved.
   Calling @ref amxc_string_clean will not free the buffer. You need
   to free the allocated memory (returned pointer) by yourself.
   The returned pointer must be freed. Failing to free the allocated memory,
   will result in a memory leak.

   @param string a pointer to the string structure

   @return
   Pointer to the string buffer, or a NULL pointer if no buffer is available
 */
char *amxc_string_take_buffer(amxc_string_t * const string);

/**
   @ingroup amxc_string
   @brief
   Sets the string buffer.

   Replaces the string buffer with the given one.
   If the string object is containing a buffer, the buffer is freed and the
   given buffer is used.

   @note
   When calling this function the string object takes ownership of the
   given pointer. Do not modify the content of the buffer, unless using
   the amxc_string functions. Do not free the buffer directly, use @ref
   amxc_string_clean or amxc_string_delete

   @param string a pointer to the string structure
   @param buffer a pointer to the buffer
   @param length the length of the allocated memory (not the string length)

   @return
   0 when successful, none 0 otherwise.
 */
int amxc_string_push_buffer(amxc_string_t * const string,
                            char *buffer,
                            size_t length);

/**
   @ingroup amxc_string
   @brief
   Creates a full or partial copy of the text in the string buffer.

   A new memory block is allocated on the heap. The full content of the string
   buffer or partial content of the string buffer is copied in this new
   allocated memory block.

   The start position can not be higher then the last used position.
   If the length (Starting to count for the start position), is going over the
   last used position, the length is re-calculated so it matches the last used
   position.

   @note
   The returned pointer must be freed. Failing to free the allocated memory, will
   result in a memory leak.

   @param string a pointer to the string structure
   @param start the position in the string buffer
   @param length the length of the text block

   @return
   Pointer to the string buffer or NULL when no buffer was available or the
   start position is over the last used position in the buffer.
 */
char *amxc_string_dup(const amxc_string_t * const string,
                      const size_t start,
                      size_t length);

/**
   @ingroup amxc_string
   @brief
   Trim left.

   Removes leading characters from the string. The characters that will be
   removed are defined by the provided character classification function.

   If no character classification function is provided the "isspace" function
   is used by default.

   You can provide any character classification function or create your own.
   Such a function must return a none zero value when the character must be
   removed from the string.

   @param string a pointer to the string structure
   @param fn a pointer to a character classification function or NULL
 */
void amxc_string_triml(amxc_string_t * const string, amxc_string_is_char_fn_t fn);

/**
   @ingroup amxc_string
   @brief
   Trim right

   Removes trailing characters from the string. The characters that will be
   removed are defined by the provided character classification function.

   If no character classifictaion function is provided the "isspace" function
   is used by default.

   You can provide any character classification function or create your own.
   Such a function must return a none zero value when the character must be
   removed from the string.

   @param string a pointer to the string structure
   @param fn a pointer to a character classification function or NULL
 */
void amxc_string_trimr(amxc_string_t * const string, amxc_string_is_char_fn_t fn);

/**
   @ingroup amxc_string
   @brief
   Trim

   Removes trailing and leading characters from the string. The characters
   that will be removed are defined by the provided character
   classification function.

   If no character classification fuinction is provided the "isspace" function
   is used by default.

   You can provide any character classification function or create your own.
   Such a function must return a none zero value when the character must be
   removed from the string.

   @param string a pointer to the string structure
   @param fn a pointer to a character classification function or NULL
 */
void amxc_string_trim(amxc_string_t * const string, amxc_string_is_char_fn_t fn);

int amxc_string_vsetf(amxc_string_t * const string, const char *fmt, va_list ap);

int amxc_string_setf(amxc_string_t * const string, const char *fmt, ...) \
    __attribute__ ((format(printf, 2, 3)));

int amxc_string_vappendf(amxc_string_t * const string,
                         const char *fmt,
                         va_list ap);

int amxc_string_appendf(amxc_string_t * const string, const char *fmt, ...) \
    __attribute__ ((format(printf, 2, 3)));

int amxc_string_vprependf(amxc_string_t * const string,
                          const char *fmt,
                          va_list ap);

int amxc_string_prependf(amxc_string_t * const string, const char *fmt, ...) \
    __attribute__ ((format(printf, 2, 3)));

/**
   @ingroup amxc_string
   @brief
   Appends text to the end of the current content of the string buffer.

   This method uses @ref amxc_string_set_at with the pos argument set to the
   last used position of the string buffer.

   When the size of the current buffer is insufficient, the buffer will grow.

   @note
   The provided text block must at least contain as much data as specified in the
   length argument. If the provide text block is smaller, the function will go out
   of boundery.

   @param string a pointer to the string structure
   @param text the text block that needs to be inserted
   @param length the length of the text block

   @return
   0 on success.
   -1 if an error has occured
 */
AMXC_INLINE
int amxc_string_append(amxc_string_t * const string,
                       const char * const text,
                       const size_t length) {
    return amxc_string_set_at(string,
                              string != NULL ? string->last_used : 0,
                              text,
                              length,
                              amxc_string_no_flags);
}

/**
   @ingroup amxc_string
   @brief
   Inserts text at the beginning of the current content of the string buffer.

   This method uses @ref amxc_string_set_at with the pos argument set to 0.

   When the size of the current buffer is insufficient, the buffer will grow.

   @note
   The provided text block must at least contain as much data as specified in the
   length argument. If the provide text block is smaller, the function will go out
   of boundery.

   @param string a pointer to the string structure
   @param text the text block that needs to be inserted
   @param length the length of the text block

   @return
   0 on success.
   -1 if an error has occured
 */
AMXC_INLINE
int amxc_string_prepend(amxc_string_t * const string,
                        const char * const text,
                        const size_t length) {
    return amxc_string_set_at(string, 0, text, length, amxc_string_no_flags);
}

/**
   @ingroup amxc_string
   @brief
   Gets the current size of the allocate string buffer.

   The allocated string buffer size is not the same as the used buffer size (or
   text length). Use @ref amxc_string_text_length to get the current used
   buffer size. The allocated buffer size and the used buffer size can be the
   same length, but the used buffer size can never be higher then the allocated
   buffer size.

   @param string a pointer to the string structure

   @return
   The size in bytes of the allocated memory for the string buffer.
 */
AMXC_INLINE
size_t amxc_string_buffer_length(const amxc_string_t * const string) {
    return string != NULL ? string->length : 0;
}

/**
   @ingroup amxc_string
   @brief
   Gets the current size of the used string buffer.

   The used string buffer size is not the same as the allocated buffer size.
   Use @ref amxc_string_buffer_length to get the allocated buffer size.
   The allocated buffer size and the used buffer size can be the same length,
   but the used buffer size can never be higher then the allocated
   buffer size.

   @param string a pointer to the string structure

   @return
   The size in bytes of the allocated memory for the string buffer.
 */
AMXC_INLINE
size_t amxc_string_text_length(const amxc_string_t * const string) {
    return string != NULL ? string->last_used : 0;
}

/**
   @ingroup amxc_string
   @brief
   Checks if the string is empty.

   A string is considered empty if the last used bytes is 0. TODO looks wrong!

   @param string a pointer to the string structure

   @return
   true when empty or false otherwise
 */
AMXC_INLINE
bool amxc_string_is_empty(const amxc_string_t * const string) {
    return string != NULL ? (string->last_used == 0) : true;
}

AMXC_INLINE
int amxc_string_insert_at(amxc_string_t * const string,
                          const size_t pos,
                          char *text,
                          size_t length) {
    return amxc_string_set_at(string, pos, text, length, amxc_string_insert);
}

#ifdef __cplusplus
}
#endif

#endif // __AMXC_STRING_H__
