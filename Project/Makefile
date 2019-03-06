run: bin/test
	./bin/test
bin/ikin: ikinTest.c matlib.o
	gcc -g -lm $^ -o $@
bin/test: test.cpp bin/matlib.o
	g++ -g -lm $^ -o $@
bin/matlib.o : lib/matlib.cpp lib/matlib.h
	g++ -g -c $< -o $@

clean:
	rm bin/*

