/******************************************************
 * 
 *              UNIVATES 2009/A
 *
 *      Projeto Sistemas Operacionais - Trabalho 1
 *      MultiTop Process
 *
 * Autor : Luiz Gilberto Gregory Filho - 509921
 *
 * Este arquivo declara a classe processo.
 * Cada processo é visto com um objeto diferente pelo sistema.
 *
 * File Create: 02/04/2009
 *
 *****************************************************/
#ifndef PROCESSO_H_
#define PROCESSO_H_

#include <pthread.h>

class Processo
{
    public:  
        // ATTRIBUTES
        char *  PID;
        int     intPID;
        int     intParentPID; 
        char    name                [100];
        char    state               [100];
        char    stateStr            [100];                
        int     size;       
        int     resident;
        int     share;
        int     text;                
        int     data;
        int     lib;
        int     dt;
        float   jiffies;
        float   lastJiffies;        
        unsigned long utime;
        unsigned long stime;
        float   cpuPercent;
        float   updateDelay;
        int    timeUpdated;

        // Class Construct
        Processo( char * PID );  

        //methods
        char    *getPID         ();
        void    update          ();
        void    readProcess     ();
        int     readStatusFile  ();        
        int     readStatFile    ();   
        int     readStatmFile   ();
        float   getUpdateDelay  ();
        float   setUpdateDelay  ( int delayTime );
};

#endif
