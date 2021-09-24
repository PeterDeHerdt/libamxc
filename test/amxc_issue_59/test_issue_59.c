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
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <inttypes.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <cmocka.h>

#include <amxc/amxc.h>

#include "test_issue_59.h"

#define UNUSED __attribute__((unused))

static amxc_var_t* var = NULL;
static amxc_var_t* device = NULL;
static amxc_var_t* phonebook = NULL;
static amxc_var_t* contact = NULL;
static amxc_var_t* first = NULL;
static amxc_var_t* second = NULL;

int test_var_path_setup(UNUSED void** state) {
    amxc_var_new(&var);
    amxc_var_set_type(var, AMXC_VAR_ID_HTABLE);

    device = amxc_var_add_key(amxc_htable_t, var, "Device", NULL);
    phonebook = amxc_var_add_key(amxc_htable_t, device, "Phonebook", NULL);
    contact = amxc_var_add_key(amxc_htable_t, phonebook, "Contact", NULL);
    first = amxc_var_add_key(cstring_t, contact, "first", "John");
    second = amxc_var_add_key(cstring_t, contact, "second", "Jane");
    return 0;
}

int test_var_path_teardown(UNUSED void** state) {
    amxc_var_delete(&var);
    return 0;
}

void test_get_var_path_returns_null_when_key_not_found(UNUSED void** state) {
    assert_non_null(amxc_var_get_path(var, "Device.Phonebook.Contact.1", AMXC_VAR_FLAG_DEFAULT));
    assert_null(amxc_var_get_path(var, "Device.Phonebook.Contact.1", AMXC_VAR_FLAG_DEFAULT | AMXC_VAR_FLAG_NO_INDEX));
}

