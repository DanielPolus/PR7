#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
int main(){
    int shmid;
    key_t key;
    int *shared_memory;
    int n, i;
    sem_t *semaphore;

    printf("How many elements: ");
    scanf("%d", &n);

    key = ftok("shmfile", 65);
    shmid = shmget(key, n * sizeof(int), IPC_CREAT | 0666);
    printf("shmid: %d\n", shmid);
    shared_memory = (int *)shmat(shmid, NULL, 0);

    printf("Enter your numbers:\n");
    shared_memory[0] = n;
    for(i = 1; i < n + 1; i++) {
        scanf("%d", &shared_memory[i]);
    }

    sem_unlink("/sem");
    semaphore = sem_open("/sem", O_CREAT | O_EXCL, 0666, 0);
    if(semaphore == SEM_FAILED) {
        perror("semaphore error");
        return -1;
    }
    
    sem_wait(semaphore);

    printf("Sum: %d\n", shared_memory[0]);
    shmdt((void *)shared_memory);
    sem_close(semaphore);

    return 0;
}
