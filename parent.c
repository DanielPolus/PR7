#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define SHM_SIZE 1024

int shmid;
int *shared_memory;

void sigint_handler(int signo) {
    printf("Parent SIGINT. Cleaning up + exit\n");

    kill(shared_memory[0], SIGTERM);

    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);

    exit(0);
}

void sigusr2_handler(int signo) {
    printf("Sum: %d\n", shared_memory[0]);

    shmdt(shared_memory);
    shmctl(shmid, IPC_RMID, NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <shared_memory_key>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    while (1) {

    key_t key = atoi(argv[1]);
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    shared_memory = (int *)shmat(shmid, NULL, 0);

    signal(SIGINT, sigint_handler);
    signal(SIGUSR2, sigusr2_handler);

    pid_t child_pid;

    if ((child_pid = fork()) == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (child_pid == 0) {
        execlp("./sum", "sum", argv[1], NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {

        int num_data;
        printf("Enter the number of data: ");
        scanf("%d", &num_data);


        shared_memory[1] = num_data;
        printf("Enter %d integers:\n", num_data);
        for (int i = 2; i < num_data + 2; i++) {
            scanf("%d", &shared_memory[i]);
            if (shared_memory[i] == -1) {
                printf("-1 was writen... We must exit\n");
                raise(SIGINT);
                exit(0);
            }
        }

        kill(child_pid, SIGUSR1);

        pause();
    }
    }
    return 0;
}
