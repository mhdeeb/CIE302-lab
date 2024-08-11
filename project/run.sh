#!/bin/sh

script_path=$(readlink -f $(dirname $0))

cd $script_path

trap '' 10
trap '' 12
make clean
make all && cd bin && clear && ./main
