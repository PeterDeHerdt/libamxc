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

#if !defined(__AMXC_UTILS_H__)
#define __AMXC_UTILS_H__

#ifdef __cplusplus
extern "C"
{
#endif

// string utilities
int amxc_string_resolve_env(amxc_string_t * const string);
int amxc_string_resolve_var(amxc_string_t * const string,
                            const amxc_var_t * const data);
int amxc_string_resolve(amxc_string_t * const string,
                        const amxc_var_t * const data);
int amxc_string_set_resolved(amxc_string_t *string,
                             const char *text,
                             const amxc_var_t * const data);
int amxc_string_new_resolved(amxc_string_t **string,
                             const char *text,
                             const amxc_var_t * const data);

// llist/string utilities
amxc_llist_it_t *amxc_llist_add_string(amxc_llist_t * const llist,
                                       const char *text);
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

#ifdef __cplusplus
}
#endif

#endif // __AMXC_UTILS_H__