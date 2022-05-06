#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

#define RANDON (float)rand()/(float) (RAND_MAX/999)

long long int dim; //dimensao do vetor de entrada
int nthreads; //numero de threads
float *vetor; //vetor de entrada com dimensao dim  

typedef struct {
    float maior;
    float menor;
} tArgs;

//fluxo das threads
void * tarefa(void *args) {
   long int id = (long int) args; //identificador da thread
   long int tamBloco = dim/nthreads;  //tamanho do bloco de cada thread 
   long int ini = id * tamBloco; //elemento inicial do bloco da thread
   long int fim; //elemento final(nao processado) do bloco da thread
   if(id == nthreads-1) fim = dim;
   else fim = ini + tamBloco; //trata o resto se houver

   tArgs *arg = (tArgs*) args;
   arg = malloc(sizeof(tArgs));
   arg->maior = vetor[ini]; //inicializa o maior valor
   arg->menor = vetor[ini]; //inicializa o menor valor

   //acha os maiores e menores elementos do bloco da thread
   for(long int i=ini; i<fim; i++) {
      if(vetor[i] > arg->maior)
        arg->maior = vetor[i];
      if(vetor[i] < arg->menor)
        arg->menor = vetor[i];
   }

   //retorna o resultado do processamento local
   pthread_exit((void *) arg);
}

//fluxo principal
int main(int argc, char *argv[]) {
   double ini, fim; //tomada de tempo
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *retorno; //valor de retorno das threads
   float maiorSeq, menorSeq;
   float maiorConc, menorConc;

   //recebe e valida os parametros de entrada (dimensao do vetor, numero de threads)
   if(argc < 3) {
       fprintf(stderr, "Digite: %s <dimensao do vetor> <numero threads>\n", argv[0]);
       return 1; 
   }
   dim = atoll(argv[1]);
   nthreads = atoi(argv[2]);
   //aloca o vetor de entrada
   vetor = (float*) malloc(sizeof(float)*dim);
   if(vetor == NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }
   //preenche o vetor de entrada
   for(long int i=0; i<dim; i++)
      vetor[i] = RANDON;
  
   //inicialização dos valores
   maiorSeq = vetor[0];
   menorSeq = vetor[0];
   maiorConc = vetor[0];
   menorConc = vetor[0];

   //cálculo sequencial dos elementos
   GET_TIME(ini);
   for(long int i=0; i<dim; i++){
      if(vetor[i] > maiorSeq)
        maiorSeq = vetor[i];
      if(vetor[i] < menorSeq)
        menorSeq = vetor[i];
   } 
   GET_TIME(fim);
   printf("Tempo sequencial:  %lf\n", fim-ini);

   ini = 0;
   fim = 0;

   //cálculo concorrente dos elementos
   GET_TIME(ini);
   tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
   if(tid==NULL) {
      fprintf(stderr, "ERRO--malloc\n");
      return 2;
   }

   //criar as threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
   }
   //aguardar o termino das threads
   for(long int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), (void**) &retorno)){
         fprintf(stderr, "ERRO--pthread_create\n");
         return 3;
      }
      if(retorno->maior > maiorConc)
        maiorConc = retorno->maior;
      if(retorno->menor < menorConc)
        menorConc = retorno->menor;
   }
   GET_TIME(fim);
   printf("Tempo concorrente:  %lf\n", fim-ini);

   //exibir os resultados
   printf("Menor seq:  %.12lf\n", menorSeq);
   printf("Menor conc: %.12lf\n", menorConc);
   printf("-------------------------------\n");
   printf("Maior seq:  %.12lf\n", maiorSeq);
   printf("Maior conc: %.12lf\n", maiorConc);

   //libera as areas de memoria alocadas
   free(vetor);
   free(tid);

   return 0;
}
