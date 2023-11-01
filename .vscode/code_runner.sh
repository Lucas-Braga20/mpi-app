#!/bin/bash

general_path=$(echo "$1" | awk -F'/code/' '{print $2}')
dir_path=$(dirname "$general_path")
file_path=$(basename "$general_path")

access_dir="cd $dir_path"

start_lam="lamboot"

cpp_compile="mpiCC -I/usr/include/ -L/usr/lib/ $file_path -o ./exec -lmpich"

cpp_run="mpirun -np 4 ./exec"

docker-compose run mpi-app bash -c "$access_dir && $start_lam && $cpp_compile && $cpp_run"
