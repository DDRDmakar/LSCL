
#include "stream.hpp"
#include "defines.hpp"

namespace LSCL
{

namespace Nodebuilder
{

/**
 * Skips spaces, tabs and newline characters.
 * Pops characters one by one, until non-spacing character is reached.
 * returns TRUE if function skips newline character.
 */
bool Stream::skip_spaces(void) const
{
	bool newline_skipped = false;
	char current_character = peek_next_char();
	
	while
	(
		current_character != EOF && 
		(
			current_character == ' '    ||
			current_character == '\t'   ||
			current_character == '\n'   ||
			current_character == '\x20' ||
			current_character == '\xA0'
		)
	)
	{
		if (current_character == '\n') newline_skipped = true;
		eat_next_char();
		current_character = peek_next_char();
	}
	
	return newline_skipped;
}

/**
 * Class constructor.
 * Initializes class from input stream,
 * reads first portion of characters into rx buffer
 */
Stream::Stream(std::istream &input) :
	in_stream(input),
	rx_buffer(new char[LSCL_PREFETCH_SIZE]),
	n_prefetched_bytes_available(0),
	n_prefetched_bytes_used(0)
{
	setup_rx_buffer();
}

/**
 * If rx buffer is empty, prefetch fixed count of new bytes from input stream.
 * It's better than fetch them one-by-one
 */
inline bool Stream::setup_rx_buffer(void) const
{
	// If no more symbols available
	if (n_prefetched_bytes_used >= n_prefetched_bytes_available)
	{
		std::streambuf *sb = in_stream.rdbuf();
		n_prefetched_bytes_available = static_cast<std::size_t>(
			sb->sgetn(
				reinterpret_cast<char*>(rx_buffer),
				LSCL_PREFETCH_SIZE
			)
		);
		// Refresh used bytes counter
		n_prefetched_bytes_used = 0;
		
		if (!n_prefetched_bytes_available) in_stream.setstate(std::ios_base::eofbit);
		return n_prefetched_bytes_available;
	}
	return true;
}

/**
 * Removes next character from input char sequence and returns it.
 */
char Stream::pop_next_char(void) const
{
	if (!setup_rx_buffer()) return '\0';
	return rx_buffer[n_prefetched_bytes_used++];
}

/**
 * Returns next character from input char sequence.
 */
inline char Stream::peek_next_char(void) const
{
	if (!setup_rx_buffer()) return '\0';
	return rx_buffer[n_prefetched_bytes_used];
}

/**
 * Removes next character from input char sequence.
 */
inline void Stream::eat_next_char(void) const
{
	if (setup_rx_buffer()) ++n_prefetched_bytes_used;
}

} // Namespace Nodebuilder

} // Namespace LSCL
