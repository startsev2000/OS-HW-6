#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>

#define SHM_KEY 1234
#define SEG_SIZE 1024
#define NUMBER_AMOUNT 15

int shm_id, *data;

void clean_memory(int signal) {
    if (shmdt((void *) data) == -1) {
        perror("shmdt error");
        exit(1);
    }
    exit(0);
}

int main() {
    signal(SIGINT, clean_memory);
    srand(time(NULL));

    if ((shm_id = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | 0666)) == -1) {
        perror("shmget error");
        exit(1);
    }

    if ((data = shmat(shm_id, NULL, 0)) == (int *) -1) {
        perror("shmat error");
        exit(1);
    }

    for (int i = 0; i < NUMBER_AMOUNT; i++) {
        int rand_num = rand() % 101;
        *data = rand_num;
        printf("Sent: %d\n", rand_num);
        data++;
        sleep(1);
        if (i == NUMBER_AMOUNT - 1) {
            *data = -1;
        }
    }

    printf("Client finished\n");

    return 0;
}
