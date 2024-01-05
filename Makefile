all: problem1 problem2

problem1:
	g++ -o main1 main1.cpp -std=c++2a

problem2:
	g++ -o main2 main2.cpp -std=c++2a

clean:
	rm -f problem1
	rm -f problem2
