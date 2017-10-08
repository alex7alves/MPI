
/* Autor : Alex Alves
   Programa para calculo em arvore
*/
#include <stdio.h>
#include <string.h>  
#include <mpi.h>     

int main(void) {
   int size; // quantidade maxima de processos
   int rank;  // numero do  processo atual
   int valor[8],local,divisor=2,difference=1,soma; 
   MPI_Status stat; // guarda o status 

   // Valores iniciais para cada processo
   valor[0]= 5,valor[1]=2,valor[2]=-1, valor[3]= -3 ;
   valor[4]= 6 , valor[5]= 5, valor[6]= -7, valor[7]=2;

   // Inicia o MPI
   MPI_Init(NULL, NULL); 
   // Pega o numero total de processos
   MPI_Comm_size(MPI_COMM_WORLD, &size); 
   // pega o numero do processo atual
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
   while( divisor<= 8) {
      for(int IndiceCore=0;IndiceCore<8 ;IndiceCore= IndiceCore+divisor) {  
         if(rank==(IndiceCore+difference)){
            //so ocorre na primeira linha da arvore   
            // local = Valor[IndiceCore+diferenca];  
            MPI_Send(&valor[IndiceCore+difference],1,MPI_INT,IndiceCore,0,MPI_COMM_WORLD);  
            
            printf(" Sou o processo  %d enviei pro %d\n ",rank, IndiceCore);
         }else if(rank==IndiceCore) {
            // rank pares
            //I_Reduce(&valor[IndiceCore],&soma,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD); 
            MPI_Recv(&local,1,MPI_INT,IndiceCore+difference,0,MPI_COMM_WORLD,&stat);
            valor[IndiceCore]=valor[IndiceCore]+local;
            printf(" Sou o processo %d recebi o valor  %d apos a soma ficou %d\n",rank,local, valor[IndiceCore]);
         }
        // MPI_Send(&Valores[i],1,MPI_INT,destino,0,MPI_COMM_WORLD); 
          //Core( IndiceCore) = Core( IndiceCore + difference);  
      }
      divisor = divisor*2;
      difference = difference *2;
   }


   if(rank==0){
   	   printf("O processo %d terminou com o valor %d\n",rank,valor[0 ]); 
   }
  
   //finalisa o MPI
   MPI_Finalize(); 
   return 0;
} 