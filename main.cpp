#include "graph.h"
#include "graphexcep.h"
#include <iostream>
#include <string>

void log_instructions() {
	std::cout << "+----------------------------------------+" << std::endl;
	std::cout << "| ADD node                               |" << std::endl;
	std::cout << "| ADD from to weight                     |" << std::endl;
	std::cout << "| REMOVE node                            |" << std::endl;
	std::cout << "| REMOVE from to                         |" << std::endl;
	std::cout << "| LIKE from to                           |" << std::endl;
	std::cout << "| LOAD file                              |" << std::endl;
	std::cout << "| MPP from to                            |" << std::endl;
	std::cout << "| SCC                                    |" << std::endl;
	std::cout << "| PRINT                                  |" << std::endl;
	std::cout << "| CLEAR                                  |" << std::endl;
	std::cout << "| QUIT                                   |" << std::endl;
	std::cout << "+----------------------------------------+" << std::endl;
}

Vector<std::string> split_to_words(std::string instr) {
	std::string word;
	Vector<std::string> words;

	for (size_t i=0; i<instr.size(); i++) {
		if (isspace(instr[i])) {
			if (word.size()!=0) {
				words.push_back(word);
				word.clear();
			}
		} else {
			word.push_back(instr[i]);
		}
	}

	if (word.size()!=0) {
		words.push_back(word);
	}

	return words;
}

int main() {
	Graph *graph = new Graph();
	log_instructions();

	std::string instr;
	while(true) {
		std::cout << "$ ";
		getline(std::cin, instr);
		Vector<std::string> words = split_to_words(instr);
		if (words[0]=="ADD") {
			try {
				if (words.size()==2) {
					graph->addNode(words[1]); //sta ako cvor postoji u grapu
				} else if (words.size()==4) {
					graph->addEdge(words[1], words[2], stod(words[3])); //sta ako grana postoji u grapu
				} else {
					std::cout << "Invalid instruction" << std::endl;
				}
			} catch (GraphExcep *e) {
				std::cout << e->message() << std::endl;
				delete e;
			}
		} else if (words[0]=="REMOVE") {
			try {
				if (words.size()==2) {
					graph->removeNode(words[1]);
				} else if(words.size()==3) {
					graph->removeEdge(words[1], words[2]);
				} else {
					std::cout << "Invalid instruction" << std::endl;
				}
			} catch (GraphExcep *e) {
				std::cout << e->message() << std::endl;
				delete e;
			}
		} else if (words[0]=="LOAD") {
			if (words.size()!=2) {
				std::cout << "Invalid instruction" << std::endl;
			}

			try {
				Graph *new_graph = new Graph(words[1]);
				delete graph;
				graph = new_graph;
			} catch (std::string err) {
				std::cout << err << std::endl;
			}
		} else if (words[0]=="PRINT") {
			if (words.size()!=1) {
				std::cout << "Invalid instruction" << std::endl;
			}

			graph->printEdges();
		} else if (words[0]=="CLEAR") {
			if (words.size()!=1) {
				std::cout << "Invalid instruction" << std::endl;
			}

			//graph->clear();
		} else if (words[0]=="QUIT") {
			if (words.size()!=1) {
				std::cout << "Invalid instruction" << std::endl;
			}

			break;
		} else if (words[0]=="LIKE") {
			if (words.size()!=3) {
				std::cout << "Invalid instruction" << std::endl;
			}

			try {
				graph->like(words[1], words[2]);
			} catch (GraphExcep *e) {
				std::cout << e->message() << std::endl;
				delete e;
			}
		} else if (words[0]=="SCC") {
			if (words.size()!=1) {
				std::cout << "Invalid instruction" << std::endl;
			}

			Vector<Vector<std::string>> sccs = graph->scc();
			for (int i=0; i<sccs.size(); i++) {
				std::cout << "{" << sccs[i][0];
				for (int j=1; j<sccs[i].size(); j++) {
					std::cout << ", " << sccs[i][j];
				}

				std::cout << "}" << std::endl;
			}
		} else if (words[0]=="MPP") {
			if (words.size()!=3) {
				std::cout << "Invalid instruction" << std::endl;
			}

			std::cout << graph->mostProbablePath(words[1], words[2]) << std::endl;
		} else {
			std::cout << "Invalid instruction" << std::endl;
		}
	}

	delete graph;
	return 0;
}
