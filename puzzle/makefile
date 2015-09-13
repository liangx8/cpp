.PHONY: clean
CC=g++
RM=rm -rf
CFLAGS=-Wall -g
TARGET=chemical


SRCS=chemical.cc data.cc
OBJS=${SRCS:.cc=.o}
LDFLAGS=-Wall -g -liconv # -static

all:${TARGET}
	@echo "*** Done ***"
release:${OBJS}
	${CC} -o ${TARGET} ${OBJS} ${LDFLAGS} -static
	strip	${TARGET}
	@echo "*** Done ***"
${TARGET}:${OBJS}
	${CC} -o $@ ${OBJS} ${LDFLAGS}
clean:
	${RM} *.o ${TARGET}
	
.cc.o:
	${CC} -c $< ${CFLAGS}

