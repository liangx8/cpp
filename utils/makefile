CC=gcc
FLAG=-Wall -g
CFLAG=${FLAG}
OBJS=buffer.o linked.o btree.o main.o array.o
TARGET=../bin/main
all:$(TARGET)
	@echo "*** Done ***"
$(TARGET):${OBJS}
	${CC} -o $@ ${OBJS}

clean:
	${RM} -rf ${TARGET} *.o *~
.c.o:
	${CC} -c $< ${CFLAG}
