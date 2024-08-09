# Define o tipo de terminal (por exemplo, png)
set terminal pngcairo size 800,600 enhanced font "Arial,10"

# Define o nome do arquivo de saída
set output 'flops_dp.png'

# Define o título do gráfico
set title "FLOPS_DP for Core 15"

# Rótulos dos eixos
set xlabel "Core"
set ylabel "DP MFLOP/s"

# Configuração do estilo de linha
set style data histograms
set style histogram cluster gap 1
set style fill solid 1.00 border -1
set boxwidth 0.9

# Dados diretamente embutidos
plot '-' using 2:xtic(1) title "DP MFLOP/s" linecolor rgb "blue"

# Dados para o gráfico
"Core 15" 2266.1007
e

