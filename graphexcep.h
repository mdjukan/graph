#pragma once
#include <sstream>
#include <iostream>
using namespace std;

class GraphExcep {
	public:
		virtual string message() = 0;
};

class NoNode : public GraphExcep {
	public:
		string name;
		NoNode(string name) : name(name) {}
		string message() override {
			stringstream error_msg;
			error_msg << "Graph doesn't contain node with name \"" << name << "\"";
			return error_msg.str();
		}
};

class NoEdge : public GraphExcep {
	public:
		string from;
		string to;
		NoEdge(string from, string to) : from(from), to(to) {}
		string message() override {
			stringstream error_msg;
			error_msg << "Graph doesn't contain an edge form \"";
			error_msg << from << "\" to \"" << to << "\"";
			return error_msg.str();
		}
};

class NodeExists : public GraphExcep {
	public:
		string name;
		NodeExists(string name) : name(name) {}
		string message() override {
			stringstream error_msg;
			error_msg << "Graph contains node with name \"" << name << "\"";
			return error_msg.str();
		}
};

class EdgeExists : public GraphExcep {
	public:
		string from;
		string to;
		EdgeExists(string from, string to) : from(from), to(to) {}
		string message() override {
			stringstream error_msg;
			error_msg << "Graph contain an edge form \"";
			error_msg << from << "\" to \"" << to << "\"";
			return error_msg.str();
		}
};
