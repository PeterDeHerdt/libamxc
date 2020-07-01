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

#include <amxc/amxc_string.h>

#include "test_amxc_string.h"

#define UNUSED __attribute__((unused))

char data[] = "abcdefghij";

void test_amxc_string_new_delete_null(UNUSED void **state) {
    // passing NULL pointers should not lead to segfault
    assert_int_not_equal(amxc_string_new(NULL, 0), 0);
    amxc_string_delete(NULL);
}

void test_amxc_string_init_reset_clean_null(UNUSED void **state) {
    // passing NULL pointers should not lead to segfault
    assert_int_equal(amxc_string_init(NULL, 0), -1);
    amxc_string_clean(NULL);
    amxc_string_reset(NULL);
}

void test_amxc_string_new_delete(UNUSED void **state) {
    amxc_string_t *string = NULL;

    assert_int_equal(amxc_string_new(&string, 0), 0);
    assert_ptr_equal(string->buffer, NULL);
    assert_int_equal(string->length, 0);
    assert_int_equal(string->last_used, 0);
    amxc_string_delete(&string);
    assert_ptr_equal(string, NULL);

    assert_int_equal(amxc_string_new(&string, 10), 0);
    assert_ptr_not_equal(string->buffer, NULL);
    assert_int_equal(string->length, 10);
    assert_int_equal(string->last_used, 0);
    amxc_string_delete(&string);
    assert_ptr_equal(string, NULL);
}

void test_amxc_string_init_reset_clean(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_ptr_equal(string.buffer, NULL);
    assert_int_equal(string.length, 0);
    assert_int_equal(string.last_used, 0);
    amxc_string_clean(&string);
    assert_int_equal(string.length, 0);
    assert_int_equal(string.last_used, 0);

    assert_int_equal(amxc_string_init(&string, 10), 0);
    assert_ptr_not_equal(string.buffer, NULL);
    assert_int_equal(string.length, 10);
    assert_int_equal(string.last_used, 0);
    amxc_string_reset(&string);
    assert_int_equal(string.length, 10);
    assert_int_equal(string.last_used, 0);
    amxc_string_clean(&string);
    assert_ptr_equal(string.buffer, NULL);
    assert_int_equal(string.length, 0);
    assert_int_equal(string.last_used, 0);
}

void test_amxc_string_grow_null(UNUSED void **state) {
    assert_int_equal(amxc_string_grow(NULL, 5), -1);
}

void test_amxc_string_grow(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 5), 0);

    assert_int_equal(amxc_string_grow(&string, 0), 0);
    assert_ptr_not_equal(string.buffer, NULL);
    assert_int_equal(string.length, 5);
    assert_int_equal(string.last_used, 0);

    assert_int_equal(amxc_string_grow(&string, 5), 0);
    assert_ptr_not_equal(string.buffer, NULL);
    assert_int_equal(string.length, 10);
    assert_int_equal(string.last_used, 0);

    assert_int_equal(amxc_string_grow(&string, 7), 0);
    assert_ptr_not_equal(string.buffer, NULL);
    assert_int_equal(string.length, 17);
    assert_int_equal(string.last_used, 0);

    amxc_string_clean(&string);
}

void test_amxc_string_shrink_null(UNUSED void **state) {
    assert_int_equal(amxc_string_shrink(NULL, 5), -1);
}

void test_amxc_string_shrink(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 15), 0);

    assert_int_equal(amxc_string_shrink(&string, 20), -1);

    assert_int_equal(amxc_string_shrink(&string, 0), 0);
    assert_ptr_not_equal(string.buffer, NULL);
    assert_int_equal(string.length, 15);
    assert_int_equal(string.last_used, 0);

    assert_int_equal(amxc_string_shrink(&string, 5), 0);
    assert_ptr_not_equal(string.buffer, NULL);
    assert_int_equal(string.length, 10);
    assert_int_equal(string.last_used, 0);

    assert_int_equal(amxc_string_shrink(&string, 3), 0);
    assert_ptr_not_equal(string.buffer, NULL);
    assert_int_equal(string.length, 7);
    assert_int_equal(string.last_used, 0);

    assert_int_equal(amxc_string_shrink(&string, 7), 0);
    assert_ptr_equal(string.buffer, NULL);
    assert_int_equal(string.length, 0);
    assert_int_equal(string.last_used, 0);

    amxc_string_clean(&string);
}

void test_amxc_string_append(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 10), 0);

    assert_int_not_equal(amxc_string_append(NULL, "abcde", 5), 0);
    assert_int_not_equal(amxc_string_append(&string, NULL, 0), 0);
    assert_int_not_equal(amxc_string_append(&string, "abcde", 0), 0);

    assert_int_equal(amxc_string_append(&string, "abcde", 5), 0);
    assert_int_equal(string.length, 10);
    assert_int_equal(string.last_used, 5);
    assert_string_equal(string.buffer, "abcde");

    assert_int_equal(amxc_string_append(&string, "12345", 5), 0);
    assert_int_equal(string.length, 11);
    assert_int_equal(string.last_used, 10);
    assert_string_equal(string.buffer, "abcde12345");

    assert_int_equal(amxc_string_append(&string, "abcde", 5), 0);
    assert_int_equal(string.length, 16);
    assert_int_equal(string.last_used, 15);
    assert_string_equal(string.buffer, "abcde12345abcde");

    assert_int_equal(amxc_string_shrink(&string, 5), 0);
    assert_int_equal(string.length, 11);
    assert_int_equal(string.last_used, 10);

    amxc_string_clean(&string);
}

void test_amxc_string_prepend(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 10), 0);

    assert_int_not_equal(amxc_string_prepend(NULL, "abcde", 5), 0);
    assert_int_not_equal(amxc_string_prepend(&string, NULL, 0), 0);
    assert_int_not_equal(amxc_string_prepend(&string, "abcde", 0), 0);

    assert_int_equal(amxc_string_prepend(&string, "abcde", 5), 0);
    assert_int_equal(string.length, 10);
    assert_int_equal(string.last_used, 5);
    assert_string_equal(string.buffer, "abcde");

    assert_int_equal(amxc_string_prepend(&string, "12345", 5), 0);
    assert_int_equal(string.length, 11);
    assert_int_equal(string.last_used, 10);
    assert_string_equal(string.buffer, "12345abcde");

    assert_int_equal(amxc_string_prepend(&string, "abcde", 5), 0);
    assert_int_equal(string.length, 16);
    assert_int_equal(string.last_used, 15);
    assert_string_equal(string.buffer, "abcde12345abcde");

    assert_int_equal(amxc_string_shrink(&string, 5), 0);
    assert_int_equal(string.length, 11);
    assert_int_equal(string.last_used, 10);

    amxc_string_clean(&string);
}

void test_amxc_string_buffer_length(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 10), 0);

    assert_int_equal(amxc_string_buffer_length(NULL), 0);
    assert_int_equal(amxc_string_buffer_length(&string), 10);
    assert_int_equal(amxc_string_shrink(&string, 5), 0);
    assert_int_equal(amxc_string_buffer_length(&string), 5);
    assert_int_equal(amxc_string_grow(&string, 15), 0);
    assert_int_equal(amxc_string_buffer_length(&string), 20);

    amxc_string_clean(&string);
}

void test_amxc_string_text_length(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 10), 0);

    assert_int_equal(amxc_string_text_length(NULL), 0);
    assert_int_equal(amxc_string_text_length(&string), 0);
    assert_int_equal(amxc_string_append(&string, "abcde", 5), 0);
    assert_int_equal(amxc_string_text_length(&string), 5);
    assert_int_equal(amxc_string_append(&string, "abcde", 5), 0);
    assert_int_equal(amxc_string_text_length(&string), 10);
    assert_int_equal(amxc_string_append(&string, "abcde", 5), 0);
    assert_int_equal(amxc_string_text_length(&string), 15);
    assert_int_equal(amxc_string_shrink(&string, 5), 0);
    assert_int_equal(amxc_string_text_length(&string), 10);
    assert_int_equal(amxc_string_grow(&string, 15), 0);
    assert_int_equal(amxc_string_text_length(&string), 10);

    amxc_string_clean(&string);
}

void test_amxc_string_set_at(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 10), 0);
    assert_int_not_equal(amxc_string_set_at(NULL,
                                            0,
                                            "hello world",
                                            11,
                                            amxc_string_no_flags), 0);
    assert_int_not_equal(amxc_string_set_at(&string,
                                            5,
                                            "hello world",
                                            11,
                                            amxc_string_no_flags), 0);
    assert_int_not_equal(amxc_string_set_at(&string,
                                            0,
                                            "",
                                            0,
                                            amxc_string_no_flags), 0);
    assert_int_equal(amxc_string_set_at(&string,
                                        0,
                                        "hello world",
                                        11,
                                        amxc_string_no_flags), 0);
    assert_int_equal(string.length, 12);
    assert_int_equal(string.last_used, 11);
    assert_int_equal(amxc_string_set_at(&string,
                                        6,
                                        "Europe",
                                        6,
                                        amxc_string_overwrite), 0);
    assert_int_equal(string.length, 12);
    assert_int_equal(string.last_used, 12);
    assert_string_equal(string.buffer, "hello Europe");

    assert_int_equal(amxc_string_set_at(&string,
                                        6,
                                        "Asia",
                                        4,
                                        amxc_string_overwrite), 0);
    assert_int_equal(string.length, 12);
    assert_int_equal(string.last_used, 12);
    assert_string_equal(string.buffer, "hello Asiape");

    amxc_string_clean(&string);
}

void test_amxc_string_remove_at(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 10), 0);
    assert_int_not_equal(amxc_string_remove_at(NULL,
                                               0,
                                               10), 0);
    assert_int_not_equal(amxc_string_remove_at(&string,
                                               0,
                                               0), 0);
    assert_int_not_equal(amxc_string_remove_at(&string,
                                               5,
                                               10), 0);
    assert_int_equal(amxc_string_set_at(&string,
                                        0,
                                        "hello world",
                                        11,
                                        amxc_string_no_flags), 0);

    assert_int_equal(amxc_string_remove_at(&string,
                                           5,
                                           AMXC_STRING_MAX), 0);
    assert_int_equal(string.length, 12);
    assert_int_equal(string.last_used, 5);
    assert_string_equal(string.buffer, "hello");

    assert_int_equal(amxc_string_remove_at(&string, 2, 2), 0);
    assert_int_equal(string.length, 12);
    assert_int_equal(string.last_used, 3);
    assert_string_equal(string.buffer, "heo");

    amxc_string_reset(&string);
    assert_int_equal(amxc_string_set_at(&string,
                                        0,
                                        "hello world",
                                        11,
                                        amxc_string_no_flags), 0);

    assert_int_equal(amxc_string_remove_at(&string, 5, 35), 0);
    assert_int_equal(string.length, 12);
    assert_int_equal(string.last_used, 5);
    assert_string_equal(string.buffer, "hello");

    amxc_string_clean(&string);
}

void test_amxc_string_get(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_ptr_equal(amxc_string_get(&string, 0), NULL);
    amxc_string_clean(&string);

    assert_int_equal(amxc_string_init(&string, 10), 0);
    assert_ptr_equal(amxc_string_get(NULL, 0), NULL);
    assert_ptr_equal(amxc_string_get(&string, 5), NULL);
    assert_int_equal(amxc_string_set_at(&string,
                                        0,
                                        "hello world",
                                        11,
                                        amxc_string_no_flags), 0);
    assert_ptr_equal(amxc_string_get(&string, 12), NULL);

    assert_string_equal(amxc_string_get(&string, 0), "hello world");
    assert_string_equal(amxc_string_get(&string, 6), "world");

    amxc_string_clean(&string);
}

void test_amxc_string_dup(UNUSED void **state) {
    amxc_string_t string;
    char *dup_buffer = NULL;

    assert_int_equal(amxc_string_init(&string, 10), 0);
    assert_ptr_equal(amxc_string_dup(NULL, 0, 10), NULL);
    assert_ptr_equal(amxc_string_dup(&string, 5, 10), NULL);
    assert_int_equal(amxc_string_set_at(&string,
                                        0,
                                        "hello world",
                                        11,
                                        amxc_string_no_flags), 0);

    assert_ptr_equal(amxc_string_dup(&string, 15, 15), NULL);
    assert_ptr_equal(amxc_string_dup(&string, 15, 0), NULL);
    assert_ptr_equal(amxc_string_dup(&string, 0, 0), NULL);

    dup_buffer = amxc_string_dup(&string, 0, AMXC_STRING_MAX);
    assert_ptr_not_equal(dup_buffer, NULL);
    assert_string_equal(dup_buffer, "hello world");
    free(dup_buffer);

    dup_buffer = amxc_string_dup(&string, 0, 25);
    assert_ptr_not_equal(dup_buffer, NULL);
    assert_string_equal(dup_buffer, "hello world");
    free(dup_buffer);

    dup_buffer = amxc_string_dup(&string, 6, 25);
    assert_ptr_not_equal(dup_buffer, NULL);
    assert_string_equal(dup_buffer, "world");
    free(dup_buffer);

    dup_buffer = amxc_string_dup(&string, 2, 2);
    assert_ptr_not_equal(dup_buffer, NULL);
    assert_string_equal(dup_buffer, "ll");
    free(dup_buffer);

    amxc_string_clean(&string);
}

void test_amxc_string_trim(UNUSED void **state) {
    amxc_string_t string;

    amxc_string_triml(NULL, NULL);
    amxc_string_trimr(NULL, NULL);
    amxc_string_trim(NULL, NULL);

    assert_int_equal(amxc_string_init(&string, 10), 0);

    amxc_string_triml(&string, isdigit);
    amxc_string_trimr(&string, isspace);
    assert_int_equal(string.last_used, 0);

    assert_int_equal(amxc_string_set_at(&string,
                                        0,
                                        "  \t hello world\t  ",
                                        18,
                                        amxc_string_no_flags), 0);

    amxc_string_triml(&string, isdigit);
    assert_int_equal(string.last_used, 18);
    amxc_string_trimr(&string, isdigit);
    assert_int_equal(string.last_used, 18);

    assert_int_equal(string.last_used, 18);
    amxc_string_triml(&string, NULL);
    assert_int_equal(string.last_used, 14);
    assert_string_equal(amxc_string_get(&string, 0), "hello world\t  ");

    amxc_string_trimr(&string, NULL);
    assert_int_equal(string.last_used, 11);
    assert_string_equal(amxc_string_get(&string, 0), "hello world");

    amxc_string_trim(&string, NULL);
    assert_string_equal(amxc_string_get(&string, 0), "hello world");
    amxc_string_reset(&string);

    assert_int_equal(amxc_string_set_at(&string,
                                        0,
                                        "1234567890",
                                        10,
                                        amxc_string_no_flags), 0);
    amxc_string_triml(&string, isdigit);
    assert_int_equal(string.last_used, 0);
    amxc_string_reset(&string);

    assert_int_equal(amxc_string_set_at(&string,
                                        0,
                                        "1234567890",
                                        10,
                                        amxc_string_no_flags), 0);
    amxc_string_trimr(&string, isdigit);
    assert_int_equal(string.last_used, 0);
    amxc_string_reset(&string);

    assert_int_equal(amxc_string_set_at(&string,
                                        0,
                                        "  \t hello world\t  ",
                                        18,
                                        amxc_string_no_flags), 0);
    amxc_string_trim(&string, NULL);
    assert_string_equal(amxc_string_get(&string, 0), "hello world");

    amxc_string_clean(&string);
}

void test_amxc_string_take_push_buffer(UNUSED void **state) {
    amxc_string_t string;
    char *buffer = NULL;

    assert_ptr_equal(amxc_string_take_buffer(NULL), NULL);
    assert_int_not_equal(amxc_string_push_buffer(NULL, NULL, 0), NULL);

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_ptr_equal(amxc_string_take_buffer(&string), NULL);
    assert_int_equal(amxc_string_append(&string, "Hello World", 11), 0);

    buffer = amxc_string_take_buffer(&string);
    assert_ptr_not_equal(buffer, NULL);
    assert_string_equal(buffer, "Hello World");
    assert_int_equal(string.length, 0);
    assert_int_equal(string.last_used, 0);
    assert_ptr_equal(string.buffer, NULL);

    assert_int_equal(amxc_string_push_buffer(&string, NULL, 0), 0);
    assert_int_not_equal(amxc_string_push_buffer(&string, buffer, 0), 0);
    assert_int_not_equal(amxc_string_push_buffer(&string, buffer, 5), 0);
    assert_int_equal(amxc_string_push_buffer(&string, buffer, 12), 0);

    amxc_string_clean(&string);
}

void test_amxc_string_setf(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_not_equal(amxc_string_setf(NULL, NULL), 0);
    assert_int_not_equal(amxc_string_setf(&string, NULL), 0);

    assert_int_equal(amxc_string_setf(&string, "Hello world"), 0);
    assert_int_equal(string.length, 12);
    assert_int_equal(string.last_used, 11);
    assert_string_equal(amxc_string_get(&string, 0), "Hello world");
    assert_int_equal(string.buffer[string.last_used], 0);

    assert_int_equal(amxc_string_setf(&string, "%s-%d", "acracadabra", 5), 0);
    assert_string_equal(amxc_string_get(&string, 0), "acracadabra-5");
    assert_int_equal(string.buffer[string.last_used], 0);


    assert_int_equal(amxc_string_setf(&string, "%d", 1), 0);

    amxc_string_clean(&string);
}

void test_amxc_string_appendf(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_not_equal(amxc_string_appendf(NULL, NULL), 0);
    assert_int_not_equal(amxc_string_appendf(&string, NULL), 0);

    assert_int_equal(amxc_string_appendf(&string, "Hello world"), 0);
    assert_int_equal(string.length, 12);
    assert_int_equal(string.last_used, 11);
    assert_string_equal(amxc_string_get(&string, 0), "Hello world");

    assert_int_equal(amxc_string_appendf(&string, "%s-%d", "acracadabra", 5), 0);
    assert_string_equal(amxc_string_get(&string, 0), "Hello worldacracadabra-5");

    amxc_string_reset(&string);

    assert_int_equal(amxc_string_appendf(&string, "%d", 1), 0);

    amxc_string_clean(&string);
}

void test_amxc_string_prependf(UNUSED void **state) {
    amxc_string_t string;

    assert_int_equal(amxc_string_init(&string, 0), 0);
    assert_int_not_equal(amxc_string_prependf(NULL, NULL), 0);
    assert_int_not_equal(amxc_string_prependf(&string, NULL), 0);

    assert_int_equal(amxc_string_prependf(&string, "Hello world"), 0);
    assert_int_equal(string.length, 12);
    assert_int_equal(string.last_used, 11);
    assert_string_equal(amxc_string_get(&string, 0), "Hello world");

    assert_int_equal(amxc_string_prependf(&string, "%s-%d", "acracadabra", 5), 0);
    assert_string_equal(amxc_string_get(&string, 0), "acracadabra-5Hello world");

    amxc_string_reset(&string);

    assert_int_equal(amxc_string_prependf(&string, "%d", 1), 0);

    amxc_string_clean(&string);
}
