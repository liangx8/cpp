CC=g++
FLAG=-Wall
CFLAG=${FLAG} -g
OBJS=main.o
TARGET=main
LIBS=-lintl -liconv
all:$(TARGET)
	@echo "*** Done ***"
$(TARGET):${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

clean:
	${RM} -rf ${TARGET} *.o *~
.cpp.o:
	${CC} -c $< ${CFLAG}
