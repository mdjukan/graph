#include "graph.h"
#include "graphexcep.h"
#include <iostream>
#include <string>
using namespace std;

void log_instructions() {
	cout << "+----------------------------------------+" << endl;
	cout << "| ADD node                               |" << endl;
	cout << "| ADD from to weight                     |" << endl;
	cout << "| REMOVE node                            |" << endl;
	cout << "| REMOVE from to                         |" << endl;
	cout << "| LIKE from to                           |" << endl;
	cout << "| LOAD file                              |" << endl;
	cout << "| PRINT                                  |" << endl;
	cout << "| CLEAR                                  |" << endl;
	cout << "| QUIT                                   |" << endl;
	cout << "+----------------------------------------+" << endl;
}

vector<string> split_to_words(string instr) {
	string word;
	vector<string> words;

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

	//TODO sta rade addNode, addEdge ukoliko node/edge vec postoji
	//addEdge -> edge alafd.af.a.
	//addNode -> node
	//
	//removeNode -> NoNode              |
	//removeEdge -> NoNode, NoEdge      | > GraphExcep

	string instr;
	while(true) {
		cout << "$ ";
		getline(cin, instr);
		vector<string> words = split_to_words(instr);
		if (words[0]=="ADD") {
			try {
				if (words.size()==2) {
					graph->addNode(words[1]); //sta ako cvor postoji u grapu
				} else if (words.size()==4) {
					graph->addEdge(words[1], words[2], stod(words[3])); //sta ako grana postoji u grapu
				} else {
					cout << "Invalid instruction" << endl;
				}
			} catch (GraphExcep *e) {
				cout << e->message() << endl;
				delete e;
			}
		} else if (words[0]=="REMOVE") {
			try {
				if (words.size()==2) {
					graph->removeNode(words[1]);
				} else if(words.size()==3) {
					graph->removeEdge(words[1], words[2]);
				} else {
					cout << "Invalid instruction" << endl;
				}
			} catch (GraphExcep *e) {
				cout << e->message() << endl;
				delete e;
			}
		} else if (words[0]=="LOAD") {
			if (words.size()!=2) {
				cout << "Invalid instruction" << endl;
			}

			try {
				Graph *new_graph = new Graph(words[1]);
				delete graph;
				graph = new_graph;
			} catch (string err) {
				cout << err << endl;
			}
		} else if (words[0]=="PRINT") {
			if (words.size()!=1) {
				cout << "Invalid instruction" << endl;
			}

			graph->printEdges();
		} else if (words[0]=="CLEAR") {
			if (words.size()!=1) {
				cout << "Invalid instruction" << endl;
			}

			//graph->clear();
		} else if (words[0]=="QUIT") {
			if (words.size()!=1) {
				cout << "Invalid instruction" << endl;
			}

			break;
		} else if (words[0]=="LIKE") {
			if (words.size()!=3) {
				cout << "Invalid instruction" << endl;
			}

			try {
				graph->like(words[1], words[2]);
			} catch (GraphExcep *e) {
				cout << e->message() << endl;
				delete e;
			}
		} else {
			cout << "Invalid instruction" << endl;
		}
	}

	delete graph;
	return 0;
}
