#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <inttypes.h>

#include "../slist.h"


/// Element.
typedef struct _elem {
  SLIST_LINK(_elem) el_next;
  intmax_t          el_num;
} elem;

/// List.
typedef SLIST_TYPE(_list, elem) list;

/// Compare two elements by the numerical value they store.
/// @return comparison result
/// @retval  0 equal
/// @retval  1 e1 is greater
/// @retval -1 e2 is greater
///
/// @param[in] a first element
/// @param[in] b second element
/// @param[in] p unused payload pointer
static int
elem_compare(const elem* restrict a, const elem* restrict b, const void* p)
{
  (void)p;
  if (a->el_num == b->el_num)
    return 0;

  if (a->el_num > b->el_num)
    return 1;
  else
    return -1;
}

/// Determine whether the number stored in the element is even.
/// @return decision
///
/// @param[in] e element
/// @param[in] p unused payload pointer
static bool
elem_is_even(const elem* e, const void* p)
{
  (void)p;
  return (e->el_num % 2) == 0;
}

/// Add the number stored in a element to the final sum.
///
/// @param[in] e element
/// @param[in] s sum
static void
elem_sum(const elem* restrict e, intmax_t* restrict s)
{
  *s += e->el_num;
}

int
main(int argc, char* argv[])
{
  list l;
  elem* e;
  elem* min;
  elem* max;
  intmax_t sum;
  bool all;
  bool any;
  bool empty;
  int i;

  // Insert numbers from the command-line arguments to the list.
  SLIST_NEW(&l);
  for (i = 1; i < argc; i++) {
    e = malloc(sizeof(*e));

    // Parse the number.
    errno = 0;
    e->el_num = strtoimax(argv[i], NULL, 10);
    if (e->el_num == 0 && errno != 0) {
      perror("strtoimax");
      SLIST_FREE(&l, elem, el_next, free);

      return EXIT_FAILURE;
    }

    SLIST_PUSH(&l, e, el_next);
  }

  // Verify that we have at least one element.
  SLIST_EMPTY(&empty, &l);
  if (empty) {
    printf("No numbers provided.\n");
    return EXIT_FAILURE;
  }

  // Compute the sum of all numbers in the list.
  sum = 0;
  SLIST_MAP(&l, elem, el_next, elem_sum, &sum);
  printf("Sum of the numbers is %" PRIiMAX "\n", sum);

  // Find the minimal number in the list.
  SLIST_MIN(&min, &l, elem, el_next, elem_compare, &min);
  printf("Minimum of the numbers is %" PRIiMAX "\n", min->el_num);

  // Find the maximal number in the list.
  SLIST_MAX(&max, &l, elem, el_next, elem_compare, &max);
  printf("Maximum of the numbers is %" PRIiMAX "\n", max->el_num);

  // Determine whether all numbers in the list are even.
  SLIST_ALL(&all, &l, elem, el_next, elem_is_even, NULL);
  printf("%s numbers are even.\n", all ? "All" : "Not all");

  // Determine whether there is at least one even number in the list.
  SLIST_ANY(&any, &l, elem, el_next, elem_is_even, NULL);
  printf("%s of the numbers are even.\n", any ? "Some" : "None");

  // Release all elements.
  SLIST_FREE(&l, elem, el_next, free);

  return EXIT_SUCCESS;
}
