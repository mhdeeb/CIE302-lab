CC=gcc

all: disk.o kernal.o process.o key

disk.o: src/disk.c
	$(CC) src/disk.c -o dst/disk

kernal.o: src/kernal.c
	$(CC) src/kernal.c -o dst/kernal

process.o: src/process.c
	$(CC) src/process.c -o dst/process

key dst/key:
	touch dst/key
