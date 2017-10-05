
/* Autor : Alex Alves
   Programa para rodar um mensagem em paralelo
   com varios processos
*/
#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */ 

// o size eh decidido na hora de executar o programa
int main(void) {
   int        size; // quantidade maxima de processos
   int        rank;  // numero do  processo atual

   // Inicia o MPI
   MPI_Init(NULL, NULL); 
   // Pega o numero total de processos
   MPI_Comm_size(MPI_COMM_WORLD, &size); 
   // pega o numero do processo atual
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
   printf("Ola ! Eu sou o processo %d de um total de %d\n",rank,size);
   //finalisa o MPI
   MPI_Finalize(); 
   return 0;
} 