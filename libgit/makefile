.PHONY: clean
CC=gcc
RM=rm -rf
CFLAGS=-Wall -g
TARGET=mgit


SRCS=${TARGET}.c common.c utils.c learning.c
OBJS=${SRCS:.c=.o}
LDFLAGS=-Wall -g -lgit2 # -static

all:${TARGET}
	@echo "*** Done ***"
${TARGET}:${OBJS}
	${CC} -o $@ ${OBJS} ${LDFLAGS}
test:gitr.o common.o
	${CC} -o $@ $< ${LDFLAGS}
clean:
	${RM} *.o test ${TARGET} *~


.c.o:
	${CC} -c -o $@ $< ${CFLAGS}

