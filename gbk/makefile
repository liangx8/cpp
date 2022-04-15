.PHONY: clean
CC=gcc
RM=rm -rf
CFLAGS=-Wall -g
TARGET=../bin/gbk


SRCS=main.c
OBJS=${SRCS:.c=.o}
LDFLAGS=-Wall  # -static

all:${TARGET}
	@echo "*** Done ***"
${TARGET}:${OBJS}
	${CC} -o $@ $< ${LDFLAGS}
clean:
	${RM} *.o ${TARGET} *~

#%.o:%.cpp
.c.o:
	${CC} -c $< ${CFLAGS}

