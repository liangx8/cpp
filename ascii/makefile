.PHONY: clean
CC=gcc
RM=rm -rf
CFLAGS=-Wall -g
TARGET=../bin/color ../bin/char


SRCS=${TARGET}.c
OBJS=${SRCS:.c=.o}
LDFLAGS=-Wall -g # -static

all:${TARGET}
	@echo "*** Done ***"
../bin/char:char.o
	${CC} -o $@ $< ${LDFLAGS}
../bin/color:color.o
	${CC} -o $@ $< ${LDFLAGS}
clean:
	${RM} *.o ${TARGET} *~

#%.o:%.cpp
.c.o:
	${CC} -c $< ${CFLAGS}

