.PHONY: clean
CC=g++
RM=rm -rf
CFLAGS=-Wall -g
TARGET=main


SRCS=irc.cc parser_cmd.cc
OBJS=${SRCS:.cc=.o}
LDFLAGS=-Wall -g -lrt -lpthread # -static

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

