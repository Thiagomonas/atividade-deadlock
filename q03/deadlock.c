#include <stdio.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>

#define NUM_RECURSOS_NECESSARIOS 2
#define NUM_RECURSOS 2

int recursos_disponiveis = NUM_RECURSOS;


pthread_t threadA, threadB;
pthread_mutex_t recA, recB;

void *processoA(void  *args){
    printf("thread A checando se há recursos suficientes\n");
    if (recursos_disponiveis == NUM_RECURSOS_NECESSARIOS){
        recursos_disponiveis -= 1;
        pthread_mutex_lock(&recA);
        printf("thread A usando recurso A\n");
        
        // Algum processamento
        sleep(1);
    }
    else {
        printf("Quantidade de recursos insuficiente!\n" 
        "Thread A esperando liberar recursos necessários\n");
        pthread_join(threadB, NULL);

        pthread_mutex_lock(&recA);
        printf("thread A usando recurso A\n");
    }
    
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
    printf("thread B checando se há recursos suficientes\n");
    if (recursos_disponiveis == NUM_RECURSOS_NECESSARIOS){
        recursos_disponiveis -= 1;
        pthread_mutex_lock(&recB);
        printf("thread B usando recurso B\n");

        // Algum processamento
        sleep(1);
    }
    else {
        printf("Quantidade de recursos insuficiente!\n" 
        "Thread B esperando liberar recursos necessários\n");
        pthread_join(threadA, NULL);

        pthread_mutex_lock(&recB);
        printf("thread B usando recurso B\n");
    }
    
    pthread_mutex_lock(&recA);
    printf("thread B usando recurso A\n");
        
    // Algum processamento
    sleep(1);

    pthread_mutex_unlock(&recB);
    printf("thread B liberou recurso B\n");

    pthread_mutex_unlock(&recA);
    printf("thread B liberou recurso A\n");
}

int main(){
    pthread_mutex_init(&recA, NULL);
    pthread_mutex_init(&recB, NULL);
    
    pthread_create(&threadA, NULL, processoA, NULL);
    pthread_create(&threadB, NULL, processoB, NULL);

    pthread_exit(0);

    return 0;
}