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

#ifndef UL_XML__HPP_
#define UL_XML__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/home/support/iterators/line_pos_iterator.hpp>
#include <boost/spirit/home/phoenix/core.hpp>
#include <boost/spirit/home/phoenix/operator.hpp>
#include <boost/spirit/home/phoenix/fusion.hpp>
#include <boost/spirit/home/phoenix/stl.hpp>
#include <boost/spirit/home/phoenix/object.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/range/iterator_range.hpp>
#include <string>
#include <vector>
#include <ostream>

///////////////////////////////////////////////////////////////////////////////
namespace qi = boost::spirit::qi;
namespace ph = boost::phoenix;

///////////////////////////////////////////////////////////////////////////////
namespace ul { namespace xml {

///////////////////////////////////////////////////////////////////////////////
struct element;

typedef boost::variant<boost::recursive_wrapper<element>, std::string> node;

struct attribute {
	std::string name;
	std::string value;
};

struct element {
	std::string            name;
	std::vector<node>      nodes;
	std::vector<attribute> attributes;
};

struct doc {
	doc()
		: major(0), minor(0)
	{ }

	void clear()
	{
		root.name.clear();
		root.nodes.clear();
		root.attributes.clear();
		major = 0;
		minor = 0;
	}

	element root;
	uint    major;
	uint    minor;
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace xml */ } /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
BOOST_FUSION_ADAPT_STRUCT(
	ul::xml::doc,
	(ul::uint, major)
	(ul::uint, minor)
	(ul::xml::element, root)
);

BOOST_FUSION_ADAPT_STRUCT(
	ul::xml::attribute,
	(std::string, name)
	(std::string, value)
);

BOOST_FUSION_ADAPT_STRUCT(
	ul::xml::element,
	(std::string, name)
	(std::vector<ul::xml::attribute>, attributes)
	(std::vector<ul::xml::node>, nodes)
);

///////////////////////////////////////////////////////////////////////////////
namespace ul { namespace xml {

///////////////////////////////////////////////////////////////////////////////
template<class Iterator>
struct skipper_grammar : qi::grammar<Iterator> {
	skipper_grammar()
		: skipper_grammar::base_type(skip)
	{
		skip = qi::standard::space
		     | (qi::lit("<!--") >> +(qi::standard::char_ - "--!>") >> "--!>");
	}

	qi::rule<Iterator> skip;
};

template<class Iterator>
struct parser_grammar : qi::grammar<Iterator, doc(), skipper_grammar<Iterator> > {
	typedef skipper_grammar<Iterator> skipper;

	parser_grammar()
		: parser_grammar::base_type(xml)
	{
		xml = qi::lit("<?xml")
		    > qi::lexeme[   qi::lit("version=")
		                 > (qi::lit('\"') | '\'')
		                 > qi::uint_[ph::at_c<0>(qi::_val) = qi::_1]
		                 > '.'
		                 > qi::uint_[ph::at_c<1>(qi::_val) = qi::_1]
		                 > (qi::lit('\"') | '\'')
		                ]
		    > -qi::lexeme[  qi::lit("encoding=")
		                  > (qi::lit('\"') | '\'')
		                  > qi::standard::no_case[qi::lit("utf-8")]
			              > (qi::lit('\"') | '\'')
		                 ]
		    > "?>"
		    > element[ph::at_c<2>(qi::_val) = qi::_1]
		    ;

		element %= stag[qi::_a = qi::_1]
		         > *attribute
		         > (('>' > *node > etag(qi::_a)) | "/>")
		         ;

		name %= !(qi::standard::digit | '-' | '.')
             >> +(qi::standard::alnum | ':' | '_' | '-' | '.')
		      ;

		stag %= '<'
		     >> !('/' | qi::no_case["xml"])
		      > name
		      ;

		node %= element | text;

		etag = "</"
		     > qi::lit(qi::_r1)
		     > '>'
		     ;

		text %= +(qi::standard::char_ - '<')
		      ;

		attribute %= qi::lexeme[  name
		                        > '='
		                        > (qi::lit('\"') | '\'')
		                        > +(qi::standard::char_ - (qi::lit('\"') | '\''))
		                        > (qi::lit('\"') | '\'')
		                       ]
		           ;

		xml.name("xml");
		name.name("name");
		element.name("element");
		stag.name("stag");
		node.name("node");
		etag.name("etag");
		text.name("text");
		attribute.name("attribute");
	}

	template<class Handler>
	void on_error(Handler h)
	{
		qi::on_error<qi::fail>(xml, h);
	}

	qi::rule<Iterator, doc(),                              skipper> xml;
	qi::rule<Iterator, element(), qi::locals<std::string>, skipper> element;
	qi::rule<Iterator, std::string()                              > name;
	qi::rule<Iterator, std::string(),                      skipper> stag;
	qi::rule<Iterator, node(),                             skipper> node;
	qi::rule<Iterator, void(std::string),                  skipper> etag;
	qi::rule<Iterator, std::string()                              > text;
	qi::rule<Iterator, attribute(),                        skipper> attribute;
};

///////////////////////////////////////////////////////////////////////////////
typedef boost::spirit::line_pos_iterator<char const*> parse_iterator;
typedef boost::iterator_range<parse_iterator> parse_range;
typedef qi::expectation_failure<parse_iterator> parse_failure;

extern template struct skipper_grammar<parse_iterator>;
extern template struct parser_grammar<parse_iterator>;

template<class Iterator>
inline bool parse(Iterator& begin, Iterator end, doc& dc,
           parser_grammar<Iterator> const& ps = parser_grammar<Iterator>())
{
	skipper_grammar<Iterator> sk;

	return qi::phrase_parse(begin, end, ps, sk, dc);
}

bool parse(parse_iterator& begin, parse_iterator end, doc& dc,
           parser_grammar<parse_iterator> const& ps = parser_grammar<parse_iterator>());

///////////////////////////////////////////////////////////////////////////////
class generator {
public:
	static const uint tabsize = 4;

	explicit generator(std::ostream& out, uint indent = tabsize)
		: _out(out), _indent(indent)
	{ }

	void operator()(doc const& dc, uint level = 0) const
	{
		indent(level);
		_out << "<?xml version=\"" << dc.major << '.' << dc.minor << "\"?>";
		newline();
		put(dc.root, level);
	}

	void operator()(element const& em, uint level = 0) const
	{
		put(em, level);
	}

	void put(element const& em, uint level) const
	{
		if (em.name.empty())
			return;

		indent(level);
		_out << '<' << em.name;
		put(em.attributes);

		if (em.nodes.empty()) {
			_out << "/>";
			newline();
			return;
		}
		_out << '>';

		int pw = 1;
		for (std::vector<node>::const_iterator i = em.nodes.begin(), e = em.nodes.end(); i != e; ++i) {
			switch (i->which()) {
			case 0:
				if (pw == 1)
					newline();
				put(boost::get<element>(*i), level + 1);
				break;

			case 1:
				put(boost::get<std::string>(*i));
				break;

			default:
				continue;
			}
			pw = i->which();
		}
		if (pw == 0)
			indent(level);

		_out << "</" << em.name << '>';
		newline();
	}

	void put(std::vector<attribute> const& a) const
	{
		for (std::vector<attribute>::const_iterator i = a.begin(), e = a.end(); i != e; ++i)
			_out << ' ' << i->name << "=\"" << i->value << '\"';
	}

	void put(std::string const& str) const
	{
		_out << str;
	}

	void comment(std::string const& str, uint level = 0) const
	{
		indent(level);
		_out << "<-- " << str << " -->";
		newline();
	}

	void indent(uint level) const
	{
		const uint n = _indent * level;

		if (n)
			_out << std::string(n, ' ');
	}

	void newline() const
	{
		if (_indent)
			_out << std::endl;
	}

private:
	std::ostream& _out;
	uint          _indent;
};

///////////////////////////////////////////////////////////////////////////////
void generate(std::ostream& out, doc const& dc, uint level = 0);

///////////////////////////////////////////////////////////////////////////////
} /* namespace xml */ } /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
#endif /* UL_XML__HPP_ */
