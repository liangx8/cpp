CC=gcc
TARGET=mathpng
OBJS=mathpng.o
CFLAGS=
LFLAGS=-lpng -lm -Wall
all:${TARGET}
${TARGET}:${OBJS}
	${CC} -o $@ $< ${LFLAGS}
.c.o:
	${CC} -c $< -o $@
clean:
	rm -rf *.o ${TARGET}
