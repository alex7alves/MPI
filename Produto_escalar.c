
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
void Soma_locais(int* local_n,double *vetor_local1,int *escalar, double *vetor_local2,double *soma_local,double *resultado,int rank);
int main(void) {
   int rank , total;
   MPI_Status stat;
   int *tamanho; // tamanho do vetor
   int *local_n;  // tamanho do vetor dividido
   int *escalar,i;
   double *vetor1,*vetor2, *soma_local;
   double *vetor_local1,*vetor_local2,*resultado;
   // alocando memoria
   tamanho= malloc(sizeof(int));
   escalar= malloc(sizeof(int));
   local_n= malloc(sizeof(int));   

   MPI_Init(NULL, NULL); 
   MPI_Comm_size(MPI_COMM_WORLD, &total); 
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
      inicio(tamanho,local_n,total,rank,escalar);
      vetor1 = malloc(*tamanho * (sizeof(double)));
   	  vetor2 = malloc(*tamanho * (sizeof(double)));
   	  vetor_local1 = malloc(*local_n*sizeof(double));
   	  vetor_local2 = malloc(*local_n*sizeof(double));
      iniciar_vetores(tamanho,local_n,rank,vetor1,vetor2,vetor_local1,vetor_local2);
     // Soma_locais(local_n,vetor_local1,escalar,vetor_local2,soma_local,resultado,rank);
   		
   	  soma_local = malloc(sizeof(double));
      resultado= malloc(sizeof(double));
   // calculando as somas locais
      for(i=0;i<*local_n;i++){
	      vetor_local1[i] = vetor_local1[i] *(*escalar);
	      vetor_local1[i] =vetor_local1[i]*vetor_local2[i];
	      *soma_local = (*soma_local) + vetor_local1[i];
	   
	   }
   		//Adiciona as somas locais no resultado
   		MPI_Reduce(soma_local, resultado, 1, MPI_DOUBLE, MPI_SUM, 0,
       		 MPI_COMM_WORLD);

	   if(rank == 0){
	      printf("O resultado final: %lf\n",*resultado);
	      free(vetor_local1);
	      free(vetor_local2);
	    }


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
         // Envia local_n para todos os processos
          MPI_Bcast(local_n, 1, MPI_INT, 0, MPI_COMM_WORLD);
          MPI_Bcast(tamanho, 1, MPI_INT, 0, MPI_COMM_WORLD);
          MPI_Bcast(escalar, 1, MPI_INT, 0, MPI_COMM_WORLD);
      
      //*local_n = *tamanho/total;
      
}

void iniciar_vetores(int* tamanho,int* local_n,int rank
      ,double *vetor1,double *vetor2,double *vetor_local1,double *vetor_local2){
   int Passou_rank0= 0;
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
        free(vetor1);
        free(vetor2);
         Passou_rank0= 1;
   }else {
   	  //if(Passou_rank0){
   	  	//local_n=2;
   		
        MPI_Scatter(vetor1,*local_n, MPI_DOUBLE, 
               vetor_local1,*local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
         MPI_Scatter(vetor2,*local_n, MPI_DOUBLE, 
               vetor_local2,*local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   
   	 // }
   	}
}

void Soma_locais(int* local_n,double *vetor_local1,int *escalar, double *vetor_local2,double *soma_local,double *resultado,int rank){
   int i;
   soma_local = malloc(sizeof(double));
   resultado= malloc(sizeof(double));
   // calculando as somas locais
   printf("\n vetor 1 \n" );
   for(i=0;i<*local_n;i++){
      printf("%lf",vetor_local1[i]);
     
     // *soma_local = (*soma_local) + vetor_local1[i];
   
   }

/*
   for(i=0;i<*local_n;i++){
      vetor_local1[i] = vetor_local1[i] *(*escalar);
      vetor_local1[i] =vetor_local1[i]*vetor_local2[i];
     // *soma_local = (*soma_local) + vetor_local1[i];
   
   }
   //Adiciona as somas locais no resultado
  // MPI_Reduce(soma_local, resultado, 1, MPI_DOUBLE, MPI_SUM, 0,
    //     MPI_COMM_WORLD);

   if(rank == 0){
      printf("O resultado final: %lf\n",*resultado);
      free(vetor_local1);
      free(vetor_local2);
   }*/
}

