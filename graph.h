#pragma once
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
using namespace std;

class Graph {
	private:
		vector<pair<int, double>> edges;
		vector<int> indices;
		map<string, int> name_to_idx;
		vector<string> idx_to_name;

	public:
		Graph();
		Graph(string file_path);
		void addNode(string name);
		bool containsNode(string name);
		bool containsEdge(string from, string to);
		void addEdge(string from, string to, double w);
		int getEdge(string from, string to);
		void removeEdge(int from_idx, int to_idx);
		void removeEdge(string from, string to);
		void removeNode(string name);
		void printEdges();
		void like(string from, string to);

		/*
		//DEBUG
		void logNameToIdx() {
		cout << "name to index map" << endl;
		for (const auto &p : name_to_idx) {
		cout << p.first << " " << p.second << endl;
		}
		cout << "~~~~~~~~~" << endl;
		}

		void logIdxToName() {
		cout << "index to name map" << endl;
		for (int i=0; i<idx_to_name.size(); i++) {
		cout << i << " " << idx_to_name[i] << endl;
		}
		cout << "~~~~~~~~~" << endl;
		}

		void logIndices() {
		cout << "indices" << endl;
		for (int i=0; i<indices.size(); i++) {
		cout << i << " " << indices[i] << endl;
		}
		cout << "~~~~~~~~~" << endl;
		}
		*/
};
