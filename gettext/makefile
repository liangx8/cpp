PACKAGE_NAME=nbd
CC=gcc
FLAG=-Wall
CFLAG=${FLAG} -g -DPACKAGE=\"${PACKAGE_NAME}\"
OBJS=main.o
TARGET=main
all:$(TARGET)
	@echo "*** Done ***"
$(TARGET):${OBJS}
	${CC} -o $@ ${OBJS}

clean:
	${RM} -rf ${TARGET} *.o *~
.c.o:
	${CC} -c $< ${CFLAG}
xgettext:
	xgettext --language=C --keyword=gettext --add-comments --sort-output -o ${PACKAGE_NAME}.pot main.c
	@echo "第一次運行必須编辑pot文件中的charset属性为UTF-8"
msginit_zh:
	msginit --input=${PACKAGE_NAME}.pot --locale=zh_CN --output=po/zh_CN/${PACKAGE_NAME}.po
msgfmt:
	msgfmt --output-file=locale/zh_CN/LC_MESSAGES/${PACKAGE_NAME}.mo po/zh_CN/${PACKAGE_NAME}.po
msgmerge:
	msgmerge --update po/zh_CN/${PACKAGE_NAME}.po ${PACKAGE_NAME}.pot
	
