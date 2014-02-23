//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_GFX_TEXTLINE__HPP_
#define UL_GFX_TEXTLINE__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/string_ref.hpp>
#include <ul/gfx/types.hpp>
#include <ul/gfx/font.hpp>
#include <ul/gfx/color.hpp>
#include <ul/gfx/glyphs.hpp>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
class textline : public glyphs {
public:
	static constexpr size_t npos = -1;

	enum alignment {
		k_left,
		k_center,
		k_right
	};

	enum decoration {
		k_overtextline     = 1 << 0,
		k_textline_through = 1 << 1,
		k_undertextline    = 1 << 2,
	};

	struct attribute {
		size_t     begin;
		size_t     end;
		class font font;
		color_argb fill;
//		color_argb background;
//		uint       decoration;
	};

public:
	textline();
	textline(textline&&);
	textline& operator=(textline&&);
	~textline();

	textline(string_ref const& txt, font const& fnt);
	textline(string_ref const& txt, attribute attr);

	void insert(textline const& ln, size_t pos);
	void replace(textline const& ln, size_t pos, size_t epos);
	void erase(size_t pos, size_t epos);

	void set_font(font const& fnt);
	void set_font(font const& fnt, size_t pos, size_t epos);
	void set_fill(color_argb const& c);
	void set_fill(color_argb const& c, size_t pos, size_t epos);
//	void set_background(color_argb const& c);
//	void set_background(color_argb const& c, size_t pos, size_t epos);
//	void set_decoration(uint decoration);
//	void set_decoration(uint decoration, size_t pos, size_t epos);

	attribute const& get_attribute(size_t pos) const;

	std::vector<attribute> const& get_attributes() const { return _attributes; }

	void           heigth(float_type h)        { _heigth = h; }
	float_type     heigth() const              { return _heigth; }
	void           alignment(enum alignment a) { _alignment = a; }
	enum alignment alignment() const           { return _alignment; }

	textline subtextline(size_t pos, size_t epos);

private:
	glyphs                 _glyphs;
	std::vector<attribute> _attributes;
	float_type             _heigth;
	enum alignment         _alignment;
};

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_GFX_TEXTLINE__HPP_ */
