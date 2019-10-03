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
#include <map>

#include "exception.hpp"
#include "global.hpp"

namespace LSCL
{
	
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
	
	enum SCALARTYPE
	{
		SCALARTYPE_NONE,
		SCALARTYPE_LONGNUMBER,
		SCALARTYPE_INT8,
		SCALARTYPE_INT16,
		SCALARTYPE_INT32,
		SCALARTYPE_INT64,
		SCALARTYPE_UINT8,
		SCALARTYPE_UINT16,
		SCALARTYPE_UINT32,
		SCALARTYPE_UINT64,
		SCALARTYPE_FLOAT,
		SCALARTYPE_DOUBLE,
		SCALARTYPE_LONG_DOUBLE
	};
	
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
	
	class Node_internal
	{
	private:
		
	protected:
		
	public:
		
		Node_internal *parent;
		NODETYPE type;
		friend class Nodebuilder::Builder;
		std::string value; // SCALAR: scalar value
		std::vector<LSCL::Node_internal> values_list; // LIST: list of values
		std::map<std::string, Node_internal> values_map; // MAP: map of values
		Node_internal *linked; // Pointer to linked node 
		
		// Default constructor
		Node_internal(void);
		// Any type (empty) - constructs empty node
		Node_internal(
			const NODETYPE nt, 
			Node_internal *parent = nullptr
		);
		// Scalar
		Node_internal(
			Node_internal *parent, 
			const std::string &value
		);
		// List
		Node_internal(
			Node_internal *parent, 
			const std::vector<Node_internal> &value
		);
		// Map
		Node_internal(
			Node_internal *parent, 
			const std::map<std::string, Node_internal> &value
		);
		
		virtual ~Node_internal(void);
		
		//virtual NODETYPE get_type(void) const;
		bool is(NODETYPE nodetype) const;
		
		template <typename T>
		T get(void) const;
		
		size_t size(void) const;
	};
	
	
	
	
	//=====[ S C A L A R ]=====//
	
	// Numbers
	template <typename T>
	T Node_internal::get(void) const
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
	template <> inline std::string Node_internal::get<std::string>(void) const
	{
		return value;
	}
	
	// Wstrings
	template <> inline std::wstring Node_internal::get<std::wstring>(void) const
	{
		return string_to_wstring(value);
	}
	
} // Namespace LSCL

#endif // _H_LSCL_NODE_INTERNAL
