CC = gcc
CFLAGS = -Wall
OBJS = main.o

all: rtrebooter

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

rtrebooter: $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f rtrebooter *.o
