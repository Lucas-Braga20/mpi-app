#include <stdio.h>
#include "mpi.h"

const int TAG = 10;

int main(int argc, char *argv[]) {
    int valor;
    int num_processadores;
    int posicao;
    int maior;

    int *entrada_geral;
    int *entrada_local;
    int *maiores;

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

        fscanf(arquivo, "%d", &valor);

        // Envia o tamanho da entrada para os demais processadores.
        for (int i = 1; i < num_processadores; i++) {
            MPI_Send(&valor, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
        }

        entrada_geral = new int[valor];

        // Lendo a entrada no arquivo.
        for (int i = 0; i <= valor; i++) {
            fscanf(arquivo, "%d", &entrada_geral[i]);
        }

        maiores = new int[num_processadores];

        fclose(arquivo);
    }
    else {
        MPI_Recv(&valor, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
    }

    //
    // Segunda etapa.
    //
    entrada_local = new int[valor / num_processadores];

    MPI_Scatter(entrada_geral, valor / num_processadores, MPI_INT, entrada_local, valor / num_processadores, MPI_INT, 0, MPI_COMM_WORLD);

    //
    // Terceira etapa.
    //
    maior = entrada_local[0];

    for (int i = 1; i < (valor / num_processadores); i++) {
        if (maior < entrada_local[i]) {
            maior = entrada_local[i];
        }
    }

    //
    // Quarta etapa.
    //
    if (posicao == 0) {
        maiores[0] = maior;

        for (int i = 1; i < num_processadores; i++ ) {
            MPI_Recv(&maiores[i], 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &status);
        }

        maior = maiores[0];

        for (int i = 1; i < num_processadores; i++) {
            if (maior < maiores[i]) {
                maior = maiores[i];
            }
        }

        printf("\nMaior valor do conjunto: %d.\n", maior);

        delete(entrada_geral);
        delete(maiores);
    }
    else {
        MPI_Send(&maior, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    delete(entrada_local);
    return 0;
}
