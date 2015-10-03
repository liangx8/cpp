CC=gcc
FLAG=-Wall -g
CFLAG=${FLAG}
OBJS=utils.o main.o
TARGET=main
all:$(TARGET)
	@echo "*** Done ***"
$(TARGET):${OBJS}
	${CC} -o $@ ${OBJS}

clean:
	${RM} -rf ${TARGET} *.o *~
.c.o:
	${CC} -c $< ${CFLAG}
