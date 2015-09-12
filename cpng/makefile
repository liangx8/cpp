CC=gcc
TARGET=mathpng
OBJS=mathpng.o
CFLAGS= -I d:/p/mingw/lpng1612
LFLAGS= -L d:/p/mingw/lpng1612 -lpng -lm -lz
all:${TARGET}
${TARGET}:${OBJS}
	${CC} -o $@ $< ${LFLAGS}
.c.o:
	${CC} -c $< -o $@ ${CFLAGS}
clean:
	rm -rf *.o ${TARGET} 
