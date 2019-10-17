#ifndef _H_LSCL_NODE_INTERNAL
#define _H_LSCL_NODE_INTERNAL

#include <string>
#include <vector>
#include <list>
#include <memory> // For smart pointers
#include <unordered_map>

#include "../cpp/global.hpp"

namespace LSCL
{
	class Node_internal
	{
	public:
		
		typedef std::vector<Node_internal*>                     lscl_list;
		typedef std::unordered_map<std::string, Node_internal*> lscl_map;
		
		Node_internal *parent;
		const NODETYPE type;
		bool has_link;
		std::string my_link_name;
		
		// Virtual destructor
		virtual ~Node_internal(void);
		
		virtual bool is(NODETYPE nodetype) const;
		
		virtual size_t size(void) const;
		virtual Node_internal& operator[](const std::string &key);
		virtual Node_internal& operator[](const size_t       idx);
	};
	
	
	
	
	
	
	
	
	class Scalar : public Node_internal
	{
	public:
		std::string value; // Scalar value
		
		template <typename T>
		T get(void) const;
	};
	
	class List : public Node_internal
	{
	public:
		lscl_list values_list;
		
		size_t size(void) const override;
		Node_internal& operator[](const size_t       idx) override;
	};
	
	class Map : public Node_internal
	{
	public:
		lscl_map  values_map;
		
		size_t size(void) const override;
		Node_internal& operator[](const std::string &key) override;
	};
	
	class Link : public Node_internal
	{
	public:
		std::list<std::string> address;
		std::string link_name;
		Node_internal *linked;
	};
	
} // Namespace LSCL

int main()
{
	return 0;
}

#endif // _H_LSCL_NODE_INTERNAL

