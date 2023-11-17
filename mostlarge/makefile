
CC=gcc
AS=as
OBJCOPY=objcopy
OBJDUMP=objdump
SIZE=size

TARGET=large

CSOURCE=$(wildcard *.c)
ASOURCE=$(wildcard *.S)


OBJS=${CSOURCE:.c=.o} ${ASOURCE:.S=.o}

OPTC=-O2
CFLAGS:=${OPTC}
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
