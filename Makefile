CC = clang
TARGET1 = huffcompress
TARGET2 = huffdecompress

all: clean huffcompress	huffdecompress

huffcompress: bv.o huffnode.o huffcode.o huffstack.o huffminheap.o utils.o huffcompress.o
	$(CC) $(CFLAGS) compress.c -o $(TARGET1) *.o

huffdecompress: bv.o huffnode.o huffstack.o utils.o huffdecompress.o
	$(CC) $(CFLAGS) decompress.c -o $(TARGET2) *.o

clean:
	rm -f huffcompress huffdecompress *.o