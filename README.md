# MPI 

Este repositório é destinado a abrigar códigos de programas em MPI.

## Conceito de MPI :

   Message Passing Interface (MPI) é um padrão de comunicação de dados para computação paralela, ou seja, troca de dados entre processos. O MPI oferece diversas abstracções que facilitam e padronizam o desenvolvimento de aplicações paralelas. Com essa ferramenta  pode-se  programar para vários processadores, nós de um cluster ou  supercomputadores.

## Como compila ?
	mpicc -o Nome_do_arquivo Nome_do_arquivo.c

Ou se quiser uma compilação mais detalhada :
    mpicc  -g  -Wall  -o  Nome_do_arquivo  Nome_do_arquivo.c

## Como executa ?
	mpiexec  -n  4  ./Nome_do_arquivo

   
Ou você pode fazer também : 
	mpirun -np 4  Nome_do_arquivo
    

Onde o 4 representa a quantidade máxima de processos que você vai usar. Esse número é variável de acordo com seu gosto.    
