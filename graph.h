#pragma once
#include "vector.h"
#include "pair.h"
#include <string>
#include <sstream>
#include <iostream>

class Graph {
	private:
		Vector<Pair<int, double>> edges;
		Vector<int> indices;
		Vector<std::string> idx_to_name;

		int idxFromName(std::string name);
	public:
		Graph();
		Graph(std::string file_path);
		void addNode(std::string name);
		bool containsNode(std::string name);
		bool containsEdge(std::string from, std::string to);
		void addEdge(std::string from, std::string to, double w);
		int getEdge(std::string from, std::string to);
		void removeEdge(int from_idx, int to_idx);
		void removeEdge(std::string from, std::string to);
		void removeNode(std::string name);
		void printEdges();
		void like(std::string from, std::string to);
		Vector<Vector<std::string>> scc();

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
