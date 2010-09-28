//=============================================================================
// Brief : Debug Helpers (platform dependent implementation - win32)
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2009-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#include <ul/debug.hpp>
#include <cstdlib>

///////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
void setup_crash_handler()
{
	//TODO: add unhandled exception suppport
}

void crash(const crash_ctx& ctx)
{
	std::fprintf(stderr,	 "\n"
							 "================================= Crash Dump ==================================\n"
							 "Code   : %s\n",
							 k_bug_code_string[ctx.code]);

	if (ctx.file) {
		std::fprintf(stderr, "Function  : %s\n"
							 "File      : %s\n"
							 "Line      : %u\n",
							 ctx.function,
							 ctx.file,
							 ctx.line);
	}

	if (ctx.expression) {
		std::fprintf(stderr, "Expression: %s\n",
							 ctx.expression);
	}

	std::fprintf(stderr,	 "===============================================================================\n"
							 "\n");
	std::abort();
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
