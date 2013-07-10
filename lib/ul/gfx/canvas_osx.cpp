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

#include <ul/gfx/canvas.hpp>
#include <ul/gfx/font.hpp>
#include <ul/gfx/glyphs.hpp>
#include <ul/gfx/textline.hpp>
#include <ul/gfx/text.hpp>
#include <CoreGraphics/CGContext.h>
#include <CoreGraphics/CGColor.h>
#include <CoreGraphics/CGColorSpace.h>
#include <CoreText/CTFont.h>
#include <mutex>

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
static_assert((uint)kCGLineJoinMiter == canvas::k_line_join_miter, "invalid assumption");
static_assert((uint)kCGLineJoinRound == canvas::k_line_join_round, "invalid assumption");
static_assert((uint)kCGLineJoinBevel == canvas::k_line_join_bevel, "invalid assumption");

static_assert((uint)kCGLineCapButt == canvas::k_line_cap_butt, "invalid assumption");
static_assert((uint)kCGLineCapRound == canvas::k_line_cap_round, "invalid assumption");
static_assert((uint)kCGLineCapSquare == canvas::k_line_cap_square, "invalid assumption");

static_assert((uint)kCGPathFill == canvas::k_draw_fill, "invalid assumption");
static_assert((uint)kCGPathEOFill == canvas::k_draw_eo_fill, "invalid assumption");
static_assert((uint)kCGPathStroke == canvas::k_draw_stroke, "invalid assumption");
static_assert((uint)kCGPathFillStroke == canvas::k_draw_fill_stroke, "invalid assumption");
static_assert((uint)kCGPathEOFillStroke == canvas::k_draw_eo_fill_stroke, "invalid assumption");

static_assert((uint)kCGTextFill == canvas::k_text_draw_fill, "invalid assumption");
static_assert((uint)kCGTextStroke == canvas::k_text_draw_stroke, "invalid assumption");
static_assert((uint)kCGTextStroke == canvas::k_text_draw_stroke, "invalid assumption");
static_assert((uint)kCGTextFillStroke == canvas::k_text_draw_fill_stroke, "invalid assumption");

////////////////////////////////////////////////////////////////////////////////
enum canvas_flags {
	k_canvas_text_matrix = 1 << 0,
};

////////////////////////////////////////////////////////////////////////////////
struct cached_color_free {
	void operator()(CGColorSpaceRef cs) const;
};

typedef std::unique_ptr<struct CGColorSpace, cached_color_free> colorspace_ref;
typedef nsobj_ref<struct CGColor>                               color_ref;

static std::pair<color_argb, CGColorRef> s_color_cache[256];
static std::mutex                        s_color_cache_mutex;

static colorspace_ref k_rgb_colorspace { CGColorSpaceCreateWithName(kCGColorSpaceSRGB) };
static CGColorRef     k_cg_color_clear { CGColorGetConstantColor(kCGColorClear) };

void cached_color_free::operator()(CGColorSpaceRef cs) const
{
	for (auto& i : s_color_cache)
		if (i.second)
			CFRelease(i.second);
	CFRelease(cs);
}

static color_ref get_color(color_argb const& c)
{
	uint8 a = c.a * 255;
	uint8 r = c.r * 255;
	uint8 g = c.g * 255;
	uint8 b = c.b * 255;
	uint8 h = (a ^ 0x36) + (r ^ 0x45)  + (g ^ 0x54) + (b ^ 0x63);

	std::lock_guard<std::mutex> lock(s_color_cache_mutex);
	CGColorRef color = s_color_cache[h].second;

	if (c == k_color_clear) {
		color = k_cg_color_clear;

	} else if (s_color_cache[h].first != c) {
		if (s_color_cache[h].second)
			CFRelease(s_color_cache[h].second);
		float_type components[] = { c.r, c.g, c.b, c.a };
		color = CGColorCreate(k_rgb_colorspace.get(), components);
		s_color_cache[h].first = c;
		s_color_cache[h].second = color;
	}

	return color_ref(CGColorRetain(color));
}

////////////////////////////////////////////////////////////////////////////////
uint canvas::save_()
{
	CGContextSaveGState(_ctx);
	return ++_saves;
}

uint canvas::restore_()
{
	CGContextRestoreGState(_ctx);
	return --_saves;
}

void canvas::restore_all_()
{
	while (_saves--)
		CGContextRestoreGState(_ctx);
}

void canvas::clip(rect const& area)
{
	CGContextClipToRect(_ctx, area);
}

void canvas::fill_color(color_argb const& c)
{
	CGContextSetFillColorWithColor(_ctx, get_color(c).get());
}

void canvas::stroke_color(color_argb const& c)
{
	CGContextSetStrokeColorWithColor(_ctx, get_color(c).get());
}

void canvas::shadow(point offset, float_type blur)
{
	CGContextSetShadow(_ctx, offset, blur);
}

void canvas::shadow(point offset, float_type blur, color_argb const& c)
{
	CGContextSetShadowWithColor(_ctx, offset, blur, get_color(c).get());
}

void canvas::line_dash(float_type phase, float_type const lengths[], size_t count)
{
	CGContextSetLineDash(_ctx, phase, lengths, count);
}

void canvas::line_width(float_type width)
{
	CGContextSetLineWidth(_ctx, width);
}

void canvas::line_join(line_join_type e)
{
	CGContextSetLineJoin(_ctx, static_cast<CGLineJoin>(e));
}

void canvas::line_cap(line_cap_type e)
{
	CGContextSetLineCap(_ctx, static_cast<CGLineCap>(e));
}

void canvas::miter_limit(float_type limit)
{
	CGContextSetMiterLimit(_ctx, limit);
}

void canvas::fill_rect(rect const& area)
{
	CGContextFillRect(_ctx, area);
}

void canvas::fill_ellipse(rect const& area)
{
	CGContextFillEllipseInRect(_ctx, area);
}

void canvas::draw_line_(point const path[], size_t count, draw_mode dm)
{
	CGContextMoveToPoint(_ctx, path[0].x, path[0].y);
	for (size_t i = 1; i < count; ++i)
		CGContextAddLineToPoint(_ctx, path[i].x, path[i].y);
	CGContextDrawPath(_ctx, static_cast<CGPathDrawingMode>(dm));
}

void canvas::draw_glyphs(glyphs const& g, point position, text_draw_mode dm)
{
	if (UL_UNLIKELY(!(_flags & k_canvas_text_matrix))) {
		_flags |= k_canvas_text_matrix;
		CGContextSetTextMatrix(_ctx, affine_scale(1, -1));
	}

	CGContextSetTextPosition(_ctx, position.x, position.y);
	CGContextSetTextDrawingMode(_ctx, static_cast<CGTextDrawingMode>(dm));
	for (auto& i : g.runs()) {
		auto ps = reinterpret_cast<CGPoint const*>(g.positions(i.begin));
		CTFontDrawGlyphs(i.font.native(), g.data(i.begin), ps, i.end - i.begin, _ctx);
	}
}

void canvas::draw_textline(textline const& l, point position, text_draw_mode dm)
{
	auto irun = std::begin(l.runs());
	color_argb fill = k_color_clear;

	if (UL_UNLIKELY(!(_flags & k_canvas_text_matrix))) {
		_flags |= k_canvas_text_matrix;
		CGContextSetTextMatrix(_ctx, affine_scale(1, -1));
	}

	CGContextSetTextPosition(_ctx, position.x, position.y);
	CGContextSetTextDrawingMode(_ctx, static_cast<CGTextDrawingMode>(dm));
	for (auto& i : l.get_attributes()) {
		if (fill.a != 0 && fill != i.fill) {
			fill = i.fill;
			fill_color(fill);
		}

		auto j = i.begin;
		while (j != i.end) {
			if (j >= irun->end)
				++irun;

			auto je = std::min(i.end, irun->end);
			auto ps = reinterpret_cast<CGPoint const*>(l.positions(j));
			CTFontDrawGlyphs(irun->font.native(), l.data(j), ps, je - j, _ctx);
			j = je;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
