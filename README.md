# BCC-ICC-Ep02
Trabalho para a disciplina de Introdução a Computação Cientifica do curso de Ciencia da Computação - UFPR 24/01
Alunos:
Davi Garcia Lazzarin

ESTRUTURA DOS ARQUIVOS:

Codigos C:
- matmult.c: Contem o main e faz as chamadas de funcao para criacao e multiplicacoes.
- matriz.h: Define os cabecalhos das funcoes e constantes
- matriz.c: Contem as funcoes de alocacao e multiplicacao com e sem as tecnicas de otimizacao

Scrips Bash:
- executa.sh: script faz toda a rotina de testes e gera os arquivos do LIKWID.

outros arquivos:
- Makefile: Define regras de compilacao e ligacao
- entrada_matmult.in: Contem a ordem das matrizes/vetores que serao passado como parametro ao matmult
- matmult: Arquivo executavel do programa

Diretorios:
- logs/: Contem logs de compilacao
- utils-master/: Contem funcoes auxiliares


DECISOES DE PROJETO:

Definicoes de constantes:
- UF: Fator de Unroll = 4
- BK: Blocos = 8
- ALN: Alinhamento do aligned_alloc = 32

Sobre a execucao dos testes:
O matmult.c foi escrito separando os blocos de testes com e sem otimizacao.
O programa deve ser compilado com as flags:
_sem_otm_  -> para ser compilado com o trecho sem otimizacao  
_com_otm_  -> para ser compilado com o trecho com otimizacao

Ambos os trechos sao monitorados com o LIKWID, o trecho com otimizacao contem a string "OTMIZADO_" nos rotulos


