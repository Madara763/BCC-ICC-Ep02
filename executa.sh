#!/bin/bash

#VARIAVEIS
#METRICA="FLOPS_DP ENERGY CACHE ICACHE MEM1"
METRICA="FLOPS_DP"
PROGRAMA="matmult"
ENTRADA="entrada_matmult.in"
ARQSAIDA="saida_lkw.txt"

#Cria um arquivo de sda novo
rm ${ARQSAIDA} 
touch ${ARQSAIDA} 


#Pega o CPU com maior ordem e salva em CPU 
#Usar nos labs do dinf
CPUORDEM=$(echo | likwid-topology -c | grep Processors: )
declare -i cpu_cont=${#CPUORDEM}-3
CPU=$(echo ${CPUORDEM} | cut -c ${cpu_cont} )

#Usar na zara
#Usar em casa
CPU=15



#========EXECUCAO SEM OTM========#

#COMPILACAO
MAKEFLAGS="sem_otm"
make $MAKEFLAGS -B > logs/make.log

# Define o modo de desempenho
echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

# Loop para percorrer o arquivo de entrada
while IFS= read -r x
do
    for k in $METRICA
    do
        likwid-perfctr -C ${CPU} -g ${k} -m ./$PROGRAMA ${x} >> ${ARQSAIDA}
	echo Executando $PROGRAMA SEM OTM para $k com tamanho $x
    done
done < "$ENTRADA"

# Define o modo de economia de energia
echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

#LIMPEZA
make clean >> logs/make.log


#========EXECUCAO COM OTM========#

#COMPILACAO
MAKEFLAGS="com_otm"
make $MAKEFLAGS -B >> logs/make.log

# Define o modo de desempenho
echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

# Loop para percorrer o arquivo de entrada
while IFS= read -r x
do
    for k in $METRICA
    do
        likwid-perfctr -C ${CPU} -g ${k} -m ./$PROGRAMA ${x} >> ${ARQSAIDA}
        echo Executando $PROGRAMA COM OTM para $k com tamanho $x
    done
done < "$ENTRADA"

# Define o modo de economia de energia
echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

#LIMPEZA
make clean >> logs/make.log


#========FORMATA A SAIDA========#

cat ${ARQSAIDA}
