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

#include <stdarg.h>

#include "node_internal.hpp"

/* 
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
	//=====[ C O N S T R U C T O R S ]=====//
	
	// Default constructor
	Node_internal::Node_internal(void) :
		parent(nullptr),
		type(NODETYPE_NONE),
		linked(nullptr)
	{}
	
	// Any type (empty)
	Node_internal::Node_internal(
		const NODETYPE nt, 
		Node_internal *parent
	)
	: 
		parent(parent), 
		type(nt),
		linked(nullptr)
	{
	}
	
	// Scalar
	Node_internal::Node_internal(
		Node_internal *parent, 
		const std::string &value
	)
	: 
		parent(parent), 
		type(NODETYPE_SCALAR), 
		value(value),
		linked(nullptr)
	{
	}
	/*
	// List
	Node_internal::Node_internal(
		Node_internal *parent, 
		const std::vector<Node_internal> &value
	)
	: 
		parent(parent), 
		type(NODETYPE_LIST), 
		values_list(value),
		linked(nullptr)
	{
	}
	
	// Map
	Node_internal::Node_internal(
		Node_internal *parent, 
		const std::map<std::string, Node_internal> &value
	)
	: 
		parent(parent), 
		type(NODETYPE_MAP), 
		values_map(value),
		linked(nullptr)
	{
	}
	*/
	
	Node_internal::~Node_internal(void)
	{
	}
	
	//=====[ M E T H O D S ]=====//
	
	size_t Node_internal::size(void) const
	{
		switch (type)
		{
			case NODETYPE_LIST: { return values_list.size(); break; }
			case NODETYPE_MAP:  { return values_map.size();  break; }
			default: throw LSCL::Exception::Exception_access("Called size() method on non-list and non-map node");
		}
	}
	
	bool Node_internal::is(NODETYPE nodetype) const { return type == nodetype; }
	
	Node_internal* Node_internal::insert_into_list(const Node_internal &node)
	{
		if (type != NODETYPE_LIST) throw LSCL::Exception::Exception_nodebuilder("Called insert_into_list() method on non-list node");
		
		values_list.push_back(node);
		return &( values_list.back() );
	}
	
	Node_internal* Node_internal::insert_into_map(const std::string &key, const Node_internal &node)
	{
		if (type != NODETYPE_MAP) throw LSCL::Exception::Exception_nodebuilder("Called insert_into_map() method on non-map node");
		
		values_map.insert(
			{
				key,
				node
			}
		);
		auto inserted = values_map.find(key);
		if (inserted == values_map.end()) return nullptr;
		else return &( inserted->second );
	}
	
} // Namespace LSCL
