
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
			unsigned int line; // Line counter value
		public:
			
			// Constructor
			Exception(const std::string &desc = "", const unsigned int l = 0) : description(desc), line(l) {}
			Exception(const Exception&) = default;
			
			// The destructor is not allowed to throw exceptions
			virtual ~Exception() throw() {}
			
			// Show message
			virtual const std::string what() { return "LSCL exception: " + description; }
		};
		
		// Nodebuilder exception
		class Exception_nodebuilder : public Exception
		{
		public:
			Exception_nodebuilder(const std::string &desc = "", const unsigned int l = 0) : Exception(desc, l) {}
			// virtual ~Config_exception() throw() {}
			const std::string what() { return "LSCL nodebuilder exception [line " + std::to_string(line) + "]: " + description; }
		};
		
	} // Namespace Exception
} // Namespace LSCL

#endif // _H_LSCL_EXCEPTION
