CC = g++
CFLAGS = -Wall -Werror
TARGET = kry

.PHONY: all clean pack

all: $(TARGET)

$(TARGET): $(TARGET).o
		$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o
		rm $(TARGET).o

$(TARGET).o: $(TARGET).cpp
		$(CC) $(CFLAGS) -c $(TARGET).cpp

clean:
		rm -f $(TARGET)
		rm -f $(TARGET).o
		rm -f 221701.zip

pack: all
	zip 221701.tar Makefile kry.cpp README doc.pdf