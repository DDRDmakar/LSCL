#ifndef _H_LSCL_SCANNER
#define _H_LSCL_SCANNER

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "lscl_parser.tab.hh"
#include "location.hh"

namespace LSCL
{
namespace Nodebuilder
{
	class LSCL_Scanner : public yyFlexLexer
	{
	public:
	
		LSCL_Scanner(std::istream *in) : yyFlexLexer(in)
		{
		};
		virtual ~LSCL_Scanner()
		{
		};
		
		//get rid of override virtual function warning
		using FlexLexer::yylex;
		
		virtual int yylex(
			LSCL_Parser::semantic_type* const lval, 
			LSCL_Parser::location_type *location
		);
		// YY_DECL defined in lscl_lexer.l
		// Method body created by flex in lscl_lexer.yy.cc
		
	private:
		// yyval ptr
		LSCL_Parser::semantic_type *yylval = nullptr;
	};

} // Namespace Nodebuilder
} // Namespace LSCL

#endif // _H_LSCL_SCANNER
