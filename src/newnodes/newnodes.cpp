#ifndef _H_LSCL_NODE_INTERNAL
#define _H_LSCL_NODE_INTERNAL

#include <iostream>

#include <string>
#include <vector>
#include <list>
#include <memory> // For smart pointers
#include <unordered_map>

#include "../cpp/global.hpp"

namespace LSCL
{
	// Abstract type, cannot be created
	class Node_internal
	{
	public:
		
		typedef std::vector<Node_internal*>                     lscl_list;
		typedef std::unordered_map<std::string, Node_internal*> lscl_map;
		
		Node_internal *parent;
		const NODETYPE type;
		bool has_link;
		std::string my_link_name;
		
		// Constructor
		Node_internal(
			Node_internal *parent,
			const NODETYPE type
		);
		// Virtual destructor
		virtual ~Node_internal(void) = 0;
		
		virtual bool is(NODETYPE nodetype) const;
		
		virtual size_t size(void) const;
		virtual Node_internal& at(const std::string &key);
		virtual Node_internal& at(const size_t       idx);
	};
	
	// Constructor
	Node_internal::Node_internal(Node_internal *parent, const NODETYPE type) :
		parent (parent),
		type (type),
		has_link (false)
	{
		std::cout << "Node_internal constructor" << std::endl;
	}
	
	inline Node_internal::~Node_internal(void) { std::cout << "Node_internal destructor" << std::endl; }
	
	bool Node_internal::is(NODETYPE nodetype) const
	{
		std::cout << "Is method" << std::endl;
		return nodetype == type;
	}
	
	size_t Node_internal::size(void) const
	{
		std::cout << "Size method - throw exception" << std::endl;
		return 0;
	}
	
	Node_internal& Node_internal::at(const std::string &key)
	{
		std::cout << "[key] method - throw exception" << std::endl;
	}
	
	Node_internal& Node_internal::at(const size_t idx)
	{
		std::cout << "[idx] method - throw exception" << std::endl;
	}
	
	
	
	class Scalar : public Node_internal
	{
	public:
		std::string value; // Scalar value
		
		Scalar(std::string value = "", Node_internal *parent = nullptr);
		~Scalar(void) override;
		
		template <typename T>
		T get(void) const;
	};
	
	Scalar::Scalar(std::string value, Node_internal *parent) :
		Node_internal(parent, NODETYPE_SCALAR),
		value    (value)
	{
		std::cout << "Scalar constructor" << std::endl;
	}
	
	Scalar::~Scalar(void)
	{
		std::cout << "Scalar destructor" << std::endl;
	}
	
	template <typename T>
	T Scalar::get(void) const
	{
		std::cout << "Get some type" << std::endl;
	}
	
	
	
	
	
	
	class List : public Node_internal
	{
	public:
		lscl_list values_list;
		
		List(Node_internal *parent = nullptr);
		~List(void) override;
		
		size_t size(void) const override;
		Node_internal& at(const size_t idx) override;
	};
	
	List::List(Node_internal *parent) :
		Node_internal(parent, NODETYPE_LIST)
	{
		std::cout << "List constructor" << std::endl;
	}
	
	List::~List(void)
	{
		std::cout << "List destructor" << std::endl;
	}
	
	size_t List::size(void) const
	{
		std::cout << "Get list size" << std::endl;
		return values_list.size();
	}
	
	Node_internal& List::at(const size_t idx)
	{
		std::cout << "Get list element" << std::endl;
		/*if (values_list->size() <= idx) throw LSCL::Exception::Exception_modify("Accessing list with non-existant key (" + std::to_string(idx) + ")");
		else return values_list->at(idx);*/
		return *this;
	}
	
	
	
	class Map : public Node_internal
	{
	public:
		lscl_map  values_map;
		
		Map(Node_internal *parent = nullptr);
		~Map(void) override;
		
		size_t size(void) const override;
		Node_internal& at(const std::string &key) override;
	};
	
	Map::Map(Node_internal *parent) :
		Node_internal(parent, NODETYPE_MAP)
	{
		std::cout << "Map constructor" << std::endl;
	}
	
	Map::~Map(void)
	{
		std::cout << "Map destructor" << std::endl;
	}
	
	size_t Map::size(void) const
	{
		std::cout << "Get map size" << std::endl;
		return values_map.size();
	}
	
	Node_internal& Map::at(const std::string &key)
	{
		std::cout << "Get map element" << std::endl;
		auto res = values_map.find(key);
		/*if (res == values_map.end()) throw LSCL::Exception::Exception_modify("Accessing map with unknown key \"" + key + "\"");
		else return *res->second;*/
		return *this;
	}
	
	
	
	
	class Link : public Node_internal
	{
	public:
		std::list<std::string> address;
		std::string link_name;
		Node_internal *linked;
		
		Link(Node_internal *parent = nullptr);
		~Link(void) override;
	};
	
	Link::Link(Node_internal *parent) :
		Node_internal(parent, NODETYPE_LINK),
		linked (nullptr)
	{
		std::cout << "Link constructor" << std::endl;
	}
	
	Link::~Link(void)
	{
		std::cout << "Link destructor" << std::endl;
	}
	
} // Namespace LSCL

int main()
{
	LSCL::List l;
	LSCL::Map m;
	LSCL::Scalar s;
	LSCL::Link y;
	
	std::cout << "================================= scalar\n";
	{
		LSCL::Node_internal &n = s;
		LSCL::Node_internal *n2 = &s;
		((LSCL::Scalar*)n2)->get<int>();
		static_cast<LSCL::Scalar*>(n2)->get<int>();
		n.at("aaa");
		n.at(5);
		n.is(LSCL::NODETYPE_LINK);
	}
	std::cout << "================================= list\n";
	{
		LSCL::Node_internal *n = &l;
		n->at("aaa");
		n->at(5);
		n->is(LSCL::NODETYPE_LINK);
	}
	std::cout << "================================= map\n";
	{
		LSCL::Node_internal *n = &m;
		n->at("aaa");
		n->at(5);
		n->is(LSCL::NODETYPE_LINK);
	}
	std::cout << "================================= link\n";
	{
		LSCL::Node_internal *n = &y;
		n->at("aaa");
		n->at(5);
		n->is(LSCL::NODETYPE_LINK);
	}
	std::cout << "=================================\n";
	
	return 0;
}

#endif // _H_LSCL_NODE_INTERNAL

