.PHONY: clean
CC=gcc
RM=rm -rf
CFLAGS=-Wall -g -I ../utils
TARGET=mgit

SRCS=${TARGET}.c common.c ../utils/string.c ../utils/auto_buffer.c ../utils/mem.c ../utils/array.c history.c allobjects.c
OBJS=${SRCS:.c=.o}
LDFLAGS=-Wall -g -lgit2 # -static

all:../bin/${TARGET}
	@echo "*** Done ***"
../bin/${TARGET}:${OBJS}
	${CC} -o $@ ${OBJS} ${LDFLAGS}
test:gitr.o common.o
	${CC} -o $@ $< ${LDFLAGS}
clean:
	${RM} *.o test ../bin/${TARGET} *~


.c.o:
	${CC} -c -o $@ $< ${CFLAGS}

