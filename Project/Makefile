run: ikin test
	./ikin
ikin: ikinTest.c matlib.o
	gcc -g -lm $^ -o $@
test: test.c matlib.o
	gcc -g -lm $^ -o $@
matlib.o : matlib.c matlib.h
	gcc -g -c $< -o $@

