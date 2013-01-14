CC := gcc
CFLAGS := -Wall -Werror
LDFLAGS := 


OBJECTS := shell.o list.o

all: w4118_sh


w4118_sh: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f w4118_sh
	rm -f shell.o
	rm -f list.o

.PHONY: clean
