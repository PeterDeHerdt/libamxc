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
#include <unistd.h>
#include <string.h>

#include <amxc/amxc_string.h>
#include <amxc/amxc_variant_type.h>
#include <amxc_variant_priv.h>
#include <amxc_assert.h>

typedef int (*amxc_var_dump_fn_t) (const amxc_var_t * const var, int indent, int fd);

static int amxc_var_dump_internal(const amxc_var_t * const var,
                                  int indent,
                                  int fd);

static void write_indentation(int indent, int fd) {
    for(int i = 0; i < indent; i++) {
        write(fd, " ", 1);
    }
}

static int variant_dump_type(const amxc_var_t * const var, int indent, int fd) {
    amxc_string_t txt;
    char addr[64] = "";
    const char *type_name = amxc_var_type_name_of(var);

    write_indentation(indent, fd);
    amxc_string_init(&txt, 64);
    amxc_string_append(&txt, "<", 1);
    amxc_string_append(&txt, type_name, strlen(type_name));
    amxc_string_append(&txt, ">:", 2);
    if(amxc_var_type_of(var) > AMXC_VAR_ID_ANY) {
        snprintf(addr, 63, "%p:", var->data.data);
        amxc_string_append(&txt, addr, strlen(addr));
    }
    write(fd, amxc_string_get(&txt, 0), amxc_string_text_length(&txt));
    amxc_string_clean(&txt);

    return 0;
}

static int variant_dump_null(AMXC_UNUSED const amxc_var_t * const var,
                             int indent, int fd) {
    write_indentation(indent, fd);
    write(fd, "<NULL>", 6);
    return 0;
}

static int variant_dump_char(const amxc_var_t * const var, int indent, int fd) {
    const char *txt = amxc_var_constcast(cstring_t, var);
    write_indentation(indent, fd);
    write(fd, "\"", 1);
    if(txt != NULL) {
        write(fd, txt, strlen(txt));
    }
    write(fd, "\"", 1);

    return 0;
}

static int variant_dump_default(const amxc_var_t * const var, int indent, int fd) {
    int retval = -1;
    char *text = amxc_var_dyncast(cstring_t, var);

    write_indentation(indent, fd);

    if(text != NULL) {
        write(fd, text, strlen(text));
        retval = 0;
    } else {
        retval = variant_dump_type(var, indent, fd);
    }

    free(text);
    return retval;
}

static int variant_dump_list(const amxc_var_t * const var,
                             int indent,
                             int fd) {

    const amxc_llist_t *list = amxc_var_constcast(amxc_llist_t, var);

    write(fd, "[\n", 2);
    indent += 4;
    amxc_llist_for_each(it, list) {
        amxc_var_t *lvar = amxc_var_from_llist_it(it);
        write_indentation(indent, fd);
        amxc_var_dump_internal(lvar, indent, fd);
        if(amxc_llist_it_get_next(it) != NULL) {
            write(fd, ",\n", 2);
        } else {
            write(fd, "\n", 1);
        }
    }
    indent -= 4;
    write_indentation(indent, fd);
    write(fd, "]", 1);

    return 0;
}

static int variant_dump_htable(const amxc_var_t * const var,
                               int indent,
                               int fd) {
    const amxc_htable_t *list = amxc_var_constcast(amxc_htable_t, var);

    write(fd, "{\n", 2);
    indent += 4;
    amxc_htable_for_each(it, list) {
        amxc_var_t *hvar = amxc_var_from_htable_it(it);
        const char *key = amxc_htable_it_get_key(it);
        write_indentation(indent, fd);
        write(fd, key, strlen(key));
        write(fd, " = ", 3);
        if((amxc_var_type_of(hvar) == AMXC_VAR_ID_HTABLE) ||
           ( amxc_var_type_of(hvar) == AMXC_VAR_ID_LIST)) {
            amxc_var_dump_internal(hvar, indent, fd);
        } else {
            amxc_var_dump_internal(hvar, 0, fd);
        }
        if(amxc_htable_it_get_next(it) != NULL) {
            write(fd, ",\n", 2);
        } else {
            write(fd, "\n", 1);
        }
    }
    indent -= 4;
    write_indentation(indent, fd);
    write(fd, "}", 1);

    return 0;
}

static int variant_dump_fd(const amxc_var_t * const var,
                           int indent,
                           int fd) {
    variant_dump_type(var, indent, fd);
    return variant_dump_default(var, indent, fd);
}

static int variant_dump_ts(const amxc_var_t * const var,
                           int indent,
                           int fd) {
    variant_dump_type(var, indent, fd);
    return variant_dump_default(var, indent, fd);
}

static int amxc_var_dump_internal(const amxc_var_t * const var,
                                  int indent,
                                  int fd) {
    int retval = -1;
    amxc_var_dump_fn_t dumpfn[AMXC_VAR_ID_CUSTOM_BASE] = {
        variant_dump_null,
        variant_dump_char,
        variant_dump_default,
        variant_dump_default,
        variant_dump_default,
        variant_dump_default,
        variant_dump_default,
        variant_dump_default,
        variant_dump_default,
        variant_dump_default,
        variant_dump_default,
        variant_dump_default,
        variant_dump_default,
        variant_dump_list,
        variant_dump_htable,
        variant_dump_fd,
        variant_dump_ts,
        NULL,
    };

    if(var->type_id >= AMXC_VAR_ID_CUSTOM_BASE) {
        retval = variant_dump_default(var, indent, fd);
    } else {
        if(dumpfn[var->type_id] != NULL) {
            retval = dumpfn[var->type_id](var, indent, fd);
        }
    }

    return retval;
}

int amxc_var_dump(const amxc_var_t * const var,
                  int fd) {
    int retval = amxc_var_dump_internal(var, 0, fd);
    write(fd, "\n", 1);

    return retval;
}