#include <stdio.h>
#include "mpi.h"

const int TAG = 10;

int main(int argc, char *argv[]) {
    int valor;
    int num_processadores;
    int posicao;
    int soma;

    int *divisores;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_processadores);
    MPI_Comm_rank(MPI_COMM_WORLD, &posicao);

    if (posicao == 0) {
        do {
            printf("\nInforme o numero de entrada: ");
            scanf("%d", &valor);

            if (valor % num_processadores != 0) {
                printf("\nNumero invalido!\n");
            }
        } while(valor % num_processadores != 0);

        divisores = new int[num_processadores];
    }

    MPI_Bcast(&valor, 1, MPI_INT, 0, MPI_COMM_WORLD);

    soma = 0;
    for (int i = posicao * (valor/num_processadores) + 1; i <= posicao * (valor/num_processadores) + (valor/num_processadores); i++) {
        if (valor % i == 0) {
            soma = soma + 1;
        }
    }

    MPI_Gather(&soma, 1, MPI_INT, divisores, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (posicao == 0) {
        soma = 0;
        for (int i = 0; i < num_processadores; i++) {
            soma = soma + divisores[i];
        }

        printf("\nQuantidade de divisores: %d\n\n", soma);

        delete(divisores);
    }

    MPI_Finalize();

	return 0;
}
