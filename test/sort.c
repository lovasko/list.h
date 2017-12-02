#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "../list.h"


/// Element.
typedef struct _elem {
  LIST_LINK(_elem) el_next;
  int              el_num;
} elem;

/// List.
typedef LIST_TYPE(_list, elem) list;

/// Compare two elements by the numerical value they store.
/// @return comparison result
/// @retval  0 a and b equal
/// @retval  1 a is greater
/// @retval -1 b is greater
///
/// @param[in] a first element
/// @param[in] b second element
/// @param[in] p unused payload pointer
static int
compare(const elem* a, const elem* b, void* p)
{
  (void)p;

  if (a->el_num == b->el_num)
    return 0;

  if (a->el_num > b->el_num)
    return 1;
  else
    return -1;
}

/// Print the numerical value stored in the element to the standard output
/// stream, followed either by a comma or a new-line character.
///
/// @param[in] a element
/// @param[in] i unused index of the element
/// @param[in] p unused payload pointer
static void
print(const elem* a, intmax_t i, void* p)
{
  elem* n;

  (void)i;
  (void)p;

  LIST_NEXT(&n, a, el_next);
  printf("%02d%s", a->el_num, n != NULL ? ", " : "\n");
}

/// Check if the list is sorted.
///
/// @param[in] a element
/// @param[in] i unused index of the element
/// @param[in] r result of the check
static void
is_sorted(const elem* a, intmax_t i, bool* r)
{
  elem* b;

  (void)i;

  LIST_NEXT(&b, a, el_next);
  if (b == NULL)
    return;

  if (a->el_num > b->el_num)
    *r = false;
}

int
main(void)
{
  list l;
  elem* e;
  int i;
  int k;
  int m;
  int n;
  bool r;

  srand(time(NULL));

  for (i = 0; i < 1000000; i++) {
    m = (rand() % 100) + 1;

    // Initialisation.
    LIST_NEW(&l);
    for (k = 0; k < m; k++) {
      e = malloc(sizeof(elem));
      e->el_num = rand() % 20;
      LIST_PUSH(&l, e, el_next);
    }

    // Sort.
    LIST_SORT(&l, elem, el_next, compare, NULL);

    // Check the result correctness.
    r = true;
    LIST_MAP(&l, elem, el_next, is_sorted, &r);
    if (!r) {
      printf("List is not sorted: ");
      LIST_MAP(&l, elem, el_next, print, NULL);
      return EXIT_FAILURE;
    }

    // Check the result length.
    LIST_LENGTH(&n, &l, elem, el_next);
    if (n != m) {
      printf("Length does not match, got: %d, expected: %d\n", n, m);
      LIST_MAP(&l, elem, el_next, print, NULL);
      return EXIT_FAILURE;
    }

    LIST_FREE(&l, elem, el_next, free);
  }

  return EXIT_SUCCESS;
}
