#include <stdio.h>
#include "mpi.h"

const int TAG = 10;

int main(int argc, char *argv[]) {
    int valor;
    int num_processadores;
    int posicao;
    int soma;

    int *entrada_geral;
    int *entrada_local;
    int *divisores;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_processadores);
    MPI_Comm_rank(MPI_COMM_WORLD, &posicao);

    // Primeira etapa.
    //
    // Recepção dos dados e comunicação.
    if (posicao == 0) {
        printf("Informe o tamanho da entrada: ");
        scanf("%d", &valor);

        entrada_geral = new int[valor];

        for (int i = 0; i < valor; i++) {
            entrada_geral[i] = i + 1;
        }

        divisores = new int[num_processadores];

        for (int i = 1; i < num_processadores; i++) {
            MPI_Send(&valor, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
        }
    }
    else {
        MPI_Recv(&valor, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
    }

    // Segunda etapa.
    //
    // Divisão da entrada em sub-entradas.
    entrada_local = new int[valor / num_processadores];

    MPI_Scatter(entrada_geral, valor / num_processadores, MPI_INT, entrada_local, valor / num_processadores, MPI_INT, 0, MPI_COMM_WORLD);

    // Terceira etapa.
    //
    // Rodada de computação. Calcula a quantidade de divisores.
    int quantidade = 0;

    for (int i = 0; i < valor / num_processadores; i++) {
        if (valor % entrada_local[i] == 0) {
            quantidade = quantidade + 1;
        }
    }

    // Quarta etapa.
    //
    // Rodada de comunicação.
    if (posicao != 0) {
        MPI_Send(&quantidade, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
    }
    else {
        divisores[0] = quantidade;

        for (int i = 1; i < num_processadores; i++ ) {
            MPI_Recv(&divisores[i], 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &status);
        }
    }

    // Quinta etapa.
    //
    // Rodada de computação.
    if (posicao == 0) {
        soma = 0;

        for (int i = 0; i < num_processadores; i++) {
            soma = soma + divisores[i];
        }

        printf("\nQuantidade de divisores: %d.\n", soma);
    }
}
