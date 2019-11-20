# Object files.
OBJ = arralloc.o uni.o percolate.o percwrite.o command_line.o
# Include files.
INC = arralloc.c uni.c percolate.c percwrite.c command_line.c
#Regression test file
TEST = regression_test.py

.PHONY: calculate
calculate: ${OBJ} percolate
.c.o:
	gcc -c $<

percolate: ${OBJ} ${INC}
	gcc ${OBJ} -o $@
	rm -f *.o

.PHONY: clean
clean:
	rm -f *.dat *.pgm  percolate output*

.PHONY: test
test:
	pytest ${TEST}