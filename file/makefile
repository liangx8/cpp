CC=g++
FLAG=-Wall
CFLAG=${FLAG}
OBJS=main.o
TARGET=main
#在新版中，iconv的定义在 libc.so中
LIBS=-lc #-liconv
all:$(TARGET)
	@echo "*** Done ***"
$(TARGET):${OBJS}
	${CC} -o $@ ${OBJS} ${LIBS}

clean:
	${RM} -rf ${TARGET} *.o
.cpp.o:
	${CC} -c $< ${CFLAG}
