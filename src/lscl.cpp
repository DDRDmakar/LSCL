
#include "../lscl.hpp"

#include <string>
#include <iostream>

using namespace std;

namespace LSCL
{

class Node
{
	
};

class Nodebuilder
{
public:
	Nodebuilder(const std::string &input);
	
	Node build_node_from_text(const std::string &input);
};

Node Nodebuilder::build_node_from_text(const std::string &input)
{
	std::cout << "Building node from text" << std::endl;
	
	Node node;
	return node;
}

Nodebuilder::Nodebuilder(const std::string &input)
{
	std::cout << "Nodebuilder constructor" << std::endl;
	build_node_from_text(input);
}

int testfunction(void)
{
	Nodebuilder builder("a: a\n b: b");
	return 228;
}



} // namespace LSCL
