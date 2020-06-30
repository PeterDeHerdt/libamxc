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

#if !defined(__AMXC_LLIST_H__)
#define __AMXC_LLIST_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <amxc/amxc_common.h>

#define AMXC_LLIST_RANGE UINT32_MAX

/**
   @file
   @brief
   Ambiorix linked list API header file
 */

/**
   @ingroup amxc_containers
   @defgroup amxc_llist Linked List
 */

/**
   @ingroup amxc_llist
   @brief
   The linked list iterator structure.
 */
typedef struct _amxc_llist_it {
    struct _amxc_llist_it *next; /**< Pointer to the next item
                                      in the linked list */
    struct _amxc_llist_it *prev; /**< Pointer to the previous item
                                      in the linked list */
    struct _amxc_llist *llist;   /**< Pointer to the linked list */
} amxc_llist_it_t;

/**
   @ingroup amxc_llist
   @brief
   The linked list structure.
 */
typedef struct _amxc_llist {
    amxc_llist_it_t *head; /**< Pointer to the first item in the linked list */
    amxc_llist_it_t *tail; /**< Pointer to the last item in the linked list */
} amxc_llist_t;

/**
   @ingroup amxc_llist
   @brief
   Gets the data pointer from a linked list iterator.
 */
#define amxc_llist_it_get_data(it, type, member) \
    ((type *) (((char *) it) - offsetof(type, member)))

/**
   @ingroup amxc_llist
   @brief
   Loops over the list from head to tail.

   Iterates over the list and updates the iterator each time.

   @warning
   Do not modify the list itself while using this macro.
   It is allowed to delete or remove the current iterator from the list.
 */
#define amxc_llist_for_each(it, list) \
    for(amxc_llist_it_t *it = list ? list->head : NULL, \
        *_next = amxc_llist_it_get_next(it); \
        it; \
        it = _next, \
        _next = amxc_llist_it_get_next(it))

/**
   @ingroup amxc_llist
   @brief
   Loops over the list from tail to head.

   Iterates over the list and updates the iterator each time.

   @warning
   Do not modify the list itself while using this macro.
   It is allowed to delete or remove the current iterator from the list.
 */
#define amxc_llist_for_each_reverse(it, list) \
    for(amxc_llist_it_t *it = list ? list->tail : NULL, \
        *_prev = amxc_llist_it_get_previous(it); \
        it; \
        it = _prev, \
        _prev = amxc_llist_it_get_previous(it))

/**
   @ingroup amxc_llist
   @brief
   Definition of the linked list item delete function.

   A pointer to a delete function is used in the following functions
   @ref amxc_llist_delete, @ref amxc_llist_clean, @ref amxc_llist_it_clean.
 */
typedef void (*amxc_llist_it_delete_t) (amxc_llist_it_t *it);

typedef int (*amxc_llist_it_cmp_t) (amxc_llist_it_t *it1,
                                    amxc_llist_it_t *it2);

/**
   @ingroup amxc_llist
   @brief
   Allocates a linked list.

   Allocates and initializes memory to store a linked list.
   This function allocates memory from the heap,
   if a linked list is on the stack, it can be initialized using
   function @ref amxc_llist_init

   @note
   The allocated memory must be freed when not used anymore,
   use @ref amxc_llist_delete to free the memory

   @param llist a pointer to the location where the pointer to the new linked
                list can be stored

   @return
   -1 if an error occured. 0 on success
 */
int amxc_llist_new(amxc_llist_t **llist);

/**
   @ingroup amxc_llist
   @brief
   Frees the previously allocated linked list.

   Removes all items from the linked list,
   if a delete function is provided, it is called for each item after it was
   removed from the list.

   Frees the allocated memory and sets the pointer to NULL.

   @note
   Only call this function for linked lists that are allocated on the heap
   using @ref amxc_llist_new

   @param llist a pointer to the location where the pointer to the linked list is stored
   @param func a pointer to a function that is called to free each item in the linked list
 */
void amxc_llist_delete(amxc_llist_t **llist, amxc_llist_it_delete_t func);

/**
   @ingroup amxc_llist
   @brief
   Initializes a linked list.

   Initializes the linked list structure. All pointers are reset to NULL.
   This function is typically called for linked lists that are on the stack.
   Allocating and initializing a linked list on the heap can be done
   using @ref amxc_llist_new

   @note
   When calling this function on an already initialized linked list,
   that contains items, the linked list is reset and all items in the list are
   lost. Use @ref amxc_llist_clean to remove all items from the list.

   @param llist a pointer to the linked list structure.

   @return
   0 on success.
   -1 if a NULL pointer is given.
 */
int amxc_llist_init(amxc_llist_t * const llist);

/**
   @ingroup amxc_llist
   @brief
   Removes all items from the linked list.

   Removes all items from the linked list,
   if a delete function is provided, it is called for each item after it was
   removed from the list.

   @param llist a pointer to the linked list structure
   @param func a pointer to a function that is called to free each item in the
               linked list
 */
void amxc_llist_clean(amxc_llist_t * const llist, amxc_llist_it_delete_t func);

/**
   @ingroup amxc_llist
   @brief
   Checks that the linked list is empty.

   @param llist a pointer to the linked list structure

   @return
   returns true when the linked list contains no items,
   false when there is at least one item in the list.
 */
bool amxc_llist_is_empty(const amxc_llist_t * const llist);

/**
   @ingroup amxc_llist
   @brief
   Calculates the size of the linked list.

   Loops over all items in the linked list and counts them.
   The size of the linked list is expressed in items.

   @note
   Use @ref amxc_llist_is_empty to check if a list is empty,
   do not use this function to check if the list is empty

   @param llist a pointer to the linked list structure

   @return
   returns the number of items in linked list
 */
size_t amxc_llist_size(const amxc_llist_t * const llist);

/**
   @ingroup amxc_llist
   @brief
   Adds an item to the end of the linked list.

   If the item is already in a list, it is removed from that list.

   @note
   Make sure that the iterator of the item is at least initialized when it is
   first used. Initializing an iterator can be done using @ref amxc_llist_it_init.
   An iterator that is already used in a linked list is considered initialized.

   @param llist a pointer to the linked list structure
   @param it a pointer to the linked list item iterator

   @return
   returns 0 when the item is added, -1 when there was an error
 */
int amxc_llist_append(amxc_llist_t * const llist, amxc_llist_it_t * const it);

/**
   @ingroup amxc_llist
   @brief
   Adds an item to the beginning of the linked list.

   If the item is already in a list, it is removed from that list.

   @note
   Make sure that the iterator of the item is at least initialized when it is
   first used. Initializing an iterator can be done useing @ref amxc_llist_it_init.
   An iterator that is already used in a linked list is considered initialized.

   @param llist a pointer to the linked list structure
   @param it a pointer to the linked list item iterator

   @return
   returns 0 when the item is added, -1 when there was an error
 */
int amxc_llist_prepend(amxc_llist_t * const llist,
                       amxc_llist_it_t * const it);

/**
   @ingroup amxc_llist
   @brief
   Gets an item at a certain position of the linked list.

   This function is not removing the item from the linked list.
   To remove the item from the linked list use @ref amxc_llist_take_at.

   @note
   This function loops over the linked list, counting each item until the
   desired index is reached.
   Avoid to use this function for big linked lists, it can have a huge impact on
   performance. Use @ref amxc_llist_get_last to get the last item in the list
   and not this function.

   @param llist a pointer to the linked list structure
   @param index the position of the item to get

   @return
   Returns the iterator of the linked list item at the requested index,
   or NULL when there is no such item.
 */
amxc_llist_it_t *amxc_llist_get_at(const amxc_llist_t * const llist,
                                   const unsigned int index);

/**
   @ingroup amxc_llist
   @brief
   Inserts an item at a certain position.

   If the item is already in a list, it is removed from that list.
   If the index is out of range (higher then number of items in the linked list)
   the item is not added.

   @note
   This function loops over the linked list, counting each item until the
   desired index is reached. The item is inserted before the found item.
   Avoid to use this function for big linked lists, it can have a huge impact on
   performance. Use @ref amxc_llist_append to add an item at the end of the list
   and not this function.

   @param llist a pointer to the linked list structure
   @param index the position where the item must be inserted
   @param it a pointer to the linked list item iterator

   @return
   returns 0 when the item is added, -1 when there was an error
 */
int amxc_llist_set_at(amxc_llist_t *llist,
                      const unsigned int index,
                      amxc_llist_it_t * const it);

/**
   @ingroup amxc_llist
   @brief
   Initializes a linked list iterator.

   Initializes the linked list iterator structure. All pointers are reset to NULL.

   @note
   When calling this function on an already initialized linked list iterator,
   the linked list iterator is reset and the list the iterator was in is
   corrupted. Use @ref amxc_llist_it_take to remove the iterator from the list
   or @ref amxc_llist_it_clean to remove it from the list and clean up the iterator

   @param it a pointer to the linked list iterator structure.

   @return
   0 on success.
   -1 if a NULL pointer is given.
 */
int amxc_llist_it_init(amxc_llist_it_t * const it);

/**
   @ingroup amxc_llist
   @brief
   Removes the iterator from the list and frees allocated memory.

   If the iterator is in a list, it is removed from the list,
   when a delete function is provided, it is called to free up the memory.

   @note
   This function is not freeing the memory taken by the iterator,
   it uses a callback function for this.

   @param it a pointer to the linked list iterator structure
   @param func a pointer to a function that is called to free the linked list item
 */
void amxc_llist_it_clean(amxc_llist_it_t * const it, amxc_llist_it_delete_t func);

/**
   @ingroup amxc_llist
   @brief
   Removes the iterator from the list.

   @param it a pointer to the linked list iterator structure
 */
void amxc_llist_it_take(amxc_llist_it_t * const it);

/**
   @ingroup amxc_llist
   @brief
   Inserts an iterator before a reference interator in the list.

   If the iterator is already in a list, it is first removed from the list.

   @param reference a pointer to the linked list iterator structure that
                    is used as reference
   @param it a pointer to the linked list iterator structure that needs
             to be inserted

   @return
   -1 if the reference iterator is not in a list.
   0 if the iterator is inserted
 */
int amxc_llist_it_insert_before(amxc_llist_it_t * const reference,
                                amxc_llist_it_t * const it);

/**
   @ingroup amxc_llist
   @brief
   Inserts an iterator after another reference interator in the list.

   If the iterator(it) is already in the list, it is first removed from the
   list before it is inserted at the correct position.

   @param reference a pointer to the linked list iterator structure that
                    is used as reference
   @param it a pointer to the linked list iterator structure that needs
             to be inserted

   @return
   -1 if the reference iterator is not in a list.
   0 if the iterator is inserted
 */
int amxc_llist_it_insert_after(amxc_llist_it_t * const reference,
                               amxc_llist_it_t * const it);

/**
   @ingroup amxc_llist
   @brief
   Gets the index of an iterator in the list.

   @param it a pointer to the linked list iterator structure for which the index
             needs to be calculated

   @return
   The index of the iterator or AMXC_LLIST_RANGE if the iterator is not in a list.
 */
unsigned int amxc_llist_it_index_of(const amxc_llist_it_t * const it);

int amxc_llist_it_swap(amxc_llist_it_t *it1,
                       amxc_llist_it_t *it2);

int amxc_llist_sort(amxc_llist_t * const llist, amxc_llist_it_cmp_t cmp);

/**
   @ingroup amxc_llist
   @brief
   Gets the first item of the linked list.

   This function does not remove the item from the linked list.
   To remove the item, use @ref amxc_llist_take_first.

   @param llist a pointer to the linked list structure

   @return
   Returns the first iterator of the linked list,
   or NULL when the linked list is empty.
 */
AMXC_INLINE
amxc_llist_it_t *amxc_llist_get_first(const amxc_llist_t * const llist) {
    return llist != NULL ? llist->head : NULL;
}

/**
   @ingroup amxc_llist
   @brief
   Gets the last item of the linked list.

   This function does not remove the item from the linked list.
   To remove the item, use @ref amxc_llist_take_last.

   @param llist a pointer to the linked list structure

   @return
   Returns the last iterator of the linked list,
   or NULL when the linked list is empty.
 */
AMXC_INLINE
amxc_llist_it_t *amxc_llist_get_last(const amxc_llist_t * const llist) {
    return llist != NULL ? llist->tail : NULL;
}

/**
   @ingroup amxc_llist
   @brief
   Removes the first item from the linked list.

   This function removes the item from the linked list.
   To get the item without removing it from the list
   use @ref amxc_llist_get_first.

   @param llist a pointer to the linked list structure

   @return
   Returns the first iterator of the linked list,
   or NULL when the linked list is empty.
 */
AMXC_INLINE
amxc_llist_it_t *amxc_llist_take_first(amxc_llist_t * const llist) {
    amxc_llist_it_t *it = amxc_llist_get_first(llist);
    amxc_llist_it_take(it);
    return it;
}

/**
   @ingroup amxc_llist
   @brief
   Removes the last item from the linked list.

   This function removes the item from the linked list.
   To get the item without removing it from the list
   use @ref amxc_llist_get_last.

   @param llist a pointer to the linked list structure

   @return
   Returns the last iterator of the linked list,
   or NULL when the linked list is empty.
 */
AMXC_INLINE
amxc_llist_it_t *amxc_llist_take_last(amxc_llist_t * const llist) {
    amxc_llist_it_t *it = amxc_llist_get_last(llist);
    amxc_llist_it_take(it);
    return it;
}

/**
   @ingroup amxc_llist
   @brief
   Removes an item at a certain position of the linked list.

   This function removes the item from the linked list.
   To get a pointer to the item, without removing it, use
   @ref amxc_llist_get_at.

   @note
   This function loops over the linked list, counting each item until the
   desired index is reached. Avoid to use this function for big linked lists,
   it can have a huge impact on performance. Use @ref amxc_llist_take_last
   to remove the last item from the list and not this function.

   @param llist a pointer to the linked list structure
   @param index the position of the item to get

   @return
   Returns the iterator of the linked list item at the requested index,
   or NULL when there is no such item.
 */
AMXC_INLINE
amxc_llist_it_t *amxc_llist_take_at(const amxc_llist_t *llist,
                                    const unsigned int index) {
    amxc_llist_it_t *it = amxc_llist_get_at(llist, index);
    amxc_llist_it_take(it);
    return it;
}

/**
   @ingroup amxc_llist
   @brief
   Gets the next iterator in the list.

   This function does not remove the item from the linked list.

   @param reference a pointer to the linked list structure used as reference

   @return
   Returns the next iterator of the linked list,
   or NULL when there is not more item in the linked list.
 */
AMXC_INLINE
amxc_llist_it_t *amxc_llist_it_get_next(const amxc_llist_it_t * const reference) {
    return reference != NULL ? reference->next : NULL;
}

/**
   @ingroup amxc_llist
   @brief
   Gets the previous iterator in the list.

   This function does not remove the item from the linked list.

   @param reference a pointer to the linked list structure used as reference

   @return
   Returns the previous iterator of the linked list,
   or NULL when there is no more items in the linked list.
 */
AMXC_INLINE
amxc_llist_it_t *amxc_llist_it_get_previous(const amxc_llist_it_t * const reference) {
    return reference != NULL ? reference->prev : NULL;
}

/**
   @ingroup amxc_llist
   @brief
   Checks that an iterator is in a list.

   @param it a pointer to the linked list structure.

   @return
   true when the iterator is in the list, or false if it is not in a list
 */
AMXC_INLINE
bool amxc_llist_it_is_in_list(const amxc_llist_it_t * const it) {
    return (it != NULL && it->llist != NULL) ? true : false;
}

#ifdef __cplusplus
}
#endif

#endif // __AMXC_LLIST_H__
