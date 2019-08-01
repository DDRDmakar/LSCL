
#include <stdint.h>
#include <vector>
#include <string>
#include <wchar.h>

#ifndef _H_LSCL_GLOBAL
#define _H_LSCL_GLOBAL

// Global variables and functions

namespace LSCL
{

/**
 * This function template transforms utf-8-encoded sequence into sequence of 32-bit integers.
 * Both I and O are classes, containers such as std::string or std::vector<>
 */
template <class I, class O>
O utf8_to_uint32(const I &inp) noexcept
{
	// 0xxx xxxx headbyte 1 -> 0 bytes after
	// 110x xxxx headbyte 2 -> 1 bytes after
	// 1110 xxxx headbyte 3 -> 2 bytes after
	// 1111 0xxx headbyte 4 -> 3 bytes after
	// 1111 10xx headbyte 5 -> 4 bytes after
	// 1111 110x headbyte 6 -> 5 bytes after (31 bit, limit !!!)
	// 1111 1110 headbyte 7 -> 6 bytes after (reserved)
	// 1111 1111 headbyte 8 -> 7 bytes after (reserved)
	
	// 10xx xxxx addition
	
	O target;   // Target vector of values
	uint32_t current_symbol;        // Current 32-bit symbol
	unsigned int i;                 // Bytes counters. i - whole bytes count
	unsigned char j, bytes_count;   // j - bytes count in one sequence
	uint8_t sample, mask;           // First byte in sequence and mask for it
	
	i = 0; // Iterates through all input values
	while (i < inp.size())
	{
		sample = inp[i];
		bytes_count = 1; // By default sequence is 1-byte long
		mask = 0x7F; // 0b01111111;
		
		// Finding how many extra-bytes we need
		while (sample & 0x80) // 0b10000000
		{
			sample <<= 1;
			mask >>= 1;
			++bytes_count;
		}
		if (bytes_count == 2) { ++i; continue; } // Skiping extra-byte if we started from it
		if (bytes_count > 2) --bytes_count;      // Compensate that 0b10xxxxxx is at 2nd position
		
		current_symbol = inp[i] & mask;          // Writing first byte in sequence
		
		for (j = 1; j < bytes_count; ++j) // Writing next n extra-bytes
		{
			++i;
			if
			( // If vector is not over and value starts with 0b10
				i < inp.size() &&
				(inp[i] & 0xC0) == 0x80 // (inp[i] & 0b11000000) == 0b10000000
			) current_symbol = (current_symbol << 6) + (inp[i] & 0x3F); // 0b00111111
			else break;
		}
		
		target.push_back(current_symbol);
		++i;
	}
	
	return target;
}


/**
 * This function template uses function above, but takes simple array of values instead of STL container.
 * Second argument "len" is a length of given sequence
 */
template <typename I, class O>
O utf8_ptr_to_uint32(const I *inp, const unsigned int len) noexcept
{
	return utf8_to_uint32<std::vector<I>, O>
	(std::vector<I>(inp, inp + len));
}


/**
 * This function template uses function above too, but input sequence could only be array of chars (valid string)
 * So, length could be calculated automatically
 */
template <class O>
O utf8_str_to_uint32(const char *inp) noexcept
{
	return utf8_to_uint32<std::string, O>
	(std::string(inp));
}

constexpr auto utf8_vector_to_uint32_vector = &utf8_to_uint32<std::vector<uint8_t>, std::vector<uint32_t>>;
constexpr auto utf8_vector_to_wstring       = &utf8_to_uint32<std::vector<uint8_t>, std::wstring>;
constexpr auto string_to_uint32_vector      = &utf8_to_uint32<std::string, std::vector<uint32_t>>;
constexpr auto string_to_wstring            = &utf8_to_uint32<std::string, std::wstring>;

constexpr auto utf8_ptr_to_uint32_vector    = &utf8_ptr_to_uint32<uint8_t, std::vector<uint32_t>>;
constexpr auto utf8_ptr_to_wstring          = &utf8_ptr_to_uint32<uint8_t, std::wstring>;
constexpr auto char_ptr_to_uint32_vector    = &utf8_str_to_uint32<std::vector<uint32_t>>;
constexpr auto char_ptr_to_wstring          = &utf8_str_to_uint32<std::wstring>;




/**
 * This function template transforms sequence of 32-bit integers into utf-8-encoded sequence.
 * Both I and O are classes, containers such as std::string or std::vector<>
 */
template <class I, class O>
O uint32_to_utf8(const I &inp) noexcept
{
	O target;
	
	// 0xxx xxxx                                                                          7  bit
	// 110x xxxx / 10xx xxxx                                                              11 bit
	// 1110 xxxx / 10xx xxxx / 10xx xxxx                                                  16 bit
	// 1111 0xxx / 10xx xxxx / 10xx xxxx / 10xx xxxx                                      21 bit
	// 1111 10xx / 10xx xxxx / 10xx xxxx / 10xx xxxx / 10xx xxxx                          26 bit
	// 1111 110x / 10xx xxxx / 10xx xxxx / 10xx xxxx / 10xx xxxx / 10xx xxxx              31 bit
	// 1111 1110 / 10xx xxxx / 10xx xxxx / 10xx xxxx / 10xx xxxx / 10xx xxxx / 10xx xxxx  36 bit
	// 1111 1111 / ...
	// 42 bit max
	
	for (auto e : inp)
	{
		// 0    : 2^7-1
		if (e <= 0x7F)
		{
			target.push_back( e & 0x7F ); // 0111 1111
		}
		// 2^7  : 2^11-1
		else if (e <= 0x7FF)
		{
			                                  // 11000000
			target.push_back( ( e & 0x7C0 ) >> 6 | 0xC0 ); // 0x7C0 = 111 1100 0000
			target.push_back( (e & 0x3F) | 0x80 );         // 0x3F  =       11 1111
		}
		// 2^11 : 2^16-1
		else if (e <= 0xFFFF)
		{
			                                      // 11100000
			target.push_back( ( e & 0xF000 ) >> 12 | 0xE0 ); // 0xF000 = 1111 0000 0000 0000
			target.push_back( ( e & 0xFC0 ) >> 6 | 0x80 );   // 0xFC0  =      1111 1100 0000
			target.push_back( (e & 0x3F) | 0x80 );           // 0x3F   =             11 1111
		}
		// 2^16 : 2^21-1
		else if (e <= 0x1FFFFF)
		{
			                                      // 11110000
			target.push_back( ( e & 0x1C0000 ) >> 18 | 0xF0 ); // 0x1C0000 = 1 1100 0000 0000 0000 0000
			target.push_back( ( e & 0x3F000 ) >> 12 | 0x80 );  // 0x3F000  =     11 1111 0000 0000 0000
			target.push_back( ( e & 0xFC0 ) >> 6 | 0x80 );     // 0xFC0    =             1111 1100 0000
			target.push_back( (e & 0x3F) | 0x80 );             // 0x3F     =                    11 1111
		}
		// 2^21 : 2^26-1
		else if (e <= 0x3FFFFFF)
		{
			                                        // 11111000
			target.push_back( ( e & 0x3000000 ) >> 24 | 0xF8 ); // 0x3000000 = 11 0000 0000 0000 0000 0000 0000
			target.push_back( ( e & 0xFC0000 ) >> 18 | 0x80 );  // 0xFC0000  =    1111 1100 0000 0000 0000 0000
			target.push_back( ( e & 0x3F000 ) >> 12 | 0x80 );   // 0x3F000   =           11 1111 0000 0000 0000
			target.push_back( ( e & 0xFC0 ) >> 6 | 0x80 );      // 0xFC0     =                   1111 1100 0000
			target.push_back( ( e & 0x3F ) | 0x80 );            // 0x3F      =                          11 1111
		}
		// 2^26 : 2^31-1 - wchar_t limit !!!
		else if ( e <= 0x7FFFFFFF )
		{
			                                        // 11111100
			target.push_back( ( e & 0x40000000 ) >> 30 | 0xFC ); // 0x40000000 = 100 0000 0000 0000 0000 0000 0000 0000
			target.push_back( ( e & 0x3F000000 ) >> 24 | 0x80 ); // 0x3F000000 =  11 1111 0000 0000 0000 0000 0000 0000
			target.push_back( ( e & 0xFC0000 ) >> 18 | 0x80 );   // 0xFC0000   =          1111 1100 0000 0000 0000 0000
			target.push_back( ( e & 0x3F000 ) >> 12 | 0x80 );    // 0x3F000    =                 11 1111 0000 0000 0000
			target.push_back( ( e & 0xFC0 ) >> 6 | 0x80 );       // 0xFC0      =                         1111 1100 0000
			target.push_back( (e & 0x3F) | 0x80 );               // 0x3F       =                                11 1111
		}
		// 2^31 : 2^36-1
		else if ( e <= 0xFFFFFFFFF )
		{
			target.push_back( (uint8_t)0xFE ); // 1111 1110
			target.push_back( ( e & 0xFC0000000 ) >> 30 | 0x80 ); // 0xFC0000000 = 1111 1100 0000 0000 0000 0000 0000 0000 0000
			target.push_back( ( e & 0x3F000000 ) >> 24 | 0x80 );  // 0x3F000000  =        11 1111 0000 0000 0000 0000 0000 0000
			target.push_back( ( e & 0xFC0000 ) >> 18 | 0x80 );    // 0xFC0000    =                1111 1100 0000 0000 0000 0000
			target.push_back( ( e & 0x3F000 ) >> 12 | 0x80 );     // 0x3F000     =                       11 1111 0000 0000 0000
			target.push_back( ( e & 0xFC0 ) >> 6 | 0x80 );        // 0xFC0       =                               1111 1100 0000
			target.push_back( (e & 0x3F) | 0x80 );                // 0x3F        =                                      11 1111
		}
		// 2^34 : 2^42-1
		/*
		else if ( e <= 0xFFFFFFFFFFFF )
		{
			target.push_back( 0xFF ); // 1111 1111
			target.push_back( ( e & 0x3F000000000 ) >> 36 | 0x80 ); // 0x3F000000000 = 11 1111 0000 0000 0000 0000 0000 0000 0000 0000 0000
			target.push_back( ( e & 0xFC0000000 ) >> 30 | 0x80 );   // 0xFC0000000   =         1111 1100 0000 0000 0000 0000 0000 0000 0000
			target.push_back( ( e & 0x3F000000 ) >> 24 | 0x80 );    // 0x3F000000    =                11 1111 0000 0000 0000 0000 0000 0000
			target.push_back( ( e & 0xFC0000 ) >> 18 | 0x80 );      // 0xFC0000      =                        1111 1100 0000 0000 0000 0000
			target.push_back( ( e & 0x3F000 ) >> 12 | 0x80 );       // 0x3F000       =                               11 1111 0000 0000 0000
			target.push_back( ( e & 0xFC0 ) >> 6 | 0x80 );          // 0xFC0         =                                       1111 1100 0000
			target.push_back( (e & 0x3F) | 0x80 );                  // 0x3F          =                                              11 1111
		}
		*/
		
		else continue;
	}
	
	return target;
}


/**
 * This function template uses function above, but takes simple array of values instead of STL container.
 * Second argument "len" is a length of given sequence
 */
template <typename I, class O>
O uint32_ptr_to_utf8(const I *inp, const unsigned int len) noexcept
{
	return uint32_to_utf8<std::vector<I>, O>
	(std::vector<I>(inp, inp + len));
}

/**
 * This function template uses function above too, but input sequence could only be array of wide chars (wchar_t) (valid wstring)
 * So, length could be calculated automatically
 */
template <class O>
O uint32_str_to_utf8(const wchar_t *inp) noexcept
{
	return uint32_to_utf8<std::wstring, O>
	(std::wstring(inp));
}

constexpr auto uint32_vector_to_utf8_vector = &uint32_to_utf8<std::vector<uint32_t>, std::vector<uint8_t>>;
constexpr auto uint32_vector_to_string      = &uint32_to_utf8<std::vector<uint32_t>, std::string>;
constexpr auto wstring_to_utf8_vector       = &uint32_to_utf8<std::wstring, std::vector<uint8_t>>;
constexpr auto wstring_to_string            = &uint32_to_utf8<std::wstring, std::string>;

constexpr auto uint32_ptr_to_utf8_vector    = &uint32_ptr_to_utf8<uint32_t, std::vector<uint8_t>>;
constexpr auto uint32_ptr_to_string         = &uint32_ptr_to_utf8<uint32_t, std::string>;
constexpr auto wchar_ptr_to_utf8_vector     = &uint32_str_to_utf8<std::vector<uint8_t>>;
constexpr auto wchar_ptr_to_string          = &uint32_str_to_utf8<std::string>;


} // Namespace LSCL

#endif // _H_LSCL_GLOBAL
