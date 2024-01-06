#include "basicDS.h"
#include "Problem1.h"
#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>
#define ID_SIZE int(1E5 + 1)
#define INF 0x3f3f3f3f

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */

class Problem1 {
	public:
		Problem1(Graph G);  //constructor
		~Problem1();        //destructor
		void insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
		void stop(int id, Graph &G, Forest &MTidForest);
		void rearrange(Graph &G, Forest &MTidForest);

		void addEdge(Graph &G, Tree& MTid, edgeList it); // add an edge to MTid
		void release(Graph &G, Tree &MTid); // release MTid resource
		void allocate(Graph &G, Tree &MTid, const int& s, const int& t, int& need); // allocate resource to MTid
		void allocateAddiction(Graph &G, Tree &MTid, const int& s, int& need);

		void printTree(Tree &MTid);
		void printAdj(); // print the AdjList
		void printGraph(Graph G); // print the Graph
		void printForest(Forest F); // print all of the MTid
	private:
		int size;
		vector<edgeList>* adjList; // sort the graph edge with bandwithcost;
		vector<Contain> contain; // MTid's bandwidth
		vector<int> bandwidth;
		Graph t_G;
		Forest t_F;
};

Problem1::Problem1(Graph G) {
	
	adjList = new vector<edgeList>[G.V.size()];
	size = G.V.size();
	for (int i = 0; i < G.E.size(); i++) {
		// undirected graph
		edgeList t = {i, G.E[i].vertex[1], G.E[i].ce};
		edgeList t2 = {i, G.E[i].vertex[0], G.E[i].ce};
		adjList[G.E[i].vertex[0] - 1].push_back(t);
		adjList[G.E[i].vertex[1] - 1].push_back(t2);
	}
	for (int i = 0; i < G.V.size(); i++) {
		// sort the adjlist edge by cost
		sort(adjList[i].begin(), adjList[i].end(), Compare);
	}
	contain.reserve(ID_SIZE);
	bandwidth.reserve(ID_SIZE);

	t_G = G;
}

Problem1::~Problem1() {

	for (int i = 0; i < size; i++) {
		vector<edgeList>().swap(adjList[i]);
	}

	delete [] adjList;
	vector<Contain>().swap(contain);
	vector<int>().swap(bandwidth);
}

void Problem1::addEdge(Graph& G, Tree& MTid , edgeList e) {

	// record index
	contain[MTid.id].vertex[e.dest-1] = e.index;

	treeEdge te;
	if (e.dest == G.E[e.index].vertex[0]) {
		te.vertex[0] = G.E[e.index].vertex[1];
		te.vertex[1] = G.E[e.index].vertex[0];
	}
	else {
		te.vertex[0] = G.E[e.index].vertex[0];
		te.vertex[1] = G.E[e.index].vertex[1];
	}

	//add the edge to tree
	MTid.E.push_back(te);
	MTid.ct += e.cost * bandwidth[MTid.id];
	// update bandwith
	G.E[e.index].b -= bandwidth[MTid.id];

	return;
}

void Problem1::release(Graph& G, Tree& MTid) {
	int index;
	for (auto it = MTid.E.begin(); it < MTid.E.end(); it++) {
		// release bandwith		
		G.E[contain[MTid.id].vertex[it->vertex[1]-1]].b += bandwidth[MTid.id];

		// clean tree information
		contain[MTid.id].vertex[it->vertex[1]-1] = -1;
	}

	MTid.ct = 0;
	MTid.E.clear();
	return;
}

void Problem1::allocate(Graph& G, Tree& MTid, const int& s, const int& t, int& need) {
	// using Prim's algorithm with priority queue
	// initialize distance table
	priority_queue<edgeList, vector<edgeList>, cmp> pq;

	for (auto it = adjList[s].begin(); it < adjList[s].end(); it++) {
		if (G.E[it->index].b >= t) {
			pq.push(*it);
		}
	}
	int i;
	while (need && !pq.empty()) {
		edgeList e = pq.top(); pq.pop();

		i = e.dest-1;
		if (contain[MTid.id].vertex[i] != -1) continue;

		addEdge(G, MTid, e);

		need--;

		// adjoint edge check
		for (auto it : adjList[i]) {
			int d  = it.dest-1;
			if (d != s && contain[MTid.id].vertex[d] == -1 && G.E[it.index].b >= t) {
				pq.push(it);
			}
		}
	}

	return;
}

void Problem1::allocateAddiction(Graph& G, Tree& MTid, const int& s, int& need) {
	// using prime's algorithm with priority queue

	priority_queue<edgeList, vector<edgeList>, cmp> pq;

	int bandw = bandwidth[MTid.id];
	int d;
	for (int i = 0; i < size; i++) {
		if (contain[MTid.id].vertex[i] == -1) continue;
		// push the edge of the adjoint node that the terminal node point at
		for (auto it : adjList[i]) {
			d = it.dest - 1;
			if (G.E[it.index].b >= bandw && contain[MTid.id].vertex[d] == -1) {
				pq.push(it);
			}
		}
	}
	int i;
	while (need && !pq.empty()) {
		edgeList e = pq.top();
		pq.pop();

		i = e.dest-1;

		if (contain[MTid.id].vertex[i] != -1) continue;

		addEdge(G, MTid, e);

		need--;

		// adjoint edge check
		for (auto it : adjList[i]) {
			int d = it.cost-1;
			if (d != s && contain[MTid.id].vertex[d] == -1 && G.E[it.index].b >= bandw) {
				pq.push(it);
			}
		}
	}

	return;
}

void Problem1::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {

	/* Store your output graph and multicast tree into G and MTid */
	Tree temp;
	temp.s = s;
	temp.id = id;
	temp.ct = 0;

	//copy set into MTid Vetex set
	temp.V = D.destinationVertices;

	int n = D.size - 1; // how many the number of edge needed
	s--;

	// store multicast tree cost
	bandwidth[id] = t;

	Contain t_c;
	for (int i = 0; i < size; i++) {
		t_c.vertex.push_back(-1);
	}
	t_c.vertex[s] = -2;
	
	contain[id] = t_c;

	// allocate resource
	allocate(t_G, temp, s, t, n);

	
	MTid = temp;

	G = t_G;
	t_F.trees.push_back(temp);
	t_F.size++;

	return;
}

void Problem1::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	
	// find the multicast tree index
	int i = 0;
	for (; i < t_F.size && t_F.trees[i].id != id; i++);
	
	if (i == t_F.size) return;

	// release resource
	int s = t_F.trees[i].s-1;

	release(t_G, t_F.trees[i]);

	t_F.trees[i].V.clear();

	// remove tree
	t_F.trees.erase(t_F.trees.begin() + i);
	t_F.size--;

	// add more node to other multicast tree
	for (int i = 0; i < t_F.size; i++) {

		int id = t_F.trees[i].id;

		int source = t_F.trees[i].s-1;

		int need = size - (t_F.trees[i].E.size() + 1);

		if (need) continue;

		allocateAddiction(t_G, t_F.trees[i], source, need);
	}

	MTidForest = t_F;
	G = t_G;
	return;
}

void Problem1::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	// release all resouce
	int s, id, t, n;
	for (int i = 0; i < t_F.size; i++) {
		s = t_F.trees[i].s-1;
		id = t_F.trees[i].id;
		t = bandwidth[id];
		release(t_G, t_F.trees[i]);
	}

	for (int i = 0; i < t_F.size; i++) {
		s = t_F.trees[i].s-1;
		id = t_F.trees[i].id;
		t = bandwidth[id];
		n = t_F.trees[i].V.size();
		allocate(t_G, t_F.trees[i], s, t, n);
	}

	G = t_G;
	MTidForest = t_F;
	return;
}

void Problem1::printTree(Tree &MTid) {

	cout << "===   [" << MTid.id << "]    ===\n";

	// print the tree information

	cout << "source: " << MTid.s << "\n";
	cout << "Vertex: { ";

	for (int it : MTid.V) {
		std::cout << it << " ";
	}

	cout << "}" << "\n";
	cout << "edge: " << "\n";

	for (treeEdge edge : MTid.E) {
		cout << edge.vertex[0] << " " << edge.vertex[1] << "\n";
	}

	cout << "total cost: " << MTid.ct << "\n\n";
	cout << "=======End======\n";

	return;
}

void Problem1::printAdj() {
	std::cout << "====AdjList====\n";
	for (int i = 0; i < size; i++) {
		vector<edgeList>::iterator it;
		for (it = adjList[i].begin(); it < adjList[i].end(); it++) {
			std::cout << i+1 << " " << it->dest << " " << it->cost << std::endl;
		}
	}
	std::cout << "================\n";
	std::cout << std::endl;
}

void Problem1::printGraph(Graph G) {
	cout << "===Graph Edge===\n";
	for (auto it = G.E.begin(); it < G.E.end(); it++) {
		cout << it->vertex[0] << " " << it->vertex[1] << ": " << it->b << "/" << it->ce << "\n";
	}
	cout << "=======End======\n";
}

void Problem1::printForest(Forest F) {
	for (int i = 0; i < F.size; i++) {
		printTree(F.trees[i]);
	}
}
