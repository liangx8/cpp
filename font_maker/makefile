.PHONY: clean
CC=g++

FLAG=-Wall -g
CFLAG= ${FLAG} -DASC_BITMAP_168_ONLY_USE_DISPLAY_CHAR
TARGET=main
#TARGET=utf8_eli
OBJS=${TARGET}.o
LFLAG=${FLAG}

all:${TARGET}
	@echo "*** Done ***"
${TARGET}:${OBJS}
	${CC} -o $@ ${OBJS} ${LFLAG}


clean:
	${RM} -rf *.o ${TARGET}
%.o:%.cpp
	${CC} -c $< ${CFLAG}

