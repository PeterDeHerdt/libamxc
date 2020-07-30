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

typedef enum _amxc_string_split_status {
    AMXC_STRING_SPLIT_OK,
    AMXC_ERROR_STRING_SPLIT_INVALID_INPUT,
    AMXC_ERROR_STRING_MISSING_DQUOTE,
    AMXC_ERROR_STRING_MISSING_SQUOTE,
    AMXC_ERROR_STRING_MISSING_RBRACKET,
    AMXC_ERROR_STRING_MISSING_SBRACKET,
    AMXC_ERROR_STRING_MISSING_CBRACKET,
} amxc_string_split_status_t;

typedef amxc_string_split_status_t (*amxc_string_split_builder_t) (amxc_llist_t *all,
                                                                   amxc_var_t *csv_list);

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

amxc_string_split_status_t
amxc_string_split_word(const amxc_string_t * const string,
                       amxc_llist_t *list,
                       const char **reason);

amxc_string_split_status_t
amxc_string_split(const amxc_string_t * const string,
                  amxc_var_t *var,
                  amxc_string_split_builder_t fn,
                  const char **reason);

amxc_string_split_status_t
amxc_string_csv_to_var(const amxc_string_t * const string,
                       amxc_var_t *var,
                       const char **reason);

amxc_string_split_status_t
amxc_string_ssv_to_var(const amxc_string_t * const string,
                       amxc_var_t *var,
                       const char **reason);

amxc_string_split_status_t
amxc_string_split_to_llist(const amxc_string_t * const string,
                           amxc_llist_t *list,
                           const char separator);


#ifdef __cplusplus
}
#endif

#endif // __AMXC_STRING_SPLIT_H__
