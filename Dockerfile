FROM ubuntu:latest

RUN apt-get update \
    && apt-get install -y g++ mpich less lam-runtime \
    && rm -rf /var/lib/apt/lists/*

RUN useradd -ms /bin/bash mpiuser
USER mpiuser
WORKDIR /home/mpiuser/code

COPY . .

CMD ["bash"]
