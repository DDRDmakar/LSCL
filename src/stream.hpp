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

#ifndef _H_LSCL_STREAM
#define _H_LSCL_STREAM

#include <string>
#include <sstream>
#include <memory>

using namespace std;

namespace LSCL
{

namespace Nodebuilder
{

/*
 * It controls a stream of characters.
 * This is a class, closest to symbol-by-symbol processing of input sequences.
 * Jesse Beder, I'm sorry ;_;
 */
class Stream
{
public:
	Stream(std::istream& input);
	~Stream(void);
	
	
	Stream(const Stream&)            = delete;
	Stream(Stream&&)                 = delete;
	Stream& operator=(const Stream&) = delete;
	Stream& operator=(Stream&&)      = delete;
	
	inline bool setup_rx_buffer(void) const;
	
	char pop_next_char(void)  const;
	char peek_next_char(void) const;
	void eat_next_char(void)  const;
	
	bool skip_spaces(void) const;
	size_t get_line(void) const;
	
	operator bool() const;
	
protected:
	
private:
	std::istream &in_stream;
	std::unique_ptr<char[]> rx_buffer;
	mutable size_t
		n_prefetched_bytes_available,
		n_prefetched_bytes_used;
	mutable unsigned int line; // Line counter
};

} // Namespace Nodebuilder

} // Namespace LSCL

#endif // _H_LSCL_STREAM

