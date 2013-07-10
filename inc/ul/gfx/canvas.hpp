//=============================================================================
// Brief : GFX Canvas
//-----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_GFX_CANVAS__HPP_
#define UL_GFX_CANVAS__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/debug.hpp>
#include <ul/gfx/types.hpp>
#include <ul/gfx/color.hpp>
#include <ul/gfx/matrix.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
class font;
class glyphs;
class textline;
class text;

////////////////////////////////////////////////////////////////////////////////
class canvas {
	canvas(canvas const&) = delete;
	canvas& operator=(canvas const&) = delete;

public:
	enum line_join_type {
		k_line_join_miter,
		k_line_join_round,
		k_line_join_bevel,
	};

	enum line_cap_type {
		k_line_cap_butt,
		k_line_cap_round,
		k_line_cap_square,
	};

	enum draw_mode {
		k_draw_fill,
		k_draw_eo_fill,
		k_draw_stroke,
		k_draw_fill_stroke,
		k_draw_eo_fill_stroke,
	};

	enum text_draw_mode {
		k_text_draw_fill,
		k_text_draw_stroke,
		k_text_draw_fill_stroke,
	};

public:
	explicit canvas(context_ref context)
		: _ctx(context), _saves(0), _flags(0)
	{ }
	explicit canvas(void* context)
		: canvas(static_cast<context_ref>(context))
	{ }
	~canvas();

	/** context save/restore **/
	uint save();
	uint restore();


	/** clip/transform/position **/
	void clip(rect const& area);

	/** style **/
	void fill_color(color_argb const& c);
	void stroke_color(color_argb const& c);
	void shadow(point offset, float_type blur);
	void shadow(point offset, float_type blur, color_argb const& c);

	template<size_t N>
	void line_dash(float_type phase, float_type const (&lengths)[N]);
	void line_dash(float_type phase, float_type const lengths[], size_t count);
	void line_width(float_type width);
	void line_join(line_join_type e);
	void line_cap(line_cap_type e);
	void miter_limit(float_type limit);


	/** draw **/
	void fill_rect(rect const& area);
	void fill_ellipse(rect const& area);

	template<size_t N>
	void draw_line(point const (&path)[N], draw_mode dm);
	void draw_line(point const path[], size_t count, draw_mode dm);

	void draw_glyphs(glyphs const& g, point position, text_draw_mode dm = k_text_draw_fill);
	void draw_textline(textline const& l, point position, text_draw_mode dm = k_text_draw_fill);

private:
	uint save_();
	uint restore_();
	void restore_all_();
	void draw_line_(point const path[], size_t count, draw_mode dm);

private:
	context_ref _ctx;
	uint        _saves;
	uint        _flags;
};

inline canvas::~canvas()
{
	if (_saves)
		restore_all_();
}

inline uint canvas::save()
{
	UL_ASSERT(_saves + 1);
	return save_();
}

inline uint canvas::restore()
{
	UL_ASSERT(_saves);
	return restore_();
}

template<size_t N>
inline void canvas::line_dash(float_type phase, float_type const (&lengths)[N])
{
	line_dash(phase, lengths, N);
}

template<size_t N>
inline void canvas::draw_line(point const (&path)[N], draw_mode dm)
{
	static_assert(N > 1, "path must have at least 2 points");
	draw_line_(path, N, dm);
}

inline void canvas::draw_line(point const path[], size_t count, draw_mode dm)
{
	UL_ASSERT(count > 1);
	draw_line_(path, count, dm);
}

////////////////////////////////////////////////////////////////////////////////
} /* namespace ul */ } /* namespace gfx */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_GFX_CANVAS__HPP_ */
