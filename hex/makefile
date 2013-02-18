.PHONY: clean
CC=g++
RM=rm -rf
CFLAGS=-Wall -g
TARGET=hex


SRCS=${TARGET}.cpp
OBJS=${SRCS:.cpp=.o}
LDFLAGS=-Wall -g # -static

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
	
#%.o:%.cpp
.cpp.o:
	${CC} -c $< ${CFLAGS}

