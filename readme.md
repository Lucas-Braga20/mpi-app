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
