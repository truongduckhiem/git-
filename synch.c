#include  "synch.h"
#include "threads/thread.h"

#include "interrupt.h"
#include "list.h"

void sema_init (struct semaphore *sema, unsigned value) {
  ASSERT(sema != NULL);
  sema->value = value;
  list_init(&sema->waiters);
}

void sema_down (struct semaphore *sema) {
  struct thread *curr = thread_current();

  ASSERT(!intr_context());
  
  while (sema->value == 0) {
    list_insert_ordered(&sema->waiters, &curr->elem, thread_priority_less, NULL);
    thread_block();
  }
  
  sema->value--;
}

void sema_up (struct semaphore *sema) {
  struct thread *t;

  ASSERT(!intr_context());

  if (!list_empty(&sema->waiters)) {
    t = list_entry(list_pop_front(&sema->waiters), struct thread, elem);
    thread_unblock(t);
  }

  sema->value++;
}

/* Các hàm lock, cond, và các hàm khác tương tự sẽ được thêm vào đây. */