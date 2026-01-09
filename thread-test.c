#include <stdio.h>
#include "threads/thread.h"
#include "threads/synch.h"
#include "devices/timer.h"
static struct semaphore sema;

void thread_func1(void *aux UNUSED) {
  printf("Thread 1: Waiting on semaphore...\n");
  sema_down(&sema);
  printf("Thread 1: Acquired semaphore!\n");
}

void thread_func2(void *aux UNUSED) {
  printf("Thread 2: Sleeping 1 sec before up semaphore...\n");
  timer_sleep(1000);
  printf("Thread 2: Waking and up semaphore\n");
  sema_up(&sema);
}

void test_thread_test(void) {
  printf("Ho ten: Phan Xuan Thuy\n");
  printf("MSSV: 20214105\n");

  sema_init(&sema, 0);
  thread_create("thread1", PRI_DEFAULT, thread_func1, NULL);
  thread_create("thread2", PRI_DEFAULT, thread_func2, NULL);
}