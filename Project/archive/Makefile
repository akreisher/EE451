ikin: ikinTest.c matlib.o
	gcc -g -lm $^ -o $@
test: test.c matlib.o
	gcc -g -lm $^ -o $@
matlib.o: matlib.c matlib.h
	gcc -g -lm -c $< -o $@
