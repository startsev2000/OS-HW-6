#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <unistd.h>

#define SHM_KEY 1234 // Shared memory key
#define SEG_SIZE 1024 // Shared memory segment size

int shm_id, *data;

void clean_memory(int signal) {
    if (shmdt(data) == -1) {
        perror("shmdt error");
        exit(1);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl error");
        exit(1);
    }
    exit(0);
}

int main() {
    signal(SIGINT, clean_memory);

    if ((shm_id = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | 0666)) == -1) {
        perror("shmget error");
        exit(1);
    }

    if ((data = shmat(shm_id, NULL, 0)) == (int *) -1) {
        perror("shmat error");
        exit(1);
    }

    while (1) {
        printf("Received: %d\n", *data);
        if (*data == -1) {
            printf("Got -1. Terminating");
            break;
        }
        data++;
        sleep(1);
    }

    printf("Server finished\n");

    return 0;
}
