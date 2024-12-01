#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>
#include "graphexcep.h"
#include "graph.h"
using namespace std;

Graph::Graph() {
	indices.push_back(0);
}

Graph::Graph(string file_path) {
	indices.push_back(0);

	ifstream fs(file_path);
	if (!fs.is_open()) {
		throw string("Can't open file");
	}

	int num_node;
	fs >> num_node;

	int num_edges;
	fs >> num_edges;

	string node;
	for (int i=0; i<num_node; i++) {
		fs >> node;
		try {
			addNode(node); //neobradjen izuzetak
		} catch (GraphExcep *e) {
			delete e;
			throw string("Error while parsing the file, duplicate node");
		}
	}

	string from, to;
	double w;

	for (int i=0; i<num_edges; i++) {
		fs >> from >> to >> w;
		try {
			addEdge(from, to, w);
		} catch (GraphExcep *e) {
			cout << from << " " << to << endl;
			delete e;
			throw string("Error while parsing the file, duplicate edge");
		}
	}
}

void Graph::addNode(string name) {
	if (containsNode(name)) {
		throw new NodeExists(name);
	}

	idx_to_name.push_back(name);
	int idx = idx_to_name.size()-1;
	name_to_idx[name] = idx;
	indices.push_back(edges.size());
}


bool Graph::containsNode(string name) {
	return name_to_idx.find(name)!=name_to_idx.end();
}

bool Graph::containsEdge(string from, string to) {
	int from_idx = name_to_idx[from];
	for (int i=indices[from_idx]; i < indices[from_idx+1]; i++) {
		if (edges[i].first==name_to_idx[to]) {
			return true;
		}
	}

	return false;
}

void Graph::addEdge(string from, string to, double w) {
	if (!containsNode(from)) {
		throw new NoNode(from);
	}

	if (!containsNode(to)) {
		throw new NoNode(to);
	}

	if (containsEdge(from, to)) {
		throw new EdgeExists(from, to);
	}

	int from_idx = name_to_idx[from];
	int to_idx = name_to_idx[to];
	edges.insert(edges.begin() + indices[from_idx+1], make_pair(to_idx, w));


	for (int i=from_idx+1; i<indices.size(); i++) {
		indices[i] += 1;
	}
}

//indx para koji opisuje granu u niziu edges, -1 ukoliko takve grane nema
int Graph::getEdge(string from, string to) {
	int from_idx = name_to_idx[from];
	int to_idx = name_to_idx[to];

	for (int i=indices[from_idx]; i<indices[from_idx+1]; i++) {
		if (edges[i].first==to_idx) {
			return i;
		}
	}

	return -1;
}

void Graph::removeEdge(int from_idx, int to_idx) {
	removeEdge(idx_to_name[from_idx], idx_to_name[to_idx]);
}


void Graph::removeEdge(string from, string to) {
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

	edges.erase(edges.begin()+edge_idx, edges.begin()+edge_idx+1);
	for (int i=name_to_idx[from]+1; i<indices.size(); i++) {
		indices[i] -= 1;
	}
}

void Graph::removeNode(string name) {
	if (!containsNode(name)) {
		throw new NoNode(name);
	}

	// {0, 1}                        X         X 
	// edges : (1, 0.1),  (2, 0.5) |  | (0, 0.8)
	//            0          1             2        3
	//
	// inidices : 0, 2, 4, 5
	// new      : 0, 2, 3 <- umanjeni za broj grana koji je uklonjen
	//               ^
	//               |
	// neki cvorovi menjaju index
	// brisem sve grane od cvora sa indeksom 1 
	// indices[idx] -> indeks u edges nizu na kojiem krece zapisa njegovih grana
	// indices[idx+1] -> prva pozicija nakon njegove liste susedonsti


	int idx = name_to_idx[name];
	//uklanjnje svih grana od njega ka drugima -> brisanje njegove cele liste 
	int num_edges = indices[idx+1] - indices[idx];
	edges.erase(edges.begin()+indices[idx], edges.begin()+indices[idx+1]);
	for (int i=idx+1; i<indices.size(); i++) {
		indices[i] -= num_edges;
	}


	//GRANE OD DRUGIH CVOROVA KA ONOM KOJI SE BRISE <----------------------
	//uklanjanje grana on grugih ka njemu -> granu po granu
	//void removeEdge(int from_idx, int to_idx) {
	vector<int> froms;
	for (int current_idx = 0; current_idx < indices.size()-1; current_idx++) {
		for (int i = indices[current_idx]; i<indices[current_idx+1]; i++) {
			if (edges[i].first==idx) {
				froms.push_back(current_idx);
			}
		}
	}


	for (int from : froms) {
		removeEdge(from, idx);
	}


	//uklanjenje obrisanog cvora iz indices

	//[1] idx_to_name -> unlanjenje elementa sa indeksom idx
	idx_to_name.erase(idx_to_name.begin()+idx, idx_to_name.begin()+idx+1);

	//[2] name_to_idx -> iteriram kroz sve elemente, gde je indeks >idx umanjim ga za 1
	map<string, int> new_name_to_idx;
	name_to_idx.erase(name);
	for (const pair<string,int> &p : name_to_idx) {
		if (p.second>idx) {
			new_name_to_idx[p.first] = p.second-1;
		} else {
			new_name_to_idx[p.first] = p.second;
		}
	}

	name_to_idx = new_name_to_idx;

	//[3] indices -> uklanjem element na poziciji indices[idx]
	indices.erase(indices.begin()+idx, indices.begin()+idx+1);

	//[4] edges -> iteriram kroz sve gde idx(dest)>idx umanjujem ga za 1
	for (pair<int, double> &p : edges) {
		if (p.first>idx) {
			p.first -= 1;
		}
	}

}

void Graph::printEdges() {
	for (int current_idx = 0; current_idx < indices.size()-1; current_idx++) {
		string from = idx_to_name[current_idx];
		for (int i = indices[current_idx]; i<indices[current_idx+1]; i++) {
			string to = idx_to_name[edges[i].first];
			double w = edges[i].second;
			cout << from << " " << to << " " << w << endl;
		}
	}
}

void Graph::like(string from, string to) {
	int edge_idx = getEdge(from, to);
	if (edge_idx==-1) {
		throw new NoEdge(from, to);
	}

	edges[edge_idx].second += 0.1;
}

