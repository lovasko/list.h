#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <stdint.h>


//////////////////////////////////////
// Function      // Time complexity //
//////////////////////////////////////
// LIST_LINK    // compile time     //
// LIST_TYPE    // compile time     //
// LIST_FIRST   // O(1)             //
// LIST_LAST    // O(n)             //
// LIST_NTH     // O(n)             //
// LIST_NEXT    // O(1)             //
// LIST_NEW     // O(1)             //
// LIST_EMPTY   // O(1)             //
// LIST_LENGTH  // O(n)             //
// LIST_MAP     // O(n)             //
// LIST_FILTER  // O(n)             //
// LIST_REVERSE // O(n)             //
// LIST_FIND    // O(n)             //
// LIST_PUSH    // O(1)             //
// LIST_INSERT  // O(1)             //
// LIST_POP     // O(1)             //
// LIST_REMOVE  // O(1)             //
// LIST_FREE    // O(n)             //
// LIST_SORT    // O(n * log n)     //
// LIST_UNIQUE  // O(n)             //
// LIST_COPY    // O(1)             //
// LIST_ATTACH  // O(1)             //
// LIST_DETACH  // O(1)             //
// LIST_DROP    // O(n)             //
// LIST_TAKE    // O(n)             //
// LIST_ALL     // O(n)             //
// LIST_ANY     // O(n)             //
// LIST_MAX     // O(n)             //
// LIST_MIN     // O(n)             //
// LIST_LENGTH  // O(n)             //
// LIST_ZIP     // O(n)             //
// LIST_MEMBER  // O(n)             //
//////////////////////////////////////

/// Definition of a new element link.
///
/// @param[in] tag struct tag name
#define LIST_LINK(tag)     \
  struct {                 \
    struct tag* _list_nxt; \
  }

/// Definition of a new list type.
///
/// @param[in] tag  struct tag name
/// @param[in] type list element type
#define LIST_TYPE(tag, type) \
  struct tag {               \
    type* _list_fst;         \
  }

/// Internal function to access the next linked element.
/// Please note that this function is for internal use only and should not be
/// used in your code.
/// @return NULL if no element is linked, next element otherwise
///
/// @param[in] elem element
/// @param[in] link element link name
#define _LIST_NXT(elem, link) \
  ((elem)->link._list_nxt)

/// Internal function to access the first element of the list.
/// Please note that this function is for internal use only and should not be
/// used in your code.
/// @return NULL if empty, first element otherwise
///
/// @param[in] list list
#define _LIST_FST(list) \
  ((list)->_list_fst)

/// Obtain the first element of the list.
///
/// @param[out] out  first element
/// @param[in]  list list
#define LIST_FIRST(out, list) \
  do {                        \
    *(out) = _LIST_FST(list); \
  } while (0)

/// Obtain the last element of the list.
///
/// @param[out] out  last element (NULL if the list is empty)
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
#define LIST_LAST(out, list, type, link)     \
  do {                                       \
    type* _list_e = _LIST_FST(list);         \
    if (_list_e == NULL) {                   \
      *(out) = NULL;                         \
      break;                                 \
    }                                        \
    while (_LIST_NXT(_list_e, link) != NULL) \
      _list_e = _LIST_NXT(_list_e, link);    \
    *(out) = _list_e;                        \
  } while (0)

/// Obtain the n-th element of the list.
///
/// @param[out] out  n-th element
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  n    position of the element
#define LIST_NTH(out, list, type, link, n)               \
  do {                                                   \
    type* _list_e = _LIST_FST(list);                     \
    if (_list_e == NULL) {                               \
      *(out) = NULL;                                     \
      break;                                             \
    }                                                    \
    for (intmax_t _list_i = 0;                           \
         _list_i < (n) && _list_e != NULL;               \
         _list_i++, _list_e = _LIST_NXT(_list_e, link)); \
    *(out) = _list_e;                                    \
  } while (0)

/// Obtain the next linked element.
///
/// @param[out] out  next element
/// @param[in]  elem element
/// @param[in]  link element link name
#define LIST_NEXT(out, elem, link)  \
  do {                              \
    *(out) = _LIST_NXT(elem, link); \
  } while (0)

/// Initialise the list.
///
/// @param[in] list list
#define LIST_NEW(list)      \
  do {                      \
    _LIST_FST(list) = NULL; \
  } while (0)

/// Determine whether the list is empty.
/// @return decision
///
/// @param[out] out  decision
/// @param[in]  list list
#define LIST_EMPTY(out, list)           \
  do {                                  \
    *(out) = (_LIST_FST(list) == NULL); \
  } while (0)

/// Traverse the list and execute a function for each element.
///
/// @param[in] list list
/// @param[in] type element C type name
/// @param[in] link element link name
/// @param[in] func function
/// @param[in] ...   variable-length arguments for the function
#define LIST_MAP(list, type, link, func, ...) \
  do {                                        \
    type* _list_e = _LIST_FST(list);          \
    intmax_t _list_i = 0;                     \
    while (_list_e != NULL) {                 \
      func(_list_e, _list_i, __VA_ARGS__);    \
      _list_e = _LIST_NXT(_list_e, link);     \
      _list_i++;                              \
    }                                         \
  } while (0)

/// Traverse the list and remove elements that fail for a predicate.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean deallocation function
/// @param[in] func  predicate
/// @param[in] ...   variable-length arguments for the predicate
#define LIST_FILTER(list, type, link, clean, func, ...) \
  do {                                                  \
    type** _list_c = &(_LIST_FST(list));                \
    type* _list_e = NULL;                               \
    intmax_t _list_i = 0;                               \
    while (*_list_c != NULL) {                          \
      _list_e = *_list_c;                               \
      if (func(_list_e, _list_i, __VA_ARGS__)) {        \
        *_list_c = _LIST_NXT(_list_e, link);            \
        if (clean != NULL)                              \
          clean(_list_e);                               \
      } else {                                          \
        _list_c = &(_LIST_NXT(_list_e, link));          \
      }                                                 \
      _list_i++;                                        \
    }                                                   \
  } while (0)

/// Reverse the order of the elements in the list.
///
/// @param[in] list list
/// @param[in] type element C type name
/// @param[in] link element link name
#define LIST_REVERSE(list, type, link)    \
  do {                                    \
    type* _list_p = NULL;                 \
    type* _list_c = _LIST_FST(list);      \
    type* _list_n = NULL;                 \
    while (_list_c != NULL) {             \
      _list_n = _LIST_NXT(_list_c, link); \
      _LIST_NXT(_list_c, link) = _list_p; \
      _list_p = _list_c;                  \
      _list_c = _list_n;                  \
    }                                     \
    _LIST_FST(list) = _list_p;            \
  } while (0)

/// Find the first matching element of the list.
///
/// @param[out] out  matching element or NULL
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  func matching function
/// @param[in]  ...  variable-length arguments for the matching function
#define LIST_FIND(out, list, type, link, func, ...) \
  do {                                              \
    *(out) = NULL;                                  \
    for (type* _list_e = _LIST_FST(list);           \
         _list_e != NULL;                           \
         _list_e = _LIST_NXT(_list_e, link)) {      \
      if (func(_list_e, __VA_ARGS__)) {             \
        *(out) = _list_e;                           \
        break;                                      \
      }                                             \
    }                                               \
  } while(0)

/// Insert an element to the head of the list.
///
/// @param[in] list list
/// @param[in] elem element
/// @param[in] link element link name
#define LIST_PUSH(list, elem, link)          \
  do {                                       \
    _LIST_NXT(elem, link) = _LIST_FST(list); \
    _LIST_FST(list) = (elem);                \
  } while (0)

/// Insert an element after the specified element.
///
/// @param[in] elem  element
/// @param[in] toadd element to be added
/// @param[in] link  element link name
#define LIST_INSERT(elem, toadd, link)              \
  do {                                              \
    _LIST_NXT(toadd, link) = _LIST_NXT(elem, link); \
    _LIST_NXT(elem, link) = (toadd);                \
  } while (0)

/// Remove an element from the head of the list.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean deallocation function
#define LIST_POP(list, type, link, clean)         \
  do {                                            \
    type* _list_e = _LIST_FST(list);              \
    if (_list_e != NULL) {                        \
      _LIST_FST(list) = _LIST_NXT(_list_e, link); \
      if (clean != NULL)                          \
        clean(_list_e);                           \
    }                                             \
  } while (0)

/// Remove an element that is linked to the specified element.
///
/// @param[in] elem  element
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean deallocation function
#define LIST_REMOVE(elem, type, link, clean)          \
  do {                                                \
    type* _list_e = _LIST_NXT(elem, link);            \
    if (_list_e == NULL)                              \
      break;                                          \
    _LIST_NXT(elem, link) = _LIST_NXT(_list_e, link); \
    if (clean != NULL)                                \
      clean(_list_e);                                 \
  } while (0)

/// Remove all elements from the list.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] clean deallocation function
#define LIST_FREE(list, type, link, clean) \
  do {                                     \
    while (_LIST_FST(list) != NULL)        \
      LIST_POP(list, type, link, clean);   \
  } while (0)

/// Attach a new chain of elements to the list instead of the current one.
///
/// @param[in] list list
/// @param[in] elem first element of the chain
#define LIST_ATTACH(list, elem) \
  do {                          \
    _LIST_FST(list) = (elem);   \
  } while (0)

/// Detach all elements from the list without releasing any resources.
///
/// @param[in] list
#define LIST_DETACH(list)   \
  do {                      \
    _LIST_FST(list) = NULL; \
  } while (0)

/// Drop the first N elements from the list.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] n     number of elements to drop
/// @param[in] clean deallocation function
#define LIST_DROP(list, type, link, n, clean) \
  do {                                        \
    for (intmax_t _list_i = 0;                \
         _list_i < (intmax_t)(n);             \
         _list_i++) {                         \
      LIST_POP(list, type, link, clean);      \
      if (_LIST_FST(list) == NULL)            \
        break;                                \
    }                                         \
  } while (0)

/// Take the first N elements from the list and dispose of the rest.
///
/// @param[in] list  list
/// @param[in] type  element C type name
/// @param[in] link  element link name
/// @param[in] n     number of elements to take
/// @param[in] clean deallocation function
#define LIST_TAKE(list, type, link, n, clean)            \
  do {                                                   \
    intmax_t _list_i = 1;                                \
    type* _list_f = _LIST_FST(list);                     \
    while (_list_f != NULL && _list_i < (intmax_t)(n)) { \
      _list_f = _LIST_NXT(_list_f, link);                \
      _list_i++;                                         \
    }                                                    \
    while (_LIST_NXT(_list_f, link) != NULL)             \
      LIST_REMOVE(_list_f, type, link, clean);           \
  } while (0)

/// Verify that all list elements satisfy a given predicate.
///
/// @param[out] out  decision
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  func function predicate
/// @param[in]  ...  variable-length arguments for the function
#define LIST_ALL(out, list, type, link, func, ...) \
  do {                                             \
    *(out) = true;                                 \
    for (type* _list_e = _LIST_FST(list);          \
         _list_e != NULL;                          \
         _list_e = _LIST_NXT(_list_e, link)) {     \
      if (!func(_list_e, __VA_ARGS__)) {           \
        *(out) = false;                            \
        break;                                     \
      }                                            \
    }                                              \
  } while (0)

/// Verify that at least one list element satisfies a given predicate.
///
/// @param[out] out  decision
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  func function predicate
/// @param[in]  ...  variable-length arguments for the function
#define LIST_ANY(out, list, type, link, func, ...) \
  do {                                             \
    *(out) = false;                                \
    for (type* _list_e = _LIST_FST(list);          \
         _list_e != NULL;                          \
         _list_e = _LIST_NXT(_list_e, link)) {     \
      if (func(_list_e, __VA_ARGS__)) {            \
        *(out) = true;                             \
        break;                                     \
      }                                            \
    }                                              \
  } while (0)

/// Find the maximal list element.
///
/// @param[out] out  maximal element
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  func comparison function
/// @param[in]  ...  variable-length arguments for the function
#define LIST_MAX(out, list, type, link, func, ...) \
  do {                                             \
    type* _list_e;                                 \
    _list_e = *(out) = _LIST_FST(list);            \
    if (*(out) == NULL)                            \
      break;                                       \
    while (true) {                                 \
      if (func(*(out), _list_e, __VA_ARGS__) < 1)  \
        *(out) = _list_e;                          \
      _list_e = _LIST_NXT(_list_e, link);          \
      if (_list_e == NULL)                         \
        break;                                     \
    }                                              \
  } while (0)

/// Find the minimal list element.
///
/// @param[out] out  maximal element
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  func comparison function
/// @param[in]  ...  variable-length arguments for the function
#define LIST_MIN(out, list, type, link, func, ...) \
  do {                                             \
    type* _list_e;                                 \
    _list_e = *(out) = _LIST_FST(list);            \
    if (*(out) == NULL)                            \
      break;                                       \
    while (true) {                                 \
      if (func(*(out), _list_e, __VA_ARGS__) > -1) \
        *(out) = _list_e;                          \
      _list_e = _LIST_NXT(_list_e, link);          \
      if (_list_e == NULL)                         \
        break;                                     \
    }                                              \
  } while (0)

/// Compute the length of the list.
///
/// @param[out] out  length of the list (zero if empty)
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
#define LIST_LENGTH(out, list, type, link)   \
  do {                                       \
    *(out) = 0;                              \
    for (type* _list_e = _LIST_FST(list);    \
         _list_e != NULL;                    \
         _list_e = _LIST_NXT(_list_e, link)) \
      *(out) += 1;                           \
  } while (0)

/// Check whether an element is a member of the list.
///
/// @param[out] out  decision
/// @param[in]  list list
/// @param[in]  type element C type name
/// @param[in]  link element link name
/// @param[in]  elem element to check
#define LIST_MEMBER(out, list, type, link, elem) \
  do {                                           \
    type* _list_e = _LIST_FST(list);             \
    *(out) = false;                              \
    while (_list_e != NULL) {                    \
      if (_list_e == elem) {                     \
        *(out) = true;                           \
        break;                                   \
      }                                          \
      _list_e = _LIST_NXT(_list_e, link);        \
    }                                            \
  } while (0)

#endif
