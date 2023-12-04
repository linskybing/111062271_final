#include "basicDS.h"

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */

class Problem1 {
public:

	Problem1(Graph G);  //constructor
	~Problem1();        //destructor
	void insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
	void stop(int id, Graph &G, Forest &MTidForest);
	void rearrange(Graph &G, Forest &MTidForest);
};

Problem1::Problem1(Graph G) {
	/* Write your code here. */

}

Problem1::~Problem1() {
	/* Write your code here. */

}

void Problem1::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	
	/* Write your code here. */

	return;
}

void Problem1::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	
	/* Write your code here. */

	return;
}

void Problem1::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	/* Write your code here. */
	
	return;
}
