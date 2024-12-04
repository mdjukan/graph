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

Graph::Graph() {
	indices.push_back(0);
}

int Graph::idxFromName(std::string name) {
	for (int i=0; i<idx_to_name.size(); i++) {
		if (idx_to_name[i]==name) {
			return i;
		}
	}

	return -1;
}

Graph::Graph(std::string file_path) {
	indices.push_back(0);

	std::ifstream fs(file_path);
	if (!fs.is_open()) {
		throw std::string("Can't open file");
	}

	int num_node;
	fs >> num_node;

	int num_edges;
	fs >> num_edges;

	std::string node;
	for (int i=0; i<num_node; i++) {
		fs >> node;
		try {
			addNode(node); //neobradjen izuzetak
		} catch (GraphExcep *e) {
			delete e;
			throw std::string("Error while parsing the file, duplicate node");
		}
	}

	std::string from, to;
	double w;

	for (int i=0; i<num_edges; i++) {
		fs >> from >> to >> w;
		try {
			addEdge(from, to, w);
		} catch (GraphExcep *e) {
			std::cout << from << " " << to << std::endl;
			delete e;
			throw std::string("Error while parsing the file, duplicate edge");
		}
	}
}

void Graph::addNode(std::string name) {
	if (containsNode(name)) {
		throw new NodeExists(name);
	}

	idx_to_name.push_back(name);
	indices.push_back(edges.size());
}


bool Graph::containsNode(std::string name) {
	return idxFromName(name)!=-1;
}


void Graph::addEdge(std::string from, std::string to, double w) {
	if (!containsNode(from)) {
		throw new NoNode(from);
	}

	if (!containsNode(to)) {
		throw new NoNode(to);
	}

	if (containsEdge(from, to)) {
		throw new EdgeExists(from, to);
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
		throw new NoNode(from);
	}

	if (!containsNode(to)) {
		throw new NoNode(to);
	}

	int edge_idx = getEdge(from, to);
	if (edge_idx==-1) {
		throw new NoEdge(from, to);
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
	if (!containsNode(name)) {
		throw new NoNode(name);
	}

	int idx = idxFromName(name);
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
	int edge_idx = getEdge(from, to);
	if (edge_idx==-1) {
		throw new NoEdge(from, to);
	}

	edges[edge_idx].second += 0.1;
}

int min(int p, int q) {
	return p < q ? p : q;
}

#define NILL -1
Vector<Vector<std::string>> Graph::scc() {
	Vector<Vector<std::string>> sccs;

	int n = idx_to_name.size(); //broj cvorova u grafu
	Vector<int> disc(n, NILL); //index iz one implementacije <-------- ovo ne radi
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

	return sccs;
}

#define INF -1
#define NIL -1

enum Status {
	VISITED,
	IN_QUEUE,
	NOT_VISITED
};

int numInQueue(Vector<Status> &status) {
	int count = 0;
	for (int i=0; i<status.size(); i++) {
		if (status[i]==Status::IN_QUEUE) {
			count += 1;
		}
	}

	return count;
}

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
	int n = idx_to_name.size();
	Vector<double> dist(n, INF);
	Vector<int> pred(n, NIL);
	Vector<Status> status(n, Status::NOT_VISITED);

	int source_idx = idxFromName(source);
	int dest_idx = idxFromName(dest);

	status[source_idx] = Status::IN_QUEUE;
	dist[source_idx] = 1;

	while (numInQueue(status)!=0) {
		int node = getMaxInQueue(dist, status);
		status[node] = Status::VISITED;

		for (int i = indices[node]; i<indices[node+1]; i++) {
			int adj = edges[i].first;
			double edge_weight = getEdgeWeight(node, adj);

			if (status[adj]==Status::NOT_VISITED) {
				status[adj] = Status::IN_QUEUE;
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
