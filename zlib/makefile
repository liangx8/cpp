.PHONY: clean
CC=gcc
RM=rm -rf
CFLAGS=-Wall -g
TARGET=../bin/z


SRCS=z.c
OBJS=${SRCS:.c=.o}
LDFLAGS=-Wall -g -lz # -static

all:${TARGET}
	@echo "*** Done ***"
${TARGET}:z.o
	${CC} -o $@ $< ${LDFLAGS}
clean:
	${RM} *.o ${TARGET} *~


.c.o:
	${CC} -c -o $@ $< ${CFLAGS}

