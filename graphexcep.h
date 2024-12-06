#pragma once
#include <sstream>
#include <iostream>
#include <string>


class NoNode {
	public:
		std::string node_name;
		NoNode(std::string node_name) : node_name(node_name) {}
};

class NoEdge {};
class NodeExists {};
class EdgeExists {};
class FileError {};
class InvalidWeight {};
class InvalidKValue {};
class EmptyGraph {};
