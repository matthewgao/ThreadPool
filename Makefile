all:
	g++ -g main.cpp ThreadPool.h ThreadPool.cpp -lpthread -o mainTest
