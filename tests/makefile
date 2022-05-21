program_TEST_CXX_SRCS:=$(wildcard ./*.test.cpp)

build/all.exe: build/main.o $(program_TEST_CXX_SRCS) ../build/libramiel.a
	g++ -I../src build/main.o $(program_TEST_CXX_SRCS) ../build/libramiel.a -o build/all.exe

build/main.o: catch2/main.cpp
	g++ -c catch2/main.cpp -o build/main.o
