run: bin/test
	./bin/test
bin/ikin: ikinTest.c matlib.o
	gcc -g -lm $^ -o $@
bin/test: test.cpp bin/matlib.o bin/ikin.o
	g++ -g -lm $^ -o $@
bin/matlib.o : lib/matlib.cpp lib/matlib.h
	g++ -g -c $< -o $@
bin/ikin.o : lib/ikin.cpp lib/ikin.h
	g++ -g -c $< -o $@

clean:
	rm bin/*

