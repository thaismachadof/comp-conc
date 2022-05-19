#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS 5

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread 1 */
void *T1 () {
  pthread_mutex_lock(&x_mutex);
  while(x < 4) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  x++;
  pthread_mutex_unlock(&x_mutex);

  printf("Volte sempre! \n");

  pthread_exit(NULL);
}

/* Thread 5 */
void *T5 () {
  printf("Seja bem-vindo!\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  pthread_cond_broadcast(&x_cond);
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Threads 2, 3 e 4 */
void *T234 (void *p) {
  char* frase = (char*)p;

  pthread_mutex_lock(&x_mutex);
  while(x < 1) {
    pthread_cond_wait(&x_cond, &x_mutex);
  }
  x++;
  puts(frase);
  if(x == 4)
    pthread_cond_signal(&x_cond);
  pthread_mutex_unlock(&x_mutex);
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

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

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
}
