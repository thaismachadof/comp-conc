#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "timer.h"

#define RANDOM (int)rand()/(int) (RAND_MAX/999999)

int s = 0; //variavel compartilhada entre as threads
pthread_mutex_t mutex; //variavel de lock para exclusao mutua
int n_threads = 0;
long long int dim, id_Global = 0;

int *vetorEntrada;
double *v_saida_conc, *v_saida_seq;
double ini, fim;

//funcao que descobre se o numero é primo
int ehPrimo(int n) {
  if (n<=1 || n%2==0) return 0;
  if (n==2) return 1;
  // if (n%2==0) return 0;
  for (int i=3; i < sqrt(n) +1; i+=2)
    if(n%i==0)
      return 0;
  return 1;
}

//funcao sequencial
void processaPrimos() {
  for(int i=0; i<dim; i++)
    v_saida_seq[i] = (ehPrimo(vetorEntrada[i]) ? sqrt(vetorEntrada[i]) : vetorEntrada[i]);
}

//funcao executada pelas threads
void *ExecutaTarefa (void * arg) {
    int indice_Local;

    pthread_mutex_lock(&mutex);
    indice_Local = id_Global;
    id_Global++;
    pthread_mutex_unlock(&mutex);

    while(id_Global < dim+1){
        if(ehPrimo(vetorEntrada[indice_Local]))
            v_saida_conc[indice_Local] = sqrt(vetorEntrada[indice_Local]);
        else
            v_saida_conc[indice_Local] = vetorEntrada[indice_Local];

        pthread_mutex_lock(&mutex);
        indice_Local = id_Global;
        id_Global++;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {


  //recebe e valida os parametros de entrada (dimensao do vetorEntrada, numero de threads)
   if(argc < 3) {
       fprintf(stderr, "Digite: %s <dimensao do vetorEntrada> <numero threads>\n", argv[0]);
       return 1;
   }
   dim = atoll(argv[1]);
   n_threads = atoi(argv[2]);

   pthread_t *tid; // Ids usados pelo sistema operacional

   //alocando os vetores
   vetorEntrada = (int*) malloc(sizeof(int)*dim);
   if(vetorEntrada == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }

   v_saida_seq = (double*) malloc(sizeof(double)*dim);
   if(v_saida_seq == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }

   v_saida_conc = (double*) malloc(sizeof(double)*dim);
   if(v_saida_conc == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   tid = (pthread_t *) malloc(sizeof(pthread_t)*dim);
   if(tid == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   
   //preenche o vetor de entrada
   for(long int i=0; i<dim; i++)
      vetorEntrada[i] = RANDOM;

  //execucao sequencial
  GET_TIME(ini);
  processaPrimos();
  GET_TIME(fim);
  printf("Tempo sequencial = %lf\n", fim-ini);

  GET_TIME(ini);
  
  //--inicilaiza o mutex (lock de exclusao mutua)
  pthread_mutex_init(&mutex, NULL);

  for(int t=0; t<n_threads; t++) {
    if (pthread_create((tid + t), NULL, ExecutaTarefa, NULL)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }
  //--espera todas as threads terminarem
  for (int t=0; t<n_threads; t++) {
    if (pthread_join(*(tid + t), NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1);
    }
  }
  GET_TIME(fim);
  printf("Tempo concorrente = %lf\n", fim-ini);

  for(long long int i = 0; i < dim; i++){
        if(v_saida_seq[i] != v_saida_conc[i])
            printf("ERRO!");
    }

  pthread_mutex_destroy(&mutex);
  free(vetorEntrada);
  free(v_saida_conc);
  free(v_saida_seq);
}
