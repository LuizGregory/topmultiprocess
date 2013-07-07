#########################################################
# 
#              UNIVATES 2009/A
#      Engenharia da Computação
#      Projeto Sistemas Operacionais - Trabalho 1
#      MultiTop Process
#
# Autor : Luiz Gilberto Gregory Filho - 509921
# File Create: 27/03/2009
#
#########################################################

CC          = g++
FLAGS       = -g -O0 -Wall

# compile groups
ALL         = displayControl.o  Processo.o Nucleo.o main.cpp
DEP_PROC    = Processo.h Processo.cpp
DEP_NUCLEO  = Nucleo.h Nucleo.cpp
DEP_DISPLAY = displayControl.h displayControl.cpp

# nome do arquivo binario de saida
BIN_NAME    = multiTop

all:			
	${CC} ${FLAGS} -c ${DEP_DISPLAY}
	${CC} ${FLAGS} -c ${DEP_PROC}
	${CC} ${FLAGS} -c ${DEP_NUCLEO}	
	${CC} -l pthread  ${ALL} -o ${BIN_NAME}

proc:
	${CC}  ${FLAGS} -c ${DEP_PROC}
	
nucleo:
	${CC}  ${FLAGS} -c ${DEP_NUCLEO}					
	
clean:
	rm -rf *.o *~ *.gch

