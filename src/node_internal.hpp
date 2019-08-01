
#ifndef _H_LSCL_NODE
#define _H_LSCL_NODE

#include <string>
#include <memory>
#include <vector>

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
	 * Each object contains and returns it's unique NODETYPE value
	 * even by Node_internal* reference. So, we can understand what
	 * node type is it.
	 * Constructor takes pointer to parent class. If it is not provided,
	 * node is treated as tree root.
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
		Node_internal *parent_;
		NODETYPE type_;
		friend class Nodebuilder::Builder;
		std::string value; // SCALAR: scalar value
		std::vector<LSCL::Node_internal> values; // LIST: list values
		
	public:
		Node_internal(const NODETYPE nt, Node_internal *parent = nullptr);
		virtual ~Node_internal(void);
		virtual NODETYPE get_type(void) const;
		bool is(NODETYPE nodetype) const;
		const Node_internal* get_parent(void) const;
		
		const Node_internal operator[](const std::string &key) const;
		      Node_internal operator[](const std::string &key)      ;
		const Node_internal operator[](const size_t      &key) const;
		      Node_internal operator[](const size_t      &key)      ;
		bool remove(const std::string& key);
		bool remove(const size_t&      key);
	};
	
} // Namespace LSCL

#endif // _H_LSCL_NODE
