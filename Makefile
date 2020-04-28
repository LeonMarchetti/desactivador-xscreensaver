CC=gcc
CFLAGS=-Wall -pthread

RUN_DIR=run/

PRJ_NOM=desactivador-xscreensaver
SOURCE=main.c
TARGET=${RUN_DIR}${PRJ_NOM}

all: ${RUN_DIR} ${TARGET}

run: ${TARGET}
	${TARGET}

${RUN_DIR}:
	mkdir ${RUN_DIR}

${TARGET}: ${SOURCE}
	${CC} -o ${TARGET} ${CFLAGS} `pkg-config --cflags --libs gtk+-3.0` ${SOURCE}

clean:
	${RM} ${RUN_DIR}*
