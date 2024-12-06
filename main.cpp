#include "graph.h"
#include "graphexcep.h"
#include <iostream>
#include <string>

void log_instructions() {
	std::cout << "+--------------------------------------------+" << std::endl;
	std::cout << "| 1 - add node                               |" << std::endl;
	std::cout << "| 2 - add edge                               |" << std::endl;
	std::cout << "| 3 - remove node                            |" << std::endl;
	std::cout << "| 4 - remove edge                            |" << std::endl;
	std::cout << "| 5 - like                                   |" << std::endl;
	std::cout << "| 6 - load from file                         |" << std::endl;
	std::cout << "| 7 - most probbable path                    |" << std::endl;
	std::cout << "| 8 - largest strongly connected component   |" << std::endl;
	std::cout << "| 9 - k-th influncer                         |" << std::endl;
	std::cout << "| 10 - print                                 |" << std::endl;
	std::cout << "| 11 - quit                                  |" << std::endl;
	std::cout << "+--------------------------------------------+" << std::endl;
}

void handle_add_node(Graph &g) {
	std::string node_name;
	std::cout << "Enter node name: ";
	std::cin >> node_name;
	try {
		g.addNode(node_name);
		std::cout << "\nNode added successfully!" << std::endl;
	} catch (NodeExists e) {
		std::cout << "\nNode with name " << node_name << " already exists.";
	}
}

void handle_add_edge(Graph &g) {
	std::string src_name, dest_name;
	double w;
	std::cout << "Enter source node name: ";
	std::cin >> src_name;
	std::cout << "Enter destination node name: ";
	std::cin >> dest_name;
	std::cout << "Enter weight: ";
	std::cin >> w;

	try {
		g.addEdge(src_name, dest_name, w);
		std::cout << "\nEdge added successfully!" << std::endl;
	} catch (NoNode e) {
		std::cout << "\nNo node with name " << e.node_name << "." << std::endl;
	} catch (EdgeExists e) {
		std::cout << "\nEdge already exists." << std::endl;
	} catch (InvalidWeight e) {
		std::cout << "\nInvalid weight." << std::endl;
	}
}

void handle_remove_node(Graph &g) {
	std::string node_name;
	std::cout << "Enter node name: ";
	std::cin >> node_name;

	try {
		g.removeNode(node_name);
		std::cout << "\nNode removed successfully!" << std::endl;
	} catch (NoNode e) {
		std::cout << "\nNode doesn't exist." << std::endl;
	}
}

void handle_remove_edge(Graph &g) {
	std::string src_name, dest_name;
	std::cout << "Enter source node name: ";
	std::cin >> src_name;
	std::cout << "Enter destination node name: ";
	std::cin >> dest_name;

	try {
		g.removeEdge(src_name, dest_name);
		std::cout << "\nEdge removed successfully!" << std::endl;
	} catch (NoNode e) {
		std::cout << "\nNode with name " << e.node_name << " doesn't exist." << std::endl;
	} catch (NoEdge e) {
		std::cout << "\nEdge from " << src_name << " to " << dest_name << " doesn't exits." << std::endl;
	}
}

void handle_like(Graph &g) {
	std::string src_name, dest_name;
	std::cout << "Enter like source node name: ";
	std::cin >> src_name;
	std::cout << "Enter like destination node name: ";
	std::cin >> dest_name;

	try {
		g.like(src_name, dest_name);
		std::cout << "\nLike added successfully!" << std::endl;
	} catch (NoNode e) {
		std::cout << "\nNode with name " << e.node_name << " doesn't exist." << std::endl;
	} catch (NoEdge e) {
		std::cout << "\nEdge from " << src_name << " to " << dest_name << " doesn't exits." << std::endl;
	}
}

void handle_load_from_file(Graph &g) {
	std::string file_path;
	std::cout << "Enter file path: ";
	std::cin >> file_path;

	try {
		g = Graph(file_path);
		std::cout << "\nGraph loaded from a file successfully!" << std::endl;
	} catch (FileError e) {
		std::cout << "\nCan't open file." << std::endl;
	} catch (NodeExists e) {
		std::cout << "\nDuplicate node." << std::endl;
	} catch (NoNode e) {
		std::cout << "\nEdge from/to node that doesn't exists." << std::endl;
	} catch (EdgeExists e) {
		std::cout << "\nDuplicate edge." << std::endl;
	} catch (InvalidWeight e) {
		std::cout << "\nInvalid weight" << std::endl;
	}
}

void handle_most_probable_path(Graph &g) {
	std::string src_name, dest_name;
	std::cout << "Enter source node name: ";
	std::cin >> src_name;
	std::cout << "Enter destination node name: ";
	std::cin >> dest_name;

	try {
		std::cout << std::endl << g.mostProbablePath(src_name, dest_name) << std::endl;
	} catch (NoNode e) {
		std::cout << "\nNode with name " << e.node_name << " doesn't exist." << std::endl;
	}
}

//sta ako se ovaj metod pozove na praznom grafu
void handle_largest_scc(Graph &g) {
	try {
		Vector<std::string> lscc = g.largestSCC();
		std::cout << "\n{" << lscc[0];
		for (int i=1; i<lscc.size(); i++) {
			std::cout << ", " << lscc[i];
		}

		std::cout << "}" << std::endl;
	} catch (EmptyGraph e) {
		std::cout << "\nGraph is empty!" << std::endl;
	}
}

void handle_kth_influencer(Graph &g) {
	std::cout << "Enter k: ";
	int k;
	std::cin >> k;
	try {
		std::cout << g.kthInfluencer(k) << std::endl;
	} catch (InvalidKValue e) {
		std::cout << "\nInvalid value for k." << std::endl;
	}
}

void handle_print(Graph &g) {
	std::cout << std::endl;
	g.printEdges();
}

int main() {
	Graph g = Graph();

	int instr;
	bool quit = false;
	while (!quit) {
		std::cout << std::endl;
		log_instructions();

		std::cout << "Enter instruction number: ";
		std::cin >> instr;

		if (instr<1 || instr>11) {
			std::cout << "Invalid instruction number." << std::endl;
			continue;
		}

		switch (instr) {
			case 1:
				handle_add_node(g);
				break;
			case 2:
				handle_add_edge(g);
				break;
			case 3:
				handle_remove_node(g);
				break;
			case 4:
				handle_remove_edge(g);
				break;
			case 5:
				handle_like(g);
				break;
			case 6: 
				handle_load_from_file(g);
				break;
			case 7: 
				handle_most_probable_path(g);
				break;
			case 8: 
				handle_largest_scc(g);
				break;
			case 9: 
				handle_kth_influencer(g);
				break;
			case 10:
				handle_print(g);
				break;
			case 11:
				quit = true;
				break;
		}
	}
}

