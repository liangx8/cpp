.PHONY: clean
CC=g++
WINDRES=windres
FLAG=-Wall -g
CFLAG= -mwindows
OBJS=main.o lifegame.res life_pattern.o
LFLAG=${FLAG} -lmingw32 -lgdi32 --enable-auto-import #--static

all:main.exe
	@echo "*** Done ***"
main.exe:${OBJS}
	${CC} -o $@ ${OBJS} ${LFLAG}


clean:
	${RM} -rf *.o *.exe *.res
	
%.o:%.cpp
	${CC} -c $< ${CFLAG}
	
%.res: %.rc resource.h
	${WINDRES} -DGCC_WINDRES -O coff $< $@