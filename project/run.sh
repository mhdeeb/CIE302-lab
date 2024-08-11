#! /bin/sh

trap '' 10
trap '' 12
make clean
make all && cd bin && clear && ./main
