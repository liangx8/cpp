
CC=gcc
AS=as
OBJCOPY=objcopy
OBJDUMP=objdump
SIZE=size

TARGET=large

CSOURCE=$(wildcard *.c)
ASOURCE=$(wildcard *.S)


OBJS=${CSOURCE:.c=.o} ${ASOURCE:.S=.o}
ifeq (${OPTS}x,x)
	OPTS=-O2
endif
CFLAGS:=${OPTS}
CFLAGS+=-Wall
LFLAGS=#-lcurses
all:${TARGET}
${TARGET}: ${OBJS}
	${CC} -o $@ ${OBJS} ${LFLAGS}

.c.o:
	${CC} ${CFLAGS} -c $< -o $@
clean:
	rm -rf ${ELF} ${TARGET} ${DISA_LIST} ${BIN}
	rm -f ../src/*.o
	find -name \*~ -or -name \*.o -exec rm \{} \;
