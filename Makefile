CC=clang

all: sensor.bpf.c loader.c vmlinux.h sensor.h
	$(CC) -O2 -g -target bpf -c sensor.bpf.c -o sensor.bpf.o
	$(CC) -O2 -g -Wall -I/usr/include -I/usr/include/bpf -o loader loader.c -lbpf