CC = clang
TARGET = huffcompress

all: clean huffcompress	

huffcompress: bv.o huffnode.o huffcode.o huffminheap.o utils.o huffcompress.o
	$(CC) $(CFLAGS) compress.c -o $(TARGET) *.o

clean:
	rm -f huffcompress *.o
	rm -f common/*.o
	rm -f compress/*.o
	rm -f decompress/*.o