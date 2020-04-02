.PHONY: clean
CC=gcc
RM=rm -rf
CFLAGS=-Wall -g
TARGET=../bin/sdl


SRCS=main.c
OBJS=${SRCS:.c=.o}
LDFLAGS=-Wall -g -lSDL2 # -static

all:${TARGET}
	@echo "*** Done ***"
release:${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LDFLAGS} -static
	strip	${TARGET}
	@echo "*** Done ***"
${TARGET}:${OBJS}
	${CC} -o $@ ${OBJS} ${LDFLAGS}
clean:
	${RM} *.o ${TARGET} *~

#%.o:%.cpp
.c.o:
	${CC} -c $< ${CFLAGS}

