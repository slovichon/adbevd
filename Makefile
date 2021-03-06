# $Id$

TARGET = adbevd
OBJS = adbevd.o
CFLAGS = -Wall

all: ${TARGET}

${TARGET}: ${OBJS}
	${CC} ${LIBS} -o ${.TARGET} ${OBJS}

.c.o:
	${CC} ${CFLAGS} -c ${.IMPSRC}

clean:
	rm -f ${TARGET} ${OBJS}
