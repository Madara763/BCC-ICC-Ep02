#!/bin/bash

METRICA="FLOPS_DP"
PROGRAMA="matmult"
ENTRADA="entrada_matmult.in"

#pega o cpu com maior ordem e salva em CPU
CPUORDEM=$(echo | likwid-topology -c | grep Processors: )
declare -i cpu_cont=${#CPUORDEM}-3
CPU=$(echo ${CPUORDEM} | cut -c ${cpu_cont} )

make -B


LIKWID_CMD="likwid-perfctr -C $1 -g $2 -m"
shift 2


echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

${LIKWID_CMD} $*

# for x in $ENTRADA
# do
#     for k in $METRICA
#     do
#         likwid-perfctr -C ${CPU} -g ${k} -m ./$PROGRAMA <$x >>$PROGRAMA.log
#     done
# done

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

#make clean


