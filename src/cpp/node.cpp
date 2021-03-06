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

#include "node.hpp"

namespace LSCL
{
	Node::Node(Node_internal &newcore) : core(&newcore)  { check_core(); }
	Node::Node(Node_internal *newcore) : core( newcore)  { check_core(); }
	Node::Node(const std::unique_ptr<Node_internal> &newcore) : core(newcore.get()) { check_core(); }
	
	inline void Node::check_core(void) const
	{
		// Throw an exception if node si constructed from link or attached node
		if (core->type == NODETYPE_ATTACHED || core->type == NODETYPE_LINK)
			throw LSCL::Exception::Exception_access("Invalid Node constructor arguments. Node core cannot be of type ATTACHED or LINK, it is LSCL internal type.");
	}
	
	Node_internal* Node::resolve_destination_node(Node_internal *destination) const
	{
		while (true)
		{
			switch (destination->type)
			{
				case NODETYPE_LINK: 
				{
					destination = static_cast<Link*>(destination)->linked;
					if (!destination) throw LSCL::Exception::Exception_access("Accessed link is NULL (not assigned)");
					break;
				}
				case NODETYPE_ATTACHED: 
				{
					destination = static_cast<Attached*>(destination)->attached;
					if (!destination) throw LSCL::Exception::Exception_access("Accessed attached node is NULL (not assigned)");
					break;
				}
				default: 
				{
					return destination;
				}
			}
		}
		return destination;
	}
	
	NODETYPE Node::get_type(void)
	{
		return core->type;
	}
	
	Node Node::get_parent(void) const
	{
		return Node(core->parent);////////////////////////////////////////////////////////////////////////////////////////////////
	}
	
	size_t Node::size(void) const
	{
		return core->size();
	}
	
	//=====[ S C A L A R ]=====//
	
	
	
	//=====[ L I S T ]=====//
	
	Node Node::operator[](const size_t idx)
	{
		return Node(
			resolve_destination_node(
				static_cast<LSCL::List*>(core)->at(idx)
			)
		);
	}
	
	void Node::insert(const Node &element, const size_t idx)
	{
		insert(element.core, idx);
	}
	
	void Node::insert(Node_internal *element, const size_t idx)
	{
		if (core->type != NODETYPE_LIST) throw LSCL::Exception::Exception_modify("Trying to insert element into non-list node");
		LSCL::List::lscl_list &l = static_cast<LSCL::List*>(core)->values_list;
		
		if (idx == SIZE_MAX) // idx == last position
		{
			l.push_back(element);
		}
		else
		{
			l.insert(l.begin() + idx, element);
		}
	}
	
	bool Node::remove(const size_t idx)
	{
		if (core->type != NODETYPE_LIST) throw LSCL::Exception::Exception_modify("Trying to remove element by index from non-list node");
		LSCL::List::lscl_list &l = static_cast<LSCL::List*>(core)->values_list;
		
		if (idx >= l.size()) return false;
		delete l[idx];
		l.erase(l.begin() + idx);
		return true;
	}
	
	
	//=====[ M A P ]=====//
	
	Node Node::operator[](const std::string &key)
	{
		return Node(
			resolve_destination_node(
				static_cast<LSCL::Map*>(core)->at(key)
			)
		);
	}
	
	void Node::insert(const Node &element, const std::string &key)
	{
		insert(element.core, key);
	}
	
	void Node::insert(Node_internal *element, const std::string &key)
	{
		if (core->type != NODETYPE_MAP) throw LSCL::Exception::Exception_modify("Trying to insert element by key into non-map node");
		static_cast<LSCL::Map*>(core)->values_map.emplace(std::make_pair(key, element)); // Insert node
	}
	
	bool Node::remove(const std::string &key)
	{
		if (core->type != NODETYPE_MAP) throw LSCL::Exception::Exception_modify("Trying to remove element by key from non-map node");
		LSCL::Map::lscl_map &m = static_cast<LSCL::Map*>(core)->values_map;
		
		auto res = m.find(key);
		if (res != m.end()) // If such key exists
		{
			delete m[key];
			m.erase(res);
			return true;
		}
		else return false;
	}
	
} // namespace LSCL

