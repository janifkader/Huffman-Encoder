CFLAGS = `pkg-config --cflags gtk+-3.0` -g
LDFLAGS = `pkg-config --libs gtk+-3.0`

huff : heap.o huffman.o gui.o encoder.o
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

encoder.o : encoder.c huffman.h gui.h heap.h
	gcc $(CFLAGS) -c $<

huffman.o : huffman.c huffman.h heap.h
	gcc $(CFLAGS) -c $<

gui.o : gui.c gui.h heap.h huffman.h
	gcc $(CFLAGS) -c $<

heap.o : heap.c heap.h
	gcc $(CFLAGS) -c $<

clean :
	rm -f *.o huff
.PHONY: clean