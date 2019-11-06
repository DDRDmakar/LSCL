/*
 * 
 * Copyright 2019 Nikita Makarevich
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */


#include "node_internal.hpp"

/* С++17 constexpr example
#include <type_traits>
template <typename T>
void foo()
{
	if constexpr (std::is_same_v<T, animal>)
	{
		// use type specific operations... 
	}
}
*/

namespace LSCL
{
	// Constructor
	Node_internal::Node_internal(Node_internal *parent, const NODETYPE type) :
		parent (parent),
		type (type),
		has_link (false)
	{}
	
	inline Node_internal::~Node_internal(void)
	{}
	
	bool Node_internal::is(NODETYPE nodetype) const
	{
		return nodetype == type;
	}
	
	size_t Node_internal::size(void) const
	{
		throw LSCL::Exception::Exception_access("Called size() method on non-list and non-map node");
	}
	
	Node_internal* Node_internal::at(const std::string &key)
	{
		(void)key;
		throw LSCL::Exception::Exception_modify("Trying to access non-list and non-map node with KEY");
	}
	
	Node_internal* Node_internal::at(const size_t idx)
	{
		(void)idx;
		throw LSCL::Exception::Exception_modify("Trying to access non-list and non-map node with INDEX");
	}
	
	
	
	
	Scalar::Scalar(std::string value, std::string initial_form, Node_internal *parent) :
		Node_internal(parent, NODETYPE_SCALAR),
		value (value),
		initial_form (initial_form)
	{}
	
	Scalar::~Scalar(void)
	{}
	
	
	
	
	List::List(lscl_list *container, Node_internal *parent) :
		Node_internal(parent, NODETYPE_LIST)
	{
		if (container) values_list = *container;
	}
	
	List::~List(void)
	{
		for (Node_internal *e : values_list)
		{
			delete e;
		}
	}
	
	size_t List::size(void) const
	{
		return values_list.size();
	}
	
	Node_internal* List::at(const size_t idx)
	{
		if (values_list.size() >= idx) return values_list.at(idx);
		else throw LSCL::Exception::Exception_modify("Accessing list with non-existant key (" + std::to_string(idx) + ")");
	}
	
	
	
	
	Map::Map(lscl_map *container, Node_internal *parent) :
		Node_internal(parent, NODETYPE_MAP)
	{
		if (container) values_map = *container;
	}
	
	Map::~Map(void)
	{
		for (auto &e : values_map)
		{
			delete e.second;
		}
	}
	
	size_t Map::size(void) const
	{
		return values_map.size();
	}
	
	Node_internal* Map::at(const std::string &key)
	{
		auto res = values_map.find(key);
		if (res == values_map.end()) throw LSCL::Exception::Exception_modify("Accessing map with unknown key \"" + key + "\"");
		else return res->second;
	}
	
	
	
	
	Link::Link(const std::string &link_name, const bool copy, Node_internal *parent) :
		Node_internal(parent, NODETYPE_LINK),
		link_name (link_name),
		linktype (false),
		copy (copy),
		linked (nullptr)
	{}
	
	Link::Link(const bool copy, Node_internal *parent) :
		Node_internal(parent, NODETYPE_LINK),
		linktype (true),
		copy (copy),
		linked (nullptr)
	{}
	
	Link::Link(const lscl_path *address, const bool copy, Node_internal *parent) :
		Node_internal(parent, NODETYPE_LINK),
		linktype (true),
		copy (copy),
		linked (nullptr),
		address (*address)
	{}
	
	Link::~Link(void)
	{}
	
	
	
	
	Node_empty::Node_empty(Node_internal *parent) :
		Node_internal(parent, NODETYPE_NONE)
	{}
	
	Node_empty::~Node_empty(void)
	{}
	
	
	
	
	Attached::Attached(Node_internal *attached, Node_internal *parent)
	:
		Node_internal(parent, NODETYPE_ATTACHED),
		attached (attached)
	{}
	
	Attached::~Attached(void)
	{}
	
} // Namespace LSCL
