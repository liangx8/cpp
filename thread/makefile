.PHONY: clean
PACKAGE_NAME=thra

CC=gcc
RM=rm -rf
CFLAGS=-Wall -g -DPACKAGE=\"${PACKAGE_NAME}\"
TARGET=thr


SRCS=${shell ls *.c}
OBJS=${SRCS:.c=.o}
LDFLAGS=-Wall -g -l pthread

all:${TARGET}
	@echo "*** Done ***"
${TARGET}:${OBJS}
	${CC} -o $@ ${OBJS} ${LDFLAGS}
clean:
	${RM} *.o ${TARGET} *~

.c.o:
	${CC} -c $< ${CFLAGS}

xgettext:
	xgettext --language=C --keyword=gettext --add-comments --sort-output -o ${PACKAGE_NAME}.pot main.c
	@echo "第一次運行必須编辑pot文件中的charset属性为UTF-8"
msginit_zh:
	msginit --input=${PACKAGE_NAME}.pot --locale=zh_CN --output=po/zh_CN/${PACKAGE_NAME}.po
msgfmt:
	msgfmt --output-file=locale/zh_CN/LC_MESSAGES/${PACKAGE_NAME}.mo po/zh_CN/${PACKAGE_NAME}.po
msgmerge:
	msgmerge --update po/zh_CN/${PACKAGE_NAME}.po ${PACKAGE_NAME}.pot
