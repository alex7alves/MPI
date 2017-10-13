
/*
   Autor : Alex Alves

   Programa para calcular um produto escalar entre
   um vetor e um vetor multiplicado com um escalar

*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h> 
#include <mpi.h> 
#include <string.h>
void inicio(int* tamanho,int* local_n,int total,int rank,int* escalar);

void iniciar_vetores(int* tamanho,int* local_n,int rank
      ,double *vetor1,double *vetor2,double *vetor_local1,double *vetor_local2); 


int main(void) {
   int rank , total;
   MPI_Status stat;
   int *tamanho; // tamanho do vetor
   int *local_n;  // tamanho do vetor dividido
   int *escalar,i;
   double *vetor1,*vetor2, *soma_local,*vetor_escalar,*vetor_final;
   double *vetor_local1,*vetor_local2,*resultado;
   // alocando memoria
   tamanho= malloc(sizeof(int));
   escalar= malloc(sizeof(int));
   local_n= malloc(sizeof(int));   

   MPI_Init(NULL, NULL); 
   MPI_Comm_size(MPI_COMM_WORLD, &total); 
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
      
      inicio(tamanho,local_n,total,rank,escalar);
      // Alocando memoria
      vetor1 = malloc(*tamanho * (sizeof(double)));
   	  vetor2 = malloc(*tamanho * (sizeof(double)));
   	  vetor_local1 = malloc(*local_n*sizeof(double));
   	  vetor_local2 = malloc(*local_n*sizeof(double));
   	  vetor_escalar =  malloc(*local_n*sizeof(double));
   	  vetor_final = malloc(*tamanho * (sizeof(double)));
      
      // Inicia os vetores e separa para cada processo
      iniciar_vetores(tamanho,local_n,rank,vetor1,vetor2,vetor_local1,vetor_local2);
    
   		
   	  soma_local = malloc(sizeof(double));
      resultado= malloc(sizeof(double));
     // calculando as somas locais
      for(i=0;i<*local_n;i++){
	      vetor_escalar[i] = vetor_local1[i] *(*escalar);
	      vetor_local1[i] =vetor_escalar[i]*vetor_local2[i];
	      *soma_local = (*soma_local) + vetor_local1[i];
	   
	   }
   		//Adiciona as somas locais no resultado
   		MPI_Reduce(soma_local, resultado, 1, MPI_DOUBLE, MPI_SUM, 0,
       		 MPI_COMM_WORLD);

   		//Pegando as partes para formar o vetor final
		if (rank == 0) {
			MPI_Gather(vetor_escalar, *local_n, MPI_DOUBLE, 
				vetor_final, *local_n, MPI_DOUBLE, 0,  MPI_COMM_WORLD);
			
		} else {
			MPI_Gather(vetor_escalar, *local_n, MPI_DOUBLE, 
				vetor_final, *local_n, MPI_DOUBLE, 0,  MPI_COMM_WORLD);
		}
		// Mostrando os resultados
	   if(rank == 0){
	      printf("\nO resultado do produto escalar : %lf\n",*resultado);
	      printf("\nO resultado do vetor pelo escalar : \n");
	      for(i=0;i<*tamanho;i++){
	      		printf("%lf ",vetor_final[i]);
	      }
	      printf("\n\n");
	      
	    }
	    // Desalocando memoria
	    free(vetor1);
	    free(vetor2);
	    free(vetor_local1);
	    free(vetor_local2);
	    free(vetor_escalar);
	    free(vetor_final);
   MPI_Finalize();
    
  
   return 0;
} 


void inicio(int* tamanho,int* local_n,int total,int rank,int* escalar){
   
      if(rank==0){
         printf(" Entre com o tamanho do vetor\n");
         scanf("%d",tamanho);
         printf(" Entre com o valor do escalar\n");
         scanf("%d",escalar);
         *local_n = *tamanho/total;
         } 
         // Envia local_n, tamanho e escalar para todos os processos
          MPI_Bcast(local_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
          MPI_Bcast(tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);
          MPI_Bcast(escalar, 1, MPI_INT, 0, MPI_COMM_WORLD);
}

void iniciar_vetores(int* tamanho,int* local_n,int rank
      ,double *vetor1,double *vetor2,double *vetor_local1,double *vetor_local2){
   
   int i;
 
   if(rank==0){
      
         printf("Digite o primeiro vetor\n");
         for(i=0;i<*tamanho;i++){
            scanf("%lf",&vetor1[i]);
         }
         MPI_Scatter(vetor1,*local_n, MPI_DOUBLE, 
               vetor_local1,*local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
         printf("Digite o segundo vetor\n");
         for(i=0;i<*tamanho;i++){ 
            scanf("%lf",&vetor2[i]);
         }
         MPI_Scatter(vetor2,*local_n, MPI_DOUBLE, 
               vetor_local2,*local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   }else {
   	  
   		
        MPI_Scatter(vetor1,*local_n, MPI_DOUBLE, 
               vetor_local1,*local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
         MPI_Scatter(vetor2,*local_n, MPI_DOUBLE, 
               vetor_local2,*local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   
   	}
}
