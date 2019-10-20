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

#ifndef _H_LSCL_NODE_INTERNAL
#define _H_LSCL_NODE_INTERNAL

#include <string>
#include <memory>
#include <vector>
#include <list>
#include <unordered_map>

#include "exception.hpp"
#include "global.hpp"

namespace LSCL
{
	
	// Declare builder class here to make it a friend of Node_internal base class
	namespace Nodebuilder
	{
		class Builder;
	}
	
	/**
	 * Each object contains and it's unique NODETYPE value
	 * even by Node_internal* reference. So, we can understand what
	 * node type is it.
	 * Constructor takes pointer to parent class. If it is not provided,
	 * node is treated as tree root.
	 * Node could be initialized with list or map of values
	 * 
	 * Scalar class - leaf of the tree
	 * Contains value and methods to interpret it
	 * 
	 * List class - list of values, which should be referenced by index
	 * or iterated through.
	 * 
	 * Map class - map of values, referenced by key
	 */
	
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
		virtual Node_internal* at(const std::string &key);
		virtual Node_internal* at(const size_t       idx);
	};
	
	
	class Scalar : public Node_internal
	{
	public:
		std::string value; // Scalar value
		
		Scalar(std::string value = "", Node_internal *parent = nullptr);
		~Scalar(void) override;
		
		template <typename T>
		T get(void) const;
	};
	
	
	class List : public Node_internal
	{
	public:
		lscl_list values_list;
		
		List(Node_internal::lscl_list *container = nullptr, Node_internal *parent = nullptr);
		~List(void) override;
		
		size_t size(void) const override;
		Node_internal* at(const size_t idx) override;
	};
	
	
	class Map : public Node_internal
	{
	public:
		lscl_map  values_map;
		
		Map(Node_internal::lscl_map *container = nullptr, Node_internal *parent = nullptr);
		~Map(void) override;
		
		size_t size(void) const override;
		Node_internal* at(const std::string &key) override;
	};
	
	
	class Link : public Node_internal
	{
	public:
		std::list<std::string> address;
		std::string link_name;
		Node_internal *linked;
		
		Link(const std::string &link_name = "", Node_internal *parent = nullptr);
		~Link(void) override;
	};
	
	
	class Node_empty : public Node_internal
	{
	public:
		Node_empty(Node_internal *parent = nullptr);
		~Node_empty(void) override;
	};
	
	
	//=====[ GET SCALAR VALUE ]=====//
	
	/*
	 * CHAR_BIT    :   8
	 * CHAR_MAX    :   127
	 * CHAR_MIN    :   -128
	 * INT_MAX     :   2147483647
	 * INT_MIN     :   -2147483648
	 * LONG_MAX    :   9223372036854775807
	 * LONG_MIN    :   -9223372036854775808
	 * SCHAR_MAX   :   127
	 * SCHAR_MIN   :   -128
	 * SHRT_MAX    :   32767
	 * SHRT_MIN    :   -32768
	 * UCHAR_MAX   :   255
	 * UINT_MAX    :   4294967295
	 * ULONG_MAX   :   18446744073709551615
	 * USHRT_MAX   :   65535
	 * 
	 * Storage size for float : 4 
	 * FLT_MAX      :   3.40282e+38
	 * FLT_MIN      :   1.17549e-38
	 * -FLT_MAX     :   -3.40282e+38
	 * -FLT_MIN     :   -1.17549e-38
	 * DBL_MAX      :   1.79769e+308
	 * DBL_MIN      :   2.22507e-308
	 * -DBL_MAX     :  -1.79769e+308
	 * Precision value: 6
	 * 
	 */
	
	// Numbers
	template <typename T>
	T Scalar::get(void) const
	{
		T v;
		if (type == NODETYPE_SCALAR)
		{
			// unsigned 1, 2, 4 bytes integers
			if (
				std::is_same<T, uint8_t>::value        ||
				std::is_same<T, uint16_t>::value       ||
				std::is_same<T, uint32_t>::value       ||
				std::is_same<T, unsigned char>::value  ||
				std::is_same<T, unsigned short>::value ||
				std::is_same<T, unsigned int>::value   ||
				std::is_same<T, unsigned long>::value
			)
			{
				v = (T)std::stoul(value);
			}
			
			// signed 1, 2, 4 bytes integers
			else if (
				std::is_same<T, int8_t>::value  ||
				std::is_same<T, int16_t>::value ||
				std::is_same<T, int32_t>::value ||
				std::is_same<T, char>::value    ||
				std::is_same<T, short>::value   ||
				std::is_same<T, int>::value     ||
				std::is_same<T, long>::value
			)
			{
				v = (T)std::stol(value);
			}
			
			// unsigned 8-byte integers
			else if (
				std::is_same<T, uint64_t>::value ||
				std::is_same<T, unsigned long long int>::value
			)
			{
				v = (T)std::stoull(value);
			}
			
			// signed 8-byte integers
			else if (
				std::is_same<T, int64_t>::value ||
				std::is_same<T, long long int>::value
			)
			{
				v = (T)std::stoll(value);
			}
			
			// float
			else if (std::is_same<T, float>::value) v = (T)std::stof(value);
			// double
			else if (std::is_same<T, double>::value) v = (T)std::stod(value);
			// long double
			else if (std::is_same<T, long double>::value) v = (T)std::stold(value);
			
			// EXCEPTION
			else throw LSCL::Exception::Exception_access("Getting scalar value of unknown type <T>");
		}
		else
		{
			switch (type)
			{
				case NODETYPE_LIST: { throw LSCL::Exception::Exception_access("Calling get<> method on LSCL list node"); break; }
				case NODETYPE_MAP: { throw LSCL::Exception::Exception_access("Calling get<> method on LSCL map node"); break; }
				default: { throw LSCL::Exception::Exception_access("Calling get<> method on LSCL non-scalar node"); break; }
			}
		}
		
		return v;
	}
	
	// Strings
	template <> inline std::string Scalar::get<std::string>(void) const
	{
		return value;
	}
	
	// Wstrings
	template <> inline std::wstring Scalar::get<std::wstring>(void) const
	{
		return string_to_wstring(value);
	}
	
} // Namespace LSCL

#endif // _H_LSCL_NODE_INTERNAL
