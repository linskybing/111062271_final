all: problem2

problem1: Problem1.h main1.cpp
	g++ -o main1 main1.cpp -std=c++2a

problem2: Problem2.h main2.cpp
	g++ -o main2 main2.cpp -std=c++2a

clean:
	rm -f problem1
	rm -f problem2
