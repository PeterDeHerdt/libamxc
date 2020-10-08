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

#include <stdio.h>

#include <stdlib.h>
#include <setjmp.h>
#include <stdarg.h>
#include <cmocka.h>
#include <amxc/amxc_lstack.h>

#include "test_amxc_lstack.h"

#define UNUSED __attribute__((unused))

void amxc_lstack_new_delete_null_check(UNUSED void** state) {
    // passing NULL pointers should not lead to segfault
    assert_int_equal(amxc_lstack_new(NULL), -1);
    amxc_lstack_delete(NULL, NULL);
}

void amxc_lstack_new_delete_check(UNUSED void** state) {
    amxc_lstack_t* lstack = NULL;
    assert_int_equal(amxc_lstack_new(&lstack), 0);
    amxc_lstack_delete(&lstack, NULL);
    assert_ptr_equal(lstack, NULL);
}

void amxc_lstack_init_clean_null_check(UNUSED void** state) {
    // passing NULL pointers should not lead to segfault
    assert_int_equal(amxc_lstack_init(NULL), -1);
    amxc_lstack_clean(NULL, NULL);
}

void amxc_lstack_init_clean_check(UNUSED void** state) {
    amxc_lstack_t lstack;

    assert_int_equal(amxc_lstack_init(&lstack), 0);
    amxc_lstack_clean(&lstack, NULL);
}

void amxc_lstack_push_check(UNUSED void** state) {
    amxc_lstack_t lstack;
    amxc_lstack_it_t it1;
    amxc_lstack_it_t it2;
    amxc_lstack_it_t it3;

    assert_int_equal(amxc_lstack_init(&lstack), 0);
    assert_int_equal(amxc_lstack_it_init(&it1), 0);
    assert_int_equal(amxc_lstack_it_init(&it2), 0);
    assert_int_equal(amxc_lstack_it_init(&it3), 0);

    assert_int_equal(amxc_lstack_push(&lstack, &it1), 0);
    assert_ptr_equal(it1.next, NULL);
    assert_ptr_equal(it1.prev, NULL);
    assert_int_equal(amxc_lstack_push(&lstack, &it2), 0);
    assert_ptr_equal(it1.next, &it2);
    assert_ptr_equal(it1.prev, NULL);
    assert_ptr_equal(it2.next, NULL);
    assert_ptr_equal(it2.prev, &it1);
    assert_int_equal(amxc_lstack_push(&lstack, &it3), 0);
    assert_ptr_equal(it1.next, &it2);
    assert_ptr_equal(it1.prev, NULL);
    assert_ptr_equal(it2.next, &it3);
    assert_ptr_equal(it2.prev, &it1);
    assert_ptr_equal(it3.next, NULL);
    assert_ptr_equal(it3.prev, &it2);

    amxc_lstack_clean(&lstack, NULL);
    assert_ptr_equal(it1.next, NULL);
    assert_ptr_equal(it1.prev, NULL);
    assert_ptr_equal(it2.next, NULL);
    assert_ptr_equal(it2.prev, NULL);
    assert_ptr_equal(it3.next, NULL);
    assert_ptr_equal(it3.prev, NULL);
}

void amxc_lstack_pop_check(UNUSED void** state) {
    amxc_lstack_t lstack;
    amxc_lstack_it_t it1;
    amxc_lstack_it_t it2;
    amxc_lstack_it_t it3;

    assert_int_equal(amxc_lstack_init(&lstack), 0);
    assert_int_equal(amxc_lstack_it_init(&it1), 0);
    assert_int_equal(amxc_lstack_it_init(&it2), 0);
    assert_int_equal(amxc_lstack_it_init(&it3), 0);

    assert_int_equal(amxc_lstack_push(&lstack, &it1), 0);
    assert_int_equal(amxc_lstack_push(&lstack, &it2), 0);
    assert_int_equal(amxc_lstack_push(&lstack, &it3), 0);

    assert_ptr_equal(amxc_lstack_pop(&lstack), &it3);
    assert_ptr_equal(it3.next, NULL);
    assert_ptr_equal(it3.prev, NULL);
    assert_ptr_equal(amxc_lstack_pop(&lstack), &it2);
    assert_ptr_equal(it2.next, NULL);
    assert_ptr_equal(it2.prev, NULL);
    assert_ptr_equal(amxc_lstack_pop(&lstack), &it1);
    assert_ptr_equal(it1.next, NULL);
    assert_ptr_equal(it1.prev, NULL);
    assert_ptr_equal(amxc_lstack_pop(&lstack), NULL);

    amxc_lstack_clean(&lstack, NULL);
}
