
/* Autor : Alex Alves
   Programa para calcular soma em arvore com qualquer numero de processos 
   e qualquer tamanho do vetor e calcular o seu tempo de execucao

   execute assim :
   mpiexec -n p ./arvore_tempo x (tamanho do vetor -> soma de 1 ate x)
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

int main(int argc, char *argv[]) {
   int size; // quantidade maxima de processos
   int rank;  // numero do  processo atual
   int n =atoi(argv[1]); // tamanho do vetor passado
   int vetor[n],local,divisor=2,difference=1,soma; 

   int local_n, local_a, local_b;
   MPI_Status stat; // guarda o status 
   int QuantidadeCoreExecutando,IndiceUltimoCoreExecucao,aux;
   // Variaveis para calcular o tempo
   double inicio,fim,duracao,tempo=0;
   // preenchendo o vetor
   for(int i=0;i<n;i++){
      vetor[i]=(i+1);
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
   //Numero  de elementos do vetor  divisível pela quantidade de processos
   if (n%size==0)  
   {
      local_a=rank*(n/size);
      local_b=local_a+(n/size)-1;
      valor[rank]=SomaLocal(vetor,local_a,local_b);
   }
   else // Tamanho do vetor nao divisel pelo numero de processos
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
   // inicia a contagem de tempo ->inicio da arvore
   inicio= MPI_Wtime();

   for(int j=1;j<=lg;j++){
      for(int IndiceCore=0;IndiceCore<size ;IndiceCore= IndiceCore+divisor) {  
         //(IndiceCore+difference)<size pra nao dar erro de enviar para numero inxistente
         //tipo ta no rank 8 e envia pro 16 mas no maximo eh 14
         if(rank==(IndiceCore+difference) && (IndiceCore+difference)<size){  
            MPI_Send(&valor[IndiceCore+difference],1,MPI_INT,IndiceCore,0,MPI_COMM_WORLD);    
         }else if(QuantidadeCoreExecutando%2!=0 && IndiceCore==IndiceUltimoCoreExecucao){
            // se for impar manda o ultimo processo para a proxima 
            // linha da arvore
            MPI_Send(&valor[IndiceUltimoCoreExecucao],1,MPI_INT,IndiceUltimoCoreExecucao,0,MPI_COMM_WORLD);
            
         }else if(rank==IndiceCore &&  (IndiceCore+difference)<size) {
            // rank pares
            MPI_Recv(&local,1,MPI_INT,IndiceCore+difference,0,MPI_COMM_WORLD,&stat);
            valor[IndiceCore]=valor[IndiceCore]+local;
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
      if(QuantidadeCoreExecutando>1 && j==lg){
         lg=lg+1;
      }
   }
   // Fim da arvore
   fim= MPI_Wtime();
   // calcula a duracao
   duracao=fim-inicio;
   MPI_Reduce(&duracao,&tempo,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
   if(rank==0){
         printf("O processo %d terminou com o valor %d\n",rank,valor[0]); 
         printf("Tempo medido: %lf\n",tempo);
   }
   MPI_Finalize(); 
   return 0;
} 