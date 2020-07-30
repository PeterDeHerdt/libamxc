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
#include <string.h>
#include <ctype.h>

#include <amxc/amxc_variant.h>
#include <amxc/amxc_string.h>
#include <amxc_assert.h>

/**
   @file
   @brief
   Ambiorix string API implementation
 */

static int amxc_string_realloc(amxc_string_t *string, const size_t length) {
    char *new_buffer = NULL;
    int retval = -1;

    if(length == 0) {
        free(string->buffer);
        string->buffer = NULL;
        string->length = 0;
        retval = 0;
    } else if(string->buffer != NULL) {
        new_buffer = realloc(string->buffer, sizeof(char) * length + 1);
    } else {
        new_buffer = calloc(length + 1, sizeof(char));
    }
    if(new_buffer != NULL) {
        string->buffer = new_buffer;
        string->length = length;
        string->last_used = string->last_used >= length ? length - 1 : string->last_used;
        string->buffer[string->last_used] = 0;
        retval = 0;
    }

    return retval;
}


int amxc_string_new(amxc_string_t **string, const size_t length) {
    int retval = -1;
    when_null(string, exit);

    /* allocate the array structure */
    *string = calloc(1, sizeof(amxc_string_t));
    when_null(*string, exit);

    /* set the number of items in the array */
    (*string)->length = length;
    (*string)->last_used = 0;

    /* if no buffer needs to pre-allocated, leave */
    if(length == 0) {
        retval = 0;
        goto exit;
    }

    /* allocate the buffer */
    amxc_string_realloc(*string, length);
    if((*string)->buffer == NULL) {
        free(*string);
        *string = NULL;
        goto exit;
    }

    retval = 0;

exit:
    return retval;
}

void amxc_string_delete(amxc_string_t **string) {
    when_null(string, exit);
    when_null((*string), exit);

    amxc_llist_it_take(&(*string)->it);
    free((*string)->buffer);
    free(*string);
    *string = NULL;

exit:
    return;
}

int amxc_string_init(amxc_string_t * const string, const size_t length) {
    int retval = -1;
    when_null(string, exit);

    string->buffer = NULL;
    string->length = 0;
    string->last_used = 0;

    amxc_llist_it_init(&string->it);

    // if no items need to be pre-allocated, leave
    if(length == 0) {
        retval = 0;
        goto exit;
    }

    amxc_string_realloc(string, length);
    when_null(string->buffer, exit);
    string->length = length;

    retval = 0;

exit:
    return retval;
}

void amxc_string_clean(amxc_string_t * const string) {
    when_null(string, exit);

    amxc_llist_it_take(&string->it);

    free(string->buffer);
    string->buffer = NULL;
    string->last_used = 0;
    string->length = 0;

exit:
    return;
}

void amxc_string_reset(amxc_string_t * const string) {
    when_null(string, exit);

    if(string->buffer) {
        string->buffer[string->last_used] = 0;
    }
    string->last_used = 0;

exit:
    return;
}

int amxc_string_copy(amxc_string_t * const dest,
                     const amxc_string_t * const src) {
    int retval = -1;
    when_null(dest, exit);
    when_null(src, exit);

    amxc_string_reset(dest);
    amxc_string_realloc(dest, src->length);
    when_null(dest->buffer, exit);
    memcpy(dest->buffer, src->buffer, src->length);

    retval = 0;

exit:
    return retval;
}

int amxc_string_grow(amxc_string_t * const string, const size_t length) {
    int retval = -1;
    size_t old_length = 0;
    when_null(string, exit);

    if(length == 0) {
        retval = 0;
        goto exit;
    }

    old_length = string->length;
    when_failed(amxc_string_realloc(string, old_length + length), exit);
    when_null(string->buffer, exit);
    memset(string->buffer + old_length, 0, length);
    retval = 0;

exit:
    return retval;
}

int amxc_string_shrink(amxc_string_t * const string, const size_t length) {
    int retval = -1;
    when_null(string, exit);
    when_true(length > string->length, exit); // out of range

    if(length == 0) {
        retval = 0;
        goto exit;
    }

    retval = amxc_string_realloc(string, string->length - length);

exit:
    return retval;
}

int amxc_string_set_at(amxc_string_t * const string,
                       const size_t pos,
                       const char * const text,
                       const size_t length,
                       const amxc_string_flags_t flags) {
    int retval = -1;
    when_null(string, exit);
    when_null(text, exit);
    when_true(length == 0, exit);
    when_true(pos > string->last_used, exit);

    if((flags & amxc_string_overwrite) == amxc_string_overwrite) {
        if(pos + length > string->length) {
            when_failed(amxc_string_realloc(string,
                                            pos + length), exit);
            when_null(string->buffer, exit);
        }
        string->last_used = pos + length > string->last_used ? pos + length : string->last_used;
    } else {
        if(length + string->last_used >= string->length) {
            when_failed(amxc_string_realloc(string,
                                            length + string->last_used + 1), exit);
            when_null(string->buffer, exit);
        }
        memmove(string->buffer + pos + length,
                string->buffer + pos,
                string->last_used - pos);
        string->last_used += length;
    }

    memcpy(string->buffer + pos, text, length);
    string->buffer[string->last_used] = 0;
    retval = 0;

exit:
    return retval;
}

int amxc_string_remove_at(amxc_string_t * const string,
                          const size_t pos,
                          size_t length) {
    int retval = -1;
    when_null(string, exit);
    when_true(length == 0, exit);
    when_true(pos > string->last_used, exit);

    if((length == SIZE_MAX) ||
       ( pos + length > string->last_used)) {
        length = string->last_used - pos;
    }

    size_t bytes_to_move = string->last_used - (pos + length);
    memmove(string->buffer + pos, string->buffer + pos + length, bytes_to_move);
    string->last_used -= length;
    string->buffer[string->last_used] = 0;
    retval = 0;

exit:
    return retval;
}

const char *amxc_string_get(const amxc_string_t * const string,
                            const size_t offset) {
    const char *text = NULL;
    when_null(string, exit);
    when_null(string->buffer, exit);
    when_true(offset > string->last_used, exit);

    string->buffer[string->last_used] = 0;
    text = string->buffer + offset;

exit:
    return text;
}

char *amxc_string_take_buffer(amxc_string_t * const string) {
    char *buffer = NULL;
    when_null(string, exit);

    if(string->buffer != NULL) {
        if(string->buffer[string->last_used] != 0) {
            string->buffer[string->last_used] = 0;
        }
    }
    buffer = string->buffer;
    string->buffer = NULL;
    string->last_used = 0;
    string->length = 0;

exit:
    return buffer;
}

int amxc_string_push_buffer(amxc_string_t * const string,
                            char *buffer,
                            size_t length) {
    int retval = -1;
    char *original = NULL;
    when_null(string, exit);

    original = string->buffer;

    if(buffer != NULL) {
        when_true(length < strlen(buffer) + 1, exit);
        string->buffer = buffer;
        string->last_used = strlen(buffer);
        string->length = length;
    } else {
        string->buffer = NULL;
        string->last_used = 0;
        string->length = 0;
    }

    free(original);

    retval = 0;

exit:
    return retval;
}

char *amxc_string_dup(const amxc_string_t * const string,
                      const size_t start,
                      size_t length) {

    char *text = NULL;
    when_null(string, exit);
    when_true(start > string->last_used, exit);
    when_true(length == 0, exit);

    if((length == SIZE_MAX) ||
       ( start + length > string->last_used)) {
        length = string->last_used - start;
    }

    text = calloc(length + 1, sizeof(char));
    memcpy(text, string->buffer + start, length);
    text[length] = 0;

exit:
    return text;
}

void amxc_string_triml(amxc_string_t * const string, amxc_string_is_char_fn_t fn) {
    uint32_t pos = 0;
    when_null(string, exit);
    when_true(string->last_used == 0, exit);

    if(fn == NULL) {
        fn = isspace;
    }

    while(pos <= string->last_used &&
          fn(string->buffer[pos]) != 0) {
        pos++;
    }

    if(pos >= string->last_used) {
        string->last_used = 0;
        goto exit;
    }

    if(pos > 0) {
        memmove(string->buffer, string->buffer + pos, string->last_used - pos);
        string->last_used -= pos;
    }

exit:
    return;
}

void amxc_string_trimr(amxc_string_t * const string, amxc_string_is_char_fn_t fn) {
    when_null(string, exit);
    when_true(string->last_used == 0, exit);

    if(fn == NULL) {
        fn = isspace;
    }

    while(string->last_used > 0 &&
          fn(string->buffer[string->last_used - 1]) != 0) {
        string->last_used--;
    }

exit:
    return;
}

void amxc_string_trim(amxc_string_t * const string, amxc_string_is_char_fn_t fn) {
    amxc_string_trimr(string, fn);
    amxc_string_triml(string, fn);
}

int amxc_string_vsetf(amxc_string_t * const string,
                      const char *fmt,
                      va_list args) {
    int retval = -1;
    size_t size_needed = 0;
    va_list copy;

    when_null(string, exit);
    when_null(fmt, exit);

    va_copy(copy, args);
    size_needed = vsnprintf(NULL, 0, fmt, args);

    if(string->length < size_needed + 1) {
        when_failed(amxc_string_grow(string,
                                     (size_needed - string->length) + 1), exit);
    }

    size_needed = vsnprintf(string->buffer, string->length, fmt, copy);
    string->buffer[string->length - 1] = 0;

    string->last_used = size_needed;

    retval = 0;

exit:
    return retval;
}

int amxc_string_setf(amxc_string_t * const string, const char *fmt, ...) {
    va_list args;
    int retval = -1;
    when_null(string, exit);
    when_null(fmt, exit);

    va_start(args, fmt);
    retval = amxc_string_vsetf(string, fmt, args);
    va_end(args);

exit:
    return retval;
}

int amxc_string_vappendf(amxc_string_t * const string,
                         const char *fmt,
                         va_list args) {

    int retval = -1;
    int size_needed = 0;
    va_list copy;

    when_null(string, exit);
    when_null(fmt, exit);

    va_copy(copy, args);
    size_needed = vsnprintf(NULL, 0, fmt, args) + 1;

    if(string->length < string->last_used + size_needed) {
        size_t grow = (string->last_used + size_needed - string->length);
        when_failed(amxc_string_grow(string, grow), exit);
    }

    size_needed = vsnprintf(string->buffer + string->last_used,
                            size_needed,
                            fmt,
                            copy);
    string->buffer[string->length] = 0;

    string->last_used += size_needed;

    retval = 0;

exit:
    return retval;
}

int amxc_string_appendf(amxc_string_t * const string, const char *fmt, ...) {
    va_list args;
    int retval = -1;
    when_null(string, exit);
    when_null(fmt, exit);

    va_start(args, fmt);
    retval = amxc_string_vappendf(string, fmt, args);
    va_end(args);

exit:
    return retval;
}

int amxc_string_vprependf(amxc_string_t * const string,
                          const char *fmt,
                          va_list args) {

    int retval = -1;
    int size_needed = 0;
    char first_char = 0;
    va_list copy;

    when_null(string, exit);
    when_null(fmt, exit);

    va_copy(copy, args);
    size_needed = vsnprintf(NULL, 0, fmt, args) + 1;

    if(string->length < string->last_used + size_needed) {
        size_t grow = (string->last_used + size_needed - string->length);
        when_failed(amxc_string_grow(string, grow), exit);
    }
    first_char = string->buffer[0];

    if(string->last_used > 0) {
        memmove(string->buffer + size_needed - 1,
                string->buffer,
                string->last_used);
    }

    size_needed = vsnprintf(string->buffer,
                            size_needed,
                            fmt,
                            copy);
    string->buffer[size_needed] = first_char;
    string->last_used += size_needed;

    retval = 0;

exit:
    return retval;
}

int amxc_string_prependf(amxc_string_t * const string, const char *fmt, ...) {
    va_list args;
    int retval = -1;
    when_null(string, exit);
    when_null(fmt, exit);

    va_start(args, fmt);
    retval = amxc_string_vprependf(string, fmt, args);
    va_end(args);

exit:
    return retval;
}
