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
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include <amxc/amxc_string.h>
#include <amxc/amxc_variant_type.h>
#include <amxc_variant_priv.h>
#include <amxc_assert.h>

int AMXC_PRIVATE amxc_var_default_copy(amxc_var_t * const dest,
                                       const amxc_var_t * const src) {
    dest->data = src->data;
    return 0;
}

int AMXC_PRIVATE amxc_var_default_convert_to_null(amxc_var_t * const dest,
                                                  AMXC_UNUSED const amxc_var_t * const src) {
    dest->data.data = NULL;
    return 0;
}

int AMXC_PRIVATE amxc_var_default_convert_to_list(amxc_var_t * const dest,
                                                  const amxc_var_t * const src) {
    int retval = -1;
    amxc_var_t *var = NULL;

    if(amxc_var_new(&var) != 0) {
        amxc_llist_clean(&dest->data.vl, NULL);
        goto exit;
    }

    amxc_var_copy(var, src);
    amxc_llist_append(&dest->data.vl, &var->lit);

    retval = 0;

exit:
    return retval;
}

int AMXC_PRIVATE amxc_var_default_convert_to_htable(amxc_var_t * const dest,
                                                    const amxc_var_t * const src) {
    int retval = -1;
    amxc_var_t *var = NULL;

    if(amxc_var_new(&var) != 0) {
        amxc_htable_clean(&dest->data.vm, NULL);
        goto exit;
    }

    amxc_var_copy(var, src);
    amxc_htable_insert(&dest->data.vm, "1", &var->hit);

    retval = 0;

exit:
    return retval;
}


int amxc_var_new(amxc_var_t **var) {
    int retval = -1;
    when_null(var, exit);

    *var = calloc(1, sizeof(amxc_var_t));
    when_null(*var, exit);

    retval = amxc_var_init(*var);

exit:
    return retval;
}

void amxc_var_delete(amxc_var_t **var) {
    when_null(var, exit);

    if(*var != NULL) {
        amxc_var_clean(*var);
        amxc_llist_it_clean(&((*var)->lit), NULL);
        amxc_htable_it_clean(&((*var)->hit), NULL);
    }

    free(*var);
    *var = NULL;

exit:
    return;
}

int amxc_var_init(amxc_var_t * const var) {
    int retval = -1;
    when_null(var, exit);

    var->type_id = 0;
    var->data.data = NULL;
    when_failed(amxc_llist_it_init(&var->lit), exit);
    when_failed(amxc_htable_it_init(&var->hit), exit)
    retval = 0;

exit:
    return retval;
}

void amxc_var_clean(amxc_var_t * const var) {
    amxc_var_type_t *var_type = NULL;
    when_null(var, exit);

    var_type = amxc_var_get_type(var->type_id);
    // ISSUE: No type available anymore, probably unregister,
    //        dangling variant !!
    if(var_type == NULL) {
        goto clean;
    }

    if((var_type != NULL) &&
       (var_type->del != NULL)) {
        var_type->del(var);
    }

clean:
    var->type_id = 0;
    var->data.data = NULL;

exit:
    return;
}

int amxc_var_set_type(amxc_var_t * const var, const uint32_t type) {
    int retval = -1;
    amxc_var_type_t *var_type = NULL;
    when_null(var, exit);

    var_type = amxc_var_get_type(type);
    when_null(var_type, exit);

    // first clean the variant
    // this to make sure all allocated memory for the content of the variant
    // is freed
    amxc_var_clean(var);

    var->type_id = type;
    if(var_type->init != NULL) {
        retval = var_type->init(var);
    } else {
        retval = 0;
    }

exit:
    return retval;
}

int amxc_var_copy(amxc_var_t * const dest, const amxc_var_t * const src) {
    int retval = -1;
    amxc_var_type_t *type = NULL;
    when_null(dest, exit);
    when_null(src, exit);

    // get the type function pointers of the src
    type = amxc_var_get_type(src->type_id);
    when_null(type, exit);
    when_null(type->copy, exit);

    when_failed(amxc_var_set_type(dest, src->type_id), exit);
    retval = type->copy(dest, src);
    if(retval != 0) {
        amxc_var_clean(dest);
    }

exit:
    return retval;
}

int amxc_var_convert(amxc_var_t * const dest,
                     const amxc_var_t * const src,
                     int type_id) {
    int retval = -1;
    amxc_var_type_t *src_type = NULL;
    amxc_var_type_t *dst_type = NULL;
    when_null(dest, exit);
    when_null(src, exit);

    src_type = amxc_var_get_type(src->type_id);
    when_null(src_type, exit);

    if(type_id != AMXC_VAR_ID_ANY) {
        dst_type = amxc_var_get_type(type_id);
        when_null(dst_type, exit);
        when_failed(amxc_var_set_type(dest, type_id), exit);
    } else {
        when_failed(amxc_var_set_type(dest, AMXC_VAR_ID_NULL), exit);
        dest->type_id = AMXC_VAR_ID_ANY;
    }

    // try to convert src to dst using covert_to function from src type
    retval = src_type->convert_to != NULL ?
        src_type->convert_to(dest, src) : -1;
    when_true(retval == 0, exit);

    if(type_id != AMXC_VAR_ID_ANY) {
        // try to convert src to dst using covert_from function from dest type
        retval = dst_type->convert_from != NULL ?
            dst_type->convert_from(dest, src) : -1;
    }

exit:
    if(retval != 0) {
        amxc_var_clean(dest);
    }
    return retval;
}

int amxc_var_compare(const amxc_var_t * const var1,
                     const amxc_var_t * const var2,
                     int *result) {
    int retval = -1;
    amxc_var_type_t *var1_type = NULL;
    amxc_var_type_t *var2_type = NULL;
    amxc_var_t converted_var;
    amxc_var_init(&converted_var);
    when_null(result, exit);

    *result = 0;

    if((var1 == NULL) && (var2 == NULL)) {
        retval = 0;
        goto exit;
    }

    if((var1 != NULL) && (var2 == NULL)) {
        retval = 0;
        *result = 1;
        goto exit;
    }

    if(var1 == NULL) {
        retval = 0;
        *result = -1;
        goto exit;
    }

    var1_type = amxc_var_get_type(var1->type_id);
    var2_type = amxc_var_get_type(var2->type_id);

    when_null(var1_type, exit);
    when_null(var2_type, exit);

    if((var1_type->compare != NULL) &&
       ( amxc_var_convert(&converted_var,
                          var2,
                          amxc_var_type_of(var1)) == 0)) {
        retval = var1_type->compare(var1, &converted_var, result);
    } else if((var2_type->compare != NULL) &&
              ( amxc_var_convert(&converted_var,
                                 var1,
                                 amxc_var_type_of(var2)) == 0)) {
        retval = var2_type->compare(&converted_var, var2, result);
    }

exit:
    amxc_var_clean(&converted_var);
    return retval;
}

amxc_var_t *amxc_var_get_key(const amxc_var_t * const var,
                             const char * const key,
                             const int flags) {
    amxc_var_t *retval = NULL;
    amxc_var_type_t *var_type = NULL;
    when_null(var, exit);
    when_null(key, exit);
    when_true(*key == 0, exit);

    var_type = amxc_var_get_type(var->type_id);
    when_null(var_type, exit);

    retval = var_type->get_key != NULL ?
        var_type->get_key(var, key, flags) : NULL;

exit:
    return retval;
}

int amxc_var_set_key(amxc_var_t * const var,
                     const char * const key,
                     amxc_var_t *data,
                     const int flags) {
    int retval = -1;
    amxc_var_type_t *var_type = NULL;
    when_null(var, exit);
    when_null(key, exit);
    when_true(*key == 0, exit);
    when_null(data, exit);

    var_type = amxc_var_get_type(var->type_id);
    when_null(var_type, exit);

    retval = var_type->set_key != NULL ?
        var_type->set_key(var, data, key, flags) : -1;

exit:
    return retval;
}

amxc_var_t *amxc_var_get_index(const amxc_var_t * const var,
                               const int64_t index,
                               const int flags) {

    amxc_var_t *retval = NULL;
    amxc_var_type_t *var_type = NULL;
    when_null(var, exit);

    var_type = amxc_var_get_type(var->type_id);
    when_null(var_type, exit);

    retval = var_type->get_index != NULL ?
        var_type->get_index(var, index, flags) : NULL;

exit:
    return retval;
}

int amxc_var_set_index(amxc_var_t * const var,
                       const int64_t index,
                       amxc_var_t *data,
                       const int flags) {
    int retval = -1;
    amxc_var_type_t *var_type = NULL;
    when_null(var, exit);
    when_null(data, exit);

    var_type = amxc_var_get_type(var->type_id);
    when_null(var_type, exit);

    retval = var_type->set_index != NULL ?
        var_type->set_index(var, data, index, flags) : -1;

exit:
    return retval;
}

amxc_var_t *amxc_var_add_new_key(amxc_var_t * const var,
                                 const char *key) {
    amxc_var_t *data = NULL;
    amxc_var_type_t *var_type = NULL;
    int retval = -1;
    when_null(var, exit);
    when_null(key, exit);
    when_true(*key == 0, exit);

    var_type = amxc_var_get_type(var->type_id);
    when_null(var_type, exit);

    when_failed(amxc_var_new(&data), exit);
    retval = var_type->set_key != NULL ?
        var_type->set_key(var, data, key, AMXC_VAR_FLAG_DEFAULT) :
        -1;

    if(retval != 0) {
        amxc_var_delete(&data);
    }

exit:
    return data;
}

amxc_var_t *amxc_var_add_new(amxc_var_t * const var) {
    amxc_var_t *data = NULL;
    amxc_var_type_t *var_type = NULL;
    int retval = -1;
    when_null(var, exit);

    var_type = amxc_var_get_type(var->type_id);
    when_null(var_type, exit);

    when_failed(amxc_var_new(&data), exit);
    retval = var_type->set_index != NULL ?
        var_type->set_index(var, data, -1, AMXC_VAR_FLAG_DEFAULT) :
        -1;

    if(retval != 0) {
        amxc_var_delete(&data);
    }

exit:
    return data;
}

amxc_var_t *amxc_var_get_path(const amxc_var_t * const var,
                              const char * const path,
                              const int flags) {
    const amxc_var_t *retval = NULL;
    char *dup_path = NULL;
    char *token = NULL;
    int length = 0;
    when_null(var, exit);
    when_null(path, exit);

    retval = var;
    length = strlen(path);
    dup_path = calloc(1, length + 1);
    memcpy(dup_path, path, length);
    token = strtok(dup_path, ".");

    while(token && retval) {
        amxc_var_t *temp = amxc_var_get_key(retval, token, AMXC_VAR_FLAG_DEFAULT);
        if(temp == NULL) {
            char *endptr = NULL;
            int index = strtol(token, &endptr, 0);
            if(*endptr == 0) {
                temp = amxc_var_get_index(retval, index, AMXC_VAR_FLAG_DEFAULT);
            }
        }
        retval = temp;
        token = strtok(NULL, ".");
    }

    free(dup_path);

    when_null(retval, exit);

    if((flags & AMXC_VAR_FLAG_COPY) == AMXC_VAR_FLAG_COPY) {
        amxc_var_t *copy = NULL;
        if(amxc_var_new(&copy) != 0) {
            retval = NULL;
            goto exit;
        }
        if(amxc_var_copy(copy, retval) != 0) {
            retval = NULL;
            free(copy);
            goto exit;
        }
        retval = copy;
    }

exit:
    return (amxc_var_t *) retval;
}

amxc_var_t *amxc_var_get_pathf(const amxc_var_t * const var,
                               const int flags,
                               const char * const fmt,
                               ...
                               ) {
    amxc_var_t *retval = NULL;
    amxc_string_t path;
    va_list args;

    amxc_string_init(&path, 0);
    when_null(var, exit);
    when_null(fmt, exit);

    va_start(args, fmt);
    amxc_string_vappendf(&path, fmt, args);
    va_end(args);

    retval = amxc_var_get_path(var, amxc_string_get(&path, 0), flags);

exit:
    amxc_string_clean(&path);
    return retval;
}

uint32_t amxc_var_type_of(const amxc_var_t * const var) {
    return var != NULL ? var->type_id : AMXC_VAR_ID_INVALID;
}

const char *amxc_var_type_name_of(const amxc_var_t * const var) {
    return var != NULL ? amxc_var_get_type_name_from_id(var->type_id) : NULL;
}

amxc_string_t *amxc_var_take_amxc_string_t(amxc_var_t * const var) {
    amxc_string_t *retval = NULL;

    when_null(var, exit);
    when_true(amxc_var_type_of(var) != AMXC_VAR_ID_CSTRING, exit);
    when_null(var->data.s, exit);

    when_failed(amxc_string_new(&retval, 0), exit);
    when_null(retval, exit);

    amxc_string_push_buffer(retval, var->data.s, strlen(var->data.s) + 1);
    var->data.s = NULL;
    var->type_id = AMXC_VAR_ID_NULL;

exit:
    return retval;
}

int amxc_var_push_amxc_string_t(amxc_var_t * const var, amxc_string_t *val) {
    int retval = -1;
    char *buffer = NULL;

    when_null(var, exit);
    when_null(val, exit);

    amxc_var_clean(var);

    buffer = amxc_string_take_buffer(val);
    retval = amxc_var_push_cstring_t(var, buffer);

exit:
    return retval;
}