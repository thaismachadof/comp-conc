#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS 2 //total de threads a serem criadas
#define n 10000

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, nThreads, *vetor;
} t_Args;

//funcao executada pelas threads
void *elevarNumeros (void *arg) {
  t_Args *args = (t_Args *) arg;

  if(args->idThread == 0)
    {
        for(int i = 0; i<n/NTHREADS; i++){
            args->vetor[i] = i*i;
        }
    }
  else
    {
        for(int i = n/NTHREADS; i<n; i++){
            args->vetor[i] = i*i;
        }
    }

  free(arg); //aqui pode liberar a alocacao feita na main

  pthread_exit(NULL);
}

//funcao principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  t_Args *arg; //receberá os argumentos para a thread

  int vetor[10000]; // vetor principal

  for(thread=0; thread<NTHREADS; thread++) {
    printf("--Aloca e preenche argumentos para thread %d\n", thread);
    arg = malloc(sizeof(t_Args));
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }
    arg->idThread = thread; 
    arg->nThreads = NTHREADS; 
    arg->vetor = vetor;
    
    printf("--Cria a thread %d\n", thread);
    if (pthread_create(&tid_sistema[thread], NULL, elevarNumeros, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  //--espera todas as threads terminarem
  for (thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }

  // verifica se os valores estão corretos
  for(int i = 0; i < n; i++)
  {
    if(arg->vetor[i] != i*i)
        printf("ERRO!");
  }

  return 0;
}