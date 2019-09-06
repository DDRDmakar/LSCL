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

#ifndef _H_LSCL_EXCEPTION
#define _H_LSCL_EXCEPTION

#include <exception>
#include <string>

namespace LSCL
{
	namespace Exception
	{
		// This is the base class for all exceptions
		class Exception : public std::exception
		{
			
		protected:
			const std::string description;
		public:
			
			// Constructor
			Exception(const std::string &desc = "") : description(desc) {}
			Exception(const Exception&) = default;
			
			// The destructor is not allowed to throw exceptions
			virtual ~Exception() throw() {}
			
			// Show message
			virtual const std::string what() { return "LSCL exception: " + description; }
		};
		
		// Nodebuilder exception
		class Exception_nodebuilder : public Exception
		{
		protected:
			std::string filename;
			unsigned int line; // Line counter value
		public:
			Exception_nodebuilder(
				const std::string &desc = "", 
				const std::string &f = "", 
				const unsigned int l = 0
			) : 
				Exception(desc),
				filename(f),
				line(l)
			{}
			// virtual ~Config_exception() throw() {}
			const std::string what() {
				return "LSCL nodebuilder exception" + 
				(filename.empty() ? "" : " [file " + filename + "]") + 
				" [line " + std::to_string(line) + "]: " + 
				description;
			}
		};
		
		// Access exception
		class Exception_access : public Exception
		{
		public:
			Exception_access(const std::string &desc = "") : Exception(desc) {}
			const std::string what() { return "LSCL access exception: " + description; }
		};
		
		// Modify exception
		class Exception_modify: public Exception
		{
		public:
			Exception_modify(const std::string &desc = "") : Exception(desc) {}
			const std::string what() { return "LSCL modify exception: " + description; }
		};
		
	} // Namespace Exception
} // Namespace LSCL

#endif // _H_LSCL_EXCEPTION
