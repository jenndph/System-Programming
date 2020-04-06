CC = gcc
CFLAGS = -W -Wall
TARGET = 20181614.out
OBJECTS = 20181614.o shell.o memory.o opcode.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(OBJECTS) $(TARGET)
