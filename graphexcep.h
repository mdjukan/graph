#pragma once
#include <sstream>
#include <iostream>
#include <string>

class GraphExcep {
	public:
		virtual std::string message() = 0;
};

class NoNode : public GraphExcep {
	public:
		std::string name;
		NoNode(std::string name) : name(name) {}
		std::string message() override {
			std::stringstream error_msg;
			error_msg << "Graph doesn't contain node with name \"" << name << "\"";
			return error_msg.str();
		}
};

class NoEdge : public GraphExcep {
	public:
		std::string from;
		std::string to;
		NoEdge(std::string from, std::string to) : from(from), to(to) {}
		std::string message() override {
			std::stringstream error_msg;
			error_msg << "Graph doesn't contain an edge form \"";
			error_msg << from << "\" to \"" << to << "\"";
			return error_msg.str();
		}
};

class NodeExists : public GraphExcep {
	public:
		std::string name;
		NodeExists(std::string name) : name(name) {}
		std::string message() override {
			std::stringstream error_msg;
			error_msg << "Graph contains node with name \"" << name << "\"";
			return error_msg.str();
		}
};

class EdgeExists : public GraphExcep {
	public:
		std::string from;
		std::string to;
		EdgeExists(std::string from, std::string to) : from(from), to(to) {}
		std::string message() override {
			std::stringstream error_msg;
			error_msg << "Graph contain an edge form \"";
			error_msg << from << "\" to \"" << to << "\"";
			return error_msg.str();
		}
};
