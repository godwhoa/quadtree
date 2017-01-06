all:
	g++ main.cpp -o main -std=c++11 -g -static-libgcc -static  -static-libstdc++
run: all
	./main