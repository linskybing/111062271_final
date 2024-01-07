#include "basicDS.h"
#include "Problem2.h"
#include <iostream>
#include <queue>
#include <utility>
#include <algorithm>

#define bpair pair<int, int>
#define btuple tuple<int, int, int>
#define ID_SIZE int(1E5 + 1)
#define STOP 10
#define Congestion 15
#define INF 10000001

class Problem2 {
	public:
		Problem2(Graph G);  //constructor
		~Problem2();        //destructor

		// main function
		bool insert(int id, int s, Set D, int t, Graph &G, Tree &MTid);
		void stop(int id, Graph &G, Forest &MTidForest);
		void rearrange(Graph &G, Forest &MTidForest);

		// sub function
		bool dijkstra(Tree &MTid, int s); // metric closure
		bool steiner(Tree &MTid);
		void addPath(Tree &MTid, bool* contain, const int& s, const int& d); // from mst add path to MTid
		bool MST(Tree &MTid); // find MST from metric closure
		void release(Tree& MTid);//

		// output
		void printTree(Tree &MTid);
		void printAdj(); // print the AdjList
		void printGraph(Graph G); // print the Graph
		void printForest(Forest &MTidForest); // print all of the MTid
		void printBandwid();
		void printShortest(Tree &MTid);

	private:
		int size;
		int call;
		int reject;
		vector<edgeList>* adjList; // sort the graph edge with bandwithcost;
		vector<int> bandwidth;
		vector<Contain> usageEdge;
		vector<int> congestion_window;
		vector<int> start;
		Metric metric;
		Graph t_G;
		Forest t_F;
};

Problem2::Problem2(Graph G) {
	adjList = new vector<edgeList>[G.V.size()];
	size = G.V.size();
	metric.distance = new int*[size];
	metric.edges = new edgeList*[size];

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
		metric.distance[i] = new int[size];
		metric.edges[i] = new edgeList[size];
		congestion_window.push_back(0);
	}
	bandwidth.reserve(ID_SIZE);
	usageEdge.reserve(ID_SIZE);
	start.reserve(ID_SIZE);
	t_G = G;
	call = 0;
}

Problem2::~Problem2() {
	for (int i = 0; i < size; i++) {
		vector<edgeList>().swap(adjList[i]);
		delete [] metric.distance[i];
		delete [] metric.edges[i];
	}

	vector<int>().swap(bandwidth);
	vector<int>().swap(start);
	vector<int>().swap(congestion_window);
	vector<Contain>().swap(usageEdge);
	delete [] metric.distance;
	delete [] metric.edges;
	delete [] adjList;
}

bool Problem2::dijkstra(Tree &MTid, int s) {
	priority_queue<bpair, vector<bpair>, greater<bpair>> pq;

	s--;
	for (int i = 0; i < size; i++) {
		metric.distance[s][i] = INF;
	}

	metric.distance[s][s] = 0;
	pq.push({0, s});

	while(!pq.empty()) {
		int u = pq.top().second;
		int dist_u = pq.top().first;
		pq.pop();

		for (auto edge : adjList[u]) {
			int v = edge.dest-1;
			if (u != v && dist_u + edge.cost < metric.distance[s][v] && t_G.E[edge.index].b >= bandwidth[MTid.id]) {
				metric.distance[s][v] = dist_u + edge.cost;
				metric.edges[s][v] = edge;

				pq.push({metric.distance[s][v], v});
			}
		}
	}

	for (auto v : MTid.V) {
		if (metric.distance[s][v-1] == INF) return false;
	}
	return true;
}

bool Problem2::MST(Tree &MTid) {
	// using prime's algorithm
	bool* contain = new bool[size];
	bool* contain_path = new bool[t_G.E.size()];

	priority_queue<btuple, vector<btuple>, greater<btuple>> pq;

	// initilize
	for (int i = 0; i < size; i++) {
		contain[i] = false;
	}

	for (int i = 0; i < t_G.E.size(); i++) {
		contain_path[i] = false;
	}

	int s = MTid.s-1;

	contain[s] = true;
	int need = MTid.V.size() - 1;

	// prime's algorithm

	for (auto v : MTid.V) {
		v--;
		if (metric.distance[s][v] != INF && v != s)
			pq.push({metric.distance[s][v], v, s});
	}

	while (need && !pq.empty()) {
		int v = get<1>(pq.top());
		int v1 = get<2>(pq.top());
		pq.pop();

		if (contain[v]) continue;

		contain[v] = true;
		need--;

		addPath(MTid, contain_path, v1, v);

		for (auto u : MTid.V) {
			u--;
			if (!contain[u] && metric.distance[v][u] != INF) {
				pq.push({metric.distance[v][u], u, v});
			}
		}
	}

	// free memonry
	delete [] contain;
	delete [] contain_path;
	return true; 
}

void Problem2::addPath(Tree &MTid, bool* contain, const int& s, const int& d) {
	int parent = d;
	edgeList e;
	vector<treeEdge> temp;

	do {
		e = metric.edges[s][parent];

		// find the parent node
		if (t_G.E[e.index].vertex[0] != e.dest) parent = t_G.E[e.index].vertex[0]-1;
		else parent = t_G.E[e.index].vertex[1]-1;

		if (!contain[e.index]) {
			temp.push_back({parent+1, e.dest});

			// update graph
			t_G.E[e.index].b -= bandwidth[MTid.id];
			MTid.ct += t_G.E[e.index].ce * bandwidth[MTid.id];

			// record edge index
			usageEdge[MTid.id].edges.push_back(e.index);

			contain[e.index] = true;
		}
	} while(parent != s);

	for (auto it = temp.end()-1; it >= temp.begin(); it--) {
		MTid.E.push_back(*it);
	}

	return;
}

bool Problem2::steiner(Tree &MTid) {

	// find all shortestPath
	for (auto i : MTid.V) {
		if (!dijkstra(MTid, i)) return false;
	}

	// using the MST to construct path
	MST(MTid);

	return true;
}

void Problem2::printShortest(Tree &MTid) {

	for (auto i : MTid.V) {
		i--;
		std::cout << "===From [" << i+1 << "] ===\n";
		for (auto j : t_G.V) {
			edgeList t = metric.edges[i][j];
			std::cout << i+1 << " "
					  << j+1 <<  " : "
					  << metric.distance[i][j] << "\n";
		}

		std::cout << "=======End======\n";
	}
}

bool Problem2::insert(int id, int s, Set D, int t, Graph &G, Tree &MTid) {
	/* Store your output graph and multicast tree into G and MTid */
	
	/* Write your code here. */
	Tree t_MTid;
	Metric metric;

	// initial vertex
	for (int i = 0; i < D.size; i++) {
		t_MTid.V.push_back(D.destinationVertices[i]);
	}

	t_MTid.s = s;
	t_MTid.id = id;
	t_MTid.ct = 0;
	s--;
	
	// store multicast tree cost
	bandwidth[id] = t;

	Contain t_c;
	usageEdge[id] = t_c;

	start[id] = call + 1;

	// build
	bool result = false;
	if (congestion_window[s] < Congestion) {
		result = steiner(t_MTid);
	}
	
	t_F.trees.push_back(t_MTid);
	t_F.size++;

	if (result) {
		MTid = t_MTid;
	}
	else {
		congestion_window[s]++;

	}

	G = t_G;
	call++;
	/* You should return true or false according the insertion result */
	return result;
}

void Problem2::release(Tree& MTid) {

	for (int i = 0; i < usageEdge[MTid.id].edges.size(); i++) {
		// release bandwith
		t_G.E[usageEdge[MTid.id].edges[i]].b += bandwidth[MTid.id];
	}

	MTid.ct = 0;
	MTid.E.clear();
	//usageEdge[MTid.id].edges.clear();
}

void Problem2::stop(int id, Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please "only" include mutlicast trees that you added nodes in MTidForest. */
	Forest r_F;
	// find the multicast tree index
	int i = 0;
	for (; i < t_F.size && t_F.trees[i].id != id; i++);
	
	if (i == t_F.size) return;

	// release resource
	int s = t_F.trees[i].s-1;

	release(t_F.trees[i]);

	//t_F.trees[i].V.clear();

	// remove tree
	t_F.trees.erase(t_F.trees.begin() + i);
	t_F.size--;
	
	// find the most expensive cost that MTid has not yet been satisfied 
	vector<btuple> list;
	int tid, t, penalty;
	for (int i = 0; i < t_F.size; i++) {
		if (t_F.trees[i].E.size() == 0) {
			s = t_F.trees[i].s -1;
			tid = t_F.trees[i].id;
			t = bandwidth[tid];
			penalty = t * (call - start[tid]) * (call - start[tid]);

			list.push_back({penalty, t , i});
		}
	}

	sort(list.begin(), list.end(), greater<btuple>());
	cout << list.size() << endl;

	// allocate resource
	for (auto e : list) {
		if (steiner(t_F.trees[get<2>(e)])) congestion_window[t_F.trees[get<2>(e)].s-1]--;
	}
	
	/*int index;
	for (index = 0; index < list.size(); index++) {
		if (index == STOP) break;
		if (steiner(t_F.trees[get<2>(list[index])])) congestion_window[t_F.trees[get<2>(list[index])].s-1]--;
	}
	for (index = list.size()-1; index >= 0; index--) {
		if (index == list.size()- 1 -STOP) break;
		if (steiner(t_F.trees[get<2>(list[index])])) congestion_window[t_F.trees[get<2>(list[index])].s-1]--;
	}*/

	// return Forest 
	for (auto e : t_F.trees) {
		if (e.E.size() != 0) {
			r_F.trees.push_back(e);
			r_F.size++;
		}
	}

	MTidForest = r_F;
	G = t_G;

	call++;
	return;
}

void Problem2::rearrange(Graph &G, Forest &MTidForest) {
	/* Store your output graph and multicast tree forest into G and MTidForest
	   Note: Please include "all" active mutlicast trees in MTidForest. */

	// because steiner is better enough, so we just need to return Multicast Tree
	// before rearrange stop has already try to satisfy any number of the tree, so rearrange would not modify any tree
	// due to no resource release
	// return Forest 
	Forest r_F;

	for (auto e : t_F.trees) {
		if (e.E.size() != 0) {
			r_F.trees.push_back(e);
			r_F.size++;
		}
	}

	G = t_G;
	MTidForest = r_F;

	call++;
	return;
}

void Problem2::printTree(Tree &MTid) {
	// find the tree in MTidForest by id

	cout << "===   [" << MTid.id << "]    ===\n";

	// print the tree information
	Tree t = MTid;
	cout << "source: " << t.s << "\n";
	cout << "Vertex: { ";

	for (int it : t.V) {
		cout << it << " ";
	}

	cout << "}" << "\n";
	cout << "edge: " << "\n";

	for (treeEdge edge : t.E) {
		cout << edge.vertex[0] << " " << edge.vertex[1] << "\n";
	}

	cout << "total cost: " << t.ct << "\n";
	cout << "=======End======\n";

	return;
}

void Problem2::printAdj() {
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

void Problem2::printGraph(Graph G) {
	std::cout << "===Graph Edge===\n";
	for (auto it = G.E.begin(); it < G.E.end(); it++) {
		std::cout << it->vertex[0] << " " << it->vertex[1] << ": " << it->b << "/" << it->ce << "\n";
	}
	std::cout << "=======End======\n";
}

void Problem2::printForest(Forest &MTidForest) {
	for (int i = 0; i < MTidForest.size; i++) {
		printTree(MTidForest.trees[i]);
	}
}