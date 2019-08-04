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
	n_prefetched_bytes_used(0),
	line(1)
{
	setup_rx_buffer();
}

/**
 * Class destructor
 */
Stream::~Stream(void)
{
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
				reinterpret_cast<char*>(rx_buffer.get()),
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
	if (rx_buffer[n_prefetched_bytes_used] == '\n') ++line;
	return rx_buffer[n_prefetched_bytes_used++];
}

/**
 * Returns next character from input char sequence.
 */
char Stream::peek_next_char(void) const
{
	if (!setup_rx_buffer()) return '\0';
	if (rx_buffer[n_prefetched_bytes_used] == '\n') ++line;
	return rx_buffer[n_prefetched_bytes_used];
}

/**
 * Removes next character from input char sequence.
 */
void Stream::eat_next_char(void) const
{
	if (setup_rx_buffer()) ++n_prefetched_bytes_used;
}

/**
 * Checks if character stream is still available
 */
Stream::operator bool() const
{
	return in_stream.good() || (peek_next_char() != '\0' && peek_next_char() != EOF);
}

unsigned int Stream::get_line(void) const
{
	return line;
}

} // Namespace Nodebuilder

} // Namespace LSCL

