# NICOLA BALDASSAR SM3201596
CC = gcc-15
CFLAGS = -O3 -fopenmp

tensorforth: tensorforth.o stack.o tensor.o operations.o
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

tensorforth.o: tensorforth.c stack.h tensor.h operations.h

stack.o: stack.c stack.h tensor.h

tensor.o: tensor.c tensor.h 

operations.o: operations.c operations.h stack.h tensor.h

.PHONY: clean

clean:
	rm -f *.o tensorforth
