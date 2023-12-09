all: problem1

problem1: Problem1.h main1.cpp
	g++ -o main1 main1.cpp -std=c++2a

clean:
	rm -f problem1
