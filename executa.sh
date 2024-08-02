#!/bin/bash

# Algumas Variáveis
METRICA="FLOPS_DP"
PROGRAMA="matmult"
ENTRADA="entrada_matmult.in"

# Pega o CPU com maior ordem e salva em CPU
CPUORDEM=$(echo | likwid-topology -c | grep Processors: )
declare -i cpu_cont=${#CPUORDEM}-3
CPU=$(echo ${CPUORDEM} | cut -c ${cpu_cont} )

# Usar na ZARA
CPU=15

# Compila
make -B

# Define o modo de desempenho
echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

# Loop para percorrer o arquivo de entrada
while IFS= read -r x
do
    for k in $METRICA
    do
        likwid-perfctr -C ${CPU} -g ${k} -m ./$PROGRAMA ${x}
    done
done < "$ENTRADA"

# Define o modo de economia de energia
echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

# Limpa os arquivos .o
make clean

