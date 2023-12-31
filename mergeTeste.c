/** Este exemplo gera uma linha da tabela no formato usado para o projeto.
 * A tabela/planilha é:
 * acessível para logins da USP.
 * O resultado da execução são os tempos de execução, então, caso prefira não
 * portar o código do teste para a linguagem que estiver usando, e, ao invés:
 * gerar os arrays iguais, registrar os tempos e lançar os valores manualmente
 * na planilha, esta alternativa será aceita.
 */
/**
 * A partir do código do RNG, enxertei outros códigos para gerar uma linha da tabela no formato certo.
 * Em princípio, deve funcionar simplesmente copiar do terminal (cmd, konsole,...) e colar 
 * na planilha aberta no navegador.
 * Além do RNG, há uma seção para definir as características do hardware.
 * O algoritmo de ordenação pode, inclusive, ser codificado dentro do método executaTeste, 
 * embora isso vá deixar o código mais bagunçado. Isto não é tão problemático 
 * pois o resultado da execução são os tempos de execução.
 */
/**
RNG especificado em Java 8 portado para C.
https://docs.oracle.com/javase/8/docs/api/java/util/Random.html
* O long em Java tem 64 bits (https://www.w3schools.com/java/java_data_types.asp)
* O tipo de dados inteiro 64 bits em C é long long (https://gcc.gnu.org/onlinedocs/gcc/Long-Long.html)
*/

#include <stdio.h>
#include <stdlib.h>

unsigned long long seed;

void setSeed (long long newSeed) {
  seed=(newSeed ^ 0x5DEECE66DLL) & ((1LL << 48) - 1);
}
//  seed=newSeed;


int next(int bits){
 seed=(seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
 return (int)(seed >> (48 - bits));
}

/**** Até aqui é igual ao rng-java8.c 
 * A partir daqui é a codificação do sort (se quiser fazer neste arquivo) e 
 * a execução dos testes. *****/

#include <sys/time.h>  // gettimeofday
#include <unistd.h>    // usleep

int DEBUG;

int *A;

int comp (const void *a, const void *b){
	return (*(int*)a)>(*(int*)b);
}


#define SEU_NUSP 12717266   
#define CPU "https://www.cpubenchmark.net/cpu.php?cpu=Intel+Core+i5-8400+%40+2.80GHz&id=3097"
#define PASSMARK 9218
#define MEMORIA_RAM  "DDR4"                     
#define FREQUENCIA_DE_OPERACAO_DA_MEMORIA 1600   
#define QUANTIDADE_DE_MEMORIA_RAM  8    //GBytes
#define QUANTIDADE_DE_PENTES_DE_MEMORIA  1       
#define SO "Linux edgar-Lenovo-IdeaPad-S145-15IWL 5.11.0-43-generic #47~20.04.2-Ubuntu SMP Mon Dec 13 11:06:56 UTC 2021 x86_64 x86_64 x86_64 GNU/Linux"
#define OBS  ""


/* Descomente o define do nome do algoritmo que usou. */
//#define ALGORITMO "QUICKSORT"
#define ALGORITMO "MERGESORT"
//#define ALGORITMO "HEAPSORT"
//#define ALGORITMO "RADIXSORT"

void identifica (void) {
    int seuHashCode;
    /** O hashCode é o terceiro número aleatório gerado a partir da 
     * inicialização do RNG com seu NUSP.
     */ 
    setSeed(SEU_NUSP);
    next(31);
    next(31);
    seuHashCode=next(31);   // calcula um código baseado no seu nusp, 
                            // para deixar pouco provável que alguém 
                            // que não tenha seu NUSP consiga obtê-lo.
#ifdef CPU
    printf ("%d\t %s\t %d\t %s\t %d\t %d\t %d\t %s\t %s\t ", seuHashCode, CPU, PASSMARK, MEMORIA_RAM, 
            FREQUENCIA_DE_OPERACAO_DA_MEMORIA, QUANTIDADE_DE_MEMORIA_RAM, QUANTIDADE_DE_PENTES_DE_MEMORIA, SO, ALGORITMO);
#endif
#ifdef PHONE_MODEL
    printf ("%d\t %s\t %d\t %s\t %d\t %d\t %d\t %s\t %s\t ", seuHashCode, PHONE_MODEL, PASSMARK, "NA", 
            -1, QUANTIDADE_DE_MEMORIA_RAM, -1, SO, ALGORITMO);
#endif

}
//////////////////////////////////////MERGESORT////////////////////////////////////////////////////////////
//////////////////////////////////////MERGESORT////////////////////////////////////////////////////////////
//////////////////////////////////////MERGESORT////////////////////////////////////////////////////////////

void intercala(int p, int q, int n, int v[]){
  int *aux = (int *) calloc(n - p, sizeof(int));


  int i, j, k;
  for (i = p, j = q, k = 0; i < q && j < n; k++){
    
    if (v[i] <= v[j]){
      aux[k] = v[i];
      i++;
      
    } else {
      aux[k] = v[j];
      j++;

    }
  }

  for (;i < q; i++, k++){
    aux[k] = v[i];
  }

  for (;j < n; j++, k++){
    aux[k] = v[j];
  }

  for (i = p; i < n; i++){
    v[i] = aux[i - p];
  }

  free(aux);
}

void mergesort(int v[], int p, int n){
  if (p < n-1){
    int q = (p + n)/2;
    mergesort(v, p, q);
    mergesort(v, q, n);
    intercala(p, q, n, v); 
  }
}



/** a medida de tempo é baseada em
 * https://levelup.gitconnected.com/8-ways-to-measure-execution-time-in-c-c-48634458d0f9
 * método 3.
 * a chamada de quicksort é baseada em: https://www.cplusplus.com/reference/cstdlib/qsort/
 */
int nTestesDistintos=11;
int N[]={1e6, 2e6, 5e6, 1e7, 2e7, 5e7, 1e8, 2e8, 5e8, 1e9, 2e9};
int sementes[]={0, 314, 7676, 202, 457, 1000, 9070140, 2394587, 5827934, 934875, 42};
// Quando testei, os tempos em segundos, para ter uma idéia.
double tempos[]={0.181221, 0.366232, 0.981735, 2.053736, 4.289579, 11.302959, 23.535343, 48.893509, 128.680322, 150.200411, 1284.245983}; 

int *A;
int n;

void preparaTeste (int iTeste) {
    // Preparação
    n=N[iTeste];
    setSeed(sementes[iTeste]);    // usar a mesma semente no seu teste.
    A=(int *) malloc(n*sizeof(int));
    if (A==NULL) {
		printf ("Teste %d: Não consegui alocar array de %d elementos - pulando teste.", 
		      iTeste, N[iTeste]);
		return;
	}
    for (int i=0;i<n;i++) {
		A[i]=next(31);
	}
	/** Para checar facilmente se o array foi gerado corretamente, 
	 * verificar se o último elemento gerado é igual ao último elemento gerado
	 * pelo teste.
	 */
	printf ("%d\t ", A[n-1]);
}

void executaTeste (int iTeste) {

    if (DEBUG) {
		/** As mensagens de depuração são incluidas para sua orientação.
		 * Na execução "pra valer", devem ser removidas.
		 */
		printf ("Este teste levou %lf segundos para executar no computador que" 
		" usei para criar o EP.", tempos[iTeste]);
    }
    // Start measuring time
    struct timeval begin, end;
    gettimeofday(&begin, 0);

	mergesort(A, 0 ,n); //CHAMADA MERGESORT

    // SUBSTITUA ESTA CHAMADA COM A CHAMADA CORRESPONDENTE DO SEU ALGORITMO DE ORDENAÇÃO.
    
    
    // Stop measuring time and calculate the elapsed time
    // Ao invés de apresentar o tempo em segundos, apresentei o tempo 
    // em microssegundos pois para copiar do terminal e colar na planilha do
    // Google, a decisão sobre qual é o separador decimal é baseado no LOCALE.
    // Dependendo do ajuste, os números em ponto flutuante são colados errado.
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    long elapsed = seconds*1e6 + microseconds;

    if (DEBUG) {
        printf("Time measured: %ld seconds.\n", elapsed);
    } else {
        printf("%ld\t ", elapsed);
    }
}

void finalizaTeste(void) {
	free (A);   // TINHA ESQUECIDO ISTO!!!
	
	// Dois mimos
	fflush(NULL); // força o envio das mensagens para a tela (https://www.gnu.org/software/libc/manual/html_node/Flushing-Buffers.html)
	// https://www.vivaolinux.com.br/topico/C-C++/Sleep()-no-gcc
	usleep(100000); // Um mimo:
					// Para o processo por 0.1s para dar tempo de imprimir o que estiver no buffer.
}

void imprimeAjudaETermina (void) {
	puts ("BenchmarkSort {A, D,V,S} \n"
		   "Exemplo de programa para gerar benchmarks de \n"
		   "algoritmos de ordenação. Se usado sem modificação, gera \n"
		   "arrays com o RNG de Java 8 portado para C e gera benchmarks \n"
		   "sobre o <stdlib.h>qsort.");
	puts ("Use somente uma das opções: A D,V,S (maiúsculas).");
	puts ("- A (array) para executar, somente a geração do array. \n"
		  "Apresenta o valor do último elemento do array, para conferência."); 
	puts ("- S (short) para executar, do conjunto de testes, o \n"
		  "de menor tempo de execução em modo debug."); 
	puts ("- D (debug) para executar todo o conjunto de testes \n"
		  "com mensagens de depuração."); 
	puts ("- V (pra Valer) para executar, todo o conjunto de \n"
		  "testes e gerar a linha da planilha formatada para copiar e colar."); 
	puts ("O maior teste que consegui executar ordena 1G elementos\n"
		  "O teste de 2G elementos é abortado pelo SO."); 
	puts ("O código original deste teste (tenta) executa(r) até o teste de 2G \n"
		  "\n"); 
	exit (0);
}

void main(int argc, char *argv[]) {

  
	if (argc!=2) {
		imprimeAjudaETermina();
  
	}
	int nTestesAExecutar=nTestesDistintos;
	DEBUG=0;
	switch (*argv[1]) {
		case 'V':
		case 'A': break;
		case 'S': nTestesAExecutar=1;
		case 'D': DEBUG=1; break;
		default: imprimeAjudaETermina();
	}
	identifica();
	for (int i=0;i<nTestesAExecutar;i++) {
    	preparaTeste(i);
    	if (*argv[1] != 'A') {
        	executaTeste(i);
        } else {
			printf ("NA\t ");
		}
		finalizaTeste();
	}
	puts (OBS);
}
