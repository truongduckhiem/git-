#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/interrupt.h"
#include "devices/timer.h"
#include <stdio.h>

static struct semaphore sema;

// Thread 1: Chờ nhận tín hiệu từ semaphore
void thread_1(void *sema_) {
    // Chờ semaphore
    sema_down(&sema);
    printf("Thread 1: Acquired semaphore!\n");
}

// Thread 2: Phát tín hiệu sau một khoảng thời gian
void thread_2(void *sema_) {
    // In thông báo trước khi phát tín hiệu
    printf("Thread 2: Up semaphore!\n");

    // Đợi 100 ticks (chờ một thời gian trước khi phát tín hiệu)
    timer_sleep(100);

    // Phát tín hiệu để đánh thức thread 1
    sema_up(&sema);
}

// Test case chính
void sema_sync_test(void) {
    // Khởi tạo semaphore
    semaphore_init(&sema, 0);

    // Tạo thread 1 và thread 2
    thread_create("Thread 1", PRI_DEFAULT, thread_1, &sema);
    thread_create("Thread 2", PRI_DEFAULT, thread_2, &sema);
}