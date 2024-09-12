#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

pthread_t threadA, threadB;
pthread_mutex_t recA, recB;

void *processoA(void  *args){
    pthread_mutex_lock(&recA);
    printf("thread A usando recurso A\n");

    sleep(1);
    
    printf("thread A esperando recurso B\n");
    pthread_mutex_lock(&recB);
    printf("thread A usando recurso B\n");

    // Algum processamento
    sleep(1);

    pthread_mutex_unlock(&recA);
    printf("thread A liberou recurso A\n");

    pthread_mutex_unlock(&recB);
    printf("thread A liberou recurso B\n");
}

void *processoB(void  *args){
    pthread_mutex_lock(&recB);
    printf("thread B usando recurso B\n");

    // Algum processamento
    sleep(1);
    
    printf("thread B esperando recurso A\n");
    pthread_mutex_lock(&recA);
    printf("thread B usando recurso A\n");

    // Algum processamento
    sleep(1);

    pthread_mutex_unlock(&recB);
    printf("thread B liberando recurso B\n");

    pthread_mutex_unlock(&recA);
    printf("thread B liberando recurso A\n");
}

int main(){
    pthread_mutex_init(&recA, NULL);
    pthread_mutex_init(&recB, NULL);
    
    pthread_create(&threadA, NULL, processoA, NULL);
    pthread_create(&threadB, NULL, processoB, NULL);

    pthread_exit(0);

    return 0;
}