# Object files.
OBJ = arralloc.o uni.o percolate.o
# Include files.
INC = arralloc.c uni.c percolate.c

.PHONY: calculate
calculate: ${OBJ} percolate.out
.c.o:
	gcc -c $<

percolate.out: ${OBJ} ${INC}
	gcc ${OBJ} -o $@
	rm *.o

.PHONY: clean
clean:
	rm *.dat *.pgm  percolate.out 

