CC=gcc
TARGET=../bin/mpg
OBJS=mpg.o

CFLAGS=
LFLAGS=-lmpg123 -Wall
all:${TARGET}
${TARGET}:${OBJS}
	${CC} -o $@ $< ${LFLAGS}
.c.o:
	${CC} -c $< -o $@ ${CFLAGS}
clean:
	rm -rf *.o ${TARGET} 
