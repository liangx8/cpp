CC=gcc
FLAG=-Wall
CFLAG=${FLAG}
OBJS=main.o
TARGET=main
all:$(TARGET)
	@echo "*** Done ***"
$(TARGET):${OBJS}
	${CC} -o $@ ${OBJS}

clean:
	${RM} -rf ${TARGET} *.o
.c.o:
	${CC} -c $< ${CFLAG}
