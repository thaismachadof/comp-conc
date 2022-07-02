#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define NTHREADS 5

/* Variaveis globais */
int x = 0; //controle das threads já executadas
sem_t condt5, condt1, condt234;     //semaforos para sincronizar a ordem de execucao das threads

/* Thread 1 */
void *T1 () {
  sem_wait(&condt1);

  printf("Volte sempre! \n");

  pthread_exit(NULL);
}

/* Thread 5 */
void *T5 () {
  printf("Seja bem-vindo!\n");
  x++;
  sem_post(&condt234);
  sem_post(&condt234);
  sem_post(&condt234);

  pthread_exit(NULL);
}

/* Threads 2, 3 e 4 */
void *T234 (void *p) {
  sem_wait(&condt234);

  char* frase = (char*)p;
  
  puts(frase);
  x++;
  
  if(x == 4) // as 4 primeiras threads já foram finalizadas? 
    sem_post(&condt1);

  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];
  
  //inicia os semaforos
  sem_init(&condt5, 0, 0); //semáforo da thread 5
  sem_init(&condt1, 0, 0); //semáforo da thread 1
  sem_init(&condt234, 0, 0); //semáforo das threads 2, 3 e 4

  /* Cria as threads */
  pthread_create(&threads[0], NULL, T1, NULL);
  pthread_create(&threads[1], NULL, T234, (void *)"Fique a vontade");
  pthread_create(&threads[2], NULL, T234, (void *)"Sente-se por favor");
  pthread_create(&threads[3], NULL, T234, (void *)"Aceita um copo d’agua?");
  pthread_create(&threads[4], NULL, T5, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  
  sem_destroy(&condt5);
  sem_destroy(&condt1);
  sem_destroy(&condt234);

  pthread_exit(NULL);
}
