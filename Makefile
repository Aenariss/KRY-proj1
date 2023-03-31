CC = g++
CFLAGS = -Wall -Werror
TARGET = kry

sources = \
	utils.cpp \
	argParse.cpp \
	encrypt.cpp \
	decrypt.cpp \
	analyse.cpp \
	kry.cpp

headers = \
	utils.hpp \
	argParse.hpp \
	encrypt.hpp \
	decrypt.hpp \
	analyse.hpp \
	kry.hpp

objects = \
	utils.o \
	argParse.o \
	encrypt.o \
	decrypt.o \
	analyse.o \
	kry.o

.PHONY: all clean pack

all: $(TARGET)

$(TARGET): $(objects)
		$(CC) $(CFLAGS) -o $(TARGET) $^
		$(MAKE) clean

$(objects): $(sources)
		$(CC) $(CFLAGS) -c $^

clean: $(objects)
		rm -f $^

pack: all
	zip 221701.zip Makefile dokumentace.pdf $(sources) $(headers)