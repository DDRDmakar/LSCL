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
	
	Node::Node(Node_internal &newcore) : core(&newcore) {}
	Node::Node(Node_internal *newcore) : core(newcore)  {}
	
	Node Node::get_parent(void) const
	{
		return Node(core->parent);
	}
	
	//=====[ S C A L A R ]=====//
	
	
	
	//=====[ L I S T ]=====//
	
	Node Node::operator[](const size_t idx)
	{
		return Node(core->values_list->at(idx));
	}
	
	void Node::insert(const Node &element, const size_t idx)
	{
		if (idx == SIZE_MAX) // idx == last position
		{
			core->values_list->push_back(*element.core);
		}
		else
		{
			core->values_list->insert(core->values_list->begin() + idx, *element.core);
		}
	}
	
	void Node::insert(const Node_internal &element, const size_t idx)
	{
		if (idx == SIZE_MAX) // idx == last position
		{
			core->values_list->push_back(element);
		}
		else
		{
			core->values_list->insert(core->values_list->begin() + idx, element);
			core->values_list->operator[](idx).parent = core;
		}
	}
	
	bool Node::remove(const size_t idx)
	{
		if (idx >= core->values_list->size()) return false;
		core->values_list->erase(core->values_list->begin() + idx);
		return true;
	}
	
	
	//=====[ M A P ]=====//
	
	Node Node::operator[](const std::string &key)
	{
		auto res = core->values_map->find(key);
		if (res != core->values_map->end()) return res->second;
		else throw LSCL::Exception::Exception_modify("Accessing map with unknown key \"" + key + "\"");
	}
	
	void Node::insert(const Node &element, const std::string &key)
	{
		auto node_internal_to_insert = *element.core; // Make a full copy of internal node
		node_internal_to_insert.parent = core;        // Change parent pointer
		core->values_map->emplace(std::make_pair(key, node_internal_to_insert)); // Insert node
	}
	
	void Node::insert(const Node_internal &element, const std::string &key)
	{
		core->values_map->emplace(std::make_pair(key, element)); // Insert node
		core->values_map->operator[](key).parent = core;
	}
	
	bool Node::remove(const std::string &key)
	{
		auto res = core->values_map->find(key);
		if (res != core->values_map->end())
		{
			core->values_map->erase(res);
			return true;
		}
		else return false;
	}
	
} // namespace LSCL

