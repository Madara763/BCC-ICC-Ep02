#gerar "warnings" detalhados e infos de depuração
CC= gcc
CFLAGS = -O3 -mavx2 -march=native -I${LIKWID_HOME}/include -DLIKWID_PERFMON
LDLIBS = -L${LIKWID_HOME}/lib -llikwid
objs = matmult.o matriz.o utils.o
PROGS = matmult

# regra default (primeira regra)
all:	$(PROGS)
 
# regras de ligacao
matmult:	$(objs)
 
# regras de compilação
matmult.o:	matmult.c
matriz.o:	matriz.c
#regras especificas para  libs em outros caminhos
utils.o:	utils-master/utils/utils.c
	gcc $(CFLAGS) -c -o utils.o utils-master/utils/utils.c

#flags de debug
debug:	CFLAGS += -g -D_DEBUG_
debug:	$(PROGS)

# remove arquivos temporários
clean:
	-rm -f $(objs) *~
 
# remove tudo o que não for o código-fonte
purge: clean
	-rm -f matmult