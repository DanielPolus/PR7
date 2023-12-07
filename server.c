#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <errno.h>
int main(int argc, char *argv[]){
    int shmid;
    key_t key;
    int *shared_memory;
    int n, i, sum = 0;
    sem_t *semaphore;
    if(argc < 2){
        printf("Where is ID? I need the SHMID\n");
        return -1;
    }
    sscanf(argv[1], "%d", &shmid);
    shared_memory = (int *)shmat(shmid, NULL, 0);
    if(shared_memory == (int *)(-1)){
        perror("shared memory error");
        return -1;
    }
    semaphore = sem_open("/sem", 1);
    if(semaphore == SEM_FAILED){
        perror("semaphore open error");
        return -1;
    }
    for(i = 1; i < shared_memory[0] + 1; i++){
        sum += shared_memory[i];
    }

    printf("Sum has been calculated\n");

    shared_memory[0] = sum;
    sem_post(semaphore);
    shmdt((void *)shared_memory);
    sem_close(semaphore);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
