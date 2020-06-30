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
#include <stdarg.h>
#include <stddef.h>
#include <ctype.h>
#include <string.h>
#include <setjmp.h>
#include <cmocka.h>

#include <amxc/amxc_string_split.h>
#include <amxc/amxc_string_join.h>

#include "test_amxc_string_join.h"

#define UNUSED __attribute__((unused))

static int iscomma(int c) {
    return c == ',' ? 1 : 0;
}

static int is_space_or_comma(int c) {
    if(isspace(c)) {
        return 1;
    }

    return c == ',' ? 1 : 0;
}

void test_amxc_string_join(UNUSED void **state) {
    amxc_string_t string;
    amxc_var_t *variant = NULL;
    const amxc_llist_t *string_list = NULL;
    amxc_string_t *joined_string = NULL;

    const char *text = "'part with spaces' /usr/bin/test,/usr/lib/test";

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);

    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), 0);
    string_list = amxc_var_constcast(amxc_llist_t, variant);
    amxc_llist_for_each(it, string_list) {
        amxc_var_t *v = amxc_var_from_llist_it(it);
        const char *p = NULL;
        assert_int_equal(amxc_var_type_of(v), AMXC_VAR_ID_CSTRING);
        p = amxc_var_constcast(cstring_t, v);
        assert_ptr_not_equal(p, NULL);

        printf("[%s]\n", p);
    }
    assert_int_equal(amxc_llist_size(string_list), 17);

    assert_int_equal(amxc_string_join_variant_until(&joined_string, variant, isspace, NULL), 0);
    text = amxc_string_get(joined_string, 0);
    assert_int_equal(amxc_llist_size(string_list), 13);
    assert_string_equal(text, "'part with spaces'");
    amxc_string_delete(&joined_string);

    assert_int_equal(amxc_string_join_variant_until(&joined_string, variant, iscomma, NULL), 0);
    text = amxc_string_get(joined_string, 0);
    assert_int_equal(amxc_llist_size(string_list), 6);
    assert_string_equal(text, "/usr/bin/test");
    amxc_string_delete(&joined_string);

    assert_int_equal(amxc_string_join_variant_until(&joined_string, variant, iscomma, NULL), 0);
    text = amxc_string_get(joined_string, 0);
    assert_int_equal(amxc_llist_size(string_list), 0);
    assert_string_equal(text, "/usr/lib/test");
    amxc_string_delete(&joined_string);

    assert_int_equal(amxc_string_join_variant_until(&joined_string, variant, iscomma, NULL), 0);
    text = amxc_string_get(joined_string, 0);
    assert_ptr_equal(text, NULL);
    amxc_string_delete(&joined_string);

    amxc_var_delete(&variant);
    amxc_string_clean(&string);
}

void test_amxc_string_join_invalid_args(UNUSED void **state) {
    amxc_string_t *joined_string = NULL;
    amxc_var_t variant;

    amxc_var_init(&variant);
    amxc_var_set_type(&variant, AMXC_VAR_ID_BOOL);

    assert_int_not_equal(amxc_string_join_variant_until(NULL, &variant, NULL, NULL), 0);

    assert_int_not_equal(amxc_string_join_variant_until(&joined_string, NULL, NULL, NULL), 0);
    assert_ptr_equal(joined_string, NULL);

    assert_int_not_equal(amxc_string_join_variant_until(&joined_string, NULL, NULL, NULL), 0);
    assert_ptr_equal(joined_string, NULL);

    assert_int_not_equal(amxc_string_join_variant_until(&joined_string, &variant, NULL, NULL), 0);
    assert_ptr_equal(joined_string, NULL);

    amxc_var_set_type(&variant, AMXC_VAR_ID_LIST);
    assert_int_equal(amxc_string_join_variant_until(&joined_string, &variant, NULL, NULL), 0);
    assert_ptr_not_equal(joined_string, NULL);

    amxc_var_clean(&variant);
    amxc_string_delete(&joined_string);
}

void test_amxc_string_join_fetch_delimiter(UNUSED void **state) {
    amxc_string_t string;
    amxc_var_t *variant = NULL;
    amxc_string_t *joined_string = NULL;
    int8_t delimiter = 0;
    const amxc_llist_t *string_list = NULL;

    const char *text = "acra.2,cadabra.3: sim/aaa,sala:123,bim";

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);

    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), 0);
    string_list = amxc_var_constcast(amxc_llist_t, variant);
    amxc_llist_for_each(it, string_list) {
        amxc_var_t *v = amxc_var_from_llist_it(it);
        const char *p = NULL;
        assert_int_equal(amxc_var_type_of(v), AMXC_VAR_ID_CSTRING);
        p = amxc_var_constcast(cstring_t, v);
        assert_ptr_not_equal(p, NULL);

        printf("[%s]\n", p);
    }

    assert_int_equal(amxc_string_join_variant_until(&joined_string, variant, is_space_or_comma, &delimiter), 0);
    text = amxc_string_get(joined_string, 0);
    assert_string_equal(text, "acra.2");
    assert_int_equal(delimiter, ',');
    amxc_string_delete(&joined_string);

    assert_int_equal(amxc_string_join_variant_until(&joined_string, variant, is_space_or_comma, &delimiter), 0);
    text = amxc_string_get(joined_string, 0);
    assert_string_equal(text, "cadabra.3:");
    assert_int_equal(delimiter, ' ');
    amxc_string_delete(&joined_string);

    assert_int_equal(amxc_string_join_variant_until(&joined_string, variant, is_space_or_comma, &delimiter), 0);
    text = amxc_string_get(joined_string, 0);
    assert_string_equal(text, "sim/aaa");
    assert_int_equal(delimiter, ',');
    amxc_string_delete(&joined_string);

    assert_int_equal(amxc_string_join_variant_until(&joined_string, variant, is_space_or_comma, &delimiter), 0);
    text = amxc_string_get(joined_string, 0);
    assert_string_equal(text, "sala:123");
    assert_int_equal(delimiter, ',');
    amxc_string_delete(&joined_string);

    assert_int_equal(amxc_string_join_variant_until(&joined_string, variant, is_space_or_comma, &delimiter), 0);
    text = amxc_string_get(joined_string, 0);
    assert_string_equal(text, "bim");
    assert_int_equal(delimiter, 0);
    amxc_string_delete(&joined_string);

    amxc_string_clean(&string);
    amxc_var_delete(&variant);
}