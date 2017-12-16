CC=mpic++


all: Topologie
    mpirun -np 5 --hostfile hostfile  topologi 5

Topologie:
    ${CC} -std=c++11 -o topologi Topologie.cpp