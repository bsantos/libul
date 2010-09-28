//=============================================================================
// Brief : Debug Helpers
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2008-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#include <ul/debug.hpp>
#include <boost/assert.hpp>
#include <cstring>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
char const* k_bug_code_string[] = {
	"bug",
	"assert",
	"assert pointer",
	"not implemented",
	"never here",
	"floting point exception",
	"invalid opcode",
	"terminal interrupt",
	"page fault",
	"breakpoint"
};

///////////////////////////////////////////////////////////////////////////////
#if defined(__GNUC__)
	static __thread ul::checkpoint* check_point_list;
#	define UL_CHECKPOINT_SET(x) check_point_list = x
#elif defined(BOOST_MSVC)
	static __declspec(thread) ul::checkpoint* check_point_list;
#	define UL_CHECKPOINT_SET(x) check_point_list = x
#else
	static nullptr_t check_point_list = nullptr_t();
#	define UL_CHECKPOINT_SET(x)
#endif

checkpoint* checkpoint::top()
{
	return check_point_list;
}

checkpoint::checkpoint(const char* file, uint line, const char* exp)
	: _prev(check_point_list), _file(file), _line(line), _exp(exp)
{
	UL_CHECKPOINT_SET(this);
}

checkpoint::~checkpoint()
{
	UL_CHECKPOINT_SET(_prev);
}

void crash(bug code, const char* function, const char* file, uint line, const char* expression)
{
	crash_ctx ctx;

	std::memset(&ctx, 0, sizeof(ctx));
	ctx.code = code;
	ctx.address = UL_RETURN_ADDRESS;
	ctx.function = function;
	ctx.file = file;
	ctx.line = line;
	ctx.expression = expression;
	crash(ctx);
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
void boost::assertion_failed(char const* expr, char const* function, char const* file, long line)
{
	ul::crash_ctx ctx;

	std::memset(&ctx, 0, sizeof(ctx));
	ctx.code = ul::bug_assert;
	ctx.address = UL_RETURN_ADDRESS;
	ctx.function = function;
	ctx.file = file;
	ctx.line = line;
	ctx.expression = expr;
	crash(ctx);
}

// EOF ////////////////////////////////////////////////////////////////////////
