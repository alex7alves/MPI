
/* Autor : Alex Alves
   Programa para calculo soma em arvore com processos impares 
   e nao potencia de 2
*/
#include <stdio.h>
#include <string.h>  
#include <mpi.h>    
#include <math.h>  

int lg2(int n){
   int aux=n;
   int i=1;
   while(aux>1){
      aux = aux/2;
      i++;
   }
   return i;
}

// FAlTA ajustar o tamanho da profundidade da arvore para ser auto-ajustável
int main(void) {
   int size; // quantidade maxima de processos
   int rank;  // numero do  processo atual
   int valor[11],local,divisor=2,difference=1,soma; 
   MPI_Status stat; // guarda o status 
   int QuantidadeCoreExecutando,IndiceUltimoCoreExecucao,aux;
   // Valores iniciais para cada processo
   valor[0]= 5,valor[1]=2,valor[2]=-1, valor[3]= -3;
   valor[4]= 6 , valor[5]= 5, valor[6]= -7,valor[7]=2,valor[8]=1,valor[9]=3,valor[10]=2;
   // tamanho do vetor
   int n = sizeof(valor)/sizeof(valor[0]);
   // Inicia o MPI
   MPI_Init(NULL, NULL); 
   // Pega o numero total de processos
   MPI_Comm_size(MPI_COMM_WORLD, &size); 
   // pega o numero do processo atual
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
   int lg = lg2(size);
  
   QuantidadeCoreExecutando = size;
   IndiceUltimoCoreExecucao=size-1;
   
   for(int j=1;j<=4;j++){
 //  while( divisor<= size) {
      for(int IndiceCore=0;IndiceCore<size ;IndiceCore= IndiceCore+divisor) {  
         //(IndiceCore+difference)<size pra nao dar erro de enviar para numero inxistente
         //tipo ta no rank 8 e envia pro 16 mas no maximo eh 14
         if(rank==(IndiceCore+difference) && (IndiceCore+difference)<size){  
            MPI_Send(&valor[IndiceCore+difference],1,MPI_INT,IndiceCore,0,MPI_COMM_WORLD);    
            printf(" Sou o processo  %d enviei pro %d\n",rank, IndiceCore);
         }else if(QuantidadeCoreExecutando%2!=0 && IndiceCore==IndiceUltimoCoreExecucao){
            // se for impar manda o ultimo processo para a proxima 
            // linha da arvore
            MPI_Send(&valor[IndiceUltimoCoreExecucao],1,MPI_INT,IndiceUltimoCoreExecucao,0,MPI_COMM_WORLD);
            
         }else if(rank==IndiceCore &&  (IndiceCore+difference)<size) {
            // rank pares
            //I_Reduce(&valor[IndiceCore],&soma,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD); 
            MPI_Recv(&local,1,MPI_INT,IndiceCore+difference,0,MPI_COMM_WORLD,&stat);
            valor[IndiceCore]=valor[IndiceCore]+local;
            printf(" Sou o processo %d recebi o valor  %d apos a soma ficou %d\n",rank,local, valor[IndiceCore]);
         
         }else if(QuantidadeCoreExecutando%2!=0 && IndiceCore==IndiceUltimoCoreExecucao){
            // se for impar equilibra a arvore 
            MPI_Recv(&local,1,MPI_INT,IndiceUltimoCoreExecucao,0,MPI_COMM_WORLD,&stat);
            valor[IndiceUltimoCoreExecucao]=valor[IndiceUltimoCoreExecucao];
            QuantidadeCoreExecutando= QuantidadeCoreExecutando+1;
            IndiceUltimoCoreExecucao =IndiceUltimoCoreExecucao;
         }         
      }
      divisor = divisor*2;
      difference = difference *2;
      QuantidadeCoreExecutando=QuantidadeCoreExecutando/2;
      IndiceUltimoCoreExecucao =IndiceUltimoCoreExecucao-aux;
   }
   if(rank==0){
   	   printf("O processo %d terminou com o valor %d\n",rank,valor[0]); 
   }
   MPI_Finalize(); 
   return 0;
} 