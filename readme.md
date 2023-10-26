# Docker MPI app

Configuração de container implementada para executar códigos c++ com a biblioteca MPI.

Para iniciar os containers é necessário executar os comandos:

```bash
docker-compose build

docker-compose run mpi-app
```

## Pacotes

### lam-runtime

O lam-runtime é um pacote instalado para obter as bibliotecas e ferramentas necessárias para executar programas MPI compilados com o LAM/MPI. Isso inclui, por exemplo, as bibliotecas compartilhadas e os executáveis necessários para iniciar e gerenciar a execução de programas paralelos.

Comandos:

- `lamboot`: configura as comunicações entre os processos MPI. Ele estabelece as conexões e configura os canais de comunicação necessários para a troca de mensagens entre os diferentes processos.
- `lamnodes`: exibe informações sobre os nós que fazem parte do ambiente LAM/MPI.
- `lamhalt`: Este comando encerra todos os processos do LAM/MPI.

### mpich

O MPICH (Message Passing Interface for CHameleon) é uma implementação de referência do padrão MPI (Message Passing Interface). O MPI é um padrão de comunicação entre processos utilizado em programação paralela e distribuída, permitindo que os programas troquem mensagens e coordenem a execução em sistemas distribuídos.

Comandos:

- mpiCC: comando utilizado para compilar o código. Exemplo: `mpiCC nomecodigo -o nomeexecutavel`;
- mpirun: comando utilizado para executar um código com uma quantidade x de processos. Exemplo: `mpirun -np numeroprocessos nomeexecutavel`;

## Biblioteca MPI

### Função Send

```cpp
MPI_Send(
    void* data,
    int count,
    MPI_Datatype datatype,
    int destination,
    int tag,
    MPI_Comm communicator)
```

A função `MPI_Send` é usada para enviar uma mensagem de um processo remetente para um processo destinatário em um ambiente de programação paralela usando o MPI (Message Passing Interface). Ela tem os seguintes parâmetros:

1. `void* data`: É um ponteiro para os dados que você deseja enviar. Os dados podem ser de qualquer tipo de dado suportado pelo MPI (inteiros, floats, structs, etc.). É importante que o tipo de dados que você está enviando corresponda ao tipo especificado pelo parâmetro datatype.

2. `int count`: Indica o número de elementos de dados que estão sendo enviados. Se você estiver enviando um único elemento de dados, count deve ser 1. Se estiver enviando um array de dados, count deve refletir o número de elementos nesse array.

3. `MPI_Datatype datatype`: Especifica o tipo de dado dos elementos que estão sendo enviados. O MPI fornece diversos tipos de dados predefinidos, como MPI_INT, MPI_FLOAT, MPI_DOUBLE, etc. O tipo de dado deve corresponder ao tipo real dos dados apontados pelo ponteiro data. Isso garante que os dados sejam corretamente interpretados no processo de destino.

4. `int destination`: É o rank (identificador) do processo destinatário que receberá a mensagem. O destino é identificado pelo seu rank no comunicador especificado.

5. `int tag`: O TAG é um identificador de etiqueta que rotula a mensagem. Ele permite que o remetente e o destinatário classifiquem e identifiquem a mensagem com base em sua finalidade. O TAG é usado para distinguir diferentes tipos de mensagens e operações.

6. `MPI_Comm communicator`: Especifica o comunicador no qual a comunicação ocorrerá. O comunicador determina o grupo de processos que podem se comunicar entre si. O comunicador pode ser `MPI_COMM_WORLD` para comunicação em todo o mundo ou um comunicador personalizado criado por meio de `MPI_Comm_create`.

### Função receive

```cpp
MPI_Recv(
    void* data,
    int count,
    MPI_Datatype datatype,
    int source,
    int tag,
    MPI_Comm communicator,
    MPI_Status* status)
```

A função `MPI_Recv` é usada para receber uma mensagem de um processo remetente em um ambiente de programação paralela usando o MPI (Message Passing Interface). Ela tem os seguintes parâmetros:

1. `void* data`: É um ponteiro para a área de memória onde os dados recebidos serão armazenados. Os dados recebidos serão colocados nessa área de memória. É importante que o tipo de dados especificado pelo parâmetro `datatype` corresponda ao tipo de dados real dos dados que serão recebidos.

2. `int count`: Indica o número de elementos de dados que se espera receber. Se você estiver esperando receber um único elemento de dados, `count` deve ser 1. Se estiver esperando receber um array de dados, `count` deve refletir o número de elementos esperados.

3. `MPI_Datatype datatype`: Especifica o tipo de dado dos elementos que serão recebidos. O MPI fornece diversos tipos de dados predefinidos, como `MPI_INT`, `MPI_FLOAT`, `MPI_DOUBLE`, etc. O tipo de dado deve corresponder ao tipo real dos dados que serão recebidos.

4. `int source`: É o rank (identificador) do processo remetente que está enviando a mensagem que será recebida. O processo remetente é identificado pelo seu rank no comunicador especificado.

5. `int tag`: O TAG é um identificador de etiqueta que rotula a mensagem a ser recebida. Ele permite que o destinatário identifique a mensagem com base em sua finalidade e distingua entre diferentes tipos de mensagens e operações.

6. `MPI_Comm communicator`: Especifica o comunicador no qual a comunicação ocorrerá. O comunicador determina o grupo de processos que podem se comunicar entre si. O comunicador pode ser `MPI_COMM_WORLD` para comunicação em todo o mundo ou um comunicador personalizado criado por meio de `MPI_Comm_create`.

7. `MPI_Status* status`: A função MPI_Recv preencherá esta estrutura com informações sobre o status da operação de recepção, como o tamanho da mensagem recebida, o TAG da mensagem, etc. O uso dessa estrutura é opcional, e você pode passar `MPI_STATUS_IGNORE` se não precisar dessas informações.

#### MPI_Datatype

|      MPI datatype      |      C equivalent      |
|:----------------------:|:----------------------:|
| MPI_SHORT              | short int              |
| MPI_INT                | int                    |
| MPI_LONG               | long int               |
| MPI_LONG_LONG          | long long int          |
| MPI_UNSIGNED_CHAR      | unsigned char          |
| MPI_UNSIGNED_SHORT     | unsigned short int     |
| MPI_UNSIGNED           | unsigned int           |
| MPI_UNSIGNED_LONG      | unsigned long int      |
| MPI_UNSIGNED_LONG_LONG | unsigned long long int |
| MPI_FLOAT              | float                  |
| MPI_DOUBLE             | double                 |
| MPI_LONG_DOUBLE        | long double            |
| MPI_BYTE               | char                   |
