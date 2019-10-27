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

#ifndef _H_LSCL_NODE
#define _H_LSCL_NODE

#include <cstddef>
#include <string>
#include <vector>

#include "node_internal.hpp"

namespace LSCL
{
	
	class Node
	{
	private:
		
		Node_internal *core; // Real node which is referenced by this object
		
	protected:
		
	public:
		Node(Node_internal &newcore);
		Node(Node_internal *newcore = nullptr);
		Node(const std::unique_ptr<Node_internal> &newcore);
		
		NODETYPE get_type(void);
		Node get_parent(void) const;
		Node operator[](const std::string &key);
		Node operator[](const size_t       idx);
		bool remove(const std::string &key);
		bool remove(const size_t       idx);
		void insert(const Node          &element, const size_t idx = SIZE_MAX);
		void insert(      Node_internal *element, const size_t idx = SIZE_MAX);
		void insert(const Node          &element, const std::string &key);
		void insert(      Node_internal *element, const std::string &key);
		
		template <typename T>
		T get(void) const;
	};
	
	
	template <typename T>
	T Node::get(void) const
	{
		switch (core->type)
		{
			case NODETYPE_SCALAR:
			{
				return static_cast<LSCL::Scalar*>(core)->get<T>();
				break;
			}
			case NODETYPE_LINK:
			{
				const Link &coreref = *( static_cast<LSCL::Link*>(core) );
				if (coreref.linked == nullptr) throw Exception::Exception_access("get<>() method called with invalid link");
				return static_cast<LSCL::Scalar*>(coreref.linked)->get<T>();
				break;
			}
			default:
			{
				throw Exception::Exception_access("get<>() method called on non-scalar node");
				break;
			}
		}
		return static_cast<LSCL::Scalar*>(core)->get<T>();
	}
	
} // Namespace LSCL

#endif // _H_LSCL_NODE
