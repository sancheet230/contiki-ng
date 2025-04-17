#include "contiki.h"
#include "sys/ctimer.h"
#include "sys/clock.h"
#include "lib/list.h"
#include <stdio.h>
#include <stdlib.h>

PROCESS(test_list_process, "Linked List Test");
AUTOSTART_PROCESSES(&test_list_process);

LIST(test_list);

struct test_item {
  struct test_item *next;
  int value;
};

static void print_list(void) {
  struct test_item *item;
  printf("List contents: ");
  for(item = list_head(test_list); item != NULL; item = item->next) {
    printf("%d ", item->value);
  }
  printf("\n");
}

static void test_list_operations(void *ptr) {
  struct test_item *item1, *item2, *item3;

  item1 = malloc(sizeof(struct test_item));
  item1->value = 1;
  list_add(test_list, item1);

  item2 = malloc(sizeof(struct test_item));
  item2->value = 2;
  list_add(test_list, item2);

  item3 = malloc(sizeof(struct test_item));
  item3->value = 3;
  list_add(test_list, item3);

  print_list();

  list_remove(test_list, item2);
  free(item2);

  print_list();

  list_push(test_list, item2);
  print_list();

  struct test_item *popped_item = list_pop(test_list);
  printf("Popped item: %d\n", popped_item->value);
  free(popped_item);

  print_list();

  struct test_item *chopped_item = list_chop(test_list);
  printf("Chopped item: %d\n", chopped_item->value);
  free(chopped_item);

  print_list();

  list_copy(test_list, test_list);
  print_list();

  list_insert(test_list, item1, item2);
  print_list();

  printf("List length: %d\n", list_length(test_list));

  list_remove(test_list, item1);
  free(item1);
  list_remove(test_list, item2);
  free(item2);
  list_remove(test_list, item3);
  free(item3);

  print_list();
}

PROCESS_THREAD(test_list_process, ev, data) {
  PROCESS_BEGIN();

  list_init(test_list);
  ctimer_set(&test_timer, CLOCK_SECOND, test_list_operations, NULL);

  PROCESS_END();
}
