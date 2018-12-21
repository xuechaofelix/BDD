.PHONY: BDD clean

PREFIX ?= /usr/local

CPPFLAGS+=-I include/ -I${PREFIX}/include -Wall 

LDFLAGS+=-L${PREFIX}/lib

SRC_FILE ?= src/main.cpp src/node.cpp src/util.cpp

clean:
	-rm -f BDD

BDD: ${SRC_FILE}
	g++ ${CPPFLAGS} ${SRC_FILE} ${LDFLAGS} -g -o BDD -std=gnu++11

