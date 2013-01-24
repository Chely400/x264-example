CC=gcc
CCFLAG=-g
LIBS=-lx264

all:scenecut

scenecut:scenecut.o
	$(CC) -o $@ $< $(LIBS)

scenecut.o:scenecut.c
	$(CC) $(CCFLAGS) -c -o $@ $<
	
clean:
	rm -rf *.o scenecut