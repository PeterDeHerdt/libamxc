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

#include "test_amxc_string_split.h"

#define UNUSED __attribute__((unused))

void test_amxc_string_split_llist(UNUSED void **state) {
    amxc_string_t string;
    amxc_llist_t *string_list = NULL;
    amxc_llist_it_t *it = NULL;
    amxc_string_t *part = NULL;
    const char *char_part = NULL;

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_equal(amxc_string_append(&string, "part1,part2,part3", 17), 0);
    string_list = amxc_string_split_llist(&string, ",");
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 3);

    it = amxc_llist_get_first(string_list);
    part = amxc_string_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_ptr_not_equal(amxc_string_get(part, 0), NULL);
    assert_string_equal(amxc_string_get(part, 0), "part1");

    it = amxc_llist_it_get_next(it);
    part = amxc_string_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_ptr_not_equal(amxc_string_get(part, 0), NULL);
    assert_string_equal(amxc_string_get(part, 0), "part2");

    it = amxc_llist_it_get_next(it);
    part = amxc_string_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_ptr_not_equal(amxc_string_get(part, 0), NULL);
    assert_string_equal(amxc_string_get(part, 0), "part3");

    part = amxc_string_get_from_llist(string_list, 0);
    assert_ptr_not_equal(part, NULL);
    assert_ptr_not_equal(amxc_string_get(part, 0), NULL);
    assert_string_equal(amxc_string_get(part, 0), "part1");

    part = amxc_string_get_from_llist(string_list, 1);
    assert_ptr_not_equal(part, NULL);
    assert_ptr_not_equal(amxc_string_get(part, 0), NULL);
    assert_string_equal(amxc_string_get(part, 0), "part2");

    part = amxc_string_get_from_llist(string_list, 2);
    assert_ptr_not_equal(part, NULL);
    assert_ptr_not_equal(amxc_string_get(part, 0), NULL);
    assert_string_equal(amxc_string_get(part, 0), "part3");

    part = amxc_string_get_from_llist(string_list, 3);
    assert_ptr_equal(part, NULL);

    char_part = amxc_string_get_text_from_llist(string_list, 0);
    assert_ptr_not_equal(char_part, NULL);
    assert_string_equal(char_part, "part1");

    char_part = amxc_string_get_text_from_llist(string_list, 1);
    assert_ptr_not_equal(char_part, NULL);
    assert_string_equal(char_part, "part2");

    char_part = amxc_string_get_text_from_llist(string_list, 2);
    assert_ptr_not_equal(char_part, NULL);
    assert_string_equal(char_part, "part3");

    char_part = amxc_string_get_text_from_llist(string_list, 3);
    assert_ptr_equal(char_part, NULL);

    amxc_llist_delete(&string_list, amxc_string_list_it_free);

    string_list = amxc_string_split_llist(NULL, ",");
    assert_ptr_equal(string_list, NULL);
    string_list = amxc_string_split_llist(&string, NULL);
    assert_ptr_equal(string_list, NULL);
    string_list = amxc_string_split_llist(&string, "");
    assert_ptr_equal(string_list, NULL);

    assert_int_equal(amxc_string_append(&string, ",a", 2), 0);
    string_list = amxc_string_split_llist(&string, ",");
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 4);
    amxc_llist_delete(&string_list, amxc_string_list_it_free);
    assert_int_equal(amxc_string_append(&string, ",", 1), 0);
    string_list = amxc_string_split_llist(&string, ",");
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 5);
    amxc_llist_delete(&string_list, amxc_string_list_it_free);

    string_list = amxc_string_split_llist(&string, "very large separator, containing to much");
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 1);
    amxc_llist_delete(&string_list, amxc_string_list_it_free);

    string_list = amxc_string_split_llist(&string, "art");
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 4);

    it = amxc_llist_get_first(string_list);
    part = amxc_string_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_ptr_not_equal(amxc_string_get(part, 0), NULL);
    assert_string_equal(amxc_string_get(part, 0), "p");

    it = amxc_llist_it_get_next(it);
    part = amxc_string_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_ptr_not_equal(amxc_string_get(part, 0), NULL);
    assert_string_equal(amxc_string_get(part, 0), "1,p");

    it = amxc_llist_it_get_next(it);
    part = amxc_string_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_ptr_not_equal(amxc_string_get(part, 0), NULL);
    assert_string_equal(amxc_string_get(part, 0), "2,p");

    it = amxc_llist_it_get_next(it);
    part = amxc_string_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_ptr_not_equal(amxc_string_get(part, 0), NULL);
    assert_string_equal(amxc_string_get(part, 0), "3,a,");

    amxc_llist_delete(&string_list, amxc_string_list_it_free);

    amxc_string_reset(&string);

    string_list = amxc_string_split_llist(&string, ",");
    assert_ptr_not_equal(string_list, NULL);
    assert_false(amxc_llist_is_empty(string_list));
    assert_int_equal(amxc_llist_size(string_list), 1);
    amxc_llist_delete(&string_list, amxc_string_list_it_free);

    assert_int_equal(amxc_string_append(&string, ",", 1), 0);
    string_list = amxc_string_split_llist(&string, ",");
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 2);
    amxc_llist_delete(&string_list, amxc_string_list_it_free);

    assert_int_equal(amxc_string_append(&string, ",", 1), 0);
    string_list = amxc_string_split_llist(&string, ",");
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 3);
    amxc_llist_delete(&string_list, amxc_string_list_it_free);

    assert_int_equal(amxc_string_append(&string, ",", 1), 0);
    string_list = amxc_string_split_llist(&string, ",");
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 4);
    amxc_llist_delete(&string_list, amxc_string_list_it_free);

    amxc_string_clean(&string);

    part = amxc_string_get_from_llist(NULL, 0);
    char_part = amxc_string_get_text_from_llist(NULL, 0);
}

void test_amxc_string_split_variant(UNUSED void **state) {
    amxc_string_t string;
    amxc_var_t *variant = NULL;
    const amxc_llist_t *string_list = NULL;
    amxc_llist_it_t *it = NULL;
    amxc_var_t *part = NULL;

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_equal(amxc_string_append(&string, "part1,part2,part3", 17), 0);
    variant = amxc_string_split_variant(&string, ",");
    assert_ptr_not_equal(variant, NULL);
    assert_int_equal(amxc_var_type_of(variant), AMXC_VAR_ID_LIST);
    string_list = amxc_var_constcast(amxc_llist_t, variant);
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 3);

    it = amxc_llist_get_first(string_list);
    part = amxc_var_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_int_equal(amxc_var_type_of(part), AMXC_VAR_ID_CSTRING);
    assert_ptr_not_equal(amxc_var_constcast(cstring_t, part), NULL);
    assert_string_equal(amxc_var_constcast(cstring_t, part), "part1");

    it = amxc_llist_it_get_next(it);
    part = amxc_var_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_int_equal(amxc_var_type_of(part), AMXC_VAR_ID_CSTRING);
    assert_ptr_not_equal(amxc_var_constcast(cstring_t, part), NULL);
    assert_string_equal(amxc_var_constcast(cstring_t, part), "part2");

    it = amxc_llist_it_get_next(it);
    part = amxc_var_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_int_equal(amxc_var_type_of(part), AMXC_VAR_ID_CSTRING);
    assert_ptr_not_equal(amxc_var_constcast(cstring_t, part), NULL);
    assert_string_equal(amxc_var_constcast(cstring_t, part), "part3");

    amxc_var_delete(&variant);

    variant = amxc_string_split_variant(NULL, ",");
    assert_ptr_equal(variant, NULL);
    variant = amxc_string_split_variant(&string, NULL);
    assert_ptr_equal(variant, NULL);
    variant = amxc_string_split_variant(&string, "");
    assert_ptr_equal(variant, NULL);

    amxc_string_clean(&string);
}

void test_amxc_string_split_word_quotes(UNUSED void **state) {
    amxc_string_t string;
    amxc_var_t *variant = NULL;
    const amxc_llist_t *string_list = NULL;
    const char *text = "\"Double quoted string \\\" with double and ' single quote\"";
    const char *reason = "";

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, &reason), 0);
    assert_ptr_not_equal(variant, NULL);
    assert_int_equal(amxc_var_type_of(variant), AMXC_VAR_ID_LIST);
    string_list = amxc_var_constcast(amxc_llist_t, variant);
    amxc_llist_for_each(it, string_list) {
        amxc_var_t *v = amxc_var_from_llist_it(it);
        const char *p = NULL;
        assert_int_equal(amxc_var_type_of(v), AMXC_VAR_ID_CSTRING);
        p = amxc_var_constcast(cstring_t, v);
        assert_ptr_not_equal(p, NULL);

        printf("[%s]\n", p);
    }

    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 3);

    amxc_string_reset(&string);

    text = "\"Missing double quote";
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, &reason), AMXC_ERROR_STRING_MISSING_DQUOTE);
    assert_int_not_equal(strlen(reason), 0);
    printf("%s\n", reason);
    assert_ptr_equal(variant, NULL);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), AMXC_ERROR_STRING_MISSING_DQUOTE);

    amxc_string_reset(&string);

    text = "'Missing single quote";
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, &reason), AMXC_ERROR_STRING_MISSING_SQUOTE);
    assert_int_not_equal(strlen(reason), 0);
    printf("%s\n", reason);
    assert_ptr_equal(variant, NULL);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), AMXC_ERROR_STRING_MISSING_SQUOTE);

    amxc_string_reset(&string);

    text = "'single quoted string with double \" and \\' single quote'";
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), 0);
    assert_ptr_not_equal(variant, NULL);
    assert_int_equal(amxc_var_type_of(variant), AMXC_VAR_ID_LIST);
    string_list = amxc_var_constcast(amxc_llist_t, variant);
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 3);

    amxc_var_delete(&variant);
    amxc_string_clean(&string);
}

void test_amxc_string_split_word_square_brackets(UNUSED void **state) {
    amxc_string_t string;
    amxc_var_t *variant = NULL;
    const amxc_llist_t *string_list = NULL;
    const char *text = "[1, 2, 3]";
    const char *reason = "";

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), 0);
    assert_ptr_not_equal(variant, NULL);
    assert_int_equal(amxc_var_type_of(variant), AMXC_VAR_ID_LIST);
    string_list = amxc_var_constcast(amxc_llist_t, variant);
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 9);

    amxc_string_reset(&string);

    text = "[1, 2, 3";
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, &reason), AMXC_ERROR_STRING_MISSING_SBRACKET);
    assert_int_not_equal(strlen(reason), 0);
    printf("%s\n", reason);
    assert_ptr_equal(variant, NULL);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), AMXC_ERROR_STRING_MISSING_SBRACKET);

    amxc_string_reset(&string);

    text = "1, 2, 3]";
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, &reason), AMXC_ERROR_STRING_MISSING_SBRACKET);
    assert_int_not_equal(strlen(reason), 0);
    printf("%s\n", reason);
    assert_ptr_equal(variant, NULL);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), AMXC_ERROR_STRING_MISSING_SBRACKET);

    amxc_string_clean(&string);
    amxc_var_delete(&variant);
}

void test_amxc_string_split_word_round_brackets(UNUSED void **state) {
    amxc_string_t string;
    amxc_var_t *variant = NULL;
    const amxc_llist_t *string_list = NULL;
    const char *text = "(1, 2, 3)";
    const char *reason = "";

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), 0);
    assert_ptr_not_equal(variant, NULL);
    assert_int_equal(amxc_var_type_of(variant), AMXC_VAR_ID_LIST);
    string_list = amxc_var_constcast(amxc_llist_t, variant);
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 9);

    amxc_string_reset(&string);

    text = "(1, 2, 3";
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, &reason), AMXC_ERROR_STRING_MISSING_RBRACKET);
    assert_int_not_equal(strlen(reason), 0);
    printf("%s\n", reason);
    assert_ptr_equal(variant, NULL);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), AMXC_ERROR_STRING_MISSING_RBRACKET);

    amxc_string_reset(&string);

    text = "1, 2, 3)";
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, &reason), AMXC_ERROR_STRING_MISSING_RBRACKET);
    assert_int_not_equal(strlen(reason), 0);
    printf("%s\n", reason);
    assert_ptr_equal(variant, NULL);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), AMXC_ERROR_STRING_MISSING_RBRACKET);

    amxc_string_clean(&string);
    amxc_var_delete(&variant);
}

void test_amxc_string_split_word_curly_brackets(UNUSED void **state) {
    amxc_string_t string;
    amxc_var_t *variant = NULL;
    const amxc_llist_t *string_list = NULL;
    const char *text = "{1, 2, 3}";
    const char *reason = "";

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), 0);
    assert_ptr_not_equal(variant, NULL);
    assert_int_equal(amxc_var_type_of(variant), AMXC_VAR_ID_LIST);
    string_list = amxc_var_constcast(amxc_llist_t, variant);
    assert_ptr_not_equal(string_list, NULL);
    assert_int_equal(amxc_llist_size(string_list), 9);

    amxc_string_reset(&string);

    text = "{1, 2, 3";
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, &reason), AMXC_ERROR_STRING_MISSING_CBRACKET);
    assert_int_not_equal(strlen(reason), 0);
    printf("%s\n", reason);
    assert_ptr_equal(variant, NULL);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), AMXC_ERROR_STRING_MISSING_CBRACKET);

    amxc_string_reset(&string);

    text = "1, 2, 3}";
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, &reason), AMXC_ERROR_STRING_MISSING_CBRACKET);
    assert_int_not_equal(strlen(reason), 0);
    printf("%s\n", reason);
    assert_ptr_equal(variant, NULL);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), AMXC_ERROR_STRING_MISSING_CBRACKET);

    amxc_string_clean(&string);
    amxc_var_delete(&variant);
}

void test_amxc_string_split_word_variant(UNUSED void **state) {
    amxc_string_t string;
    amxc_var_t *variant = NULL;
    const amxc_llist_t *string_list = NULL;
    amxc_llist_it_t *it = NULL;
    amxc_var_t *part = NULL;

    const char *text = "  This is a text\t\t  with space characters and \"quoted text with \\\" escapes\" and , some (punctuation). A = The end   ";

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), 0);
    assert_ptr_not_equal(variant, NULL);
    assert_int_equal(amxc_var_type_of(variant), AMXC_VAR_ID_LIST);
    string_list = amxc_var_constcast(amxc_llist_t, variant);
    assert_ptr_not_equal(string_list, NULL);

    amxc_llist_for_each(it, string_list) {
        amxc_var_t *v = amxc_var_from_llist_it(it);
        const char *p = NULL;
        assert_int_equal(amxc_var_type_of(v), AMXC_VAR_ID_CSTRING);
        p = amxc_var_constcast(cstring_t, v);
        assert_ptr_not_equal(p, NULL);

        printf("[%s]\n", p);
    }

    assert_int_equal(amxc_llist_size(string_list), 39);

    it = amxc_llist_get_first(string_list);
    part = amxc_var_from_llist_it(it);
    assert_ptr_not_equal(part, NULL);
    assert_int_equal(amxc_var_type_of(part), AMXC_VAR_ID_CSTRING);
    assert_ptr_not_equal(amxc_var_constcast(cstring_t, part), NULL);
    assert_string_equal(amxc_var_constcast(cstring_t, part), "This");

    amxc_var_delete(&variant);
    amxc_string_clean(&string);
}

void test_amxc_string_split_word_quotes2(UNUSED void **state) {
    amxc_string_t string;
    amxc_var_t *variant = NULL;
    const amxc_llist_t *string_list = NULL;
    int counter = 0;

    const char *text = "!History.save \"/tmp/test.txt\"";

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_equal(amxc_string_append(&string, text, strlen(text)), 0);
    assert_int_equal(amxc_string_split_word_variant(&string, &variant, NULL), 0);

    string_list = amxc_var_constcast(amxc_llist_t, variant);
    assert_ptr_not_equal(string_list, NULL);

    amxc_llist_for_each(it, string_list) {
        amxc_var_t *v = amxc_var_from_llist_it(it);
        const char *p = NULL;
        p = amxc_var_constcast(cstring_t, v);
        assert_ptr_not_equal(p, NULL);

        switch(counter) {
        case 0: assert_string_equal(p, "!"); break;
        case 1: assert_string_equal(p, "History"); break;
        case 2: assert_string_equal(p, "."); break;
        case 3: assert_string_equal(p, "save"); break;
        case 4: assert_string_equal(p, " "); break;
        case 5: assert_string_equal(p, "\""); break;
        case 6: assert_string_equal(p, "/tmp/test.txt"); break;
        case 7: assert_string_equal(p, "\""); break;
        }

        printf("[%s]\n", p);
        counter++;
    }

    amxc_var_delete(&variant);
    amxc_string_clean(&string);
}