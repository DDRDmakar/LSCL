
#include "../lscl.hpp"

#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

namespace LSCL
{

enum Node_type
{
	VOID, // Restricted to use
	MAP,
	LIST,
	SCALAR
};

class Node
{
public:
	void free(void);
	Node_type get_type(void);
protected:
	Node_type type;
};

class List : Node
{
private:
	std::vector<Node> storage; // TODO reserve space in vector
};

class Scalar : Node
{
	std::string storage;
};

class Map : Node
{
	std::unordered_map<std::string, Node> storage;
};


class Nodebuilder
{
public:
	Nodebuilder(const std::string &input);
	
	Node build_node_from_text(const std::string &input);
	
	Map    build_map_from_text(const std::string &input, std::string::const_iterator &it);
	List   build_list_from_text(void/*TODO*/);
	Scalar build_scalar_from_text(/*TODO*/);
};

inline bool skip_spaces(const std::string &input, std::string::const_iterator &it)
{
	bool newline_skipped = false;
	
	while
	(
		it != input.cend() && 
		(
			*it == ' '    ||
			*it == '\t'   ||
			*it == '\x20' ||
			*it == '\xA0'
		)
	)
	{
		++it;
		if (it != input.cend() && *it == '\n') newline_skipped = true;
	}
	
	return newline_skipped;
}

Map Nodebuilder::build_map_from_text(const std::string &input, std::string::const_iterator &it)
{
	unsigned int level = 0;
	Map result;
	
	while (it != input.cend())
	{
		skip_spaces(input, it); // Here i have some symbol, which is not space.
		if (!level && *it == '}') return result;
		
		
	}
	
	return result;
}

Node Nodebuilder::build_node_from_text(const std::string &input)
{
	std::cout << "Building node from text" << std::endl;
	
	Node root;
	
	size_t line_counter = 0;
	
	for (std::string::const_iterator it = input.cbegin(); it != input.cend(); ++it)
	{
		++line_counter;
		
		std::cout << "aaaaaaaaaa " << *it << std::endl;
	}
	
	return root;
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
