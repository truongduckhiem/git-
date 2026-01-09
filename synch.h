#ifndef THREADS_SYNCH_H
#define THREADS_SYNCH_H

#include "interrupt.h"
#include "thread.h"

/* Semaphore implementation. */
struct semaphore {
  unsigned value;
  struct list waiters;
};

/* Lock implementation. */
struct lock {
  struct thread *holder;     /* Thread holding lock (or NULL). */
  struct semaphore semaphore; /* Binary semaphore for lock. */
};

/* Condition variable implementation. */
struct condition {
  struct list waiters; /* List of threads waiting on condition variable. */
};

/* Function declarations. */
void sema_init (struct semaphore *sema, unsigned value);
void sema_down (struct semaphore *sema);
void sema_up (struct semaphore *sema);
bool sema_try_down (struct semaphore *sema);

void lock_init (struct lock *lock);
void lock_acquire (struct lock *lock);
void lock_release (struct lock *lock);
bool lock_held_by_current_thread (const struct lock *lock);

void cond_init (struct condition *cond);
void cond_wait (struct condition *cond, struct lock *lock);
void cond_signal (struct condition *cond, struct lock *lock);
void cond_broadcast (struct condition *cond, struct lock *lock);

#endif /* THREADS_SYNCH_H */