#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_LEITORAS 4
#define NUM_ESCRITORAS 2

// Globais
sem_t em_e, em_l, escr, leit; //semaforos
int e=0, l=0; //globais

void * leitores(void *args){
    int id = *(int *) args;

    while(1){
        printf("thread %d -- Quero ler!!\n", id);

        sem_wait(&leit);

        sem_wait(&em_l); // inicio de exclusao mutua
        l++;

        if(l == 1){ // se tem alguem lendo
            sem_wait(&escr); // bloqueia escrita
        }
        sem_post(&em_l);
        sem_post(&leit);

        printf("thread %d -- Lendo...\n", id);
        sem_wait(&em_l);

        l--;
        if(l == 0) {
            sem_post(&escr);
        }

        sem_post(&em_l);
    }
	printf("thread %d -- Terminei!\n", id);

    pthread_exit(NULL);
}

void * escritores(void *args){
    int id = *(int *) args;
    while(1){
        printf("thread %d -- Quero escrever!!\n", id);
        sem_wait(&em_e);
        e++;
        if(e == 1) { // se tem alguem escrevendo
            sem_wait(&leit); // bloqueia leitura
        }
        sem_post(&em_e);
        sem_wait(&escr);
        printf("thread %d -- Escrevendo...\n", id);
        sem_post(&escr);
        sem_wait(&em_e);
        e--;
        if(e == 0) {
            sem_post(&leit);
        }
        sem_post(&em_e);
		printf("thread %d -- Terminei!\n", id);
    }
    pthread_exit(NULL);

}

int main(){
    pthread_t identificadores[NUM_ESCRITORAS + NUM_LEITORAS];
    int *id = (int *) malloc(sizeof(int) * NUM_ESCRITORAS + NUM_LEITORAS);

    sem_init(&em_e, 0, 1);
    sem_init(&em_l, 0, 1);
    sem_init(&escr, 0, 1);
    sem_init(&leit, 0, 1);
    
    for(int i = 0; i < NUM_LEITORAS; i++){ // iniciando threads leitoras
        id[i] = i; 
        pthread_create(&identificadores[i],NULL, leitores, (void *) &id[i] );
    }

    for(int i = NUM_LEITORAS; i < NUM_LEITORAS + NUM_ESCRITORAS; i++){ // iniciando threads escritoras
        id[i] = i;
        pthread_create(&identificadores[i],NULL, escritores, (void *) &id[i] );
    }

    for(int i = 0; i < NUM_ESCRITORAS+NUM_LEITORAS; i++){
        pthread_join(identificadores[i],NULL);
    }
    
    sem_destroy(&em_e);
    sem_destroy(&em_l);
    sem_destroy(&escr);
    sem_destroy(&leit);

    pthread_exit(NULL);
    return 0;
}
