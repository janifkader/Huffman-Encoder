CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -g
LDFLAGS = `pkg-config --libs gtk+-3.0`

# Files
SRC = huffman.o
OUT = huff

# Default target
all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

huffman.o: huffman.c heap.h huffman.h

# Clean up
clean:
	rm -f $(OUT)