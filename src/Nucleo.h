/******************************************************
 * 
 *              UNIVATES 2009/A
 *
 *      Projeto Sistemas Operacionais - Trabalho 1
 *      MultiTop Process
 *
 * Autor : Luiz Gilberto Gregory Filho - 509921
 *
 * Este arquivo declara a classe núcleo, que controla o sistema.
 *
 * File Create: 02/04/2009
 *
 *****************************************************/

#ifndef NUCLEO_H_
#define NUCLEO_H_

#include <pthread.h>
#include "Processo.h"
#include "constants.h"

class Nucleo
{
    private :    
        // ATTRIBUTES    
        pthread_t       MyThread   [MAX_PROCESS];
        pthread_attr_t  threadAttr;      
         
	public:        
        
        //ATTRIBUTES
        Processo   *MyProcess  [MAX_PROCESS];
        int         currentProcess;
        int         nProc;              
        int         totalMemSize;
        int         freeMemSize;         

        // class Constructor
    	Nucleo();    		

        // METHODS        
        int     addProcess          ( char * PID, int delay);
        void    startThreads        ();
        void    clean               ();        
        int     checkValidPID       ( char * PID ); 
        int     checkProcessExists  ( char * PID );
        int     readMemoryInfo();       

};

#endif
