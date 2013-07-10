//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_GFX_GLYPHS__HPP_
#define UL_GFX_GLYPHS__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/string_ref.hpp>
#include <ul/gfx/types.hpp>
#include <ul/gfx/font.hpp>
#include <memory>
#include <string>
#include <vector>
#include <tuple>

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
class glyphs {
public:
	static constexpr size_t npos = -1;

	enum direction {
		k_run_rigth_to_left,
		k_run_left_to_rigth,
	};

	struct run {
		size_t         begin;
		size_t         end;
		size_t         location;
		std::string    text;
		class font     font;
		enum direction direction;
	};

public:
	glyphs();
	glyphs(glyphs&&);
	glyphs& operator=(glyphs&&);
	~glyphs();

	glyphs(string_ref const& txt, font const& fnt);

	void insert(glyphs const& g, size_t pos);
	void insert(string_ref const& txt, font const& fnt, size_t pos);
	void replace(glyphs const& g, size_t pos, size_t epos);
	void replace(string_ref const& txt, font const& fnt, size_t pos, size_t epos);
	void erase(size_t pos, size_t epos);

	size_t        count() const               { return _glyphs.size(); }
	ushort const* data(size_t idx) const      { return &_glyphs[idx]; }
	size_t const* indices(size_t idx) const   { return &_indices[idx]; }
	point  const* positions(size_t idx) const { return &_positions[idx]; }
	point  const* advances(size_t idx) const  { return &_advances[idx]; }

	rect       bounds() const  { return rect { 0, descent(), width(), _ascent + _descent }; }
	point      size() const    { return point { width(), _ascent + _descent }; }
	float_type width() const   { return _width; }
	float_type ascent() const  { return _ascent; }
	float_type descent() const { return _descent; }
	float_type leading() const { return _leading; }

	std::vector<run> const& runs() const { return _runs; }

	glyphs subglyphs(size_t pos, size_t epos);

	explicit operator bool() const { return !_glyphs.empty(); }

private:
	std::vector<ushort> _glyphs;
	std::vector<size_t> _indices;
	std::vector<point>  _positions;
	std::vector<point>  _advances;
	std::vector<run>    _runs;
	float_type          _width;
	float_type          _ascent;
	float_type          _descent;
	float_type          _leading;
};

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_GFX_GLYPHS__HPP_ */
