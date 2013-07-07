/******************************************************
 * 
 *              UNIVATES 2009/A
 *
 *      Projeto Sistemas Operacionais - Trabalho 1
 *      MultiTop Process
 *
 * Autor : Luiz Gilberto Gregory Filho - 509921
 *
 * Neste arquivo são declaradas constantes utilizadas
 * no sistema.
 *
 * File Create: 02/04/2009
 *
 *****************************************************/

// define o caminho dos processos
#define PROC_DIR "/proc"

//define o numero maximo de processos a exibir
#define MAX_PROCESS 50

//page size
#define PAGE_SIZE ( sysconf(_SC_PAGESIZE) / 1024 )
