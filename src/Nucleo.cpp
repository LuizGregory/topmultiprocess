/******************************************************
 * 
 *              UNIVATES 2009/A
 *      Engenharia da Computação
 *      Projeto Sistemas Operacionais - Trabalho 1
 *      MultiTop Process
 *
 * Autor : Luiz Gilberto Gregory Filho - 509921
 * Email : luiz.gregory@gmail.com
 *
 * Descrição do Arquivo: Neste arquivo estão implementados 
 *          metodos da classe núcleo.
 *          Resumindo, esta classe controla todo o programa.
 *
 * File Create: 02/04/2009
 *
 *****************************************************/

#include <stdio.h>
#include <iostream>
#include <regex.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#include "constants.h"
#include "Nucleo.h"
#include "Processo.h"
#include "displayControl.h"

using namespace std;


/**
 * Method Construtor
 */
Nucleo::Nucleo()
{
    this->clean();    
    this->readMemoryInfo();
}


/**
 * Este metodo adiciona os processos.
 * Este processo só sera adicionado ao nucleo se o mesmo for um processo válido
 *
 * @param char * PID
 */
int Nucleo::addProcess( char * PID , int delay)
{
    int x;

    if(!this->checkValidPID(PID))
    {
        return 0;
    }    
    if(!this->checkProcessExists(PID))
    {
        return 0;
    }

    for(x = 0; x < MAX_PROCESS; x++)
    {
        if(this->MyProcess[x] == NULL)
        {
            this->MyProcess[x] = new Processo(PID);
            this->MyProcess[x]->setUpdateDelay(delay);
            this->nProc++;
            return 1;
        }
    }
    
    return 0; 
}




/**
 * Este é um metodo que nao faz parte do OBJETO NUCLEO.
 * este metodo é chamado por uma thread que fica atualizando ium determinado processo.
 *
 * @param Processo * _obj
 */
void *_threadStart( void* _obj )
{
    Processo * P = (Processo*)_obj;
    P->update();          
    pthread_exit((void*) 0);
}



/**
 * Este é um metodo que nao faz parte do OBJETO NUCLEO.
 * este metodo é chamado por uma thread que controla a exibiçao de conteudo na tela.
 *
 * @param Nucleo * _obj
 */
void *_threadStartDisplayControl( void* _obj )
{
    Nucleo * N = (Nucleo*)_obj;           
    int x,
        line        = 0, 
        cW          = 18,
        procDisplay = 0,
        width       = 150;    
        
    char *columns[5];
        
    columns[0] = "PID";
    columns[1] = "Name";
    columns[2] = "Status";
    columns[3] = "CPU %";            
    columns[4] = "Memory %";      
        
    printf("\n");
    clrscr();
    usleep(100);                        
        
    while(true)
    {   
        printLineStr(++line, 1, width);
        gotoxy      (++line, 1);
        cout << "| Total Mem: "<< N->totalMemSize << "kB ";
        gotoxy      (++line, 1);
        cout << "| Free Mem: "<< N->freeMemSize << "kB";        
        printLineStr(++line, 1, width);
        printColumns(columns, cW, ++line, 1);        
        printLineStr(++line, 1, width);        
    
        procDisplay = 0;
        
        for(x = 0; x < MAX_PROCESS; x++)
        { 
            if(N->MyProcess[x] == NULL)
            {
                continue;
            }                                                         
            
            cleanLine   (++line);
            gotoxy(line, 1);            
            cout << "| " << N->MyProcess[x]->getPID();
            gotoxy(line, 1+ (cW*1));
            cout << "| " << N->MyProcess[x]->name;
            gotoxy(line, 1+ (cW*2));
            cout << "| " << N->MyProcess[x]->stateStr;     
            gotoxy(line, 1+ (cW*3));            
            cout << "| " << (float)N->MyProcess[x]->cpuPercent << "%";
            gotoxy(line, 1+ (cW*4));                        
            cout << "| " << (float)(N->MyProcess[x]->resident * 100) / N->totalMemSize << "%";                         
            gotoxy(line, 1+ (cW*5));                        
            cout << "| Atualiz:" << N->MyProcess[x]->timeUpdated;
            gotoxy(line, 17+ (cW*5));   
            cout << "| Sleep: " << N->MyProcess[x]->getUpdateDelay();
            
            if(++procDisplay > N->nProc)
            {                
                break;
            }
        }                 
        
        // FOOTER
        printLineStr    (++line, 1, width);
        cleanLine       (++line);
        gotoxy          (  line, 1);
        cout            << "|";
        gotoxy          (  line, 3);
        printCurrentTime();       
        printLineStr    (++line, 1, width);        
        printf          ("\n");                
        
        // DISPLAY DELAY
        usleep(75000);
          
        line = 0;               
        N->readMemoryInfo();
    }

    pthread_exit((void*) 0);
}


/**
 * Este Metodo inicia as threads.
 * para cada processo ele cria uma thread.
 * depois de criar as threads para os processos, é criada uma thread para controle 
 * de exibiçao das informaçoes na tela.
 */
void Nucleo::startThreads()
{
    void        *status; 
    int         procDisplay = 0;
    pthread_t   ThreadDisplay; 

    // verifica se tem algum processo válido
    if(this->nProc == 0)
    {
        return;
    }

    pthread_attr_init(&this->threadAttr);
    pthread_attr_setdetachstate(&this->threadAttr, PTHREAD_CREATE_JOINABLE);

    // CRIA AS THREADS PARA OS PROCESSOS
    for(this->currentProcess = 0; this->currentProcess < MAX_PROCESS; this->currentProcess++)
    {
        if(this->MyProcess[this->currentProcess] != NULL)
        {
            pthread_create  ( &this->MyThread[this->currentProcess], &this->threadAttr, _threadStart, this->MyProcess[this->currentProcess]);
            
            procDisplay++;
            if(procDisplay > this->nProc)
            {                
                break;
            }
        }
    }    

    // CRIA A THREAD PARA CONTROLE DE CONTEUDO
    pthread_create  ( &ThreadDisplay, &this->threadAttr, _threadStartDisplayControl, this);    
    pthread_join    (  ThreadDisplay, &status);            


    // FAZ UM JOIN ENTRE AS THREADS CRIADAS
    for(this->currentProcess = 0; this->currentProcess < MAX_PROCESS; this->currentProcess++)
    {
        if(this->MyProcess[this->currentProcess] != NULL)
        {
            pthread_join    (  this->MyThread[this->currentProcess], &status);            
            
            procDisplay++;
            if(procDisplay > this->nProc)
            {                
                break;
            }
        }
    }    

    pthread_attr_destroy(&this->threadAttr);    
    pthread_exit(NULL);
}



/**
 * Este metodo le o arquivo /proc/meminfo em busca da 
 * quantidade de memoria total da maquina.
 */
int Nucleo::readMemoryInfo()
{
    FILE *  FileID;
    int     pathSize = (strlen(PROC_DIR)+1)+strlen("meminfo")+1;
    char    filePath[pathSize];
	
	//trabalha string com caminho do arquivo
    snprintf(filePath, pathSize, "%s/meminfo", PROC_DIR);           
        
    //abre o arquivo para leitura    
	if((FileID = fopen(filePath, "r")) == NULL )
	{
		return 0;		
	}

	//scaneia o arquivo
    fscanf 
    (
        FileID, 
        "MemTotal: %d kB\n"
        "MemFree: %d kB\n",
        &this->totalMemSize, &this->freeMemSize
    );      
    
    fclose(FileID);    
}




/**
 * Limpa a lista de Processos
 */  
void Nucleo::clean()
{
    int x;

    for(x = 0; x < MAX_PROCESS; x++)
    {
        this->MyProcess[x] = NULL;        
    }
    
    this->nProc = 0;   
}
 
 
 

/**
 * valida o nome de um processo com uma expressão regular
 * aceita somente numeros
 *
 * @param  char * PID  - Process ID
 * @return boolean
 */
int Nucleo::checkValidPID( char * PID )
{
	regex_t reg;

	if (regcomp(&reg , "[0-9]{1,}", REG_EXTENDED|REG_NOSUB) != 0)
	{
		return 0;
	}

	if ((regexec(&reg, PID, 0, (regmatch_t *)NULL, 0)) == 0)
	{
		return 1;
    }

    return 0;
}



/**
 * Este método checa se o processo solicitado existe.
 * Faz uma leitura no diretorio /proc em busca do processo
 *
 * @return boolean
 */
int Nucleo::checkProcessExists(char * PID )
{    
    struct dirent *ent;

    DIR *dir = opendir(PROC_DIR);

    if(!dir)    
    {
        printf("\nError opening directory!\n DirName: %s \n", PROC_DIR);
        return 0;
    }
    
    while((ent = readdir(dir)) != NULL)
    {
        if(strcmp(PID,ent->d_name) == 0)
        {
            return 1;
        }
    }

    return 0;
}


