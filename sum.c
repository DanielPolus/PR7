#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>

#define SHM_SIZE 1024

int shmid;
int *shared_memory;

void sigusr1_handler(int signo) {
    int num_data = shared_memory[1];
    int sum = 0;
    for (int i = 2; i < num_data + 2; i++) {
        sum += shared_memory[i];
    }
    shared_memory[0] = sum;
    kill(getppid(), SIGUSR2);
}

void sigterm_handler(int signo) {
    printf("Child SIGTERM. Exiting\n");
    shmdt(shared_memory);
    exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <shared_memory_key>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    key_t key = atoi(argv[1]);
    shmid = shmget(key, SHM_SIZE, 0666);
    shared_memory = (int *)shmat(shmid, NULL, 0);

    signal(SIGUSR1, sigusr1_handler);

    while (1) {
        pause();
    }

    return 0;
}
