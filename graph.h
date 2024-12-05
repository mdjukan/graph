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

		//TODO bsearch -> odrazavamo niz imena sortiran
		//{(idx, name)} on je sortiran
		//binarna pretraga po imenu -> O(logn)
		//dodajem novi cvor -> O(n)  usteda je, ali
		//O(1)
		//
		//........X..........
		//        ^
		//        |
		int idxFromName(std::string name);

	public: //TEMP public
		Vector<Vector<double>> allPairMostProbbalePath();
		Vector<double> influence();

	public:
		//TODO neki metodio bi trebali da postanu private
		Graph();
		Graph(std::string file_path);
		int numNodes();
		void addNode(std::string name);
		bool containsNode(std::string name);
		bool containsEdge(std::string from, std::string to);
		void addEdge(std::string from, std::string to, double w);
		int getEdge(std::string from, std::string to);
		int getEdge(int from_idx, int to_idx);
		double getEdgeWeight(int from_idx, int to_idx);
		void removeEdge(int from_idx, int to_idx);
		void removeEdge(std::string from, std::string to);
		void removeNode(std::string name);
		void printEdges();
		void like(std::string from, std::string to);
		Vector<Vector<std::string>> scc();
		std::string mostProbablePath(std::string source, std::string dest);

		//sta ako k nije validna vrednost
		//k<0 || k>=numNodes() -> exception
		//throw std::string("invalid value for k...")
		
		std::string kthInfluencer(int k);


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
