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

#include <type_traits>

#include "node_internal.hpp"

/*
 
 #include <type_traits>

template <typename T>
void foo() {
    if constexpr (std::is_same_v<T, animal>) {
        // use type specific operations... 
    } 
}
 
 * */

namespace LSCL
{
	//=====[ C O N S T R U C T O R S ]=====//
	
	// Any type (empty)
	Node_internal::Node_internal(
		const NODETYPE nt, 
		Node_internal *parent
	)
	: 
		//value_prepaired_type_(SCALARTYPE_NONE),
		//value_prepaired_(nullptr), 
		parent(parent), 
		type(nt)
	{
	}
	
	// Scalar
	Node_internal::Node_internal(
		Node_internal *parent, 
		const std::string &value
	)
	: 
		//value_prepaired_type_(SCALARTYPE_NONE),
		//value_prepaired_(nullptr),
		parent(parent), 
		type(NODETYPE_SCALAR), 
		value(value)
	{
	}
	
	// List
	Node_internal::Node_internal(
		Node_internal *parent, 
		const std::vector<Node_internal> &value
	)
	: 
		//value_prepaired_type_(SCALARTYPE_NONE),
		//value_prepaired_(nullptr),
		parent(parent), 
		type(NODETYPE_LIST), 
		values_list(value)
	{
	}
	
	// Map
	Node_internal::Node_internal(
		Node_internal *parent, 
		const std::map<std::string, Node_internal> &value
	)
	: 
		//value_prepaired_type_(SCALARTYPE_NONE),
		//value_prepaired_(nullptr),
		parent(parent), 
		type(NODETYPE_MAP), 
		values_map(value)
	{
	}
	
	Node_internal::~Node_internal(void)
	{
	}
	
	//=====[ N O D E ]=====//
	
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
	
	//=====[ S C A L A R ]=====//
	
	
	
	//=====[ L I S T ]=====//
	/*
	Node_internal& Node_internal::operator[](const size_t idx)
	{
		if (idx >= values_list_.size())
		{
			throw LSCL::Exception::Exception_access("List index is out-of-range");
		}
		return values_list_[idx];
	}
	*/
	/*
	void Node_internal::insert(const Node_internal &element, const size_t idx)
	{
		if (!idx) // idx == 0
		{
			values_list_.push_back(element);
		}
		else
		{
			values_list_.insert(values_list_.begin(), element);
		}
	}
	*/
	/*
	bool Node_internal::remove(const size_t idx)
	{
		if (idx >= values_list_.size())
		{
			// throw LSCL::Exception::Exception_modify("List index is out-of-range");
			return false;
		}
		values_list_.erase(values_list_.begin() + idx);
		return true;
	}
	*/
	
	//=====[ M A P ]=====//
	/*
	Node_internal& Node_internal::operator[](const std::string &key)
	{
		
	}
	*/
	/*
	bool Node_internal::remove(const std::string &key)
	{
		
	}
	*/
	
} // Namespace LSCL
