CC=gcc
TARGET=../bin/mathpng
OBJS=mathpng.o

CFLAGS=
LFLAGS=-lpng -lm -Wall
all:${TARGET}
${TARGET}:${OBJS}
	${CC} -o $@ $< ${LFLAGS}
.c.o:
	${CC} -c $< -o $@ ${CFLAGS}
clean:
	rm -rf *.o ${TARGET} 
