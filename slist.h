#ifndef SLIST_H
#define SLIST_H

#include <stdlib.h>


//////////////////////////////////////
// Function      // Time complexity //
//////////////////////////////////////
// SLIST_LINK    // compile time    //
// SLIST_TYPE    // compile time    //
// SLIST_FIRST   // O(1)            //
// SLIST_NEXT    // O(1)            //
// SLIST_NEW     // O(1)            //
// SLIST_EMPTY   // O(1)            //
// SLIST_LENGTH  // O(n)            //
// SLIST_MAP     // O(n)            //
// SLIST_FILTER  // O(n)            //
// SLIST_REVERSE // O(n)            //
// SLIST_FIND    // O(n)            //
// SLIST_PUSH    // O(1)            //
// SLIST_INSERT  // O(1)            //
// SLIST_POP     // O(1)            //
// SLIST_REMOVE  // O(1)            //
// SLIST_FREE    // O(n)            //
//////////////////////////////////////

/// Definition of a new element link.
///
/// @param[in] tag struct tag name
#define SLIST_LINK(tag)       \
  struct {                    \
    struct tag* __slist_next; \
  }

/// Definition of a new list type.
///
/// @param[in] tag  struct tag name
/// @param[in] type list element type
#define SLIST_TYPE(tag, type) \
  struct tag {                \
    type* __slist_fst;        \
  }

/// Obtain the first element of the list.
/// @return NULL if empty, first element otherwise
///
/// @param[in] list list
#define SLIST_FIRST(list) \
  ((list)->__slist_fst)

/// Obtain the linked element.
/// @return NULL if no element is linked, next element otherwise
///
/// @param[in] elem element
/// @param[in] link element link name
#define SLIST_NEXT(elem, link) \
  ((elem)->link.__slist_next)

/// Initialise the list.
///
/// @param[in] list list
#define SLIST_NEW(list)       \
  do {                        \
    SLIST_FIRST(list) = NULL; \
  } while (0)

/// Determine whether the list is empty.
/// @return decision
///
/// @param[in] list list
#define SLIST_EMPTY(list) \
  (SLIST_FIRST(list) == NULL)

/// Traverse the list and execute a function for each element.
///
/// @param[in] list list
/// @param[in] type element C type name
/// @param[in] link element link name
/// @param[in] func function
/// @param[in] ...   variable-length arguments for the function
#define SLIST_MAP(list, type, link, func, ...)           \
  do {                                                   \
    type* __slist_elem;                                  \
    for (__slist_elem = SLIST_FIRST(list);               \
         __slist_elem != NULL;                           \
         __slist_elem = SLIST_NEXT(__slist_elem, link))  \
      func(__slist_elem, __VA_ARGS__);                   \
  } while (0)

/// Traverse the list and remove elements that fail for a predicate.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean resource deallocation decision
/// @param[in] func  predicate
/// @param[in] ...   variable-length arguments for the predicate
#define SLIST_FILTER(list, type, link, clean, func, ...)  \
  do {                                                    \
    type** __slist_curr;                                  \
    type* __slist_elem;                                   \
    __slist_curr = &(SLIST_FIRST(list));                  \
    while (*__slist_curr != NULL) {                       \
      __slist_elem = *__slist_curr;                       \
      if (func(__slist_elem, __VA_ARGS__ )) {             \
        *__slist_curr = SLIST_NEXT(__slist_elem, link);   \
        if (clean)                                        \
          free(__slist_elem);                             \
      } else {                                            \
        __slist_curr = &(SLIST_NEXT(__slist_elem, link)); \
      }                                                   \
    }                                                     \
  } while (0)

/// Reverse the order of the elements in the list.
///
/// @param[in] list list
/// @param[in] type element C type name
/// @param[in] link element link name
#define SLIST_REVERSE(list, type, link)              \
  do {                                               \
    type* __slist_prev;                              \
    type* __slist_curr;                              \
    type* __slist_next;                              \
    __slist_prev = NULL;                             \
    __slist_curr = SLIST_FIRST(list);                \
    while (__slist_curr != NULL) {                   \
      __slist_next = SLIST_NEXT(__slist_curr, link); \
      SLIST_NEXT(__slist_curr, link) = __slist_prev; \
      __slist_prev = __slist_curr;                   \
      __slist_curr = __slist_next;                   \
    }                                                \
    SLIST_FIRST(list) = __slist_prev;                \
  } while (0)

/// Insert an element to the head of the list.
///
/// @param[in] list list
/// @param[in] elem element
/// @param[in] link element link name
#define SLIST_PUSH(list, elem, link)            \
  do {                                          \
    SLIST_NEXT(elem, link) = SLIST_FIRST(list); \
    SLIST_FIRST(list) = (elem);                 \
  } while (0)

/// Insert an element after the specified element.
///
/// @param[in] elem  element
/// @param[in] toadd element to be added
/// @param[in] link  element link name
#define SLIST_INSERT(elem, toadd, link)               \
  do {                                                \
    SLIST_NEXT(toadd, link) = SLIST_NEXT(elem, link); \
    SLIST_NEXT(elem, link) = (toadd);                 \
  } while (0)

/// Remove an element from the head of the list.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean decision to deallocate memory
#define SLIST_POP(list, type, link, clean)                \
  do {                                                    \
    type* __slist_elem;                                   \
    __slist_elem = SLIST_FIRST(list);                     \
    if (__slist_elem != NULL) {                           \
      SLIST_FIRST(list) = SLIST_NEXT(__slist_elem, link); \
      if (clean)                                          \
        free(__slist_elem);                               \
    }                                                     \
  } while (0)

/// Remove an element that is linked to the specified element.
///
/// @param[in] elem  element
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean decision to deallocate memory
#define SLIST_REMOVE(elem, type, link, clean)                \
  do {                                                       \
    type* __slist_elem;                                      \
    __slist_elem = SLIST_NEXT(elem, link);                   \
    if (__slist_elem == NULL)                                \
      break;                                                 \
    SLIST_NEXT(elem, link) = SLIST_NEXT(__slist_elem, link); \
    if (clean)                                               \
      free(__slist_elem);                                    \
  } while (0)

/// Remove all elements from the list.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean decision to deallocate memory
#define SLIST_FREE(list, type, link, clean) \
  do {                                      \
    while (!SLIST_EMPTY(list))              \
      SLIST_POP(list, type, link, clean);   \
  } while (0)

#endif
