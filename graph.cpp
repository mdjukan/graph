#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "graphexcep.h"
#include "graph.h"
#include "vector.h"
#include "stack.h"
#include "tuple.h"
#include "queue.h"

//TEMP
#include <cstdio>

Graph::Graph() {
	indices.push_back(0);
}

int Graph::getIdxNamePair(std::string name) {
	int p = 0, q = idx_name_pairs.size()-1;
	while (p<=q) {
		int m = (p+q)/2;
		if (idx_name_pairs[m].second==name) {
			return m;
		} else if (idx_name_pairs[m].second<name) {
			p = m + 1;
			q = q;
		} else {
			p = p;
			q = m - 1;
		}
	}

	return -1;
}

int Graph::idxFromName(std::string name) {
	int pair_idx = getIdxNamePair(name);
	if (pair_idx==-1) {
		return -1;
	}

	return idx_name_pairs[pair_idx].first;
}

Graph::Graph(std::string file_path) {
	indices.push_back(0);

	std::ifstream fs(file_path);
	if (!fs.is_open()) {
		throw FileError();
	}

	int num_node;
	fs >> num_node;

	int num_edges;
	fs >> num_edges;

	std::string node;
	for (int i=0; i<num_node; i++) {
		fs >> node;
		addNode(node);
	}

	std::string from, to;
	double w;

	for (int i=0; i<num_edges; i++) {
		fs >> from >> to >> w;
		addEdge(from, to, w);
	}
}

void Graph::addNode(std::string name) {
	if (containsNode(name)) {
		throw NodeExists();
	}

	idx_to_name.push_back(name);
	indices.push_back(edges.size());

	int i = 0;
	while (i<idx_name_pairs.size()) {
		if (idx_name_pairs[i].second>name) { // > koji je def std::string -> char[MAX_LEN]
			break;
		}

		i++;
	}

	idx_name_pairs.insert(i, Pair<int, std::string>(idx_to_name.size()-1, name));
}

bool Graph::containsNode(std::string name) {
	return getIdxNamePair(name)!=-1;
}

void Graph::addEdge(std::string from, std::string to, double w) {
	if (!containsNode(from)) {
		throw NoNode(from);
	}

	if (!containsNode(to)) {
		throw NoNode(to);
	}

	if (containsEdge(from, to)) {
		throw EdgeExists();
	}

	if (w<=0) {
		throw InvalidWeight();
	}

	int from_idx = idxFromName(from);
	int to_idx = idxFromName(to);

	edges.insert(indices[from_idx+1], Pair<int,double>(to_idx, w));

	for (int i=from_idx+1; i<indices.size(); i++) {
		indices[i] += 1;
	}
}

//TODO rename getEdgeIdx
int Graph::getEdge(std::string from, std::string to) {
	int from_idx = idxFromName(from);
	int to_idx = idxFromName(to);

	return getEdge(from_idx, to_idx);
}

//TODO rename getEdgeIdx
int Graph::getEdge(int from_idx, int to_idx) {
	for (int i=indices[from_idx]; i<indices[from_idx+1]; i++) {
		if (edges[i].first==to_idx) {
			return i;
		}
	}

	return -1;
}

double Graph::getEdgeWeight(int from_idx, int to_idx) {
	return edges[getEdge(from_idx, to_idx)].second;
}

bool Graph::containsEdge(std::string from, std::string to) {
	return getEdge(from, to)!=-1;
}

void Graph::removeEdge(std::string from, std::string to) {
	if (!containsNode(from)) {
		throw NoNode(from);
	}

	if (!containsNode(to)) {
		throw NoNode(to);
	}

	int edge_idx = getEdge(from, to);
	if (edge_idx==-1) {
		throw NoEdge();
	}

	edges.erase(edge_idx);
	int from_idx = idxFromName(from);
	for (int i=from_idx+1; i<indices.size(); i++) {
		indices[i] -= 1;
	}
}

void Graph::removeEdge(int from_idx, int to_idx) {
	removeEdge(idx_to_name[from_idx], idx_to_name[to_idx]);
}

void Graph::removeNode(std::string name) {
	int pair_idx = getIdxNamePair(name);

	if (pair_idx==-1) {
		throw NoNode(name);
	}

	int idx = idx_name_pairs[pair_idx].first;
	int num_edges = indices[idx+1] - indices[idx];

	edges.erase(indices[idx], indices[idx+1]);
	for (int i=idx+1; i<indices.size(); i++) {
		indices[i] -= num_edges;
	}

	Vector<int> froms;
	for (int current_idx = 0; current_idx < indices.size()-1; current_idx++) {
		for (int i = indices[current_idx]; i<indices[current_idx+1]; i++) {
			if (edges[i].first==idx) {
				froms.push_back(current_idx);
			}
		}
	}

	for (int i=0; i<froms.size(); i++) {
		removeEdge(froms[i], idx);
	}

	idx_to_name.erase(idx);
	indices.erase(idx);

	for (int i=0; i<edges.size(); i++) {
		if (edges[i].first>idx) {
			edges[i].first -= 1;
		}
	}

	idx_name_pairs.erase(pair_idx);
}

void Graph::printEdges() {
	for (int current_idx = 0; current_idx < indices.size()-1; current_idx++) {
		std::string from = idx_to_name[current_idx];
		for (int i = indices[current_idx]; i<indices[current_idx+1]; i++) {
			std::string to = idx_to_name[edges[i].first];
			double w = edges[i].second;
			std::cout << from << " " << to << " " << w << std::endl;
		}
	}
}

void Graph::like(std::string from, std::string to) {
	if (!containsNode(from)) {
		throw NoNode(from);
	}

	if (!containsNode(to)) {
		throw NoNode(to);
	}

	int edge_idx = getEdge(from, to);
	if (edge_idx==-1) {
		throw NoEdge();
	}

	edges[edge_idx].second += 0.1;
}

int min(int p, int q) {
	return p < q ? p : q;
}

#define NILL -1
Vector<std::string> Graph::largestSCC() {
	int n = numNodes();
	if (n==0) {
		throw EmptyGraph();
	}

	Vector<Vector<std::string>> sccs;

	Vector<int> disc(n, NILL);
	Vector<int> low_link(n, NILL);

	Stack<int> scc_stack;
	Vector<bool> on_stack(n, false);

	size_t time = 0;
	
	for (int i=0; i<n; i++) {
		if (disc[i]==NILL) {
			Stack<Tuple<int, int, int>> dfs_stack;

			dfs_stack.push(Tuple<int,int,int>(i, indices[i], NILL));

			while (!dfs_stack.empty()) {
				Tuple<int, int, int> t = dfs_stack.pop();
				int idx = t.first;
				int edge_idx = t.second;
				int prev_adj = t.third;

				if (prev_adj!=NILL) {
					low_link[idx] = min(low_link[idx], low_link[prev_adj]);
				}

				if (edge_idx==indices[idx]) {
					disc[idx] = time;
					low_link[idx] = time;

					scc_stack.push(idx);
					on_stack[idx] = true;

					time += 1;
				}

				bool broken = false;
				for (int j=edge_idx; j<indices[idx+1]; j++) {
					int adj_idx = edges[j].first;

					if (disc[adj_idx]==NILL) {
						//Pair-> Tuple<int,int,int> -> (idx, j+1, adj_idx)
						//kad skinem tuple -> adj_idx u prethodnom koraku je izvrsena njegova obrada
						dfs_stack.push(Tuple<int,int,int>(idx, j+1, adj_idx));

						//on ga je dodao stek
						dfs_stack.push(Tuple<int,int,int>(adj_idx, indices[adj_idx], NILL));
						//kada je ovaj skinut sa steka
						broken = true;
						break;
					} else if (on_stack[adj_idx]) {
						low_link[idx] = min(low_link[idx], disc[adj_idx]);
					}
				}

				if (!broken && low_link[idx]==disc[idx]) {
					Vector<std::string> scc;
					int popped;
					do {
						popped = scc_stack.pop();
						on_stack[popped] = false;

						scc.push_back(idx_to_name[popped]);
					} while (popped!=idx);

					sccs.push_back(scc);
				}

			}

		}
	}

	int max_len_idx = 0;
	for (int i=1; i<sccs.size(); i++) {
		if (sccs[i].size()>sccs[max_len_idx].size()) {
			max_len_idx = i;
		}
	}

	return sccs[max_len_idx];
}

#define INF -1
#define NIL -1

enum Status {
	VISITED,
	IN_QUEUE,
	NOT_VISITED
};

int getMaxInQueue(Vector<double> &dist, Vector<Status> &status) {
	int max_idx = NIL;
	for (int i = 0; i<status.size(); i++) {
		if (status[i]==Status::IN_QUEUE) {
			if (max_idx==NIL || dist[max_idx] < dist[i]) {
				max_idx = i;
			}
		}
	}

	return max_idx;
}

std::string Graph::mostProbablePath(std::string source, std::string dest) {
	if (!containsNode(source)) {
		throw NoNode(source);
	}

	if (!containsNode(dest)) {
		throw NoNode(dest);
	}

	int n = idx_to_name.size();
	Vector<double> dist(n, INF);
	Vector<int> pred(n, NIL);
	Vector<Status> status(n, Status::NOT_VISITED);
	int nodes_in_queue = 0;

	int source_idx = idxFromName(source);
	int dest_idx = idxFromName(dest);

	status[source_idx] = Status::IN_QUEUE;
	nodes_in_queue += 1;
	dist[source_idx] = 1;

	while (nodes_in_queue!=0) {
		int node = getMaxInQueue(dist, status);
		status[node] = Status::VISITED;
		nodes_in_queue -= 1;

		for (int i = indices[node]; i<indices[node+1]; i++) {
			int adj = edges[i].first;
			double edge_weight = getEdgeWeight(node, adj);

			if (status[adj]==Status::NOT_VISITED) {
				status[adj] = Status::IN_QUEUE;
				nodes_in_queue += 1;
				dist[adj] = dist[node] * edge_weight;
				pred[adj] = node;
			} else if (status[adj]==Status::IN_QUEUE) {
				if (dist[node] * edge_weight > dist[adj]) {
					dist[adj] = dist[node] * edge_weight;
					pred[adj] = node;
				}
			}
		}
	}

	if (status[dest_idx]==Status::NOT_VISITED) {
		return std::string("no path");
	} else {
		Vector<int> path;
		path.push_back(dest_idx);

		while (path[0]!=source_idx) {
			path.insert(0, pred[path[0]]);
		}

		std::stringstream s;
		for (int i=0; i<path.size(); i++) {
			s << idx_to_name[path[i]];
			if (i!=path.size()-1) {
				int edge_idx = getEdge(path[i], path[i+1]);
				s << "-(" << edges[edge_idx].second << ")->";
			}
		}

		return s.str();
	}
}

int Graph::numNodes() {
	return idx_to_name.size();
}

#define UNDEF -1
Vector<Vector<double>> Graph::allPairMostProbbalePath() {
	Vector<double> row(numNodes(), UNDEF);
	Vector<Vector<double>> d(numNodes(), row);

	for (int i=0; i<numNodes(); i++) {
		for (int j=indices[i]; j<indices[i+1]; j++) {
			d[i][edges[j].first] = edges[j].second;
		}
	}

	for (int i=0; i<numNodes(); i++) {
		d[i][i] = 1;
	}

	for (int k=0; k<numNodes(); k++) {
		for (int i=0; i<numNodes(); i++) {
			for (int j=0; j<numNodes(); j++) {
				if (d[i][k]!=UNDEF && d[k][j]!=UNDEF) {
					if (d[i][j]==UNDEF || d[i][j] < d[i][k] * d[k][j]) {
						d[i][j] = d[i][k] * d[k][j];
					}
				}
			}
		}
	}

	/*
	for (int i=0; i<numNodes(); i++) {
		for (int j=0; j<numNodes(); j++) {
			printf("%5.2f ", d[i][j]);
		}
		printf("\n");
	}
	*/

	return d;
}

Vector<double> Graph::influence() {
	Vector<double> inf(numNodes(), 0);
	Vector<Vector<double>> d = allPairMostProbbalePath();

	//jesno formulisti definiciju influence-a
	for (int i=0; i<numNodes(); i++) {
		for (int j=0; j<numNodes(); j++) {
			if (i!=j && d[i][j]!=UNDEF) {
				inf[i] += d[j][i];
			}
		}

		inf[i] /= numNodes();
	}

	return inf;
}

void swap(Pair<int, double> &x, Pair<int, double> &y) {
	int tmp = x.first;
	x.first = y.first;
	y.first = tmp;

	double _tmp = x.second;
	x.second = y.second;
	y.second = _tmp;
}

//>>>>>>><<<<<<??????????
//
int partition(Vector<Pair<int, double>> &A, int p, int q) {
	//x in [i, j) only if x > A[q]
	//x in [j, k) only if x <= A[q]
	//>>>>>>>>><<<<<<<<?????????????P
	//^        ^       ^
	//i        j       k
	//
	//[1] A[k] <= A[q]
	//>>>>>>>>><<<<<<<<<????????????P
	//^        ^        ^
	//i        j        k
	//---> k++
	//[2] A[k] > A[q]
	//>>>>>>>>>><<<<<<<<????????????P
	//^         ^       ^
	//i         j       k
	//---> swap(A[j], A[k])
	//---> j++, k++
	
	int i = p, j = p, k = p;
	while (k<q) {
		if (A[k].second > A[q].second) {
			swap(A[j], A[k]);
			j ++;
			k ++;
		} else {
			k ++;
		}
	}

	//>>>>>>>>>>>><<<<<<<<<<<<P
	//^           ^           ^
	//i           j           k=q
	//---> swap(A[j], A[q])
	//>>>>>>>>>>>>P<<<<<<<<<<<<
	//^           ^           ^
	//i           j           k=q
	
	swap(A[j], A[q]);
	return j;

}

//#include <algorithm>
//#include <vector>
//indeksiranje od 1 za korisnika
//interno je indeksirano od 0
std::string Graph::kthInfluencer(int k) {
	if (k<1 || k>numNodes()) {
		throw InvalidKValue();
	}

	k -= 1;

	Vector<double> inf = influence();
	Vector<Pair<int, double>> idx_inf;

	for (int i=0; i<inf.size(); i++) {
		idx_inf.push_back(Pair<int, double>(i, inf[i]));
	}

	int p = 0, q = inf.size()-1;

	while (true) {
		int m = partition(idx_inf, p, q);
		if (k==m) {
			break;
		} else if (k < m) {
			p = p;
			q = m - 1;
		} else { //k > m
			p = m + 1;
			q = q;
		}
	}


	/*
	int qs_res = idx_inf[k].first;
	//lambda(x, y) -> true ako x pa y dobar poredak
	//
	std::vector<std::pair<int, double>> X;
	for (int i=0; i < idx_inf.size(); i++) {
		X.emplace_back(idx_inf[i].first, idx_inf[i].second);
	}

	std::sort(X.begin(), X.end(), [](const std::pair<int, double> &x, const std::pair<int, double> &y){return x.second>y.second;});
	int correct_res = X[k].first;
	std::cout << "qs_res==correct_res -> " << (qs_res==correct_res) << std::endl;
	*/

	return idx_to_name[idx_inf[k].first];
}

