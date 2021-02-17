/** @file llist.h
 * 
 * @brief linked-list library support queue/stack handling for void*
 *        Requires pthread library
 */
#ifndef LLIST_H
#define LLIST_H

#include <stdbool.h>

/**
 * @brief Struct that defines pointer to head node, pthread lock,
 *        and del_f that knows how to delete void*
 */
typedef struct llist_t llist_t;

/**
 * @brief User provided function to delete void* without memory leaks
 */
typedef void (*del_f)(void *data);

/**
 * @brief Allocates linked-list
 * 
 * Provides user with the option of a custom delete function pending what
 * datatype is used.
 *  - Built-in datatype : NULL
 *  - "Simple" alloc'd  : free()
 *  - Custom Structure  : User Defined
 * 
 * The purpose of this option, is so this library does not need to know
 * anything about the user defined data structure (keeping it anonymous)
 * 
 * @param del Function to delete void*. Pass NULL if not wanted
 * @return llist_t* On success, NULL on failure 
 */
llist_t *llist_create(del_f del);

/**
 * @brief Function to delete linked list and free memory
 * 
 * @param p_llist Linked-list to delete
 */
void llist_delete(llist_t **p_llist);

/**
 * @brief Adds void* to linked-list as a queue
 * 
 * @param llist Linked-list to enqueue() to
 * @param data Void* for data to be added
 * @return true On success
 * @return false On failure
 */
bool llist_enqueue(llist_t *llist, void *data);

/**
 * @brief Removes void* from linked-list as a queue
 * 
 * @param llist Linked-list to dequeue() from
 * @param data Void** to store data in
 * @return true On success
 * @return false On failure
 */
bool llist_dequeue(llist_t *llist, void **data);

/**
 * @brief Adds void* to linked-list as a stack
 * 
 * @param llist Linked-list to push() to
 * @param data Void* for data to be added
 * @return true On success
 * @return false On failure
 */
bool llist_push(llist_t *llist, void *data);

/**
 * @brief Removes void* from linked-list as a stack
 * 
 * @param llist Linked-list to pop() from
 * @param data Void** to store data in
 * @return true On success
 * @return false On failure
 */
bool llist_pop(llist_t *llist, void **data);

/**
 * @brief Checks if linked-list is empty
 * 
 * @param list Linked-list to check
 * @return true Is empty
 * @return false Not empty
 */
bool llist_is_empty(llist_t *llist);

#endif				/* LLIST_H */

/*** end of file ***/
