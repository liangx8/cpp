CC=gcc
FLAG=-Wall -O2 -DNDEBUG
CFLAG=${FLAG}
OBJS=main.o btree.o printtree.o
TARGET=../bin/main
all:$(TARGET)
	@echo "*** Done ***"
$(TARGET):${OBJS}
	${CC} -o $@ ${OBJS}

clean:
	${RM} -rf ${TARGET} *.o
.c.o:
	${CC} -c $< ${CFLAG}
