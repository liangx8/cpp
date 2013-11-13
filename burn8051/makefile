.PHONY: clean
CC=gcc
WINDRES=windres
CFLAGS= -mwindows -Wall -g -DUNICODE
OBJS=burn.o readmcu.o
LDFLAGS=${FLAG} siutil.lib --enable-auto-import #--static -lgdi32  -lmingw32
TARGET=burn

all:${TARGET}.exe
	@echo "*** Done ***"
${TARGET}.exe:${OBJS}
	${CC} -o $@ ${OBJS} ${LDFLAGS}


clean:
	${RM} -rf *.o *.exe *.res
	
.c.o:
	${CC} -c $< ${CFLAGS}
	
%.res: %.rc resource.h
	${WINDRES} -DGCC_WINDRES -O coff $< $@