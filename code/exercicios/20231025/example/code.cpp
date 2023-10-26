#include <stdio.h>
#include "mpi.h"

const int TAG = 10;

int main(int argc, char *argv[]) {
    int entrada_tamanho;
    int processadores_tamanho;
    int posicao;
    int soma;

    int *entrada_geral;
    int *entrada_local;
    int *soma_local;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &processadores_tamanho);
    MPI_Comm_rank(MPI_COMM_WORLD, &posicao);

    // Primeira etapa.
    //
    // O primeiro processador vai fazer a leitura do tamanho do vetor
    // adquirido a partir da primeira linha do arquivo. Depois será enviado
    // para os demais processadores o valor da entrada.
    //
    // O primeiro processador também fará a alocação da entrada geral em buffer.
    // Quando chegar o momento de sub-dividir a entrada geral será usado esse buffer.
    //
    // Os demais processadores ficarão esperando pelo valor do tamanho da entrada.
    if (posicao == 0) {
        FILE *arquivo;

        if (!(arquivo = fopen("entrada.txt", "r"))) {
            printf("\nERRO: Houve um erro na abertura do arquivo.");
            MPI_Finalize();
            return 0;
        }

        fscanf(arquivo, "%d", &entrada_tamanho);

        // Envia o tamanho da entrada para os demais processadores.
        for (int i = 1; i < processadores_tamanho; i++) {
            MPI_Send(&entrada_tamanho, 1, MPI_INT, i, TAG, MPI_COMM_WORLD);
        }

        entrada_geral = new int[entrada_tamanho];

        // Lendo a entrada no arquivo.
        for (int i = 0; i <= entrada_tamanho; i++) {
            fscanf(arquivo, "%d", &entrada_geral[i]);
        }

        soma_local = new int[processadores_tamanho];

        fclose(arquivo);
    }
    else {
        // Os demais processadores recebe o tamanho da entrada;
        MPI_Recv(&entrada_tamanho, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
    }

    // Segunda etapa.
    //
    // Todos os processadores irão alocar a entrada local (Sub divisão da entrada).
    // A sub-divisão é feita a oartir MPI_Scatter.
    entrada_local = new int[entrada_tamanho / processadores_tamanho];

    MPI_Scatter(entrada_geral, entrada_tamanho / processadores_tamanho, MPI_INT, entrada_local, entrada_tamanho / processadores_tamanho, MPI_INT, 0, MPI_COMM_WORLD);

    // Terceira etapa.
    //
    // Esta é uma etapa de computação interna. Todos os processadores
    // realizam a soma interna da entrada local.
    soma = 0;

    for (int i = 0; i < (entrada_tamanho / processadores_tamanho); i++) {
        soma = soma + entrada_local[i];
    }

    // Quarta etapa.
    //
    // Na quarta etapa, o primeiro processador irá 
    // receber a soma interna dos demais processadores
    // e recalcular a soma com todas as somas internas.
    //
    // Depois de feito a soma interna, ele irá escrever o
    // resultado no arquivo de saída.
    //
    // Enquanto os demais processadores irão enviar a soma interna
    // para o primeiro processador.
    if (posicao == 0) {
        FILE *arquivo;

        soma_local[0] = soma;

        for (int i = 1; i < processadores_tamanho; i++ ) {
            MPI_Recv(&soma_local[i], 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &status);
        }

        soma = 0;

        for (int i = 0; i < processadores_tamanho; i++) {
            soma = soma + soma_local[i];
        }

        if(!(arquivo = fopen("saida.txt", "w"))) {
            printf("\nErro: Houve um erro na abertura do arquivo.");
            MPI_Finalize();
            return 0;
        }

        fprintf(arquivo, "Resultado da Soma: %d ", soma);
        
        fclose(arquivo);
        delete(entrada_geral);
        delete(soma_local);
    }
    else {
        MPI_Send(&soma, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    delete(entrada_local);
    return 0;
}