.PHONY: clean
CC=gcc
RM=rm -rf
CFLAGS=-Wall -g
TARGET=z


SRCS=${TARGET}.c
OBJS=${SRCS:.c=.o}
LDFLAGS=-Wall -g -lz # -static

all:${TARGET}
	@echo "*** Done ***"
gitr:gitr.o
	${CC} -o $@ $< ${LDFLAGS}
clean:
	${RM} *.o ${TARGET} *~


.c.o:
	${CC} -c -o $@ $< ${CFLAGS}

