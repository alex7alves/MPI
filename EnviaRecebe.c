
/* Autor : Alex Alves
   Programa para enviar e receber  mensagens entre processos
*/
#include <stdio.h>
#include <string.h>  
#include <mpi.h>     

int main(void) {
   int size; // quantidade maxima de processos
   int rank;  // numero do  processo atual
   int mensagem; // mensagem vaiza parra  processo 1 ee cheia para 0
   int destino,remetente,assunto;
   MPI_Status stat; // guarda o status 
   // Inicia o MPI
   MPI_Init(NULL, NULL); 
   // Pega o numero total de processos
   MPI_Comm_size(MPI_COMM_WORLD, &size); 
   // pega o numero do processo atual
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
   if(rank==0){
   	 	// processo 0 envia mensagem
   		mensagem =30, destino=1,assunto=0;
   	   MPI_Send(&mensagem,1,MPI_INT,destino,assunto,MPI_COMM_WORLD);
   	   printf("O processo %d enviou o numero  %d para o processo %d\n",rank,mensagem,destino);
         // processo 0 recebe a  mensagem
         remetente=1,assunto=1;
         MPI_Recv(&mensagem,1,MPI_INT,remetente,assunto,MPI_COMM_WORLD,&stat);
         printf("Ok processo %d o numero  %d  esta certo, saudacoes de   %d\n",remetente,mensagem,rank);
   }
   if(rank==1){
   		// processo 1 recebe a mensagem
   		assunto=0,remetente=0;
   		MPI_Recv(&mensagem,1,MPI_INT,remetente,assunto,MPI_COMM_WORLD,&stat);
   		printf("O processo %d recebeu o numero  %d vindo do processo %d\n",rank,mensagem,remetente);
         // processo 1 envia mensagem
         mensagem =47, destino=0,assunto=1;
         MPI_Send(&mensagem,1,MPI_INT,destino,assunto,MPI_COMM_WORLD);
         printf("Processo %d fiz uns claculos insanos e deu esse numero %d saudacoes  do processo %d\n",destino,mensagem,rank);
   }
   
   //finalisa o MPI
   MPI_Finalize(); 
   return 0;
} 