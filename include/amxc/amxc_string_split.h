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

#if !defined(__AMXC_STRING_SPLIT_H__)
#define __AMXC_STRING_SPLIT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amxc/amxc_variant.h>

#define AMXC_ERROR_STRING_MISSING_DQUOTE            -10
#define AMXC_ERROR_STRING_MISSING_SQUOTE            -11
#define AMXC_ERROR_STRING_MISSING_RBRACKET          -12
#define AMXC_ERROR_STRING_MISSING_SBRACKET          -13
#define AMXC_ERROR_STRING_MISSING_CBRACKET          -14

/**
   @ingroup amxc_string
   @brief
   Helper function to delete an item in a linked list.

   This function can be passed to @ref amxc_llist_delete or amxc_llist_clean:
   --> amxc_llist_delete(ptr_to_llist_ptr, amxc_string_list_it_free);

   Typically a linked list containing amxc_string_t structures is created using
   @ref amxc_string_split_llist

   @note
   Only use this function when clean up a linked list containing only
   amxc_string_t structures.

   @param it a pointer to a linked list iterator
 */
void amxc_string_list_it_free(amxc_llist_it_t *it);

/**
   @ingroup amxc_string
   @brief
   Splits a string into string parts using a separator.

   Splits a string in parts, put each part in a linked list.
   The returned linked list will contain amxc_string_t items, each of these
   items is a part of the original string.

   Empty items (items between 2 seprators) are added to the list.
   If the string ends with a separator, an extra empty item is added to the list.

   Iterating or accessing items can be done using the linked list functions.
   A linked list iterator can be converted back to a amxc_string_t pointer using
   amxc_string_from_llist_it macro.

   @note
   The returned pointer must be freed using @ref amxc_llist_delete.
   Provide the function @ref amxc_string_list_it_free as delete function

   @param string a pointer to the string structure
   @param separator the separator string

   @return
   A pointer to a linked list (amxc_llist_t), when successful.
   NULL when failed to split the string
 */
amxc_llist_t *amxc_string_split_llist(const amxc_string_t * const string,
                                      const char *separator);

/**
   @ingroup amxc_string
   @brief
   Helper function to be used with amxc_string_split_llist.

   It gets the reference to amxc_string_t object from
   an amxc_llist_t, it's not a copy.

   The element is free'd when the list is deleted.

   Taking a copy is the programmer's responsibility!

   @param llist a pointer to the linked list structure
   @param index the position where the item must be inserted

   @return
   A pointer to the string (amxc_string_t) at the index of the llist, when successful.
   NULL when failed to to index the llist
 */
amxc_string_t *amxc_string_get_from_llist(const amxc_llist_t * const llist,
                                          const unsigned int index);

/**
   @ingroup amxc_string
   @brief
   Helper function to be used with amxc_string_split_llist.

   It gets the reference to the amxc_string_t object from
   an amxc_llist_t, it's not a copy.

   The element is free'd when the list is deleted.

   Taking a copy is the programmer's responsibility!

   @param llist a pointer to the linked list structure
   @param index the position where the item must be inserted

   @return
   Pointer to the string buffer at the index of the llist, when successful.
   NULL when failed to point to index the llist.
 */
const char *amxc_string_get_text_from_llist(const amxc_llist_t * const llist,
                                            const unsigned int index);

/**
   @ingroup amxc_string
   @brief
   Splits a string into string parts using a separator.

   Splits a string in parts, put each part in a variant. The variant is added
   to a linked list, which is contained in another variant.

   Empty items (items between 2 seprators) are added to the list.
   If the string ends with a separator, an extra empty item is added to the list.

   The linked list can be retrieved using the @ref amxc_var_constcast.

   @param string a pointer to the string structure
   @param var a variant pointer in which the list is created
   @param separator the separator string

   @return
   0 when success
 */
int amxc_string_split_into_variant(const amxc_string_t * const string,
                                   amxc_var_t *var,
                                   const char *separator);

/**
   @ingroup amxc_string
   @brief
   Splits a string into string parts using a separator.

   Does exactly the same as @ref amxc_string_split_into_var, but allocates
   a new amxc_var_t structure.

   @note
   The returned pointer must be freed using @ref amxc_var_delete.

   @param string a pointer to the string structure
   @param separator the separator string

   @return
   A pointer to a variant (amxc_var_t), when successful. The variant will be
   of the AMXC_VAR_ID_LIST type.
   NULL when failed to split the string.
 */
amxc_var_t *amxc_string_split_variant(const amxc_string_t * const string,
                                      const char *separator);

/**
   @ingroup amxc_string
   @brief
   Splits a string into words.

   Splits a string in words and creates a linked list of variants,
   All variants contained in the list contain a string
   (Type AMXC_VAR_)ID_CSTRING).

   The linked list itself is contained in a variant (Type AMXC_VAR_ID_LIST)

   The linked list can be retrieved using the @ref amxc_var_constcast.

   A word is all text between spaces or punctuation characters or text between
   quotes (single and doube).

   Punctuations and spaces are added to the list as well.
   Subsequent spaces are trimmed to one single space character (isspace).

   For each open quote (single or double) a closing quote must be available.

   The characters [,(,{ must have theire counterparts },),]

   @note
   A pre initialized variant can be passed or a pointer to a pointer containing
   a NULL pointer. In the last case memory will be allocated to store the variant,
   and there for needs to be freed using @ref amxc_var_delete when not needed
   anymore.

   @note
   The reason pointer should not be freed,

   @param string a pointer to the string structure, containing the string that will be splitted
   @param var the resulting variant, allocates one if needed
   @param reason can be NULL, if provided the reason string of the error is stored in this char

   @return
   returns 0 when splitting is ok, or an error code.(TODO define error codes)
 */
int amxc_string_split_word_variant(const amxc_string_t * const string,
                                   amxc_var_t **var,
                                   const char **reason);

#ifdef __cplusplus
}
#endif

#endif // __AMXC_STRING_SPLIT_H__
