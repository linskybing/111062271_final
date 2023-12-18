#include "basicDS.h"
#include "Problem1.h"
#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>

class Problem2 {
	public:
		Problem2(Graph G);  //constructor
		~Problem2();        //destructor
		bool insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
		void stop(int id, Graph &G, Forest &MTidForest);
		void rearrange(Graph &G, Forest &MTidForest);

	private:
		int size;
		vector<edgeList>* adjList; // sort the graph edge with bandwithcost;
		int** vetexCotain; // store the vertex already in multicast tree
		vector<vector<bpair>> bandwidth;
};

Problem2::Problem2(Graph G) {
	/* Write your code here. */
	
}

Problem2::~Problem2() {
	/* Write your code here. */

}

bool Problem2::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	
	/* Write your code here. */


	/* You should return true or false according the insertion result */
	return true;
}

void Problem2::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	
	/* Write your code here. */
	
	return;
}

void Problem2::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	/* Write your code here. */
	
	return;
}
