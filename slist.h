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
// SLIST_SORT    // O(n * log n)    //
// SLIST_UNIQUE  // O(n)            //
// SLIST_COPY    // O(1)            //
// SLIST_DETACH  // O(1)            //
// SLIST_ATTACH  // O(1)            //
// SLIST_DROP    // O(n)            //
// SLIST_TAKE    // O(n)            //
// SLIST_LAST    // O(n)            //
// SLIST_ALL     // O(n)            //
// SLIST_ANY     // O(n)            //
// SLIST_MAX     // O(n)            //
// SLIST_MIN     // O(n)            //
// SLIST_LENGTH  // O(n)            //
// SLIST_ZIP     // O(n)            //
//////////////////////////////////////

/// Definition of a new element link.
///
/// @param[in] tag struct tag name
#define SLIST_LINK(tag)      \
  struct {                   \
    struct tag* _slist_next; \
  }

/// Definition of a new list type.
///
/// @param[in] tag  struct tag name
/// @param[in] type list element type
#define SLIST_TYPE(tag, type) \
  struct tag {                \
    type* _slist_fst;         \
  }

/// Obtain the first element of the list.
/// @return NULL if empty, first element otherwise
///
/// @param[in] list list
#define SLIST_FIRST(list) \
  ((list)->_slist_fst)

/// Obtain the linked element.
/// @return NULL if no element is linked, next element otherwise
///
/// @param[in] elem element
/// @param[in] link element link name
#define SLIST_NEXT(elem, link) \
  ((elem)->link._slist_next)

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
#define SLIST_MAP(list, type, link, func, ...)        \
  do {                                                \
    type* _slist_elem;                                \
    for (_slist_elem = SLIST_FIRST(list);             \
         _slist_elem != NULL;                         \
         _slist_elem = SLIST_NEXT(_slist_elem, link)) \
      func(_slist_elem, __VA_ARGS__);                 \
  } while (0)

/// Traverse the list and remove elements that fail for a predicate.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean deallocation function
/// @param[in] func  predicate
/// @param[in] ...   variable-length arguments for the predicate
#define SLIST_FILTER(list, type, link, clean, func, ...) \
  do {                                                   \
    type** _slist_curr;                                  \
    type* _slist_elem;                                   \
    _slist_curr = &(SLIST_FIRST(list));                  \
    while (*_slist_curr != NULL) {                       \
      _slist_elem = *_slist_curr;                        \
      if (func(_slist_elem, __VA_ARGS__)) {              \
        *_slist_curr = SLIST_NEXT(_slist_elem, link);    \
        if (clean != NULL)                               \
          clean(_slist_elem);                            \
      } else {                                           \
        _slist_curr = &(SLIST_NEXT(_slist_elem, link));  \
      }                                                  \
    }                                                    \
  } while (0)

/// Reverse the order of the elements in the list.
///
/// @param[in] list list
/// @param[in] type element C type name
/// @param[in] link element link name
#define SLIST_REVERSE(list, type, link)            \
  do {                                             \
    type* _slist_prev;                             \
    type* _slist_curr;                             \
    type* _slist_next;                             \
    _slist_prev = NULL;                            \
    _slist_curr = SLIST_FIRST(list);               \
    while (_slist_curr != NULL) {                  \
      _slist_next = SLIST_NEXT(_slist_curr, link); \
      SLIST_NEXT(_slist_curr, link) = _slist_prev; \
      _slist_prev = _slist_curr;                   \
      _slist_curr = _slist_next;                   \
    }                                              \
    SLIST_FIRST(list) = _slist_prev;               \
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
/// @param[in] clean deallocation function
#define SLIST_POP(list, type, link, clean)               \
  do {                                                   \
    type* _slist_elem;                                   \
    _slist_elem = SLIST_FIRST(list);                     \
    if (_slist_elem != NULL) {                           \
      SLIST_FIRST(list) = SLIST_NEXT(_slist_elem, link); \
      if (clean != NULL)                                 \
        clean(_slist_elem);                              \
    }                                                    \
  } while (0)

/// Remove an element that is linked to the specified element.
///
/// @param[in] elem  element
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean deallocation function
#define SLIST_REMOVE(elem, type, link, clean)               \
  do {                                                      \
    type* _slist_elem;                                      \
    _slist_elem = SLIST_NEXT(elem, link);                   \
    if (_slist_elem == NULL)                                \
      break;                                                \
    SLIST_NEXT(elem, link) = SLIST_NEXT(_slist_elem, link); \
    if (clean != NULL)                                      \
      clean(_slist_elem);                                   \
  } while (0)

/// Remove all elements from the list.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean deallocation function
#define SLIST_FREE(list, type, link, clean) \
  do {                                      \
    while (!SLIST_EMPTY(list))              \
      SLIST_POP(list, type, link, clean);   \
  } while (0)

#endif
