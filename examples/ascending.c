#include <sys/time.h>

#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "../list.h"


/// Element.
typedef struct _elem {
  LIST_LINK(_elem) el_next;
  long             el_num;
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
elem_compare(const elem* a, const elem* b, void* p)
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
elem_print(const elem* a, intmax_t i, void* p)
{
  elem* n;

  (void)i;
  (void)p;

  LIST_NEXT(&n, a, el_next);
  printf("%02ld%s", a->el_num, n != NULL ? ", " : "\n");
}

int
main(int argc, char* argv[])
{
  list l;
  elem* e;
  int i;
  struct timeval t1;
  struct timeval t2;

  if (argc != 2) {
    fprintf(stderr, "ERROR: expecting exactly one argument.\n");
    return EXIT_FAILURE;
  }

  srand(time(NULL));
  LIST_NEW(&l);

  // Generate random elements.
  for (i = 0; i < atoi(argv[1]); i++) {
    e = malloc(sizeof(elem));
    e->el_num = rand() % 100;
    LIST_PUSH(&l, e, el_next);
  }

  // Print all elements unsorted.
  printf("Random: ");
  LIST_MAP(&l, elem, el_next, elem_print, NULL);

  // Sort all elements and measure the time it takes.
  gettimeofday(&t1, NULL);
  LIST_SORT(&l, elem, el_next, elem_compare, NULL);
  gettimeofday(&t2, NULL);

  // Print all elements sorted.
  printf("Sorted: ");
  LIST_MAP(&l, elem, el_next, elem_print, NULL);

  // Compute and report the sorting time.
  printf("Timing: %ldus\n",
         (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec));

  // Release all elements.
  LIST_FREE(&l, elem, el_next, free);

  return EXIT_SUCCESS;
}
