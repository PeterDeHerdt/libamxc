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
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>

#include <amxc/amxc.h>
#include "amxc_variant_priv.h"
#include "test_amxc_variant.h"

#include "test_amxc_variant.h"

int main(void) {
    amxc_array_t* types = amxc_variant_get_types_array();
    for(int i = 0; i < AMXC_VAR_ID_CUSTOM_BASE + 1; i++) {
        amxc_array_it_t* ait = amxc_array_get_at(types, i);
        amxc_array_it_take_data(ait);
    }
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_amxc_var_new_delete),
        cmocka_unit_test(test_amxc_var_init_clean),
        cmocka_unit_test(test_amxc_var_set_type),
        cmocka_unit_test(test_amxc_var_copy),
        cmocka_unit_test(test_amxc_var_convert),
        cmocka_unit_test(test_amxc_var_compare),
        cmocka_unit_test(test_amxc_var_type_of),
        cmocka_unit_test(test_amxc_var_get_set_key),
        cmocka_unit_test(test_amxc_var_add_new_key),
        cmocka_unit_test(test_amxc_var_add_new),
        cmocka_unit_test(test_amxc_var_get_path),
        cmocka_unit_test(test_amxc_var_push_take_amxc_string),
        cmocka_unit_test(test_amxc_var_move),
        cmocka_unit_test(test_amxc_var_take_it),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
