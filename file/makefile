CC=g++
FLAG=-Wall

#CFLAG=${FLAG}
#OBJS=main.o
#TARGET=main
#在新版中，iconv的定义在 libc.so中
#LIBS=-lc #-liconv

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
