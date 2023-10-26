#include <stdio.h>
#include "mpi.h"

const int TAG = 10;

int main(int argc, char *argv[]) {
    int valor;
    int num_processadores;
    int posicao;

    int *entrada_geral;
    int *entrada_local;
    int *soma_local;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_processadores);
    MPI_Comm_rank(MPI_COMM_WORLD, &posicao);

    if (posicao == 0) {
        printf("Informe o tamanho da entrada: ");
        scanf("%d", &valor);

        entrada_geral = new int[valor];

        printf("Informe os valores da entrada: \n");
        for (int i = 0; i < valor; i++) {
            scanf("%d", &entrada_geral[i]);
        }

        soma_local = new int[num_processadores];

        for (int i = 1; i < num_processadores; i++) {
            MPI_Send(&valor, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
        }
    }
    else {
        // Os demais processadores recebe o tamanho da entrada;
        MPI_Recv(&valor, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
    }

    entrada_local = new int[valor / num_processadores];

    MPI_Scatter(entrada_geral, valor / num_processadores, MPI_INT, entrada_local, valor / num_processadores, MPI_INT, 0, MPI_COMM_WORLD);

    int soma = 0;

    for (int i = 0; i < valor / num_processadores; i++) {
        soma = soma + entrada_local[i];
    }

    if (posicao != 0) {
        MPI_Send(&soma, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
    }
    else {
        soma_local[0] = soma;

        for (int i = 1; i < num_processadores; i++ ) {
            MPI_Recv(&soma_local[i], 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &status);
        }
    }

    if (posicao == 0) {
        soma = 0;

        for (int i = 0; i < num_processadores; i++) {
            soma = soma + soma_local[i];
        }

        printf("\nResultado da soma de prefixo: %d.\n", soma);
    }
}
