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
	Node::Node(Node_internal &newcore) : core(&newcore)  {}
	Node::Node(Node_internal *newcore) : core( newcore)  {}
	Node::Node(const std::unique_ptr<Node_internal> &newcore) : core(newcore.get()) {}
	
	NODETYPE Node::get_type(void)
	{
		return core->type;
	}
	
	Node Node::get_parent(void) const
	{
		return Node(core->parent);
	}
	
	//=====[ S C A L A R ]=====//
	
	
	
	//=====[ L I S T ]=====//
	
	Node Node::operator[](const size_t idx)
	{
		return Node(static_cast<LSCL::List*>(core)->at(idx));
	}
	
	void Node::insert(const Node &element, const size_t idx)
	{
		insert(element.core, idx);
	}
	
	void Node::insert(Node_internal *element, const size_t idx)
	{
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
		LSCL::List::lscl_list &l = static_cast<LSCL::List*>(core)->values_list;
		
		if (idx >= l.size()) return false;
		delete l[idx];
		l.erase(l.begin() + idx);
		return true;
	}
	
	
	//=====[ M A P ]=====//
	
	Node Node::operator[](const std::string &key)
	{
		return Node(static_cast<LSCL::Map*>(core)->at(key));
	}
	
	void Node::insert(const Node &element, const std::string &key)
	{
		static_cast<LSCL::Map*>(core)->values_map.emplace(std::make_pair(key, element.core)); // Insert node
	}
	
	void Node::insert(Node_internal *element, const std::string &key)
	{
		static_cast<LSCL::Map*>(core)->values_map.emplace(std::make_pair(key, element)); // Insert node
		//core->values_map->operator[](key).parent = core;
	}
	
	bool Node::remove(const std::string &key)
	{
		LSCL::Map::lscl_map &m = static_cast<LSCL::Map*>(core)->values_map;
		
		auto res = m.find(key);
		if (res != m.end())
		{
			delete m[key];
			m.erase(res);
			return true;
		}
		else return false;
	}
	
} // namespace LSCL

