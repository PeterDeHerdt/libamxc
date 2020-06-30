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

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <amxc/amxc_variant.h>
#include <amxc/amxc_string_split.h>
#include <amxc_assert.h>

/**
   @file
   @brief
   Ambiorix string API implementation
 */

typedef struct _string_word_flags {
    bool start_quote;
    bool between_double_quotes;
    bool between_single_quotes;
    bool escape;
    int round_brackets;
    int curly_brackets;
    int square_brackets;
} amxc_string_word_flags_t;

typedef int (*amxc_string_create_part_t) (const amxc_string_t * const string,
                                          amxc_llist_t * const list,
                                          const size_t start_pos,
                                          const size_t length);

typedef bool (*amxc_string_check_delimiter_t) (amxc_llist_t *list,
                                               const char delimiter);

static int amxc_string_create_part(const amxc_string_t * const string,
                                   amxc_llist_t * const list,
                                   const size_t start_pos,
                                   const size_t length) {
    int retval = -1;
    amxc_string_t *part = NULL;
    char *buffer = NULL;

    if(length == 0) {
        buffer = strdup("");
    } else {
        buffer = amxc_string_dup(string, start_pos, length);
    }

    when_failed(amxc_string_new(&part, 0), exit);
    when_failed(amxc_string_push_buffer(part, buffer, length == 0 ? 1 : length + 1),
                exit);
    amxc_llist_append(list, &part->it);

    retval = 0;

exit:
    if(retval != 0) {
        free(buffer);
        amxc_string_delete(&part);
    }
    return retval;
}

static int amxc_string_create_var_trim_part(const amxc_string_t * const string,
                                            amxc_llist_t * const list,
                                            const size_t start_pos,
                                            const size_t length) {
    int retval = -1;
    amxc_string_t str_part;
    amxc_var_t *part = NULL;
    char *buffer = NULL;

    if(length == 0) {
        buffer = strdup("");
    } else {
        buffer = amxc_string_dup(string, start_pos, length);
    }

    amxc_string_init(&str_part, 0);
    amxc_string_push_buffer(&str_part, buffer, length == 0 ? 1 : length + 1);
    amxc_string_trim(&str_part, NULL);
    buffer = amxc_string_take_buffer(&str_part);
    amxc_string_clean(&str_part);

    when_failed(amxc_var_new(&part), exit);
    when_failed(amxc_var_push(cstring_t, part, buffer), exit);
    amxc_llist_append(list, &part->lit);

    retval = 0;

exit:
    return retval;
}

static int amxc_string_create_var_part(const amxc_string_t * const string,
                                       amxc_llist_t * const list,
                                       const size_t start_pos,
                                       const size_t length) {
    int retval = -1;
    amxc_var_t *part = NULL;
    char *buffer = amxc_string_dup(string, start_pos, length);

    when_failed(amxc_var_new(&part), exit);
    when_failed(amxc_var_push(cstring_t, part, buffer), exit);
    amxc_llist_append(list, &part->lit);

    retval = 0;

exit:
    return retval;
}

static int amxc_string_split_internal(const amxc_string_t * const string,
                                      amxc_llist_t *list,
                                      const char *separator,
                                      amxc_string_create_part_t fn) {
    size_t sep_length = 0;
    size_t start_pos = 0;
    size_t i = 0;
    int retval = -1;

    sep_length = strlen(separator);

    for(i = 0; i + sep_length <= string->last_used; i++) {
        if(memcmp(string->buffer + i, separator, sep_length) == 0) {
            when_failed(fn(string, list, start_pos, i - start_pos), exit);
            start_pos = i + sep_length;
            i += sep_length - 1;
        }
    }

    i = string->last_used;

    when_failed(fn(string, list, start_pos, i - start_pos), exit);

    retval = 0;

exit:
    return retval;
}

static bool amxc_string_split_update_status(const amxc_string_t * const string,
                                            size_t i,
                                            amxc_string_word_flags_t *flags) {
    bool skip = false;
    if(string->buffer[i] == '\\') {
        flags->escape = true;
        skip = true;
    }
    flags->start_quote = false;
    if(!flags->between_single_quotes && (string->buffer[i] == '"')) {
        flags->between_double_quotes = !flags->between_double_quotes;
        flags->start_quote = flags->between_double_quotes;
    }
    if(!flags->between_double_quotes && (string->buffer[i] == '\'')) {
        flags->between_single_quotes = !flags->between_single_quotes;
        flags->start_quote = flags->between_single_quotes;
    }
    if(!(flags->between_double_quotes || flags->between_single_quotes)) {
        switch(string->buffer[i]) {
        case '(':
            flags->round_brackets++;
            break;
        case ')':
            flags->round_brackets--;
            break;
        case '{':
            flags->curly_brackets++;
            break;
        case '}':
            flags->curly_brackets--;
            break;
        case '[':
            flags->square_brackets++;
            break;
        case ']':
            flags->square_brackets--;
            break;
        }
    }

    return skip;
}

static int amxc_string_split_word_is_valid(amxc_string_word_flags_t *flags,
                                           const char **reason) {
    int retval = 0;
    const char *msg = "";

    if(flags->between_double_quotes) {
        retval = AMXC_ERROR_STRING_MISSING_DQUOTE;
        msg = "Missing closing double quote - \"";
        goto exit;
    }
    if(flags->between_single_quotes) {
        retval = AMXC_ERROR_STRING_MISSING_SQUOTE;
        msg = "Missing closing single quote - '";
        goto exit;
    }
    if(flags->round_brackets > 0) {
        retval = AMXC_ERROR_STRING_MISSING_RBRACKET;
        msg = "Missing closing round bracket - )";
        goto exit;
    }
    if(flags->round_brackets < 0) {
        retval = AMXC_ERROR_STRING_MISSING_RBRACKET;
        msg = "Missing open round bracket - (";
        goto exit;
    }
    if(flags->curly_brackets > 0) {
        retval = AMXC_ERROR_STRING_MISSING_CBRACKET;
        msg = "Missing closing curly bracket - }";
        goto exit;
    }
    if(flags->curly_brackets < 0) {
        retval = AMXC_ERROR_STRING_MISSING_CBRACKET;
        msg = "Missing opening curly bracket - {";
        goto exit;
    }
    if(flags->square_brackets > 0) {
        retval = AMXC_ERROR_STRING_MISSING_SBRACKET;
        msg = "Missing closing square bracket - ]";
        goto exit;
    }
    if(flags->square_brackets < 0) {
        retval = AMXC_ERROR_STRING_MISSING_SBRACKET;
        msg = "Missing opening square bracket - [";
        goto exit;
    }

exit:
    if(reason != NULL) {
        *reason = msg;
    }
    return retval;
}

static bool amxc_need_to_add_delimiter_variant(amxc_llist_t *list,
                                               const char delimiter) {
    bool retval = false;
    amxc_llist_it_t *it = amxc_llist_get_last(list);
    amxc_var_t *var_part = amxc_var_from_llist_it(it);
    const char *part = amxc_var_constcast(cstring_t, var_part);

    if(it == NULL) {
        if(!isspace(delimiter)) {
            retval = true;
        }
        goto exit;
    }

    if(isspace(part[0])) {
        when_true(isspace(delimiter), exit);
    }

    retval = true;

exit:
    return retval;
}

// TODO: refactor function too long
static int amxc_string_split_words_internal(const amxc_string_t * const string,
                                            amxc_llist_t *list,
                                            amxc_string_create_part_t create,
                                            amxc_string_check_delimiter_t check,
                                            const char **reason) {
    amxc_string_word_flags_t flags = {
        .start_quote = false,
        .between_double_quotes = false,
        .between_single_quotes = false,
        .escape = false,
        .round_brackets = 0,
        .curly_brackets = 0,
        .square_brackets = 0,
    };
    size_t start_pos = 0;
    size_t i = 0;
    int retval = -1;

    for(i = 0; i < string->last_used; i++) {
        if(flags.escape == false) {
            if(amxc_string_split_update_status(string, i, &flags) == true) {
                continue;
            }
        }

        flags.escape = false;

        if(((flags.between_double_quotes == false) &&
            (flags.between_single_quotes == false)) ||
           ( flags.start_quote == true)) {
            if(isspace(string->buffer[i])) {
                if(i - start_pos > 0) {
                    when_failed(create(string, list, start_pos, i - start_pos), exit);
                }
                if(check(list, string->buffer[i])) {
                    when_failed(create(string, list, i, 1), exit);
                }
                start_pos = i + 1;
                continue;
            }
            if(ispunct(string->buffer[i])) {
                if(i - start_pos > 0) {
                    when_failed(create(string, list, start_pos, i - start_pos), exit);
                }
                if(check(list, string->buffer[i])) {
                    when_failed(create(string, list, i, 1), exit);
                }
                start_pos = i + 1;
                continue;
            }
        }
    }

    if(i - start_pos != 0) {
        when_failed(create(string, list, start_pos, i - start_pos), exit);
    }

    retval = amxc_string_split_word_is_valid(&flags, reason);

exit:
    return retval;
}

void amxc_string_list_it_free(amxc_llist_it_t *it) {
    amxc_string_t *part = amxc_string_from_llist_it(it);
    amxc_string_delete(&part);
}

amxc_llist_t *amxc_string_split_llist(const amxc_string_t * const string,
                                      const char *separator) {
    amxc_llist_t *string_list = NULL;

    when_null(string, exit);
    when_null(separator, exit);
    when_true(separator[0] == 0x00, exit);

    when_failed(amxc_llist_new(&string_list), exit);

    if(amxc_string_split_internal(string,
                                  string_list,
                                  separator,
                                  amxc_string_create_part) != 0) {
        amxc_llist_delete(&string_list, amxc_string_list_it_free);
    }

exit:
    return string_list;
}

int amxc_string_split_into_variant(const amxc_string_t * const string,
                                   amxc_var_t *var,
                                   const char *separator) {
    int retval = -1;
    amxc_llist_t *string_list = NULL;

    when_null(string, exit);
    when_null(var, exit);
    when_null(separator, exit);
    when_true(separator[0] == 0x00, exit);

    amxc_var_set_type(var, AMXC_VAR_ID_LIST);

    string_list = &var->data.vl;

    if(amxc_string_split_internal(string,
                                  string_list,
                                  separator,
                                  amxc_string_create_var_trim_part) != 0) {
        amxc_var_clean(var);
        goto exit;
    }

    retval = 0;

exit:
    return retval;
}

amxc_var_t *amxc_string_split_variant(const amxc_string_t * const string,
                                      const char *separator) {
    amxc_var_t *var = NULL;

    when_failed(amxc_var_new(&var), exit);
    if(amxc_string_split_into_variant(string, var, separator) != 0) {
        amxc_var_delete(&var);
    }

exit:
    return var;
}

int amxc_string_split_word_variant(const amxc_string_t * const string,
                                   amxc_var_t **var,
                                   const char **reason) {
    int retval = -1;
    amxc_llist_t *string_list = NULL;

    when_null(var, exit);
    when_null(string, exit);
    if(*var != NULL) {
        amxc_var_clean(*var);
    } else {
        when_failed(amxc_var_new(var), exit);
    }
    amxc_var_set_type(*var, AMXC_VAR_ID_LIST);

    string_list = &(*var)->data.vl;

    retval = amxc_string_split_words_internal(string,
                                              string_list,
                                              amxc_string_create_var_part,
                                              amxc_need_to_add_delimiter_variant,
                                              reason);
    if(retval != 0) {
        amxc_var_delete(var);
    }

exit:
    return retval;
}

amxc_string_t *amxc_string_get_from_llist(const amxc_llist_t * const llist,
                                          const unsigned int index) {
    amxc_string_t *retval = NULL;
    amxc_llist_it_t *it = NULL;

    when_null(llist, exit);

    it = amxc_llist_get_at(llist, index);
    when_null(it, exit);

    retval = amxc_string_from_llist_it(it);
exit:
    return retval;
}

const char *amxc_string_get_text_from_llist(const amxc_llist_t * const llist,
                                            const unsigned int index) {
    const char *retval = NULL;
    amxc_string_t *var_str = NULL;

    when_null(llist, exit);

    var_str = amxc_string_get_from_llist(llist, index);
    when_null(var_str, exit);

    retval = amxc_string_get(var_str, 0);
exit:
    return retval;
}
