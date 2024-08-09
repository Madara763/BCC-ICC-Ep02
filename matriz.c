#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> // Para uso de função 'memset()'

#include "matriz.h"

/**
 * Função que gera valores para para ser usado em uma matriz
 * @param i,j coordenadas do elemento a ser calculado (0<=i,j<n)
*  @return valor gerado para a posição i,j
  */
static inline real_t generateRandomA( unsigned int i, unsigned int j)
{
  static real_t invRandMax = 1.0 / (real_t)RAND_MAX;
  return ( (i==j) ? (real_t)(BASE<<1) : 1.0 )  * (real_t)random() * invRandMax;
}

/**
 * Função que gera valores aleatórios para ser usado em um vetor
 * @return valor gerado
 *
 */
static inline real_t generateRandomB( )
{
  static real_t invRandMax = 1.0 / (real_t)RAND_MAX;
  return (real_t)(BASE<<2) * (real_t)random() * invRandMax;
}



/* ----------- FUNÇÕES ---------------- */

/**
 *  Funcao geraMatRow: gera matriz como vetor único, 'row-oriented'
 * 
 *  @param m     número de linhas da matriz
 *  @param n     número de colunas da matriz
 *  @param zerar se 0, matriz  tem valores aleatórios, caso contrário,
 *               matriz tem valores todos nulos
 *  @return  ponteiro para a matriz gerada
 *
 */

MatRow geraMatRow (int m, int n, int zerar)
{
  MatRow matriz = (real_t *) malloc(m*n*sizeof(real_t));

  if (matriz) {
    if (zerar)
      memset(matriz,0,m*n*sizeof(real_t));
    else
      for (int i=0; i < m; ++i)
	for (int j=0; j < n; ++j)
	  matriz[i*n + j] = generateRandomA(i, j);
  }
  
  return (matriz);
}


/**
 *  Funcao geraVetor: gera vetor de tamanho 'n'
 * 
 *  @param n  número de elementos do vetor
 *  @param zerar se 0, vetor  tem valores aleatórios, caso contrário,
 *               vetor tem valores todos nulos
 *  @return  ponteiro para vetor gerado
 *
 */

Vetor geraVetor (int n, int zerar)
{
  Vetor vetor = (real_t *) malloc(n*sizeof(real_t));

  if (vetor) {
    if (zerar)
      memset(vetor,0,n*sizeof(real_t));
    else
      for (int i=0; i < n; ++i)
	vetor[i] = generateRandomB();
  }
  
  return (vetor);
}

/**
 *  \brief: libera vetor
 * 
 *  @param  ponteiro para vetor
 *
 */
void liberaVetor (void *vet)
{
	free(vet);
}


/**
 *  Funcao multMatVet:  Efetua multiplicacao entre matriz 'mxn' por vetor
 *                       de 'n' elementos
 *  @param mat matriz 'mxn'
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *  @param res vetor que guarda o resultado. Deve estar previamente alocado e com
 *             seus elementos inicializados em 0.0 (zero)
 *  @return vetor de 'm' elementos
 *
 */

void multMatVet (MatRow mat, Vetor v, int m, int n, Vetor res)
{
    
  /* Efetua a multiplicação */
  if (res) {
    for (int i=0; i < m; ++i)
      for (int j=0; j < n; ++j)
        res[i] += mat[n*i + j] * v[j];
  }
}


/**
 *  Funcao multMatMat: Efetua multiplicacao de duas matrizes 'n x n' 
 *  @param A matriz 'n x n'
 *  @param B matriz 'n x n'
 *  @param n ordem da matriz quadrada
 *  @param C   matriz que guarda o resultado. Deve ser previamente gerada com 'geraMatPtr()'
 *             e com seus elementos inicializados em 0.0 (zero)
 *
 */

void multMatMat (MatRow A, MatRow B, int n, MatRow C)
{

  /* Efetua a multiplicação */
  for (int i=0; i < n; ++i)
    for (int j=0; j < n; ++j)
      for (int k=0; k < n; ++k)
	C[i*n+j] += A[i*n+k] * B[k*n+j];
}


/**
 *  Funcao prnMat:  Imprime o conteudo de uma matriz em stdout
 *  @param mat matriz
 *  @param m número de linhas da matriz
 *  @param n número de colunas da matriz
 *
 */

void prnMat (MatRow mat, int m, int n)
{
  for (int i=0; i < m; ++i) {
    for (int j=0; j < n; ++j)
      printf(DBL_FIELD, mat[n*i + j]);
    printf("\n");
  }
  printf(SEP_RES);
}

/**
 *  Funcao prnVetor:  Imprime o conteudo de vetor em stdout
 *  @param vet vetor com 'n' elementos
 *  @param n número de elementos do vetor
 *
 */

void prnVetor (Vetor vet, int n)
{
  for (int i=0; i < n; ++i)
    printf(DBL_FIELD, vet[i]);
  printf(SEP_RES);
}


/* ----------- FUNÇÕES OTIMIZADAS---------------- */

MatRow otm_geraMatRow (int m, int n, int zerar){
  MatRow matriz = (real_t *) aligned_alloc(ALN, m*n*sizeof(real_t));

  if (matriz) {
    if (zerar)
      memset(matriz,0,m*n*sizeof(real_t));
    else
      for (int i=0; i < m; ++i)
        for (int j=0; j < n; ++j)
          matriz[i*n + j] = generateRandomA(i, j);
  }

  return (matriz);
}


Vetor otm_geraVetor (int n, int zerar){
  Vetor vetor = (real_t *) aligned_alloc(ALN, n*sizeof(real_t));

  if (vetor) {
    if (zerar)
      memset(vetor,0,n*sizeof(real_t));
    else
      for (int i=0; i < n; ++i)
    vetor[i] = generateRandomB();
  }

  return (vetor);
}

void otm_multMatVet (MatRow restrict mat, Vetor restrict v, int m, int n, Vetor restrict res){
  if (!res) return;

  //declaracao variaveis 
  int l_iniBloco = 0, l_fimBloco = 0, c_iniBloco = 0, c_fimBloco = 0;
  int aux = 0;

  //Blocking com BK definido no .h
  for (int iBloco = 0; iBloco < m / BK; ++iBloco) {
    l_iniBloco = BK * iBloco;
    l_fimBloco = BK + l_iniBloco;

    for (int jBloco = 0; jBloco < n / BK; ++jBloco) {
      c_iniBloco = BK * jBloco;
      c_fimBloco = BK + c_iniBloco;

      //Unroll com UF definido no .h
      for (int i = l_iniBloco; i < l_fimBloco; i += UF){
        aux = (i * n);
        for (int j = c_iniBloco; j < c_fimBloco; ++j) {
          res[i]   += mat[aux + j] * v[j];
          res[i+1] += mat[aux + n + j] * v[j];
          res[i+2] += mat[aux + n + n + j] * v[j];
          res[i+3] += mat[aux + n + n + n + j] * v[j];
        }
      }//fim unroll
    }
  }//fim block

  //Se nao sobrou bloco incompleto finaliza
  if (! m % BK)
    return;
  //Executa para os meio blocos que restaram
  for (int i = m - (m % BK); i < m; ++i) {
    for (int j = 0; j < n; j++) {
      res[i] += mat[(i * n) + j] * v[j];
    }
  }
  for (int i = 0; i < n - (n % BK); ++i) {
    for (int j = n - (n % BK); j < n; ++j) {
      res[i] += mat[(i * n) + j] * v[j];
    }
  }

  return;
}//fim otm_multMatVet


void otm_multMatMat (MatRow restrict A, MatRow restrict B, int n, MatRow restrict C)
{
  int l_iniBloco, l_fimBloco;
  int c_iniBloco, c_fimBloco;
  int x_ini, x_fim;
  int ind_A = 0, ind_B = 0, ind_C = 0; //ind_C eh um in-di-ce :D (os outros tbm sao)

  //bloco i
  for (int iBloco = 0; iBloco < n / BK; ++iBloco) {
    l_iniBloco = iBloco * BK;
    l_fimBloco = l_iniBloco + BK;
    
    //bloco j
    for (int jBloco = 0; jBloco < n / BK; ++jBloco) {
      c_iniBloco = jBloco * BK;
      c_fimBloco = c_iniBloco + BK;

      for (int k = 0; k < n / BK; ++k) {
        x_ini = k * BK;
        x_fim = x_ini + BK;

        for (int i = l_iniBloco; i < l_fimBloco; ++i) {
          for(int j = c_iniBloco; j < c_fimBloco; j += UF) {
            ind_C = i*n+j;

            for (int x = x_ini; x < x_fim; ++x) {
              ind_A = (ind_C - j) + x;
              ind_B = (x * n) + j;
              C[ind_C + 0] += A[ind_A] * B[ind_B + 0];
              C[ind_C + 1] += A[ind_A] * B[ind_B + 1];
              C[ind_C + 2] += A[ind_A] * B[ind_B + 2];
              C[ind_C + 3] += A[ind_A] * B[ind_B + 3];
            }//fim for x
          }//fim for j
        }//fim for i
      }//fim for k
    }//fim for bloco J
  }//fim for bloco i

  //Se nao sobrou bloco incompleto finaliza  
  if (! (n % BK))
    return;

  //Faz pro bloco restante
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      for (int k = 0; k < n; ++k)
        C[i * n + j] += A[i * n + k] * B[k * n + j];

  return;
}

