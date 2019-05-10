
#ifndef _H_LSCL_STREAM
#define _H_LSCL_STREAM

#include <string>
#include <sstream>

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
	explicit Stream(std::istream& input);
	
	inline bool setup_rx_buffer(void) const;
	
	char        pop_next_char(void)  const;
	inline char peek_next_char(void) const;
	inline void eat_next_char(void)  const;
	
	bool skip_spaces(void) const;
	
protected:
	
private:
	std::istream &in_stream;
	char *rx_buffer;
	mutable size_t
		n_prefetched_bytes_available,
		n_prefetched_bytes_used;
};

} // Namespace Nodebuilder

} // Namespace LSCL

#endif // _H_LSCL_STREAM

