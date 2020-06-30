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

#if !defined(__AMXC_STRING_JOIN_H__)
#define __AMXC_STRING_JOIN_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <amxc/amxc_variant.h>

/**
   @ingroup amxc_string
   @brief
   Joins a list of words into a string.

   Joins a list of words into a string until a matching character is found
   or end of list is encountered, whatever comes first.

   Only delimiter parts are verified using the provided function. When the
   function returns nonzero, the joining of parts is stopped.

   The function can be NULL, when so, `isspace` is used as delimiter checking function.

   A new @ref amxc_string_t structure is allocated.

   The variant passed must be of type AMXC_VAR_ID_LIST and must contain a list
   of variants all of them containing a string.

   All items used from the list are removed from the list, including the
   delimiter when encountered.

   @note
   The allocated string must be freed using @ref amxc_string_delete.

   @param string a pointer to a pointer to the string structure,
   @param var contains the list of strings
   @param fn a function pointer to check delimiter, when returning nonzero, joining parts stops
   @param delimiter when a delimiter is encountered, joining is stopped

   @return
   returns 0 when successfull (TODO, document error cases)
 */
int amxc_string_join_variant_until(amxc_string_t **string,
                                   amxc_var_t *var,
                                   amxc_string_is_char_fn_t fn,
                                   int8_t *delimiter);

#ifdef __cplusplus
}
#endif

#endif // __AMXC_STRING_JOIN_H__
