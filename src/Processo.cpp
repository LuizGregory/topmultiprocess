/******************************************************
 * 
 *              UNIVATES 2009/A
 *
 *      Projeto Sistemas Operacionais - Trabalho 1
 *      MultiTop Process
 *
 * Autor : Luiz Gilberto Gregory Filho - 509921
 *
 * Este arquivo implementa a classe Processo.
 * Cada processo é visto como um objeto diferente pelo sistema.
 *
 *
 * File Create: 02/04/2009
 *
 *****************************************************/

#include "Processo.h"
#include "constants.h"
#include "displayControl.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <pthread.h>
#include <sys/param.h>

#include <iostream>
#include <fstream>
#include <cmath>


using namespace std;


/**
 * Class Constructor
 *
 * @param int pid - (ID do processo)
 */
Processo::Processo( char * PID )
{
    this->PID           = PID;
    this->jiffies       = 0;
    this->lastJiffies   = 0;
    this->readProcess   ();
//    this->setUpdateDelay(1);       
}


/**
 * Retorna o PID do processo
 * @return char *
 */
char * Processo::getPID()
{
    return this->PID;
}


/**
 * Este método fica atualizando o "Processo".
 * este fica em um laço infinito que é chamado por uma thread. 
 */
void Processo::update()
{
    this->timeUpdated = 0;
    while(true)
    {   
        this->readProcess();
        sleep(this->getUpdateDelay()); 
        this->timeUpdated++;
    }    
}


/**
 * Retorna o tempo de atualizaçao do processo
 */
float Processo::getUpdateDelay()
{
    return this->updateDelay;
}


/**
 * Seta o tempo de atualizaçao do processo
 */
float Processo::setUpdateDelay( int delayTime )
{
    if(delayTime <= 0)
    {
        delayTime = 1;
    }
    if(delayTime >= 9)
    {
        delayTime = 9;
    }    

    this->updateDelay = delayTime;
}


/**
 * este metodo chama 3 metodos que leem o status do processo.
 * cada um dos 3 metodos é responsavel pela leitura de um arquivo diferente
 */
void Processo::readProcess()
{   
    this->readStatusFile();        
    this->readStatFile();   
    this->readStatmFile();
}


/**
 * Este metodo le o arquivo /proc/<pid>/status
 * A leitura deste arquivo retorna o nome e o status do processo
 */
int Processo::readStatusFile()
{
    FILE *  FileID;
    int     pathSize = (strlen(PROC_DIR)+1)+strlen(this->getPID())+8;
    char    filePath[pathSize]; 

	//trabalha string com caminho do arquivo
    snprintf(filePath, pathSize, "%s/%s/status", PROC_DIR, this->getPID());           
        
    //abre o arquivo para leitura    
	if((FileID = fopen(filePath, "r")) == NULL )
	{
		return 0;		
	}
	
	//scaneia o arquivo
    fscanf 
    (
        FileID, 
        "Name: %s\nState: %s %s\n",
        &this->name, &this->state, &this->stateStr
    );
    fclose(FileID); 
    
    return 1;
}



/**
 * Este metodo le o arquivo /proc/<pid>/stat
 * a leitura deste arquivo serve para calcular o percentual de uso do processador
 * pelo processo.
 */
int Processo::readStatFile()
{
    FILE *  FileID;
    int     pathSize = (strlen(PROC_DIR)+1)+strlen(this->getPID())+8;
    char    filePath[pathSize]; 
      
    unsigned long   luNull;
    int             dNull;
    char            cNull[100];

	//trabalha string com caminho do arquivo
    snprintf(filePath, pathSize, "%s/%s/stat", PROC_DIR, this->getPID());  
                     
    //abre o arquivo para leitura    
	if((FileID = fopen(filePath, "r")) == NULL )
	{
		return 0;		
	}

    fscanf
    (
        FileID, 
        "%d %s %c %d "
        "%d %d %d %d "
        "%lu %lu %lu %lu "
        "%lu %lu %lu",
        &dNull, &cNull, &cNull, &dNull,
        &dNull, &dNull, &dNull, &dNull,
        &luNull, &luNull, &luNull, &luNull,
        &luNull, &this->utime, &this->stime
    );
    fclose(FileID);     

    float totalTime = (float) this->utime + this->stime;   
    this->jiffies = totalTime - this->lastJiffies;
    this->lastJiffies = totalTime;      
           
    //CALCULA PERCENTUAL DE PROCESSAMENTO
    this->cpuPercent = (float)((this->jiffies / (HZ * this->getUpdateDelay())) * 100);    
    
    return 1;  
}



/**
 * Este metodo le o arquivo /proc/<pid>/statm
 * a leitura deste arquivo serve para buscar a quantidade de memoria utilizado
 * pelo processo.
 */
int Processo::readStatmFile()
{
    FILE *  FileID;
    int     pathSize = (strlen(PROC_DIR)+1)+strlen(this->getPID())+8;
    char    filePath[pathSize]; 
      
	//trabalha string com caminho do arquivo
    snprintf(filePath, pathSize, "%s/%s/statm", PROC_DIR, this->getPID());           
        
    //abre o arquivo para leitura    
	if((FileID = fopen(filePath, "r")) == NULL )
	{
		return 0;		
	}    
	
    fscanf
    (
        FileID, 
        "%d %d %d %d %d %d %d",
        &this->size,
        &this->resident,
        &this->share,
        &this->text,
        &this->lib,
        &this->data,
        &this->dt
    );
    fclose(FileID); 

    this->resident = this->resident * PAGE_SIZE; 
    
    return 1;
}



