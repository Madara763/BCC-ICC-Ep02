#include <stdio.h>
#include <stdlib.h>    /* exit, malloc, calloc, etc. */
#include <string.h>
#include <getopt.h>    /* getopt */
#include <time.h>
#include <likwid.h>

#include "matriz.h"
#include "utils-master/utils/utils.h"

/*
 Exibe mensagem de erro indicando forma de uso do programa e termina
  o programa.
*/

static void usage(char *progname)
{
  fprintf(stderr, "Forma de uso: %s [ <ordem> ] \n", progname);
  exit(1);
}

/*
 * Programa principal
 * Forma de uso: matmult [ -n <ordem> ]
 * -n <ordem>: ordem da matriz quadrada e dos vetores
 */

int main (int argc, char *argv[]) 
{
  int n=DEF_SIZE;
  rtime_t time;
  MatRow mRow_1, mRow_2, resMat;
  Vetor vet, res;
  
  /* =============== TRATAMENTO DE LINHA DE COMANDO =============== */

  if (argc < 2)
    usage(argv[0]);

  n = atoi(argv[1]);
  
  /* ================ FIM DO TRATAMENTO DE LINHA DE COMANDO ========= */

  /* ================    INICIO EXECUCAO SEM OTIMIZACAO     ========= */
  #ifdef _SEM_OTM_

  srandom(20232);
      
  res = geraVetor (n, 0); 
  resMat = geraMatRow(n, n, 1);    
  mRow_1 = geraMatRow (n, n, 0);
  mRow_2 = geraMatRow (n, n, 0);
  vet = geraVetor (n, 0);

  if (!res || !resMat || !mRow_1 || !mRow_2 || !vet) {
    fprintf(stderr, "Falha em alocação de memória !!\n");
    liberaVetor ((void*) mRow_1);
    liberaVetor ((void*) mRow_2);
    liberaVetor ((void*) resMat);
    liberaVetor ((void*) vet);
    liberaVetor ((void*) res);
    exit(2);
  }
    
  #ifdef _DEBUG_
    prnMat (mRow_1, n, n);
    prnMat (mRow_2, n, n);
    prnVetor (vet, n);
    printf ("=================================\n\n");
  #endif /* _DEBUG_ */
  //inicio monitoramento MatVet
  time=timestamp();
  LIKWID_MARKER_INIT;
  LIKWID_MARKER_START ("multMatVet");

  multMatVet (mRow_1, vet, n, n, res);
  
  LIKWID_MARKER_STOP ("multMatVet");
  LIKWID_MARKER_CLOSE;
  time = (timestamp() - time);
  //fim MatVet

  printf("mult Mat x Vet n=%d: %.5lfms\n", n, time);
  
  //inicio monitoramento MatMat
  time=timestamp();
  LIKWID_MARKER_INIT;
  LIKWID_MARKER_START ("multMatMat");
  
  multMatMat (mRow_1, mRow_2, n, resMat);
 
  LIKWID_MARKER_STOP ("multMatMat");
  LIKWID_MARKER_CLOSE;  
  time = (timestamp() - time);
  //fim MatMat

  printf("mult Mat x Mat n=%d: %.5lfms\n", n, time);

  #ifdef _DEBUG_
    prnVetor (res, n);
    prnMat (resMat, n, n);
  #endif /* _DEBUG_ */

  liberaVetor ((void*) mRow_1);
  liberaVetor ((void*) mRow_2);
  liberaVetor ((void*) resMat);
  liberaVetor ((void*) vet);
  liberaVetor ((void*) res);
  
  #endif /* _SEM_OTM_  */

  /* ================    INICIO EXECUCAO COM OTIMIZACAO     ========= */
  #ifdef _COM_OTM_
 
  srandom(20232);

  res = otm_geraVetor (n, 0);
  resMat = otm_geraMatRow(n, n, 1);
  mRow_1 = otm_geraMatRow (n, n, 0);
  mRow_2 = otm_geraMatRow (n, n, 0);
  vet = otm_geraVetor (n, 0);

  if (!res || !resMat || !mRow_1 || !mRow_2 || !vet) {
    fprintf(stderr, "Falha em alocação de memória !!\n");
    liberaVetor ((void*) mRow_1);
    liberaVetor ((void*) mRow_2);
    liberaVetor ((void*) resMat);
    liberaVetor ((void*) vet);
    liberaVetor ((void*) res);
    exit(2);
  }

  #ifdef _DEBUG_
    prnMat (mRow_1, n, n);
    prnMat (mRow_2, n, n);
    prnVetor (vet, n);
    printf ("OTM=================================\n\n");
  #endif /* _DEBUG_ */
  //inicio monitoramento Otm_MatVet
  time=timestamp();
  LIKWID_MARKER_INIT;
  LIKWID_MARKER_START ("OTIMIZADO_multMatVet");

  multMatVet (mRow_1, vet, n, n, res);

  LIKWID_MARKER_STOP ("OTIMIZADO_multMatVet");
  LIKWID_MARKER_CLOSE;
  time = (timestamp() - time);
  //fim OtmMatVet

  printf("OTM Mat x Vet n=%d: %.5lfms\n", n, time);

  //inicio monitoramento OtmMatMat
  time=timestamp();
  LIKWID_MARKER_INIT;
  LIKWID_MARKER_START ("OTIMIZADO_multMatMat");

  multMatMat (mRow_1, mRow_2, n, resMat);

  LIKWID_MARKER_STOP ("OTIMIZADO_multMatMat");
  LIKWID_MARKER_CLOSE;
  time = (timestamp() - time);
  //fim MatMat

  printf("OTM Mat x Mat n=%d: %.5lfms\n", n, time);

  #ifdef _DEBUG_
    prnVetor (res, n);
    prnMat (resMat, n, n);
  #endif /* _DEBUG_ */

  liberaVetor ((void*) mRow_1);
  liberaVetor ((void*) mRow_2);
  liberaVetor ((void*) resMat);
  liberaVetor ((void*) vet);
  liberaVetor ((void*) res);
  #endif /* _COM_OTM_  */
  
  return 0;
 
}

