.PHONY: clean
CC=g++
FLAG=-Wall -g -L./
CFLAG=
OBJS=chemical.o

LFLAG=${FLAG} 

all:main.exe
	@echo "*** Done ***"
main.exe:${OBJS}
	${CC} -o $@ ${OBJS} ${LFLAG}


clean:
	${RM} -rf *.o *.exe
	
%.o:%.cpp
	${CC} -c $< ${CFLAG}