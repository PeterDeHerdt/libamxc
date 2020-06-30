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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <amxc/amxc_variant.h>
#include <amxc/amxc_string_join.h>
#include <amxc_assert.h>

/**
   @file
   @brief
   Ambiorix string API implementation
 */


int amxc_string_join_variant_until(amxc_string_t **string,
                                   amxc_var_t *var,
                                   amxc_string_is_char_fn_t fn,
                                   int8_t *delimiter) {
    int retval = -1;
    const amxc_llist_t *string_list = NULL;
    amxc_llist_it_t *it = NULL;

    when_null(var, exit);
    when_null(string, exit);
    when_true(amxc_var_type_of(var) != AMXC_VAR_ID_LIST, exit);

    if(fn == NULL) {
        fn = isspace;
    }

    if(delimiter != NULL) {
        *delimiter = 0;
    }

    when_failed(amxc_string_new(string, 0), exit);

    string_list = amxc_var_constcast(amxc_llist_t, var);
    it = amxc_llist_get_first(string_list);
    while(it) {
        amxc_var_t *var_part = amxc_var_from_llist_it(it);
        const char *part = amxc_var_constcast(cstring_t, var_part);
        int length = 1;
        length = strlen(part);
        if((length == 1) && (fn(part[0]) != 0)) {
            if(delimiter != NULL) {
                *delimiter = part[0];
            }
            amxc_var_delete(&var_part);
            break;
        }
        amxc_string_append(*string, part, length);
        amxc_var_delete(&var_part);
        it = amxc_llist_get_first(string_list);
    }

    retval = 0;

exit:
    return retval;
}