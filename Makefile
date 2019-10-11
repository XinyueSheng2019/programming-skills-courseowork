# Object files.
OBJ = arralloc.o uni.o percolate.o
# Include files.
INC = arralloc.c uni.c percolate.c

.PHONY: calculate
calculate: ${OBJ} a.out
.c.o:
	gcc -c $<

a.out: ${OBJ} ${INC}
	gcc ${OBJ} -o $@
	rm *.o

.PHONY: clean
clean:
	rm *.dat *.pgm *.txt a.out 

