.PHONY: clean
CC=gcc
RM=rm -rf
CFLAGS=-Wall -g
TARGET=color char


SRCS=${TARGET}.c
OBJS=${SRCS:.c=.o}
LDFLAGS=-Wall -g # -static

all:${TARGET}
	@echo "*** Done ***"
char:char.o
	${CC} -o $@ $< ${LDFLAGS}
color:color.o
	${CC} -o $@ $< ${LDFLAGS}
clean:
	${RM} *.o ${TARGET} *~

#%.o:%.cpp
.c.o:
	${CC} -c $< ${CFLAGS}

