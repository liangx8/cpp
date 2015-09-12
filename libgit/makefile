.PHONY: clean
CC=gcc
RM=rm -rf
CFLAGS=-Wall -g
TARGET=gitr


SRCS=${TARGET}.c
OBJS=${SRCS:.c=.o}
LDFLAGS=-Wall -g -lgit2 # -static

all:${TARGET}
	@echo "*** Done ***"
gitr:gitr.o
	${CC} -o $@ $< ${LDFLAGS}
clean:
	${RM} *.o ${TARGET} *~


.c.o:
	${CC} -c -o $@ $< ${CFLAGS}

