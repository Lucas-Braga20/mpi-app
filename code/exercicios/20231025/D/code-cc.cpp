#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

const int TAG = 10;

int main(int argc, char *argv[]) {
    int valor;
    int num_processadores;
    int posicao;
    int quantidade;
    int x;

    int *entrada_geral;
    int *entrada_local;
    int *quantidades;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &num_processadores);
    MPI_Comm_rank(MPI_COMM_WORLD, &posicao);

    //
    // Primeira etapa.
    //
    if (posicao == 0) {
        FILE *arquivo;

        if (!(arquivo = fopen("entrada.txt", "r"))) {
            printf("\nERRO: Houve um erro na abertura do arquivo.");
            MPI_Finalize();
            return 0;
        }

        printf("Informe o valor de x: ");
        scanf("%d", &x);

        MPI_Barrier(MPI_COMM_WORLD);

        MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

        fscanf(arquivo, "%d", &valor);

        entrada_geral = new int[valor];

        for (int i = 0; i < valor; i++) {
            fscanf(arquivo, "%d", &entrada_geral[i]);
        }

        fclose(arquivo);

        MPI_Bcast(&valor, 1, MPI_INT, 0, MPI_COMM_WORLD);

        quantidades = new int[num_processadores];
    } else {
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&valor, 1, MPI_INT, 0, MPI_COMM_WORLD);
    }

    //
    // Segunda etapa.
    //
    entrada_local = new int[valor / num_processadores];

    MPI_Scatter(entrada_geral, valor / num_processadores, MPI_INT, entrada_local, valor / num_processadores, MPI_INT, 0, MPI_COMM_WORLD);

    //
    // Terceira etapa.
    //
    quantidade = 0;

    for (int i = 0; i < (valor / num_processadores); i++) {
        if (entrada_local[i] == x) {
            quantidade = quantidade + 1;
        }
    }

    //
    // Quarta etapa.
    //
    MPI_Gather(&quantidade, 1, MPI_INT, quantidades, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (posicao == 0) {
        quantidade = 0;

        for (int i = 0; i < num_processadores; i++) {
            quantidade = quantidade + quantidades[i];
        }

        printf("\nA quantidade de repetições é: %d.\n", quantidade);

        delete[] entrada_geral;
        delete[] quantidades;
    }

    delete[] entrada_local;
    MPI_Finalize();
    return 0;
}