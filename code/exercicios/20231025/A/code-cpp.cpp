#include <stdio.h>
#include "mpi.h"

const int TAG = 10;

int main(int argc, char *argv[]) {
    int valor;
    int num_processadores;
    int posicao;
    int soma;

    int *soma_local;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_processadores);
    MPI_Comm_rank(MPI_COMM_WORLD, &posicao);

    //
    // Primeira etapa.
    //
    if (posicao == 0) {
        do {
            printf("\nInforme o tamanho da entrada: ");
            scanf("%d", &valor);

            if (valor % num_processadores != 0) {
                printf("\nNumero invalido!\n");
            }
        } while(valor % num_processadores != 0);

        for (int i = 1; i < num_processadores; i++) {
            MPI_Send(&valor, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
        }

        soma_local = new int[num_processadores];
    }
    else {
        MPI_Recv(&valor, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
    }

    //
    // Segunda etapa.
    //
    soma = 0;
    for (int i = posicao * (valor/num_processadores) + 1; i <= posicao * (valor/num_processadores) + (valor/num_processadores); i++) {
        soma = soma + i;
    }

    //
    // Terceira etapa.
    //
    if (posicao == 0) {
        soma_local[0] = soma;

        for (int i = 1; i < num_processadores; i++) {
            MPI_Recv(&soma_local[i], 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &status);
        }

        soma = 0;
        for (int i = 0; i < num_processadores; i++) {
            soma = soma + soma_local[i];
        }

        printf("\nSoma de Prefixo: %d\n\n", soma);

        delete(soma_local);
    }
    else {
        MPI_Send(&soma, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
