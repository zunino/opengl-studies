link_libs = -lGL -lglfw -ldl

basic: build/main.o
	g++ $^ libglad.a $(link_libs) -o $@

build/main.o: src/main.cpp
	g++ -c -std=c++2a -I inc/ -o $@ $^

libglad.a: build/glad.o
	ar rcs $@ $^

build/glad.o: src/glad.c
	gcc -c -I inc/ -o $@ $^
