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

#include <cmath>
#include <wchar.h>
#include <sstream>

#include "global.hpp"

namespace LSCL
{

// Определяет, входит ли символ в заданный промежуток
inline bool detect_symbol_range( wchar_t symbol, wchar_t range_front, wchar_t range_back )
{
	return ( range_front <= symbol && symbol <= range_back );
}

// Определяет, есть ли в строке символы из данного промежутка
bool detect_range_one( const std::wstring &current_string , int beginning, int ending )
{
	if( beginning > ending ) return false;
	for(size_t i = 0; i < current_string.length(); ++i ) if( current_string[i] >= beginning && current_string[i] <= ending ) return true;
	return false;
}

// Определяет, все ли символы в строке попадают в промежуток
bool detect_range_all( const std::wstring &current_string , int beginning, int ending )
{
	if( beginning > ending ) return false;
	for(size_t i = 0; i < current_string.length(); ++i ) if( current_string[i] < beginning || current_string[i] > ending ) return false;
	return true;
}



// Normal text to HEX surrogates
std::wstring to_safestring(std::wstring current_line, uint8_t maxbit)
{
	int32_t maxbit_value = (int32_t)(pow(2, maxbit) - 1);
	
	replace_in_text<std::wstring>(current_line, L"\\x", L"\\\\x");
	
	for (size_t i = 0; i < current_line.length(); ++i)
	{
		if (current_line[i] > maxbit_value)
		{
			std::wstringstream current_surrogate;
			current_surrogate << std::hex << (uint32_t)current_line[i];
			std::wstring current_surrogate_line = current_surrogate.str();
			for (size_t j = 0; j < current_surrogate_line.length(); ++j)
			{
				const wchar_t e = current_surrogate_line[j];
				if (
					!detect_symbol_range(e, L'0', L'9') &&
					!detect_symbol_range(e, L'a', L'f') &&
					!detect_symbol_range(e, L'A', L'F')
				) current_surrogate_line.erase(current_surrogate_line.begin() + j--);
			}
			current_line.replace(i, 1, L"\\x" + current_surrogate_line + L";");
			i += current_surrogate_line.length() + 2;
		}
	}
	
	return current_line;
}
inline std::string to_safestring(std::string current_line, uint8_t maxbit)
{
	return wstring_to_string(to_safestring(string_to_wstring(current_line), maxbit));
}
// Специальный вызов этой функции для двухбайтных символов (<= 0x7FF)
inline std::wstring to_safestring_16(std::wstring current_line) { return to_safestring(current_line, 11); }
inline std::string  to_safestring_16(std::string  current_line) { return to_safestring(current_line, 11); }

std::wstring from_safestring(std::wstring current_line)
{
	bool escaped = false;
	std::wstring current_surrogate_code;
	
	for(size_t i = 0; i < current_line.length(); ++ i)
	{
		if (current_line[i] == L'\\')
		{
			if (escaped) 
			{
				current_line.erase(current_line.begin() + i);
				escaped = false;
			}
			else escaped = true;
			
			continue;
		}
		
		if (escaped && current_line[i] == L'x')
		{
			size_t surrogate_position = i - 1;
			wchar_t ss = current_line[++i]; // Current surrogate symbol
			
			while (
				detect_symbol_range(ss, L'0', L'9') ||
				detect_symbol_range(ss, L'a', L'f') ||
				detect_symbol_range(ss, L'A', L'F')
			)
			{
				current_surrogate_code.push_back(ss);
				ss = current_line[++i];
			}
			
			if (ss == L';')
			{
				std::wstring insertation_line;
				if (!current_surrogate_code.empty()) insertation_line.push_back((wchar_t)std::wcstoul(current_surrogate_code.c_str(), nullptr, 16));
				current_line.replace(surrogate_position, current_surrogate_code.length() + 3, insertation_line);
				i -= (current_surrogate_code.length() + 3 - insertation_line.length());
			}
			current_surrogate_code.clear();
		}
		
		escaped = false;
	}
	
	return current_line;
}
inline std::string from_safestring(std::string current_line)
{
	return wstring_to_string(from_safestring(string_to_wstring(current_line)));
}

std::string get_nodetype_name(LSCL::NODETYPE type)
{
	switch (type)
	{
		case NODETYPE_LINK: { return "LINK"; break; }
		case NODETYPE_NONE: { return "NONE"; break; }
		case NODETYPE_MAP: { return "MAP"; break; }
		case NODETYPE_LIST: { return "LIST"; break; }
		case NODETYPE_SCALAR: { return "SCALAR"; break; }
		default: return std::to_string(type);
	}
}

} // Namespace LSCL
