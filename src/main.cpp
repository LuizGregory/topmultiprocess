/******************************************************
 * 
 *              UNIVATES 2009/A
 *      Engenharia da Computação
 *      Projeto Sistemas Operacionais - Trabalho 1
 *      MultiTop Process
 *
 * Autor : Luiz Gilberto Gregory Filho - 509921
 * Email : luiz.gregory@gmail.com
 * Linguagem : C++
 * Plataforma: Linux (Ubuntu 8.10)
 * Descrição Geral: Implementar um sistema para exibir informações
 *              sobre determinados processos que serão passados como parametro.
 *              Estas informações seriam, quantidade de memória utilizada 
 *              pelo processo, processamento, etc.
 *
 * GoogleCode: http://code.google.com/p/topmultiprocess/
 * Svn: svn checkout http://topmultiprocess.googlecode.com/svn/trunk/ topmultiprocess-read-only
 *
 * Arquivo pricipal.
 * File Create: 27/03/2009
 *
 * Créditos:
 * Este trabalho teve como fonte de pesquisa 
 *  o trabalho de conclusão de curso de Jamiel Spezia.
 *
 *****************************************************/

// INCLUDES
#include <iostream>
#include <fstream>
#include <cmath>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/param.h>

// INCLUDE .H
#include "constants.h"
#include "displayControl.h"
#include "Processo.h"
#include "Nucleo.h"


using namespace std;

//Global Object
Nucleo  *NucleoBase     = new Nucleo();


/**
 * Help Básico
 */
void help()
{
    int line = 1;
    printLineStr(line++,1,150);
    gotoxy(line++, 1);
    cout << "|";
    gotoxy(line++, 1);
    cout << "| Bem vindo ao help!!! uheuhuhauh";
    gotoxy(line++, 1);
    cout << "|";    
    gotoxy(line++, 1);    
    cout << "| as opçoes m|c não serão implementadas, sempre sera exibido CPU% & MEMORY% ";
    gotoxy(line++, 1);
    cout << "|";    
    gotoxy(line++, 1);    
    cout << "| Exemplo: ./multiTop -d1 6625 -d 5 6963 -d 9 6943";       
    gotoxy(line++, 1);
    cout << "|";    
    gotoxy(line++, 1);    
    cout << "| -d = Delay, valores validos de 1 a 9";
    gotoxy(line++, 1);    
    cout << "|";    
    gotoxy(line++, 1);
    cout << "| Obrigado, volte sempre!";              
    gotoxy(line++, 1);
    cout << "|";    
    printLineStr(line++,1,150);
}


/**
 * Metodo MAIN. 
 * Este metodo instancia o núcleo.
 * Seta os parametros recebidos e 
 * 'Starta' as threads.
 */
int main(int argc, char *argv[])
{
    clrscr();

    if(argc == 1 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "?") == 0)
    {
        help();
        return 0;
    }        
    
    int proximoDelay = 0;
    char *delay;
    
    delay = "1";   

    // neste FOR são adicionados os processos ao NUCLEO
    for(int x = 1; x < argc; x++)
    {                    
        // opçao e parametro separados
        if(strcmp(argv[x], "-d") == 0)
        {
            delay = argv[++x];
            x++;
        }
        // opçao e parametro juntos
        else if(strcmp(substr(argv[x], 0, 2), "-d") == 0)
        {         
            delay = substr(argv[x], 2, 3);            
            x++;            
        }                        

        NucleoBase->addProcess(argv[x], atoi(delay));
    }
    
    NucleoBase->startThreads();
       
    return 0;
    
}// metodo main

