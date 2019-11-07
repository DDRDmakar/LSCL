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
			std::string description;
		public:
			Exception(const std::string &description = "") : description(description) {}
			Exception(const Exception&) = default;
			virtual std::string what(void) { return description; }
		};
		
		// Exceptions while parsing config
		class Exception_nodebuilder : public Exception
		{
		protected:
			std::string filename;
			size_t lineno;
		public:
			Exception_nodebuilder(
				const std::string &description = "", 
				const std::string &filename = "", 
				const size_t lineno = 0
			)
			:
				Exception(description),
				filename(filename),
				lineno(lineno)
			{}
			
			std::string what(void) override {
				return "LSCL nodebuilder exception" + 
				(
					filename.empty() ? 
					"" : 
					" [file " + filename + "] [line " + std::to_string(lineno) + "]"
				) + 
				": " + description;
			}
		};
		
		// Access exception
		class Exception_access : public Exception
		{
		public:
			Exception_access(const std::string &desc = "") : Exception(desc) {}
			std::string what() override { return "LSCL access exception: " + description; }
		};
		
		// Modify exception
		class Exception_modify: public Exception
		{
		public:
			Exception_modify(const std::string &desc = "") : Exception(desc) {}
			std::string what() override { return "LSCL modify exception: " + description; }
		};
		
	} // Namespace Exception
} // Namespace LSCL

#endif // _H_LSCL_EXCEPTION
