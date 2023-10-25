#include <stdio.h>
#include "mpi.h"

const int TAG = 10;

int main(int argc, char *argv[]) {
    int n; // Tamanho da entrada
    int size; // Quantidade de processadores
    int rank; // Rank do processador
    int soma; // Variavel p/ a soma de valores locais
    int *A; // Buffer p/ entrada geral
    int *B; // Buffer p/ entrada local
    int *S; // Buffer p/ as somas locais

    MPI_Status status;

    MPI_Init(&argc, &argv); // Iniciando o mpi

    MPI_Comm_size(MPI_COMM_WORLD, &size); // Retornando a quantidade de processadores em operacao
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Retornando o rank do processador

    if (rank == 0) {
        FILE *arquivo; // Ponteiro p/ o arquivo de entrada

        char entrada[30];

        printf("\nInforme o arquivo de entrada: ");
        scanf("%s", entrada);

        if(!(arquivo = fopen( entrada, "r" ))){
            printf("\n ERRO NA ABERTURA DO ARQUIVO!!! ");
            MPI_Finalize();
            return 0;
        }

        fscanf(arquivo,"%d",&n);

        for( int i = 1; i < size; i++ ) {
            MPI_Send(&n, 1, MPI_INT, i, TAG, MPI_COMM_WORLD); // Distribuindo o tamanho da entrada para os demais processadores
        }

        A = new int[n]; // Alocando um buffer temporario p/ distribuir a entrada geral

        for( int i = 0; i <= n; i++ ) // Lendo a entrada no arquivo
            fscanf( arquivo,"%d", &A[i] );

        fclose(arquivo);// Fechando o arquivo de entrada

        S = new int[size]; // Alocando um buffer temporario p/ receber a soma de todos processadores
    }
    else {
        MPI_Recv(&n, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status ); // Recebendo o tamanho da entrada do primeiro processador
    }

    B = new int[n/size]; // Alocando o buffer p/ a entrada local de valores

    MPI_Scatter( A, n/size, MPI_INT, B, n/size, MPI_INT, 0, MPI_COMM_WORLD ); // Realizando a distribuicao da entrada entre os processadores

    soma = 0;
    // Realizando a soma dos valores locais
    for( int i = 0; i < n/size; i++ ) {
        soma = soma + B[i];
    }


    if (rank == 0) {
        FILE *arquivo; // Ponteiro p/ o arquivo de saida

        S[0] = soma;

        for( int i = 1; i < size; i++ ) {
            MPI_Recv(&S[i], 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &status ); // /Coletando a soma dos demais processadores
        }

        soma = 0;

        // Calculando a soma dos dados recebidos
        for( int i = 0; i < size; i++ ) {
            soma = soma + S[i];
        }

        // Criando o arquivo de saida
        if(!(arquivo = fopen( "saida.txt", "w" ))) {
            printf("\n ERRO NA ABERTURA DO ARQUIVO!!! ");
            MPI_Finalize();
            return 0;
        }

        fprintf(arquivo, "Resultado da Soma: %d ", soma);
        
        fclose(arquivo);// Fechando o arquivo de saida

        delete( A ); // Desalocando buffer temporario
        delete( S ); // Desalocando buffer temporario
    }
    else {
        MPI_Send(&soma, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD); // Enviando a soma local para o primeiro processador
    }

    MPI_Finalize(); // Finalizando o mpi

    delete(B); // Desalocando o buffer de entrada local

    return 0;
}
