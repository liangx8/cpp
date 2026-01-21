CC=gcc

CFLAGS:=-Wall
ifeq (${OPTC}X,X)
	CFLAGS+=-O2
else
	CFLAGS+=${OPTC}
endif

CSOURCE:=${wildcard ${TARGET}/*.c}
-include ${TARGET}/custom.mk

OBJS=${CSOURCE:.c=.o}

${TARGET}:${OBJS}
	${CC} ${LDFLAGS} -o bin/${TARGET} ${OBJS}

.c.o:
	${CC} ${CFLAGS} -c $< -o $@

clean:
	find -name *.o -exec rm \{} \;