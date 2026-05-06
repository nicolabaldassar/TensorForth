# NICOLA BALDASSAR SM3201596
CC = gcc-15
CFLAGS = -O3 

main: main.o stack.o tensor.o operations.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

main.o: main.c stack.h tensor.h operations.h

stack.o: stack.c stack.h tensor.h

tensor.o: tensor.c tensor.h 

operations.o: operations.c operations.h stack.h

.PHONY: all clean

clean:
	rm *.o