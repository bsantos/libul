//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#include <ul/gfx/text.hpp>
#include <ul/nsobj.hpp>
#include <ul/debug.hpp>
#include <ul/unicode.hpp>
#include <CoreFoundation/CFDictionary.h>
#include <CoreFoundation/CFArray.h>
#include <CoreFoundation/CFAttributedString.h>
#include <CoreText/CTStringAttributes.h>
#include <CoreText/CTFont.h>
#include <CoreText/CTLine.h>
#include <CoreText/CTRun.h>
#include <numeric>

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
typedef nsobj_ref<__CFDictionary const>       dictionary_ref;
typedef nsobj_ref<__CFAttributedString const> attr_string_ref;
typedef nsobj_ref<__CFString const>           cf_string_ref;
typedef nsobj_ref<__CTLine const>             line_ref;

////////////////////////////////////////////////////////////////////////////////
static line_ref create_line(std::vector<ushort> const& txt, font const& fnt)
{
	auto ctfnt = fnt.native();

	dictionary_ref attr(CFDictionaryCreate(kCFAllocatorDefault,
                                           (const void**) &kCTFontAttributeName,
                                           (const void**) &ctfnt,
                                           1,
                                           &kCFTypeDictionaryKeyCallBacks,
                                           &kCFTypeDictionaryValueCallBacks));

	cf_string_ref str(CFStringCreateWithCharactersNoCopy(kCFAllocatorDefault,
	                                                     txt.data(), txt.size(),
	                                                     kCFAllocatorNull));

	attr_string_ref astr(CFAttributedStringCreate(kCFAllocatorDefault, str.get(), attr.get()));

	return line_ref(CTLineCreateWithAttributedString(astr.get()));
}

static std::pair<std::vector<ushort>, std::vector<size_t>> u8_to_u16(string_ref const& cstr)
{
	std::pair<std::vector<ushort>, std::vector<size_t>> res;
	std::vector<ushort>& ustr = res.first;
	std::vector<size_t>& uidx = res.second;
	char const* str = cstr.begin();

	ustr.reserve(cstr.length());
	uidx.reserve(cstr.length());

	while (str != cstr.end()) {
		size_t const idx = str - cstr.begin();
		uint cp = uc8_to_uc32(str, cstr.end());

		uidx.push_back(idx);
		if (cp < 0xffff) {
			ustr.push_back(cp);
		} else if (cp <= k_uc_max_utf16) {
			uidx.push_back(idx);
			ustr.push_back(uc16_high_surrogate(cp));
			ustr.push_back(uc16_low_surrogate(cp));
		} else {
			ustr.push_back(k_uc_replacement_char);
		}
	}
	return res;
}

static void make_glyphs(string_ref const& txt, font const& fnt,
                        std::vector<ushort>& glypha, std::vector<size_t>& indices,
                        std::vector<point>& advances, std::vector<point>& positions,
                        float_type& width, float_type& ascent, float_type& descent,
                        float_type& leading, std::vector<glyphs::run>& vrun)
{
	auto ustr = u8_to_u16(txt);
	auto line = create_line(ustr.first, fnt);
	auto const count = CTLineGetGlyphCount(line.get());

	width = CTLineGetTypographicBounds(line.get(), &ascent, &descent, &leading);
	glypha.resize(count);
	indices.resize(count);
	advances.resize(count);
	positions.resize(count);

	auto runs = CTLineGetGlyphRuns(line.get());
	size_t const rc = CFArrayGetCount(runs);

	size_t pos = 0;
	for (size_t i = 0; i < rc; ++i) {
		auto run = (CTRunRef) CFArrayGetValueAtIndex(runs, i);
		auto const rcnt = CTRunGetGlyphCount(run);

		CTRunGetGlyphs(run, CFRange{0, 0}, &glypha[pos]);
		CTRunGetStringIndices(run, CFRange{0, 0}, reinterpret_cast<CFIndex*>(&indices[pos]));
		CTRunGetPositions(run, CFRange{0, 0}, reinterpret_cast<CGPoint*>(&positions[pos]));
		CTRunGetAdvances(run, CFRange{0, 0}, reinterpret_cast<CGSize*>(&advances[pos]));

		auto sr = CTRunGetStringRange(run);
		auto at = CTRunGetAttributes(run);
		auto rf = (CTFontRef) CFDictionaryGetValue(at, kCTFontAttributeName);
		auto rd = CTRunGetStatus(run) & kCTRunStatusRightToLeft ? glyphs::k_run_rigth_to_left
		                                                        : glyphs::k_run_left_to_rigth;

		auto const se = sr.location + sr.length;
		auto const tl = ustr.second[sr.location];
		auto tb = txt.begin() + tl;
		auto te = (se < count) ? txt.begin() + ustr.second[se] : txt.end();

		vrun.push_back(glyphs::run { pos, pos + rcnt, tl, std::string(tb, te), font(rf), rd });

		pos += rcnt;
	}

	for (size_t i = 0; i < count; ++i)
		indices[i] = ustr.second[indices[i]];
}

////////////////////////////////////////////////////////////////////////////////
glyphs::glyphs() = default;
glyphs::glyphs(glyphs&&) = default;
glyphs& glyphs::operator=(glyphs&&) = default;
glyphs::~glyphs() = default;

glyphs::glyphs(string_ref const& txt, font const& fnt)
	: _width{}, _ascent{}, _descent{}, _leading{}
{
	make_glyphs(txt, fnt, _glyphs, _indices, _advances, _positions,
	            _width, _ascent, _descent, _leading, _runs);
}

//void glyphs::insert(glyphs const& g, size_t pos);
//void glyphs::insert(string_ref const& txt, font const& fnt, size_t pos);
//void glyphs::replace(glyphs const& g, size_t pos, size_t epos);
//void glyphs::replace(string_ref const& txt, font const& fnt, size_t pos, size_t epos);
//void glyphs::erase(size_t pos, size_t epos);
//glyphs glyphs::subglyphs(size_t pos, size_t epos);

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
