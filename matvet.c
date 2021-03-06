/* Multiplicacao de matriz-matriz (considerando matrizes quadradas) */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include "timer.h"

float *mat; //matriz de entrada
float *mat2; //matriz2 de entrada
float *saidaConcorrente; //matriz de saidaConcorrente
float *saidaSequencial; //matriz de saidaConcorrente
int nthreads; //numero de threads

typedef struct{
   int id; //identificador do elemento que a thread ira processar
   int dim; //dimensao das estruturas de entrada
} tArgs;

//funcao que as threads executarao
void * tarefa(void *arg) {
   tArgs *args = (tArgs*) arg;
   //printf("Thread %d\n", args->id);
   for(int i=args->id; i<args->dim; i+=nthreads) // anda pela linha
      for(int j=0; j<args->dim; j++) // anda pela coluna
        for(int k=0; k<args->dim; k++) // anda pelas células
            saidaConcorrente[i*(args->dim) + j] += mat[i*(args->dim) + k] * mat2[k*(args->dim) + j];
   pthread_exit(NULL);
}

void * multiplicacaoSequencial(int dim){
    for(int i=0; i<dim; i++) // anda pela linha
      for(int j=0; j<dim; j++) // anda pela coluna
        for(int k=0; k<dim; k++) // anda pelas células
            saidaSequencial[i*(dim) + j] += mat[i*(dim) + k] * mat2[k*(dim) + j];
}

//fluxo principal
int main(int argc, char* argv[]) {
   int dim; //dimensao da matriz de entrada
   pthread_t *tid; //identificadores das threads no sistema
   tArgs *args; //identificadores locais das threads e dimensao
   double inicio, fim, delta;
   
   //leitura e avaliacao dos parametros de entrada
   if(argc<3) {
      printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
      return 1;
   }
   dim = atoi(argv[1]);
   nthreads = atoi(argv[2]);
   if (nthreads > dim) nthreads=dim;

   //alocacao de memoria para as estruturas de dados
   mat = (float *) malloc(sizeof(float) * dim * dim);
   if (mat == NULL) {printf("ERRO--malloc\n"); return 2;}
   mat2 = (float *) malloc(sizeof(float) * dim * dim);
   if (mat2 == NULL) {printf("ERRO--malloc\n"); return 2;}
   saidaConcorrente = (float *) malloc(sizeof(float) * dim * dim);
   if (saidaConcorrente == NULL) {printf("ERRO--malloc\n"); return 2;}
   saidaSequencial = (float *) malloc(sizeof(float) * dim * dim);
   if (saidaSequencial == NULL) {printf("ERRO--malloc\n"); return 2;}

   //inicializacao das estruturas de dados de entrada e saidaConcorrente
   for(int i=0; i<dim; i++) {
      for(int j=0; j<dim; j++){
         mat[i*dim+j] = 1;    //equivalente mat[i][j]
         mat2[i*dim+j] = 1; 
         saidaConcorrente[i*dim+j] = 0;
         saidaSequencial[i*dim+j] = 0;
      }
   }

   //multiplicacao da matriz de forma concorrente
   GET_TIME(inicio);
   //alocacao das estruturas
   tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
   if(tid==NULL) {puts("ERRO--malloc"); return 2;}
   args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
   if(args==NULL) {puts("ERRO--malloc"); return 2;}
   //criacao das threads
   for(int i=0; i<nthreads; i++) {
      (args+i)->id = i;
      (args+i)->dim = dim;
      if(pthread_create(tid+i, NULL, tarefa, (void*) (args+i))){
         puts("ERRO--pthread_create"); return 3;
      }
   } 
   //espera pelo termino da threads
   for(int i=0; i<nthreads; i++) {
      pthread_join(*(tid+i), NULL);
   }
   GET_TIME(fim)   
   delta = fim - inicio;
   printf("Tempo multiplicacao concorrente (dimensao %d) (nthreads %d): %lf\n", dim, nthreads, delta);

   double inicio2, fim2, delta2 = 0;

   // execução sequencial
   GET_TIME(inicio2);
   multiplicacaoSequencial(dim);
   GET_TIME(fim2);
   delta2 = fim2 - inicio2;

   printf("Tempo multiplicacao sequencial(dimensao %d): %lf\n", dim, delta2);
   
   // comparação de saídas
   for(int i=0; i < dim; i++)
        for(int j=0; j < dim; j++)
            if(saidaConcorrente[i * dim + j] != saidaSequencial[i * dim + j])
                printf("ERRO");

   //liberacao da memoria
   free(mat);
   free(mat2);
   free(saidaConcorrente);
   free(args);
   free(tid);

   return 0;
}