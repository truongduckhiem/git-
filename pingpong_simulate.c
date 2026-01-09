#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>  // mmap
#include <sys/wait.h>  // wait

typedef struct {
    int value;
} semaphore_t;

void sema_down(semaphore_t* sem) {
    while (1) {
        if (sem->value > 0) {
            sem->value--;
            break;
        }
        usleep(1000); // Chờ 1ms rồi thử lại (giả lập wait)
    }
}

void sema_up(semaphore_t* sem) {
    sem->value++;
}

void ping_thread(semaphore_t* sema_ping, semaphore_t* sema_pong) {
    for (int i = 0; i < 5; i++) {
        sema_down(sema_ping);
        printf("ping ");
        fflush(stdout);
        usleep(100000);
        sema_up(sema_pong);
    }
}

void pong_thread(semaphore_t* sema_ping, semaphore_t* sema_pong) {
    for (int i = 0; i < 5; i++) {
        sema_down(sema_pong);
        printf("pong ");
        fflush(stdout);
        usleep(100000);
        sema_up(sema_ping);
    }
}

int main() {
    // Dùng mmap để tạo semaphore dùng chung giữa process
    semaphore_t* sema_ping = mmap(NULL, sizeof(semaphore_t), 
                                   PROT_READ | PROT_WRITE, 
                                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    semaphore_t* sema_pong = mmap(NULL, sizeof(semaphore_t), 
                                   PROT_READ | PROT_WRITE, 
                                   MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    sema_ping->value = 1;  // Ping bắt đầu trước
    sema_pong->value = 0;  // Pong chờ sau

    pid_t pid1 = fork();
    if (pid1 == 0) {  // Process con 1: ping
        ping_thread(sema_ping, sema_pong);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {  // Process con 2: pong
        pong_thread(sema_ping, sema_pong);
        exit(0);
    }

    // Process cha chờ 2 process con kết thúc
    wait(NULL);
    wait(NULL);

    // Giải phóng mmap
    munmap(sema_ping, sizeof(semaphore_t));
    munmap(sema_pong, sizeof(semaphore_t));

    printf("\n");
    return 0;
}