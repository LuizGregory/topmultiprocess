

#include <stdio.h>
#include <time.h>
#include <iostream>

using namespace std;

#include <ctime>
struct tm *localtime( const time_t *time );

extern void clrscr();
extern void gotoxy( int line, int col);
extern void cleanLine( int line );
extern void printLineStr(int line, int col, int length );
extern void printCurrentTime();
extern void printColumns( char * columns[], int length, int line, int col);
extern char* substr(char* string, int posIni, int posFim);
extern char *getTimeStr();

