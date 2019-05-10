
#ifndef _H_LSCL
#define _H_LSCL

#include <string>
#include <vector>
#include <unordered_map>

#include "../src/nodebuilder.hpp"

namespace LSCL
{
	enum Node_type
	{
		NODETYPE_VOID, // Restricted to use
		NODETYPE_MAP,
		NODETYPE_LIST,
		NODETYPE_SCALAR
	};

} // Namespace LSCL

#endif // _H_LSCL
