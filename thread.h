#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include <debug.h>
#include "list.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "synch.h"

#define PRI_MIN    0   /* Lowest priority. */
#define PRI_DEFAULT 31  /* Default priority. */
#define PRI_MAX    63  /* Highest priority. */
enum thread_status {
  THREAD_RUNNING,
  THREAD_READY,
  THREAD_BLOCKED,
  THREAD_DYING
};

/* A thread in the system. */
struct thread {
  /* Thread state. */
  enum thread_status status; /* Thread state. */
  char name[16];             /* Name (for debugging). */
  int priority;              /* Priority. */
  struct list_elem allelem;  /* All threads list element. */
  struct list_elem elem;     /* List of threads that are ready to run. */
  struct list locks;         /* List of locks held by this thread. */
};

void thread_init(void);
void thread_start(void);
void thread_tick(void);
void thread_print_stats(void);

#endif /* THREADS_THREAD_H */