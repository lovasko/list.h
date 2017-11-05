#ifndef SLIST_H
#define SLIST_H

#include <stdlib.h>
#include <stdint.h>


//////////////////////////////////////
// Function      // Time complexity //
//////////////////////////////////////
// SLIST_LINK    // compile time    //
// SLIST_TYPE    // compile time    //
// SLIST_FIRST   // O(1)            //
// SLIST_LAST    // O(n)            //
// SLIST_NTH     // O(n)            //
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
// SLIST_ATTACH  // O(1)            //
// SLIST_DETACH  // O(1)            //
// SLIST_DROP    // O(n)            //
// SLIST_TAKE    // O(n)            //
// SLIST_ALL     // O(n)            //
// SLIST_ANY     // O(n)            //
// SLIST_MAX     // O(n)            //
// SLIST_MIN     // O(n)            //
// SLIST_LENGTH  // O(n)            //
// SLIST_ZIP     // O(n)            //
// SLIST_MEMBER  // O(n)            //
//////////////////////////////////////

/// Definition of a new element link.
///
/// @param[in] tag struct tag name
#define SLIST_LINK(tag)     \
  struct {                  \
    struct tag* _slist_nxt; \
  }

/// Definition of a new list type.
///
/// @param[in] tag  struct tag name
/// @param[in] type list element type
#define SLIST_TYPE(tag, type) \
  struct tag {                \
    type* _slist_fst;         \
  }

/// Internal function to access the next linked element.
/// Please note that this function is for internal use only and should not be
/// used in your code.
/// @return NULL if no element is linked, next element otherwise
///
/// @param[in] elem element
/// @param[in] link element link name
#define _SLIST_NXT(elem, link) \
  ((elem)->link._slist_nxt)

/// Internal function to access the first element of the list.
/// Please note that this function is for internal use only and should not be
/// used in your code.
/// @return NULL if empty, first element otherwise
///
/// @param[in] list list
#define _SLIST_FST(list) \
  ((list)->_slist_fst)

/// Obtain the first element of the list.
///
/// @param[out] out  first element
/// @param[in]  list list
#define SLIST_FIRST(out, list) \
  do {                         \
    *(out) = _SLIST_FST(list); \
  } while (0)

/// Obtain the last element of the list.
///
/// @param[out] out  last element (NULL if the list is empty)
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
#define SLIST_LAST(out, list, type, link)      \
  do {                                         \
    type* _slist_e = _SLIST_FST(list);         \
    if (_slist_e == NULL) {                    \
      *(out) = NULL;                           \
      break;                                   \
    }                                          \
    while (_SLIST_NXT(_slist_e, link) != NULL) \
      _slist_e = _SLIST_NXT(_slist_e, link);   \
    *(out) = _slist_e;                         \
  } while (0)

/// Obtain the n-th element of the list.
///
/// @param[out] out  n-th element
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  n    position of the element
#define SLIST_NTH(out, list, type, link, n)                  \
  do {                                                       \
    type* _slist_e = _SLIST_FST(list);                       \
    if (_slist_e == NULL) {                                  \
      *(out) = NULL;                                         \
      break;                                                 \
    }                                                        \
    for (intmax_t _slist_i = 0;                              \
         _slist_i < (n) && _slist_e != NULL;                 \
         _slist_i++, _slist_e = _SLIST_NXT(_slist_e, link)); \
    *(out) = _slist_e;                                       \
  } while (0)

/// Obtain the next linked element.
///
/// @param[out] out  next element
/// @param[in]  elem element
/// @param[in]  link element link name
#define SLIST_NEXT(out, elem, link)  \
  do {                               \
    *(out) = _SLIST_NXT(elem, link); \
  } while (0)

/// Initialise the list.
///
/// @param[in] list list
#define SLIST_NEW(list)      \
  do {                       \
    _SLIST_FST(list) = NULL; \
  } while (0)

/// Determine whether the list is empty.
/// @return decision
///
/// @param[out] out  decision
/// @param[in]  list list
#define SLIST_EMPTY(out, list)           \
  do {                                   \
    *(out) = (_SLIST_FST(list) == NULL); \
  } while (0)

/// Traverse the list and execute a function for each element.
///
/// @param[in] list list
/// @param[in] type element C type name
/// @param[in] link element link name
/// @param[in] func function
/// @param[in] ...   variable-length arguments for the function
#define SLIST_MAP(list, type, link, func, ...)  \
  do {                                          \
    for (type* _slist_e = _SLIST_FST(list);     \
         _slist_e != NULL;                      \
         _slist_e = _SLIST_NXT(_slist_e, link)) \
      func(_slist_e, __VA_ARGS__);              \
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
    type** _slist_c = &(_SLIST_FST(list));               \
    type* _slist_e = NULL;                               \
    while (*_slist_c != NULL) {                          \
      _slist_e = *_slist_c;                              \
      if (func(_slist_e, __VA_ARGS__)) {                 \
        *_slist_c = _SLIST_NXT(_slist_e, link);          \
        if (clean != NULL)                               \
          clean(_slist_e);                               \
      } else {                                           \
        _slist_c = &(_SLIST_NXT(_slist_e, link));        \
      }                                                  \
    }                                                    \
  } while (0)

/// Reverse the order of the elements in the list.
///
/// @param[in] list list
/// @param[in] type element C type name
/// @param[in] link element link name
#define SLIST_REVERSE(list, type, link)      \
  do {                                       \
    type* _slist_p = NULL;                   \
    type* _slist_c = _SLIST_FST(list);       \
    type* _slist_n = NULL;                   \
    while (_slist_c != NULL) {               \
      _slist_n = _SLIST_NXT(_slist_c, link); \
      _SLIST_NXT(_slist_c, link) = _slist_p; \
      _slist_p = _slist_c;                   \
      _slist_c = _slist_n;                   \
    }                                        \
    _SLIST_FST(list) = _slist_p;             \
  } while (0)

/// Find the first matching element of the list.
///
/// @param[out] out  matching element or NULL
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  func matching function
/// @param[in]  ...  variable-length arguments for the matching function
#define SLIST_FIND(out, list, type, link, func, ...) \
  do {                                               \
    *(out) = NULL;                                   \
    for (type* _slist_e = _SLIST_FST(list);          \
         _slist_e != NULL;                           \
         _slist_e = _SLIST_NXT(_slist_e, link)) {    \
      if (func(_slist_e, __VA_ARGS__)) {             \
        *(out) = _slist_e;                           \
        break;                                       \
      }                                              \
    }                                                \
  } while(0)

/// Insert an element to the head of the list.
///
/// @param[in] list list
/// @param[in] elem element
/// @param[in] link element link name
#define SLIST_PUSH(list, elem, link)           \
  do {                                         \
    _SLIST_NXT(elem, link) = _SLIST_FST(list); \
    _SLIST_FST(list) = (elem);                 \
  } while (0)

/// Insert an element after the specified element.
///
/// @param[in] elem  element
/// @param[in] toadd element to be added
/// @param[in] link  element link name
#define SLIST_INSERT(elem, toadd, link)               \
  do {                                                \
    _SLIST_NXT(toadd, link) = _SLIST_NXT(elem, link); \
    _SLIST_NXT(elem, link) = (toadd);                 \
  } while (0)

/// Remove an element from the head of the list.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean deallocation function
#define SLIST_POP(list, type, link, clean)           \
  do {                                               \
    type* _slist_e = _SLIST_FST(list);               \
    if (_slist_e != NULL) {                          \
      _SLIST_FST(list) = _SLIST_NXT(_slist_e, link); \
      if (clean != NULL)                             \
        clean(_slist_e);                             \
    }                                                \
  } while (0)

/// Remove an element that is linked to the specified element.
///
/// @param[in] elem  element
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean deallocation function
#define SLIST_REMOVE(elem, type, link, clean)            \
  do {                                                   \
    type* _slist_e = _SLIST_NXT(elem, link);             \
    if (_slist_e == NULL)                                \
      break;                                             \
    _SLIST_NXT(elem, link) = _SLIST_NXT(_slist_e, link); \
    if (clean != NULL)                                   \
      clean(_slist_e);                                   \
  } while (0)

/// Remove all elements from the list.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean deallocation function
#define SLIST_FREE(list, type, link, clean) \
  do {                                      \
    while (_SLIST_FST(list) != NULL)        \
      SLIST_POP(list, type, link, clean);   \
  } while (0)

/// Attach a new chain of elements to the list instead of the current one.
///
/// @param[in] list list
/// @param[in] elem first element of the chain
#define SLIST_ATTACH(list, elem) \
  do {                           \
    _SLIST_FST(list) = (elem);  \
  } while (0)

/// Detach all elements from the list without releasing any resources.
///
/// @param[in] list
#define SLIST_DETACH(list)   \
  do {                       \
    _SLIST_FST(list) = NULL; \
  } while (0)

/// Drop the first N elements from the list.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] n     number of elements to drop
/// @param[in] clean deallocation function
#define SLIST_DROP(list, type, link, n, clean) \
  do {                                         \
    for (intmax_t _slist_i = 0;                \
         _slist_i < (intmax_t)(n);             \
         _slist_i++) {                         \
      SLIST_POP(list, type, link, clean);      \
      if (_SLIST_FST(list) == NULL)            \
        break;                                 \
    }                                          \
  } while (0)

/// Take the first N elements from the list and dispose of the rest.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] n     number of elements to take
/// @param[in] clean deallocation function
#define SLIST_TAKE(list, type, link, n, clean)             \
  do {                                                     \
    intmax_t _slist_i = 1;                                 \
    type* _slist_f = _SLIST_FST(list);                     \
    while (_slist_f != NULL && _slist_i < (intmax_t)(n)) { \
      _slist_f = _SLIST_NXT(_slist_f, link);               \
      _slist_i++;                                          \
    }                                                      \
    while (_SLIST_NXT(_slist_f, link) != NULL)             \
      SLIST_REMOVE(_slist_f, type, link, clean);           \
  } while (0)

/// Verify that all list elements satisfy a given predicate.
///
/// @param[out] out  decision
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  func function predicate
/// @param[in]  ...  variable-length arguments for the function
#define SLIST_ALL(out, list, type, link, func, ...) \
  do {                                              \
    *(out) = true;                                  \
    for (type* _slist_e = _SLIST_FST(list);         \
         _slist_e != NULL;                          \
         _slist_e = _SLIST_NXT(_slist_e, link)) {   \
      if (!func(_slist_e, __VA_ARGS__)) {           \
        *(out) = false;                             \
        break;                                      \
      }                                             \
    }                                               \
  } while (0)

/// Verify that at least one list element satisfies a given predicate.
///
/// @param[out] out  decision
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  func function predicate
/// @param[in]  ...  variable-length arguments for the function
#define SLIST_ANY(out, list, type, link, func, ...) \
  do {                                              \
    *(out) = false;                                 \
    for (type* _slist_e = _SLIST_FST(list);         \
         _slist_e != NULL;                          \
         _slist_e = _SLIST_NXT(_slist_e, link)) {   \
      if (func(_slist_e, __VA_ARGS__)) {            \
        *(out) = true;                              \
        break;                                      \
      }                                             \
    }                                               \
  } while (0)

/// Find the maximal list element.
///
/// @param[out] out  maximal element
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  func comparison function
/// @param[in]  ...  variable-length arguments for the function
#define SLIST_MAX(out, list, type, link, func, ...) \
  do {                                              \
    type* _slist_e;                                 \
    _slist_e = *(out) = _SLIST_FST(list);           \
    if (*(out) == NULL)                             \
      break;                                        \
    while (true) {                                  \
      if (func(*(out), _slist_e, __VA_ARGS__) < 1)  \
        *(out) = _slist_e;                          \
      _slist_e = _SLIST_NXT(_slist_e, link);        \
      if (_slist_e == NULL)                         \
        break;                                      \
    }                                               \
  } while (0)

/// Find the minimal list element.
///
/// @param[out] out  maximal element
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  func comparison function
/// @param[in]  ...  variable-length arguments for the function
#define SLIST_MIN(out, list, type, link, func, ...) \
  do {                                              \
    type* _slist_e;                                 \
    _slist_e = *(out) = _SLIST_FST(list);           \
    if (*(out) == NULL)                             \
      break;                                        \
    while (true) {                                  \
      if (func(*(out), _slist_e, __VA_ARGS__) > -1) \
        *(out) = _slist_e;                          \
      _slist_e = _SLIST_NXT(_slist_e, link);        \
      if (_slist_e == NULL)                         \
        break;                                      \
    }                                               \
  } while (0)

/// Compute the length of the list.
///
/// @param[out] out  length of the list (zero if empty)
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
#define SLIST_LENGTH(out, list, type, link)     \
  do {                                          \
    *(out) = 0;                                 \
    for (type* _slist_e = _SLIST_FST(list);     \
         _slist_e != NULL;                      \
         _slist_e = _SLIST_NXT(_slist_e, link)) \
      *(out) += 1;                              \
  } while (0)

/// Check whether an element is a member of the list.
///
/// @param[out] out  decision
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  elem element to check
#define SLIST_MEMBER(out, list, type, link, elem) \
  do {                                            \
    type* _slist_e = _SLIST_FST(list);            \
    *(out) = false;                               \
    while (_slist_e != NULL) {                    \
      if (_slist_e == elem) {                     \
        *(out) = true;                            \
        break;                                    \
      }                                           \
      _slist_e = _SLIST_NXT(_slist_e, link);      \
    }                                             \
  } while (0)

#endif
