//=============================================================================
// Brief : XML Parser and Generator
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2010-2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#include <ul/xml.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul { namespace xml {

///////////////////////////////////////////////////////////////////////////////
template struct skipper_grammar<parse_iterator>;
template struct parser_grammar<parse_iterator>;

bool parse(parse_iterator& begin, parse_iterator end, doc& dc,
           parser_grammar<parse_iterator> const& ps)
{
	skipper_grammar<parse_iterator> sk;

	return qi::phrase_parse(begin, end, ps, sk, dc);
}

///////////////////////////////////////////////////////////////////////////////
void generate(std::ostream& out, doc const& dc, uint level)
{
	generator g(out);

	g(dc, level);
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace xml */ } /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
