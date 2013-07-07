/******************************************************
 *
 *          Display Control
 * 
 * Autor : Luiz Gilberto Gregory Filho - 509921
 * Descrição: Neste arquivo são implementado metodos
 * para controle de exibiçao de conteudo no terminal.
 *
 * File Create: 20/04/2009
 *
 *****************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

#include <ctime>
struct tm *localtime( const time_t *time );

/**
 * Este método limpa a tela e posiciona o cursor no inicio da tela
 */
void clrscr()
{
    printf("\e[2J");
}



/**
 * Este método posiciona o cursor em uma determinada coordenada definada pelos
 * paramentros line e col
 */
void gotoxy( int line, int col)    
{
    printf("\e[%d;%dH", line, col);
}    
 
 
 
/**
 * Este método limpa uma determinada linha do terminal
 */ 
void cleanLine( int line )
{
    gotoxy(line, 1);
    printf("\e[2K");
}


void printLineStr(int line, int col, int length )
{
    int x;
    
    gotoxy(line , col++);
    cout << "+";
    
    for(x = 1; x < length-1 ; x++)
    {
        gotoxy(line , col++);    
        cout << "-";
    }
    
    gotoxy(line , col+length);
    cout << "+";    
}

char *getTimeStr()
{
    time_t theTime;
    time( &theTime );   // get the calendar time
    tm *t = localtime( &theTime );  // convert to local
    return asctime(t);
}


void printCurrentTime()
{
    
    cout << "Current Time: " << getTimeStr(); 
}


void printColumns( char * columns[], int length, int line, int col)
{
    int x;

    for (x = 0; x <= sizeof(columns); x++)
    {
        gotoxy(line , col);        
        cout << "| " << columns[x];
        col += length;
    }
    gotoxy(line , col);        
    cout << "|";
}


/**
 * Corta um string em partes
 */
char* substr(char* string, int posIni, int posFim)
{
    char* resp;
    int i, c=0;
    
    resp = strdup(string);
    
    for ( i = posIni; i < posFim; i++, c++ )
    {
       resp[c] = string[i];
    }   
    resp[c] = '\0';
    
    return resp;
}

