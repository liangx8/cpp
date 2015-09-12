CC=g++
FLAG=-Wall
CFLAG=${FLAG} -g
OBJS=file.o main.o winmode.o listview.o
TARGET=main
LIBS=-lintl -lcomctl32 # -static -liconv
all:$(TARGET)
	@echo "*** Done ***"
$(TARGET):${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

clean:
	${RM} -rf ${TARGET} *.o
.cpp.o:
	${CC} -c $< ${CFLAG}
