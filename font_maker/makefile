.PHONY: clean
CC=g++
WINDRES=windres
FLAG=-Wall -g
CFLAG= ${FLAG} -mwindows -DASC_BITMAP_168_ONLY_USE_DISPLAY_CHAR
TARGET=main
#TARGET=utf8_eli
OBJS=${TARGET}.o
LFLAG=${FLAG} -lmingw32

all:${TARGET}.exe
	@echo "*** Done ***"
${TARGET}.exe:${OBJS}
	${CC} -o $@ ${OBJS} ${LFLAG}


clean:
	${RM} -rf *.o *.exe *.res
	
%.o:%.cpp
	${CC} -c $< ${CFLAG}
	
%.res: %.rc resource.h
	${WINDRES} -DGCC_WINDRES -O coff $< $@