#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

sem_t recA, recB;

void *processoA(void  *args){
    sem_wait(&recA);
    printf("thread A usando recurso A\n");

    // Algum processamento
    //sleep(1);
    
    printf("thread A esperando recurso B\n");
    sem_wait(&recB);
    printf("thread A usando recurso B\n");

    // Algum processamento
    //sleep(1);

    sem_post(&recA);
    printf("thread A liberando recurso A\n");

    sem_post(&recB);
    printf("thread A liberando recurso B\n");
}

void *processoB(void  *args){
    sem_wait(&recB);
    printf("thread B usando recurso B\n");

    // Algum processamento
    //sleep(1);
    
    printf("thread B esperando recurso A\n");
    sem_wait(&recA);
    printf("thread B usando recurso A\n");

    // Algum processamento
    //sleep(1);

    sem_post(&recB);
    printf("thread B liberando recurso B\n");

    sem_post(&recA);
    printf("thread B liberando recurso A\n");
}

int main(){
    pthread_t threadA, threadB;
    sem_init(&recA, 0, 1);
    sem_init(&recB, 0, 1);
    
    pthread_create(&threadA, NULL, processoA, NULL);
    pthread_create(&threadB, NULL, processoB, NULL);
    sem_destroy(&recA);
    sem_destroy(&recB);

    return 0;
}