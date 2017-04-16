CC="avr-gcc"
OBJCOPY="avr-objcopy"
CFLAGS=-g -Os -mmcu=attiny24
SOURCES="main.c"
BINARY="main.elf"

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BINARY)
	
