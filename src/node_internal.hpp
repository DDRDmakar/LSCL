
#ifndef _H_LSCL_NODE
#define _H_LSCL_NODE

#include <string>
#include <memory>
#include <vector>
#include <map>

#include "nodebuilder.hpp"

namespace LSCL
{
	
	enum NODETYPE
	{
		NODETYPE_NONE,
		NODETYPE_MAP,
		NODETYPE_LIST,
		NODETYPE_SCALAR
	};
	
	// Declare builder class here to make it a friend of Node_internal base class
	namespace Nodebuilder
	{
		class Builder;
	}
	
	/**
	 * Each object contains and it's unique NODETYPE value
	 * even by Node_internal* reference. So, we can understand what
	 * node type is it.
	 * Constructor takes pointer to parent class. If it is not provided,
	 * node is treated as tree root.
	 * Node could be initialized with list or map of values
	 * 
	 * Scalar class - leaf of the tree
	 * Contains value and methods to interpret it
	 * 
	 * List class - list of values, which should be referenced by index
	 * or iterated through.
	 */
	
	class Node_internal
	{
	private:
		
	protected:
		
	public:
		
		Node_internal *parent;
		NODETYPE type;
		friend class Nodebuilder::Builder;
		std::string value; // SCALAR: scalar value
		std::vector<LSCL::Node_internal> values_list; // LIST: list of values
		std::map<std::string, Node_internal> values_map; // MAP: map of values
		
		// Any type (empty) - constructs empty node
		Node_internal(
			const NODETYPE nt, 
			Node_internal *parent = nullptr
		);
		// Scalar
		Node_internal(
			Node_internal *parent, 
			const std::string &value
		);
		// List
		Node_internal(
			Node_internal *parent, 
			const std::vector<Node_internal> &value
		);
		// Map
		Node_internal(
			Node_internal *parent, 
			const std::map<std::string, Node_internal> &value
		);
		
		virtual ~Node_internal(void);
		
		//virtual NODETYPE get_type(void) const;
		bool is(NODETYPE nodetype) const;
		//const Node_internal* get_parent(void) const;
		
		//Node_internal& operator[](const std::string &key);
		//Node_internal& operator[](const size_t       idx);
		//bool remove(const std::string &key);
		//bool remove(const size_t       idx);
		//void insert(const Node_internal &element, const size_t idx = 0);
		//std::vector<Node_internal>* get_vector(void) const;
		//void set_parent(const Node_internal *parent);
		
		size_t size(void) const;
	};
	
} // Namespace LSCL

#endif // _H_LSCL_NODE
