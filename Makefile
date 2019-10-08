# Object files.
OBJ = arralloc.o uni.o percolate.o
# Include files.
INC = arralloc.c uni.c percolate.c

.PHONY: calculate
calculate: ${OBJ} scheduler
.c.o:
	${CC} -c $<

scheduler: ${OBJ} ${INC}
	${CC} ${OBJ} -o $@
	rm *.o

.PHONY: checksum
checksum: output.txt check.txt map.dat map.pgm

output.txt: scheduler
	./scheduler > $@
check.txt: output.txt map.dat map.pgm
	cksum output.txt >> check.txt
	cksum map.dat map.pgm >> check.txt
	rm output.txt

.PHONY: clean
clean:
	rm map.dat map.pgm scheduler 

