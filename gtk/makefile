CC=gcc
FLAG=-Wall -g
CFLAG=${FLAG} `pkg-config --cflags gtk+-3.0`
OBJS=example.o
TARGET=../bin/gtk-demo
all:$(TARGET)
	@echo "*** Done ***"
$(TARGET):${OBJS}
	${CC} -o $@ $< `pkg-config --libs gtk+-3.0`

clean:
	${RM} -rf ${TARGET} *.o
.c.o:
	${CC} -c $< ${CFLAG}

