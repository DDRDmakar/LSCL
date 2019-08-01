
#include "node_internal.hpp"

namespace LSCL
{
	
	Node_internal::Node_internal(const NODETYPE nt, Node_internal *parent) : parent_(parent), type_(nt)
	{
	}
	
	Node_internal::~Node_internal(void)
	{
	}
	
	NODETYPE Node_internal::get_type(void) const { return type_; }
	bool Node_internal::is(NODETYPE nodetype) const { return get_type() == nodetype; }
	const Node_internal* Node_internal::get_parent(void) const { return parent_; }
	
	//=====[ S C A L A R ]=====//
	
	
	
	//=====[ L I S T ]=====//
	
	
	const Node_internal Node_internal::operator[](const size_t &key) const
	{
		
	}
	Node_internal Node_internal::operator[](const size_t &key)
	{
		
	}
	
	bool Node_internal::remove(const size_t &key)
	{
		
	}

	//=====[ M A P ]=====//
	
	const Node_internal Node_internal::operator[](const std::string &key) const
	{
		
	}
	
	Node_internal Node_internal::operator[](const std::string &key)
	{
		
	}
	
	bool Node_internal::remove(const std::string &key)
	{
		
	}
	
	
} // Namespace LSCL
