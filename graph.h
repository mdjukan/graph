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
		Vector<Pair<int, std::string>> idx_name_pairs;

		int idxFromName(std::string name);
		Vector<Vector<double>> allPairMostProbbalePath();
		Vector<double> influence();
		int numNodes();
		bool containsNode(std::string name);
		bool containsEdge(std::string from, std::string to);
		int getEdge(std::string from, std::string to);
		int getEdge(int from_idx, int to_idx);
		double getEdgeWeight(int from_idx, int to_idx);
		void removeEdge(int from_idx, int to_idx);
		int getIdxNamePair(std::string name);

	public:
		Graph();
		Graph(std::string file_path);
		void addNode(std::string name);
		void addEdge(std::string from, std::string to, double w);
		void removeEdge(std::string from, std::string to);
		void removeNode(std::string name);
		void printEdges();
		void like(std::string from, std::string to);
		Vector<std::string> largestSCC();
		std::string mostProbablePath(std::string source, std::string dest);
		std::string kthInfluencer(int k);
};
