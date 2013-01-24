CC=gcc
CCFLAG=-g
LIBS=-lx264

all:yuv2h264

yuv2h264:yuv2h264.o
	$(CC) -o $@ $< $(LIBS) 

yuv2h264.o:yuv2h264.c
	$(CC) $(CCFLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o yuv2h264