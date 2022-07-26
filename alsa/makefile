#CC=d:/p/mcu_dev/sdcc/bin/sdcc


CC=gcc


CSOURCE=$(shell find -name \*.c -and -type f)
ASOURCE=$(shell find -name \*.S -and -type f)


OBJS=${CSOURCE:.c=.o} ${ASOURCE:.S=.o}

#PORT=-mcpu=cortex-m3  -mthumb # -mlittle-endian

OPTC=-O2
#OPTC=-g

# -nostdlib会导致一些编译错误。缺省的内置函数如果不能用简单的方法实现时，会调用标准函数。如memset
# 连接的时候加 -mthumb让连接器在连接的时候，一些标准函数使用 thumb标准库
LFLAGS=-lasound
all:../bin/alsa

../bin/alsa:${OBJS}
	${CC} -o $@ ${LFLAGS} ${OBJS} 


.c.o:
	${CC} ${CFLAGS} -c $< -o $@
.S.o:
	${AS} ${PORT} -g $< -o $@

clean:

	find -name \*~ -or -name \*.o -exec rm \{} \;

