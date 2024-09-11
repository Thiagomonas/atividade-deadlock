#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

pthread_t threadA, threadB, threadC;
sem_t recA, recB;

void *processoA(void  *args){
    sem_wait(&recA);
    printf("thread A usando recurso A\n");

    // Algum processamento
    sleep(1);
    
    printf("thread A esperando recurso B\n");
    sem_wait(&recB);
    printf("thread A usando recurso B\n");

    // Algum processamento
    sleep(1);

    sem_post(&recA);
    printf("thread A liberando recurso A\n");

    sem_post(&recB);
    printf("thread A liberando recurso B\n");
}

void *processoB(void  *args){
    sem_wait(&recB);
    printf("thread B usando recurso B\n");

    // Algum processamento
    sleep(1);
    
    printf("thread B esperando recurso A\n");
    sem_wait(&recA);
    printf("thread B usando recurso A\n");

    // Algum processamento
    sleep(1);

    sem_post(&recB);
    printf("thread B liberando recurso B\n");

    sem_post(&recA);
    printf("thread B liberando recurso A\n");
}

void tratar_deadlock(){
    // Eliminação da thread A para liberar seu recurso
    pthread_cancel(threadA);
    sem_post(&recA);
    pthread_join(threadB, NULL);
    // Recriação da thread A após o término de B
    pthread_create(&threadA, NULL, processoA, NULL);
}

void *checar_deadlock(void *args){
    int valorA, valorB;
    while (1){
        sem_getvalue(&recA, &valorA);
        sem_getvalue(&recB, &valorB);

        if (valorA == 0 && valorB == 0){
            printf("Deadlock encontrada\n");
            tratar_deadlock();
            break;
        }
    }
}

int main(){
    sem_init(&recA, 0, 1);
    sem_init(&recB, 0, 1);
    
    pthread_create(&threadA, NULL, processoA, NULL);
    pthread_create(&threadB, NULL, processoB, NULL);
    pthread_create(&threadC, NULL, checar_deadlock, NULL);
    
    sem_destroy(&recA);
    sem_destroy(&recB);

    pthread_exit(0);

    return 0;
}