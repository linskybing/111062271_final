#include "basicDS.h"
#include "Problem1.h"
#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>

#define bpair pair<int, int>

/* You can add more functions or variables in each class. 
   But you "Shall Not" delete any functions or variables that TAs defined. */

class Problem1 {
	public:
		Problem1(Graph G);  //constructor
		~Problem1();        //destructor
		void insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
		void stop(int id, Graph &G, Forest &MTidForest);
		void rearrange(Graph &G, Forest &MTidForest);
		void addEdge(Graph &G, Tree& MTid, edgeList it, int bandw); // add an edge to MTid
		void release(Graph &G, Tree &MTid, const int& bandw); // release MTid resource
		void allocate(Graph &G, Tree &MTid, const int& s, const int& t, int& need); // allocate resource to MTid
		void allocateAddiction(Graph &G, Tree &MTid, const int& s, int& need, const int& vi);
		int getBandwith(const int& s, const int& id);
		int getContainIndex(const int s, const int& id);
		void printTree(int id, Forest MTidForest);
		void printAdj(); // print the AdjList
		void printGraph(Graph G); // print the Graph
		void printForest(Forest F); // print all of the MTid
		void printBandwid();
	private:
		int size;
		vector<edgeList>* adjList; // sort the graph edge with bandwithcost;
		vector<vector<Contain>> contain; // key: id value: 1D array
		vector<vector<bpair>> bandwidth;
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
	contain.reserve(size);
	bandwidth.reserve(size);

	t_G = G;
	//printAdj();
}

Problem1::~Problem1() {

	for (int i = 0; i < size; i++) {
		vector<edgeList>().swap(adjList[i]);
		vector<Contain>().swap(contain[i]);
		vector<bpair>().swap(bandwidth[i]);
	}

	delete [] adjList;
	vector<vector<Contain>>().swap(contain);
	vector<vector<bpair>>().swap(bandwidth);
}

int Problem1::getBandwith(const int& s,const int& id) {
	for (int i = 0; i < bandwidth[s].size(); i++) {
		if (bandwidth[s][i].first == id) return i;
	}
	return -1;
}

int Problem1::getContainIndex(const int source, const int& id) {
	int i;
	for (i = 0; i < contain[source].size() && contain[source][i].id != id; i++);
	return i;
}

void Problem1::addEdge(Graph& G, Tree& MTid , edgeList e, int bandw) {

	// record index
	int index = getContainIndex(MTid.s-1, MTid.id);
	contain[MTid.s-1][index].vertex[e.dest-1] = e.index;

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
	MTid.ct += e.cost * bandw;
	// update bandwith
	G.E[e.index].b -= bandw;

	return;
}

void Problem1::release(Graph& G, Tree& MTid, const int& bandw) {
	int index;
	for (auto it = MTid.E.begin(); it < MTid.E.end(); it++) {
		// release bandwith
		index = getContainIndex(MTid.s-1, MTid.id);
		G.E[contain[MTid.s-1][index].vertex[it->vertex[1]-1]].b += bandw;

		// clean tree information
		contain[MTid.s-1][index].vertex[it->vertex[1]-1] = -1;
	}

	MTid.ct = 0;
	MTid.E.clear();
	return;
}

void Problem1::allocate(Graph& G, Tree& MTid, const int& s, const int& t, int& need) {
	// using prime's algorithm with priority queue
	// initialize distance table
	priority_queue<edgeList, vector<edgeList>, cmp> pq;

	for (auto it = adjList[s].begin(); it < adjList[s].end(); it++) {
		if (G.E[it->index].b >= t) {
			pq.push(*it);
		}
	}

	int index = getContainIndex(s, MTid.id);

	while (need && !pq.empty()) {
		edgeList e = pq.top(); pq.pop();

		int i = e.dest-1;
		if (contain[s][index].vertex[i] != -1) continue;

		addEdge(G, MTid, e, t);

		need--;

		// adjoint edge check
		vector<edgeList>::iterator it = adjList[i].begin();
		for (; it < adjList[i].end(); it++) {
			if (it->dest-1 != s && contain[s][index].vertex[it->dest-1] == -1 && G.E[it->index].b >= t) {
				pq.push(*it);
			}
		}
	}

	return;
}

void Problem1::allocateAddiction(Graph& G, Tree& MTid, const int& s, int& need, const int& vi) {
	// using prime's algorithm with priority queue

	priority_queue<edgeList, vector<edgeList>, cmp> pq;

	int bandw = bandwidth[s][getBandwith(s, MTid.id)].second;

	for (int i = 0; i < size; i++) {
		if (contain[s][vi].vertex[i] == -1) continue;
		// push the edge of the adjoint node that the terminal node point at
		for (auto it = adjList[i].begin(); it < adjList[i].end(); it++) {
			if (G.E[it->index].b >= bandw && contain[s][vi].vertex[it->dest-1] == -1) {
				pq.push(*it);
			}
		}
	}

	while (need && !pq.empty()) {
		edgeList e = pq.top(); pq.pop();

		int i = e.dest-1;
		if (contain[s][vi].vertex[i] != -1) continue;

		addEdge(G, MTid, e, bandw);

		need--;

		// adjoint edge check
		vector<edgeList>::iterator it = adjList[i].begin();
		for (; it < adjList[i].end(); it++) {
			if (it->dest-1 != s && contain[s][vi].vertex[it->dest-1] == -1 && G.E[it->index].b >= bandw) {
				pq.push(*it);
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
	bandwidth[s].push_back(make_pair(id, t));

	Contain t_c;
	t_c.id = id;
	for (int i = 0; i < size; i++) {
		t_c.vertex.push_back(-1);
	}
	t_c.vertex[s] = -2;
	contain[s].push_back(t_c);

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
	int index = getBandwith(s, id);
	int vi = getContainIndex(s, id);
	release(t_G, t_F.trees[i], bandwidth[s][index].second);
	t_F.trees[i].V.clear();

	// remove tree
	contain[s].erase(contain[s].begin() + vi);
	bandwidth[s].erase(bandwidth[s].begin() + index); // clean the multicast bandwith
	t_F.trees.erase(t_F.trees.begin() + i);
	t_F.size--;

	// add more node to other multicast tree
	for (int i = 0; i < t_F.size; i++) {

		vi = getContainIndex(t_F.trees[i].s-1, t_F.trees[i].id);

		int source = t_F.trees[i].s-1;

		// count the number that has been in the tree (should be optimal)
		int count = 0;
		for (int i = 0; i < contain[source][vi].vertex.size(); i++) {
			if (contain[source][vi].vertex[i] != -1) count++;
		}
		if (count == size) continue;
		int need = size - count;
		allocateAddiction(t_G, t_F.trees[i], source, need, vi);
	}
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
		t = bandwidth[s][getBandwith(s, id)].second;
		release(t_G, t_F.trees[i], t);
	}

	for (int i = 0; i < t_F.size; i++) {
		s = t_F.trees[i].s-1;
		id = t_F.trees[i].id;
		t = bandwidth[s][getBandwith(s, id)].second;
		n = t_F.trees[i].V.size();
		allocate(t_G, t_F.trees[i], s, t, n);
	}
	G = t_G;
	MTidForest = t_F;
	return;
}

void Problem1::printTree(int id, Forest MTidForest) {
	// find the tree in MTidForest by id
	int index = 0;
	for (; index < MTidForest.size && MTidForest.trees[index].id != id; index++);
	if (index >= MTidForest.size) return;
	std::cout << "===   [" << id << "]    ===\n";
	// print the tree information
	Tree t = MTidForest.trees[index];
	std::cout << "source: " << t.s << "\n";
	std::cout << "Vertex: { ";
	for (int it : t.V) {
		std::cout << it << " ";
	}
	std::cout << "}" << "\n";
	std::cout << "edge: " << "\n";
	for (treeEdge edge : t.E) {
		std::cout << edge.vertex[0] << " " << edge.vertex[1] << "\n";
	}
	std::cout << "total cost: " << t.ct << "\n\n";
	std::cout << "=======End======\n";
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
	std::cout << "===Graph Edge===\n";
	for (auto it = G.E.begin(); it < G.E.end(); it++) {
		std::cout << it->vertex[0] << " " << it->vertex[1] << ": " << it->b << "/" << it->ce << "\n";
	}
	std::cout << "=======End======\n";
}

void Problem1::printForest(Forest F) {
	for (int i = 0; i < F.size; i++) {
		printTree(F.trees[i].id, F);
	}
}

void Problem1::printBandwid() {
	for (int i = 0; i < size; i++) {
		std::cout << "vertex " << i+1 << ":\n";
		for (auto it = bandwidth[i].begin(); it < bandwidth[i].end(); it++) {
			std::cout << it->first << " " << it->second << "\n";
		}
	}
}