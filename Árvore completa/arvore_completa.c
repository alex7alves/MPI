
/* Autor : Alex Alves
   Programa para calculo soma em arvore com processos impares 
   e nao potencia de 2
*/
#include <stdio.h>
#include <stdlib.h>
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
int SomaLocal(int *vetor,int a,int b){
   int soma=0;
   for(int i=a;i<=b;i++)
   {
      soma=soma+vetor[i];
   } 
   return soma;
}

// FAlTA ajustar o tamanho da profundidade da arvore para ser auto-ajustável
int main(int argc, char *argv[]) {
   int size; // quantidade maxima de processos
   int rank;  // numero do  processo atual
   int vetor[argc-1],local,divisor=2,difference=1,soma; 
   int n = argc-1;
   int local_n, local_a, local_b;
   MPI_Status stat; // guarda o status 
   int QuantidadeCoreExecutando,IndiceUltimoCoreExecucao,aux;
   
   for(int i=0;i<argc-1;i++){
      vetor[i]=atoi(argv[i+1]);
   }
   // Inicia o MPI
   MPI_Init(NULL, NULL); 
   // Pega o numero total de processos
   MPI_Comm_size(MPI_COMM_WORLD, &size); 
   // pega o numero do processo atual
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
   int valor[size];
   int lg = lg2(size);
   QuantidadeCoreExecutando = size;
   IndiceUltimoCoreExecucao=size-1;
   local_n=(n/size);
   if (n%size==0)  // caso o número total de elementos do vetor seja divisível pela qtd de núcleos
   {
      local_a=rank*(n/size);
      local_b=local_a+(n/size)-1;
      valor[rank]=SomaLocal(vetor,local_a,local_b);
   }
   else  //caso o número total de elementos do vetor NÃO seja divisível pela qtd de núcleos 
   {
      if(rank<(n%size))
      {
        local_n = local_n+1;
        local_a = rank*local_n;
        local_b = local_a +local_n-1;
        valor[rank]=SomaLocal(vetor,local_a,local_b);
      }
      else
      {
         // para nao pegar valores ja usados
         // faz o reajuste n%size
        local_a = rank*local_n+(n%size);
        local_b = local_a + local_n-1;
        valor[rank]=SomaLocal(vetor,local_a,local_b); 
      }
   }
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